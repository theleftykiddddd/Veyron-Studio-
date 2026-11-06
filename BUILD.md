# GitHub Actions build

The only supported CI target in this repository is Windows x64 for Geometry Dash 2.2091.

The workflow must remain at:

```text
.github/workflows/build.yml
```

Do not move `build.yml` to the repository root. The build action reads the exact SDK version from `mod.json` because the workflow uses `sdk: given`.

After a successful run, download the artifact named `Veyron-Studio-Windows-GD-2.2091` from the run summary.
