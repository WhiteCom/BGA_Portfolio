#include "iArray.h"

#include "iStd.h"

iArray::iArray(ARRAY_METHOD m)
{
	node = NULL;
	count = 0;
	method = m;
}

iArray::~iArray()
{
	removeAllObject();
}

void iArray::addObject(void* data)
{
	iNode* n = new iNode();
	n->data = data;
	n->prev = node;

	node = n;
	count++;
}

void iArray::addObject(int index, void* data)
{
	if (index < 0)
		index = 0;

	else if(index >= count)
	{
		addObject(data);
		return;
	}

	iNode* n = node;
	int last = count - 1;
	for (int i=0;n;i++)
	{
		if (last - i == index)
		{
			iNode* tmp = new iNode();
			tmp->data = data;
			tmp->prev = n->prev;
			n->prev = tmp;
			
			count++;
			return;
		}
		n = n->prev;
	}
}

void iArray::removeObject(int index)
{
	if (index < 0)
		index = 0;
	else
	{
		
	}
}

void iArray::removeData(void* data)
{
	iNode* n = node;
	iNode* p = NULL;
	for (; n;)
	{
		
		if (n->data == data)
		{
			p->prev = n->prev;
			if (method)
				method(n->data);
			delete n;
			count--;
			//return;
		}
		p = n;
		n = n->prev;
	}
}

void iArray::removeAllObject()
{
	iNode* n = node;
	for (; n;)
	{
		iNode* p = n->prev;
		if (method)
			method(n->data);
		delete n;

		n = p;
	}
}

void* iArray::objectAtIndex(int index)
{
	iNode* n = node;
	for (; n;)
	{
		n = n->prev;
	}
	return NULL;
}

iNode* iArray::nodeAtIndex(int index)
{
	iNode* n = NULL;
	return n;
}