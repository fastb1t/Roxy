#ifndef _ROXY_SURFACE_H_
#define _ROXY_SURFACE_H_

#include <Windows.h>

namespace Roxy {

    class Surface {
        HDC m_hDC;
        HBITMAP m_hBitmap;
        HBITMAP m_hOldBitmap;
        size_t m_width;
        size_t m_height;

    public:
        Surface();
        ~Surface();

        bool HasImage() const {
            return (m_hDC != NULL && m_width > 0 && m_height > 0);
        }

        bool New(size_t width, size_t height, HDC hParentDC = NULL);
        void Delete();

        bool Draw(HDC hDC, int x, int y) const;

        void DrawLine(int x1, int y1, int x2, int y2) {
            if (HasImage())
            {
                MoveToEx(m_hDC, x1, y1, NULL);
                LineTo(m_hDC, x2, y2);
            }
        }
    };

};

#endif // !_ROXY_SURFACE_H_
