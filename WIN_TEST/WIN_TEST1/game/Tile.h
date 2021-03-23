#pragma once

#include "iRect.h"

#define tileW 16
#define tileH 12
#define tileWSize 32
#define tileHSize 32
#define typeNum 2

#define tileMapSize iSizeMake(tileWSize, tileHSize) //32x32


//----------------------------------------
//Tile1.bmp 내의 필드종류
//#need update
//----------------------------------------
static int ForestIndex[4] = {
	20, 76, 132
};

static int DesertIndex[4] = {
	27,
};

static int IceIndex[4] = {

};

static int FireIndex[4] = {

};

//공간아닌곳
static int NoneSection = 119;

//----------------------------------------
//Tile1.bmp 내의 벽 종류 (통행불가 지역)
//#need update
//----------------------------------------
static int ForestWallIndex[4] = {
	18, 26,
};

static int DesertWallIndex[4] = {
	26,
};

static int IceWallIndex[4] = {

};

static int FireWallIndex[4] = {

};


//16 x 12 타일맵 생성
//int -> struct 변형 필요(가중치 포함용)
struct Tile 
{
	//타일 관련 변수들 -> iRect, 가중치(벽&갈수없는 곳, 적, 아이템, 워프포인트 등)
	iSize size;
	int tile; //타일종류
	int value; //가중치
};

//추가사항 이미지에 따라 다른 타일을 그릴수 있도록 해야함 
//256x1024 를 (32x32) 로 끊어, 타입별로 다양한 이미지들생성
//타입 : 필드 - 벽 & 구멍 - 워프 이미지 - 아이템 - 적

//여러 타일맵을 만들어야하기에 클래스로 선언
class TileMap
{
public:
	TileMap();
	virtual ~TileMap();

	//타일을 만드는 함수
	void init(); //타일 초기세팅
	void setTile(int& ti, const char* str); //타일 세팅

	void makeMap(Tile* ti); //맵 만들기
	void drawTile();

public:
	//Tile mapTile; //선택된 칩셋
	Tile* tileType; 
	int typeCount;
};
