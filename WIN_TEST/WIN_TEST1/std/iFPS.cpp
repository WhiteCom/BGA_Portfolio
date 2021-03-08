#include "iFPS.h"
#include "iWindow.h"

iFPS* iFPS::instance()
{
	static iFPS* ins = new iFPS();
	return ins;
}

iFPS::iFPS()
{
	now = GetTickCount();
#ifdef DISPLAY_FPS
	num = 0;
	tickDt = 0.0f;
	f = 0;
#endif
	lastDt = 0.0f;
}

iFPS::~iFPS()
{

}

float iFPS::update()
{
#ifdef DISPLAY_FPS
	fps(true);
#endif
	unsigned long n = GetTickCount();

	float dt = (n - now) / 1000.f;
	now = n;
	lastDt = dt;
	return dt;
}

void iFPS::setSlowTime(float duration)
{

}

#ifdef DISPLAY_FPS
int iFPS::fps(bool apply)
{
	if (apply == false)
		return f;
	unsigned long n = GetTickCount();

	float dt = (n - now) / 1000.f;
	tickDt += dt;
	num++;
	if (tickDt > 0.3f)
	{
		f = num / tickDt;
		num = 0;
		tickDt = 0.0f;
	}

	return f;
}
#endif //DISPLAY_FPS