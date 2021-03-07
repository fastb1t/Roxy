#include "Window.h"

// [DummyWindowProcedure]:
static LRESULT CALLBACK DummyWindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    Roxy::Window* window = NULL;

    if (message == WM_CREATE)
    {
        LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
        if (lpcs && lpcs->lpCreateParams)
        {
            window = reinterpret_cast<Roxy::Window*>(lpcs->lpCreateParams);
            if (window)
            {
                SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR)window);
                window->setWindowHandle(hWnd);
            }
        }
    }

    window = reinterpret_cast<Roxy::Window*>(GetWindowLongPtr(hWnd, GWL_USERDATA));
    if (window)
    {
        window->Process_ButtonPressed(hWnd, message, wParam, lParam);
        return window->WindowProcedure(hWnd, message, wParam, lParam);
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}
// [/DummyWindowProcedure]


// [Roxy::Window::Window]:
Roxy::Window::Window(String sTitle, HINSTANCE hInstance) :
    m_hWnd(NULL),
    m_point({ CW_USEDEFAULT, CW_USEDEFAULT }),
    m_size({ 700, 500 }),
    m_sWindowTitle(sTitle)
{
    m_sWindowClassName = GetRandomString(32 + rand() % 32);

    if (!hInstance)
    {
        m_hInstance = GetModuleHandle(NULL);
    }


    WNDCLASSEX wcex;
    memset(&wcex, 0, sizeof(WNDCLASSEX));

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = DummyWindowProcedure;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = m_hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = m_sWindowClassName.c_str();
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
    {
        m_bWidnowClassIsRegistered = false;

        m_sError = _T("Window Registration Failed.");
    }
    else
    {
        m_bWidnowClassIsRegistered = true;
    }
}
// [/Roxy::Window::Window]


// [Roxy::Window::~Window]:
Roxy::Window::~Window()
{
    if (m_bWidnowClassIsRegistered)
    {
        UnregisterClass(m_sWindowClassName.c_str(), m_hInstance);
    }
}
// [/Roxy::Window::~Window]


// [Roxy::Window::setInstanceHandle]:
void Roxy::Window::setInstanceHandle(HINSTANCE hInstance)
{
    m_hInstance = hInstance;
}
// [/Roxy::Window::setInstanceHandle]


// [Roxy::Window::setWindowHandle]:
void Roxy::Window::setWindowHandle(HWND hWnd)
{
    m_hWnd = hWnd;
}
// [/Roxy::Window::setWindowHandle]


// [Roxy::Window::WindowProcedure]:
LRESULT Roxy::Window::WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    std::map<UINT, MSG_PROC>::iterator it = m_msg_proc_map.find(message);
    if (it != m_msg_proc_map.end())
    {
        return it->second(hWnd, wParam, lParam);
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}
// [/Roxy::Window::WindowProcedure]


// [Roxy::Window::Process_ButtonPressed]:
void Roxy::Window::Process_ButtonPressed(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_COMMAND)
    {
        std::map<int, BTN_PRESSED_PROC>::iterator it = m_buttons.find(LOWORD(wParam));
        if (it != m_buttons.end())
        {
            if (it->second)
            {
                it->second(this);
            }
        }
    }
}
// [/Roxy::Window::Process_ButtonPressed]


// [Roxy::Window::addMsgProc]:
void Roxy::Window::addMsgProc(UINT msg, MSG_PROC msgproc)
{
    if (msgproc)
    {
        m_msg_proc_map.insert(std::make_pair(msg, msgproc));
    }
}
// [/Roxy::Window::addMsgProc]


// [Roxy::Window::create]:
bool Roxy::Window::create()
{
    m_hWnd = CreateWindowEx(
        WS_EX_APPWINDOW,
        m_sWindowClassName.c_str(),
        m_sWindowTitle.c_str(),
        WS_OVERLAPPEDWINDOW,
        m_point.x,
        m_point.y,
        m_size.cx,
        m_size.cy,
        HWND_DESKTOP,
        NULL,
        m_hInstance,
        reinterpret_cast<LPVOID>(this));

    if (!m_hWnd)
    {
        m_sError = _T("Window Creation Failed.");
        return false;
    }
    return true;
}
// [/Roxy::Window::create]


// [Roxy::Window::show]:
void Roxy::Window::show()
{
    ShowWindow(m_hWnd, SW_SHOW);
    UpdateWindow(m_hWnd);
}
// [/Roxy::Window::show]


// [Roxy::Window::hide]:
void Roxy::Window::hide()
{
    ShowWindow(m_hWnd, SW_HIDE);
}
// [/Roxy::Window::hide]


// [Roxy::Window::maximize]:
void Roxy::Window::maximize()
{
    ShowWindow(m_hWnd, SW_MAXIMIZE);
}
// [/Roxy::Window::maximize]


// [Roxy::Window::minimize]:
void Roxy::Window::minimize()
{
    ShowWindow(m_hWnd, SW_MINIMIZE);
}
// [/Roxy::Window::minimize]


// [Roxy::Window::move]:
void Roxy::Window::move(int x, int y)
{
    m_point = { x, y };

    if (IsWindow(m_hWnd))
    {
        SetWindowPos(m_hWnd, NULL, x, y, 0, 0, SWP_NOSIZE);
    }
}
// [/Roxy::Window::move]


// [Roxy::Window::resize]:
void Roxy::Window::resize(int width, int height)
{
    m_size = { width, height };

    if (IsWindow(m_hWnd))
    {
        SetWindowPos(m_hWnd, NULL, 0, 0, width, height, SWP_NOMOVE);
    }
}
// [/Roxy::Window::resize]


// [Roxy::Window::setGeometry]:
void Roxy::Window::setGeometry(int x, int y, int width, int height)
{
    m_point = { x, y };
    m_size = { width, height };

    if (IsWindow(m_hWnd))
    {
        MoveWindow(m_hWnd, x, y, width, height, TRUE);
    }
}
// [/Roxy::Window::setGeometry]


// [Roxy::Window::centralizePosition]:
void Roxy::Window::centralizePosition()
{
    m_point = {
        (GetSystemMetrics(SM_CXSCREEN) >> 1) - (m_size.cx >> 1),
        (GetSystemMetrics(SM_CYSCREEN) >> 1) - (m_size.cy >> 1),
    };

    if (IsWindow(m_hWnd))
    {
        RECT tmp;
        if (GetWindowRect(m_hWnd, &tmp))
        {
            int iWindowWidth = tmp.right - tmp.left;
            int iWindowHeight = tmp.bottom - tmp.top;

            MoveWindow(
                m_hWnd,
                (GetSystemMetrics(SM_CXSCREEN) >> 1) - (iWindowWidth >> 1),
                (GetSystemMetrics(SM_CYSCREEN) >> 1) - (iWindowHeight >> 1),
                iWindowWidth,
                iWindowHeight,
                TRUE
            );
        }
    }
}
// [/Roxy::Window::centralizePosition]


// [Roxy::Window::addPushButton]:
bool Roxy::Window::addPushButton(String title, int x, int y, int width, int height, int id, BTN_PRESSED_PROC proc)
{
    HWND hButton = CreateWindowEx(0, _T("button"), title.c_str(), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y, width, height, m_hWnd, (HMENU)id, NULL, NULL);
    if (hButton)
    {
        m_buttons.insert(std::make_pair(id, proc));
    }
    return hButton != NULL;
}
// [/Roxy::Window::addPushButton]


// [Roxy::GetWidget]:
Roxy::Window* Roxy::GetWindowPtrFromHandle(HWND hWnd)
{
    return reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWL_USERDATA));
}
// [/Roxy::GetWidget]
