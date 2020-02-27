#include "DataBase.hpp"

#include <iostream>
#include <fstream>

using namespace mbgl;
using namespace std;

void Help() {
    cout << "'h' - help, 'q' - exit, 'd' - display, 'n' - new, 'r' - remove, 'g' - get, 'S' - sort, 'f' - find, 'W' - save, 'L' - load" << endl;
}

std::string GetInput(std::string ask_message = "") {
    cout << ask_message;
    std::string s;
    getline(cin, s);
    return s;
}
std::string GetInput(std::string ask_message, std::string& outS) {
    cout << ask_message;
    std::string s;
    getline(cin, s);
    if (s != "")
        outS = s;
    return s;
}

void NewNode(DataBase& db) {
    std::string name, firm, processor_type, cpu_frequency, ram_capacity, hard_drive_capacity;
    name = GetInput("Name: ");
    firm = GetInput("Firm: ");
    processor_type = GetInput("Processor type: ");
    cpu_frequency = GetInput("CPU frequency: ");
    ram_capacity = GetInput("RAM capacity: ");
    hard_drive_capacity = GetInput("Hard Drive capacity: ");    
    db.Construct(name, name, firm, processor_type, cpu_frequency, ram_capacity, hard_drive_capacity);
}

void RemoveNode(DataBase& db) {
    std::string name;
    cin >> name;
    db.Remove(name);
}

void PrintNodes(const std::vector<PersonalIBM*> nodes) {
    for (auto& node : nodes) {
        cout << "---------------------------" << endl;
        cout << "Name: " << node->name << endl;
        cout << "Firm: " << node->firm << endl;
        cout << "Processor type: " << node->processor_type << endl;
        cout << "CPU frequency: " << node->cpu_frequency << endl;
        cout << "RAM capacity: " << node->ram_capacity << endl;
        cout << "Hard Drive capacity: " << node->hard_drive_capacity << endl;
        cout << "---------------------------" << endl;
        cout << endl;
    }
}

bool isLGrater(string self, string other) {
    int len = min(self.size(), other.size());
    for (int i = 0; i < len; i++) {
        if (self[i] == other[i])
            continue;
        return self[i] > other[i];
    }
    return self.size() > other.size();
}

void SortByName(DataBase& db) {
    PrintNodes(db.GetSorted([](PersonalIBM* f, PersonalIBM* s) { return  !isLGrater(f->name, s->name); }));
}

void Get(DataBase& db) {
    std::string name;
    cin >> name;
    PersonalIBM* node = db.Get(name);
    cout << "---------------------------" << endl;
    cout << "Name: " << node->name << endl;
    cout << "Firm: " << node->firm << endl;
    cout << "Processor type: " << node->processor_type << endl;
    cout << "CPU frequency: " << node->cpu_frequency << endl;
    cout << "RAM capacity: " << node->ram_capacity << endl;
    cout << "Hard Drive capacity: " << node->hard_drive_capacity << endl;
    cout << "---------------------------" << endl;
    cout << endl;
}

void Set(DataBase& db) {
    std::string name;
    cin >> name;
    PersonalIBM* node = db.Get(name);
    if (node == nullptr)
        return;
    GetInput("Name: ", node->name);
    GetInput("Firm: ", node->firm);
    GetInput("Processor type: ", node->processor_type);
    GetInput("CPU frequency: ", node->cpu_frequency);
    GetInput("RAM capacity: ", node->ram_capacity);
    GetInput("Hard Drive capacity: ", node->hard_drive_capacity);
}

void Find(DataBase& db) {
    PersonalIBM inputNode;
    GetInput("Name: ", inputNode.name);
    GetInput("Firm: ", inputNode.firm);
    GetInput("Processor type: ", inputNode.processor_type);
    GetInput("CPU frequency: ", inputNode.cpu_frequency);
    GetInput("RAM capacity: ", inputNode.ram_capacity);
    GetInput("Hard Drive capacity: ", inputNode.hard_drive_capacity);

    std::vector<PersonalIBM*> nodes = db.GetSubset([&](PersonalIBM* node) {
        if (inputNode.name != "")
            if (inputNode.name != node->name)
                return false;
        if (inputNode.firm != "")
            if (inputNode.firm != node->firm)
                return false;
        if (inputNode.processor_type != "")
            if (inputNode.processor_type != node->processor_type)
                return false;
        if (inputNode.cpu_frequency != "")
            if (inputNode.cpu_frequency != node->cpu_frequency)
                return false;
        if (inputNode.ram_capacity != "")
            if (inputNode.ram_capacity != node->ram_capacity)
                return false;
        if (inputNode.hard_drive_capacity != "")
            if (inputNode.hard_drive_capacity != node->hard_drive_capacity)
                return false;
        
        return true;
    });
    cout << "Found: " << nodes.size() << endl;
    PrintNodes(nodes);
}

void Save(DataBase& db) {
    std::string fileName = GetInput("File name: ");
    ofstream o_file(fileName);
    std::vector<PersonalIBM*> nodes = db.GetNodes();
    for (auto& node : nodes) {
        o_file << "b:" << endl;
        o_file << node->name << endl;
        o_file << node->firm << endl;
        o_file << node->processor_type << endl;
        o_file << node->cpu_frequency << endl;
        o_file << node->ram_capacity << endl;
        o_file << node->hard_drive_capacity << endl;        
    }
    o_file.close();
}

void Load(DataBase& db) {
    db.Clear();
    std::string fileName = GetInput("File name: ");
    ifstream i_file(fileName);
    std::string line;
    while (getline(i_file, line)) {
        if (line == "b:") {
            std::string name, firm, processor_type, cpu_frequency, ram_capacity, hard_drive_capacity;
            getline(i_file, line); name = line;
            getline(i_file, line); firm = line;
            getline(i_file, line); processor_type = line;
            getline(i_file, line); cpu_frequency = line;
            getline(i_file, line); ram_capacity = line;
            getline(i_file, line); hard_drive_capacity = line;
            db.Construct(name, name, firm, processor_type, cpu_frequency, ram_capacity, hard_drive_capacity);
        }
    }
    i_file.close();
}   

int main() {
    DataBase dataBase;

    Help();
    bool isRunning = true;
    std::string inputS;
    char c;
    while (isRunning) {
        getline(cin, inputS);
        cout << endl;
        if (inputS.size() > 1) {
            cout << "Incorrect command" << endl;
            continue;
        }
        c = inputS[0];
        switch (c) {
        case 'q':
            isRunning = false;
            break;
        case 'h':
            Help();
            break;
        case 'n':
            NewNode(dataBase);
            break;
        case 'r':
            RemoveNode(dataBase);
            break;
        case 'S':
            SortByName(dataBase);
            break;
        case 'f':
            Find(dataBase);
            break;
        case 'g':
            Get(dataBase);
            break;
        case 's':
            Set(dataBase);
            break;
        case 'd':
            PrintNodes(dataBase.GetNodes());
            break;
        case 'W':
            Save(dataBase);
            break;
        case 'L':
            Load(dataBase);
            break;
        
        default:
            cout << "Incorrect command" << endl;
            break;
        }
    }
}

