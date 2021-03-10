#include "WIN_TEST1.h"

#define MAX_LOADSTRING 100

bool runWnd = false;
HINSTANCE hInst;        
HWND hWnd;
HDC hdc;
WCHAR szTitle[MAX_LOADSTRING] = TEXT("WIN_TEST");               
WCHAR szWindowClass[MAX_LOADSTRING] = TEXT("WIN_TEST2");          



LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

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
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINTEST1));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINTEST1);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    RegisterClassExW(&wcex);

    hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (hWnd == NULL)
    {
        return FALSE;
    }
    hdc = GetDC(hWnd);

    //#NEED UPDATE
    ULONG_PTR token = startApp(hdc, NULL);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

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
            //#NEED UPDATE
            drawApp(NULL);
        }
    }
    //#NEDD UPDATE
    endApp(token, NULL);
    return (int) msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {

        return 0;
    }
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
            return 0;
        }
    case WM_KEYDOWN:
    {
        setKeyDown(iKeyStatBegan, wParam);
        return 0;
    }
    case WM_KEYUP:
    {
        setKeyDown(iKeyStatEnded, wParam);
        return 0;
    }
    case WM_CLOSE:
    {
        const char* s0[3] =
        {
            "나는 나다.",
            "I am me",
            "私は灘"
        };

        int language = 0; //kr : 0, en : 1, jp : 2

        wchar_t* ws0 = utf8_to_utf16(s0[language]);
        const wchar_t* ws1 = L"私は灘";

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
    
    return DefWindowProc(hWnd, message, wParam, lParam);
}
