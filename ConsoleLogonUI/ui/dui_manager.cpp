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

struct ImageData
{
    DWORD w;
    DWORD h;
    unsigned long long resIdConsumer;
    unsigned long long resIdServer;
    const wchar_t* OEMPath;
};

struct ratioItem
{
    float ratio;
    int numberOfImages;
    ImageData* RatioImageDatas;
};

ImageData imageDatas125[] = {{1280, 1024, 5031, 5044, L"%windir%\\system32\\oobe\\info\\backgrounds\\background1280x1024.jpg"}};
ImageData imageDatas1333333[] = 
{ 
    {1280, 960, 5032, 5045, L"%windir%\\system32\\oobe\\info\\backgrounds\\background1280x960.jpg"},
    {1024, 768, 5033, 5046, L"%windir%\\system32\\oobe\\info\\backgrounds\\background1024x768.jpg"},
    {1600, 1200, 5034, 5047, L"%windir%\\system32\\oobe\\info\\backgrounds\\background1600x1200.jpg"} 
};
ImageData imageDatas16[] = 
{ 
    {1440, 900, 5035, 5048, L"%windir%\\system32\\oobe\\info\\backgrounds\\background1440x900.jpg"},
    {1920, 1200, 5036, 5049, L"%windir%\\system32\\oobe\\info\\backgrounds\\background1920x1200.jpg"} 
};
ImageData imageData166666[] = { {1280, 768, 5037, 5050, L"%windir%\\system32\\oobe\\info\\backgrounds\\background1280x768.jpg"} };
ImageData imageData177[] = { {1360, 768, 5038, 5051, L"%windir%\\system32\\oobe\\info\\backgrounds\\background1360x768.jpg"} };
ImageData imageData08[] = { {1024, 1280, 5039, 5052, L"%windir%\\system32\\oobe\\info\\backgrounds\\background1024x1280.jpg"} };
ImageData imageData075[] = { {960, 1280, 5040, 5053, L"%windir%\\system32\\oobe\\info\\backgrounds\\background960x1280.jpg"} };
ImageData imageData0625[] = { {900, 1440, 5041, 5054, L"%windir%\\system32\\oobe\\info\\backgrounds\\background900x1440.jpg"} };
ImageData imageData06000[] = { {768, 1280, 5042, 5055, L"%windir%\\system32\\oobe\\info\\backgrounds\\background768x1280.jpg"} };
ImageData imageData0564[] = { {768, 1360, 5043, 5056, L"%windir%\\system32\\oobe\\info\\backgrounds\\background768x1360.jpg"} };

ratioItem ratioItems[10] = { {1.25, 1, imageDatas125 },
    {1.3333334,3,imageDatas1333333},
    {1.6, 2, imageDatas16},
    {1.6666666, 1,imageData166666},
    {1.7708334, 1, imageData177},
    {0.80000001, 1, imageData08},
    {0.75, 1, imageData075},
    {0.625, 1, imageData0625},
    {0.60000002, 1, imageData06000},
    {0.56470591, 1, imageData0564} };

bool __fastcall OEMBackgroundFileExists(ImageData* a1)
{
    bool v1; // bl
    WCHAR Dst[264]; // [rsp+20h] [rbp-228h] BYREF

    v1 = 0;
    if (ExpandEnvironmentStringsW(a1->OEMPath, Dst, 260) - 1 <= 259)
    {
        v1 = PathFileExistsW(Dst);
    }
    return v1;
}

bool BitmapAspectRatioEqualsScreen(HBITMAP h)
{
    bool v1 = 0; // di
    float w = 0; // xmm6_4
    float ratio = 0; // xmm6_4
    float v5 = 0; // xmm1_4
    float v6 = 0; // xmm0_4
    float v7 = 0; // xmm2_4
    
    BITMAP pv;
    if (GetObjectW(h, (int)sizeof(BITMAP), &pv))
    {
        w = (float)GetSystemMetrics(0);
        ratio = w / (float)GetSystemMetrics(1);
        v5 = (float)pv.bmWidth / (float)pv.bmHeight;
        if (ratio <= v5)
            v6 = (float)pv.bmWidth / (float)pv.bmHeight;
        else
            v6 = ratio;
        if (v5 <= ratio)
            v7 = (float)pv.bmWidth / (float)pv.bmHeight;
        else
            v7 = ratio;
        if (ratio <= v5)
            ratio = (float)pv.bmWidth / (float)pv.bmHeight;
        if ((float)((float)(v6 - v7) / ratio) < 0.01)
            v1 = 1;
    }
    return v1;
}
bool GetBitmapResolution(HBITMAP ha, int* w, int* h)
{
    bool v1 = 0; // di

    BITMAP pv;
    if (GetObjectW(ha, (int)sizeof(BITMAP), &pv))
    {
        if (w && h)
        {
            *w = pv.bmWidth;
            *h = pv.bmHeight;
            v1 = 1;
        }
    }
    return v1;
}

// this is messy as hell, but i do not care, this was mostly reverse engineered from windows 7 authui, and likely due to compiler optimizations, there were a lot of jmps/gotos
// if anyone wants to pull request and clean this up, feel free to do so!
static bool GetBackground(HBITMAP* OutBitmap)
{
    int MemoryFlag = 0;
    *OutBitmap = 0;

    int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
    float lastDistance = 1.0e7;

    int ScreenWidthCopy = ScreenWidth;
    int Height = ScreenHeight;
    int iterations = 10;
    int swapIt = 10;

    float ScreenRatio = (float)ScreenWidth / (float)ScreenHeight;
    float difference = 0;

    ImageData* dataToUse = 0;

    int i = 0;

    do
    {
        ratioItem item = ratioItems[i];
        difference = ScreenRatio - item.ratio;
        if (difference < 0.0)
            difference = difference * -1.0;
        if (lastDistance <= difference)
        {
            ScreenHeight = Height;
            ScreenWidth = (int)ScreenWidthCopy;
            i++;
            swapIt = --iterations;
            continue;
        }

        lastDistance = difference;
        dataToUse = 0;
        if (!duiManager::UseOEMBackground())
        {
            int imageDataNumIterations = 0;
            if (item.numberOfImages <= 0)
            {
                dataToUse = &item.RatioImageDatas[0];
            }
            else
            {
                bool bBroke = false;
                ImageData* ImageDataIt = &item.RatioImageDatas[0];
                while (ImageDataIt->w != ScreenWidth || ImageDataIt->h != ScreenHeight)
                {
                    ++imageDataNumIterations;
                    if (imageDataNumIterations >= item.numberOfImages)
                    {
                        bBroke = true;
                        break;
                    }

                    ImageDataIt = &item.RatioImageDatas[imageDataNumIterations];
                }
                if (!bBroke)
                    dataToUse = ImageDataIt;

                if (!dataToUse)
                    dataToUse = &item.RatioImageDatas[0];
            }

            ScreenHeight = Height;
            ScreenWidth = (int)ScreenWidthCopy;
            i++;
            swapIt = --iterations;
            
            continue;
        }
        int imageDataIterations = 0;
        if (item.numberOfImages <= 0)
        {
            ScreenWidth = (int)ScreenWidthCopy;
            dataToUse = &item.RatioImageDatas[0];
            ScreenHeight = Height;
            ScreenWidth = (int)ScreenWidthCopy;
            continue;
        }
        bool bBroke = false;
        ImageData* OEMImageDataIt = 0;
        while (1)
        {
            bool bNotEqual = false;
            OEMImageDataIt = &item.RatioImageDatas[imageDataIterations];
            if (OEMImageDataIt->w != ScreenWidthCopy || OEMImageDataIt->h != Height)
            {
                if (OEMBackgroundFileExists(OEMImageDataIt))
                    dataToUse = OEMImageDataIt;

                bNotEqual = true;
            }

            if (!bNotEqual && OEMBackgroundFileExists(OEMImageDataIt))
            {
                break;
            }
            ++imageDataIterations;
            if (imageDataIterations >= item.numberOfImages)
            {
                bBroke = true;
                break;
            }
        }
        if (!bBroke)
            dataToUse = OEMImageDataIt;

        iterations = swapIt;
        if (!dataToUse)
        {
            ScreenHeight = Height;
            ScreenWidth = (int)ScreenWidthCopy;

            int imageDataNumIterations = 0;
            if (item.numberOfImages <= 0)
            {
                dataToUse = &item.RatioImageDatas[0];
            }
            else
            {
                bBroke = false;
                ImageData* ImageDataIt = &item.RatioImageDatas[0];
                while (ImageDataIt->w != ScreenWidth || ImageDataIt->h != ScreenHeight)
                {
                    ++imageDataNumIterations;
                    if (imageDataNumIterations >= item.numberOfImages)
                    {
                        bBroke = true;
                        break;
                    }
                    ImageDataIt = &item.RatioImageDatas[imageDataNumIterations];
                }
                if (!bBroke)
                    dataToUse = ImageDataIt;
                if (!dataToUse)
                    dataToUse = &item.RatioImageDatas[0];
            }
            
        }
        ScreenHeight = Height;
        ScreenWidth = (int)ScreenWidthCopy;
        swapIt = --iterations;
        i++;
    } 
    while (iterations);

    WCHAR path[MAX_PATH] = L"C:\\Windows\\system32\\oobe\\info\\backgrounds\\backgroundDefault.jpg";

    if (duiManager::UseOEMBackground() && (PathFileExistsW(path) || OEMBackgroundFileExists(dataToUse)))
    {
        if (OEMBackgroundFileExists(dataToUse))
        {
            WCHAR Dst[264];

            if (ExpandEnvironmentStringsW(dataToUse->OEMPath, Dst, 260) - 1 <= 259)
            {
                HBITMAP bitmapDefault = GetHBITMAPFromImageFile(path);
                
                bool bDefaultBetter = false; //check if src fallback is better, just so u can have nice images if u have a src image

                int dw = 0; int dh = 0;
                if (GetBitmapResolution(bitmapDefault, &dw, &dh) && BitmapAspectRatioEqualsScreen(bitmapDefault))
                {
                    if ((dw > dataToUse->w || dh > dataToUse->h) && dw <= GetSystemMetrics(SM_CXSCREEN) && dh <= GetSystemMetrics(SM_CYSCREEN))
                        bDefaultBetter = true;
                }

                if (!bDefaultBetter)
                {
                    wcscpy_s(path, MAX_PATH, Dst);
                    DeleteObject(bitmapDefault);
                }
            }
        }

        HBITMAP bitmaplocal = GetHBITMAPFromImageFile(path);
        *OutBitmap = bitmaplocal;
    }
    else
    {
        unsigned long long resIdToUse = IsOS(OS_ANYSERVER) ? dataToUse->resIdServer : dataToUse->resIdConsumer;
        *OutBitmap = GetHBITMAPFromImageResource(resIdToUse);
    }

    if (*OutBitmap)
    {
        return BitmapAspectRatioEqualsScreen(*OutBitmap);
    }
    
    return 0;
}

void duiManager::LoadBackground()
{
    if (!this->pUIElement) return;

    auto InsideFrameElement = this->pUIElement->FindDescendent(ATOMID(L"InsideFrame"));
    if (!InsideFrameElement) return;

    
    HBITMAP bitmap;
    bool ratioMatches = GetBackground(&bitmap);
    auto graphic = DirectUI::Value::CreateGraphic(bitmap, (unsigned char)(4), (unsigned int)0xFFFFFFFF, (bool)0, 0, false);
    if (!graphic) return;

    InsideFrameElement->SetValue(DirectUI::Element::BackgroundProp, 1, graphic);
    graphic->Release();
    
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
        case WM_CLOSE:
        {
            MessageBox(0,L"close",0,0);
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

MIDL_INTERFACE("b1325ef5-dd4d-4988-a2b3-c776ad45d0d6")
CLegacyShutdownDialog : public IUnknown
{
public:
    virtual __int64 __fastcall Show(HWND a2, unsigned int a3, __int64* a4) = 0;
    virtual __int64 __fastcall ShowSETOnly(HWND a2, int a3, __int64* a4) = 0;
};

GUID CLSID_AuthUILegacyShutdownDialog{ 0x0B1325EF5,0x0DD4D,0x4988,0x0A2,0x0B3,0x0C7,0x76,0x0AD,0x45,0x0D0,0x0D6 };
GUID GUID_b1325ef5_dd4d_4988_a2b3_c776ad45d0d6{0x0EC530685 ,0x6C7B ,0x4D06 ,0x0A5,0x5B, 0x5A, 0x1A, 0x81, 0x78, 0x3E, 0x0F4 };

void duiBackgroundWindow::OnEvent(DirectUI::Event* iev)
{
    if (iev->target->GetID() == DirectUI::StrToID((DirectUI::UCString)L"Shutdown") && iev->type == DirectUI::Button::Click) //good enough
    {
        system("shutdown /s /t 0");

        //TODO: FIGURE OUT HOW TO GET THIS TO WORK PROPERLY AND NOT CRASH
        //CComPtr<CLegacyShutdownDialog> legacyPtr;
        //if (CoCreateInstance(CLSID_AuthUILegacyShutdownDialog,0,1u, GUID_b1325ef5_dd4d_4988_a2b3_c776ad45d0d6,(LPVOID*)&legacyPtr.p))
        //{
        //    int two = 2;
        //    legacyPtr->ShowSETOnly(duiManager::Get()->pWndHost->GetHWND(),1,(long long*)&two);
        //}
    }

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
