cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

project(VeyronStudio VERSION 1.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

option(VEYRON_BUILD_TESTS "Build Veyron Studio's standalone core tests" OFF)

# Geode's official template uses add_library + setup_geode_mod.
# The build-geode-mod GitHub Action provides GEODE_SDK for us.
if (DEFINED ENV{GEODE_SDK} AND NOT "$ENV{GEODE_SDK}" STREQUAL "")
    file(GLOB_RECURSE VEYRON_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    )

    add_library(VeyronStudio SHARED ${VEYRON_SOURCES})
    target_include_directories(VeyronStudio PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
    )

    add_subdirectory("$ENV{GEODE_SDK}" "${CMAKE_CURRENT_BINARY_DIR}/geode")
    setup_geode_mod(VeyronStudio)
elseif (NOT VEYRON_BUILD_TESTS)
    message(FATAL_ERROR "GEODE_SDK is not set. Build with the Geode CLI / GitHub Action, or set VEYRON_BUILD_TESTS=ON for standalone core tests.")
else()
    message(STATUS "GEODE_SDK not found: building standalone core tests only.")
endif()
if (VEYRON_BUILD_TESTS)
    enable_testing()
    add_executable(veyron-core-tests
        tests/core_tests.cpp
        src/core/LevelData.cpp
        src/core/LevelParser.cpp
        src/analyzer/Diagnostics.cpp
        src/analyzer/LevelAnalyzer.cpp
        src/performance/FPSProfiler.cpp
        src/frames/FrameAnalyzer.cpp
        src/ids/ColorManager.cpp
        src/ids/GroupManager.cpp
        src/version/SnapshotManager.cpp
        src/visualizer/StructureVisualizer.cpp
        src/core/ProjectManager.cpp
        src/core/StudioManager.cpp
    )
    target_include_directories(veyron-core-tests PRIVATE include)
    add_test(NAME veyron-core-tests COMMAND veyron-core-tests)
endif()
