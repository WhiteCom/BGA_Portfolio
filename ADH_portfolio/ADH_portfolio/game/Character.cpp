#include "Character.h"

#include "iImage.h"
#include "TileType.h"
#include "Map.h"
#include "Loading.h"
#include "Stage.h"

bool warpEvent;

Character::Character()
{
	iPoint startP = iPointMake(0, -20);

	//24 * 32 imgs
#if 0 //to do...
	//Texture** texs = createImageAlphaDivide(12, 8, "assets/Image/CharSet/[瑛] 마리사 모션 (1).bmp");
#else

#if (OS==OS_WINDOW)
	Texture** texs = createImageDivide(12, 8, "assets/Image/CharSet/Marisa1.bmp");
#elif (OS==OS_ANDROID)
	Texture** texs = createImageAlphaDivide(12, 8, "Image/CharSet/[瑛] 마리사 모션 (1).bmp");
#endif //OS
#endif

	Texture* hero[24];

	int i, j;
	int heroIndex[24], num = 0;
	for (j = 0; j < 4; j++)
	{
		if (num >= 24) break;
		for (i = 0; i < 6; i++)
		{
			heroIndex[num] = i + j * 12;
			num++;
		}
	}

	for (i = 0; i < 24; i++)
		hero[i] = texs[heroIndex[i]];

	//
	//BehavWait
	// 

	imgs = new iImage * [BehaveMax];

	iImage* img = new iImage();
	img->addObject(hero[13]);
	img->position = startP;
	imgs[BehaveWait] = img;

	//
	//BehaveWalkLeft
	//
	int leftWalk[WALK_NUM] = { 18, 19, 20, 21, 22, 23 };
	img = new iImage();
	for (i = 0; i < WALK_NUM; i++)
		img->addObject(hero[leftWalk[i]]);
	img->_delta = 0.1f;
	img->position = startP;
	img->startAnimation();
	imgs[BehaveWalkLeft] = img;

	//
	//BehaveWalkRight
	//
	imgs[BehaveWalkRight] = imgs[BehaveWalkLeft]->copy();
	imgs[BehaveWalkRight]->reverse = REVERSE_WIDTH;

	//
	//BehaveWalkUp
	//
	int upWalk[WALK_NUM] = { 0,1,2,3,4,5 };
	img = new iImage();
	for (i = 0; i < WALK_NUM; i++)
		img->addObject(hero[upWalk[i]]);
	img->_delta = 0.1f;
	img->position = startP;
	img->startAnimation();
	imgs[BehaveWalkUp] = img;

	//
	//BehaveWalkDown
	//
	int downWalk[WALK_NUM] = { 12, 13, 14, 15, 16, 17 };
	img = new iImage();
	for (i = 0; i < WALK_NUM; i++)
		img->addObject(hero[downWalk[i]]);
	img->_delta = 0.1f;
	img->position = startP;
	img->startAnimation();
	imgs[BehaveWalkDown] = img;

	for (i = 0; i < 24; i++)
	{
		freeImage(hero[i]);
	}

	be = BehaveWait;
	imgCurr = imgs[be];

	moveDt = 0.0f;
	_moveDt = MOVE_TIME;
	position = tPosition = iPointZero;
}

Character::~Character()
{
	for (int i = 0; i < BehaveMax; i++)
		delete imgs[i];
	delete imgs;
}

void Character::paint(float dt, iPoint pos)
{
	iPoint p = position;
	if (moveDt < _moveDt)
	{
		moveDt += dt;
		if (moveDt >= _moveDt)
		{
			moveDt = _moveDt;
			position = tPosition;
		}
		p = linear(moveDt / _moveDt, position, tPosition);

	}

#if 0 //적당한 사이즈의 이미지를 찾았을 경우
	imgCurr->paint(dt, p - iPointMake(0, TILE_HSIZE / 2 - 6), iPointOne);
#else
	imgCurr->paint(dt, p - iPointMake(0, 0), iPointMake(1.3f, 1.3f));
#endif

}

Character* mainCharacter;
Map* map;

void loadCharacter(iPoint off, const char* str)
{
	mainCharacter = new Character();
	map = new Map();
	map->load(str);
	mainCharacter->position += off;
	mainCharacter->tPosition = mainCharacter->position;
	mainCharacter->moveDt = 0.0f;
}

int charDir = -1;

void drawCharacter(float dt, iPoint off)
{
	//Step Over

	iPoint v = iPointZero;
	int x = mainCharacter->position.x; x /= TILE_WSIZE;
	int y = mainCharacter->position.y; y /= TILE_HSIZE;
	int _x = off.x; _x /= TILE_WSIZE;
	int _y = off.y; _y /= TILE_HSIZE;
	int xy = TILE_W * (y - _y) + (x - _x); //타일의 인덱스 값

	int check;

#if (OS==OS_WINDOW)
	if (loadingDt == 0.0f &&
		mainCharacter->moveDt == MOVE_TIME &&
		step > 0)
	{
		if (keyDown & keysA || keyDown & keysLeft)
		{
			printf("left\n");
			charDir = 0;
		}
		else if (keyDown & keysD || keyDown & keysRight)
		{
			printf("right\n");
			charDir = 1;
		}
		else if (keyDown & keysW || keyDown & keysUp)
		{
			printf("Up\n");
			charDir = 2;
		}
		else if (keyDown & keysS || keyDown & keysDown)
		{
			printf("Down\n");
			charDir = 3;
		}
	}
#elif (OS==OS_ANDROID)
	// keyStage에서 처리
#endif
	if (charDir == 0)
	{
		mainCharacter->moveDt = 0.0f;
		mainCharacter->_moveDt = MOVE_TIME;
		check = map->tileWeight[xy - 1];

		if (x > 0 + _x && check != 9)
		{
			v.x = -1;
			mainCharacter->be = BehaveWalkLeft;
			mainCharacter->leftRight = 0;
			mainCharacter->imgCurr = mainCharacter->imgs[BehaveWalkLeft];
		}
	}

	else if (charDir == 1)
	{
		mainCharacter->moveDt = 0.0f;
		mainCharacter->_moveDt = MOVE_TIME;
		check = map->tileWeight[xy + 1];

		if (x < TILE_W - 1 + _x && check != 9)
		{
			v.x = 1;
			mainCharacter->be = BehaveWalkRight;
			mainCharacter->leftRight = 1;
			mainCharacter->imgCurr = mainCharacter->imgs[BehaveWalkRight];
		}
	}

	else if (charDir == 2)
	{
		mainCharacter->moveDt = 0.0f;
		mainCharacter->_moveDt = MOVE_TIME;
		check = map->tileWeight[xy - TILE_W];

		if (y > 0 + _y && check != 9)
		{
			v.y = -1;
			mainCharacter->be = BehaveWalkUp;
			mainCharacter->imgCurr = mainCharacter->imgs[BehaveWalkUp];
		}
	}

	else if (charDir == 3)
	{
		mainCharacter->moveDt = 0.0f;
		mainCharacter->_moveDt = MOVE_TIME;
		check = map->tileWeight[xy + TILE_W];

		if (y < TILE_H - 1 + _y && check != 9)
		{
			v.y = 1;
			mainCharacter->be = BehaveWalkDown;
			mainCharacter->imgCurr = mainCharacter->imgs[BehaveWalkDown];
		}
	}

	float mDt = mainCharacter->moveDt;
	float _mDt = mainCharacter->_moveDt;
	if (mDt < _mDt)
	{
		if (v != iPointZero) //move
		{
			v /= iPointLength(v);
			mainCharacter->tPosition += v * 32; //타일크기만큼 움직이므로 dt 영향 x
#if (OS==OS_WINDOW)
			printf("now charcter pos : %.2f %.2f\n", mainCharacter->position.x, mainCharacter->position.y);
#elif(OS==OS_ANDROID)
			loge("now charcter pos : %.2f %.2f\n", mainCharacter->position.x, mainCharacter->position.y);
#endif
		}
		mainCharacter->paint(dt, off);
	}

	else //wait
	{
		mainCharacter->be = BehaveWait;
		mainCharacter->imgCurr = mainCharacter->imgs[BehaveWait];
		mainCharacter->paint(dt, off);
	}

}

void freeCharacter()
{
	delete mainCharacter;
	delete map;
}

void keyCharacter(iKeyStat stat, iPoint point)
{
	//to do...
}

