#ifndef UNICODE 
#define UNICODE
#endif 

#include <windows.h>

#include <string>
#include <fstream>
#include <streambuf>
#include <vector>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HWND hwndMain = NULL;
HWND hwndEdit = NULL;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[]  = L"Sample Window Class";
    WNDCLASS wc = { };
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);

    RECT rect;
    HWND taskBar = FindWindow(L"Shell_traywnd", NULL);
    GetWindowRect(taskBar, &rect);

    hwndMain = CreateWindowEx(WS_EX_TOOLWINDOW | WS_EX_LAYERED, wc.lpszClassName, 0, WS_POPUP | WS_VISIBLE | WS_SYSMENU, 0, 3 * rect.top / 4, 300, rect.top / 4, 0, 0, 0, 0);
    hwndEdit = CreateWindowEx(0, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL, 0, 0, 300, rect.top / 4, hwndMain, 0, (HINSTANCE) GetWindowLong(hwndMain, GWL_HINSTANCE), NULL);      
    SetLayeredWindowAttributes(hwndMain, 0, 192, LWA_ALPHA);
    ShowWindow(hwndMain, nCmdShow);

    std::ifstream t("neverforget.txt", std::ios::in  | std::ios::binary);
    std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

    SendMessageA(hwndEdit, WM_SETTEXT, 0, (LPARAM) str.c_str()); 

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_DESTROY:
        {
            int len = GetWindowTextLengthA(hwndEdit);
            std::vector<char> text(len+1);
            GetWindowTextA(hwndEdit, &text[0], len+1);
            std::ofstream file("neverforget.txt", std::ios::out  | std::ios::binary);
            file.write(&text[0], len);
            file.close();
            PostQuitMessage(0);
            return 0;
        } 

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            HBRUSH hBrush = CreateSolidBrush(RGB(255,255,0));
            FillRect(hdc, &ps.rcPaint, hBrush);
            EndPaint(hwnd, &ps);
            return 0;            
        }

        case WM_WINDOWPOSCHANGING:
        {
            ShowWindow(hwndMain, SW_SHOW);
            return 0;  
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
