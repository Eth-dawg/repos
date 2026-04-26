#pragma once

#include "camelotpad/Types.hpp"
#include "camelotpad/actions/ActionRouter.hpp"
#include "camelotpad/config/ProfileManager.hpp"
#include "camelotpad/discovery/ValueDiscovery.hpp"
#include "camelotpad/input/XInputProcessor.hpp"
#include "camelotpad/integrations/DaochookInterfaces.hpp"
#include "camelotpad/platform/WinInputEmitter.hpp"
#include "camelotpad/render/Dx9LuaUiBridge.hpp"

#include <atomic>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <string>

namespace camelotpad {

class CamelotPadApp {
public:
    CamelotPadApp();
    ~CamelotPadApp();

    bool Start();
    void Stop();
    void Tick();
    void OnDx9EndScene(void* device);

private:
    void InitializeDefaultBindings();
    void PollController();
    void UpdateMovement();
    void UpdateCamera();
    void UpdateTargeting();
    void DispatchActions();
    void RenderUi();
    void UpdatePerformanceEstimates();
    void BootstrapDefaultProfile();

    void Log(const std::string& message);

    std::atomic<bool> running_ {false};
    RuntimeFrame frame_ {};
    actions::ActionRouter actionRouter_ {};
    config::ProfileManager profileManager_ {};
    input::XInputProcessor xinput_ {};
    platform::WinInputEmitter inputEmitter_ {};
    discovery::ValueDiscoveryAdapter valueDiscovery_ {};
    integrations::NullDaocMemoryBridge nullMemoryBridge_ {};
    integrations::NullDaocCommandBridge nullCommandBridge_ {};
    integrations::DaochookInterfaceSet daocInterfaces_ {&nullMemoryBridge_, &nullCommandBridge_};
    render::Dx9LuaUiBridge uiBridge_ {};
    std::filesystem::path logFilePath_ {};
    std::ofstream logFile_ {};
    std::mutex logMutex_ {};
    std::chrono::steady_clock::time_point lastFrameTime_ {};
};

} // namespace camelotpad
