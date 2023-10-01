#ifndef _ROXY_ALGORITHMS_H_
#define _ROXY_ALGORITHMS_H_

#include "Types.h"

#include <Windows.h>

namespace Roxy {

    std::string GetInstancePathA(HINSTANCE hInstance);
    std::wstring GetInstancePathW(HINSTANCE hInstance);

    std::string BuildPathA(const std::string& sPath1, const std::string& sPath2);
    std::wstring BuildPathW(const std::wstring& sPath1, const std::wstring& sPath2);

    bool FileExistsA(const std::string& sFileName);
    bool FileExistsW(const std::wstring& sFileName);

    bool DirExistsA(const std::string& sDirName);
    bool DirExistsW(const std::wstring& sDirName);

    std::string GetFileNameFromPathA(const std::string& sPath);
    std::wstring RemoveFileNameFromPathW(const std::wstring& sPath);

    std::string GetPathFromPathA(const std::string& sPath);
    std::wstring RemovePathFromPathW(const std::wstring& sPath);

    std::string GetFileExtensionA(const std::string& sFileName);
    std::wstring GetFileExtensionW(const std::wstring& sFileName);

    std::string RemoveFileExtensionA(const std::string& sFileName);
    std::wstring RemoveFileExtensionW(const std::wstring& sFileName);


    std::string GetTextFromEditA(HWND hEdit);
    std::wstring GetTextFromEditW(HWND hEdit);

    bool TextHasNumberA(const std::string sText);
    bool TextHasNumberW(const std::wstring sText);

    std::string GetRandomStringA(size_t length = 6);
    std::wstring GetRandomStringW(size_t length = 6);


    std::wstring StringToWstring(const std::string& input, DWORD locale = CP_ACP);

#ifdef UNICODE
#define GetInstancePath GetInstancePathW
#define BuildPath BuildPathW
#define FileExists FileExistsW
#define DirExists DirExistsW
#define GetFileNameFromPath GetFileNameFromPathW
#define GetPathFromPath GetPathFromPathW
#define GetFileExtension GetFileExtensionW
#define RemoveFileExtension RemoveFileExtensionW

#define GetTextFromEdit GetTextFromEditW
#define TextHasNumber TextHasNumberW
#define GetRandomString GetRandomStringW
#else
#define GetInstancePath GetInstancePathA
#define BuildPath BuildPathA
#define FileExists FileExistsA
#define DirExists DirExistsA
#define GetFileNameFromPath GetFileNameFromPathA
#define GetPathFromPath GetPathFromPathA
#define GetFileExtension GetFileExtensionA
#define RemoveFileExtension RemoveFileExtensionA

#define GetTextFromEdit GetTextFromEditA
#define TexHassNumber TextHasNumberA
#define GetRandomString GetRandomStringA
#endif

}

#endif // !_ROXY_ALGORITHMS_H_
