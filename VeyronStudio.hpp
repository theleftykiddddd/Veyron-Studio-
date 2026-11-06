#pragma once

#include <cstddef>
#include <cstdint>
#include <deque>
#include <map>
#include <string>
#include <vector>

namespace Veyron {

struct LevelObject {
    int objectID = 0;
    int groupID = 0;
    int colorID = 0;
    double x = 0.0;
    double y = 0.0;
    bool isTrigger = false;
};

struct LevelData {
    std::string name;
    std::vector<LevelObject> objects;
};

enum class DiagnosticSeverity {
    Info,
    Warning,
    Error
};

struct Diagnostic {
    DiagnosticSeverity severity = DiagnosticSeverity::Info;
    std::string code;
    std::string message;
};

struct FrameAnalysis {
    std::size_t inputCount = 0;
    int smallestGap = 0;
    int largestGap = 0;
    double averageGap = 0.0;
    std::size_t oneFrameGaps = 0;
    std::size_t twoFrameGaps = 0;
};

struct ProfilerStats {
    std::size_t samples = 0;
    double averageFPS = 0.0;
    double lowestFPS = 0.0;
    std::size_t slowFrames = 0;
};

struct SnapshotDiff {
    std::size_t added = 0;
    std::size_t removed = 0;
    std::size_t unchanged = 0;
};

struct ProjectProfile {
    std::string name = "Untitled project";
    std::string target = "Geometry Dash 2.2091 / Windows x64";
    std::string notes;
};

class LevelDataStore {
public:
    static std::size_t countTriggers(const LevelData& level);
};

class LevelParser {
public:
    // Parses Veyron's simple fixture format, not Geometry Dash's encoded level string.
    static bool parseFixture(const std::string& text, LevelData& output, std::string& error);
};

class Diagnostics {
public:
    static std::string severityName(DiagnosticSeverity severity);
    static std::string format(const std::vector<Diagnostic>& diagnostics);
};

class LevelAnalyzer {
public:
    std::vector<Diagnostic> analyze(const LevelData& level, int sensitivity = 2) const;
};

class FPSProfiler {
public:
    void recordFrameDelta(double deltaSeconds);
    ProfilerStats getStats() const;
    void clear();

private:
    std::deque<double> m_frameDeltas;
    static constexpr std::size_t MAX_SAMPLES = 240;
};

class FrameAnalyzer {
public:
    static FrameAnalysis analyzeInputFrames(std::vector<int> frames);
};

class ColorManager {
public:
    static std::map<int, std::size_t> countUsage(const LevelData& level);
    static std::vector<int> findUnused(const LevelData& level, int firstID = 1, int lastID = 999);
};

class GroupManager {
public:
    static std::map<int, std::size_t> countUsage(const LevelData& level);
    static std::vector<int> findUnused(const LevelData& level, int firstID = 1, int lastID = 999);
};

class SnapshotManager {
public:
    void takeSnapshot(const LevelData& level);
    SnapshotDiff compareWithLatest(const LevelData& level) const;
    bool hasSnapshot() const;
    std::size_t snapshotCount() const;

private:
    std::vector<LevelObject> m_latestSnapshot;
    std::size_t m_snapshotCount = 0;
    bool m_hasSnapshot = false;
};

class ProjectManager {
public:
    const ProjectProfile& getProfile() const;
    void setProjectName(std::string name);
    void setNotes(std::string notes);

private:
    ProjectProfile m_profile;
};

class StructureVisualizer {
public:
    static std::string buildSummary(const LevelData& level);
};

class StudioManager {
public:
    static StudioManager& get();
    LevelData makeDemoLevel() const;
    std::string getVersionLabel() const;

private:
    StudioManager() = default;
};

} // namespace Veyron
