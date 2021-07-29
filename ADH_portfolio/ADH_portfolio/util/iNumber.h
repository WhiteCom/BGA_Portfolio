#pragma once

#include "iType.h"

#define iNumber_delta 0.2f

class iNumber
{
public:
	iNumber();
	virtual ~iNumber();

	int get(float dt);
	void set(int num);
	void add(int num);
	void minus(int num);

	const char* getStep(float dt);

public:
	int num, tNum;
	float delta, _delta;

	char* str;
};