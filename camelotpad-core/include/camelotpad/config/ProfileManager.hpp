#pragma once

#include "camelotpad/Types.hpp"

#include <filesystem>
#include <string>

namespace camelotpad::config {

class ProfileManager {
public:
    bool ExportProfile(const RuntimeFrame& frame, const std::filesystem::path& outputPath) const;
    bool ImportProfile(const std::filesystem::path& profilePath, RuntimeFrame& frameOut) const;
    std::string LastError() const;

private:
    mutable std::string lastError_ {};
};

} // namespace camelotpad::config
