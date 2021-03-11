#include "iWindow.h"
#include "iStd.h"

static Bitmap* bmpBack;
Graphics* graphicsFromBmp;
Graphics* graphicsFromHDC;
Graphics* graphics;

ULONG_PTR startApp(HDC hdc, VOID_METHOD m)
{
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;

    // Initialize GDI+.
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    devSize.width = DEV_WIDTH;
    devSize.height = DEV_HEIGHT;

    bmpBack = new Bitmap(devSize.width, devSize.height, PixelFormat32bppPARGB);
    Graphics* g = Graphics::FromImage(bmpBack);
    g->SetPageUnit(UnitPixel);
    //g->SetPageScale(1.0f);
    g->SetPixelOffsetMode(PixelOffsetModeHalf);
    g->SetTextRenderingHint(TextRenderingHintClearTypeGridFit);
    g->SetTextContrast(0xffffffff);
    g->SetCompositingMode(CompositingModeSourceOver);
    g->SetCompositingQuality(CompositingQualityAssumeLinear);
    g->SetSmoothingMode(SmoothingModeAntiAlias8x8);
    g->SetInterpolationMode(InterpolationModeHighQualityBicubic);
    graphicsFromBmp = g;

    graphicsFromHDC = new Graphics(hdc);

    graphics = graphicsFromBmp;

    return gdiplusToken;
}

void freeArray(void* data)
{
    char* s = (char*)data;
    printf("%s\n", s);
    delete s;
}

void drawApp(FLOAT_METHOD m)
{
    int f = iFPS::instance()->fps();
    float dt = iFPS::instance()->update();
    //if (dt > 0.001f)
    //    printf("%f %d\n", dt, f); 

    graphics = graphicsFromBmp;
    //m(dt);
    igImage* bg = createImage("assets/intro.jpg");
    
    setRGBA(0.5f, 0.5f, 0.5f, 1.0f);
    clearRect();
    drawImage(bg, 0, 0, TOP | LEFT);

    freeImage(bg);

    float w = igImageWidth(bmpBack);
    float h = igImageHeight(bmpBack);
    graphics = graphicsFromHDC;
    drawImage(bmpBack, viewport.origin.x, viewport.origin.y, TOP | LEFT,
        0, 0, w, h,
        viewport.size.width / w, viewport.size.height / h, 2, 0);

    extern HDC hdc;
    SwapBuffers(hdc);
}

void freeApp()
{

}

void endApp(ULONG_PTR token, VOID_METHOD m)
{
    //m();
    //delete graphics;
    delete graphicsFromBmp;
    delete bmpBack;
    delete graphicsFromHDC;
    GdiplusShutdown(token);
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

void clearApp()
{
    graphics = graphicsFromHDC;
    setRGBA(0, 0, 0, 1);
    clearRect();
}

iPoint coordinate(int x, int y)
{
    iPoint p = iPointZero;
    return p;
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