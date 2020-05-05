#include <iostream>


namespace mbgl {
    template <typename T>
    class LList {
    public:
        class LLNode {
        private:
            T _content;
            LLNode* _nextNode = nullptr;

        public:
            LLNode(const T& content)
            : _content(content) {}

            const T& Get() const {
                return _content;
            }
            void Set(T& value) {
                _content = value;
            }

            LLNode* Next() {
                return _nextNode;
            }

            friend class LList;
        };

    private:
        LLNode* _rootNode = nullptr;
        LLNode* _lastNode = nullptr;
        uint32_t _size = 0;        

    public:
        LList() = default;
        LList(const LList& other) {
            if (other._size == 0)
                return;
            
            _rootNode = new LLNode(other._rootNode->Get());
            LLNode* cNodeSelf = _rootNode;
            for (LLNode* cNodeOther = other._rootNode->Next(); cNodeOther != nullptr; cNodeOther = cNodeOther->Next(), cNodeSelf = cNodeSelf->Next()) {
                cNodeSelf->_nextNode = new LLNode(cNodeOther->Get());
            }
            _lastNode = cNodeSelf;
            _size = other._size;
        }
        ~LList() {
            Clear();
        }

        uint32_t Size() {
            return _size;
        }

        LLNode* Root() {
            return _rootNode;
        }
        LLNode* Last() {
            return _lastNode;
        }

        LLNode* Get(const int num) const {
            if (num < 0 || num >= _size) {
                std::cout << "Bad argument value\n";
                return nullptr;
            }

            LLNode* cNode = _rootNode;
            for (int i = 0; i < num; i++) {
                cNode = cNode->_nextNode;
            }
            return cNode;
        }

        LLNode* Find(const T& item) const {
            LLNode* cNode = _rootNode;
            while (cNode != nullptr) {
                if (cNode->Get() == item)
                    break;
                cNode = cNode->_nextNode;
            }
            return cNode;
        }

        LLNode* Add(const T& item) {
            LLNode* node = new LLNode(item);
            if (_size == 0) {
                _rootNode = node;
                _lastNode = node;
            } else {
                _lastNode->_nextNode = node;
                _lastNode = node;
            }            
            _size++;
            return node;
        }

        LLNode* InsertAfter(const T& item, LLNode* preNode) {
            LLNode* cNode = _rootNode;
            while (cNode != nullptr) {
                if (cNode == preNode)
                    break;
                
                cNode = cNode->_nextNode;
            }
            if (cNode == nullptr) {
                std::cout << "The previous node not found";
                return nullptr;
            }

            LLNode* node = new LLNode(item);
            node->_nextNode = cNode->_nextNode;
            cNode->_nextNode = node;
            _size++;

            if (cNode == _lastNode)
                _lastNode = node;
                
            return node;
        }

        LLNode* InsertAfter(const T& item, int index) {
            if (index < 0 || index >= _size) {
                std::cout << "Bad argument value\n";
                return nullptr;
            }

            LLNode* cNode = _rootNode;
            for (int i = 0; i < index; i++) {
                cNode = cNode->_nextNode;
            }

            LLNode* node = new LLNode(item);
            node->_nextNode = cNode->_nextNode;
            cNode->_nextNode = node;
            _size++;

            if (cNode == _lastNode)
                _lastNode = node;
            
            return node;
        }

        void Remove(LLNode* node, LLNode* preNode) {
            if (preNode->_nextNode != node || _size < 2) {
                std::cout << "Bad arguments value\n";
                return;
            }

            preNode->_nextNode = node->_nextNode;
            if (node == _lastNode)
                _lastNode = preNode;
            
            delete node;
            _size--;
        }

        void Remove(LLNode* node) {
            if (_size == 0) {
                std::cout << "The list is empty\n";
                return;
            }

            LLNode* cNode = _rootNode;
            LLNode* cPreNode = nullptr;
            while (cNode != nullptr) {
                if (cNode == node) {
                    break;
                }
                cPreNode = cNode;
                cNode = cNode->_nextNode;
            }
            if (cNode != nullptr) {
                if (cPreNode != nullptr) {
                    cPreNode->_nextNode = cNode->_nextNode;
                } else {
                    _rootNode = cNode->_nextNode;
                }

                if (cNode == _lastNode)
                    _lastNode = cPreNode;

                delete cNode;
                _size--;
            }
        }

        void Remove(const T& item) {
            if (_size == 0) {
                std::cout << "The list is empty\n";
                return;
            }

            LLNode* cNode = _rootNode;
            LLNode* cPreNode = nullptr;
            while (cNode != nullptr) {
                if (cNode->Get() == item) {
                    break;
                }
                cPreNode = cNode;
                cNode = cNode->_nextNode;
            }
            if (cNode != nullptr) {
                if (cPreNode != nullptr) {
                    cPreNode->_nextNode = cNode->_nextNode;
                } else {
                    _rootNode = cNode->_nextNode;
                }

                if (cNode == _lastNode)
                    _lastNode = cPreNode;

                delete cNode;
                _size--;
            }
        }

        void RemoveByIndex(int num) {
            if (_size == 0) {
                std::cout << "The list is empty\n";
                return;
            }
            if (num < 0 || num >= _size) {
                std::cout << "Bad argument value\n";
                return;
            }

            LLNode* cNode = _rootNode;
            LLNode* cPreNode = nullptr;
            for (int i = 0; i < num; i++) {
                cPreNode = cNode;
                cNode = cNode->_nextNode;
            }
            if (cNode != nullptr) {
                if (cPreNode != nullptr) {
                    cPreNode->_nextNode = cNode->_nextNode;
                } else {
                    _rootNode = cNode->_nextNode;
                }

                if (cNode == _lastNode)
                    _lastNode = cPreNode;

                delete cNode;
                _size--;
            }
        }

        void Clear() {
            LLNode* cNode = _rootNode;
            while (cNode != nullptr) {
                LLNode* node = cNode;
                cNode = cNode->_nextNode;
                delete node;
            }
            _rootNode = nullptr;
            _lastNode = nullptr;
            _size = 0;
        }

        LList& operator=(const LList& other) {
            Clear();
            if (other._size == 0)
                return *this;
            
            _rootNode = new LLNode(other._rootNode->Get());
            LLNode* cNodeSelf = _rootNode;
            for (LLNode* cNodeOther = other._rootNode->Next(); cNodeOther != nullptr; cNodeOther = cNodeOther->Next(), cNodeSelf = cNodeSelf->Next()) {
                cNodeSelf->_nextNode = new LLNode(cNodeOther->Get());
            }
            _lastNode = cNodeSelf;
            _size = other._size;
            return *this;
        }
        
    };
}