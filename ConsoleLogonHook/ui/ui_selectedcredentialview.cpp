#include "ui_selectedcredentialview.h"
#include <Windows.h>
#include "detours.h"
#include "spdlog/spdlog.h"
#include "../util/util.h"
#include <winstring.h>
#include "ui_userselect.h"
#include <vector>
#include "ui_securitycontrol.h"
#include "util/interop.h"

//std::vector<EditControlWrapper> editControls;

__int64 (__fastcall* CredentialFieldControlBase__GetVisibility)(void* _this, bool* a2);
__int64(__fastcall* EditControl__v_HandleKeyInput)(__int64 _this, const struct _KEY_EVENT_RECORD* a2, int* a3);

__int64(__fastcall* SelectedCredentialView__v_OnKeyInput)(void* _this, const struct _KEY_EVENT_RECORD* a2, int* a3);
__int64 SelectedCredentialView__v_OnKeyInput_Hook(void* _this, const struct _KEY_EVENT_RECORD* a2, int* a3)
{
    if (a2->wVirtualKeyCode == VK_ESCAPE || a2->wVirtualKeyCode == VK_BACK)
    {
        //uiRenderer::Get()->GetWindowOfTypeId<uiUserSelect>(5)->wasInSelectedCredentialView = true;
		external::NotifyWasInSelectedCredentialView();
		globals::wasInSelectedCredentialView = true;
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

	//auto selectedCredentialView = uiRenderer::Get()->GetWindowOfTypeId<uiSelectedCredentialView>(6);
	//selectedCredentialView->SetInactive();
	//editControls.clear();

	auto res = SelectedCredentialView__RuntimeClassInitialize(a1,a2,a3,a4);

	//if (selectedCredentialView)
	//{
	//	SPDLOG_INFO("Setting active status selectedCredentialView");
	//	selectedCredentialView->accountNameToDisplay = ConvertHStringToString(a4);
	//	selectedCredentialView->texture = nullptr;
	//	selectedCredentialView->textureExists = true;
	//	selectedCredentialView->SetActive();
	//
	//	MinimizeLogonConsole();
	//}
	external::SelectedCredentialView_SetActive(ConvertHStringToString(a4).c_str());

	SPDLOG_INFO("SelectedCredentialView__RuntimeClassInitialize_Hook {} {} {} {}",a1,a2,a3,ws2s(ConvertHStringToString(a4)));

	return res;
}

__int64 (__fastcall* EditControl__RuntimeClassInitialize)(void* _this, void* a2, void* a3);
__int64 EditControl__RuntimeClassInitialize_Hook(void* _this, void* a2, void* a3)
{
	auto res = EditControl__RuntimeClassInitialize(_this,a2,a3);
	SPDLOG_INFO("EditControl__RuntimeClassInitialize_Hook {} {} {} ",_this,a2,a3);

	//EditControlWrapper wrapper;
	//wrapper.actualInstance = _this;
	//wrapper.fieldNameCache = ws2s(wrapper.GetFieldName());
	//wrapper.inputBuffer = ws2s(wrapper.GetInputtedText());
	//strcpy_s(wrapper.label, ws2s(wrapper.GetFieldName()).c_str());
	//strcpy_s(wrapper.fieldNameCache, ws2s(wrapper.GetFieldName()).c_str());
	external::EditControl_Create(_this);
	//SPDLOG_INFO("{}: {} is visible {}", ws2s(wrapper.GetFieldName()) , ws2s(wrapper.GetInputtedText()), (int)wrapper.isVisible());

	//for (int i = editControls.size() - 1; i >= 0; --i)
	//{
	//	auto& control = editControls[i];
	//	if (control.GetFieldName() == wrapper.GetFieldName())
	//	{
	//		editControls.erase(editControls.begin() + i);
	//		break;
	//	}
	//}
	//
	//editControls.push_back(wrapper);

	return res;
}

__int64 (__fastcall* CheckboxControl__Destructor)(void* _this, char a2); //Edit Control inherits this, and this will be called on it
__int64 CheckboxControl__Destructor_Hook(void* _this, char a2)
{
	//for (int i = 0; i < editControls.size(); ++i)
    //{
    //    auto& button = editControls[i];
    //    if (button.actualInstance == _this)
    //    {
    //        SPDLOG_INFO("FOUND AND DELETING edit control");
    //        editControls.erase(editControls.begin() + i);
    //        break;
    //    }
    //}
	external::EditControl_Destroy(_this);

	return CheckboxControl__Destructor(_this,a2);
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

const wchar_t* external::EditControl_GetFieldName(void* actualInstance)
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

	return ConvertHStringToRawString(v27);
}

const wchar_t* external::EditControl_GetInputtedText(void* actualInstance)
{
	HSTRING string{};
	uintptr_t v2 = *(uintptr_t*)(__int64(actualInstance) + 0x70);
	(*(__int64(__fastcall**)(__int64, HSTRING*))(*(__int64*)v2 + 0x30i64))(v2, &string);

	return ConvertHStringToRawString(string);
}

void external::EditControl_SetInputtedText(void* actualInstance, const wchar_t* input)
{
	//HSTRING string;
	//uintptr_t v2 = *(uintptr_t*)(__int64(actualInstance) + 0x70);
	//(*(__int64(__fastcall**)(__int64, HSTRING*))(*(__int64*)v2 + 0x30i64))(v2, &string);

	std::wstring winput = input;

	HSTRING newString;
	fWindowsCreateString(winput.c_str(), winput.size(), &newString);

	auto plus8 = (uintptr_t)(__int64(actualInstance) + 8);

	auto v16 = (*(__int64(__fastcall**)(uintptr_t, HSTRING))(**(uintptr_t**)(plus8 + 0x68) + 0x38i64))(
		*(uintptr_t*)(plus8 + 0x68),
		newString);
}

bool external::EditControl_isVisible(void* actualInstance)
{
	bool returnval = *(bool*)(__int64(actualInstance) + 0x78);
	return returnval;
}

//bool EditControlWrapper::isVisible()
//{
//	bool returnval = *(bool*)(__int64(actualInstance) + 0x78);
//	//CredentialFieldControlBase__GetVisibility(actualInstance,&returnval);
//	return returnval;
//}
