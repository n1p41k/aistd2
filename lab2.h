#pragma once

#include <vector>


class BinaryTree
{
public:
	class Node {
	public:
		Node();
		Node(int key = 0, Node* leftChild = nullptr, Node* rightChild = nullptr);
		~Node() = default;

		int key() const;
		void setKey(const int key);

		Node* leftChild() const;
		Node* rightChild() const;

		void setLeftChild(Node* leftChild);
		void setRightChild(Node* rightChild);

	private:
		int m_key = 0;
		Node* m_leftChild = nullptr;
		Node* m_rightChild = nullptr;
	};

	BinaryTree() = default;
	BinaryTree(Node* root);
	BinaryTree(const BinaryTree& other);
	BinaryTree(BinaryTree&& other);
	virtual ~BinaryTree();

	Node* root() const;


	void clear();
	void removeSubtrees();

	bool isEmpty() const;

	int height() const;
	int nodeHeightIndex(int key) const;
	int indexNode(int key) const;

	int countNodes() const;

	int maxKey() const;
	int minKey() const;

	virtual Node* addKey(int key);
	Node* findKey(int key) const;

	bool removeKey(int key);
	Node* searchParent(Node* root, Node* node) const;

	bool contains(int key) const;
	std::vector<int> getVectorKeys() const;

	void levelPrint() const;
	void printTreeHorizontal(int marginLeft, int levelSpacing) const;

	bool isBalanced() const;

	BinaryTree& operator=(const BinaryTree& other);
	BinaryTree& operator=(BinaryTree&& other);

	void setRoot(Node* root);

protected:
	virtual int nodeHeightIndex(Node* root, int key, int level) const;

	virtual int maxKey(Node* node) const;
	virtual int minKey(Node* node) const;

	virtual Node* addKey(Node* root, int key);

	virtual bool removeKey(Node* root, int key);

	virtual std::vector<int> getVectorKeys(Node* root, std::vector<int>& keys) const;

	virtual Node* findKey(Node* root, int key) const;
private:
	Node* m_root = nullptr;

private:
	Node* copyTree(Node* root);

	void clear(Node* node);
	void removeSubtrees(Node* node);

	int height(Node* root) const;
	int indexNode(Node* root, int key) const;

	int countNodes(Node* node) const;
	bool contains(Node* root, int key) const;

	void levelPrint(Node* root) const;
	void printHorizontal(Node* root, int marginLeft, int levelSpacing) const;

	bool isBalanced(Node* root) const;
};

