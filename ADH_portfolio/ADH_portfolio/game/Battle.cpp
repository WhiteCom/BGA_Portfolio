#include "Battle.h"
#include "BattleUI.h"

#include "Loading.h"
#include "Stage.h"
#include "Ending.h"

//================================================================
// Battle.cpp
//================================================================

Texture* texBg;

BattleManager* bm;
BattlePop* bp;

void loadBattle()
{
	for (int i = 0; i < SOUND_NUM; i++)
		audioStop(i);
	audioPlay(4);

	bm = new BattleManager();

	const char* Bglist[9] = {
		"assets/Image/BattleBg/초원+.bmp",
		"assets/Image/BattleBg/[瑛] 평야.bmp",
		"assets/Image/BattleBg/던젼２+.bmp",
		"assets/Image/BattleBg/설원+.bmp",
		"assets/Image/BattleBg/황야+.bmp",
		"assets/Image/BattleBg/사구.bmp",
		"assets/Image/BattleBg/바위+.bmp",
		"assets/Image/BattleBg/던젼３+.bmp",
		"assets/Image/BattleBg/신전+.bmp",
	};

	texBg = createImage(Bglist[stageFrom]);

	createBattlePopTopUI();
	createBattlePopBottomUI();

	showBattlePopTopUI(true);
	showBattlePopBottomUI(true);
}

void freeBattle()
{
	delete bm;
	freeBattlePopTopUI();
	freeBattlePopBottomUI();
}

void drawBattle(float dt)
{
	//
	//drawBg
	//
	//현재 Bg Image 320 x 240 
	float rateX = devSize.width / texBg->width;
	float rateY = devSize.height / texBg->height;
	drawImage(texBg, 0, 0, TOP | LEFT,
		0, 0, texBg->width, texBg->height, rateX, rateY, 2, 0);

	//
	//draw unit(monster, hero)
	//
	bm->paint(dt);

	//
	//draw popUp
	//

	drawBattlePopTopUI(dt, iPointZero);
	drawBattlePopBottomUI(dt, iPointZero);
}

bool keyBattle(iKeyStat stat, iPoint point)
{
	if (keyBattlePopTopUI(stat, point))
		return false;

#if 0 //#need update! not complete Control logic
	for (int i = 0; i < bm->heroNum; i++)
	{
		if (bm->hero[i]->keyHero(stat, point))
			return true;
	}
#endif

	switch (stat)
	{
	case iKeyStatBegan:
		break;

	case iKeyStatMoved:
		break;

	case iKeyStatEnded:
		break;
	}
	return false;
}

