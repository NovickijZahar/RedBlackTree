#pragma once
#include "redblacktree.h"

template<typename T>
RedBlackTree<T>::RedBlackTree()
{
	root = nullptr;
}

template<typename T>
RedBlackTree<T>::RedBlackTree(std::initializer_list<T> list)
{
	root = nullptr;
	for (auto l : list)
	{
		insert(l);
	}
}

template<typename T>
RedBlackTree<T>::RedBlackTree(RedBlackTree&& other) noexcept
{
	root = other.root;
	size_ = other.size_;
	other.root = nullptr;
	other.size_ = 0;
}

template<typename T>
RedBlackTree<T>::RedBlackTree(const RedBlackTree& other)
{
	size_ = other.size_;
	root = copyNode(other.root);
}

template<typename T>
RedBlackTree<T>::~RedBlackTree()
{
	clearNode(root);
	root = nullptr;
	size_ = 0;
}

template<typename T>
void RedBlackTree<T>::insert(T data)
{
	if (root == nullptr)
	{
		Node<T>* newNode = new Node<T>(data);
		root = newNode;
		root->is_black = false;
		Node<T>* l = new Node<T>(T(), 1);
		Node<T>* r = new Node<T>(T(), 1);
		l->parent = root;
		r->parent = root;
		root->left = l;
		root->right = r;
		insert1(newNode);
		size_ = 1;
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
				Node<T>* l = new Node<T>(T(), 1);
				Node<T>* r = new Node<T>(T(), 1);
				l->parent = cur->left;
				r->parent = cur->left;
				cur->left->left = l;
				cur->left->right = r;
				insert1(cur->left);
				size_++;
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
				Node<T>* l = new Node<T>(T(), 1);
				Node<T>* r = new Node<T>(T(), 1);
				l->parent = cur->right;
				r->parent = cur->right;
				cur->right->left = l;
				cur->right->right = r;
				insert1(cur->right);
				size_++;
				return;
			}
			cur = cur->right;
		}
	}
}

template<typename T>
template<typename ...Args>
void RedBlackTree<T>::emplace(Args&&...args)
{
	T elem = T(std::forward<Args>(args)...);
	insert(elem);
}

template<typename T>
void RedBlackTree<T>::remove(T value)
{
	Node<T>* node = get_node(value);
	if (root == node and root->left->is_leaf and root->right->is_leaf)
	{
		delete root->left;
		delete root->right;
		delete node;
		root = nullptr;
		size_ = 0;
	}
	else if (root == node and (root->left->is_leaf or root->right->is_leaf))
	{
		root = root->left->is_leaf ? root->right : root->left;
		delete root->parent;
		root->parent = nullptr;
		root->is_black = true;
		size_--;
	}
	else if (node->left->is_leaf or node->right->is_leaf)
	{
		remove0(node);
		size_--;
	}
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
		size_--;
	}
}

template<typename T>
void RedBlackTree<T>::swap(RedBlackTree& other)
{
	std::swap(size_, other.size_);
	std::swap(root, other.root);
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
	if (size_ == 0)
		std::cout << "{}";
	else 
	{
		std::cout << "{";
		dfs(root);
		std::cout << "\b\b}";
	}
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
size_t RedBlackTree<T>::size()
{
	return size_;
}

template<typename T>
void RedBlackTree<T>::dfs(Node<T>* node)
{
	if (node != nullptr)
	{
		dfs(node->left);
		if (node->is_leaf == false)
			std::cout << node->data << ", ";
		dfs(node->right);
	}
}

template<typename T>
bool RedBlackTree<T>::empty()
{
	return size_ == 0;
}

template<typename T>
void RedBlackTree<T>::clearNode(Node<T>* node)
{
	if (node != nullptr)
	{
		clearNode(node->left);
		clearNode(node->right);
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
void RedBlackTree<T>::clear()
{
	clearNode(root);
	root = nullptr;
	size_ = 0;
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
	throw "There is no such element";
}

template<typename T>
void RedBlackTree<T>::remove0(Node<T>* node)
{
	Node<T>* child = node->right->is_leaf ? node->left : node->right;
	delete (node->right->is_leaf ? node->right : node->left);
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

template<typename T>
Node<T>* RedBlackTree<T>::copyNode(Node<T>* node)
{
	if (node == nullptr)
		return nullptr;

	Node<T>* newNode = new Node<T>(node->data, node->is_leaf);
	newNode->is_black = node->is_black;
	newNode->parent = node->parent;
	newNode->left = copyNode(node->left);
	newNode->right = copyNode(node->right);

	return newNode;
}

template<typename T>
Iterator<T> RedBlackTree<T>::find(T value)
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
			return Iterator<T>(cur, root);
		}
	}
	throw "There is no such element";
}

template<typename T>
Iterator<T> RedBlackTree<T>::begin()
{
	Node<T>* current = root;
	while (current != nullptr && !current->left->is_leaf)
		current = current->left;
	return Iterator<T>(current, root);
}

template<typename T>
Iterator<T> RedBlackTree<T>::end()
{
	return Iterator<T>(nullptr, root);
}

template<typename T>
ReversedIterator<T> RedBlackTree<T>::crbegin()
{
	Node<T>* current = root;
	while (current != nullptr && !current->right->is_leaf)
		current = current->right;
	return ReversedIterator<T>(current, root);
}

template<typename T>
ReversedIterator<T> RedBlackTree<T>::crend()
{
	return ReversedIterator<T>(nullptr, root);
}

template<typename T>
Iterator<T>::Iterator(Node<T>* node, Node<T>* nodeRoot)
{
	current = node;
	root = nodeRoot;
}

template<typename T>
Iterator<T>& Iterator<T>::operator++()
{
	if (current == nullptr)
		return *this;
	if (!current->right->is_leaf)
	{
		current = current->right;
		while (!current->left->is_leaf)
		{
			current = current->left;
		}
	}
	else
	{
		Node<T>* parent = current->parent;
		while (parent != nullptr and current == parent->right)
		{
			current = parent;
			parent = parent->parent;
		}
		current = parent;
	}
	return *this;
}

template<typename T>
Iterator<T>& Iterator<T>::operator--()
{
	if (current == nullptr)
	{
		current = root;
		while (current != nullptr and !current->right->is_leaf)
			current = current->right;
	}
	else if (!current->left->is_leaf)
	{
		current = current->left;
		while (!current->right->is_leaf)
			current = current->right;
	}
	else
	{
		Node<T>* parent = current->parent;
		while (parent != nullptr and current == parent->left)
		{
			current = parent;
			parent = parent->parent;
		}
		current = parent;
	}
	return *this;
}

template<typename T>
Iterator<T>& Iterator<T>::operator++(int)
{
	Iterator<T> iter = *this;
	++(*this);
	return iter;
}

template<typename T>
Iterator<T>& Iterator<T>::operator--(int)
{
	Iterator iter = *this;
	--(*this);
	return iter;
}

template<typename T>
T& Iterator<T>::operator*() const
{
	return current->data;
}

template<typename T>
bool Iterator<T>::operator==(const Iterator<T>& other) const
{
	return current == other.current;
}

template<typename T>
bool Iterator<T>::operator!=(const Iterator<T>& other) const
{
	return current != other.current;
}

template<typename T>
ReversedIterator<T>::ReversedIterator(Node<T>* node, Node<T>* nodeRoot)
{
	current = node;
	root = nodeRoot;
}

template<typename T>
ReversedIterator<T>& ReversedIterator<T>::operator++()
{
	if (current == nullptr)
	{
		current = root;
		while (current != nullptr and !current->right->is_leaf)
			current = current->right;
	}
	else if (!current->left->is_leaf)
	{
		current = current->left;
		while (!current->right->is_leaf)
			current = current->right;
	}
	else
	{
		Node<T>* parent = current->parent;
		while (parent != nullptr and current == parent->left)
		{
			current = parent;
			parent = parent->parent;
		}
		current = parent;
	}
	return *this;
}

template<typename T>
ReversedIterator<T>& ReversedIterator<T>::operator--()
{
	if (current == nullptr)
		return *this;
	if (!current->right->is_leaf)
	{
		current = current->right;
		while (!current->left->is_leaf)
		{
			current = current->left;
		}
	}
	else
	{
		Node<T>* parent = current->parent;
		while (parent != nullptr and current == parent->right)
		{
			current = parent;
			parent = parent->parent;
		}
		current = parent;
	}
	return *this;
}

template<typename T>
ReversedIterator<T>& ReversedIterator<T>::operator++(int)

{
	ReversedIterator<T> iter = *this;
	++(*this);
	return iter;
}

template<typename T>
ReversedIterator<T>& ReversedIterator<T>::operator--(int)
{
	ReversedIterator iter = *this;
	--(*this);
	return iter;
}

template<typename T>
T& ReversedIterator<T>::operator*() const
{
	return current->data;
}

template<typename T>
bool ReversedIterator<T>::operator == (const ReversedIterator<T>&other) const
{
	return current == other.current;
}

template<typename T>
bool ReversedIterator<T>::operator!=(const ReversedIterator<T>& other) const
{
	return current != other.current;
}