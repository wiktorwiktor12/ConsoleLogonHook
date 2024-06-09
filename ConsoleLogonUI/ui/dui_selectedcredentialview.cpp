#include "dui_selectedcredentialview.h"
#include "spdlog/spdlog.h"
#include "../util/util.h"
#include <winstring.h>
#include "ui_helper.h"
#include "dui_userselect.h"
#include <vector>
#include "dui_securitycontrol.h"
#include "util/interop.h"
#include "resources/resource.h"

std::vector<EditControlWrapper> editControls;

void external::NotifyWasInSelectedCredentialView()
{
	//duiManager::Get()->GetWindowOfTypeId<uiUserSelect>(5)->wasInSelectedCredentialView = true;
}

void external::SelectedCredentialView_SetActive(const wchar_t* accountNameToDisplay, int flag)
{
	HideConsoleUI();
	duiManager::SetPageActive((DirectUI::UCString)MAKEINTRESOURCEW(IDUIF_SELECTEDCREDENTIALVIEW), [](DirectUI::Element*) -> void {return; });
	//auto selectedCredentialView = duiManager::Get()->GetWindowOfTypeId<uiSelectedCredentialView>(6);
	//if (selectedCredentialView)
	//{
	//	SPDLOG_INFO("Setting active status selectedCredentialView");
	//	selectedCredentialView->accountNameToDisplay = accountNameToDisplay;
	//	selectedCredentialView->texture = nullptr;
	//	selectedCredentialView->textureExists = true;
	//	selectedCredentialView->flag = flag;
	//	selectedCredentialView->SetActive();
	//}
}

void external::EditControl_Create(void* actualInstance)
{
	EditControlWrapper wrapper;
	wrapper.actualInstance = actualInstance;
	wrapper.fieldNameCache = ws2s(wrapper.GetFieldName());
	//MessageBox(0, wrapper.GetFieldName().c_str(), wrapper.GetFieldName().c_str(),0);
	wrapper.inputBuffer = ws2s(wrapper.GetInputtedText());
	//MessageBox(0, wrapper.GetInputtedText().c_str(), wrapper.GetInputtedText().c_str(),0);


	for (int i = editControls.size() - 1; i >= 0; --i)
	{
		auto& control = editControls[i];
		if (control.GetFieldName() == wrapper.GetFieldName())
		{
			editControls.erase(editControls.begin() + i);
			break;
		}
	}

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
void duiSelectedCredentialView::Begin()
{
	if (!hasSetupNotify)
	{
		auto uiRenderer = duiManager::Get();
		if (uiRenderer)
		{
			//auto securityControl = uiRenderer->GetWindowOfTypeId<uiSecurityControl>(2);
			//if (securityControl)
			//{
			//	securityControl->wasInSecurityControlNotifies.push_back([]() -> void { bWasInSecurityControl = true; });
			//	hasSetupNotify = true;
			//}
		}
	}
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
	return external::EditControl_isVisible(actualInstance);
}

DirectUI::IClassInfo* duiSelectedCredentialView::Class = NULL;

duiSelectedCredentialView::duiSelectedCredentialView()
{
}

duiSelectedCredentialView::~duiSelectedCredentialView()
{
}

HRESULT duiSelectedCredentialView::CreateInstance(DirectUI::Element* rootElement, unsigned long* debugVariable, DirectUI::Element** newElement)
{
	int hr = E_OUTOFMEMORY;

	// Using HeapAlloc instead of new() is required as DirectUI::Element::_DisplayNodeCallback calls HeapFree() with the element
	duiSelectedCredentialView* instance = (duiSelectedCredentialView*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(duiSelectedCredentialView));

	if (instance != NULL)
	{
		new (instance) duiSelectedCredentialView();
		hr = instance->Initialize(0, rootElement, debugVariable);
		if (SUCCEEDED(hr))
		{
			*newElement = instance;
		}
		else
		{
			if (instance != NULL)
			{
				instance->Destroy(TRUE);
				instance = NULL;
			}
		}
	}

	return hr;
}

DirectUI::IClassInfo* duiSelectedCredentialView::GetClassInfoW()
{
	return duiSelectedCredentialView::Class;
}

void duiSelectedCredentialView::OnEvent(DirectUI::Event* iev)
{
	if (iev->flag != DirectUI::GMF_BUBBLED)
		return;
	if (!iev->handled)
		DirectUI::Element::OnEvent(iev);

}

void duiSelectedCredentialView::OnDestroy()
{
	DirectUI::Element::OnDestroy();

}