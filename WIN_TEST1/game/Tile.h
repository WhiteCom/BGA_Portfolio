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

//16 x 12 Ÿ�ϸ� ����
struct Tile 
{
	//0 : �ʿ�, 1 : Ȳ��, 2 : ����, 3 : ��, 4 : ����
	int index; //Ÿ�� �̹��� index
	//iPoint pos;
	int layer; 
	int value; //����ġ
};

class MapTile
{
public:
	MapTile();
	virtual ~MapTile();

	//Ÿ���� ����� �Լ�
	void init(); //Ÿ�� �ʱ⼼��

	void makeMap(const char* str, int n); //�� �����
	void drawTile();

	void grassMap(int n); //�ʿ�
	void wildnessMap(int n); //Ȳ��
	void iceMap(int n); //����
	void fireMap(int n); //��
	void bossMap(int n); //����

public:
	Texture** tile_texs; //�پ��� Ÿ�ϼµ�
	Tile* tile_map; //���� ������ ��
	int tileNum;

	iPoint off;
};



