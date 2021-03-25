#pragma once

//타입 : 필드 - 벽 & 구멍 - 워프 이미지 - 아이템 - 적

//----------------------------------------
//Tile1.bmp 내의 필드종류
//#need update
//----------------------------------------
int ForestIndex[4] = {
	20, 76, 86, 
};

int DesertIndex[4] = {
	27, 84, 
};

int IceIndex[4] = {
	71, 179, 181,
};

int FireIndex[4] = {
	14, 15, 
};

int BossIndex[4] = {
	231,
};

//공간아닌곳
int NoneSection = 128, Warp = 132; //warp : 132

//----------------------------------------
//Tile1.bmp 내의 벽 종류 (통행불가 지역)
//#need update
//----------------------------------------
int ForestWallIndex[4] = {
	18, 116,
};

int DesertWallIndex[4] = {
	26, 121,
};

int IceWallIndex[4] = {
	42, 187, 131, 189
};

int FireWallIndex[4] = {
	89, 126,
};

int BossWallIndex[4] = {
	129, 
};