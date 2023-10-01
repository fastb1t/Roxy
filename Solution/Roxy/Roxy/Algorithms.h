#ifndef _ROXY_ALGORITHMS_H_
#define _ROXY_ALGORITHMS_H_

#include "Types.h"

#include <Windows.h>

namespace Roxy {

    String GetRandomString(size_t length = 6);


    bool TextHasNumber(const String& sText);

    String GetFileNameFromPath(const String& sPath);
    String RemoveFileNameFromPath(const String& sPath);

    String GetPathFromPath(const String& sPath);
    String RemovePathFromPath(const String& sPath);

    //String GetFileExtension(const String& sFileName);
    String RemoveFileExtension(const String& sFileName);



    std::string GetInstancePathA(HINSTANCE hInstance);
    std::wstring GetInstancePathW(HINSTANCE hInstance);

    std::string BuildPathA(const std::string& sPath1, const std::string& sPath2);
    std::wstring BuildPathW(const std::wstring& sPath1, const std::wstring& sPath2);

    bool FileExistsA(const std::string& sFileName);
    bool FileExistsW(const std::wstring& sFileName);

    bool DirExistsA(const std::string& sDirName);
    bool DirExistsW(const std::wstring& sDirName);

    std::string GetFileExtensionA(const std::string& sFileName);
    std::wstring GetFileExtensionW(const std::wstring& sFileName);

    std::string GetTextFromEditA(HWND hEdit);
    std::wstring GetTextFromEditW(HWND hEdit);

    bool TextIsNumberA(const std::string sText);
    bool TextIsNumberW(const std::wstring sText);

    std::wstring StringToWstring(const std::string& input, DWORD locale = CP_ACP);

#ifdef UNICODE
#define GetInstancePath GetInstancePathW
#define BuildPath BuildPathW
#define FileExists FileExistsW
#define DirExists DirExistsW
#define GetFileExtension GetFileExtensionW
#define GetTextFromEdit GetTextFromEditW
#define TextIsNumber TextIsNumberW
#else
#define GetInstancePath GetInstancePathA
#define BuildPath BuildPathA
#define FileExists FileExistsA
#define DirExists DirExistsA
#define GetFileExtension GetFileExtensionA
#define GetTextFromEdit GetTextFromEditA
#define TextIsNumber TextIsNumberA
#endif

}

#endif // !_ROXY_ALGORITHMS_H_
