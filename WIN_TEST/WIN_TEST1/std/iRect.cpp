#include "iRect.h"

iRect iRectMake(iPoint p, iSize s)
{
	iRect tmp;
	tmp.origin.x = p.x;
	tmp.origin.y = p.y;
	tmp.size.width = s.width;
	tmp.size.height = s.height;
	return tmp;
}

iRect iRectMake(float x, float y, float width, float height)
{
	iRect tmp;
	tmp.origin.x = x;
	tmp.origin.y = y;
	tmp.size.width = width;
	tmp.size.height = height;
	return tmp;
}