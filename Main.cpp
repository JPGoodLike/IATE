#include <iostream>
#include <fstream>
#include <set>
#include <unordered_map>
#include <vector>
#include <memory>

using namespace std;

struct Segment {
    int leftEnd, rightEnd;
    Segment(int leftEnd=0, int rightEnd=0) 
    : leftEnd(leftEnd), rightEnd(rightEnd) {}
};

struct SegmentComparator {
    bool operator()(const unique_ptr<Segment>& first, const unique_ptr<Segment>& second) const {
        return first->leftEnd < second->leftEnd;
    }
};

multiset<unique_ptr<Segment>, SegmentComparator> g_leftTipSorted;
unordered_map<Segment*, vector<Segment*>> g_innerSegmentMap;
unordered_map<Segment*, vector<Segment*>> g_outerSegmentMap;


int main() {
    ifstream ib_file("input.bin", ios::in | ios::binary);
    Segment segment;
    // for (int i = 10; i > 5; i--) {
    //     segment.leftEnd = i;
    //     segment.rightEnd = 1000 - i*i;
    //     ib_file.write((char*)&segment, sizeof(Segment));
    // }
    // ib_file.seekg(0);
    while (ib_file.read((char*)&segment, sizeof(Segment))) {
        printf("left end: %i, right end: %i\n", segment.leftEnd, segment.rightEnd);
        auto it_segment = g_leftTipSorted.insert(make_unique<Segment>(segment));
        g_innerSegmentMap.insert(make_pair((*it_segment).get(), vector<Segment*>()));
        g_outerSegmentMap.insert(make_pair((*it_segment).get(), vector<Segment*>()));

        auto it_cGreater = it_segment;
        for (; it_cGreater != g_leftTipSorted.end(); it_cGreater++)
            if ((*it_cGreater)->leftEnd > (*it_segment)->leftEnd)
                break;
        for (; it_cGreater != g_leftTipSorted.end(); it_cGreater++) {
            if ((*it_cGreater)->rightEnd < (*it_segment)->rightEnd) {
                g_innerSegmentMap[(*it_segment).get()].push_back((*it_cGreater).get());
                g_outerSegmentMap[(*it_cGreater).get()].push_back((*it_segment).get());
                for (auto& el : g_innerSegmentMap[(*it_cGreater).get()]) {
                    g_innerSegmentMap[(*it_segment).get()].push_back(el);
                    g_outerSegmentMap[el].push_back((*it_segment).get());
                }
                break;
            }
        }

        auto it_cLess = make_reverse_iterator(it_segment);
        for (; it_cLess != g_leftTipSorted.rend(); it_cLess++)
            if ((*it_cLess)->leftEnd < (*it_segment)->leftEnd)
                break;
        for (; it_cLess != g_leftTipSorted.rend(); it_cLess++) {
            if ((*it_cLess)->rightEnd > (*it_segment)->rightEnd) {
                g_outerSegmentMap[(*it_segment).get()].push_back((*it_cLess).get());
                g_innerSegmentMap[(*it_cLess).get()].push_back((*it_segment).get());
                for (auto& el : g_outerSegmentMap[(*it_cLess).get()]) {
                    g_outerSegmentMap[(*it_segment).get()].push_back(el);
                    g_innerSegmentMap[el].push_back((*it_segment).get());
                }
                break;
            }
        }
    }
    ib_file.close();

    ofstream ob_file("output.bin", ios::binary | ios::out);
    printf("\n");
    for (auto it = g_leftTipSorted.begin(); it != g_leftTipSorted.end(); it++) {
        vector<Segment*>& innerList = g_innerSegmentMap[(*it).get()];
        if (innerList.size() == 0)
            continue;

        printf("Segment with left end: %i, right end: %i contains:\n", (*it)->leftEnd, (*it)->rightEnd);
        ob_file.write((char*)(*it).get(), sizeof(Segment));
        int innersQ = innerList.size();
        ob_file.write((char*)&innersQ, sizeof(int));
        for (auto& el : innerList) {
            printf("Segment with left end: %i, right end: %i\n", el->leftEnd, el->rightEnd);
            ob_file.write((char*)el, sizeof(Segment));
        }
    }
    
    ob_file.close();
}