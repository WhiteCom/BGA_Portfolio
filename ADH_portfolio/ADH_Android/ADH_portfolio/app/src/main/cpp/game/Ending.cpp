#include "Ending.h"
#include "Loading.h"
#include "Menu.h"

//const char* strEnding = "취업 성공을 위해서 열심히 준비했습니다. 누가 보면 부족해 보일지 모르나, 저는 제가 이룬것에 대해 아주 만족합니다. 화이팅";

Texture* gameOverBg;

#define _VIEWTIME 4.f

float viewTime;


void loadEnding()
{
#if 0 //#openAL
	for (int i = 0; i < SOUND_NUM; i++)
		audioStop(i);
#endif

	gameOverBg = createImage("assets/gameOverBg.png");

	viewTime = 1.0f;
}
void freeEnding()
{
	freeImage(gameOverBg);
}

void drawEnding(float dt)
{
	if (viewTime == 0.0f)
		return;

	float a = 1.0f;

	if (viewTime < _VIEWTIME)
	{
		viewTime += dt;
		if (viewTime > _VIEWTIME)
			viewTime = _VIEWTIME;
		a = viewTime / _VIEWTIME;
	}

	else if (viewTime == _VIEWTIME)
	{
		viewTime = _VIEWTIME + 0.1f;

		setLoading(gs_menu, freeEnding, loadMenu);

		a = 1.0f; //완전 까맣게
	}

	else if (viewTime <= _VIEWTIME * 2)
	{
		viewTime += dt;
		if (viewTime > _VIEWTIME * 2)
		{
			viewTime = 0.0f;
			freeImage(gameOverBg);
			return;
		}
		a = 1.0f - (viewTime - _VIEWTIME) / viewTime; //점점 밝아진다.
	}

	float rate_x = devSize.width / gameOverBg->width;
	float rate_y = devSize.height / gameOverBg->height;

	drawImage(gameOverBg, devSize.width / 2, devSize.height / 2,
		VCENTER | HCENTER, 0, 0, gameOverBg->width, gameOverBg->height,
		rate_x, rate_y, 2, 0);
	setRGBA(1, 1, 1, 1);
}

void keyEnding(iKeyStat stat, iPoint point)
{

}
