#include "Loading.h"

int gameState = gs_intro, toGameState;
MethodLoad methodFree, methodLoad;
MethodTime methodTime;
float loadingDt;

#define _loadingDt 1.5f

//static Texture* texLoading;
static Texture* texGameOver;

void setLoading(int gameState, MethodLoad free, MethodLoad load, MethodTime time)
{
	if (loadingDt)
		return;

	toGameState = gameState;
	methodFree = free;
	methodLoad = load;
	methodTime = time;
	loadingDt = 0.0000001f;

	loadProgress(100);
}

void drawLoading(float dt)
{
	if (loadingDt == 0.0f)
		return;

	float a = 1.0f;


#if 0 //#issue! gameState
	if (loadingDt < _loadingDt)
	{
		loadingDt += dt;
		if (loadingDt > _loadingDt)
			loadingDt = _loadingDt;

		a = loadingDt / _loadingDt;

		setProgress(a * 100);
	}
	else if (loadingDt == _loadingDt)
	{
		loadingDt = _loadingDt + 0.0000001f;

		if (methodFree)
			methodFree();
		
		a = 1.0f; //완전까맣게
	}

	else if (loadingDt < _loadingDt + 1.0f)
	{
		//delay 0.75f;
		loadingDt += dt;
		if (loadingDt >= _loadingDt + 1.0f)
		{
			loadingDt = _loadingDt + 1.000001f;
			if (methodLoad)
				methodLoad();

			gameState = toGameState;
		}
		a = 1.0f; //완전까맣게
	}

	else if (loadingDt < _loadingDt * 2 + 1.0f)
	{
		loadingDt += dt;
		if (loadingDt > _loadingDt * 2 + 1.0f)
		{
			loadingDt = 0.0f;
			freeProgress();
			return;
		}

		a = 1.0f - (loadingDt - _loadingDt - 1.0f) / _loadingDt; //점점 밝아진다.
	}
#else
	if (loadingDt < _loadingDt)
	{
		loadingDt += dt;
		if (loadingDt > _loadingDt)
			loadingDt = _loadingDt;

		a = loadingDt / _loadingDt;

		setProgress(a * 100);
	}
	else if (loadingDt == _loadingDt)
	{
		loadingDt = _loadingDt + 0.0000001f;

		if (methodFree)
			methodFree();

		if (methodLoad)
			methodLoad();

		gameState = toGameState;

		a = 1.0f; //완전까맣게
	}
	else if (loadingDt < _loadingDt * 2)
	{
		loadingDt += dt;
		if (loadingDt >= _loadingDt * 2)
		{
			loadingDt = 0.0f;
			freeProgress();
			return;
		}

		a = 1.0f - (loadingDt - _loadingDt) / _loadingDt; //점점 밝아진다.
	}
#endif

	setRGBA(0, 0, 0, a);
	fillRect(0, 0, devSize.width, devSize.height);
	setRGBA(1, 1, 1, 1);

	drawProgress(dt);
	
}

Texture* texProgressDot;
#define PROGRESS_DOT_DT 0.5f
struct ProgressDot
{
	iPoint p = iPointMake(100, 200);
	float delta = PROGRESS_DOT_DT;

	bool set(iPoint p)
	{
		if (delta < PROGRESS_DOT_DT)
			return false;
		this->p = p;
		delta = 0.0f;
		return true;
	}
	bool paint(float dt)
	{
		float s = 1.0f - delta / PROGRESS_DOT_DT;

		Texture* t = texProgressDot;
		drawImage(t, p.x, p.y, VCENTER | HCENTER, 0, 0,
			t->width, t->height, s,  s, 2, 0);
			
		delta += dt;
		if (delta > PROGRESS_DOT_DT)
			return true;
		return false;
	}
};

ProgressDot* _progressDot;
ProgressDot** progressDot;
int progressDotNum;
float progressDotCreateDt;
#define _PROGRESS_DOT_CREATE_DT 1.0f / 12
int progressDotCreateDtIndex;

int progressMax;
int progressCurr;
iStrTex* stProgressPercent;
Texture* methodStProgressPercent(const char* str);

void loadProgress(int max)
{
	_progressDot = new ProgressDot[100];

	progressDot = new ProgressDot * [100];
	progressDotNum = 0;
	progressDotCreateDt = 0.0f;
	progressDotCreateDt = 0.0f;
	progressDotCreateDtIndex = 0;

	iGraphics* g = iGraphics::share();
	iSize size = iSizeMake(48, 48);
	g->init(size);

	setRGBA(1, 1, 1, 1);
	g->fillRect(0, 0, size.width, size.height, size.width / 2);

	texProgressDot = g->getTexture();

	progressMax = max;
	progressCurr = 0;

	stProgressPercent = new iStrTex(methodStProgressPercent);
	stProgressPercent->setString("0");

}

Texture* methodStProgressPercent(const char* str)
{
	iGraphics* g = iGraphics::share();
	iSize size = iSizeMake(devSize.width, devSize.height);
	g->init(size);
	setRGBA(0, 0, 0, 1);
	g->fillRect(0,0,size.width, size.height);
	setRGBA(1, 1, 1, 1);

	size = iSizeMake(128, 32);
	g->init(size);

	int percent = atoi(str);
	setStringSize(30);
	setStringRGBA(1, 1, 1, 1);

	g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER,
		"%d%%%%%%%", percent);

	return g->getTexture();
}

void freeProgress()
{
	delete _progressDot;
	delete progressDot;

	freeImage(texProgressDot);

	delete stProgressPercent;
}

void drawProgress(float dt)
{
	for (int i = 0; i < progressDotNum; i++)
	{
		if (progressDot[i]->paint(dt))
		{
			progressDot[i] = progressDot[progressDotNum - 1];
			progressDotNum--;
			i--;
		}
	}

	stProgressPercent->paint(iPointMake(devSize.width / 2, devSize.height / 2), VCENTER | HCENTER);
	
	progressDotCreateDt += dt;
	if (progressDotCreateDt > _PROGRESS_DOT_CREATE_DT)
	{
		progressDotCreateDt -= _PROGRESS_DOT_CREATE_DT;

		float degree = (progressDotCreateDtIndex % 12) * 30;
		progressDotCreateDtIndex++;

		iPoint c = iPointMake(devSize.width / 2, devSize.height / 2);
		iPoint p = iPointRotate(c + iPointMake(80, 0), c, degree);
		for (int i = 0; i < 100; i++)
		{
			if (_progressDot[i].set(p))
			{
				progressDot[progressDotNum] = &_progressDot[i];
				progressDotNum++;
				break;
			}
		}
	}
}

void setProgress(int tick)
{
	progressCurr = tick;
	int percent = 100.0f * progressCurr / progressMax;
	stProgressPercent->setString("%d", percent);
}