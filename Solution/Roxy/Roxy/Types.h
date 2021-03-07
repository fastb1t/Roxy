#ifndef _ROXY_TYPES_H_
#define _ROXY_TYPES_H_

#include <tchar.h>
#include <string>

namespace Roxy {
    typedef std::basic_string<TCHAR, std::char_traits<TCHAR>, std::allocator<TCHAR>> String;
    typedef std::basic_stringstream<TCHAR, std::char_traits<TCHAR>, std::allocator<TCHAR>> StringStream;
    typedef std::basic_istringstream<TCHAR, std::char_traits<TCHAR>, std::allocator<TCHAR>> IStringStream;
    typedef std::basic_ostringstream<TCHAR, std::char_traits<TCHAR>, std::allocator<TCHAR>> OStringStream;
    typedef std::basic_fstream<TCHAR, std::char_traits<TCHAR>> FileStream;
    typedef std::basic_ifstream<TCHAR, std::char_traits<TCHAR>> IFileStream;
    typedef std::basic_ofstream<TCHAR, std::char_traits<TCHAR>> OFileStream;
}

#endif
