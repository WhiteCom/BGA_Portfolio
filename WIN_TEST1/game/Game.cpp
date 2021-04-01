#include "Game.h"

#include "Intro.h"
#include "Map2.h"
#include "Ending.h"

#include "Loading.h"

void loadGame()
{
    loadIntro(); 
    gameState = gs_intro;
}

void drawGame(float dt)
{
    switch (gameState)
    {
    case gs_intro: drawIntro(dt); break;
    //case gs_map: drawMap(dt, "grass"); break;
    case gs_map2: drawMap(dt, "grass"); break;
    case gs_ending: drawEnding(dt); break;
    }

    drawLoading(dt);
}

void freeGame()
{
    switch (gameState)
    {
    case gs_intro: freeIntro(); break;
    //case gs_map: freeMap(); break;
    case gs_map2: freeMap(); break;
    case gs_ending: freeEnding(); break;
    }
}

void keyGame(iKeyStat stat, iPoint p)
{
    switch (gameState)
    {
    case gs_intro: keyIntro(stat, p); break;
    //case gs_map: keyMap(stat, p); break;
    case gs_map2: keyMap(stat, p); break;
    case gs_ending: keyEnding(stat, p); break;
    }
}