#include "ui_statusview.h"
#include "spdlog/spdlog.h"
#include "../util/util.h"
#include <winstring.h>
#include "ui_helper.h"
#include "util/interop.h"

void external::StatusView_SetActive(const wchar_t* text)
{
    auto statusview = uiRenderer::Get()->GetWindowOfTypeId<uiStatusView>(4);
    if (statusview)
    {
        SPDLOG_INFO("Setting active status view instance");
        statusview->statusText = text;
        statusview->SetActive();
    }
}

void uiStatusView::Draw()
{
    ImGui::Begin("Status View", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground);

    ImGui::SetWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::SetWindowPos(ImVec2(0, 0));

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y * 0.45);

    if (statusText.size() > 0)
    {
        std::vector<std::wstring> lines = split((statusText), L"\n");
        for (int i = 0; i < lines.size(); ++i)
            CustomTextCenteredOnLine(ws2s(lines[i]).c_str());
    }

    ImGui::End();
}
