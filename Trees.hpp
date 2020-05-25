#include <iostream>

namespace mbgl {

    template <typename T>
    class AVLTree {
    public:
        class Node {
        private:
            Node* _leftNode = nullptr;
            Node* _rightNode = nullptr;
            int _height = 0;

            T _data;

        public:
            Node(const T& data)
            : _data(data) {}

            Node* Left() const {
                return _leftNode;
            }
            Node* Right() const {
                return _rightNode;
            }

            T& Get() const {
                return _data;
            }
            void Set(const T& value) {
                _data = value;
            }
        };

    private:
        Node* _rootNode = nullptr;
        uint32_t _size = 0;

    public:
        AVLTree() = default;
        AVLTree(const AVLTree& other) {

        }
        ~AVLTree() {
            Clear();
        }

        uint32_t Size() const {
            return _size;
        }

        Node* Root() const {
            return _rootNode;
        }

        void Clear() {
            
        }

    };
}