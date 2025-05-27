#include <iostream>
#include <compare>
#include <algorithm>
#include <memory>
#include <queue>
#include <stack>
#include <functional>
#include "BST.hpp"



template <typename T, typename F>
void printNode(const T &key, const F &val) { std::cout << "( Key: " << key << ", Value:" << val << ")" << std::endl; }

int main()
{

    MyMap<int, int> tree;
    /* BST<int, int> tree; */
    tree.insert(7, 4);
    tree.insert(4, 2);
    tree.insert(3, 1);
    tree.insert(5, 3);
    tree.insert(10, 6);
    tree.insert(9, 5);
    tree.insert(11, 7);

    for (auto i = tree.begin(); i != tree.end(); i++)
    {
        std::cout << *i << std::endl;
    }

    tree.preoder(printNode<int, int>);
    std::cout << "______________________\n";
    tree.inorder(printNode<int, int>);
    std::cout << "______________________\n";
    tree.postorder(printNode<int, int>);
    

    return 0;
}