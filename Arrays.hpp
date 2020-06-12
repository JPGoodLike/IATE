#pragma once
#include <iostream>
#include <sstream>
#include <stdexcept>


namespace mbgl {

    template <typename T>
    class SArray {
    private:
        int _len;
        T* _items;

    public:
        SArray(int len = 0) {
            this->_len = len;
            _items = new T[len];
        }
        ~SArray() {
            Clean();
        }
        
        void Set(const SArray<T>& other) {
            if (_len != other.Length()) {
                Clean();
                _len = other.Length();
                _items = new T[_len];
            }
            for (int i = 0; i < _len; i++) {
                _items[i] = other[i];
            }
        }

        int Length() const {
            return _len;
        }

        void Clean() {
            delete[] _items;
            _len = 0;
        }
        
        void operator=(const SArray<T>& other) {
            Set(other);
        }
        
        T& operator[](int i) const {
            if (0 <= i && i < _len) {    
                return _items[i];
            } else
                throw std::invalid_argument("Index is out of the array range");
        }

        friend std::ostream& operator<<(std::ostream& stream, const SArray& array) {
            for (int i = 0; i < array.Length() - 1; i++) {
                stream << array[i] << " ";
            }
            stream << array[array.Length()-1];
            return stream;
        }
    };

    template <typename T>
    class DArray {
    private:
        int _capacity;

        int _len;
        T* items;

    public:
        DArray(int initialCapacity = 0) 
        : _capacity(initialCapacity), _len(0) { 
            if (initialCapacity > 0) {           
                items = new T[initialCapacity];
            } else
                items = nullptr;
        }
        DArray(const DArray<T>& other)
        : _capacity(0), _len(0), items(nullptr) {
            Set(other);
        }
        ~DArray() {
            Clean();
        }
        
        void SetCapacity(int newCapacity) {
            if (newCapacity == 0)
                return;

            _capacity = newCapacity;
            T* new_items = new T[_capacity];
            int bound = std::min(_len, _capacity);
            for (int i = 0; i < bound; i++) {
                new_items[i] = items[i];
            }
            delete[] items;
            items = new_items;
        }
        void Set(const DArray<T>& other) {
            if (other.Length() > _capacity) {
                Clean();             
                _capacity = other.Length();
                items = new T[_capacity];
            }

            _len = other.Length();
            for (int i = 0; i < _len; i++) {
                items[i] = other[i];
            }
        }

        int Length() const {
            return _len;
        }
        int Size() const {
            return _len;
        }

        void Clean() {
            if (_capacity > 0) {
                delete[] items;
                items = nullptr;
                _capacity = 0;
                _len = 0;
            }
        }

        DArray<T> SubArray(int startI, int lastI) const {
            int subALen = lastI - startI + 1;
            DArray<T> subA(subALen);
            for(int i = 0, j = startI; i < subA._len; i++, j++)
                subA[i] = items[j];
            return subA;
        }

        //dynamic array methods


        void Push(T item) {
            if (_len + 1 > _capacity) {
                int newCapacity = (_capacity == 0 ? 1 : _capacity * 2);
                SetCapacity(newCapacity);
            }
            items[_len] = item;
            _len++;
        }

        T Pop() {
            if (_len <= 0)
                throw std::invalid_argument("Attempt to pop a non-existing item");

            _len--;
            return items[_len];
        }

        void Insert(T item, int index) {
            if (index < 0 || index > _len)
                throw std::invalid_argument("Index is out of the array range");

            if (_len + 1 > _capacity) {
                int newCapacity = (_capacity == 0) ? 1 : _capacity * 2;
                SetCapacity(newCapacity);
            }
            for (int i = _len; i > index; i--) {
                items[i] = items[i-1];
            }
            items[index] = item;
            _len++;
        }

        void Extend(unsigned int value) {
            if (_len + value > _capacity) {
                int upB = std::max(_len + (long)value, (long)_capacity);
                int newCapacity = (_capacity == 0) ? 1 : upB * 2;
                SetCapacity(newCapacity);
            }
            int newLen = _len + value;
            for (int i = _len; i < newLen; i++)
                items[i] = 0;

            _len = newLen;

        }

        void ShrinkTop(unsigned int value) {
            if (value > _len)
                std::invalid_argument("Shrinking value is greater than the array length");
            
            _len -= value;
        }
        
        T& operator[](int i) const {
            if (i < 0 || i >= _len)
                throw std::invalid_argument("Index is out of the array range");

            return items[i];
        }

        DArray& operator=(const DArray& other) {
            Set(other);
            return *this;
        }

        friend std::ostream& operator<<(std::ostream& stream, const DArray& array) {
            if (array.Length() > 0) {
                for (int i = 0; i < array.Length() - 1; i++) {
                    stream << array[i] << " ";
                }
                stream << array[array.Length()-1];
            }
            return stream;
        }
    };

}