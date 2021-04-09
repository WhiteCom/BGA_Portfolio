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

    texTiles = NULL;
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

    texTiles = NULL;
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

    delete tileIndex;
    delete tileWeight;

    int i;

    if (texTiles[numTiles - 1] != NULL)
    {
        for (i = 0; i < numTiles; i++)
            freeImage(texTiles[i]);
    }
    delete texTiles;
}


void MapEditor::draw(float dt, iPoint off, const char* ImgPath)
{
    int i, xy = tileX * tileY;
    int ti, tw;

    for (i = 0; i < xy; i++)
    {
        int x = off.x + i % tileX * tileWidth;
        int y = off.y + i / tileX * tileHeight;
        //ti = tileIndex[i];
        
        if(texTiles[i]) //NULL 아닌경우만
            drawImage(texTiles[i], x, y, TOP | LEFT);
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
    tileIndex = new int[tileXY] { 0, };
    tileWeight = new int[tileXY] { 0, };
    
    texTiles = new Texture * [tileXY];
    //여기서 문제 
    //이전처럼 Texture** texs를 여기서 쓰면 이미지를 오픈할때마다, 계속 갱신됨. 
    //따라서 여기서 쓰는게 아닌, loadMap이나, insert할때 Texture** texs를 넣어줄 생각해야함.
   
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
    tileIndex = new int[xy];
    tileWeight = new int[xy];
    fread(tileIndex, sizeof(int), xy, pf);
    fread(tileWeight, sizeof(int), xy, pf);

    fread(&numTiles, sizeof(int), xy, pf);
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
    fwrite(tileIndex, sizeof(int), xy, pf);
    fwrite(tileWeight, sizeof(int), xy, pf);

    fwrite(&numTiles, sizeof(int), xy, pf);
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

    fclose(pf);
}

void MapEditor::insert(iPoint point, const char* ImgPath)
{
    int x = point.x; x /= tileWidth;
    int y = point.y; y /= tileHeight;
    int xy = tileX * y + x;

    Texture** texs = createImageDivide(8, 32, ImgPath);
    tileIndex[xy] = selectedTile;
    tileWeight[xy] = selectedWeight;
    
    int ti = tileIndex[xy];
    texTiles[xy] = texs[ti]; //실제 타일의 넘버링에 tileindex에 해당하는 인덱스 번호의 텍스처들어가기
    
   
    for (int i = 0; i < 256; i++)
        freeImage(texs[i]);
    delete texs;
}

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

//to do...
//기존 map2.cpp 에서 tEditor 내에 타일을 저장하는 방식을 바꿔야함. 
//draw에서 tex** createImage 하지 않기 -> load 등 한번만 불리는 곳에서 처리해야함
//1) 클래스 내 멤버변수로 tex** 생성 (생성자 혹은 load 등에서 처리)
//2) load 등에서 본 파일(map3.cpp) 에서만 쓰이는 파일 tex** 생성 (이미지 생성 후 객체 내부 tex** 변수에도 들어갈수 있게처리)

//==============================================================
//Texture** tex = createImageDivide(...);
//이거를 할때 Texture* tmp = new Texture; 이런식으로 하나 tmp 변수 만들고 tmp = tex[tileIndex] 해서 처리하는 방식은 어떨까?
//이후에 delete 시 
//for(i=0;i<256;i++)
//  freeImage(tex[i]);
//delete tex; 
//==============================================================

//==========================================================
//iRect영역들
//==========================================================

iRect EditRT;
iRect TileRT;
iRect TileImgRT;
iRect TileWeiRT;
iRect TileSelRT;
iRect LoadBtn;
iRect SaveBtn;
iRect ImgOpenBtn;
iRect selectedRT;

iPoint EditRT_point;
iPoint TileRT_point;
iPoint TileImgRT_point;
iPoint TileWeiRT_point;
iPoint TileSelRT_point;
iPoint LoadBtn_point;
iPoint SaveBtn_point;
iPoint ImgOpenBtn_point;
iPoint selectedRT_point;

static bool set_check = false;

const char* ImgPath = NULL;

Texture** texs;
Texture* selecTex; //커서로 선택한 타일이미지

MapEditor* tEditor;


void RTset()
{
    //좌표 세팅을 위한 일회성 함수로 한번 쓰고 안쓸거임.
    EditRT_point =          iPointMake(0, 0);
    TileRT_point =          iPointMake(tileWSize * 17,  0);

    TileImgRT_point =       iPointMake(0,               tileHSize * 13);
    TileWeiRT_point =       iPointMake(tileWSize * 9,   tileHSize * 13);
    TileSelRT_point =       iPointMake(tileWSize * 18,  tileHSize * 13);

    LoadBtn_point =         iPointMake(tileWSize * 34,  tileHSize);
    SaveBtn_point =         iPointMake(tileWSize * 34,  tileHSize * 3);
    ImgOpenBtn_point =      iPointMake(0,               tileHSize * 21);
    
    selectedRT_point =      iPointMake(tileWSize * 22 - tileWSize/2, tileHSize * 17 - tileHSize/2);

    EditRT =        iRectMake(EditRT_point.x, EditRT_point.y, tileWSize * 16, tileHSize * 12);
    TileRT =        iRectMake(TileRT_point.x, TileRT_point.y, tileWSize * 16, tileHSize * 12);
    TileImgRT =     iRectMake(TileImgRT_point.x, TileImgRT_point.y, tileWSize * 8, tileHSize * 8);
    TileWeiRT =     iRectMake(TileWeiRT_point.x, TileWeiRT_point.y, tileWSize * 8, tileHSize * 8);
    TileSelRT =     iRectMake(TileSelRT_point.x, TileSelRT_point.y, tileWSize * 8, tileHSize * 8);
    LoadBtn =       iRectMake(LoadBtn_point.x, LoadBtn_point.y, tileWSize * 4, tileHSize);
    SaveBtn =       iRectMake(SaveBtn_point.x, SaveBtn_point.y, tileWSize * 4, tileHSize);
    ImgOpenBtn =    iRectMake(ImgOpenBtn_point.x, ImgOpenBtn_point.y, tileWSize * 4, tileHSize);
    selectedRT =    iRectMake(selectedRT_point.x, selectedRT_point.y, tileWSize, tileHSize);

    tEditor = new MapEditor();
    tEditor->init(tileW, tileH, tileWSize, tileHSize);

    set_check = true;
}


void loadMap()
{
    if (!set_check)
        RTset();

    if (ImgPath == NULL)
        ImgPath = "assets/Image/Tile1.bmp";
    else
        ImgPath = openImg();

    texs = createImageDivide(8, 32, ImgPath);
    
    //인덱스 처리해줘야함. 아직 안해준것들이 좀 있음. 

}

void drawMap(float dt)
{
    clearRect();
    //===========================================
    //draw iRect
    //===========================================

    setRGBA(1, 0, 0, 1);
    drawRect(EditRT);
    drawRect(TileRT);
    drawRect(TileImgRT);
    drawRect(TileWeiRT);
    drawRect(TileSelRT);
    drawRect(LoadBtn);
    drawRect(SaveBtn);
    drawRect(ImgOpenBtn);
    drawRect(selectedRT);
    setRGBA(1, 1, 1, 1);

}
void freeMap()
{
    int i;
    for (i = 0; i < 256; i++)
        freeImage(texs[i]);
    delete texs;
    
    //#issue 현재 할당만 했지, 값이 없어서 삭제하면 터짐
    delete tEditor;
}

void keyMap(iKeyStat stat, iPoint point)
{
    if (stat == iKeyStatBegan)
    {

    }
    else if (stat == iKeyStatMoved)
    {

    }
    else //if(stat == iKeyStatEnded)
    {

    }
}