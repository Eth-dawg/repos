# CamelotPad Troubleshooting

## No Controller Input
- Confirm controller is detected by Windows Game Controllers panel.
- Verify `XInputGetState` path is available in your runtime.
- Check `logs/camelotpad.log` for connection and startup errors.

## Overlay Does Not Appear
- Verify `camelotpad-lua/ui/main.lua` exists in deployment path.
- Ensure DX9 hook path calls `CamelotPad_OnDx9EndScene`.
- Toggle panel with `Back + Start`.

## Targeting Not Working
- Inspect `docs/values_registry.md`; unresolved entries trigger fallback behavior.
- Confirm runtime log does not report `Value registry validation` failures for required targeting keys.
- Ensure DAoC memory bridge implementation is not using null stubs.

## Quickbar Not Triggering
- Confirm active modifier layer (`LT`, `LB`, `LT+LB`) maps expected bar.
- Validate command bridge supports `/qbar` execution.
- Verify bindings in exported profile JSON.

## Performance / Stutter
- Disable optional overlays (FPS/ping, radial menu) and compare frame pacing.
- Confirm Release build and x86 target.
- Profile EndScene hook duration to keep overlay draw budget low.
