# CamelotPad Install Guide

## Prerequisites
- Windows 10/11.
- Visual Studio 2022 Build Tools (Desktop C++).
- CMake 3.25+.
- vcpkg available in your environment.
- DAoC client compatible with your validated DAoC Hook target.

## Build Steps
1. Install dependencies:
   - `vcpkg install imgui sol2 lua detours:x86-windows`
2. Configure:
   - `cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=<vcpkg_root>/scripts/buildsystems/vcpkg.cmake`
3. Build:
   - `cmake --build build --config Release`

## Deploy
1. Copy the built `camelotpad_core` DLL to your DAoC Hook integration folder.
2. Ensure `camelotpad-lua/ui/main.lua` is available alongside deployment assets.
3. Ensure `docs/values_registry.md` entries for your client are validated.
4. Launch DAoC through your DAoC Hook injector path.

## Open Config In-Game
- Controller combo: `Back + Start`.
- Optional fallback: bind hotkey via hook injector config.
