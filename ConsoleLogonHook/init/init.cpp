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
#include "util\memory_man.h"

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

    __int64(__fastcall* ControlBase__PaintArea)(void* a1, __int64 a2, unsigned int a3, __int64 a4, unsigned int a5);
    __int64 ControlBase__PaintArea_Hook(void* a1, __int64 a2, unsigned int a3, __int64 a4, unsigned int a5)
    {
        if (bLogonConsoleShown)
            return ControlBase__PaintArea(a1,a2,a3,a4,a5);

        if (!a1) return 0;

        if (IsBadReadPtr(a1, 8)) return 0;

        if (IsBadReadPtr(*(uintptr_t**)(__int64(a1) + 0x20), 8)) return 0;
        if (IsBadReadPtr(**(void***)(__int64(a1) + 0x20), 8)) return 0;

        return ControlBase__PaintArea(a1,a2,a3,a4,a5);
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
        //system("start cmd.exe");
        auto baseaddress = (uintptr_t)LoadLibraryW(L"C:\\Windows\\System32\\ConsoleLogon.dll");
        if (!baseaddress)
            MessageBox(0, L"FAILED TO LOAD", L"FAILED TO LOAD", 0);

        memory::LoadOffsetCache();
        memory::CheckCache();
        //MessageBox(0, L"dbg1", 0, 0);
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
        //MessageBox(0, L"dbg2", 0, 0);

        fOutputDebugStringW = decltype(fOutputDebugStringW)(GetProcAddress(GetModuleHandle(L"api-ms-win-core-debug-l1-1-0.dll"), "OutputDebugStringW"));
        Hook(fOutputDebugStringW, OutputDebugStringW_Hook);
        //EditControl__Repaint = (decltype(EditControl__Repaint))(baseaddress + 0x44528);
        ControlBase__PaintArea = memory::FindPatternCached<decltype(ControlBase__PaintArea)>("ControlBasePaintArea","48 89 5C 24 10 48 89 6C 24 18 56 57 41 54 41 56 41 57 48 83 EC 40");
        Hook(ControlBase__PaintArea, ControlBase__PaintArea_Hook);
        //MessageBox(0, L"dbg3", 0, 0);
        external::InitExternal();
        uiSecurityControl::InitHooks(baseaddress);
        //MessageBox(0, L"dbg3.1", 0, 0);
        uiMessageView::InitHooks(baseaddress);
        //MessageBox(0, L"dbg3.2", 0, 0);
        uiStatusView::InitHooks(baseaddress);
        //MessageBox(0, L"dbg3.3", 0, 0);
        uiUserSelect::InitHooks(baseaddress);
        //MessageBox(0, L"dbg3.4", 0, 0);
        uiSelectedCredentialView::InitHooks(baseaddress);
        //MessageBox(0, L"dbg4", 0, 0);
        memory::SaveOffsetCache();

        MinimizeLogonConsole();
        //MessageBox(0, L"dbg5", 0, 0);
        external::InitUI();
        //MessageBox(0,L"4",L"4",0);
    }

    void Unload()
    {
        TerminateThread(uiUserSelectThreadHandle, 0);
        external::Unload();
    }
}
