#include <iostream>
#include <fstream>

#include "Lists.hpp"

using namespace std;
using namespace mbgl;

int main() {
    ifstream f_in("input.txt");
    ofstream f_out("output.txt");

    int q;
    f_in >> q;

    if (q == 0)
        return 0;

    DLList<float> list;
    float maxEl = -214748364;
    for (int i = 0; i < q; i++) {
        float temp;
        f_in >> temp;
        list.AddFront(temp);
        if (temp > maxEl)
            maxEl = temp;
    }

    for (auto cNode = list.Root(); cNode != nullptr;) {
        if (cNode->Get() == maxEl) {
            auto temp = cNode;
            cNode = cNode->Next();
            list.Remove(temp);
        } else {
            f_out << cNode->Get() << " ";
            cNode = cNode->Next();   
        }         
    }
    f_out << endl;

    f_in.close();
    f_out.close();
}