#ifndef _H_AVL_TREE
#define _H_AVL_TREE

#include <iostream>
#include <iomanip>

template <typename T>
struct AVLNode
{
	AVLNode(T v, int b = 0, AVLNode<T> *l = 0, AVLNode<T> *r = 0) :
		value(v),
		balance(b),

		left(l),
		right(r)
	{}

	T value;
	int balance;

	AVLNode<T> *left;
	AVLNode<T> *right;

	void Print(int offset = 0)
	{
		using namespace std;

		if (right) right->Print(5 + offset);
		cout << setw(offset) << value << "(" << balance << ")" << endl;
		if (left) left->Print(5 + offset);
	}
};

template <typename T>
class AVLTree
{
private:
	AVLNode<T> *root;

public:
	AVLTree(T root_key) :
		root(new AVLNode<T>(root_key, 0))
	{}
	~AVLTree()
	{
		Delete(root);
	}

	void Print()
	{
		root->Print();
	}

	bool Add(T item)
	{
		return Add(item, root);
	}

	AVLNode<T>* Find(T item)
	{
		return Find(item, root);
	}

	bool Remove(T item)
	{
		return Remove(item, root);
	}

	T Max()
	{
		return Max(root);
	}

	T Min()
	{
		return Min(root);
	}

private:

	void Delete(AVLNode<T> *&node)
	{
		if (node != NULL)
		{
			Delete(node->left);
			Delete(node->right);

			delete node;
			node = NULL;
		}
	}

	void RotateRight(AVLNode<T> *&node)
	{
		AVLNode<T> *x = node;
		AVLNode<T> *y = node->left;

		x->left = y->right;
		y->right = x;
		node = y;

		x->balance += 1;
		if (y->balance < 0) x->balance += -y->balance;

		y->balance += 1;
		if (x->balance > 0) y->balance += x->balance;
	}

	void RotateLeft(AVLNode<T> *&node)
	{
		AVLNode<T> *x = node;
		AVLNode<T> *y = node->right;

		x->right = y->left;
		y->left = x;
		node = y;

		x->balance -= 1;
		if (y->balance > 0) x->balance -= y->balance;

		y->balance -= 1;
		if (x->balance < 0) y->balance -= -x->balance;
	}

	bool IncrementBalance(AVLNode<T> *&node)
	{
		node->balance++;

		if (node->balance == 0)
			return false;

		if (node->balance == 1)
			return true;

		if (node->balance == 2)
		{
			if (node->right->balance == -1)
				RotateRight(node->right);

			RotateLeft(node);

			return false;
		}

		// ERROR: initially non-balanced node
		return true;
	}

	bool DecrementBalance(AVLNode<T> *&node)
	{
		node->balance--;

		if (node->balance == 0)
			return false;

		if (node->balance == -1)
			return true;

		if (node->balance == -2)
		{
			if (node->left->balance == 1)
				RotateLeft(node->left);

			RotateRight(node);

			return false;
		}

		// ERROR: initially non-balanced node
		return true;
	}

	bool Add(T item, AVLNode<T> *&node)
	{
		if (node == NULL)
		{
			node = new AVLNode<T>(item);
			return true;
		}
		else if (item > node->value)
		{
			return Add(item, node->right) && IncrementBalance(node);
		}
		else if (item < node->value)
		{
			return Add(item, node->left) && DecrementBalance(node);
		}

		return false; // no need to add already existing node
	}

	AVLNode<T>* Find(T item, AVLNode<T>* node)
	{
		if (node == NULL)
			return NULL;

		if (item > node->value)
			return Find(item, node->right);
		else if (item < node->value)
			return Find(item, node->left);
		else
			return node;
	}

	bool Remove(T item, AVLNode<T> *&node)
	{
		if (node == NULL)
		{
			return false;
		}
		else if (node->value > item)
		{
			return Remove(item, node->left) && IncrementBalance(node);
		}
		else if (node->value < item)
		{
			return Remove(item, node->right) && DecrementBalance(node);
		}
		else // if (node->value == item)
		{
			if (node->left == NULL && node->right == NULL)
			{
				Delete(node);
			}
			else if (node->left != NULL && node->right != NULL)
			{
				// NOTE: "node" is reference to pointer, that can be
				//  changed during new remove. It is necessary to
				//  create another local pointer that cannot be changed
				//  form outside, to point current node.
				AVLNode<T> *current_node = node;
				T min_node_value = Min(node->right);

				bool isHeightChanged = Remove(min_node_value, node);
				current_node->value = min_node_value;

				return isHeightChanged;
			}
			else
			{
				AVLNode<T> *node_to_remove = node;

				if (node->left == NULL)
				{
					node = node_to_remove->right;
					node_to_remove->right = NULL;
				}
				else
				{
					node = node_to_remove->left;
					node_to_remove->left = NULL;
				}

				Delete(node_to_remove);
			}

			return true;
		}
	}

	T Max(const AVLNode<T> *const node)
	{
		return node->right == NULL ? node->value : Max(node->right);
	}

	T Min(const AVLNode<T> *const node)
	{
		return node->left == NULL ? node->value : Min(node->left);
	}
};

#endif