#ifndef _ROXY_SURFACE_H_
#define _ROXY_SURFACE_H_

#include <Windows.h>

namespace Roxy {

    class Surface {
        HDC m_hDC;
        size_t m_width;
        size_t m_height;

    public:
        Surface();
        ~Surface();

        bool New(size_t width, size_t height, HDC hParentDC = NULL);
        void Remove();

        bool Draw(HDC hDC, int x, int y);
    };

};

#endif
