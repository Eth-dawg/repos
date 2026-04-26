# CamelotPad DAoC Values Registry

This file is the source of truth for all DAoC memory values and signatures used by CamelotPad.

## Rules
- Do not ship any value without live-client validation.
- Prefer `pattern_scanned` entries over `static_offset` when practical.
- Include the DAoC client version and DAoC Hook commit/tag that validated each entry.

## Registry Template

| key | category | source_type | value_or_signature | module | daoc_client_version | daochook_reference | validated | fallback_behavior |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| entity_list_head | targeting | pattern_scanned | TODO | TargetingModule | TODO | TODO | no | disable_reticle_targeting |
| current_target_ptr | targeting | pattern_scanned | TODO | TargetingModule | TODO | TODO | no | use_tab_target_fallback |
| camera_yaw_addr | camera | pattern_scanned | TODO | CameraModule | TODO | TODO | no | mouse_delta_only |
| camera_pitch_addr | camera | pattern_scanned | TODO | CameraModule | TODO | TODO | no | clamp_internal_pitch |
| quickbar_state_base | quickbar | static_offset | TODO | ActionModule | TODO | TODO | no | keyboard_qbind_only |
| group_member_base | ui | pattern_scanned | TODO | UiModule | TODO | TODO | no | disable_group_overlay |

## Validation Status
- Runtime adapter: `ValueDiscoveryAdapter::ValidateRegistry`.
- Current result: expected **not ready** until concrete signatures/offsets replace `TODO` and `validated` is set to `yes`.
- Shipping rule: if any required key is not ready, affected feature must enter its fallback behavior.
