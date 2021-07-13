#pragma once

#include "iStd.h"
#include "Character.h"


extern int stageFrom, stageTo;
extern Character* mainCharacter;

void loadStage();
void freeStage();
void drawStage(float dt);
void keyStage(iKeyStat stat, iPoint point);

//=========================================================
// popTopUI : popSetting, popStageHow, popInven
//=========================================================

void createPopTopUI();
void freePopTopUI();
void drawPopTopUI(float dt);
void showPopTopUI(bool show);
bool keyPopTopUI(iKeyStat stat, iPoint point);

//=========================================================
// popSetting : 설정창
//=========================================================

void createPopSetting();
void freePopSetting();
void drawPopSetting(float dt);
void showPopSetting(bool show);
bool keyPopSetting(iKeyStat stat, iPoint point);

//=========================================================
// popStageHow : 게임방법
//=========================================================

void createPopStageHow();
void freePopStageHow();
void drawPopStageHow(float dt);
void showPopStageHow(bool show);
bool keyPopStageHow(iKeyStat stat, iPoint point);

//=========================================================
// popOption : option -> popSetting
//=========================================================

void createPopStageOption();
void freePopStageOption();
void drawPopStageOption(float dt);
void showPopStageOption(bool show);
bool keyPopStageOption(iKeyStat stat, iPoint point);

//=========================================================
// popTmp : option 임시 팝업 -> popSetting
//=========================================================

void createPopTmp();
void freePopTmp();
void drawPopTmp(float dt);
void showPopTmp(bool show);
bool keyPopTmp(iKeyStat stat, iPoint point);

//=========================================================
// popExit : 나가기 팝업 -> popSetting
//=========================================================

void createPopStageExit();
void freePopStageExit();
void drawPopStageExit(float dt);
void showPopStageExit(bool show);
bool keyPopStageExit(iKeyStat stat, iPoint point);

//=========================================================
// popInven : 인벤토리
//=========================================================

void createPopInven();
void freePopInven();
void drawPopInven(float dt);
void showPopInven(bool show);
bool keyPopInven(iKeyStat stat, iPoint point);

//=========================================================
// popBottomUI : 대화 스크립트
//=========================================================

void createPopBottomUI();
void freePopBottomUI();
void drawPopBottomUI(float dt);
void showPopBottomUI(bool show);
bool keyPopBottomUI(iKeyStat stat, iPoint point);

//=========================================================
// popOverStep : 발걸음 다되서 종료
//=========================================================

void createPopOverStep();
void freePopOverStep();
void showPopOverStep(bool show);
void drawPopOverStep(float dt);
bool keyPopOverStep(iKeyStat stat, iPoint point);

//=========================================================
// popStageStr : 스테이지 글자표시
//=========================================================

void createPopStageStr();
void freePopStageStr();
void showPopStageStr(bool show);
void drawPopStageStr(float dt);

//=========================================================
// popStepStr : 스테이지 발걸음표시
//=========================================================

void createPopStepStr();
void freePopStepStr();
void showPopStepStr(bool show);
void drawPopStepStr(float dt);


