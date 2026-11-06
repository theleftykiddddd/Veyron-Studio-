# Upload the complete Veyron Studio project to GitHub

## Recommended: GitHub Desktop

1. Download and extract this ZIP.
2. In GitHub Desktop, open the existing `Veyron-Studio` repository with **Repository > Show in Explorer**.
3. Close Geometry Dash and any running build.
4. Copy the *contents* of the extracted `Veyron-Studio-1.1.0` folder into the repository folder and allow Windows to replace matching files.
5. Confirm that `.github/workflows/build.yml` exists. If Windows hides `.github`, enable **View > Show > Hidden items** in File Explorer.
6. In GitHub Desktop, review the changed files, enter a commit message such as `Update Veyron Studio 1.1.0`, click **Commit to main**, then **Push origin**.
7. Open the repository's **Actions** tab and select **Build Veyron Studio (Windows / GD 2.2091)**.

## Important folder layout

The workflow must be located at the repository root under:

```text
.github/workflows/build.yml
```

Do not place it under another nested project folder. The repository root should also contain `CMakeLists.txt` and `mod.json`.

## Download the result

After a successful workflow run, open its summary and download the artifact named:

`Veyron-Studio-Windows-GD-2.2091`

The artifact contains the compiled `.geode` package. A red run means the SDK or source still needs a build-specific fix; the standalone core tests passing do not guarantee the Geode UI compiled.
