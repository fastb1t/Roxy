#include "Surface.h"


Roxy::Surface::Surface() :
    m_hDC(NULL),
    m_hBitmap(NULL),
    m_hOldBitmap(NULL),
    m_width(0),
    m_height(0)
{

}

Roxy::Surface::~Surface()
{
    Delete();
}

bool Roxy::Surface::New(size_t width, size_t height, HDC hParentDC)
{
    bool bIsSuccess = false;
    bool bReleaseDC = false;

    if (!hParentDC)
    {
        hParentDC = GetDC(HWND_DESKTOP);
        bReleaseDC = true;
    }

    if (hParentDC)
    {
        m_hDC = CreateCompatibleDC(hParentDC);
        if (m_hDC)
        {
            m_hBitmap = CreateCompatibleBitmap(m_hDC, width, height);
            if (m_hBitmap)
            {
                m_width = width;
                m_height = height;

                m_hOldBitmap = (HBITMAP)SelectObject(m_hDC, m_hBitmap);
                
                RECT rc = { 0, 0, (LONG)m_width, (LONG)m_height };
                FillRect(m_hDC, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));
            }
            else
            {
                DeleteDC(m_hDC);
                m_hDC = NULL;
            }
        }

        if (bReleaseDC)
        {
            ReleaseDC(HWND_DESKTOP, hParentDC);
        }
    }

    return bIsSuccess;
}

// [Roxy::Surface::Delete]:
void Roxy::Surface::Delete()
{
    if (m_hDC)
    {
        SelectObject(m_hDC, m_hOldBitmap);
        DeleteObject(m_hBitmap);
        DeleteDC(m_hDC);
    }

    m_hDC = NULL;
    m_hBitmap = NULL;
    m_hOldBitmap = NULL;
    m_width = 0;
    m_height = 0;
}
// [/Roxy::Surface::Delete]


// [Roxy::Surface::Draw]:
bool Roxy::Surface::Draw(HDC hDC, int x, int y) const
{
    if (HasImage())
    {
        return (bool)BitBlt(hDC, x, y, m_width, m_height, m_hDC, 0, 0, SRCCOPY);
    }
    return false;
}
// [/Roxy::Surface::Draw]
