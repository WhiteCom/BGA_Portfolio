#include "iWindow.h"
#include "iStd.h"

void startApp()
{

}

void freeArray(void* data)
{
    char* s = (char*)data;
    printf("%s\n", s);
    delete s;
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
#if 0
    struct AAA
    {
        const char* name;
        int score;
    };
    AAA aaa[5] = {
        {"en", 50},
        {"ger", 80},
        {"jp", 90},
        {"chi", 85},
        {"Fre", 60}
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
        printf("%s %d\n", a->name, a->score);
    }
    printf("-----\n");
#endif

    iArray* a = new iArray(freeArray);

    const char* str[3] = {
        "Hello", "Game", "World"
    };

    for (int i = 0; i < 3; i++)
    {
        char* s = new char[64];
        strcpy(s, str[i]);
        a->addObject(s);
    }
    char *szText = (char*)"Game";
    a->removeData(szText);
    printf("--%s--\n", (char*)a->objectAtIndex(1));

    delete a;
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