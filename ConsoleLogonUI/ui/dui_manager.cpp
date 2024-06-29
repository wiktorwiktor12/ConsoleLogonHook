#include "dui_manager.h"
#include "ui_sink.h"
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
#include <util/util.h>
#include <tlhelp32.h>
#include <shellapi.h>
#include <dwmapi.h>

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
ATOM hotkeyid = 0;
DWORD WINAPI DuiInitThread(LPVOID lparam)
{
    auto pDuiManager = duiManager::Get();
    HINSTANCE hInstance = duiManager::Get()->hInstance;
    DWORD dwDefer;

    CoInitializeEx(NULL, 0); 

    DirectUI::InitProcessPriv(14, (unsigned short*)hInstance, 0, true);

    DirectUI::InitThread(2);
    DirectUI::RegisterAllControls();

    DirectUI::ClassInfo<duiSecurityControl, DirectUI::Element>::Register(hInstance);
    DirectUI::ClassInfo<duiBackgroundWindow, DirectUI::Element>::Register(hInstance);
    DirectUI::ClassInfo<duiMessageView, DirectUI::Element>::Register(hInstance);
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
            int w = GetSystemMetrics(SM_CXSCREEN);
            int h = GetSystemMetrics(SM_CYSCREEN);
            pDuiManager->pWndHost = NULL;
            hr = DirectUI::NativeHWNDHost::Create(
                (DirectUI::UCString)L"DUI LogonUI",
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

                        pDuiManager->pWndHost->Host(pDuiManager->pUIElement);
                        
                        pDuiManager->pWndHost->ShowWindow(SW_SHOW);

                        pDuiManager->pageContainerElement = pDuiManager->pUIElement->FindDescendent(ATOMID(L"activePageContainer"));
                        if (!pDuiManager->pageContainerElement)
                            err("page container element not found");

                        pDuiManager->LoadBackground();
                        pDuiManager->LoadBranding();

                        pDuiManager->IsReady = true;

                        hotkeyid = GlobalAddAtomW(L"ConsoleUIShowHotkey");
                        auto res = RegisterHotKey(pDuiManager->pWndHost->GetHWND(), hotkeyid, MOD_CONTROL | MOD_SHIFT | MOD_ALT, 0x43);
                        if (!res)
                            MessageBox(0,L"register hotkey failed",0,0);


                        //Have to do this instead of startmessagepump because for some reason WM_HOTKEY isn't captured in the wndproc messages for dui
                        BOOL result;
                        MSG Msg;

                        while (1)
                        {
                            result = GetMessageW(&Msg, 0i64, 0, 0);
                            if (!result)
                                break;

                            if (Msg.message == WM_HOTKEY)
                            {
                                if (Msg.wParam == hotkeyid)
                                {
                                    external::ShowConsoleUI();
                                }
                            }

                            TranslateMessage(&Msg);
                            DispatchMessageW(&Msg);
                        }
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

    UnregisterHotKey(pDuiManager->pWndHost->GetHWND(), hotkeyid);
    pDuiManager->pWndHost->DestroyWindow();

    DirectUI::UnInitThread();
    DirectUI::UnInitProcessPriv(0);
}

void duiManager::FrameResize(int a3, int a4, int a5, int cy)
{
    auto pDuiManager = duiManager::Get();
    if (a5 <= 0 || cy <= 0)
    {
        HWND hwnd = pDuiManager->pWndHost->GetHWND();
        int yScreen = GetSystemMetrics(1);
        int xScreen = GetSystemMetrics(0);
        SetWindowPos(hwnd, 0i64, 0, 0, xScreen, yScreen, 0x14u);
    }
    else
    {
        SetWindowPos(pDuiManager->pWndHost->GetHWND(), 0i64, a3, a4, a5, cy, 0x14u);
    }
    pDuiManager->LoadBackground();

    RECT rect;
    auto res = GetWindowRect(pDuiManager->pWndHost->GetHWND(),&rect);

    if (res)
    {
        pDuiManager->pUIElement->SetWidth(rect.right - rect.left);
        pDuiManager->pUIElement->SetHeight(rect.bottom - rect.top);
    }
}

void duiManager::SendWorkToUIThread(std::function<void(void*)> workfunction, void* params)
{
    SendMessageW(duiManager::Get()->pWndElement->GetHWND(), WM_USER+69, (WPARAM)&workfunction, (LPARAM)params);
}

void duiManager::LoadBackground()
{
    if (!this->pUIElement) return;

    auto backgroundElement = this->pUIElement->FindDescendent(ATOMID(L"Background"));
    if (!backgroundElement) return;

    if (UseOEMBackground())
    {
        WCHAR oemPath[] = L"C:\\Windows\\system32\\oobe\\info\\backgrounds\\backgroundDefault.jpg";

        if (!PathFileExistsW(oemPath)) return;

        HBITMAP bitmap = GetHBITMAPFromImageFile(oemPath);
        auto graphic = DirectUI::Value::CreateGraphic(bitmap, (unsigned char)2, (unsigned int)0xFFFFFFFF, (bool)0, 0, 0);
        if (!graphic) return;

        backgroundElement->SetValue(DirectUI::Element::BackgroundProp, 1, graphic);
        graphic->Release();
    }

    /*WCHAR path[] = L"C:\\Windows\\System32\\logonhookimage.jpg";
    if (!PathFileExistsW(path))
    {
        if (UseOEMBackground())
        {
            WCHAR oemPath[] = L"C:\\Windows\\system32\\oobe\\info\\backgrounds\\backgroundDefault.jpg";

            if (!PathFileExistsW(oemPath)) return;

            HBITMAP bitmap = GetHBITMAPFromImageFile(oemPath);
            auto graphic = DirectUI::Value::CreateGraphic(bitmap, (unsigned char)2, (unsigned int)0xFFFFFFFF, (bool)0, 0, 0);
            if (!graphic) return;

            backgroundElement->SetValue(DirectUI::Element::BackgroundProp, 1, graphic);
            graphic->Release();
        }


    }
    else
    {
        HBITMAP bitmap = GetHBITMAPFromImageFile(path);
        auto graphic = DirectUI::Value::CreateGraphic(bitmap, (unsigned char)2, (unsigned int)0xFFFFFFFF, (bool)0, 0, 0);
        if (!graphic) return;

        backgroundElement->SetValue(DirectUI::Element::BackgroundProp, 1, graphic);
        graphic->Release();
    }*/
}

void duiManager::LoadHighResImageForDPI()
{
    int DPI = GetDpiForSystem();
    if (DPI <= 96) // no change
        return;


}

void duiManager::LoadBranding()
{
    if (!this->pUIElement) return;

    auto brandingElement = this->pUIElement->FindDescendent(ATOMID(L"BrandingImage"));
    if (!brandingElement) return;

    const int brandingSizes[3][2] = {{122,350}, {1122,438} ,{2122,525}};

    int residToUse = 122;
    int lastdist = 9999999;
    int DPI = GetDpiForSystem();
    int scalecompare = MulDiv(350, DPI, 96);/* 350 * (DPI / 96);*/
    for (int i = 0; i < 3; ++i)
    {
        auto pair = brandingSizes[i];
        int resid = pair[0];
        int reso = pair[1];

        int dist = abs(reso - scalecompare);
        if (dist < lastdist)
        {
            lastdist = dist;
            residToUse = resid;
        }
    }
    //MessageBoxW(0,std::format(L"resid{} dist{} DPI {} scalecompare{}",residToUse,lastdist,DPI,scalecompare).c_str(),0,0);
    HBITMAP bitmap = external::BrandingLoadImage(L"Basebrd", residToUse, 0, 0, 0, 0);
    if (!bitmap)
    {
        bitmap = LoadBitmapW(duiManager::Get()->hInstance, MAKEINTRESOURCEW(residToUse));
    }

    auto graphic = DirectUI::Value::CreateGraphic(bitmap, (unsigned char)2, (unsigned int)0xFFFFFFFF, (bool)0, 0, 0);
    if (!graphic)
    {
        bitmap = LoadBitmapW(duiManager::Get()->hInstance, MAKEINTRESOURCEW(residToUse));
        graphic = DirectUI::Value::CreateGraphic(bitmap, (unsigned char)2, (unsigned int)0xFFFFFFFF, (bool)0, 0, 0);
    }
    if (!graphic) return;
    //MessageBoxW(0,L"",0,0);


    //if (!brandingElement->SetMinSize(0, 0)) return;

    brandingElement->SetValue(DirectUI::Element::ContentProp, 1, graphic);
    graphic->Release();
}

bool duiManager::UseOEMBackground()
{
    HKEY result;
    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows\\CurrentVersion\\Authentication\\LogonUI\\Background", 0, KEY_READ, &result) == S_OK)
    {
        DWORD size = sizeof(DWORD);
        DWORD type = REG_DWORD;

        DWORD data;
        if (RegQueryValueExW(result, L"OEMBackground", 0, &type, (LPBYTE)&data, &size) == S_OK)
        {
            return (bool)data;
        }
    }
    return 0;
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

            auto options = pDuiManager->pUIElement->FindDescendent(ATOMID(L"OptionsButton")); //UNHIDE power options, ease of access
            if (options)
            {
                options->SetVisible(true);
            }

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

    //hacky as hell, but the compiler kept fucking up when i tried to do this properly, so hacky method woohoo
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
        {
            if (!wParam || !lParam)
                break;
            auto& workFunction = *reinterpret_cast<std::function<void(void*)>*>(wParam);
            workFunction((void*)lParam);
            break;
        }
        case WM_SETTINGCHANGE:
        {
            int pvParam = 0;
            int v18 = 0;
            int v19 = 0;
            int v20 = 0;
            if (SystemParametersInfoW(SPI_GETWORKAREA, 0, &pvParam, 0))
                duiManager::FrameResize(pvParam, v18, v19 - pvParam, v20 - v18);
            break;
        }
        //case (WM_DISPLAYCHANGE):
        //{
        //    //int w = GetSystemMetrics(SM_CXSCREEN);
        //    //int h = GetSystemMetrics(SM_CYSCREEN);
        //    SetWindowLongPtr(hwnd, GWL_STYLE, WS_VISIBLE | WS_POPUP);
        //    SetWindowPos(hwnd, HWND_TOP, 0, 0, LOWORD(lParam), HIWORD(lParam), SWP_FRAMECHANGED);
        //    break;
        //}
        
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

void duiBackgroundWindow::OnInput(DirectUI::InputEvent* a2)
{

    DirectUI::Element::OnInput(a2);
}

HRESULT duiBackgroundWindow::CreateInstance(DirectUI::Element* rootElement, unsigned long* debugVariable, DirectUI::Element** newElement)
{
    int hr = E_OUTOFMEMORY;

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

//#define WITHCMD true
#define WITHCMD false

void StartUtilMan()
{
    ShellExecuteW(0, L"open", L"utilman.exe", L"-debug", 0, SW_SHOWNORMAL);
}

void duiBackgroundWindow::OnEvent(DirectUI::Event* iev)
{
    if (iev->target->GetID() == DirectUI::StrToID((DirectUI::UCString)L"buttonEaseOfAccess"))
    {
        if (iev->type == DirectUI::Button::Click)
        {
            if (WITHCMD)
            {
                if (!GetAsyncKeyState(VK_SHIFT))
                    StartUtilMan();
                else
                    system("start cmd.exe");
            }
            else
                StartUtilMan();
        }
    }

    DirectUI::Element::OnEvent(iev);
}

void duiBackgroundWindow::OnDestroy()
{
    DirectUI::Element::OnDestroy();

}

void duiBackgroundWindow::Begin()
{

}