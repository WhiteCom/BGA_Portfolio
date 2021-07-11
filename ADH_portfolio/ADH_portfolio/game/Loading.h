#pragma once

#include "iStd.h"

#define gs_intro			0
#define gs_menu				1
#define gs_map				2
#define gs_proc				3
#define gs_ending			4
#define gs_stage			5
#define gs_battle			6
#define gs_gameover			7
#define gs_loading			0xff

extern int gameState;
extern float loadingDt;

typedef void(*MethodLoad)();
typedef void(*MethodTime)(float dt);

void setLoading(int gameState, MethodLoad free, MethodLoad load, MethodTime time=NULL);
void drawLoading(float dt);

void loadProgress(int max);
void freeProgress();
void drawProgress(float dt);
void setProgress(int tick);

void setGameOver(int gameState, MethodLoad free, MethodLoad load, MethodTime time = NULL);
void drawGameOver(float dt);