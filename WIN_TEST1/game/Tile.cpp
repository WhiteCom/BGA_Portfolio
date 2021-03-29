#include "Tile.h"
#include "iStd.h"

#include "TileType.h"

static int texW = 8, texH = 32;

MapTile::MapTile()
{
	tile_texs = createImageDivide(texW, texH, "assets/Image/Tile1.bmp");

	Tile tmp = NoneSection;
	
	tile_map = new Tile[tileW * tileH]; //16x12
	for (int i = 0; i < tileW * tileH; i++)
		tile_map[i] = tmp;

	tileNum = tileW * tileH; //타일 개수
	
	off = iPointMake(tileWSize, tileHSize);
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

void MapTile::makeMap(const char* str, int n)
{
	
	if (str == "grass" || str == "Grass" || str == "GRASS")
		grassMap(n);

	else if (str == "wildness" || str == "Wildness" || str == "WILDNESS")
		wildnessMap(n);

	else if (str == "ice" || str == "Ice" || str == "ICE")
		iceMap(n);

	else if (str == "fire" || str == "Fire" || str == "FIRE")
		fireMap(n);

	else //if(str == "boss" || str == "Boss" || str == "BOSS")
		bossMap(n);

}

void MapTile::drawTile()
{
	for (int j = 0; j < tileH; j++)
	{
		for (int i = 0; i < tileW; i++)
		{
			int k = j * tileW + i;
			Texture* tex = tile_texs[tile_map[k].index];
			drawImage(tex, off.x + tileWSize * i, off.y + tileHSize * j, TOP | LEFT);

			setRGBA(0.5, 0, 0, 1);
			iRect rt = iRectMake(off.x + tileWSize * i, off.y + tileHSize * j, 32, 32);
			drawRect(rt);
		}
	}
}

static Tile no = NoneSection;
static Tile fi = ForestField(0);
static Tile wp = WarpSection;
static Tile wa = ForestWall(0);

void MapTile::grassMap(int n) //초원
{
	fi = ForestField(n);
	wa = ForestWall(n);
	
	Tile* map;

	if (n == 0) //top-left
	{
		map = new Tile[tileW * tileH]{ //16x12
			no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no,
			no, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			no, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			no, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			no, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, wp,
			no, fi, fi, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, fi, no,
			no, fi, fi, fi, fi, fi, wa, fi, fi, fi, wa, fi, fi, fi, fi, no,
			no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
			no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
			no, no, no, no, no, no, wp, no, no, no, no, no, no, no, no, no,
		};
	}

	else if (n == 1) //top-right
	{
		map = new Tile[tileW * tileH]{ //16x12
			no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no,
			no, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			no, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			no, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			wp, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, no,
			no, fi, fi, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, fi, no,
			no, fi, fi, fi, fi, fi, wa, fi, fi, fi, wa, fi, fi, fi, fi, no,
			no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
			no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
			no, no, no, no, no, no, wp, no, no, no, no, no, no, no, no, no,
		};
	}
	else if (n == 2) //bottom-left
	{
		map = new Tile[tileW * tileH]{ //16x12
		no, no, no, no, no, no, no, wp, no, no, no, no, no, no, no, no,
		no, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, wp,
		no, fi, fi, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, fi, wa, fi, fi, fi, wa, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
		no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no,
		};
	}
	else //if (n == 3) //bottom-right
	{
		map = new Tile[tileW * tileH]{ //16x12
		no, no, no, no, no, no, no, wp, no, no, no, no, no, no, no, no,
		no, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		wp, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, fi, wa, fi, fi, fi, wa, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
		no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no,
		};
	}

	tile_map = map;
}

void MapTile::wildnessMap(int n) //황야
{
	fi = DesertField(n);
	wa = DesertWall(n);

	Tile* map;

	if (n == 0) //top-left
	{
		map = new Tile[tileW * tileH]{ //16x12
			no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no,
			no, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			no, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			no, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			no, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, wp,
			no, fi, fi, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, fi, no,
			no, fi, fi, fi, fi, fi, wa, fi, fi, fi, wa, fi, fi, fi, fi, no,
			no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
			no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
			no, no, no, no, no, no, wp, no, no, no, no, no, no, no, no, no,
		};
	}

	else if (n == 1) //top-right
	{
		map = new Tile[tileW * tileH]{ //16x12
			no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no,
			no, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			no, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			no, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			wp, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, no,
			no, fi, fi, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, fi, no,
			no, fi, fi, fi, fi, fi, wa, fi, fi, fi, wa, fi, fi, fi, fi, no,
			no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
			no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
			no, no, no, no, no, no, wp, no, no, no, no, no, no, no, no, no,
		};
	}
	else if (n == 2) //bottom-left
	{
		map = new Tile[tileW * tileH]{ //16x12
		no, no, no, no, no, no, no, wp, no, no, no, no, no, no, no, no,
		no, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, wp,
		no, fi, fi, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, fi, wa, fi, fi, fi, wa, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
		no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no,
		};
	}
	else //if (n == 3) //bottom-right
	{
		map = new Tile[tileW * tileH]{ //16x12
		no, no, no, no, no, no, no, wp, no, no, no, no, no, no, no, no,
		no, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		wp, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, fi, wa, fi, fi, fi, wa, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
		no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no,
		};
	}
	tile_map = map;
}

void MapTile::iceMap(int n) //얼음
{
	fi = IceField(n);
	wa = IceWall(n);

	Tile* map;

	if (n == 0) //top-left
	{
		map = new Tile[tileW * tileH]{ //16x12
			no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no,
			no, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			no, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			no, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			no, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, wp,
			no, fi, fi, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, fi, no,
			no, fi, fi, fi, fi, fi, wa, fi, fi, fi, wa, fi, fi, fi, fi, no,
			no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
			no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
			no, no, no, no, no, no, wp, no, no, no, no, no, no, no, no, no,
		};
	}

	else if (n == 1) //top-right
	{
		map = new Tile[tileW * tileH]{ //16x12
			no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no,
			no, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			no, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			no, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			wp, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, no,
			no, fi, fi, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, fi, no,
			no, fi, fi, fi, fi, fi, wa, fi, fi, fi, wa, fi, fi, fi, fi, no,
			no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
			no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
			no, no, no, no, no, no, wp, no, no, no, no, no, no, no, no, no,
		};
	}
	else if (n == 2) //bottom-left
	{
		map = new Tile[tileW * tileH]{ //16x12
		no, no, no, no, no, no, no, wp, no, no, no, no, no, no, no, no,
		no, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, wp,
		no, fi, fi, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, fi, wa, fi, fi, fi, wa, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
		no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no,
		};
	}
	else //if (n == 3) //bottom-right
	{
		map = new Tile[tileW * tileH]{ //16x12
		no, no, no, no, no, no, no, wp, no, no, no, no, no, no, no, no,
		no, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		wp, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, fi, wa, fi, fi, fi, wa, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
		no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no,
		};
	}

	tile_map = map;
}

void MapTile::fireMap(int n) //불
{
	fi = FireField(n);
	wa = FireWall(n);

	Tile* map;

	if (n == 0) //top-left
	{
		map = new Tile[tileW * tileH]{ //16x12
			no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no,
			no, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			no, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			no, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			no, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, wp,
			no, fi, fi, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, fi, no,
			no, fi, fi, fi, fi, fi, wa, fi, fi, fi, wa, fi, fi, fi, fi, no,
			no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
			no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
			no, no, no, no, no, no, wp, no, no, no, no, no, no, no, no, no,
		};
	}

	else if (n == 1) //top-right
	{
		map = new Tile[tileW * tileH]{ //16x12
			no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no,
			no, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			no, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			no, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			wp, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, no,
			no, fi, fi, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, fi, no,
			no, fi, fi, fi, fi, fi, wa, fi, fi, fi, wa, fi, fi, fi, fi, no,
			no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
			no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
			no, no, no, no, no, no, wp, no, no, no, no, no, no, no, no, no,
		};
	}
	else if (n == 2) //bottom-left
	{
		map = new Tile[tileW * tileH]{ //16x12
		no, no, no, no, no, no, no, wp, no, no, no, no, no, no, no, no,
		no, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, wp,
		no, fi, fi, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, fi, wa, fi, fi, fi, wa, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
		no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no,
		};
	}
	else //if (n == 3) //bottom-right
	{
		map = new Tile[tileW * tileH]{ //16x12
		no, no, no, no, no, no, no, wp, no, no, no, no, no, no, no, no,
		no, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		wp, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, fi, wa, fi, fi, fi, wa, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
		no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no,
		};
	}
	tile_map = map;
}
void MapTile::bossMap(int n) //보스
{
	fi = BossField(n);
	wa = BossWall(n);

	Tile* map;

	if (n == 0) //top-left
	{
		map = new Tile[tileW * tileH]{ //16x12
			no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no,
			no, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			no, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			no, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			no, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, wp,
			no, fi, fi, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, fi, no,
			no, fi, fi, fi, fi, fi, wa, fi, fi, fi, wa, fi, fi, fi, fi, no,
			no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
			no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
			no, no, no, no, no, no, wp, no, no, no, no, no, no, no, no, no,
		};
	}

	else if (n == 1) //top-right
	{
		map = new Tile[tileW * tileH]{ //16x12
			no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no,
			no, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			no, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			no, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
			wp, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, no,
			no, fi, fi, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, fi, no,
			no, fi, fi, fi, fi, fi, wa, fi, fi, fi, wa, fi, fi, fi, fi, no,
			no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
			no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
			no, no, no, no, no, no, wp, no, no, no, no, no, no, no, no, no,
		};
	}
	else if (n == 2) //bottom-left
	{
		map = new Tile[tileW * tileH]{ //16x12
		no, no, no, no, no, no, no, wp, no, no, no, no, no, no, no, no,
		no, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, wp,
		no, fi, fi, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, fi, wa, fi, fi, fi, wa, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
		no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no,
		};
	}
	else //if (n == 3) //bottom-right
	{
		map = new Tile[tileW * tileH]{ //16x12
		no, no, no, no, no, no, no, wp, no, no, no, no, no, no, no, no,
		no, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, wa, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, no,
		wp, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, wa, wa, fi, fi, fi, fi, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, fi, wa, fi, fi, fi, wa, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
		no, fi, fi, fi, fi, fi, fi, fi, fi, fi, wa, fi, fi, fi, fi, no,
		no, no, no, no, no, no, no, no, no, no, no, no, no, no, no, no,
		};
	}

	tile_map = map;
}