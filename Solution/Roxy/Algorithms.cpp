#include "Algorithms.h"

// [getRandomString]:
String getRandomString(size_t length)
{
    TCHAR buff[] = {
        'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'z', 'x', 'c', 'v', 'b', 'n', 'm',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        '-', '_'
    };

    String str;
    while (length--)
    {
        str += buff[rand() % (sizeof(buff) / sizeof(buff[0]))];
    }
    return str;
}
// [/getRandomString]
