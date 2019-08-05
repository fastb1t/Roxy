#pragma once

#include <tchar.h>
#include <map>
#include <windows.h>

#include "Algorithms.h"

class Widget;

typedef LRESULT(*MSG_PROC)(HWND, WPARAM, LPARAM);
typedef void(*BTN_PRESSED_PROC)(Widget*);

class Widget {
    HINSTANCE hInstance;
    HWND hWnd;

    POINT point;
    SIZE size;

    String window_title;
    String window_class_name;

    bool bWidnowClassIsRegistered;

    std::map<UINT, MSG_PROC> msg_proc_map;
    std::map<int, BTN_PRESSED_PROC> buttons;

    String error;

public:
    Widget(String title = _T("Window"));

    ~Widget();

    void setWindowHandle(HWND handle);

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
        MessageBox(hWnd, msg.c_str(), title.c_str(), uType);
    }

    //friend class Application;
    //friend LRESULT CALLBACK DummyWindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

Widget* GetWidget(HWND hWnd);
