#include "Character.h"

#include "iImage.h"

//먼저는 이미지만 그려보자. 
//후에 Hero 파일같이 스프라이트 애니메이션 작업 ㄱ

enum Behavior
{
	BehaveLeft = 0,
	BehaveRight,
	BehaveUp,
	BehaveDown,

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

		for (int i = 0; i < 12; i++)
			hero[i] = texs[heroIndex[i]];

		imgs = new iImage * [BehaveMax];

		iImage* img = new iImage();
		//#need update
		
	}
	~Character()
	{
		delete imgs;
		delete imgCurr;
	}

public:
	iImage** imgs;
	iImage* imgCurr;

	Behavior be;
	int leftRight;
	iPoint position;

};




