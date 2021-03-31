#pragma once

#include "iType.h"

class MapEditor
{
public:
	MapEditor();
	MapEditor(const char* szFormat, ...);
	virtual ~MapEditor();

	void clean();
	void openObject(const char* szFormat, ...);

	void draw(float dt, iPoint off);

	void load(const char* szFormat, ...);
	void save(const char* str);

	void insert(iPoint point);

public:
	int tileX, tileY, tileWidth, tileHeight;
	int *tileIndex, *tileWeight, *objIndex;

	Texture** texTiles;
	int numTiles;

	Texture** texObjects;
	iPoint* positionObjects;
	int numObjects;

	int mode;// 0:tile, 1:attr, 2:obj
	int selectedTile, selectedAttr, selectedObject;
};

void drawToolRect();
void freeMapEditor();
