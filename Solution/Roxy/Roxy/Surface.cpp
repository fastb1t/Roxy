#include "Surface.h"

Roxy::Surface::Surface()
{
    m_hDC = NULL;
    m_width = 0;
    m_height = 0;
}

Roxy::Surface::~Surface()
{
    Remove();
}

bool Roxy::Surface::New(size_t width, size_t height, HDC hParentDC)
{
    if (!hParentDC)
    {
        hParentDC = GetDC(NULL);
    }
    
    m_hDC = CreateCompatibleDC(hParentDC);

    return true;
}

// [Roxy::Surface::Remove]:
void Roxy::Surface::Remove()
{
    if (m_hDC)
    {
        DeleteDC(m_hDC);
    }

    m_hDC = NULL;
    m_width = 0;
    m_height = 0;
}
// [/Roxy::Surface::Remove]


// [Roxy::Surface::Draw]:
bool Roxy::Surface::Draw(HDC hDC, int x, int y)
{
    if (hDC && m_hDC && m_width > 0 && m_height > 0)
    {
        return (bool)BitBlt(hDC, x, y, m_width, m_height, m_hDC, 0, 0, SRCCOPY);
    }
    return false;
}
// [/Roxy::Surface::Draw]
