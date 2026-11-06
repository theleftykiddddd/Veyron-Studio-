#include "VeyronStudio.hpp"

#include <algorithm>
#include <sstream>
#include <unordered_map>

namespace Veyron {
namespace {

std::string objectKey(const LevelObject& object) {
    std::ostringstream key;
    key.precision(12);
    key << object.objectID << ':' << object.groupID << ':' << object.colorID
        << ':' << object.x << ':' << object.y << ':' << object.isTrigger;
    return key.str();
}

} // namespace

void SnapshotManager::takeSnapshot(const LevelData& level) {
    m_latestSnapshot = level.objects;
    ++m_snapshotCount;
    m_hasSnapshot = true;
}

SnapshotDiff SnapshotManager::compareWithLatest(const LevelData& level) const {
    SnapshotDiff diff;
    if (!m_hasSnapshot) return diff;

    std::unordered_map<std::string, std::size_t> oldCounts;
    std::unordered_map<std::string, std::size_t> newCounts;
    for (const auto& object : m_latestSnapshot) ++oldCounts[objectKey(object)];
    for (const auto& object : level.objects) ++newCounts[objectKey(object)];

    for (const auto& [key, count] : oldCounts) {
        const auto found = newCounts.find(key);
        const std::size_t current = found == newCounts.end() ? 0 : found->second;
        if (count > current) diff.removed += count - current;
    }
    for (const auto& [key, count] : newCounts) {
        const auto found = oldCounts.find(key);
        const std::size_t previous = found == oldCounts.end() ? 0 : found->second;
        if (count > previous) diff.added += count - previous;
    }
    const std::size_t total = std::min(m_latestSnapshot.size(), level.objects.size());
    diff.unchanged = total > diff.added + diff.removed ? total - diff.added - diff.removed : 0;
    return diff;
}

bool SnapshotManager::hasSnapshot() const {
    return m_hasSnapshot;
}

std::size_t SnapshotManager::snapshotCount() const {
    return m_snapshotCount;
}

} // namespace Veyron
