#include "sc_Std.h"


// Real Data
extern _Script* script;
extern Say* dummySay;

// array Say
extern HWND* hBtnProjIO;
extern HWND hLbProjData;
extern HWND* hBtnProjMgt;

// struct Say
extern HWND* hCbSayWhoBg;
extern HWND* hEbSayRegion;
extern HWND hEbSayComment;

void loadScriptTool();
void freeScriptTool();
void updateScriptTool(WPARAM wParam, LPARAM lParam);

//hwndBtnProj
void callProj(); // hwndBtnProj[0];
void saveProj(); // hwndBtnProj[1];

// hLbProjData
void cbLbSay();

// hWndBtnSayCtrl
void sayAdd();
void sayDel();
void sayUp();
void sayDown();
void sayUpdate();

Say* getSay();
void resetSay();