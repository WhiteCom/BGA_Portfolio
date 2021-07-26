#pragma once

#include "iStd.h"
#include "Map.h"

#define WALK_NUM 6

enum Behavior
{
	BehaveWait = 0,

	BehaveWalkLeft,
	BehaveWalkRight,
	BehaveWalkUp,
	BehaveWalkDown,

	BehaveMax,
};

#define MOVE_TIME 0.2f

class Character
{
public:
	Character();

	virtual ~Character();
	void paint(float dt, iPoint pos = iPointZero);

public:
	iImage** imgs;
	iImage* imgCurr;

	Behavior be;
	int leftRight;
	float moveDt, _moveDt;
	iPoint position, tPosition;
	iPoint off;
};

extern Character* mainCharacter;
extern Map* map;
extern int step;

//void loadCharacter(iPoint off);
//void drawCharacter(float dt, MapTile* currMap);
void loadCharacter(iPoint off, const char* str);
void drawCharacter(float dt, iPoint off);
void freeCharacter();
void keyCharacter(iKeyStat stat, iPoint point);