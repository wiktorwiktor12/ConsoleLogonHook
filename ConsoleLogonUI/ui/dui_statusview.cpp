#include "dui_statusview.h"
#include "spdlog/spdlog.h"
#include "../util/util.h"
#include <winstring.h>
#include "util/interop.h"
#include "resources/resource.h"

const wchar_t* ptext;

int g_fHighDPIAware;
int g_iLPX;
int g_fHighDPI;
int g_iLPY;
void InitDPI(void)
{
    int v0 = 0; // eax
    HDC v1 = 0; // rax
    int v2 = 0; // edi
    HDC v3 = 0; // rbx
    int v4 = 0; // eax

    v0 = IsProcessDPIAware();
    if (g_iLPX == -1 || g_fHighDPIAware != v0)
    {
        g_fHighDPIAware = v0;
        v1 = GetDC(0i64);
        v2 = 0;
        v3 = v1;
        if (v1)
        {
            g_iLPX = GetDeviceCaps(v1, 88);
            v4 = GetDeviceCaps(v3, 90);
            v2 = g_iLPX != 96;
            g_fHighDPI = v2;
            g_iLPY = v4;
            ReleaseDC(0i64, v3);
        }
    }
}

void __fastcall SHLogicalToPhysicalDPI(int* a1, int* a2)
{
    InitDPI();
    if (a1)
        *a1 = MulDiv(*a1, g_iLPX, 96);
    if (a2)
        *a2 = MulDiv(*a2, g_iLPY, 96);
}

static int GetSpinnerResourceID(int* OutSize)
{
    int v18 = 20;
    SHLogicalToPhysicalDPI(&v18,0);
    int v15 = 0;
    
    int v14 = v18;
    if (v18 == 20)
    {
        v15 = IDB_SPINNER3;
    }
    else if (v18 == 25)
    {
        v15 = IDB_SPINNER4;
    }
    else
    {
        v15 = IDB_SPINNER5;
        if (v18 != 30)
            v14 = 30;
        v18 = v14;
    }
    if (OutSize)
        *OutSize = v14;

    return v15;
}

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


        auto animationStrip = (DirectUI::AnimationStrip*)pDuiManager->pUIElement->FindDescendent(ATOMID(L"WaitAnimation"));
        if (animationStrip)
        {
            int size = 0;
            int resid = GetSpinnerResourceID(&size);
            auto graphic = DirectUI::Value::CreateGraphic((DirectUI::UCString)MAKEINTRESOURCE(resid), 3, (UINT)-1, 0, 0, pDuiManager->hInstance,0,0);
            if (graphic)
            {
                animationStrip->SetValue(DirectUI::Element::ContentProp, 1, graphic);
                graphic->Release();

                animationStrip->SetFrameWidth(size);

                animationStrip->SetVisible(true);
                animationStrip->SetPlay(true);
            }
        }

        });
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
