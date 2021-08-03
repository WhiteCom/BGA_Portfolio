#include "Intro.h"

#include "Loading.h"
#include "MapEditor.h"
#include "Menu.h"
#include "Stage.h"

Texture* texIntro;

float viewDt;

void loadIntro()
{
	iGraphics* g = iGraphics::share();

	iSize size = iSizeMake(640, 480);
	g->init(size);
	setRGBA((random() % 256)/255.f,
			(random() % 256) / 255.f,
			(random() % 256) / 255.f, 0.2f);
	g->fillRect(0, 0, size.width, size.height);

	setStringSize(30);
	setStringRGBA(1, 1, 1, 1);
	setStringBorder(2);
	setStringBorderRGBA(0, 0, 0, 1);
	
	g->drawString(size.width / 2, 50, VCENTER | HCENTER, "안녕하세요~!");
	g->drawString(size.width / 2, 100, VCENTER | HCENTER, "안다흡의 포트폴리오입니다.");
	g->drawString(size.width / 2, 150, VCENTER | HCENTER, "본 게임은 턴제 RPG 게임입니다.");
	g->drawString(size.width / 2, 200, VCENTER | HCENTER, "마우스왼쪽버튼 또는");
	g->drawString(size.width / 2, 250, VCENTER | HCENTER, "Space버튼을 누르면 화면이 넘어갑니다");
	texIntro = g->getTexture();
	viewDt = 0.0f;

}

void freeIntro()
{
	freeImage(texIntro);
}

void drawIntro(float dt)
{
	setRGBA(0, 0, 0, 1.0f);
	clearRect();
	setRGBA(1, 1, 1, 1.0f);

	viewDt += dt;

	float a = _sin(180 * viewDt);
	if (a < 0)
		a *= -1;
	setRGBA(1, 1, 1, a);
	drawImage(texIntro, devSize.width / 2, devSize.height / 2, VCENTER | HCENTER);
	setRGBA(1, 1, 1, 1);

	if (keyDown & keysSpace)
	{
		setLoading(gs_menu, freeIntro, loadMenu);
	}

	setRGBA(1, 0, 0, 1);

	setDotSize(10.0f);
	drawDot(100, 100);

	setDotSize(50.f);
	drawCircle(100, 250, 50.f);

	setLineWidth(2);
	drawLine(500, 500, 550, 600);

	drawRect(150, 150, 100, 150, 10);
	setLineWidth(1);


	setRGBA(0, 1, 0, 1);
	fillRect(400, 100, 200, 100, 30);
	setRGBA(1, 1, 1, 1);
}

void keyIntro(iKeyStat stat, iPoint point)
{
	switch(stat)
	{
		case iKeyStatBegan:
			setLoading(gs_menu, freeIntro, loadMenu);
			break;
		case iKeyStatMoved:
			break;
		case iKeyStatEnded:
			break;
	}
}