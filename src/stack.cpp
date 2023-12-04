#include "stack.h"

/*node*/
node::node(node* n, double data)
{
	value = data;
	next = n;
}
void node::setNext(node *n)
{
	next = n;
	return;
}
node* node::getNext()
{
	return next;
}

double node::getValue()
{
	return value;
}
/*ENDE node*/

/*stack*/
stack::stack()
{
	m_size = 0;
	top = NULL;
}

void stack::push(double data)
{
	node *n = new node(top, data);
	if(n == NULL)return;
	top = n;
	m_size++;
	return;
}

double stack::pop()
{
	if(top == NULL)return NAN;
	double retVal = top->getValue();
	node* temp = top;
	top = top->getNext();
	delete temp;
	m_size--;
	return retVal;
}

double stack::peek()
{
	if(top == NULL)return NAN;
	double retVal = top->getValue();
	return retVal;
}

stack::~stack()
{
	while(top)
	{
		node* temp = top;
		top = top->getNext();
		delete temp;
	}
}

void stack::print()
{
	node *temp = top;
	while(temp)
	{
		std::cout<<temp->getValue()<<", ";
		temp = temp->getNext();
	}
	std::cout<<"\n";
	return;
}

void stack::leeren()
{
	while(top)
	{
		node* temp = top;
		top = top->getNext();
		delete temp;
	}
}

int stack::size()
{
	return m_size;
}

double stack::min()
{
	node* n = top;
	double dMin = top->getValue();
	while(n != NULL)
	{
		if(dMin > n->getValue())dMin = n->getValue();
		n = n->getNext();
	}
	return dMin;
}

double stack::max()
{
	node* n = top;
	double dMax = top->getValue();
	while(n != NULL)
	{
		if(dMax < n->getValue())dMax = n->getValue();
		n = n->getNext();
	}
	return dMax;
}

double stack::median()
{
	double retVal;
	stack kleiner, groesser;
	double mitte = this->pop();
	if(std::isnan(mitte))return NAN;
	
	do
	{
		while(top != NULL)
		{
			if(mitte < this->peek())
				groesser.push(this->pop());
			else
				kleiner.push(this->pop());
		}
		if(groesser.size() < kleiner.size())
		{
			groesser.push(mitte);
		}else
		{
			kleiner.push(mitte);
		}
		
		if(groesser.size() == kleiner.size())
		{
			retVal = ((groesser.min() + kleiner.max())/2);
			break;
		}else if(groesser.size() == kleiner.size() + 1)
		{
			retVal = (groesser.min());
			break;
		}else if(groesser.size() + 1 == kleiner.size())
		{
			retVal = (kleiner.max());
			break;
		}
		if(groesser.size() < kleiner.size())
		{
			groesser.leeren();
			while(!std::isnan(kleiner.peek()))
			{
				this->push(kleiner.pop());
			}
		}else
		{
			kleiner.leeren();
			while(!std::isnan(groesser.peek()))
			{
				this->push(groesser.pop());
			}
		}
		mitte = this->pop();
	}while(1);

	return retVal;
}
/*ENDE stack*/