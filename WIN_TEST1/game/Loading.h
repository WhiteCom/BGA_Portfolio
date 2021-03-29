#pragma once

#include "iStd.h"

#define gs_intro			0
#define gs_menu				1
#define gs_map				2
#define gs_proc				3
#define gs_ending			4

#define gs_loading			0xff

//#define gs_grassMap1		4
//#define gs_grassMap2		5
//#define gs_grassMap3		6
//#define gs_grassMap4		7
//
//#define gs_desertMap1		8
//#define gs_desertMap2		9
//#define gs_desertMap3		10
//#define gs_desertMap4		11
//
//#define gs_iceMap1			12
//#define gs_iceMap2			13
//#define gs_iceMap3			14
//#define gs_iceMap4			15
//
//#define gs_fireMap1			16
//#define gs_fireMap2			17
//#define gs_fireMap3			18
//#define gs_fireMap4			19
//
//#define gs_bossMap1			20
//#define gs_bossMap2			21
//#define gs_bossMap3			22
//#define gs_bossMap4			23



extern int gameState;

typedef void(*MethodLoad)();
void setLoading(int gameState, MethodLoad free, MethodLoad load);
void drawLoading(float dt);