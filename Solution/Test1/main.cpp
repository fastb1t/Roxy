#include "../Roxy/Roxy/Application.h"
#include "../Roxy/Roxy/Window.h"
#include "../Roxy/Roxy/Enumerator.h"
#include "../Roxy/Roxy/Surface.h"

#include <iostream>

#pragma comment(lib, "../Debug/Roxy.lib")

// TranslateMessage - https://doxygen.reactos.org/d4/d8c/win32ss_2user_2user32_2windows_2message_8c.html#a601d5db3c8d8100630fe50b98b0451d0
// TranslateMessageEx - https://doxygen.reactos.org/d4/d8c/win32ss_2user_2user32_2windows_2message_8c.html#ada18e6199f2f59ad0b6f8379ebb10bb9
// DispatchMessageW - https://doxygen.reactos.org/d4/d8c/win32ss_2user_2user32_2windows_2message_8c.html#ae957857aee887b2c5957b144a0b37c4d
// DispathcMessageA - https://doxygen.reactos.org/d4/d8c/win32ss_2user_2user32_2windows_2message_8c.html#a9858a588c2ee36f8f0e181a6843cbba4
// DummyWindowProcedure - http://www.cyberforum.ru/win-api/thread1074072.html

void btn_pressed(Roxy::Window* window)
{
    // BTN_PRESSED_PROC pressed = [](Widget *widget2) -> void
    // BTN_PRESSED_PROC pressed = [](Widget *widget2)
    // auto pressed = [](Widget *widget2) -> void
    auto pressed = [](Roxy::Window* window2)
    {
        window2->messageBox(_T("Message #1"), _T("Title"), MB_OK | MB_ICONINFORMATION | MB_TOPMOST);
    };

    window->addPushButton(_T("button #4"), 10, 100, 100, 20, 1001, pressed);

    window->addPushButton(_T("button #5"), 10, 130, 100, 20, 1002, [](Roxy::Window* window2) {
        window2->messageBox(_T("Message #2"), _T("Title"), MB_OK | MB_ICONINFORMATION | MB_TOPMOST);
        });
}

LRESULT OnCreate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    Roxy::Window* window = Roxy::GetWindowPtrFromHandle(hWnd);
    if (window)
    {
        window->addPushButton(_T("Button #1"), 10, 10, 100, 20, 1000, btn_pressed);
        window->addPushButton(_T("Button #2"), 10, 40, 100, 20, -1);
        window->addPushButton(_T("Button #3"), 10, 70, 100, 20, -1);
    }
    return TRUE;
}

LRESULT OnPaint(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hDC = BeginPaint(hWnd, &ps);
    FillRect(hDC, &ps.rcPaint, (HBRUSH)GetStockObject(GRAY_BRUSH));

    Roxy::Surface surface;
    surface.New(100, 100);
    surface.DrawLine(10, 10, 90, 90);
    surface.Draw(hDC, 10, 100);
    surface.Delete();

    EndPaint(hWnd, &ps);
    return DefWindowProc(hWnd, WM_PAINT, wParam, lParam);
}

LRESULT OnDestroy(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    PostQuitMessage(0);
    return DefWindowProc(hWnd, WM_DESTROY, wParam, lParam);
}

int main(int argc, char* argv[])
{
    Roxy::Application app;

    Roxy::Window window1;

    window1.setGeometry(10, 10, 400, 300);

    window1.addMsgProc(WM_CREATE, OnCreate);
    window1.addMsgProc(WM_PAINT, OnPaint);
    window1.addMsgProc(WM_DESTROY, OnDestroy);

    window1.create();
    window1.show();

    Roxy::Window window2(_T("Widget #2"));

    window2.setGeometry(420, 10, 400, 300);

    window2.addMsgProc(WM_CREATE, OnCreate);
    window2.addMsgProc(WM_DESTROY, OnDestroy);

    window2.create();
    window2.show();

    return app.loop();
}
