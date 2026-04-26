#include "camelotpad/CamelotPadApp.hpp"

#include <chrono>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <stdexcept>

struct IDirect3DDevice9;

namespace {
constexpr std::uint16_t kSprintToggleButton = 0x0040U; // XINPUT_GAMEPAD_LEFT_THUMB
constexpr float kCameraSensitivity = 3.2F;
constexpr std::uint16_t kFaceTargetButton = 0x2000U; // XINPUT_GAMEPAD_Y
constexpr std::uint16_t kJumpButton = 0x1000U; // XINPUT_GAMEPAD_A
constexpr std::uint16_t kToggleOverlayCombo = 0x0030U; // back + start
} // namespace

namespace camelotpad {

CamelotPadApp::CamelotPadApp() = default;

CamelotPadApp::~CamelotPadApp() {
    Stop();
}

bool CamelotPadApp::Start() {
    if (running_.load()) {
        return true;
    }

    std::filesystem::create_directories("logs");
    logFilePath_ = std::filesystem::path("logs") / "camelotpad.log";
    logFile_.open(logFilePath_, std::ios::app);
    if (!logFile_.is_open()) {
        return false;
    }

    InitializeDefaultBindings();
    uiBridge_.SetDrawCallback([this]() {
        // Placeholder until real Lua VM integration is bound to main.draw().
        frame_.overlayScale = std::clamp(frame_.overlayScale, 0.7F, 1.5F);
    });
    uiBridge_.Initialize();
    BootstrapDefaultProfile();

    const auto report = valueDiscovery_.ValidateRegistry(std::filesystem::path("docs") / "values_registry.md");
    if (!report.success) {
        for (const auto& error : report.errors) {
            Log("Value registry validation: " + error);
        }
    } else {
        Log("Value registry validation passed.");
    }

    running_.store(true);
    lastFrameTime_ = std::chrono::steady_clock::now();
    Log("CamelotPad started.");
    return true;
}

void CamelotPadApp::Stop() {
    if (!running_.exchange(false)) {
        return;
    }

    Log("CamelotPad stopped.");
    inputEmitter_.ResetAll();
    uiBridge_.Shutdown();
    if (logFile_.is_open()) {
        logFile_.flush();
        logFile_.close();
    }
}

void CamelotPadApp::Tick() {
    if (!running_.load()) {
        return;
    }

    try {
        PollController();
        UpdateMovement();
        UpdateCamera();
        UpdateTargeting();
        DispatchActions();
        inputEmitter_.EmitFrame(frame_);
        RenderUi();
        UpdatePerformanceEstimates();
    } catch (const std::exception& ex) {
        Log(std::string("Tick exception: ") + ex.what());
    } catch (...) {
        Log("Tick exception: unknown.");
    }
}

void CamelotPadApp::OnDx9EndScene(void* device) {
    uiBridge_.OnEndScene(static_cast<IDirect3DDevice9*>(device));
}

void CamelotPadApp::InitializeDefaultBindings() {
    frame_.quickbarDefaults = {{
        {"RT", 1, 1},
        {"RB", 2, 1},
        {"A", 3, 1},
        {"B", 4, 1},
        {"X", 5, 1},
        {"Y", 6, 1},
        {"DPadUp", 7, 1},
        {"DPadDown", 8, 1},
        {"DPadLeft", 9, 1},
        {"DPadRight", 10, 1},
    }};
}

void CamelotPadApp::PollController() {
    frame_.controller = xinput_.Poll();
    if ((frame_.controller.buttons & kToggleOverlayCombo) == kToggleOverlayCombo) {
        frame_.overlayVisible = !frame_.overlayVisible;
        uiBridge_.TogglePanel();
    }
}

void CamelotPadApp::UpdateMovement() {
    if (!frame_.controller.connected) {
        frame_.movement = {};
        return;
    }

    const float leftY = frame_.controller.left.y;
    const float leftX = frame_.controller.left.x;

    frame_.movement.forwardBack = leftY;
    frame_.movement.strafe = leftX;
    frame_.movement.sprint = (frame_.controller.buttons & kSprintToggleButton) != 0;
    frame_.movement.jump = (frame_.controller.buttons & kJumpButton) != 0;
}

void CamelotPadApp::UpdateCamera() {
    frame_.camera.yawDelta = frame_.controller.right.x * kCameraSensitivity;
    const float rawPitch = frame_.controller.right.y * kCameraSensitivity;
    frame_.camera.pitchDelta = std::clamp(rawPitch, frame_.camera.pitchMin, frame_.camera.pitchMax);
    frame_.camera.autoCenter = false;
}

void CamelotPadApp::UpdateTargeting() {
    const auto maybeEntity = daocInterfaces_.Memory()->GetReticleEntity();
    frame_.targeting.hasReticleTarget = maybeEntity.has_value();
    frame_.targeting.reticleEntityId = maybeEntity.has_value() ? maybeEntity->entityId : 0;
    frame_.targeting.priority = TargetPriority::HostileFirst;
}

void CamelotPadApp::DispatchActions() {
    actionRouter_.ResolveQuickbarAction(frame_);
    actionRouter_.ResolveMacroActions(frame_);
    actionRouter_.ResolveAssistActions(frame_);

    if ((frame_.controller.buttons & kFaceTargetButton) != 0 && frame_.targeting.hasReticleTarget) {
        if (daocInterfaces_.Memory()->FaceTarget(frame_.targeting.reticleEntityId)) {
            std::ostringstream message;
            message << "Face target action on entity " << frame_.targeting.reticleEntityId;
            Log(message.str());
        } else {
            Log("Face target action failed (bridge not ready or blocked).");
        }
    }

    if (frame_.activeQuickbarAction.triggered) {
        std::ostringstream command;
        command << "/qbar " << static_cast<int>(frame_.activeQuickbarAction.bar);
        daocInterfaces_.Command()->ExecuteSlash(command.str());
    }
}

void CamelotPadApp::RenderUi() {
    // Per-frame UI state updates can be added here.
}

void CamelotPadApp::UpdatePerformanceEstimates() {
    const auto now = std::chrono::steady_clock::now();
    const auto delta = now - lastFrameTime_;
    lastFrameTime_ = now;

    const float deltaSeconds = std::chrono::duration<float>(delta).count();
    if (deltaSeconds > 0.0F) {
        frame_.fpsEstimate = 1.0F / deltaSeconds;
    }
    frame_.pingEstimateMs = frame_.controller.connected ? 35.0F : 0.0F;
}

void CamelotPadApp::BootstrapDefaultProfile() {
    std::filesystem::create_directories("profiles");
    const auto defaultPath = std::filesystem::path("profiles") / "default.json";
    if (!std::filesystem::exists(defaultPath)) {
        profileManager_.ExportProfile(frame_, defaultPath);
        Log("Created default profile export.");
    }
}

void CamelotPadApp::Log(const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex_);
    if (!logFile_.is_open()) {
        return;
    }

    const auto now = std::chrono::system_clock::now();
    const auto timeValue = std::chrono::system_clock::to_time_t(now);
    std::tm localTime {};
#if defined(_WIN32)
    localtime_s(&localTime, &timeValue);
#else
    localtime_r(&timeValue, &localTime);
#endif

    logFile_ << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S") << " | " << message << '\n';
}

} // namespace camelotpad
