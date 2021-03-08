#include "iPoint.h"
#include "iStd.h"

bool iPoint::operator == (const iPoint& p)
{
	return x == p.x && y == p.y;
}
bool iPoint::operator != (const iPoint& p)
{
	return x != p.x || y != p.y;
}

iPoint iPoint::operator + (const iPoint& p)
{
	iPoint r;
	r.x = x + p.x;
	r.y = y + p.y;
	return r;
}

iPoint iPoint::operator - (const iPoint& p)
{
	iPoint r;
	r.x = x - p.x;
	r.y = y - p.y;
	return r;
}

iPoint iPoint::operator * (const iPoint& p)
{
	iPoint r;
	r.x = x * p.x;
	r.y = y * p.y;
	return r;
}

iPoint iPoint::operator / (const iPoint& p)
{
	iPoint r;
	r.x = x / p.x;
	r.y = y / p.y;
	return r;
}

iPoint iPoint::operator + (const float p)
{
	iPoint r;
	r.x = x + p;
	r.y = y + p;
	return r;
}

iPoint iPoint::operator - (const float p)
{
	iPoint r;
	r.x = x - p;
	r.y = y - p;
	return r;
}
iPoint iPoint::operator * (const float p)
{
	iPoint r;
	r.x = x * p;
	r.y = y * p;
	return r;
}
iPoint iPoint::operator / (const float p)
{
	iPoint r;
	r.x = x / p;
	r.y = y / p;
	return r;
}

iPoint& iPoint::operator += (const iPoint& p)
{
	x += p.x;
	y += p.y;
	return *this;
}

iPoint& iPoint::operator -= (const iPoint& p)
{
	x -= p.x;
	y -= p.y;
	return *this;
}
iPoint& iPoint::operator *= (const iPoint& p)
{
	x *= p.x;
	y *= p.y;
	return *this;
}
iPoint& iPoint::operator /= (const iPoint& p)
{
	x /= p.x;
	y /= p.y;
	return *this;
}

iPoint& iPoint::operator += (const float p)
{
	x += p;
	y += p;
	return *this;
}

iPoint& iPoint::operator -= (const float p)
{
	x -= p;
	y -= p;
	return *this;
}
iPoint& iPoint::operator *= (const float p)
{
	x *= p;
	y *= p;
	return *this;
}

iPoint& iPoint::operator /= (const float p)
{
	x /= p;
	y /= p;
	return *this;
}

iPoint iPointMake(float _x, float _y)
{
	iPoint r;
	r.x = _x;
	r.y = _y;
	return r;
}

float iPointDot(iPoint p1, iPoint p2)
{
	return p1.x * p2.x + p1.y * p2.y;
}

float iPointDistance(iPoint p1, iPoint p2)
{
	iPoint r;
	r.x = p1.x - p2.x;
	r.y = p1.y - p2.y;
	return sqrt(r.x * r.x + r.y * r.y);
}

float iPointDistance2(iPoint p1, iPoint p2)
{
	float a = p1.x - p2.x;
	float b = p1.y - p2.y;
	return a * a + b * b;
}

float iPointLength(iPoint p)
{
	float len = p.x * p.x + p.y * p.y;
	return sqrt(len);
}