#include <iostream>

using namespace std;

int main() {
    for (char i = '1'; i < '6'; i++) {
        string if_name = "./TestFiles/input"; if_name += i;
        string of_name = "./TestFiles/output"; of_name += i;

        string com_BinWriter = "./BinWriter " + if_name;
        string com_Main2 = "./Main2 " + if_name + " " + of_name;
        cout << system(com_BinWriter.c_str()) << "\n";
        cout << system(com_Main2.c_str()) << "\n";
    }

}