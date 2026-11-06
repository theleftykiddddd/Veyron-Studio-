#include "VeyronStudio.hpp"

#include <map>
#include <sstream>

namespace Veyron {

std::string StructureVisualizer::buildSummary(const LevelData& level) {
    if (level.objects.empty()) return "No object records to visualize.";

    std::map<int, std::size_t> sections;
    for (const auto& object : level.objects) {
        const int section = static_cast<int>(object.x / 500.0);
        ++sections[section];
    }

    std::ostringstream output;
    output << "STRUCTURE MAP // " << level.name << '\n';
    for (const auto& [section, count] : sections) {
        output << "SEG " << section << "  ";
        const std::size_t bars = count > 24 ? 24 : count;
        for (std::size_t i = 0; i < bars; ++i) output << '#';
        output << "  " << count << " objects\n";
    }
    output << "\nEach segment represents a 500-unit X interval in the supplied data.";
    return output.str();
}

} // namespace Veyron
