#include "util.h"
#include "interop.h"

const wchar_t* external::GetProfilePicturePathFromUsername(const wchar_t* username,bool bHighRes)
{
    const wchar_t* sid = L"NULL";
    auto hr = ::GetSIDStringFromUsername(username, const_cast<wchar_t**>(&sid));

    if (hr != S_OK)
    {
        const wchar_t* finalpath = L"C:\\ProgramData\\Microsoft\\User Account Pictures\\user-48.png";

        if (bHighRes)
            finalpath = L"C:\\ProgramData\\Microsoft\\User Account Pictures\\user-192.png";

        return finalpath;
    }

	auto path = ::GetProfilePicturePathFromSID(sid, bHighRes);
    return path;
}

void external::GetProfilePicturePathFromSID(const wchar_t* sid, const wchar_t* outUsername, bool bHighRes)
{
    ::GetProfilePicturePathFromSID(std::wstring(sid),outUsername,bHighRes);
}
void external::GetSIDFromName(const wchar_t* username, wchar_t** sid)
{
    auto hr = ::GetSIDStringFromUsername(username, const_cast<wchar_t**>(sid));
}

void external::HideConsoleUI()
{
    MinimizeLogonConsole();
}