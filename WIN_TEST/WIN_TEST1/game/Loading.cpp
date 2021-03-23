#include "Loading.h"

int gameState = gs_intro, toGameState;
MethodLoad methodFree, methodLoad;
float loadingDt;

#define _loadingDt 0.5f

static Texture* texLoading;

void setLoading(int gameState, MethodLoad free, MethodLoad load)
{
	if (loadingDt)
		return;

	toGameState = gameState;
	methodFree = free;
	methodLoad = load;
	loadingDt = 0.0000001f;

	texLoading = createImage("assets/loading/loading.jpg");
}
void drawLoading(float dt)
{
	if (loadingDt == 0.0f)
		return;

	float a = 1.0f;

	if (loadingDt < _loadingDt)
	{
		loadingDt += dt;
		if (loadingDt > _loadingDt)
			loadingDt = _loadingDt;

		a = loadingDt / _loadingDt;
	}
	else if (loadingDt == _loadingDt)
	{
		loadingDt = _loadingDt + 0.0000001f;

		if (methodFree)
			methodFree();
		if (methodLoad)
			methodLoad();
		gameState = toGameState;

		a = 1.0f;
	}
	else if (loadingDt < _loadingDt * 2)
	{
		loadingDt += dt;
		if (loadingDt > _loadingDt * 2)
		{
			loadingDt = 0.0f;
			freeImage(texLoading);
			return;
		}
		
		a = 1.0f - (loadingDt - _loadingDt) / _loadingDt;
	}

	setRGBA(0, 0, 0, a);
	fillRect(0, 0, devSize.width, devSize.height);
	drawImage(texLoading, devSize.width / 2, devSize.height / 2,
		VCENTER | HCENTER, 0, 0, texLoading->width, texLoading->height,
		1.0f, 1.0f, 2, 0);
	setRGBA(1, 1, 1, 1);
}