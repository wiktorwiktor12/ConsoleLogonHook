#include "ui_securitycontrol.h"
#include <Windows.h>
#include "detours.h"
#include "spdlog/spdlog.h"
#include "../util/util.h"
#include <winstring.h>
#include <util/interop.h>
#include <util/memory_man.h>

//std::vector<SecurityOptionControlWrapper> buttonsList;

inline long(__fastcall* SecurityOptionControlHandleKeyInput)(void* _this, const _KEY_EVENT_RECORD* keyrecord, int* result);
long SecurityOptionControlHandleKeyInput_Hook(void* _this, _KEY_EVENT_RECORD* keyrecord, int* result)
{
    auto res = SecurityOptionControlHandleKeyInput(_this, keyrecord, result);
    SPDLOG_INFO("this {} keycode {} result {}", _this, (int)keyrecord->wVirtualKeyCode, (result ? *result : 0));
    return res;
}

__int64(__fastcall* LogonViewManager__ShowSecurityOptionsUIThread)(unsigned __int64 a1, unsigned int a2, __int64* a3);
__int64 __fastcall LogonViewManager__ShowSecurityOptionsUIThread_Hook(unsigned __int64 a1, unsigned int a2, __int64* a3)
{
    SPDLOG_INFO("LogonViewManager__ShowSecurityOptionsUIThread_Hook Called! {} {} {}", (void*)a1, a2, (void*)a3);

    external::SecurityControlButtonsList_Clear();
    //buttonsList.clear();

    return LogonViewManager__ShowSecurityOptionsUIThread(a1, a2, a3);
}

__int64(__fastcall* LogonViewManager__ShowSecurityOptions)(__int64 a1, int a2, __int64* a3);
__int64 __fastcall LogonViewManager__ShowSecurityOptions_Hook(__int64 a1, int a2, __int64* a3)
{
    SPDLOG_INFO("LogonViewManager__ShowSecurityOptions_Hook Called! {} {} {}", (void*)a1, a2, (void*)a3);

    return LogonViewManager__ShowSecurityOptions(a1, a2, a3);
}

__int64(__fastcall* SecurityOptionsView__RuntimeClassInitialize)(__int64 a1, char a2, __int64* a3);
__int64 __fastcall SecurityOptionsView__RuntimeClassInitialize_Hook(__int64 a1, char a2, __int64* a3)
{
    external::SecurityControl_SetActive();

    /*for (int i = 0; i < uiRenderer::Get()->inactiveWindows.size(); ++i) //theres prob a better and nicer way to do this
    {
        auto& window = uiRenderer::Get()->inactiveWindows[i];
        if (window->windowTypeId == 2) //typeid for securityoptions
        {
            auto& notifies = reinterpret_cast<uiSecurityControl*>(window.get())->wasInSecurityControlNotifies;
            for (int x = 0; x < notifies.size(); ++x)
            {
                auto& notify = notifies[x];
                if (notify)
                    notify();
            }

            window->SetActive();

            break;
        }
    }*/

    //MinimizeLogonConsole();

    //auto consoleWindow = FindWindowW(0,L"C:\\Windows\\system32\\LogonUI.exe");
    //if (consoleWindow)
    //    ShowWindow(consoleWindow, SW_FORCEMINIMIZE);
        //ShowWindow(consoleWindow, SW_HIDE);

    auto res = SecurityOptionsView__RuntimeClassInitialize(a1, a2, a3);
    external::SecurityControl_ButtonsReady();
    /*auto SwapButton = [&](int a, int b) -> void
        {
            SecurityOptionControlWrapper temp = buttonsList[a];
            buttonsList[a] = buttonsList[b];
            buttonsList[b] = temp;
        };

    SwapButton(0,1);
    SwapButton(1,3);*/

    return res;
}

/*__int64(__fastcall* MakeAndInitialize_SecurityOptionControl)(void** _this, void* a2, int* a3, void* a4);
__int64 __fastcall MakeAndInitialize_SecurityOptionControl_Hook(void** _this, void* a2, int* a3, void* a4)
{
    auto res = MakeAndInitialize_SecurityOptionControl(_this, a2, a3, a4);

    auto control = *_this;
    if (control)
    {
        //SPDLOG_INFO("Got Control!");

        wchar_t* text = *(wchar_t**)(__int64(control) + 0x48);

        //SecurityOptionControlWrapper button(control);

        SPDLOG_INFO("text: {}, comptr: {}, controlptr {} a2 {} a3 {} a4 {}", ws2s(text).c_str(), (void*)_this, (void*)control,(void*)a2,(void*)a3,(void*)a4);

        external::SecurityOptionControl_Create(control);

        //buttonsList.push_back(button);
    }

    return res;
}*/

__int64(__fastcall* SecurityOptionControl_RuntimeClassInitialize)(void* _this, void* a2, int a3, void* a4);
__int64 __fastcall SecurityOptionControl_RuntimeClassInitialize_Hook(void** _this, void* a2, int a3, void* a4)
{
    auto res = SecurityOptionControl_RuntimeClassInitialize(_this, a2, a3, a4);

    //SPDLOG_INFO("Got Control!");

    wchar_t* text = *(wchar_t**)(__int64(_this) + 0x48);

    //SecurityOptionControlWrapper button(control);

    SPDLOG_INFO("text: {}, controlptr {} a2 {} a3 {} a4 {}", ws2s(text).c_str(), (void*)_this, (void*)a2, (void*)a3, (void*)a4);

    external::SecurityOptionControl_Create(_this);

    //buttonsList.push_back(button);

    return res;
}

void* (__fastcall* SecurityOptionControl_Destructor)(__int64 a1, unsigned int a2);
void* SecurityOptionControl_Destructor_Hook(__int64 a1, unsigned int a2)
{
    /*for (int i = 0; i < buttonsList.size(); ++i)
    {
        auto& button = buttonsList[i];
        if ((void*)(__int64(button.actualInstance) + 8) == (void*)a1)
        {
            SPDLOG_INFO("FOUND AND DELETING BUTTON");
            buttonsList.erase(buttonsList.begin() + i);
            break;
        }
    }*/
    
    external::SecurityOptionControl_Destroy((void*)(__int64(a1) - 8));

    //auto consoleWindow = FindWindowW(0, L"C:\\Windows\\system32\\LogonUI.exe");
    //if (consoleWindow)
    //{
    //    ShowWindow(consoleWindow, SW_SHOW);
    //    ShowWindow(consoleWindow, SW_RESTORE);
    //}

    return SecurityOptionControl_Destructor(a1,a2);
}

__int64(__fastcall*CredUIManager__ShowCredentialView)(/*CredUIViewManager*/void* _this, HSTRING a2);
__int64 CredUIManager__ShowCredentialView_Hook(void* _this, HSTRING a2)
{
    UINT32 length;
    std::wstring convertedString = fWindowsGetStringRawBuffer(a2, &length);
    SPDLOG_INFO("CredUIManager__ShowCredentialView _this: {} a2: {}",_this, ws2s(convertedString));
    
    

    return CredUIManager__ShowCredentialView(_this,a2);
}

__int64(__fastcall* SecurityOptionsView__Destructor)(__int64 a1, char a2);
__int64 SecurityOptionsView__Destructor_Hook(__int64 a1, char a2)
{
    //auto securityControl = uiRenderer::Get()->GetWindowOfTypeId<uiSecurityControl>(2);
    //if (securityControl)
    //{
    //    SPDLOG_INFO("setting inactive security control instance");
    //    securityControl->SetInactive();
    //}
    external::SecurityControl_SetInactive();
    return SecurityOptionsView__Destructor(a1,a2);
}

void external::SecurityOptionControl_Press(void* actualInstance, const struct _KEY_EVENT_RECORD* keyrecord, int* success)
{
    if (actualInstance)
    {

        SecurityOptionControlHandleKeyInput((void*)(__int64(actualInstance) + 8), keyrecord, success);
    }
}

const wchar_t* external::SecurityOptionControl_getString(void* actualInstance)
{
    return *(wchar_t**)(__int64(actualInstance) + 0x48);
}

/*void SecurityOptionControlWrapper::Press()
{
    _KEY_EVENT_RECORD keyrecord;
    keyrecord.bKeyDown = true;
    keyrecord.wVirtualKeyCode = VK_RETURN;
    int result;
    if (actualInstance)
    {
        SPDLOG_INFO("Actual instance {} isn't null, so we are calling handlekeyinput with enter on the control!", actualInstance);

        SecurityOptionControlHandleKeyInput((void*)(__int64(actualInstance) + 8), &keyrecord, &result); // + 8 makes it work, weird af class structure but fuck it we ball
    }
}*/

void uiSecurityControl::InitHooks(uintptr_t baseaddress)
{
    LogonViewManager__ShowSecurityOptionsUIThread = memory::FindPatternCached<decltype(LogonViewManager__ShowSecurityOptionsUIThread)>("LogonViewManager__ShowSecurityOptionsUIThread", { "48 8B EC 48 83 EC 40 49 8B F8 8B F2 4C 8B F1 E8" },true);
    LogonViewManager__ShowSecurityOptions = memory::FindPatternCached<decltype(LogonViewManager__ShowSecurityOptions)>("LogonViewManager__ShowSecurityOptions", { "48 89 ?? 28 44 89 ?? 30 ?? 89 ?? 38 ?? 89 73 40 ?? 85 F6 74 10 ?? 8B 06 ?? 8B CE 48 8B 40 08 FF 15" },true);
    SecurityOptionControl_RuntimeClassInitialize = memory::FindPatternCached<decltype(SecurityOptionControl_RuntimeClassInitialize)>("SecurityOptionControl_RuntimeClassInitialize", {"B9 10 00 00 00 E8 ?? ?? ?? ?? 4C 8B F0 48 85 C0 74 22 48 8B 07 49 89 06 48 8B 4F 08 49 89 4E 08 48 85 C9 74 12 48 8B 01"}, true);
    SecurityOptionControlHandleKeyInput = memory::FindPatternCached<decltype(SecurityOptionControlHandleKeyInput)>("SecurityOptionControlHandleKeyInput", { "48 89 5C 24 10 48 89 74 24 20 55 57 41 56 48 8B EC 48 83 EC 70 48 8B 05 ?? ?? ?? ?? 48 33 C4" });
    //SecurityOptionControlHandleKeyInput = decltype(SecurityOptionControlHandleKeyInput)(baseaddress + 0x44490);
    //ConsoleUIView__Initialize = decltype(ConsoleUIView__Initialize)(baseaddress + 0x42710);
    //ConsoleUIView__HandleKeyInput = decltype(ConsoleUIView__HandleKeyInput)(baseaddress + 0x43530);
    
    void** SecurityOptionControlVtable = (void**)REL(memory::FindPatternCached<uintptr_t>("SecurityOptionControlVtable", { "48 8D 05 ?? ?? ?? ?? 48 83 63 48 00 48 83 63 50 00 48 83 63 58 00 48 83 63 68 00 83 63 70 00 48 89 43 08","48 8D 05 ?? ?? ?? ?? 48 89 43 08 48 8D 05 ?? ?? ?? ?? 48 89 43 30 48 89 6B 48"}), 3);
    
    SecurityOptionControl_Destructor = (decltype(SecurityOptionControl_Destructor))(SecurityOptionControlVtable[7]);
    SecurityOptionsView__RuntimeClassInitialize = memory::FindPatternCached<decltype(SecurityOptionsView__RuntimeClassInitialize)>("SecurityOptionsView__RuntimeClassInitialize", { "55 56 57 41 56 41 57 48 8B EC 48 83 EC 30" },true);
    //CredUIManager__ShowCredentialView = memory::FindPatternCached<decltype(CredUIManager__ShowCredentialView)>("CredUIManager__ShowCredentialView", "48 89 5C 24 08 55 56 57 41 54 41 55 41 56 41 57 48 8B EC");
    SecurityOptionsView__Destructor = memory::FindPatternCached<decltype(SecurityOptionsView__Destructor)>("SecurityOptionsView__Destructor", { "48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 20 8B F2 48 8B D9 48 8B 79 78 48 83 61 78 00","48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 20 48 8B 79 78 8B F2 48 83 61 78 00 48 8B D9"});

    Hook(LogonViewManager__ShowSecurityOptionsUIThread, LogonViewManager__ShowSecurityOptionsUIThread_Hook);
    Hook(LogonViewManager__ShowSecurityOptions, LogonViewManager__ShowSecurityOptions_Hook);
    Hook(SecurityOptionControl_RuntimeClassInitialize, SecurityOptionControl_RuntimeClassInitialize_Hook);
    Hook(SecurityOptionControlHandleKeyInput, SecurityOptionControlHandleKeyInput_Hook);
    Hook(SecurityOptionControl_Destructor, SecurityOptionControl_Destructor_Hook);
    Hook(SecurityOptionsView__RuntimeClassInitialize, SecurityOptionsView__RuntimeClassInitialize_Hook);
    //Hook(CredUIManager__ShowCredentialView, CredUIManager__ShowCredentialView_Hook);
    Hook(SecurityOptionsView__Destructor, SecurityOptionsView__Destructor_Hook);

    //Hook(ConsoleUIView__Initialize, ConsoleUIView__Initialize_Hook);
}