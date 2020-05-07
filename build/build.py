#!/usr/bin/python3

import subprocess as sp
import os
import argparse
import json
import glob
import re

origin_dir = os.getcwd()
file_dir = os.path.dirname(os.path.realpath(__file__))

build_mode = ""
verbose_mode = False

def SubstitutePathVars(data, path_vars):
    if isinstance(data, dict):
        for key, value in data.items():
            data[key] = SubstitutePathVars(value, path_vars)
    elif isinstance(data, list):
        for i in range(len(data)):
            data[i] = SubstitutePathVars(data[i], path_vars)
    elif isinstance(data, str):
        reVars = re.findall(r"\$\{.*\}", data)
        data = data.replace('~', os.getenv("HOME"))
        for v in reVars:
            data = data.replace(v, path_vars[v[2:-1]]).replace("//", '/')
    return data


def UpdateCache():
    global file_dir, verbose_mode
    os.chdir(file_dir)
    with open("build_settings.json") as f:
        settings = json.load(f)
    for key, value in settings["path_vars"].items():
        settings["path_vars"][key] = value.replace('~', os.getenv("HOME"))
    settings = SubstitutePathVars(settings, settings["path_vars"].copy())

    # cache init
    cache = {}
    cache["path_vars"] = settings["path_vars"]
    cache["debug"] = settings["debug"]
    cache["release"] = settings["release"]
    cache["source_files"] = []
    cache["header_files"] = []
    cache["include_dirs"] = []
    cache["binary_files"] = []
    cache["debug"]["compile_list"] = []
    cache["release"]["compile_list"] = []
    cache["tree"] = {}

    for pathname in settings["source_files"]:
        for filename in glob.glob(pathname, recursive=True):
            cache["source_files"].append(filename)

    for pathname in settings["header_files"]:
        for filename in glob.glob(pathname, recursive=True):
            cache["header_files"].append(filename)
    
    for pathname in settings["include_dirs"]:
        for dirname in glob.glob(pathname, recursive=True):
            cache["include_dirs"].append(dirname)
    
    for pathname in settings["binary_files"]:
        for filename in glob.glob(pathname, recursive=True):
            cache["binary_files"].append(filename)
    
    if os.path.exists("cache.json"):
        with open("cache.json") as f:
            pre_cache = json.load(f)
            cache["tree"] = pre_cache["tree"]
            cache["debug"]["compile_list"] = pre_cache["debug"]["compile_list"]
            cache["release"]["compile_list"] = pre_cache["release"]["compile_list"]

    if verbose_mode:
        print(cache)
    with open("cache.json", 'w') as f:
        json.dump(cache, f)
        
def BuildDepTree():
    global build_mode, verbose_mode, file_dir
    os.chdir(file_dir)
    with open("cache.json") as f:
        cache = json.load(f)
    scripts = set(cache["source_files"])
    scripts.update(cache["header_files"])

    # tree init
    cache["tree"] = {}
    for filename in scripts:
        cache["tree"].update({ filename : { "parents" : [], "children" : [] } })

    # fill up tree
    for filename in scripts:
        with open(filename) as f:
            if verbose_mode:
                print("In file: " + str(filename))
            for line in f:
                m = re.match(r"^#include\s?\".*\"", line)
                if m:
                    dep = re.findall(r"\".*\"", line)[0][1:-1]
                    parent_filename = os.path.join(os.path.dirname(filename), dep)
                    if verbose_mode:
                        print("Found: " + str(parent_filename))
                    if os.path.exists(parent_filename):
                        parent_filename = os.path.realpath(parent_filename)
                        if parent_filename in scripts:
                            cache["tree"][filename]["parents"].append(parent_filename)
                            cache["tree"][parent_filename]["children"].append(filename)
                    else:
                       for i_dir in cache["include_dirs"]:
                            parent_filename = os.path.join(i_dir, dep)
                            if os.path.exists(parent_filename):
                                parent_filename = os.path.realpath(parent_filename)
                                if parent_filename in scripts:
                                    cache["tree"][filename]["parents"].append(parent_filename)
                                    cache["tree"][parent_filename]["children"].append(filename)
                                break

    if verbose_mode:
        print(cache)
    with open("cache.json", 'w') as f:
        json.dump(cache, f)

def UpdateDepTreeNodes(files):
    global build_mode, verbose_mode, file_dir, origin_dir
    os.chdir(file_dir)
    with open("cache.json") as f:
        cache = json.load(f)
    scripts = set(cache["source_files"])
    scripts.update(cache["header_files"])

    for filename in files:
        filename = os.path.realpath(os.path.join(origin_dir, filename))
        if not os.path.exists(filename):
            print("There is no file: " + filename)
            continue
        
        # get new parent list
        new_parents = set()
        with open(filename) as f:
            if verbose_mode:
                print("In file: " + str(filename))
            for line in f:
                m = re.match(r"^#include\s?\".*\"", line)
                if m:
                    dep = re.findall(r"\".*\"", line)[0][1:-1]
                    parent_filename = os.path.join(os.path.dirname(filename), dep)
                    if verbose_mode:
                        print("Found: " + str(parent_filename))
                    if os.path.exists(parent_filename):
                        parent_filename = os.path.realpath(parent_filename)
                        if parent_filename in scripts:
                            new_parents.add(parent_filename)
                    else:
                       for i_dir in cache["include_dirs"]:
                            parent_filename = os.path.join(i_dir, dep)
                            if os.path.exists(parent_filename):
                                parent_filename = os.path.realpath(parent_filename)
                                if parent_filename in scripts:
                                    new_parents.add(parent_filename)
                                break
        # update tree            
        for parent_filename in set(cache["tree"][filename]["parents"]).difference(new_parents):
            cache["tree"][filename]["parents"].remove(parent_filename)
            cache["tree"][parent_filename]["children"].remove(filename)
        for parent_filename in new_parents.difference(cache["tree"][filename]["parents"]):
            cache["tree"][filename]["parents"].append(parent_filename)
            cache["tree"][parent_filename]["children"].append(filename)
        
        if verbose_mode:
            print(cache)
        with open("cache.json", 'w') as f:
            json.dump(cache, f)


def Compile(files): 
    global build_mode, verbose_mode, file_dir, origin_dir
    os.chdir(file_dir)
    with open("cache.json") as f:
        cache = json.load(f)
    sources = set(cache["source_files"])
    headers = set(cache["header_files"])

    files2Compile = set()
    for filename in files:
        filename = os.path.realpath(os.path.join(origin_dir, filename))
        if not os.path.exists(filename):
            print("There is no file: " + filename)
            continue

        stack = [filename]
        while stack:
            cFile = stack.pop()
            if cFile in sources:
                files2Compile.add(cFile)
            elif not cFile in headers:
                print("Unknown file: " + str(cFile))
                continue
            stack.extend(cache["tree"][cFile]["children"])

    args = []
    args.extend(cache[build_mode]["compile_command"].split())
    for dirname in cache["include_dirs"]:
        args.extend(["-I", dirname])
    args.extend(files2Compile)
    args.extend(cache[build_mode]["compile_options"])

    obj_path = os.path.join(cache[build_mode]["build_dir"], "obj")
    if not os.path.exists(obj_path):
        os.makedirs(obj_path)
    os.chdir(obj_path)

    if verbose_mode:
        print(args)
    sp.run(args)
    


def CompileAll(): 
    global build_mode, verbose_mode, file_dir
    os.chdir(file_dir)
    with open("cache.json") as f:
        cache = json.load(f)
    args = []
    args.extend(cache[build_mode]["compile_command"].split())
    for dirname in cache["include_dirs"]:
        args.extend(["-I", dirname])
    args.extend(cache["source_files"])
    args.extend(cache[build_mode]["compile_options"])

    obj_path = os.path.join(cache[build_mode]["build_dir"], "obj")
    if not os.path.exists(obj_path):
        os.makedirs(obj_path)
    os.chdir(obj_path)

    if verbose_mode:
        print(args)
    sp.run(args)

def Link():
    global build_mode, verbose_mode, file_dir
    os.chdir(file_dir)
    with open("cache.json") as f:
        cache = json.load(f)
    args = []
    args.extend(cache[build_mode]["linking_command"].split())

    obj_path = os.path.join(cache[build_mode]["build_dir"], "obj/*.o")
    args.extend(glob.glob(obj_path, recursive=True))

    args.extend(cache["binary_files"])

    out_path = os.path.join(cache[build_mode]["build_dir"], "out")
    if not os.path.exists(out_path):
        os.makedirs(out_path)
    args.extend(["-o", os.path.join(out_path, cache[build_mode]["executable_name"])])
    args.extend(cache[build_mode]["linking_options"])
    
    if verbose_mode:
        print(args)
    sp.run(args)

def ShowList():
    global build_mode, file_dir
    os.chdir(file_dir)
    with open("cache.json") as f:
        cache = json.load(f)
    for filename in cache[build_mode]["compile_list"]:
        print(filename)

def AddToCompileList(files):
    global build_mode, verbose_mode, file_dir, origin_dir
    os.chdir(file_dir)
    with open("cache.json") as f:
        cache = json.load(f)
    compileList = set(cache[build_mode]["compile_list"])
    for filename in files:
        filename = os.path.realpath(os.path.join(origin_dir, filename))
        if not os.path.exists(filename):
            print("There is no file: " + filename)
            continue
        
        compileList.add(filename)
    cache[build_mode]["compile_list"] = list(compileList)

    if verbose_mode:
        print(cache)
    with open("cache.json", 'w') as f:
        json.dump(cache, f)

def RemoveFromCompileList(files):
    global build_mode, verbose_mode, file_dir, origin_dir
    os.chdir(file_dir)
    with open("cache.json") as f:
        cache = json.load(f)
    compileList = set(cache[build_mode]["compile_list"])
    for filename in files:
        filename = os.path.realpath(os.path.join(origin_dir, filename))
        if not os.path.exists(filename):
            print("There is no file: " + filename)
            continue
        
        compileList.remove(filename)
    cache[build_mode]["compile_list"] = list(compileList)

    if verbose_mode:
        print(cache)
    with open("cache.json", 'w') as f:
        json.dump(cache, f)

def ClearList():
    global build_mode, verbose_mode, file_dir
    os.chdir(file_dir)
    with open("cache.json") as f:
        cache = json.load(f)
    
    cache[build_mode]["compile_list"] = []
    if verbose_mode:
        print(cache)
    with open("cache.json", 'w') as f:
        json.dump(cache, f)

def CompileFilesFromList():
    global build_mode, verbose_mode, file_dir
    os.chdir(file_dir)
    with open("cache.json") as f:
        cache = json.load(f)
    
    if not cache[build_mode]["compile_list"]:
        if verbose_mode:
            print("There are no files to compile")
    Compile(cache[build_mode]["compile_list"])


def main():
    global build_mode, verbose_mode, file_dir

    ap = argparse.ArgumentParser()
    ap.add_argument("-v", "--verbose", action="store_true", help="Provide extra information")
    ap.add_argument("-d", "--debug", action="store_true", help="Debug flag, if set then compile, link, build commands will be relative to debug folder, otherwise to release folder")

    mode = ap.add_mutually_exclusive_group()
    mode.add_argument("-u", "--update-cache", action="store_true", help="Update cache file, always use it after making changes to build_settings.json file")
    mode.add_argument("-c", "--compile", nargs='+', help="Compile source files by a source or header file name")
    mode.add_argument("-C", "--compile-all", action="store_true", help="Compile all source files")
    mode.add_argument("-l", "--link", action="store_true", help="Link all .o files")
    mode.add_argument("-b", "--build", nargs='+', help="Compile and link source files by a source or header file name")
    mode.add_argument("-B", "--build-all", action="store_true", help="Compile and link all source files")
    mode.add_argument("-t", "--update-tree-node", nargs='+', help="Update a node of a dependency tree for given header and source files (Use only after --build-tree command)")
    mode.add_argument("-T", "--build-tree", action="store_true", help="Build dependency tree for all header and source files")
    mode.add_argument("--show-list", action="store_true", help="Show the compile list")
    mode.add_argument("--add-to-list", nargs='+', help="Add files to the compile list (debug or release list)")
    mode.add_argument("--remove-from-list", nargs='+', help="Remove files from the compile list (debug or release list)")
    mode.add_argument("--clear-list", action="store_true", help="Clear the compile list")
    mode.add_argument("--compile-list", action="store_true", help="Compile files from the compile list (debug or release list)")
    mode.add_argument("--build-list", action="store_true", help="Compile and link files from the compile list (debug or release list)")
    args = ap.parse_args()
    
    verbose_mode = args.verbose
    build_mode = "debug" if args.debug else "release"

    # UpdateDepTreeNodes(["TestObject.cpp"])

    if args.add_to_list:
        AddToCompileList(args.add_to_list)
    elif args.remove_from_list:
        RemoveFromCompileList(args.remove_from_list)
    elif args.show_list:
        ShowList()
    elif args.compile_list:
        CompileFilesFromList()
    elif args.build_list:
        CompileFilesFromList()
        Link()
    elif args.clear_list:
        ClearList()
    elif args.update_cache:
        UpdateCache()
    elif args.compile:
        Compile(args.compile)
    elif args.compile_all:
        CompileAll()
    elif args.link:
        Link()
    elif args.build:
        Compile(args.build)
        Link()
    elif args.build_all:
        CompileAll()
        Link()
    elif args.update_tree_node:
        UpdateDepTreeNodes(args.update_tree_node)
    elif args.build_tree:
        BuildDepTree()
    

    

if __name__ == "__main__":
    main()

# -b - build project with list of changed files, dep. tree ( -C -l )
# -B - build project without dep. tree (compiles all .cpp files)
# -d - debug flag
# -c - compile file (for .cpp just compile it to .o, for .hpp compile all .cpp files which depend on this)
# -C - compile all .cpp files according to lis of changed files
# -l - link all .o files
# -t - update dep. tree for current file
# -T - update dep. tree for all files
