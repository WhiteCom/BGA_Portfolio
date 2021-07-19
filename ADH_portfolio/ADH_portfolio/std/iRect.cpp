#include "iRect.h"

iRect iRect::operator + (const iRect rect)
{
	iRect rt;
	rt.origin = origin + rect.origin;
	rt.size = size + rect.size;
	return rt;
}

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

bool containPoint(iPoint point, iRect rect)
{
	return !(
		point.x < rect.origin.x ||
		point.x > rect.origin.x + rect.size.width ||
		point.y < rect.origin.y ||
		point.y > rect.origin.y + rect.size.height);
}

bool containRect(iRect rt0, iRect rt1)
{
	return !(
		rt0.origin.x + rt0.size.width < rt1.origin.x ||
		rt0.origin.x > rt1.origin.x + rt1.size.width ||
		rt0.origin.y + rt0.size.height < rt1.origin.y ||
		rt0.origin.y > rt1.origin.y + rt1.size.height);
}
