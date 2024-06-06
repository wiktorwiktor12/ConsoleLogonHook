#include "ui_messageview.h"
#include "spdlog/spdlog.h"
#include <winstring.h>
#include "util/util.h"
#include "ui_helper.h"
#include "util/interop.h"

std::vector<MessageOptionControlWrapper> controls;

void external::MessageView_SetActive()
{
    HideConsoleUI();
    auto messageView = uiRenderer::Get()->GetWindowOfTypeId<uiMessageView>(3);
    if (messageView)
        messageView->SetActive();
}

void external::MessageOptionControl_Create(void* actualInsance, int optionflag)
{
    MessageOptionControlWrapper wrapper;
    wrapper.actualInstance = actualInsance;
    wrapper.optionflag = optionflag;
    controls.push_back(wrapper);
}

void external::MessageView_SetMessage(const wchar_t* message)
{
    auto messageView = uiRenderer::Get()->GetWindowOfTypeId<uiMessageView>(3);
    if (messageView)
    {
        messageView->message = message;

    }
}

void external::MessageOptionControl_Destroy(void* actualInstance)
{
    for (int i = 0; i < controls.size(); ++i)
    {
        auto& button = controls[i];
        if (button.actualInstance == actualInstance)
        {
            SPDLOG_INFO("FOUND AND DELETING MessageOptionControl");
            controls.erase(controls.begin() + i);
            break;
        }
    }
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

void MessageOptionControlWrapper::Press()
{
    _KEY_EVENT_RECORD keyrecord;
    keyrecord.bKeyDown = true;
    keyrecord.wVirtualKeyCode = VK_RETURN;
    int result;
    if (actualInstance)
    {
        SPDLOG_INFO("Actual instance {} isn't null, so we are calling MessageOptionControl_Press with enter on the control!", actualInstance);

        external::MessageOptionControl_Press(actualInstance, &keyrecord, &result);
    }
}

std::wstring MessageOptionControlWrapper::GetText()
{
    return external::MessageOptionControl_GetText(actualInstance);
}