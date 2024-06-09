#include "dui_messageview.h"
#include "spdlog/spdlog.h"
#include <winstring.h>
#include "util/util.h"
#include "ui_helper.h"
#include "util/interop.h"
#include "resources/resource.h"

std::vector<MessageOptionControlWrapper> controls;

void external::MessageView_SetActive()
{
    HideConsoleUI();
    duiManager::SetPageActive((DirectUI::UCString)MAKEINTRESOURCEW(IDUIF_MESSAGEVIEW), [](DirectUI::Element*) -> void {return; });
    //auto messageView = duiManager::Get()->GetWindowOfTypeId<uiMessageView>(3);
    //if (messageView)
    //    messageView->SetActive();
}

void external::MessageOptionControl_Create(void* actualInsance, int optionflag)
{
    MessageOptionControlWrapper wrapper;
    wrapper.actualInstance = actualInsance;
    wrapper.optionflag = optionflag;
    controls.push_back(wrapper);
}

void external::MessageView_SetMessage(const wchar_t* message)
{
    //auto messageView = duiManager::Get()->GetWindowOfTypeId<uiMessageView>(3);
    //if (messageView)
    //{
    //    messageView->message = message;
    //
    //}
}

void external::MessageOptionControl_Destroy(void* actualInstance)
{
    for (int i = 0; i < controls.size(); ++i)
    {
        auto& button = controls[i];
        if (button.actualInstance == actualInstance)
        {
            SPDLOG_INFO("FOUND AND DELETING MessageOptionControl");
            controls.erase(controls.begin() + i);
            break;
        }
    }
}

void MessageOptionControlWrapper::Press()
{
    _KEY_EVENT_RECORD keyrecord;
    keyrecord.bKeyDown = true;
    keyrecord.wVirtualKeyCode = VK_RETURN;
    int result;
    if (actualInstance)
    {
        SPDLOG_INFO("Actual instance {} isn't null, so we are calling MessageOptionControl_Press with enter on the control!", actualInstance);

        external::MessageOptionControl_Press(actualInstance, &keyrecord, &result);
    }
}

std::wstring MessageOptionControlWrapper::GetText()
{
    return external::MessageOptionControl_GetText(actualInstance);
}

DirectUI::IClassInfo* duiMessageView::Class = NULL;

duiMessageView::duiMessageView()
{
}

duiMessageView::~duiMessageView()
{
    
}

HRESULT duiMessageView::CreateInstance(DirectUI::Element* rootElement, unsigned long* debugVariable, DirectUI::Element** newElement)
{
    int hr = E_OUTOFMEMORY;

    // Using HeapAlloc instead of new() is required as DirectUI::Element::_DisplayNodeCallback calls HeapFree() with the element
    duiMessageView* instance = (duiMessageView*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(duiMessageView));

    if (instance != NULL)
    {
        new (instance) duiMessageView();
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

DirectUI::IClassInfo* duiMessageView::GetClassInfoW()
{
    return duiMessageView::Class;
}

void duiMessageView::OnEvent(DirectUI::Event* iev)
{
    if (iev->flag != DirectUI::GMF_BUBBLED)
        return;
    if (!iev->handled)
        DirectUI::Element::OnEvent(iev);
}

void duiMessageView::OnDestroy()
{
    DirectUI::Element::OnDestroy();

}

void duiMessageView::Begin()
{

}