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

duiManager* duiManager::Get()
{
    static duiManager renderer{};
    return &renderer;
}

duiManager::duiManager()
{
    bAppIsFocused = false;
}

#define err(msg) MessageBoxW(NULL, L ## msg, L"CLHUI", MB_ICONERROR)

DWORD WINAPI DuiInitThread(LPVOID lparam)
{
    auto pDuiManager = duiManager::Get();
    HINSTANCE hInstance = GetModuleHandle(L"ConsoleLogonUI.dll");
    pDuiManager->hInstance = hInstance;
    DWORD dwDefer;

    CoInitializeEx(NULL, 0);

    // Try to initialise for the current version.
    // Since we can just depend on it failing during the prologue and not
    // doing any work, we can simply keep trying versions until we find one
    // that works.
    for (int i = 8; i < 100; i++)
        if (!DirectUI::InitProcessPriv(i, NULL, 0, true))
        {
            pDuiManager->version = i;
            break;
        }

    DirectUI::InitThread(2);
    DirectUI::RegisterAllControls();

    DirectUI::ClassInfo<duiBackgroundWindow, DirectUI::Element>::Register(hInstance);

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
                hr = DirectUI::HWNDElement::Create(
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

                        pDuiManager->pWndHost->Host(pDuiManager->pUIElement);
                        pDuiManager->pWndHost->ShowWindow(SW_SHOW);

                        pDuiManager->pageContainerElement = pDuiManager->pUIElement->FindDescendent(ATOMID(L"activePageContainer"));
                        if (!pDuiManager->pageContainerElement)
                            err("page container element not found");

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

void duiManager::SetPageActive(DWORD resourceId)
{
    auto pDuiManager = Get();
    if (!pDuiManager) return;
    if (!pDuiManager->pageContainerElement) return;

    pDuiManager->pageContainerElement->RemoveAll();

    HRESULT hr = pDuiManager->pParser->SetXMLFromResource(
        (DirectUI::UCString)MAKEINTRESOURCEW(resourceId),
        pDuiManager->hInstance,
        pDuiManager->hInstance
    );

    if (SUCCEEDED(hr))
    {
        DirectUI::Element* newElement = 0;
        hr = pDuiManager->pParser->CreateElement(
            (DirectUI::UCString)L"test",
            pDuiManager->pageContainerElement,
            NULL,
            NULL,
            &newElement
        );
        if (!newElement)
            err("newelement is null!");
    }
}


void duiBaseElement::Begin()
{

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
            duiManager::SetPageActive(IDUIF_TEST);
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
