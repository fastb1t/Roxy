#ifndef _ROXY_ALGORITHMS_H_
#define _ROXY_ALGORITHMS_H_

#include <string>
#include <vector>
#include <Windows.h>

namespace Roxy {

    std::wstring StringToWstring(const std::string& input, DWORD locale = CP_ACP);

    std::string GetInstancePathA(HINSTANCE hInstance);
    std::wstring GetInstancePathW(HINSTANCE hInstance);

    std::string BuildPathA(const std::string& sPath1, const std::string& sPath2);
    std::wstring BuildPathW(const std::wstring& sPath1, const std::wstring& sPath2);

    bool FileExistsA(const std::string& sFileName);
    bool FileExistsW(const std::wstring& sFileName);

    bool DirExistsA(const std::string& sDirName);
    bool DirExistsW(const std::wstring& sDirName);

    std::string GetFileNameFromPathA(const std::string& sPath);
    std::wstring GetFileNameFromPathW(const std::wstring& sPath);

    std::string RemoveFileNameFromPathA(const std::string& sPath);
    std::wstring RemoveFileNameFromPathW(const std::wstring& sPath);

    std::string GetPathFromPathA(const std::string& sPath);
    std::wstring GetPathFromPathW(const std::wstring& sPath);

    std::string RemovePathFromPathA(const std::string& sPath);
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


    std::vector<std::string> FindFilesA(const std::string& sPath, const std::string& sExtension, bool bFullPath = true);
    std::vector<std::wstring> FindFilesW(const std::wstring& sPath, const std::wstring& sExtension, bool bFullPath = true);

    void DeleteFilesA(const std::string& sPath, const std::string& sExtension);
    void DeleteFilesW(const std::wstring& sPath, const std::wstring& sExtension);

    size_t CountFilesA(const std::string& sPath, const std::string& sExtension);
    size_t CountFilesW(const std::wstring& sPath, const std::wstring& sExtension);

    std::vector<std::string> CopyFilesA(const std::string& sPath, const std::string& sDestPath, const std::string& sOldExtension, const std::string& sNewExtension);
    std::vector<std::wstring> CopyFilesW(const std::wstring& sPath, const std::wstring& sDestPath, const std::wstring& sOldExtension, const std::wstring& sNewExtension);

#ifdef UNICODE
#define GetInstancePath GetInstancePathW
#define BuildPath BuildPathW
#define FileExists FileExistsW
#define DirExists DirExistsW
#define GetFileNameFromPath GetFileNameFromPathW
#define RemoveFileNameFromPath RemoveFileNameFromPathW
#define GetPathFromPath GetPathFromPathW
#define RemovePathFromPath RemovePathFromPathW
#define GetFileExtension GetFileExtensionW
#define RemoveFileExtension RemoveFileExtensionW

#define GetTextFromEdit GetTextFromEditW
#define TextHasNumber TextHasNumberW
#define GetRandomString GetRandomStringW

#define FindFiles FindFilesW
#define DeleteFiles DeleteFilesW
#define CountFiles CountFilesW
#define CopyFiles CopyFilesW
#else
#define GetInstancePath GetInstancePathA
#define BuildPath BuildPathA
#define FileExists FileExistsA
#define DirExists DirExistsA
#define GetFileNameFromPath GetFileNameFromPathA
#define RemoveFileNameFromPath RemoveFileNameFromPathA
#define GetPathFromPath GetPathFromPathA
#define RemovePathFromPath RemovePathFromPathA
#define GetFileExtension GetFileExtensionA
#define RemoveFileExtension RemoveFileExtensionA

#define GetTextFromEdit GetTextFromEditA
#define TexHassNumber TextHasNumberA
#define GetRandomString GetRandomStringA

#define FindFiles FindFilesA
#define DeleteFiles DeleteFilesA
#define CountFiles CountFilesA
#define CopyFiles CopyFilesA
#endif

}

#endif // !_ROXY_ALGORITHMS_H_
