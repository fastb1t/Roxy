#ifndef _ROXY_ALGORITHMS_H_
#define _ROXY_ALGORITHMS_H_

#include <tchar.h>
#include <xstring>
#include <windows.h>

namespace Roxy {
    typedef std::basic_string<TCHAR, std::char_traits<TCHAR>, std::allocator<TCHAR>> String;

    String getRandomString(size_t length = 6);
}

#endif
