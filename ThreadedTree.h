//
//  ThreadedTree.h
//  Algoritmen
//
//  Created by Jonas Anseeuw on 29/05/13.
//  Copyright (c) 2013 Jonas Anseeuw. All rights reserved.
//

#include <iostream>
#include <stack>
using namespace std;

template<class T>
class Node;

template<class T>
class Tree {
public:
	Node<T> * n;
	// If type = 0 ( direct connection, parent to child)
	// elseif type = 1 ( successor or predecessor)
	// else type = 2 ( nullpointer)
	int type;
	
	Tree():n(0),type(2){}
	Tree(int type):n(0),type(type){}
	Tree<T>* search(T);
	Tree<T>* search(T, Node<T> *&, Node<T> *&);
	T lookforsuccessor(T key);
	void walk_inorder();
	void add(T);
};

template<class T>
Tree<T>* Tree<T>::search(T key){
	Tree<T>* start = this;
	while(start->n!=0 && start->n->value != key){
		if(start->n->value > key)
			start = &(start->n->left);
		else
			start = &(start->n->right);
	}
	return start;
}

template<class T>
Tree<T>* Tree<T>::search(T key, Node<T> *& predecessor, Node<T> *& successor){
	Tree<T> * start = this;
	while(
				// Not a successor connection
				start->type!=1 &&
				start->n!=0 && start->n->value != key
				){
		if(start->n->value > key){
			successor = start->n;
			start = &(start->n->left);
		}
		else{
			predecessor = start->n;
			start = &(start->n->right);
		}
	}
	return start;
}

template<class T>
void Tree<T>::add(T key){
	// If root == nullpointer, no nodes in the tree
	// Special case
	if(n==0){
		n = new Node<T>(key);
		// Two nullpointers
		n->left.type = 2;
		n->right.type = 2;
	}
	else{
		Node<T> * predecessor = 0;
		Node<T> * successor = 0;
		Tree<T>*position=search(key,predecessor,successor);
		if(position->n==0 || position->type == 1){
			// New connection to Node becomes
			// a direct connection
			position->n = new Node<T>(key);
			position->type = 0;
			
			// Change predecessor
			position->n->left.n = predecessor;
			if(position->n->left.n==0)
				position->n->left.type = 2;
			else
				position->n->left.type = 1;
			
			// Change successor
			position->n->right.n = successor;
			if(position->n->right.n==0)
				position->n->right.type = 2;
			else
				position->n->right.type = 1;
		}
		//else
		//	cout << "Key " << key << " already in the tree :(" << endl;
	}
}

template<class T>
T Tree<T>::lookforsuccessor(T key){
	Tree<T> * start = this;
	bool stopit = false;
	while(
				// Not a successor connection
				start->type!=1 && start->n != 0 && !stopit
				){
		if(start->n->value > key){
			if(start->n->left.type==1 || start->n->left.type==2)
				stopit = true;
			else
				start = &(start->n->left);
		}
		else
			start = &(start->n->right);
	}
	return (start->n!=0)?start->n->value:-1;
}

template<class T>
void Tree<T>::walk_inorder(){
	// if tree not empty!
	if(n!=0)
	{
		// We start at root
		Tree<T> * start = this;
		// We have a stack to determine previously visited nodes
		// This  stack is only used for a history of left sub-trees
		// If a node is on the stack, it means that it's subtree
		// already has been searched
		stack<T> bezocht;
		
		// Go to the left (minimum of the threaded (search) tree)
		while(start->n->left.n!=0){
			bezocht.push(start->n->value);
			start = &(start->n->left);
		}
		bezocht.push(start->n->value);
		
		// We are the minimum of the tree
		// Start following right pointers until
		// the right pointer is zero (max of tree, end of inorder traversal)
		while(start->n->right.n!=0){
			cout << start->n->value << ", ";
			// Follow right pointer
			start = &(start->n->right);
			
			// If left connection is a direct connection
			if(start->n->left.type==0){
				// If the left node is already been located
				// previously we will not search in that sub-tree
				// (else we get endless loop)
				if(start->n->left.n->value==bezocht.top())
					bezocht.pop();
				// If sub tree isn't searched previously we will go to the
				// minimum of that subtree, and we add the (left)
				// nodes to the stack, so we that subtree has been handled
				else{
					while(start->n->left.type==0){
						start = &(start->n->left);
						bezocht.push(start->n->value);
					}
				}
			}
		}
		cout << start->n->value << ", " << endl;
	}
}

template<class T>
class Node {
public:
	Tree<T> left, right;
	T value;
	friend class Tree<T>;
	
	Node(){}
	Node(T value):value(value){}
};
