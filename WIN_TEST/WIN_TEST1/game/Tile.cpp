#include "Tile.h"
#include "iStd.h"

#include "TileType.h"

//0 : 초원, 1 : 황야, 2 : 얼음, 3 : 불, 4 : 보스
#define none		{0, 0, 0}
#define warp		{1, 0, 0}
#define field(n)	{n, 0, 0}
#define wall(n)		{n, 0, 0}

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

	tileNum = tileW * tileH; //타일 개수

	//0 : 초원, 1 : 황야, 2 : 얼음, 3 : 불, 4 : 보스
	field = new Tile[typeNum]{
		{tile_texs[ForestIndex[0]], 0, 0}, //초원
		{tile_texs[DesertIndex[0]], 0, 0}, //황야
		{tile_texs[IceIndex[0]], 0, 0}, //얼음
		{tile_texs[FireIndex[0]], 0, 0}, //불
		{tile_texs[BossIndex[0]], 0, 0}, //보스
	};

	wall = new Tile[typeNum]{
		{tile_texs[ForestWallIndex[0]], 1, 1}, //초원
		{tile_texs[DesertWallIndex[0]], 1, 1}, //황야
		{tile_texs[IceWallIndex[0]], 1, 1}, //얼음
		{tile_texs[FireWallIndex[0]], 1, 1}, //불
		{tile_texs[BossWallIndex[0]], 1, 1}, //보스
	};

	warp.tex = tile_texs[Warp];
	warp.layer = 0;
	warp.value = 0;

	none.tex = tile_texs[NoneSection];
	none.layer = INF;
	none.value = INF;
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

void MapTile::makeMap(const char* str)
{
	
	if (str == "grass" || str == "Grass" || str == "GRASS")
		tile_map = grassMap();

	else if (str == "wildness" || str == "Wildness" || str == "WILDNESS")
		tile_map = wildnessMap();

	else if (str == "ice" || str == "Ice" || str == "ICE")
		tile_map = iceMap();

	else if (str == "fire" || str == "Fire" || str == "FIRE")
		tile_map = fireMap();

	else //if(str == "boss" || str == "Boss" || str == "BOSS")
		tile_map = bossMap();

}


void MapTile::drawTile()
{
	iPoint off = iPointMake(32, 32);

	for (int j = 0; j < tileH; j++)
	{
		for (int i = 0; i < tileW; i++)
		{
			int k = j * tileW + i;
			Texture* tex = tile_texs[tile_map[k].index];
			drawImage(tex, off.x + 32 * i, off.y + 32 * j, TOP | LEFT);

			setRGBA(0.5, 0, 0, 1);
			iRect rt = iRectMake(off.x + 32 * i, off.y + 32 * j, 32, 32);
			drawRect(rt);
		}
	}
}



Tile* MapTile::grassMap() //초원
{
	//0 : 초원, 1 : 황야, 2 : 얼음, 3 : 불, 4 : 보스
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

	return map;
}

Tile* MapTile::wildnessMap() //황야
{

	//#need update 이따구로 쓰면 맵 생성때마다 계속 이지랄해야함.
	Tile* map = new Tile[tileW * tileH]{
		none,	none,	none,		none,	  none,		none,	  none,		warp,	  none,		none,	  none,		none,	  none,		none,	  none,		none,
		none, field(1), field[1],	field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], none,
		none, field(1), wall[1],	field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], warp,
		none, field(1), wall[1],	field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], none,
		none, field(1), field[1],	field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], none,
		none, field(1), field[1],	field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], none,
		none, field(1), field[1],	field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], none,
		none, field(1), field[1],	field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], none,
		none, field(1), field[1],	field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], none,
		warp, field(1), wall[1],	field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], none,
		none, field(1), wall[1],	field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], field[1], none,
		none, none,		none,		none,	  none,		none,	  none,		warp,	  none,		none,	  none,		none,	  none,		none,	  none,		none,
	};

	return map;
}
Tile* MapTile::iceMap() //얼음
{
	//0 : 초원, 1 : 황야, 2 : 얼음, 3 : 불, 4 : 보스
	field[2];
	wall[2];

	//#need update 이따구로 쓰면 맵 생성때마다 계속 이지랄해야함.
	Tile* map = new Tile[tileW * tileH]{
		none,	none,	none,		none,	  none,		none,	  none,		warp,	  none,		none,	  none,		none,	  none,		none,	  none,		none,
		none, field[2], field[2],	field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], none,
		none, field[2], wall[2],	field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], warp,
		none, field[2], wall[2],	field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], none,
		none, field[2], field[2],	field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], none,
		none, field[2], field[2],	field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], none,
		none, field[2], field[2],	field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], none,
		none, field[2], field[2],	field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], none,
		none, field[2], field[2],	field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], none,
		warp, field[2], wall[2],	field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], none,
		none, field[2], wall[2],	field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], field[2], none,
		none, none,		none,		none,	  none,		none,	  none,		warp,	  none,		none,	  none,		none,	  none,		none,	  none,		none,
	};

	return map;
}
Tile* MapTile::fireMap() //불
{
	//0 : 초원, 1 : 황야, 2 : 얼음, 3 : 불, 4 : 보스
	field[3];
	wall[3];

	//#need update 이따구로 쓰면 맵 생성때마다 계속 이지랄해야함.
	Tile* map = new Tile[tileW * tileH]{
		none,	none,	none,		none,	  none,		none,	  none,		warp,	  none,		none,	  none,		none,	  none,		none,	  none,		none,
		none, field[3], field[3],	field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], none,
		none, field[3], wall[3],	field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], warp,
		none, field[3], wall[3],	field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], none,
		none, field[3], field[3],	field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], none,
		none, field[3], field[3],	field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], none,
		none, field[3], field[3],	field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], none,
		none, field[3], field[3],	field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], none,
		none, field[3], field[3],	field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], none,
		warp, field[3], wall[3],	field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], none,
		none, field[3], wall[3],	field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], field[3], none,
		none, none,		none,		none,	  none,		none,	  none,		warp,	  none,		none,	  none,		none,	  none,		none,	  none,		none,
	};

	return map;
}
Tile* MapTile::bossMap() //보스
{
	//0 : 초원, 1 : 황야, 2 : 얼음, 3 : 불, 4 : 보스
	field[4];
	wall[4];

	//#need update 이따구로 쓰면 맵 생성때마다 계속 이지랄해야함.
	Tile* map = new Tile[tileW * tileH]{
		none,	none,	none,		none,	  none,		none,	  none,		warp,	  none,		none,	  none,		none,	  none,		none,	  none,		none,
		none, field[4], field[4],	field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], none,
		none, field[4], wall[4],	field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], warp,
		none, field[4], wall[4],	field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], none,
		none, field[4], field[4],	field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], none,
		none, field[4], field[4],	field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], none,
		none, field[4], field[4],	field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], none,
		none, field[4], field[4],	field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], none,
		none, field[4], field[4],	field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], none,
		warp, field[4], wall[4],	field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], none,
		none, field[4], wall[4],	field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], field[4], none,
		none, none,		none,		none,	  none,		none,	  none,		warp,	  none,		none,	  none,		none,	  none,		none,	  none,		none,
	};

	return map;
}