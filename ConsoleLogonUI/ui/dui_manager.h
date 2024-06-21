#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "d3d11.h"
#include <vector>
#include <memory>
#include <functional>
#include "dui/dui_includes.h"

inline HWND gWindowHandle;

#define DEFINE_DUIELEMENTCLASS(className) static DirectUI::IClassInfo* Class; \
static HRESULT CreateInstance(DirectUI::Element* rootElement, unsigned long* debugVariable, DirectUI::Element** newElement); \
virtual DirectUI::IClassInfo* GetClassInfoW() override; \
virtual void OnEvent(DirectUI::Event* iev) override; \
virtual void OnDestroy() override; \
static inline DirectUI::IClassInfo* GetClassInfoPtr() { return Class; } \
static inline DirectUI::UCString DoGetClassName() { return (DirectUI::UCString)className; } \
virtual void Begin() override;

#define ATOMID(id) (DirectUI::StrToID((DirectUI::UCString)id))

class duiBaseElement : public DirectUI::Element //DO NOT INSTANTIATE
{
public:

    virtual void Begin();
};

class duiBackgroundWindow : public duiBaseElement
{
public:
    duiBackgroundWindow();
    virtual ~duiBackgroundWindow() override;

    DEFINE_DUIELEMENTCLASS(L"duiBackgroundWindow");
};

class duiWindowListener : public DirectUI::HWNDElement
{
public:
    //static DirectUI::IClassInfo* Class;
    //virtual DirectUI::IClassInfo* GetClassInfoW() override;

    __declspec(noinline) static HRESULT Create(HWND hwnd, bool a2, unsigned int a3, Element* rootElement, unsigned long* debugVariable, Element** pOut);
    LRESULT WndProcCustom(HWND, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //static inline DirectUI::IClassInfo* GetClassInfoPtr()
    //{
    //    return Class;
    //} 
    //static inline DirectUI::UCString DoGetClassName()
    //{
    //    return (DirectUI::UCString)L"duiWindowListener";
    //} 
};

class duiManager
{
public:

    HINSTANCE hInstance;
    bool IsReady;
    int version;
    DirectUI::DUIXmlParser* pParser;
    DirectUI::NativeHWNDHost* pWndHost;
    DirectUI::Element* pUIElement;
    duiWindowListener* pWndElement;

    DirectUI::Element* pageContainerElement;

    static duiManager* Get();

    duiManager();

    static void InitDUI();
    static void UnloadDUI();
    static void SetPageActive(DirectUI::UCString resource, std::function<void(DirectUI::Element*)> elementReadyCallback);
    static void FrameResize(int a3, int a4, int a5, int cy);

    static void SendWorkToUIThread(std::function<void(void* userParams)> workFunction, void* params);

    void LoadBranding();
    void LoadBackground();
    bool UseOEMBackground();
};