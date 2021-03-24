#pragma once

#include "iRect.h"
#include "iType.h"

#define tileW 16
#define tileH 12
#define tileWSize 32
#define tileHSize 32
#define typeNum 2

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
	void setTile(Tile& _tile, Texture* _tex, int _layer, int _value); //Ÿ�� ����

	void makeMap(Tile* ti); //�� �����
	void drawTile();

public:
	Texture** tile_texs; //�پ��� Ÿ�ϼµ�
	Tile* tile_map; //���� ������ ��
};
