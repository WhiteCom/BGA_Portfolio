#include "Intro.h"

#include "Loading.h"
#include "Map3.h"

Texture* texIntroBg;

iImage* imgIntro;



void loadIntro()
{
	texIntroBg = createImage("assets/proc.jpg");

	//imgIntro = new iImage(); //애니메이션용
	
}

void freeIntro()
{
	freeImage(texIntroBg);
}

void drawIntro(float dt)
{
	setRGBA(0.5f, 0.5f, 0.5f, 1.0f);
	clearRect();

	drawImage(texIntroBg, 0, 0, TOP | LEFT);

	if (keyStat == keysSpace)
		setLoading(gs_map2, freeIntro, loadMap);
}

void keyIntro(iKeyStat stat, iPoint point)
{
	switch(stat)
	{
		case iKeyStatBegan:
			setLoading(gs_map2, freeIntro, loadMap);
			break;
		case iKeyStatMoved:
			break;
		case iKeyStatEnded:
			break;
	}


}