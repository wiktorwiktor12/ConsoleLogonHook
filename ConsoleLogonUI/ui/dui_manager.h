#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "d3d11.h"
#include <vector>
#include <memory>
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

class duiManager
{
public:

    HINSTANCE hInstance;
    bool bAppIsFocused;
    int version;
    DirectUI::DUIXmlParser* pParser;
    DirectUI::NativeHWNDHost* pWndHost;
    DirectUI::Element* pUIElement;
    DirectUI::HWNDElement* pWndElement;

    DirectUI::Element* pageContainerElement;

    static duiManager* Get();

    duiManager();

    static void InitDUI();
    static void UnloadDUI();
    static void SetPageActive(DWORD resourceId);
};