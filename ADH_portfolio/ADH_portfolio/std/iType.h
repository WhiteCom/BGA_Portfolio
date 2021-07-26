#pragma once

typedef signed char int8;
typedef unsigned char uint8;
typedef signed short int16;
typedef unsigned short uint16;
typedef signed int int32;
typedef unsigned int uint32;

typedef void (*VOID_METHOD)();
typedef void (*FLOAT_METHOD)(float );

//typedef void igImage;
struct Texture
{
	uint32 texID;
	int width, height;
	int potWidth, potHeight;
	int retainCount;
};


enum iKeyStat
{
	iKeyStatBegan = 0,
	iKeyStatBegan2,
	iKeyStatMoved,
	iKeyStatEnded
};

struct iPoint;
typedef void (*KEY_METHOD)(iKeyStat, iPoint);

#define keysA			1
#define keysS			2
#define keysD			4
#define keysW			8
#define keysSpace		16

#define keysLeft		32
#define keysRight		64
#define keysUp			128
#define	keysDown		256

//anc
#define LEFT			1
#define RIGHT			2
#define HCENTER			4

#define TOP				8
#define BOTTOM			16
#define VCENTER			32

#define REVERSE_NONE	0
#define REVERSE_WIDTH	1
#define REVERSE_HEIGHT	2