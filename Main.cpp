#include "DataBase.hpp"

#include <iostream>
#include <string>
#include <fstream>

using namespace mbgl;
using namespace std;

void Help() {
    cout << "'h' - help, 'q' - exit, 'd' - display, 'n' - new, 'r' - remove, 'g' - get, 'S' - sort, 'f' - find, 'w' - save, 'wb' - save in binary, 'l' - load, 'lb' - load from binary," << endl;
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
    db -= name;
}

void PrintNodes(const DArray<PersonalIBM*> nodes) {
    for (int i = 0; i < nodes.Size(); i++) {
        cout << "---------------------------" << endl;
        cout << "Name: " << nodes[i]->name << endl;
        cout << "Firm: " << nodes[i]->firm << endl;
        cout << "Processor type: " << nodes[i]->processor_type << endl;
        cout << "CPU frequency: " << nodes[i]->cpu_frequency << endl;
        cout << "RAM capacity: " << nodes[i]->ram_capacity << endl;
        cout << "Hard Drive capacity: " << nodes[i]->hard_drive_capacity << endl;
        cout << "---------------------------" << endl;
        cout << endl;
    }
}

int LCmp(string self, string other) {
    int len = min(self.size(), other.size());
    for (int i = 0; i < len; i++) {
        if (self[i] == other[i])
            continue;

        if (self[i] > other[i]) {
            return 1;
        } else if (self[i] < other[i]) {
            return -1;
        } else
            return 0;
    }

    if (self.size() > other.size()) {
        return 1;
    } else if (self.size() < other.size()) {
        return -1;
    } else
        return 0;
}

int NameCmp(PersonalIBM* f, PersonalIBM* s) {
    return  LCmp(f->name, s->name);
}
void SortByName(DataBase& db) {
    PrintNodes(db << NameCmp);
}

void Get(DataBase& db) {
    std::string name;
    cin >> name;
    PersonalIBM* node = db[name];
    if (node == nullptr) {
        cout << "No such node\n";
        return;
    }

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
    if (node == nullptr) {
        cout << "No such node\n";
        return;
    }

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

    auto cond = [&](PersonalIBM* node) {
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
    };

    DArray<PersonalIBM*> nodes = db.GetSubset(cond);
    cout << "\n\n" << "Found: " << nodes.Size() << endl;
    PrintNodes(nodes);
}

void Save(DataBase& db) {
    std::string fileName = GetInput("File name: ");
    ofstream o_file(fileName);
    DArray<PersonalIBM*> nodes = ~db;
    for (int i = 0; i < nodes.Size(); i++) {
        o_file << "b:" << endl;
        o_file << nodes[i]->name << endl;
        o_file << nodes[i]->firm << endl;
        o_file << nodes[i]->processor_type << endl;
        o_file << nodes[i]->cpu_frequency << endl;
        o_file << nodes[i]->ram_capacity << endl;
        o_file << nodes[i]->hard_drive_capacity << endl;        
    }
    o_file.close();
}

void SaveBin(DataBase& db) {
    std::string fileName = GetInput("File name: ");
    ofstream o_file(fileName, std::ios::out | std::ios::binary);

    DArray<PersonalIBM*> nodes = ~db;

    for (int i = 0; i < nodes.Size(); i++) {
        o_file.write(nodes[i]->name.c_str(), sizeof(char) * nodes[i]->name.size() + 1);
        o_file.write(nodes[i]->firm.c_str(), sizeof(char) * nodes[i]->firm.size() + 1);
        o_file.write(nodes[i]->processor_type.c_str(), sizeof(char) * nodes[i]->processor_type.size() + 1);
        o_file.write(nodes[i]->cpu_frequency.c_str(), sizeof(char) * nodes[i]->cpu_frequency.size() + 1);
        o_file.write(nodes[i]->ram_capacity.c_str(), sizeof(char) * nodes[i]->ram_capacity.size() + 1);
        o_file.write(nodes[i]->hard_drive_capacity.c_str(), sizeof(char) * nodes[i]->hard_drive_capacity.size() + 1);
    }
    o_file.close();
}

void Load(DataBase& db) {
    db--;
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

std::string ReadStringBin(std::ifstream& f_input) {
    std::string sOut = "";
    char c;
    while (f_input.read((char*)&c, sizeof(char))) {
        if (c == '\0')
            break;
        sOut += c;
    }
    return sOut;
}

void LoadBin(DataBase& db) {
    db--;
    std::string fileName = GetInput("File name: ");
    ifstream i_file(fileName, std::ios::in | std::ios::binary);

    std::string line;
    while (true) {
        std::string name, firm, processor_type, cpu_frequency, ram_capacity, hard_drive_capacity;
        name = ReadStringBin(i_file);
        if (name == "")
            break;
        firm = ReadStringBin(i_file);
        processor_type = ReadStringBin(i_file);
        cpu_frequency = ReadStringBin(i_file);
        ram_capacity = ReadStringBin(i_file);
        hard_drive_capacity = ReadStringBin(i_file);
        db.Construct(name, name, firm, processor_type, cpu_frequency, ram_capacity, hard_drive_capacity);
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
        if (inputS == "q") {
            isRunning = false;
        } else if (inputS == "h") {
            Help();
        } else if (inputS == "n") {
            NewNode(dataBase);
        } else if (inputS == "r") {
            RemoveNode(dataBase);
        } else if (inputS == "S") {
            SortByName(dataBase);
        } else if (inputS == "f") {
            Find(dataBase);
        } else if (inputS == "g") {
            Get(dataBase);
        } else if (inputS == "s") {
            Set(dataBase);
        } else if (inputS == "d") {
            PrintNodes(~dataBase);
        } else if (inputS == "w") {
            Save(dataBase);
        } else if (inputS == "wb") {
            SaveBin(dataBase);
        } else if (inputS == "l") {
            Load(dataBase);
        } else if (inputS == "lb") {
            LoadBin(dataBase);
        } else {
            cout << "Incorrect command" << endl;
        }
    }
}

