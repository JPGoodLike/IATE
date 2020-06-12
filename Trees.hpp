#include <iostream>
#include <queue>
#include <math.h>

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
            int Height() const {
                return _height;
            }

            T& Get() {
                return _data;
            }
            void Set(const T& value) {
                _data = value;
            }

            friend class AVLTree;
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

        int Height() const {
            if (_rootNode == nullptr)
                return 0;
            return _rootNode->_height;
        }

        Node* Root() const {
            return _rootNode;
        }

        Node* Find(const T& value, Node* node) const {
            if (node == nullptr)
                return nullptr;
            if (node->_data == value)
                return node;
            
            if (value > node->_data) {
                return Find(value, node->_rightNode);
            } else {
                return Find(value, node->_leftNode);
            }
        }

        Node* Find(const T& value) const {
            return Find(value, _rootNode);
        }

        Node* FindMax(Node* node) const {
            if (node == nullptr)
                return nullptr;
            
            auto cNode = node;
            while (cNode->_rightNode != nullptr) {
                cNode = cNode->_rightNode;
            }

            return cNode;
        }

        Node* FindMin(Node* node) const {
            if (node == nullptr)
                return nullptr;
            
            auto cNode = node;
            while (cNode->_leftNode != nullptr) {
                cNode = cNode->_leftNode;
            }

            return cNode;
        }

        Node* Insert(const T& value, Node* node) {
            if (node == nullptr) {
                _size++;
                return new Node(value);
            }
            
            if (node->_data == value)
                return node;

            if (value > node->_data) {
                node->_rightNode = Insert(value, node->_rightNode);
            } else {
                node->_leftNode = Insert(value, node->_leftNode);
            }

            UpdateHeight(node);

            return Balance(node);
        }

        Node* Insert(const T& value) {
            if (_rootNode != nullptr) {
                return Insert(value, _rootNode);
            } else {
                _rootNode = new Node(value);
                _size++;
                return _rootNode;
            }
        }

        Node* Remove(const T& value, Node* node) {
            if (node == nullptr)
                return nullptr;
            
            if (value > node->_data) {
                node->_rightNode = Remove(value, node->_rightNode);
            } else if (value < node->_data) {
                node->_leftNode = Remove(value, node->_leftNode);
            } else {
                if (node->_leftNode == nullptr) {
                    return node->_rightNode;
                } else if (node->_rightNode == nullptr) {
                    return node->_leftNode;
                } else {
                    if (node->_rightNode->_height > node->_leftNode->_height) {
                        auto succesor = FindMin(node->_rightNode);
                        node->_data = succesor->_data;
                        node->_rightNode = Remove(succesor->_data, node->_rightNode);
                    } else {
                        auto succesor = FindMax(node->_leftNode);
                        node->_data = succesor->_data;
                        node->_leftNode = Remove(succesor->_data, node->_leftNode);
                    }
                }
                _size--;
            }

            UpdateHeight(node);
            
            return Balance(node);
        }

        Node* Remove(const T& value) {
            if (_rootNode != nullptr) {
                return Remove(value, _rootNode);
            } else {
                return nullptr;
            }
        }

        void UpdateHeight(Node* node) {
            int lh = (node->_leftNode == nullptr) ? -1 : node->_leftNode->_height;
            int rh = (node->_rightNode == nullptr) ? -1 : node->_rightNode->_height;

            node->_height = std::max(lh, rh) + 1;
        }

        int BalanceFactor(Node* node) const {
            int lh = (node->_leftNode == nullptr) ? -1 : node->_leftNode->_height;
            int rh = (node->_rightNode == nullptr) ? -1 : node->_rightNode->_height;

            return rh - lh;
        }

        Node* Balance(Node* node) {
            int bf = BalanceFactor(node);

            if (bf < -1) {
                if (BalanceFactor(node->_leftNode) < 0) {
                    return LeftLeftRotation(node);
                } else {
                    return LeftRightRotation(node);
                }
            } 
            if (bf > 1) {
                if (BalanceFactor(node->_rightNode) > 0) {
                    return RightRightRotation(node);
                } else {
                    return RightLeftRotation(node);
                }
            }

            return node;
        }

        Node* LeftLeftRotation(Node* node) {
            Node* tipNode = node->_leftNode;
            node->_leftNode = tipNode->_rightNode;
            tipNode->_rightNode = node;

            UpdateHeight(tipNode->_rightNode);
            UpdateHeight(tipNode);

            if (node == _rootNode)
                _rootNode = tipNode;

            return tipNode;
        }
        Node* LeftRightRotation(Node* node) {
            Node* tipNode = node->_leftNode->_rightNode;
            node->_leftNode->_rightNode = tipNode->_leftNode;
            tipNode->_leftNode = node->_leftNode;
            node->_leftNode = tipNode->_rightNode;
            tipNode->_rightNode = node;

            UpdateHeight(tipNode->_leftNode);
            UpdateHeight(tipNode->_rightNode);
            UpdateHeight(tipNode);

            if (node == _rootNode)
                _rootNode = tipNode;

            return tipNode;
        }
        Node* RightRightRotation(Node* node) {
            Node* tipNode = node->_rightNode;
            node->_rightNode = tipNode->_leftNode;
            tipNode->_leftNode = node;

            UpdateHeight(tipNode->_leftNode);
            UpdateHeight(tipNode);

            if (node == _rootNode)
                _rootNode = tipNode;

            return tipNode;
        }
        Node* RightLeftRotation(Node* node) {
            Node* tipNode = node->_rightNode->_leftNode;
            node->_rightNode->_leftNode = tipNode->_rightNode;
            tipNode->_rightNode = node->_rightNode;
            node->_rightNode = tipNode->_leftNode;
            tipNode->_leftNode = node;

            UpdateHeight(tipNode->_leftNode);
            UpdateHeight(tipNode->_rightNode);
            UpdateHeight(tipNode);

            if (node == _rootNode)
                _rootNode = tipNode;

            return tipNode;
        }

        void Clear(Node* node) {
            if (node == nullptr)
                return;
            
            Clear(node->_leftNode);
            Clear(node->_rightNode);
        }

        void Clear() {
            Clear(_rootNode);
        }

    };

    // template <typename T>
    // void PrintAVLTree(const AVLTree<T>& tree, int spaceWidth = 3) {
    //     int height = tree.Height();
    //     std::queue<typename AVLTree<T>::Node*> nodeQueue;
    //     nodeQueue.push(tree.Root());
    //     for (int i = height; i >= 0; i--) {
    //         int widthNodes = std::pow(2, i);
    //         int widthSpaces = widthNodes + (widthNodes-1)*spaceWidth;
    //         int leftMargin = ((widthSpaces-1)/2);
    //         int spaceBetNubers = leftMargin * 2 + spaceWidth;

    //         std::string buffer = "";
            
    //         for (int k = 0; k < leftMargin; k++) { //write left margin
    //             buffer += " ";
    //         }
            
    //         int q = nodeQueue.size();
    //         for (int j = 0; j < q; j++) {
    //             auto cNode = nodeQueue.front();
    //             nodeQueue.pop();

    //             if (cNode != nullptr) {
    //                 nodeQueue.push(cNode->Left());
    //                 nodeQueue.push(cNode->Right());
    //             }
                
    //             buffer += ((cNode != nullptr) ? std::to_string(cNode->Get()) : " ") ;

    //             for (int k = 0; k < spaceBetNubers; k++) {
    //                 buffer += " ";
    //             }
    //         }
    //         std::cout << buffer << "\n";
    //     }
    // }

    template <typename T>
    void PrintAVLTree(const AVLTree<T>& tree, int spaceWidth = 3) {
        int height = tree.Height();
        std::queue<typename AVLTree<T>::Node*> nodeQueue;
        nodeQueue.push(tree.Root());
        for (int i = height; i >= 0; i--) {
            std::string buffer = "";
            
            int q = nodeQueue.size();
            for (int j = 0; j < q; j++) {
                auto cNode = nodeQueue.front();
                nodeQueue.pop();

                if (cNode != nullptr) {
                    nodeQueue.push(cNode->Left());
                    nodeQueue.push(cNode->Right());
                } else {
                    nodeQueue.push(nullptr);
                    nodeQueue.push(nullptr);
                }
                
                buffer += ((cNode != nullptr) ? std::to_string(cNode->Get()) : "n") + " ";
            }
            std::cout << buffer << "\n";
        }
    }
}