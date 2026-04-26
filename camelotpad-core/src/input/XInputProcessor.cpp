#include "camelotpad/input/XInputProcessor.hpp"

#include <Windows.h>
#include <Xinput.h>

#include <algorithm>
#include <cmath>

namespace camelotpad::input {

namespace {
constexpr float kMaxAxisMagnitude = 32767.0F;
constexpr float kTriggerMax = 255.0F;
} // namespace

ControllerState XInputProcessor::Poll() const {
    XINPUT_STATE nativeState {};
    const DWORD result = XInputGetState(0, &nativeState);

    ControllerState state {};
    if (result != ERROR_SUCCESS) {
        state.connected = false;
        return state;
    }

    state.connected = true;
    state.buttons = nativeState.Gamepad.wButtons;

    StickTuning leftTuning {};
    leftTuning.deadzoneNormalized = static_cast<float>(XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) / kMaxAxisMagnitude;

    StickTuning rightTuning {};
    rightTuning.deadzoneNormalized = static_cast<float>(XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) / kMaxAxisMagnitude;

    TriggerTuning triggerTuning {};
    triggerTuning.deadzoneNormalized = static_cast<float>(XINPUT_GAMEPAD_TRIGGER_THRESHOLD) / kTriggerMax;

    state.left = ProcessStick(nativeState.Gamepad.sThumbLX, nativeState.Gamepad.sThumbLY, leftTuning);
    state.right = ProcessStick(nativeState.Gamepad.sThumbRX, nativeState.Gamepad.sThumbRY, rightTuning);

    const float leftTriggerFloat = ProcessTrigger(nativeState.Gamepad.bLeftTrigger, triggerTuning);
    const float rightTriggerFloat = ProcessTrigger(nativeState.Gamepad.bRightTrigger, triggerTuning);
    state.leftTrigger = static_cast<unsigned char>(std::clamp(leftTriggerFloat, 0.0F, 1.0F) * 255.0F);
    state.rightTrigger = static_cast<unsigned char>(std::clamp(rightTriggerFloat, 0.0F, 1.0F) * 255.0F);
    return state;
}

float XInputProcessor::NormalizeAxis(short raw) {
    const float normalized = std::clamp(static_cast<float>(raw) / kMaxAxisMagnitude, -1.0F, 1.0F);
    return normalized;
}

StickState XInputProcessor::ProcessStick(short rawX, short rawY, const StickTuning& tuning) {
    float x = NormalizeAxis(rawX);
    float y = NormalizeAxis(rawY);

    if (tuning.invertX) {
        x = -x;
    }
    if (tuning.invertY) {
        y = -y;
    }

    const float magnitude = std::sqrt((x * x) + (y * y));
    if (magnitude <= 0.0F) {
        return {};
    }

    const float curvedMagnitude = ApplyCurve(magnitude, tuning.deadzoneNormalized, tuning.exponent);
    if (curvedMagnitude <= 0.0F) {
        return {};
    }

    const float unitX = x / magnitude;
    const float unitY = y / magnitude;
    return {
        unitX * curvedMagnitude,
        unitY * curvedMagnitude,
    };
}

float XInputProcessor::ProcessTrigger(unsigned char raw, const TriggerTuning& tuning) {
    const float normalized = static_cast<float>(raw) / kTriggerMax;
    return ApplyCurve(normalized, tuning.deadzoneNormalized, tuning.exponent);
}

float XInputProcessor::ApplyCurve(float value, float deadzone, float exponent) {
    const float sign = (value < 0.0F) ? -1.0F : 1.0F;
    const float absValue = std::abs(value);
    if (absValue <= deadzone) {
        return 0.0F;
    }

    const float scaled = std::clamp((absValue - deadzone) / (1.0F - deadzone), 0.0F, 1.0F);
    const float curved = std::pow(scaled, std::max(exponent, 1.0F));
    return sign * curved;
}

} // namespace camelotpad::input
