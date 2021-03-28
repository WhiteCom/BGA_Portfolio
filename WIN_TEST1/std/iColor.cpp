#include "iColor.h"

iColor4f iColor4f::operator = (const iColor4b& c)
{
	iColor4f tmp;
	tmp.r = c.r / 255.f;
	tmp.g = c.g / 255.f;
	tmp.b = c.b / 255.f;
	tmp.a = c.a / 255.f;
	return tmp;
}

iColor4b iColor4b::operator= (const iColor4f& c)
{
	iColor4b tmp;
	tmp.r = c.r * 0xFF;
	tmp.g = c.r * 0xFF;
	tmp.b = c.r * 0xFF;
	tmp.a = c.r * 0xFF;
	return tmp;
}

iColor4f iColor4fMake(float r, float g, float b, float a)
{
	iColor4f tmp;
	tmp.r = r;
	tmp.g = g;
	tmp.b = b;
	tmp.a = a;
	return tmp;
}
iColor4b iColor4bMake(uint8 r, uint8 g, uint8 b, uint8 a)
{
	iColor4b tmp;
	tmp.r = r;
	tmp.g = g;
	tmp.b = b;
	tmp.a = a;
	return tmp;
}
