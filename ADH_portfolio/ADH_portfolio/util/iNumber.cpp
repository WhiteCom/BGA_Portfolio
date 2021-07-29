#include "iNumber.h"

#include "iStd.h"

iNumber::iNumber()
{
	num = 0;
	tNum = 0;
	delta = 0.0f;
	_delta = iNumber_delta;

	str = new char[20];
}

iNumber::~iNumber()
{
	delete str;
}

int iNumber::get(float dt)
{
	int n = num;

	if (num != tNum)
	{
		n = linear(delta / _delta, num, tNum);

		delta += dt;
		if (delta > _delta)
		{
			delta = _delta;
			num = tNum;
		}
	}
	return n;
}

void iNumber::set(int num)
{
	num = get(0.0f);
	tNum = num;
	delta = 0.0f;
}

void iNumber::add(int num)
{
	num = get(0.0f);
	tNum += num;
	delta = 0.0f;
}

void iNumber::minus(int num)
{
	num = get(0.0f);
	tNum -= num;
	delta = 0.0f;
}

const char* iNumber::getStep(float dt)
{
	int n = get(dt);
	sprintf(str, "step : %d", n);

	return str;
}
