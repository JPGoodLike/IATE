#include <iostream>
#include <fstream>
#include <set>
#include <vector>

using namespace std;

struct Segment {
    int leftEnd, rightEnd;
    mutable int count;
    Segment(int leftEnd=0, int rightEnd=0, int count=1) 
    : leftEnd(leftEnd), rightEnd(rightEnd), count(count) {}
};

struct SegmentComparator {
    bool operator()(const Segment& first, const Segment& second) const {
        return first.leftEnd < second.leftEnd;
    }
};

multiset<Segment, SegmentComparator> g_leftTipSorted;


int main(int argc, const char* argv[]) {
    if (argc != 3)
        return 1;

    string ib_fileName = string(argv[1]) + ".bin";
    string ob_fileName = string(argv[2]) + ".bin";
    string ot_fileName = string(argv[2]) + ".txt";
    
    ifstream ib_file(ib_fileName, ios::in | ios::binary);
    Segment segment;
    while (ib_file.read((char*)&segment, 2*sizeof(int))) {
        printf("left end: %i, right end: %i\n", segment.leftEnd, segment.rightEnd);

        auto it_seg = g_leftTipSorted.lower_bound(segment);
        bool b = false;
        for (auto it = it_seg; it != g_leftTipSorted.end(); it++) {
            if (it->leftEnd != segment.leftEnd)
                break;
            
            if (it->rightEnd == segment.rightEnd) {
                it->count++;
                b = true;
                break;
            }                
        }
        if (!b)
            g_leftTipSorted.insert(segment);
    }
    ib_file.close();

    // printf("\n");

    ofstream ot_file(ot_fileName);
    ofstream ob_file(ob_fileName, ios::binary | ios::out);
    for (auto it = g_leftTipSorted.begin(); it != g_leftTipSorted.end(); it++) {
        segment = *it;
        ob_file.write((char*)&segment, sizeof(Segment));

        vector<Segment> v_out;
        for (auto it_other = it; it_other != g_leftTipSorted.end(); it_other++) {
            if (it->leftEnd < it_other->rightEnd && it_other->rightEnd < it->rightEnd) {
                segment = *it_other;
                ob_file.write((char*)&segment, sizeof(Segment));

                v_out.push_back(*it_other);
            }
        }

        if (v_out.size() > 0) {
            ot_file << "Segment with" << " count: " << it->count << ", left end: " << it->leftEnd << ", right end: " << it->rightEnd << " contains " << v_out.size() << " segments\n";
            ob_file.write((char*)&(*it), sizeof(Segment));
            int len = v_out.size();
            ob_file.write((char*)&len, sizeof(int));

            for (auto& s : v_out) {
                ot_file << "Segment with" << " count: " << s.count << ", left end: " << s.leftEnd << ", right end: " << s.rightEnd << "\n";
                ob_file.write((char*)&s, sizeof(Segment));
            }

            ot_file << "\n";
        }
    }
    
    ot_file.close();
    ob_file.close();
}