# Run In DAoC (Local Test)

## 1) Build x86 binaries
- Use an x86 generator because DAoC Hook/DAoC client are 32-bit.
- Example:
  - `cmake -S . -B build-x86 -A Win32 -DCMAKE_TOOLCHAIN_FILE=<vcpkg_root>/scripts/buildsystems/vcpkg.cmake`
  - `cmake --build build-x86 --config Release`

## 2) Start DAoC client
- Launch DAoC normally (or through your DAoC Hook launcher flow).
- Ensure the target process is running before injection.

## 3) Inject CamelotPad
- Injector binary: `build-x86/Release/camelotpad_injector.exe`
- DLL path: `build-x86/Release/camelotpad_core.dll`
- Run:
  - `build-x86/Release/camelotpad_injector.exe game.dll build-x86/Release/camelotpad_core.dll`

If your DAoC executable name differs, replace `game.dll` with the real process name.

## 4) Verify in-client behavior
- Left stick should drive `W/S/Q/E` emulation.
- Right stick should emit smooth mouse movement.
- `Back + Start` toggles overlay panel visibility.
- Check `logs/camelotpad.log` for startup/validation errors.

## 5) Known current limits
- DAoC bridge currently uses null bridge implementations; real memory/command hook symbols still need binding.
- `docs/values_registry.md` remains validation-gated until concrete signatures are filled and validated.
