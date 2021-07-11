#include "Battle.h"

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
	bm = new BattleManager();

	texBg = createImage("assets/Image/BattleBg/옥좌.bmp");

	createBattlePopTopUI();
	createBattlePopBottomUI();

	showBattlePopTopUI(true);
	showBattlePopBottomUI(true);
}

void freeBattle()
{
	delete bm;
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

	for (int i = 0; i < bm->heroNum; i++)
	{
		if (bm->hero[i]->keyHero(stat, point))
			return true;
	}

	switch (stat)
	{
	case iKeyStatBegan:
#if 1 //test code
		//setLoading(gs_stage, freeBattle, loadStage);
#endif
		break;

	case iKeyStatMoved:
		break;

	case iKeyStatEnded:
		break;
	}
	return false;
}


//========================================================
// popTopUI : playTime, Btn(Menu, Inven, Option)
//========================================================


void createBattlePopTopUI()
{

}
void freeBattlePopTopUI()
{

}
void showBattlePopTopUI(bool show)
{

}
void drawBattlePopTopUI(float dt, iPoint off)
{

}
bool keyBattlePopTopUI(iKeyStat stat, iPoint point)
{
	return false;
}

//========================================================
// popBottomUI 
//========================================================
iPopup* popBottomUIStatus; //전투메뉴
iStrTex** imgBottomUIStatus; //Enemy, Hero

TimeRate* timeRate;

Texture* texEnemyStat(const char* str);
Texture* texHeroStat(const char* str);

void drawBattlePopAfter(iPopup* pop, float dt, float rate);

void createBattlePopBottomUI()
{
	iImage* img;
	Texture* tex;
	iPopup* pop;
	imgBottomUIStatus = new iStrTex * [MAX_ENEMY + MAX_HERO];
	int statIndex = 0;

	//
	//Total Bg
	//
	pop = new iPopup();
	iGraphics* g = iGraphics::share();
	iSize size = iSizeMake(1000, 300);
	iPoint BgOff = iPointMake(devSize.width / 2 - 500, devSize.height / 2 + 50);
	g->init(size);
	
	setRGBA(0.5f, 0.5f, 0.5f, 0.5f);
	g->fillRect(0,0,size.width, size.height, 10);

	tex = g->getTexture();
	img = new iImage();
	img->addObject(tex);
	freeImage(tex);
	
	pop->addObject(img);


	//
	// EnemyStatusBg
	//
	iPoint MenuBgOff = iPointMake(40, 30);

	size = iSizeMake(320, 240);
	g->init(size);

	setRGBA(1, 1, 1, 1.0f);
	g->drawRect(0, 0, size.width, size.height, 10);
	setRGBA(0.2f, 0.2f, 0.2f, 0.8f);
	g->fillRect(0, 0, size.width, size.height, 10);
	setRGBA(1, 1, 1, 1);
	tex = g->getTexture();

	img = new iImage();
	img->addObject(tex);
	freeImage(tex);
	img->position = MenuBgOff;
	pop->addObject(img);
	
	//
	// EnemyStatus
	//

	size = iSizeMake(100, 60);

	const char* stBottomEnemy[4] = {"적1", "적2", "적3", "적4"};
	

	for (int i = 0; i < bm->enemyNum; i++)
	{
		iStrTex* stStat = new iStrTex(texEnemyStat); //name, HP, MP
		stStat->setString("[%s] %s : %d",
			stBottomEnemy[i], "HP", bm->enemy[i]->hp);
		
		img = new iImage();
		img->addObject(stStat->tex);
		img->position = MenuBgOff + iPointMake(0, 50*i);
		pop->addObject(img);
		imgBottomUIStatus[statIndex] = stStat;
		statIndex++;
	}

	//
	// HeroStatusBg
	//
	iPoint StatBgOff = iPointMake(400, 30); //80 + 320

	size = iSizeMake(560, 240);
	g->init(size);

	setRGBA(1, 1, 1, 1);
	g->drawRect(0, 0, size.width, size.height, 10);
	setRGBA(0.2f, 0.2f, 0.2f, 0.8f);
	g->fillRect(0, 0, size.width, size.height, 10);

	tex = g->getTexture();
	img = new iImage();
	img->addObject(tex);
	freeImage(tex);
	img->position = StatBgOff;
	pop->addObject(img);

	//
	// HeroStat
	//
	const char* stBottomHero[4] = { "영웅1", "영웅2", "영웅3", "영웅4" };
	const char* stat[2] = { "HP", "MP" };
	
//#need update! if change bm->hero
	for (int i = 0; i < bm->heroNum; i++)
	{
		//
		//str
		//
		iStrTex* stStat = new iStrTex(texHeroStat); //name, HP, MP
		stStat->setString("[%s]  %s : %d / %s : %d",
			stBottomHero[i], stat[0], bm->hero[i]->hp, stat[1], bm->hero[i]->mp);
		
		img = new iImage();
		img->addObject(stStat->tex);
		img->position = StatBgOff + iPointMake(0, 50*i);
		pop->addObject(img);
		imgBottomUIStatus[statIndex] = stStat;
		statIndex++;
	}

	pop->methodAfter = drawBattlePopAfter;
	pop->openPoint = BgOff;
	pop->closePoint = BgOff;

	popBottomUIStatus = pop;

	timeRate = new TimeRate();
}
void freeBattlePopBottomUI()
{
	delete popBottomUIStatus;
	for (int i = 0; i < MAX_ENEMY + MAX_HERO; i++)
		delete imgBottomUIStatus[i];
	delete imgBottomUIStatus;

	delete timeRate;
}

Texture* texEnemyStat(const char* str)
{
	iGraphics* g = iGraphics::share();
	iSize size = iSizeMake(320, 50);
	g->init(size);

	setStringSize(28);
	setStringRGBA(1, 1, 1, 1);
	g->drawString(25, 25, TOP | LEFT, str);

	return g->getTexture();
}

Texture* texHeroStat(const char* str)
{
	iGraphics* g = iGraphics::share();
	iSize size = iSizeMake(600, 50);
	g->init(size);

	setStringSize(28);
	setStringRGBA(1, 1, 1, 1);
	g->drawString(25, 25, TOP | LEFT, str);

	
	return g->getTexture();
}

void drawBattlePopAfter(iPopup* pop, float dt, float rate)
{
	//2 3 4 5 : enemy
	//7 8 9 10 : hero
	const char* stBottomEnemy[4] = { "적1", "적2", "적3", "적4" };
	const char* stBottomHero[4] = { "영웅1", "영웅2", "영웅3", "영웅4" };
	const char* stat[2] = { "HP", "MP" };

	int enemyNum = bm->enemyNum;
	int heroNum = bm->heroNum;

	for (int i = 0; i < enemyNum; i++)
		imgBottomUIStatus[i]->setString("[%s] %s : %d ", stBottomEnemy[i], stat[0], bm->enemy[i]->hp);

	for (int i = 0; i < heroNum; i++)
		imgBottomUIStatus[i+enemyNum]->setString("[%s]  %s : %d / %s : %d", stBottomHero[i], stat[0], bm->hero[i]->hp, stat[1], 0);
	
	
	iPoint StatBgOff = iPointMake(400, 30) + pop->closePoint; //80 + 320
	for (int i = 0; i < bm->heroNum; i++)
	{
		iPoint gp = StatBgOff + iPointMake(423, 25 + 50 * i);
		
		float heroRate = bm->hero[i]->attDt;
		float _heroRate = bm->hero[i]->_attDt;

		timeRate->paint(dt, gp, heroRate, _heroRate);
	}

}

void showBattlePopBottomUI(bool show)
{
	popBottomUIStatus->show(show);
	if (show)
	{

	}
	else
	{

	}
}
void drawBattlePopBottomUI(float dt, iPoint off)
{
	popBottomUIStatus->paint(dt);
}


//================================================================
// BattleUnit, Monster, Hero
//================================================================

//================================================================
//BattleUnit
//================================================================

#define MAX_HP 100
#define MAX_AP 10

BattleUnit::BattleUnit(int index)
{
	this->index = index;
	imgs = NULL;
	imgCurr = NULL;
	BuPop = NULL;
	be = BeWait;
	imgIndex = 0;
	imgNum = 0;
	position = iPointZero;
	hp = MAX_HP;
	atk = MAX_AP;
//#need update! set mp
	mp = 0;

	attAniDt = _attAniDt = 0.0f;
	attDt = 0.0f;
	_attDt = 0.0f;
	target = NULL;
	attacking = false;
	state = 0;
}

BattleUnit::~BattleUnit()
{
	if (imgs == NULL)
		return;

	for (int i = 0; i < imgNum; i++)
		delete imgs[i];
	delete imgs;
}

float BattleUnit::update(float dt)
{
	attDt += dt;
	if (attDt >= _attDt)
	{
		return (attDt - _attDt) / _attDt;
	}
	return 0.0f;
}

bool BattleUnit::paint(float dt, iPoint off)
{
	//imgCurr->paint(dt, position + off, iPointMake(1, 1));
	imgCurr->paint(dt, position + off, iPointMake(1, 1));

	if (hp == 0)
		return true;
	else
		return false;
}

bool BattleUnit::paint(float dt, iPoint off, iPoint rate)
{
	imgCurr->paint(dt, position + off, rate);

	if (hp == 0)
		return true;
	else
		return false;
}
bool BattleUnit::paint(float dt, iPoint off, float rx, float ry)
{
	imgCurr->paint(dt, position + off, rx, ry);

	if (hp == 0)
		return true;
	else
		return false;
}

void BattleUnit::attack()
{
	attDt = 0.0f;
	_position = position;
}

//================================================================
// BUMonster
//================================================================

BUMonster::BUMonster(int index) : BattleUnit(index)
{
	const char* path[2][5] = {
		{
			"assets/Image/Monster/[Z] (異) 요우무.bmp",
			"assets/Image/Monster/[Z] (異) 요우무 Ct.bmp",
			"assets/Image/Monster/[Z] (異) 요우무 Ct.2.bmp",
			"assets/Image/Monster/[Z] (異) 요우무 M.Sl(4).bmp",
			"assets/Image/Monster/[Z] (異) 요우무.bmp"
		},
		{
			"assets/Image/Monster/[Z] (異) 요우무.bmp",
			"assets/Image/Monster/[Z] (異) 요우무 Ct.bmp",
			"assets/Image/Monster/[Z] (異) 요우무 Ct.2.bmp",
			"assets/Image/Monster/[Z] (異) 요우무 M.Sl(4).bmp",
			"assets/Image/Monster/[Z] (異) 요우무.bmp"
		},
	};
	imgs = new iImage * [BehaveNum];
	for (int i = 0; i < BehaveNum; i++)
	{
		Texture* tex = createImageAlpha(path[index][i]);
		iImage* img = new iImage();
		img->addObject(tex);
		freeImage(tex);

		imgs[BeWait + i] = img;
	}
	imgNum = BehaveNum;
	be = BeWait;
	imgCurr = imgs[BeWait];

	
	//_attAniDt = attackDt[index];
	_attDt = ENEMY_COOL_TIME;
}

BUMonster::~BUMonster()
{

}

float BUMonster::update(float dt)
{
	//to do...(버퍼)

	return BattleUnit::update(dt);
}

bool BUMonster::paint(float dt, iPoint off)
{
	float atkTime = 0.0f;
	atkTime += dt;

	iPoint tp = iPointZero;
	iPoint tp2 = iPointZero;

	float imgW = imgCurr->tex->width;
	float imgH = imgCurr->tex->height;
	iPoint center = off + iPointMake(-imgW / 2, -imgH / 2);

	bool dead = false; //default;

	if (target && attacking)
	{
		//move
		if (state == 0)
		{
			attAniDt += dt;

			if (attAniDt >= ENEMY_MOVE_TIME)
			{
				attAniDt = ENEMY_MOVE_TIME;
				imgCurr = imgs[BeWait];
				attAniDt = 0.0f;
				state = 1;
			}
			iPoint tmpP = target->position - position - iPointMake(50, 0);
			tp = easeIn(attAniDt / ENEMY_MOVE_TIME, position - position, tmpP);

			dead = BattleUnit::paint(dt, center + tp);

			return dead;
		}

		//attack
		else if (state == 1)
		{
			attAniDt += dt;

			if (attAniDt >= ENEMY_ATK_TIME)
			{
				attAniDt = ENEMY_ATK_TIME;
				imgCurr = imgs[BeWait];
				attAniDt = 0.0f;
				state = 2;
				target->hp -= atk;
				if (target->hp < 0)
					target->hp = 0;
			}
			else if (attAniDt >= ENEMY_ATK_TIME / 2)
			{
				imgCurr = imgs[BeAttack];
			}

			iPoint tmpP = target->position - position - iPointMake(50, 0);
			tp = linear(attAniDt / (ENEMY_ATK_TIME), tmpP, tmpP);

			dead = BattleUnit::paint(dt, center + tp);

			return dead;
		}

		//back
		else if (state == 2)
		{
			attAniDt += dt;

			if (attAniDt >= ENEMY_BACK_TIME)
			{
				attAniDt = ENEMY_BACK_TIME;
				imgCurr = imgs[BeWait];
				state = 0;
				attAniDt = 0.0f;
				bm->attacking = false;
				attacking = false;
			}
			iPoint tmpP = target->position - position - iPointMake(50, 0);
			tp = easeIn(attAniDt / (ENEMY_BACK_TIME), tmpP, position - position);

			dead = BattleUnit::paint(dt, center + tp);

			return dead;
		}
	}

	else
	{
		dead = BattleUnit::paint(dt, center);
//#need update! add tp position
		state = 0;
		attAniDt = 0.0f;

		return dead;
	}

	//공격 이후

	//overlay...drawing;;;


	return dead;
}

void BUMonster::attack()
{
	//to do...
	//맞는 놈 선택 
	if (index == 0)
	{
		//체력 가장 높은놈...
	}
	else if (index == 1)
	{
		//공격력 가장 높은 놈...
	}
	else if (index == 2)
	{
		//한 놈만 패는 놈
	}

	if(bm->heroNum > 0)
		target = (BattleUnit*)bm->hero[random() % bm->heroNum];

	BattleUnit::attack();
}

//================================================================
// BUHero
//================================================================

#define HERO 0
BUHero::BUHero(int index) : BattleUnit(index)
{
#if HERO //#need update! move to Monster
	const char* path[2][5] = {
			{
				"assets/Image/Monster/[Z] (異) 사쿠야.bmp",
				"assets/Image/Monster/[Z] (異) 사쿠야 (2).bmp",
				"assets/Image/Monster/[Z] (異) 사쿠야 (3).bmp",
				"assets/Image/Monster/[Z] (異) 사쿠야 (2).bmp",
				"assets/Image/Monster/[Z] (異) 사쿠야 (2).bmp",
			},
			{
				"assets/Image/Monster/[Z] (異) 사쿠야.bmp",
				"assets/Image/Monster/[Z] (異) 사쿠야 (2).bmp",
				"assets/Image/Monster/[Z] (異) 사쿠야 (3).bmp", 
				"assets/Image/Monster/[Z] (異) 사쿠야 (2).bmp",
				"assets/Image/Monster/[Z] (異) 사쿠야 (2).bmp",
			},
	};
#else
	//#need update! add index & use index 
	//24 * 32 img wc : 12, hc : 8
	const char* path[4] = {
		"assets/Image/CharSet/마리사 공격모션 (1).bmp",
		"assets/Image/CharSet/마을 주민(1).bmp",
		"assets/Image/CharSet/메이라 공격모션 (1).bmp",
		"assets/Image/CharSet/모코우 공격모션 (1).bmp",
	};

#endif
	imgs = new iImage * [BehaveNum];

#if HERO
	for (int i = 0; i < BehaveNum; i++)
	{
		Texture* tex = createImageAlpha(path[index][i]);
		iImage* img = new iImage();
		img->reverse = REVERSE_WIDTH;
		img->addObject(tex);
		freeImage(tex);

		imgs[BeWait + i] = img;
	}
#else
	Texture** texs = createImageAlphaDivide(12, 8, path[0]);

	for (int j = 0; j < BehaveNum; j++)
	{
		iImage* img = new iImage();
		if (j == BeAttack)
		{
			//
			// BeAttack
			//
			for (int i = 0; i < 8; i++)
				img->addObject(texs[i]);
			img->startAnimation();
			img->_delta = 0.1f;
			imgs[BeAttack] = img;
		}
		
		else //if (j == BeWait)
		{
			//
			// BeWait
			//
			img->addObject(texs[0]);
			imgs[BeWait + j] = img;
		}
	}
	
	for (int i = 0; i < 12 * 8; i++)
		freeImage(texs[i]);
	delete texs;

#endif
	imgNum = BehaveNum;
	be = BeWait;
	imgCurr = imgs[BeWait];

	_attDt = HERO_COOL_TIME;

	enemyIndex = -1;

	bp = new BattlePop();

//#need update! temp code
#if 1
	bp->showPopBattle(false);
#endif
}

BUHero::~BUHero()
{
	delete bp;
}

float BUHero::update(float dt)
{
	//to do...
	//printf("Hero Time\n");
	return BattleUnit::update(dt);
}

bool BUHero::paint(float dt, iPoint off)
{
	float atkTime = 0.0f;

	iPoint tp = iPointZero;

	bool dead = false; //default;

	float imgW = imgCurr->tex->width;
	float imgH = imgCurr->tex->height;

	iPoint center = off + iPointMake(-imgW / 2, -imgH / 2) * HERO_RATE;

	bp->paint(dt, position + iPointMake(0, -65));

	if (target && attacking)
	{
		//move
		if (state == 0)
		{
			attAniDt += dt;

			if (attAniDt >= HERO_MOVE_TIME)
			{
				attAniDt = HERO_MOVE_TIME;
				imgCurr = imgs[BeWait];
				attAniDt = 0.0f;
				state = 1;
			}
			iPoint tmpP = target->position - position - iPointMake(-50, 0);
			tp = easeIn(attAniDt / HERO_MOVE_TIME, position - position, tmpP);

			dead = BattleUnit::paint(dt, center + tp, HERO_RATE);

			return dead;
		}

		//attack
		else if (state == 1)
		{
			attAniDt += dt;

			if (attAniDt >= HERO_ATK_TIME)
			{
				attAniDt = HERO_ATK_TIME;
				imgCurr = imgs[BeWait];
				attAniDt = 0.0f;
				state = 2;
				target->hp -= atk;
				if (target->hp < 0)
					target->hp = 0;
			}
			else if (attAniDt >= HERO_ATK_TIME / 2)
			{
				imgCurr = imgs[BeAttack];
			}
			iPoint tmpP = target->position - position - iPointMake(-50, 0);
			tp = linear(attAniDt / (HERO_ATK_TIME), tmpP, tmpP);

			dead = BattleUnit::paint(dt, center + tp, HERO_RATE);

			return dead;
		}

		//back
		else if (state == 2)
		{
			attAniDt += dt;

			if (attAniDt >= HERO_BACK_TIME)
			{
				attAniDt = HERO_BACK_TIME;
				imgCurr = imgs[BeWait];
				state = 0;
				attAniDt = 0.0f;
				bm->attacking = false;
				attacking = false;
			}
			iPoint tmpP = target->position - position - iPointMake(-50, 0);
			tp = easeIn(attAniDt / (HERO_BACK_TIME), tmpP, position - position);

			dead = BattleUnit::paint(dt, center + tp, HERO_RATE);

			return dead;
		}
	}

	else
	{
		dead = BattleUnit::paint(dt, center, HERO_RATE);
//#need update! add tp position
		state = 0;
		attAniDt = 0.0f;

		return dead;
	}

	//그려줄거 다 그려준 후

	//overlay...drawing;;;
	//if(enemyIndex == -1)
	//{
	// 화살표
	//}

	return dead;
}

void BUHero::attack()
{
	//to do...
	//맞는 놈 선택 

	//키 처리 대기중
	//if(enemyIndex == -1)
	//{
	//	//ui에서 때리는 놈 지정 대기 상태
	//}
	//else

	if (bm->enemyNum > 0)
		target = (BattleUnit*)bm->enemy[random() % bm->enemyNum];

	BattleUnit::attack();
}

bool BUHero::keyHero(iKeyStat stat, iPoint point)
{
	bool result = false;

	switch (stat)
	{
	case iKeyStatBegan:
		iSize imgSize = iSizeMake(imgCurr->tex->width, imgCurr->tex->height);
		if(containPoint(point, imgCurr->touchRect(position, imgSize)))
			result = bp->keyPopBattle(stat, point);
		break;
	case iKeyStatMoved:
		break;
	case iKeyStatEnded:
		break;
	}
		
	return result;
}

//================================================================
// BattleManager
//================================================================

BattleManager::BattleManager()
{
	_enemyNum = MAX_ENEMY;
	_heroNum = MAX_HERO;

	enemyNum = (random() % _enemyNum) + 1;
	heroNum = (random() % _heroNum) + 1;

#if 1
	int enemyType = 0;
	int heroType = 0;
	enemy = new BUMonster * [enemyNum];
	for (int i = 0; i < enemyNum; i++)
	{
		int type = 0;
		enemy[i] = new BUMonster(type);
	}

	hero = new BUHero * [heroNum];
	for (int i = 0; i < heroNum; i++)
	{
		int type = 0;
		hero[i] = new BUHero(type);
	}

#endif

	attacking = false;
}

BattleManager::~BattleManager()
{
	for (int i = 0; i < enemyNum; i++)
		delete enemy[i];
	delete enemy;

	for (int i = 0; i < heroNum; i++)
		delete hero[i];
	delete hero;
}

void BattleManager::paint(float dt)
{
	iPoint off = iPointMake(devSize.width / 2, devSize.height / 2 - 120);

	int i;

	//
	//Enemy
	//
	for (i = 0; i < enemyNum; i++)
	{
		iPoint enemP;
		int offWidth = -250;
		if (enemyNum == 1)
			enemP = iPointMake(-250, 0);
		else if (enemyNum == 2)
		{
			if (i == 0)			enemP = iPointMake(offWidth, -40);
			else				enemP = iPointMake(offWidth, 40);
		}
		else if (enemyNum == 3)
		{
			if (i == 0)			enemP = iPointMake(offWidth, -80);
			else if (i == 1)	enemP = iPointMake(offWidth, 0);
			else				enemP = iPointMake(offWidth, 80);
		}
		else if (enemyNum == 4)
		{
			if (i == 0)			enemP = iPointMake(offWidth, -120);
			else if (i == 1)	enemP = iPointMake(offWidth, -40);
			else if (i == 2)	enemP = iPointMake(offWidth, 40);
			else				enemP = iPointMake(offWidth, 120);
		}
		else
			enemP = iPointZero;

		enemy[i]->position = off + enemP;
		if (enemy[i]->paint(dt, iPointZero))
		{
			enemyNum--;
			enemy[i] = enemy[enemyNum];
			i--;
		}
	}

	//
	//Hero
	//

	for (i = 0; i < heroNum; i++)
	{
		iPoint heroP;
		int offWidth = 250;
		if (heroNum == 1)
			heroP = iPointMake(250, 0);
		else if (heroNum == 2)
		{
			if (i == 0)			heroP = iPointMake(offWidth, -40);
			else				heroP = iPointMake(offWidth, 40);
		}
		else if (heroNum == 3)
		{
			if (i == 0)			heroP = iPointMake(offWidth, -80);
			else if (i == 1)	heroP = iPointMake(offWidth, 0);
			else				heroP = iPointMake(offWidth, 80);
		}
		else if (heroNum == 4)
		{
			if (i == 0)			heroP = iPointMake(offWidth, -120);
			else if (i == 1)	heroP = iPointMake(offWidth, -40);
			else if (i == 2)	heroP = iPointMake(offWidth, 40);
			else				heroP = iPointMake(offWidth, 120);
		}
		else
			heroP = iPointZero;
	
		hero[i]->position = off + heroP;
		if (hero[i]->paint(dt, iPointZero))
		{
			heroNum--;
			hero[i] = hero[heroNum];
			i--;
		}
	}


	if (enemyNum == 0)
	{
		//win
		printf("전투승리!\n");

		//to do... 승리 팝업
		setLoading(gs_stage, freeBattle, loadStage);
	}

	else if (heroNum == 0)
	{
		//lose
		printf("전투패배!\n");

		//to do... 패배 팝업
		setLoading(gs_ending, freeBattle, loadEnding);
	}

	BattleUnit* bu = NULL;
	float attRate = 0.0f;
	float t = 0.0f;

	//key 처리
#if 1
	for (i = 0; i < heroNum; i++)
	{
		t = hero[i]->update(dt);
		if (attRate < t)
		{
			bu = hero[i];
			attRate = t;
		}
	}
#endif
	for (i = 0; i < enemyNum; i++)
	{
		t = enemy[i]->update(dt);
		if (attRate < t)
		{
			bu = enemy[i];
			attRate = t;
		}
	}

	if (attacking)
		return;
	
	if (bu)
	{
		bu->attack();
		attacking = true;
		bu->attacking = true;
	}
}

// =============================================================
// TimeRate 
// =============================================================

TimeRate::TimeRate()
{
	iGraphics* g = iGraphics::share();
	iSize size = iSizeMake(101, 25);
	g->init(size);

	setRGBA(0, 0, 0, 1);
	g->fillRect(0, 0, size.width, size.height, 5);

	texBg = g->getTexture();

	texTick = new Texture * [3];
	for (int i = 0; i < 3; i++)
	{
		size = iSizeMake(8, 25);
		g->init(size);
		if (i == 0)			setRGBA(0.5, 0.5, 0.5, 1); 
		else if(i==1)		setRGBA(0.8, 0.8, 0.0, 1);
		else				setRGBA(0.0, 1.0, 0.0, 1);
		g->fillRect(0, 0, size.width, size.height, 3);
		setRGBA(1, 1, 1, 1);

		texTick[i] = g->getTexture();
	}
}

TimeRate::~TimeRate()
{
	freeImage(texBg);
	for (int i = 0; i < 3; i++)
		freeImage(texTick[i]);
	delete texTick;
}

void TimeRate::paint(float dt, iPoint off, int num)
{
	drawImage(texBg, off.x, off.y, TOP | LEFT);
	for (int i = 0; i < 10; i++)
	{
		Texture* tex;
		if(num == 10) 
			tex = texTick[2];
		else		
			tex = texTick[i < num ? 1 : 0];
		drawImage(tex, off.x + 1 + 10 * i, off.y, TOP | LEFT);
	}
}

void TimeRate::paint(float dt, iPoint off, float attDt, float _attDt)
{
	int num = attDt / _attDt * 10;
	if (num > 10)
		num = 10;
			
	paint(dt, off, num);
}

// =============================================================
// BattlePop
// =============================================================


//#issue! 현재 imgBtns는 따로 처리해줘야함. (drawBefore 메소드를 사용할 수 없음)

BattlePop::BattlePop()
{
	popNum = 1; //화살표, 팝업(공격, 방어)
	show = false;
	popBattle = new iPopup * [popNum];

	iPopup* pop = new iPopup();

	Texture* tex;
	iImage* img;

	//----------------------------------------------------------
	// 화살표
	//----------------------------------------------------------

	iGraphics* g = iGraphics::share();
	iSize size = iSizeMake(20, 20);
	g->init(size);
	
	igImage* igImg = g->createIgImage("assets/Image/arrow1.png");
	for (int i = 0; i < 2; i++)
	{
		if (i == 0)
		{
			setRGBA(1, 1, 1, 0);
			g->fillRect(0, 0, size.width, size.height);
		}
		else 
			g->drawigImage(igImg, size.width / 2, size.height / 2, VCENTER | HCENTER, 
				0, 0, igImg->GetWidth(), igImg->GetHeight(), 0.5f, 0.5f, 2.0f, 0);
	}
	
	tex = g->getTexture();
	img = new iImage();
	img->addObject(tex);
	freeImage(tex);
	pop->addObject(img);
	
	//----------------------------------------------------------
	// 공격, 방어
	//----------------------------------------------------------
	imgBtns = new iImage * [4];

	//
	//Bg
	//

	size = iSizeMake(125, 25);
	g->init(size);

	setRGBA(0, 0, 0, 0.8);
	g->fillRect(0, 0, size.width, size.height, 10);
	setRGBA(1, 1, 1, 1);
	g->drawRect(0, 0, size.width, size.height, 10);

	img = new iImage();
	tex = g->getTexture();
	img->addObject(tex);
	freeImage(tex);
	img->position = iPointMake(0, -25); //화살표보단 조금 위에
	pop->addObject(img);

	popBattle[0] = pop;
	
	//
	//Btn
	//

	for (int i = 0; i < 4; i++)
	{
		size = iSizeMake(20, 20);
		
		for (int j = 0; j < 2; j++)
		{
			g->init(size);
			img = new iImage();
			if (j == 0) setRGBA(0.5f, 0.5f, 0.5f, 1);
			else		setRGBA(1, 1, 0, 1);
			g->fillRect(0, 0, size.width, size.height, 3);
			setRGBA(1, 1, 1, 1);

			tex = g->getTexture();
			img->addObject(tex);
			freeImage(tex);
		}
		img->position = iPointMake(5 + (5+25*i), -25 - 25/2);
		imgBtns[i] = img;
	}

	//----------------------------------------------------------
	// 적에게 활성화 되는 화살표 (공격버튼 눌렀을때 적용)
	//----------------------------------------------------------


}

BattlePop::~BattlePop()
{
	for (int i = 0; i < popNum; i++)
		delete popBattle[i];
	delete popBattle;
}


void BattlePop::paint(float dt, iPoint off)
{
	for (int i = 0; i < popNum; i++)
	{
		popBattle[i]->openPoint = off;
		popBattle[i]->closePoint = off;

		popBattle[i]->paint(dt);
	}

	iPoint btnP = iPointMake(0, -25);
	for (int i = 0; i < 4; i++)
	{
		imgBtns[i]->paint(dt, off, iPointOne);
	}
}

void BattlePop::showPopBattle(bool check)
{
	for (int i = 0; i < popNum; i++)
		popBattle[i]->show(check);
}

bool BattlePop::keyPopBattle(iKeyStat stat, iPoint point)
{
	int j = -1;

	switch (stat)
	{
		
	case iKeyStatBegan:
	{
//#need update! 공격상태창까지 고려하면 이렇게 짜면 안됨.
		show = !show;
		popBattle[0]->bShow = show;
		showPopBattle(show);
		return true;
	}
		break;

	case iKeyStatMoved:
		
		for (int i = 0; i < 4; i++)
		{
			if (containPoint(point, imgBtns[i]->touchRect(popBattle[0]->closePoint)))
			{
				j = i;
				break;
			}
		}
		if (popBattle[0]->selected != j)
		{
			popBattle[0]->selected = j;
		}
		break;

	case iKeyStatEnded:
		break;
	}
	return false;
}
