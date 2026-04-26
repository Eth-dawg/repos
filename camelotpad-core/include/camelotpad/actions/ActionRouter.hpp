#pragma once

#include "camelotpad/Types.hpp"

namespace camelotpad::actions {

class ActionRouter {
public:
    void ResolveQuickbarAction(RuntimeFrame& frame) const;
    void ResolveMacroActions(RuntimeFrame& frame) const;
    void ResolveAssistActions(RuntimeFrame& frame) const;

private:
    static bool ButtonPressed(std::uint16_t buttons, std::uint16_t flag);
    static std::uint8_t ResolveModifierBar(const ControllerState& controller);
};

} // namespace camelotpad::actions
