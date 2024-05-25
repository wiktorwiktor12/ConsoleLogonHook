#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "d3d11.h"
#include <vector>
#include <memory>

inline HWND gWindowHandle;

class uiWindow
{
public:
    int windowTypeId = -1;
    uiWindow()
    {

    }

    virtual void Tick();
    virtual void Draw(); //implemented by the classes

    void SetActive();
    void SetInactive();
};

class logWindow : public uiWindow
{
public:

    logWindow()
    {
        windowTypeId = 1;
    }

    bool showInfo = true;
    bool showWarning = true;
    bool showError = true;
    bool showDebug = false;

    virtual void Tick() override;
    virtual void Draw() override;
};

class uiRenderer
{
public:

    std::shared_ptr<uiWindow> activeWindow;
    std::shared_ptr<logWindow> logWindowInstance;
    std::vector<std::shared_ptr<uiWindow>> inactiveWindows;

    static uiRenderer* Get();

    uiRenderer()
    {
        activeWindow = std::shared_ptr<uiWindow>(nullptr);
        logWindowInstance = std::shared_ptr<::logWindow>(new ::logWindow());
    }

    static void SetupUI();
    static void AddWindow(std::shared_ptr<uiWindow> newWindow, bool ShouldPushToFront = true);

    bool DoesWindowOfTypeIdExist(int typeId);

    template<class T = uiWindow>
    T* GetWindowOfTypeId(int typeId)
    {
        if (activeWindow && activeWindow->windowTypeId == typeId)
            return (T*)activeWindow.get();

        if (logWindowInstance && logWindowInstance->windowTypeId == typeId)
            return (T*)logWindowInstance.get();

        for (int i = 0; i < inactiveWindows.size(); ++i)
        {
            auto& window = inactiveWindows[i];
            if (window->windowTypeId == typeId)
                return (T*)window.get();
        }
        return NULL;
    }

    void Tick();
    void Draw();
};