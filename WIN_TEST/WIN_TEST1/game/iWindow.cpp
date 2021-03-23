#include "iWindow.h"
#include "iStd.h"
#include "Loading.h"

static Texture* texBack;
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

    Bitmap* bmp = new Bitmap(devSize.width, devSize.height, PixelFormat32bppPARGB);
    Graphics* g = Graphics::FromImage(bmp);
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

    Texture* t = new Texture;
    t->texID = bmp;
    t->width = bmp->GetWidth();
    t->height = bmp->GetHeight();
    t->potWidth = t->width;
    t->potHeight = t->height;
    t->retainCount = 1;
    texBack = t;

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

void makeTile()
{
    static Texture* tex = createImage("assets/Image/tile1.bmp");

    iRect tile;
    int W = 16;
    int H = 12;
    int tileIndex[16 * 12] = {
        1, 8, 1, 1, 1, 1, 8, 8, 8, 1, 1, 8, 1, 1, 1, 1, 
        1, 8, 1, 1, 1, 1, 8, 8, 8, 1, 1, 8, 1, 1, 1, 1,
        1, 8, 1, 1, 1, 1, 8, 8, 8, 1, 1, 8, 1, 1, 1, 1,
        1, 8, 1, 1, 1, 1, 8, 8, 8, 1, 1, 8, 1, 1, 1, 1,
        1, 8, 1, 1, 1, 1, 8, 8, 8, 1, 1, 8, 1, 1, 1, 1,
        1, 8, 1, 1, 1, 1, 8, 8, 8, 1, 1, 8, 1, 1, 1, 1,
        1, 8, 1, 1, 1, 1, 8, 8, 8, 1, 1, 8, 1, 1, 1, 1,
        1, 8, 1, 1, 1, 1, 8, 8, 8, 1, 1, 8, 1, 1, 1, 1,
        1, 8, 1, 1, 1, 1, 8, 8, 8, 1, 1, 8, 1, 1, 1, 1,
        1, 8, 1, 1, 1, 1, 8, 8, 8, 1, 1, 8, 1, 1, 1, 1,
    };

    for (int j = 0; j < tileH; j++)
    {
        for (int i = 0; i < tileW; i++)
        {
            tile = iRectMake(32*i + 32, 32*j + 32, 32-1, 32-1);
            setRGBA(1, 0, 0, 1);
            drawRect(tile);
            //setRGBA(0, 1, 0, 1);
            //fillRect(tile);

            //graphics->SetClip(Rect(32 * i, 32 * j, 32, 32));
            int ti = tileIndex[j * 10 + i];
            int x = ti%8, y = ti/8;
            //drawImage(tex, tile.origin.x - 32 * x, tile.origin.y - 32 * y, TOP | LEFT);
            //graphics->SetClip(Rect(0, 0, devSize.width, devSize.height));
            drawImage(tex, 32 * i + 32, 32 * j + 32, TOP | LEFT, 32 * x, 32 * y, 32, 32, 1.0f, 1.0f, 2, 0);
        }
        
    }
}

void drawApp(FLOAT_METHOD m)
{
    int f = iFPS::instance()->fps();
    float dt = iFPS::instance()->update();
    //if (dt > 0.001f)
    //    printf("%f %d\n", dt, f); 

    graphics = graphicsFromBmp;
    //m(dt);

    Texture* bg = createImage("assets/map.jpg");

    setRGBA(0.5f, 0.5f, 0.5f, 1.0f);
    clearRect();
    drawImage(bg, 0, 0, TOP | LEFT);

    freeImage(bg);

    //makeTile();
    TileMap t;

    //index는 typeNum보다 적게
    int index = 0;
    t.setTile(index, "field");
    t.setTile(++index, "wall");
    t.drawTile();

#if 1
    extern void drawCursor(float dt);
    drawCursor(iFPS::instance()->lastDt);
#endif
    graphics = graphicsFromHDC;
    drawImage(texBack, viewport.origin.x, viewport.origin.y, TOP | LEFT,
        0, 0, texBack->width, texBack->height,
        viewport.size.width / texBack->width, viewport.size.height / texBack->height, 2, 0);
    if (getKeyDown() & keysSpace)
    {
        setLoading(gs_loading, NULL, NULL);
        drawLoading(dt);
    }

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
    delete (Bitmap*)texBack->texID;
    delete texBack;
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
    float r = devSize.width / viewport.size.width;
    //float r = devSize.height / viewport.size.height;
    iPoint p;
    p.x = (x - viewport.origin.x) * r;
    p.y = (y - viewport.origin.y) * r;

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