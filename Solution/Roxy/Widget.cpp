#include "Widget.h"

// [DummyWindowProcedure]:
static LRESULT CALLBACK DummyWindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    Roxy::Widget* widget = NULL;

    if (message == WM_CREATE)
    {
        LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
        if (lpcs && lpcs->lpCreateParams)
        {
            widget = reinterpret_cast<Roxy::Widget*>(lpcs->lpCreateParams);
            if (widget)
            {
                SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR)widget);
                widget->setWindowHandle(hWnd);
            }
        }
    }

    widget = reinterpret_cast<Roxy::Widget*>(GetWindowLongPtr(hWnd, GWL_USERDATA));
    if (widget)
    {
        widget->Process_ButtonPressed(hWnd, message, wParam, lParam);
        return widget->WindowProcedure(hWnd, message, wParam, lParam);
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}
// [/DummyWindowProcedure]


// [Roxy::Widget::Widget]:
Roxy::Widget::Widget(String title) :
    hInstance(GetModuleHandle(NULL)),
    hWnd(NULL),
    point({ CW_USEDEFAULT, CW_USEDEFAULT }),
    size({ 700, 500 }),
    window_title(title),
    window_class_name(_T("")),
    bWidnowClassIsRegistered(false),
    error(_T(""))
{

}
// [/Roxy::Widget::Widget]


// [Roxy::Widget::~Widget]:
Roxy::Widget::~Widget()
{
    if (bWidnowClassIsRegistered)
    {
        UnregisterClass(window_class_name.c_str(), hInstance);
    }
}
// [/Roxy::Widget::~Widget]


// [Roxy::Widget::setWindowHandle]:
void Roxy::Widget::setWindowHandle(HWND handle)
{
    hWnd = handle;
}
// [/Roxy::Widget::setWindowHandle]


// [Roxy::Widget::WindowProcedure]:
LRESULT Roxy::Widget::WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    std::map<UINT, MSG_PROC>::iterator it = msg_proc_map.find(message);
    if (it != msg_proc_map.end())
    {
        return it->second(hWnd, wParam, lParam);
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}
// [/Roxy::Widget::WindowProcedure]


// [Roxy::Widget::Process_ButtonPressed]:
void Roxy::Widget::Process_ButtonPressed(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_COMMAND)
    {
        std::map<int, BTN_PRESSED_PROC>::iterator it = buttons.find(LOWORD(wParam));
        if (it != buttons.end())
        {
            if (it->second)
            {
                it->second(this);
            }
        }
    }
}
// [/Roxy::Widget::Process_ButtonPressed]


// [Roxy::Widget::addMsgProc]:
void Roxy::Widget::addMsgProc(UINT msg, MSG_PROC msgproc)
{
    if (msgproc)
    {
        msg_proc_map.insert(std::make_pair(msg, msgproc));
    }
}
// [/Roxy::Widget::addMsgProc]


// [Roxy::Widget::createWindow]:
bool Roxy::Widget::createWindow()
{
    window_class_name = getRandomString(32 + rand() % 32);

    if (bWidnowClassIsRegistered == false)
    {
        WNDCLASSEX wcex;
        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = DummyWindowProcedure;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = hInstance;
        wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszMenuName = NULL;
        wcex.lpszClassName = window_class_name.c_str();
        wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

        if (!RegisterClassEx(&wcex))
        {
            error = _T("Window Registration Failed.");
            return false;
        }
        bWidnowClassIsRegistered = true;
    }

    hWnd = CreateWindowEx(
        WS_EX_APPWINDOW,
        window_class_name.c_str(),
        window_title.c_str(),
        WS_OVERLAPPEDWINDOW,
        point.x,
        point.y,
        size.cx,
        size.cy,
        HWND_DESKTOP,
        NULL,
        hInstance,
        reinterpret_cast<LPVOID>(this));

    if (!hWnd)
    {
        error = _T("Window Creation Failed.");
        return false;
    }
    return true;
}
// [/Roxy::Widget::createWindow]


// [Roxy::Widget::show]:
void Roxy::Widget::show()
{
    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);
}
// [/Roxy::Widget::show]


// [Roxy::Widget::hide]:
void Roxy::Widget::hide()
{
    ShowWindow(hWnd, SW_HIDE);
}
// [/Roxy::Widget::hide]


// [Roxy::Widget::maximize]:
void Roxy::Widget::maximize()
{
    ShowWindow(hWnd, SW_MAXIMIZE);
}
// [/Roxy::Widget::maximize]


// [Roxy::Widget::minimize]:
void Roxy::Widget::minimize()
{
    ShowWindow(hWnd, SW_MINIMIZE);
}
// [/Roxy::Widget::minimize]


// [Roxy::Widget::move]:
void Roxy::Widget::move(int x, int y)
{
    if (IsWindow(hWnd))
    {
        SetWindowPos(hWnd, NULL, x, y, 0, 0, SWP_NOSIZE);
    }
    else
    {
        point = { x, y };
    }
}
// [/Roxy::Widget::move]


// [Roxy::Widget::resize]:
void Roxy::Widget::resize(int width, int height)
{
    if (IsWindow(hWnd))
    {
        SetWindowPos(hWnd, NULL, 0, 0, width, height, SWP_NOMOVE);
    }
    else
    {
        size = { width, height };
    }
}
// [/Roxy::Widget::resize]


// [Roxy::Widget::setGeometry]:
void Roxy::Widget::setGeometry(int x, int y, int width, int height)
{
    if (IsWindow(hWnd))
    {
        MoveWindow(hWnd, x, y, width, height, TRUE);
    }
    else
    {
        point = { x, y };
        size = { width, height };
    }
}
// [/Roxy::Widget::setGeometry]


// [Roxy::Widget::centralizePosition]:
void Roxy::Widget::centralizePosition()
{
    if (IsWindow(hWnd))
    {
        RECT tmp;
        if (GetWindowRect(hWnd, &tmp))
        {
            int iWindowWidth = tmp.right - tmp.left;
            int iWindowHeight = tmp.bottom - tmp.top;

            MoveWindow(
                hWnd,
                (GetSystemMetrics(SM_CXSCREEN) >> 1) - (iWindowWidth >> 1),
                (GetSystemMetrics(SM_CYSCREEN) >> 1) - (iWindowHeight >> 1),
                iWindowWidth,
                iWindowHeight,
                TRUE
            );
        }
    }
    else
    {
        point = {
            (GetSystemMetrics(SM_CXSCREEN) >> 1) - (size.cx >> 1),
            (GetSystemMetrics(SM_CYSCREEN) >> 1) - (size.cy >> 1),
        };
    }
}
// [/Roxy::Widget::centralizePosition]


// [Roxy::Widget::addPushButton]:
bool Roxy::Widget::addPushButton(String title, int x, int y, int width, int height, int id, BTN_PRESSED_PROC proc)
{
    HWND hButton = CreateWindowEx(0, _T("button"), title.c_str(), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y, width, height, hWnd, (HMENU)id, NULL, NULL);
    if (hButton)
    {
        buttons.insert(std::make_pair(id, proc));
    }
    return hButton != NULL;
}
// [/Roxy::Widget::addPushButton]


// [Roxy::GetWidget]:
Roxy::Widget* Roxy::GetWidget(HWND hWnd)
{
    return reinterpret_cast<Widget*>(GetWindowLongPtr(hWnd, GWL_USERDATA));
}
// [/Roxy::GetWidget]
