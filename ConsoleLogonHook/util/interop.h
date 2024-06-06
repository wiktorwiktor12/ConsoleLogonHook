#pragma once
#include <windows.h>

#define EXTERNAL(a,b) (a)(GetProcAddress(externalUiModule, b))

namespace external
{
    inline HMODULE externalUiModule = 0;

    static bool InitExternal()
    {
        externalUiModule = LoadLibraryW(L"ConsoleLogonUI.dll");
        if (!externalUiModule)
        {
            MessageBox(0, L"UI DLL NOT FOUND", L"UI DLL NOT FOUND", MB_ICONERROR);
            return false;
        }
        return true;
    }

    static void Unload()
    {
        FreeLibraryAndExitThread(externalUiModule,0);
    }

    static void InitUI()
    {
        static auto fInitUI = EXTERNAL(void(*)(),"InitUI");
        if (fInitUI)
            fInitUI();
    }

    static void MessageView_SetActive()
    {
        static auto fMessageView_SetActive = EXTERNAL(void(*)(), "MessageView_SetActive");
        if (fMessageView_SetActive)
            fMessageView_SetActive();
    }

    static void MessageOptionControl_Create(void* actualInsance, int optionflag)
    {
        static auto fMessageView_SetActive = EXTERNAL(void(*)(void* actualInsance, int optionflag), "MessageOptionControl_Create");
        if (fMessageView_SetActive)
            fMessageView_SetActive(actualInsance,optionflag);
    }

    static void MessageOptionControl_Destroy(void* actualInstance)
    {
        static auto fMessageOptionControl_Destroy = EXTERNAL(void(*)(void* actualInstance), "MessageOptionControl_Destroy");
        if (fMessageOptionControl_Destroy)
            fMessageOptionControl_Destroy(actualInstance);
    }

    static void MessageView_SetMessage(std::wstring message)
    {
        static auto fMessageView_SetMessage = EXTERNAL(void(*)(const wchar_t* message), "MessageView_SetMessage");
        if (fMessageView_SetMessage)
            fMessageView_SetMessage(message.c_str());
    }

    static void SecurityControlButtonsList_Clear()
    {
        static auto fSecurityControlButtonsList_Clear = EXTERNAL(void(*)(), "SecurityControlButtonsList_Clear");
        if (fSecurityControlButtonsList_Clear)
            fSecurityControlButtonsList_Clear();
    }

    static void SecurityControl_SetActive()
    {
        static auto fSecurityControl_SetActive = EXTERNAL(void(*)(), "SecurityControl_SetActive");
        if (fSecurityControl_SetActive)
            fSecurityControl_SetActive();
    }

    static void SecurityControl_SetInactive()
    {
        static auto fSecurityControl_SetInactive = EXTERNAL(void(*)(), "SecurityControl_SetInactive");
        if (fSecurityControl_SetInactive)
            fSecurityControl_SetInactive();
    }

    static void SecurityControl_ButtonsReady()
    {
        static auto fSecurityControl_ButtonsReady = EXTERNAL(void(*)(), "SecurityControl_ButtonsReady");
        if (fSecurityControl_ButtonsReady)
            fSecurityControl_ButtonsReady();
    }

    static void SecurityOptionControl_Create(void* actualInstance)
    {
        static auto fSecurityOptionControl_Create = EXTERNAL(void(*)(void* actualInstance), "SecurityOptionControl_Create");
        if (fSecurityOptionControl_Create)
            fSecurityOptionControl_Create(actualInstance);
    }

    static void SecurityOptionControl_Destroy(void* actualInstance)
    {
        static auto fSecurityOptionControl_Destroy = EXTERNAL(void(*)(void* actualInstance), "SecurityOptionControl_Destroy");
        if (fSecurityOptionControl_Destroy)
            fSecurityOptionControl_Destroy(actualInstance);
    }

    static void NotifyWasInSelectedCredentialView()
    {
        static auto fNotifyWasInSelectedCredentialView = EXTERNAL(void(*)(), "NotifyWasInSelectedCredentialView");
        if (fNotifyWasInSelectedCredentialView)
            fNotifyWasInSelectedCredentialView();
    }

    static void SelectedCredentialView_SetActive(const wchar_t* accountNameToDisplay, int flag)
    {
        static auto fSelectedCredentialView_SetActive = EXTERNAL(void(*)(const wchar_t* accountNameToDisplay, int flag), "SelectedCredentialView_SetActive");
        if (fSelectedCredentialView_SetActive)
            fSelectedCredentialView_SetActive(accountNameToDisplay, flag);
    }

    static void EditControl_Create(void* actualInstance)
    {
        static auto fEditControl_Create = EXTERNAL(void(*)(void* actualInstance), "EditControl_Create");
        if (fEditControl_Create)
            fEditControl_Create(actualInstance);
    }

    static void EditControl_Destroy(void* actualInstance)
    {
        static auto fEditControl_Destroy = EXTERNAL(void(*)(void* actualInstance), "EditControl_Destroy");
        if (fEditControl_Destroy)
            fEditControl_Destroy(actualInstance);
    }

    static void StatusView_SetActive(std::wstring text)
    {
        static auto fStatusView_SetActive = EXTERNAL(void(*)(const wchar_t*), "StatusView_SetActive");
        if (fStatusView_SetActive)
            fStatusView_SetActive(text.c_str());
    }

    static void UserSelect_SetActive()
    {
        static auto fUserSelect_SetActive = EXTERNAL(void(*)(), "UserSelect_SetActive");
        if (fUserSelect_SetActive)
            fUserSelect_SetActive();
    }

    static void SelectableUserOrCredentialControl_Sort()
    {
        static auto fSelectableUserOrCredentialControl_Sort = EXTERNAL(void(*)(), "SelectableUserOrCredentialControl_Sort");
        if (fSelectableUserOrCredentialControl_Sort)
            fSelectableUserOrCredentialControl_Sort();
    }

    static void SelectableUserOrCredentialControl_Create(void* actualInstance, std::wstring path)
    {
        static auto fSelectableUserOrCredentialControl_Create = EXTERNAL(void(*)(void* actualInstance, const wchar_t* path), "SelectableUserOrCredentialControl_Create");
        if (fSelectableUserOrCredentialControl_Create)
            fSelectableUserOrCredentialControl_Create(actualInstance,path.c_str());
    }

    static void SelectableUserOrCredentialControl_Destroy(void* actualInstance)
    {
        static auto fSelectableUserOrCredentialControl_Destroy = EXTERNAL(void(*)(void* actualInstance), "SelectableUserOrCredentialControl_Destroy");
        if (fSelectableUserOrCredentialControl_Destroy)
            fSelectableUserOrCredentialControl_Destroy(actualInstance);
    }

    extern "C" __declspec(dllexport) void MessageOptionControl_Press(void* actualInstance, const struct _KEY_EVENT_RECORD* keyrecord, int* success);
    extern "C" __declspec(dllexport) const wchar_t* MessageOptionControl_GetText(void* actualInstance);

    extern "C" __declspec(dllexport) void SecurityOptionControl_Press(void* actualInstance, const struct _KEY_EVENT_RECORD* keyrecord, int* success);
    extern "C" __declspec(dllexport) const wchar_t* SecurityOptionControl_getString(void* actualInstance);

    extern "C" __declspec(dllexport) const wchar_t* EditControl_GetFieldName(void* actualInstance);
    extern "C" __declspec(dllexport) const wchar_t* EditControl_GetInputtedText(void* actualInstance);
    extern "C" __declspec(dllexport) void EditControl_SetInputtedText(void* actualInstance, const wchar_t* input);
    extern "C" __declspec(dllexport) bool EditControl_isVisible(void* actualInstance);

    extern "C" __declspec(dllexport) void ConsoleUIView__HandleKeyInputExternal(void* instance, const struct _KEY_EVENT_RECORD* keyrecord);
    extern "C" __declspec(dllexport) void* GetConsoleUIView();
    //extern "C" __declspec(dllexport) const wchar_t* GetProfilePicturePathFromUsername(const wchar_t* username, bool bHighRes);
    extern "C" __declspec(dllexport) void GetProfilePicturePathFromSID(const wchar_t* sid, const wchar_t* outUsername, bool bHighRes);
    extern "C" __declspec(dllexport) void GetSIDFromName(const wchar_t* username, wchar_t** sid);

    extern "C" __declspec(dllexport) const wchar_t* SelectableUserOrCredentialControl_GetText(void* actualInstance);
    extern "C" __declspec(dllexport) void SelectableUserOrCredentialControl_Press(void* actualInstance);
    extern "C" __declspec(dllexport) bool SelectableUserOrCredentialControl_isCredentialControl(void* actualInstance);

    extern "C" __declspec(dllexport) void HideConsoleUI();
}

#ifdef EXTERNAL
#undef EXTERNAL
#endif