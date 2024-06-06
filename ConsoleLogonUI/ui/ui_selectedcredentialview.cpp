#include "ui_selectedcredentialview.h"
#include "spdlog/spdlog.h"
#include "../util/util.h"
#include <winstring.h>
#include "ui_helper.h"
#include "ui_userselect.h"
#include <vector>
#include "ui_securitycontrol.h"
#include "util/interop.h"

std::vector<EditControlWrapper> editControls;

void external::NotifyWasInSelectedCredentialView()
{
	uiRenderer::Get()->GetWindowOfTypeId<uiUserSelect>(5)->wasInSelectedCredentialView = true;
}

void external::SelectedCredentialView_SetActive(const wchar_t* accountNameToDisplay)
{
	auto selectedCredentialView = uiRenderer::Get()->GetWindowOfTypeId<uiSelectedCredentialView>(6);
	if (selectedCredentialView)
	{
		SPDLOG_INFO("Setting active status selectedCredentialView");
		selectedCredentialView->accountNameToDisplay = accountNameToDisplay;
		selectedCredentialView->texture = nullptr;
		selectedCredentialView->textureExists = true;
		selectedCredentialView->SetActive();
	}
}

void external::EditControl_Create(void* actualInstance)
{
	EditControlWrapper wrapper;
	wrapper.actualInstance = actualInstance;
	wrapper.fieldNameCache = ws2s(wrapper.GetFieldName());
	//MessageBox(0, wrapper.GetFieldName().c_str(), wrapper.GetFieldName().c_str(),0);
	wrapper.inputBuffer = ws2s(wrapper.GetInputtedText());
	//MessageBox(0, wrapper.GetInputtedText().c_str(), wrapper.GetInputtedText().c_str(),0);


	//for (int i = editControls.size() - 1; i >= 0; --i)
	//{
	//	auto& control = editControls[i];
	//	if (control.GetFieldName() == wrapper.GetFieldName())
	//	{
	//		editControls.erase(editControls.begin() + i);
	//		break;
	//	}
	//}

	editControls.push_back(wrapper);
}

void external::EditControl_Destroy(void* actualInstance)
{
	for (int i = 0; i < editControls.size(); ++i)
	{
		auto& button = editControls[i];
		if (button.actualInstance == actualInstance)
		{
			SPDLOG_INFO("FOUND AND DELETING edit control");
			editControls.erase(editControls.begin() + i);
			break;
		}
	}
}

bool bWasInSecurityControl = false;
void uiSelectedCredentialView::Begin()
{
	if (!hasSetupNotify)
	{
		auto uiRenderer = uiRenderer::Get();
		if (uiRenderer)
		{
			auto securityControl = uiRenderer->GetWindowOfTypeId<uiSecurityControl>(2);
			if (securityControl)
			{
				securityControl->wasInSecurityControlNotifies.push_back([]() -> void { bWasInSecurityControl = true; });
				hasSetupNotify = true;
			}
		}
	}
}

void uiSelectedCredentialView::Tick()
{
	static bool bPressedEnter = false;
	if (GetAsyncKeyState(VK_RETURN))
	{
		if (!bPressedEnter)
		{
			bPressedEnter = true;

			if (uiRenderer::Get()->activeWindow.get() == this && uiRenderer::Get()->bAppIsFocused) //only send an enter if we are active window
			{
				KEY_EVENT_RECORD rec;
				rec.wVirtualKeyCode = VK_RETURN; //forward it to consoleuiview
				external::ConsoleUIView__HandleKeyInputExternal(external::GetConsoleUIView(), &rec);
			}
		}
	
	}
	else
		bPressedEnter = false;
}

void uiSelectedCredentialView::Draw()
{
	ImGui::Begin("Selected Credential View", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground);

    ImGui::SetWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::SetWindowPos(ImVec2(0, 0));

	float prcntToTakeInAccount = (128.f) / ImGui::GetContentRegionAvail().y;

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y * (0.45 - prcntToTakeInAccount));

	if (!texture && textureExists)
	{
		auto path = external::GetProfilePicturePathFromUsername(accountNameToDisplay);
		if (path.size() > 0)
		{
			int w = 0;
			int h = 0;
			textureExists = uiRenderer::LoadTextureFromFile(ws2s(path).c_str(), &texture, &w, &h);
		}
	}

	ImGui::SetCursorPosX((ImGui::GetIO().DisplaySize.x - 128) * 0.5f);
	if (texture)
		ImGui::Image(texture, ImVec2(128, 128));

	TextCenteredOnLine(ws2s(accountNameToDisplay).c_str());

	int lastIndex = -1;
	for (int x = editControls.size() - 1; x >= 0; --x)
	{
		auto& control = editControls[x];
		//if (control.isVisible())
		{
			lastIndex = x;
			break;
		}
	}

	for (int i = 0; i < editControls.size(); ++i)
	{
		auto& control = editControls[i];
		//if (control.isVisible())
		{
			ImGui::PushID(&control);
			float width = ImGui::CalcTextSize("a").x * 28;
			ImGui::SetNextItemWidth(width);
			ImVec2 size = CalcTextButtonSize(control.fieldNameCache);
			size.x = width;
	
			bool bCensor = (i > 0 && i <= 3);
	
			ButtonCenteredOnLineNoCall(control.fieldNameCache.c_str(), size);
			if (ImGui::InputTextWithHint("", control.fieldNameCache.c_str(), &control.inputBuffer, bCensor ? ImGuiInputTextFlags_Password : 0))
			{
				SPDLOG_INFO("TRUE {}",control.inputBuffer);
				control.SetInputtedText(s2ws(control.inputBuffer));
			}
			ImGui::PopID();
	
			bool isLast = (i == lastIndex);
	
			if (isLast)
			{
				ImGui::SameLine();
	
				if (ImGui::Button("->"))
				{
					KEY_EVENT_RECORD rec;
					rec.wVirtualKeyCode = VK_RETURN;
					external::ConsoleUIView__HandleKeyInputExternal(external::GetConsoleUIView(), &rec);
				}
			}
		}
	}

	std::string& text = bWasInSecurityControl ? cancel : switchUser;
	ImVec2 size;
	size = CalcTextButtonSize(text);
	size.y *= 2;

	if (ButtonCenteredOnLine(text.c_str(), size))
	{
		KEY_EVENT_RECORD rec;
		rec.wVirtualKeyCode = VK_ESCAPE;
		external::ConsoleUIView__HandleKeyInputExternal(external::GetConsoleUIView(), &rec);
	}

	ImGui::End();
}

std::wstring EditControlWrapper::GetFieldName()
{
	return external::EditControl_GetFieldName(actualInstance);
}

std::wstring EditControlWrapper::GetInputtedText()
{
	return external::EditControl_GetInputtedText(actualInstance);
}

void EditControlWrapper::SetInputtedText(std::wstring input)
{
	return external::EditControl_SetInputtedText(actualInstance,input);
}

bool EditControlWrapper::isVisible()
{
	return external::EditControl_IsVisible(actualInstance);
}
