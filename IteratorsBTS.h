#pragma once
#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <iterator>  
using namespace std;
template <typename T>
class Tree;

//===================����� ��������� ��������� ������ ������===================
template <typename T>
class MyIterator :std::iterator<std::input_iterator_tag, Tree<T>>
{
	Tree<T>*p;
	
public:

	MyIterator(Tree<T>*pp) { p = pp; }

	template <typename TT>
	friend bool operator!=(MyIterator<TT>t1, MyIterator<TT>t2)
	{
		return t1.p != t2.p;
	}

	T & operator*()
	{
		return *(p->getData());
	}

	MyIterator<T> operator++()
	{
		if (p->root != nullptr)
		{
			auto R = p->root;
			if (p->root->right != nullptr)
				if (p->root->right->left != nullptr) {
					p->root = p->root->right;
					while (p->root->left != nullptr && p->root->left->data > R->data)
						p->root = p->root->left;
					if (p->root->left == nullptr || p->root->left->data < R->data)
						return MyIterator(p);
				}
				else
					p->root = p->root->right;
			else if (p->root->parent != nullptr && p->root->parent->data > p->root->data)
				p->root = p->root->parent;
			else {
				while (p->root->parent != nullptr && p->root->parent->data < p->root->data)
					p->root = p->root->parent;
				p->root = p->root->parent;
				if (p->root != nullptr && p->root->data > R->data)
					return MyIterator(p);
				if (p->root != nullptr && p->root->right != nullptr)
					p->root = p->root->right;
				else
				{
					Tree<T>* h = new Tree<T>(R);
					return MyIterator(h);
				}
			}
			return *this;
		}
	}
};

//===================����� ��������� ������ ������===================
template <typename T>
class Tree
{
	template <typename T>
	struct node
	{
		T data;
		node * parent;
		node * left;
		node * right;

		node(T d, node<T>* parent = nullptr, node<T>* left = nullptr, node<T>* right = nullptr)
		{
			data = d;
			this->parent = parent;
			this->left = left;
			this->right = right;
		}

		node() {}
	};

public:

	node<T>* root;

	T *getData()
	{
		return &root->data;
	}

	Tree<T>* Right()
	{
		return new Tree<T>(root->right);
	}

	Tree<T>* Left()
	{
		return new Tree<T>(root->left);
	}

	Tree<T>* Parent()
	{ 
		return new Tree<T>(root->parent); 
	}

	//��������������� �������, ������������ ���������� ���������� � ������
	template <typename T>
	int TreesSize(Tree<T> tree)
	{
		int res = 0;
		while (tree.root != nullptr)
		{
			++res;
			if (tree.root->left != nullptr)
				res += TreesSize(*Left());
			if (tree.root->right != nullptr)
				res += TreesSize(*Right());
		}
		return res;
	}

	//�������� �� ������ ������
	MyIterator<T> begin()
	{
		Tree<T>* p = this;
		while (p->root->left != nullptr)
		{
			p = p->Left();
		}
		return MyIterator<T>::MyIterator(p);
	}

	//�������� �� ����� ������
	MyIterator<T> end()
	{
		auto it = MyIterator<T>::MyIterator(maxTree());
		return ++it;
	}

	//�����������
	Tree(T d)
	{
		root = new node<T>(d);
	}

	//����������� �� ���������
	Tree() : root(nullptr) {}

	//�����������
	Tree(node<T>* rt)
	{
			this->root = rt;
	}

	//����������
	~Tree() {}

	//������
	Tree<T>* Root()
	{
		return new Tree<T>(root);
	}

	//������ ������
	void printTree()
	{
		if (root == nullptr)
			return;
		if (root->left != nullptr)
		{
			node<T>* l = root->left;
			while (l != nullptr)
			{
				cout << l->data << endl;
				l = l->left;
			}
		}
		cout << root->data << endl;
		if (root->right != nullptr)
		{
			node<T>* r = root->right;
			while (r != nullptr)
			{
				cout << r->right << endl;
				r = r->right;
			}
		}
	}

	//���������� ��������
    void insertTree(T x)
	{
		if (root != nullptr)
		{
			if (root->data > x)
			{
				if (root->left == nullptr)
					root->left = new node<T>(x, root);
				else if (root->left->data > x)
					root->left = new node<T>(x, root, nullptr, root->left);
				else
					root->left = new node<T>(x, root, root->left);
			}
			else
			{
				if (root->right == nullptr)
					root->right = new node<T>(x, root);
				else if (root->right->data > x)
					root->right = new node<T>(x, root, nullptr, root->right);
				else
					root->right = new node<T>(x, root, root->right);
			}
		}
		else
			root = new node<T>(x);
	}

	/*void insertTree(Tree<T> tree) {
		if (root != nullptr)
		{
			if (tree.root->data < root->data)
				if (Left() != nullptr) Left()->insertTree(tree);
				else *Left() = tree;
			else
				if (Right() != nullptr) Right()->insertTree(tree);
				else *Right() = tree;
		}
		else
			*this = tree;
	}*/

	//����� ��������
	bool findTree(T x)
	{
		if (root != nullptr) {
			bool l = false;
			bool r = false;
			if (root->left != nullptr)
				l = Left()->findTree(x);
			if (root->right != nullptr)
				r = Right()->findTree(x);
			return root->data == x || l || r;
		}
		else return false;
	}

	//����� ������������� �������� � ������ 
	Tree * maxTree() {
		auto p = root;
		if (p != nullptr) {
			while (p->parent != nullptr)
				p = p->parent;
			while (p->right != nullptr)
				p = p->right;
		}
		return new Tree(p);
	}

	//����� ������������ ��������
	node<T> minTree(node<T> x)
	{
		while (x.left != nullptr)
			x = x.left;
		return x;
	}

	//����� ������� ��������, ������ ��� ������� ���������
	node<T>* lessTree(T x)
	{
		if (root == nullptr)
			return nullptr;
		if (root->data == x)
			return root;
		node<T>* el = root;
		if (el->right != nullptr && el->right->data <= x)
			el = el->right;
		while (el->left && el->left->data <= x)
			el = el->left;
		return el;
	}

	//����� ������� ��������, ������ ��� ������� ���������
	node<T>* greateTree(T x)
	{
		if (root == nullptr)
			return nullptr;
		if (root->data == x)
			return root;
		node<T>* el = root;
		if (el->right != nullptr && el->right->data >= x)
			el = el->right;
		while (el->left && el->left->data >= x)
			el = el->left;
		return el;
	}

	//����� ������� ��������, ������ ���������
	Tree<T>* onlyGreateTree(T x)
	{
		if (root == nullptr)
			return nullptr;
		//if (root->data == x)
		//	return root;
		Tree<T>* el = this;
		if (el->root->right != nullptr && el->root->right->data > x)
			el = el->Right();
		while (el->root->left && el->root->left->data > x)
			el = el->Left();
		return el;
	}

	//����� �� ����� � ����������� ����� (���)
	void printTreeRekur()
	{
		if (root == nullptr)
			return;
		if (root->left != nullptr)
			Left()->printTreeRekur();
		cout << root->data << " ";
		if (root->right != nullptr)
			Right()->printTreeRekur();
	}

	//�������� ��������
	template <typename T>
	void deleteTree(T x, node<T>* root)
	{
		if (root == nullptr)
			return;
		if (root->left == nullptr && root->right == nullptr)
		{
			if (root->parent == nullptr)
				root = nullptr;
			else
			{
				if (x < root->parent->data)
					root->parent->left = root->right;
				else
					root->parent->right = root->right;
			}
			return;
		}
		if (x < root->data)
			deleteTree(x, root->left);
		else if (x > root->data)
			deleteTree(x, root->right);
		else if (root->left != nullptr && root->right != nullptr)
		{
			node<T>* el = root->right;
			while (el->left)
				el = el->left;
			el->parent = root->parent;
			root->data = el->data;
			deleteTree(root->data, root->right);
		}
		else
		{
			if (root->left)
			{

				if (root->data == x)
				{
					if (root->parent == nullptr)
						root->left->parent = nullptr;
					else
					{
						root->left->parent = root->parent;
						root->parent = nullptr;
					}
					return;
				}
				if (root->left->data == x)
				{
					root->left->parent = root->parent;
					root = root->left;
				}

			}
			else
			{
				if (root->right->data == x)
				{
					root->right->parent = root->parent;
					root = root->right;
				}
			}
		}
	}
};


//===================C�������a ������ ���������� �� ������ ��������� ������ ������===================
template <typename T>
class mySet
{
	Tree<T>* tree;
public:
	//����������� �� ���������
	mySet()
	{
		tree = new Tree<T>();
	}

	//���������� ��������
	void insertMySet(T x)
	{
		tree->insertTree(x);
	}

	//�������� ��������
	void deleteMySet(T x)
	{
		tree->deleteTree<T>(x, tree->Root());
	}

	//�������� �� ��������� ��������
	bool findMySet(int x)
	{
		return tree->findTree(x);
	}
};
#endif //!HEADER_H