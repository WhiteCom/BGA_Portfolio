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
	//0 : 초원, 1 : 황야, 2 : 얼음, 3 : 불, 4 : 보스
	Tile field[5] = { //제일 아래 레이어, 지나가도록 가중치 0
		{tile_texs[ForestIndex[0]], 0, 0}, //초원
		{tile_texs[DesertIndex[0]], 0, 0}, //황야
		{tile_texs[IceIndex[0]], 0, 0}, //얼음
		{tile_texs[FireIndex[0]], 0, 0}, //불
		{tile_texs[Boss[0]], 0, 0},
	};

	Tile wall[4] = { //바닥보다 위 레이어, 지나갈수없도록 가중치 1
		{tile_texs[ForestWallIndex[0]], 1, 1}, //초원
		{tile_texs[DesertWallIndex[0]], 1, 1}, //황야
		{tile_texs[IceWallIndex[0]], 1, 1}, //얼음
		{tile_texs[FireWallIndex[0]], 1, 1}, //불
	};

	Tile warp;
	warp.tex = tile_texs[Warp];
	warp.layer = 1;
	warp.value = 0;

	Tile none; //아무도 갈수없는영역
	none.tex = tile_texs[NoneSection];
	none.layer = INF;
	none.value = INF;

	field[0];
	wall[0];

	//#need update 이따구로 쓰면 맵 생성때마다 계속 이지랄해야함.
	Tile* map = new Tile[tileW * tileH]{
		none,	none,	none,		none,	  none,		none,	  none,		warp,	  none,		none,	  none,		none,	  none,		none,	  none,		none,
		none, field[0], field[0],	field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], none,
		none, field[0], wall[0],	field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], warp,
		none, field[0], wall[0],	field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], none,
		none, field[0], field[0],	field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], none,
		none, field[0], field[0],	field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], none,
		none, field[0], field[0],	field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], none,
		none, field[0], field[0],	field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], none,
		none, field[0], field[0],	field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], none,
		warp, field[0], wall[0],	field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], none,
		none, field[0], wall[0],	field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], field[0], none,
		none, none,		none,		none,	  none,		none,	  none,		warp,	  none,		none,	  none,		none,	  none,		none,	  none,		none,
	};

	tile_map = map;
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