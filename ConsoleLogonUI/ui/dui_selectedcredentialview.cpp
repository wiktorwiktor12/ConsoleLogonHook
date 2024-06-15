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

const wchar_t* gname = 0;
int gflag = 0;

bool bReady = false;
void external::SelectedCredentialView_SetActive(const wchar_t* accountNameToDisplay, int flag)
{
	HideConsoleUI();
	gname = accountNameToDisplay;
	gflag = flag;
	bReady = false;
	duiManager::SetPageActive((DirectUI::UCString)MAKEINTRESOURCEW(IDUIF_SELECTEDCREDENTIALVIEW), [](DirectUI::Element*) -> void 
		{
			auto pDuiManager = duiManager::Get();
			auto UserList = (DirectUI::Selector*)pDuiManager->pUIElement->FindDescendent(ATOMID(L"UserList"));
			auto InsideFrame = pDuiManager->pUIElement->FindDescendent(ATOMID(L"InsideFrame"));
			DirectUI::Button* btn = 0;
			HRESULT hr = pDuiManager->pParser->CreateElement(
				(DirectUI::UCString)L"UserTileTemplate",
				NULL,
				NULL,
				NULL,
				(DirectUI::Element**)&btn
			);

			if (!btn)
			{
				MessageBox(0, L"CreateElement failed", 0, 0);
				return;
			}
			std::wstring sid = L"";
			external::GetSIDFromName(gname, &sid);

			std::wstring nametoshow = L"";
			std::wstring nameforPfp = L"";

			if (sid.size() > 0)
			{
				nametoshow = gname;
				nameforPfp = gname;
			}
			else if (editControls.size() > 0 && editControls[0].actualInstance)
			{
				nameforPfp = editControls[0].GetInputtedText();
				external::GetSIDFromName(nameforPfp.c_str(), &sid);
				if (sid.size() <= 0)
					nameforPfp = L"";
			}
			auto userName = duiSelectedCredentialView::CreateStringField(btn, nametoshow, true, false);
			
			//TODO: FIND A WAY TO CHECK IF A USER IS LOCKED
			bool locked = false;

			if (locked)
				duiSelectedCredentialView::CreateStringField(btn, L"Locked", true, true); //TODO: LOCALISE THIS STRING

			auto pic = btn->FindDescendent(ATOMID(L"Picture"));
			if (pic && nameforPfp.size() > 0)
			{
				auto path = external::GetProfilePicturePathFromSID(sid,true);
				if (path.size() > 0)
				{
					auto pfp = GetHBITMAPFromImageFile(const_cast<WCHAR*>(path.c_str()));

					auto graphic = DirectUI::Value::CreateGraphic(pfp, (unsigned char)2, (unsigned int)0xFFFFFFFF, (bool)0, 0, 0);
					if (graphic)
					{
						pic->SetValue(DirectUI::Element::ContentProp, 1, graphic);
						graphic->Release();
					}
				}
			}

			btn->SetID((DirectUI::UCString)L"BigUserTile");

			DirectUIElementAdd(UserList, btn);

			btn->SetLayoutPos(-1);
			btn->SetVisible(true);
			btn->SetEnabled(true);
			bReady = true;

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
				bool bIsPasswordField = (i > 0 && i <= 3);
				auto field = duiSelectedCredentialView::CreateEditField(btn, control.GetFieldName(), control.isVisible(), (i == lastIndex), bIsPasswordField);


				auto inputtedText = control.GetInputtedText();
				if (inputtedText.size() > 0)
				{
					auto TouchEditInnnerElement = field->FindDescendent(ATOMID(L"TouchEditInnnerElement"));
					if (TouchEditInnnerElement)
					{
						if (bIsPasswordField)
						{
							TouchEditInnnerElement->SetEncodedContentString((DirectUI::UCString)inputtedText.c_str());
							TouchEditInnnerElement->SetAccState(0x20000000);
						}
						else
							TouchEditInnnerElement->SetContentString((DirectUI::UCString)inputtedText.c_str());

					}
				}

				if (field)
					field->SetID((DirectUI::UCString)std::format(L"editControl:{}", i).c_str());
			}

			bool isCancel = (gflag == 2);
			std::wstring text = isCancel ? L"Cancel" : L"Switch User"; //TODO: LOCALISE

			DirectUI::Button* optbtn = 0;
			hr = pDuiManager->pParser->CreateElement(
				(DirectUI::UCString)L"securitycancelid",
				NULL,
				NULL,
				NULL,
				(DirectUI::Element**)&optbtn
			);
			if (optbtn)
			{
				auto dialogbtnframe = pDuiManager->pUIElement->FindDescendent(ATOMID(L"DialogButtonFrame"));
				auto textElm = optbtn->FindDescendent(ATOMID(L"textElement"));
				if (textElm)
					textElm->SetContentString((DirectUI::UCString)text.c_str());

				optbtn->SetID((DirectUI::UCString)L"SWITCHUSERCANCELBUTTON");

				DirectUIElementAdd(dialogbtnframe,optbtn);
			}
			DumpDuiTree(pDuiManager->pUIElement,0);
			return;
		});
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
int it = 0;
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

	it++;
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
	it--;
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

DirectUI::Element* duiSelectedCredentialView::CreateStringField(DirectUI::Element* UserTile, std::wstring content, bool bVisible, bool bIsSmall)
{
	auto pDuiManager = duiManager::Get();
	if (!pDuiManager || !UserTile) return 0;

	auto SelectorFieldFrame = UserTile->FindDescendent(ATOMID(L"SelectorFieldFrame"));

	DirectUI::Element* container = 0;
	HRESULT hr = pDuiManager->pParser->CreateElement(
		(DirectUI::UCString)L"FieldContainerTemplate",
		NULL,
		NULL,
		NULL,
		&container
	);
	if (!container)
	{
		MessageBox(0,L"Failed to make fieldcontainer",0,0);
		return 0;
	}
	auto forceCenter = container->FindDescendent(ATOMID(L"ForceCenter"));
	auto fieldParent = container->FindDescendent(ATOMID(L"FieldParent"));
	
	const wchar_t* resid = bIsSmall ? L"smallTextFieldTemplate" : L"largeTextFieldTemplate";

	DirectUI::Element* textElement = 0;
	hr = pDuiManager->pParser->CreateElement(
		(DirectUI::UCString)resid,
		NULL,
		NULL,
		NULL,
		&textElement
	);
	if (!textElement)
	{
		MessageBox(0, L"Failed to make textElement", 0, 0);
		container->Release();
		return 0;
	}
	
	if (fieldParent)
	{
		textElement->SetContentString((DirectUI::UCString)content.c_str());
		DirectUIElementAdd(fieldParent,textElement);

		if (textElement->SetLayoutPos(bVisible ? -1 : -3))
			textElement->SetVisible(bVisible);
	}

	DirectUIElementAdd(SelectorFieldFrame, container);
	return textElement;
}

struct EditListener : public DirectUI::IElementListener 
{
	void OnListenerAttach(DirectUI::Element* elem) override 
	{
		
	}

	void OnListenerDetach(DirectUI::Element* elem) override 
	{
		delete this;
	}

	bool OnPropertyChanging(DirectUI::Element* elem, const DirectUI::PropertyInfo* prop, int unk, DirectUI::Value* v1, DirectUI::Value* v2) override 
	{
		return true;
	}

	void OnListenedPropertyChanged(DirectUI::Element* elem, const DirectUI::PropertyInfo* prop, int type, DirectUI::Value* v1, DirectUI::Value* v2) override 
	{
		if (elem && prop && wcscmp((const wchar_t*)prop->name, L"Content") == 0)
		{
			OutputDebugStringW(L"Content Property Changed!");
			DirectUI::Value* val = 0;
			std::wstring str = L"";

			bool bIsPassword = external::TouchEditBaseGetPasswordCharacter(elem) == 9679;

			if (bIsPassword)
			{
				WCHAR text[256];
				text[0] = 0;
				memset(&text[1], 0, 0x1FE);
				elem->GetEncodedContentString((unsigned short*)text,0x100);
				str = text;
			}
			else
			{
				const wchar_t* ptr = 0;
				ptr = (const wchar_t*)elem->GetContentString(&val);
				if (ptr)
					str = ptr;
			}
			
			auto parent = elem->GetParent();
			if (!parent) return;

			auto PromptText = parent->FindDescendent(ATOMID(L"PromptText"));
			if (!PromptText) return;

			if (str.size() > 0)
			{
				PromptText->SetVisible(false);
			}
			else
				PromptText->SetVisible(true);

			auto id = parent->GetID();
			WCHAR atomName[256];
			GetAtomNameW(id, atomName, 256);

			std::wstring sAtomName = atomName;
			if (sAtomName.find(L"editControl:") != std::wstring::npos)
			{
				auto splits = split(sAtomName, L":");
				if (splits.size() >= 2)
				{
					auto id = splits[1];
					int index = std::stoi(id);
					auto& editControl = editControls[std::clamp<int>(index,0,editControls.size() - 1)];
					if (editControl.actualInstance)
						editControl.SetInputtedText(str);
				}
			}

			if (val)
				val->Release();
		}
	}

	void OnListenedEvent(DirectUI::Element* elem, struct DirectUI::Event* ev) override
	{
		
	}

	void OnListenedInput(DirectUI::Element* elem, struct DirectUI::InputEvent* iev) override
	{
		
	}
};

DirectUI::Element* duiSelectedCredentialView::CreateEditField(DirectUI::Element* UserTile, std::wstring content, bool bVisible, bool bShowSubmit, bool bIsPassword)
{
	auto pDuiManager = duiManager::Get();
	if (!pDuiManager || !UserTile) return 0;

	auto SelectorFieldFrame = UserTile->FindDescendent(ATOMID(L"SelectorFieldFrame"));

	DirectUI::Element* container = 0;
	HRESULT hr = pDuiManager->pParser->CreateElement(
		(DirectUI::UCString)L"FieldContainerTemplate",
		NULL,
		NULL,
		NULL,
		&container
	);
	if (!container)
	{
		MessageBox(0, L"Failed to make fieldcontainer", 0, 0);
		return 0;
	}
	auto forceCenter = container->FindDescendent(ATOMID(L"ForceCenter"));
	auto fieldParent = container->FindDescendent(ATOMID(L"FieldParent"));
	
	const wchar_t* resid = bIsPassword ? L"EditFieldPasswordTemplate" : L"EditFieldTemplate";

	DirectUI::Element* textElement = 0;
	hr = pDuiManager->pParser->CreateElement(
		(DirectUI::UCString)resid,
		NULL,
		NULL,
		NULL,
		(DirectUI::Element**)&textElement
	);
	if (!textElement)
	{
		MessageBox(0, L"Failed to make textElement", 0, 0);
		container->Release();
		return 0;
	}

	if (fieldParent)
	{
		auto TouchEditInnnerElement = textElement->FindDescendent(ATOMID(L"TouchEditInnnerElement"));
		if (TouchEditInnnerElement)
		{
			EditListener* listener = new EditListener();
			TouchEditInnnerElement->AddListener(listener);
		}
		auto promptText = textElement->FindDescendent(ATOMID(L"PromptText"));
		if (promptText)
			promptText->SetContentString((DirectUI::UCString)content.c_str());

		textElement->SetStdCursor(1);
		textElement->SetAbsorbsShortcut(1);

		DirectUIElementAdd(fieldParent, textElement);

		if (textElement->SetLayoutPos(bVisible ? -1 : -3))
			textElement->SetVisible(bVisible);
	}

	if (bShowSubmit)
	{
		auto submitElement = container->FindDescendent(ATOMID(L"Submit"));
		if (forceCenter)
		{
			forceCenter->SetLayoutPos(-1);
		}
		if (submitElement)
		{
			submitElement->SetLayoutPos(-1);
			submitElement->SetVisible(1);
			submitElement->SetEnabled(1);
		}
	}

	DirectUIElementAdd(SelectorFieldFrame, container);
	return textElement;
}

DirectUI::Element* duiSelectedCredentialView::CreateCommandLinkField(DirectUI::Element* UserTile, std::wstring content, bool bVisible)
{
	auto pDuiManager = duiManager::Get();
	if (!pDuiManager || !UserTile) return 0;

	auto SelectorFieldFrame = UserTile->FindDescendent(ATOMID(L"SelectorFieldFrame"));

	DirectUI::Element* container = 0;
	HRESULT hr = pDuiManager->pParser->CreateElement(
		(DirectUI::UCString)L"FieldContainerTemplate",
		NULL,
		NULL,
		NULL,
		&container
	);
	if (!container)
	{
		MessageBox(0, L"Failed to make fieldcontainer", 0, 0);
		return 0;
	}
	auto forceCenter = container->FindDescendent(ATOMID(L"ForceCenter"));
	auto fieldParent = container->FindDescendent(ATOMID(L"FieldParent"));

	DirectUI::Element* textElement = 0;
	hr = pDuiManager->pParser->CreateElement(
		(DirectUI::UCString)L"commandLinkFieldTemplate",
		NULL,
		NULL,
		NULL,
		&textElement
	);
	if (!textElement)
	{
		MessageBox(0, L"Failed to make textElement", 0, 0);
		container->Release();
		return 0;
	}

	if (fieldParent)
	{
		textElement->SetContentString((DirectUI::UCString)content.c_str());
		DirectUIElementAdd(fieldParent, textElement);

		if (textElement->SetLayoutPos(bVisible ? -1 : -3))
			textElement->SetVisible(bVisible);
	}

	DirectUIElementAdd(SelectorFieldFrame, container);
	return textElement;
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

	if (iev->type == DirectUI::Button::Click)
	{
		if (iev->target->GetID() == ATOMID(L"Submit"))
		{
			KEY_EVENT_RECORD rec;
			rec.wVirtualKeyCode = VK_RETURN; //forward it to consoleuiview
			external::ConsoleUIView__HandleKeyInputExternal(external::GetConsoleUIView(), &rec);
		}
		else if (iev->target->GetID() == ATOMID(L"SWITCHUSERCANCELBUTTON"))
		{
			KEY_EVENT_RECORD rec;
			rec.wVirtualKeyCode = VK_ESCAPE; //forward it to consoleuiview
			external::ConsoleUIView__HandleKeyInputExternal(external::GetConsoleUIView(), &rec);
		}
	}
}


void duiSelectedCredentialView::OnInput(DirectUI::InputEvent* a2)
{
	if (a2->target && a2->device == DirectUI::GINPUT_KEYBOARD)
	{
		if (a2->target->GetID() == ATOMID(L"TouchEditInnnerElement") && GetAsyncKeyState(VK_RETURN))
		{
			KEY_EVENT_RECORD rec;
			rec.wVirtualKeyCode = VK_RETURN; //forward it to consoleuiview
			external::ConsoleUIView__HandleKeyInputExternal(external::GetConsoleUIView(), &rec);
		}
	}

	DirectUI::Element::OnInput(a2);
}

void duiSelectedCredentialView::OnDestroy()
{
	DirectUI::Element::OnDestroy();

}