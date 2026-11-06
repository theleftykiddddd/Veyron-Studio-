# Architecture

- `main.cpp`: Geode `MenuLayer` hook, animated entry point and tabbed dashboard popup.
- `include/VeyronStudio.hpp`: pure C++ data structures and module contracts. It does not include Geode headers so the core can be tested independently.
- `src/core`: fixture parser, demo data and project profile helpers.
- `src/analyzer`: deterministic object-record checks and diagnostics formatting.
- `src/frames`: frame-index gap analysis.
- `src/performance`: rolling frame-time profiler.
- `src/ids`: group and color usage maps.
- `src/version`: in-memory snapshot comparison.
- `src/visualizer`: section-based structure summary.

The current UI uses a bundled demo dataset for demonstrations. An editor adapter will be required before any displayed report can claim to describe the level currently open in Geometry Dash.
