#pragma once

struct iNode
{
	void* data;
	iNode* prev;
};

typedef void (*ARRAY_METHOD)(void* parm);

class iArray
{
public:
	iArray(ARRAY_METHOD m);
	virtual ~iArray();

	void addObject(void* data);
	void addObject(int index, void* data);

	void removeObject(int index);
	void removeData(void* data);
	void removeAllObject();

	void* objectAtIndex(int index);
private:
	iNode* nodeAtIndex(int index);
public:
	iNode* node;
	int count;
	ARRAY_METHOD method;
};