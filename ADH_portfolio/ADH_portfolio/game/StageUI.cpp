#include "StageUI.h"
#include "Stage.h"

#include "Common.h"
#include "Loading.h"
#include "Menu.h"
#include "Ending.h"
#if (OS==OS_WINDOW)
#include "App.h"
#endif

//=========================================================
// popTopUI : 메뉴, 설정, 인벤토리
//=========================================================

iPopup* popTopUI; //메뉴, 설정, 인벤토리
iImage** imgTopBtn; //메뉴, 설정, 인벤토리

void drawPopTopUIBefore(iPopup* pop, float dt, float rate);

void createPopTopUI()
{
    iImage* img;
    Texture* tex;

    popTopUI = new iPopup();
    imgTopBtn = new iImage * [3]; // 설정, 방법, 인벤

#if (OS==OS_WINDOW)
    const char* imgPath[3] = {
        "assets/setting2.png",
        "assets/how.png",
        "assets/Inven2.png",
    };
#elif (OS==OS_ANDROID)
    const char* imgPath[3] = {
            "setting2.png",
            "how.png",
            "inven2.png",
    };
#endif

    iGraphics* g = iGraphics::share();
    iSize size = iSizeMake(50, 50);

    iPopup* pop = new iPopup();
    for (int i = 0; i < 3; i++)
    {

        img = new iImage();
        for (int j = 0; j < 2; j++)
        {
            g->init(size);
            igImage* btnImg = g->createIgImage(imgPath[i]);
            if (j == 0)
            {
                setRGBA(0, 0, 0, 1);
                g->fillRect(0, 0, size.width, size.height, 10);
                setRGBA(1, 1, 1, 1);
                g->drawRect(0, 0, size.width, size.height, 10);
#if (OS==OS_WINDOW)
                g->drawigImage(btnImg, size.width / 2, size.height / 2, VCENTER | HCENTER,
                    0, 0, btnImg->GetWidth(), btnImg->GetHeight(), (float)50 / 512, (float)50 / 512,
                    2, 0);
#elif (OS==OS_ANDROID)
                g->drawigImage(btnImg, size.width / 2, size.height / 2, VCENTER | HCENTER,
                    0, 0, g->igImageGetWidth(btnImg), g->igImageGetHeight(btnImg),
                    (float)50 / 512, (float)50 / 512,
                    2, 0);
#endif
            }
            else
            {
                setRGBA(1, 1, 0, 1);
                g->fillRect(0, 0, size.width, size.height, 10);
                setRGBA(0, 0, 0, 1);
                g->drawRect(0, 0, size.width, size.height, 10);
                setRGBA(1, 1, 1, 1);
#if (OS==OS_WINDOW)
                g->drawigImage(btnImg, size.width / 2, size.height / 2, VCENTER | HCENTER,
                    0, 0, btnImg->GetWidth(), btnImg->GetHeight(), (float)50 / 512, (float)50 / 512,
                    2, 0);
#elif (OS==OS_ANDROID)
                g->drawigImage(btnImg, 0, 0, TOP | LEFT,
                    0, 0, (float)g->igImageGetWidth(btnImg), (float)g->igImageGetHeight(btnImg),
                    (float)50 / 512, (float)50 / 512,
                    2, 0);
#endif
            }
            tex = g->getTexture();
            img->addObject(tex);
            freeImage(tex);
        }
        img->position = iPointMake((25 + i * 80), 25);
        pop->addObject(img);

        imgTopBtn[i] = img;
    }

    pop->methodBefore = drawPopTopUIBefore;
    pop->style = iPopupAlpha;
    pop->openPoint = iPointZero;
    pop->closePoint = iPointZero;

    popTopUI = pop;
}

void freePopTopUI()
{
    delete popTopUI;
    delete imgTopBtn;
}

void drawPopTopUIBefore(iPopup* pop, float dt, float rate)
{
    for (int i = 0; i < 3; i++)
        imgTopBtn[i]->setTexObject(i == popTopUI->selected);
}
void drawPopTopUI(float dt)
{
    popTopUI->paint(dt);
    drawPopSetting(dt);
    drawPopStageHow(dt);
    drawPopInven(dt);
}
void showPopTopUI(bool show)
{
    popTopUI->show(show);
    if (show)
    {

    }
}

// for Android
#if (OS==OS_ANDROID)
bool keyPopTopUI(iKeyStat stat, iPoint point)
{
    if (keyPopSetting(stat, point) ||
        keyPopStageHow(stat, point) ||
        keyPopInven(stat, point))
    {
        return true;
    }

    iPopup* pop = popTopUI;
    if (pop->bShow == false)
        return false;

#if 0 //  화면에서 벗어나면
    float left = devSize.width;
    float right = 0.0;
    float top = devSize.height;
    float bottom = 0.0;
    xprint("keyTopUI l r t b : %.1f %.1f %.1f %.1f", left, right, top, bottom);

    for (int i = 0; i < pop->arrayImg->count; i++)
    {
        xprint("i : %d", i);
        iImage* img = (iImage*)pop->arrayImg->objectAtIndex(i);
        xprint("keyTopUI img : %d", img);
        if (img == NULL)
        {
            continue;
        }
        Texture* tex = img->tex;
        xprint("keyTopUI tex : %d", tex);
        if (tex == NULL)
        {
            continue;
        }
        xprint("img position, tex->width : %f %f", img->position.x, tex->width);
        if (left > img->position.x)
        {
            left = img->position.x;
        }
        xprint("img position, tex->width : %f %f", img->position.x, tex->width);
        if (right < img->position.x + tex->width)
        {
            right = img->position.x + tex->width;
        }
        if (top > img->position.y)
        {
            top = img->position.y;
        }
        if (bottom < img->position.x + tex->height)
        {
            bottom = img->position.x + tex->height;
        }
        xprint("l r t b : %f %f %f %f", left, right, top, bottom);
    }
    iRect rt = iRectMake(left, top, right - left, bottom - top);
    xprint("keyTopUI rt l t w h : %.1f %.1f %.1f %.1f", rt.origin.x, rt.origin.y, rt.size.width, rt.size.height);
    if (containPoint(point, rt) == false)
        return false;
#else
    //if( containPoint(point, iRectMake(0, 0, 200, 80))==false )
    //    return false;
#endif
    if (pop->stat != iPopupProc)
    {
        return true;
    }
    int i, j = -1;

    switch (stat)
    {
    case iKeyStatBegan:
        for (i = 0; i < 3; i++)
        {
            if (containPoint(point, imgTopBtn[i]->touchRect(pop->closePoint, iSizeMake(30, 30))))
            {
                j = i;
                break;
            }
        }
        if (pop->selected != j)
        {
#if 0 //#openAL
            audioPlay(0);
#endif
            pop->selected = j;
        }
        break;

    case iKeyStatMoved:
        i = pop->selected;
        if (i == -1) break;
        if (containPoint(point, imgTopBtn[i]->touchRect(pop->closePoint, iSizeMake(30, 30))) == false)
        {
#if 0 //#openAL
            audioPlay(0);
#endif
            pop->selected = -1;
        }
        break;

    case iKeyStatEnded:
        i = pop->selected;
        if (i == -1) break;
        pop->selected = -1;

        if (i == 0)
        {
            xprint("Setting");

            showPopSetting(true);
        }
        else if (i == 1)
        {

            xprint("How");

            showPopStageHow(true);
        }
        else //if (i == 2)
        {
            xprint("Inven");

            showPopInven(true);
        }
        break;
    }
    return false;
}

// =========================================================================================
// for Window
// =========================================================================================

#elif(OS==OS_WINDOW)
bool keyPopTopUI(iKeyStat stat, iPoint point)
{
    if (keyPopSetting(stat, point) ||
        keyPopStageHow(stat, point) ||
        keyPopInven(stat, point))
    {
        return true;
    }

    iPopup* pop = popTopUI;

    if (pop->bShow == false)
        return false;

    if (pop->stat != iPopupProc)
        return true;

    int i, j = -1;

    switch (stat)
    {
    case iKeyStatBegan:
        if (pop->selected == -1)
            break;

        if (pop->selected == 0)
        {
            xprint("Setting\n");
            pop->selected = -1;

            showPopSetting(true);
        }
        else if (pop->selected == 1)
        {
            xprint("How\n");
            pop->selected = -1;

            showPopStageHow(true);
        }
        else //if (pop->selected == 2)
        {
            xprint("Inven\n");
            pop->selected = -1;

            showPopInven(true);
        }
        break;

    case iKeyStatMoved:
        for (i = 0; i < 3; i++)
        {
            if (containPoint(point, imgTopBtn[i]->touchRect(pop->closePoint)))
            {
                j = i;
                break;
            }
        }
        if (pop->selected != j)
        {
#if 0 //#openAL
            audioPlay(0);
#endif
            pop->selected = j;
        }
        break;

    case iKeyStatEnded:
        break;
    }

    return true;
}
#endif

//=========================================================
// popSetting : 설정창
//=========================================================
iPopup* popSetting;
iImage** imgSetBtn;

void drawMethodPopSet(iPopup* pop, float dt, float rate);
void closePopSettingMethod(iPopup* pop);

void createPopSetting()
{
    popSetting = new iPopup();

    iImage* img;
    Texture* tex;
    iPopup* pop = new iPopup();

    //
    //Bg
    //
    iGraphics* g = iGraphics::share();
    iSize size = iSizeMake(480, 480);
    g->init(size);

    setRGBA(0.5f, 0.5f, 0.5f, 0.8f);
    g->fillRect(0, 0, size.width, size.height, 10);
    setRGBA(1, 1, 1, 1);

    setStringSize(40);
    setStringRGBA(0, 0, 0, 1);
    setStringBorder(2);
    setStringBorderRGBA(1, 1, 1, 1);
    g->drawString(size.width / 2, size.height / 2 - 100, VCENTER | HCENTER, "<설정>");
    img = new iImage();
    tex = g->getTexture();
    img->addObject(tex);
    freeImage(tex);
    pop->addObject(img);

    //
    //Btn
    //

    imgSetBtn = new iImage * [4]; //Resume, Menu, Option, Exit

    const char* strBtn[4] = {
        "Resume",
        "Menu",
        "Option",
        "Exit"
    };

    size = iSizeMake(200, 40);
    for (int i = 0; i < 4; i++)
    {
        img = new iImage();
        for (int j = 0; j < 2; j++)
        {
            g->init(size);

            if (j == 0)
            {
                setRGBA(0, 0, 0, 1);
                g->fillRect(0, 0, size.width, size.height, 10);
                setRGBA(1, 1, 1, 1);
                g->drawRect(0, 0, size.width, size.height, 10);

                setStringSize(25);
                setStringRGBA(0, 0, 0, 1);
                setStringBorder(2);
                setStringBorderRGBA(1, 1, 1, 1);
                g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, strBtn[i]);
                tex = g->getTexture();
                img->addObject(tex);
                freeImage(tex);
            }
            else
            {
                setRGBA(1, 1, 0, 1);
                g->fillRect(0, 0, size.width, size.height, 10);
                setRGBA(1, 1, 1, 1);
                g->drawRect(0, 0, size.width, size.height, 10);

                setStringSize(25);
                setStringRGBA(0.4f, 0.5f, 1, 1);
                setStringBorder(2);
                setStringBorderRGBA(1, 1, 1, 1);
                g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, strBtn[i]);
                tex = g->getTexture();
                img->addObject(tex);
                freeImage(tex);
            }

        }
        img->position = iPointMake(240 - size.width / 2, 240 + i * 60);
        pop->addObject(img);
        imgSetBtn[i] = img;
    }
    pop->style = iPopupZoom;
    pop->openPoint = iPointMake(devSize.width / 2, devSize.height / 2);
    pop->closePoint = iPointMake(devSize.width / 2 - 240, devSize.height / 2 - 240);
    pop->methodBefore = drawMethodPopSet;
    pop->methodClose = closePopSettingMethod;

    popSetting = pop;
}

void freePopSetting()
{
    delete popSetting;
    delete imgSetBtn;
}

void drawMethodPopSet(iPopup* pop, float dt, float rate)
{
    for (int i = 0; i < 4; i++)
        imgSetBtn[i]->setTexObject(popSetting->selected == i);
}
void closePopSettingMethod(iPopup* pop)
{
    if (popSetting->selected == 2)
        showPopStageOption(true);
    else if (popSetting->selected == 3)
        showPopStageExit(true);

    popSetting->selected = -1;
}

void drawPopSetting(float dt)
{
    popSetting->paint(dt);
    drawPopStageOption(dt);
    drawPopStageExit(dt);
}
void showPopSetting(bool show)
{
    popSetting->show(show);
    if (show)
    {
    }
}
#if (OS==OS_WINDOW)
bool keyPopSetting(iKeyStat stat, iPoint point)
{
    iPopup* pop = popSetting;
    if (keyPopStageOption(stat, point) ||
        keyPopStageExit(stat, point))
    {
        return true;
    }

    if (popSetting->bShow == false)
        return false;

    int i, j = -1;

    switch (stat)
    {
    case iKeyStatBegan:
        if (pop->selected == -1)
            break;

        else if (pop->selected == 0) //Resume
        {
            showPopSetting(false);
        }
        else if (pop->selected == 1) //Menu
        {
            setLoading(gs_menu, freeStage, loadMenu);
        }
        else if (pop->selected == 2) //Option
        {
            showPopSetting(false);
        }
        else if (pop->selected == 3) //Exit
        {
            showPopSetting(false);
        }

        break;

    case iKeyStatMoved:
        for (i = 0; i < 4; i++)
        {
            if (containPoint(point, imgSetBtn[i]->touchRect(pop->closePoint)))
            {
                j = i;
                break;
            }
        }
        if (pop->selected != j)
        {
#if 0 //#openAL
            audioPlay(0);
#endif
            pop->selected = j;
        }
        break;

    case iKeyStatEnded:
        break;
    }
    return false;

}
#elif (OS==OS_ANDROID)
bool keyPopSetting(iKeyStat stat, iPoint point)
{
    iPopup* pop = popSetting;
    if (keyPopStageOption(stat, point) ||
        keyPopStageExit(stat, point))
    {
        return true;
    }

    if (popSetting->bShow == false)
        return false;

#if 1 //화면에서 벗어나면 /////////////////////////////////////////////////////////

#endif /////////////////////////////////////////////////////////////////////////

    int i, j = -1;

    switch (stat)
    {
    case iKeyStatBegan:
        for (i = 0; i < 4; i++)
        {
            if (containPoint(point, imgSetBtn[i]->touchRect(pop->closePoint, iSizeMake(20, 20))))
            {
                j = i;
                break;
            }
        }
        if (pop->selected != j)
        {
#if 0 //openAL
            audioPlay(0);
#endif
            pop->selected = j;
        }

        break;

    case iKeyStatMoved:
        i = pop->selected;
        if (i == -1) break;

        if (containPoint(point, imgSetBtn[i]->touchRect(pop->closePoint, iSizeMake(20, 20))) == false)
        {
#if 0 //openAL
            audioPlay(0);
#endif
            pop->selected = -1;
        }

        break;

    case iKeyStatEnded:
        i = pop->selected;
        if (i == -1) break;
#if 0
        pop->selected = -1;
        //closeMethod 영향으로 해당함수에 있음.
#endif
        if (i == -1)
            break;

        else if (i == 0) //Resume
        {
            showPopSetting(false);
        }
        else if (i == 1) //Menu
        {
            setLoading(gs_menu, freeStage, loadMenu);
        }
        else if (i == 2) //Option
        {
            showPopSetting(false);
        }
        else if (i == 3) //Exit
        {
            showPopSetting(false);
        }
        break;
    }
    return true;
}
#endif

//=========================================================
// popStageOption : option -> popSetting
//=========================================================
iPopup* popStageOption;
iImage** imgStageOptionBtn;
iStrTex* stStageSound, * stStageEff;

void drawPopStageOptionBefore(iPopup* pop, float dt, float rate);
void closeMethodPopStageOption(iPopup* pop);
Texture* stStageSoundMethod(const char* str);

void createPopStageOption()
{
    iPopup* pop = new iPopup();
    iImage* img;
    Texture* tex;

    //
    // Bg
    //
    iGraphics* g = iGraphics::share();
    iSize size = iSizeMake(480, 480);
    g->init(size);

    setRGBA(0.5f, 0.5f, 0.5f, 0.8f);
    g->fillRect(0, 0, size.width, size.height, 10);

    setStringSize(40);
    setStringRGBA(0, 0, 0, 1);
    setStringBorder(1);
    setStringBorderRGBA(1, 1, 1, 1);
    g->drawString(size.width / 2, 40, VCENTER | HCENTER, "<Option>");

    setStringSize(35);
#if (OS==OS_WINDOW)
    g->drawString(35, 40 + 80, TOP | LEFT, "전체화면 : ");
    g->drawString(35, 40 + 130, TOP | LEFT, "BGM : ");
    g->drawString(35, 40 + 180, TOP | LEFT, "EFF : ");
#elif(OS==OS_ANDROID)
    g->drawString(35, 40 + 80, TOP | LEFT, "BGM : ");
    g->drawString(35, 40 + 150, TOP | LEFT, "EFF : ");
#endif

    tex = g->getTexture();
    img = new iImage();
    img->addObject(tex);
    freeImage(tex);
    pop->addObject(img);

    //
    //Btn
    //

#if (OS==OS_WINDOW)
    imgStageOptionBtn = new iImage * [7];

    //닫기

    img = new iImage();

    size = iSizeMake(60, 40);
    for (int i = 0; i < 2; i++)
    {
        g->init(size);
        if (i == 0) setRGBA(0, 0, 0, 1);
        else        setRGBA(1, 1, 0, 1);
        g->fillRect(0, 0, size.width, size.height, 10);

        setStringSize(25);
        setStringRGBA(1, 1, 1, 1);
        g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "닫기");

        tex = g->getTexture();
        img->addObject(tex);
        freeImage(tex);
    }
    img->position = iPointMake(240 - size.width / 2, 480 - 100);
    pop->addObject(img);
    imgStageOptionBtn[0] = img;

    // 전체화면 on/off

    for (int i = 1; i < 3; i++)
    {
        img = new iImage();
        size = iSizeMake(60, 40);
        for (int j = 0; j < 2; j++)
        {
            g->init(size);
            if (j == 0)
            {
                setRGBA(0, 0, 0, 1);
                g->fillRect(0, 0, size.width, size.height, 10);

                setStringSize(30);
                setStringRGBA(0, 0, 0, 1);
            }
            else
            {
                setRGBA(1, 1, 0, 1);
                g->fillRect(0, 0, size.width, size.height, 10);

                setStringSize(30);
                setStringRGBA(0.5f, 0.2f, 1, 1);
            }
            setStringBorder(2);
            setStringBorderRGBA(1, 1, 1, 1);

            if (i == 1)
                g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "ON");
            else if (i == 2)
                g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "OFF");
            tex = g->getTexture();
            img->addObject(tex);
            freeImage(tex);
        }
        img->position = iPointMake(200 + i * 70, 120);
        pop->addObject(img);
        imgStageOptionBtn[i] = img;
    }

    // 음량 btn

    for (int i = 3; i < 7; i++)
    {
        img = new iImage();
        size = iSizeMake(40, 40);
        for (int j = 0; j < 2; j++)
        {
            g->init(size);
            if (j == 0)
            {
                setRGBA(0, 0, 0, 1);
                g->fillRect(0, 0, size.width, size.height, 10);

                setStringSize(30);
                setStringRGBA(0, 0, 0, 1);
            }
            else
            {
                setRGBA(1, 1, 0, 1);
                g->fillRect(0, 0, size.width, size.height, 10);

                setStringSize(30);
                setStringRGBA(0.5f, 0.2f, 1, 1);
            }
            setRGBA(1, 1, 1, 1);
            setStringBorder(2);
            setStringBorderRGBA(1, 1, 1, 1);

            if (i % 2 == 1)
                g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "-");
            else if (i % 2 == 0)
                g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "+");

            tex = g->getTexture();
            img->addObject(tex);
            freeImage(tex);
        }
        img->position = imgStageOptionBtn[1]->position + iPointMake(140 * (!(i % 2)), 50 + (i / 5) * 45);
        pop->addObject(img);
        imgStageOptionBtn[i] = img;
    }

    //음량 str

    iStrTex* st = new iStrTex(stStageSoundMethod);
    st->setString("%f", appData->bgm * 10);
    img = new iImage();
    img->addObject(st->tex);
    img->position = imgStageOptionBtn[3]->position + iPointMake(60, 0);
    pop->addObject(img);
    stStageSound = st;

    //eff str

    st = new iStrTex(stStageSoundMethod);
    st->setString("%f", appData->eff * 10);
    img = new iImage();
    img->addObject(st->tex);
    img->position = imgStageOptionBtn[5]->position + iPointMake(60, 0);
    pop->addObject(img);
    stStageEff = st;

    pop->style = iPopupZoom;
    pop->openPoint = iPointMake(devSize.width / 2, devSize.height / 2);
    pop->closePoint = iPointMake(devSize.width / 2 - 240, devSize.height / 2 - 240);
    pop->methodBefore = drawPopStageOptionBefore;
    pop->methodClose = closeMethodPopStageOption;

    popStageOption = pop;
#elif (OS==OS_ANDROID)
    imgStageOptionBtn = new iImage * [5];

    //닫기

    img = new iImage();

    size = iSizeMake(60, 40);
    for (int i = 0; i < 2; i++)
    {
        g->init(size);
        if (i == 0) setRGBA(0, 0, 0, 1);
        else        setRGBA(1, 1, 0, 1);
        g->fillRect(0, 0, size.width, size.height, 10);

        setStringSize(25);
        setStringRGBA(1, 1, 1, 1);
        g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "닫기");

        tex = g->getTexture();
        img->addObject(tex);
        freeImage(tex);
    }
    img->position = iPointMake(240 - size.width / 2, 480 - 100);
    pop->addObject(img);
    imgStageOptionBtn[0] = img;

    // 음량 btn

    for (int i = 1; i < 5; i++)
    {
        img = new iImage();
        size = iSizeMake(40, 40);
        for (int j = 0; j < 2; j++)
        {
            g->init(size);
            if (j == 0)
            {
                setRGBA(0, 0, 0, 1);
                g->fillRect(0, 0, size.width, size.height, 10);

                setStringSize(30);
                setStringRGBA(0, 0, 0, 1);
            }
            else
            {
                setRGBA(1, 1, 0, 1);
                g->fillRect(0, 0, size.width, size.height, 10);

                setStringSize(30);
                setStringRGBA(0.5f, 0.2f, 1, 1);
            }
            setRGBA(1, 1, 1, 1);
            setStringBorder(2);
            setStringBorderRGBA(1, 1, 1, 1);

            if (i % 2 == 1)
                g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "-");
            else if (i % 2 == 0)
                g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "+");

            tex = g->getTexture();
            img->addObject(tex);
            freeImage(tex);
        }
        img->position = iPointMake(170, 65) + iPointMake(140 * (!(i % 2)), 50 + (i / 3) * 70);
        pop->addObject(img);
        imgStageOptionBtn[i] = img;
    }

    //음량 str

    iStrTex* st = new iStrTex(stStageSoundMethod);
    st->setString("%f", appData->bgm * 10);
    img = new iImage();
    img->addObject(st->tex);
    img->position = imgStageOptionBtn[1]->position + iPointMake(60, 0);
    pop->addObject(img);
    stStageSound = st;

    //eff str

    st = new iStrTex(stStageSoundMethod);
    st->setString("%f", appData->eff * 10);
    img = new iImage();
    img->addObject(st->tex);
    img->position = imgStageOptionBtn[3]->position + iPointMake(60, 0);
    pop->addObject(img);
    stStageEff = st;

    pop->style = iPopupZoom;
    pop->openPoint = iPointMake(devSize.width / 2, devSize.height / 2);
    pop->closePoint = iPointMake(devSize.width / 2 - 240, devSize.height / 2 - 240);
    pop->methodBefore = drawPopStageOptionBefore;
    pop->methodClose = closeMethodPopStageOption;

    popStageOption = pop;
#endif
}

Texture* stStageSoundMethod(const char* str)
{
    iGraphics* g = iGraphics::share();
    iSize size = iSizeMake(60, 40);
    g->init(size);

    setRGBA(0, 0, 0, 1);
    g->fillRect(0, 0, size.width, size.height, 10);

    setStringSize(30);
    setStringRGBA(1, 1, 1, 1);
    setStringBorder(0);

    float ss = atof(str);
    if (ss <= 0.0)
        ss = 0.0;
    g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "%.0f", ss);

    return g->getTexture();
}
void freePopStageOption()
{
    delete popStageOption;
    delete imgStageOptionBtn;
    delete stStageSound;
    delete stStageEff;
}
void drawPopStageOptionBefore(iPopup* pop, float dt, float rate)
{
#if (OS==OS_WINDOW)
    for (int i = 0; i < 7; i++)
        imgStageOptionBtn[i]->setTexObject(popStageOption->selected == i);
#elif(OS==OS_ANDROID)
    for (int i = 0; i < 5; i++)
        imgStageOptionBtn[i]->setTexObject(popStageOption->selected == i);
#endif
}
void closeMethodPopStageOption(iPopup* pop)
{
    showPopSetting(true);
}
void drawPopStageOption(float dt)
{
    popStageOption->paint(dt);
}
void showPopStageOption(bool show)
{
    popStageOption->show(show);
}

#if (OS==OS_WINDOW)
bool keyPopStageOption(iKeyStat stat, iPoint point)
{
    if (popStageOption->bShow == false)
        return false;

    if (popStageOption->stat != iPopupProc)
        return true;

    int i, j = -1;

    switch (stat)
    {
    case iKeyStatBegan:
        if (popStageOption->selected == -1)
            break;

        if (popStageOption->selected == 0)
        {
            showPopStageOption(false);
        }
        else if (popStageOption->selected == 1)
        {
#if (OS==OS_WINDOW)
            if (!isFullscreen)
                goFullscreen();
#endif
        }
        else if (popStageOption->selected == 2)
        {
#if (OS==OS_WINDOW)
            if (isFullscreen)
                goFullscreen();
#endif
        }
        else if (popStageOption->selected == 3)
        {
            if (appData->bgm * 10 > 0)
                appData->bgm -= 0.1;
            stStageSound->setString("%f", appData->bgm * 10);
#if 0 //#openAL
            audioVolume(appData->bgm, appData->eff, 1);
#endif
        }
        else if (popStageOption->selected == 4)
        {
            if (appData->bgm * 10 < 10)
                appData->bgm += 0.1;
            stStageSound->setString("%f", appData->bgm * 10);
#if 0 //#openAL
            audioVolume(appData->bgm, appData->eff, 1);
#endif
        }
        else if (popStageOption->selected == 5)
        {
            if (appData->eff * 10 > 0)
                appData->eff -= 0.1;
            stStageEff->setString("%f", appData->eff * 10);
#if 0 //#openAL
            audioVolume(appData->bgm, appData->eff, 1);
#endif
        }
        else if (popStageOption->selected == 6)
        {
            if (appData->eff * 10 < 10)
                appData->eff += 0.1;
            stStageEff->setString("%f", appData->eff * 10);
#if 0 //#openAL
            audioVolume(appData->bgm, appData->eff, 1);
#endif
        }
        break;

    case iKeyStatMoved:

        for (i = 0; i < 7; i++)
        {
            if (containPoint(point, imgStageOptionBtn[i]->touchRect(popStageOption->closePoint)))
            {
                j = i;
                break;
            }
        }
        if (popStageOption->selected != j)
        {
#if 0 //#openAL
            audioPlay(0);
#endif
            popStageOption->selected = j;
        }
        break;

    case iKeyStatEnded:
        break;
    }
    return true;
}
#elif (OS==OS_ANDROID)
bool keyPopStageOption(iKeyStat stat, iPoint point)
{
    iPopup* pop = popStageOption;

    if (pop->bShow == false)
        return false;

#if 1 //화면에서 벗어나면 false /////////////////////////////////////////////////////////////

#endif //////////////////////////////////////////////////////////////////////////////

    if (pop->stat != iPopupProc)
        return true;

    int i, j = -1;

    switch (stat)
    {
    case iKeyStatBegan:
        for (i = 0; i < 5; i++)
        {
            if (containPoint(point, imgStageOptionBtn[i]->touchRect(pop->closePoint, iSizeMake(20, 20))))
            {
                j = i;
                break;
            }
        }
        if (pop->selected != j)
        {
#if 0 //#openAL
            audioPlay(0);
#endif
            pop->selected = j;
        }
        break;

    case iKeyStatMoved:
        i = pop->selected;
        if (i == -1) break;
        if (containPoint(point, imgStageOptionBtn[i]->touchRect(pop->closePoint, iSizeMake(20, 20))) == false)
        {
#if 0 //openAL
            audioPlay(0);
#endif
            pop->selected = -1;
        }
        break;

    case iKeyStatEnded:
        i = pop->selected;
        if (i == -1) break;
        pop->selected = -1;

        if (i == 0)
        {
            showPopStageOption(false);
        }
        else if (i == 1)
        {
            if (appData->bgm * 10 > 0)
                appData->bgm -= 0.1;
            stStageSound->setString("%f", appData->bgm * 10);
#if 0 //#openAL
            audioVolume(appData->bgm, appData->eff, 1);
#endif
        }
        else if (i == 2)
        {
            if (appData->bgm * 10 < 10)
                appData->bgm += 0.1;
            stStageSound->setString("%f", appData->bgm * 10);
#if 0 //#openAL
            audioVolume(appData->bgm, appData->eff, 1);
#endif
        }
        else if (i == 3)
        {
            if (appData->eff * 10 > 0)
                appData->eff -= 0.1;
            stStageEff->setString("%f", appData->eff * 10);
#if 0 //#openAL
            audioVolume(appData->bgm, appData->eff, 1);
#endif
        }
        else if (i == 4)
        {
            if (appData->eff * 10 < 10)
                appData->eff += 0.1;
            stStageEff->setString("%f", appData->eff * 10);
#if 0 //#openAL
            audioVolume(appData->bgm, appData->eff, 1);
#endif
        }
        break;
    }
    return true;
}
#endif

//=========================================================
// popStageExit : 나가기 팝업 -> popSetting
//=========================================================

iPopup* popStageExit;
iImage** imgStageExitBtn;

void drawPopStageExitBefore(iPopup* pop, float dt, float rate);
void closePopStageExit(iPopup* pop)
{
    showPopSetting(true);
}

void createPopStageExit()
{
    iImage* img = new iImage();
    Texture* tex;

    int i, j;

    iPopup* pop = new iPopup();

    //Bg

    iGraphics* g = iGraphics::share();
    iSize size = iSizeMake(640, 480);
    g->init(size);

    setRGBA(1, 1, 1, 0.7f);
    g->fillRect(0, 0, size.width, size.height, 10);
    setRGBA(1, 1, 1, 1);

    setStringSize(40);
    setStringRGBA(0, 0, 0, 1);
    setStringBorder(2);
    setStringBorderRGBA(1, 1, 1, 1);
    g->drawString(size.width / 2, size.height / 2 - 50, VCENTER | HCENTER, "정말 종료하시겠습니까?");

    tex = g->getTexture();
    img->addObject(tex);
    freeImage(tex);
    pop->addObject(img);

    //Btn

  //Btn

    const char* strBtn[2] = {
        "예", "아니요"
    };
    imgStageExitBtn = new iImage * [2];

    for (i = 0; i < 2; i++)
    {
        img = new iImage();
        for (j = 0; j < 2; j++)
        {
            size = iSizeMake(150, 40);
            g->init(size);

            if (j == 0)
            {
                setRGBA(0, 0, 0, 1);
                g->fillRect(0, 0, size.width, size.height, 10);
                setRGBA(1, 1, 1, 1);
                setStringSize(30);
                setStringRGBA(0, 0, 0, 1);
                setStringBorder(2);
                setStringBorderRGBA(1, 1, 1, 1);
                g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, strBtn[i]);
            }
            else
            {
                setRGBA(1, 1, 0, 1);
                g->fillRect(0, 0, size.width, size.height, 10);
                setRGBA(1, 1, 1, 1);
                setStringSize(30);
                setStringRGBA(0.4f, 0.5f, 1, 1);
                setStringBorder(2);
                setStringBorderRGBA(1, 1, 1, 1);
                g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, strBtn[i]);
            }

            tex = g->getTexture();
            img->addObject(tex);
            freeImage(tex);
        }
        img->position = iPointMake(size.width + 180 * i, 270);
        pop->addObject(img);

        imgStageExitBtn[i] = img;
    }
    pop->style = iPopupRotate;
    pop->openPoint = iPointMake(devSize.width / 2, devSize.height / 2);
    pop->closePoint = iPointMake((devSize.width - 640) / 2, (devSize.height - 480) / 2);
    pop->methodBefore = drawPopStageExitBefore;
    pop->methodClose = closePopStageExit;

    popStageExit = pop;
}
void freePopStageExit()
{
    delete popStageExit;
    delete imgStageExitBtn;
}
void drawPopStageExitBefore(iPopup* pop, float dt, float rate)
{
    for (int i = 0; i < 2; i++)
        imgStageExitBtn[i]->setTexObject(i == popStageExit->selected);
}
void drawPopStageExit(float dt)
{
    popStageExit->paint(dt);
}
void showPopStageExit(bool show)
{
    popStageExit->show(show);
}
#if (OS==OS_WINDOW)
bool keyPopStageExit(iKeyStat stat, iPoint point)
{
    iPopup* pop = popStageExit;

    if (pop->bShow == false)
        return false;
    if (pop->stat != iPopupProc)
        return true;
    int i, j = -1;

    switch (stat)
    {
    case iKeyStatBegan:
        if (pop->selected == -1)
            break;

        if (pop->selected == 0)
        {
            //xprint("예\n");
#if (OS==OS_WINDOW)
            //프로그램 종료
            runWnd = false;
#elif(OS==OS_ANDROID)

#endif
        }
        else //if (pop->selected == 1)
        {
            //xprint("아니요\n");
            showPopStageExit(false);
        }
        break;

    case iKeyStatMoved:
        for (i = 0; i < 2; i++)
        {
            if (containPoint(point, imgStageExitBtn[i]->touchRect(pop->closePoint)))
            {
                j = i;
                break;
            }
        }
        if (pop->selected != j)
        {
#if 0 //#openAL
            audioPlay(0);
#endif
            pop->selected = j;
        }
        break;

    case iKeyStatEnded:
        break;
    }

    return true;
}
#elif (OS==OS_ANDROID)
bool keyPopStageExit(iKeyStat stat, iPoint point)
{
    iPopup* pop = popStageExit;

    if (pop->bShow == false)
        return false;

#if 1 //화면에서 벗어나면 /////////////////////////////////////////////////////////////////////////
#endif //////////////////////////////////////////////////////////////////////////////////////////

    if (pop->stat != iPopupProc)
        return true;
    int i, j = -1;

    switch (stat)
    {
    case iKeyStatBegan:
        for (i = 0; i < 2; i++)
        {
            if (containPoint(point, imgStageExitBtn[i]->touchRect(pop->closePoint)))
            {
                j = i;
                break;
            }
        }
        if (pop->selected != j)
        {
#if 0 //#openAL
            audioPlay(0);
#endif
            pop->selected = j;
        }
        break;

    case iKeyStatMoved:
        i = pop->selected;
        if (i == -1) break;
        if (containPoint(point, imgStageExitBtn[i]->touchRect(pop->closePoint, iSizeMake(20, 20))) == false)
        {
#if 0 //openAL
            audioPlay(0);
#endif
            pop->selected = -1;
        }
        break;

    case iKeyStatEnded:
        i = pop->selected;
        if (i == -1) break;
        pop->selected = -1;

        if (i == 0)
        {
#if (OS==OS_WINDOW)
            //프로그램 종료
            runWnd = false;
#elif(OS==OS_ANDROID)

#endif
        }
        else //if (i == 1)
        {
            showPopStageExit(false);
        }
        break;
    }

    return true;
}
#endif

//=========================================================
// popStageHow : 게임방법
//=========================================================

iStrTex* stPopHow;
iPopup* popStageHow;
iImage** imgHowBtns;

static int page, _page;

Texture* methodStPopHow(const char* str);
void drawBeforePopHow(iPopup* pop, float dt, float rate);

void createPopStageHow()
{
    iImage* img = new iImage();
    iPopup* pop = new iPopup();
    Texture* tex;

    //Bg + Str

    page = 0;
    _page = 3;

    iStrTex* stPop = new iStrTex(methodStPopHow);
    stPop->setString("%d", page);
    img->addObject(stPop->tex);
    pop->addObject(img);
    stPopHow = stPop;

    //Btn

    iGraphics* g = iGraphics::share();
    iSize size = iSizeMake(50, 50);

    const char* strBtn[3] = { "X", "◀", "▶" };
    iPoint positionBtn[3] = {
        {800 - 25, -25},
        {400 - 25 - 100, 540 - 25},
        {400 - 25 + 100, 540 - 25},
    };
    imgHowBtns = new iImage * [3];

    for (int i = 0; i < 3; i++)
    {
        img = new iImage();
        for (int j = 0; j < 2; j++)
        {
            g->init(size);

            if (j == 0) setRGBA(0, 0, 0, 1);
            else setRGBA(1, 1, 0, 1);
            g->fillRect(0, 0, size.width, size.height, 10);

            setStringSize(30);
            setStringRGBA(1, 0, 1, 1);
            setStringBorder(0);
            g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, strBtn[i]);

            tex = g->getTexture();
            img->addObject(tex);
            freeImage(tex);
        }
        img->position = positionBtn[i];
        pop->addObject(img);
        imgHowBtns[i] = img;
    }

    pop->style = iPopupZoom;
    pop->openPoint = iPointMake(devSize.width / 2, devSize.height / 2);
    pop->closePoint = iPointMake(devSize.width / 2 - 400, devSize.height / 2 - 300);
    pop->methodBefore = drawBeforePopHow;

    popStageHow = pop;
}

Texture* methodStPopHow(const char* str)
{
    iGraphics* g = iGraphics::share();
    iSize size = iSizeMake(800, 600);
    g->init(size);

    //
    // Bg
    //
    setRGBA(1, 1, 1, 0.9f);
    g->fillRect(0, 0, size.width, size.height, 5);
    setRGBA(1, 1, 1, 1);

    setStringSize(30);
    setStringRGBA(0, 0, 0, 1);
    setStringBorder(1);
    setStringBorderRGBA(0, 0.7f, 0.5f, 1);

    const char** content = new const char* [4];

    content[0] = "<개발일지 - 기획>";
    content[1] = "- C++, WIN32, GDI+, OpenGL, OpenAL";
    content[2] = "- 기획 : 턴제 RPG 형식 게임제작";
    content[3] = "- 참고 : 헬테이커, 동방영강창(동인게임)";

    const char** content2 = new const char* [5];

    content2[0] = "<개발일지 - 기획>";
    content2[1] = "- 개발목표";
    content2[2] = "- 1순위 : 게임시스템(씬 전환/UI/이벤트/전투)";
    content2[3] = "- 2순위 : 게임완성도";
    content2[4] = "- 3순위 : 플레이 스토어";

    const char** content3 = new const char* [4];

    content3[0] = "<개발일지 - 개발현황>";
    content3[1] = "- 현재 개발 된것 : UI/맵 툴/전투/씬 전환";
    content3[2] = "- 차후 개발목표 : 게임완성도";
    content3[3] = "- 3순위는 다음기회에...";

    int p = atoi(str);

    if (p == 0)
    {
        for (int i = 0; i < 4; i++)
            g->drawString(20, (1 + i) * 45, TOP | LEFT, content[i]);
    }
    else if (p == 1)
    {
        for (int i = 0; i < 5; i++)
            g->drawString(20, (1 + i) * 45, TOP | LEFT, content2[i]);
    }
    else //if(p==3)
    {
        for (int i = 0; i < 4; i++)
            g->drawString(20, (1 + i) * 45, TOP | LEFT, content3[i]);
    }
    g->drawString(size.width / 2, size.height * 0.9f, VCENTER | HCENTER, "%d / %d", 1 + p, _page);

    delete content;
    delete content2;
    delete content3;

    return g->getTexture();
}

void drawBeforePopHow(iPopup* pop, float dt, float rate)
{
    for (int i = 0; i < 3; i++)
        imgHowBtns[i]->setTexObject(popStageHow->selected == i);
}

void freePopStageHow()
{
    delete popStageHow;
    delete stPopHow;
    delete imgHowBtns;
}
void drawPopStageHow(float dt)
{
    popStageHow->paint(dt);
}
void showPopStageHow(bool show)
{
    popStageHow->show(show);
    if (show)
    {
        // to do...

    }
}

//for Window
#if (OS==OS_WINDOW)
bool keyPopStageHow(iKeyStat stat, iPoint point)
{
    iPopup* pop = popStageHow;

    if (pop->bShow == false)
        return false;
    if (pop->stat != iPopupProc)
        return true;

    int i, j = -1;

    switch (stat)
    {
    case iKeyStatBegan:
        if (pop->selected == -1)
            break;

#if 0 //#openAL
        audioPlay(0);
#endif
        if (pop->selected == 0)
        {
            showPopStageHow(false);
        }
        else if (pop->selected == 1)
        {
            page--;
            if (page < 0)
                page = 0;
            stPopHow->setString("%d", page);
        }
        else if (pop->selected == 2)
        {
            page++;
            if (page > _page - 1)
                page = _page - 1;
            stPopHow->setString("%d", page);
        }
        break;

    case iKeyStatMoved:
        for (i = 0; i < 3; i++)
        {
            if (containPoint(point, imgHowBtns[i]->touchRect(pop->closePoint)))
            {
                j = i;
                break;
            }
        }
        if (pop->selected != j)
        {
#if 0 //#openAL
            audioPlay(0);
#endif
            pop->selected = j;
        }
        break;

    case iKeyStatEnded:
        break;
    }

    return true;
}

//for Android
#elif (OS==OS_ANDROID)
bool keyPopStageHow(iKeyStat stat, iPoint point)
{
    iPopup* pop = popStageHow;

    if (pop->bShow == false)
        return false;

#if 1 //화면에서 벗어나면 false ///////////////////////////////////////////////////////

#endif //////////////////////////////////////////////////////////////////////////////

    if (pop->stat != iPopupProc)
        return true;

    int i, j = -1;

    switch (stat)
    {
    case iKeyStatBegan:
        for (i = 0; i < 3; i++)
        {
            if (containPoint(point, imgHowBtns[i]->touchRect(pop->closePoint, iSizeMake(20, 20))))
            {
                j = i;
                break;
            }
        }
        if (pop->selected != j)
        {
#if 0 //#openAL
            audioPlay(0);
#endif
            pop->selected = j;
        }
        break;

    case iKeyStatMoved:
        i = pop->selected;
        if (i == -1) break;
        if (containPoint(point, imgHowBtns[i]->touchRect(pop->closePoint, iSizeMake(20, 20))) == false)
        {
#if 0 //openAL
            audioPlay(0);
#endif
            pop->selected = -1;
        }
        break;

    case iKeyStatEnded:
        i = pop->selected;
        if (i == -1) break;
        pop->selected = -1;
#if 0 //#openAL
        audioPlay(0);
#endif
        if (i == 0)
        {
            showPopStageHow(false);
        }
        else if (i == 1)
        {
            page--;
            if (page < 0)
                page = 0;
            stPopHow->setString("%d", page);
        }
        else if (i == 2)
        {
            page++;
            if (page > _page - 1)
                page = _page - 1;
            stPopHow->setString("%d", page);
        }
        break;
    }

    return true;
}
#endif

//=========================================================
// popInven : 인벤토리
//=========================================================

iPopup* popInven;
iStrTex* stPopInven; //영웅 속성에 대한 이미지
iImage** imgInvenBtn;

Texture* stMethodPopInven(const char* str);
void drawBeforePopInven(iPopup* pop, float dt, float rate);
static char heroName[64];
static char heroHP[64];
static char heroAtk[64];

void createPopInven()
{
    const char* tmpHeroName = "영웅1";
    const char* tmpHeroHP = "100";
    const char* tmpHeroAtk = "50";

#if 0
    strcpy(heroName, tmpHeroName);
    strcpy(heroHP, tmpHeroHP);
    strcpy(heroAtk, tmpHeroAtk);
#else
    int HeroNameLen = strlen(tmpHeroName);
    int HeroHpLen = strlen(tmpHeroHP);
    int HeroAtkLen = strlen(tmpHeroAtk);

    memcpy(heroName, tmpHeroName, HeroNameLen);
    heroName[HeroNameLen] = 0;

    memcpy(heroHP, tmpHeroHP, HeroHpLen);
    heroHP[HeroHpLen] = 0;

    memcpy(heroAtk, tmpHeroAtk, HeroAtkLen);
    heroAtk[HeroAtkLen] = 0;
#endif

    char str[1024];

    //strcpy(str, heroName);
    memcpy(str, heroName, HeroNameLen);
    str[HeroNameLen] = 0;
    strcat(str, "/");
    strcat(str, heroHP);
    strcat(str, "/");
    strcat(str, heroAtk);
    strcat(str, "/");

    iPopup* pop = new iPopup();
    iImage* img = new iImage();
    Texture* tex;

    //
    // Bg + stat
    //

    iStrTex* st = new iStrTex(stMethodPopInven);
    st->setString(str);
    img->addObject(st->tex);
    pop->addObject(img);
    stPopInven = st;

    //
    // Inven Exit Btn
    //

    imgInvenBtn = new iImage * [5];

    iGraphics* g = iGraphics::share();
    iSize size = iSizeMake(50, 50);
    iPoint positionBtn = { 800 - 25, -25 };

    img = new iImage();

    for (int i = 0; i < 2; i++)
    {
        g->init(size);
        if (i == 0) setRGBA(0, 0, 0, 1);
        else        setRGBA(1, 1, 0, 1);
        g->fillRect(0, 0, size.width, size.height, 10);

        setStringSize(30);
        setStringRGBA(1, 0, 1, 1);
        setStringBorder(0);
        g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "X");

        Texture* tex = g->getTexture();
        img->addObject(tex);
        freeImage(tex);
    }
    img->position = positionBtn;
    imgInvenBtn[0] = img;
    pop->addObject(img);

    //
    // Hero Bg
    //

    size = iSizeMake(360, 260);
    g->init(size);

    setRGBA(0.2f, 0.2f, 0.2f, 1);
    g->fillRect(0, 0, size.width, size.height, 5);
    setRGBA(1, 1, 1, 1);

    tex = g->getTexture();
    img = new iImage();
    img->addObject(tex);
    freeImage(tex);
    img->position = iPointMake(20, 20);
    pop->addObject(img);

    //
    // Hero Btns (center : hero, / up, down, left, right)
    //

    //Hero

    iPoint HeroP = iPointMake(200 - 20, 150 - 20);
    img = new iImage();
    size = iSizeMake(40, 40);
    g->init(size);
    setRGBA(1, 1, 1, 1);
    g->drawRect(0, 0, size.width, size.height, 5);
#if (OS==OS_WINDOW) ////////////////////////////////////////////////////////////////////////////////////////////////////////
    igImage* Heroimg = g->createIgImage("assets/Image/Hero0.png");
    g->drawigImage(Heroimg, size.width / 2, size.height / 2, VCENTER | HCENTER);
#elif (OS==OS_ANDROID)
    igImage* Heroimg = g->createIgImage("Image/Hero0.png");
    g->drawigImage(Heroimg, size.width / 2, size.height / 2, VCENTER | HCENTER,
        0, 0, (float)g->igImageGetWidth(Heroimg), (float)g->igImageGetHeight(Heroimg),
        1.0f, 1.0f, //(float)40 / 20, (float)40 / 30,
        2, 0);
#endif ///////////////////////////////////////////////////////////////////////////////////////////////////////
    tex = g->getTexture();
    img->addObject(tex);
    freeImage(tex);
    img->position = HeroP;
    pop->addObject(img);

    //Hero parts : head / armor / shoe / weapon etc...

    iPoint partsP[4] = {
            {HeroP.x - 60,  HeroP.y},
            {HeroP.x + 60,  HeroP.y},
            {HeroP.x,       HeroP.y - 60},
            {HeroP.x,       HeroP.y + 60}
    };

    for (int i = 0; i < 4; i++)
    {
        img = new iImage();
        size = iSizeMake(40, 40);
        for (int j = 0; j < 2; j++)
        {
            g->init(size);
            if (j == 0) setRGBA(0, 0, 0, 1);
            else        setRGBA(1, 1, 0, 1);
            g->fillRect(0, 0, size.width, size.height, 5);
            setRGBA(1, 1, 1, 1);
            g->drawRect(0, 0, size.width, size.height, 5);

            tex = g->getTexture();
            img->addObject(tex);
            freeImage(tex);
        }
        img->position = partsP[i];
        pop->addObject(img);
        imgInvenBtn[i + 1] = img;
    }

    //
    // Items
    //

    //Bg

    img = new iImage();

    size = iSizeMake(760, 280);
    g->init(size);

    setRGBA(0.2f, 0.2f, 0.2f, 1);
    g->fillRect(0, 0, size.width, size.height, 10);
    setRGBA(1, 1, 1, 1);

    tex = g->getTexture();
    img->addObject(tex);
    freeImage(tex);
    img->position = iPointMake(20, 300);
    pop->addObject(img);

    //Items

#if 0
    imgItems = new iImage * [18 * 6];
#endif

    iPoint itemsOff = iPointMake(40, 320);

    for (int i = 0; i < 18 * 6; i++)
    {
        img = new iImage();
        size = iSizeMake(40, 40);
        g->init(size);
        setRGBA(0, 0, 0, 1);
        g->fillRect(0, 0, size.width, size.height, 5);
        setRGBA(1, 1, 1, 1);
        g->drawRect(0, 0, size.width, size.height, 5);

        tex = g->getTexture();
        img->addObject(tex);
        freeImage(tex);
        img->position = itemsOff + iPointMake(i % 18 * 40, i / 18 * 40);
        pop->addObject(img);
    }

    pop->style = iPopupZoom;
    pop->openPoint = iPointMake(devSize.width / 2, devSize.height / 2);
    pop->closePoint = iPointMake(devSize.width / 2 - 400, devSize.height / 2 - 300);
    pop->methodBefore = drawBeforePopInven;
    popInven = pop;
}

Texture* stMethodPopInven(const char* str)
{
    iGraphics* g = iGraphics::share();
    iSize size = iSizeMake(800, 600);
    g->init(size);

    //
    // Bg
    //

    setRGBA(0.5f, 0.5f, 0.5f, 0.9f);
    g->fillRect(0, 0, size.width, size.height, 10);
    setRGBA(1, 1, 1, 1);

    //
    // Stat
    //

    setStringSize(30);
    setStringRGBA(0, 0, 0, 1);
    setStringBorder(0);

    const char* content[3] = {
        "이름",
        "HP",
        "ATK",
    };

    char HeroStr[3][64];
    int cnt = 0;
    int strNum = 0;
    for (int j = 0; str[j]; j++)
    {
        if (str[j] == '/')
        {
            int last = j - strNum;
            memcpy(HeroStr[cnt], &str[strNum], sizeof(char) * last);
            HeroStr[cnt][last] = 0;
            cnt++;
            strNum = j + 1;
        }
    }

    for (int i = 0; i < 3; i++)
    {
        switch (i)
        {
        case 0: g->drawString(size.width - 400, 32 * (i + 1), TOP | LEFT, "[%s] : %s", content[i], HeroStr[i]); break;
        case 1: g->drawString(size.width - 400, 32 * (i + 1), TOP | LEFT, "[%s] : %s", content[i], HeroStr[i]); break;
        case 2: g->drawString(size.width - 400, 32 * (i + 1), TOP | LEFT, "[%s] : %s", content[i], HeroStr[i]); break;
        }
    }

    return g->getTexture();
}
void freePopInven()
{
    delete popInven;
    delete stPopInven;
    delete imgInvenBtn;
}

void drawBeforePopInven(iPopup* pop, float dt, float rate)
{
    for (int i = 0; i < 5; i++)
    {
        imgInvenBtn[i]->setTexObject(popInven->selected == i);
    }
}

void drawPopInven(float dt)
{
    popInven->paint(dt);
}
void showPopInven(bool show)
{
    popInven->show(show);
    if (show)
    {
        //to do...
    }
}

// for Window
#if (OS==OS_WINDOW)
bool keyPopInven(iKeyStat stat, iPoint point)
{
    iPopup* pop = popInven;

    if (pop->bShow == false)
        return false;
    if (pop->stat != iPopupProc)
        return true;

    int j = -1;
    iPoint positionBtn = { devSize.width / 2 + 400 - 25, devSize.height / 2 - 300 - 25 };
    switch (stat)
    {
    case iKeyStatBegan:
        if (pop->selected == -1)
            break;

        if (pop->selected == 0)
        {
            showPopInven(false);
        }
        break;

    case iKeyStatMoved:
        for (int i = 0; i < 5; i++)
        {
            if (containPoint(point, imgInvenBtn[i]->touchRect(pop->closePoint)))
            {
                j = i;
                break;
            }
        }

        if (pop->selected != j)
        {
            pop->selected = j;
            audioPlay(0);
        }
        break;

    case iKeyStatEnded:
        break;
    }

    return true;
}

// for Android
#elif(OS==OS_ANDROID)
bool keyPopInven(iKeyStat stat, iPoint point)
{
    iPopup* pop = popInven;

    if (pop->bShow == false)
        return false;

#if 1 //화면에서 벗어나면 false ////////////////////////////////////////////////////////////////////

#endif ///////////////////////////////////////////////////////////////////////////////////////////

    if (pop->stat != iPopupProc)
        return true;

    int i, j = -1;
    iPoint positionBtn = { devSize.width / 2 + 400 - 25, devSize.height / 2 - 300 - 25 };
    switch (stat)
    {
    case iKeyStatBegan:
        for (i = 0; i < 5; i++)
        {
            if (containPoint(point, imgInvenBtn[i]->touchRect(pop->closePoint, iSizeMake(20, 20))))
            {
                j = i;
                break;
            }
        }

        if (pop->selected != j)
        {
#if 0//#openAL
            audioPlay(0);
#endif
            pop->selected = j;
        }
        break;

    case iKeyStatMoved:
        i = pop->selected;
        if (i == -1) break;
        if (containPoint(point, imgInvenBtn[i]->touchRect(pop->closePoint, iSizeMake(20, 20))) == false)
        {
#if 0 //openAL
            //audioPlay(0);
#endif
            pop->selected = -1;
        }
        break;

    case iKeyStatEnded:
        i = pop->selected;
        if (i == -1) break;
        pop->selected = -1;

        if (i == 0)
        {
            showPopInven(false);
        }
        break;
    }

    return true;
}
#endif

//=========================================================
// popBottomUI : 대화 스크립트
//=========================================================

void createPopBottomUI()
{

}
void freePopBottomUI()
{

}
void drawPopBottomUI(float dt)
{

}
void showPopBottomUI(bool show)
{

}
bool keyPopBottomUI(iKeyStat stat, iPoint point)
{
    return false;
}


//=========================================================
//popOverStep
//=========================================================
iPopup* popOverStep;
iImage** imgOverStepBtn;

void drawPopOverStepBefore(iPopup* pop, float dt, float rate);
void createPopOverStep()
{
    iImage* img;
    Texture* tex;
    int i, j;

    iPopup* pop = new iPopup();

    iGraphics* g = iGraphics::share();
    //
    // bg
    //
    iSize size = iSizeMake(640, 480);
    g->init(size);

    setRGBA(0.5f, 0.5f, 0.5f, 0.8f);
    g->fillRect(0, 0, size.width, size.height, 10);
    setRGBA(1, 1, 1, 1);

    setStringSize(30);
    setStringRGBA(1, 1, 1, 1);
    setStringBorder(2);
    setStringBorderRGBA(0, 0, 0, 1);
    g->drawString(size.width / 2, size.height / 2 - 40, VCENTER | HCENTER, "스텝을 모두 소진하였습니다.");

    setStringSize(50);
    setStringRGBA(1, 0.1f, 0.1f, 1);
    g->drawString(size.width / 2, size.height / 2 + 15, VCENTER | HCENTER, "Game Over");

    tex = g->getTexture();
    img = new iImage();
    img->addObject(tex);
    freeImage(tex);
    pop->addObject(img);

    //
    // btn
    //
    const char* strBtn[1] = {
        "확인"
    };
    iPoint posBtn[1] = {
        {(640 - 200) / 2, 240 + 52}
    };
    imgOverStepBtn = new iImage * [1];

    size = iSizeMake(200, 50);
    for (i = 0; i < 1; i++)
    {
        img = new iImage();
        for (j = 0; j < 2; j++)
        {
            g->init(size);

            if (j == 0)
            {
                setRGBA(0, 0, 0, 1);
                g->fillRect(0, 0, size.width, size.height, 10);
                setRGBA(1, 1, 1, 1);

                setStringSize(40);
                setStringRGBA(1, 1, 1, 1);
                setStringBorder(0);
            }
            else
            {
                setRGBA(1, 1, 0, 1);
                g->fillRect(0, 0, size.width, size.height, 10);
                setRGBA(0.4f, 0.4f, 0.4f, 1);

                setStringSize(40);
                setStringRGBA(0, 0, 0, 1);
                setStringBorder(0);
            }
            g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, strBtn[i]);

            tex = g->getTexture();
            img->addObject(tex);
            freeImage(tex);
        }

        img->position = posBtn[i];
        pop->addObject(img);

        imgOverStepBtn[i] = img;
    }

    pop->style = iPopupZoom;
    pop->openPoint = iPointMake((devSize.width - 640) / 2, (devSize.height - 480) / 2);
    pop->closePoint = iPointMake((devSize.width - 640) / 2, (devSize.height - 480) / 2);
    pop->methodBefore = drawPopOverStepBefore;
    popOverStep = pop;
}

void freePopOverStep()
{
    delete popOverStep;
    delete imgOverStepBtn;
}

void showPopOverStep(bool show)
{
    popOverStep->show(show);
    if (show)
    {
    }
    else
    {

    }
}
void drawPopOverStepBefore(iPopup* pop, float dt, float rate)
{
    for (int i = 0; i < 1; i++)
        imgOverStepBtn[i]->setTexObject(popOverStep->selected == i);
}
void drawPopOverStep(float dt)
{
    popOverStep->paint(dt);
}

#if (OS==OS_WINDOW)
bool keyPopOverStep(iKeyStat stat, iPoint point)
{
    iPopup* pop = popOverStep;

    if (pop->bShow == false)
        return false;

    if (pop->stat != iPopupProc)
        return true;
    int i, j = -1;

    switch (stat) {

    case iKeyStatBegan:
        if (pop->selected == -1) break;
        if (pop->selected == 0)
        {
            setLoading(gs_ending, freeStage, loadEnding);
        }
        else// if (pop->selected == 1)
        {

        }
        break;

    case iKeyStatMoved:
        for (i = 0; i < 1; i++)
        {
            if (containPoint(point, imgOverStepBtn[i]->touchRect(pop->closePoint)))
            {
                j = i;
                break;
            }
        }
        if (pop->selected != j)
        {
#if 0 //#openAL
            audioPlay(0);
#endif
            pop->selected = j;
        }
        break;

    case iKeyStatEnded:
        break;
    }

    return true;
}
#elif (OS==OS_ANDROID)
bool keyPopOverStep(iKeyStat stat, iPoint point)
{
    iPopup* pop = popOverStep;

    if (pop->bShow == false)
        return false;

#if 1 //화면에서 벗어났을때 false ///////////////////////////////////////////////////////////////////

#endif ///////////////////////////////////////////////////////////////////////////////////////////

    if (pop->stat != iPopupProc)
        return true;

    int i, j = -1;

    switch (stat) {

    case iKeyStatBegan:
        for (i = 0; i < 1; i++)
        {
            if (containPoint(point, imgOverStepBtn[i]->touchRect(pop->closePoint, iSizeMake(20, 20))))
            {
                j = i;
                break;
            }
        }
        if (pop->selected != j)
        {
#if 0 //#openAL
            audioPlay(0);
#endif
            pop->selected = j;
        }
        break;

    case iKeyStatMoved:
        i = pop->selected;
        if (i == -1) break;
        if (containPoint(point, imgOverStepBtn[i]->touchRect(pop->closePoint, iSizeMake(20, 20))) == false)
        {
#if 0 //openAL
            audioPlay(0);
#endif
            pop->selected = -1;
        }
        break;

    case iKeyStatEnded:
        i = pop->selected;
        if (i == -1) break;
        pop->selected = -1;

        if (i == 0)
        {
            setLoading(gs_ending, freeStage, loadEnding);
        }
        break;
    }

    return true;
}
#endif

//=========================================================
// popStageStr : 스테이지 글자표시
//=========================================================
iPopup* popStageStr;
iStrTex* stageStr;

Texture* methodStageStr(const char* str);

void createPopStageStr()
{
    iPopup* pop = new iPopup();
    iImage* img = new iImage();
    iStrTex* str = new iStrTex(methodStageStr);
    str->setString("STAGE%d", stageNum);
    img->addObject(str->tex);
    pop->addObject(img);

    pop->openPoint = iPointMake(tileOff.x + TILE_WSIZE, tileOff.y - TILE_HSIZE * 2);
    pop->closePoint = iPointMake(tileOff.x + TILE_WSIZE, tileOff.y - TILE_HSIZE * 2);
    popStageStr = pop;
}

Texture* methodStageStr(const char* str)
{
    iGraphics* g = iGraphics::share();
    iSize size = iSizeMake(120, 40);
    g->init(size);

    setRGBA(0, 0, 0, 1);
    g->fillRect(0, 0, size.width, size.height, 5);
    setRGBA(1, 1, 1, 1);
    g->drawRect(0, 0, size.width, size.height, 5);


    setStringSize(30);
    setStringRGBA(1, 1, 1, 1);
    g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, str);

    return g->getTexture();
}
void freePopStageStr()
{
    delete popStageStr;
}
void showPopStageStr(bool show)
{
    popStageStr->show(show);
    if (show)
    {

    }
    else
    {

    }
}
void drawPopStageStr(float dt)
{
    popStageStr->paint(dt);
}

//=========================================================
// popStepStr : 스테이지 발걸음 표시
//=========================================================
iPopup* popStepStr;
iStrTex* stepStr;
Texture* methodStepStr(const char* str);
void drawPopStepStrBefore(iPopup* pop, float dt, float rate);

void createPopStepStr()
{
    iPopup* pop = new iPopup();
    iImage* img = new iImage();

    //Str
    iStrTex* str = new iStrTex(methodStepStr);
    str->setString("0");
    char _str[20];
    sprintf(_str, "%d", step);
    str->setString(_str);
    img->addObject(str->tex);
    img->position = iPointZero;

    pop->addObject(img);

    stepStr = str;

    pop->methodBefore = drawPopStepStrBefore;
    pop->openPoint = iPointMake(tileOff.x + (TILE_W / 2 + 2) * TILE_WSIZE, tileOff.y - TILE_HSIZE * 2);
    pop->closePoint = iPointMake(tileOff.x + (TILE_W / 2 + 2) * TILE_WSIZE, tileOff.y - TILE_HSIZE * 2);
    popStepStr = pop;
}

Texture* methodStepStr(const char* str)
{
    iGraphics* g = iGraphics::share();
    iSize size = iSizeMake(150, 40);
    g->init(size);

    setRGBA(0, 0, 0, 1);
    g->fillRect(0, 0, size.width, size.height, 5);
    setRGBA(1, 1, 1, 1);
    g->drawRect(0, 0, size.width, size.height, 5);

    setStringSize(30);
    setStringRGBA(1, 1, 1, 1);
    g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, str);

    return g->getTexture();
}

void freePopStepStr()
{
    delete popStepStr;
#if 0
    delete stepStr;
#endif 
}
void showPopStepStr(bool show)
{
    popStepStr->show(show);
    if (show)
    {

    }
    else
    {

    }
}

void drawPopStepStrBefore(iPopup* pop, float dt, float rate)
{
    stepStr->setString("STEP:%d", step);
}

void drawPopStepStr(float dt)
{
    popStepStr->paint(dt);
}

