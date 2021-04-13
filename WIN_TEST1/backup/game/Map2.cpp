#if 0
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
	selectedTile = 0; 
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

void MapEditor::draw(float dt, iPoint off, const char* ImgPath)
{
	int i, xy = tileX * tileY;
	int ti, tw, oi;
	
	//#issue! 
	//여기서 계속 그려주면 안됨. 그리고 texs를 클래스내의 멤버변수로 고정시키면 어떨지 고민해봐야함.
	//타일이미지가 바뀐다고, (openImage) 클래스 내의 인자에해당하는 이미지가 전부 바뀌면 안됨
	Texture** texs = createImageDivide(8, 32, ImgPath);

	
	for (i = 0; i < xy; i++)
	{
		int x = off.x + i % tileX * tileWidth;
		int y = off.y + i / tileX * tileHeight;
		ti = tileIndex[i];
		texTiles[ti] = texs[ti]; //바뀐 이미지로 체인지하기. 
		//위의 로직을 draw가 아닌 다른곳에서 처리되도록 해줘야함.

		Texture* tex = texTiles[ti];
		drawImage(tex, x, y, TOP | LEFT);

		//가중치
		//tw = tileWeight[i];

		//oi = objIndex[i];
		//tex = texObjects[oi];
		//x = positionObjects[oi].x + x + tileWidth / 2;
		//y = positionObjects[oi].y + y + tileHeight / 2;
		//drawImage(tex, x, y, TOP | LEFT);
	}

	
	delete texs;
}

void MapEditor::init(int x, int y, int w, int h, const char* ImgPath)
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

	Texture** tmp = createImageDivide(8, 32, ImgPath);

	texTiles = new Texture * [tileXY];

	for (i = 0; i < tileXY; i++)
	{
		int ti = tileIndex[i];
		texTiles[i] = tmp[ti];
	}

	numTiles = tileXY;

	//#need update
	//임시로 할당한것 실제 오브젝트 집어넣을땐 거기에 맞게 적용해야 함.
	texObjects = new Texture * [tileXY];
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

	//==================================================================
	//#need update 현재 오브젝트처리를 하나도 안해줌.
	//==================================================================
#if 1
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

	//==================================================================
	//#need update 현재 오브젝트처리를 하나도 안해줌.
	//==================================================================
#if 0
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
#endif
	fclose(pf);
}

void MapEditor::insert(iPoint point) //매개변수 const char* ImgPath 넣어야 하지 않을까?
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

const char* openImage()
{
	WCHAR pathCurr[1024];
	GetCurrentDirectory(1024, pathCurr);

	OPENFILENAME ofn;
	memset(&ofn, 0x00, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	extern HWND hWnd;
	ofn.hwndOwner = hWnd;
	TCHAR szFile[260] = { 0 };
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
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
		//printf("path[%s]\n", path);

		SetCurrentDirectory(pathCurr);
	}
	else
	{
		printf("FAIL!\n");

	}

	return path;
}

Texture* texBg;
Texture** tex;
Texture** tmpTiles;

//#need update
//가중치 정보를 글자로 그려질 수 있도록 하는게 중요 
//0, 1, 2, 3, 4, 5, 6, 7, 8, 9, INF
//weight 변수 필요함.

Texture* texSelectTile;
iPoint positionTile, prevPosition;
iPoint* positionEditRT;
bool movingTile;

MapEditor* tEditor;
iRect* EditRT;

const char* ImgPath = NULL;

//EditRT 영역들
iRect rt;
iRect TileRT;
iRect selectTileRT;
iRect WeightRT;
iRect ObjRT;
iRect ImgOpenBtn;
iRect tSave;
iRect tLoad;

//load 할때 현재 타일에 대한 처리만 했음
//next step : 가중치에 대한 처리 -> drawString으로 가중치 숫자를 지정해보자.
//next step2 : 오브젝트에 대한 처리 -> 타일과 비슷하게
void loadMap()
{
	//=========================================================================
	//Rect
	//=========================================================================
	//EditRT = new iRect[tileW * tileH];

	float stw = ((tileWSize * 9) + (tileWSize * 16 + tileWSize)) / 2 - tileWSize / 2;
	float sth = ((tileHSize * (tileH + 1)) + (tileHSize * (tileH + 9))) / 2 - tileHSize / 2;

	rt = iRectMake(0, 0, tileW * tileWSize - 1, tileH * tileHSize - 1);
	TileRT = iRectMake(0, tileHSize * tileH + tileHSize, tileWSize * 8, tileHSize * 8);
	selectTileRT = iRectMake(stw, sth, tileWSize, tileHSize);
	WeightRT = iRectMake(tileWSize * 8 + tileWSize, tileHSize * tileH + tileHSize, tileWSize * 8, tileHSize * 8);
	ObjRT = iRectMake(tileWSize * 8 * 2 + tileWSize * 2, tileHSize * tileH + tileHSize, tileWSize * 8, tileHSize * 8);
	ImgOpenBtn = iRectMake(0, tileHSize * 21, tileWSize * 4, tileHSize);
	tLoad = iRectMake(tileWSize * 27, tileHSize * tileH + tileHSize, tileWSize * 4, tileHSize);
	tSave = iRectMake(tileWSize * 27, tileHSize * (tileH+2) + tileHSize, tileWSize * 4, tileHSize);

	//=========================================================================
	//Texture, Position
	//=========================================================================

	if (ImgPath == NULL)
		ImgPath = "assets/Image/Tile1.bmp";
	else
		ImgPath = openImage();
	
	texBg = createImage("assets/map.jpg");

	positionTile = iPointMake(0, tileHSize * tileH + tileHSize);
	prevPosition = iPointZero;
	positionEditRT = new iPoint[tileW * tileH];
	movingTile = false;

	tex = createImageDivide(8, 32, ImgPath);

	tmpTiles = new Texture * [tileW * tileH];
	for (int i = 0; i < tileW * tileH; i++)
		tmpTiles[i] = tex[128];


	tEditor = new MapEditor();
	tEditor->init(tileW, tileH, tileWSize, tileHSize, ImgPath);

	int texIdx = tEditor->selectedTile;
	texSelectTile = tex[texIdx];

	EditRT = new iRect[tileW * tileH];
}

void freeMap()
{
	freeImage(texBg);

	int i;
	delete positionEditRT;

	for (i = 0; i < 256; i++)
		freeImage(tex[i]);
	delete tex;
	delete tmpTiles;

	delete tEditor;
	delete EditRT;
}

void drawToolRect()
{
	setRGBA(1, 0, 0, 1);
	int i;
	for (i = 0; i < tileW * tileH; i++)
	{
		EditRT[i] = iRectMake(i % tileW * tileWSize, i / tileW * tileWSize, tileWSize, tileHSize);
		positionEditRT[i] = iPointMake(i % tileW * tileWSize, i / tileW * tileWSize);
		drawRect(EditRT[i]);
		drawImage(tmpTiles[i], i % tileW * tileWSize, i / tileW * tileWSize, TOP | LEFT);
		
	}

	drawRect(TileRT);
	drawRect(WeightRT);
	drawRect(ObjRT);
	fillRect(selectTileRT);

	setRGBA(1, 1, 0, 1);
	fillRect(ImgOpenBtn);

	setRGBA(0, 0, 1, 1);
	fillRect(tLoad);
	
	setRGBA(0, 1, 0, 1);
	fillRect(tSave);
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
	tEditor->draw(dt, off, ImgPath);
}

void keyMap(iKeyStat stat, iPoint point)
{
	if (stat == iKeyStatBegan)
	{
		//TileRT 영역에 있으면 
		//mode == 0
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
		//else if : containPoint WeightRT (가중치 박스에 커서가 간 경우)
		//mode == 1

		//else if : containPoint objectRT (오브젝트 박스에 커서가 간 경우)
		//mode == 2

		//EditRT에 커서가 간 경우 : 실제로 저장되는 타일 영역
		if (containPoint(point, rt))
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

		//타일 이미지 열기
		if (containPoint(point, ImgOpenBtn))
		{
			//ImgPath = openImage();
			////for (int i = 0; i < 256; i++)
			////	delete tex[i];
			////delete tex;
			//tex = createImageDivide(8, 32, ImgPath);
			
			loadMap();
			//ImgPath = openImage();
		}
		
		//맵 로드
		if (containPoint(point, tLoad))
		{
			//const char* tmp_path = openFile();
			tEditor->load("map.tile");
			//tEditor("map.tile");
		}

		//맵 저장하기
		if (containPoint(point, tSave))
		{
			tEditor->save("map.tile");
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
#endif