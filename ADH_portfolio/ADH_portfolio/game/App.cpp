#include "App.h"
#include "Game.h"

#define MAX_LOADSTRING 100

bool runWnd;
bool isFullscreen;
HINSTANCE hInst;        
HWND hWnd;
HDC hDC;
void updateWindow();
WCHAR szTitle[MAX_LOADSTRING] = TEXT("ADH_Portfolio");               
WCHAR szWindowClass[MAX_LOADSTRING] = TEXT("ADH_Portfolio");          

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

void mainLoop();
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.
    
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ADHPORTFOLIO));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_ADHPORTFOLIO);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    RegisterClassExW(&wcex);

    hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    int x = 0, y = 0, w = DEV_WIDTH, h = DEV_HEIGHT;

    hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        x, y, w, h, nullptr, nullptr, hInstance, nullptr);

    if (hWnd == NULL)
    {
        return FALSE;
    }
    hDC = GetDC(hWnd);
    isFullscreen = false;
    loadOpenGL(hDC);

    startApp(hDC, loadGame);
    loadCursor();
    ShowWindow(hWnd, nCmdShow);
    //UpdateWindow(hWnd);
    updateWindow();
    goFullscreen();

    MSG msg;

    // 기본 메시지 루프입니다:
    for(runWnd = true;runWnd;)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            mainLoop();
        }
    }

    //freeOpenGL();
    ReleaseDC(hWnd, hDC);
    freeCursor();
    endApp(freeGame);
    return (int) msg.wParam;
}

void mainLoop()
{
    //readyOpenGL();
fbo->bind();
glViewport(0, 0, devSize.width, devSize.height);
glClearColor(0, 0, 0, 1);
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawApp(drawGame);

fbo->unbind();
glViewport(viewport.origin.x, viewport.origin.y,
    viewport.size.width, viewport.size.height);
glClearColor(0, 0, 0, 1);
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
setGLBlend(GLBlendMultiplied);
setRGBA(1, 1, 1, 1);
Texture* t = fbo->tex;
drawImage(t, 0, 0, TOP | LEFT,
    0, 0, t->width, t->height, 1.0f, 1.0f, 2, 0, REVERSE_HEIGHT);
//drawImage(t, devSize.width, devSize.height, BOTTOM | RIGHT,
//    0, 0, t->width, t->height, 0.3f, 0.3f, 2, 0, REVERSE_HEIGHT);
setGLBlend(GLBlendAlpha);
    SwapBuffers(hDC);
}

iSize sizeMonitor;
static int win_border_width = 0, win_border_height = 0;

void updateWindow()
{
    RECT rect;
    GetWindowRect(hWnd, &rect);

    int w = rect.right - rect.left;
    int h = rect.bottom - rect.top;
    int x = (sizeMonitor.width - w) / 2;
    int y = (sizeMonitor.height - h) / 2;

    SetWindowPos(hWnd, HWND_TOP,
        x, y, w + win_border_width, h + win_border_height, SWP_SHOWWINDOW);
}

#define TEST2 1 ////////////////////////////////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        sizeMonitor = iSizeMake(GetSystemMetrics(SM_CXSCREEN),
                                GetSystemMetrics(SM_CYSCREEN));
        printf("monitor : %.f %.f\n", sizeMonitor.width, sizeMonitor.height);

        RECT rt;
        GetWindowRect(hwnd, &rt); printf("wrt : %d %d %d %d\n", rt.left, rt.top, rt.right, rt.bottom);
        RECT rtWnd = rt;
        GetClientRect(hwnd, &rt); printf("crt : %d %d %d %d\n", rt.left, rt.top, rt.right, rt.bottom);
        win_border_width = (rtWnd.right - rtWnd.left) - (rt.right - rt.left);
        win_border_height = (rtWnd.bottom - rtWnd.top) - (rt.bottom - rt.top);
        //GetSystemMetrics(SM_CXFRAME) * 2;
        //GetSystemMetrics(SM_CYFRAME) * 2; + GetSystemMetrics(SM_CYCAPTION);
        break;
    }
    case WM_GETMINMAXINFO:
    {
        if (win_border_width == 0 || win_border_height == 0)
            break;

        int minWidth = sizeMonitor.width * RATE_MONITOR;
        int minHeight = minWidth * (devSize.height / devSize.width);

        MINMAXINFO* info = (MINMAXINFO*)lParam;
        info->ptMinTrackSize.x = minWidth + win_border_width;
        info->ptMinTrackSize.y = minHeight + win_border_height;
        break;
    }
    
    case WM_SIZING:
    case WM_MOVING:
    {
        //RECT rt;
        //GetWindowRect(hWnd, &rt);
        RECT& rect = *reinterpret_cast<LPRECT>(lParam);
        enforceResolution((int)wParam, rect, win_border_width, win_border_height);
        resizeApp(  rect.right - rect.left - win_border_width,
                    rect.bottom - rect.top - win_border_height);
#if TEST2 //#issue! map editor
        readyOpenGL();
        mainLoop();
#else
        drawApp(drawGame);
#endif
        return 0;
    }
    

    case WM_SIZE:
    {
        resizeApp(LOWORD(lParam), HIWORD(lParam));
#if TEST2 //#issue! map editor
        readyOpenGL();
        mainLoop();
#else
        drawApp(drawGame);
#endif
        return 0;// break;

    }
    case WM_LBUTTONDOWN:
    {
        cursor = coordinate(LOWORD(lParam), HIWORD(lParam));
        keyGame(iKeyStatBegan, cursor);
        return 0;
    }
    case WM_MOUSEMOVE:
    {
        cursor = coordinate(LOWORD(lParam), HIWORD(lParam));
        keyGame(iKeyStatMoved, cursor);
        return 0;
    }

    case WM_LBUTTONUP:
    {
        cursor = coordinate(LOWORD(lParam), HIWORD(lParam));
        keyGame(iKeyStatEnded, cursor);
        return 0;
    }

    case WM_RBUTTONDOWN:
    {
        cursor = coordinate(LOWORD(lParam), HIWORD(lParam));
        keyGame(iKeyStatBegan2, cursor);
        return 0;
    }

    case WM_KEYDOWN:
    {
       
        setKeyStat(iKeyStatBegan, wParam);
        setKeyDown(iKeyStatBegan, wParam);
        return 0;
    }
    case WM_KEYUP:
    {
        if (wParam == VK_ESCAPE)
        {
            goFullscreen();
            return 0;
        }
        setKeyStat(iKeyStatEnded, wParam);
        setKeyDown(iKeyStatEnded, wParam);
        return 0;
    }

    case WM_SETCURSOR:
    {
        
        if (updateCursor(LOWORD(lParam)==HTCLIENT))
            return true;
        break;
    }
    case WM_CLOSE:
    {
        const char* s0[3] =
        {
            "정말 종료하시겠습니까?",
            "I am me",
            "私は灘"
        };

        int language = 0; //kr : 0, en : 1, jp : 2

        wchar_t* ws0 = utf8_to_utf16(s0[language]);
        const wchar_t* ws1 = L"종료";

        if (MessageBox(NULL, ws0, ws1, MB_YESNO) == IDYES)
        {
            runWnd = false;
        }
        
        return 0;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    
    return DefWindowProc(hwnd, message, wParam, lParam);
}

void setCurrentMonitor(RECT& rt)
{
    GetWindowRect(hWnd, &rt);
    HMONITOR hMonitor = MonitorFromRect(&rt, MONITOR_DEFAULTTONEAREST);
    MONITORINFO mi;
    mi.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(hMonitor, &mi);
    if (mi.dwFlags == MONITORINFOF_PRIMARY)
    {
        rt.left = 0;
        rt.right = sizeMonitor.width;
        rt.top = 0;
        rt.bottom = sizeMonitor.height;
    }
    else
    {
        memcpy(&rt, &mi.rcWork, sizeof(RECT));
    }
}

#define CENTER 1 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

RECT rtPrev;
void goFullscreen()
{
    isFullscreen = !isFullscreen;

    if (isFullscreen)
    {
        GetWindowRect(hWnd, &rtPrev);
        //GetClientRect(hWnd, &rtPrev);
        RECT rt;
        setCurrentMonitor(rt);
        int x = rt.left,
            y = rt.top,
            w = rt.right - rt.left,
            h = rt.bottom - rt.top;
        SetWindowLong(hWnd, GWL_STYLE, WS_POPUP);
        SetWindowPos(hWnd, HWND_TOP,
            x, y, w, h,
            SWP_SHOWWINDOW);
    }
    else
    {
#if CENTER
        int w = rtPrev.right - rtPrev.left,
            h = rtPrev.bottom - rtPrev.top;

        int x = (sizeMonitor.width - w) / 2,
            y = (sizeMonitor.height - h) / 2;
#else
        int x = rtPrev.left,
            y = rtPrev.top,
            w = rtPrev.right - rtPrev.left,
            h = rtPrev.bottom - rtPrev.top;
#endif
        SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
        SetWindowPos(hWnd, HWND_TOP,
            x, y, w, h, SWP_SHOWWINDOW);
    }
}
void enforceResolution(int edge, RECT& rect, int win_border_width, int win_border_height)
{
    switch (edge)
    {
    case WMSZ_BOTTOM:
    case WMSZ_TOP:
    {
        //w : h = devSize.width : devSize.height
        int h = (rect.bottom - rect.top) - win_border_height;
        int w = h * devSize.width / devSize.height + win_border_width;
        rect.left = (rect.left + rect.right) / 2 - w / 2;
        rect.right = rect.left + w;

        break;
    }
    case WMSZ_LEFT:
    case WMSZ_RIGHT:
    {
        //w:h = devSize.width : devSize.height
        int w = (rect.right - rect.left) - win_border_width;
        int h = w * devSize.height / devSize.width + win_border_height;
        rect.top = (rect.top + rect.bottom) / 2 - h / 2;
        rect.bottom = rect.top + h;
        
        break;
    }
    case WMSZ_TOPLEFT:
    {
        int w = (rect.right - rect.left) - win_border_width;
        int h = (rect.bottom - rect.top) - win_border_height;
        //if(w/h > devSize.width / devSize.height)
        if (w * devSize.height > h * devSize.width)
        {
            //w : h = devSize.width : devSize.height
            w = h * devSize.width / devSize.height + win_border_width;
            rect.left = rect.right - w;
        }
        else
        {
            //w : h = devSize.width : devSize.height
            h = w * devSize.height / devSize.width + win_border_height;
            rect.top = rect.bottom - h;
        }
        break;
    }
    case WMSZ_TOPRIGHT:
    {
        int w = (rect.right - rect.left) - win_border_width;
        int h = (rect.bottom - rect.top) - win_border_height;

        //if(w/h > devSize.width / devSize.height)
        if (w * devSize.height > h * devSize.width)
        {
            //w : h = devSize.width : devSize.height
            w = h * devSize.width / devSize.height + win_border_width;
            rect.right = rect.left + w;
        }
        else
        {
            //w:h = devSize.width / devSize.height
            h = w * devSize.height / devSize.width + win_border_height;
            rect.top = rect.bottom - h;
        }
        break;
    }
    case WMSZ_BOTTOMLEFT:
    {
        int w = (rect.right - rect.left) - win_border_width;
        int h = (rect.bottom - rect.top) - win_border_height;
        if (w * devSize.height > devSize.width * h)
        {
            //w : h = devSize.width : devSize.height
            w = h * devSize.width / devSize.height + win_border_width;
            rect.left = rect.right - w;
        }
        else
        {
            //w : h = devSize.width : devSize.height
            h = w * devSize.height / devSize.width + win_border_height;
            rect.bottom = rect.top + h;
        }
        break;

    }
    case WMSZ_BOTTOMRIGHT:
    {
        int w = (rect.right - rect.left) - win_border_width;
        int h = (rect.bottom - rect.top) - win_border_height;
        //if(w/h > devSize.width / devSize.height)
        if (w * devSize.height > h * devSize.width)
        {
            //w:h = devSize.width : devSize.height
            w = h * devSize.width / devSize.height + win_border_width;
            rect.right = rect.left + w;
        }
        else
        {
            //w:h = devSize.width : devSize.height
            h = w * devSize.height / devSize.width + win_border_height;
            rect.bottom = rect.top + h;
        }
        break;
    }
    break;
    }
}

Texture* texCursor;
iPoint cursor;
bool bCursor;
void loadCursor()
{
    texCursor = createImage("assets/cursor.png");

    cursor = iPointZero;
    bCursor = false;
}
void freeCursor()
{
    freeImage(texCursor);
}
void drawCursor(float dt)
{
    if (bCursor)
        drawImage(texCursor, cursor.x, cursor.y, TOP | LEFT);
}
bool updateCursor(bool inClient)
{
    if (bCursor == inClient)
        return false;

    bCursor = inClient;
#if 0 
    ShowCursor(bCursor ? FALSE : TRUE);
#else
    if (bCursor)
    {
        for (;;)
        {
            int n = ShowCursor(FALSE);

            if (n < 0) break;
        }
    }
    else
    {
        for (;;)
        {
            int n = ShowCursor(TRUE);
            if (n > -1) break;
        }
    }
#endif
    return true;
}