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
            LLNode* node = new LLNode(item);
            node->_nextNode = preNode->_nextNode;
            preNode->_nextNode = node;

            if (preNode == _lastNode)
                _lastNode = node;
            
            _size++;
                
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

    template <typename T>
    class DLList {
    public:
        class DLLNode {
        private:
            T _content;
            DLLNode* _nextNode = nullptr;
            DLLNode* _preNode = nullptr;

        public:
            DLLNode(const T& content)
            : _content(content) {}

            const T& Get() const {
                return _content;
            }
            void Set(T& value) {
                _content = value;
            }

            DLLNode* Next() {
                return _nextNode;
            }
            DLLNode* Back() {
                return _preNode;
            }

            friend class DLList;
        };

    private:
        DLLNode* _rootNode = nullptr;
        DLLNode* _lastNode = nullptr;
        uint32_t _size = 0;        

    public:
        DLList() = default;
        DLList(const DLList& other) {
            if (other._size == 0)
                return;
            
            _rootNode = new DLLNode(other._rootNode->Get());
            DLLNode* cNodeSelf = _rootNode;
            for (DLLNode* cNodeOther = other._rootNode->Next(); cNodeOther != nullptr; cNodeOther = cNodeOther->Next(), cNodeSelf = cNodeSelf->Next()) {
                cNodeSelf->_nextNode = new DLLNode(cNodeOther->Get());
                cNodeSelf->_nextNode->_preNode = cNodeSelf;
            }
            _lastNode = cNodeSelf;
            _size = other._size;
        }
        ~DLList() {
            Clear();
        }

        uint32_t Size() {
            return _size;
        }

        DLLNode* Root() {
            return _rootNode;
        }
        DLLNode* Last() {
            return _lastNode;
        }

        DLLNode* Get(const int index) const {
            if (index < 0 || index >= _size) {
                std::cout << "Bad argument value\n";
                return nullptr;
            }

            DLLNode* cNode = nullptr;
            if (index + 1 < _size - (index + 1)) {
                cNode = _rootNode;
                for (int i = 0; i < index; i++) {
                    cNode = cNode->_nextNode;
                }
            } else {
                cNode = _lastNode;
                for (int i = _size - 1; i > index; i--) {
                    cNode = cNode->_preNode;
                }
            }

            return cNode;
        }

        DLLNode* FindFront(const T& item) const {
            DLLNode* cNode = _lastNode;
            while (cNode != nullptr) {
                if (cNode->Get() == item)
                    break;
                cNode = cNode->_preNode;
            }
            return cNode;
        }

        DLLNode* FindBack(const T& item) const {
            DLLNode* cNode = _rootNode;
            while (cNode != nullptr) {
                if (cNode->Get() == item)
                    break;
                cNode = cNode->_nextNode;
            }
            return cNode;
        }

        DLLNode* AddFront(const T& item) {
            DLLNode* node = new DLLNode(item);
            if (_size == 0) {
                _rootNode = node;
                _lastNode = node;
            } else {
                _lastNode->_nextNode = node;
                node->_preNode = _lastNode;
                _lastNode = node;
            }            
            _size++;
            return node;
        }
        
        DLLNode* AddBack(const T& item) {
            DLLNode* node = new DLLNode(item);
            if (_size == 0) {
                _rootNode = node;
                _lastNode = node;
            } else {
                _rootNode->_preNode = node;
                node->_nextNode = _rootNode;
                _rootNode = node;
            }            
            _size++;
            return node;
        }

        DLLNode* InsertAfter(const T& item, DLLNode* preNode) {
            DLLNode* node = new DLLNode(item);
            node->_nextNode = preNode->_nextNode;
            preNode->_nextNode = node;
            node->_preNode = preNode;

            if (preNode == _lastNode) {
                _lastNode = node;
            } else {
                node->_nextNode->_preNode = node;
            }

            _size++;
                
            return node;
        }

        DLLNode* InsertAfter(const T& item, int index) {
            DLLNode* preNode = Get(index);
            InsertAfter(item, preNode);
        }

        DLLNode* InsertBefore(const T& item, DLLNode* nextNode) {
            DLLNode* node = new DLLNode(item);
            node->_preNode = nextNode->_preNode;
            nextNode->_preNode = node;
            node->_nextNode = nextNode;

            if (nextNode == _rootNode) {
                _rootNode = node;
            } else {
                node->_preNode->_nextNode = node;
            }

            _size++;
                
            return node;
        }

        DLLNode* InsertBefore(const T& item, int index) {
            DLLNode* nextNode = Get(index);
            InsertBefore(item, nextNode);
        }

        void Remove(DLLNode* node) {
            if (_size == 0) {
                std::cout << "The list is empty\n";
                return;
            }

            if (node == _rootNode) {
                _rootNode = node->_nextNode;
                if (node->_nextNode != nullptr)
                    node->_nextNode->_preNode = nullptr;
                else {
                    _lastNode = nullptr;
                }
            } else if (node == _lastNode) {
                _lastNode = node->_preNode;
                node->_preNode->_nextNode = nullptr;
            } else {
                node->_preNode->_nextNode = node->_nextNode;
                node->_nextNode->_preNode = node->_preNode;
            }

            delete node;
            _size--;
        }

        void Remove(const T& item) {
            if (_size == 0) {
                std::cout << "The list is empty\n";
                return;
            }

            DLLNode* node = FindBack(item);

            if (node == _rootNode) {
                _rootNode = node->_nextNode;
                if (node->_nextNode != nullptr)
                    node->_nextNode->_preNode = nullptr;
                else {
                    _lastNode = nullptr;
                }
            } else if (node == _lastNode) {
                _lastNode = node->_preNode;
                node->_preNode->_nextNode = nullptr;
            } else {
                node->_preNode->_nextNode = node->_nextNode;
                node->_nextNode->_preNode = node->_preNode;
            }

            delete node;
            _size--;
        }

        void RemoveByIndex(int index) {
            if (_size == 0) {
                std::cout << "The list is empty\n";
                return;
            }
            if (index < 0 || index >= _size) {
                std::cout << "Bad argument value\n";
                return;
            }

            DLLNode* node = Get(index);

            if (node == _rootNode) {
                _rootNode = node->_nextNode;
                if (node->_nextNode != nullptr)
                    node->_nextNode->_preNode = nullptr;
                else {
                    _lastNode = nullptr;
                }
            } else if (node == _lastNode) {
                _lastNode = node->_preNode;
                node->_preNode->_nextNode = nullptr;
            } else {
                node->_preNode->_nextNode = node->_nextNode;
                node->_nextNode->_preNode = node->_preNode;
            }

            delete node;
            _size--;
        }

        void Clear() {
            DLLNode* cNode = _rootNode;
            while (cNode != nullptr) {
                DLLNode* node = cNode;
                cNode = cNode->_nextNode;
                delete node;
            }
            _rootNode = nullptr;
            _lastNode = nullptr;
            _size = 0;
        }

        DLList& operator=(const DLList& other) {
            Clear();
            if (other._size == 0)
                return *this;
            
            _rootNode = new DLLNode(other._rootNode->Get());
            DLLNode* cNodeSelf = _rootNode;
            for (DLLNode* cNodeOther = other._rootNode->Next(); cNodeOther != nullptr; cNodeOther = cNodeOther->Next(), cNodeSelf = cNodeSelf->Next()) {
                cNodeSelf->_nextNode = new DLLNode(cNodeOther->Get());
                cNodeSelf->_nextNode->_preNode = cNodeSelf;
            }
            _lastNode = cNodeSelf;
            _size = other._size;
            return *this;
        }
        
    };

    template <typename T>
    class CDLList {
    public:
        class CDLLNode {
        private:
            T _content;
            CDLLNode* _nextNode = nullptr;
            CDLLNode* _preNode = nullptr;

        public:
            CDLLNode(const T& content)
            : _content(content) {}

            const T& Get() const {
                return _content;
            }
            void Set(T& value) {
                _content = value;
            }

            CDLLNode* Next() {
                return _nextNode;
            }
            CDLLNode* Back() {
                return _preNode;
            }

            friend class CDLList;
        };

    private:
        CDLLNode* _rootNode = nullptr;
        uint32_t _size = 0;        

    public:
        CDLList() = default;
        CDLList(const CDLList& other) {
            if (other._size == 0)
                return;
            
            _rootNode = new CDLLNode(other._rootNode->Get());
            CDLLNode* cNodeSelf = _rootNode;
            for (CDLLNode* cNodeOther = other._rootNode->Next(); cNodeOther != other._rootNode; cNodeOther = cNodeOther->Next(), cNodeSelf = cNodeSelf->Next()) {
                cNodeSelf->_nextNode = new CDLLNode(cNodeOther->Get());
                cNodeSelf->_nextNode->_preNode = cNodeSelf;
            }
            _rootNode->_preNode = cNodeSelf;
            cNodeSelf->_nextNode = _rootNode;

            _size = other._size;
        }
        ~CDLList() {
            Clear();
        }

        uint32_t Size() {
            return _size;
        }

        CDLLNode* Root() {
            return _rootNode;
        }

        CDLLNode* Get(const int index) const {
            if (index < 0 || index >= _size) {
                std::cout << "Bad argument value\n";
                return nullptr;
            }

            CDLLNode* cNode = _rootNode;
            if (index + 1 < _size - (index + 1)) {
                for (int i = 0; i < index; i++) {
                    cNode = cNode->_nextNode;
                }
            } else {
                for (int i = _size; i > index; i--) {
                    cNode = cNode->_preNode;
                }
            }

            return cNode;
        }

        CDLLNode* FindFront(const T& item) const {
            if (_rootNode == nullptr)
                return nullptr;

            CDLLNode* cNode = _rootNode;            
            while (true) {
                if (cNode->Get() == item)
                    break;
                if (cNode->_preNode == _rootNode)
                    return nullptr;
                cNode = cNode->_preNode;
            }
            return cNode;
        }

        CDLLNode* FindBack(const T& item) const {
            if (_rootNode == nullptr)
                return nullptr;

            CDLLNode* cNode = _rootNode;            
            while (true) {
                if (cNode->Get() == item)
                    break;
                if (cNode->_nextNode == _rootNode)
                    return nullptr;
                cNode = cNode->_nextNode;
            }
            return cNode;
        }

        CDLLNode* AddFront(const T& item) {
            CDLLNode* node = new CDLLNode(item);
            if (_size == 0) {
                _rootNode = node;
                node->_preNode = node;
                node->_nextNode = node;
            } else {
                _rootNode->_preNode->_nextNode = node;
                node->_preNode = _rootNode->_preNode;
                _rootNode->_preNode = node;
                node->_nextNode = _rootNode;
            }            
            _size++;
            return node;
        }
        
        CDLLNode* AddBack(const T& item) {
            CDLLNode* node = new CDLLNode(item);
            if (_size == 0) {
                _rootNode = node;
                node->_preNode = node;
                node->_nextNode = node;
            } else {
                _rootNode->_preNode->_nextNode = node;
                node->_preNode = _rootNode->_preNode;
                _rootNode->_preNode = node;
                node->_nextNode = _rootNode;
                _rootNode = node;
            }            
            _size++;
            return node;
        }

        CDLLNode* InsertAfter(const T& item, CDLLNode* preNode) {
            CDLLNode* node = new CDLLNode(item);
            node->_nextNode = preNode->_nextNode;
            preNode->_nextNode = node;
            node->_preNode = preNode;
            node->_nextNode->_preNode = node;

            _size++;
                
            return node;
        }

        CDLLNode* InsertAfter(const T& item, int index) {
            CDLLNode* preNode = Get(index);
            InsertAfter(item, preNode);
        }

        CDLLNode* InsertBefore(const T& item, CDLLNode* nextNode) {
            CDLLNode* node = new CDLLNode(item);
            node->_preNode = nextNode->_preNode;
            nextNode->_preNode = node;
            node->_nextNode = nextNode;
            node->_preNode->_nextNode = node;

            if (nextNode == _rootNode) {
                _rootNode = node;
            }

            _size++;
                
            return node;
        }

        CDLLNode* InsertBefore(const T& item, int index) {
            CDLLNode* nextNode = Get(index);
            InsertBefore(item, nextNode);
        }

        void Remove(CDLLNode* node) {
            if (_size == 0) {
                std::cout << "The list is empty\n";
                return;
            }

            if (node == _rootNode) {
                if (_size == 1) {
                    _rootNode = nullptr;
                } else {
                    node->_preNode->_nextNode = node->_nextNode;
                    node->_nextNode->_preNode = node->_preNode;
                    _rootNode = _rootNode->_nextNode;
                }
            } else {
                node->_preNode->_nextNode = node->_nextNode;
                node->_nextNode->_preNode = node->_preNode;
            }

            delete node;
            _size--;
        }

        void Remove(const T& item) {
            if (_size == 0) {
                std::cout << "The list is empty\n";
                return;
            }

            CDLLNode* node = FindBack(item);

            if (node == _rootNode) {
                if (_size == 1) {
                    _rootNode = nullptr;
                } else {
                    node->_preNode->_nextNode = node->_nextNode;
                    node->_nextNode->_preNode = node->_preNode;
                    _rootNode = _rootNode->_nextNode;
                }
            } else {
                node->_preNode->_nextNode = node->_nextNode;
                node->_nextNode->_preNode = node->_preNode;
            }

            delete node;
            _size--;
        }

        void RemoveByIndex(int index) {
            if (_size == 0) {
                std::cout << "The list is empty\n";
                return;
            }
            if (index < 0 || index >= _size) {
                std::cout << "Bad argument value\n";
                return;
            }

            CDLLNode* node = Get(index);

            if (node == _rootNode) {
                if (_size == 1) {
                    _rootNode = nullptr;
                } else {
                    node->_preNode->_nextNode = node->_nextNode;
                    node->_nextNode->_preNode = node->_preNode;
                    _rootNode = _rootNode->_nextNode;
                }
            } else {
                node->_preNode->_nextNode = node->_nextNode;
                node->_nextNode->_preNode = node->_preNode;
            }

            delete node;
            _size--;
        }

        void Clear() {
            if (_size == 0)
                return;
            
            _rootNode->_preNode->_nextNode = nullptr;
            CDLLNode* cNode = _rootNode;
            while (cNode != nullptr) {
                CDLLNode* node = cNode;
                cNode = cNode->_nextNode;
                delete node;
            }
            _rootNode = nullptr;
            _size = 0;
        }

        CDLList Reversed() {
            CDLList reversedList;
            if (_size == 0) 
                return reversedList;
            
            reversedList._rootNode = new CDLLNode(_rootNode->Get());
            CDLLNode* cNodeRev = reversedList._rootNode;
            for (CDLLNode* cNodeSelf = _rootNode->Back(); cNodeSelf != _rootNode; cNodeSelf = cNodeSelf->Back(), cNodeRev = cNodeRev->Next()) {
                cNodeRev->_nextNode = new CDLLNode(cNodeSelf->Get());
                cNodeRev->_nextNode->_preNode = cNodeRev;
            }
            reversedList._rootNode->_preNode = cNodeRev;
            cNodeRev->_nextNode = reversedList._rootNode;
            reversedList._rootNode = reversedList._rootNode->_nextNode;

            reversedList._size = _size;
            return reversedList;
        }

        CDLList& operator=(const CDLList& other) {
            Clear();
            if (other._size == 0)
                return *this;
            
            _rootNode = new CDLLNode(other._rootNode->Get());
            CDLLNode* cNodeSelf = _rootNode;
            for (CDLLNode* cNodeOther = other._rootNode->Next(); cNodeOther != other._rootNode; cNodeOther = cNodeOther->Next(), cNodeSelf = cNodeSelf->Next()) {
                cNodeSelf->_nextNode = new CDLLNode(cNodeOther->Get());
                cNodeSelf->_nextNode->_preNode = cNodeSelf;
            }
            _rootNode->_preNode = cNodeSelf;
            cNodeSelf->_nextNode = _rootNode;

            _size = other._size;
            return *this;
        }
        
    };
}