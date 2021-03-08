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
	f = 0;
	tickDt = 0.0f;
	num = 0;
#endif
	lastDt = 0.0f;
}

void iFPS::setSlowTime(float duration)
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

		tickDt = 0.0f;
		num = 0;
	}

	return f;
}
#endif //DISPLAY_FPS
