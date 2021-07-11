#include "Menu.h"

#include "Loading.h"
#include "Intro.h"
#include "MapEditor.h"
#include "Stage.h"
#include "App.h"

void loadMenu()
{
	createPopMenu();
	createPopHow();
	createPopOption();
	createPopExit();

	showPopMenu(true);
}
void freeMenu()
{
	freePopMenu();
	freePopHow();
	freePopOption();
	freePopExit();
}
void drawMenu(float dt)
{
	setRGBA(0.1f, 0.1f, 0.1f, 1.0f);
	clearRect();
	setRGBA(1, 1, 1, 1.0f);

	drawPopMenu(dt);
	drawPopHow(dt);
	drawPopOption(dt);
	drawPopExit(dt);

}

void keyMenu(iKeyStat stat, iPoint point)
{
	if (keyPopMenu(stat, point) ||
		keyPopHow(stat, point) ||
		keyPopOption(stat, point) ||
		keyPopExit(stat, point) 
		)
		return;

	switch (stat)
	{
	case iKeyStatBegan :
		printf("menu began (%f, %f)\n", point.x, point.y);
		break;
	case iKeyStatMoved:
		break;
	case iKeyStatEnded:
		break;
	}
	}
	

//===========================================================
//popMenu
//===========================================================

iPopup** popMenu;
iImage** imgMenuBtn;


void drawPopMenuBefore(iPopup* pop, float dt, float rate);
int indexCloseMenu;
void closePopMenu(iPopup* pop)
{
	if (indexCloseMenu == -1)
		return;

	void (*MethodShow[3])(bool show) =
	{
		showPopHow,
		showPopOption,
		showPopExit
	};
	MethodShow[indexCloseMenu](true);
}

void createPopMenu()
{
	iImage* img;
	Texture* tex;
	int i, j;

#ifdef LANGUAGE_KOR //한글
	const char* strBtn[5] = {
		"게임시작",
		"맵 에디터",
		"게임설명",
		"옵션",
		"종료"
	};
#else //영어
	const char* strBtn[5] = {
		"Game Start",
		"Map Editor",
		"How to Play",
		"Option",
		"Exit"
	};
#endif
	imgMenuBtn = new iImage * [5];

	iGraphics* g = iGraphics::share();

	popMenu = new iPopup * [2];
	for (int k = 0; k < 2; k++)
	{
		iPopup* pop = new iPopup();

		for (i = 0; i < 5; i++)
		{
			if (i % 2 != k) continue;
			
			img = new iImage();
			for (j = 0; j < 2; j++)
			{
				iSize size = iSizeMake(300, 60);
				g->init(size);

				if (j == 0)
				{
					setRGBA(1, 1, 1, 1);
					g->drawRect(0, 0, size.width, size.height, 10);
					setRGBA(1, 1, 1, 1);
					setStringSize(30);
					setStringRGBA(0, 0, 0, 1);
					setStringBorder(2);
					setStringBorderRGBA(1, 1, 1, 1);
					g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, strBtn[i]);
				}
				else
				{
					setRGBA(1, 1, 0, 1);
					g->fillRect(0, 0, size.width, size.height, 10);
					setRGBA(1, 1, 1, 1);
					setStringSize(30);
					setStringRGBA(0.4f, 0.5f, 1, 1);
					setStringBorder(2);
					setStringBorderRGBA(1, 1, 1, 1);
					g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, strBtn[i]);
				}

				tex = g->getTexture();
				img->addObject(tex);
				freeImage(tex);
			}

			img->position = iPointMake(0, 80 * i);
			pop->addObject(img);

			imgMenuBtn[i] = img;
		}

		pop->style = iPopupMove;
		if (k == 0)
		{
			pop->openPoint = iPointMake(-150, (devSize.height) / 2 - 160);

			pop->methodBefore = drawPopMenuBefore;
			pop->methodClose = closePopMenu;
		}
		else
		{
			pop->openPoint = iPointMake(devSize.width, (devSize.height) / 2 - 160);
		}

		pop->closePoint = iPointMake(devSize.width / 2 - 150, (devSize.height) / 2 - 160);
		popMenu[k] = pop;
	}

	indexCloseMenu = -1;

}

void freePopMenu()
{
	for (int i = 0; i < 2; i++)
		delete popMenu[i];
	delete popMenu;
	delete imgMenuBtn;
}

void drawPopMenuBefore(iPopup* pop, float dt, float rate)
{
	setRGBA(0, 0, 0, 0.7f * rate);
	fillRect(0, 0, devSize.width, devSize.height);
	setRGBA(1, 1, 1, 1);
	
	for (int i = 0; i < 5; i++)
		imgMenuBtn[i]->setTexObject(i == popMenu[0]->selected);
}

void drawPopMenu(float dt)
{
	for (int i = 0; i < 2; i++)
	{
		popMenu[i]->paint(dt);
	}
}

void showPopMenu(bool show)
{
	for (int i = 0; i < 2; i++)
		popMenu[i]->show(show);
	if (show)
	{

	}
}
bool keyPopMenu(iKeyStat stat, iPoint point)
{
	iPopup* pop = popMenu[0];
	
	if (pop->bShow == false)
		return false;
	if (pop->stat != iPopupProc)
		return true;

	int i, j = -1;

	switch (stat)
	{
	case iKeyStatBegan:
		if (pop->selected == -1)
			break;
		if (pop->selected == 0)
		{
			printf("게임시작\n");
			stageFrom = stageTo = 10;
			setLoading(gs_stage, freeMenu, loadStage);
		}
		else if (pop->selected == 1)
		{
			printf("에디터\n");
			setLoading(gs_map, freeMenu, loadMapEditor);
		}
		else if (pop->selected == 2)
		{
			printf("게임설명\n");
			indexCloseMenu = 0;
			showPopMenu(false);
		}
		else if (pop->selected == 3)
		{
			printf("옵션\n");
			indexCloseMenu = 1;
			showPopMenu(false);
		}

		else //if(pop->selected == 4)
		{
			printf("게임종료\n");
			indexCloseMenu = 2;
			showPopMenu(false);
		}
		break;

	case iKeyStatMoved:
		for (i = 0; i < 5; i++)
		{
			if (containPoint(point, imgMenuBtn[i]->touchRect(pop->closePoint)))
			{
				j = i;
				break;
			}

		}
		if (pop->selected != j)
		{
			//printf("audioPlay\n");
			audioPlay(0);
			pop->selected = j;
		}
		break;

	case iKeyStatEnded:
		break;
	}

	return true;
}

//===========================================================
//popHow
//===========================================================

iStrTex* stHow;
iPopup* popHow;
iImage** imgHowBtn;

int page, _page;

Texture* methodStHow(const char* str);
void drawMethodPopHow(iPopup* pop, float dt, float rate);
void closeMethodPopHow(iPopup* pop);

void createPopHow()
{
	iImage* img;
	Texture* tex;
	iPopup* pop;

	//Bg

	page = 0;
	_page = 3;

	iStrTex* st = new iStrTex(methodStHow);
	st->setString("%d", page);
	img = new iImage();
	img->addObject(st->tex);
	pop = new iPopup();
	pop->addObject(img);
	stHow = st;

	//Btn

	iGraphics* g = iGraphics::share();
	iSize size = iSizeMake(50, 50);
	
	const char* strBtn[3] = { "X", "◀", "▶" };
	iPoint positionBtn[3] = { {800 - 25, -25}, {400-25 - 100, 540-25}, {400 - 25 + 100, 540-25} };
	imgHowBtn = new iImage * [3];

	for (int i = 0; i < 3; i++)
	{
		img = new iImage();
		for (int j = 0; j < 2; j++)
		{
			g->init(size);

			if (j == 0) setRGBA(0, 0, 0, 1);
			else setRGBA(1, 1, 0, 1);
			g->fillRect(0, 0, size.width, size.height, 10);

			setStringSize(30);
			setStringRGBA(1, 0, 1, 1);
			setStringBorder(0);
			g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, strBtn[i]);

			tex = g->getTexture();
			img->addObject(tex);
			freeImage(tex);
		}
		img->position = positionBtn[i];
		pop->addObject(img);
		imgHowBtn[i] = img;
	}

	pop->style = iPopupZoom;
	pop->openPoint = iPointMake(devSize.width / 2, devSize.height / 2);
	pop->closePoint = iPointMake(devSize.width / 2 - 400, devSize.height / 2 - 300);
	pop->methodBefore = drawMethodPopHow;
	pop->methodClose = closeMethodPopHow;
	popHow = pop;

}
Texture* methodStHow(const char* str)
{
	iGraphics* g = iGraphics::share();
	iSize size = iSizeMake(800, 600);
	g->init(size);

	setRGBA(0.5f, 0.5f, 0.5f, 1);
	g->fillRect(0, 0, size.width, size.height, 5);

	setStringSize(30);
	setStringRGBA(0, 0, 0, 1);
	setStringBorder(1);
	setStringBorderRGBA(1, 1, 0.5f, 1);

	const char** content = new const char* [9];

	content[0] = "<게임방법>";
	content[1] = "1.맵을 돌아다닌다.\n";
	content[2] = "[방향키 : ←, →, ↑, ↓ 또는 w,a,s,d]\n";
	content[3] = "2.STEP이 0이되면 GAME OVER\n";
	content[4] = "3.적을 만나면 싸워야 한다.\n";
	content[5] = "적과의 싸움에서 패배하면 GAMEOVER\n";
	content[6] = "4.아이템 및 동료획득가능\n";
	content[7] = "(승리조건 : 미구현)\n";
	content[8] = "☆최적의 방법으로 게임을 클리어해봐요!☆\n";

	int p = atoi(str);
	if (p == 0)
	{
		//g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "첫번째 페이지\n");
		for(int i=0;i<9;i++)
			g->drawString(20, (1+i) * 40, TOP | LEFT, content[i]);
	}
	else if (p == 1)
	{
		g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "두번째 페이지");
	}
	else //if(p==3)
	{
		g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "세번째 페이지");
	}
	g->drawString(size.width / 2, size.height * 0.9f, VCENTER | HCENTER, "%d / %d", 1 + p, _page);

	return g->getTexture();
}

void closeMethodPopHow(iPopup* pop)
{
	showPopMenu(true);
}

void freePopHow()
{
	delete popHow;
	delete stHow;
}

#if 0
static float testDt = 0.0f;
#endif
void drawMethodPopHow(iPopup* pop, float dt, float rate)
{
	for (int i = 0; i < 3; i++)
		imgHowBtn[i]->setTexObject(popHow->selected == i);

//#issue! 자동으로 페이지 넘어가는 코드
#if 0
	testDt += dt;
	if (testDt > 1.0f)
	{
		testDt -= 1.0f;
		page++;
		if (page > _page - 1)
			page = _page - 1;
		stHow->setString("%d", page);
	}
#endif 
}

void drawPopHow(float dt)
{
	popHow->paint(dt);
}
void showPopHow(bool show)
{
	popHow->show(show);
	if (show)
	{
		//to do...
		indexCloseMenu = -1;
	}
	else
	{
		//to do...
	}
}
bool keyPopHow(iKeyStat stat, iPoint point)
{
	if (popHow->bShow == false)
		return false;
	if (popHow->stat != iPopupProc)
		return true;

	int i, j = -1;

	switch (stat)
	{
	case iKeyStatBegan:
		if (popHow->selected == -1)
			break;

		audioPlay(0);
		if (popHow->selected == 0)
		{
			showPopHow(false);
		}
		else if (popHow->selected == 1)
		{
			page--;
			if (page < 0)
				page = 0;
			stHow->setString("%d", page);
		}
		else if (popHow->selected == 2)
		{
			page++;
			if (page > _page - 1)
				page = _page - 1;
			stHow->setString("%d", page);
		}
		break;

	case iKeyStatMoved:
		for (i = 0; i < 3; i++)
		{
			if (containPoint(point, imgHowBtn[i]->touchRect(popHow->closePoint)))
			{
				j = i;
				break;
			}
		}
		if (popHow->selected != j)
		{
			audioPlay(0);
			popHow->selected = j;
			printf("popHow : %d\n", popHow->selected);
		}
		break;

	case iKeyStatEnded:
		break;
	}

	return true;
}

//===========================================================
//popOption
//===========================================================

void createPopOption()
{
#if 1 //#need update! temp code
	createPopTemp();
#endif
}
void freePopOption()
{
#if 1 //#need update! temp code
	freePopTemp();
#endif
}
void drawPopOption(float dt)
{
#if 1 //#need update! temp code
	drawPopTemp(dt);
#endif
}
void showPopOption(bool show)
{
#if 1 //#need update! temp code
	showPopTemp(show);
#endif
}
bool keyPopOption(iKeyStat stat, iPoint point)
{
#if 1 //#need update! temp code
	return keyPopTemp(stat, point);
#endif

}

//===========================================================
//popExit
//===========================================================

iPopup* popExit;
iImage** imgExitBtn;

void drawPopExitBefore(iPopup* pop, float dt, float rate);
void closePopMenuCommon(iPopup* pop)
{
	showPopMenu(true);
}

void createPopExit()
{
	iImage* img;
	Texture* tex;
	int i, j;

	iPopup* pop = new iPopup();

	//Bg

	iGraphics* g = iGraphics::share();
	iSize size = iSizeMake(640, 480);
	g->init(size);

	setRGBA(1, 1, 1, 0.7f);
	g->fillRect(0, 0, size.width, size.height, 10);
	setRGBA(1, 1, 1, 1);
	
	setStringSize(40);
	setStringRGBA(0, 0, 0, 1);
	setStringBorder(2);
	setStringRGBA(1, 1, 1, 1);
	g->drawString(size.width / 2, size.height / 2 - 50, VCENTER | HCENTER, "정말 종료하시겠습니까?");

	tex = g->getTexture();

	img = new iImage();
	img->addObject(tex);
	freeImage(tex);
	pop->addObject(img);

	//Btn

	const char* strBtn[2] = {
		"예", "아니요"
	};
	imgExitBtn = new iImage * [2];

	for (i = 0; i < 2; i++)
	{
		img = new iImage();
		for (j = 0; j < 2; j++)
		{
			size = iSizeMake(150, 40);
			g->init(size);
			
			if (j == 0)
			{
				setRGBA(0, 0, 0, 1);
				g->fillRect(0, 0, size.width, size.height, 10);
				setRGBA(1, 1, 1, 1);
				setStringSize(30);
				setStringRGBA(0, 0, 0, 1);
				setStringBorder(2);
				setStringBorderRGBA(1, 1, 1, 1);
				g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, strBtn[i]);
			}
			else
			{
				setRGBA(1, 1, 0, 1);
				g->fillRect(0, 0, size.width, size.height, 10);
				setRGBA(1, 1, 1, 1);
				setStringSize(30);
				setStringRGBA(0.4f, 0.5f, 1, 1);
				setStringBorder(2);
				setStringBorderRGBA(1, 1, 1, 1);
				g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, strBtn[i]);
			}

			tex = g->getTexture();
			img->addObject(tex);
			freeImage(tex);
		}
		img->position = iPointMake(size.width + 180 * i, 270);
		pop->addObject(img);

		imgExitBtn[i] = img;
	}
	pop->style = iPopupRotate;
	pop->openPoint = iPointMake(devSize.width / 2, devSize.height / 2);
	pop->closePoint = iPointMake((devSize.width - 640) / 2, (devSize.height - 480) / 2);
	pop->methodBefore = drawPopExitBefore;
	pop->methodClose = closePopMenuCommon;
	popExit = pop;
}

void freePopExit()
{
	delete popExit;
	delete imgExitBtn;
}

void drawPopExitBefore(iPopup* pop, float dt, float rate)
{
	for (int i = 0; i < 2; i++)
		imgExitBtn[i]->setTexObject(i == popExit->selected);
}
void drawPopExit(float dt)
{
	popExit->paint(dt);
}
void showPopExit(bool show)
{
	popExit->show(show);
	if (show)
	{
		indexCloseMenu = -1;
	}

}
bool keyPopExit(iKeyStat stat, iPoint point)
{
	if (popExit->bShow == false)
		return false;
	if (popExit->stat != iPopupProc)
		return true;

	int i, j = -1;

	switch (stat)
	{
	case iKeyStatBegan:
		if (popExit->selected == -1)
			break;

		if (popExit->selected == 0)
		{
			printf("예\n");
			//프로그램 종료
			runWnd = false;
		}
		else// if (popExit->selected == 1)
		{
			printf("아니오\n");
			showPopExit(false);
		}
		break;

	case iKeyStatMoved:
		for (i = 0; i < 2; i++)
		{
			if (containPoint(point, imgExitBtn[i]->touchRect(popExit->closePoint)))
			{
				j = i;
				break;
			}
		}
		if (popExit->selected != j)
		{
			printf("audio play\n");
			popExit->selected = j;
		}
		break;

	case iKeyStatEnded:
		break;
	}

	return true;
}

//===========================================================
//popTemp : 미구현 팝업
//===========================================================

iPopup* popTemp;
iImage** imgTempBtn;

void drawPopTempBefore(iPopup* pop, float dt, float rate);
void closeMethodPopTemp(iPopup* pop);

void createPopTemp()
{
	iPopup* pop = new iPopup();
	iImage* img;
	Texture* tex;

	//
	//Bg
	//

	iGraphics* g = iGraphics::share();
	iSize size = iSizeMake(640, 480);
	g->init(size);

	setRGBA(0.5f, 0.5f, 0.5f, 1.0f);
	g->fillRect(0, 0, size.width, size.height, 10);

	setStringSize(40);
	setStringRGBA(0, 0, 0, 1);
	setStringBorder(1);
	setStringBorderRGBA(1, 1, 1, 1);
	g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "미구현된 기능입니다.");

	tex = g->getTexture();
	img = new iImage();
	img->addObject(tex);
	freeImage(tex);
	pop->addObject(img);

	//
	//Btn
	//

	imgTempBtn = new iImage * [1];
	img = new iImage();

	size = iSizeMake(80, 40);
	for(int i = 0; i < 2; i++)
	{
		g->init(size);

		if (i == 0) setRGBA(0, 0, 0, 1);
		else		setRGBA(1, 1, 0, 1);
		g->fillRect(0, 0, size.width, size.height, 10);

		setStringSize(25);
		setStringRGBA(1, 1, 1, 1);
		g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "닫기");
		
		tex = g->getTexture();
		img->addObject(tex);
		freeImage(tex);
	}
	img->position = iPointMake(320 - size.width / 2, 480 - 100);
	pop->addObject(img);
	imgTempBtn[0] = img;

	pop->style = iPopupZoom;
#if 1
	pop->openPoint = iPointMake(devSize.width / 2, devSize.height / 2);
	pop->closePoint = iPointMake((devSize.width - 640) / 2, (devSize.height-480) / 2);
#else
	pop->openPoint = iPointZero;
	pop->closePoint = iPointZero;
#endif
	pop->methodBefore = drawPopTempBefore;
	pop->methodClose = closeMethodPopTemp;

	popTemp = pop;
}

void drawPopTempBefore(iPopup* pop, float dt, float rate)
{
	for (int i = 0; i < 1; i++)
		imgTempBtn[i]->setTexObject(popTemp->selected == i);
}

void closeMethodPopTemp(iPopup* pop)
{
	showPopMenu(true);
}

void freePopTemp()
{
	delete popTemp;
	delete imgTempBtn;
}
void drawPopTemp(float dt)
{
	popTemp->paint(dt);
}
void showPopTemp(bool show)
{
	popTemp->show(show);
	if (show)
	{
		indexCloseMenu = -1;
	}
	else
	{

	}
}
bool keyPopTemp(iKeyStat stat, iPoint point)
{
	if (popTemp->bShow == false)
		return false;
	if (popTemp->stat != iPopupProc)
		return true;

	int i, j = -1;

	switch (stat)
	{
	case iKeyStatBegan:
		if (popTemp->selected == -1)
			break;

		audioPlay(0);
		if (popTemp->selected == 0)
		{
			showPopTemp(false);
		}
		break;

	case iKeyStatMoved:
		for (i = 0; i < 1; i++)
		{
			if (containPoint(point, imgTempBtn[i]->touchRect(popTemp->closePoint)))
			{
				j = i;
				break;
			}
		}
		if (popTemp->selected != j)
		{
			audioPlay(0);
			popTemp->selected = j;
			printf("popTemp : %d\n", popTemp->selected);
		}
		break;

	case iKeyStatEnded:
		break;
	}

	return true;
}


