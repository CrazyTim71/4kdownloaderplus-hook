#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#pragma comment(linker, "/DLL")
#include "MinHook.hpp"
#include "outputdebugstring.hpp"

#include <shellapi.h>
#include <stdlib.h>

namespace Original {
    decltype(&ShellExecuteW) ShellExecuteW { ShellExecuteW };
    decltype(&ShellExecuteA) ShellExecuteA { ShellExecuteA };
    decltype(&system) system { system };
}


namespace Detour {
    HINSTANCE WINAPI ShellExecuteW(
        HWND    hwnd,
        LPCWSTR lpOperation,
        LPCWSTR lpFile,
        LPCWSTR lpParameters,
        LPCWSTR lpDirectory,
        INT     nShowCmd
    ) {
        outputDebugString(L"URL_Blocker Plugin Hook: ShellExecuteW(%s, %s)", lpFile, lpParameters);
        return nullptr;
    }

    HINSTANCE WINAPI ShellExecuteA(
        HWND    hwnd,
        LPCSTR  lpOperation,
        LPCSTR  lpFile,
        LPCSTR  lpParameters,
        LPCSTR  lpDirectory,
        INT     nShowCmd
    ) {
        outputDebugString(L"URL_Blocker Plugin Hook: ShellExecuteA(%S, %S)", lpFile, lpParameters);
        return nullptr;
    }
}


void attach() {
    using namespace MinHookApi;
    MH_Initialize();
    MH_CreateHook(&ShellExecuteA, &Detour::ShellExecuteA, reinterpret_cast<void**>(&Original::ShellExecuteA));
    MH_EnableHook(&ShellExecuteA);
    MH_CreateHook(&ShellExecuteW, &Detour::ShellExecuteW, reinterpret_cast<void**>(&Original::ShellExecuteW));
    MH_EnableHook(&ShellExecuteW);
}


void detach() {
    using namespace MinHookApi;
    MH_DisableHook(&CreateFileW);
}


BOOL APIENTRY DllMain(HMODULE, DWORD ul_reason_for_call, LPVOID) {
    switch(ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        attach();
        break;

    case DLL_PROCESS_DETACH:
        detach();
        break;

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    default:
        break;
    }

    return TRUE;
}
