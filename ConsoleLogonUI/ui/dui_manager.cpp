#define STB_IMAGE_IMPLEMENTATION
#define IM_VEC2_CLASS_EXTRA
#define IMGUI_DEFINE_MATH_OPERATORS
#include "dui_manager.h"
#include "ui_sink.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "ui_helper.h"
#include "stb_image.h"
#include "resources/resource.h"
#include <util/interop.h>
#include <dui/templates.h>
#include <iostream>
#include <fstream>
#include "dui_messageview.h"
#include "dui_securitycontrol.h"
#include "dui_selectedcredentialview.h"
#include "dui_statusview.h"
#include "dui_userselect.h"

duiManager* duiManager::Get()
{
    static duiManager renderer{};
    return &renderer;
}

duiManager::duiManager()
{
    IsReady = false;
}

#define err(msg) MessageBoxW(NULL, L ## msg, L"CLHUI", MB_ICONERROR)

std::vector<DirectUI::UCString> pendingPages;
DWORD WINAPI DuiPageWorkerThread(LPVOID lparam)
{
    while (true)
    {
        MSG Msg;
        bool result = GetMessageW(&Msg, 0i64, 0, 0);
        if (!result)
            break;


        TranslateMessage(&Msg);
        DispatchMessageW(&Msg);
    }
    return 0;
}
HWND backgroundHWND;
DWORD WINAPI DuiInitThread(LPVOID lparam)
{
    auto pDuiManager = duiManager::Get();
    HINSTANCE hInstance = duiManager::Get()->hInstance;
    //hostwindow   backgroundWindow(hInstance);
    //pDuiManager->hInstance = hInstance;
    DWORD dwDefer;

    CoInitializeEx(NULL, 0); 

    // Try to initialise for the current version.
    // Since we can just depend on it failing during the prologue and not
    // doing any work, we can simply keep trying versions until we find one
    // that works.
    DirectUI::InitProcessPriv(14, (unsigned short*)hInstance, 0, true);

    DirectUI::InitThread(2);
    DirectUI::RegisterAllControls();

    DirectUI::ClassInfo<duiSecurityControl, DirectUI::Element>::Register(hInstance);
    DirectUI::ClassInfo<duiBackgroundWindow, DirectUI::Element>::Register(hInstance);
    DirectUI::ClassInfo<duiMessageView, DirectUI::Element>::Register(hInstance);
    DirectUI::ClassInfo<duiSelectedCredentialView, DirectUI::Element>::Register(hInstance);
    DirectUI::ClassInfo<duiStatusView, DirectUI::Element>::Register(hInstance);
    DirectUI::ClassInfo<duiUserSelect, DirectUI::Element>::Register(hInstance);
    //DirectUI::ClassInfoNoCreator<duiWindowListener, DirectUI::HWNDElement>::Register(hInstance);

    HRESULT hr;
    pDuiManager->pParser = NULL;
    hr = DirectUI::DUIXmlParser::Create(&pDuiManager->pParser, NULL, NULL, NULL, NULL);
    if (SUCCEEDED(hr))
    {
        hr = pDuiManager->pParser->SetXMLFromResource(
            (DirectUI::UCString)MAKEINTRESOURCEW(IDUIF_MAIN),
            hInstance,
            hInstance
        );
        if (SUCCEEDED(hr))
        {
            //backgroundHWND = backgroundWindow.Create();
            int w = GetSystemMetrics(SM_CXSCREEN);
            int h = GetSystemMetrics(SM_CYSCREEN);
            pDuiManager->pWndHost = NULL;
            hr = DirectUI::NativeHWNDHost::Create(
                (DirectUI::UCString)L"Fucking around.",
                NULL,
                NULL,
                0, 0,
                w, h,
                NULL,
                WS_POPUP,
                0,
                &pDuiManager->pWndHost
            );

            if (SUCCEEDED(hr))
            {
                pDuiManager->pWndElement = NULL;
                hr = duiWindowListener::Create(
                    pDuiManager->pWndHost->GetHWND(),
                    true,
                    0,
                    NULL,
                    &dwDefer,
                    (DirectUI::Element**)&pDuiManager->pWndElement
                );

                if (SUCCEEDED(hr))
                {
                    pDuiManager->pUIElement = NULL;
                    hr = pDuiManager->pParser->CreateElement(
                        (DirectUI::UCString)L"main",
                        pDuiManager->pWndElement,
                        NULL,
                        NULL,
                        &pDuiManager->pUIElement
                    );
                    if (SUCCEEDED(hr))
                    {
                        pDuiManager->pUIElement->SetVisible(true);
                        pDuiManager->pUIElement->EndDefer(dwDefer);

                        //*(uint8_t*)(__int64(pDuiManager->pWndHost) + 0x18 ) = *(uint8_t*)(__int64(pDuiManager->pWndHost) + 0x18) & ~0x40;

                        pDuiManager->pWndHost->Host(pDuiManager->pUIElement);
                        
                        pDuiManager->pWndHost->ShowWindow(SW_SHOW);

                        pDuiManager->pageContainerElement = pDuiManager->pUIElement->FindDescendent(ATOMID(L"activePageContainer"));
                        if (!pDuiManager->pageContainerElement)
                            err("page container element not found");

                        pDuiManager->IsReady = true;

                        DirectUI::StartMessagePump();
                    }
                    else
                    {
                        err("DirectUI::DuiXmlParser::CreateElement failed.");
                    }
                }
                else
                {
                    err("DirectUI::HWNDElement::Create failed.");
                }
            }
            else
            {
                err("DirectUI::NativeHWNDHost::Create failed.");
            }
        }
        else
        {
            err("DirectUI::DuiXMLParser::SetXMLFromResource failed.");
        }
    }
    else
    {
        err("DirectUI::DuiXMLParser::Create failed.");
    }

    return 0;
}

void duiManager::InitDUI()
{
    CreateThread(0,0, DuiInitThread,0,0,0);
    //DuiInitThread(0);
}

void duiManager::UnloadDUI()
{
    DirectUI::StopMessagePump();

    auto pDuiManager = Get();
    if (pDuiManager->pUIElement)
    {
        pDuiManager->pUIElement->DestroyAll(true);
    }
    if (pDuiManager->pWndElement)
    {
        pDuiManager->pWndElement->DestroyAll(true);

    }
    if (pDuiManager->pWndHost)
    {
        pDuiManager->pWndHost->DestroyWindow();
        pDuiManager->pWndHost->Destroy();
    }
    if (pDuiManager->pParser)
        pDuiManager->pParser->Destroy();

    pDuiManager->pParser = NULL;
    pDuiManager->pWndHost = NULL;
    pDuiManager->pUIElement = NULL;
    pDuiManager->pWndElement = NULL;
    pDuiManager->pageContainerElement = NULL;

    DirectUI::UnInitThread();
    DirectUI::UnInitProcessPriv(0);
}

void duiManager::SendWorkToUIThread(std::function<void(void*)> workfunction, void* params)
{
    SendMessageW(duiManager::Get()->pWndElement->GetHWND(), WM_USER+69, (WPARAM)&workfunction, (LPARAM)params);
}


bool bDirty = false;

void duiManager::SetPageActive(DirectUI::UCString resource, std::function<void(DirectUI::Element*)> elementReadyCallback)
{
    struct paramsstruct
    {
        DirectUI::UCString nextString;
        std::function<void(DirectUI::Element*)> nextCallback;
    } prms;
    prms.nextString = resource;
    prms.nextCallback = elementReadyCallback;
    std::function<void(void* params)> workFunction = [](void* params) -> void
        {
            auto prms = reinterpret_cast<paramsstruct*>(params);
            auto pDuiManager = duiManager::Get();

            DWORD defer;
            pDuiManager->pWndElement->StartDefer(&defer);

            pDuiManager->pageContainerElement->DestroyAll(true);

            //DirectUI::DUIXmlParser* parser = 0;
            //DirectUI::DUIXmlParser::Create(&parser, 0, 0, 0, 0);

            HRESULT hr = pDuiManager->pParser->SetXMLFromResource(
                prms->nextString,
                pDuiManager->hInstance,
                pDuiManager->hInstance
            );

            if (SUCCEEDED(hr))
            {
                DirectUI::Element* newElement = 0;
                hr = pDuiManager->pParser->CreateElement(
                    (DirectUI::UCString)L"Main",
                    NULL,
                    NULL,
                    NULL,
                    &newElement
                );
                if (!newElement)
                    err("newelement is null!");
                else
                {
                    (*(__int64(__fastcall**)(DirectUI::Element*, struct DirectUI::Element**, __int64))(*(uintptr_t*)pDuiManager->pageContainerElement + 128i64))(
                        pDuiManager->pageContainerElement,
                        (DirectUI::Element**)&newElement,
                        1i64);
                    prms->nextCallback(newElement);
                }
            }
            else
                err("setxmlfromresource failed");

            //parser->Destroy(); 

            pDuiManager->pUIElement->EndDefer(defer);
        };
    SendWorkToUIThread(workFunction,(void*)&prms);
}

void duiBaseElement::Begin()
{

}

static inline void* GetMemberFuncPtr(auto Func) { return reinterpret_cast<void*&>(Func); }
HRESULT duiWindowListener::Create(HWND hwnd, bool a2, unsigned int a3, DirectUI::Element* rootElement, unsigned long* debugVariable, DirectUI::Element** pOut)
{
    auto res = DirectUI::HWNDElement::Create(hwnd,a2,a3,rootElement,debugVariable,pOut);

    DWORD old;
    auto vtable = *(void***)(__int64(*pOut) + 0x0);
    VirtualProtect(vtable,0x1D0,PAGE_EXECUTE_READWRITE,&old);
    vtable[56] = GetMemberFuncPtr(&duiWindowListener::WndProcCustom);
    VirtualProtect(vtable,0x1D0,old,0);
    return res;
}

LRESULT duiWindowListener::WndProcCustom(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    auto res = DirectUI::HWNDElement::WndProc(hwnd,uMsg,wParam,lParam);
    
    switch (uMsg)
    {
    case (WM_USER + 69): //work function on uithread
        if (!wParam || !lParam)
            break;

        auto& workFunction = *reinterpret_cast<std::function<void(void*)>*>(wParam);
        workFunction((void*)lParam);
        break;
    }

    return res;
}

DirectUI::IClassInfo* duiBackgroundWindow::Class = NULL;

duiBackgroundWindow::duiBackgroundWindow()
{
}

duiBackgroundWindow::~duiBackgroundWindow()
{
}

HRESULT duiBackgroundWindow::CreateInstance(DirectUI::Element* rootElement, unsigned long* debugVariable, DirectUI::Element** newElement)
{
    int hr = E_OUTOFMEMORY;

    // Using HeapAlloc instead of new() is required as DirectUI::Element::_DisplayNodeCallback calls HeapFree() with the element
    duiBackgroundWindow* instance = (duiBackgroundWindow*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(duiBackgroundWindow));

    if (instance != NULL)
    {
        new (instance) duiBackgroundWindow();
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

DirectUI::IClassInfo* duiBackgroundWindow::GetClassInfoW()
{
    return duiBackgroundWindow::Class;
}

void duiBackgroundWindow::OnEvent(DirectUI::Event* iev)
{
    if (iev->flag != DirectUI::GMF_BUBBLED)
        return;
    if (!iev->handled)
        DirectUI::Element::OnEvent(iev);

    if (iev->target->GetID() == DirectUI::StrToID((DirectUI::UCString)L"buttonShutdown"))
    {
        if (iev->type == DirectUI::Button::Click)
        {
            //MessageBox(0,L"buttonShutdown Button Pressed!",L"",0);
            duiManager::SetPageActive((DirectUI::UCString)MAKEINTRESOURCEW(IDUIF_TEST), [](DirectUI::Element*) -> void { return; });
        }
    }
}

void duiBackgroundWindow::OnDestroy()
{
    DirectUI::Element::OnDestroy();

}

void duiBackgroundWindow::Begin()
{

}