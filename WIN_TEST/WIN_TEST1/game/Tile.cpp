#include "Tile.h"
#include "iStd.h"

TileMap::TileMap()
{
	tileType = new Tile[typeNum];
	for (int i = 0; i < typeNum; i++)
	{
		tileType[i].size = tileMapSize;
		tileType[i].tile = 0;
		tileType[i].value = 0;
	}
	typeCount = 0;
}
TileMap::~TileMap()
{
	delete tileType;
}

void TileMap::init()
{
	 
}

void TileMap::setTile(int& ti, const char* str) //타일 세팅
{
	Tile field[4] = {
		{tileMapSize, ForestIndex[0], 1}, //초원
		{tileMapSize, DesertIndex[0], 1}, //황야
		{tileMapSize, IceIndex[0], 1}, //얼음
		{tileMapSize, FireIndex[0], 1}, //불
	};

	Tile wall[4] = {
		{tileMapSize, ForestWallIndex[0], 2}, //초원
		{tileMapSize, DesertWallIndex[0], 2}, //황야
		{tileMapSize, IceWallIndex[0], 2}, //얼음
		{tileMapSize, FireWallIndex[0], 2}, //불
	};

	Tile warp; 

	Tile tmp = field[0];
	
	if (str == "field")
		tmp = field[ti];
	else if (str == "wall")
		tmp = wall[ti];
	else if (str == "warp");
		//tmp = warp;

	if (typeCount < typeNum)
	{
		tileType[typeCount] = tmp;
		typeCount++;
	}
	
}

void TileMap::makeMap(Tile* ti)
{
	
}

void TileMap::drawTile()
{
	static Texture* tex = createImage("assets/Image/Tile1.bmp");

	//16x12
	Tile a = tileType[0];
	Tile b = tileType[1];
	Tile tileMap[tileW * tileH] =
	{
		a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b,
		a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b,
		a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b,
		a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b,
		a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b,
		a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b,
		a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b,
		a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b,
		a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b,
		a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b,
		a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b,
		a, b, a, b, a, b, a, b, a, b, a, b, a, b, a, b,
	};
	
	for (int j = 0; j < tileH; j++)
	{
		for (int i = 0; i < tileW; i++)
		{
			setRGBA(1, 0, 0, 1);
			iRect rect = iRectMake(tileWSize * i, tileHSize * j, tileWSize, tileHSize);
			drawRect(rect);
			int x = tileMap[tileW * j + i].tile % 8;
			int y = tileMap[tileW * j + i].tile / 8;

			drawImage(tex, 32 * i, 32 * j, TOP | LEFT,
				32 * x, 32 * y, 32, 32, 1.0f, 1.0f, 2, 0);
		}
	}
	
}