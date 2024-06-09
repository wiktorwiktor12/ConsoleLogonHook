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
    CBackgroundWindow   backgroundWindow(hInstance);
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

    DirectUI::ClassInfo<duiBackgroundWindow, DirectUI::Element>::Register(hInstance);
    DirectUI::ClassInfo<duiMessageView, DirectUI::Element>::Register(hInstance);
    DirectUI::ClassInfo<duiSecurityControl, DirectUI::Element>::Register(hInstance);
    DirectUI::ClassInfo<duiSelectedCredentialView, DirectUI::Element>::Register(hInstance);
    DirectUI::ClassInfo<duiStatusView, DirectUI::Element>::Register(hInstance);
    DirectUI::ClassInfo<duiUserSelect, DirectUI::Element>::Register(hInstance);

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
            backgroundHWND = backgroundWindow.Create();
            int w = GetSystemMetrics(SM_CXSCREEN);
            int h = GetSystemMetrics(SM_CYSCREEN);
            pDuiManager->pWndHost = NULL;
            hr = DirectUI::NativeHWNDHost::Create(
                (DirectUI::UCString)L"Fucking around.",
                backgroundHWND,
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

                        *(uint8_t*)(__int64(pDuiManager->pWndHost) + 0x18 ) = *(uint8_t*)(__int64(pDuiManager->pWndHost) + 0x18) & ~0x40;

                        pDuiManager->pWndHost->Host(pDuiManager->pUIElement);
                        
                        pDuiManager->pWndHost->ShowWindow(SW_SHOW);

                        pDuiManager->pageContainerElement = pDuiManager->pUIElement->FindDescendent(ATOMID(L"activePageContainer"));
                        if (!pDuiManager->pageContainerElement)
                            err("page container element not found");

                        pDuiManager->IsReady = true;

                        //CreateThread(0,0,DuiPageWorkerThread,0,0,0);
                        //while (true)
                        //{
                        //    Sleep(1);
                        //}
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


bool bDirty = false;
DirectUI::UCString nextString;

void duiManager::SetPageActive(DirectUI::UCString resource)
{
    //pendingPages.push_back(resource);
    nextString = resource;
    bDirty = true;
    SendMessageW(backgroundHWND,WM_KEYDOWN,0,0);
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

    //static bool bOnce = true;
    //if (bOnce)
    //{
    //    CreateThread(0, 0, DuiPageWorkerThread, 0, 0, 0);
    //    bOnce = false;
    //}

    if (iev->target->GetID() == DirectUI::StrToID((DirectUI::UCString)L"buttonShutdown"))
    {
        if (iev->type == DirectUI::Button::Click)
        {
            //MessageBox(0,L"buttonShutdown Button Pressed!",L"",0);
            duiManager::SetPageActive((DirectUI::UCString)MAKEINTRESOURCEW(IDUIF_TEST));
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
const TCHAR     CBackgroundWindow::s_szWindowClassName[] = TEXT("LogonUIBackgroundWindowClass");
CBackgroundWindow::CBackgroundWindow(HINSTANCE hInstance) :
    _hInstance(hInstance),
    _hwnd(NULL)

{
    WNDCLASSEX  wndClassEx;

    ZeroMemory(&wndClassEx, sizeof(wndClassEx));
    wndClassEx.cbSize = sizeof(wndClassEx);
    wndClassEx.lpfnWndProc = WndProc;
    wndClassEx.hInstance = hInstance;
    wndClassEx.lpszClassName = s_szWindowClassName;
    wndClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);
    _atom = RegisterClassEx(&wndClassEx);
}

CBackgroundWindow::~CBackgroundWindow(void)

{
    if (_hwnd != NULL)
    {
        (BOOL)DestroyWindow(_hwnd);
    }
    if (_atom != 0)
    {
        (UnregisterClass(MAKEINTRESOURCE(_atom), _hInstance));
    }
}

HWND    CBackgroundWindow::Create(void)

{
    HWND    hwnd;

#if     DEBUG

    hwnd = NULL;

#else

    hwnd = CreateWindowEx(0,
        s_szWindowClassName,
        NULL,
        WS_POPUP,
        GetSystemMetrics(SM_XVIRTUALSCREEN), GetSystemMetrics(SM_YVIRTUALSCREEN),
        GetSystemMetrics(SM_CXVIRTUALSCREEN), GetSystemMetrics(SM_CYVIRTUALSCREEN),
        NULL, NULL, _hInstance, this);
    if (hwnd != NULL)
    {
        (BOOL)ShowWindow(hwnd, SW_SHOW);
        BOOL(SetForegroundWindow(hwnd));
        (BOOL)EnableWindow(hwnd, FALSE);
    }

#endif

    return(hwnd);
}

LRESULT     CALLBACK    CBackgroundWindow::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)

{
    LRESULT             lResult;
    CBackgroundWindow* pThis;

    if (bDirty)
    {
        bDirty = false;
        auto pDuiManager = duiManager::Get();
        //if (!pDuiManager) return;
        //if (!pDuiManager->pageContainerElement) return;

        DWORD defer;
        pDuiManager->pWndElement->StartDefer(&defer);

        INITCOMMONCONTROLSEX iccex = { sizeof(INITCOMMONCONTROLSEX), 0x80000000 | ICC_STANDARD_CLASSES | ICC_TREEVIEW_CLASSES };
        InitCommonControlsEx(&iccex);

        pDuiManager->pageContainerElement->DestroyAll(true);
        //CoInitializeEx(NULL, 0);
        //DirectUI::InitProcessPriv(14, NULL, 0, true);
        DirectUI::InitThread(2);
        //DirectUI::DUIXmlParser* parser = 0;
        //DirectUI::DUIXmlParser::Create(&parser, 0, 0, 0, 0);
        //err("wait");
        HRESULT hr = pDuiManager->pParser->SetXMLFromResource(
            nextString,
            pDuiManager->hInstance,
            pDuiManager->hInstance
        );

        if (SUCCEEDED(hr))
        {
            DirectUI::Element* newElement = 0;
            hr = pDuiManager->pParser->CreateElement(
                (DirectUI::UCString)L"Main",
                pDuiManager->pageContainerElement,
                NULL,
                NULL,
                &newElement
            );
            if (!newElement)
                err("newelement is null!");
            else
            {
                //pDuiManager->pUIElement->SetVisible(true);


                //newElement->SetVisible(true);
                //newElement->EndDefer(dwDefer);


                //pDuiManager->pWndHost->Host(pDuiManager->pUIElement);
                //pDuiManager->pWndHost->ShowWindow(SW_SHOW);
                //pDuiManager->pWndHost->RestoreFocus();
                //pDuiManager->pWndHost->Host(pDuiManager->pWndElement);
                //newElement->SetActive(1);
                //newElement->EnsureVisible();
                //AllocConsole();
                //freopen_s();

                //std::ofstream file("C:\\duitreedump.txt");

                //DumpDuiTree(pDuiManager->pageContainerElement,1);

                //std::cout.rdbuf(oldCoutStreamBuf);


                //newElement->SetVisible(true);
            }
        }
        else
            err("setxmlfromresource failed");
        pDuiManager->pUIElement->EndDefer(defer);
    }

    pThis = reinterpret_cast<CBackgroundWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    switch (uMsg)
    {
    case WM_CREATE:
    {
        CREATESTRUCT* pCreateStruct;

        pCreateStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
        pThis = reinterpret_cast<CBackgroundWindow*>(pCreateStruct->lpCreateParams);
        (LONG_PTR)SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
        lResult = 0;
        break;
    }
    case WM_PAINT:
    {



        HDC             hdcPaint;
        PAINTSTRUCT     ps;

        hdcPaint = BeginPaint(hwnd, &ps);
        BOOL(FillRect(ps.hdc, &ps.rcPaint, reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH))));
        BOOL(EndPaint(hwnd, &ps));
        lResult = 0;
        break;
    }
    default:
        lResult = DefWindowProc(hwnd, uMsg, wParam, lParam);
        break;
    }
    return(lResult);
}

