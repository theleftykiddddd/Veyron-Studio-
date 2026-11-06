#include "VeyronStudio.hpp"

#include <cassert>
#include <cmath>
#include <iostream>

int main() {
    using namespace Veyron;

    LevelData level;
    std::string error;
    const std::string fixture =
        "# Veyron fixture\n"
        "name=Core Test\n"
        "object=1,2,3,0,0,0\n"
        "object=901,2,3,10,0,1\n";
    assert(LevelParser::parseFixture(fixture, level, error));
    assert(level.name == "Core Test");
    assert(level.objects.size() == 2);
    assert(LevelDataStore::countTriggers(level) == 1);

    const auto frameStats = FrameAnalyzer::analyzeInputFrames({10, 11, 20, 22, 35});
    assert(frameStats.inputCount == 5);
    assert(frameStats.smallestGap == 1);
    assert(frameStats.largestGap == 13);
    assert(frameStats.oneFrameGaps == 1);
    assert(frameStats.twoFrameGaps == 1);

    FPSProfiler profiler;
    profiler.recordFrameDelta(1.0 / 60.0);
    profiler.recordFrameDelta(1.0 / 30.0);
    const auto fps = profiler.getStats();
    assert(fps.samples == 2);
    assert(fps.slowFrames == 1);
    assert(std::abs(fps.averageFPS - 45.0) < 0.01);

    const auto groupUsage = GroupManager::countUsage(level);
    const auto colorUsage = ColorManager::countUsage(level);
    assert(groupUsage.at(2) == 2);
    assert(colorUsage.at(3) == 2);

    SnapshotManager snapshots;
    snapshots.takeSnapshot(level);
    auto changed = level;
    changed.objects.push_back({44, 4, 4, 20, 0, false});
    const auto diff = snapshots.compareWithLatest(changed);
    assert(diff.added == 1);
    assert(diff.removed == 0);

    const auto diagnostics = LevelAnalyzer().analyze(level);
    assert(!diagnostics.empty());
    assert(!StructureVisualizer::buildSummary(level).empty());

    LevelData invalid;
    assert(!LevelParser::parseFixture("object=1,2,3,0,0,0\n", invalid, error));
    assert(!error.empty());
    const auto unusedGroups = GroupManager::findUnused(level, 1, 3);
    assert(unusedGroups.size() == 2);
    const auto unusedColors = ColorManager::findUnused(level, 1, 3);
    assert(unusedColors.size() == 2);

    ProjectManager projects;
    projects.setProjectName("Core Test Project");
    projects.setNotes("Verified by unit tests.");
    assert(projects.getProfile().name == "Core Test Project");
    assert(projects.getProfile().notes == "Verified by unit tests.");

    std::cout << "Veyron core tests passed.\n";
    return 0;
}
