#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace camelotpad::discovery {

struct ValueRegistryEntry {
    std::string key {};
    std::string sourceType {};
    std::string valueOrSignature {};
    std::string validated {};
    bool readyForUse {false};
};

struct ValidationReport {
    bool success {false};
    std::vector<std::string> errors {};
    std::vector<ValueRegistryEntry> entries {};
};

class ValueDiscoveryAdapter {
public:
    ValidationReport ValidateRegistry(const std::filesystem::path& registryPath) const;

private:
    static std::vector<std::string> SplitTableCells(const std::string& row);
    static std::string Trim(std::string value);
};

} // namespace camelotpad::discovery
