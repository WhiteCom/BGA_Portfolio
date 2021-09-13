#include "MapEditorUI.h"
#include "MapEditor.h"

//================================================
//popLoad
//================================================
void drawPopLoadBefore(iPopup* pop, float dt, float rate);

iPopup* popLoad;
iImage** imgLoadBtn;
void createPopLoad()
{
    iImage* img;
    Texture* tex;
    int i, j;

    iPopup* pop = new iPopup();

    iGraphics* g = iGraphics::share();

    //bg

    iSize size = iSizeMake(320, 240);

    g->init(size);
    setRGBA(1, 0, 1, 0.5f);
    g->fillRect(0, 0, size.width, size.height, 10);
    setRGBA(1, 1, 1, 1);

    setStringSize(30);
    setStringRGBA(1, 1, 1, 1);
    setStringBorder(1);
    setStringBorderRGBA(0, 0, 0, 1);
    g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "로드 완료");

    tex = g->getTexture();
    img = new iImage();
    img->addObject(tex);
    freeImage(tex);
    pop->addObject(img);

    //btn

    const char* loadBtn[1] = {
        "확인"
    };

    iPoint posBtn[1] = {
        {(size.width - 160) / 2, (size.height + 80) / 2},
    };
    imgLoadBtn = new iImage * [1];

    size = iSizeMake(150, 40);
    for (i = 0; i < 1; i++)
    {
        img = new iImage();
        for (j = 0; j < 2; j++)
        {
            g->init(size);

            if (j == 0) setRGBA(0, 0, 0, 1);
            else     setRGBA(1, 1, 0, 1);
            g->fillRect(0, 0, size.width, size.height, 10);
            setRGBA(1, 1, 1, 1);

            setStringSize(30);
            setStringRGBA(1, 1, 1, 1);
            setStringBorder(1);
            setStringBorderRGBA(0, 0, 0, 1);
            g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, loadBtn[i]);

            tex = g->getTexture();
            img->addObject(tex);
            freeImage(tex);
        }
        img->position = posBtn[i];
        pop->addObject(img);

        imgLoadBtn[i] = img;
    }

    pop->style = iPopupZoom;
    pop->openPoint = iPointMake(devSize.width / 2, devSize.height / 2);
    pop->closePoint = iPointMake((devSize.width - 320) / 2, (devSize.height - 240) / 2);
    pop->methodBefore = drawPopLoadBefore;
    popLoad = pop;
}
void freePopLoad()
{
    delete popLoad;
    delete imgLoadBtn;
}
void showPopLoad(bool show)
{
    popLoad->show(show);
}

void drawPopLoadBefore(iPopup* pop, float dt, float rate)
{
    for (int i = 0; i < 1; i++)
        imgLoadBtn[i]->setTexObject(popLoad->selected == i);
}

void drawPopLoad(float dt)
{
    popLoad->paint(dt);
}
bool keyPopLoad(iKeyStat stat, iPoint point)
{
    if (popLoad->bShow == false || popLoad->stat != iPopupProc)
        return false;

    int i, j = -1;

    switch (stat)
    {
    case iKeyStatBegan:
        if (popLoad->selected == -1) break;
        if (popLoad->selected == 0)
        {
            showPopLoad(false);
        }
        else //if(popLoad->selected == 1)
        {

        }
        break;
    case iKeyStatMoved:
        for (i = 0; i < 1; i++)
        {
            if (containPoint(point, imgLoadBtn[i]->touchRect(popLoad->closePoint)))
            {
                j = i;
                break;
            }

        }
        if (popLoad->selected != j)
        {
            popLoad->selected = j;
        }
        break;
    case iKeyStatEnded:
        break;

    }
    return true;
}

//================================================
//popSave
//================================================
void drawPopSaveBefore(iPopup* pop, float dt, float rate);

iPopup* popSave;
iImage** imgSaveBtn;

void createPopSave()
{
    iImage* img;
    Texture* tex;
    int i, j;
    iPopup* pop = new iPopup();

    iGraphics* g = iGraphics::share();

    //bg

    iSize size = iSizeMake(320, 240);

    g->init(size);
    setRGBA(0, 1, 1, 0.5f);
    g->fillRect(0, 0, size.width, size.height, 10);
    setRGBA(1, 1, 1, 1);

    setStringSize(30);
    setStringRGBA(1, 1, 1, 1);
    setStringBorder(1);
    setStringBorderRGBA(0, 0, 0, 1);
    g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "저장 완료");

    tex = g->getTexture();
    img = new iImage();
    img->addObject(tex);
    freeImage(tex);
    pop->addObject(img);

    //btn

    const char* saveBtn[1] = {
        "확인",
    };

    iPoint posBtn[1] = {
        {(size.width - 160) / 2, (size.height + 80) / 2},
    };
    imgSaveBtn = new iImage * [1];

    size = iSizeMake(150, 40);

    for (i = 0; i < 1; i++)
    {
        img = new iImage();
        for (j = 0; j < 2; j++)
        {
            g->init(size);

            if (j == 0) setRGBA(0, 0, 0, 1);
            else        setRGBA(1, 1, 0, 1);
            g->fillRect(0, 0, size.width, size.height, 10);
            setRGBA(1, 1, 1, 1);

            setStringSize(30);
            setStringRGBA(1, 1, 1, 1);
            setStringBorder(1);
            setStringBorderRGBA(0, 0, 0, 1);
            g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, saveBtn[i]);

            tex = g->getTexture();
            img->addObject(tex);
            freeImage(tex);
        }
        img->position = posBtn[i];
        pop->addObject(img);

        imgSaveBtn[i] = img;
    }

    pop->style = iPopupZoom;
    pop->openPoint = iPointMake(devSize.width / 2, devSize.height / 2);
    pop->closePoint = iPointMake((devSize.width - 320) / 2, (devSize.height - 240) / 2);
    pop->methodBefore = drawPopSaveBefore;
    popSave = pop;
}
void freePopSave()
{
    delete popSave;
    delete imgSaveBtn;
}
void showPopSave(bool show)
{
    popSave->show(show);
}
void drawPopSaveBefore(iPopup* pop, float dt, float rate)
{
    for (int i = 0; i < 1; i++)
        imgSaveBtn[i]->setTexObject(popSave->selected == i);
}
void drawPopSave(float dt)
{
    popSave->paint(dt);
}

bool keyPopSave(iKeyStat stat, iPoint point)
{
    if (popSave->bShow == false || popSave->stat != iPopupProc)
        return false;

    int i, j = -1;

    switch (stat)
    {
    case iKeyStatBegan:
        if (popSave->selected == -1) break;
        if (popSave->selected == 0)
        {
            showPopSave(false);
        }
        else
        {

        }
        break;
    case iKeyStatMoved:
        for (i = 0; i < 1; i++)
        {
            if (containPoint(point, imgSaveBtn[i]->touchRect(popSave->closePoint)))
            {
                j = i;
                break;
            }
        }
        if (popSave->selected != j)
        {
            popSave->selected = j;
        }
        break;
    case iKeyStatEnded:
        break;
    }
    return true;
}