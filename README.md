# Veyron Studio

**Veyron Studio 1.1.0** is a Windows x64 Geode mod prototype targeting Geometry Dash **2.2091** and Geode **v6.0.0-alpha.1**.

It adds a compact animated dashboard to the main menu. The dashboard has tabs for Overview, Analyzer, Frame Lab, ID Map, Snapshots, Visualizer and Settings.

## Included in this build

- Animated main-menu entry point and dashboard panel.
- Tabbed UI with a subtle open transition and title animation.
- Standalone object-record diagnostics (ID range and coordinate sanity checks).
- Frame-index gap analysis with minimum, maximum, average, one-frame and two-frame gaps.
- Group and color usage counters plus unused-ID queries in the core API.
- In-memory snapshots and added/removed record comparisons.
- A simple horizontal structure summary.
- A small fixture parser for Veyron's own text test format.
- Settings for UI animation, tool hints and diagnostic sensitivity.
- GitHub Actions workflow that builds a Windows x64 `.geode` artifact.

## Important scope notes

This is a working foundation, not yet a full editor-integrated IDE. The dashboard's Analyzer, Frame Lab and other pages currently demonstrate the core engines with an explicitly labelled bundled demo dataset. The mod does **not** yet read the currently open Geometry Dash editor level, capture live inputs, import/export macros, persist snapshots to disk, or modify/rollback real level objects.

The Veyron fixture parser is a small developer/test format. It is **not** a parser for Geometry Dash's encoded level string.

## Build locally

Use the Geode CLI version compatible with the SDK declared in `mod.json`, then run:

```sh
geode build
```

The project uses Geode's official CMake setup (`add_library` + `setup_geode_mod`).

## Build on GitHub

The workflow is located at `.github/workflows/build.yml` and uses `geode-sdk/build-geode-mod` with `sdk: given` so it follows the exact Geode version in `mod.json`.

1. Push the complete repository to `main`.
2. Open **Actions**.
3. Select **Build Veyron Studio (Windows / GD 2.2091)**.
4. Open the successful run and download `Veyron-Studio-Windows-GD-2.2091` from Artifacts.

The workflow targets Windows x64 only. Geode v6 is currently an alpha SDK for GD 2.209x, so a successful CI build still needs to be tested in the user's own GD 2.2091 + Geode installation.

## Standalone core tests

The logic-only modules can be tested without Geometry Dash or Geode:

```sh
cmake -S . -B core-build -DVEYRON_BUILD_TESTS=ON
cmake --build core-build --target veyron-core-tests
ctest --test-dir core-build --output-on-failure
```

These tests validate the parser, diagnostics, frame analyzer, profiler, ID usage, snapshot diff and structure summary. They do not compile the Geode UI layer.
