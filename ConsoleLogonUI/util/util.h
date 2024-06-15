#pragma once
#include <windows.h>
#include <string>
#include <iostream>
#include <Gdiplus.h>
#include <gdiplusheaders.h>
#include <gdiplusinit.h>
#include <atlbase.h>
#include <dui/DirectUI/DirectUI.h>

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

static HBITMAP GetHBITMAPFromImageFile(WCHAR* pFilePath)
{
    Gdiplus::GdiplusStartupInput gpStartupInput;
    ULONG_PTR gpToken;
    Gdiplus::GdiplusStartup(&gpToken, &gpStartupInput, NULL);
    HBITMAP result = NULL;
    Gdiplus::Bitmap* bitmap = Gdiplus::Bitmap::FromFile(pFilePath, false);
    if (bitmap)
    {
        bitmap->GetHBITMAP(Gdiplus::Color(255, 255, 255), &result);
        delete bitmap;
    }
    Gdiplus::GdiplusShutdown(gpToken);
    return result;
}

static __int64 DirectUIElementAdd(DirectUI::Element* Parent, DirectUI::Element* Child)
{
    //ELEMENT::ADD
    //https://imgur.com/a/pEyfcVQ
    return (*(__int64(__fastcall**)(DirectUI::Element*, struct DirectUI::Element**, __int64))(*(uintptr_t*)Parent + 128i64))(
        Parent,
        (DirectUI::Element**)&Child,
        1i64);
}

static std::wstring AtomToStr(ATOM atom)
{
    WCHAR atomName[256];
    GetAtomNameW(atom, atomName, 256);
    return atomName;
}