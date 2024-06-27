#include "ui_messageview.h"
#include <windows.h>
#include "detours.h"
#include "spdlog/spdlog.h"
#include "../util/util.h"
#include <winstring.h>
#include <util/interop.h>
#include <util/memory_man.h>

__int64 (__fastcall* MessageOptionControl__v_HandleKeyInput)(void* _this, const struct _KEY_EVENT_RECORD* a2, int* a3);

__int64(__fastcall* MessageView__RuntimeClassInitialize)(__int64 a1, HSTRING a2, HSTRING a3, char a4, __int64 a5, __int64 a6);
__int64 MessageView__RuntimeClassInitialize_Hook(__int64 a1, HSTRING a2, HSTRING a3, char a4, __int64 a5, __int64 a6)
{
    auto convertString = [&](HSTRING str) -> std::wstring
        {
            const wchar_t* convertedString = fWindowsGetStringRawBuffer(a2, 0);
            return convertedString;
        };
   
    std::wstring string = convertString(a3);
    auto res = MessageView__RuntimeClassInitialize(a1, a2, a3, a4, a5, a6);
    if (string.size() == 0)
        string = convertString(a3);

    external::MessageView_SetActive();

    //SPDLOG_INFO("MessageView__RuntimeClassInitialize a1[{}] a2[{}] a3[{}] a4[{}] a5[{}] a6[{}]", (void*)a1, ws2s(convertString(a2)).c_str(), ws2s(string).c_str(), (int)a4, (void*)a5, (void*)a6);
    SPDLOG_INFO("a3 {} length {}", ws2s(string), string.size());
    return res;
}

__int64(__fastcall* MessageOptionControl__RuntimeClassInitialize)(void* _this, void* a2, int a3, void* a4);
__int64 MessageOptionControl__RuntimeClassInitialize_Hook(void* _this, void* a2, int a3, void* a4)
{
    auto res = MessageOptionControl__RuntimeClassInitialize(_this,a2,a3,a4);

    //MessageOptionControlWrapper wrapper;
    //wrapper.actualInstance = _this;
    //wrapper.optionflag = a3;

    //SPDLOG_INFO("MessageOptionControl__RuntimeClassInitialize_Hook {} {}", ws2s(wrapper.GetText()),a3);

    //MessageOptionControlWrapper_controls.push_back(wrapper);

    external::MessageOptionControl_Create(_this,a3);

    return res;
}

__int64(__fastcall* BasicTextControl__RuntimeClassInitialize1)(void* _this, void* a2, const wchar_t* a3, char a4);
__int64 BasicTextControl__RuntimeClassInitialize1_Hook(void* _this, void* a2, const wchar_t* a3, char a4)
{
    auto res = BasicTextControl__RuntimeClassInitialize1(_this, a2, a3, a4);

    SPDLOG_INFO("BasicTextControl__RuntimeClassInitialize1_Hook {} {} {} {}", _this, a2, ws2s(a3), (int)a4);

    external::MessageView_SetMessage(a3);

    return res;
}

__int64(__fastcall* BasicTextControl__RuntimeClassInitialize2)(void* _this, void* a2, __int64 a3);
__int64 BasicTextControl__RuntimeClassInitialize2_Hook(void* _this, void* a2, __int64 a3)
{
    auto res = BasicTextControl__RuntimeClassInitialize2(_this, a2, a3);

    SPDLOG_INFO("BasicTextControl__RuntimeClassInitialize2_Hook {} {} {} ", _this, a2, a3);

    SPDLOG_INFO("text is {}",ws2s(*(const wchar_t**)(__int64(_this) + 0x40)));

    return res;
}

__int64(__fastcall* MessageOptionControl__Destructor)(void* _this, char a2);
__int64 MessageOptionControl__Destructor_Hook(void* _this, char a2)
{
    /*for (int i = 0; i < MessageOptionControlWrapper_controls.size(); ++i)
    {
        auto& button = MessageOptionControlWrapper_controls[i];
        if (button.actualInstance == _this)
        {
            SPDLOG_INFO("FOUND AND DELETING MessageOptionControl");

            external::MessageOptionControl_Destroy(button.actualInstance);

            MessageOptionControlWrapper_controls.erase(MessageOptionControlWrapper_controls.begin() + i);
            break;
        }
    }*/
    external::MessageOptionControl_Destroy(_this);

    return MessageOptionControl__Destructor(_this, a2);
}

void uiMessageView::InitHooks(uintptr_t baseaddress)
{
    MessageView__RuntimeClassInitialize = memory::FindPatternCached<decltype(MessageView__RuntimeClassInitialize)>("MessageView__RuntimeClassInitialize", { "48 89 5C 24 10 48 89 74 24 18 55 57 41 54 41 56 41 57 48 8B EC 48 83 EC 50 41 8B F9","48 8B C4 48 89 58 10 48 89 70 18 48 89 78 20 55 41 54 41 55 41 56 41 57 48 8D 68 B1 48 81 EC D0 00 00 00"});
    //CredUIViewManager__ShowCredentialView = decltype(CredUIViewManager__ShowCredentialView)(baseaddress + 0x201BC);
    BasicTextControl__RuntimeClassInitialize1 = memory::FindPatternCached<decltype(BasicTextControl__RuntimeClassInitialize1)>("BasicTextControl__RuntimeClassInitialize1", { "48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 48 89 78 20 41 56 48 83 EC 20 48 8B F9 44 88 49 58" });
    BasicTextControl__RuntimeClassInitialize2 = memory::FindPatternCached<decltype(BasicTextControl__RuntimeClassInitialize2)>("BasicTextControl__RuntimeClassInitialize2", { "48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 20 48 8B F2 48 8B F9 48 83 C1" });
    //MessageOptionControl__RuntimeClassInitialize = memory::FindPatternCached<decltype(MessageOptionControl__RuntimeClassInitialize)>("MessageOptionControl__RuntimeClassInitialize", "48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 4C 89 48 20 57 41 56 41 57 48 83 EC 20 49 8B D9 41 8B F8 4C 8B FA 48 8B F1 44 89 41 70");
    MessageOptionControl__RuntimeClassInitialize = memory::FindPatternCached<decltype(MessageOptionControl__RuntimeClassInitialize)>("MessageOptionControl__RuntimeClassInitialize", { "48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 4C 89 48 20 57 41 56 41 57 48 83 EC 20 49 8B D9 41 8B F8 4C 8B FA 48 8B F1 44 89 41 70","48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 41 56 41 57 48 83 EC 20 4C 8B FA 44 89 41 70 48 8B F1"});
    MessageOptionControl__Destructor = memory::FindPatternCached<decltype(MessageOptionControl__Destructor)>("MessageOptionControl__Destructor", {"48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 20 8B F2 48 8B D9 48 8B 79 68 48 83 61 68 00","48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 20 48 8B 79 68 8B F2 48 83 61 68 00 48 8B D9"});
    MessageOptionControl__v_HandleKeyInput = memory::FindPatternCached<decltype(MessageOptionControl__v_HandleKeyInput)>("MessageOptionControl__v_HandleKeyInput", { "48 89 5C 24 10 55 56 57 41 56 41 57 48 8B EC 48 83 EC 60 48 8B 05 ?? ?? ?? ?? 48 33 C4" });


    Hook(MessageView__RuntimeClassInitialize, MessageView__RuntimeClassInitialize_Hook);
    //Hook(CredUIViewManager__ShowCredentialView, CredUIViewManager__ShowCredentialView_Hook);
    Hook(BasicTextControl__RuntimeClassInitialize1, BasicTextControl__RuntimeClassInitialize1_Hook);
    Hook(BasicTextControl__RuntimeClassInitialize2, BasicTextControl__RuntimeClassInitialize2_Hook);
    Hook(MessageOptionControl__RuntimeClassInitialize, MessageOptionControl__RuntimeClassInitialize_Hook);
    Hook(MessageOptionControl__Destructor, MessageOptionControl__Destructor_Hook);
}

void external::MessageOptionControl_Press(void* actualInstance, const struct _KEY_EVENT_RECORD* keyrecord, int* success)
{
    if (actualInstance)
    {
        SPDLOG_INFO("Actual instance {} isn't null, so we are calling handlekeyinput with enter on the control!", actualInstance);

        MessageOptionControl__v_HandleKeyInput((void*)(__int64(actualInstance) + 8), keyrecord, success);
    }
}

const wchar_t* external::MessageOptionControl_GetText(void* actualInstance)
{
    const wchar_t*& rawptr = *(const wchar_t**)(__int64(actualInstance) + 0x48);
    return rawptr;
}

/*void MessageOptionControlWrapper::Press()
{
    _KEY_EVENT_RECORD keyrecord;
    keyrecord.bKeyDown = true;
    keyrecord.wVirtualKeyCode = VK_RETURN;
    int result;
    if (actualInstance)
    {
        SPDLOG_INFO("Actual instance {} isn't null, so we are calling handlekeyinput with enter on the control!", actualInstance);

        MessageOptionControl__v_HandleKeyInput((void*)(__int64(actualInstance) + 8), &keyrecord, &result);
    }
}*/

/*std::wstring MessageOptionControlWrapper::GetText()
{
    const wchar_t*& rawptr = *(const wchar_t**)(__int64(actualInstance) + 0x48);
    return std::wstring(rawptr);
}*/