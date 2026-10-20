# Separate Windows runtimes

The Alpha launcher starts one complete game installation at a time. Geode 5
mods can use Geometry Dash 2.2081 and official Geode 5.11.2, while the current
port stays in its own Geometry Dash 2.2091 installation. The launcher does not
translate old DLLs or run two game versions inside one process.

## Prepare the installations

Keep the entire game and engine files together in each directory. Use different
EXE names, such as `GeometryDashAlpha.exe` and `GeometryDashLegacy.exe`, so their
save directories under `%LOCALAPPDATA%` remain separate. Existing saves are not
moved or converted. Mods belong in each installation's `geode/mods` directory.

For a Steam account that owns the game, download the older Windows depot through
the Steam console:

```text
download_depot 322170 322171 3816559102876907245
```

Wait for Steam's completion message, then copy the whole downloaded depot into
a separate directory. Its usual location is
`Steam/steamapps/content/app_322170/depot_322171`. Rename the copied EXE there.
Install the DLLs from the official
[Geode 5.11.2 Windows ZIP](https://github.com/geode-sdk/geode/releases/download/v5.11.2/geode-v5.11.2-win.zip)
beside that EXE, and extract its
[resources](https://github.com/geode-sdk/geode/releases/download/v5.11.2/resources.zip)
into `geode/resources/geode.loader`. Use the current Alpha DLLs and their
matching resources only in the 2.2091 installation.

The initializer verifies the tested game/engine hashes and the official legacy
loader. It records fingerprints of all three binaries for each profile:

```powershell
./Initialize-AlphaRuntimes.ps1 `
    -CurrentExe 'C:/Games/GD Alpha/GeometryDashAlpha.exe' `
    -LegacyExe 'C:/Games/GD Legacy/GeometryDashLegacy.exe' `
    -Output './alpha-runtimes.json'
```

Keep the generated configuration beside `Start-Alpha.cmd`, or supply its path
to the PowerShell script. The paths can be absolute or relative to the JSON
file. The initializer refuses an existing output unless `-Force` is provided.
After deliberately updating a runtime, verify its files before regenerating
the fingerprints. Game files, personal configurations and third-party mods are
not included in this repository.

## Start a profile

Double-click `Start-Alpha.cmd`. Auto mode chooses the only profile containing
mod packages. If both profiles contain mods, choose explicitly; if neither
contains mods, it prefers Current.

```powershell
./Start-Alpha.ps1 -Config './alpha-runtimes.json' -Runtime Legacy
./Start-Alpha.ps1 -Config './alpha-runtimes.json' -Runtime Current
./Start-Alpha.ps1 -Config './alpha-runtimes.json' -Runtime Legacy -SafeMode
./Start-Alpha.ps1 -Config './alpha-runtimes.json' -Plan
```

`-Plan` checks the configuration and prints the selection without starting a
game. Before starting, the launcher checks file fingerprints, resource version,
mod SDK major and declared Windows game version. Geode performs the remaining
dependency and version checks. Safe mode skips mod-manifest checks and starts
Geode without loading external mods, including when a package is corrupt.

`-Runtime Current -Experimental` retains the existing Alpha version override.
It permits attempts to load mismatched mods and does not establish compatibility.
The legacy loader does not provide that override. Neither the launcher
nor the separate legacy installation completes the unfinished 2.2091 port.

## Validation

Run `Test-AlphaLauncher.ps1` with Windows PowerShell 5.1 and PowerShell 7. Its
fixtures exercise selection, profile isolation, corrupt packages, fingerprints,
safe mode and explicit experimental arguments without launching a game.

The tested Steam manifest runs as GD 2.2081 with official Geode 5.11.2. All
38 installed test packages had their required Geode exports and reached the
loader's mod-loading stage. Process inspection confirmed 37 mod DLLs loaded.
Silicate was blocked by its declared incompatibilities with xDBot and CBF
Extrapolate. That initial startup also reported third-party setting, theme and
cache errors. The [repair tools and save-order backport](alpha-native-validation.md)
were subsequently tested with all 38 copied packages: main-menu startup and
shutdown completed without ERROR entries. Existing Eclipse empty-keybind
warnings remain. This does not prove every mod feature works. Preserve the
desired enabled mod selection when setting up a new profile.
