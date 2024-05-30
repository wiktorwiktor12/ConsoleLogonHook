#include "init.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <Detours\detours.h>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "d3d11.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "../ui/ui_main.h"
#include "../ui/ui_sink.h"
#include "../ui/ui_securitycontrol.h"
#include "../util/util.h"
#include "../ui/ui_messageview.h"
#include "../ui/ui_statusview.h"
#include "ui/ui_selectedcredentialview.h"
#include "ui/ui_userselect.h"

namespace init
{
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

    void InitHooks()
    {
        InitSpdlog();

        auto baseaddress = (uintptr_t)LoadLibraryW(L"C:\\Windows\\System32\\ConsoleLogon.dll");
        if (!baseaddress)
            MessageBox(0, L"FAILED TO LOAD", L"FAILED TO LOAD", 0);

        //check we are running correct consolelogon, very very low chance will this check pass if diff version dll
        auto SecurityOptionsView__RuntimeClassIntialise = (uint8_t*)(baseaddress + 0x36EB4);
        if (SecurityOptionsView__RuntimeClassIntialise[0] != 0x48 || SecurityOptionsView__RuntimeClassIntialise[1] != 0x89 || SecurityOptionsView__RuntimeClassIntialise[2] != 0x5C)
            return;


        auto stringdll = LoadLibraryW(L"api-ms-win-core-winrt-string-l1-1-0.dll");
        if (!stringdll)
        {
            MessageBoxW(0, L"ur fucked", L"ur fucked", 0);
        }
        else
        {
            fWindowsGetStringRawBuffer = decltype(fWindowsGetStringRawBuffer)(GetProcAddress(stringdll, "WindowsGetStringRawBuffer"));
            fWindowsDeleteString = decltype(fWindowsDeleteString)(GetProcAddress(stringdll, "WindowsDeleteString"));
            fWindowsCreateString = decltype(fWindowsCreateString)(GetProcAddress(stringdll, "WindowsCreateString"));
        }

        uiSecurityControl::InitHooks(baseaddress);
        uiMessageView::InitHooks(baseaddress);
        uiStatusView::InitHooks(baseaddress);
        uiUserSelect::InitHooks(baseaddress);
        uiSelectedCredentialView::InitHooks(baseaddress);
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
}
