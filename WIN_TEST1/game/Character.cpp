#include "Character.h"

#include "iImage.h"
#include "Map.h"

extern bool warpEvent;

//먼저는 이미지만 그려보자. 
//후에 Hero 파일같이 스프라이트 애니메이션 작업 ㄱ

enum Behavior
{
	BehaveWait = 0,

	BehaveWalkLeft,
	BehaveWalkRight,
	BehaveWalkUp,
	BehaveWalkDown,

	BehaveMax,
};


class Character
{
public:
	Character()
	{
		int type = 1;
		
		iPoint startP = iPointMake(4, 0);
		
		Texture** texs = createImageDivide(12, 8, "assets/Image/character%d.png", type);
		//24 * 32 캐릭 이미지를 들고와야함.
	
		Texture* hero[12];

		//king, queen, jack, maid, jack2, knight, hero1, hero2
		int idxW = 0, idxH = 0; //이거에 따라 캐릭이 바뀜
		int i, j;

		int heroIndex[12], num = 0;
		for (j = idxH; j < idxH * 4 + 4; j++)
		{
			if (num >= 12) break;
			for (i = idxW; i < idxW * 3 + 3; i++)
			{
				heroIndex[num] = i + j * 12;
				num++;
			}
		}

		for (i = 0; i < 12; i++)
			hero[i] = texs[heroIndex[i]];

		imgs = new iImage * [BehaveMax];

		iImage* img = new iImage();
		img->addObject(hero[7]);
		img->_delta = 0.018f;
		img->position = startP;
		img->startAnimation(); //1장이라 없어도 될듯...
		imgs[BehaveWait] = img;

		int walkLeftIndex[4] = {9,10,11,10};
		img = new iImage();
		for (i = 0; i < 4; i++)
			img->addObject(hero[walkLeftIndex[i]]);
		img->_delta = 0.018f;
		img->position = startP;
		img->startAnimation();
		imgs[BehaveWalkLeft] = img;
		imgs[BehaveWalkRight] = imgs[BehaveWalkLeft]->copy();
		imgs[BehaveWalkRight]->reverse = REVERSE_WIDTH;
		
		int walkUpIndex[4] = { 0,1,2,1 };
		img = new iImage();
		for (i = 0; i < 4; i++)
			img->addObject(hero[walkUpIndex[i]]);
		img->_delta = 0.018f;
		img->position = startP;
		img->startAnimation();
		imgs[BehaveWalkUp] = img;

		int walkDownIndex[4] = { 6,7,8,7 };
		img = new iImage();
		for (i = 0; i < 4; i++)
			img->addObject(hero[walkDownIndex[i]]);
		img->_delta = 0.018f;
		img->position = startP;
		img->startAnimation();
		imgs[BehaveWalkDown] = img;

		for (i = 0; i < 12; i++)
			freeImage(hero[i]);

		be = BehaveWait;
		//leftRigt = 0;
		imgCurr = imgs[be];
		
	}
	virtual ~Character()
	{
		for (int i = 0; i < BehaveMax; i++)
			delete imgs[i];
		delete imgs;
	}

	void paint(float dt)
	{
		imgCurr->paint(dt, position, iPointOne);
	}

public:
	iImage** imgs;
	iImage* imgCurr;

	Behavior be;
	int leftRight;
	iPoint position;

};

Character* mainCharacter;

//void loadCharacter(iPoint off)
void loadCharacter(iPoint off)
{
	mainCharacter = new Character();
	mainCharacter->position += off;
}

void drawCharacter(float dt)//(float dt, MapTile* currMap)
{

	//MapTile* map = currMap;

	iPoint v = iPointZero;

	//int x = mainCharacter->position.x - map->off.x; x /= tileWSize;
	//int y = mainCharacter->position.y - map->off.y; y /= tileHSize;


	int x = mainCharacter->position.x; x /= tileWSize;
	int y = mainCharacter->position.y; y /= tileHSize;
	int xy = tileW * y + x; //타일의 인덱스 값

	int check, wpCheck;
	

	if (keyStat & keysA)
	{
		printf("left\n");
		//check = map->tile_map[xy - 1].value;
		//if (x > 0 && (check == 0 || check == 1))
		if (x > 1)
		{
			//warp일때 씬전환이 일어나는 변수를 조작해줘야함
			//if (check == 1)
			//	warpEvent = true;

			v.x = -1;
			mainCharacter->be = BehaveWalkLeft;
			mainCharacter->leftRight = 0;
			mainCharacter->imgCurr = mainCharacter->imgs[BehaveWalkLeft];
		}
	}

	else if (keyStat & keysD)
	{
		printf("right\n");
		//check = map->tile_map[xy + 1].value;
		//if (x < tileW - 1 && (check == 0 || check == 1))
		if (x < tileW - 2)
		{
			//warp일때 씬전환이 일어나는 변수를 조작해줘야함
			//if (check == 1)
			//	warpEvent = true;

			v.x = 1;
			mainCharacter->be = BehaveWalkRight;
			mainCharacter->leftRight = 1;
			mainCharacter->imgCurr = mainCharacter->imgs[BehaveWalkRight];
		}
	}

	else if (keyStat & keysW)
	{
		printf("Up\n");
		//check = map->tile_map[xy - tileW].value;
		//if (y > 0 && (check == 0 || check == 1))
		if (y > 1)
		{
			//warp일때 씬전환이 일어나는 변수를 조작해줘야함
			//if (check == 1)
			//	warpEvent = true;

			v.y = -1;
			mainCharacter->be = BehaveWalkUp;
			mainCharacter->imgCurr = mainCharacter->imgs[BehaveWalkUp];
		}
	}

	else if (keyStat & keysS)
	{
		printf("Down\n");
		//check = map->tile_map[xy + tileW].value;
		//if (y < tileH - 1 && (check == 0 || check == 1))
		if (y < tileH - 2)
		{
			//warp일때 씬전환이 일어나는 변수를 조작해줘야함
			//if (check == 1)
			//	warpEvent = true;

			v.y = 1;
			mainCharacter->be = BehaveWalkDown;
			mainCharacter->imgCurr = mainCharacter->imgs[BehaveWalkDown];
		}
	}

	if (v != iPointZero) //move
	{
		v /= iPointLength(v);
		mainCharacter->position += v * 32; //타일크기만큼 움직이므로 dt 영향 x
		printf("now charcter pos : %.2f %.2f\n", mainCharacter->position.x, mainCharacter->position.y);
	}
	else //wait
	{
		mainCharacter->imgCurr = mainCharacter->imgs[BehaveWait];
	}

	mainCharacter->paint(dt);
	
}

void freeCharacter()
{
	delete mainCharacter;
	
}

void keyCharacter(iKeyStat stat, iPoint point)
{
	
}


