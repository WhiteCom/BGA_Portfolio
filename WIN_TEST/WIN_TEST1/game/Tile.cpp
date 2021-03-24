#include "Tile.h"
#include "iStd.h"

//타입 : 필드 - 벽 & 구멍 - 워프 이미지 - 아이템 - 적

//----------------------------------------
//Tile1.bmp 내의 필드종류
//#need update
//----------------------------------------
static int ForestIndex[4] = {
	20, 76, 86,
};

static int DesertIndex[4] = {
	27, 84,
};

static int IceIndex[4] = {
	71, 179, 181, 
};

static int FireIndex[4] = {
	14, 15,
};

static int Boss[4] = {
	129, 231,
};

//공간아닌곳
static int NoneSection = 128, Warp = 132; //warp : 132

//----------------------------------------
//Tile1.bmp 내의 벽 종류 (통행불가 지역)
//#need update
//----------------------------------------
static int ForestWallIndex[4] = {
	18, 116
};

static int DesertWallIndex[4] = {
	26, 121, 
};

static int IceWallIndex[4] = {
	42, 187, 131, 189
};

static int FireWallIndex[4] = {
	89, 126,
};

MapTile::MapTile()
{
	tile_texs = createImageDivide(8, 32, "assets/Image/Tile1.bmp");

	Tile tmp;
	tmp.tex = tile_texs[NoneSection];
	tmp.layer = 0;
	tmp.value = 0;

	tile_map = new Tile[tileW * tileH]; //16x12
	for (int i = 0; i < tileW * tileH; i++)
		tile_map[i] = tmp;

}
MapTile::~MapTile()
{
	delete tile_map;
}

void MapTile::init()
{
	Tile tmp;
	tmp.tex = tile_texs[Warp];
	tmp.layer = 0;
	tmp.value = 0;
	
	Tile* another_map = new Tile[tileW * tileH]{
		tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp,
		tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp,
		tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp,
		tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp,
		tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp,
		tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp,
		tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp,
		tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp,
		tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp,
		tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp,
		tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp,
		tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp,
	};

	tile_map = another_map;
}

void MapTile::setTile(Tile& _tile, Texture* _tex, int _layer, int _value) //타일 세팅
{
	
}

void MapTile::makeMap(Tile* ti)
{
	Tile field[4] = {
		{}, //초원
		{}, //황야
		{}, //얼음
		{}, //불
	};
}

void MapTile::drawTile()
{
	iPoint off = iPointMake(32, 32);

	for (int j = 0; j < tileH; j++)
	{
		for (int i = 0; i < tileW; i++)
		{
			int k = j * tileW + i;
			drawImage(tile_map[k].tex, off.x + 32 * i, off.y + 32 * j, TOP | LEFT);

			setRGBA(0.5, 0, 0, 1);
			iRect rt = iRectMake(off.x + 32 * i, off.y + 32 * j, 32, 32);
			drawRect(rt);
		}
	}
}