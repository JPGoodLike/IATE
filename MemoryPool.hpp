#pragma once

#include <vector>

namespace mbgl {

    template <typename T>
    struct MemoryBlock {
        T* startP;
        T* endP; // points after the last element

        MemoryBlock(T* startP, T* endP)
        : startP(startP), endP(endP) {}
    };

    template <typename T>
    class MemoryPool {
    private:
        const unsigned int c_chunkSize; // in T

        std::vector<T*> _chunks;
        std::vector<MemoryBlock<T>> _freeList;

        void AllocateChunk() {
            T* chunk = new T[c_chunkSize];
            _chunks.push_back(chunk);
            _freeList.emplace_back(chunk, chunk + c_chunkSize);
        }
    
    public:
        MemoryPool(const unsigned int size) 
        : c_chunkSize(size) {            
            AllocateChunk();
        }
        ~MemoryPool() {
            for (auto& chunk : _chunks)
                delete[] chunk;
            _chunks.clear();
        }

        T* Push(const T& obj) {
            if (_freeList.empty()) {
                AllocateChunk();
            }
            MemoryBlock<T>& freeBlock = _freeList.back();
            T* startP = freeBlock.startP;
            *startP = obj;
            freeBlock.startP++;
            if (freeBlock.startP == freeBlock.endP) {
                _freeList.pop_back();
            } 
            return startP;
        }

        template <typename... Args>
        T* Emplace(Args... args) {
            if (_freeList.empty()) {
                AllocateChunk();
            }
            MemoryBlock<T>& freeBlock = _freeList.back();
            T* startP = freeBlock.startP;
            *startP = T(args...);
            freeBlock.startP++;
            if (freeBlock.startP == freeBlock.endP) {
                _freeList.pop_back();
            } 
            return startP;
        }

        void Remove(T* obj) {
            for (auto& mb : _freeList) {
                if (obj == mb.startP - 1) {
                    mb.startP--;
                    return;
                }
                if (obj == mb.endP) {
                    mb.endP++;
                    return;
                }
            }
            _freeList.emplace_back(obj, obj + 1);
        }
    };
}