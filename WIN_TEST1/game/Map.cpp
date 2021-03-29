#include "Map.h"
#include "Loading.h"
#include "Intro.h"

Texture* bg;

MapTile* t;

void loadMap()
{
    if (keyStat == keysSpace)
        keyStat &= ~keysSpace;

    bg = createImage("assets/map.jpg");

	t = new MapTile();
}

void freeMap()
{
    freeImage(bg);
	delete t;
}

void resetMap()
{
    loadMap();
    
    // to do....
    //1. 새로운 타일맵과 함께, 아이템 및 적 위치 갱신하여 그릴수 있도록.
    //or
    //2. 기존 타일맵 재활용 (적 + 아이템 위치 정보를 수정하여 그릴 수 있도록)
}

void drawMap(float dt, const char* str)
{
    //draw Bg
    setRGBA(0.5f, 0.5f, 0.5f, 1.0f);
    clearRect();
    drawImage(bg, 0, 0, TOP | LEFT);

    //drawMap
    t->makeMap(str, 0);
    t->drawTile();

    //drawCharacter
    static bool test = true;
    iPoint off = iPointZero;

    if (test)
    {
        test = false;
        if (off == iPointZero)
            off = iPointMake(tileWSize * 2, tileHSize * 2); //초기씬에서
        else
        {
            //#need update
            //씬 전환시 워프좌표 이동해야함.
        }
        loadCharacter(off);
    }

    drawCharacter(dt, t);

    if (keyStat == keysSpace)
        setLoading(gs_map, freeMap, resetMap);
        //#need update 
    //setLoading 오버로딩 함수가 필요함. 
    //resetMap을 이용할때, 인자값이 들어가서, 새로운 타일맵을 생성할 수 있도록 해줘야함.
}

void keyMap(iKeyStat stat, iPoint point)
{
    switch (stat)
    {
    case iKeyStatBegan:
        setLoading(gs_map, freeMap, loadMap);
        break;

    case iKeyStatMoved:
        break;

    case iKeyStatEnded:
        break;
    }
}