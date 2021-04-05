#pragma once

#include "iStd.h"

class MapEditor
{
public:
	MapEditor();
	MapEditor(const char* szFormat, ...);
	virtual ~MapEditor();

	void clean();
	void openObject(const char* szFormat, ...);

	void draw(float dt, iPoint off, const char* ImgPath);

	//init ¸Å°³º¯¼ö ´ú ÀÛ¼ºµÊ.
	void init(int x, int y, int w, int h, const char* ImgPath);

	void load(const char* szFormat, ...);
	void save(const char* str);

	void insert(iPoint point);

public:
	int tileX, tileY, tileWidth, tileHeight;
	int* tileIndex, * tileWeight, * objIndex;

	Texture** texTiles;
	int numTiles;

	Texture** texObjects;
	iPoint* positionObjects;
	int numObjects;

	int mode;// 0:tile, 1:attr, 2:obj
	int selectedTile, selectedAttr, selectedObject;
};

const char* openImage();
void loadMap();
void freeMap();
void drawMap(float dt, const char* str);
void keyMap(iKeyStat stat, iPoint point);

