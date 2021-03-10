#include "iStd.h"

static int keys = 0;

iSize devSize;
iRect viewport;

void setKeyDown(iKeyStat stat, int c)
{
	if (stat == iKeyStatBegan)
	{
		if (c == 'a' || c == 'A')
			keys |= keysA;
		else if (c == 's' || c == 'S')
			keys |= keysS;
		else if (c == 'd' || c == 'D')
			keys |= keysD;
		else if (c == 'w' || c == 'W')
			keys |= keysW;
		else if (c == ' ')
			keys |= keysSpace;
	}
	
	else if (stat == iKeyStatEnded)
	{
		if (c == 'a' || c == 'A')
			keys &= ~keysA;
		else if (c == 's' || c == 'S')
			keys &= ~keysS;
		else if (c == 'd' || c == 'D')
			keys &= ~keysD;
		else if (c == 'w' || c == 'W')
			keys &= ~keysW;
		else if (c == ' ')
			keys &= ~keysSpace;
	}
}

int getKeyDown()
{
	return keys;
}

void appInitialize()
{
	srand(time(NULL));
}
int random()
{
	return rand();
}

float _cos(float degree)
{
	return cos(degree * M_PI / 180);
}
float _sin(float degree)
{
	return sin(degree * M_PI / 180);
}