#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#include <winstring.h>
#include <sddl.h>
#include <Shlwapi.h>
#include <vector>
#include <spdlog/spdlog.h>

#define Hook(a,b) DetourTransactionBegin(); \
if (a) \
{DetourAttach(&(PVOID&)a, b);} \
DetourTransactionCommit();

inline HRESULT(__stdcall* fWindowsCreateString)(PCWSTR sourceString,UINT32 length,HSTRING* string);
inline PCWSTR(__stdcall* fWindowsGetStringRawBuffer)(HSTRING string, UINT32* length);
inline HRESULT(__stdcall* fWindowsDeleteString)(HSTRING string);

namespace globals
{
    inline void* ConsoleUIView;
    inline __int64(__fastcall* ConsoleUIView__Initialize)(void* _this);
    inline __int64(__fastcall* ConsoleUIView__HandleKeyInput)(void* _this, const _KEY_EVENT_RECORD* a2);

    inline bool wasInSelectedCredentialView;
};

static std::string ws2s(const std::wstring& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, 0, 0, 0, 0);
    char* buf = new char[len];
    WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, buf, len, 0, 0);
    std::string r(buf);
    delete[] buf;
    return r;
}

static std::wstring s2ws(const std::string& s)
{
    const int len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), -1, NULL, 0);
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), -1, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}

static std::vector<std::string> split(std::string s, std::string delimiter)
{
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}

static std::vector<std::wstring> split(std::wstring s, std::wstring delimiter)
{
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::wstring token;
    std::vector<std::wstring> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}

static std::wstring ConvertHStringToString(HSTRING string)
{
    const wchar_t* convertedString = fWindowsGetStringRawBuffer(string, 0);
    return convertedString;
}

static const wchar_t* ConvertHStringToRawString(HSTRING string)
{
    const wchar_t* convertedString = fWindowsGetStringRawBuffer(string, 0);
    return convertedString;
}

inline bool bLogonConsoleShown = true;
static void MinimizeLogonConsole()
{
    auto consoleWindow = FindWindowW(0, L"C:\\Windows\\system32\\LogonUI.exe");
    if (!consoleWindow) return;

    //ShowWindow(consoleWindow, SW_FORCEMINIMIZE);
    bLogonConsoleShown = false;
    ShowWindow(consoleWindow, SW_HIDE);
    //ShowWindow(consoleWindow, SW_RESTORE);
}

static void ShowLogonConsole()
{
    auto consoleWindow = FindWindowW(0, L"C:\\Windows\\system32\\LogonUI.exe");
    if (!consoleWindow) return;

    bLogonConsoleShown = true;
    ShowWindow(consoleWindow, SW_SHOW);
}

static void* GetVirtualFunctionFromTable(void* vtable, int index)
{
    uintptr_t* func = (uintptr_t*)((uintptr_t)vtable + index);

    return reinterpret_cast<void*>(*func);
}

//taken from win7 sdk samples
//https://github.com/Ippei-Murofushi/WindowsSDK7-Samples/blob/e8fe83b043727e71f5179da11fc6228475e7973c/security/parentalcontrols/utilities/Utilities.cpp#L85
__declspec(noinline) static HRESULT GetSIDStringFromUsername(PCWSTR pcszUserName, PWSTR* ppszSID)
{
    HRESULT hr = E_INVALIDARG;

    if (pcszUserName && ppszSID)
    {
        DWORD cbSID = 0, cchDomain = 0;
        SID_NAME_USE SidNameUse;

        // Call twice, first with null SID buffer.  Retrieves required buffer
        //  size for domain name
        LookupAccountNameW(NULL, pcszUserName, NULL, &cbSID, NULL,
            &cchDomain, &SidNameUse);
        if (!cbSID || !cchDomain)
        {
            hr = E_FAIL;
        }
        else
        {
            WCHAR* pszDomain = NULL;
            // Allocate properly sized buffer (with termination character) 
            //  for domain name
            pszDomain = new WCHAR[cchDomain];
            if (!pszDomain)
            {
                hr = E_OUTOFMEMORY;
            }
            else
            {
                // Allocate properly sized buffer (with termination character)
                //  for PSID
                PSID pSID = static_cast<PSID> (new BYTE[cbSID]); //(LocalAlloc (LMEM_FIXED, cbSID));
                if (!pSID)
                {
                    hr = E_OUTOFMEMORY;
                }
                else
                {
                    // Second call with buffers allocated and sizes set
                    if (!LookupAccountName(NULL, pcszUserName, pSID, &cbSID,
                        pszDomain, &cchDomain, &SidNameUse))
                    {
                        hr = HRESULT_FROM_WIN32(GetLastError());
                    }
                    else
                    {
                        // Convert PSID to SID string
                        if (!ConvertSidToStringSidW(pSID, ppszSID))
                        {
                            hr = HRESULT_FROM_WIN32(GetLastError());
                        }
                        else
                        {
                            hr = S_OK;
                        }
                    }
                    delete[] pSID; // (LocalFree (pSID);
                }
                delete[] pszDomain;
            }
        }
    }

    return (hr);
}

__declspec(noinline) static HRESULT GetSIDStringFromUsername(PCWSTR pcszUserName, std::wstring* ppszSID)
{
    WCHAR* str = 0;
    HRESULT hr = GetSIDStringFromUsername(pcszUserName, &str);
    *ppszSID = str ? str : L"";

    LocalFree(str);

    return hr;
}

static std::wstring GetProfilePicturePathFromSID(const wchar_t* sid, bool bHighRes = false)
{
    std::wstring finalpath = L"C:\\ProgramData\\Microsoft\\User Account Pictures\\user-48.png";

    if (bHighRes)
        finalpath = L"C:\\ProgramData\\Microsoft\\User Account Pictures\\user-192.png";

    //WCHAR* str = 0;
    //auto hr = GetSIDStringFromUsername(username.c_str(), &str);
    //if (hr != S_OK) return finalpath;

    std::wstring subkey = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\AccountPicture\\Users\\" + std::wstring(sid);
    //SPDLOG_INFO("subkey {}", ws2s(subkey));
    BYTE byteArray[MAX_PATH * 2];
    //for (int i = 0; i < MAX_PATH; ++i)
    //    reinterpret_cast<WCHAR*>(byteArray)[i] = L'\0';
    HKEY result;
    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, subkey.c_str(), 0, KEY_READ, &result) == S_OK)
    {
        //SPDLOG_INFO("OK");

        DWORD size = MAX_PATH * 2;
        DWORD type = REG_SZ;

        const wchar_t* imageKey = bHighRes ? L"Image240" : L"Image64";

        if (RegQueryValueExW(result, imageKey, 0, &type, byteArray, &size) == S_OK)
        {
            LPWSTR path = (LPWSTR)(&byteArray);
            //if (size > MAX_PATH * 2)
            //    size = MAX_PATH * 2;
            //path[((int)(size / 2)) - 1] = L'\0';
            //SPDLOG_INFO("path {}", ws2s(path));

            if (PathFileExistsW(path))
            {
                finalpath = path;
            }
        }

        RegCloseKey(result);
    }

    //LocalFree(str);

    return finalpath;
}


//static const wchar_t* GetProfilePicturePathFromSID(std::wstring sid, bool bHighRes = false)
//{
//    return GetProfilePicturePathFromSID(sid.c_str(),bHighRes);
//}

static void GetProfilePicturePathFromSID(std::wstring sid, const wchar_t* outUsername, bool bHighRes = false)
{
    const wchar_t* finalpath = L"C:\\ProgramData\\Microsoft\\User Account Pictures\\user-48.png";

    if (bHighRes)
        finalpath = L"C:\\ProgramData\\Microsoft\\User Account Pictures\\user-192.png";

    //WCHAR* str = 0;
    //auto hr = GetSIDStringFromUsername(username.c_str(), &str);
    //if (hr != S_OK) return finalpath;

    std::wstring subkey = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\AccountPicture\\Users\\" + std::wstring(sid);
    //SPDLOG_INFO("subkey {}", ws2s(subkey));
    BYTE byteArray[MAX_PATH * 2];
    for (int i = 0; i < MAX_PATH; ++i)
        reinterpret_cast<WCHAR*>(byteArray)[i] = L'\0';
    HKEY result;
    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, subkey.c_str(), 0, KEY_READ, &result) == S_OK)
    {
        //SPDLOG_INFO("OK");

        DWORD size = MAX_PATH * 2;
        DWORD type = REG_SZ;

        const wchar_t* imageKey = bHighRes ? L"Image240" : L"Image64";

        if (RegQueryValueExW(result, imageKey, 0, &type, byteArray, &size) == S_OK)
        {
            LPWSTR path = (LPWSTR)(&byteArray);
            //if (size > MAX_PATH * 2)
            //    size = MAX_PATH * 2;
            //path[((int)(size / 2)) - 1] = L'\0';
            //SPDLOG_INFO("path {}", ws2s(path));

            if (PathFileExistsW(path))
            {
                finalpath = path;
            }
        }

        RegCloseKey(result);
    }

    //LocalFree(str);

    wcscpy_s(const_cast<WCHAR*>(outUsername),MAX_PATH + 2, finalpath);
}