#include "camelotpad/CamelotPadApp.hpp"

#include <Windows.h>

namespace {
camelotpad::CamelotPadApp gApp;
} // namespace

extern "C" __declspec(dllexport) void CamelotPad_OnFrame() {
    gApp.Tick();
}

extern "C" __declspec(dllexport) void CamelotPad_OnDx9EndScene(void* device) {
    gApp.OnDx9EndScene(device);
}

BOOL APIENTRY DllMain(HMODULE moduleHandle, DWORD reasonForCall, LPVOID reserved) {
    (void)reserved;

    switch (reasonForCall) {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(moduleHandle);
            return gApp.Start() ? TRUE : FALSE;
        case DLL_PROCESS_DETACH:
            gApp.Stop();
            return TRUE;
        default:
            return TRUE;
    }
}
