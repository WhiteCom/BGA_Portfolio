#include "Menu.h"
#include "MenuUI.h"

#include "Loading.h"

#if (OS==OS_WINDOW)
#include "App.h"
#include "MapEditor.h"
#endif

void loadMenu()
{
    setStringName("assets/font/DungGeunMo.ttf");

#if 0 //#openAL
	for (int i = 0; i < SOUND_NUM; i++)
		audioStop(i);
	audioPlay(3);
#endif
	createPopMenu();
	createPopHow();
	createPopOption();
	createPopExit();

	showPopMenu(true);
}
void freeMenu()
{
	freePopMenu();
	freePopHow();
	freePopOption();
	freePopExit();
}
void drawMenu(float dt)
{
	setRGBA(0, 0, 0, 1);
	clearRect();
	setRGBA(1, 1, 1, 1);

	drawPopMenu(dt);
	drawPopHow(dt);
	drawPopOption(dt);
	drawPopExit(dt);
}

void keyMenu(iKeyStat stat, iPoint point)
{
	if (keyPopMenu(stat, point) ||
		keyPopHow(stat, point) ||
		keyPopOption(stat, point) ||
		keyPopExit(stat, point)
		)
		return;

	switch (stat)
	{
	case iKeyStatBegan :
		break;
	case iKeyStatMoved:
		break;
	case iKeyStatEnded:
		break;
	}
}

