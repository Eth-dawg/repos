#include "camelotpad/config/ProfileManager.hpp"

#include <fstream>
#include <sstream>

namespace camelotpad::config {

bool ProfileManager::ExportProfile(const RuntimeFrame& frame, const std::filesystem::path& outputPath) const {
    std::ofstream out(outputPath);
    if (!out.is_open()) {
        lastError_ = "Failed to open output profile path: " + outputPath.string();
        return false;
    }

    out << "{\n";
    out << "  \"profileName\": \"default\",\n";
    out << "  \"input\": {\n";
    out << "    \"leftDeadzone\": 0.15,\n";
    out << "    \"rightDeadzone\": 0.15,\n";
    out << "    \"invertY\": false\n";
    out << "  },\n";
    out << "  \"camera\": {\n";
    out << "    \"sensitivityX\": 3.2,\n";
    out << "    \"sensitivityY\": 3.2,\n";
    out << "    \"pitchMin\": " << frame.camera.pitchMin << ",\n";
    out << "    \"pitchMax\": " << frame.camera.pitchMax << "\n";
    out << "  },\n";
    out << "  \"targeting\": {\n";
    out << "    \"priority\": \"" << (frame.targeting.priority == TargetPriority::HostileFirst ? "hostileFirst" : "friendlyFirst") << "\",\n";
    out << "    \"maxRange\": 3500,\n";
    out << "    \"tabFallback\": true\n";
    out << "  },\n";
    out << "  \"quickbars\": [\n";
    for (std::size_t i = 0; i < frame.quickbarDefaults.size(); ++i) {
        const auto& binding = frame.quickbarDefaults[i];
        out << "    { \"button\": \"" << binding.buttonId << "\", \"bar\": " << static_cast<int>(binding.bar)
            << ", \"slot\": " << static_cast<int>(binding.slot) << " }";
        out << (i + 1 < frame.quickbarDefaults.size() ? ",\n" : "\n");
    }
    out << "  ]\n";
    out << "}\n";

    lastError_.clear();
    return true;
}

bool ProfileManager::ImportProfile(const std::filesystem::path& profilePath, RuntimeFrame& frameOut) const {
    std::ifstream input(profilePath);
    if (!input.is_open()) {
        lastError_ = "Failed to open profile path: " + profilePath.string();
        return false;
    }

    // Minimal safe import: keep defaults and only verify expected keys exist.
    std::stringstream buffer;
    buffer << input.rdbuf();
    const std::string json = buffer.str();
    if (json.find("\"quickbars\"") == std::string::npos || json.find("\"camera\"") == std::string::npos) {
        lastError_ = "Profile missing required keys.";
        return false;
    }

    (void)frameOut;
    lastError_.clear();
    return true;
}

std::string ProfileManager::LastError() const {
    return lastError_;
}

} // namespace camelotpad::config
