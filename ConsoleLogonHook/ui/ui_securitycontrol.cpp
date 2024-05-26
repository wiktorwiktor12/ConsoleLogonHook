#include "ui_securitycontrol.h"
#include "detours.h"
#include "spdlog/spdlog.h"
#include "../util/util.h"
#include <winstring.h>
#include "ui_helper.h"

std::vector<SecurityOptionControlWrapper> buttonsList;

inline long(__fastcall* SecurityOptionControlHandleKeyInput)(void* _this, _KEY_EVENT_RECORD* keyrecord, int* result);
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

    buttonsList.clear();

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
    buttonsList.clear();
    for (int i = 0; i < uiRenderer::Get()->inactiveWindows.size(); ++i) //theres prob a better and nicer way to do this
    {
        auto& window = uiRenderer::Get()->inactiveWindows[i];
        if (window->windowTypeId == 2) //typeid for securityoptions
        {
            window->SetActive();
            break;
        }
    }

    auto consoleWindow = FindWindowW(0,L"C:\\Windows\\system32\\LogonUI.exe");
    if (consoleWindow)
        ShowWindow(consoleWindow, SW_FORCEMINIMIZE);
        //ShowWindow(consoleWindow, SW_HIDE);

    auto res = SecurityOptionsView__RuntimeClassInitialize(a1, a2, a3);

    auto SwapButton = [&](int a, int b) -> void
        {
            SecurityOptionControlWrapper temp = buttonsList[a];
            buttonsList[a] = buttonsList[b];
            buttonsList[b] = temp;
        };

    SwapButton(0,1);
    SwapButton(1,3);

    return res;
}

__int64(__fastcall* MakeAndInitialize_SecurityOptionControl)(void** _this, void* a2, int* a3, void* a4);
__int64 __fastcall MakeAndInitialize_SecurityOptionControl_Hook(void** _this, void* a2, int* a3, void* a4)
{
    auto res = MakeAndInitialize_SecurityOptionControl(_this, a2, a3, a4);

    auto control = *_this;
    if (control)
    {
        //SPDLOG_INFO("Got Control!");

        wchar_t* text = *(wchar_t**)(__int64(control) + 0x48);

        SecurityOptionControlWrapper button(control);

        SPDLOG_INFO("text: {}, comptr: {}, controlptr {} a2 {} a3 {} a4 {}", ws2s(text).c_str(), (void*)_this, (void*)control,(void*)a2,(void*)a3,(void*)a4);

        buttonsList.push_back(button);
    }

    return res;
}
void* (__fastcall* SecurityOptionControl_Destructor)(__int64 a1, unsigned int a2);
void* SecurityOptionControl_Destructor_Hook(__int64 a1, unsigned int a2)
{
    for (int i = 0; i < buttonsList.size(); ++i)
    {
        auto& button = buttonsList[i];
        if ((void*)(__int64(button.actualInstance) + 8) == (void*)a1)
        {
            SPDLOG_INFO("FOUND AND DELETING BUTTON");
            buttonsList.erase(buttonsList.begin() + i);
            break;
        }
    }

    auto consoleWindow = FindWindowW(0, L"C:\\Windows\\system32\\LogonUI.exe");
    if (consoleWindow)
    {
        ShowWindow(consoleWindow, SW_SHOW);
        ShowWindow(consoleWindow, SW_RESTORE);
    }

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

__int64(__fastcall* SecurityOptionsView__Destructor)(__int64 a1, unsigned int a2);
__int64 SecurityOptionsView__Destructor_Hook(__int64 a1, unsigned int a2)
{
    auto securityControl = uiRenderer::Get()->GetWindowOfTypeId<uiSecurityControl>(2);
    if (securityControl)
    {
        SPDLOG_INFO("setting inactive security control instance");
        securityControl->SetInactive();
    }
    return SecurityOptionsView__Destructor(a1,a2);
}

void uiSecurityControl::Draw()
{
    ImGui::Begin("Security Options",0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    
    ImGui::SetWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::SetWindowPos(ImVec2(0,0));

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y * 0.55);

    TextCenteredOnLine("Security options");
    float longestWidth = 0;
    for (int i = 0; i < buttonsList.size(); ++i)
    {
        auto& button = buttonsList[i];
        if (button.actualInstance)
        {
            longestWidth = std::max<float>(ImGui::CalcTextSize(ws2s(button.getString()).c_str()).x, longestWidth);
        }
    }

    for (int i = 0; i < buttonsList.size(); ++i)
    {
        auto& button = buttonsList[i];
        if (!button.actualInstance)
            continue;

        bool isLastButton = i == buttonsList.size() - 1;

        ImVec2 size = ImVec2(0, 0);
        if (isLastButton)
        {
            size = ImGui::CalcTextSize(ws2s(button.getString()).c_str());
            size.x *= 2;

            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + size.y);

            size.y *= 2;
        }

        if (ButtonCenteredOnLine(ws2s(button.getString()).c_str(), size,0.5f, (!isLastButton ? longestWidth : 0)))
        {
            button.Press();
        }
    }


    ImGui::End();
}

void SecurityOptionControlWrapper::Press()
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
}

void uiSecurityControl::InitHooks(uintptr_t baseaddress)
{
    LogonViewManager__ShowSecurityOptionsUIThread = decltype(LogonViewManager__ShowSecurityOptionsUIThread)(baseaddress + 0x2BFC0);
    LogonViewManager__ShowSecurityOptions = decltype(LogonViewManager__ShowSecurityOptions)(baseaddress + 0x2964C);
    MakeAndInitialize_SecurityOptionControl = decltype(MakeAndInitialize_SecurityOptionControl)(baseaddress + 0x37774);
    SecurityOptionControlHandleKeyInput = decltype(SecurityOptionControlHandleKeyInput)(baseaddress + 0x40680);
    //SecurityOptionControlHandleKeyInput = decltype(SecurityOptionControlHandleKeyInput)(baseaddress + 0x44490);
    //ConsoleUIView__Initialize = decltype(ConsoleUIView__Initialize)(baseaddress + 0x42710);
    //ConsoleUIView__HandleKeyInput = decltype(ConsoleUIView__HandleKeyInput)(baseaddress + 0x43530);
    SecurityOptionControl_Destructor = decltype(SecurityOptionControl_Destructor)(baseaddress + 0x40C40);
    SecurityOptionsView__RuntimeClassInitialize = decltype(SecurityOptionsView__RuntimeClassInitialize)(baseaddress + 0x36EB4);
    CredUIManager__ShowCredentialView = decltype(CredUIManager__ShowCredentialView)(baseaddress + 0x201BC);
    SecurityOptionsView__Destructor = decltype(SecurityOptionsView__Destructor)(baseaddress + 0x37880);

    Hook(LogonViewManager__ShowSecurityOptionsUIThread, LogonViewManager__ShowSecurityOptionsUIThread_Hook);
    Hook(LogonViewManager__ShowSecurityOptions, LogonViewManager__ShowSecurityOptions_Hook);
    Hook(MakeAndInitialize_SecurityOptionControl, MakeAndInitialize_SecurityOptionControl_Hook);
    Hook(SecurityOptionControlHandleKeyInput, SecurityOptionControlHandleKeyInput_Hook);
    Hook(SecurityOptionControl_Destructor, SecurityOptionControl_Destructor_Hook);
    Hook(SecurityOptionsView__RuntimeClassInitialize, SecurityOptionsView__RuntimeClassInitialize_Hook);
    Hook(CredUIManager__ShowCredentialView, CredUIManager__ShowCredentialView_Hook);
    Hook(SecurityOptionsView__Destructor, SecurityOptionsView__Destructor_Hook);
    //Hook(ConsoleUIView__Initialize, ConsoleUIView__Initialize_Hook);
}