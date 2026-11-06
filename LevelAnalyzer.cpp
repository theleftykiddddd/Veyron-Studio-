#include "VeyronStudio.hpp"

#include <cmath>
#include <string>
#include <vector>

namespace Veyron {

std::vector<Diagnostic> LevelAnalyzer::analyze(const LevelData& level, int sensitivity) const {
    std::vector<Diagnostic> results;
    if (sensitivity < 1) sensitivity = 1;
    if (sensitivity > 5) sensitivity = 5;

    if (level.name.empty()) {
        results.push_back({DiagnosticSeverity::Error, "LEVEL_NAME", "The level has no name."});
    }
    if (level.objects.empty()) {
        results.push_back({DiagnosticSeverity::Info, "EMPTY_LEVEL", "No object records were supplied to the analyzer."});
        return results;
    }

    std::size_t triggerCount = 0;
    for (std::size_t index = 0; index < level.objects.size(); ++index) {
        const auto& object = level.objects[index];
        const std::string location = "Object record " + std::to_string(index + 1) + ": ";

        if (object.objectID <= 0) {
            results.push_back({DiagnosticSeverity::Error, "OBJECT_ID", location + "object ID must be greater than zero."});
        }
        if (object.groupID < 0 || object.groupID > 999) {
            results.push_back({DiagnosticSeverity::Warning, "GROUP_RANGE", location + "group ID is outside the common 0-999 range."});
        }
        if (object.colorID < 0 || object.colorID > 999) {
            results.push_back({DiagnosticSeverity::Warning, "COLOR_RANGE", location + "color ID is outside the common 0-999 range."});
        }
        if (!std::isfinite(object.x) || !std::isfinite(object.y)) {
            results.push_back({DiagnosticSeverity::Error, "POSITION_NAN", location + "position is not a finite number."});
        } else {
            const double coordinateLimit = sensitivity >= 4 ? 50000.0 : 100000.0;
            if (std::abs(object.x) > coordinateLimit || std::abs(object.y) > coordinateLimit) {
                results.push_back({DiagnosticSeverity::Warning, "POSITION_EXTREME", location + "position is unusually far from the origin; review it manually."});
            }
        }
        if (object.isTrigger) ++triggerCount;
    }

    if (triggerCount == 0) {
        results.push_back({DiagnosticSeverity::Info, "NO_TRIGGERS", "No trigger records were marked in this dataset."});
    }
    if (results.empty()) {
        results.push_back({DiagnosticSeverity::Info, "CLEAN", "No issues were found in the supplied object records."});
    }
    return results;
}

} // namespace Veyron
