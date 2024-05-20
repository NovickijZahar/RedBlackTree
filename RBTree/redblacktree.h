#pragma once
#include <iostream>

template <typename T>
struct Node
{
	T data;
	bool is_black;
	Node<T>* parent;
	Node<T>* left;
	Node<T>* right;
	bool is_leaf;
	Node(T value, bool leaf=0) :
		data(value), is_black(true), parent(nullptr), left(nullptr), right(nullptr), is_leaf(leaf) {};
};

template <typename T>
class Iterator
{
public:
	Iterator(Node<T>* node, Node<T>* nodeRoot);
	Iterator<T>& operator++();
	Iterator<T>& operator--();
	Iterator<T>& operator++(int);
	Iterator<T>& operator--(int);
	T& operator*() const;
	bool operator==(const Iterator<T>& other) const;
	bool operator!=(const Iterator<T>& other) const;
private:
	Node<T>* current;
	Node<T>* root;
};

template<typename T>
class ReversedIterator
{
public:
	ReversedIterator(Node<T>* node, Node<T>* nodeRoot);
	ReversedIterator<T>& operator++();
	ReversedIterator<T>& operator--();
	ReversedIterator<T>& operator++(int);
	ReversedIterator<T>& operator--(int);
	T& operator*() const;
	bool operator==(const ReversedIterator<T>& other) const;
	bool operator!=(const ReversedIterator<T>& other) const;
private:
	Node<T>* current;
	Node<T>* root;
};


template <typename T>
class RedBlackTree
{
public:
	RedBlackTree();
	RedBlackTree(std::initializer_list<T> list);
	RedBlackTree(RedBlackTree&& other) noexcept;
	RedBlackTree(const RedBlackTree& other);
	~RedBlackTree();
	void insert(T data);
	void remove(T data);
	void swap(RedBlackTree& other);
	template<typename...Args>
	void emplace(Args&&...args);
	void print();
	void replace_node(Node<T>* node, Node<T>* newNode);
	size_t size();
	bool empty();
	void clear();
	Iterator<T> begin();
	Iterator<T> end();
	ReversedIterator<T> crbegin();
	ReversedIterator<T> crend();
	Iterator<T> find(T value);
private:
	Node<T>* root;
	size_t size_;
	Node<T>* uncle(Node<T>* node);
	Node<T>* grandparent(Node<T>* node);
	Node<T>* sibling(Node<T>* node);
	Node<T>* get_node(T value);
	void rotate_left(Node<T>* node);
	void rotate_right(Node<T>* node);
	void insert1(Node<T>* node);
	void insert2(Node<T>* node);
	void insert3(Node<T>* node);
	void insert4(Node<T>* node);
	void insert5(Node<T>* node);
	void dfs(Node<T>* node);
	void clearNode(Node<T>* node);
	void remove0(Node<T>* node);
	void remove1(Node<T>* node);
	void remove2(Node<T>* node);
	void remove3(Node<T>* node);
	void remove4(Node<T>* node);
	void remove5(Node<T>* node);
	void remove6(Node<T>* node);
	Node<T>* copyNode(Node<T>* node);
};

#include "redblacktree.hpp"
