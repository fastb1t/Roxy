#include "Algorithms.h"

#include <filesystem>
#include <shlwapi.h>

#pragma comment(lib, "shlwapi.lib")


// [Roxy::GetInstancePathA]:
std::string Roxy::GetInstancePathA(HINSTANCE hInstance)
{
    char szTemp[2048] = { 0 };
    if (GetModuleFileNameA(hInstance, szTemp, 2048) > 0)
    {
        PathRemoveFileSpecA(szTemp);
        return szTemp;
    }
    return "";
}
// [/Roxy::GetInstancePathA]


// [Roxy::GetInstancePathW]:
std::wstring Roxy::GetInstancePathW(HINSTANCE hInstance)
{
    wchar_t szTemp[2048] = { 0 };
    if (GetModuleFileNameW(hInstance, szTemp, 2048) > 0)
    {
        PathRemoveFileSpecW(szTemp);
        return szTemp;
    }
    return L"";
}
// [/Roxy::GetInstancePathW]


// [Roxy::BuildPathA]:
std::string Roxy::BuildPathA(const std::string& sPath1, const std::string& sPath2)
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
        return sPath1 + "\\" + sPath2;
    }

    if (*sPath1.rbegin() == '\\' && *sPath2.begin() == '\\')
    {
        return sPath1.substr(0, sPath1.size() - 1) + sPath2;
    }

    return sPath1 + sPath2;
}
// [/Roxy::BuildPathA]


// [Roxy::BuildPathW]:
std::wstring Roxy::BuildPathW(const std::wstring& sPath1, const std::wstring& sPath2)
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
        return sPath1 + L"\\" + sPath2;
    }

    if (*sPath1.rbegin() == '\\' && *sPath2.begin() == '\\')
    {
        return sPath1.substr(0, sPath1.size() - 1) + sPath2;
    }

    return sPath1 + sPath2;
}
// [/Roxy::BuildPathW]


// [Roxy::FileExistsA]:
bool Roxy::FileExistsA(const std::string& sFileName)
{
    if (sFileName.empty())
    {
        return false;
    }

    WIN32_FIND_DATAA wfd;
    memset(&wfd, 0, sizeof(WIN32_FIND_DATAA));

    HANDLE hFile = FindFirstFileA(sFileName.c_str(), &wfd);
    if (hFile != INVALID_HANDLE_VALUE)
    {
        FindClose(hFile);
        return true;
    }
    return false;
}
// [/Roxy::FileExistsA]


// [Roxy::FileExistsW]:
bool Roxy::FileExistsW(const std::wstring& sFileName)
{
    if (sFileName.empty())
    {
        return false;
    }

    WIN32_FIND_DATAW wfd;
    memset(&wfd, 0, sizeof(WIN32_FIND_DATAW));

    HANDLE hFile = FindFirstFileW(sFileName.c_str(), &wfd);
    if (hFile != INVALID_HANDLE_VALUE)
    {
        FindClose(hFile);
        return true;
    }
    return false;
}
// [/Roxy::FileExistsW]:


// [Roxy::DirExistsA]:
bool Roxy::DirExistsA(const std::string& sDirName)
{
    if (sDirName.empty())
    {
        return false;
    }

    DWORD dwAttr = GetFileAttributesA(sDirName.c_str());

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
// [/Roxy::DirExistsA]


// [Roxy::DirExistsW]:
bool Roxy::DirExistsW(const std::wstring& sDirName)
{
    if (sDirName.empty())
    {
        return false;
    }

    DWORD dwAttr = GetFileAttributesW(sDirName.c_str());

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
// [/Roxy::DirExistsW]


// [Roxy::GetFileNameFromPathA]:
std::string Roxy::GetFileNameFromPathA(const std::string& sPath)
{

}
// [/Roxy::GetFileNameFromPathA]


// [Roxy::RemoveFileNameFromPathW]:
std::wstring Roxy::RemoveFileNameFromPathW(const std::wstring& sPath)
{

}
// [/Roxy::RemoveFileNameFromPathW]


// [Roxy::GetPathFromPathA]:
std::string Roxy::GetPathFromPathA(const std::string& sPath)
{

}
// [/Roxy::GetPathFromPathA]


// [Roxy::RemovePathFromPathW]:
std::wstring Roxy::RemovePathFromPathW(const std::wstring& sPath)
{

}
// [/Roxy::RemovePathFromPathW]


// [Roxy::GetFileExtensionA]:
std::string Roxy::GetFileExtensionA(const std::string& sFileName)
{
    char* lpszStr = PathFindExtensionA(sFileName.c_str());
    return (lpszStr == nullptr ? "" : lpszStr);
}
// [/Roxy::GetFileExtensionA]


// [Roxy::GetFileExtensionW]:
std::wstring Roxy::GetFileExtensionW(const std::wstring& sFileName)
{
    wchar_t* lpszStr = PathFindExtensionW(sFileName.c_str());
    return (lpszStr == nullptr ? L"" : lpszStr);
}
// [/Roxy::GetFileExtensionW]


// [Roxy::RemoveFileExtensionA]:
std::string Roxy::RemoveFileExtensionA(const std::string& sFileName)
{

}
// [/Roxy::RemoveFileExtensionA]


// [Roxy::RemoveFileExtensionW]:
std::wstring Roxy::RemoveFileExtensionW(const std::wstring& sFileName)
{

}
// [/Roxy::RemoveFileExtensionW]


// [Roxy::GetTextFromEditA]:
std::string Roxy::GetTextFromEditA(HWND hEdit)
{
    std::string sResult = "";

    if (hEdit != NULL)
    {
        int iLength = GetWindowTextLengthA(hEdit);
        if (iLength > 0)
        {
            char* pszText = new (std::nothrow) char[iLength + 2];
            if (pszText != nullptr)
            {
                memset(pszText, 0, iLength + 2);

                if (GetWindowTextA(hEdit, pszText, iLength + 1) > 0)
                {
                    sResult = pszText;
                }

                delete[] pszText;
            }
        }
    }

    return sResult;
}
// [/Roxy::GetTextFromEditA]


// [Roxy::GetTextFromEditW]:
std::wstring Roxy::GetTextFromEditW(HWND hEdit)
{
    std::wstring sResult = L"";

    if (hEdit != NULL)
    {
        int iLength = GetWindowTextLengthW(hEdit);
        if (iLength > 0)
        {
            wchar_t* pszText = new (std::nothrow) wchar_t[iLength + 2];
            if (pszText != nullptr)
            {
                memset(pszText, 0, iLength + 2);

                if (GetWindowTextW(hEdit, pszText, iLength + 1) > 0)
                {
                    sResult = pszText;
                }

                delete[] pszText;
            }
        }
    }

    return sResult;
}
// [/Roxy::GetTextFromEditW]


// [Roxy::TextHasNumberA]:
bool Roxy::TextHasNumberA(const std::string sText)
{
    if (sText.length() > 0)
    {
        if (sText[0] == '0')
        {
            return false;
        }

        return sText.find_first_not_of("0123456789") == std::string::npos;
    }
    return false;
}
// [/Roxy::TextHasNumberA]


// [Roxy::TextHasNumberW]:
bool Roxy::TextHasNumberW(const std::wstring sText)
{
    if (sText.length() > 0)
    {
        if (sText[0] == '0')
        {
            return false;
        }

        return sText.find_first_not_of(L"0123456789") == std::string::npos;
    }
    return false;
}
// [/Roxy::TextHasNumberW]


// [Roxy::StringToWstring]:
std::wstring Roxy::StringToWstring(const std::string& input, DWORD locale)
{
    // Default locale = CP_ACP

    std::wstring output(input.size(), 0);
    MultiByteToWideChar(locale, 0, input.c_str(), (int)input.length(), const_cast<wchar_t*> (output.data()), input.size());
    return output;
}
// [/Roxy::StringToWstring]


// [Roxy::GetRandomStringA]:
std::string Roxy::GetRandomStringA(size_t length)
{
    std::string str;

    std::string base = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

    while (length--)
    {
        str += base[rand() % base.length()];
    }

    return str;
}
// [/Roxy::GetRandomStringA]


// [Roxy::GetRandomStringW]:
std::wstring Roxy::GetRandomStringW(size_t length)
{
    std::wstring str;

    std::wstring base = L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

    while (length--)
    {
        str += base[rand() % base.length()];
    }

    return str;
}
// [/Roxy::GetRandomStringW]
