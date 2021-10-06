#pragma once

#include "iType.h"

#include <jni.h>
#include <string>
#include <android/log.h>

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#define logv(...) __android_log_print(ANDROID_LOG_VERBOSE , "ADH", __VA_ARGS__)
#define logd(...) __android_log_print(ANDROID_LOG_DEBUG , "ADH", __VA_ARGS__)
#define logi(...) __android_log_print(ANDROID_LOG_INFO , "ADH", __VA_ARGS__)
#define logw(...) __android_log_print(ANDROID_LOG_WARN , "ADH", __VA_ARGS__)
#define loge(...) __android_log_print(ANDROID_LOG_ERROR, "ADH", __VA_ARGS__)
#define xprint loge

JNIEnv* getEnv();
jobject getMain();

jobject ndkLocal2global(jobject obj);

// ----------------------------------------------------
// 문자열
// ----------------------------------------------------

// java : String, char, byte
// c/c++: char
#if 1//////////////////////////////////////////////////////////////////
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Platform.h>

#include <SLES/OpenSLES_Android.h>
#include <SLES/OpenSLES_AndroidConfiguration.h>

//
#endif ////////////////////////////////////////////////////////////////

char* jbyteArray2cstr(JNIEnv* env, jbyteArray javaBytes);
char* jstring2cstr(JNIEnv* env, jstring jstr);
jbyteArray cstr2byteArray(JNIEnv* env, const char* cstr);
jstring javaNewStringEncoding(JNIEnv* env, jbyteArray javaBytes, const char* encoding);
jstring javaNewStringChar(JNIEnv* env, const char* cstr);
jint jstringLength(JNIEnv* env, jstring str);
jstring  jstringSubstring(JNIEnv* env, jstring, int start, int end);
void jstringPrint(jstring jstr);

// ----------------------------------------------------
// File IO
// ----------------------------------------------------
char* getStream(const char* fileName, int& length);

void setDevicePath(jstring path);
const char* getDevicePath();

char* loadFile(const char* fileName, int& length);
void saveFile(const char* fileName, char* buf, int bufLength);

// ----------------------------------------------------
// vs Bitmap(android.util.Bitmap Native)
// ----------------------------------------------------
jobject ndkCreateBitmap(const char* fileName);// 디바이스 저장소에 있는 이미지(논 압축)
jobject ndkCreateBitmap(JNIEnv* env, int width, int height, const char* config = "ARGB_8888");
jobject ndkCreateBitmap(char* buf, int length);
void ndkFreeBitmap(jobject bitmap);

Texture* ndkCreateImage(jobject bitmap, int BgChk = 0);
Texture* ndkCreateImage(const char* fileName, int BgChk = 0);
Texture** ndkCreateImageDevide(int numX, int numY, const char* fileName, int BgChk = 0);

iSize ndkGetBitmapSize(jobject bitmap);

void ndkDrawBitmap(jobject canvas, jobject paint, jobject bitmap,
                   float x, float y, float width, float height, int anc);
void ndkDrawBitmap(jobject canvas, jobject paint, jobject bitmap,
                   float x, float y, float width, float height, int anc,
                   float ix, float iy, float iw, float ih);


jobject ndkNewCanvas(JNIEnv* env, jobject bitmap);
jobject ndkNewPaint(JNIEnv* env);
void ndkSetStringName(jobject paint, const char* fileName);
void ndkDrawARGB(jobject canvas, int a, int r, int g, int b);

void ndkSetAntialias(jobject paint, bool anti);
void ndkSetTextSize(jobject paint, float stringSize);
void ndkSetTextScaleX(jobject paint, float scaleX);
void ndkSetARGB(jobject paint, int a, int r, int g, int b);
void ndkSetStrokeWidth(jobject paint, float lineWidth);
void ndkDrawLine(jobject canvas, jobject paint, float x0, float y0, float x1, float y1);

void ndkSetPaintStyle(jobject paint, const char* strType);
void ndkDrawRect(jobject canvas, jobject paint, float x, float y, float width, float height, float radius);

jobject ndkCreateRect(int x, int y, int w, int h);
jobject ndkCreateRect();
iRect ndkGetRect(jobject rt);
iRect ndkRectOfString(jobject paint, const char* str);
void ndkDrawChar(jobject canvas, const char* szText, float x, float y, jobject paint);

// ----------------------------------------------------
// Activity method
// ----------------------------------------------------
void ndkShutDownApp();