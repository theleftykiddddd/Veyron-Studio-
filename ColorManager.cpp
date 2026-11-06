#include "VeyronStudio.hpp"

namespace Veyron {

std::map<int, std::size_t> ColorManager::countUsage(const LevelData& level) {
    std::map<int, std::size_t> usage;
    for (const auto& object : level.objects) {
        if (object.colorID > 0) ++usage[object.colorID];
    }
    return usage;
}

std::vector<int> ColorManager::findUnused(const LevelData& level, int firstID, int lastID) {
    std::vector<int> unused;
    const auto usage = countUsage(level);
    if (firstID > lastID) return unused;
    for (int id = firstID; id <= lastID; ++id) {
        if (usage.find(id) == usage.end()) unused.push_back(id);
    }
    return unused;
}

} // namespace Veyron
