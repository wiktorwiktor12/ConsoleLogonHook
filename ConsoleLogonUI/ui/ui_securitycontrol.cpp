#include "ui_securitycontrol.h"
#include "spdlog/spdlog.h"
#include "../util/util.h"
#include <winstring.h>
#include "ui_helper.h"
#include "util/interop.h"

std::vector<SecurityOptionControlWrapper> buttonsList;

void external::SecurityControlButtonsList_Clear()
{
    buttonsList.clear();
}

void external::SecurityControl_SetActive()
{
    HideConsoleUI();
    buttonsList.clear();
    for (int i = 0; i < uiRenderer::Get()->inactiveWindows.size(); ++i) //theres prob a better and nicer way to do this
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
    }
}

void external::SecurityControl_ButtonsReady()
{
    auto SwapButton = [&](int a, int b) -> void
        {
            SecurityOptionControlWrapper temp = buttonsList[a];
            buttonsList[a] = buttonsList[b];
            buttonsList[b] = temp;
        };
    
    SwapButton(0, 1);
    SwapButton(1, 3);
}

void external::SecurityOptionControl_Create(void* actualInstance)
{
    SecurityOptionControlWrapper button(actualInstance);
    SPDLOG_INFO("SecurityOptionControl_Create {} {}",(void*)actualInstance, ws2s(button.getString()));

    buttonsList.push_back(button);
}

void external::SecurityOptionControl_Destroy(void* actualInstance)
{
    for (int i = 0; i < buttonsList.size(); ++i)
    {
        auto& button = buttonsList[i];
        if (button.actualInstance == actualInstance)
        {
            SPDLOG_INFO("FOUND AND DELETING BUTTON");
            buttonsList.erase(buttonsList.begin() + i);
            break;
        }
    }
}

void external::SecurityControl_SetInactive()
{
    auto securityControl = uiRenderer::Get()->GetWindowOfTypeId<uiSecurityControl>(2);
    if (securityControl)
    {
        SPDLOG_INFO("setting inactive security control instance");
        securityControl->SetInactive();
    }
}

void uiSecurityControl::Draw()
{
    ImGui::Begin("Security Options",0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground);
    
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
            longestWidth = std::max<float>(CalcTextButtonSize(ws2s(button.getString())).x, longestWidth);
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
            //size = calcsize(ws2s(button.getString()));

            size = ImGui::CalcTextSize(ws2s(button.getString()).c_str());
            size.x *= 2;

            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + size.y);

            size.y *= 2;
        }
        else
        {
            size = CalcTextButtonSize(ws2s(button.getString()));
            size.x = longestWidth;
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
        SPDLOG_INFO("Actual instance {} isn't null, so we are calling SecurityOptionControl_Press with enter on the control!", actualInstance);

        external::SecurityOptionControl_Press(actualInstance, &keyrecord, &result);
    }
}

std::wstring SecurityOptionControlWrapper::getString()
{
    return external::SecurityOptionControl_getString(actualInstance);
}