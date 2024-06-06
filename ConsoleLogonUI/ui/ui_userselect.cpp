#include "ui_userselect.h"
#include "spdlog/spdlog.h"
#include "../util/util.h"
#include <winstring.h>
#include "ui_helper.h"
#include <sddl.h>
#include <vector>
#include <atlbase.h>
#include "util/interop.h"

std::vector<SelectableUserOrCredentialControlWrapper> buttons;

void external::UserSelect_SetActive()
{
    //MessageBoxW(0, L"1", L"1", 0);
    HideConsoleUI();
    auto userSelect = uiRenderer::Get()->GetWindowOfTypeId<uiUserSelect>(5);
    if (userSelect)
    {
        SPDLOG_INFO("Setting active status userSelect");
        userSelect->SetActive();


    }
}

void external::SelectableUserOrCredentialControl_Sort()
{
    std::sort(buttons.begin(), buttons.end(), [](SelectableUserOrCredentialControlWrapper& a, SelectableUserOrCredentialControlWrapper& b) { return a.GetText() < b.GetText(); });
}

//TODO: move the texture loading to another thread so that it does not delay the whole program loading the textures
void external::SelectableUserOrCredentialControl_Create(void* actualInstance, const wchar_t* path)
{
    SelectableUserOrCredentialControlWrapper wrapper;
    wrapper.actualInstance = actualInstance;
    int w = 0;
    int h = 0;
    if (uiRenderer::LoadTextureFromFile(ws2s(path).c_str(), &wrapper.texture, &w, &h))
        SPDLOG_INFO("TEXTURE LOADED");

    buttons.push_back(wrapper);
}

void external::SelectableUserOrCredentialControl_Destroy(void* actualInstance)
{
    for (int i = 0; i < buttons.size(); ++i)
    {
        auto& button = buttons[i];
        if (button.actualInstance == actualInstance)
        {
            SPDLOG_INFO("Found button instance and removing!");
            buttons.erase(buttons.begin() + i);
            break;
        }
    }
}

void uiUserSelect::Tick()
{

}

void uiUserSelect::Draw()
{
    ImGui::Begin("Status View", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground);

    ImGui::SetWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::SetWindowPos(ImVec2(0, 0));

    //ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y * 0.5);
    //ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x * 0.5);

    //ImGui::BeginChild("", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetContentRegionAvail().y * 0.5),0,ImGuiWindowFlags_HorizontalScrollbar);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y * 0.45);

    ImVec2 size;
    for (int i = 0; i < buttons.size(); ++i)
    {
        auto& control = buttons[i];

        if (!control.actualInstance)
            continue;

        if (control.isCredentialControl())
            continue;

        if (size.y <= 0)
        {
            size = CalcTextButtonSize(ws2s(control.GetText()));
            continue;
        }

        size.x += CalcTextButtonSize(ws2s(control.GetText())).x;
    }

    //bool bFirst = true;
    //for (int i = 0; i < buttons.size(); ++i)
    //{
    //    auto& button = buttons[i];
    //    if (!button.actualInstance)
    //        continue;
    //
    //    if (button.isCredentialControl())
    //        continue;
    //    //ImGui::Image(button.texture, ImVec2(32, 32));
    //    if (bFirst)
    //    {
    //        //ImGui::SameLine();
    //        ButtonCenteredOnLineNoCall(ws2s(button.GetText()).c_str(), size);
    //        bFirst = false;
    //    }
    //    else if (i < buttons.size())
    //    {
    //        //ImGui::SameLine();
    //        ImGui::SameLine();
    //    }
    //
    //    auto btnsize = CalcTextButtonSize(ws2s(button.GetText()).c_str());
    //
    //    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ((btnsize.x - 32) / 2));
    //    ImGui::Image(button.texture, ImVec2(64, 64));
    //
    //    //if (i != buttons.size() - 1)
    //    //    ImGui::SameLine();
    //}
    bool bFirst = true;
    for (int i = 0; i < buttons.size(); ++i)
    {
        auto& button = buttons[i];
        if (!button.actualInstance)
            continue;

        if (button.isCredentialControl())
            continue;
        //ImGui::Image(button.texture, ImVec2(32, 32));
        if (bFirst)
        {
            //ImGui::SameLine();
            ButtonCenteredOnLineNoCall(ws2s(button.GetText()).c_str(), size);
            bFirst = false;
        }
        else if (i < buttons.size())
        {
            //ImGui::SameLine();
            ImGui::SameLine();
        }


        //if (ImGui::ImageButton(ws2s(button.GetText()).c_str()))
        //if (ImGui::ImageButton(ws2s(button.GetText()).c_str(),button.texture,ImVec2(64,64)))
        if (button.texture)
        {
            if (ImGui::ImageButtonLabelled(ws2s(button.GetText()).c_str(), button.texture, ImVec2(48, 48)))
                button.Press();
        }
        else
            if (ImGui::Button(ws2s(button.GetText()).c_str()))
                button.Press();


        //if (i != buttons.size() - 1)
        //    ImGui::SameLine();
    }

    //bFirst = true;
    //for (int i = 0; i < buttons.size(); ++i)
    //{
    //    auto& button = buttons[i];
    //    if (!button.actualInstance)
    //        continue;
    //
    //    if (button.isCredentialControl())
    //        continue;
    //
    //    if (!button.texture)
    //        continue;
    //
    //    //float xStart = (ImGui::GetIO().DisplaySize.x / 2) - ((48) * (buttons.size() - i)) /* - (ImGui::GetStyle().FramePadding.x * i)*/;
    //    float xStart = (ImGui::GetIO().DisplaySize.x - (48 * 3 * 0.5) );
    //    xStart = xStart + (48 * i) + ImGui::GetStyle().FramePadding.x * 2*i;
    //    float xEnd = xStart + ImGui::GetStyle().FramePadding.x * 2 + 48;
    //    //ImGui::SetCursorPosX(xStart);
    //
    //    if (bFirst)
    //    {
    //        //ButtonCenteredOnLineNoCall(ws2s(button.GetText()).c_str(), size);
    //        bFirst = false;
    //    }
    //    else if (i < buttons.size())
    //    {
    //        ImGui::SameLine();
    //    
    //    
    //        //if (textSize.x < 48)
    //        //    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ((textSize.x - 24) / 2));
    //        
    //        //ImGui::SetCursorPosX(ImGui::GetCursorPosX() + textSize.x / 2);
    //    }
    //
    //    ImVec2 textSize = ImGui::CalcTextSize(ws2s(button.GetText()).c_str());
    //    ImGui::SetCursorPosX(xStart + textSize.x / 2);
    //
    //    ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + xEnd);
    //    ImGui::TextWrapped(ws2s(button.GetText()).c_str());
    //}
    ImGui::EndChild();

    ImGui::End();
}

std::wstring SelectableUserOrCredentialControlWrapper::GetText()
{
    if (hastext)
        return text;

    text = external::SelectableUserOrCredentialControl_GetText(actualInstance);
    hastext = true;
    return text;
}

void SelectableUserOrCredentialControlWrapper::Press()
{
    return external::SelectableUserOrCredentialControl_Press(actualInstance);
}

bool SelectableUserOrCredentialControlWrapper::isCredentialControl()
{
    return external::SelectableUserOrCredentialControl_isCredentialControl(actualInstance);
}
