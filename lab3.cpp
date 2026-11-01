#include "lab3.h"

BinarySearchTree::BinarySearchTree(Node* node)
{
	setRoot(node);
}


BinarySearchTree::Node* BinarySearchTree::findKey(int key) const
{
	return findKey(root(), key);
}

BinarySearchTree::Node* BinarySearchTree::addKey(Node* root, int key)
{
	if (!root) {
		root = new Node(key);
	}
	else if (key < root->key()) {
		root->setLeftChild(addKey(root->leftChild(), key));
	}
	else if (key > root->key()) {
		root->setRightChild(addKey(root->rightChild(), key));
	}
	return root;
}

bool BinarySearchTree::removeKey(Node* root, int key)
{
	Node* node = findKey(key);
	Node* nodeParent = searchParent(root, node);
	if (node == nullptr)
		return false;

	//нет детей
	if (node->leftChild() == nullptr && node->rightChild() == nullptr)
	{
		if (nodeParent != nullptr)
		{
			if (nodeParent->leftChild() == node)
				nodeParent->setLeftChild(nullptr);
			else
				nodeParent->setRightChild(nullptr);
			delete node;
			node = nullptr;
		}
		else
		{
			delete node;
			setRoot(nullptr);
		}
	}
	//один ребёнок
	else if ((node->leftChild() != nullptr && node->rightChild() == nullptr) || (node->leftChild() == nullptr && node->rightChild() != nullptr))
	{
		if (nodeParent != nullptr)
		{
			if (nodeParent->leftChild() == node)
			{
				if (node->leftChild() != nullptr)
					nodeParent->setLeftChild(node->leftChild());
				else
					nodeParent->setLeftChild(node->rightChild());
				delete node;
			}
			else
			{
				if (node->leftChild() != nullptr)
					nodeParent->setRightChild(node->leftChild());
				else
					nodeParent->setRightChild(node->rightChild());
				delete node;
			}
		}
		else
		{
			if (node->leftChild() != nullptr)
				setRoot(node->leftChild());
			else
				setRoot(node->rightChild());
			delete node;
		}
	}
	//два ребёнка
	else
	{
		Node* replacementNode, * replacementNodeParent;
		replacementNode = node->rightChild();
		replacementNodeParent = node;

		while (replacementNode->leftChild() != nullptr)
		{
			replacementNodeParent = replacementNode;
			replacementNode = replacementNode->leftChild();
		}

		if (replacementNodeParent != node)
		{
			replacementNodeParent->setLeftChild(replacementNode->rightChild());
			replacementNode->setRightChild(node->rightChild());
		}
		replacementNode->setLeftChild(node->leftChild());

		if (nodeParent != nullptr)
		{
			if (nodeParent->rightChild() == node)
			{
				nodeParent->setRightChild(replacementNode);
			}
			else
			{
				nodeParent->setLeftChild(replacementNode);
			}
		}
		else
		{
			setRoot(replacementNode);
		}
		delete node;

	}
	return true;
}

BinarySearchTree::Node* BinarySearchTree::findKey(Node* root, int key) const
{
	if (root == nullptr)
		return nullptr;

	if (root->key() == key)
		return root;

	if (key < root->key())
		findKey(root->leftChild(), key);
	else
		findKey(root->rightChild(), key);
}

int BinarySearchTree::minKey(Node* root) const
{
	if (root == nullptr)
		return INT_MAX;

	Node* nodeMin = root;

	while (nodeMin->leftChild() != nullptr)
	{
		nodeMin = nodeMin->leftChild();
	}
	return nodeMin->key();
}

int BinarySearchTree::maxKey(Node* root) const
{
	if (root == nullptr)
		return INT_MIN;

	Node* nodeMax = root;

	while (nodeMax->rightChild() != nullptr)
	{
		nodeMax = nodeMax->rightChild();
	}

	return nodeMax->key();
}

int BinarySearchTree::nodeHeightIndex(Node* node, int key, int level) const
{
	if (node == nullptr)
		return -1;
	if (key < node->key())
		level = nodeHeightIndex(node->leftChild(), key, level + 1);
	else if (key > node->key())
		level = nodeHeightIndex(node->rightChild(), key, level + 1);

	return level;
}

std::vector<int> BinarySearchTree::getVectorKeys(Node* root, std::vector<int>& keys) const
{
	if (!root) {
		return keys;
	}
	getVectorKeys(root->leftChild(), keys);
	keys.push_back(root->key());
	getVectorKeys(root->rightChild(), keys);

	return keys;
}
