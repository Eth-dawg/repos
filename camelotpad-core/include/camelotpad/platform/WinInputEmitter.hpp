#pragma once

#include "camelotpad/Types.hpp"

namespace camelotpad::platform {

class WinInputEmitter {
public:
    void EmitFrame(const RuntimeFrame& frame);
    void ResetAll();

private:
    void EmitMovement(const MovementState& movement);
    void EmitCamera(const CameraState& camera);
    void SetKeyDown(unsigned short virtualKey, bool shouldBeDown);

    bool wDown_ {false};
    bool sDown_ {false};
    bool qDown_ {false};
    bool eDown_ {false};
    bool spaceDown_ {false};
};

} // namespace camelotpad::platform
