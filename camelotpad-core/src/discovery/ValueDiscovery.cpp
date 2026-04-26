#include "camelotpad/discovery/ValueDiscovery.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>

namespace camelotpad::discovery {

ValidationReport ValueDiscoveryAdapter::ValidateRegistry(const std::filesystem::path& registryPath) const {
    ValidationReport report {};
    std::ifstream input(registryPath);
    if (!input.is_open()) {
        report.errors.emplace_back("Unable to open values registry: " + registryPath.string());
        return report;
    }

    std::string line {};
    while (std::getline(input, line)) {
        if (line.empty() || line.front() != '|') {
            continue;
        }
        if (line.find("| ---") != std::string::npos || line.find("| key ") != std::string::npos) {
            continue;
        }

        const auto cells = SplitTableCells(line);
        if (cells.size() < 8) {
            continue;
        }

        ValueRegistryEntry entry {};
        entry.key = Trim(cells[0]);
        entry.sourceType = Trim(cells[2]);
        entry.valueOrSignature = Trim(cells[3]);
        entry.validated = Trim(cells[7]);

        const bool hasConcreteValue = !entry.valueOrSignature.empty() && entry.valueOrSignature != "TODO";
        const bool hasValidationFlag = entry.validated == "yes";
        entry.readyForUse = hasConcreteValue && hasValidationFlag;

        if (!entry.readyForUse) {
            std::ostringstream message;
            message << "Registry entry '" << entry.key << "' is not production-ready"
                    << " (value='" << entry.valueOrSignature << "', validated='" << entry.validated << "').";
            report.errors.push_back(message.str());
        }

        report.entries.push_back(entry);
    }

    report.success = !report.entries.empty() && report.errors.empty();
    if (report.entries.empty()) {
        report.errors.emplace_back("No registry entries detected in values registry markdown table.");
    }
    return report;
}

std::vector<std::string> ValueDiscoveryAdapter::SplitTableCells(const std::string& row) {
    std::vector<std::string> cells {};
    std::string current {};
    for (char ch : row) {
        if (ch == '|') {
            if (!current.empty()) {
                cells.push_back(current);
                current.clear();
            }
            continue;
        }
        current.push_back(ch);
    }
    if (!current.empty()) {
        cells.push_back(current);
    }
    return cells;
}

std::string ValueDiscoveryAdapter::Trim(std::string value) {
    value.erase(value.begin(), std::find_if(value.begin(), value.end(), [](unsigned char c) { return !std::isspace(c); }));
    value.erase(std::find_if(value.rbegin(), value.rend(), [](unsigned char c) { return !std::isspace(c); }).base(), value.end());
    return value;
}

} // namespace camelotpad::discovery
