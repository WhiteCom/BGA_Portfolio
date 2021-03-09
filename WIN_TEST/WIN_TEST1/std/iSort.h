#pragma once

struct SortData
{
	int index;
	float y;
};

class iSort
{
public:
	iSort(int max = 100);
	virtual ~iSort();

	void init();

	void update();

	void add(int index, float y);
	void add(float y);

	int get(int index);
public:
	SortData* _sd;
	SortData** sd;
	int sdNum;
};

