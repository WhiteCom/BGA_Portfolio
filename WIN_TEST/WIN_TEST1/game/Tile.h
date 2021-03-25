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
	Texture* tex; //Ÿ�� �̹���
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

	void makeMap(const char* str); //�� �����
	void drawTile();

	Tile* grassMap(); //�ʿ�
	Tile* wildnessMap(); //Ȳ��
	Tile* iceMap(); //����
	Tile* fireMap(); //��
	Tile* bossMap(); //����

private:
	//���������� �� Ÿ�Ϻ�����
	Tile* field;
	Tile* wall;
	Tile warp;
	Tile none;

public:
	Texture** tile_texs; //�پ��� Ÿ�ϼµ�
	Tile* tile_map; //���� ������ ��
	

};


