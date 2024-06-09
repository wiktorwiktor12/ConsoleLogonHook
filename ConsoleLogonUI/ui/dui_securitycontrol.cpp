#include "dui_securitycontrol.h"
#include "spdlog/spdlog.h"
#include "../util/util.h"
#include <winstring.h>
#include "ui_helper.h"
#include "util/interop.h"
#include "resources/resource.h"

std::vector<SecurityOptionControlWrapper> buttonsList;

void external::SecurityControlButtonsList_Clear()
{
    buttonsList.clear();
}

void external::SecurityControl_SetActive()
{
    HideConsoleUI();
    buttonsList.clear();

    if (!duiManager::Get()->IsReady)
        MessageBoxW(0,L"not ready",0,0);
    duiManager::SetPageActive((DirectUI::UCString)MAKEINTRESOURCEW(IDUIF_SECURITYCONTROL));
    //for (int i = 0; i < duiManager::Get()->inactiveWindows.size(); ++i) //theres prob a better and nicer way to do this
    //{
    //    auto& window = duiManager::Get()->inactiveWindows[i];
    //    if (window->windowTypeId == 2) //typeid for securityoptions
    //    {
    //        auto& notifies = reinterpret_cast<uiSecurityControl*>(window.get())->wasInSecurityControlNotifies;
    //        for (int x = 0; x < notifies.size(); ++x)
    //        {
    //            auto& notify = notifies[x];
    //            if (notify)
    //                notify();
    //        }
    //
    //        window->SetActive();
    //
    //        break;
    //    }
    //}
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
    //auto securityControl = duiManager::Get()->GetWindowOfTypeId<uiSecurityControl>(2);
    //if (securityControl)
    //{
    //    SPDLOG_INFO("setting inactive security control instance");
    //    securityControl->SetInactive();
    //}
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

DirectUI::IClassInfo* duiSecurityControl::Class = NULL;

duiSecurityControl::duiSecurityControl()
{
}

duiSecurityControl::~duiSecurityControl()
{
}

HRESULT duiSecurityControl::CreateInstance(DirectUI::Element* rootElement, unsigned long* debugVariable, DirectUI::Element** newElement)
{
    int hr = E_OUTOFMEMORY;

    // Using HeapAlloc instead of new() is required as DirectUI::Element::_DisplayNodeCallback calls HeapFree() with the element
    duiSecurityControl* instance = (duiSecurityControl*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(duiSecurityControl));

    if (instance != NULL)
    {
        new (instance) duiSecurityControl();
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

DirectUI::IClassInfo* duiSecurityControl::GetClassInfoW()
{
    return duiSecurityControl::Class;
}

void duiSecurityControl::OnEvent(DirectUI::Event* iev)
{
    if (iev->flag != DirectUI::GMF_BUBBLED)
        return;
    if (!iev->handled)
        DirectUI::Element::OnEvent(iev);

}

void duiSecurityControl::OnDestroy()
{
    DirectUI::Element::OnDestroy();

}

void duiSecurityControl::Begin()
{

}