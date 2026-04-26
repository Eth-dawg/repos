#include "camelotpad/platform/WinInputEmitter.hpp"

#include <Windows.h>

#include <algorithm>
#include <cmath>

namespace camelotpad::platform {

namespace {
constexpr float kMovePressThreshold = 0.2F;
constexpr float kMouseScale = 14.0F;
} // namespace

void WinInputEmitter::EmitFrame(const RuntimeFrame& frame) {
    EmitMovement(frame.movement);
    EmitCamera(frame.camera);
}

void WinInputEmitter::ResetAll() {
    SetKeyDown('W', false);
    SetKeyDown('S', false);
    SetKeyDown('Q', false);
    SetKeyDown('E', false);
    SetKeyDown(VK_SPACE, false);
}

void WinInputEmitter::EmitMovement(const MovementState& movement) {
    SetKeyDown('W', movement.forwardBack > kMovePressThreshold);
    SetKeyDown('S', movement.forwardBack < -kMovePressThreshold);
    SetKeyDown('E', movement.strafe > kMovePressThreshold);
    SetKeyDown('Q', movement.strafe < -kMovePressThreshold);
    SetKeyDown(VK_SPACE, movement.jump);
}

void WinInputEmitter::EmitCamera(const CameraState& camera) {
    const float clampedYaw = std::clamp(camera.yawDelta, -50.0F, 50.0F);
    const float clampedPitch = std::clamp(camera.pitchDelta, -50.0F, 50.0F);

    const LONG dx = static_cast<LONG>(std::round(clampedYaw * kMouseScale));
    const LONG dy = static_cast<LONG>(std::round(-clampedPitch * kMouseScale));
    if (dx == 0 && dy == 0) {
        return;
    }

    INPUT input {};
    input.type = INPUT_MOUSE;
    input.mi.dx = dx;
    input.mi.dy = dy;
    input.mi.dwFlags = MOUSEEVENTF_MOVE;
    SendInput(1, &input, sizeof(INPUT));
}

void WinInputEmitter::SetKeyDown(unsigned short virtualKey, bool shouldBeDown) {
    bool* state = nullptr;
    if (virtualKey == 'W') {
        state = &wDown_;
    } else if (virtualKey == 'S') {
        state = &sDown_;
    } else if (virtualKey == 'Q') {
        state = &qDown_;
    } else if (virtualKey == 'E') {
        state = &eDown_;
    } else if (virtualKey == VK_SPACE) {
        state = &spaceDown_;
    } else {
        return;
    }

    if (*state == shouldBeDown) {
        return;
    }

    INPUT input {};
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = static_cast<WORD>(virtualKey);
    input.ki.dwFlags = shouldBeDown ? 0 : KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT));
    *state = shouldBeDown;
}

} // namespace camelotpad::platform
