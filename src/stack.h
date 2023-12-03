#ifndef __STACK_H
#define __STACK_H

#include<cmath>
#include <iostream>

class node{
	double value;
	node *next;
public:
	node(node* n, double data);
	void setNext(node *n);
	node* getNext();
	double getValue();
};

class stack{
	node *top;
	int m_size;
public:
	stack();
	~stack();
	void push(double data);
	double pop();
	double peek();
	void leeren();
	int size();
	void size(int);
	double median();
	double min();
	double max();
	
	void print();
};

#endif //__STACK_H