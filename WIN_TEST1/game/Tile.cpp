#include "Tile.h"
#include "iStd.h"

#include "TileType.h"

#define NoneSection				{NoneIdx, INF, INF}
#define WarpSection				{WarpIdx, 0, 0}

//0 : 초원, 1 : 황야, 2 : 얼음, 3 : 불, 4 : 보스
#define ForestField(n)			{ForestIdx[n], 0, 0}
#define DesertField(n)			{DesertIdx[n], 0, 0}
#define IceField(n)				{IceIdx[n], 0, 0}
#define FireField(n)			{FireIdx[n], 0, 0}
#define BossField(n)			{BossIdx[n], 0, 0}

#define ForestWall(n)			{ForestWallIdx[n], 1, 1}
#define DesertWall(n)			{DesertWallIdx[n], 1, 1}
#define IceWall(n)				{IceWallIdx[n], 1, 1}
#define FireWall(n)				{FireWallIdx[n], 1, 1}
#define BossWall(n)				{BossWallIdx[n], 1, 1}

static int texW = 8, texH = 32;

MapTile::MapTile()
{
	tile_texs = createImageDivide(texW, texH, "assets/Image/Tile1.bmp");

	Tile tmp = NoneSection;
	
	tile_map = new Tile[tileW * tileH]; //16x12
	for (int i = 0; i < tileW * tileH; i++)
		tile_map[i] = tmp;

	tileNum = tileW * tileH; //타일 개수
}

MapTile::~MapTile()
{
	delete tile_map;
	//#maybe issue
	for (int i = 0; i < texW * texH; i++)
		delete tile_texs[i];
	delete tile_texs;
}

void MapTile::init()
{
	Tile tmp = NoneSection;
	
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
		grassMap();

	else if (str == "wildness" || str == "Wildness" || str == "WILDNESS")
		wildnessMap();

	else if (str == "ice" || str == "Ice" || str == "ICE")
		iceMap();

	else if (str == "fire" || str == "Fire" || str == "FIRE")
		fireMap();

	else //if(str == "boss" || str == "Boss" || str == "BOSS")
		bossMap();

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

static Tile no = NoneSection;
static Tile fi = ForestField(0);
static Tile wp = WarpSection;
static Tile wa = ForestWall(0);

void MapTile::grassMap() //초원
{
	fi = ForestField(0);
	wa = ForestWall(0);
	
	Tile* map = new Tile[tileW * tileH]{ //16x12
		no, no, no, no, no, no, no, wp, no, no, no, no, no, no, no, no,
		no, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		wp, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, wp,
		no, fi, fi, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, fi, wa, fi, fi, fi, wa, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
		no, no, no, no, no, no, wp, no, no, no, no, no, no, no, no, no,
	};

	tile_map = map;
}

void MapTile::wildnessMap() //황야
{
	fi = DesertField(0);
	wa = DesertWall(0);

	Tile* map = new Tile[tileW * tileH]{ //16x12
		no, no, no, no, no, no, no, wp, no, no, no, no, no, no, no, no,
		no, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		wp, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, wp,
		no, fi, fi, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, fi, wa, fi, fi, fi, wa, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
		no, no, no, no, no, no, wp, no, no, no, no, no, no, no, no, no,
	};

	tile_map = map;
}

void MapTile::iceMap() //얼음
{
	fi = IceField(0);
	wa = IceWall(0);

	Tile* map = new Tile[tileW * tileH]{ //16x12
		no, no, no, no, no, no, no, wp, no, no, no, no, no, no, no, no,
		no, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		wp, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, wp,
		no, fi, fi, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, fi, wa, fi, fi, fi, wa, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
		no, no, no, no, no, no, wp, no, no, no, no, no, no, no, no, no,
	};

	tile_map = map;
}

void MapTile::fireMap() //불
{
	fi = FireField(0);
	wa = FireWall(0);

	Tile* map = new Tile[tileW * tileH]{ //16x12
		no, no, no, no, no, no, no, wp, no, no, no, no, no, no, no, no,
		no, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		wp, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, wp,
		no, fi, fi, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, fi, wa, fi, fi, fi, wa, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
		no, no, no, no, no, no, wp, no, no, no, no, no, no, no, no, no,
	};

	tile_map = map;
}
void MapTile::bossMap() //보스
{
	fi = BossField(0);
	wa = BossWall(0);

	Tile* map = new Tile[tileW * tileH]{ //16x12
		no, no, no, no, no, no, no, wp, no, no, no, no, no, no, no, no,
		no, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		wp, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, wp,
		no, fi, fi, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, fi, wa, fi, fi, fi, wa, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
		no, no, no, no, no, no, wp, no, no, no, no, no, no, no, no, no,
	};

	tile_map = map;
}