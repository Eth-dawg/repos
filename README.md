# CamelotPad

CamelotPad is a standalone Xbox controller add-on for Dark Age of Camelot (DAoC) with a modern overlay and fully configurable controller-first UX.

## Current Build State

- Greenfield C++20 CMake project scaffold.
- Injected DLL skeleton with runtime tick pipeline and DX9 hook callbacks (`CamelotPad_OnFrame`, `CamelotPad_OnDx9EndScene`).
- DAoC Hook interface layer (`IDaocMemoryBridge`, `IDaocCommandBridge`) for clean integration with v1.1.0.0 hook surfaces.
- XInput polling with radial deadzone normalization and configurable power curves (based on Microsoft XInput guidance).
- DAoC values registry adapter that validates `docs/values_registry.md` and enforces production-readiness checks.
- Lua UI entrypoint, JSON profile schema baseline, and DX9 Lua+ImGui bridge callback path.
- Action routing for qbar defaults + LT/LB modifier bars.
- Profile manager for JSON export/import baseline and default profile bootstrap.
- Release docs covering install, troubleshooting, and compatibility testing.

## Project Layout

- `camelotpad-core/` - C++ injected core (hooking/input/action runtime).
- `camelotpad-lua/` - Lua UI/config script layer.
- `camelotpad-shared/` - shared schemas and data contracts.
- `docs/` - technical and operational documentation.

## Build

```powershell
cmake -S . -B build-x86 -A Win32
cmake --build build-x86 --config Release
```

## Validation + Packaging

- Validate profile structure:
  - `python tools/validate_profile.py profiles/default.json`
- Assemble release package bundle:
  - `powershell -ExecutionPolicy Bypass -File tools/package_release.ps1`

## DAoC Test Run

- Follow `docs/run_daoc_test.md`.
- Injector executable is built from `tools/injector`.
- Required runtime artifacts:
  - `build-x86/Release/camelotpad_core.dll`
  - `build-x86/Release/camelotpad_injector.exe`
  - `camelotpad-lua/ui/main.lua`

## Dependencies

- Windows SDK XInput (`XInputGetState`) for controller polling.
- Planned package dependencies in `vcpkg.json`:
  - `imgui`
  - `lua`
  - `sol2`
  - `detours`

## Internet Research Used

- DAoC Hook repository and docs for architecture and integration surface:
  - `https://github.com/daochook/daochook`
  - `https://daochook.github.io/`
  - `https://daochook.github.io/features/`
  - `https://daochook.github.io/developers/libraries/imgui/`
- Microsoft XInput deadzone/normalization recommendations:
  - `https://learn.microsoft.com/en-us/windows/win32/xinput/getting-started-with-xinput`

## Key Files

- `camelotpad-core/include/camelotpad/integrations/DaochookInterfaces.hpp`
- `camelotpad-core/include/camelotpad/input/XInputProcessor.hpp`
- `camelotpad-core/include/camelotpad/discovery/ValueDiscovery.hpp`
- `camelotpad-core/include/camelotpad/render/Dx9LuaUiBridge.hpp`
- `camelotpad-core/include/camelotpad/actions/ActionRouter.hpp`
- `camelotpad-core/include/camelotpad/config/ProfileManager.hpp`

## Remaining Integration Work

1. Replace null DAoC bridges with real v1.1.0.0-backed implementations from your local DAoC Hook symbols.
2. Connect `Dx9LuaUiBridge` callback to actual ImGui backend + Lua VM draw invocation.
3. Populate concrete signatures/offsets in `docs/values_registry.md` and set `validated` to `yes` only after in-client verification.

