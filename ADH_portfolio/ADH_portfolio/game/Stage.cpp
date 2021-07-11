#include "Stage.h"

#include "TileType.h"
#include "Map.h"
#include "Character.h"
#include "Loading.h"
#include "Menu.h"
#include "Battle.h"
#include "Ending.h"

#include "Common.h"

#define INIT_STEP 50

struct StageInfo
{
    const char* strPathTile;
    const char* strPathData;
};

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

Texture** texs;
Map* tEditor;
iPoint off;
iPoint tileOff;

int stageFrom, stageTo;
int lastHeroIndex;
int stageNum = 1;
int step = INIT_STEP; //총 발걸음 수

#if 1
const char* str = "_copy";
#endif

char gameFile[2048];

void copyFile(const char* szFormat, ...)
{
    char szText[1024];
    va_start_end(szText, szFormat);

    FILE* pfr = fopen(szText, "rb");
    if (pfr != NULL) //해당 이름의 파일이 있는 경우
    {
        fclose(pfr);
        return;
    }

    StageInfo* si = &stageInfo[stageFrom];
    pfr = fopen(si->strPathData, "rb");

    FILE* pfw = fopen(szText, "wb");

    char fileContent;
    while (feof(pfr) == 0)
    {
        fread(&fileContent, sizeof(char), 1, pfr);
        fwrite(&fileContent, sizeof(char), 1, pfw);
    }

    fclose(pfr);
    fclose(pfw);
}

//#issue! tileWeight 가 현재 다 바뀌었음. 가중치가 00,00,00 이런식으로 쪼개져서 
//타일, 적, 오브젝트 관계로 지어주고 있기에, 이에 따라 캐릭터 및 전투 이벤트를 바꿔줘야 함.
void loadStage()
{

//===============================================
//맵 타일 & 캐릭터 위치 세팅
//===============================================
    bool fromMenu = (stageFrom == stageTo && stageFrom == 10 && stageTo == 10);
    stageFrom = stageTo %= 10;
    StageInfo* si = &stageInfo[stageFrom];

    texs = createImageDivide(8, 32, si->strPathTile);

    strcpy(gameFile, si->strPathData);
    strcat(gameFile, str);
    copyFile(gameFile);

    tEditor = new Map();
    tEditor->init(TILE_W, TILE_H, TILE_WSIZE, TILE_HSIZE);
#if 1
    tEditor->load(gameFile);
#else
    int stage = 0;
    tEditor->loadA(&appData->mapData[MAP_FILE_SIZE * stage]);
#endif

#if 1
    //map info
    for (int i = 0; i < TILE_W * TILE_H; i++)
    {
        if (i % TILE_W == 0) printf("\n");
        printf("%d ",tEditor->tileWeight[i]);
    }
#endif

    int x = devSize.width / 2 - TILE_WSIZE * TILE_W / 2;
    int y = devSize.height / 2 - TILE_HSIZE * TILE_H / 2;
    tileOff = iPointMake(x, y);

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
                if (tEditor->tileWeight[TILE_W * i + TILE_W -1] > 9)
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
            printf("critical error.....+ exception cave\n");
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
    createPopStageHow();
    createPopInven();

    //BottomUI
    createPopBottomUI();

    createPopOverStep();
    createPopStageStr();
    createPopStepStr();

    showPopStageStr(true);
    showPopStepStr(true);
    showPopTopUI(true);
}

void freeStage()
{
    int i, j;

    for (int i = 0; i < 256; i++)
        freeImage(texs[i]);
    delete texs;

    delete tEditor;

    freeCharacter();

//===============================================
// popUp
//===============================================

    //TopUI
    freePopTopUI();
    freePopSetting();
    freePopStageHow();
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

void drawStage(float dt)
{
    clearRect();

//=========================================================
// draw Stage
//=========================================================

//BackGround
    setRGBA(0, 0, 0, 1);
    fillRect(0, 0, devSize.width, devSize.height);
    setRGBA(1, 1, 1, 1);

    int x = tEditor->tileX;
    int y = tEditor->tileY;
    lastHeroIndex = heroIndex();

    for (int i = 0; i < x * y; i++)
    {
        if (tEditor->tileIndex[i] > -1)
            drawImage(texs[tEditor->tileIndex[i]], (tileOff.x) + i % x * TILE_WSIZE, (tileOff.y) + i / x * TILE_HSIZE, TOP | LEFT);
        else
        {
            setRGBA(1, 1, 1, 0.5f);
            drawImage(texs[tEditor->tileIndex[i]], (tileOff.x) + i % x * TILE_WSIZE, (tileOff.y) + i / x * TILE_HSIZE, TOP | LEFT);
            setRGBA(1, 1, 1, 1);
        }

    }

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
        //tEditor->save(gameFile);
        setLoading(gs_stage, freeStage, loadStage);
    }

    //===================
    // 적 만남
    //===================

//#need Update! 파일입출력 저장 -> AppData 구조체 이용
    if (locationWarp > 99 && locationWarp < 10000)
    {
        tEditor->tileWeight[newHeroIndex] = 1;
        tEditor->save(gameFile);
        setLoading(gs_battle, freeStage, loadBattle);
    }
    
//=========================================================
// draw Popup
//=========================================================

    drawPopOverStep(dt);
    drawPopStageStr(dt);
    drawPopStepStr(dt);

    //BottomUI
    drawPopBottomUI(dt);

    //TopUI
    drawPopTopUI(dt);
    drawPopSetting(dt);
    drawPopStageHow(dt);
    drawPopInven(dt);

}

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
        break;

    case iKeyStatMoved:
        break;

    case iKeyStatEnded:
        break;
    }
}

//=========================================================
// popTopUI : 메뉴, 설정, 인벤토리
//=========================================================

iPopup** popTopUI; //메뉴, 설정, 인벤토리
iImage** imgTopBtn; //메뉴, 설정, 인벤토리

void drawPopTopUIBefore(iPopup* pop, float dt, float rate);

void createPopTopUI()
{
    iImage* img;
    Texture* tex;

    popTopUI = new iPopup * [2];
    imgTopBtn = new iImage * [3]; // 설정, 방법, 인벤

    const char* imgPath[3] = {
        "assets/setting2.png",
        "assets/how.png",
        "assets/Inven2.png",
    };
    
    for (int k = 0; k < 2; k++)
    {
        iPopup* pop = new iPopup();
        for (int i = 0; i < 3; i++)
        {
            img = new iImage();
            for (int j = 0; j < 2; j++)
            {
                iGraphics* g = iGraphics::share();
                iSize size = iSizeMake(50, 50);
                g->init(size);
                igImage* btnImg = g->createIgImage(imgPath[i]);
                if (j == 0)
                {
                    setRGBA(0, 0, 0, 1);
                    g->fillRect(0, 0, size.width, size.height, 5);
                    setRGBA(1, 1, 1, 1);
                    g->drawRect(0, 0, size.width, size.height, 5);
                    g->drawigImage(btnImg, size.width / 2, size.height / 2, VCENTER | HCENTER,
                        0, 0, btnImg->GetWidth(), btnImg->GetHeight(), (float)50 / 512, (float)50 / 512,
                        2, 0);
                }
                else
                {
                    setRGBA(1, 1, 0, 1);
                    g->fillRect(0, 0, size.width, size.height, 5);
                    setRGBA(0, 0, 0, 1);
                    g->drawRect(0, 0, size.width, size.height, 5);
                    setRGBA(1, 1, 1, 1);
                    g->drawigImage(btnImg, size.width / 2, size.height / 2, VCENTER | HCENTER,
                        0, 0, btnImg->GetWidth(), btnImg->GetHeight(), (float)50 / 512, (float)50 / 512, 
                        2, 0);
                }
                tex = g->getTexture();
                img->addObject(tex);
                freeImage(tex);
            }
            img->position = iPointMake((25 + i*80), 25);
            pop->addObject(img);

            imgTopBtn[i] = img;
        }
        if (k == 0)
        {
            pop->methodBefore = drawPopTopUIBefore;
            
        }
        pop->style = iPopupAlpha;
        pop->openPoint = iPointZero;
        pop->closePoint = iPointZero;

        popTopUI[k] = pop;
    }
}

void freePopTopUI()
{
    for (int i = 0; i < 2; i++)
        delete popTopUI[i];
    delete popTopUI;
    delete imgTopBtn;
}

void drawPopTopUIBefore(iPopup* pop, float dt, float rate)
{
    for (int i = 0; i < 3; i++)
        imgTopBtn[i]->setTexObject(i == popTopUI[0]->selected);
}
void drawPopTopUI(float dt)
{
    for (int i = 0; i < 2; i++)
        popTopUI[i]->paint(dt);
}
void showPopTopUI(bool show)
{
    for (int i = 0; i < 2; i++)
        popTopUI[i]->show(show);
    if (show)
    {

    }
}
bool keyPopTopUI(iKeyStat stat, iPoint point)
{
    if (keyPopSetting(stat, point) ||
        keyPopStageOption(stat, point) ||
        keyPopStageExit(stat, point)
        )
        return true;

    iPopup* pop = popTopUI[0];

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
            printf("Setting\n");
            pop->selected = -1;
            showPopSetting(true);
        }
        else if (pop->selected == 1)
        {
            printf("How\n");
            pop->selected = -1;
            showPopHow(true);
        }
        else //if (pop->selected == 2)
        {
            printf("Inven\n");
            pop->selected = -1;
            showPopInven(true);
        }
        break;

    case iKeyStatMoved:
        for (i = 0; i < 3; i++)
        {
            if (containPoint(point, imgTopBtn[i]->touchRect(pop->closePoint)))
            {
                j = i;
                break;
            }
        }
        if (pop->selected != j)
        {
            printf("audio Play\n");
            //audioPlay(0);
            pop->selected = j;
        }
        break;

    case iKeyStatEnded:
        break;
    }

    return true;
}

//=========================================================
// popSetting : 설정창
//=========================================================
iPopup* popSetting;
iImage** imgSetBtn;

void drawMethodPopSet(iPopup* pop, float dt, float rate);

void createPopSetting()
{
    popSetting = new iPopup();

    iImage* img;
    Texture* tex;
    iPopup* pop = new iPopup();
    
    //Bg
    iGraphics* g = iGraphics::share();
    iSize size = iSizeMake(480, 480);
    g->init(size);

    setRGBA(0.5f, 0.5f, 0.5f, 0.8f);
    g->fillRect(0, 0, size.width, size.height, 10);
    setRGBA(1, 1, 1, 1);
    
    setStringSize(40);
    setStringRGBA(0, 0, 0, 1);
    setStringBorder(2);
    setStringBorderRGBA(1, 1, 1, 1);
    g->drawString(size.width / 2, size.height / 2 - 100, VCENTER | HCENTER, "<설정>");

    img = new iImage();
    tex = g->getTexture();
    img->addObject(tex);
    freeImage(tex);
    pop->addObject(img);


    //Btn
    imgSetBtn = new iImage * [4]; //Resume, Menu, Option, Exit

    const char* strBtn[4] = {
        "Resume",
        "Menu",
        "Option",
        "Exit"
    };

    size = iSizeMake(200, 40);
    for (int i = 0; i < 4; i++)
    {
        img = new iImage();
        for (int j = 0; j < 2; j++)
        {
            g->init(size);

            if (j == 0)
            {
                setRGBA(0, 0, 0, 1);
                g->fillRect(0, 0, size.width, size.height, 10);
                setRGBA(1, 1, 1, 1);
                g->drawRect(0, 0, size.width, size.height, 10);
                
                setStringSize(25);
                setStringRGBA(0, 0, 0, 1);
                setStringBorder(2);
                setStringBorderRGBA(1, 1, 1, 1);
                g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, strBtn[i]);
                tex = g->getTexture();
                img->addObject(tex);
                freeImage(tex);
            }
            else
            {
                setRGBA(1, 1, 0, 1);
                g->fillRect(0, 0, size.width, size.height, 10);
                setRGBA(1, 1, 1, 1);
                g->drawRect(0, 0, size.width, size.height, 10);

                setStringSize(25);
                setStringRGBA(0.4f, 0.5f, 1, 1);
                setStringBorder(2);
                setStringBorderRGBA(1, 1, 1, 1);
                g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, strBtn[i]);
                tex = g->getTexture();
                img->addObject(tex);
                freeImage(tex);
            }
           
        }
        img->position = iPointMake(240 - size.width / 2, 240 + i * 60);
        pop->addObject(img);
        imgSetBtn[i] = img;
    }
    pop->style = iPopupZoom;
    pop->openPoint = iPointMake(devSize.width / 2, devSize.height / 2);
    pop->closePoint = iPointMake(devSize.width / 2 - 240, devSize.height / 2 - 240);
    pop->methodBefore = drawMethodPopSet;

    popSetting = pop;
}

void freePopSetting()
{
    delete popSetting;
    delete imgSetBtn;
}

void drawMethodPopSet(iPopup* pop, float dt, float rate)
{
    for (int i = 0; i < 4; i++)
        imgSetBtn[i]->setTexObject(popSetting->selected == i);
}

void drawPopSetting(float dt)
{
    popSetting->paint(dt);
}
void showPopSetting(bool show)
{
    popSetting->show(show);
    if (show)
    {
    }
}
bool keyPopSetting(iKeyStat stat, iPoint point)
{
    iPopup* pop = popSetting;

    if (popSetting->bShow == false)
        return false;
    if (popSetting->stat != iPopupProc)
        return true;
    
    int i, j = -1;

    switch (stat)
    {
    case iKeyStatBegan:
        if (pop->selected == -1)
            break;

        else if (pop->selected == 0) //Resume
        {
            showPopSetting(false);
        }
        else if (pop->selected == 1) //Menu
        {
            setLoading(gs_menu, freeStage, loadMenu);
        }
        else if (pop->selected == 2) //Option
        {
            showPopSetting(false);
#if 1 
            //showPopOption(true);
            showPopTmp(true);
#endif
        }
        else if (pop->selected == 3) //Exit
        {
            showPopSetting(false);
            showPopExit(true); //to do...
        }

        break;

    case iKeyStatMoved:
        for (i = 0; i < 4; i++)
        {
            if (containPoint(point, imgSetBtn[i]->touchRect(pop->closePoint)))
            {
                j = i;
                break;
            }
        }
        if (pop->selected != j)
        {
            printf("audio Play\n");
            audioPlay(0);
            pop->selected = j;
        }
        break;

    case iKeyStatEnded:
        break;
    }

    return true;
}

//=========================================================
// popOption : option -> popSetting
//=========================================================

//#need Update! 임시 팝업 -> option 팝업
void createPopStageOption()
{
#if 1
    createPopTmp();
#endif
}
void freePopStageOption()
{
#if 1
    freePopTmp();
#endif
}
void drawPopStageOption(float dt)
{
#if 1
    drawPopTmp(dt);
#endif
}
void showPopStageOptionmp(bool show)
{
#if 1
    showPopTmp(show);
#endif
}
bool keyPopStageOption(iKeyStat stat, iPoint point)
{
#if 1
    if (keyPopTmp(stat, point))
        return true;
#endif

    return false;
}

//=========================================================
// popTmp : 임시 팝업 -> popSetting(option)
//=========================================================

//#need update!  Option 구현 시 이부분 수정
void createPopTmp()
{

}
void freePopTmp()
{

}
void drawPopTmp(float dt)
{

}
void showPopTmp(bool show)
{

}
bool keyPopTmp(iKeyStat stat, iPoint point)
{
    return false;
}

//=========================================================
// popExit : 나가기 팝업 -> popSetting
//=========================================================

void createPopStageExit()
{

}
void freePopStageExit()
{

}
void drawPopStageExit(float dt)
{

}
void showPopStageExit(bool show)
{

}
bool keyPopStageExit(iKeyStat stat, iPoint point)
{
    return false;
}


//=========================================================
// popStageHow : 게임방법
//=========================================================

void createPopStageHow()
{

}
void freePopStageHow()
{

}
void drawPopStageHow(float dt)
{

}
void showPopStageHow(bool show)
{

}
bool keyPopStageHow(iKeyStat stat, iPoint point)
{
    return false;
}

//=========================================================
// popInven : 인벤토리
//=========================================================

void createPopInven()
{

}
void freePopInven()
{

}
void drawPopInven(float dt)
{

}
void showPopInven(bool show)
{

}
bool keyPopInven(iKeyStat stat, iPoint point)
{
    return false;
}

//=========================================================
// popBottomUI : 대화 스크립트
//=========================================================

void createPopBottomUI()
{

}
void freePopBottomUI()
{

}
void drawPopBottomUI(float dt)
{

}
void showPopBottomUI(bool show)
{

}
bool keyPopBottomUI(iKeyStat stat, iPoint point)
{
    return false;
}


//=========================================================
//popOverStep
//=========================================================
iPopup* popOverStep;
iImage** imgOverStepBtn;

void drawPopOverStepBefore(iPopup* pop, float dt, float rate);
void createPopOverStep()
{
    iImage* img;
    Texture* tex;
    int i, j;

    iPopup* pop = new iPopup();

    iGraphics* g = iGraphics::share();
    //
    // bg
    //
    iSize size = iSizeMake(480, 320);
    g->init(size);

    setRGBA(0.5f, 0.5f, 0.5f, 0.8f);
    g->fillRect(0, 0, size.width, size.height, 10);
    setRGBA(1, 1, 1, 1);

    setStringSize(20);
    setStringRGBA(1, 1, 1, 1);
    setStringBorder(2);
    setStringBorderRGBA(0, 0, 0, 1);
    g->drawString(size.width / 2, size.height / 2, VCENTER|HCENTER, "스텝을 모두 소진하였습니다.");

    setStringSize(30);
    setStringRGBA(1, 0.1f, 0.1f, 1);
    g->drawString(size.width / 2, size.height / 2 + 20, VCENTER | HCENTER, "Game Over");

    tex = g->getTexture();
    img = new iImage();
    img->addObject(tex);
    freeImage(tex);
    pop->addObject(img);

    //
    // btn
    //
    const char* strBtn[1] = {
        "확인"
    };
    iPoint posBtn[1] = {
        {(480 - 200) / 2, 160 + 52}
    };
    imgOverStepBtn = new iImage*[1];

    size = iSizeMake(200, 50);
    for (i = 0; i < 1; i++)
    {
        img = new iImage();
        for (j = 0; j < 2; j++)
        {
            g->init(size);

            if (j == 0)
            {
                setRGBA(0, 0, 0, 1);
                g->fillRect(0, 0, size.width, size.height, 10);
                setRGBA(1, 1, 1, 1);

                setStringSize(40);
                setStringRGBA(1, 1, 1, 1);
                setStringBorder(0);
            }
            else
            {
                setRGBA(1, 1, 0, 1);
                g->fillRect(0, 0, size.width, size.height, 10);
                setRGBA(0.4f, 0.4f, 0.4f, 1);

                setStringSize(40);
                setStringRGBA(0, 0, 0, 1);
                setStringBorder(0);
            }
            g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, strBtn[i]);

            tex = g->getTexture();
            img->addObject(tex);
            freeImage(tex);
        }

        img->position = posBtn[i];
        pop->addObject(img);

        imgOverStepBtn[i] = img;
    }

    pop->style = iPopupZoom;
    pop->openPoint = iPointMake((devSize.width-480) / 2, (devSize.height-320) / 2);
    pop->closePoint = iPointMake((devSize.width - 480) / 2, (devSize.height - 320) / 2);
    pop->methodBefore = drawPopOverStepBefore;
    popOverStep = pop;
}

void freePopOverStep()
{
    delete popOverStep;
    delete imgOverStepBtn;
}

void showPopOverStep(bool show)
{
    popOverStep->show(show);
    if (show)
    {
    }
    else
    {

    }
}
void drawPopOverStepBefore(iPopup* pop, float dt, float rate)
{
    for (int i = 0; i < 1; i++)
        imgOverStepBtn[i]->setTexObject(popOverStep->selected == i);
}
void drawPopOverStep(float dt)
{
    popOverStep->paint(dt);
}
bool keyPopOverStep(iKeyStat stat, iPoint point)
{
    if (popOverStep->bShow == false || popOverStep->stat != iPopupProc)
        return false;

    int i, j = -1;

    switch (stat) {

    case iKeyStatBegan:
        if (popOverStep->selected == -1) break;
        if (popOverStep->selected == 0)
        {
            setLoading(gs_ending, freeStage, loadEnding);
        }
        else// if (popOverStep->selected == 1)
        {

        }
        break;

    case iKeyStatMoved:
        for (i = 0; i < 1; i++)
        {
            if (containPoint(point, imgOverStepBtn[i]->touchRect(popOverStep->closePoint)))
            {
                printf("popOverStep\n");
                j = i;
                break;
            }
        }
        if (popOverStep->selected != j)
        {
            //audioPlay
            popOverStep->selected = j;
        }
        break;

    case iKeyStatEnded:
        break;
    }

    return true;
}

//=========================================================
// popStageStr : 스테이지 글자표시
//=========================================================
iPopup* popStageStr;
iStrTex* stageStr;

Texture* methodStageStr(const char* str);

void createPopStageStr()
{
    iPopup* pop = new iPopup();
    iImage* img = new iImage();
    iStrTex* str = new iStrTex(methodStageStr);
    str->setString("STAGE%d",stageNum);
    img->addObject(str->tex);
    pop->addObject(img);

    pop->openPoint = iPointMake(tileOff.x + TILE_WSIZE, tileOff.y - TILE_HSIZE * 2);
    pop->closePoint = iPointMake(tileOff.x + TILE_WSIZE, tileOff.y - TILE_HSIZE * 2);
    popStageStr = pop;
}

Texture* methodStageStr(const char* str)
{
    iGraphics* g = iGraphics::share();
    iSize size = iSizeMake(120, 40);
    g->init(size);

    setRGBA(0, 0, 0, 1);
    g->fillRect(0, 0, size.width, size.height, 5);
    setRGBA(1, 1, 1, 1);
    g->drawRect(0, 0, size.width, size.height, 5);


    setStringSize(30);
    setStringRGBA(1, 1, 1, 1);
    g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, str);

    return g->getTexture();
}
void freePopStageStr()
{
    delete popStageStr;
}
void showPopStageStr(bool show)
{
    popStageStr->show(show);
    if (show)
    {

    }
    else
    {

    }
}
void drawPopStageStr(float dt)
{
    popStageStr->paint(dt);
}

//=========================================================
// popStepStr : 스테이지 발걸음 표시
//=========================================================
iPopup* popStepStr;
iStrTex* stepStr;
Texture* methodStepStr(const char* str);
void drawPopStepStrBefore(iPopup* pop, float dt, float rate);

void createPopStepStr()
{
    iPopup* pop = new iPopup();
    iImage* img = new iImage();

    //Str
    iStrTex* str = new iStrTex(methodStepStr);
    str->setString("0");
    char _str[20];
    sprintf(_str, "%d", step);
    str->setString(_str);
    img->addObject(str->tex);
    img->position = iPointZero;
    
    pop->addObject(img);

    stepStr = str;

    pop->methodBefore = drawPopStepStrBefore;
    pop->openPoint = iPointMake(tileOff.x + (TILE_W / 2 + 2) * TILE_WSIZE, tileOff.y - TILE_HSIZE*2);
    pop->closePoint = iPointMake(tileOff.x + (TILE_W / 2 + 2) * TILE_WSIZE, tileOff.y - TILE_HSIZE * 2);
    popStepStr = pop;
}

Texture* methodStepStr(const char* str)
{
    iGraphics* g = iGraphics::share();
    iSize size = iSizeMake(150, 40);
    g->init(size);
    
    setRGBA(0, 0, 0, 1);
    g->fillRect(0, 0, size.width, size.height, 5);
    setRGBA(1, 1, 1, 1);
    g->drawRect(0, 0, size.width, size.height, 5);
    
    setStringSize(30);
    setStringRGBA(1, 1, 1, 1);
    g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, str);

    return g->getTexture();
}

void freePopStepStr()
{
    delete popStepStr;
#if 0
    delete stepStr;
#endif 
}
void showPopStepStr(bool show)
{
    popStepStr->show(show);
    if (show)
    {

    }
    else
    {

    }
}

void drawPopStepStrBefore(iPopup* pop, float dt, float rate)
{
    stepStr->setString("STEP:%d", step);
}

void drawPopStepStr(float dt)
{
    popStepStr->paint(dt);
}

