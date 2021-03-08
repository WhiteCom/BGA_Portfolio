#pragma once

#include "iFPS.h"

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


#define va_start_end(szText, szFormat) \
va_list args; \
va_start(args, szFormat); \
vsnprintf(szText, sizeof(szText), szFormat, args); \
va_end(args)