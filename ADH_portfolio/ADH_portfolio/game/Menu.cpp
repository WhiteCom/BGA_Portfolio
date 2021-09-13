#include "Menu.h"
#include "MenuUI.h"

#include "Loading.h"
#include "Intro.h"
#include "MapEditor.h"
#include "Stage.h"
#include "App.h"
#include "Common.h"

void loadMenu()
{
	setStringName("assets/font/DungGeunMo.ttf");

	for (int i = 0; i < SOUND_NUM; i++)
		audioStop(i);
	audioPlay(3);


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
		printf("menu began (%f, %f)\n", point.x, point.y);
		break;
	case iKeyStatMoved:
		break;
	case iKeyStatEnded:
		break;
	}
	}
	

