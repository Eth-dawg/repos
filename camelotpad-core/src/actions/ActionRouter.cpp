#include "camelotpad/actions/ActionRouter.hpp"

namespace camelotpad::actions {

namespace {
constexpr std::uint16_t kButtonA = 0x1000U;
constexpr std::uint16_t kButtonB = 0x2000U;
constexpr std::uint16_t kButtonX = 0x4000U;
constexpr std::uint16_t kButtonY = 0x8000U;
constexpr std::uint16_t kDPadUp = 0x0001U;
constexpr std::uint16_t kDPadDown = 0x0002U;
constexpr std::uint16_t kDPadLeft = 0x0004U;
constexpr std::uint16_t kDPadRight = 0x0008U;
constexpr std::uint16_t kLeftShoulder = 0x0100U;
constexpr std::uint16_t kRightShoulder = 0x0200U;
} // namespace

void ActionRouter::ResolveQuickbarAction(RuntimeFrame& frame) const {
    frame.activeQuickbarAction = {};
    frame.activeQuickbarAction.bar = ResolveModifierBar(frame.controller);
    frame.currentQuickbarBar = frame.activeQuickbarAction.bar;

    auto triggerSlot = [&](std::uint8_t slot) {
        frame.activeQuickbarAction.slot = slot;
        frame.activeQuickbarAction.triggered = true;
    };

    if (frame.controller.rightTrigger > 30) {
        triggerSlot(1);
    } else if (ButtonPressed(frame.controller.buttons, kRightShoulder)) {
        triggerSlot(2);
    } else if (ButtonPressed(frame.controller.buttons, kButtonA)) {
        triggerSlot(3);
    } else if (ButtonPressed(frame.controller.buttons, kButtonB)) {
        triggerSlot(4);
    } else if (ButtonPressed(frame.controller.buttons, kButtonX)) {
        triggerSlot(5);
    } else if (ButtonPressed(frame.controller.buttons, kButtonY)) {
        triggerSlot(6);
    } else if (ButtonPressed(frame.controller.buttons, kDPadUp)) {
        triggerSlot(7);
    } else if (ButtonPressed(frame.controller.buttons, kDPadDown)) {
        triggerSlot(8);
    } else if (ButtonPressed(frame.controller.buttons, kDPadLeft)) {
        triggerSlot(9);
    } else if (ButtonPressed(frame.controller.buttons, kDPadRight)) {
        triggerSlot(10);
    }
}

void ActionRouter::ResolveMacroActions(RuntimeFrame& frame) const {
    for (const auto& macro : frame.macroBindings) {
        if (macro.comboId.empty()) {
            continue;
        }
        // Marker flag used by UI for now; command execution is routed via chat bridge.
        frame.interruptAlert = frame.interruptAlert || (macro.comboId == "LT+Y");
    }
}

void ActionRouter::ResolveAssistActions(RuntimeFrame& frame) const {
    if (frame.targeting.hasReticleTarget && frame.activeQuickbarAction.triggered) {
        frame.groundTargetAssist = true;
    }
}

bool ActionRouter::ButtonPressed(std::uint16_t buttons, std::uint16_t flag) {
    return (buttons & flag) != 0;
}

std::uint8_t ActionRouter::ResolveModifierBar(const ControllerState& controller) {
    const bool lt = controller.leftTrigger > 30;
    const bool lb = (controller.buttons & kLeftShoulder) != 0;

    if (lt && lb) {
        return 4;
    }
    if (lb) {
        return 3;
    }
    if (lt) {
        return 2;
    }
    return 1;
}

} // namespace camelotpad::actions
