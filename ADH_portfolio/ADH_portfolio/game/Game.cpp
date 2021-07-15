#include "Game.h"

#include "Intro.h"
#include "MapEditor.h"
#include "Menu.h"
#include "Ending.h"
#include "Stage.h"
#include "Battle.h"

#include "Loading.h"

#include "Common.h"

void loadGame()
{
#if 0
    callAppData();
#endif

    AudioInfo ai[SOUND_NUM] = {
        {"assets/sound/cursor.wav", false, 1.0f},
        {"assets/sound/EditorBg.wav", true, 1.0f},
        {"assets/sound/BgMusic.wav", true, 1.0f},
        {"assets/sound/BgMusic2.wav", true, 1.0f},
        {"assets/sound/BattleBg1.wav", true, 1.0f},
        {"assets/sound/Hit00.wav", false, 1.0f},
        {"assets/sound/Hit03 A.wav", false, 1.0f},
    };
    loadAudio(ai, SOUND_NUM);
    //audioVolume(appData->bgm, appData->eff, 2);
    audioVolume(1.0f, 1.0f, 0);

    setStringName("assets/font/DungGeunMo.ttf");
    
    loadIntro(); 
    gameState = gs_intro;
}

void drawGame(float dt)
{
    switch (gameState)
    {
    case gs_intro: drawIntro(dt); break;
    case gs_menu: drawMenu(dt); break;
    case gs_map: drawMapEditor(dt); break;
    case gs_stage: drawStage(dt); break;
    case gs_battle: drawBattle(dt); break;
    case gs_ending: drawEnding(dt); break;
    }

    drawLoading(dt);
}

void freeGame()
{
    //saveAppData();

    switch (gameState)
    {
    case gs_intro: freeIntro(); break;
    case gs_menu: freeMenu(); break;
    case gs_map: freeMapEditor(); break;
    case gs_stage: freeStage(); break;
    case gs_battle: freeBattle(); break;
    case gs_ending: freeEnding(); break;
    }
}

void keyGame(iKeyStat stat, iPoint p)
{
    switch (gameState)
    {
    case gs_intro: keyIntro(stat, p); break;
    case gs_menu: keyMenu(stat, p); break;
    case gs_map: keyMapEditor(stat, p); break;
    case gs_stage:  keyStage(stat, p); break;
    case gs_battle: keyBattle(stat, p); break;
    case gs_ending: keyEnding(stat, p); break;
    }
}