#pragma once

#include "iType.h"
#include "iStd.h"

/*
* 팝업 클래스
- Texture
- size (width, height)
- 문자열(const char* str, iStrTex)
- 버튼 (iImage, iPopup)
- 함수내 쓰이는 함수포인터 함수들( static 함수로 선언 )

(변수)
- iPopup* pop;
- iStrTex** iStr; (iStrTex가 2개쓰이는데도 있음
- iImage** imgBtn;

(함수)
- void createPop()
- void freePop();
- void draPop(float dt);
- void showPop(bool show);
- bool keyPop(iKeyStat stat, iPoint point);


----
* 참고 파일 (Stage)
- iPopup* popStageOption;
- iImage** imgStageOptionBtn;
- iStrTex* stStageSound, *stStageEff;

- void drawPopStageOptionBefore(iPopup* pop, float dt, float rate);
- void closeMethodPopStageOption(iPopup* pop);
- Texture* stStageSoundMethod(const char* str);

*/

class UI
{
public:
	void createUI();
	void freeUI();
	void drawUI(float dt);
	void showUI(bool show);
	bool keyUI(iKeyStat stat, iPoint point);

	//함수포인터관련
	static void drawUIBefore(iPopup* pop, float dt, float rate);
	static void closeUIMethod(iPopup* pop);
	static Texture* TextureUIMethod(const char* str);
public:
	iPopup* UIpop;
	iImage** UIimg;
	iStrTex** UIiStr;
};