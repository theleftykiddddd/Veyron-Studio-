#include "VeyronStudio.hpp"

namespace Veyron {

StudioManager& StudioManager::get() {
    static StudioManager instance;
    return instance;
}

LevelData StudioManager::makeDemoLevel() const {
    LevelData level;
    level.name = "Veyron Diagnostic Demo";
    level.objects = {
        {1, 1, 1, 0.0, 0.0, false},
        {901, 4, 2, 120.0, 0.0, true},
        {1815, 4, 2, 240.0, 48.0, false},
        {2067, 1004, 3, 360.0, 48.0, true},
        {211, 5, 1008, 500.0, 48.0, false},
        {1, 6, 4, 150000.0, 0.0, false}
    };
    return level;
}

std::string StudioManager::getVersionLabel() const {
    return "Veyron Studio v1.1.0";
}

} // namespace Veyron
