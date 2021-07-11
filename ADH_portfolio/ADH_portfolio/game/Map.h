#pragma once

#include "iStd.h"

struct enemyType
{
	int type;
	int num;
};

struct enemyInfo
{
	enemyType* eType;
	int eTypeNum; 
};

class Map
{
public:
	Map();
	Map(const char* szFormat, ...);
	virtual ~Map();

	void clean();

	void draw(iPoint off, Texture** texs);

	void init(int x, int y, int w, int h);

	void load(const char* szFormat, ...);
	void save(const char* str, ...);

#if 1
	void loadA(char* buf);
	void saveA(char* buf);
#endif

	void insert(iPoint point, int type);

public:
	int tileX, tileY, tileWidth, tileHeight;
	
	//tileX * tileY
	int* tileIndex;
	int* tileWeight;
	enemyInfo* tileEnemy;

	int selectedTile, selectedWeight;
};