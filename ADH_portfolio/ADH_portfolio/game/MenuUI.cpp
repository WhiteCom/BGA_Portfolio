#include "Menu.h"
#include "MenuUI.h"

#include "Loading.h"
#include "Intro.h"
#include "Stage.h"
#include "Common.h"
#if (OS==OS_WINDOW)
#include "App.h"
#include "MapEditor.h"
#endif

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
#if(OS==OS_WINDOW)
	const char* strBtn[5] = {
		"게임시작",
		"맵 에디터",
		"게임설명",
		"옵션",
		"종료"
	};
#elif(OS==OS_ANDROOS_ANDROID)
	const char* strBtn[4] = {
		"게임시작",
		"게임설명",
		"옵션",
		"종료"
	};
#endif
#else //영어
#if (OS==OS_WINDOW)
	const char* strBtn[5] = {
		"Game Start",
		"Map Editor",
		"How to Play",
		"Option",
		"Exit"
	};
#elif(OS==OS_ANDROID)
	const char* strBtn[4] = {
			"Game Start",
			"How to Play",
			"Option",
			"Exit"
	};
#endif
#endif

#if (OS==OS_WINDOW)
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

				setStringSize(30);
				setStringBorder(2);
				if (j == 0)
				{
					setRGBA(1, 1, 1, 1);
					g->drawRect(0, 0, size.width, size.height, 10);
					setStringRGBA(0, 0, 0, 1);
					setStringBorderRGBA(1, 1, 1, 1);
				}
				else
				{
					setRGBA(1, 1, 0, 1);
					g->fillRect(0, 0, size.width, size.height, 10);
					setRGBA(1, 1, 1, 1);
					setStringRGBA(0.4f, 0.5f, 1, 1);
					setStringBorderRGBA(1, 1, 1, 1);
				}
				g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, strBtn[i]);

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
#elif (OS==OS_ANDROID)
	imgMenuBtn = new iImage * [4];

	iGraphics* g = iGraphics::share();

	popMenu = new iPopup * [2];
	for (int k = 0; k < 2; k++)
	{
		iPopup* pop = new iPopup();

		for (i = 0; i < 4; i++)
		{
			if (i % 2 != k) continue;

			img = new iImage();
			for (j = 0; j < 2; j++)
			{
				iSize size = iSizeMake(500, 120);
				g->init(size);

				setStringSize(60);
				setStringBorder(2);
				if (j == 0)
				{
					setRGBA(1, 1, 1, 1);
					g->drawRect(0, 0, size.width, size.height, 10);
					setStringRGBA(0, 0, 0, 1);
					setStringBorderRGBA(1, 1, 1, 1);
				}

				else
				{
					setRGBA(1, 1, 0, 1);
					g->fillRect(0, 0, size.width, size.height, 10);
					setRGBA(1, 1, 1, 1);
					setStringRGBA(0.4f, 0.5f, 1, 1);
					setStringBorderRGBA(1, 1, 1, 1);
				}
				g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, strBtn[i]);

				tex = g->getTexture();
				img->addObject(tex);
				freeImage(tex);
			}

			img->position = iPointMake(0, 150 * i);
			pop->addObject(img);
			imgMenuBtn[i] = img;
		}

		pop->style = iPopupMove;
		if (k == 0)
		{
			pop->openPoint = iPointMake(-250, (devSize.height) / 2 - 280);

			pop->methodBefore = drawPopMenuBefore;
			pop->methodClose = closePopMenu;
		}
		else
		{
			pop->openPoint = iPointMake(devSize.width, (devSize.height) / 2 - 280);
		}
		pop->closePoint = iPointMake(devSize.width / 2 - 250, (devSize.height) / 2 - 280);
		popMenu[k] = pop;
	}
	indexCloseMenu = -1;
#endif
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

#if (OS==OS_WINDOW)
	for (int i = 0; i < 5; i++)
		imgMenuBtn[i]->setTexObject(i == popMenu[0]->selected);
#elif (OS==OS_ANDROID)
	for (int i = 0; i < 4; i++)
		imgMenuBtn[i]->setTexObject(i == popMenu[0]->selected);
#endif
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
#if (OS==OS_ANDROID)
bool keyPopMenu(iKeyStat stat, iPoint point)
{
	iPopup* pop = popMenu[0];

	if (pop->bShow == false)
		return false;

#if 1 //화면에서 벗어나면 ////////////////////////////////////////////////////
	if (containPoint(point, iRectMake(devSize.width / 2 - 280,
		devSize.height / 2 - 300,
		560, 600)) == false)
		return false;
#endif ////////////////////////////////////////////////////////////////////
	if (pop->stat != iPopupProc)
		return true;

	int i, j = -1;

	switch (stat)
	{
	case iKeyStatBegan:
		for (i = 0; i < 4; i++)
		{
			if (containPoint(point, imgMenuBtn[i]->touchRect(pop->closePoint)))
			{
				j = i;
				break;
			}
		}
		if (pop->selected != j)
		{
#if 0 //#openAL
			audioPlay(0);
#endif
			pop->selected = j;
		}
		break;

	case iKeyStatMoved:
		i = pop->selected;
		if (i == -1) break;
		if (containPoint(point, imgMenuBtn[i]->touchRect(pop->closePoint)) == false)
		{
#if 0 //#openAL
			audioPlay(0);
#endif
			pop->selected = -1;
		}
		break;

	case iKeyStatEnded:
		i = pop->selected;
		if (i == -1) break;
		pop->selected = -1;

		if (i == 0)
		{
			stageFrom = stageTo = 10;
			setLoading(gs_stage, freeMenu, loadStage);
		}
		else if (i == 1)
		{
			indexCloseMenu = 0;
			showPopMenu(false);
		}
		else if (i == 2)
		{
			indexCloseMenu = 1;
			showPopMenu(false);
		}

		else //if(i == 3)
		{
			indexCloseMenu = 2;
			showPopMenu(false);
		}
		break;
	}

	return true;
}

#elif(OS==OS_WINDOW)
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
			xprint("게임시작\n");
			stageFrom = stageTo = 10;
			setLoading(gs_stage, freeMenu, loadStage);
		}
		else if (pop->selected == 1)
		{
			xprint("에디터\n");
			setLoading(gs_map, freeMenu, loadMapEditor);
		}
		else if (pop->selected == 2)
		{
			xprint("게임설명\n");
			indexCloseMenu = 0;
			showPopMenu(false);
		}
		else if (pop->selected == 3)
		{
			xprint("옵션\n");
			indexCloseMenu = 1;
			showPopMenu(false);
		}

		else //if(pop->selected == 4)
		{
			xprint("게임종료\n");
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
#if 0 //#openAL
			audioPlay(0);
#endif
			pop->selected = j;
		}
		break;

	case iKeyStatEnded:
		break;
	}

	return true;
}
#endif // keyPopMenu

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
	content[2] = "[방향키 (←,→,↑,↓) 또는 w,a,s,d]\n";
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

	int p = atof(str);
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

//for Android
#if (OS==OS_ANDROID)
bool keyPopHow(iKeyStat stat, iPoint point)
{
	iPopup* pop = popHow;
	if (pop->bShow == false)
		return false;

#if 1 // 화면에서 벗어난 경우 ////////////////////////////////////////////////////////////////////////////////////
	if (containPoint(point, iRectMake(devSize.width / 2 - 450, devSize.height / 2 - 350, 900, 700)) == false)
		return false;
#endif /////////////////////////////////////////////////////////////////////////////////////////////////////////

	if (pop->stat != iPopupProc)
		return true;

	int i, j = -1;

	switch (stat)
	{
	case iKeyStatBegan:
		for (i = 0; i < 3; i++)
		{
			if (containPoint(point, imgHowBtn[i]->touchRect(popHow->closePoint, iSizeMake(30, 30))))
			{
				j = i;
				break;
			}
		}
		if (pop->selected != j)
		{
#if 0 //#openAL
			audioPlay(0);
#endif
			pop->selected = j;
			xprint("popHow : %d", pop->selected);
		}
		break;

	case iKeyStatMoved:
		i = pop->selected;
		if (i == -1) break;
		if (containPoint(point, imgHowBtn[i]->touchRect(pop->closePoint, iSizeMake(30, 30))) == false)
		{
#if 0 //#openAL
			audioPlay(0);
#endif
			pop->selected = -1;
		}
		break;

	case iKeyStatEnded:
		i = pop->selected;
		if (i == -1) break;
		pop->selected = -1;

		if (i == -1)
			break;
#if 0 //#openAL
		audioPlay(0);
#endif
		if (i == 0)
		{
			showPopHow(false);
		}
		else if (i == 1)
		{
			page--;
			if (page < 0)
				page = 0;
			stHow->setString("%d", page);
		}
		else if (i == 2)
		{
			page++;
			if (page > _page - 1)
				page = _page - 1;
			stHow->setString("%d", page);
		}
		break;
	}

	return true;
}

// for Window
#elif (OS==OS_WINDOW)
bool keyPopHow(iKeyStat stat, iPoint point)
{
	iPopup* pop = popHow;

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
#if 0 //#openAL
		audioPlay(0);
#endif
		if (pop->selected == 0)
		{
			showPopHow(false);
		}
		else if (pop->selected == 1)
		{
			page--;
			if (page < 0)
				page = 0;
			stHow->setString("%d", page);
		}
		else if (pop->selected == 2)
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
			if (containPoint(point, imgHowBtn[i]->touchRect(pop->closePoint)))
			{
				j = i;
				break;
			}
		}
		if (pop->selected != j)
		{
#if 0 //#openAL
			audioPlay(0);
#endif
			pop->selected = j;
			xprint("popHow : %d\n", pop->selected);
		}
		break;

	case iKeyStatEnded:
		break;
	}

	return true;
}
#endif //keyPopHow
//===========================================================
//popOption
//===========================================================
iPopup* popOption;
iImage** imgOptionBtn;
iStrTex* stSound, * stEff;

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

#if (OS==OS_WINDOW)
	g->drawString(size.width / 2, 40, VCENTER | HCENTER, "<Option>");
	g->drawString(40, 40 + 80, TOP | LEFT, "전체화면 : ");
	g->drawString(40, 40 + 130, TOP | LEFT, "BGM : ");
	g->drawString(40, 40 + 180, TOP | LEFT, "EFF : ");
#elif (OS==OS_ANDROID)
	g->drawString(size.width / 2, 40, VCENTER | HCENTER, "<Option>");
	g->drawString(40, 40 + 80, TOP | LEFT, "BGM : ");
	g->drawString(40, 40 + 150, TOP | LEFT, "EFF : ");
#endif

	tex = g->getTexture();
	img = new iImage();
	img->addObject(tex);
	freeImage(tex);
	pop->addObject(img);

	//
	//Btn
	//
#if (OS==OS_WINDOW)
	imgOptionBtn = new iImage * [7];

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

			if (i == 1)
				g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "ON");
			else if (i == 2)
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
	for (int i = 3; i < 7; i++)
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

			if (i % 2 == 1)
				g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "-");
			else if (i % 2 == 0)
				g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "+");

			tex = g->getTexture();
			img->addObject(tex);
			freeImage(tex);
		}
		img->position = imgOptionBtn[1]->position + iPointMake(150 * (!(i % 2)), 50 + (i / 5) * 45);
		pop->addObject(img);
		imgOptionBtn[i] = img;
	}

	// 음량 str

	iStrTex* st = new iStrTex(stSoundMethod);
	st->setString("%f", appData->bgm * 10);
	img = new iImage();
	img->addObject(st->tex);
	img->position = imgOptionBtn[3]->position + iPointMake(65, 0);
	pop->addObject(img);
	stSound = st;

	// eff str

	st = new iStrTex(stSoundMethod);
	st->setString("%f", appData->eff * 10);
	img = new iImage();
	img->addObject(st->tex);
	img->position = imgOptionBtn[5]->position + iPointMake(65, 0);
	pop->addObject(img);
	stEff = st;

	pop->style = iPopupZoom;
	pop->openPoint = iPointMake(devSize.width / 2, devSize.height / 2);
	pop->closePoint = iPointMake((devSize.width - 640) / 2, (devSize.height - 480) / 2);
	pop->methodBefore = drawPopOptionBefore;
	pop->methodClose = closeMethodPopOption;

	popOption = pop;
#elif (OS==OS_ANDROID)
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

	// 사운드

	//음량 btn

	size = iSizeMake(40, 40);
	for (int i = 1; i < 5; i++)
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

			if (i % 2 == 1)
				g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "-");
			else if (i % 2 == 0)
				g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "+");

			tex = g->getTexture();
			img->addObject(tex);
			freeImage(tex);
		}
		img->position = iPointMake(270, 65) + iPointMake(150 * (!(i % 2)), 50 + (i / 3) * 70);
		pop->addObject(img);
		imgOptionBtn[i] = img;
	}

	// 음량 str

	iStrTex* st = new iStrTex(stSoundMethod);
	st->setString("%f", appData->bgm * 10);
	img = new iImage();
	img->addObject(st->tex);
	img->position = imgOptionBtn[1]->position + iPointMake(65, 0);
	pop->addObject(img);
	stSound = st;

	// eff str

	st = new iStrTex(stSoundMethod);
	st->setString("%f", appData->eff * 10);
	img = new iImage();
	img->addObject(st->tex);
	img->position = imgOptionBtn[3]->position + iPointMake(65, 0);
	pop->addObject(img);
	stEff = st;

	pop->style = iPopupZoom;
	pop->openPoint = iPointMake(devSize.width / 2, devSize.height / 2);
	pop->closePoint = iPointMake((devSize.width - 640) / 2, (devSize.height - 480) / 2);
	pop->methodBefore = drawPopOptionBefore;
	pop->methodClose = closeMethodPopOption;

	popOption = pop;
#endif
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

	float ss = atof(str);
	if (ss <= 0.0)
		ss = 0.0;
	g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "%.0f", ss);

	return g->getTexture();
}

void freePopOption()
{
	delete popOption;
	delete imgOptionBtn;
	delete stSound;
	delete stEff;
}
void drawPopOptionBefore(iPopup* pop, float dt, float rate)
{
#if (OS==OS_WINDOW)
	for (int i = 0; i < 7; i++)
		imgOptionBtn[i]->setTexObject(popOption->selected == i);
#elif (OS==OS_ANDROID)
	for (int i = 0; i < 5; i++)
		imgOptionBtn[i]->setTexObject(popOption->selected == i);
#endif
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
#if (OS==OS_ANDROID)
bool keyPopOption(iKeyStat stat, iPoint point)
{
	iPopup* pop = popOption;

	if (pop->bShow == false)
		return false;

#if 1 // 화면에서 벗어나면 ///////////////////////////////////////////

#endif /////////////////////////////////////////////////////////////
	if (pop->stat != iPopupProc)
		return true;

	int i, j = -1;

	switch (stat)
	{
	case iKeyStatBegan:
		for (i = 0; i < 5; i++)
		{
			if (containPoint(point, imgOptionBtn[i]->touchRect(popOption->closePoint, iSizeMake(20, 20))))
			{
				j = i;
				break;
			}
		}
		if (pop->selected != j)
		{
#if 0 //#openAL
			audioPlay(0);
#endif
			pop->selected = j;
			xprint("popOption : %d", pop->selected);
		}
		break;

	case iKeyStatMoved:
		i = pop->selected;
		if (i == -1) break;
		if (containPoint(point, imgOptionBtn[i]->touchRect(pop->closePoint, iSizeMake(20, 20))) == false)
		{
#if 0 //#openAL
			audioPlay(0);
#endif
			pop->selected = -1;
		}
		break;

	case iKeyStatEnded:
		i = pop->selected;
		if (i == -1) break;
		pop->selected = -1;
#if 0 //#openAL
		audioPlay(0);
#endif
		if (i == 0)
		{
			showPopOption(false);
		}
		else if (i == 1)
		{
			if (appData->bgm * 10 > 0.0)
				appData->bgm -= 0.1;
			stSound->setString("%f", appData->bgm * 10);
#if 0 //#openAL
			audioVolume(appData->bgm, appData->eff, 1);
#endif
		}
		else if (i == 2)
		{
			if (appData->bgm * 10 < 10.0)
				appData->bgm += 0.1;
			stSound->setString("%f", appData->bgm * 10);
#if 0 //#openAL
			audioVolume(appData->bgm, appData->eff, 1);
#endif
		}
		else if (i == 3)
		{
			if (appData->eff * 10 > 0.0)
				appData->eff -= 0.1;
			stEff->setString("%f", appData->eff * 10);
#if 0 //#openAL
			audioVolume(appData->bgm, appData->eff, 1);
#endif
		}
		else if (i == 4)
		{
			if (appData->eff * 10 < 10.0)
				appData->eff += 0.1;
			stEff->setString("%f", appData->eff * 10);
#if 0 //#openAL
			audioVolume(appData->bgm, appData->eff, 1);
#endif
		}
		break;
	}
	return true;
}

// for Window
#elif (OS==OS_WINDOW)
bool keyPopOption(iKeyStat stat, iPoint point)
{
	iPopup* pop = popOption;

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

		audioPlay(0);

		if (pop->selected == 0)
		{
			showPopOption(false);
		}
		else if (pop->selected == 1)
		{
			if (!isFullscreen)
				goFullscreen();
		}
		else if (pop->selected == 2)
		{
			if (isFullscreen)
				goFullscreen();
		}
		else if (pop->selected == 3)
		{
			if (appData->bgm * 10 > 0.0)
				appData->bgm -= 0.1;
			stSound->setString("%f", appData->bgm * 10);
			audioVolume(appData->bgm, appData->eff, 1);
		}
		else if (pop->selected == 4)
		{
			if (appData->bgm * 10 < 10.0)
				appData->bgm += 0.1;
			stSound->setString("%f", appData->bgm * 10);
			audioVolume(appData->bgm, appData->eff, 1);
		}
		else if (pop->selected == 5)
		{
			if (appData->eff * 10 > 0.0)
				appData->eff -= 0.1;
			stEff->setString("%f", appData->eff * 10);
			audioVolume(appData->bgm, appData->eff, 1);
		}
		else if (pop->selected == 6)
		{
			if (appData->eff * 10 < 10.0)
				appData->eff += 0.1;
			stEff->setString("%f", appData->eff * 10);
			audioVolume(appData->bgm, appData->eff, 1);
		}
		break;

	case iKeyStatMoved:
		for (i = 0; i < 7; i++)
		{
			if (containPoint(point, imgOptionBtn[i]->touchRect(pop->closePoint)))
			{
				j = i;
				break;
			}
		}
		if (pop->selected != j)
		{
			audioPlay(0);
			pop->selected = j;
			xprint("popOption : %d\n", pop->selected);
		}
		break;

	case iKeyStatEnded:
		break;
	}

	return true;
}
#endif //keyPopOption

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
//for Window
#if (OS==OS_WINDOW)
bool keyPopExit(iKeyStat stat, iPoint point)
{
	iPopup* pop = popExit;

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
			xprint("예\n");
#if (OS==OS_WINDOW)
			//프로그램 종료
			runWnd = false;
#elif(OS==OS_ANDROID)
#endif
		}
		else// if (pop->selected == 1)
		{
			xprint("아니오\n");
			showPopExit(false);
		}
		break;

	case iKeyStatMoved:
		for (i = 0; i < 2; i++)
		{
			if (containPoint(point, imgExitBtn[i]->touchRect(pop->closePoint)))
			{
				j = i;
				break;
			}
		}
		if (pop->selected != j)
		{
			xprint("audio play\n");
			pop->selected = j;
		}
		break;

	case iKeyStatEnded:
		break;
	}

	return true;
}
#elif (OS==OS_ANDROID)
bool keyPopExit(iKeyStat stat, iPoint point)
{
	iPopup* pop = popExit;

	if (pop->bShow == false)
		return false;

#if 1 //화면에서 벗어나면 false ////////////////////////////////////////////////////

#endif ///////////////////////////////////////////////////////////////////////////

	if (pop->stat != iPopupProc)
		return true;

	int i, j = -1;

	switch (stat)
	{
	case iKeyStatBegan:
		for (i = 0; i < 2; i++)
		{
			if (containPoint(point, imgExitBtn[i]->touchRect(pop->closePoint, iSizeMake(20, 20))))
			{
				j = i;
				break;
			}
		}
		if (pop->selected != j)
		{
			xprint("audio play\n");
			pop->selected = j;
		}
		break;

	case iKeyStatMoved:
		i = pop->selected;
		if (i == -1) break;
		if (containPoint(point, imgExitBtn[i]->touchRect(pop->closePoint, iSizeMake(20, 20))) == false)
		{
#if 0 //#openAL
			audioPlay(0);
#endif
			pop->selected = -1;
		}
		break;

	case iKeyStatEnded:
		i = pop->selected;
		if (i == -1) break;
		pop->selected = -1;

		if (i == 0)
		{
			xprint("예\n");
#if (OS==OS_WINDOW)
			//프로그램 종료
			runWnd = false;
#elif(OS==OS_ANDROID)
#endif
		}
		else// if (i == 1)
		{
			xprint("아니오\n");
			showPopExit(false);
		}
		break;
	}

	return true;
}
#endif


