#pragma once

#include "camelotpad/Types.hpp"

namespace camelotpad::input {

struct StickTuning {
    float deadzoneNormalized {0.15F};
    float exponent {1.8F};
    bool invertX {false};
    bool invertY {false};
};

struct TriggerTuning {
    float deadzoneNormalized {0.12F};
    float exponent {1.4F};
};

class XInputProcessor {
public:
    ControllerState Poll() const;

private:
    static float NormalizeAxis(short raw);
    static StickState ProcessStick(short rawX, short rawY, const StickTuning& tuning);
    static float ProcessTrigger(unsigned char raw, const TriggerTuning& tuning);
    static float ApplyCurve(float value, float deadzone, float exponent);
};

} // namespace camelotpad::input
