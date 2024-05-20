#pragma once
#include "redblacktree.h"
#include <iostream>

template<typename T>
RedBlackTree<T>::RedBlackTree()
{
	root = nullptr;
}

template<typename T>
RedBlackTree<T>::~RedBlackTree()
{
	clear(root);
}

template<typename T>
void RedBlackTree<T>::insert(T data)
{
	if (root == nullptr)
	{
		Node<T>* newNode = new Node<T>(data);
		root = newNode;
		root->is_black = false;
		Node<T>* l = new Node<T>(NULL, 1);
		Node<T>* r = new Node<T>(NULL, 1);
		l->parent = root;
		r->parent = root;
		root->left = l;
		root->right = r;
		insert1(newNode);
		return;
	}
	Node<T>* cur = root;
	while (true)
	{
		if (data <= cur->data)
		{
			if (cur->left->is_leaf)
			{
				cur->left->data = data;
				cur->left->is_black = false;
				cur->left->is_leaf = false;
				Node<T>* l = new Node<T>(NULL, 1);
				Node<T>* r = new Node<T>(NULL, 1);
				l->parent = cur->left;
				r->parent = cur->left;
				cur->left->left = l;
				cur->left->right = r;
				insert1(cur->left);
				return;
			}
			cur = cur->left;
		}
		else
		{
			if (cur->right->is_leaf)
			{
				cur->right->data = data;
				cur->right->is_black = false;
				cur->right->is_leaf = false;
				Node<T>* l = new Node<T>(NULL, 1);
				Node<T>* r = new Node<T>(NULL, 1);
				l->parent = cur->right;
				r->parent = cur->right;
				cur->right->left = l;
				cur->right->right = r;
				insert1(cur->right);
				return;
			}
			cur = cur->right;
		}
	}
}

template<typename T>
void RedBlackTree<T>::remove(T value)
{
	Node<T>* node = get_node(value);
	if (node->left->is_leaf or node->right->is_leaf)
		remove0(node);
	else
	{
		Node<T>* cur = node->right;
		while(!cur->left->is_leaf)
		{
			cur = cur->left;
		}
		T temp1 = cur->data;
		bool temp2 = cur->is_black;
		cur->data = node->data;
		cur->is_black = node->is_black;
		node->data = temp1;
		cur->is_black = temp2;
		remove0(cur);
	}
}

template<typename T>
Node<T>* RedBlackTree<T>::uncle(Node<T>* node)
{
	Node<T>* grand = grandparent(node);
	if (grand == nullptr)
		return nullptr;
	if (node->parent == grand->left)
		return grand->right;
	return grand->left;
}

template<typename T>
void RedBlackTree<T>::print()
{
	dfs(root);
}

template<typename T>
Node<T>* RedBlackTree<T>::grandparent(Node<T>* node)
{
	if (node != nullptr and node->parent != nullptr)
		return node->parent->parent;
	return nullptr;
}

template<typename T>
Node<T>* RedBlackTree<T>::sibling(Node<T>* node)
{
	if (node == node->parent->left)
		return node->parent->right;
	else
		return node->parent->left;
}

template<typename T>
void RedBlackTree<T>::rotate_left(Node<T>* node)
{
	Node<T>* pivot = node->right;
	pivot->parent = node->parent;
	if (node->parent != nullptr)
	{
		if (node->parent->left == node)
			node->parent->left = pivot;
		else
			node->parent->right = pivot;
	}
	else
	{
		root = pivot;
	}

	node->right = pivot->left;
	if (pivot->left != nullptr)
		pivot->left->parent = node;
	node->parent = pivot;
	pivot->left = node;
}

template<typename T>
void RedBlackTree<T>::rotate_right(Node<T>* node)
{
	Node<T>* pivot = node->left;
	pivot->parent = node->parent;

	pivot->parent = node->parent;
	if (node->parent != nullptr)
	{
		if (node->parent->left == node)
			node->parent->left = pivot;
		else
			node->parent->right = pivot;
	}
	else
	{
		root = pivot;
	}

	node->left = pivot->right;
	if (pivot->right != nullptr)
		pivot->right->parent = node;
	node->parent = pivot;
	pivot->right = node;
}

template<typename T>
void RedBlackTree<T>::insert1(Node<T>* node)
{
	if (node->parent == nullptr)
		node->is_black = true;
	else
		insert2(node);
}


template<typename T>
void RedBlackTree<T>::insert2(Node<T>* node)
{
	if (node->parent->is_black == true)
		return;
	else
		insert3(node);
}

template<typename T>
void RedBlackTree<T>::insert3(Node<T>* node)
{
	Node<T>* unc = uncle(node);
	if (unc != nullptr and unc->is_black == false)
	{
		node->parent->is_black = true;
		unc->is_black = true;
		Node<T>* grand = grandparent(node);
		grand->is_black = false;
		insert1(grand);
	}
	else
	{
		insert4(node);
	}
}

template<typename T>
void RedBlackTree<T>::insert4(Node<T>* node)
{
	Node<T>* grand = grandparent(node);

	if (node == node->parent->right and node->parent == grand->left)
	{
		rotate_left(node->parent);
		node = node->left;
	}
	else if (node == node->parent->left and node->parent == grand->right)
	{
		rotate_right(node->parent);
		node = node->right;
	}
	insert5(node);
}

template<typename T>
void RedBlackTree<T>::insert5(Node<T>* node)
{
	Node<T>* grand = grandparent(node);
	node->parent->is_black = true;
	grand->is_black = false;
	if (node == node->parent->left and node->parent == grand->left)
		rotate_right(grand);
	else
		rotate_left(grand);
}

template<typename T>
void RedBlackTree<T>::dfs(Node<T>* node)
{
	if (node != nullptr)
	{
		dfs(node->left);
		if (node->is_leaf == false)
			std::cout << node->data << ' ';
		dfs(node->right);
	}
}

template<typename T>
void RedBlackTree<T>::clear(Node<T>* node)
{
	if (node != nullptr)
	{
		clear(node->left);
		clear(node->right);
		delete node;
	}
}

template<typename T>
void RedBlackTree<T>::replace_node(Node<T>* node, Node<T>* newNode)
{
	newNode->parent = node->parent;
	if (node == node->parent->left)
		node->parent->left = newNode;
	else
		node->parent->right = newNode;
}

template<typename T>
Node<T>* RedBlackTree<T>::get_node(T value)
{
	Node<T>* cur = root;
	while (cur != nullptr)
	{
		if (value > cur->data)
		{
			cur = cur->right;
		}
		else if (value < cur->data)
		{
			cur = cur->left;
		}
		else
		{
			return cur;
		}
	}
	return cur;
}

template<typename T>
void RedBlackTree<T>::remove0(Node<T>* node)
{
	Node<T>* child = node->right->is_leaf ? node->left : node->right;
	replace_node(node, child);
	if (node->is_black)
	{
		if (!child->is_black)
		{
			child->is_black = true;
		}
		else
			remove1(child);
	}
	delete node;
}

template<typename T>
void RedBlackTree<T>::remove1(Node<T>* node)
{
	if (node->parent != nullptr)
		remove2(node);
}

template<typename T>
void RedBlackTree<T>::remove2(Node<T>* node)
{
	Node<T>* sib = sibling(node);
	if (!sib->is_black)
	{
		node->parent->is_black = false;
		sib->is_black = true;
		if (node == node->parent->left)
			rotate_left(node->parent);
		else
			rotate_right(node->parent);
	}
	remove3(node);
}

template<typename T>
void RedBlackTree<T>::remove3(Node<T>* node)
{
	Node<T>* sib = sibling(node);
	if (node->parent->is_black and sib->is_black and sib->left->is_black and sib->right->is_black)
	{
		sib->is_black = false;
		remove1(node->parent);
	}
	else
		remove4(node);
}

template<typename T>
void RedBlackTree<T>::remove4(Node<T>* node)
{
	Node<T>* sib = sibling(node);
	if (!node->parent->is_black and sib->is_black and sib->left->is_black and sib->right->is_black)
	{
		sib->is_black = false;
		node->parent->is_black = true;
	}
	else
		remove5(node);
}

template<typename T>
void RedBlackTree<T>::remove5(Node<T>* node)
{
	Node<T>* sib = sibling(node);
	if (sib->is_black)
	{
		if (node == node->parent->left and sib->right->is_black and !sib->left->is_black)
		{
			sib->is_black = false;
			sib->left->is_black = true;
			rotate_right(sib);
		}
		else if (node == node->parent->right and sib->left->is_black and !sib->right->is_black)
		{
			sib->is_black = false;
			sib->right->is_black = true;
			rotate_left(sib);
		}
	}
	remove6(node);
}

template<typename T>
void RedBlackTree<T>::remove6(Node<T>* node)
{
	Node<T>* sib = sibling(node);
	sib->is_black = node->parent->is_black;
	node->parent->is_black = true;
	if (node == node->parent->left)
	{
		sib->right->is_black = true;
		rotate_left(node->parent);
	}
	else
	{
		sib->left->is_black = true;
		rotate_right(node->parent);
	}
}