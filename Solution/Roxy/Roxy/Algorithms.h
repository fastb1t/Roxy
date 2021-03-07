#ifndef _ROXY_ALGORITHMS_H_
#define _ROXY_ALGORITHMS_H_

#include "Types.h"

#include <Windows.h>

namespace Roxy {

    String GetRandomString(size_t length = 6);


    bool TextHasNumber(const String& sText);


    String GetPath(HINSTANCE hInstance);
    String BuildPath(const String& sPath1, const String& sPath2);
    bool FileExists(const String& sFile);
    bool DirExists(const String& sDir);

    String GetFileNameFromPath(const String& sPath);
    String RemoveFileNameFromPath(const String& sPath);

    String GetPathFromPath(const String& sPath);
    String RemovePathFromPath(const String& sPath);

    String GetFileExtension(const String& sFileName);
    String RemoveFileExtension(const String& sFileName);

}

#endif
