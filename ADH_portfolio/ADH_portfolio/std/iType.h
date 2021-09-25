#pragma once

#if 0
#define OS_WINDOW 0
#define OS_ANDROID 1

#define OS OS_ANDROID
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#if (OS==OS_WINDOW)

#include <windows.h>

#elif (OS==OS_ANDROID)

#include <jni.h>
#include <android/log.h>

#include <sys/time.h>

enum VK {
	keysLeft = 0,
	keysRight = 0,

	VK_RETURN,
	VK_ESC,
	VK_LEFT,
	VK_RIGHT
};

#endif

typedef signed char int8;
typedef unsigned char uint8;
typedef signed short int16;
typedef unsigned short uint16;
typedef signed int int32;
typedef unsigned int uint32;

typedef void (*VOID_METHOD)();
typedef void (*FLOAT_METHOD)(float);

struct Texture
{
	uint32 texID;// GLuint texID;
	float width, height;
	float potWidth, potHeight;
	int retainCount;
};

enum iKeyStat
{
	iKeyStatBegan = 0,
	iKeyStatBegan2,
	iKeyStatMoved,
	iKeyStatEnded,
};
struct iPoint;
typedef void (*KEY_METHOD)(iKeyStat, iPoint);

struct iSize;
struct iRect;

#define keysA		1 // 0000 0001 left
#define keysD		2 // 0000 0010 right
#define keysW		4 // 0000 0100 up
#define keysS		8 // 0000 1000 down
#define keysSpace	16// 0001 0000 space

#define keysLeft	32// 0000 0001 left
#define keysRight	64// 0000 0010 right
#define keysUp		128// 0000 0100 up
#define keysDown	256// 0000 1000 down

// anc
#define LEFT		 1// 0000 0001
#define RIGHT		 2// 0000 0010
#define HCENTER		 4// 0000 0100

#define TOP			 8// 0000 1000
#define BOTTOM		16// 0001 0000
#define VCENTER		32// 0010 0000

#define REVERSE_NONE	0
#define REVERSE_WIDTH	1
#define REVERSE_HEIGHT	2
