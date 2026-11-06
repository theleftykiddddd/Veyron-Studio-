#include "VeyronStudio.hpp"

#include <algorithm>
#include <cmath>

namespace Veyron {

void FPSProfiler::recordFrameDelta(double deltaSeconds) {
    if (!std::isfinite(deltaSeconds) || deltaSeconds <= 0.0 || deltaSeconds > 1.0) return;
    m_frameDeltas.push_back(deltaSeconds);
    while (m_frameDeltas.size() > MAX_SAMPLES) {
        m_frameDeltas.pop_front();
    }
}

ProfilerStats FPSProfiler::getStats() const {
    ProfilerStats stats;
    stats.samples = m_frameDeltas.size();
    if (m_frameDeltas.empty()) return stats;

    double totalFPS = 0.0;
    stats.lowestFPS = 1000000.0;
    for (const double delta : m_frameDeltas) {
        const double fps = 1.0 / delta;
        totalFPS += fps;
        stats.lowestFPS = std::min(stats.lowestFPS, fps);
        if (delta > (1.0 / 45.0)) ++stats.slowFrames;
    }
    stats.averageFPS = totalFPS / static_cast<double>(m_frameDeltas.size());
    return stats;
}

void FPSProfiler::clear() {
    m_frameDeltas.clear();
}

} // namespace Veyron
