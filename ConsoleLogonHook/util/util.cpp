#include "util.h"
#include "interop.h"

const wchar_t* external::GetProfilePicturePathFromUsername(const wchar_t* username,bool bHighRes)
{
    std::wstring sid = L"NULL";
    auto hr = GetSIDStringFromUsername(username, &sid);

    if (hr != S_OK)
    {
        std::wstring finalpath = L"C:\\ProgramData\\Microsoft\\User Account Pictures\\user-48.png";

        if (bHighRes)
            finalpath = L"C:\\ProgramData\\Microsoft\\User Account Pictures\\user-192.png";

        return finalpath.c_str();
    }

	auto path = GetProfilePicturePathFromSID(sid, bHighRes);
    return path.c_str();
}
