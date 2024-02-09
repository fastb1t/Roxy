#include "Tabs.h"

#include "Tabs.h"

#include <windowsx.h>
#include <commctrl.h>

#pragma comment(lib, "rpcrt4.lib")
#pragma comment(lib, "comctl32.lib")

static bool RegisterClass(HINSTANCE hInstance, const std::string& sClassName, WNDPROC wndproc);
static LRESULT CALLBACK Manager_WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK Tab_WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#define MANAGER_CLASS "__TabsManagerClass"
#define TAB_CLASS "__TabClass"

static bool g_bTabsSystemInit = false;

static HINSTANCE g_hInstance = NULL;

namespace Roxy {
    class Tab {
        HWND m_hParentWnd;
        HWND m_hWnd;
        std::string m_sCaption;
        UUID m_id;

        Roxy::TabsManager* m_manager;

        Roxy::TabCreationCallback m_pTabCreationCallback;
        Roxy::TabPaintCallback m_pTabPaintCallback;

    public:
        Tab() :
            m_hParentWnd(NULL),
            m_hWnd(NULL),
            m_manager(nullptr),
            m_pTabCreationCallback(NULL),
            m_pTabPaintCallback(NULL) {

            memset(&m_id, 0, sizeof(GUID));
            if (UuidCreate(&m_id) != RPC_S_OK)
            {
                // Failed to create UUID.
            }
        }

        ~Tab() {

        }

        void SetTabsManager(Roxy::TabsManager* manager) {
            m_manager = manager;
        }

        Roxy::TabsManager* GetTabsManager() const {
            return m_manager;
        }

        void SetParent(HWND hParentWnd) {
            m_hParentWnd = hParentWnd;
        }

        HWND GetParent() const {
            return m_hParentWnd;
        }

        void SetHandle(HWND hWnd) {
            m_hWnd = hWnd;
        }

        HWND GetHandle() const {
            return m_hWnd;
        }

        void SetCaption(const std::string& sCaption) {
            m_sCaption = sCaption;
        }

        std::string GetCaption() const {
            return m_sCaption;
        }

        void SetTabCreationCallback(Roxy::TabCreationCallback pTabCreationCallback) {
            m_pTabCreationCallback = pTabCreationCallback;
        }

        Roxy::TabCreationCallback GetTabCreationCallback() {
            return m_pTabCreationCallback;
        }

        void SetTabPaintCallback(Roxy::TabPaintCallback pTabPaintCallback) {
            m_pTabPaintCallback = pTabPaintCallback;
        }

        Roxy::TabPaintCallback GetTabPaintCallback() {
            return m_pTabPaintCallback;
        }

        std::string GetUUID() {
            std::string str;
            RPC_CSTR szUUID = NULL;
            if (UuidToString(&m_id, &szUUID) == RPC_S_OK)
            {
                str = (LPCSTR)szUUID;
                RpcStringFree(&szUUID);
            }
            return str;
        }

        void Show() {
            ShowWindow(m_hWnd, SW_SHOW);
            Update();
        }

        void Hide() {
            ShowWindow(m_hWnd, SW_HIDE);
        }

        void Update() {
            InvalidateRect(m_hWnd, NULL, FALSE);
        }
    };
}

// [RegisterClass]:
static bool RegisterClass(HINSTANCE hInstance, const std::string& sClassName, WNDPROC wndproc)
{
    WNDCLASSEX wcex;
    memset(&wcex, 0, sizeof(WNDCLASSEX));

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = wndproc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = sClassName.c_str();
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    return RegisterClassEx(&wcex);
}
// [/RegisterClass]


// [Manager_WindowProcedure]: 
static LRESULT CALLBACK Manager_WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static HBRUSH hBackgroundBrush = NULL;

    static HBRUSH hTabBrush = NULL;
    static HBRUSH hTabActiveBrush = NULL;

    static HFONT hCaptionFont = NULL;

    switch (msg)
    {
    case WM_CREATE:
    {
        hCaptionFont = CreateFont(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 0, 0, 0, 0, "Arial");

        LPCREATESTRUCT lpcs = (LPCREATESTRUCT)lParam;
        if (lpcs != nullptr)
        {
            Roxy::TabsManager* manager = (Roxy::TabsManager*)lpcs->lpCreateParams;
            if (manager != nullptr)
            {
                SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR)manager);

                Roxy::T_VISUALDATA* pVisualData = manager->GetVisualData();
                if (pVisualData != nullptr)
                {
                    hBackgroundBrush = CreateSolidBrush(pVisualData->clrManagerBackgroundColor);

                    hTabBrush = CreateSolidBrush(pVisualData->clrInActiveTabBackgroundColor);
                    hTabActiveBrush = CreateSolidBrush(pVisualData->clrActiveTabBackgroundColor);
                }
            }
        }

        return TRUE;
    }
    break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hDC = BeginPaint(hWnd, &ps);
        RECT rc;
        GetClientRect(hWnd, &rc);
        const int iWindowWidth = rc.right - rc.left;
        const int iWindowHeight = rc.bottom - rc.top;
        HDC hMemDC = CreateCompatibleDC(hDC);
        HBITMAP hBitmap = CreateCompatibleBitmap(hDC, iWindowWidth, iWindowHeight);
        HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

        FillRect(hMemDC, &ps.rcPaint, hBackgroundBrush);

        int iOldBkMode = SetBkMode(hMemDC, TRANSPARENT);
        HFONT hOldFont = (HFONT)GetCurrentObject(hMemDC, OBJ_FONT);
        HBRUSH hOldBrush = (HBRUSH)GetCurrentObject(hMemDC, OBJ_BRUSH);
        HPEN hOldPen = (HPEN)GetCurrentObject(hMemDC, OBJ_PEN);
        COLORREF clrOldColor = GetTextColor(hMemDC);

        Roxy::TabsManager* manager = (Roxy::TabsManager*)GetWindowLongPtr(hWnd, GWL_USERDATA);
        if (manager != nullptr)
        {
            if (manager->TabsSwitcherIsActive())
            {
                TCHAR szText[1024] = { 0 };
                SIZE size;
                RECT rect;

                SelectObject(hMemDC, hCaptionFont);

                Roxy::T_VISUALDATA* pVisualData = manager->GetVisualData();

                for (size_t i = 0; i < manager->GetTabsCount(); i++)
                {
                    if (manager->GetTabRect(i, &rect))
                    {
                        if (manager->GetActiveTab() == i)
                        {
                            SelectObject(hMemDC, hTabActiveBrush);
                            SetTextColor(hMemDC, pVisualData->clrActiveTabTextColor);
                        }
                        else
                        {
                            SelectObject(hMemDC, hTabBrush);
                            SetTextColor(hMemDC, pVisualData->clrInActiveTabTextColor);
                        }

                        Rectangle(hMemDC, rect.left, rect.top, rect.right, rect.bottom);

                        lstrcpy(szText, manager->GetTabCaption(i).c_str());
                        GetTextExtentPoint32(hMemDC, szText, lstrlen(szText), &size);

                        TextOut(
                            hMemDC,
                            rect.left + ((rect.right - rect.left) >> 1) - (size.cx >> 1),
                            rect.top + ((rect.bottom - rect.top) >> 1) - (size.cy >> 1),
                            szText,
                            lstrlen(szText)
                        );

                        if (manager->GetOnHoverTab() == i)
                        {
                            MoveToEx(hMemDC, rect.left + 5, rect.bottom - 5, NULL);
                            LineTo(hMemDC, rect.right - 5, rect.bottom - 5);
                        }
                    }
                }
            }
        }

        SetTextColor(hMemDC, clrOldColor);
        SelectObject(hMemDC, hOldPen);
        SelectObject(hMemDC, hOldBrush);
        SelectObject(hMemDC, hOldFont);
        SetBkMode(hMemDC, iOldBkMode);

        BitBlt(hDC, 0, 0, iWindowWidth, iWindowHeight, hMemDC, 0, 0, SRCCOPY);
        SelectObject(hMemDC, hOldBitmap);
        DeleteObject(hBitmap);
        DeleteDC(hMemDC);
        EndPaint(hWnd, &ps);
    }
    break;

    case WM_LBUTTONDOWN:
    {
        Roxy::TabsManager* manager = (Roxy::TabsManager*)GetWindowLongPtr(hWnd, GWL_USERDATA);
        if (manager != nullptr)
        {
            if (manager->TabsSwitcherIsActive())
            {
                POINT pt = { 0 };
                RECT rect;

                pt.x = LOWORD(lParam);
                pt.y = HIWORD(lParam);

                for (size_t i = 0; i < manager->GetTabsCount(); i++)
                {
                    if (manager->GetTabRect(i, &rect))
                    {
                        if (PtInRect(&rect, pt))
                        {
                            manager->SetActiveTab(i);
                            break;
                        }
                    }
                }
            }
        }
    }
    break;

    case WM_MOUSEMOVE:
    {
        Roxy::TabsManager* manager = (Roxy::TabsManager*)GetWindowLongPtr(hWnd, GWL_USERDATA);
        if (manager != nullptr)
        {
            if (manager->TabsSwitcherIsActive())
            {
                POINT pt = { 0 };
                pt.x = LOWORD(lParam);
                pt.y = HIWORD(lParam);

                bool bFound = false;
                int prev = -1;

                RECT rect;
                for (unsigned int i = 0; i < manager->GetTabsCount(); i++)
                {
                    if (manager->GetTabRect(i, &rect))
                    {
                        if (PtInRect(&rect, pt))
                        {
                            prev = manager->GetOnHoverTab();

                            manager->OnHover(i);

                            if (prev != i)
                            {
                                InvalidateRect(hWnd, NULL, FALSE);
                            }

                            bFound = true;

                            break;
                        }
                    }
                }

                if (!bFound)
                {
                    manager->OnHover(-1);

                    InvalidateRect(hWnd, NULL, FALSE);
                }

                TRACKMOUSEEVENT tme;
                memset(&tme, 0, sizeof(TRACKMOUSEEVENT));

                tme.cbSize = sizeof(tme);
                tme.hwndTrack = hWnd;
                tme.dwFlags = TME_LEAVE;

                _TrackMouseEvent(&tme);
            }
        }
    }
    break;

    case WM_MOUSELEAVE:
    {
        Roxy::TabsManager* manager = (Roxy::TabsManager*)GetWindowLongPtr(hWnd, GWL_USERDATA);
        if (manager != nullptr)
        {
            manager->OnHover(-1);
        }

        InvalidateRect(hWnd, NULL, FALSE);
    }
    break;

    case WM_DESTROY:
    {
        DeleteObject(hBackgroundBrush);

        DeleteObject(hTabBrush);
        DeleteObject(hTabActiveBrush);

        DeleteObject(hCaptionFont);
    }
    break;

    case WM_ERASEBKGND:
        return 1;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}
// [/Manager_WindowProcedure]


//#include "IDs.h"


// [Tab_WindowProcedure]: 
static LRESULT CALLBACK Tab_WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static HBRUSH hBackgroundBrush = NULL;

    static HFONT hDefaultFont = NULL;

    switch (msg)
    {
    case WM_CTLCOLORSTATIC:
    {
        if (hBackgroundBrush != NULL)
        {
            if (GetWindowLongPtr((HWND)lParam, GWL_ID) > 1711 &&
                GetWindowLongPtr((HWND)lParam, GWL_ID) < 1711 + 8);
            else
            {
                //if (GetWindowLongPtr((HWND)lParam, GWL_ID) != IDC_PAGE1_NUMBER_OF_COPIES &&
                //    GetWindowLongPtr((HWND)lParam, GWL_ID) != IDC_PAGE1_SETUP_PATH)
                {
                    return (LRESULT)hBackgroundBrush;
                }
            }
        }
    }
    break;

    case WM_CREATE:
    {
        hDefaultFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 0, 0, 0, 0, "Arial");

        LPCREATESTRUCT lpcs = (LPCREATESTRUCT)lParam;
        if (lpcs != nullptr)
        {
            Roxy::Tab* tab = (Roxy::Tab*)lpcs->lpCreateParams;
            if (tab != nullptr)
            {
                SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR)tab);

                Roxy::TabsManager* pManager = tab->GetTabsManager();
                if (pManager != nullptr)
                {
                    Roxy::T_VISUALDATA* pVisualData = pManager->GetVisualData();
                    if (pVisualData != nullptr)
                    {
                        hBackgroundBrush = CreateSolidBrush(pVisualData->clrActiveTabBackgroundColor);
                    }
                }

                Roxy::TabCreationCallback callback = tab->GetTabCreationCallback();
                if (callback != NULL)
                {
                    if (pManager != nullptr)
                    {
                        // В цьому випадку в m_tabs ще не добавлена вкладка, тому FindTabID її не знайде.
                        //callback(hWnd, lpcs, pManager->FindTabID(tab));

                        // Так як вкладка ще не добавлена в m_tabs, то GetTabsCount поверне ID поточної вкладки, який буде присвоєний.
                        callback(hWnd, lpcs, pManager->GetTabsCount());
                    }
                }
            }
        }

        return TRUE;
    }
    break;

    case WM_COMMAND:
    {
        Roxy::Tab* tab = (Roxy::Tab*)GetWindowLongPtr(hWnd, GWL_USERDATA);
        if (tab != nullptr)
        {
            Roxy::TabsManager* manager = tab->GetTabsManager();
            if (manager != nullptr)
            {
                HWND hParentWnd = manager->GetParent();
                if (hParentWnd != NULL)
                {
                    SendMessage(hParentWnd, WM_COMMAND, wParam, lParam);
                }
            }
        }
    }
    break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hDC = BeginPaint(hWnd, &ps);
        RECT rc;
        GetClientRect(hWnd, &rc);
        const int iWindowWidth = rc.right - rc.left;
        const int iWindowHeight = rc.bottom - rc.top;
        HDC hMemDC = CreateCompatibleDC(hDC);
        HBITMAP hBitmap = CreateCompatibleBitmap(hDC, iWindowWidth, iWindowHeight);
        HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

        FillRect(hMemDC, &ps.rcPaint, hBackgroundBrush);

        //*
        int iOldBkMode = SetBkMode(hMemDC, TRANSPARENT);
        HFONT hOldFont = (HFONT)GetCurrentObject(hMemDC, OBJ_FONT);
        HBRUSH hOldBrush = (HBRUSH)GetCurrentObject(hMemDC, OBJ_BRUSH);
        HPEN hOldPen = (HPEN)GetCurrentObject(hMemDC, OBJ_PEN);
        COLORREF clrOldColor = GetTextColor(hMemDC);

        SetTextColor(hMemDC, RGB(0, 0, 0));
        SelectObject(hMemDC, hDefaultFont);

        TCHAR szText[1024] = { 0 };

        Roxy::Tab* tab = (Roxy::Tab*)GetWindowLongPtr(hWnd, GWL_USERDATA);
        if (tab != nullptr)
        {
            //std::string uuid = "UUID: " + tab->GetUUID();
            //TextOut(hMemDC, 10, 10, uuid.c_str(), uuid.size());

            Roxy::TabPaintCallback callback = tab->GetTabPaintCallback();
            if (callback != NULL)
            {
                callback(hWnd, hMemDC, tab->GetTabsManager()->FindTabID(tab));
            }
        }

        SetTextColor(hMemDC, clrOldColor);
        SelectObject(hMemDC, hOldPen);
        SelectObject(hMemDC, hOldBrush);
        SelectObject(hMemDC, hOldFont);
        SetBkMode(hMemDC, iOldBkMode);
        //*/

        BitBlt(hDC, 0, 0, iWindowWidth, iWindowHeight, hMemDC, 0, 0, SRCCOPY);
        SelectObject(hMemDC, hOldBitmap);
        DeleteObject(hBitmap);
        DeleteDC(hMemDC);
        EndPaint(hWnd, &ps);
    }
    break;

    case WM_DESTROY:
    {
        DeleteObject(hBackgroundBrush);

        DeleteObject(hDefaultFont);
    }
    break;

    case WM_ERASEBKGND:
        return 1;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}
// [/Tab_WindowProcedure]


// [Roxy::Tabs_Init]:
bool Roxy::Tabs_Init(HINSTANCE hInstance)
{
    if (!g_bTabsSystemInit &&
        RegisterClass(hInstance, MANAGER_CLASS, Manager_WindowProcedure) &&
        RegisterClass(hInstance, TAB_CLASS, Tab_WindowProcedure))
    {
        g_hInstance = hInstance;
        g_bTabsSystemInit = true;
    }
    return g_bTabsSystemInit;
}
// [/Roxy::Tabs_Init]


// [Roxy::Tabs_Shutdown]:
void Roxy::Tabs_Shutdown()
{
    if (g_bTabsSystemInit)
    {
        UnregisterClass(MANAGER_CLASS, g_hInstance);
        UnregisterClass(TAB_CLASS, g_hInstance);

        g_hInstance = NULL;
        g_bTabsSystemInit = false;
    }
}
// [/Roxy::Tabs_Shutdown]


// [Roxy::TabsManager::GetTabPointer]:
Roxy::Tab* Roxy::TabsManager::GetTabPointer(unsigned int tab)
{
    if (tab < m_tabs.size())
    {
        return m_tabs[tab];
    }
    return nullptr;
}
// [/Roxy::TabsManager::GetTabPointer]


// [Roxy::TabsManager::Create]:
bool Roxy::TabsManager::Create(int x, int y, int width, int height, HWND hParentWnd)
{
    m_hParentWnd = hParentWnd;

    m_hWnd = CreateWindowEx(
        WS_EX_APPWINDOW,
        MANAGER_CLASS,
        "TabsManager",
        WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,// | WS_BORDER,
        x,
        y,
        width,
        height,
        m_hParentWnd,
        NULL,
        NULL,
        reinterpret_cast<LPVOID>(this)
    );

    if (m_hWnd != NULL)
    {
        ShowWindow(m_hWnd, SW_HIDE);
    }

    return m_hWnd != NULL;
}
// [/Roxy::TabsManager::Create]


// [Roxy::TabsManager::Show]:
void Roxy::TabsManager::Show()
{
    ShowWindow(m_hWnd, SW_SHOW);
    UpdateWindow(m_hWnd);

    Tab* tab = GetTabPointer(GetActiveTab());
    if (tab != nullptr)
    {
        tab->Show();
    }
}
// [/Roxy::TabsManager::Show]


// [Roxy::TabsManager::Update]:
void Roxy::TabsManager::Update()
{
    InvalidateRect(m_hWnd, NULL, FALSE);

    Tab* tab = GetTabPointer(GetActiveTab());
    if (tab != nullptr)
    {
        tab->Update();
    }
}
// [/Roxy::TabsManager::Update]


// [Roxy::TabsManager::AddTab]:
bool Roxy::TabsManager::AddTab(const std::string& sCaption, TabCreationCallback callback)
{
    Tab* tab = new (std::nothrow) Tab;
    if (tab != nullptr)
    {
        tab->SetTabsManager(this);

        tab->SetCaption(sCaption);
        tab->SetParent(m_hWnd);
        tab->SetTabCreationCallback(callback);

        RECT rc;
        GetClientRect(tab->GetParent(), &rc);

        tab->SetHandle(CreateWindowEx(
            WS_EX_APPWINDOW,
            TAB_CLASS,
            tab->GetUUID().c_str(),
            WS_CHILD | WS_VISIBLE,
            0,
            TabsSwitcherIsActive() ? 30 : 0,
            rc.right - rc.left,
            rc.bottom - rc.top - (TabsSwitcherIsActive() ? 30 : 0),
            m_hWnd,
            NULL,
            NULL,
            reinterpret_cast<LPVOID>(tab)
        ));

        if (tab->GetHandle() != NULL)
        {
            ShowWindow(tab->GetHandle(), SW_HIDE);
            UpdateWindow(tab->GetHandle());

            m_tabs.push_back(tab);
        }
    }
    return false;
}
// [/Roxy::TabsManager::AddTab]


// [Roxy::TabsManager::RemoveAllTabs]:
void Roxy::TabsManager::RemoveAllTabs()
{
    if (m_tabs.size() > 0)
    {
        for (std::vector<Tab*>::const_iterator it = m_tabs.begin(); it != m_tabs.end(); it++)
        {
            delete (*it);
        }

        m_tabs.clear();
    }
}
// [/Roxy::TabsManager::RemoveAllTabs]


// [Roxy::TabsManager::SetTabCaption]:
void Roxy::TabsManager::SetTabCaption(unsigned int tab, const std::string& sCaption)
{
    if (tab < m_tabs.size())
    {
        m_tabs[tab]->SetCaption(sCaption);
    }
}
// [/Roxy::TabsManager::SetTabCaption]


// [Roxy::TabsManager::GetTabCaption]:
std::string Roxy::TabsManager::GetTabCaption(unsigned int tab) const
{
    if (tab < m_tabs.size())
    {
        return m_tabs[tab]->GetCaption();
    }
    return "";
}
// [/Roxy::TabsManager::GetTabCaption]


// [Roxy::TabsManager::GetTabHandle]:
HWND Roxy::TabsManager::GetTabHandle(unsigned int tab) const
{
    if (tab < m_tabs.size())
    {
        return m_tabs[tab]->GetHandle();
    }
    return NULL;
}
// [/Roxy::TabsManager::GetTabHandle]


// [Roxy::TabsManager::SetActiveTab]:
void Roxy::TabsManager::SetActiveTab(unsigned int tab)
{
    if (tab != m_iActiveTab && tab < m_tabs.size())
    {
        Tab* pTab = GetTabPointer(m_iActiveTab);
        if (pTab != nullptr)
        {
            pTab->Hide();
        }

        m_iActiveTab = tab;

        pTab = GetTabPointer(m_iActiveTab);
        if (pTab != nullptr)
        {
            pTab->Show();
        }

        Update();
    }
}
// [/Roxy::TabsManager::SetActiveTab]


// [Roxy::TabsManager::SetTabPaintCallback]:
void Roxy::TabsManager::SetTabPaintCallback(unsigned int tab, TabPaintCallback callback)
{
    if (tab < m_tabs.size())
    {
        m_tabs[tab]->SetTabPaintCallback(callback);
    }
}
// [/Roxy::TabsManager::SetTabPaintCallback]


// [Roxy::TabsManager::GetTabRect]:
bool Roxy::TabsManager::GetTabRect(unsigned int tab, RECT* rect) const
{
    if (tab < GetTabsCount())
    {
        if (rect != nullptr)
        {
            int x = 2 + tab * 42;

            rect->left = x;
            rect->top = 2;
            rect->right = x + 40;
            rect->bottom = 32;
        }

        return true;
    }
    return false;
}
// [/Roxy::TabsManager::GetTabRect]


// [Roxy::TabsManager::GetTabPoint]:
bool Roxy::TabsManager::GetTabPoint(unsigned int tab, POINT* point) const
{
    if (tab < GetTabsCount())
    {
        if (point != nullptr)
        {
            point->x = 2 + tab * 42;
            point->y = 2;
        }

        return true;
    }
    return false;
}
// [/Roxy::TabsManager::GetTabPoint]


// [Roxy::TabsManager::GetTabSize]:
bool Roxy::TabsManager::GetTabSize(unsigned int tab, SIZE* size) const
{
    if (tab < GetTabsCount())
    {
        if (size != nullptr)
        {
            size->cx = 40;
            size->cy = 30;
        }

        return true;
    }
    return false;
}
// [/Roxy::TabsManager::GetTabSize]
