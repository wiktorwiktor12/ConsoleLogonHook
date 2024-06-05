// dllmain.cpp : Defines the entry point for the DLL application.
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <hstring.h>
#include <pplwin.h>
#include "init/init.h"

HMODULE GetConsoleLogonDLL()
{
    static auto lib = LoadLibraryW(L"C:\\Windows\\System32\\ConsoleLogon.dll");
    if (!lib)
        MessageBox(0, L"FAILED TO LOAD", L"FAILED TO LOAD", 0);
    return lib;
}

extern "C" __declspec(dllexport) HRESULT __stdcall DllCanUnloadNow()
{
    return reinterpret_cast<HRESULT(*)()>(GetProcAddress(GetConsoleLogonDLL(), "DllCanUnloadNow"))();
}

extern "C" __declspec(dllexport) __int64 DllGetActivationFactory(HSTRING string, PVOID Ptr)
{
    return reinterpret_cast<__int64(*)(HSTRING string, PVOID Ptr)>(GetProcAddress(GetConsoleLogonDLL(), "DllGetActivationFactory"))(string, Ptr);
}

extern "C" __declspec(dllexport) HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return reinterpret_cast<HRESULT(*)(REFCLSID rclsid, REFIID riid, LPVOID * ppv)>(GetProcAddress(GetConsoleLogonDLL(), "DllGetClassObject"))(rclsid, riid, ppv);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        init::InitHooks();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:

        break;
    }
    return TRUE;
}

