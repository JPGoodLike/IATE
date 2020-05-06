#include <iostream>
#include "Lists.hpp"

using namespace std;
using namespace mbgl;

void PrintList(LList<int> list) {
    if (list.Size() == 0) {
        cout << "The list is empty\n";
        return;
    }
    for (auto cNode = list.Root(); cNode != nullptr; cNode = cNode->Next()) {
        cout << cNode->Get() << " ";
    }
    cout << endl;
}

void PrintList(DLList<int> list) {
    if (list.Size() == 0) {
        cout << "The list is empty\n";
        return;
    }
    for (auto cNode = list.Root(); cNode != nullptr; cNode = cNode->Next()) {
        cout << cNode->Get() << " ";
    }
    cout << endl;
}

void PrintList(CDLList<int> list) {
    if (list.Size() == 0) {
        cout << "The list is empty\n";
        return;
    }
    for (auto cNode = list.Root(); true; cNode = cNode->Next()) {
        cout << cNode->Get() << " ";
        if (cNode->Next() == list.Root())
            break;
    }
    cout << endl;
}

template <typename T>
void Print(const T& value) {
    cout << value << "\n";
}

void LListTest() {
    LList<int> list;
    list.Add(5);
    list.Add(3);
    PrintList(list);
    list.Remove(5);
    PrintList(list);
    list.RemoveByIndex(0);
    PrintList(list);
    list.Add(7); list.Add(9); list.Add(3);
    {
        LList<int> list2;
        list2 = list;
        PrintList(list);
        PrintList(list2);
        auto node1 = list.Find(9); 
        Print(node1->Get());
        list.Remove(list.Root());
        list2.RemoveByIndex(list2.Size()-1);
        PrintList(list);
        PrintList(list2);
        list.RemoveByIndex(0);
        list2.Remove(list2.Get(1));
        PrintList(list);
        PrintList(list2);

        list.InsertAfter(8, list.Find(3));
        list.InsertAfter(5, list.Size()-1);
        list2.InsertAfter(57, 0);
        PrintList(list);
        PrintList(list2);
    }
    PrintList(list);
}

void DLListTest() {
    DLList<int> list;
    list.AddFront(5);
    list.AddFront(3);
    PrintList(list);
    list.Remove(5);
    PrintList(list);
    list.RemoveByIndex(0);
    PrintList(list);
    list.AddFront(7); list.AddFront(9); list.AddFront(3);
    {
        DLList<int> list2;
        list2 = list;
        PrintList(list);
        PrintList(list2);
        auto node1 = list.FindFront(9); 
        Print(node1->Get());
        list.Remove(list.Root());
        list2.RemoveByIndex(list2.Size()-1);
        PrintList(list);
        PrintList(list2);
        list.RemoveByIndex(0);
        list2.Remove(list2.Get(1));
        PrintList(list);
        PrintList(list2);

        list.InsertAfter(8, list.FindBack(3));
        list.InsertAfter(5, list.Size()-1);
        list2.InsertAfter(57, 0);
        PrintList(list);
        PrintList(list2);
    }
    PrintList(list);
    cout << "Extra Test^:\n";
    list.AddBack(11);
    auto node = list.Get(1);
    list.InsertBefore(list.Get(0)->Get(), node);
    list.Remove(node);
    PrintList(list);
}

void CDLListTest() {
    CDLList<int> list;
    list.AddFront(5);
    list.AddFront(3);
    PrintList(list);
    list.Remove(5);
    PrintList(list);
    list.RemoveByIndex(0);
    PrintList(list);
    list.AddFront(7); list.AddFront(9); list.AddFront(3);
    {
        CDLList<int> list2;
        list2 = list;
        PrintList(list);
        PrintList(list2);
        auto node1 = list.FindFront(9); 
        Print(node1->Get());
        list.Remove(list.Root());
        list2.RemoveByIndex(list2.Size()-1);
        PrintList(list);
        PrintList(list2);
        list.RemoveByIndex(0);
        list2.Remove(list2.Get(1));
        PrintList(list);
        PrintList(list2);

        list.InsertAfter(8, list.FindBack(3));
        list.InsertAfter(5, list.Size()-1);
        list2.InsertAfter(57, 0);
        PrintList(list);
        PrintList(list2);
    }
    PrintList(list);
    cout << "Extra Test^:\n";
    list.AddBack(11);
    auto node = list.Get(1);
    list.InsertBefore(list.Get(0)->Get(), node);
    list.Remove(node);
    PrintList(list);
}

int main() {
    CDLListTest();

}