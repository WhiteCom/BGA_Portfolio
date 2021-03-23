#pragma once

#include "iRect.h"

#define tileW 16
#define tileH 12
#define tileWSize 32
#define tileHSize 32
#define typeNum 2

#define tileMapSize iSizeMake(tileWSize, tileHSize) //32x32


//----------------------------------------
//Tile1.bmp ���� �ʵ�����
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

//�����ƴѰ�
static int NoneSection = 119;

//----------------------------------------
//Tile1.bmp ���� �� ���� (����Ұ� ����)
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


//16 x 12 Ÿ�ϸ� ����
//int -> struct ���� �ʿ�(����ġ ���Կ�)
struct Tile 
{
	//Ÿ�� ���� ������ -> iRect, ����ġ(��&�������� ��, ��, ������, ��������Ʈ ��)
	iSize size;
	int tile; //Ÿ������
	int value; //����ġ
};

//�߰����� �̹����� ���� �ٸ� Ÿ���� �׸��� �ֵ��� �ؾ��� 
//256x1024 �� (32x32) �� ����, Ÿ�Ժ��� �پ��� �̹��������
//Ÿ�� : �ʵ� - �� & ���� - ���� �̹��� - ������ - ��

//���� Ÿ�ϸ��� �������ϱ⿡ Ŭ������ ����
class TileMap
{
public:
	TileMap();
	virtual ~TileMap();

	//Ÿ���� ����� �Լ�
	void init(); //Ÿ�� �ʱ⼼��
	void setTile(int& ti, const char* str); //Ÿ�� ����

	void makeMap(Tile* ti); //�� �����
	void drawTile();

public:
	//Tile mapTile; //���õ� Ĩ��
	Tile* tileType; 
	int typeCount;
};
