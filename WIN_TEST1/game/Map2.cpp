#include "Map2.h"
#include "TileType.h"
//#include "MapEditor.h"
#include "iWindow.h"


//타입 : 필드 - 벽 & 구멍 - 워프 이미지 - 아이템 - 적
#define NoneSection				{NoneIdx, INF, 1}
#define WarpSection				{WarpIdx, 0, 2}

//0 : 초원, 1 : 황야, 2 : 얼음, 3 : 불, 4 : 보스
#define ForestField(n)			{ForestIdx[n], 0, 0}
#define DesertField(n)			{DesertIdx[n], 0, 0}
#define IceField(n)				{IceIdx[n], 0, 0}
#define FireField(n)			{FireIdx[n], 0, 0}
#define BossField(n)			{BossIdx[n], 0, 0}

#define ForestWall(n)			{ForestWallIdx[n], 1, 3}
#define DesertWall(n)			{DesertWallIdx[n], 1, 0}
#define IceWall(n)				{IceWallIdx[n], 1, 0}
#define FireWall(n)				{FireWallIdx[n], 1, 0}
#define BossWall(n)				{BossWallIdx[n], 1, 0}

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

#include "MapEditor.h"

#include "iStd.h"
#include "iWindow.h"

MapEditor::MapEditor()
{
	tileX = 0;
	tileY = 0;
	tileWidth = 0;
	tileHeight = 0;
	tileIndex = NULL;
	tileWeight = NULL;
	objIndex = NULL;

	texTiles = NULL;
	numTiles = 0;

	texObjects = NULL;
	positionObjects = NULL;
	numObjects = 0;

	mode = 0;
	selectedTile = -1;
	selectedAttr = -1;
	selectedObject = -1;
}

MapEditor::MapEditor(const char* szFormat, ...)
{
	tileX = 0;
	tileY = 0;
	tileWidth = 0;
	tileHeight = 0;
	tileIndex = NULL;
	tileWeight = NULL;
	objIndex = NULL;

	texTiles = NULL;
	numTiles = 0;

	texObjects = NULL;
	positionObjects = NULL;
	numObjects = 0;

	mode = 0;
	selectedTile = -1;
	selectedAttr = -1;
	selectedObject = -1;

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
	delete objIndex;

	int i;
	
	//for (i = 0; i < numTiles; i++)
	//	freeImage(texTiles[i]);
	delete texTiles;

	//for (i = 0; i < numObjects; i++)
	//	freeImage(texObjects[i]);
	delete texObjects;
	delete positionObjects;
}

void MapEditor::openObject(const char* szFormat, ...)
{
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

	if (GetOpenFileName(&ofn) == TRUE)
	{
		char* path = utf16_to_utf8(ofn.lpstrFile);
		printf("path[%s]\n", path);

		if (mode == 0)
		{
			//mode 가 0이면 타일에 대한 이미지 정보

		}

		if (mode == 1)
		{
			//모드가 1이면 가중치에 대한 이미지 정보
		}

		if (mode == 2)
		{
			//모드가 2이면 오브젝트에 대한 이미지 정보
			//texObects = createImageDivide();
		}

		delete path;
	}
}

void MapEditor::draw(float dt, iPoint off)
{
	int i, xy = tileX * tileY;
	int ti, tw, oi;

	for (i = 0; i < xy; i++)
	{
		int x = off.x + i % tileX * tileWidth;
		int y = off.y + i / tileX * tileHeight;
		ti = tileIndex[i];
		Texture* tex = texTiles[ti];
		drawImage(tex, x, y, TOP | LEFT);

		//tw = tileWeight[i];

		//oi = objIndex[i];
		//tex = texObjects[oi];
		//x = positionObjects[oi].x + x + tileWidth / 2;
		//y = positionObjects[oi].y + y + tileHeight / 2;
		//drawImage(tex, x, y, TOP | LEFT);
	}
}

void MapEditor::init(int x, int y, int w, int h)
{
	tileX = x;
	tileY = y;
	tileWidth = w;
	tileHeight = h;

	int tileXY = tileX * tileY, i;
	tileIndex = new int[tileXY];

	for (i = 0; i < tileXY; i++)
		tileIndex[i] = 128;

	tileWeight = new int[tileXY];
	objIndex = new int[tileXY];

	Texture** tmp = createImageDivide(8, 32, "assets/Image/tile1.bmp");

	texTiles = new Texture* [tileXY];
	
	//#need issue!! 이거 인덱스 번호만 잘 수정하면 됨.
	for (i = 0; i<tileXY; i++)
	{
		if (i == 180) 
			int test = 0;
		int ti = tileIndex[i];
		texTiles[i] = tmp[ti];
	}

	numTiles = tileXY;

	//#need update
	//임시로 할당한것 실제 오브젝트 집어넣을땐 거기에 맞게 적용해야 함.
	texObjects = new Texture* [tileXY];
	numObjects = tileXY;
	
	//texTiles
	//texObjects 할당해줘야함.
	mode = 0;
}

void MapEditor::load(const char* szFormat, ...)
{
	clean();

	char szText[1024];
	va_start_end(szText, szFormat);

	FILE* pf = fopen(szText, "rb");

	// tile info
	fread(&tileX, sizeof(int), 1, pf);
	fread(&tileY, sizeof(int), 1, pf);
	fread(&tileWidth, sizeof(int), 1, pf);
	fread(&tileHeight, sizeof(int), 1, pf);

	int xy = tileX * tileY;
	tileIndex = new int[xy];
	tileWeight = new int[xy];
	objIndex = new int[xy];
	fread(tileIndex, sizeof(int), xy, pf);
	fread(tileWeight, sizeof(int), xy, pf);
	fread(objIndex, sizeof(int), xy, pf);

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

	fread(&numObjects, sizeof(int), xy, pf);
	texObjects = new Texture * [numObjects];
	positionObjects = new iPoint[numObjects];
	for (i = 0; i < numObjects; i++)
	{
		int w, h;
		fread(&w, sizeof(int), 1, pf);
		fread(&h, sizeof(int), 1, pf);

		Bitmap* bmp = new Bitmap(w, h, PixelFormat32bppARGB);
		Rect rt(0, 0, w, h);
		BitmapData bd;
		bmp->LockBits(&rt, ImageLockModeWrite, PixelFormat32bppARGB, &bd);
		int stride = bd.Stride / 4;
		int* rgba = (int*)bd.Scan0;
		for (j = 0; j < h; j++)
			fread(&rgba[stride], sizeof(int), w, pf);
		bmp->UnlockBits(&bd);

		Texture* tex = new Texture;
		tex->texID = bmp;
		tex->width = w;
		tex->height = h;
		tex->potWidth = w;
		tex->potHeight = h;
		tex->retainCount = 1;

		texObjects[i] = tex;

		fread(&positionObjects[i].x, sizeof(float), 1, pf);
		fread(&positionObjects[i].y, sizeof(float), 1, pf);
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
	fwrite(objIndex, sizeof(int), xy, pf);

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

	fwrite(&numObjects, sizeof(int), xy, pf);
	for (i = 0; i < numObjects; i++)
	{
		int w = texObjects[i]->width, h = texObjects[i]->height;
		fwrite(&w, sizeof(int), 1, pf);
		fwrite(&h, sizeof(int), 1, pf);

		Bitmap* bmp = (Bitmap*)texObjects[i]->texID;
		Rect rt(0, 0, w, h);
		BitmapData bd;
		bmp->LockBits(&rt, ImageLockModeRead, PixelFormat32bppARGB, &bd);
		int stride = bd.Stride / 4;
		int* rgba = (int*)bd.Scan0;
		for (j = 0; j < h; j++)
			fwrite(&rgba[stride], sizeof(int), w, pf);
		bmp->UnlockBits(&bd);

		fwrite(&positionObjects[i].x, sizeof(float), 1, pf);
		fwrite(&positionObjects[i].y, sizeof(float), 1, pf);
	}

	fclose(pf);
}

void MapEditor::insert(iPoint point)
{
	int x = point.x; x /= tileWidth;
	int y = point.y; y /= tileHeight;
	int xy = tileX * y + x;

	if (mode == 0)
	{
		tileIndex[xy] = selectedTile;
	}
	else if (mode == 1)
	{
		tileWeight[xy] = selectedAttr;
	}
	else// if (mode == 2)
	{
		objIndex[xy] = selectedObject;
	}
}

//Texture** tex 이친구를 init할때 멤버변수 texTiles에 집어넣고, 
//이후에 insert할때, 아래쪽에 보면 이미 다 만들어진 그것을 갖고, 멤버변수 tileIndex에 집어넣은 후, 
//draw에서 texTile과 tileIndex를 이용해서, 그릴 수 있으면 된다. (x, y, width, height 다 있으니 가능함)

Texture* texBg;
Texture** tex; 
Texture** tmpTiles;
Texture* texSelectTile;
iPoint positionTile, prevPosition;
iPoint* positionEditRT;
bool movingTile;

MapEditor* tEditor;
iRect* EditRT;

void loadMap()
{
    texBg = createImage("assets/map.jpg");

    positionTile = iPointMake(0, tileHSize * tileH + tileHSize);
    prevPosition = iPointZero;
	positionEditRT = new iPoint[tileW * tileH];
    movingTile = false;

    tex = createImageDivide(8, 32, "assets/Image/tile1.bmp");

	tmpTiles = new Texture * [tileW * tileH];
	for (int i = 0; i < tileW * tileH; i++)
		tmpTiles[i] = tex[128];

	texSelectTile = tex[128];

	tEditor = new MapEditor();
	tEditor->init(tileW, tileH, tileWSize, tileHSize);

	EditRT = new iRect[tileW * tileH];
}

void freeMap()
{
    freeImage(texBg);

    int i;
	delete positionEditRT;

    for (i = 0; i < 256; i++)
        delete tex[i];
    delete tex;
	delete tmpTiles;

	delete tEditor;
	delete EditRT;
}

iRect TileRT;
iRect selectTileRT;
iRect WeightRT;
iRect ObjRT;

void drawToolRect()
{
	//EditRT = new iRect[tileW * tileH];
    TileRT        = iRectMake(0, tileHSize * tileH + tileHSize, tileWSize * 8, tileHSize * 8);
    selectTileRT  = iRectMake(tileWSize * tileW , 0, tileWSize, tileHSize);
    WeightRT      = iRectMake(tileWSize * 8 + tileWSize, tileHSize * tileH + tileHSize, tileWSize * 8, tileHSize * 8);
    ObjRT         = iRectMake(tileWSize * 8 * 2 + tileWSize * 2, tileHSize * tileH + tileHSize, tileWSize * 8, tileHSize * 8);

    setRGBA(1, 0, 0, 1);
	int i;
	for (i = 0; i < tileW * tileH; i++)
	{
		EditRT[i] = iRectMake(i % tileW * tileWSize, i/tileW * tileWSize, tileWSize, tileHSize);
		positionEditRT[i] = iPointMake(i % tileW * tileWSize, i / tileW * tileWSize);
		drawRect(EditRT[i]);
		drawImage(tmpTiles[i], i % tileW * tileWSize, i / tileW * tileWSize, TOP|LEFT);
	}

    drawRect(TileRT);
    drawRect(selectTileRT);
    drawRect(WeightRT);
    drawRect(ObjRT);
    setRGBA(1, 1, 1, 1);

	drawImage(texSelectTile, selectTileRT.origin.x, selectTileRT.origin.y, TOP | LEFT);

    setClip(0, tileHSize * tileH + tileHSize, tileWSize * 8, tileHSize * 8);

    for (i = 0; i < 256; i++)
    {
        drawImage(tex[i], positionTile.x + (i % 8) * tileWSize, positionTile.y + (i / 8) * tileHSize, TOP | LEFT);
    }

    setClip(0, 0, 0, 0);

}

void drawMap(float dt, const char* str)
{
    clearRect();
    
    drawToolRect();

	iPoint off = iPointMake(tileW * tileWSize + tileWSize * 2, 0);
	tEditor->draw(dt, off);
}

void keyMap(iKeyStat stat, iPoint point)
{
    switch (stat)
    {
    case iKeyStatBegan:
        //t.openObject("test");
        break;

    case iKeyStatMoved:
        break;

    case iKeyStatEnded:
        break;
    }

	iRect rt = iRectMake(0, 0, tileW * tileWSize-1, tileH * tileHSize-1);

    if (stat == iKeyStatBegan)
    {
        if (containPoint(point, TileRT))
        {
            movingTile = true;
            prevPosition = point;

            for (int i = 0; i < 256; i++)
            {
                //iRect seletTileRT;
                iRect texRT = iRectMake(positionTile.x + (i % 8) * tileWSize, positionTile.y + (i / 8) * tileHSize, tileWSize, tileHSize);
				if (containPoint(point, texRT))
				{
					texSelectTile = tex[i];
					tEditor->selectedTile = i;
				}
            }
        }
		else if (containPoint(point, rt))
		{
			tEditor->insert(point - rt.origin);
			for (int i = 0; i < tileW * tileH; i++)
			{
				if (containPoint(point, EditRT[i]))
				{
					tmpTiles[i] = texSelectTile;
				}
			}
		}
    }
    else if (stat == iKeyStatMoved)
    {
        if (movingTile)
        {
            iPoint mp = point - prevPosition;
            prevPosition = point;

            positionTile.y += mp.y;
            if (positionTile.y < tileHSize * tileH + tileHSize - tileHSize * 24)
                positionTile.y = tileHSize * tileH + tileHSize - tileHSize * 24;
            else if (positionTile.y > tileHSize * tileH + tileHSize)
                positionTile.y = tileHSize * tileH + tileHSize;
        }

    }
    else if (stat == iKeyStatEnded)
    {
        movingTile = false;
    }
}
