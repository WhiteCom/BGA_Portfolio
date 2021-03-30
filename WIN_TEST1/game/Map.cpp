#include "Map.h"
#include "TileType.h"
#include "Character.h"

#if 0
#include "Loading.h"
#include "Intro.h"
#include "MapGlobal.h"

Texture* bg;

MapTile* t;
static const char* MapStr; //타일 맵에 해당하는 문자열 인자를 저장하기 위한 변수.
static float MapDt; //mapTime에서 들고올 시간.

void loadMap()
{
    if (warpEvent)
        warpEvent = false;

    bg = createImage("assets/map.jpg");

	t = new MapTile();
}

void freeMap()
{
    freeImage(bg);
	delete t;
}

void newMap()
{
    printf("---new map---\n");
    loadMap();
    
    // to do....
    //1. 새로운 타일맵과 함께, 아이템 및 적 위치 갱신하여 그릴수 있도록.
    //or
    //2. 기존 타일맵 재활용 (적 + 아이템 위치 정보를 수정하여 그릴 수 있도록)
    
    //drawMap
    //현재 문제점 : 이 파일의 전역 문자열변수로는 이전 맵에 있는 문자를 갖고오지 못함.
    //따라서 아래의 mapTime같이 함수포인터로 인자를 넘겨주고 하는 그런걸 해줘야함.
    if (MapStr)
        MapStr = "boss";
    t->makeMap(MapStr, 3);
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

    drawCharacter(MapDt, t);

    //if (warpEvent)
    //    setLoading(gs_map, freeMap, newMap, mapTime);
}

void mapTime(float dt)
{
    MapDt = dt;
    printf("dt : %f\n", MapDt);
}

void mapStr(const char* str)
{
    MapStr = str;
    printf("str : %s\n", MapStr);
}

void drawMap(float dt, const char* str)
{
    //draw Bg
    setRGBA(0.5f, 0.5f, 0.5f, 1.0f);
    clearRect();
    drawImage(bg, 0, 0, TOP | LEFT);

    //drawMap
    MapStr = str;
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

    if (warpEvent)
        setLoading(gs_map, freeMap, newMap, mapTime);
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

#else

//타입 : 필드 - 벽 & 구멍 - 워프 이미지 - 아이템 - 적
#define NoneSection				{NoneIdx, INF, INF}
#define WarpSection				{WarpIdx, 0, 1}

//0 : 초원, 1 : 황야, 2 : 얼음, 3 : 불, 4 : 보스
#define ForestField(n)			{ForestIdx[n], 0, 0}
#define DesertField(n)			{DesertIdx[n], 0, 0}
#define IceField(n)				{IceIdx[n], 0, 0}
#define FireField(n)			{FireIdx[n], 0, 0}
#define BossField(n)			{BossIdx[n], 0, 0}

#define ForestWall(n)			{ForestWallIdx[n], 1, 2}
#define DesertWall(n)			{DesertWallIdx[n], 1, 2}
#define IceWall(n)				{IceWallIdx[n], 1, 2}
#define FireWall(n)				{FireWallIdx[n], 1, 2}
#define BossWall(n)				{BossWallIdx[n], 1, 2}

//----------------------------------------
//Tile1.bmp 내의 필드종류
//#need update
//----------------------------------------
int ForestIdx[4] = {
    20, 76, 86,
};

int DesertIdx[4] = {
    27, 84,
};

int IceIdx[4] = {
    71, 179, 181,
};

int FireIdx[4] = {
    14, 15,
};

int BossIdx[4] = {
    231,
};

//공간아닌곳
int NoneIdx = 128, WarpIdx = 132; //warp : 132

//----------------------------------------
//Tile1.bmp 내의 벽 종류 (통행불가 지역)
//#need update
//----------------------------------------
int ForestWallIdx[4] = {
    18, 116,
};

int DesertWallIdx[4] = {
    26, 121,
};

int IceWallIdx[4] = {
    42, 187, 131, 189
};

int FireWallIdx[4] = {
    89, 126,
};

int BossWallIdx[4] = {
    129,
};

bool warpEvent;// delete !!!!!

Texture* texBg;

Texture** texTile;
Texture** texDataObj;
Texture** texObject;

// assets/obj/obj%d.png
MObject dataObj[3] = {
    { "assets/tree.png", {0, 0} },
    { "assets/stone.png", {0, 0} },
};

MTile dataTile[tileW * tileH] = {
    {0, 1, 0},{0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},
    {0, 1, 0},{0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},
    {0, 1, 0},{0, 9, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},
    {0, 9, 0},{0, 9, 1},{0, 9, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},
    {0, 1, 0},{0, 9, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},
    {0, 1, 0},{0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},
    {0, 1, 0},{0, 1, 0},{0, 1, 0}, {0, 9, 2},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},
    {0, 1, 0},{0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},
    {0, 1, 0},{0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},
    {0, 1, 0},{0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},
    {0, 1, 0},{0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},
    {0, 1, 0},{0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},{0, 1, 0}, {0, 1, 0},
};


struct FObject
{
    uint8 index;
    iPoint position;

    void paint(float dt)
    {
        Texture* tex = texObject[index];
        MObject* mo = &dataObj[index];
        drawImage(tex, position.x + mo->position.x, position.y + mo->position.y, TOP | LEFT);
    }
};

FObject* fobj;
int fobjNum;

void loadMap()
{
    texBg = createImage("assets/map.jpg");

    int i, j;

    texTile = createImageDivide(8, 32, "assets/Image/tile1.bmp");

    // 10 x 10
    fobjNum = 0;
    int fobjIndex[100], fobjX[100], fobjY[100];
    for (j = 0; j < tileH; j++)
    {
        for (i = 0; i < tileW; i++)
        {
            MTile* mt = &dataTile[tileW * j + i];
            if (mt->obj > 0)
            {
                fobjIndex[fobjNum] = mt->obj - 1;
                fobjX[fobjNum] = i;
                fobjY[fobjNum] = j;
                fobjNum++;

            }
        }
    }


    fobj = new FObject[fobjNum];
    texObject = new Texture * [fobjNum];
    memset(texObject, 0x00, sizeof(Texture*) * fobjNum);

    for (i = 0; i < fobjNum; i++)
    {
        fobj[i].index = fobjIndex[i];
        fobj[i].position = iPointMake(32 * fobjX[i] + 16, 32 * fobjY[i] + 16);

        texObject[fobjIndex[i]] = createImage(dataObj[fobjIndex[i]].strPath);
    }
}

void freeMap()
{
    freeImage(texBg);

    int i;

    for (i = 0; i < 256; i++)
        freeImage(texTile[i]);
    delete texTile;

    delete fobj;

    for (i = 0; i < fobjNum; i++)
    {
        if( texObject[i] )
            freeImage(texObject[i]);
    }
    delete texObject;
}

void drawMap(float dt, const char* str)
{
    //draw Bg
    setRGBA(0.5f, 0.5f, 0.5f, 1.0f);
    clearRect();
    drawImage(texBg, 0, 0, TOP | LEFT);

    int i;

    for (i = 0; i < tileW * tileH; i++)
    {
        MTile* mt = &dataTile[i];
        Texture* tex = texTile[mt->img];
        int x = i % tileW * tileWSize;
        int y = i / tileW * tileHSize;
        drawImage(tex, x, y, TOP | LEFT);

        if (mt->weight == 9)
        {
            setRGBA(1, 0, 0, 0.3f);
            fillRect(x, y, tileWSize, tileHSize);
            setRGBA(1, 1, 1, 1);
        }
    }

    // obj + hero : sorting

    for (i = 0; i < fobjNum; i++)
    {
        FObject* o = &fobj[i];
        Texture* tex = texObject[o->index];
        drawImage(tex, o->position.x, o->position.y, TOP | LEFT);
    }

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

    drawCharacter(dt);

}

void keyMap(iKeyStat stat, iPoint point)
{
    switch (stat)
    {
    case iKeyStatBegan:
        break;

    case iKeyStatMoved:
        break;

    case iKeyStatEnded:
        break;
    }
}
#endif