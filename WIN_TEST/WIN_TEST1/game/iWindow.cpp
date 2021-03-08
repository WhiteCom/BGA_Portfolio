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
#if 0
    iString s0, s1;
    const char* str = "this\nis\ntest";

    int l_num = 0;
    char** line = iString::getStringLine(str, l_num);

    for (int i = 0; i <= l_num; i++)
    {
        printf("%s\n", line[i]);
    }
#endif

    struct AAA
    {
        const char* name;
        int score;
    };
    AAA aaa[5] = {
        {"a1", 1},
        {"a2", 2},
        {"a3", 3},
        {"a4", 4},
        {"a5", 5}
    };

    iSort s;
    s.init();
    for (int i = 0; i < 5; i++)
    {
        s.add(aaa[i].score);
    }
    s.update();

    for (int i = 0; i < 5; i++)
    {
        AAA* a = &aaa[s.get(i)];
        printf("name : %s, score : %d\n", a->name, a->score);
    }

}

void freeApp()
{

}

void endApp()
{

}

void resizeApp(int width, int height)
{

}

wchar_t* utf8_to_utf16(const char* szFormat, ...)
{
    char szText[1024];
    va_start_end(szText, szFormat);

    int length = MultiByteToWideChar(CP_UTF8, 0, szText, -1, NULL, 0);
    wchar_t* ws = new wchar_t[length];
    MultiByteToWideChar(CP_UTF8, 0, szText, strlen(szText) + 1, ws, length);
    
    return ws;
}

char* utf16_to_utf8(const wchar_t* str)
{
    int length = WideCharToMultiByte(CP_UTF8, 0, str, lstrlenW(str), NULL, 0, NULL, NULL);
    char* s = new char[length];
    WideCharToMultiByte(CP_UTF8, 0, str, lstrlenW(str), s, length, NULL, NULL);

    return s;
}