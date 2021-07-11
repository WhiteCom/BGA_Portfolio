#pragma once

#include "iType.h"
#include "iArray.h"
#include "iPoint.h"
#include "iSize.h"
#include "iRect.h"

#include <stdio.h>

typedef void (*ANIMATION_METHOD)(void* parm);

class iImage
{
public:
	iImage();
	virtual ~iImage();

	static void freeObject(void* parm);
	void addObject(Texture* tex);
	void setTexObject(int index);

	iRect touchRect(iPoint off = iPointZero, iSize size = iSizeZero);

	void startAnimation(ANIMATION_METHOD m = NULL, void* p = NULL);
	void stopAnimation();

	void paint(float dt, iPoint off, iPoint rate);
	void paint(float dt, iPoint off, float rx, float ry);
	void paint(float dt, float x, float y, float sx, float sy);

	iImage* copy();
public:
	iArray* arrayTex;
	Texture* tex;

	bool ani;
	uint8 repeatNum, repeatIdx;
	uint8 frame, lastframe;
	float _delta, delta;

	iPoint position;
	int anc;
	int8 reverse;

	ANIMATION_METHOD method;
	void* parm;
};