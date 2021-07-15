#include "Menu.h"

#include "Loading.h"
#include "Intro.h"
#include "MapEditor.h"
#include "Stage.h"
#include "App.h"

void loadMenu()
{
	for (int i = 0; i < SOUND_NUM; i++)
		audioStop(i);
	audioPlay(3);

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

static int page, _page;

Texture* methodStHow(const char* str);
void drawMethodPopHow(iPopup* pop, float dt, float rate);
void closeMethodPopHow(iPopup* pop);

void createPopHow()
{
	iImage* img;
	Texture* tex;
	iPopup* pop;

	//Bg + Str

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
	iPoint positionBtn[3] = { {800 - 25, -25}, {400 - 25 - 100, 540 - 25}, {400 - 25 + 100, 540 - 25} };
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
	setRGBA(1, 1, 1, 1);

	setStringSize(30);
	setStringRGBA(0, 0, 0, 1);
	setStringBorder(1);
	setStringBorderRGBA(1, 1, 0.5f, 1);

	const char** content = new const char* [6];

	content[0] = "<게임방법>";
	content[1] = "1.맵을 돌아다닌다.\n";
	content[2] = "[방향키 : ←, →, ↑, ↓ 또는 w,a,s,d]\n";
	content[3] = "2.걸어다니면 Step이 깍임.\n";
	content[4] = "3.맵 마다 적이 있다. 적을 만나면 싸워야 한다.\n";
	content[5] = "(ESC 키를 누르면 전체화면 On/Off이 됩니다.)\n";

	const char** content2 = new const char* [4];

	content2[0] = "<승리조건>";
	content2[1] = "1.Step이 0이 되지 않게.\n";
	content2[2] = "2.모든 적들을 쓰러뜨림\n";
	content2[3] = "☆최적의 방법으로 게임을 클리어해봐요!☆\n";

	const char** content3 = new const char* [4];

	content3[0] = "<패배조건>";
	content3[1] = "1.Step이 0이 된 경우\n";
	content3[2] = "2.적과의 싸움에서 진경우(HP = 0)\n";
	content3[3] = "☆최적의 방법으로 게임을 클리어해봐요!☆\n";

	int p = atoi(str);
	if (p == 0)
	{
		for (int i = 0; i < 6; i++)
			g->drawString(20, (1 + i) * 40, TOP | LEFT, content[i]);
	}
	else if (p == 1)
	{
		for (int i = 0; i < 4; i++)
			g->drawString(20, (1 + i) * 40, TOP | LEFT, content2[i]);
	}
	else //if(p==3)
	{
		for (int i = 0; i < 4; i++)
			g->drawString(20, (1 + i) * 40, TOP | LEFT, content3[i]);
	}
	g->drawString(size.width / 2, size.height * 0.9f, VCENTER | HCENTER, "%d / %d", 1 + p, _page);

	delete content;
	delete content2;
	delete content3;
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
	delete imgHowBtn;
}

void drawMethodPopHow(iPopup* pop, float dt, float rate)
{
	for (int i = 0; i < 3; i++)
		imgHowBtn[i]->setTexObject(popHow->selected == i);
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
iPopup* popOption;
iImage** imgOptionBtn;
iStrTex* stSound;

int soundSize = 10;

void drawPopOptionBefore(iPopup* pop, float dt, float rate);
void closeMethodPopOption(iPopup* pop);
Texture* stSoundMethod(const char* str);

void createPopOption()
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

	setRGBA(0.5f, 0.5f, 0.5f, 1);
	g->fillRect(0, 0, size.width, size.height, 10);

	setStringSize(40);
	setStringRGBA(0, 0, 0, 1);
	setStringBorder(1);
	setStringBorderRGBA(1, 1, 1, 1);
	
	g->drawString(size.width / 2, 40	  , VCENTER | HCENTER, "<Option>");
	g->drawString(40,			  40 + 80 , TOP | LEFT, "전체화면 : ");
	g->drawString(40,			  40 + 130 , TOP | LEFT, "사운드 : ");

	tex = g->getTexture();
	img = new iImage();
	img->addObject(tex);
	freeImage(tex);
	pop->addObject(img);

	//
	//Btn
	//

	imgOptionBtn = new iImage * [5];
	
	//닫기
	
	img = new iImage();

	size = iSizeMake(80, 40);
	for (int i = 0; i < 2; i++)
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
	imgOptionBtn[0] = img;

	// 전체화면 on/off

	size = iSizeMake(60, 40);
	for (int i = 1; i < 3; i++)
	{
		img = new iImage();
		for (int j = 0; j < 2; j++)
		{
			g->init(size);
			if (j == 0)
			{
				setRGBA(0, 0, 0, 1);
				g->fillRect(0, 0, size.width, size.height, 10);

				setStringSize(30);
				setStringRGBA(0, 0, 0, 1);
			}
			else
			{
				setRGBA(1, 1, 0, 1);
				g->fillRect(0, 0, size.width, size.height, 10);

				setStringSize(30);
				setStringRGBA(0.5f, 0.2f, 1, 1);
			}
			setStringBorder(2);
			setStringBorderRGBA(1, 1, 1, 1);
			
			if(i==1)
				g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "ON");
			else if(i==2)
				g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "OFF");
			tex = g->getTexture();
			img->addObject(tex);
			freeImage(tex);
		}
		img->position = iPointMake(200 + i * 70, 120);
		pop->addObject(img);
		imgOptionBtn[i] = img;
	}

	// 사운드

	//음량 btn

	size = iSizeMake(40, 40);
	for (int i = 3; i < 5; i++)
	{
		img = new iImage();
		for (int j = 0; j < 2; j++)
		{
			g->init(size);
			if (j == 0)
			{
				setRGBA(0, 0, 0, 1);
				g->fillRect(0, 0, size.width, size.height, 10);

				setStringSize(30);
				setStringRGBA(0, 0, 0, 1);
			}
			else
			{
				setRGBA(1, 1, 0, 1);
				g->fillRect(0, 0, size.width, size.height, 10);

				setStringSize(30);
				setStringRGBA(0.5f, 0.2f, 1, 1);
			}
			setRGBA(1, 1, 1, 1);
			setStringBorder(2);
			setStringBorderRGBA(1, 1, 1, 1);

			if (i == 3)
				g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "-");
			else if (i == 4)
				g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "+");
			
			tex = g->getTexture();
			img->addObject(tex);
			freeImage(tex);
		}
		img->position = imgOptionBtn[1]->position + iPointMake(150 * (i - 3), 50);
		pop->addObject(img);
		imgOptionBtn[i] = img;
	}
	
	// 음량 str

	iStrTex* st = new iStrTex(stSoundMethod);
	st->setString("%d", soundSize);
	img = new iImage();
	img->addObject(st->tex);
	img->position = imgOptionBtn[3]->position + iPointMake(65, 0);
	pop->addObject(img);
	stSound = st;

	pop->style = iPopupZoom;
	pop->openPoint = iPointMake(devSize.width / 2, devSize.height / 2);
	pop->closePoint = iPointMake((devSize.width - 640) / 2, (devSize.height - 480) / 2);
	pop->methodBefore = drawPopOptionBefore;
	pop->methodClose = closeMethodPopOption;

	popOption = pop;

}

Texture* stSoundMethod(const char* str)
{
	iGraphics* g = iGraphics::share();
	iSize size = iSizeMake(60, 40);
	g->init(size);

	setRGBA(0, 0, 0, 1);
	g->fillRect(0, 0, size.width, size.height, 10);

	setStringSize(30);
	setStringRGBA(1, 1, 1, 1);
	setStringBorder(0);

	int ss = atoi(str);
	g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "%d", ss);

	return g->getTexture();
}

void freePopOption()
{
	delete popOption;
	delete imgOptionBtn;
	delete stSound;
}
void drawPopOptionBefore(iPopup* pop, float dt, float rate)
{
	for (int i = 0; i < 5; i++)
		imgOptionBtn[i]->setTexObject(popOption->selected == i);
}
void closeMethodPopOption(iPopup* pop)
{
	showPopMenu(true);
}

void drawPopOption(float dt)
{
	popOption->paint(dt);
}
void showPopOption(bool show)
{
	popOption->show(show);
	if (show)
	{
		indexCloseMenu = -1;
	}
	else
	{

	}
}
bool keyPopOption(iKeyStat stat, iPoint point)
{
	if (popOption->bShow == false)
		return false;
	if (popOption->stat != iPopupProc)
		return true;

	int i, j = -1;

	switch (stat)
	{
	case iKeyStatBegan:
		if (popOption->selected == -1)
			break;

		audioPlay(0);
		
		if (popOption->selected == 0)  
		{
			showPopOption(false);
		}
		else if (popOption->selected == 1) 
		{
			if(!isFullscreen)
				goFullscreen();
		}
		else if (popOption->selected == 2)
		{
			if (isFullscreen)
				goFullscreen();
		}
		else if (popOption->selected == 3)
		{
			if (soundSize > 0)
				soundSize--;
			stSound->setString("%d", soundSize);
			float sSize = soundSize * 0.1f;
			audioVolume(sSize, sSize, 0);
		}
		else //if(popOpion->selected == 4)
		{
			if (soundSize < 10)
				soundSize++;
			stSound->setString("%d", soundSize);
			float sSize = soundSize * 0.1f;
			audioVolume(sSize, sSize, 0);
		}
		break;

	case iKeyStatMoved:
		for (i = 0; i < 5; i++)
		{
			if (containPoint(point, imgOptionBtn[i]->touchRect(popOption->closePoint)))
			{
				j = i;
				break;
			}
		}
		if (popOption->selected != j)
		{
			audioPlay(0);
			popOption->selected = j;
			printf("popOption : %d\n", popOption->selected);
		}
		break;

	case iKeyStatEnded:
		break;
	}

	return true;
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
	setStringBorderRGBA(1, 1, 1, 1);
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


