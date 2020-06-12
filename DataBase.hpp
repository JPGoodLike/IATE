#pragma once

#include "MemoryPool.hpp"

#include "Arrays.hpp"
#include "Trees.hpp"
#include <string>


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

    const unsigned int c_chunkCapacity = 134217728 / sizeof(PersonalIBM); // 128 Mb / Sizeof(Node)

    struct TreePair {
        std::string key;
        PersonalIBM* value;

        TreePair(const std::string& key, PersonalIBM* value)
        : key(key), value(value) {}
        TreePair(const std::string& key)
        : key(key), value(nullptr) {}

        bool operator > (const TreePair& other) const {
            return key > other.key;
        }
        bool operator < (const TreePair& other) const {
            return key < other.key;
        }
        bool operator == (const TreePair& other) const {
            return key == other.key;
        }
    };

    class DataBase {
    private:
        MemoryPool<PersonalIBM> _mp;
        AVLTree<TreePair> _nodes;
    
    private:
        void TreeToArray(AVLTree<TreePair>::Node* rootNode, DArray<PersonalIBM*>& outArray) {
            if (rootNode == nullptr)
                return;
            
            TreeToArray(rootNode->Left(), outArray);
            outArray.Push(rootNode->Get().value);
            TreeToArray(rootNode->Right(), outArray);
        }

        void QSort(DArray<PersonalIBM*>& array, int low, int high, int (*comparator)(PersonalIBM*, PersonalIBM*)) {
            int i = low;
            int j = high;
            PersonalIBM* pivot = array[(i + j) / 2];
            PersonalIBM* temp;

            while (i <= j) {
                while (comparator(array[i], pivot) < 0)
                    i++;
                while (comparator(array[j], pivot) > 0)
                    j--;
                if (i <= j) {
                    temp = array[i];
                    array[i] = array[j];
                    array[j] = temp;
                    i++;
                    j--;
                }
            }
            if (j > low)
                QSort(array, low, j, comparator);
            if (i < high)
                QSort(array, i, high, comparator);
        }
    
    public:
        DataBase()
        : _mp(c_chunkCapacity) {}

        void Add(const std::string name, const PersonalIBM& node) {
            _nodes.Insert(TreePair(name, _mp.Push(node)));
        }
        
        template <typename... Args>
        void Construct(const std::string name, Args... args) {
            _nodes.Insert(TreePair(name, _mp.Emplace(args...)));
        }

        void Remove(const std::string name) {
            auto node = _nodes.Find(name);
            if (node == nullptr)
                return;
            _mp.Remove(node->Get().value);
            _nodes.Remove(name);
        }

        PersonalIBM* Get(const std::string name) {
            auto node = _nodes.Find(name);
            if (node == nullptr)
                return nullptr;
            return node->Get().value;
        }

        DArray<PersonalIBM*> GetNodes() {
            DArray<PersonalIBM*> outV(_nodes.Size());

            TreeToArray(_nodes.Root(), outV);

            return outV;
        }

        DArray<PersonalIBM*> GetSorted(int (*comparator)(PersonalIBM*, PersonalIBM*) ) {
            DArray<PersonalIBM*> outV(_nodes.Size());
            TreeToArray(_nodes.Root(), outV);

            if (outV.Size() > 1)
                QSort(outV, 0, outV.Size()-1, comparator);

            return outV;
        }

        template <typename F>
        DArray<PersonalIBM*> GetSubset(F& condition) {
            DArray<PersonalIBM*> outV;

            DArray<AVLTree<TreePair>::Node*> stack;
            stack.Push(_nodes.Root());

            while (stack.Size() > 0) {
                auto cNode = stack.Pop();

                if (cNode == nullptr)
                    continue;
                
                if (condition(cNode->Get().value)) 
                    outV.Push(cNode->Get().value);
                
                stack.Push(cNode->Left());
                stack.Push(cNode->Right());
            }

            return outV;
        }

        void Clear() {
            DArray<AVLTree<TreePair>::Node*> stack;
            stack.Push(_nodes.Root());

            while (stack.Size() > 0) {
                auto cNode = stack.Pop();

                if (cNode == nullptr)
                    continue;
                
                _mp.Remove(cNode->Get().value);

                stack.Push(cNode->Left());
                stack.Push(cNode->Right());
            }

            _nodes.Clear();
        }

        PersonalIBM* operator[](const std::string name) { 
            return Get(name);
        }

        void operator-=(const std::string name) {
            Remove(name);
        }

        DArray<PersonalIBM*> operator~() {
            return GetNodes();
        }

        DArray<PersonalIBM*> operator<<(int (*comparator)(PersonalIBM*, PersonalIBM*)) {
            return GetSorted(comparator);
        }

        void operator--(int) {
            Clear();
        }

    };
}