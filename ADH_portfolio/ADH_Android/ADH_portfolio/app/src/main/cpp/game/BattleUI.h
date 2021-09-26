#pragma once

#include "iStd.h"

//========================================================
// popTopUI
//========================================================

void createBattlePopTopUI();
void freeBattlePopTopUI();
void showBattlePopTopUI(bool show);
void drawBattlePopTopUI(float dt, iPoint off);
bool keyBattlePopTopUI(iKeyStat stat, iPoint point);

//========================================================
// popBottomUI : 전투메뉴(공격, 방어, 아이템, 도망) / Hero 체력, 마력...
//========================================================

void createBattlePopBottomUI();
void freeBattlePopBottomUI();
void showBattlePopBottomUI(bool show);
void drawBattlePopBottomUI(float dt, iPoint off);

enum BehaveState
{
	BeWait = 0,
	BeMoved,
	BeAttack,
	BeDefense,
	BeRun,

	BehaveNum,
};

struct BattleManager;
extern BattleManager* bm;

#define ENEMY_COOL_TIME 4.0f
#define ENEMY_MOVE_TIME 1.0f
#define ENEMY_ATK_TIME 2.0f
#define ENEMY_BACK_TIME 1.0f

#define HERO_COOL_TIME 3.0f
#define HERO_MOVE_TIME 0.7f
#define HERO_ATK_TIME 1.5f
#define HERO_BACK_TIME 1.0f

#define WAIT_STATE_TIME 0.5f

//#need update! if change Hero img rate
#define HERO_RATE iPointMake(2.2f, 2.2f)

class BattlePop;

struct BattleUnit
{
	BattleUnit(int index);
	virtual ~BattleUnit();
	
	virtual float update(float dt);
	virtual bool paint(float dt, iPoint off) = 0;
	virtual bool paint(float dt, iPoint off, iPoint rate) = 0;
	virtual bool paint(float dt, iPoint off, float rx, float ry) = 0;
	virtual void attack();
	

	int index;
	iImage** imgs;
	iImage* imgCurr;
	iPopup* BuPop;
	BehaveState be;
	int imgIndex, imgNum;
	iPoint position, _position;
	int hp, mp, atk;

	float attDt, _attDt;
	float attAniDt, _attAniDt;
	BattleUnit* target;
	bool attacking;
	int state; //animation state : move, attack, back
};

struct BUMonster : BattleUnit
{
	BUMonster(int index);
	virtual ~BUMonster();

	virtual float update(float dt);
	virtual bool paint(float dt, iPoint off);
	virtual bool paint(float dt, iPoint off, iPoint rate);
	virtual bool paint(float dt, iPoint off, float rx, float ry);
	virtual void attack();
};

struct BUHero : BattleUnit
{
	BUHero(int index);
	virtual ~BUHero();
	virtual float update(float dt);
	virtual bool paint(float dt, iPoint off);
	virtual bool paint(float dt, iPoint off, iPoint rate);
	virtual bool paint(float dt, iPoint off, float rx, float ry);
	virtual void attack();
	virtual bool keyHero(iKeyStat stat, iPoint p);
	
	int enemyIndex;// default -1
	
	BattlePop* bp;
};

#define MAX_ENEMY 4
#define MAX_HERO 1

struct BattleManager
{
	BattleManager();
	~BattleManager();

	void paint(float dt);

	BUMonster** enemy, *_enemy;
	int enemyNum, _enemyNum;
	BUHero** hero, * _hero;
	int heroNum, _heroNum;

	bool attacking;
};
/*
#need update!
타일 가중치에 따른 몬스터 타입 제작 (툴 + BattleManager)
*/

//=============================================
// TimeRate
//=============================================

class TimeRate
{
public:
	TimeRate();
	virtual ~TimeRate();

	void paint(float dt, iPoint off, int num);
	void paint(float dt, iPoint off, float attDt, float _attDt);
public:
	Texture* texBg;
	Texture** texTick;
};

