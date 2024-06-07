// dllmain.cpp : Defines the entry point for the DLL application.
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <ui/ui_main.h>
#include <ui/ui_securitycontrol.h>
#include <ui/ui_messageview.h>
#include <ui/ui_statusview.h>
#include <ui/ui_userselect.h>
#include <ui/ui_selectedcredentialview.h>
#include <util/interop.h>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <ui/ui_sink.h>

void InitSpdlog()
{
    //auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/CLH.log", true);
    auto ui_sink = std::make_shared<::ui_sink_mt>();

    //file_sink->set_level(spdlog::level::debug);
    //file_sink->set_pattern("[%H:%M:%S] [%! (%s:%#)] %^%l%$: %v");

    ui_sink->set_level(spdlog::level::debug);
    ui_sink->set_pattern("[%s:%#] %^%l%$: %v");

    spdlog::logger logger("multi_sink", { ui_sink });
    logger.set_level(spdlog::level::debug);

    register_logger(std::make_shared<spdlog::logger>(logger));
    set_default_logger(std::make_shared<spdlog::logger>(logger));
    spdlog::flush_every(std::chrono::microseconds(100));
}

extern "C" __declspec(dllexport) void InitUI()
{
    InitSpdlog();
    external::InitExternal();
    uiRenderer::SetupUI();

    uiSecurityControl* securityControl = new uiSecurityControl();
    uiRenderer::AddWindow(std::shared_ptr<uiWindow>(securityControl), false);

    uiMessageView* messageView = new uiMessageView();
    uiRenderer::AddWindow(std::shared_ptr<uiWindow>(messageView), false);

    uiStatusView* statusView = new uiStatusView();
    uiRenderer::AddWindow(std::shared_ptr<uiWindow>(statusView), false);

    uiUserSelect* userSelect = new uiUserSelect();
    uiRenderer::AddWindow(std::shared_ptr<uiWindow>(userSelect), false);

    uiSelectedCredentialView* selectedCredentialView = new uiSelectedCredentialView();
    uiRenderer::AddWindow(std::shared_ptr<uiWindow>(selectedCredentialView), false);

    securityControl->Begin();
    messageView->Begin();
    statusView->Begin();
    userSelect->Begin();
    selectedCredentialView->Begin();
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {

    case DLL_PROCESS_DETACH:
        uiRenderer::Get()->activeWindow.reset();
        uiRenderer::Get()->inactiveWindows.clear();
        uiRenderer::Get()->logWindowInstance.reset();
        uiRenderer::Get()->backgroundWindowInstance.reset();

        break;
    }
    return TRUE;
}

