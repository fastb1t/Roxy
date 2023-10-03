#include "RoxySetup.h"

#include <string>
#include <shlwapi.h>
#include <shlobj.h>

#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "comctl32.lib")


// [StringToWstring]:
static std::wstring StringToWstring(const std::string& input, DWORD locale = CP_ACP)
{
    // Default locale = CP_ACP

    std::wstring output(input.size(), 0);
    MultiByteToWideChar(locale, 0, input.c_str(), (int)input.length(), const_cast<wchar_t*> (output.data()), input.size());
    return output;
}
// [/StringToWstring]


// [CreateShortCut]:
static HRESULT CreateShortCut(const char* szExeFileName, const char* szShortCutFileName)
{
    IShellLinkA* psl = nullptr;
    HRESULT hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl);
    if (SUCCEEDED(hr))
    {
        psl->SetPath(szExeFileName);

        IPersistFile* ppf = nullptr;
        hr = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf);
        if (SUCCEEDED(hr))
        {
            ppf->Save(StringToWstring(szShortCutFileName).c_str(), TRUE);
            ppf->Release();
        }
        else
        {
            psl->Release();
            return hr;
        }
        psl->Release();
    }
    else
    {
        return hr;
    }
    return S_OK;
}
// [/CreateShortCut]


// [LastError]:
static DWORD LastError(HWND hWnd, const char* szFunction, DWORD dwLastError)
{
    if (szFunction == 0)
    {
        return dwLastError;
    }

    char* lpMessageBuffer = 0;
    int iMessageSize = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dwLastError,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&lpMessageBuffer,
        0,
        NULL);

    int iErrorMessageSize = iMessageSize + lstrlenA(szFunction) + 64;
    char* szErrorMessage = new char[iErrorMessageSize + 1];
    if (szErrorMessage != 0)
    {
        memset(szErrorMessage, 0, iErrorMessageSize + 1);
        wsprintfA(szErrorMessage, "%s failed with error %d: %s", szFunction, dwLastError, lpMessageBuffer);
        MessageBoxA(hWnd, szErrorMessage, "Error", MB_OK | MB_ICONERROR | MB_TOPMOST);
    }

    LocalFree(lpMessageBuffer);
    return dwLastError;
}
// [/LastError]


// [GetFileFromResource]:
static DWORD GetFileFromResource(HWND hWnd, HINSTANCE hInstance, int id, std::string sFile)
{
    DWORD dwLastError = 0;

    HRSRC hResource = FindResourceA(hInstance, MAKEINTRESOURCEA(id), MAKEINTRESOURCEA(RC_BINARY_TYPE));
    if (hResource == NULL)
    {
        return LastError(hWnd, "FindResourceA", GetLastError());
    }

    HGLOBAL hMemory = LoadResource(hInstance, hResource);
    if (hMemory == NULL)
    {
        return LastError(hWnd, "LoadResource", GetLastError());
    }

    DWORD dwSize = SizeofResource(hInstance, hResource);
    if (dwSize == 0)
    {
        dwLastError = GetLastError();
        FreeResource(hMemory);
        return LastError(hWnd, "SizeofResource", dwLastError);
    }

    LPVOID lpAddress = LockResource(hMemory);
    if (lpAddress == NULL)
    {
        dwLastError = GetLastError();
        FreeResource(hMemory);
        return LastError(hWnd, "LockResource", dwLastError);
    }

    HANDLE hFile = CreateFileA(sFile.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE)
    {
        DWORD dwWritten;
        if (!WriteFile(hFile, lpAddress, dwSize, &dwWritten, NULL))
        {
            dwLastError = GetLastError();
            CloseHandle(hFile);
            UnlockResource(lpAddress);
            FreeResource(hMemory);
            return LastError(hWnd, "WriteFileA", dwLastError);
        }
        CloseHandle(hFile);
    }
    else
    {
        dwLastError = GetLastError();
        UnlockResource(lpAddress);
        FreeResource(hMemory);
        return LastError(hWnd, "CreateFileA", dwLastError);
    }

    UnlockResource(lpAddress);
    FreeResource(hMemory);

    return ERROR_SUCCESS;
}
// [/GetFileFromResource]


// [UpdateResourceFileFromFile]:
static bool UpdateResourceFileFromFile(int type, int name, std::string sExeFile, std::string sFile)
{
    if (sExeFile.empty() || sFile.empty())
    {
        return false;
    }

    HANDLE hUpdateRes = BeginUpdateResourceA(sExeFile.c_str(), false);
    if (!hUpdateRes)
    {
        return false;
    }

    HANDLE hFile = CreateFileA(sFile.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile)
    {
        DWORD dwSize = GetFileSize(hFile, NULL);
        if (dwSize > 0)
        {
            char* szBuff = new char[dwSize + 1];
            if (szBuff)
            {
                DWORD dwReaded;
                if (!ReadFile(hFile, szBuff, dwSize, &dwReaded, NULL))
                {
                    delete[] szBuff;
                    CloseHandle(hFile);
                    EndUpdateResourceA(hUpdateRes, false);
                    return false;
                }

                if (dwReaded == 0)
                {
                    delete[] szBuff;
                    CloseHandle(hFile);
                    EndUpdateResourceA(hUpdateRes, false);
                    return false;
                }

                if (!UpdateResourceA(hUpdateRes, MAKEINTRESOURCEA(type), MAKEINTRESOURCEA(name),
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), (LPVOID)szBuff, dwSize))
                {
                    delete[] szBuff;
                    CloseHandle(hFile);
                    EndUpdateResourceA(hUpdateRes, false);
                    return false;
                }

                delete[] szBuff;
            }
            else
            {
                CloseHandle(hFile);
                EndUpdateResourceA(hUpdateRes, false);
                return false;
            }
        }
        else
        {
            CloseHandle(hFile);
            EndUpdateResourceA(hUpdateRes, false);
            return false;
        }
        CloseHandle(hFile);
    }
    else
    {
        EndUpdateResourceA(hUpdateRes, false);
        return false;
    }

    if (!EndUpdateResourceA(hUpdateRes, false))
    {
        return false;
    }

    return true;
}
// [/UpdateResourceFileFromFile]


// [UpdateResourceFileFromBuff]:
static bool UpdateResourceFileFromBuff(int type, int name, std::string sExeFile, std::string buff)
{
    HANDLE hUpdateRes = BeginUpdateResourceA(sExeFile.c_str(), false);
    if (!hUpdateRes)
    {
        return false;
    }

    if (!UpdateResourceA(hUpdateRes, MAKEINTRESOURCEA(type), MAKEINTRESOURCEA(name),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), (LPVOID)buff.data(), buff.size()))
    {
        EndUpdateResourceA(hUpdateRes, false);
        return false;
    }

    if (!EndUpdateResourceA(hUpdateRes, false))
    {
        return false;
    }

    return true;
}
// [/UpdateResourceFileFromBuff]
