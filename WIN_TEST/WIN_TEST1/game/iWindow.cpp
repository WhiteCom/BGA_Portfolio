#include "iWindow.h"
#include "iStd.h"

void startApp()
{

}

void drawApp()
{
    int f = iFPS::instance()->fps();
    float dt = iFPS::instance()->update();
    //if (dt > 0.001f)
    //    printf("%f %d\n", dt, f);


}

void freeApp()
{

}

void endApp()
{

}