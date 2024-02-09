#ifndef _ROXY_TABS_H_
#define _ROXY_TABS_H_

#include <string>
#include <vector>
#include <windows.h>

namespace Roxy {

    //#define TS_BACKGROUND_COLOR 0x01

    bool Tabs_Init(HINSTANCE hInstance);
    void Tabs_Shutdown();

    typedef void(*TabCreationCallback)(HWND, LPCREATESTRUCT, size_t TabID);
    typedef void(*TabPaintCallback)(HWND, HDC, size_t TabID);

    //typedef LRESULT(CALLBACK* MSG_PROC)(HWND, UINT, WPARAM, LPARAM);

    struct T_VISUALDATA {
        COLORREF clrManagerBackgroundColor;     // Цвет фона менеджера вкладок.

        COLORREF clrActiveTabBackgroundColor;   // Цвет фона активной вкладки.
        COLORREF clrActiveTabTextColor;         // Цвет текста активной вкладки.

        COLORREF clrInActiveTabBackgroundColor; // Цвет фона неактивной вкладки.
        COLORREF clrInActiveTabTextColor;       // Цвет текста неактивной вкладки.
    };

    class Tab;

    class TabsManager {
        HWND m_hParentWnd;
        HWND m_hWnd;
        std::vector<Tab*> m_tabs;
        unsigned int m_iActiveTab;
        unsigned int m_iOnHoverTab;

        // Options:
        bool m_bTabsSwitcher; // Если будет false, то переключателя вкладок не будет.

        T_VISUALDATA* m_pVisualData;

    public:
        TabsManager(T_VISUALDATA* pVisualData = nullptr, bool bTabsSwitcher = true) :
            m_hParentWnd(NULL),
            m_hWnd(NULL),
            m_iActiveTab(0),
            m_iOnHoverTab(-1),
            m_bTabsSwitcher(bTabsSwitcher),
            m_pVisualData(nullptr) {

            m_pVisualData = new (std::nothrow) T_VISUALDATA;
            if (m_pVisualData != nullptr)
            {
                if (pVisualData != nullptr)
                {
                    memcpy(m_pVisualData, pVisualData, sizeof(T_VISUALDATA));
                }
                else
                {
                    m_pVisualData->clrManagerBackgroundColor = RGB(0, 0, 0);
                    m_pVisualData->clrActiveTabBackgroundColor = RGB(0, 0, 0);
                    m_pVisualData->clrActiveTabTextColor = RGB(0, 0, 0);
                    m_pVisualData->clrInActiveTabBackgroundColor = RGB(0, 0, 0);
                    m_pVisualData->clrInActiveTabTextColor = RGB(0, 0, 0);
                }
            }
        }

        T_VISUALDATA* GetVisualData() const {
            return m_pVisualData;
        }

        ~TabsManager() {
            RemoveAllTabs();

            if (m_pVisualData) {
                delete m_pVisualData;
            }
        }

        bool Create(int x, int y, int width, int height, HWND hParentWnd);

        HWND GetParent() const {
            return m_hParentWnd;
        }

        HWND GetHandle() const {
            return m_hWnd;
        }

        void Show();

        void Update();


        bool AddTab(const std::string& sCaption, TabCreationCallback callback = NULL);

        unsigned int GetTabsCount() const {
            return m_tabs.size();
        }

        void RemoveAllTabs();


        void SetTabCaption(unsigned int tab, const std::string& sCaption);
        std::string GetTabCaption(unsigned int tab) const;

        HWND GetTabHandle(unsigned int tab) const;

        void SetActiveTab(unsigned int tab);

        unsigned int GetActiveTab() const {
            return m_iActiveTab;
        }

        Tab* GetTabPointer(unsigned int tab);

        void SetTabPaintCallback(unsigned int tab, TabPaintCallback callback);

        void OnHover(int tab) {
            m_iOnHoverTab = tab;
        }

        unsigned int GetOnHoverTab() const {
            return m_iOnHoverTab;
        }

        bool GetTabRect(unsigned int tab, RECT* rect) const;
        bool GetTabPoint(unsigned int tab, POINT* point) const;
        bool GetTabSize(unsigned int tab, SIZE* size) const;

        bool TabsSwitcherIsActive() const {
            return m_bTabsSwitcher;
        }

        HWND GetControlHandle(unsigned int iTab, unsigned int iControlID) const {
            if (iTab < m_tabs.size())
            {
                HWND hTabWnd = GetTabHandle(iTab);
                if (hTabWnd != NULL)
                {
                    return GetDlgItem(hTabWnd, iControlID);
                }
            }
            return NULL;
        }

        size_t FindTabID(const Tab* tab) const {
            for (size_t i = 0; i < m_tabs.size(); i++)
            {
                if (m_tabs[i] == tab)
                {
                    return i;
                }
            }
            return 0;
        }
    };

}

#endif // !_ROXY_TABS_H_
