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
#include "../ui/ui_sink.h"
#include "../ui/ui_securitycontrol.h"
#include "../util/util.h"
#include "../ui/ui_messageview.h"
#include "../ui/ui_statusview.h"
#include "ui/ui_selectedcredentialview.h"
#include "ui/ui_userselect.h"
#include "util\interop.h"

namespace init
{
    void InitSpdlog()
    {
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/CLH.log", true);

        file_sink->set_level(spdlog::level::debug);
        file_sink->set_pattern("[%H:%M:%S] [%! (%s:%#)] %^%l%$: %v");

        spdlog::logger logger("multi_sink", { file_sink });
        logger.set_level(spdlog::level::debug);

        register_logger(std::make_shared<spdlog::logger>(logger));
        set_default_logger(std::make_shared<spdlog::logger>(logger));
        spdlog::flush_every(std::chrono::microseconds(100));
    }

    __int64(__fastcall* EditControl__Repaint)(void* a1);
    __int64 EditControl__Repaint_Hook(void* a1)
    {
        if (!a1) return 0;

        if (IsBadReadPtr(a1, 8)) return 0;

        if (IsBadReadPtr(*(uintptr_t**)(__int64(a1) + 0x20), 8)) return 0;
        if (IsBadReadPtr(**(void***)(__int64(a1) + 0x20), 8)) return 0;

        return EditControl__Repaint(a1);
    }

    void(__stdcall* fOutputDebugStringW)(LPCWSTR lpoutputstring);
    void __stdcall OutputDebugStringW_Hook(LPCWSTR lpoutputstring)
    {
        std::wstring str = lpoutputstring;

        if (str[str.size() - 1] == L'\n')
            str.pop_back();

        SPDLOG_INFO(ws2s(str));
        fOutputDebugStringW(lpoutputstring);
    }

    void InitHooks()
    {
        InitSpdlog();

        auto baseaddress = (uintptr_t)LoadLibraryW(L"C:\\Windows\\System32\\ConsoleLogon.dll");
        if (!baseaddress)
            MessageBox(0, L"FAILED TO LOAD", L"FAILED TO LOAD", 0);
        //MessageBox(0,L"1",L"1",0);
        //check we are running correct consolelogon, very very low chance will this check pass if diff version dll
        auto SecurityOptionsView__RuntimeClassIntialise = (uint8_t*)(baseaddress + 0x36EB4);
        if (SecurityOptionsView__RuntimeClassIntialise[0] != 0x48 || SecurityOptionsView__RuntimeClassIntialise[1] != 0x89 || SecurityOptionsView__RuntimeClassIntialise[2] != 0x5C)
            return;
        //MessageBox(0,L"2",L"2",0);
        MinimizeLogonConsole();
        //MessageBox(0,L"3",L"3",0);

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

        fOutputDebugStringW = decltype(fOutputDebugStringW)(GetProcAddress(GetModuleHandle(L"api-ms-win-core-debug-l1-1-0.dll"), "OutputDebugStringW"));
        Hook(fOutputDebugStringW, OutputDebugStringW_Hook);
        EditControl__Repaint = (decltype(EditControl__Repaint))(baseaddress + 0x44528);
        //Hook(EditControl__Repaint, EditControl__Repaint_Hook);

        external::InitExternal();
        uiSecurityControl::InitHooks(baseaddress);
        uiMessageView::InitHooks(baseaddress);
        uiStatusView::InitHooks(baseaddress);
        uiUserSelect::InitHooks(baseaddress);
        uiSelectedCredentialView::InitHooks(baseaddress);

        MinimizeLogonConsole();

        external::InitUI();
        //MessageBox(0,L"4",L"4",0);
    }

    void Unload()
    {
        TerminateThread(uiUserSelectThreadHandle, 0);
        external::Unload();
    }
}
