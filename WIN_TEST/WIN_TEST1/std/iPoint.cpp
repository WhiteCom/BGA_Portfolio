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

float iPointAngle(const iPoint& v1, const iPoint& v2, const iPoint& v3)
{
	float a2 = (v1.x - v3.x) * (v1.x - v3.x) + (v1.y - v3.y) * (v1.y - v3.y);
	float b2 = (v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y);
	float c2 = (v2.x - v3.x) * (v2.x - v3.x) + (v2.y - v3.y) * (v2.y - v3.y);
	float degree = acos((b2 + c2 - a2) / sqrtf(4 * b2 * c2)) * 180 / M_PI;
	if (v3.y > v1.y)
		degree = 360 - degree;
	return degree;
}
iPoint iPointRotate(iPoint p, iPoint t, float degree)
{
	while (degree < 0)		degree += 360;
	while (degree > 360)	degree -= 360;
	float sinr = _sin(degree);
	float cosr = _cos(degree);

	iPoint r = iPointMake(p.x - t.x, t.y - p.y);
	iPoint _p;
	_p.x = r.x * cosr - r.y * sinr;
	_p.y = r.x * sinr + r.y * cosr;

	_p.x = t.x + _p.x;
	_p.y = t.y - _p.y;
	return _p;
}