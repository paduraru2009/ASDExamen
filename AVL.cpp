// https://courses.cs.washington.edu/courses/cse373/06sp/handouts/lecture12.pdf
// https://kukuruku.co/post/avl-trees/

#include <stdio.h>
#include <vector>
#include <cstdlib>
#include "profiler.h"
#include <set>
#include "AVL_better.h"
#include "BinaryTree.h"

#define max(a,b) (a > b ? a : b)

struct Node
{
	int key;
	void* externalData;
	Node* left;
	Node* right;
	unsigned char height;

	Node(const int k, void* data) 
	{ 
		key = k; left = right = nullptr; 
		height = 1; externalData = data;
	}
};

// Returns the height of a given node
inline unsigned char height(const Node* x) 
{ 
	return x ? x->height : -1; 
}

// Updates the height of a given node
inline void updateHeight(Node *x)
{
	const unsigned char hLeft = height(x->left);
	const unsigned char hRight = height(x->right);
	x->height = 1 + max(hLeft, hRight);
}

// Returns the balance factor of a given node
// 0 perfect balanced
// -2 if unbalanced in left side
// +2 if unbalanced in right side
inline int balanceFactor(const Node* x)
{
	return height(x->right) - height(x->left);
}

// Right rotate X child, 
// which is a left child of y
// Returns the root of subtree after rotation
inline Node* rotateRight(Node* y)
{
	Node* x = y->left;
	y->left = x->right;
	x->right = y;
	updateHeight(x);
	updateHeight(y);
	return x;
}

// Right rotate X child, 
// which is a left child of y
// Returns the root of subtree after rotation
inline Node* rotateLeft(Node* x)
{
	Node* y = x->right;
	x->right = y->left;
	y->left = x;
	updateHeight(x);
	updateHeight(y);
	return y;
}

inline Node* balance(Node* x)
{
	updateHeight(x);
	// Symmetric cases. Right then left 
	if (balanceFactor(x) == 2)
	{
		if (balanceFactor(x->right) < 0)
		{
			x->right = rotateRight(x->right);
		}
		return rotateLeft(x);
	}
	if (balanceFactor(x) == -2)
	{
		if (balanceFactor(x->left) > 0)
		{
			x->left = rotateLeft(x->left);
		}
		return rotateRight(x);
	}
	return x; // x is balanced now
}

// Insert in tree with root x
// a node with key k and some external data
inline Node* insert(Node* x, int k, void* data) 
{
	if (!x) return new Node(k, data);
	if (k < x->key)
		x->left = insert(x->left, k, data);
	else
		x->right = insert(x->right, k, data);

	return balance(x);
}

void printTree(Node* root)
{
	if (!root)
		return;

	printTree(root->left);
	printf("%d ", root->key);
	printTree(root->right);
}

// Search and return the node with the minimal key in tree x
Node* findMin(Node* x) 
{
	return x->left ? findMin(x->left) : x;
}

// Removes the node with the minimal key from tree x
// Return its root
Node* removeMin(Node* x) 
{
	if (x->left == 0)
		return x->right;
	x->left = removeMin(x->left);
	return balance(x);
}

// Remove the key with key k from tree x
Node* remove(Node* x, int k) 
{
	if (!x) return 0;
	if (k < x->key)
	{
		x->left = remove(x->left, k);
	}
	else if (k > x->key)
	{
		x->right = remove(x->right, k);
	}
	else //  k == x->key 
	{
		Node* xLeft  = x->left;
		Node* xRight = x->right;
		delete x;
		if (!xRight) 
			return xLeft;

		Node* min = findMin(xRight);
		min->right = removeMin(xRight);
		min->left = xLeft;
		return balance(min);
	}

	return balance(x);
}

void unitTest()
{
	Node* tree = nullptr;
	int sequence[] = { 60, 20, 55, 10, 5, 18,  50, 4, 45, };
	for (int i = 0; i < sizeof(sequence) / sizeof(sequence[0]); i++)
		tree = insert(tree, sequence[i], nullptr);

	tree = remove(tree, 60);
	tree = remove(tree, 4);
	tree = remove(tree, 10);

	printTree(tree);
}

#define SEED_TO_USE 0
#define NUM_TESTS 5
const int numItemsToInsert[NUM_TESTS] = { 1 << 20, 1 << 21,  1 << 22};

int main()
{
	for (int testId = 0; testId < NUM_TESTS; testId++)
	{
		const int itemsToInsert = numItemsToInsert[testId];

		PROFILE_BLOCK("Binary insert");
		//Node* tree = nullptr;
		btree bt;
		for (int i = 0; i < itemsToInsert; i++)
		{
			bt.insert(std::rand());
		}
		/*
		std::srand(SEED_TO_USE);
		{
			PROFILE_BLOCK("AVL insert");
			//Node* tree = nullptr;
			AVLTree<int> tree(0);
			for (int i = 0; i < itemsToInsert; i++)
			{
				tree.Add(std::rand());
				//tree = insert(tree, std::rand(), nullptr);
			}
		}

		std::srand(SEED_TO_USE);
		{
			PROFILE_BLOCK("RBT insert");
			std::set<int> rbTree;
			for (int i = 0; i < itemsToInsert; i++)
			{
				rbTree.insert(std::rand());
			}
		}
		*/
	}

	return 0;
}
