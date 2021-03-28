#include "iSize.h"

iSize iSize::operator + (const iSize& s)
{
	iSize tmp;
	tmp.width = width + s.width;
	tmp.height = height + s.height;
	return tmp;
}

iSize iSize::operator - (const iSize& s)
{
	iSize tmp;
	tmp.width = width - s.width;
	tmp.height = height - s.height;
	return tmp;
}
iSize iSize::operator * (const iSize& s)
{
	iSize tmp;
	tmp.width = width * s.width;
	tmp.height = height * s.height;
	return tmp;
}
iSize iSize::operator / (const iSize& s)
{
	iSize tmp;
	tmp.width = width / s.width;
	tmp.height = height / s.height;
	return tmp;
}

iSize& iSize::operator += (const iSize& s)
{
	width += s.width;
	height += s.height;
	return *this;
}

iSize& iSize::operator -= (const iSize& s)
{
	width -= s.width;
	height -= s.height;
	return *this;
}
iSize& iSize::operator *= (const iSize& s)
{
	width *= s.width;
	height *= s.height;
	return *this;
}
iSize& iSize::operator /= (const iSize& s)
{
	width /= s.width;
	height /= s.height;
	return *this;
}

iSize iSize::operator + (const float s)
{
	iSize tmp;
	tmp.width = width + s;
	tmp.height = height + s;
	return tmp;
}

iSize iSize::operator - (const float s)
{
	iSize tmp;
	tmp.width = width - s;
	tmp.height = height - s;
	return tmp;
}
iSize iSize::operator * (const float s)
{
	iSize tmp;
	tmp.width = width * s;
	tmp.height = height * s;
	return tmp;
}
iSize iSize::operator / (const float s)
{
	iSize tmp;
	tmp.width = width / s;
	tmp.height = height / s;
	return tmp;
}


iSize& iSize::operator += (const float s)
{
	width += s;
	height += s;
	return *this;
}

iSize& iSize::operator -= (const float s)
{
	width -= s;
	height -= s;
	return *this;
}
iSize& iSize::operator *= (const float s)
{
	width *= s;
	height *= s;
	return *this;
}
iSize& iSize::operator /= (const float s)
{
	width /= s;
	height /= s;
	return *this;
}

iSize iSizeMake(float width, float height)
{
	iSize tmp;
	tmp.width = width;
	tmp.height = height;
	return tmp;
}

