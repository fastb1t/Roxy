#ifndef _ROXY_BITMAP_H_
#define _ROXY_BITMAP_H_

/***
 *  Version: 1.0
 *  Author: fastb1t
 *  Date: 01.11.2019 23:54
***/

#include <windows.h>

namespace Roxy {

    class BitMap {
        HBITMAP hBitmap;
        HBITMAP hOldBitmap;
        HDC hDC;
        bool bPaintInit;

        LPVOID lpPrivateVar;

    public:
        BitMap();
        ~BitMap();

        void Clear();

        bool BeginPaint();
        bool EndPaint();

        bool SetBitmapHandle(HBITMAP hBitmap);
        HBITMAP GetBitmapHandle() const;
        HDC GetDCHandle() const;
        bool HasImage() const;

        bool CreateNewBitmap(int iWidth, int iHeight);
        bool ReplaceColors(COLORREF clrOldColor, COLORREF clrNewColor);
        bool MakeScreenShot(HWND hWnd, RECT* rect = nullptr);
        bool ScaleImage(int iWidth, int iHeight);

        bool Draw(HDC hDC, int x, int y);
        bool DrawScalable(HDC hDC, int x, int y, int iMaxWidth, int iMaxHeight);

        SIZE GetOriginalSize() const;
        SIZE GetScalableSize(int iMaxWidth, int iMaxHeight);

        void SetPrivateVar(LPVOID lpVar);
        LPVOID GetPrivateVar();
    };

}

#endif // !_ROXY_BITMAP_H_
