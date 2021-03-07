#ifndef _ROXY_WIDGET_H_
#define _ROXY_WIDGET_H_

#include <tchar.h>
#include <map>
#include <windows.h>

#include "Algorithms.h"

namespace Roxy {
    class Window;

    typedef LRESULT(*MSG_PROC)(HWND, WPARAM, LPARAM);
    typedef void(*BTN_PRESSED_PROC)(Window*);

    class Window {
        HINSTANCE m_hInstance;
        HWND m_hWnd;

        POINT m_point;
        SIZE m_size;

        String m_sWindowTitle;
        String m_sWindowClassName;

        bool m_bWidnowClassIsRegistered;

        std::map<UINT, MSG_PROC> m_msg_proc_map;
        std::map<int, BTN_PRESSED_PROC> m_buttons;

        String m_sError;

    public:
        Window(String sTitle = _T("Window"));

        ~Window();

        void setInstanceHandle(HINSTANCE hInstance);
        void setWindowHandle(HWND hWnd);

        LRESULT WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
        void Process_ButtonPressed(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
        void addMsgProc(UINT, MSG_PROC);

        bool createWindow();

        void show();
        void hide();
        void maximize();
        void minimize();

        void move(int x, int y);
        void resize(int width, int height);
        void setGeometry(int x, int y, int width, int height);
        void centralizePosition();

        bool addPushButton(String title, int x, int y, int width, int height, int id, BTN_PRESSED_PROC proc = NULL);

        void messageBox(String msg, String title, UINT uType)
        {
            MessageBox(m_hWnd, msg.c_str(), title.c_str(), uType);
        }

        //friend class Application;
        //friend LRESULT CALLBACK DummyWindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    };

    Window* GetWindowPtrFromHandle(HWND hWnd);
}

#endif
