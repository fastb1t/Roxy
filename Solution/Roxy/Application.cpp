#include "Application.h"

// [Roxy::Application::Application]:
Roxy::Application::Application()
{
    srand(static_cast<size_t>(time(NULL)));
}
// [/Roxy::Application::Application]


// [Roxy::Application::~Application]:
Roxy::Application::~Application()
{

}
// [/Roxy::Application::~Application]


// [Roxy::Application::loop]:
int Roxy::Application::loop()
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
// [/Roxy::Application::loop]
