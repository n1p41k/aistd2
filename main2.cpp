#include "lab2.h"
#include <iostream>
#include <vector>

int main() {

    std::cout << "=== BinaryTree Class Demonstration ===\n\n";

    // 1. Create an empty tree
    BinaryTree tree;
    std::cout << "Created an empty tree.\n";
    tree.levelPrint();
    // 2. Add keys
    std::cout << "\nAdding keys: 50, 30, 70, 20, 40, 60, 80\n";
    tree.addKey(50);
    tree.addKey(30);
    tree.addKey(70);
    tree.addKey(20);
    tree.addKey(40);
    tree.addKey(60);
    tree.addKey(80);
    std::cout << "Tree after insertion (horizontal print):\n";
    tree.printTreeHorizontal(2, 4);
    std::cout << "\nLevel-order print:\n";
    tree.levelPrint();


    // 3. Tree properties
    std::cout << "\nTree properties:\n";
    std::cout << "Number of nodes: " << tree.countNodes() << "\n";
    std::cout << "Height: " << tree.height() << "\n";
    std::cout << "Minimum key: " << tree.minKey() << "\n";
    std::cout << "Maximum key: " << tree.maxKey() << "\n";
    std::cout << "Is balanced? " << (tree.isBalanced() ? "Yes" : "No") << "\n";

    // 4. Key search
    int key = 39;
    BinaryTree::Node* found = tree.findKey(key);
    std::cout << "\nSearching for key " << key << ": " << (found ? "found" : "not found") << "\n";
    if (found) {
        std::cout << "Node index (breadth-first order): " << tree.indexNode(key) << "\n";
        std::cout << "Node level (height index): " << tree.nodeHeightIndex(key) << "\n";
    }

    key = 99;
    std::cout << "Searching for key " << key << ": " << (tree.contains(key) ? "found" : "not found") << "\n";

    // 5. Get all keys as a sorted vector
    std::vector<int> keys = tree.getVectorKeys();
    std::cout << "\nAll keys (sorted): ";
    for (int k : keys) std::cout << k << " ";
    std::cout << "\n";

    // 6. Copy tree (copy constructor)
    BinaryTree treeCopy(tree);
    std::cout << "\nCreated a copy of the tree (copy constructor).\n";
    std::cout << "Copy (horizontal):\n";
    treeCopy.printTreeHorizontal(2, 4);

    // 7. Copy assignment
    BinaryTree treeAssign;
    treeAssign = tree;
    std::cout << "\nCopy assignment:\n";
    std::cout << "New tree (level-order):\n";
    treeAssign.levelPrint();

    // 8. Key removal
    std::cout << "\nTesting removal\n";
    std::cout << "Removing leaf (20):\n";
    tree.removeKey(20);
    tree.printTreeHorizontal(2, 4);
    std::cout << "Number of nodes after removal: " << tree.countNodes() << "\n";

    std::cout << "\nRemoving node with one child (30):\n";
    tree.removeKey(30);
    tree.printTreeHorizontal(2, 4);
    std::cout << "Number of nodes after removal: " << tree.countNodes() << "\n";

    return 0;
}
