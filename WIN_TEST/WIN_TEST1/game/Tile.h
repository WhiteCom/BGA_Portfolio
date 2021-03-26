#pragma once

#include "iRect.h"
#include "iType.h"

#define tileW		16
#define tileH		12
#define tileWSize	32
#define tileHSize	32
#define typeNum		5

#define INF			99

#define tileMapSize iSizeMake(tileWSize, tileHSize) //32x32

//16 x 12 타일맵 생성
struct Tile 
{
	//0 : 초원, 1 : 황야, 2 : 얼음, 3 : 불, 4 : 보스
	int index; //타일 이미지 index
	//iPoint pos;
	int layer; 
	int value; //가중치
};

class MapTile
{
public:
	MapTile();
	virtual ~MapTile();

	//타일을 만드는 함수
	void init(); //타일 초기세팅

	void makeMap(const char* str); //맵 만들기
	void drawTile();

	Tile* grassMap(); //초원
	Tile* wildnessMap(); //황야
	Tile* iceMap(); //얼음
	Tile* fireMap(); //불
	Tile* bossMap(); //보스

public:
	Texture** tile_texs; //다양한 타일셋들
	Tile* tile_map; //현재 생성된 맵
	int tileNum;

};



