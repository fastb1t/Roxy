#include "BitMap.h"


// [BitMap::BitMap]:
BitMap::BitMap() :
    hBitmap(NULL),
    hOldBitmap(NULL),
    hDC(NULL),
    bPaintInit(false),
    lpPrivateVar(NULL)
{

}
// [/BitMap::BitMap]


// [BitMap::~BitMap]:
BitMap::~BitMap()
{
    Clear();
}
// [/BitMap::~BitMap]


// [BitMap::Clear]:
void BitMap::Clear()
{
    EndPaint();

    if (hBitmap)
    {
        DeleteObject(hBitmap);
        hBitmap = NULL;
    }
}
// [/BitMap::Clear]


// [BitMap::BeginPaint]:
bool BitMap::BeginPaint()
{
    if (bPaintInit)
    {
        return true;
    }

    HDC hParentDC = GetDC(NULL);
    if (hParentDC)
    {
        hDC = CreateCompatibleDC(hParentDC);
        if (!hDC)
        {
            ReleaseDC(NULL, hParentDC);
            return false;
        }

        hOldBitmap = (HBITMAP)SelectObject(hDC, hBitmap);
        if (!hOldBitmap)
        {
            DeleteDC(hDC);
            hDC = NULL;
            ReleaseDC(NULL, hParentDC);
            return false;
        }

        bPaintInit = true;
        ReleaseDC(NULL, hParentDC);
        return true;
    }
    return false;
}
// [/BitMap::BeginPaint]


// [BitMap::EndPaint]:
bool BitMap::EndPaint()
{
    if (!bPaintInit)
        return true;

    if (hDC && hOldBitmap)
        SelectObject(hDC, hOldBitmap);

    if (hDC)
        DeleteDC(hDC);

    hOldBitmap = NULL;
    hDC = NULL;

    bPaintInit = false;
    return true;
}
// [BitMap::EndPaint]


// [BitMap::SetBitmapHandle]:
bool BitMap::SetBitmapHandle(HBITMAP hBitmap)
{
    if (hBitmap)
        this->hBitmap = hBitmap;
    return hBitmap != NULL;
}
// [/BitMap::SetBitmapHandle]


// [BitMap::GetBitmapHandle]:
HBITMAP BitMap::GetBitmapHandle() const
{
    return hBitmap;
}
// [/BitMap::GetBitmapHandle]


// [BitMap::GetDCHandle]:
HDC BitMap::GetDCHandle() const
{
    return HasImage() ? hDC : NULL;
}
// [/BitMap::GetDCHandle]


// [BitMap::HasImage]:
bool BitMap::HasImage() const
{
    return hBitmap != NULL;
}
// [/BitMap::HasImage]


// [BitMap::CreateNewBitmap]:
bool BitMap::CreateNewBitmap(int iWidth, int iHeight)
{
    if (iWidth <= 0 || iHeight <= 0)
    {
        return false;
    }

    Clear();

    HDC hParentDC = GetDC(NULL);
    if (hParentDC)
    {
        hDC = CreateCompatibleDC(hParentDC);
        if (!hDC)
        {
            ReleaseDC(NULL, hParentDC);
            return false;
        }

        hBitmap = CreateCompatibleBitmap(hParentDC, iWidth, iHeight);
        if (!hBitmap)
        {
            DeleteDC(hDC);
            hDC = NULL;
            ReleaseDC(NULL, hParentDC);
            return false;
        }

        hOldBitmap = (HBITMAP)SelectObject(hDC, hBitmap);
        if (!hOldBitmap)
        {
            DeleteObject(hBitmap);
            DeleteDC(hDC);
            hBitmap = NULL;
            hDC = NULL;
            ReleaseDC(NULL, hParentDC);
            return false;
        }

        PatBlt(hDC, 0, 0, iWidth, iHeight, WHITENESS);

        SelectObject(hDC, hOldBitmap);
        DeleteDC(hDC);

        hOldBitmap = NULL;
        hDC = NULL;

        ReleaseDC(NULL, hParentDC);
        return true;
    }
    return false;
}
// [/BitMap::CreateNewBitmap]


// [BitMap::ReplaceColors]:
bool BitMap::ReplaceColors(COLORREF clrOldColor, COLORREF clrNewColor)
{
    if (!HasImage())
        return false;

    if (!BeginPaint())
        return false;

    BITMAPINFO bi;
    memset(&bi, 0, sizeof(BITMAPINFO));

    bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

    if (!GetDIBits(hDC, hBitmap, 0, 0, NULL, &bi, DIB_RGB_COLORS))
    {
        EndPaint();
        return false;
    }

    bi.bmiHeader.biCompression = BI_RGB;

    WORD wColorBits = bi.bmiHeader.biBitCount >> 3;
    if (!wColorBits)
    {
        EndPaint();
        return false;
    }

    BYTE* pPixels = (BYTE*)GlobalAlloc(GPTR, bi.bmiHeader.biSizeImage * wColorBits);
    if (pPixels)
    {
        if (!GetDIBits(hDC, hBitmap, 0, bi.bmiHeader.biHeight, (LPVOID)pPixels, &bi, DIB_RGB_COLORS))
        {
            EndPaint();
            GlobalFree(pPixels);
            return false;
        }

        SIZE size = GetOriginalSize();

        for (long x = 0; x < size.cx; x++)
        {
            for (long y = 0; y < size.cy; y++)
            {
                COLORREF clrCurrentPixel = RGB(
                    pPixels[wColorBits * ((size.cy - y - 1) * size.cx + x) + 2],
                    pPixels[wColorBits * ((size.cy - y - 1) * size.cx + x) + 1],
                    pPixels[wColorBits * ((size.cy - y - 1) * size.cx + x)]
                );

                if (clrCurrentPixel == clrOldColor)
                {
                    SetPixel(hDC, x, y, clrNewColor);
                }
            }
        }
        GlobalFree(pPixels);
        EndPaint();
        return true;
    }

    EndPaint();
    return false;
}
// [/BitMap::ReplaceColors]


// [BitMap::MakeScreenShot]:
bool BitMap::MakeScreenShot(HWND hWnd, RECT* rect)
{
    Clear();

    HDC hParentDC = GetDC(hWnd);
    if (hParentDC)
    {
        RECT rc;
        if (!GetClientRect(hWnd, &rc))
        {
            if (hWnd)
            {
                ReleaseDC(hWnd, hParentDC);
                return false;
            }
        }

        SIZE size = { 0, 0 };

        if (rect == nullptr)
        {
            if (hWnd != NULL)
            {
                size.cx = rc.right - rc.left;
                size.cy = rc.bottom - rc.top;
            }
            else
            {
                size.cx = GetSystemMetrics(SM_CXSCREEN);
                size.cy = GetSystemMetrics(SM_CYSCREEN);
            }
        }
        else
        {
            size.cx = rect->right - rect->left;
            size.cy = rect->bottom - rect->top;
        }

        hDC = CreateCompatibleDC(hParentDC);
        if (!hDC)
        {
            ReleaseDC(hWnd, hParentDC);
            return false;
        }

        hBitmap = CreateCompatibleBitmap(hParentDC, size.cx, size.cy);
        if (!hBitmap)
        {
            DeleteDC(hDC);
            hDC = NULL;
            ReleaseDC(hWnd, hParentDC);
            return false;
        }

        hOldBitmap = (HBITMAP)SelectObject(hDC, hBitmap);
        if (!hOldBitmap)
        {
            DeleteObject(hBitmap);
            DeleteDC(hDC);
            hBitmap = NULL;
            hDC = NULL;
            ReleaseDC(hWnd, hParentDC);
            return false;
        }

        if (rect == nullptr)
        {
            BitBlt(hDC, 0, 0, size.cx, size.cy, hParentDC, 0, 0, SRCCOPY);
        }
        else
        {
            BitBlt(hDC, 0, 0, size.cx, size.cy, hParentDC, rect->left, rect->top, SRCCOPY);
        }

        SelectObject(hDC, hOldBitmap);
        DeleteDC(hDC);

        hOldBitmap = NULL;
        hDC = NULL;

        ReleaseDC(hWnd, hParentDC);
        return true;
    }
    return false;
}
// [/BitMap::MakeScreenShot]


// [BitMap::ScaleImage]:
bool BitMap::ScaleImage(int iWidth, int iHeight)
{
    if (iWidth <= 0 || iHeight <= 0)
        return false;

    if (!BeginPaint())
        return false;

    HDC hParentDC = GetDC(NULL);
    if (hParentDC)
    {
        HDC hTmpDC = CreateCompatibleDC(hParentDC);
        if (!hTmpDC)
        {
            ReleaseDC(NULL, hParentDC);
            EndPaint();
            return false;
        }

        HBITMAP hTmpBitmap = CreateCompatibleBitmap(hParentDC, iWidth, iHeight);
        if (!hTmpBitmap)
        {
            DeleteDC(hTmpDC);
            ReleaseDC(NULL, hParentDC);
            EndPaint();
            return false;
        }

        HBITMAP hOldTmpBitmap = (HBITMAP)SelectObject(hTmpDC, hTmpBitmap);
        if (!hOldTmpBitmap)
        {
            DeleteObject(hTmpBitmap);
            DeleteDC(hTmpDC);
            ReleaseDC(NULL, hParentDC);
            EndPaint();
            return false;
        }

        BITMAP bm = { 0 };
        if (GetObject(hBitmap, sizeof(BITMAP), &bm))
        {
            StretchBlt(hTmpDC, 0, 0, iWidth, iHeight, hDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
        }
        else
        {
            SelectObject(hTmpDC, hOldTmpBitmap);
            DeleteObject(hTmpBitmap);
            DeleteDC(hTmpDC);
            ReleaseDC(NULL, hParentDC);
            EndPaint();
            return false;
        }

        SelectObject(hTmpDC, hOldTmpBitmap);
        //DeleteObject(hTmpBitmap);
        DeleteDC(hTmpDC);
        ReleaseDC(NULL, hParentDC);
        EndPaint();

        DeleteObject(hBitmap);

        hBitmap = hTmpBitmap;

        return true;
    }

    EndPaint();
    return false;
}
// [/BitMap::ScaleImage]


// [BitMap::Draw]:
bool BitMap::Draw(HDC hDC, int x, int y)
{
    if (!HasImage())
        return false;

    if (BeginPaint())
    {
        BITMAP bm = { 0 };
        if (GetObject(hBitmap, sizeof(BITMAP), &bm))
        {
            if (!BitBlt(hDC, x, y, bm.bmWidth, bm.bmHeight, this->hDC, 0, 0, SRCCOPY))
            {
                EndPaint();
                return false;
            }
        }
        else
        {
            EndPaint();
            return false;
        }

        EndPaint();
        return true;
    }
    return false;
}
// [/BitMap::Draw]


// [BitMap::DrawScalable]:
bool BitMap::DrawScalable(HDC hDC, int x, int y, int iMaxWidth, int iMaxHeight)
{
    BITMAP bm = { 0 };
    if (!GetObject(hBitmap, sizeof(BITMAP), &bm))
    {
        return false;
    }

    SIZE new_size = GetScalableSize(iMaxWidth, iMaxHeight);
    if (new_size.cx <= 0 || new_size.cy <= 0)
    {
        return false;
    }

    if (!BeginPaint())
    {
        return false;
    }

    if (!StretchBlt(hDC, x, y, new_size.cx, new_size.cy, this->hDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY))
    {
        EndPaint();
        return false;
    }

    EndPaint();
    return true;
}
// [/BitMap::DrawScalable]


// [BitMap::GetOriginalSize]:
SIZE BitMap::GetOriginalSize() const
{
    BITMAP bm = { 0 };
    if (GetObject(hBitmap, sizeof(BITMAP), &bm))
    {
        return { bm.bmWidth, bm.bmHeight };
    }
    return { 0, 0 };
}
// [/BitMap::GetOriginalSize]


// [BitMap::GetScalableSize]:
SIZE BitMap::GetScalableSize(int iMaxWidth, int iMaxHeight)
{
    BITMAP bm = { 0 };
    if (!GetObject(hBitmap, sizeof(BITMAP), &bm))
    {
        return { 0, 0 };
    }

    SIZE ret = { bm.bmWidth, bm.bmHeight };

    if (iMaxWidth <= 0 || iMaxHeight <= 0)
        return ret;

    if (bm.bmWidth > bm.bmHeight)
    {
        ret.cx = iMaxWidth;
        ret.cy = bm.bmHeight * iMaxWidth / bm.bmWidth;
        if (ret.cy > iMaxHeight)
        {
            ret.cx = ret.cx * iMaxHeight / ret.cy;
            ret.cy = iMaxHeight;
        }
    }
    else if (bm.bmHeight > bm.bmWidth)
    {
        ret.cx = bm.bmWidth * iMaxHeight / bm.bmHeight;
        ret.cy = iMaxHeight;
        if (ret.cx > iMaxWidth)
        {
            ret.cx = iMaxHeight;
            ret.cy = bm.bmHeight * iMaxWidth / ret.cx;
        }
    }
    else
    {
        if (iMaxWidth > iMaxHeight)
        {
            ret = { iMaxHeight, iMaxHeight };
        }
        else if (iMaxHeight > iMaxWidth)
        {
            ret = { iMaxWidth, iMaxWidth };
        }
        else
        {
            ret = { iMaxWidth, iMaxHeight };
        }
    }
    return ret;
}
// [/BitMap::GetScalableSize]


// [BitMap::SetPrivateVar]:
void BitMap::SetPrivateVar(LPVOID lpVar)
{
    lpPrivateVar = lpVar;
}
// [/BitMap::SetPrivateVar]


// [BitMap::GetPrivateVar]:
LPVOID BitMap::GetPrivateVar()
{
    return lpPrivateVar;
}
// [/BitMap::GetPrivateVar]
