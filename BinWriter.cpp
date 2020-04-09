#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

struct Segment {
    int leftEnd, rightEnd;
    Segment(int leftEnd=0, int rightEnd=0) 
    : leftEnd(leftEnd), rightEnd(rightEnd) {}
};

int main (int argc, const char* argv[]) {
    if (argc != 2)
        return 1;
    
    string it_fileName = string(argv[1]) + ".txt";
    string ob_fileName = string(argv[1]) + ".bin";

    ifstream it_file(it_fileName);
    ofstream ob_file(ob_fileName, ios::out | ios::binary);

    Segment segment;
    while(it_file >> segment.leftEnd) {
        it_file >> segment.rightEnd;
        ob_file.write((char*)&segment, sizeof(Segment));
    }

    it_file.close();
    ob_file.close();
}