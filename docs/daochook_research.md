# DAoC Hook Research Notes

This document tracks internet-sourced DAoC Hook references used to shape CamelotPad implementation.

## Primary Sources
- Repository: `https://github.com/daochook/daochook`
- Documentation: `https://daochook.github.io/`
- Features page: `https://daochook.github.io/features/`

## Confirmed Capability Areas
- In-process injection model with launcher support.
- Hooked game function pipeline (chat command handler, packet handlers).
- Direct3D device hook for in-scene rendering.
- ImGui exposed to Lua for addon UI.
- Lua addon environment (MoonJIT/LuaJIT style ecosystem).

## CamelotPad Implementation Impact
- Use in-process design for low-latency action routing and reticle behavior.
- Route configuration and overlay behavior through Lua + ImGui.
- Keep command/targeting compatibility with native client flows instead of bypassing them.
- Treat memory-facing data as versioned and validated through `docs/values_registry.md`.

## Values Still Required Before Full Feature Completion
- Entity traversal and reticle hit-testing pointers/signatures.
- Current target and face-target related addresses/signatures.
- Camera pitch/yaw integration points compatible with smooth mouse-look.
- Quickbar state (cooldown/stack/proc) memory layout references.
- Group member data layout for overlay frame rendering.

## Validation Policy
- Any value found via forums or third-party snippets is considered untrusted until confirmed through local client validation.
- Each validated value must be linked to DAoC client version and an internal test note.
