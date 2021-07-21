#include "MapEditor.h"

#include "Map.h"
#include "TileType.h"
#include "Loading.h"
#include "Menu.h"

#include "iStd.h"

//#need update! insert enemyInfo

Texture* methodStMapBtn(const char* str);
Texture* methodStWeiBtn(const char* str);
char* openFile(bool open, LPCWSTR filter);

//==========================================================
//iRect 영역들
//==========================================================

#if 0
iRect* EditRT; //편집 영역
iRect* WeiRT; //가중치 선택하는 영역의 숫자칸들
iRect TileRT;
iRect TileSelRT;
iRect selectedImgRT;
iRect selectedWeiRT;
iRect ExitRT;
#endif

iPoint TileRT_point;
iPoint TileImgRT_point;
iPoint TileWeiRT_point;

iImage** imgMapBtn;
iImage** imgWeiBtn;
iImage** imgWeiTypeBtn;

//==========================================================
//TileImg, string 관련
//==========================================================

static bool movingTileImg = false;
static bool movingWeiImg = false;

static const char* ImgPath[3];

static Texture*** texs;
static Texture** tmpTileTex;
static Texture* E_RT;
static Texture* selectedTex = NULL; //커서로 선택한 타일이미지
static Texture* weiFBO = NULL;
static Texture* texFBO = NULL;
static int selectedTexIdx;
static int* tmpTileWei;
static int selectedWei; //커서로 선택한 가중치
static int selectedWeiType; //0 : tile, 1 : enemy, 2 : item
static int selectedBtn;
static int texs_idx;
static Map* tEditor;
static iFont* tFont;

static char selWei[10] = { NULL, };
static char tmpWei[10] = { NULL, };

void RTset();
void ButtonSet();
void loadMapEditor()
{
    for (int i = 0; i < SOUND_NUM; i++)
        audioStop(i);
    audioPlay(1);

    tFont = loadFont("assets/font/BMJUA_ttf.ttf", 24, "0123456789");
    setFont(tFont);

    ImgPath[0] = "assets/Image/Tile1.bmp";
    ImgPath[1] = "assets/Image/Tile2.bmp";
    ImgPath[2] = "assets/Image/Tile3.bmp";

    texs = new Texture **[3];
    texs[0] = createImageDivide(8, 32, ImgPath[0]);
    texs[1] = createImageDivide(8, 32, ImgPath[1]);
    texs[2] = createImageDivide(8, 32, ImgPath[2]);

    weiFBO = createTexture(TILE_WSIZE * 8, TILE_HSIZE * 8);
    texFBO = createTexture(TILE_WSIZE * 8, TILE_HSIZE * 8);
    
    ButtonSet();
    RTset();

    tEditor = new Map();
    tEditor->init(TILE_W, TILE_H, TILE_WSIZE, TILE_HSIZE);

    createPopLoad();
    createPopSave();
}

void ButtonSet()
{
    const char* strMap[3] = {
    "Load", "Save", "Exit"
    };

    const char* strWeiType[3] = {
        "Tile", "Enemy", "Item"
    };

    const char* strWei[100] = {
         "0",  "1",  "2",  "3",  "4",  "5",  "6",  "7",  "8",  "9",
        "10", "11", "12", "13", "14", "15", "16", "17", "18", "19",
        "20", "21", "22", "23", "24", "25", "26", "27", "28", "29",
        "30", "31", "32", "33", "34", "35", "36", "37", "38", "39",
        "40", "41", "42", "43", "44", "45", "46", "47", "48", "49",
        "50", "51", "52", "53", "54", "55", "56", "57", "58", "59",
        "60", "61", "62", "63", "64", "65", "66", "67", "68", "69",
        "70", "71", "72", "73", "74", "75", "76", "77", "78", "79",
        "80", "81", "82", "83", "84", "85", "86", "87", "88", "89",
        "90", "91", "92", "93", "94", "95", "96", "97", "98", "99",
    };

    imgMapBtn = new iImage * [3];
    imgWeiTypeBtn = new iImage * [3];
    for (int i = 0; i < 3; i++)
    {
        iImage* img = new iImage();
        iImage* img2 = new iImage();

        for (int j = 0; j < 2; j++)
        {
            iStrTex* st = new iStrTex(methodStMapBtn);
            st->setString("%s\n%d", strMap[i], j);
            img->addObject(st->tex);

            st = new iStrTex(methodStMapBtn);
            st->setString("%s\n%d", strWeiType[i], j);
            img2->addObject(st->tex);
            delete st;
        }

        img->position = iPointMake(TILE_WSIZE * 34, TILE_HSIZE * (1 + 2 * i));
        img2->position = iPointMake(TILE_WSIZE * 9 + TILE_WSIZE * 9, TILE_HSIZE * 13 + (i % 3) * TILE_HSIZE * 2);

        imgMapBtn[i] = img;
        imgWeiTypeBtn[i] = img2;
    }

    imgWeiBtn = new iImage * [100];
    for (int i = 0; i < 100; i++)
    {
        iImage* img = new iImage();
        for (int j = 0; j < 2; j++)
        {
            iStrTex* st = new iStrTex(methodStWeiBtn);
            st->setString("%s\n%d", strWei[i], j);
            img->addObject(st->tex);
            delete st;
        }
        img->position = iPointMake(TILE_WSIZE * 2 * (i % 4), TILE_HSIZE * 2 * (i / 4));

        imgWeiBtn[i] = img;
    }

    //Load, Save, Exit, Img, Wei Btn 관련 변수
    selectedBtn = -1;
    selectedWei = -1;
    selectedWeiType = 0; //초기값 : tile
    selectedTexIdx = -1;
}


void createEditRT()
{
    iGraphics* g = iGraphics::share();
    iSize size = iSizeMake(2 * (TILE_W+1) * TILE_WSIZE, TILE_H * TILE_HSIZE);
    g->init(size);

    setRGBA(1, 0, 0, 1);
    for (int i = 0; i < TILE_W * TILE_H; i++)
        g->drawRect(i % TILE_W * TILE_WSIZE, i / TILE_W * TILE_HSIZE, TILE_WSIZE, TILE_HSIZE);
    
    g->drawRect((TILE_W + 1) * TILE_WSIZE, 0, TILE_W * TILE_WSIZE, TILE_H * TILE_HSIZE);
    setRGBA(1, 1, 1, 1);

    E_RT = g->getTexture();
}

void RTset()
{
    TileRT_point = iPointMake(TILE_WSIZE * 17, 0);
    TileImgRT_point = iPointZero;
    TileWeiRT_point = iPointMake(TILE_WSIZE * 9, TILE_HSIZE * 13);

    tmpTileTex = new Texture * [TILE_W * TILE_H];
    tmpTileWei = new int[TILE_W * TILE_H];
    for (int i = 0; i < TILE_W * TILE_H; i++)
    {
        tmpTileTex[i] = NULL;
        tmpTileWei[i] = -1;
    }

    createEditRT();
}

Texture* methodStMapBtn(const char* str)
{
    int lineNum;
    char** line = iString::getStringLine(str, lineNum);
    const char* s = line[0];
    int i = atoi(line[1]);

    iGraphics* g = iGraphics::share();
   
    iSize size = iSizeMake(TILE_WSIZE * 4, TILE_HSIZE);
    g->init(size);
    
    setRGBA(0, 0, 0, 1);
    g->fillRect(0, 0, size.width, size.height, 10);
    if(i != 0)
    {
        setStringRGBA(0, 1, 0, 1);
        setRGBA(1, 1, 0, 1);
        g->fillRect(0, 0, size.width, size.height, 10);
    }

    setStringSize(20);
    setStringRGBA(1, 0, 0, 1);
    setStringBorder(2);
    setStringBorderRGBA(0, 0, 1, 1);
    setStringLineHeight(30);
    g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, s);

    iString::freeStringLine(line, lineNum);

    return g->getTexture();
}

Texture* methodStWeiBtn(const char* str)
{
    int lineNum;
    char** line = iString::getStringLine(str, lineNum);
    const char* s = line[0];
    int i = atoi(line[1]);

    iGraphics* g = iGraphics::share();

    iSize size = iSizeMake(TILE_WSIZE * 2, TILE_HSIZE * 2);
    g->init(size);
    setRGBA(0, 0, 0, 1);
    g->fillRect(0, 0, size.width, size.height);

    setStringSize(20);
    setStringRGBA(1, 0, 1, 1);
    setStringBorder(2);
    
    if (i != 0) //버튼 활성화
    {
        setStringRGBA(0, 1, 0, 1);
        setRGBA(1, 1, 0, 1);
        g->fillRect(0, 0, size.width, size.height);
    }
    setStringBorderRGBA(0, 0, 1, 1);
    setStringLineHeight(30);
    g->drawString(TILE_WSIZE, TILE_HSIZE, VCENTER | HCENTER, s);

    iString::freeStringLine(line, lineNum);

    return g->getTexture();
}

void drawMapEditor(float dt)
{
    clearRect();
    //===========================================
    //draw Rect
    //===========================================
    drawImage(E_RT, 0, 0, TOP | LEFT);

    //===========================================
    //draw Button
    //===========================================
    int i;

    for (i = 0; i < 3; i++)
    {
        //버튼이 활성화 됬을 때 비활성화 됬을 때
        imgMapBtn[i]->setTexObject(selectedBtn == i);
        imgMapBtn[i]->paint(dt, iPointZero, iPointMake(1, 1));

        imgWeiTypeBtn[i]->setTexObject(selectedWeiType == i);
        imgWeiTypeBtn[i]->paint(dt, iPointZero, iPointMake(1, 1));
    }

    fbo->bind(weiFBO);

    for (int i = 0; i < 100; i++)
    {
        //버튼이 활성화 됬을 때 비활성화 됬을 때
        if (selectedWeiType > -1)
        {
            if (selectedWei < 100)
                imgWeiBtn[i]->setTexObject((selectedWei) == i);
            else if (selectedWei > 99 && selectedWei <= 10000)
                imgWeiBtn[i]->setTexObject((selectedWei - 100) / 100 == i);
            else //if(selectedWei > 10000)
                imgWeiBtn[i]->setTexObject((selectedWei - 10001) / 10000 == i);
        }
        imgWeiBtn[i]->paint(dt, iPointZero, iPointMake(1, 1));
    }

    fbo->unbind();

    setGLBlend(GLBlendMultiplied);

    drawImage(weiFBO, TILE_WSIZE * 9, TILE_HSIZE * 13, TOP | LEFT,
        0, 0, weiFBO->width, weiFBO->height, 1.0f, 1.0f, 2, 0,
        REVERSE_HEIGHT);

    setGLBlend(GLBlendAlpha);

    //===========================================
    //draw tmpTile
    //===========================================

    for (int i = 0; i < TILE_W * TILE_H; i++)
    {
        if (tmpTileTex[i])
        {
            drawImage(tmpTileTex[i], i % TILE_W * TILE_WSIZE, i / TILE_W * TILE_HSIZE, TOP | LEFT);
        }
        if (tmpTileWei[i] > -1)
        {
            //Tile
            if (tmpTileWei[i] < 100)
            {
                sprintf(tmpWei, "%d", tmpTileWei[i]);
            }
            //#issue! tmpTileWei
                        //Enemy
            else if (tmpTileWei[i] > 99 && tmpTileWei[i] <= 10000)
            {
                sprintf(tmpWei, "%d", (tmpTileWei[i] - 100) / 100);
                setRGBA(0.5f, 0.5f, 0.5f, 1);
            }

            //Item
            else
            {
                sprintf(tmpWei, "%d", (tmpTileWei[i] - 10001) / 10000);
                setRGBA(
                    random() % 256 / 255.f,
                    random() % 256 / 255.f,
                    random() % 256 / 255.f, 1);
            }
            drawString((i % TILE_W) * TILE_WSIZE,
                (i / TILE_W) * TILE_HSIZE, TOP | LEFT, tmpWei);
            setRGBA(1, 1, 1, 1);
        }
    }

    //===========================================
    //draw Tile
    //===========================================

    if (tEditor)
        tEditor->draw(TileRT_point, texs[0]);

    //===========================================
    //draw TileImg 1, 2, 3 & TileWeight 
    //===========================================

     //TileImgRT
    fbo->bind(texFBO);

    for (i = 0; i < 256; i++)
    {
        if (i == selectedTexIdx)
            setRGBA(1, 1, 1, 1);
        else
            setRGBA(0.25f, 0.25f, 0.25f, 0.5f);

        drawImage(texs[0][i], TileImgRT_point.x + (i % 8) * TILE_WSIZE,  TileImgRT_point.y + (i / 8) * TILE_HSIZE, TOP | LEFT);
    }

    fbo->unbind();
    
    setRGBA(1, 1, 1, 1);

    setGLBlend(GLBlendMultiplied);

    drawImage(texFBO, 0, TILE_HSIZE * 13, TOP | LEFT,
        0, 0, texFBO->width, texFBO->height, 1.0f, 1.0f, 2, 0,
        REVERSE_HEIGHT);

    setGLBlend(GLBlendAlpha);


    drawPopLoad(dt);
    drawPopSave(dt);
}

void freeMapEditor()
{
    int i, j;

    //Map3.cpp 전역변수 초기화
    selectedTex = NULL;
    selectedWei = -1;
    selectedTexIdx = -1;

    for (i = 0; i < 10; i++)
    {
        selWei[i] = NULL;
        tmpWei[i] = NULL;
    }

    freeFont(tFont);

    //delete EditRT;

    for (j = 0; j < 3; j++)
    {
        for (i = 0; i < 256; i++)
            freeImage(texs[j][i]);
        delete texs[j];
    }
    delete texs;

    freeImage(weiFBO);
    freeImage(texFBO);

    delete tmpTileTex;
    delete tmpTileWei;
    delete E_RT;

    //객체 초기화
    delete tEditor;

    delete imgMapBtn;
    delete imgWeiBtn;
    delete imgWeiTypeBtn;

    freePopLoad();
    freePopSave();
}

void containTileImg(iPoint point, iPoint off)
{
    movingTileImg = true;
   
    iRect texRT;
    
    if (movingTileImg)
        texs_idx = 0;

    for (int i = 0; i < 256; i++)
    {
        texRT = iRectMake(off.x + (i % 8) * TILE_WSIZE, off.y + TILE_HSIZE*13 + (i / 8) * TILE_HSIZE, TILE_WSIZE, TILE_HSIZE);
        //texRT = iRectMake(off.x , off.y + TILE_HSIZE * 13, texFBO->width, texFBO->height);
        if (containPoint(point, texRT))
        {
            if (selectedTexIdx == i)
            {
                selectedTexIdx = -1;
                selectedTex = NULL;
                tEditor->selectedTile = selectedTexIdx;
            }
            else
            {
                selectedTexIdx = i;
                selectedTex = texs[texs_idx][selectedTexIdx];
                tEditor->selectedTile = selectedTexIdx;
            }
        }
    }
}

void containWeiImg(iPoint point)
{
    movingWeiImg = true;

    for (int i = 0; i < 100; i++)
    {
        //weiFBO draw position : TILE_WSIZE * 9, TILE_HSIZE * 13
        if (containPoint(point, 
            imgWeiBtn[i]->touchRect(iPointZero) + iRectMake(TILE_WSIZE * 9, TILE_HSIZE * 13, 0, 0)))
        {
            //#issue! selectedWei
            if (selectedWei == i)
                selectedWei = -1;
            else if ((selectedWei - 100) / 100 == i)
                selectedWei = -1;
            else if ((selectedWei - 10001) / 10000 == i)
                selectedWei = -1;

            else
            {
                //#issue! selectedWei
                if (selectedWeiType == 0) //Tile
                    selectedWei = i;
                else if (selectedWeiType == 1) //Enemy
                    selectedWei = 100 + (i) * 100;
                else if (selectedWeiType == 2) //Item
                    selectedWei = 10001 + (i) * 10000;
            }
        }
    }


    sprintf(selWei, "%d", selectedWei);
    tEditor->selectedWeight = selectedWei;
}

void containWeiType(iPoint point)
{
    for (int i = 0; i < 3; i++)
    {
        if (containPoint(point, imgWeiTypeBtn[i]->touchRect (iPointZero)))
        {
            if (selectedWeiType == i)
            {
                selectedWeiType = -1;
            }
            else
            {
                selectedWeiType = i;
                selectedWei = -1;
            }
        }
    }
}

char* openFile(bool open, LPCWSTR filter)
{
    WCHAR currPath[1024];
    GetCurrentDirectory(1024, currPath);

    OPENFILENAME ofn;
    memset(&ofn, 0x00, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    extern HWND hWnd;
    ofn.hwndOwner = hWnd;
    TCHAR szFile[260] = { 0 };
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    char* path = NULL;

    bool success = false;
    if (open) success = GetOpenFileName(&ofn);
    else success = GetSaveFileName(&ofn);

    if (success)
    {
        wsprintf(szFile, TEXT("%s"), ofn.lpstrFile);
        path = utf16_to_utf8(ofn.lpstrFile);
#ifdef _DEBUG
        printf("path[%s]\n", path);
#endif

        SetCurrentDirectory(currPath);
    }
    return path;
}

static iPoint prevPosition = iPointZero;
void keyMapEditor(iKeyStat stat, iPoint point)
{
    
    if (keyPopLoad(stat, point) || keyPopSave(stat, point))
        return;

    iRect E_RT = iRectMake(0, 0, TILE_WSIZE * 16, TILE_HSIZE * 12); //큰 영역
    iRect EditRT; //세부 영역
    iRect TileImgRT = iRectMake(0, TILE_HSIZE * 13, TILE_WSIZE * 8, TILE_HSIZE * 8);
    iRect TileWeiRT = iRectMake(TileWeiRT_point.x, TileWeiRT_point.y, TILE_WSIZE * 8, TILE_HSIZE * 8);

    switch (stat)
    {
    case iKeyStatBegan:
    {
        //편집영역에 있을때 (격자무늬)
        if (containPoint(point, E_RT))
        {
            if (selWei[0] || selectedTex)
            {
                tEditor->insert(point, 0);
                for (int i = 0; i < TILE_W * TILE_H; i++)
                {
                    int x = i % TILE_W, y = i / TILE_W;
                    EditRT = iRectMake(x * TILE_WSIZE,
                        y * TILE_WSIZE,
                        TILE_WSIZE,
                        TILE_HSIZE);
                    if (containPoint(point, EditRT))
                    {
                        if (selectedTex)
                            tmpTileTex[i] = selectedTex;
                        if (selectedWei > -1)
                        {
                            tmpTileWei[i] = selectedWei;
                        }
                    }
                }
            }
        }

        prevPosition = point;
        //타일 이미지 내
        if (containPoint(point, TileImgRT))
            containTileImg(point, TileImgRT_point);

        //Wei 버튼 (숫자버튼)
        if (containPoint(point, TileWeiRT))
            containWeiImg(point);

        //WeiType 버튼(toggle btn)
        containWeiType(point);

        //load 버튼
        if (selectedBtn == 0)
        {
            char* ch = NULL;
            const wchar_t* filter = L"Map Files(*.tile)\0*.tile;.TILE\0All Files(*.*)\0*.*\0";

            ch = openFile(true, filter);

            if (ch == NULL)
                return;

            for (int i = 0; i < TILE_W * TILE_H; i++)
            {
                tmpTileTex[i] = NULL;
                tmpTileWei[i] = -1;
            }
#if 0
            tEditor->load(ch);
#else
            int length;
            char* ch2 = loadFile(ch, length);
            tEditor->loadA(ch2);
#endif

            //map info
            for (int i = 0; i < TILE_W * TILE_H; i++)
            {
                if (i % TILE_W == 0) printf("\n");
                printf("%d ", tEditor->tileWeight[i]);
            }

            //편집영역에 로드 파일을 옮기는 용도
            for (int i = 0; i < TILE_W * TILE_H; i++)
            {
                if (tEditor->tileIndex[i] > -1)
                {
                    tmpTileTex[i] = texs[0][tEditor->tileIndex[i]];
                    tmpTileWei[i] = tEditor->tileWeight[i];
                }
            }

            showPopLoad(true);
        }
        //save 버튼
        else if (selectedBtn == 1)
        {
            char* tmpPath = NULL;
            const wchar_t* filter = L"Map Files(*.tile)\0*.tile;.TILE\0All Files(*.*)\0*.*\0";
            tmpPath = openFile(false, filter);

            if (tmpPath == NULL)
                return;

            char savePath[1024];
            int strLen = strlen(tmpPath);
            if (strcmp(&tmpPath[strLen - 4], ".tile") == 0 ||
                strcmp(&tmpPath[strLen - 4], ".TILE") == 0)
                strcpy(savePath, tmpPath);
            else
                sprintf(savePath, "%s.tile", tmpPath);

            tEditor->save(savePath);

            showPopSave(true);
        }
        //Exit 버튼
        else if (selectedBtn == 2)
            setLoading(gs_menu, freeMapEditor, loadMenu);
    }
        break;

    case iKeyStatMoved:
    {
        int i, j = -1;

        for (i = 0; i < 3; i++)
        {
            if (containPoint(point, imgMapBtn[i]->touchRect(iPointZero)))
            {
                j = i;
                break;
            }
        }

        if (j != -1)
        {
            selectedBtn = j;
            return;
        }
        else
        {
            selectedBtn = j;
        }

        if (movingTileImg)
        {
            iPoint mp = point - prevPosition;
            prevPosition = point;

            TileImgRT_point.y += mp.y;
            if (TileImgRT_point.y < -TILE_HSIZE * 24)
                TileImgRT_point.y = -TILE_HSIZE * 24;
            else if (TileImgRT_point.y > 0)
                TileImgRT_point.y = 0;
            return;
        }

        if (movingWeiImg)
        {
            // to do...
            iPoint mp = point - prevPosition;
            prevPosition = point;

            for (int i = 0; i < 100; i++)
            {
                imgWeiBtn[i]->position.y += mp.y;
                if (imgWeiBtn[i]->position.y < TILE_HSIZE * (-42) + TILE_HSIZE * 2 * (i / 4))
                    imgWeiBtn[i]->position.y = TILE_HSIZE * (-42) + TILE_HSIZE * 2 * (i / 4);
                else if (imgWeiBtn[i]->position.y > TILE_HSIZE * (0) + TILE_HSIZE * 2 * (i / 4))
                    imgWeiBtn[i]->position.y = TILE_HSIZE * (0) + TILE_HSIZE * 2 * (i / 4);
            }
            return;
        }
    }
        break;

    case iKeyStatEnded:
        movingTileImg = false;
        movingWeiImg = false;
        break;
    }
}

//================================================
//popLoad
//================================================
void drawPopLoadBefore(iPopup* pop, float dt, float rate);

iPopup* popLoad;
iImage** imgLoadBtn;
void createPopLoad()
{
    iImage* img;
    Texture* tex;
    int i, j;

    iPopup* pop = new iPopup();

    iGraphics* g = iGraphics::share();
    
    //bg

    iSize size = iSizeMake(320, 240);

    g->init(size);
    setRGBA(1, 0, 1, 0.5f);
    g->fillRect(0, 0, size.width, size.height, 10);
    setRGBA(1, 1, 1, 1);

    setStringSize(30);
    setStringRGBA(1, 1, 1, 1);
    setStringBorder(1);
    setStringBorderRGBA(0, 0, 0, 1);
    g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "로드 완료");

    tex = g->getTexture();
    img = new iImage();
    img->addObject(tex);
    freeImage(tex);
    pop->addObject(img);

    //btn

    const char* loadBtn[1] = {
        "확인"
    };

    iPoint posBtn[1] = {
        {(size.width-160) / 2, (size.height+80) / 2},
    };
    imgLoadBtn = new iImage * [1];

    size = iSizeMake(150, 40);
    for (i = 0; i < 1; i++)
    {
        img = new iImage();
        for (j = 0; j < 2; j++)
        {
            g->init(size);
            
            if(j==0) setRGBA(0, 0, 0, 1);
            else     setRGBA(1, 1, 0, 1);
            g->fillRect(0, 0, size.width, size.height, 10);
            setRGBA(1, 1, 1, 1);

            setStringSize(30);
            setStringRGBA(1, 1, 1, 1);
            setStringBorder(1);
            setStringBorderRGBA(0, 0, 0, 1);
            g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, loadBtn[i]);

            tex = g->getTexture();
            img->addObject(tex);
            freeImage(tex);
        }
        img->position = posBtn[i];
        pop->addObject(img);
        
        imgLoadBtn[i] = img;
    }

    pop->style = iPopupZoom;
    pop->openPoint = iPointMake(devSize.width / 2 , devSize.height / 2 );
    pop->closePoint = iPointMake((devSize.width-320) / 2 , (devSize.height-240) / 2);
    pop->methodBefore = drawPopLoadBefore;
    popLoad = pop;
}
void freePopLoad()
{
    delete popLoad;
    delete imgLoadBtn;
}
void showPopLoad(bool show)
{
    popLoad->show(show);
}

void drawPopLoadBefore(iPopup* pop, float dt, float rate)
{
    for (int i = 0; i < 1; i++)
        imgLoadBtn[i]->setTexObject(popLoad->selected == i);
}

void drawPopLoad(float dt)
{
    popLoad->paint(dt);
}
bool keyPopLoad(iKeyStat stat, iPoint point)
{
    if (popLoad->bShow == false || popLoad->stat != iPopupProc)
        return false;

    int i, j = -1;

    switch (stat)
    {
    case iKeyStatBegan:
        if (popLoad->selected == -1) break;
        if (popLoad->selected == 0)
        {
            showPopLoad(false);
        }
        else //if(popLoad->selected == 1)
        {

        }
        break;
    case iKeyStatMoved:
        for (i = 0; i < 1; i++)
        {
            if (containPoint(point, imgLoadBtn[i]->touchRect(popLoad->closePoint)))
            {
                j = i;
                break;
            }

        }
        if (popLoad->selected != j)
        {
            popLoad->selected = j;
        }
        break;
    case iKeyStatEnded:
        break;

    }
    return true;
}

//================================================
//popSave
//================================================
void drawPopSaveBefore(iPopup* pop, float dt, float rate);

iPopup* popSave;
iImage** imgSaveBtn;

void createPopSave()
{
    iImage* img;
    Texture* tex;
    int i, j;
    iPopup* pop = new iPopup();

    iGraphics* g = iGraphics::share();

    //bg

    iSize size = iSizeMake(320, 240);

    g->init(size);
    setRGBA(0, 1, 1, 0.5f);
    g->fillRect(0, 0, size.width, size.height, 10);
    setRGBA(1, 1, 1, 1);

    setStringSize(30);
    setStringRGBA(1, 1, 1, 1);
    setStringBorder(1);
    setStringBorderRGBA(0, 0, 0, 1);
    g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "저장 완료");

    tex = g->getTexture();
    img = new iImage();
    img->addObject(tex);
    freeImage(tex);
    pop->addObject(img);

    //btn

    const char* saveBtn[1] = {
        "확인",
    };

    iPoint posBtn[1] = {
        {(size.width - 160) / 2, (size.height + 80) / 2},
    };
    imgSaveBtn = new iImage * [1];

    size = iSizeMake(150, 40);

    for (i = 0; i < 1; i++)
    {
        img = new iImage();
        for (j = 0; j < 2; j++)
        {
            g->init(size);

            if (j == 0) setRGBA(0, 0, 0, 1);
            else        setRGBA(1, 1, 0, 1);
            g->fillRect(0, 0, size.width, size.height, 10);
            setRGBA(1, 1, 1, 1);

            setStringSize(30);
            setStringRGBA(1, 1, 1, 1);
            setStringBorder(1);
            setStringBorderRGBA(0, 0, 0, 1);
            g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, saveBtn[i]);

            tex = g->getTexture();
            img->addObject(tex);
            freeImage(tex);
        }
        img->position = posBtn[i];
        pop->addObject(img);

        imgSaveBtn[i] = img;
    }

    pop->style = iPopupZoom;
    pop->openPoint = iPointMake(devSize.width / 2, devSize.height / 2);
    pop->closePoint = iPointMake((devSize.width-320) / 2 , (devSize.height-240) / 2);
    pop->methodBefore = drawPopSaveBefore;
    popSave = pop;
}
void freePopSave()
{
    delete popSave;
    delete imgSaveBtn;
}
void showPopSave(bool show)
{
    popSave->show(show);
}
void drawPopSaveBefore(iPopup* pop, float dt, float rate)
{
    for (int i = 0; i < 1; i++)
        imgSaveBtn[i]->setTexObject(popSave->selected == i);
}
void drawPopSave(float dt)
{
    popSave->paint(dt);
}
   
bool keyPopSave(iKeyStat stat, iPoint point)
{
    if (popSave->bShow == false || popSave->stat != iPopupProc)
        return false;

    int i, j = -1;

    switch (stat)
    {
    case iKeyStatBegan:
        if (popSave->selected == -1) break;
        if (popSave->selected == 0)
        {
            showPopSave(false);
        }
        else
        {

        }
        break;
    case iKeyStatMoved:
        for (i = 0; i < 1; i++)
        {
            if (containPoint(point, imgSaveBtn[i]->touchRect(popSave->closePoint)))
            {
                j = i;
                break;
            }
        }
        if (popSave->selected != j)
        {
            popSave->selected = j;
        }
        break;
    case iKeyStatEnded:
        break;
    }
    return true;
}