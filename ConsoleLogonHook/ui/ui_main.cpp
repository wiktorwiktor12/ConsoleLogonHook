#define STB_IMAGE_IMPLEMENTATION
#define IM_VEC2_CLASS_EXTRA
#define IMGUI_DEFINE_MATH_OPERATORS
#include "ui_main.h"
#include "ui_sink.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "ui_helper.h"
#include "stb_image.h"
#include <util/util.h>

static ID3D11Device* d3dDevice = nullptr;
static ID3D11DeviceContext* d3dDeviceContext = nullptr;
static IDXGISwapChain* swapChain = nullptr;
static bool                     swapChainOccluded = false;
static UINT                     resizeWidth = 0, resizeHeight = 0;
static ID3D11RenderTargetView* mainRenderTargetView = nullptr;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
        return true;

    switch (uMsg)
    {
    case WM_DISPLAYCHANGE:
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        resizeWidth = (UINT)LOWORD(lParam); // Queue resize
        resizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    d3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &mainRenderTargetView);
    pBackBuffer->Release();
}

bool CreateDeviceD3D(HWND handle)
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = handle;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &swapChain, &d3dDevice, &featureLevel, &d3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &swapChain, &d3dDevice, &featureLevel, &d3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupRenderTarget()
{
    if (mainRenderTargetView) { mainRenderTargetView->Release(); mainRenderTargetView = nullptr; }
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (swapChain) { swapChain->Release(); swapChain = nullptr; }
    if (d3dDeviceContext) { d3dDeviceContext->Release(); d3dDeviceContext = nullptr; }
    if (d3dDevice) { d3dDevice->Release(); d3dDevice = nullptr; }
}

DWORD WINAPI ImGUIThread(LPVOID lparam)
{
    //create a new window

    WNDCLASSEX cls = { sizeof(cls), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Debug Menu", nullptr };

    RegisterClassExW(&cls);
    int w = GetSystemMetrics(SM_CXSCREEN);
    int h = GetSystemMetrics(SM_CYSCREEN);
    HWND windowHandle = CreateWindowW(cls.lpszClassName, L"Custom Logon Hook debug menu", WS_POPUP, 0, 0, w, h, NULL, NULL, cls.hInstance, NULL);
    gWindowHandle = windowHandle;

    if (!CreateDeviceD3D(windowHandle))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(cls.lpszClassName, cls.hInstance);
        return 1;
    }

    ::ShowWindow(windowHandle, SW_SHOW);
    ::UpdateWindow(windowHandle);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    
    //travis — Today at 20:05
    //pixel font mogs
    //io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/segoeui.ttf",16);
    //io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/MS Sans Serif.ttf",12);

    ImGui::StyleColorsClassic();
    ImGui::GetStyle().FrameBorderSize = 1;

    ImGui_ImplWin32_Init(windowHandle);
    ImGui_ImplDX11_Init(d3dDevice, d3dDeviceContext);

    MinimizeLogonConsole();

    const ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    bool done = false;
    while (!done)
    {
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        if (swapChainOccluded && swapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED)
        {
            ::Sleep(10);
            continue;
        }
        swapChainOccluded = false;

        // Handle window resize (we don't resize directly in the WM_SIZE handler)
        if (resizeWidth != 0 && resizeHeight != 0)
        {
            CleanupRenderTarget();
            swapChain->ResizeBuffers(0, resizeWidth, resizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            resizeWidth = resizeHeight = 0;
            CreateRenderTarget();
        }
        uiRenderer* renderer = uiRenderer::Get();

        if (renderer->activeWindow)
            renderer->activeWindow->Tick();
        for (int i = 0; i < renderer->inactiveWindows.size(); ++i)
        {
            auto& window = renderer->inactiveWindows[i];
            window->Tick();
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        renderer->Draw();

        // Rendering
        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        d3dDeviceContext->OMSetRenderTargets(1, &mainRenderTargetView, nullptr);
        d3dDeviceContext->ClearRenderTargetView(mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        // Present
        HRESULT hr = swapChain->Present(1, 0);   // Present with vsync
        //HRESULT hr = g_pSwapChain->Present(0, 0); // Present without vsync
        swapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);
    }

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(windowHandle);
    ::UnregisterClassW(cls.lpszClassName, cls.hInstance);

    return 0;
}

uiRenderer* uiRenderer::Get()
{
    static uiRenderer renderer{};
    return &renderer;
}

uiRenderer::uiRenderer()
{
    activeWindow = std::shared_ptr<uiWindow>(nullptr);
    logWindowInstance = std::shared_ptr<::logWindow>(new ::logWindow());
    backgroundWindowInstance = std::shared_ptr<::backgroundWindow>(new backgroundWindow());
}

void uiRenderer::SetupUI()
{
    CreateThread(0,0,ImGUIThread,0,0,0);
}

void uiRenderer::AddWindow(std::shared_ptr<uiWindow> newWindow, bool ShouldPushToFront)
{
    auto instance = uiRenderer::Get();
    if (ShouldPushToFront)
    {
        auto& oldActiveWindow = instance->activeWindow;
        instance->activeWindow = newWindow;

        instance->inactiveWindows.push_back(oldActiveWindow);
    }
    else
        instance->inactiveWindows.push_back(newWindow);
}

bool uiRenderer::LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height)
{
    // Load from disk into a raw RGBA buffer
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create texture
    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Width = image_width;
    desc.Height = image_height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;

    ID3D11Texture2D* pTexture = NULL;
    D3D11_SUBRESOURCE_DATA subResource;
    subResource.pSysMem = image_data;
    subResource.SysMemPitch = desc.Width * 4;
    subResource.SysMemSlicePitch = 0;
    d3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);

    // Create texture view
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = desc.MipLevels;
    srvDesc.Texture2D.MostDetailedMip = 0;
    d3dDevice->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
    pTexture->Release();

    *out_width = image_width;
    *out_height = image_height;
    stbi_image_free(image_data);

    return true;
}
bool uiRenderer::DoesWindowOfTypeIdExist(int typeId)
{
    return false;
}

void uiRenderer::Tick()
{

}

//#define ENABLELOG

void uiRenderer::Draw()
{
    if (backgroundWindowInstance)
        backgroundWindowInstance->Draw();

    if (activeWindow)
        activeWindow->Draw();

#ifdef ENABLELOG
    if (logWindowInstance)
        logWindowInstance->Draw();
#endif
}

void uiWindow::Begin()
{
}

void uiWindow::Tick()
{
}

void uiWindow::Draw()
{
}

void uiWindow::SetActive()
{
    auto instance = uiRenderer::Get();
    if (instance->activeWindow.get() == this)
        return; //already in front

    auto oldActiveWindow = instance->activeWindow;
    for (auto it = instance->inactiveWindows.begin(); it != instance->inactiveWindows.end(); ++it)
    {
        if ((*it).get() == this)
        {
            instance->activeWindow = (*it);
            instance->inactiveWindows.erase(it);
            break;
        }
    }
    if (oldActiveWindow)
        instance->inactiveWindows.push_back(oldActiveWindow);
}

void uiWindow::SetInactive()
{
    auto instance = uiRenderer::Get();
    if (instance->activeWindow.get() != this)
        return; //already not front

    std::shared_ptr<uiWindow> window = instance->activeWindow;
    instance->activeWindow = 0;

    instance->inactiveWindows.push_back(window);
}

void logWindow::Tick()
{

}

void logWindow::Draw()
{
    ImGui::Begin("Logs window",0, ImGuiWindowFlags_NoNavFocus);
    // Toolbar
    {
        if (ImGui::Button("CMD"))
        {
            system("start cmd");
        }
        ImGui::SameLine();
        if (ImGui::Button("Style Picker"))
        {
            stylePicker = !stylePicker;
        }

        if (ImGui::Button("Clear"))
        {
            log_global::logs.clear();
        }
        ImGui::SameLine();

        // forge horizontal layout mode
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        window->DC.LayoutType = ImGuiLayoutType_Horizontal;
        ImGui::Separator();
        window->DC.LayoutType = ImGuiLayoutType_Vertical;

        std::string logLevelString;
        if (showInfo)
            logLevelString += (logLevelString.empty() ? "Info" : ", info");
        if (showWarning)
            logLevelString += (logLevelString.empty() ? "Warnings" : ", warnings");
        if (showError)
            logLevelString += (logLevelString.empty() ? "Errors" : ", errors");
        if (showDebug)
            logLevelString += (logLevelString.empty() ? "Debug logs" : ", debug logs");
        if (logLevelString.empty())
            logLevelString = "None";

        // Log level selector
        if (ImGui::BeginCombo("Log levels", logLevelString.c_str()))
        {
            ImGui::Checkbox("Information", &showInfo);
            ImGui::Checkbox("Warnings", &showWarning);
            ImGui::Checkbox("Errors", &showError);
            ImGui::Checkbox("Debug logs", &showDebug);
            ImGui::EndCombo();
        }

        ImGui::Separator();
    }
    ImGui::BeginChild("log_container", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));
    {
        for (int i = 0; i < log_global::logs.size(); i++)
        {
            const auto& log = log_global::logs[i];
            if ((log.level == spdlog::level::info && !showInfo ||
                log.level == spdlog::level::warn && !showWarning ||
                log.level == spdlog::level::err && !showError ||
                log.level == spdlog::level::debug && !showDebug) == false)
            {
                ImGui::Selectable(log.msg.c_str(), i % 2 == 1);
            }
        }
        if (log_global::should_console_scroll_down)
        {
            ImGui::SetScrollHereY(1.f);
            log_global::should_console_scroll_down = false;
        }
    }
    ImGui::EndChild();

    ImGui::End();

    if (stylePicker)
        ImGui::ShowStyleSelector("editor");
}

void backgroundWindow::Draw()
{
    if (!texture && !failedToLoadTexture)
        failedToLoadTexture = !uiRenderer::LoadTextureFromFile("logonhookimage.jpg", &texture, &w, &h);

    ImGui::Begin("background image", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoScrollbar);
    ImGui::BringWindowToDisplayBack(ImGui::GetCurrentWindow());

    ImGui::SetWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::SetWindowPos(ImVec2(0, 0));

    if (texture)
    {
        auto imagesize = ImVec2(w,h);

        float scale = ImGui::GetWindowSize().y / h;
        if (scale > 1)
            imagesize *= scale;
        ImGui::SetCursorPos((ImGui::GetWindowSize() - imagesize) * 0.5f);

        ImGui::Image(texture, imagesize);
    }

    ImGui::End();
}
