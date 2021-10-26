#include "Stage.h"
#include "StageUI.h"

#define INIT_STEP 50
#include "TileType.h"
#include "Map.h"
#include "Character.h"
#include "Loading.h"
#include "Menu.h"
#include "Ending.h"
#include "Battle.h"

#if (OS==OS_WINDOW)
#include "App.h"
#endif

#include "Common.h"

#define INIT_STEP 50

struct StageInfo
{
    const char* strPathTile;
    const char* strPathData;
};
#if (OS==OS_WINDOW)
StageInfo stageInfo[10] =
{
   {
        "assets/Image/Tile1.bmp",
        "map/map0.tile",
    },

    {
        "assets/Image/Tile1.bmp",
        "map/map1.tile",
    },

    {
        "assets/Image/Tile1.bmp",
        "map/map2.tile",
    },

    {
        "assets/Image/Tile1.bmp",
        "map/map3.tile",
    },

    {
        "assets/Image/Tile1.bmp",
        "map/map4.tile",
    },

    {
        "assets/Image/Tile1.bmp",
        "map/map5.tile",
    },

    {
        "assets/Image/Tile1.bmp",
        "map/map6.tile",
    },

    {
        "assets/Image/Tile1.bmp",
        "map/map7.tile",
    },

    {
        "assets/Image/Tile1.bmp",
        "map/map8.tile",
    },

    {
        "assets/Image/Tile1.bmp",
        "map/map9.tile",
    },
};
#elif (OS==OS_ANDROID)
StageInfo stageInfo[10] =
        {
                {
                        "Image/Tile1.bmp",
                        "map/map0.tile",
                },

                {
                        "Image/Tile1.bmp",
                        "map/map1.tile",
                },

                {
                        "Image/Tile1.bmp",
                        "map/map2.tile",
                },

                {
                        "Image/Tile1.bmp",
                        "map/map3.tile",
                },

                {
                        "Image/Tile1.bmp",
                        "map/map4.tile",
                },

                {
                        "Image/Tile1.bmp",
                        "map/map5.tile",
                },

                {
                        "Image/Tile1.bmp",
                        "map/map6.tile",
                },

                {
                        "Image/Tile1.bmp",
                        "map/map7.tile",
                },

                {
                        "Image/Tile1.bmp",
                        "map/map8.tile",
                },

                {
                        "Image/Tile1.bmp",
                        "map/map9.tile",
                },
        };
#endif

Texture** texs;
Texture* texStage;
Map* tEditor;
iPoint off;
iPoint tileOff;

int stageFrom, stageTo;
int lastHeroIndex;
int stageNum = 1;
int step = INIT_STEP; //총 발걸음 수

//Android 에서 drawStage 부분을 고쳐야 해서 이렇게 해봄
Texture* createTexStage()
{
    iGraphics* g = iGraphics::share();
    iSize size = iSizeMake(devSize.width, devSize.height);
    g->init(size);

    int x = tEditor->tileX;
    int y = tEditor->tileY;
    int tx = 8;

#if (OS==OS_WINDOW)
    igImage* img = g->createIgImage("assets/Image/Tile1.bmp");
#elif (OS==OS_ANDROID)
    igImage* img = g->createIgImage("Image/Tile1.bmp");
#endif

    for (int i = 0; i < x * y; i++)
    {
        //g->drawigImage(texs[tEditor->tileIndex[i]], (tileOff.x) + i % x * TILE_WSIZE, (tileOff.y) + i / x * TILE_HSIZE, TOP | LEFT);
        //g->drawigImage(img, (tileOff.x) + (i % x) * TILE_WSIZE, (tileOff.y) + (i / x) * TILE_HSIZE, TOP | LEFT,
        //               (tEditor->tileIndex[i] % tx) * TILE_WSIZE, (tEditor->tileIndex[i] / tx) * TILE_HSIZE, TILE_WSIZE, TILE_HSIZE,
        //               1.0f, 1.0f, 2, 0);
        g->drawigImage(img, (tileOff.x) + i % x * TILE_WSIZE, tileOff.y + i / x * TILE_HSIZE, TOP | LEFT,
                       (tEditor->tileIndex[i] % tx) * TILE_WSIZE, (tEditor->tileIndex[i] / tx) * TILE_HSIZE, TILE_WSIZE, TILE_HSIZE,
                       1.0f, 1.0f, 2, 0);
        //g->fillRect((tileOff.x) + i % x * TILE_WSIZE, (tileOff.y) + i / x * TILE_HSIZE, TILE_WSIZE, TILE_HSIZE, 4);
    }

    return g->getTexture();
}


//#issue! tileWeight 가 현재 다 바뀌었음. 가중치가 00,00,00 이런식으로 쪼개져서
//타일, 적, 오브젝트 관계로 지어주고 있기에, 이에 따라 캐릭터 및 전투 이벤트를 바꿔줘야 함.
void loadStage()
{
    callMapData();
#if (OS==OS_WINDOW) //#openAL
    for (int i = 0; i < SOUND_NUM; i++)
    {
        audioStop(i);
    }
    audioPlay(2);
#endif

    //===============================================
    //맵 타일 & 캐릭터 위치 세팅
    //===============================================
    bool fromMenu = (stageFrom == stageTo && stageFrom == 10 && stageTo == 10);
    stageFrom = stageTo %= 10;
    StageInfo* si = &stageInfo[stageFrom];
    texs = createImageDivide(8, 32, si->strPathTile);

    tEditor = new Map();
    tEditor->init(TILE_W, TILE_H, TILE_WSIZE, TILE_HSIZE);

    tEditor->loadA(&appData->mapData[MAP_FILE_SIZE * stageFrom]);
#if 1 //#test
    //map info
    for (int i = 0; i < TILE_W * TILE_H; i++)
    {
        if (i % TILE_W == 0) xprint("\n");
        xprint("%d ", tEditor->tileWeight[i]);
    }
#endif

    int x = devSize.width / 2 - TILE_WSIZE * TILE_W / 2;
    int y = devSize.height / 2 - TILE_HSIZE * TILE_H / 2;
    tileOff = iPointMake(x, y);

    //
    // createTexStage
    //
    texStage = createTexStage();

    //메뉴에서 왔을 때
    if (fromMenu)
    {
        step = INIT_STEP;
        off = iPointMake(x + TILE_WSIZE, y + TILE_HSIZE);
    }
    else// if (fromMenu == false)
    {
        int lastX = lastHeroIndex % TILE_W;
        int lastY = lastHeroIndex / TILE_W;
        int newX = x + TILE_WSIZE, newY = y + TILE_HSIZE;

        if (lastX == 0)
        {
            //우측
            for (int i = 0; i < TILE_H; i++)
            {
                if (tEditor->tileWeight[TILE_W * i + TILE_W - 1] > 9)
                {
                    //워프 포인트보다 한블럭 앞에위치
                    newX = TILE_W - 2;
                    newY = i;
                    break;
                }
            }
            off = iPointMake(x + newX * TILE_WSIZE, y + newY * TILE_HSIZE);
        }
        else if (lastX == TILE_W - 1)
        {
            //좌측
            for (int i = 0; i < TILE_H; i++)
            {
                if (tEditor->tileWeight[i * TILE_W + 0] > 9)
                {
                    newX = 1;
                    newY = i;
                    break;
                }
            }
            off = iPointMake(x + newX * TILE_WSIZE, y + newY * TILE_HSIZE);
        }
        else if (lastY == 0)
        {
            //하단
            for (int i = 0; i < TILE_W; i++)
            {
                if (tEditor->tileWeight[i + (TILE_H - 1) * TILE_W] > 9)
                {
                    newX = i;
                    newY = TILE_H - 2;
                    break;
                }
            }
            off = iPointMake(x + newX * TILE_WSIZE, y + newY * TILE_HSIZE);
        }
        else if (lastY == TILE_H - 1)
        {
            //상단
            for (int i = 0; i < TILE_W; i++)
            {
                if (tEditor->tileWeight[i + 0] > 9)
                {
                    newX = i;
                    newY = 1;
                    break;
                }
            }
            off = iPointMake(x + newX * TILE_WSIZE, y + newY * TILE_HSIZE);
        }
        else
        {
            //배틀에서 다시 왔을때
            xprint("critical error.....+ exception cave\n");

            off = iPointMake(x + lastX * TILE_WSIZE, y + lastY * TILE_HSIZE);
        }
    }
    loadCharacter(off, si->strPathData);
    //===============================================
    // popUp
    //===============================================
    setStringName("assets/font/DungGeunMo.ttf");
    //TopUI
    createPopTopUI();
    createPopSetting();
    createPopStageOption();
    createPopStageExit();
    createPopStageHow();
    createPopInven();

    //BottomUI
    createPopBottomUI();

    createPopOverStep();
    createPopStageStr();
    createPopStepStr();

    showPopStageStr(true);
    showPopStepStr(true);
    showPopOverStep(false);

    showPopTopUI(true);
    showPopSetting(false);
    showPopStageHow(false);
    showPopInven(false);
    showPopStageOption(false);
    showPopStageExit(false);
}

void freeStage()
{
    int i, j;

    for (int i = 0; i < 256; i++)
        freeImage(texs[i]);
    delete texs;

    delete tEditor;

    delete texStage;

    freeCharacter();

    //===============================================
    // popUp
    //===============================================

    //TopUI
    freePopTopUI();
    freePopSetting();
    freePopStageHow();
    freePopStageExit();
    freePopStageOption();
    freePopInven();

    //BottomUI
    freePopBottomUI();

    freePopOverStep();
    freePopStageStr();
    freePopStepStr();
}

int heroIndex()
{
    int cx = mainCharacter->position.x; cx /= TILE_WSIZE;
    int cy = mainCharacter->position.y; cy /= TILE_HSIZE;
    int tx = tileOff.x; tx /= TILE_WSIZE;
    int ty = tileOff.y; ty /= TILE_HSIZE;
    int index = TILE_W * (cy - ty) + (cx - tx);

    return index;
}

extern iPopup* popOverStep;
void drawStage(float dt)
{

    //=========================================================
    // draw Stage
    //=========================================================

    //BackGround
    setRGBA(0, 0, 0, 1);
    clearRect();
    setRGBA(1, 1, 1, 1);

    int x = tEditor->tileX;
    int y = tEditor->tileY;
    lastHeroIndex = heroIndex();

    drawImage(texStage, 0, 0, TOP | LEFT);

    drawCharacter(dt, tileOff);

    int newHeroIndex = heroIndex();

    //발걸음 체크
    if (lastHeroIndex != newHeroIndex && step > 0)
    {
        if (tEditor->tileWeight[newHeroIndex] < 10)
            step -= tEditor->tileWeight[newHeroIndex];
        else // if(tEditor->tileWeight[newHeroIndex] >= 10)
            step -= 1;

        if (step < 0)
            step = 0;

        if (step <= 0)
        {
            step = 0;
            showPopOverStep(true);

            //to do... 게임 멈추기
        }
        lastHeroIndex = newHeroIndex;
    }

    //===================
    // 워프
    //===================

    int locationWarp = tEditor->tileWeight[newHeroIndex];
    if (locationWarp >= 10 && locationWarp < 100)
    {
        stageTo = locationWarp - 10;
        setLoading(gs_stage, freeStage, loadStage);
    }

    //===================
    // 적 만남
    //===================

//#need Update! 파일입출력 저장 -> AppData 구조체 이용
    if (locationWarp > 99 && locationWarp < 10000)
    {
        tEditor->tileWeight[newHeroIndex] = 1;
        tEditor->saveA(&appData->mapData[MAP_FILE_SIZE * stageFrom]);
#if 1 //#loadBattle
        setLoading(gs_battle, freeStage, loadBattle);
#endif
    }

    //=========================================================
    // draw Popup
    //=========================================================
    drawPopStageStr(dt);
    drawPopStepStr(dt);

    //BottomUI
    drawPopBottomUI(dt);

    //TopUI
    drawPopTopUI(dt);

    drawPopOverStep(dt);
}

iPoint first = iPointZero;
void keyStage(iKeyStat stat, iPoint point)
{
    if (keyPopOverStep(stat, point))
        return;

    if (keyPopTopUI(stat, point))
        return;

    if (keyPopBottomUI(stat, point))
        return;

    switch (stat) {

        case iKeyStatBegan:
            first = point;
            break;

        case iKeyStatMoved:

            break;

        case iKeyStatEnded:
#if (OS==OS_ANDROID)
        {
            iPoint dp = point - first;

            if (fabs(dp.x) > fabs(dp.y))
            {
                if (dp.x > 0)
                    charDir = 1; //right
                else
                    charDir = 0; //left
            }
            else
            {
                if (dp.y > 0)
                    charDir = 3; //up
                else
                    charDir = 2; //down
            }
        }
#endif
            break;
    }
}

