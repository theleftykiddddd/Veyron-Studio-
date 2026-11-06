#include "VeyronStudio.hpp"

#include <algorithm>
#include <numeric>

namespace Veyron {

FrameAnalysis FrameAnalyzer::analyzeInputFrames(std::vector<int> frames) {
    FrameAnalysis result;
    frames.erase(std::remove_if(frames.begin(), frames.end(), [](int frame) { return frame < 0; }), frames.end());
    std::sort(frames.begin(), frames.end());
    frames.erase(std::unique(frames.begin(), frames.end()), frames.end());
    result.inputCount = frames.size();
    if (frames.size() < 2) return result;

    std::vector<int> gaps;
    gaps.reserve(frames.size() - 1);
    for (std::size_t i = 1; i < frames.size(); ++i) {
        const int gap = frames[i] - frames[i - 1];
        gaps.push_back(gap);
        if (gap == 1) ++result.oneFrameGaps;
        if (gap == 2) ++result.twoFrameGaps;
    }
    result.smallestGap = *std::min_element(gaps.begin(), gaps.end());
    result.largestGap = *std::max_element(gaps.begin(), gaps.end());
    const long long total = std::accumulate(gaps.begin(), gaps.end(), 0LL);
    result.averageGap = static_cast<double>(total) / static_cast<double>(gaps.size());
    return result;
}

} // namespace Veyron
