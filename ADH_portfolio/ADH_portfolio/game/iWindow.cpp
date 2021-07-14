#include "iWindow.h"
#include "iStd.h"
#include "Game.h"
#include "App.h"

void startApp(HDC hdc, VOID_METHOD methodLoad)
{
    // Initialize GDI+.
    devSize.width = DEV_WIDTH;
    devSize.height = DEV_HEIGHT;

    iGraphics::share()->start(hdc);
    loadOpenGL(hdc);
    
    fbo = new iFBO(devSize.width, devSize.height);
    texFboForiPopup = createTexture(devSize.width, devSize.height);
    appInitialize();

    methodLoad(); //loadGame()
}

void drawApp(FLOAT_METHOD methodDraw)
{
    int f = iFPS::instance()->fps();
    float dt = iFPS::instance()->update();

    methodDraw(dt); //drawGame()

    keyDown = 0;

#if 1
    extern void drawCursor(float dt);
    drawCursor(iFPS::instance()->lastDt);
#endif

}

void endApp(VOID_METHOD methodFree)
{
    methodFree(); //freeGame
    iGraphics::share()->end();
    delete fbo;
    freeOpenGL();
}

void resizeApp(int width, int height)
{
    float rx = width / devSize.width;
    float ry = height / devSize.height;
    if (rx < ry)
    {
        viewport.origin.x = 0;
        viewport.size.width = width;

        viewport.size.height = devSize.height * rx;
        viewport.origin.y = (height - viewport.size.height) / 2;
    }
    else
    {
        viewport.origin.y = 0;
        viewport.size.height = height;

        viewport.size.width = devSize.width * ry;
        viewport.origin.x = (width - viewport.size.width) / 2;
    }
}

iPoint coordinate(int x, int y)
{
    float r = devSize.width / viewport.size.width;
    //float r = devSize.height / viewport.size.height;
    iPoint p;
    p.x = (x - viewport.origin.x) * r;
    p.y = (y - viewport.origin.y) * r;

    return p;
}



