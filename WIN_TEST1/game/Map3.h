#pragma once

#include "iStd.h"

class MapEditor
{
public:
	MapEditor();
	MapEditor(const char* szFormat, ...);
	virtual ~MapEditor();

	void clean();

	void draw(float dt, iPoint off);

	void init(int x, int y, int w, int h, Texture** texs);

	void load(const char* szFormat, ...);
	void save(const char* str);

	void insert(iPoint point, const char* ImgPath);

public:
	int tileX, tileY, tileWidth, tileHeight;
	int* tileIndex, * tileWeight;

	Texture** texTiles;
	int numTiles;

	//int mode;// 0:tile, 1:attr, 2:obj
	int selectedTile, selectedWeight;
};

char* openImg(const char* szFormaat, ...);

void loadMap();
void drawMap(float dt);
void freeMap();
void keyMap(iKeyStat stat, iPoint point);