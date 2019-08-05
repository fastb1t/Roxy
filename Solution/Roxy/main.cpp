#include "Application.h"
#include "Widget.h"

// TranslateMessage - https://doxygen.reactos.org/d4/d8c/win32ss_2user_2user32_2windows_2message_8c.html#a601d5db3c8d8100630fe50b98b0451d0
// TranslateMessageEx - https://doxygen.reactos.org/d4/d8c/win32ss_2user_2user32_2windows_2message_8c.html#ada18e6199f2f59ad0b6f8379ebb10bb9
// DispatchMessageW - https://doxygen.reactos.org/d4/d8c/win32ss_2user_2user32_2windows_2message_8c.html#ae957857aee887b2c5957b144a0b37c4d
// DispathcMessageA - https://doxygen.reactos.org/d4/d8c/win32ss_2user_2user32_2windows_2message_8c.html#a9858a588c2ee36f8f0e181a6843cbba4
// DummyWindowProcedure - http://www.cyberforum.ru/win-api/thread1074072.html

void btn_pressed(Widget* widget)
{
    // BTN_PRESSED_PROC pressed = [](Widget *widget2) -> void
    // BTN_PRESSED_PROC pressed = [](Widget *widget2)
    // auto pressed = [](Widget *widget2) -> void
    auto pressed = [](Widget* widget2)
    {
        widget2->messageBox(_T("Message #1"), _T("Title"), MB_OK | MB_ICONINFORMATION | MB_TOPMOST);
    };

    widget->addPushButton(_T("button #4"), 10, 100, 100, 20, 1001, pressed);

    widget->addPushButton(_T("button #5"), 10, 130, 100, 20, 1002, [](Widget* widget2) {
        widget2->messageBox(_T("Message #2"), _T("Title"), MB_OK | MB_ICONINFORMATION | MB_TOPMOST);
    });
}

LRESULT OnCreate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    Widget* widget = GetWidget(hWnd);
    if (widget)
    {
        widget->addPushButton(_T("Button #1"), 10, 10, 100, 20, 1000, btn_pressed);
        widget->addPushButton(_T("Button #2"), 10, 40, 100, 20, -1);
        widget->addPushButton(_T("Button #3"), 10, 70, 100, 20, -1);
    }
    return TRUE;
}

LRESULT OnPaint(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hDC = BeginPaint(hWnd, &ps);
    FillRect(hDC, &ps.rcPaint, (HBRUSH)GetStockObject(GRAY_BRUSH));
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
    Application app;

    Widget widget1;
    widget1.setGeometry(10, 10, 400, 300);

    widget1.addMsgProc(WM_CREATE, OnCreate);
    widget1.addMsgProc(WM_PAINT, OnPaint);
    widget1.addMsgProc(WM_DESTROY, OnDestroy);

    widget1.createWindow();
    widget1.show();

    Widget widget2(_T("Widget #2"));
    widget2.setGeometry(420, 10, 400, 300);

    widget2.addMsgProc(WM_CREATE, OnCreate);
    widget2.addMsgProc(WM_DESTROY, OnDestroy);

    widget2.createWindow();
    widget2.show();

    return app.execute();
}
