#pragma once

#include <array>
#include <cstdint>
#include <string>

namespace camelotpad {

enum class TargetPriority : std::uint8_t {
    HostileFirst = 0,
    FriendlyFirst = 1
};

struct StickState {
    float x {0.0F};
    float y {0.0F};
};

struct ControllerState {
    StickState left {};
    StickState right {};
    std::uint16_t buttons {0};
    std::uint8_t leftTrigger {0};
    std::uint8_t rightTrigger {0};
    bool connected {false};
};

struct MovementState {
    float forwardBack {0.0F};
    float strafe {0.0F};
    bool sprint {false};
    bool jump {false};
};

struct CameraState {
    float yawDelta {0.0F};
    float pitchDelta {0.0F};
    bool autoCenter {false};
    float pitchMin {-85.0F};
    float pitchMax {85.0F};
};

struct TargetingState {
    bool hasReticleTarget {false};
    std::uint32_t reticleEntityId {0};
    TargetPriority priority {TargetPriority::HostileFirst};
};

struct QuickbarBinding {
    std::string buttonId {};
    std::uint8_t slot {0};
    std::uint8_t bar {1};
};

struct QuickbarAction {
    std::uint8_t bar {1};
    std::uint8_t slot {1};
    bool triggered {false};
};

struct MacroBinding {
    std::string comboId {};
    std::string command {};
};

struct RuntimeFrame {
    ControllerState controller {};
    MovementState movement {};
    CameraState camera {};
    TargetingState targeting {};
    std::array<QuickbarBinding, 10> quickbarDefaults {};
    std::array<MacroBinding, 4> macroBindings {};
    QuickbarAction activeQuickbarAction {};
    std::uint8_t currentQuickbarBar {1};
    bool overlayVisible {true};
    bool radialMenuVisible {false};
    bool groundTargetAssist {false};
    bool interruptAlert {false};
    float overlayScale {1.0F};
    float fpsEstimate {0.0F};
    float pingEstimateMs {0.0F};
};

} // namespace camelotpad
