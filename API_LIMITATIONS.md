# Compatibility and limitations

- Target platform: Windows x64.
- Target Geometry Dash: 2.2091.
- Target Geode SDK: v6.0.0-alpha.1 (pre-release SDK for the 2.209x game line).
- The mod currently hooks the main menu only; it does not hook or read editor object storage.
- Diagnostic, frame, ID and snapshot algorithms operate on `Veyron::LevelData` supplied by code. The dashboard uses a clearly labelled sample dataset.
- Direct GPU-cost estimation, real FPS sampling from the game loop, GD level-string decoding, macro import/export and editor rollback are not implemented.
