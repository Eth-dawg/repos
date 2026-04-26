# CamelotPad Integration Notes

## Internet Research Baseline
- DAoC Hook repository: `https://github.com/daochook/daochook`
- DAoC Hook docs: `https://daochook.github.io/`
- DAoC Hook features: `https://daochook.github.io/features/`
- DAoC Hook README (raw): `https://raw.githubusercontent.com/daochook/daochook/master/README.md`

## Confirmed Integration Constraints
- DAoC Hook and DAoC client run as 32-bit.
- DAoC Hook exposes ImGui and Lua addon hooks suitable for in-game configuration and overlay controls.
- DAoC Hook uses in-process hooking and a launcher/injector model.
- Supported client versions are explicit and should be treated as hard compatibility gates.

## Required Value Families (To Populate in `values_registry.md`)
- `targeting.*`: target pointer chain, entity list links, entity type/faction markers.
- `camera.*`: yaw/pitch write points and clamp-safe ranges.
- `quickbar.*`: bar index, slot cooldown/stack/proc access patterns.
- `group.*`: group member status and HP/power data pointers.
- `reticle.*`: world-to-screen projection linkage for hover target acquisition.

## Implementation Rule
Every feature that relies on memory values must:
1. Reference a key in `docs/values_registry.md`.
2. Check value validity at runtime.
3. Enter fallback mode when unresolved.
