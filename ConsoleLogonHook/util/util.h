#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#include <winstring.h>

#define Hook(a,b) DetourTransactionBegin(); DetourAttach(&(PVOID&)a, b); DetourTransactionCommit();

inline HRESULT(__stdcall* fWindowsCreateString)(PCWSTR sourceString,UINT32 length,HSTRING* string);
inline PCWSTR(__stdcall* fWindowsGetStringRawBuffer)(HSTRING string, UINT32* length);
inline HRESULT(__stdcall* fWindowsDeleteString)(HSTRING string);

namespace globals
{
    inline void* ConsoleUIView;
    inline __int64(__fastcall* ConsoleUIView__Initialize)(void* _this);
    inline __int64(__fastcall* ConsoleUIView__HandleKeyInput)(void* _this, _KEY_EVENT_RECORD* a2);
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

static void MinimizeLogonConsole()
{
    auto consoleWindow = FindWindowW(0, L"C:\\Windows\\system32\\LogonUI.exe");
    if (!consoleWindow) return;

    //MoveWindow(consoleWindow,0,0,1,1,0);
    //ShowWindow(consoleWindow, SW_FORCEMINIMIZE);
    ShowWindow(consoleWindow, SW_HIDE);
    //ShowWindow(consoleWindow, SW_RESTORE);
}

static void* GetVirtualFunctionFromTable(void* vtable, int index)
{
    uintptr_t* func = (uintptr_t*)((uintptr_t)vtable + index);

    return reinterpret_cast<void*>(*func);
}