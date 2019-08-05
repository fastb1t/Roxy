#include "Application.h"

// [Application::Application]:
Application::Application()
{
    srand(static_cast<size_t>(time(NULL)));
}
// [/Application::Application]


// [Application::~Application]:
Application::~Application()
{

}
// [/Application::~Application]


// [Application::execute]:
int Application::execute()
{
    MSG msg;
    BOOL bRet;
    while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
    {
        if (bRet == -1)
        {
            break;
        }
        else
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return (int)msg.wParam;
}
// [/Application::execute]
