#pragma once

//타입 : 필드 - 벽 & 구멍 - 워프 이미지 - 아이템 - 적
#define NoneSection				{NoneIdx, INF, INF}
#define WarpSection				{WarpIdx, 0, 1}

//0 : 초원, 1 : 황야, 2 : 얼음, 3 : 불, 4 : 보스
#define ForestField(n)			{ForestIdx[n], 0, 0}
#define DesertField(n)			{DesertIdx[n], 0, 0}
#define IceField(n)				{IceIdx[n], 0, 0}
#define FireField(n)			{FireIdx[n], 0, 0}
#define BossField(n)			{BossIdx[n], 0, 0}

#define ForestWall(n)			{ForestWallIdx[n], 1, 2}
#define DesertWall(n)			{DesertWallIdx[n], 1, 2}
#define IceWall(n)				{IceWallIdx[n], 1, 2}
#define FireWall(n)				{FireWallIdx[n], 1, 2}
#define BossWall(n)				{BossWallIdx[n], 1, 2}

//----------------------------------------
//Tile1.bmp 내의 필드종류
//#need update
//----------------------------------------
int ForestIdx[4] = {
	20, 76, 86, 
};

int DesertIdx[4] = {
	27, 84, 
};

int IceIdx[4] = {
	71, 179, 181,
};

int FireIdx[4] = {
	14, 15, 
};

int BossIdx[4] = {
	231,
};

//공간아닌곳
int NoneIdx = 128, WarpIdx = 132; //warp : 132

//----------------------------------------
//Tile1.bmp 내의 벽 종류 (통행불가 지역)
//#need update
//----------------------------------------
int ForestWallIdx[4] = {
	18, 116,
};

int DesertWallIdx[4] = {
	26, 121,
};

int IceWallIdx[4] = {
	42, 187, 131, 189
};

int FireWallIdx[4] = {
	89, 126,
};

int BossWallIdx[4] = {
	129, 
};