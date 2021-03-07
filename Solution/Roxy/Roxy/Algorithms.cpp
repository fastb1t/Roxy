#include "Algorithms.h"

#include <shlwapi.h>

#pragma comment(lib, "shlwapi.lib")


// [Roxy::GetRandomString]:
Roxy::String Roxy::GetRandomString(size_t length)
{
    String str;

    String base = _T("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_");

    while (length--)
    {
        str += base[rand() % base.length()];
    }

    return str;
}
// [/Roxy::GetRandomString]


// [Roxy::TextHasNumber]:
bool Roxy::TextHasNumber(const String& sText)
{
    if (sText.length() > 0)
    {
        if (sText[0] == '0')
        {
            return false;
        }

        return sText.find_first_not_of(_T("0123456789")) == std::string::npos;
    }
    return false;
}
// [/Roxy::TextHasNumber]


// [Roxy::GetPath]:
Roxy::String Roxy::GetPath(HINSTANCE hInstance)
{
    TCHAR szTemp[2048] = { 0 };
    if (GetModuleFileName(hInstance, szTemp, 2048) > 0)
    {
        PathRemoveFileSpec(szTemp);
        return szTemp;
    }
    return _T("");
}
// [/Roxy::GetPath]


// [Roxy::BuildPath]:
Roxy::String Roxy::BuildPath(const String& sPath1, const String& sPath2)
{
    if (sPath1.empty())
    {
        return sPath2;
    }

    if (sPath2.empty())
    {
        return sPath1;
    }

    if (*sPath1.rbegin() != '\\' && *sPath2.begin() != '\\')
    {
        return sPath1 + _T("\\") + sPath2;
    }

    if (*sPath1.rbegin() == '\\' && *sPath2.begin() == '\\')
    {
        return sPath1.substr(0, sPath1.size() - 1) + sPath2;
    }

    return sPath1 + sPath2;
}
// [/Roxy::BuildPath]


// [Roxy::FileExists]:
bool Roxy::FileExists(const String& sFile)
{
    if (sFile.empty())
    {
        return false;
    }

    WIN32_FIND_DATA wfd;
    HANDLE hFile = FindFirstFile(sFile.c_str(), &wfd);
    if (hFile != INVALID_HANDLE_VALUE)
    {
        FindClose(hFile);
        return true;
    }
    return false;
}
// [/Roxy::FileExists]


// [Roxy::DirExists]:
bool Roxy::DirExists(const String& sDir)
{
    DWORD dwAttr = GetFileAttributes(sDir.c_str());

    if (dwAttr == INVALID_FILE_ATTRIBUTES)
    {
        return false;
    }

    if (dwAttr & FILE_ATTRIBUTE_DIRECTORY)
    {
        return true;
    }

    return false;
}
// [/Roxy::DirExists]


// [Roxy::GetFileNameFromPath]:
Roxy::String Roxy::GetFileNameFromPath(const String& sPath)
{
    String str;

    return str;
}
// [/Roxy::GetFileNameFromPath]


// [Roxy::RemoveFileNameFromPath]:
Roxy::String Roxy::RemoveFileNameFromPath(const String& sPath)
{
    String str;

    return str;
}
// [/Roxy::RemoveFileNameFromPath]


// [Roxy::GetPathFromPath]:
Roxy::String Roxy::GetPathFromPath(const String& sPath)
{
    String str;

    return str;
}
// [/Roxy::GetPathFromPath]


// [Roxy::RemovePathFromPath]:
Roxy::String Roxy::RemovePathFromPath(const String& sPath)
{
    String str;

    return str;
}
// [/Roxy::RemovePathFromPath]


// [Roxy::GetFileExtension]:
Roxy::String Roxy::GetFileExtension(const String& sFileName)
{
    String str;

    return str;
}
// [/Roxy::GetFileExtension]


// [Roxy::RemoveFileExtension]:
Roxy::String Roxy::RemoveFileExtension(const String& sFileName)
{
    char* lpszStr = PathFindExtension(sFileName.c_str());
    return (lpszStr == nullptr ? "" : lpszStr);

    String str;

    return str;
}
// [/Roxy::RemoveFileExtension]
