#include "dui_statusview.h"
#include "spdlog/spdlog.h"
#include "../util/util.h"
#include <winstring.h>
#include "ui_helper.h"
#include "util/interop.h"
#include "resources/resource.h"

const wchar_t* ptext;
void external::StatusView_SetActive(const wchar_t* text)
{
    HideConsoleUI();
    ptext = text;
    duiManager::SetPageActive((DirectUI::UCString)MAKEINTRESOURCEW(IDUIF_STATUSVIEW), [](DirectUI::Element* element) -> void { 
        auto pDuiManager = duiManager::Get();
        auto str = element->FindDescendent(ATOMID(L"StatusText"));
        str->SetContentString((DirectUI::UCString)ptext);

        auto options = pDuiManager->pUIElement->FindDescendent(ATOMID(L"OptionsButton")); //HIDE power options, ease of access
        if (options)
        {
            options->SetVisible(false);
        }

        //TODO: APPLY DIFFERENT IMAGE PER DPi
        /*auto animationStrip = (DirectUI::AnimationStrip*)pDuiManager->pUIElement->FindDescendent(ATOMID(L"WaitAnimation"));
        if (animationStrip)
        {
            HBITMAP bitmap = (HBITMAP)LoadImageW(pDuiManager->hInstance, MAKEINTRESOURCEW(IDB_SPINNER3), IMAGE_BITMAP, 360, 20, LR_LOADTRANSPARENT);
            if (bitmap)
            {
                auto graphic = DirectUI::Value::CreateGraphic(bitmap, (unsigned char)2, (unsigned int)0xFFFFFFFF, (bool)0, 0, 0);
                if (graphic)
                {
                    {
                        animationStrip->SetValue(DirectUI::Element::ContentProp, 1, graphic);
                        graphic->Release();
                    }
                }
            }
            animationStrip->SetVisible(true);
            animationStrip->SetPlay(true);
        
        }*/

        });
    //ptext = 0;
    //auto statusText = duiManager::Get()->pWndElement->FindDescendent(ATOMID(L"StatusText"));
    //if (!statusText)
    //{
    //    MessageBoxW(0,L"failed to find statustext",0,0);
    //    return;
    //}
    
    

    //auto statusview = duiManager::Get()->GetWindowOfTypeId<uiStatusView>(4);
    //if (statusview)
    //{
    //    SPDLOG_INFO("Setting active status view instance");
    //    statusview->statusText = text;
    //    statusview->SetActive();
    //}
}

DirectUI::IClassInfo* duiStatusView::Class = NULL;

duiStatusView::duiStatusView()
{
}

duiStatusView::~duiStatusView()
{
}

HRESULT duiStatusView::CreateInstance(DirectUI::Element* rootElement, unsigned long* debugVariable, DirectUI::Element** newElement)
{
    int hr = E_OUTOFMEMORY;

    // Using HeapAlloc instead of new() is required as DirectUI::Element::_DisplayNodeCallback calls HeapFree() with the element
    duiStatusView* instance = (duiStatusView*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(duiStatusView));

    if (instance != NULL)
    {
        new (instance) duiStatusView();
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

DirectUI::IClassInfo* duiStatusView::GetClassInfoW()
{
    return duiStatusView::Class;
}

void duiStatusView::OnEvent(DirectUI::Event* iev)
{
    if (iev->flag != DirectUI::GMF_BUBBLED)
        return;
    if (!iev->handled)
        DirectUI::Element::OnEvent(iev);

}

void duiStatusView::OnDestroy()
{
    DirectUI::Element::OnDestroy();

}

void duiStatusView::Begin()
{

}
