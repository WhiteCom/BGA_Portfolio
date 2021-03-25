#include "Character.h"

#include "iImage.h"

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
		int type = 0;
		
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
		img->position = iPointZero;
		img->startAnimation(); //1장이라 없어도 될듯...
		imgs[BehaveWait] = img;

		int walkLeftIndex[4] = {9,10,11,10};
		img = new iImage();
		for (i = 0; i < 4; i++)
			img->addObject(texs[walkLeftIndex[i]]);
		img->_delta = 0.018f;
		img->position = iPointZero;
		img->startAnimation();
		imgs[BehaveWalkLeft] = img;
		imgs[BehaveWalkRight] = imgs[BehaveWalkLeft]->copy();
		imgs[BehaveWalkRight]->reverse = REVERSE_WIDTH;
		
		int walkUpIndex[4] = { 0,1,2,1 };
		img = new iImage();
		for (i = 0; i < 4; i++)
			img->addObject(texs[walkUpIndex[i]]);
		img->_delta = 0.018f;
		img->position = iPointZero;
		img->startAnimation();
		imgs[BehaveWalkUp] = img;

		int walkDownIndex[4] = { 6,7,8,7 };
		img = new iImage();
		for (i = 0; i < 4; i++)
			img->addObject(texs[walkUpIndex[i]]);
		img->_delta = 0.018f;
		img->position = iPointZero;
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

void drawHero()
{

}
void freeHero()
{

}
bool moveCheck(MapTile* map) //맵의 가중치를 보고 움직일수없다 여부 판단
{
	return false;
}



