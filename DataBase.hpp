#pragma once

#include "MemoryPool.hpp"

#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>

namespace mbgl {
    struct PersonalIBM {
        std::string name;
        std::string firm;
        std::string processor_type;
        std::string cpu_frequency;
        std::string ram_capacity;
        std::string hard_drive_capacity;

        PersonalIBM(std::string name = "", std::string firm = "", std::string processor_type = "",
        std::string cpu_frequency = "", std::string ram_capacity = "", std::string hard_drive_capacity = "") 
        : name(name), firm(firm), processor_type(processor_type), cpu_frequency(cpu_frequency), 
        ram_capacity(ram_capacity), hard_drive_capacity(hard_drive_capacity) {}
     };

    const unsigned int c_chunkCapacity = 134217728 / sizeof(PersonalIBM); // 128 Mb / sizeof(Node)

    class DataBase {
    private:
        MemoryPool<PersonalIBM> _mp;
        std::unordered_map<std::string, PersonalIBM*> _nodes;
    
    public:
        DataBase()
        : _mp(c_chunkCapacity) {}

        void Add(const std::string name, const PersonalIBM& node) {
            _nodes.insert(std::make_pair(name, _mp.Push(node)));
        }
        
        template <typename... Args>
        void Construct(const std::string name, Args... args) {
            _nodes.insert(std::make_pair(name, _mp.Emplace(args...)));
        }

        void Remove(const std::string name) {
            if (_nodes.find(name) == _nodes.end())
                return;
            _mp.Remove(_nodes[name]);
            _nodes.erase(name);
        }

        PersonalIBM* Get(const std::string name) {
            if (_nodes.find(name) == _nodes.end())
                return nullptr;
            return _nodes[name];
        }

        std::vector<PersonalIBM*> GetNodes() {
            std::vector<PersonalIBM*> outV(_nodes.size());
            auto it2 = outV.begin();
            for (auto it1 = _nodes.begin(); it1 != _nodes.end(); it1++, it2++) {
                *it2 = it1->second;
            }
            return outV;
        }

        std::vector<PersonalIBM*> GetSorted(const std::function<bool(PersonalIBM*, PersonalIBM*)> comparator) {
            std::vector<PersonalIBM*> outV(_nodes.size());
            auto it2 = outV.begin();
            for (auto it1 = _nodes.begin(); it1 != _nodes.end(); it1++, it2++) {
                *it2 = it1->second;
            }
            sort(outV.begin(), outV.end(), comparator);
            return outV;
        }

        std::vector<PersonalIBM*> GetSubset(const std::function<bool(PersonalIBM*)> condition) {
            std::vector<PersonalIBM*> outV;
            for (auto& node : _nodes) {
                if (condition(node.second)) 
                    outV.push_back(node.second);
            }
            return outV;
        }

        void Clear() {
            for (auto& node : _nodes) {
                Remove(node.first);
            }
            _nodes.clear();
        }
    };
}