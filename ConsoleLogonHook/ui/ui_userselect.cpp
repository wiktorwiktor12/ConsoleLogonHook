#include "ui_userselect.h"
#include "detours.h"
#include "spdlog/spdlog.h"
#include "../util/util.h"
#include <winstring.h>
#include <sddl.h>
#include <vector>
#include <atlbase.h>
#include "util/interop.h"

std::vector<SelectableUserOrCredentialControlWrapper> buttons;
const int signInOptionChoice = 0;
int choiceIteration = 0;

__int64(__fastcall* CredProvSelectionView__v_OnKeyInput)(void* _this, const struct _KEY_EVENT_RECORD* a2, int* a3);
__int64(__fastcall* UserSelectionView__v_OnKeyInput)(void* _this, const struct _KEY_EVENT_RECORD* a2, int* a3);

void* UserSelectionView = 0;

__int64(__fastcall* UserSelectionView__RuntimeClassInitialize)(void* _this, void* a2);
__int64 UserSelectionView__RuntimeClassInitialize_Hook(void* _this, void* a2)
{
    SPDLOG_INFO("UserSelectionView__RuntimeClassInitialize_Hook a2 [{}]", a2);
    UserSelectionView = _this;

    /*auto userSelect = uiRenderer::Get()->GetWindowOfTypeId<uiUserSelect>(5);
    if (userSelect)
    {
        SPDLOG_INFO("Setting active status userSelect");
        userSelect->SetActive();
        MinimizeLogonConsole();

        
    }*/
    external::UserSelect_SetActive();

    auto res = UserSelectionView__RuntimeClassInitialize(_this, a2);

    external::SelectableUserOrCredentialControl_Sort();

    std::sort(buttons.begin(), buttons.end(), [](SelectableUserOrCredentialControlWrapper& a, SelectableUserOrCredentialControlWrapper& b) { return a.GetText() < b.GetText(); });

    return res;
}

void* CredProvSelectionView;

__int64(__fastcall* CredProvSelectionView__RuntimeClassInitialize)(void* _this, void* a2, HSTRING a3, char a4);
__int64 CredProvSelectionView__RuntimeClassInitialize_Hook(void* _this, void* a2, HSTRING a3, char a4)
{
    SPDLOG_INFO("CredProvSelectionView__RuntimeClassInitialize a3 [{}]", ws2s(ConvertHStringToString(a3)));
    choiceIteration = 0;
    CredProvSelectionView = _this;
    auto res = CredProvSelectionView__RuntimeClassInitialize(_this, a2, a3, a4);

    return res;
}

__int64 ConsoleUIView__Initialize_Hook(void* _this)
{
    globals::ConsoleUIView = _this;
    return globals::ConsoleUIView__Initialize(_this);
}

__int64 (__fastcall* SelectableUserOrCredentialControl__RuntimeClassInitialize)(void* _this, void* a2, void* a3);
__int64 SelectableUserOrCredentialControl__RuntimeClassInitialize_Hook(void* _this, void* a2, void* a3)
{
    auto res = SelectableUserOrCredentialControl__RuntimeClassInitialize(_this,a2,a3);

    SelectableUserOrCredentialControlWrapper wrapper;
    wrapper.actualInstance = _this;

    if (wrapper.isCredentialControl())
    {
        if (choiceIteration == signInOptionChoice)
        {
			//auto userSelect = uiRenderer::Get()->GetWindowOfTypeId<uiUserSelect>(5);
            if (globals::wasInSelectedCredentialView)
            {
                wrapper.virtualKeyCodeToPress = VK_ESCAPE;
                globals::wasInSelectedCredentialView = false;
            }
            wrapper.markedPressed = true; // it wont work if we press here, so we defer it till it do work
            wrapper.tickMarkedPressed = GetTickCount64();
        }
        choiceIteration++;
    }


    for (int i = 0; i < *(int*)(__int64(globals::ConsoleUIView) + 0x30); ++i)
    {
        void* controlHandle = *(void**)(*(uintptr_t*)(__int64(globals::ConsoleUIView) + 0x28) + 8 * i);
        if (controlHandle && *(void**)(__int64(controlHandle) + 0x20) == (void*)(__int64(_this) + 8))
        {
            SPDLOG_INFO("Found at index {} controlhandleptr {}",i,controlHandle);
            wrapper.controlHandleIndex = i;
            break;
        }
    }

    WCHAR* str = 0;
    //GetSIDStringFromUsername(wrapper.GetText().c_str(), &str);
    //PCWSTR fromString = wrapper.GetText().c_str();
    
    //std::wstring wstr = wrapper.GetText();
    //int size = (wstr.length() + 1) * sizeof(wchar_t);
    //wchar_t* username = (wchar_t*)malloc(size);;
    //wcscpy_s(username, size, wstr.c_str());

    //MessageBoxW(0, username, username, 0);

    if (!wrapper.isCredentialControl())
    {

        std::wstring sid = L"";
        auto hr = GetSIDStringFromUsername(wrapper.GetText().c_str(), &sid);
        auto path = GetProfilePicturePathFromSID(sid);
            
        external::SelectableUserOrCredentialControl_Create(wrapper.actualInstance, path);

    }

    SPDLOG_INFO("SelectableUserOrCredentialControl__RuntimeClassInitialize_Hook, user name {} this {} a3 {} SID {}", ws2s(wrapper.GetText()),_this,a3, str ? ws2s(str) : "NULL");
    buttons.push_back(wrapper);

    return res;
}

__int64(__fastcall* CredUIViewManager__ShowCredentialView)(void* _this, HSTRING a2);
__int64 CredUIViewManager__ShowCredentialView_Hook(void* _this, HSTRING a2)
{
    auto res = CredUIViewManager__ShowCredentialView(_this,a2);

    return res;
}

void* (__fastcall* SelectableUserOrCredentialControl_Destructor)(void* _this,char a2);
void* SelectableUserOrCredentialControl_Destructor_Hook(void* _this, char a2)
{
    external::SelectableUserOrCredentialControl_Destroy(_this);
    for (int i = 0; i < buttons.size(); ++i)
    {
        auto& button = buttons[i];
        if (button.actualInstance == _this)
        {
			//auto userSelect = uiRenderer::Get()->GetWindowOfTypeId<uiUserSelect>(5);
            if (globals::wasInSelectedCredentialView)
            {
                button.virtualKeyCodeToPress = VK_ESCAPE;
                button.markedPressed = false;
                globals::wasInSelectedCredentialView = false;
            }

            SPDLOG_INFO("Found button instance and removing!");
            buttons.erase(buttons.begin() + i);
            break;
        }
    }


    return SelectableUserOrCredentialControl_Destructor(_this,a2);
}

unsigned long long tickLocked = INT_MAX - 1;
bool locked = false;

__int64(__fastcall* LogonViewManager__Lock)(__int64 a1, int a2, char a3, HSTRING a4, __int64 a5);
__int64 LogonViewManager__Lock_Hook(__int64 a1, int a2, char a3, HSTRING a4, __int64 a5)
{
    auto res = LogonViewManager__Lock(a1, a2, a3, a4, a5);

    SPDLOG_INFO("LogonViewManager__Lock_Hook {} {} {} {} {}", (void*)a1, a2, (int)a3, ws2s(ConvertHStringToString(a4)), (void*)a5);

    tickLocked = GetTickCount64();
    locked = true;

    return res;
}

DWORD WINAPI TickThread(LPVOID lparam)
{
    while (true)
    {
        for (int i = 0; i < buttons.size(); ++i)
        {
            auto& button = buttons[i];
            if (button.markedPressed && GetTickCount64() - button.tickMarkedPressed >= 15)
            {
                button.Press();
                button.markedPressed = false;
                button.virtualKeyCodeToPress = VK_RETURN; //reset after an override
                button.tickMarkedPressed = 0;
            }
        }
        if (locked && GetTickCount64() - tickLocked >= 25)
        {
            locked = false;

            KEY_EVENT_RECORD rec;
            rec.bKeyDown = true;
            rec.dwControlKeyState = LEFT_CTRL_PRESSED | LEFT_ALT_PRESSED;
            rec.wVirtualKeyCode = VK_DELETE;
            globals::ConsoleUIView__HandleKeyInput((void*)(__int64(globals::ConsoleUIView) + 8), &rec);
        }
    }

    return 0;
}

void uiUserSelect::InitHooks(uintptr_t baseaddress)
{
    UserSelectionView__RuntimeClassInitialize = decltype(UserSelectionView__RuntimeClassInitialize)(baseaddress + 0x378F0);
    SelectableUserOrCredentialControl__RuntimeClassInitialize = decltype(SelectableUserOrCredentialControl__RuntimeClassInitialize)(baseaddress + 0x3FD24);
    CredProvSelectionView__RuntimeClassInitialize = decltype(CredProvSelectionView__RuntimeClassInitialize)(baseaddress + 0x35640);
    CredProvSelectionView__v_OnKeyInput = decltype(CredProvSelectionView__v_OnKeyInput)(baseaddress + 0x35A00);

    SelectableUserOrCredentialControl_Destructor = decltype(SelectableUserOrCredentialControl_Destructor)(baseaddress + 0x363A8);
    UserSelectionView__v_OnKeyInput = decltype(UserSelectionView__v_OnKeyInput)(baseaddress + 0x37C30);
    CredUIViewManager__ShowCredentialView = decltype(CredUIViewManager__ShowCredentialView)(baseaddress + 0x201BC);

    globals::ConsoleUIView__Initialize = decltype(globals::ConsoleUIView__Initialize)(baseaddress + 0x42710);
    globals::ConsoleUIView__HandleKeyInput = decltype(globals::ConsoleUIView__HandleKeyInput)(baseaddress + 0x43530);

    LogonViewManager__Lock = decltype(LogonViewManager__Lock)(baseaddress + 0x2884C);
    Hook(LogonViewManager__Lock, LogonViewManager__Lock_Hook);

    Hook(UserSelectionView__RuntimeClassInitialize, UserSelectionView__RuntimeClassInitialize_Hook);
    Hook(SelectableUserOrCredentialControl__RuntimeClassInitialize, SelectableUserOrCredentialControl__RuntimeClassInitialize_Hook);
    Hook(CredProvSelectionView__RuntimeClassInitialize, CredProvSelectionView__RuntimeClassInitialize_Hook);
    Hook(SelectableUserOrCredentialControl_Destructor, SelectableUserOrCredentialControl_Destructor_Hook);
    Hook(globals::ConsoleUIView__Initialize, ConsoleUIView__Initialize_Hook);
    Hook(CredUIViewManager__ShowCredentialView, CredUIViewManager__ShowCredentialView_Hook);

    CreateThread(0,0, TickThread,0,0,0);
}

void external::ConsoleUIView__HandleKeyInputExternal(void* instance, const struct _KEY_EVENT_RECORD* keyrecord)
{
    globals::ConsoleUIView__HandleKeyInput((void*)(__int64(instance) + 8),keyrecord);
}

void* external::GetConsoleUIView()
{
    return globals::ConsoleUIView;
}

const wchar_t* external::SelectableUserOrCredentialControl_GetText(void* actualInstance)
{
    for (int i = 0; i < buttons.size(); ++i)
    {
        auto& button = buttons[i];
        if (button.actualInstance != actualInstance)
            continue;

        return button.GetText().c_str();
    }
    return L"";
}

void external::SelectableUserOrCredentialControl_Press(void* actualInstance)
{
    for (int i = 0; i < buttons.size(); ++i)
    {
        auto& button = buttons[i];
        if (button.actualInstance != actualInstance)
            continue;

        button.Press();

        break;
    }
}

bool external::SelectableUserOrCredentialControl_isCredentialControl(void* actualInstance)
{
    for (int i = 0; i < buttons.size(); ++i)
    {
        auto& button = buttons[i];
        if (button.actualInstance != actualInstance)
            continue;

        return button.isCredentialControl();
    }
    return false;
}

std::wstring SelectableUserOrCredentialControlWrapper::GetText()
{
    if (hastext)
        return text;

    uintptr_t pointer = *(uintptr_t*)(__int64(actualInstance) + 0x58);
    if (!pointer)
        pointer = *(uintptr_t*)(__int64(actualInstance) + 0x50);

    HSTRING hstring;

    __int64 result = (*(__int64(__fastcall**)(__int64, HSTRING*))(*(uintptr_t*)pointer + 0x40i64))(pointer, &hstring); //keep this line as is, this is very funky when it comes down to specifics
    if (result < 0)
        return L""; //failure

    text = ConvertHStringToString(hstring);
    hastext = true;

    fWindowsDeleteString(hstring);
    return text;
}

//please dont touch, its messy, needs clean up, but its fragile af, ill fix at a later date
void SelectableUserOrCredentialControlWrapper::Press()
{
    _KEY_EVENT_RECORD keyrecord;
    keyrecord.bKeyDown = true;
    keyrecord.wVirtualKeyCode = virtualKeyCodeToPress;
    int result;
    if (actualInstance)
    {
        SPDLOG_INFO("Actual instance {} {} isn't null, so we are calling handlekeyinput with enter on the control!", actualInstance, virtualKeyCodeToPress);

        void* instanceToUse = nullptr;

        if (isCredentialControl())
        {
            if (!CredProvSelectionView)
            {
                SPDLOG_INFO("CredProvSelectionView IS NULL< CANNOT PRESS!");

            }
            instanceToUse = CredProvSelectionView;
        }
        else
        {
            if (!UserSelectionView)
            {
                SPDLOG_INFO("UserSelectionView IS NULL< CANNOT PRESS!");

            }
            instanceToUse = CredProvSelectionView;
        }

        if (instanceToUse)
        {
            if (isCredentialControl())
            {
                void* plus8 = (void*)(__int64(globals::ConsoleUIView) + 8);
                *(int*)(__int64(plus8) + 0x40) = controlHandleIndex;

                globals::ConsoleUIView__HandleKeyInput((void*)(__int64(globals::ConsoleUIView) + 8), &keyrecord);
                //*(void**)(__int64(instanceToUse) + 0x70) = actualInstance;
                //CredProvSelectionView__v_OnKeyInput((void*)(__int64(instanceToUse) + 8),&keyrecord,&result);
            }
            else
            {
                SPDLOG_INFO("Calling UserSelectionView__v_OnKeyInput");
                //*(void**)(__int64(instanceToUse) + 0x48) = actualInstance;
                //UserSelectionView__v_OnKeyInput((void*)(__int64(instanceToUse) + 8), &keyrecord, &result);
                //UserSelectionView__v_OnKeyInput((void*)(__int64(instanceToUse) + 8), &keyrecord, &result);
                void* plus8 = (void*)(__int64(globals::ConsoleUIView) + 8);
                *(int*)(__int64(plus8) + 0x40) = controlHandleIndex;
                //UserSelectionView__v_OnKeyInput((void*)(__int64(instanceToUse) + 8), &keyrecord, &result);
                globals::ConsoleUIView__HandleKeyInput((void*)(__int64(globals::ConsoleUIView) + 8),&keyrecord);
            }

            //virtualKeyCodeToPress = VK_RETURN; //reset after an override
        }
        //else
        //{
        //    *(void**)(__int64(UserSelectionView) + 0x70) = actualInstance;
        //    UserSelectionView__v_OnKeyInput((void*)(__int64(UserSelectionView) + 8), &keyrecord, &result);
        //}
    }
}

bool SelectableUserOrCredentialControlWrapper::isCredentialControl()
{
    return *(uintptr_t*)(__int64(actualInstance) + 0x50) != NULL;
}
