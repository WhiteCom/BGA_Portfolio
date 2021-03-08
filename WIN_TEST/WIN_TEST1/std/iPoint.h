#pragma once

struct iPoint
{
	float x, y;

	bool operator == (const iPoint& p);
	bool operator != (const iPoint& p);

	iPoint operator + (const iPoint& p);
	iPoint operator - (const iPoint& p);
	iPoint operator * (const iPoint& p);
	iPoint operator / (const iPoint& p);

	iPoint operator + (const float p);
	iPoint operator - (const float p);
	iPoint operator * (const float p);
	iPoint operator / (const float p);

	iPoint& operator += (const iPoint& p);
	iPoint& operator -= (const iPoint& p);
	iPoint& operator *= (const iPoint& p);
	iPoint& operator /= (const iPoint& p);

	iPoint& operator += (const float p);
	iPoint& operator -= (const float p);
	iPoint& operator *= (const float p);
	iPoint& operator /= (const float p);
};

#define iPointZero iPointMake(0, 0);
#define iPointOne iPointMake(1, 1);

iPoint iPointMake(float _x, float _y);
float iPointDot(iPoint p1, iPoint p2);
float iPointDistance(iPoint p1, iPoint p2);
float iPointDistance2(iPoint p1, iPoint p2);
float iPointLength(iPoint p);