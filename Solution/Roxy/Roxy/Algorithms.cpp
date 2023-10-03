#include "Algorithms.h"

#include <filesystem>
#include <shlwapi.h>

#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "comctl32.lib")


// [Roxy::StringToWstring]:
std::wstring Roxy::StringToWstring(const std::string& input, DWORD locale)
{
    // Default locale = CP_ACP

    std::wstring output(input.size(), 0);
    MultiByteToWideChar(locale, 0, input.c_str(), (int)input.length(), const_cast<wchar_t*> (output.data()), input.size());
    return output;
}
// [/Roxy::StringToWstring]


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


// [Roxy::FindFilesA]:
std::vector<std::string> Roxy::FindFilesA(const std::string& sPath, const std::string& sExtension, bool bFullPath)
{
    std::vector<std::string> list;

    WIN32_FIND_DATAA wfd;
    memset(&wfd, 0, sizeof(WIN32_FIND_DATAA));

    std::string sMask = "";

    sMask += "*.";
    sMask += sExtension;

    HANDLE hFile = FindFirstFileA(BuildPathA(sPath, sMask).c_str(), &wfd);
    if (hFile)
    {
        do {
            if (bFullPath)
            {
                list.push_back(BuildPathA(sPath, wfd.cFileName));
            }
            else
            {
                list.push_back(wfd.cFileName);
            }
        } while (FindNextFileA(hFile, &wfd));

        FindClose(hFile);
    }

    return list;
}
// [/Roxy::FindFilesA]


// [Roxy::FindFilesW]:
std::vector<std::wstring> Roxy::FindFilesW(const std::wstring& sPath, const std::wstring& sExtension, bool bFullPath)
{
    std::vector<std::wstring> list;

    WIN32_FIND_DATAW wfd;
    memset(&wfd, 0, sizeof(WIN32_FIND_DATAW));

    std::wstring sMask = L"";

    sMask += L"*.";
    sMask += sExtension;

    HANDLE hFile = FindFirstFileW(BuildPathW(sPath, sMask).c_str(), &wfd);
    if (hFile)
    {
        do {
            if (bFullPath)
            {
                list.push_back(BuildPathW(sPath, wfd.cFileName));
            }
            else
            {
                list.push_back(wfd.cFileName);
            }
        } while (FindNextFileW(hFile, &wfd));

        FindClose(hFile);
    }

    return list;
}
// [/Roxy::FindFilesW]


// [Roxy::DeleteFilesA]:
void Roxy::DeleteFilesA(const std::string& sPath, const std::string& sExtension)
{
    std::vector<std::string> files = FindFilesA(sPath, sExtension);

    for (std::vector<std::string>::const_iterator it = files.begin(); it != files.end(); it++)
    {
        DeleteFileA((*it).c_str());
    }
}
// [/Roxy::DeleteFilesA]


// [Roxy::DeleteFilesW]:
void Roxy::DeleteFilesW(const std::wstring& sPath, const std::wstring& sExtension)
{
    std::vector<std::wstring> files = FindFilesW(sPath, sExtension);

    for (std::vector<std::wstring>::const_iterator it = files.begin(); it != files.end(); it++)
    {
        DeleteFileW((*it).c_str());
    }
}
// [/Roxy::DeleteFilesW]


// [Roxy::CountFilesA]:
size_t Roxy::CountFilesA(const std::string& sPath, const std::string& sExtension)
{
    return FindFilesA(sPath, sExtension).size();
}
// [/Roxy::CountFilesA]


// [Roxy::CountFilesW]:
size_t Roxy::CountFilesW(const std::wstring& sPath, const std::wstring& sExtension)
{
    return FindFilesW(sPath, sExtension).size();
}
// [/Roxy::CountFilesW]


// [Roxy::CopyFilesA]:
std::vector<std::string> Roxy::CopyFilesA(const std::string& sPath, const std::string& sDestPath, const std::string& sOldExtension, const std::string& sNewExtension)
{
    std::vector<std::string> list;

    WIN32_FIND_DATAA wfd;
    memset(&wfd, 0, sizeof(WIN32_FIND_DATAA));

    HANDLE hFile = FindFirstFileA(BuildPathA(sPath, "*." + sOldExtension).c_str(), &wfd);
    if (hFile)
    {
        std::string sFileName;

        do {
            sFileName = wfd.cFileName;

            if (sFileName.length() > max(sOldExtension.length(), sNewExtension.length()))
            {
                if (GetFileExtensionA(sFileName) == std::string(".") + sOldExtension)
                {
                    sFileName.erase(sFileName.begin() + sFileName.length() - sOldExtension.length(), sFileName.end());
                    sFileName += sNewExtension;

                    DeleteFileA(BuildPathA(sDestPath, sFileName).c_str());

                    CopyFileA(BuildPathA(sPath, wfd.cFileName).c_str(), BuildPathA(sDestPath, sFileName).c_str(), FALSE);

                    list.push_back(BuildPathA(sDestPath, sFileName));
                }
            }
        } while (FindNextFileA(hFile, &wfd));

        FindClose(hFile);
    }

    return list;
}
// [/Roxy::CopyFilesA]


// [Roxy::CopyFilesW]:
std::vector<std::wstring> Roxy::CopyFilesW(const std::wstring& sPath, const std::wstring& sDestPath, const std::wstring& sOldExtension, const std::wstring& sNewExtension)
{
    std::vector<std::wstring> list;

    WIN32_FIND_DATAW wfd;
    memset(&wfd, 0, sizeof(WIN32_FIND_DATAW));

    HANDLE hFile = FindFirstFileW(BuildPathW(sPath, L"*." + sOldExtension).c_str(), &wfd);
    if (hFile)
    {
        std::wstring sFileName;

        do {
            sFileName = wfd.cFileName;

            if (sFileName.length() > max(sOldExtension.length(), sNewExtension.length()))
            {
                if (GetFileExtensionW(sFileName) == std::wstring(L".") + sOldExtension)
                {
                    sFileName.erase(sFileName.begin() + sFileName.length() - sOldExtension.length(), sFileName.end());
                    sFileName += sNewExtension;

                    DeleteFileW(BuildPathW(sDestPath, sFileName).c_str());

                    CopyFileW(BuildPathW(sPath, wfd.cFileName).c_str(), BuildPathW(sDestPath, sFileName).c_str(), FALSE);

                    list.push_back(BuildPathW(sDestPath, sFileName));
                }
            }
        } while (FindNextFileW(hFile, &wfd));

        FindClose(hFile);
    }

    return list;
}
// [/Roxy::CopyFilesW]
