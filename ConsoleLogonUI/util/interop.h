#pragma once
#include <windows.h>

#define EXTERNAL(a,b) (a)(GetProcAddress(externalHookModule, b))

namespace external
{
    inline HMODULE externalHookModule = 0;

    static bool InitExternal()
    {
        externalHookModule = GetModuleHandleW(L"ConsoleLogonHook.dll");
        if (!externalHookModule)
        {
            MessageBox(0, L"HOOK DLL NOT FOUND", L"HOOK DLL NOT FOUND", 0);
            return false;
        }
        return true;
    }

    static void MessageOptionControl_Press(void* actualInstance, const struct _KEY_EVENT_RECORD* keyrecord, int* success)
    {
        static auto fMessageOptionControl_Press = EXTERNAL(void(*)(void* actualInstance, const struct _KEY_EVENT_RECORD* keyrecord, int* success),"MessageOptionControl_Press");
        if (fMessageOptionControl_Press)
            fMessageOptionControl_Press(actualInstance,keyrecord,success);
    }

    static std::wstring MessageOptionControl_GetText(void* actualInstance)
    {
        static auto fMessageOptionControl_GetText = EXTERNAL(const wchar_t*(*)(void* actualInstance), "MessageOptionControl_GetText");
        if (fMessageOptionControl_GetText)
            return fMessageOptionControl_GetText(actualInstance);

        return std::wstring();
    }

    static void SecurityOptionControl_Press(void* actualInstance, const struct _KEY_EVENT_RECORD* keyrecord, int* success)
    {
        static auto fSecurityOptionControl_Press = EXTERNAL(void(*)(void* actualInstance, const struct _KEY_EVENT_RECORD* keyrecord, int* success), "SecurityOptionControl_Press");
        if (fSecurityOptionControl_Press)
            fSecurityOptionControl_Press(actualInstance, keyrecord, success);
    }

    static std::wstring SecurityOptionControl_getString(void* actualInstance)
    {
        static auto fSecurityOptionControl_getString = EXTERNAL(const wchar_t*(*)(void* actualInstance), "SecurityOptionControl_getString");
        if (fSecurityOptionControl_getString)
            return std::wstring(fSecurityOptionControl_getString(actualInstance));
    }

    static void ConsoleUIView__HandleKeyInputExternal(void* instance, const struct _KEY_EVENT_RECORD* keyrecord)
    {
        static auto fConsoleUIView__HandleKeyInputExternal = EXTERNAL(void(*)(void* actualInstance, const struct _KEY_EVENT_RECORD* keyrecord), "ConsoleUIView__HandleKeyInputExternal");
        if (fConsoleUIView__HandleKeyInputExternal)
            fConsoleUIView__HandleKeyInputExternal(instance, keyrecord);
    }

    static void* GetConsoleUIView()
    {
        static auto fGetConsoleUIView = EXTERNAL(void*(*)(), "GetConsoleUIView");
        if (fGetConsoleUIView)
            return fGetConsoleUIView();

        return nullptr;
    }

    static std::wstring GetProfilePicturePathFromSID(std::wstring sid, bool bHighRes = false)
    {
        //void GetProfilePicturePathFromSID(const wchar_t* sid, const wchar_t** outUsername, bool bHighRes);
        static auto fGetProfilePicturePathFromSID = EXTERNAL(void (*)(const wchar_t* sid, wchar_t* outUsername, bool bHighRes), "GetProfilePicturePathFromSID");
        if (fGetProfilePicturePathFromSID)
        {
            WCHAR* ptr = (WCHAR*)malloc((MAX_PATH + 1) * sizeof(WCHAR));
            ZeroMemory(ptr,(MAX_PATH + 1) * sizeof(WCHAR));
            ptr[MAX_PATH] = L'\0';
            fGetProfilePicturePathFromSID(sid.c_str(), ptr,bHighRes);
            std::wstring ret = ptr;
            free(ptr);
            return ret;
        }

        return std::wstring();
    }

    __declspec(noinline) static void GetSIDFromName(const wchar_t* username, std::wstring* sid)
    {
        //void GetSIDFromName(const wchar_t* username, wchar_t** sid);
        static auto fGetSIDFromName = EXTERNAL(void (*)(const wchar_t* username, wchar_t** sid), "GetSIDFromName");
        if (fGetSIDFromName)
        {
            WCHAR* str = 0;
            fGetSIDFromName(username, &str);
            *sid = str ? str : L"";

            LocalFree(str);
            //const WCHAR* psid = L"";
            //fGetSIDFromName(username,&psid);
            //*sid = psid;
            //LocalFree(psid);
        }
    }

    static std::wstring GetProfilePicturePathFromUsername(std::wstring username, bool bHighRes = false)
    {
        std::wstring sid;
        GetSIDFromName(username.c_str(),&sid);
        auto res = GetProfilePicturePathFromSID(sid,bHighRes);
        return sid;
        /*static auto fGetProfilePicturePathFromUsername = EXTERNAL(const wchar_t* (*)(const wchar_t*, bool bHighRes), "GetProfilePicturePathFromUsername");
        if (fGetProfilePicturePathFromUsername)
            return fGetProfilePicturePathFromUsername(username.c_str(), bHighRes);*/
    }

    static std::wstring EditControl_GetFieldName(void* actualInstance)
    {
        static auto fEditControl_GetFieldName = EXTERNAL(const wchar_t* (*)(void* actualInstance), "EditControl_GetFieldName");
        if (fEditControl_GetFieldName)
            return fEditControl_GetFieldName(actualInstance);

        return std::wstring();
    }

    static std::wstring EditControl_GetInputtedText(void* actualInstance)
    {
        static auto fEditControl_GetInputtedText = EXTERNAL(const wchar_t* (*)(void* actualInstance), "EditControl_GetInputtedText");
        if (fEditControl_GetInputtedText)
            return fEditControl_GetInputtedText(actualInstance);

        return std::wstring();
    }

    static void EditControl_SetInputtedText(void* actualInstance,std::wstring input)
    {
        static auto fEditControl_SetInputtedText = EXTERNAL(const wchar_t* (*)(void* actualInstance, const wchar_t* input), "EditControl_SetInputtedText");
        if (fEditControl_SetInputtedText)
            fEditControl_SetInputtedText(actualInstance,input.c_str());
    }

    static bool EditControl_isVisible(void* actualInstance)
    {
        static auto fEditControl_IsVisible = EXTERNAL(bool(*)(void* actualInstance), "EditControl_isVisible");
        //SPDLOG_INFO("fEditControl_IsVisible {}", (void*)fEditControl_IsVisible);
        if (fEditControl_IsVisible)
        {
            bool val = fEditControl_IsVisible(actualInstance);
            //SPDLOG_INFO("val {}", (int)val);
            return val;
        }

        return false;
    }

    static std::wstring SelectableUserOrCredentialControl_GetText(void* actualInstance)
    {
        static auto fSelectableUserOrCredentialControl_GetText = EXTERNAL(const wchar_t*(*)(void* actualInstance), "SelectableUserOrCredentialControl_GetText");
        if (fSelectableUserOrCredentialControl_GetText)
            return fSelectableUserOrCredentialControl_GetText(actualInstance);

        return std::wstring();
    }

    static void SelectableUserOrCredentialControl_Press(void* actualInstance)
    {
        static auto fSelectableUserOrCredentialControl_Press = EXTERNAL(void(*)(void* actualInstance), "SelectableUserOrCredentialControl_Press");
        if (fSelectableUserOrCredentialControl_Press)
            fSelectableUserOrCredentialControl_Press(actualInstance);
    }

    static bool SelectableUserOrCredentialControl_isCredentialControl(void* actualInstance)
    {
        static auto fSelectableUserOrCredentialControl_isCredentialControl = EXTERNAL(bool(*)(void* actualInstance), "SelectableUserOrCredentialControl_isCredentialControl");
        if (fSelectableUserOrCredentialControl_isCredentialControl)
            return fSelectableUserOrCredentialControl_isCredentialControl(actualInstance);

        return false;
    }

    //TODO: FIND BETTER WAY TO HIDE CONSOLEUI AUTOMATICALLY
    static void HideConsoleUI()
    {
        static auto fHideConsoleUI = EXTERNAL(void(*)(), "HideConsoleUI");
        if (fHideConsoleUI)
            fHideConsoleUI();
    }

    static HBITMAP BrandingLoadImage(const wchar_t* a1, __int64 a2, UINT a3, int a4, int a5, UINT a6)
    {
        static auto fBrandingLoadImage = reinterpret_cast<HBITMAP(__fastcall*)(const wchar_t* a1, __int64 a2, UINT a3, int a4, int a5, UINT a6)>(GetProcAddress(LoadLibrary(L"winbrand.dll"),"BrandingLoadImage"));
        if (fBrandingLoadImage)
            return fBrandingLoadImage(a1,a2,a3,a4,a5,a6);

        return 0;
    }

    static __int64 TouchEditBaseGetPasswordCharacter(void* touchEditbase)
    {
        static auto fGetPasswordCharacter = reinterpret_cast<__int64(__fastcall*)(void*)>(GetProcAddress(GetModuleHandleW(L"dui70.dll"),"?GetPasswordCharacter@TouchEditBase@DirectUI@@QEAAHXZ"));
        if (fGetPasswordCharacter)
            return fGetPasswordCharacter(touchEditbase);

        return 0;
    }

    static __int64 TouchEditBaseSetPasswordCharacter(void* touchEditbase, unsigned int passwordCharacter)
    {
        static auto fSetPasswordCharacter = reinterpret_cast<__int64(__fastcall*)(void*, unsigned int)>(GetProcAddress(GetModuleHandleW(L"dui70.dll"), "?SetPasswordCharacter@TouchEditBase@DirectUI@@QEAAJH@Z"));
        if (fSetPasswordCharacter)
            return fSetPasswordCharacter(touchEditbase, passwordCharacter);

        return 0;
    }

    extern "C" __declspec(dllexport) void MessageView_SetActive();
    extern "C" __declspec(dllexport) void MessageOptionControl_Create(void* actualInsance, int optionflag);
    extern "C" __declspec(dllexport) void MessageView_SetMessage(const wchar_t* message);
    extern "C" __declspec(dllexport) void MessageOptionControl_Destroy(void* actualInstance);

    extern "C" __declspec(dllexport) void SecurityControlButtonsList_Clear();
    extern "C" __declspec(dllexport) void SecurityControl_SetActive();
    extern "C" __declspec(dllexport) void SecurityControl_ButtonsReady();
    extern "C" __declspec(dllexport) void SecurityOptionControl_Create(void* actualInstance);
    extern "C" __declspec(dllexport) void SecurityOptionControl_Destroy(void* actualInstance);
    extern "C" __declspec(dllexport) void SecurityControl_SetInactive();

    extern "C" __declspec(dllexport) void NotifyWasInSelectedCredentialView();
    extern "C" __declspec(dllexport) void SelectedCredentialView_SetActive(const wchar_t* accountNameToDisplay, int flag);
    extern "C" __declspec(dllexport) void EditControl_Create(void* actualInstance);
    extern "C" __declspec(dllexport) void EditControl_Destroy(void* actualInstance);

    extern "C" __declspec(dllexport) void StatusView_SetActive(const wchar_t* text);
    extern "C" __declspec(dllexport) void MessageOrStatusView_Destroy(void);

    extern "C" __declspec(dllexport) void UserSelect_SetActive();
    extern "C" __declspec(dllexport) void SelectableUserOrCredentialControl_Sort();
    extern "C" __declspec(dllexport) void SelectableUserOrCredentialControl_Create(void* actualInstance, const wchar_t* path);
    extern "C" __declspec(dllexport) void SelectableUserOrCredentialControl_Destroy(void* actualInstance);
}

#ifdef EXTERNAL
#undef EXTERNAL
#endif