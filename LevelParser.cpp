#include "VeyronStudio.hpp"

#include <sstream>
#include <string>
#include <vector>

namespace Veyron {
namespace {

std::vector<std::string> split(const std::string& value, char delimiter) {
    std::vector<std::string> parts;
    std::stringstream stream(value);
    std::string part;
    while (std::getline(stream, part, delimiter)) {
        parts.push_back(part);
    }
    return parts;
}

bool parseInt(const std::string& value, int& output) {
    try {
        std::size_t used = 0;
        const int parsed = std::stoi(value, &used);
        if (used != value.size()) return false;
        output = parsed;
        return true;
    } catch (...) {
        return false;
    }
}

bool parseDouble(const std::string& value, double& output) {
    try {
        std::size_t used = 0;
        const double parsed = std::stod(value, &used);
        if (used != value.size()) return false;
        output = parsed;
        return true;
    } catch (...) {
        return false;
    }
}

} // namespace

bool LevelParser::parseFixture(const std::string& text, LevelData& output, std::string& error) {
    LevelData parsed;
    std::stringstream stream(text);
    std::string line;
    std::size_t lineNumber = 0;

    while (std::getline(stream, line)) {
        ++lineNumber;
        if (line.empty() || line[0] == '#') continue;

        if (line.rfind("name=", 0) == 0) {
            parsed.name = line.substr(5);
            continue;
        }

        if (line.rfind("object=", 0) == 0) {
            const auto fields = split(line.substr(7), ',');
            if (fields.size() != 6) {
                error = "Line " + std::to_string(lineNumber) + ": object requires 6 comma-separated fields";
                return false;
            }

            LevelObject object;
            int trigger = 0;
            if (!parseInt(fields[0], object.objectID) ||
                !parseInt(fields[1], object.groupID) ||
                !parseInt(fields[2], object.colorID) ||
                !parseDouble(fields[3], object.x) ||
                !parseDouble(fields[4], object.y) ||
                !parseInt(fields[5], trigger) || (trigger != 0 && trigger != 1)) {
                error = "Line " + std::to_string(lineNumber) + ": invalid object field";
                return false;
            }
            object.isTrigger = trigger == 1;
            parsed.objects.push_back(object);
            continue;
        }

        error = "Line " + std::to_string(lineNumber) + ": unknown directive";
        return false;
    }

    if (parsed.name.empty()) {
        error = "Missing name=<level name> header";
        return false;
    }

    output = std::move(parsed);
    error.clear();
    return true;
}

} // namespace Veyron
