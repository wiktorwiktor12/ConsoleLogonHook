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
    bool stylePicker = false;

    virtual void Tick() override;
    virtual void Draw() override;
};

class backgroundWindow;

class uiRenderer
{
public:

    std::shared_ptr<uiWindow> activeWindow;
    std::shared_ptr<logWindow> logWindowInstance;
    std::shared_ptr<backgroundWindow> backgroundWindowInstance;
    std::vector<std::shared_ptr<uiWindow>> inactiveWindows;

    static uiRenderer* Get();

    uiRenderer();

    static void SetupUI();
    static void AddWindow(std::shared_ptr<uiWindow> newWindow, bool ShouldPushToFront = true);
    //https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples#example-for-directx11-users
    static bool LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);

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

class backgroundWindow : public uiWindow
{
public:

    int h = 0;
    int w = 0;
    ID3D11ShaderResourceView* texture = NULL;
    bool failedToLoadTexture = false;

    backgroundWindow()
    {
        windowTypeId = 69420;
    }


    virtual void Draw() override;
};