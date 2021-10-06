#include "native-lib.h"
#include "SyncKey.h"

#include "iNDK.h"

#include "Game.h"

SyncKey* sk;
extern "C" JNIEXPORT void JNICALL
Java_com_adh_lib_Native_loadGame(JNIEnv* env, jobject obj,
                                    jobject ipkFilePath, jobject ioPath){
    xprint("loadGame");
    sk = new SyncKey;

    sk->loadKey();

    //jstringPrint((jstring)ioPath);
    setDevicePath((jstring)ioPath);

    devSize.width = DEV_WIDTH;
    devSize.height = DEV_HEIGHT;

    //iGraphics::share()->start(hdc);
    loadOpenGL();

    appInitialize();

    loadGame();
}
extern "C" JNIEXPORT void JNICALL
Java_com_adh_lib_Native_freeGame(JNIEnv* env, jobject obj){
    //xprint("freeGame");
    sk->freeKey();
}

extern "C" JNIEXPORT void JNICALL
Java_com_adh_lib_Native_drawGame(JNIEnv* env, jobject obj){
    xprint("mainLoop");
    sk->updateKey();
    mainLoop();
}
extern "C" JNIEXPORT void JNICALL
Java_com_adh_lib_Native_keyGame(JNIEnv* env, jobject obj,
                                      jint state, jfloat x, jfloat y){
    //xprint("keyGame(%.2f, %.2f)", x, y);
    float keyX = x, keyY = y;

    if(real_w > devSize.width)
    {
        keyX *= devSize.width / real_w;
        keyY *= devSize.height / real_h;
    }
    sk->insertKey((iKeyStat)state, iPointMake(keyX, keyY));
}

extern "C" JNIEXPORT void JNICALL
Java_com_adh_lib_Native_multiKeyGame(JNIEnv* env, jobject obj,
                                      jint state, jint num, jobject mx, jobject my){
    //xprint("Multi keyGame");
}

extern "C" JNIEXPORT void JNICALL
Java_com_adh_lib_Native_resizeGame(JNIEnv* env, jobject obj,
                                      jint width, jint height){
    resizeApp(width, height);
    readyOpenGL();
    //mainLoop();
}


extern "C" JNIEXPORT void JNICALL
Java_com_adh_lib_Native_pauseGame(JNIEnv* env, jobject obj,
                                         jboolean pause){
    //xprint(pause==0 ? "pause" : "resume");
}

int real_w = 0, real_h = 0;
void resizeApp(int width, int height)
{
    xprint("resizeGame %d, %d", width, height);
#if 0
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
#else
    real_w = width;
    real_h = height;

    viewport.origin = iPointZero;
    viewport.size = iSizeMake(width, height);
#endif
#if 0
    xprint("한글 devSize(%.f,%.f), real(%d, %d), viewport(%.f,%.f,%.f,%.f)\n",
            devSize.width, devSize.height,
            width, height,
            viewport.origin.x, viewport.origin.y, viewport.size.width, viewport.size.height);
#endif
}

void drawApp(FLOAT_METHOD methodDraw)
{
#if 0
    int f = iFPS::instance()->framesPerSec;
#endif
    float dt = iFPS::instance()->update();

#if 1
    if (dt > 0.1f)
        dt = 0.001f;
#endif

    // binding fbo
    methodDraw(dt);// drawGame
#if (OS==OS_WINDOW)
    keyDown = 0;

    // 원래는 App.cpp 56 라인에 있어야 함.
    extern void drawCursor(float dt);
    drawCursor(iFPS::instance()->lastDt);
#endif

    // unbind fbo
    // draw fbo
}

void mainLoop()
{
#if 0// None-FBO
    readyOpenGL();
    drawApp(drawGame);
    //glFlush();// glFinish()
    SwapBuffers(hDC);
#else
    //readyOpenGL();
fbo->bind();
    glViewport(0, 0, devSize.width, devSize.height);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawApp(drawGame);
    // 원래는 여기 있는게 맞음.
    //drawCursor(iFPS::instance()->lastDt);
fbo->unbind();
    glViewport(viewport.origin.x, viewport.origin.y,
               viewport.size.width, viewport.size.height);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    setGLBlend(GLBlendMultiplied);
    setRGBA(1, 1, 1, 1);

    iPoint p = iPointZero;
    float s = updateZoom(iFPS::instance()->lastDt, p);

//drawImage(fbo->tex, 0, 0, TOP | LEFT);
    Texture* t = fbo->tex;
    drawImage(t, p.x, p.y, TOP | LEFT,
              0, 0, t->width, t->height, s, s, 2, 0, REVERSE_HEIGHT);
//drawImage(t, devSize.width, devSize.height, BOTTOM | RIGHT,
//    0, 0, t->width, t->height, 0.3f, 0.3f, 2, 0, REVERSE_HEIGHT);
    setGLBlend(GLBlendAlpha);
    //glFlush();// glFinish()

#if (OS==OS_WINDOW)
    SwapBuffers(hDC);
#endif

#endif
}
