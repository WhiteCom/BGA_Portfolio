#include "Map3.h"

#include "iWindow.h"
#include "TileType.h"

/*
   public:
   int tileX, tileY, tileWidth, tileHeight;
   int* tileIndex, *tileWeight;

   Texture** texTiles;
   int numTiles;


   int mode;// 0:tile, 1:attr, 2:obj
   int selectedTile, selectedWeight;

*/


MapEditor::MapEditor()
{
    tileX = 0;
    tileY = 0;
    tileWidth = 0;
    tileHeight = 0;
    tileIndex = NULL;
    tileWeight = 0;

    numTiles = 0;

    selectedTile = 0;
    selectedWeight = 0;
}

MapEditor::MapEditor(const char* szFormat, ...)
{
    tileX = 0;
    tileY = 0;
    tileWidth = 0;
    tileHeight = 0;
    tileIndex = NULL;
    tileWeight = NULL;

    numTiles = 0;

    selectedTile = 0;
    selectedWeight = 0;

    char szText[1024];
    va_start_end(szText, szFormat);

    load(szText);
}

MapEditor::~MapEditor()
{
    clean();
}

void MapEditor::clean()
{
    if (tileIndex == NULL)
        return;

    for (int i = 0; i < 3; i++)
        delete tileIndex[i];
    delete tileIndex;

    delete tileWeight;

    int i;
}

void MapEditor::draw(float dt, iPoint off)
{
    int i, xy = tileX * tileY;
    int ti, tw;

    for (i = 0; i < xy; i++)
    {
        int x = off.x + i % tileX * tileWidth;
        int y = off.y + i / tileX * tileHeight;
        
        //if(texTiles[i]) //NULL 아닌경우만
        //    drawImage(texTiles[i], x, y, TOP | LEFT);
    }
}

void MapEditor::init(int x, int y, int w, int h)
{
    tileX = x;
    tileY = y;
    tileWidth = w;
    tileHeight = h;
    
    int tileXY = tileX * tileY;
    int i;
    tileIndex = new int* [3];
    for (i = 0; i < 3; i++)
        tileIndex[i] = new int[tileXY];

    for (i = 0; i < tileXY; i++)
        tileIndex[0][i] = 0;

    tileWeight = new int[tileXY];
    for (i = 0; i < tileXY; i++)
        tileWeight[i] = 0;
    
    numTiles = tileXY;
}

void MapEditor::load(const char* szFormat, ...)
{
    clean();

    char szText[1024];
    va_start_end(szText, szFormat);

    FILE* pf = fopen(szText, "rb");
    if (pf == NULL)
    {
        printf("No File!");
        return;
    }

    // tile info
    fread(&tileX, sizeof(int), 1, pf);
    fread(&tileY, sizeof(int), 1, pf);
    fread(&tileWidth, sizeof(int), 1, pf);
    fread(&tileHeight, sizeof(int), 1, pf);

    int xy = tileX * tileY;
    int i;
    tileIndex = new int* [3];
    for (i = 0; i < 3; i++)
        tileIndex[i] = new int[xy];
    tileWeight = new int[xy];

    for(i = 0;i<3;i++)
        fread(tileIndex[i], sizeof(int), xy, pf);
    fread(tileWeight, sizeof(int), xy, pf);

    fread(&numTiles, sizeof(int), xy, pf);

#if 0 //texTiles 멤버변수 안쓰면 사용안할것들
    texTiles = new Texture * [numTiles];
    int i, j;
    for (i = 0; i < numTiles; i++)
    {
        Bitmap* bmp = new Bitmap(tileWidth, tileHeight, PixelFormat32bppARGB);
        Rect rt(0, 0, tileWidth, tileHeight);
        BitmapData bd;
        bmp->LockBits(&rt, ImageLockModeWrite, PixelFormat32bppARGB, &bd);
        int stride = bd.Stride / 4;
        int* rgba = (int*)bd.Scan0;
        for (j = 0; j < tileHeight; j++)
            fread(&rgba[stride], sizeof(int), tileWidth, pf);
        bmp->UnlockBits(&bd);

        Texture* tex = new Texture;
        tex->texID = bmp;
        tex->width = tileWidth;
        tex->height = tileHeight;
        tex->potWidth = tileWidth;
        tex->potHeight = tileHeight;
        tex->retainCount = 1;

        texTiles[i] = tex;
    }
#endif

    fclose(pf);
}

void MapEditor::save(const char* str)
{
    FILE* pf = fopen(str, "wb");

    // tile info
    fwrite(&tileX, sizeof(int), 1, pf);
    fwrite(&tileY, sizeof(int), 1, pf);
    fwrite(&tileWidth, sizeof(int), 1, pf);
    fwrite(&tileHeight, sizeof(int), 1, pf);

    int xy = tileX * tileY;
    for(int i=0;i<3;i++)
        fwrite(tileIndex[i], sizeof(int), xy, pf);

    fwrite(tileWeight, sizeof(int), xy, pf);

    fwrite(&numTiles, sizeof(int), xy, pf);
#if 0 //texTiles 멤버변수 안쓰면 사용안할것들
    int i, j;
    for (i = 0; i < numTiles; i++)
    {
        Bitmap* bmp = (Bitmap*)texTiles[i]->texID;
        Rect rt(0, 0, tileWidth, tileHeight);
        BitmapData bd;
        bmp->LockBits(&rt, ImageLockModeRead, PixelFormat32bppARGB, &bd);
        int stride = bd.Stride / 4;
        int* rgba = (int*)bd.Scan0;
        for (j = 0; j < tileHeight; j++)
            fwrite(&rgba[stride], sizeof(int), tileWidth, pf);
        bmp->UnlockBits(&bd);
    }
#endif
    fclose(pf);
}

void MapEditor::insert(iPoint point, int type)
{
    int x = point.x; x /= tileWidth;
    int y = point.y; y /= tileHeight;
    int xy = tileX * y + x;

    tileIndex[type][xy] = selectedTile;
    tileWeight[xy] = selectedWeight;
    
#if 0 //texTiles 멤버변수 안쓰면 사용안할것들
    Texture** texs = createImageDivide(8, 32, ImgPath);
    int ti = tileIndex[xy];
    texTiles[xy] = texs[ti]; //실제 타일의 넘버링에 tileindex에 해당하는 인덱스 번호의 텍스처들어가기
   
    for (int i = 0; i < 256; i++)
        freeImage(texs[i]);
    delete texs;
#endif
}

#if 0
char* openImg()
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
    ofn.lpstrFilter = _T("All\0*.*\0Text\0*.TXT\0");
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    char* path = NULL;

    if (GetOpenFileName(&ofn) == TRUE)
    {
        path = utf16_to_utf8(ofn.lpstrFile);
        printf("path[%s]\n", path);

        SetCurrentDirectory(currPath);
    }

    return path;
}
#endif

//to do...
//기존 map2.cpp 에서 tEditor 내에 타일을 저장하는 방식을 바꿔야함. 
//draw에서 tex** createImage 하지 않기 -> load 등 한번만 불리는 곳에서 처리해야함
//1) 클래스 내 멤버변수로 tex** 생성 (생성자 혹은 load 등에서 처리)
//2) load 등에서 본 파일(map3.cpp) 에서만 쓰이는 파일 tex** 생성 (이미지 생성 후 객체 내부 tex** 변수에도 들어갈수 있게처리)

//==========================================================
//iRect영역들
//==========================================================

iRect RT;
iRect* EditRT;
iRect TileRT;
iRect TileImgRT;
iRect TileImgRT2;
iRect TileImgRT3;
iRect TileWeiRT;
iRect TileSelRT;
iRect LoadBtn;
iRect SaveBtn;
iRect selectedImgRT;
iRect selectedWeiRT;
iRect ExitRT;

iPoint prevPosition;
iPoint EditRT_point;
iPoint TileRT_point;
iPoint TileImgRT_point;
iPoint TileImgRT2_point;
iPoint TileImgRT3_point;
iPoint TileWeiRT_point;
iPoint TileSelRT_point;
iPoint LoadBtn_point;
iPoint SaveBtn_point;
iPoint selectedImgRT_point;
iPoint selectedWeiRT_point;
iPoint ExitRT_point;

static bool set_check = false;
static bool movingTileImg = false;
static bool movingTileImg2 = false;
static bool movingTileImg3 = false;

static const char* ImgPath[3];

Texture*** texs;

Texture* selectedTex = NULL; //커서로 선택한 타일이미지

MapEditor* tEditor;


void RTset()
{
    //좌표 세팅을 위한 일회성 함수로 한번 쓰고 안쓸거임.
    prevPosition =          iPointMake(0, 0);
    EditRT_point =          iPointMake(0, 0);
    TileRT_point =          iPointMake(tileWSize * 17,  0);
    //TileRT_point =          iPointMake(0, 0);

    TileImgRT_point =       iPointMake(0,               tileHSize * 13);
    TileImgRT2_point =      iPointMake(tileWSize * 9,   tileHSize * 13);
    TileImgRT3_point =      iPointMake(tileWSize * 18,  tileHSize * 13);
    TileWeiRT_point =       iPointMake(tileWSize * 27,  tileHSize * 13);

    LoadBtn_point =         iPointMake(tileWSize * 34,  tileHSize);
    SaveBtn_point =         iPointMake(tileWSize * 34,  tileHSize * 3);
    TileSelRT_point =       iPointMake(tileWSize * 34,  tileHSize * 5);
    selectedImgRT_point =   iPointMake(tileWSize * 36 - tileWSize/2,  tileHSize * 6);
    selectedWeiRT_point =   iPointMake(tileWSize * 36 - tileWSize/2, tileHSize * 8);

    EditRT = new iRect[tileW * tileH];

    for (int i = 0; i < tileW * tileH; i++)
    {
        int x = i % tileW, y = i/tileW;
        EditRT[i] = iRectMake(EditRT_point.x + x * tileWSize, EditRT_point.y + y * tileHSize, tileWSize, tileHSize);
    }

    RT =            iRectMake(0, 0, tileWSize * 16, tileHSize * 12);
    TileRT =        iRectMake(TileRT_point.x, TileRT_point.y, tileWSize * 16, tileHSize * 12);
    TileImgRT =     iRectMake(TileImgRT_point.x, TileImgRT_point.y, tileWSize * 8, tileHSize * 8);
    TileImgRT2 =    iRectMake(TileImgRT2_point.x, TileImgRT2_point.y, tileWSize * 8, tileHSize * 8);
    TileImgRT3 =    iRectMake(TileImgRT3_point.x, TileImgRT3_point.y, tileWSize * 8, tileHSize * 8);
    TileWeiRT =     iRectMake(TileWeiRT_point.x, TileWeiRT_point.y, tileWSize * 8, tileHSize * 8);
    TileSelRT =     iRectMake(TileSelRT_point.x, TileSelRT_point.y, tileWSize * 4, tileHSize * 5);
    LoadBtn =       iRectMake(LoadBtn_point.x, LoadBtn_point.y, tileWSize * 4, tileHSize);
    SaveBtn =       iRectMake(SaveBtn_point.x, SaveBtn_point.y, tileWSize * 4, tileHSize);
    selectedImgRT = iRectMake(selectedImgRT_point.x, selectedImgRT_point.y, tileWSize, tileHSize);
    selectedWeiRT = iRectMake(selectedWeiRT_point.x, selectedWeiRT_point.y, tileWSize, tileHSize);

    tEditor = new MapEditor();
    tEditor->init(tileW, tileH, tileWSize, tileHSize);
  
    set_check = true;
}

void loadMap()
{
    
    ImgPath[0] = "assets/Image/Tile1.bmp";
    ImgPath[1] = "assets/Image/Tile2.bmp";
    ImgPath[2] = "assets/Image/Tile3.bmp";

    texs = new Texture **[3];
    texs[0] = createImageDivide(8, 32, ImgPath[0]);
    texs[1] = createImageDivide(8, 32, ImgPath[1]);
    texs[2] = createImageDivide(8, 32, ImgPath[2]);

    if (!set_check)
        RTset();
    
    //to do...
    //가중치 처리해줘야함. 아직 안해준것들이 좀 있음. 
    tEditor;
}



void drawMap(float dt)
{
    clearRect();
    //===========================================
    //draw iRect
    //===========================================
    int i;
    setRGBA(1, 0, 0, 1);
    for(i=0;i<tileW * tileH;i++)
        drawRect(EditRT[i]);
    drawRect(TileRT);
    drawRect(TileImgRT);
    drawRect(TileImgRT2);
    drawRect(TileImgRT3);
    drawRect(TileWeiRT);
    drawRect(TileSelRT);
    drawRect(LoadBtn);
    drawRect(SaveBtn);
    drawRect(selectedImgRT);
    drawRect(selectedWeiRT);
    setRGBA(1, 1, 1, 1);

    //===========================================
    //draw string
    //===========================================
    //to do...
    const char* weight[11] = {
        "0","1","2","3","4","5","6","7","8","9", "INF"
    };
    setStringName("굴림");
    setStringSize(30);
    setStringRGBA(1, 0, 0, 1);
    setStringBorder(0.5f);
    setStringBorderRGBA(0, 0, 0, 1);
    //test
    for (int i = 0; i < 10; i++)
        drawString(TileWeiRT_point.x, TileWeiRT_point.y + (i*20), TOP | LEFT, weight[i]); 

    //#need update 위치조정
    const char* loadStr = "Load";
    const char* saveStr = "Save";
    drawString(LoadBtn_point.x, LoadBtn_point.y, TOP | LEFT, loadStr);
    drawString(SaveBtn_point.x, SaveBtn_point.y, TOP | LEFT, saveStr);

    //===========================================
    //draw Tile
    //===========================================
    //tEditor->draw(dt, TileRT_point);
    int x = tEditor->tileX;
    int y = tEditor->tileY;
    for (int j = 0; j < 3; j++)
    {
        for (int i = 0; i < x * y; i++)
        {
            if (tEditor->tileIndex[j][i] > -1)
                drawImage(texs[j][tEditor->tileIndex[j][i]], TileRT_point.x + i%x*tileWSize, TileRT_point.y + i/x*tileHSize, TOP | LEFT);
        }
    }

    if (selectedTex)
        drawImage(selectedTex, selectedImgRT_point.x, selectedImgRT_point.y, TOP | LEFT);

    //===========================================
    //draw TileImg 1, 2, 3 & TileWeight 
    //===========================================
    if(selectedTex)
        drawImage(selectedTex, selectedImgRT_point.x, selectedImgRT_point.y, TOP|LEFT);

    //TileImgRT
    setClip(0, tileHSize * 13, tileWSize * 8, tileHSize * 8);
    for (i = 0; i < 256; i++)
        drawImage(texs[0][i], TileImgRT_point.x + (i % 8) * tileWSize, TileImgRT_point.y + (i / 8) * tileHSize, TOP | LEFT);
    setClip(0, 0, 0, 0);

    //TileImgRT2
    setClip(tileWSize * 9, tileHSize * 13, tileWSize * 8, tileHSize * 8);
    for (i = 0; i < 256; i++)
        drawImage(texs[1][i], TileImgRT2_point.x + (i % 8) * tileWSize, TileImgRT2_point.y + (i / 8) * tileHSize, TOP | LEFT);
    setClip(0, 0, 0, 0);

    //TileImgRT3
    setClip(tileWSize * 18, tileHSize * 13, tileWSize * 8, tileHSize * 8);
    for (i = 0; i < 256; i++)
        drawImage(texs[2][i], TileImgRT3_point.x + (i % 8) * tileWSize, TileImgRT3_point.y + (i / 8) * tileHSize, TOP | LEFT);
    setClip(0, 0, 0, 0);

}

void freeMap()
{

    int i, j;

    delete EditRT;

    for (j = 0; j < 3; j++)
    {
        for (i = 0; i < 256; i++)
            freeImage(texs[j][i]);
        delete texs[j];
    }
    delete texs;

    delete tEditor;
}

void containTileImg(iPoint point, iPoint off)
{
    iRect texRT;
    int texs_idx;
    if (movingTileImg)
        texs_idx = 0;
    else if (movingTileImg2)
        texs_idx = 1;
    else //if(movingTileImg3)
        texs_idx = 2;

    for (int i = 0; i < 256; i++)
    {
        texRT = iRectMake(off.x + (i % 8) * tileWSize, off.y + (i / 8) * tileHSize, tileWSize, tileHSize);
        if (containPoint(point, texRT))
        {
            selectedTex = texs[texs_idx][i];
            //#issue! 선택한 타일의 인덱스를 객체의 인덱스에 집어넣을때,
            //이전에 쓴 1차원 배열의 방식이 아닌, 2차원 배열의 방식을 생각해봐야함
            //tEditor->tileIndex[texs_idx][]
            //tEditor->selectedTile = i;

        }
    }
}

void keyMap(iKeyStat stat, iPoint point)
{
    if (stat == iKeyStatBegan)
    {

        //편집영역에 있을때 (격자무늬)
        if (containPoint(point, RT))
        {
        }

        prevPosition = point;
        //타일 이미지 내
        if (containPoint(point, TileImgRT))
        {
            movingTileImg = true;
            containTileImg(point, TileImgRT_point);
        }
        else if (containPoint(point, TileImgRT2))
        {
            movingTileImg2 = true;
            containTileImg(point, TileImgRT2_point);
        }
        else if (containPoint(point, TileImgRT3))
        {
            movingTileImg3 = true;
            containTileImg(point, TileImgRT3_point);
        }

        //load 버튼
        if (containPoint(point, LoadBtn))
        {
#ifdef WIN32
            MessageBox(NULL, TEXT("로드완료"), TEXT("Load"), MB_OK);
#endif //WIN32
        }

        //save 버튼
        if (containPoint(point, SaveBtn))
        {
#ifdef WIN32
            MessageBox(NULL, TEXT("저장완료"), TEXT("Save"), MB_OK);
#endif //WIN32
        }

    }
    else if (stat == iKeyStatMoved)
    {
        if (movingTileImg)
        {
            iPoint mp = point - prevPosition;
            prevPosition = point;

            TileImgRT_point.y += mp.y;
            if (TileImgRT_point.y < tileHSize * (tileH + 1) - tileHSize * 24)
                TileImgRT_point.y = tileHSize * (tileH + 1) - tileHSize * 24;
            else if (TileImgRT_point.y > tileHSize * (tileH + 1))
                TileImgRT_point.y = tileHSize * (tileH + 1);
        }
        else if (movingTileImg2)
        {
            iPoint mp = point - prevPosition;
            prevPosition = point;

            TileImgRT2_point.y += mp.y;
            if (TileImgRT2_point.y < tileHSize * (tileH + 1) - tileHSize * 24)
                TileImgRT2_point.y = tileHSize * (tileH + 1) - tileHSize * 24;
            else if (TileImgRT2_point.y > tileHSize * (tileH + 1))
                TileImgRT2_point.y = tileHSize * (tileH + 1);
        }
        else if (movingTileImg3)
        {
            iPoint mp = point - prevPosition;
            prevPosition = point;

            TileImgRT3_point.y += mp.y;
            if (TileImgRT3_point.y < tileHSize * (tileH + 1) - tileHSize * 24)
                TileImgRT3_point.y = tileHSize * (tileH + 1) - tileHSize * 24;
            else if (TileImgRT3_point.y > tileHSize * (tileH + 1))
                TileImgRT3_point.y = tileHSize * (tileH + 1);
        }
    }
    else //if(stat == iKeyStatEnded)
    {
        movingTileImg = false;
        movingTileImg2 = false;
        movingTileImg3 = false;
    }
}


