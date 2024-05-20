#pragma once

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
class RedBlackTree
{
public:
	RedBlackTree();
	~RedBlackTree();
	void insert(T data);
	void remove(T data);
	void print();
	Node<T>* get_node(T value);
	void replace_node(Node<T>* node, Node<T>* newNode);
private:
	Node<T>* root;
	Node<T>* uncle(Node<T>* node);
	Node<T>* grandparent(Node<T>* node);
	Node<T>* sibling(Node<T>* node);
	void rotate_left(Node<T>* node);
	void rotate_right(Node<T>* node);
	void insert1(Node<T>* node);
	void insert2(Node<T>* node);
	void insert3(Node<T>* node);
	void insert4(Node<T>* node);
	void insert5(Node<T>* node);
	void dfs(Node<T>* node);
	void clear(Node<T>* node);
	void remove0(Node<T>* node);
	void remove1(Node<T>* node);
	void remove2(Node<T>* node);
	void remove3(Node<T>* node);
	void remove4(Node<T>* node);
	void remove5(Node<T>* node);
	void remove6(Node<T>* node);
};

#include "redblacktree.hpp"
