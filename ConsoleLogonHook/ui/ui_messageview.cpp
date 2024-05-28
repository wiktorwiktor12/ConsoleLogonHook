#include "ui_messageview.h"
#include "detours.h"
#include "spdlog/spdlog.h"
#include "../util/util.h"
#include <winstring.h>
#include "ui_helper.h"

std::vector<MessageOptionControlWrapper> controls;

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

    auto messageView = uiRenderer::Get()->GetWindowOfTypeId<uiMessageView>(3);
    if (messageView)
        messageView->SetActive();

    //SPDLOG_INFO("MessageView__RuntimeClassInitialize a1[{}] a2[{}] a3[{}] a4[{}] a5[{}] a6[{}]", (void*)a1, ws2s(convertString(a2)).c_str(), ws2s(string).c_str(), (int)a4, (void*)a5, (void*)a6);
    SPDLOG_INFO("a3 {} length {}", ws2s(string), string.size());
    return res;
}

__int64(__fastcall* MessageOptionControl__RuntimeClassInitialize)(void* _this, void* a2, int a3, void* a4);
__int64 MessageOptionControl__RuntimeClassInitialize_Hook(void* _this, void* a2, int a3, void* a4)
{
    auto res = MessageOptionControl__RuntimeClassInitialize(_this,a2,a3,a4);

    MessageOptionControlWrapper wrapper;
    wrapper.actualInstance = _this;
    wrapper.optionflag = a3;

    SPDLOG_INFO("MessageOptionControl__RuntimeClassInitialize_Hook {} {}", ws2s(wrapper.GetText()),a3);

    controls.push_back(wrapper);

    return res;
}

//__int64(__fastcall* CredUIViewManager__ShowCredentialView)(void* _this, HSTRING a2);
//__int64 CredUIViewManager__ShowCredentialView_Hook(void* _this, HSTRING a2)
//{
//    SPDLOG_INFO("CredUIViewManager__ShowCredentialView_Hook a2 [{}]", ws2s(ConvertHStringToString(a2)));
//    return CredUIViewManager__ShowCredentialView(_this,a2);
//}


__int64(__fastcall* BasicTextControl__RuntimeClassInitialize1)(void* _this, void* a2, const wchar_t* a3, char a4);
__int64 BasicTextControl__RuntimeClassInitialize1_Hook(void* _this, void* a2, const wchar_t* a3, char a4)
{
    auto res = BasicTextControl__RuntimeClassInitialize1(_this, a2, a3, a4);

    SPDLOG_INFO("BasicTextControl__RuntimeClassInitialize1_Hook {} {} {} {}", _this, a2, ws2s(a3), (int)a4);

    auto messageView = uiRenderer::Get()->GetWindowOfTypeId<uiMessageView>(3);
    if (messageView)
    {
        messageView->message = a3;
        
    }

    return res;
}

__int64(__fastcall* BasicTextControl__RuntimeClassInitialize2)(void* _this, void* a2, __int64 a3);
__int64 BasicTextControl__RuntimeClassInitialize2_Hook(void* _this, void* a2, __int64 a3)
{
    auto res = BasicTextControl__RuntimeClassInitialize2(_this, a2, a3);

    SPDLOG_INFO("BasicTextControl__RuntimeClassInitialize2_Hook {} {} {} ", _this, a2, a3);

    return res;
}

__int64(__fastcall* MessageOptionControl__Destructor)(void* _this, char a2);
__int64 MessageOptionControl__Destructor_Hook(void* _this, char a2)
{
    for (int i = 0; i < controls.size(); ++i)
    {
        auto& button = controls[i];
        if (button.actualInstance == _this)
        {
            SPDLOG_INFO("FOUND AND DELETING MessageOptionControl");
            controls.erase(controls.begin() + i);
            break;
        }
    }

    return MessageOptionControl__Destructor(_this, a2);
}


void uiMessageView::Draw()
{
    ImGui::Begin("Message View", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground);

    ImGui::SetWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::SetWindowPos(ImVec2(0, 0));

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y * 0.45);

    if (message.size() > 0)
    {
        std::vector<std::wstring> lines = split((message), L"\n");
        for (int i = 0; i < lines.size(); ++i)
            CustomTextCenteredOnLine(ws2s(lines[i]).c_str());
    }

    ImVec2 size;
    for (int i = 0; i < controls.size(); ++i)
    {
        auto& control = controls[i];
        if (size.y <= 0)
        {
            size = CalcTextButtonSize(ws2s(control.GetText()));
            continue;
        }

        size.x += CalcTextButtonSize(ws2s(control.GetText())).x;
    }

    for (int i = 0; i < controls.size(); ++i)
    {
        auto& control = controls[i];
        bool pressed = false;
        if (i == 0)
            ButtonCenteredOnLineNoCall(ws2s(control.GetText()).c_str(), size);
        else if (i < controls.size() - 1)
            ImGui::SameLine();

        if (ImGui::Button(ws2s(control.GetText()).c_str()))
        {
            SPDLOG_INFO("PRESSED");
            control.Press();
        }
    }

    ImGui::End();
}

void uiMessageView::InitHooks(uintptr_t baseaddress)
{
    MessageView__RuntimeClassInitialize = decltype(MessageView__RuntimeClassInitialize)(baseaddress + 0x389B0);
    //CredUIViewManager__ShowCredentialView = decltype(CredUIViewManager__ShowCredentialView)(baseaddress + 0x201BC);
    BasicTextControl__RuntimeClassInitialize1 = decltype(BasicTextControl__RuntimeClassInitialize1)(baseaddress + 0x43FB8);
    BasicTextControl__RuntimeClassInitialize2 = decltype(BasicTextControl__RuntimeClassInitialize2)(baseaddress + 0x44104);
    MessageOptionControl__RuntimeClassInitialize = decltype(MessageOptionControl__RuntimeClassInitialize)(baseaddress + 0x40D38);
    MessageOptionControl__Destructor = decltype(MessageOptionControl__Destructor)(baseaddress + 0x402A0);
    MessageOptionControl__v_HandleKeyInput = decltype(MessageOptionControl__v_HandleKeyInput)(baseaddress + 0x41040);


    Hook(MessageView__RuntimeClassInitialize, MessageView__RuntimeClassInitialize_Hook);
    //Hook(CredUIViewManager__ShowCredentialView, CredUIViewManager__ShowCredentialView_Hook);
    Hook(BasicTextControl__RuntimeClassInitialize1, BasicTextControl__RuntimeClassInitialize1_Hook);
    Hook(BasicTextControl__RuntimeClassInitialize2, BasicTextControl__RuntimeClassInitialize2_Hook);
    Hook(MessageOptionControl__RuntimeClassInitialize, MessageOptionControl__RuntimeClassInitialize_Hook);
    Hook(MessageOptionControl__Destructor, MessageOptionControl__Destructor_Hook);
}

void MessageOptionControlWrapper::Press()
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
}

std::wstring MessageOptionControlWrapper::GetText()
{
    const wchar_t*& rawptr = *(const wchar_t**)(__int64(actualInstance) + 0x48);
    return std::wstring(rawptr);
}