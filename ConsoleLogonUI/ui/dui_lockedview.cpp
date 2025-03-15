#pragma once
#include "dui_lockedview.h"
#include "resources/resource.h"

void external::LockedView_SetActive()
{
	duiManager::SetPageActive((DirectUI::UCString)MAKEINTRESOURCEW(IDUIF_LOCKEDVIEW), [](DirectUI::Element *element) -> void {
		HideConsoleUI();
		auto pDuiManager = duiManager::Get();

		WCHAR szCtrlAltDel[256] = { 0 };
		LoadStringW(GetModuleHandleW(L"ConsoleLogon.dll"), 100, szCtrlAltDel, 256);

		auto text = pDuiManager->pUIElement->FindDescendent(ATOMID(L"LockedText"));
		if (text)
		{
			text->SetContentString((DirectUI::UCString)szCtrlAltDel);
		}
	});
}