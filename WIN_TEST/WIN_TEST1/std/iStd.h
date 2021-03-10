#pragma once

#include "iType.h"

#include "iFPS.h"
#include "iString.h"
#include "iPoint.h"
#include "iSort.h"
#include "iArray.h"
#include "iSize.h"
#include "iRect.h"

// C 런타임 헤더 파일입니다.
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#define DEV_WIDTH 1980
#define DEV_HEIGHT 1080
extern iSize devSize;
extern iRect viewport;

#define va_start_end(szText, szFormat) \
va_list args; \
va_start(args, szFormat); \
vsnprintf(szText, sizeof(szText), szFormat, args); \
va_end(args)

void setKeyDown(iKeyStat stat, int c);
int getKeyDown();

void appInitialize();
int random();

float _cos(float degree);
float _sin(float degree);