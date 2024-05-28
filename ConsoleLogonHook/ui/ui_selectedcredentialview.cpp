#include "ui_selectedcredentialview.h"
#include "detours.h"
#include "spdlog/spdlog.h"
#include "../util/util.h"
#include <winstring.h>
#include "ui_helper.h"
#include "ui_userselect.h"
#include <vector>
#include "ui_securitycontrol.h"

std::vector<EditControlWrapper> editControls;

__int64 (__fastcall* CredentialFieldControlBase__GetVisibility)(void* _this, bool* a2);
__int64(__fastcall* EditControl__v_HandleKeyInput)(__int64 _this, const struct _KEY_EVENT_RECORD* a2, int* a3);

__int64(__fastcall* SelectedCredentialView__v_OnKeyInput)(void* _this, const struct _KEY_EVENT_RECORD* a2, int* a3);
__int64 SelectedCredentialView__v_OnKeyInput_Hook(void* _this, const struct _KEY_EVENT_RECORD* a2, int* a3)
{
    if (a2->wVirtualKeyCode == VK_ESCAPE || a2->wVirtualKeyCode == VK_BACK)
    {
        uiRenderer::Get()->GetWindowOfTypeId<uiUserSelect>(5)->wasInSelectedCredentialView = true;
    }

    SPDLOG_INFO("SelectedCredentialView__v_OnKeyInput_Hook");

    return SelectedCredentialView__v_OnKeyInput(_this,a2,a3);
}

__int64 (__fastcall* CredUISelectedCredentialView__RuntimeClassInitialize)(void* _this, void* a2, void* a3, void* a4, HSTRING a5);
__int64 CredUISelectedCredentialView__RuntimeClassInitialize_Hook(void* _this, void* a2, void* a3, void* a4, HSTRING a5)
{
	SPDLOG_INFO("CredUISelectedCredentialView__RuntimeClassInitialize_Hook {} {} {} {} {}", (void*)_this ,a2,a3,a4, ws2s(ConvertHStringToString(a5)));

	auto res = CredUISelectedCredentialView__RuntimeClassInitialize(_this,a2,a3,a4,a5);


	return res;
}

__int64 (__fastcall* SelectedCredentialView__RuntimeClassInitialize)(void* a1, int a2, __int64 a3, HSTRING a4);
__int64 SelectedCredentialView__RuntimeClassInitialize_Hook(void* a1, int a2, __int64 a3, HSTRING a4)
{

	auto selectedCredentialView = uiRenderer::Get()->GetWindowOfTypeId<uiSelectedCredentialView>(6);
	//selectedCredentialView->SetInactive();
	//editControls.clear();

	auto res = SelectedCredentialView__RuntimeClassInitialize(a1,a2,a3,a4);

	if (selectedCredentialView)
	{
		SPDLOG_INFO("Setting active status selectedCredentialView");
		selectedCredentialView->accountNameToDisplay = ConvertHStringToString(a4);
		selectedCredentialView->SetActive();
		MinimizeLogonConsole();
	}

	SPDLOG_INFO("SelectedCredentialView__RuntimeClassInitialize_Hook {} {} {} {}",a1,a2,a3,ws2s(ConvertHStringToString(a4)));

	return res;
}

__int64 (__fastcall* EditControl__RuntimeClassInitialize)(void* _this, void* a2, void* a3);
__int64 EditControl__RuntimeClassInitialize_Hook(void* _this, void* a2, void* a3)
{
	auto res = EditControl__RuntimeClassInitialize(_this,a2,a3);
	SPDLOG_INFO("EditControl__RuntimeClassInitialize_Hook {} {} {} ",_this,a2,a3);

	EditControlWrapper wrapper;
	wrapper.actualInstance = _this;
	wrapper.fieldNameCache = ws2s(wrapper.GetFieldName());
	wrapper.inputBuffer = ws2s(wrapper.GetInputtedText());
	//strcpy_s(wrapper.label, ws2s(wrapper.GetFieldName()).c_str());
	//strcpy_s(wrapper.fieldNameCache, ws2s(wrapper.GetFieldName()).c_str());

	SPDLOG_INFO("{}: {} is visible {}", ws2s(wrapper.GetFieldName()) , ws2s(wrapper.GetInputtedText()), (int)wrapper.isVisible());

	editControls.push_back(wrapper);

	return res;
}

__int64 (__fastcall* CheckboxControl__Destructor)(void* _this, char a2); //Edit Control inherits this, and this will be called on it
__int64 CheckboxControl__Destructor_Hook(void* _this, char a2)
{
	for (int i = 0; i < editControls.size(); ++i)
    {
        auto& button = editControls[i];
        if (button.actualInstance == _this)
        {
            SPDLOG_INFO("FOUND AND DELETING edit control");
            editControls.erase(editControls.begin() + i);
            break;
        }
    }

	return CheckboxControl__Destructor(_this,a2);
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

			if (uiRenderer::Get()->activeWindow.get() == this) //only send an enter if we are active window
			{
				KEY_EVENT_RECORD rec;
				rec.wVirtualKeyCode = VK_RETURN; //forward it to consoleuiview
				globals::ConsoleUIView__HandleKeyInput((void*)(__int64(globals::ConsoleUIView) + 8), &rec);
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

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y * 0.45);

	TextCenteredOnLine(ws2s(accountNameToDisplay).c_str());

	int lastIndex = -1;
	for (int x = editControls.size() - 1; x >= 0; --x)
	{
		auto& control = editControls[x];
		if (control.isVisible())
		{
			lastIndex = x;
			break;
		}
	}

	for (int i = 0; i < editControls.size(); ++i)
	{
		auto& control = editControls[i];
		if (control.isVisible())
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
					globals::ConsoleUIView__HandleKeyInput((void*)(__int64(globals::ConsoleUIView) + 8),&rec);
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
		globals::ConsoleUIView__HandleKeyInput((void*)(__int64(globals::ConsoleUIView) + 8), &rec);
	}

	ImGui::End();
}

GUID guid;

void uiSelectedCredentialView::InitHooks(uintptr_t baseaddress)
{
	SelectedCredentialView__v_OnKeyInput = decltype(SelectedCredentialView__v_OnKeyInput)(baseaddress + 0x38720);
	CredUISelectedCredentialView__RuntimeClassInitialize = decltype(CredUISelectedCredentialView__RuntimeClassInitialize)(baseaddress + 0x338CC);
	SelectedCredentialView__RuntimeClassInitialize = decltype(SelectedCredentialView__RuntimeClassInitialize)(baseaddress + 0x37EE0);
	EditControl__RuntimeClassInitialize = decltype(EditControl__RuntimeClassInitialize)(baseaddress + 0x3E8F0);
	CheckboxControl__Destructor = decltype(CheckboxControl__Destructor)(baseaddress + 0x3C3AC);
	CredentialFieldControlBase__GetVisibility = decltype(CredentialFieldControlBase__GetVisibility)(baseaddress + 0x4162C);
	EditControl__v_HandleKeyInput = decltype(EditControl__v_HandleKeyInput)(baseaddress + 0x3EF20);

	uint8_t* focusPatch = (uint8_t*)(baseaddress + 0x38793); //to patch the check for the bottom most field being selected when pressing enter

	DWORD old;
	VirtualProtect(focusPatch,2,PAGE_EXECUTE_READWRITE,&old);
	memset(focusPatch,0x90,2);
	VirtualProtect(focusPatch,2,old,0);

	CLSIDFromString(L"{ddc7731f-aaf1-4bd4-b20a-d125a3bc23d8}", &guid);

	Hook(SelectedCredentialView__v_OnKeyInput, SelectedCredentialView__v_OnKeyInput_Hook);
	Hook(CredUISelectedCredentialView__RuntimeClassInitialize, CredUISelectedCredentialView__RuntimeClassInitialize_Hook);
	Hook(SelectedCredentialView__RuntimeClassInitialize, SelectedCredentialView__RuntimeClassInitialize_Hook);
	Hook(EditControl__RuntimeClassInitialize, EditControl__RuntimeClassInitialize_Hook);
	Hook(CheckboxControl__Destructor, CheckboxControl__Destructor_Hook);
}

std::wstring EditControlWrapper::GetFieldName()
{
	__int64 v28 = 0;
	(***(__int64(__fastcall****)(uintptr_t, GUID*, void*))(__int64(actualInstance) + 0x70))(
		*(uintptr_t*)(__int64(actualInstance) + 0x70),
		&guid,
		&v28);

	if (!v28)
	{
		SPDLOG_INFO("v28 is null, returning nothing");
		return L"";
	}

	HSTRING v27;
	(*(__int64(__fastcall**)(__int64, HSTRING*))(*(uintptr_t*)v28 + 48i64))(v28, &v27);

	std::wstring result;
	result = ConvertHStringToString(v27);
	return result;
}

std::wstring EditControlWrapper::GetInputtedText()
{
	HSTRING string;
	uintptr_t v2 = *(uintptr_t*)(__int64(actualInstance) + 0x70);
	(*(__int64(__fastcall**)(__int64, HSTRING*))(*(__int64*)v2 + 0x30i64))(v2, &string);

	return ConvertHStringToString(string);
}

void EditControlWrapper::SetInputtedText(std::wstring input)
{
	//HSTRING string;
	//uintptr_t v2 = *(uintptr_t*)(__int64(actualInstance) + 0x70);
	//(*(__int64(__fastcall**)(__int64, HSTRING*))(*(__int64*)v2 + 0x30i64))(v2, &string);

	HSTRING newString;
	fWindowsCreateString(input.c_str(), input.size(), &newString);

	auto plus8 = (uintptr_t)(__int64(actualInstance) + 8);
	SPDLOG_INFO("plus8");

	auto v16 = (*(__int64(__fastcall**)(uintptr_t, HSTRING))(**(uintptr_t**)(plus8 + 0x68) + 0x38i64))(
		*(uintptr_t*)(plus8 + 0x68),
		newString);
	SPDLOG_INFO("call");
	
}

bool EditControlWrapper::isVisible()
{
	bool returnval = *(bool*)(__int64(actualInstance) + 0x78);
	//CredentialFieldControlBase__GetVisibility(actualInstance,&returnval);
	return returnval;
}
