# CamelotPad Test Matrix

## Compatibility Matrix

| Area | Scenario | Expected |
| --- | --- | --- |
| Display Mode | Windowed | Overlay draws, no stuck input |
| Display Mode | Borderless | Overlay draws, camera smooth |
| Display Mode | Fullscreen | DX9 callback stable, no device-loss crash |
| Controller | Xbox Series X/S | All default qbar mappings trigger |
| Controller | Elite | Paddle remaps do not break modifiers |
| Session Stability | 2+ hour session | No memory growth spikes, no key lock |

## Functional Tests

| Feature | Test | Expected |
| --- | --- | --- |
| Movement | Left stick forward/strafe | `W/S/Q/E` equivalent behavior |
| Camera | Right stick mouse-look | Smooth yaw/pitch with clamp |
| Targeting | Reticle over entity + face | Face target command fires |
| Qbars | RT/RB/A/B/X/Y/D-pad | Slots 1-10 trigger |
| Modifiers | LT/LB/LT+LB | Correct bar layers 2/3/4 |
| Profiles | Export/import | Schema-compatible JSON roundtrip |

## Soak and Fault Tests
- Controller unplug/replug during combat.
- Alt-tab during held stick input.
- Missing registry value fallback path.
- Null entity pointer handling in targeting module.
