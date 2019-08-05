#pragma once

#include <tchar.h>
#include <xstring>
#include <windows.h>

typedef std::basic_string<TCHAR, std::char_traits<TCHAR>, std::allocator<TCHAR>> String;

String getRandomString(size_t length = 6);
