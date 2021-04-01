#include "MapEditor.h"

#include "iStd.h"
#include "iWindow.h"
#if 0
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
	for (i = 0; i < numTiles; i++)
		freeImage(texTiles[i]);
	delete texTiles;

	for (i = 0; i < numObjects; i++)
		freeImage(texObjects[i]);
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
			texTiles = createImageDivide(tileX, tileY, path);

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

		oi = objIndex[i];
		tex = texObjects[oi];
		x = positionObjects[oi].x + x + tileWidth / 2;
		y = positionObjects[oi].y + y + tileHeight / 2;
		drawImage(tex, x, y, TOP | LEFT);
	}
}

void MapEditor::init(int x, int y, int w, int h)
{
	tileX = 16;
	tileY = 12;
	tileWidth = 32;
	tileHeight = 32;

	int tileXY = tileX * tileY;
	tileIndex = new int[tileXY];
	tileWeight = new int[tileXY];
	objIndex = new int[tileXY];

	numTiles = 8 * 32;

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

MapEditor t;

void loadMapEditor()
{
	t.init(16, 12, 32, 32);
	iPoint point = iPointMake(32, 32);
	t.insert(point);


}
void drawMapEditor(float dt)
{

}
void freeMapEditor()
{

}
#endif
