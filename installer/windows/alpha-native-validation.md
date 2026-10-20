# Windows native validation

This port is tested against GD 2.2091 EXE SHA-256
`4eee06d5c2e2213b4ba2b1f6233dd426bbdd0e9b29d7c375e499516b6f8f69d5`
and engine SHA-256
`79c2f6567e5851225520803f460c6c5f6f49a2eac9afb8f3bd88d4c5697bef4e`.

`cmake/AlphaWindowsAddresses.cmake` resolves 30 of the previous 209 Windows
placeholders, adds the native GameLevelManager constructor and corrects nine
previously populated addresses. It verifies each expected source declaration
before patching the build copy. Game and engine RVAs use their respective
modules. The remaining 179 Windows placeholders are in GeometryDash.bro;
the engine declarations have no remaining Windows placeholders. This count
does not establish completeness of class layouts or SDK implementations.

The integrated upstream v6 revision is
`26cd9ca` and the bindings revision is
`19c6429ab446054d98d84393378e9675f4d80607`. The generated Windows build has
5,228 populated native wrappers, all pointing into executable sections of
their respective tested modules. That range audit does not verify semantics.

The GameLevelManager allocation is 0x690 bytes. Constructor initialization,
the GLM serialization keys and native cache lookups establish its known field
offsets. Unknown storage is preserved as padding. Its Windows singleton uses
the native constructor to initialize the containers in that storage. GJLevelList
is 0x250 bytes, with its native unique ID at 0x24c.

Use a separate game copy with matching loader resources and run:

```powershell
./Test-AlphaNative.ps1 -Executable 'C:/path/to/GeometryDashPrivate.exe'
```

The script runs strict and explicitly experimental compatibility modes. Its
17 checks cover native constructors, level/list cache fixtures, fresh/expired
timers, easing, integer merges, robot creation, lightning drawing and properties,
invalid PVR header rejection, settings, version gates and asynchronous saving.
It requires normal shutdown and rejects new crash reports. Tests restore
temporary manager fields before saving. A loader test token is persisted in
the private profile. These checks do not exercise every game or mod feature.

## Existing Geode 5 mods

Prism 1.10.9 omits the required default for its custom array setting. The repair
script adds an empty array to that manifest. It verifies SHA-256 of every other
archive entry, replaces the package only after validation and retains the
original package under `geode/alpha-backups`. It skips other Prism versions.

Eclipse 1.9.4 and BetterInfo 4.4.11 report missing initial theme/cache files in
a fresh profile. The same script initializes only absent files, using Eclipse's
bundled desktop theme and BetterInfo's empty cache schema. Existing theme and
cache files are preserved.

Close the selected private copy, then run:

```powershell
./Repair-AlphaMods.ps1 -GameDirectory 'C:/path/to/private-game' `
    -SaveDirectory 'C:/Users/name/AppData/Local/GeometryDashLegacy'
```

`Test-AlphaModRepairs.ps1` verifies archive preservation, backups, idempotence,
existing user data and rejection of unexpected manifests on PowerShell 5.1/7.

The loader also fixes a reproducible shutdown failure: a native save after the
Exiting event invoked BetterInfo's DataSaved listener after the async runtime
had stopped. Normal Windows teardown now saves mod data before Exiting and
does not emit another mod save during native teardown. Async cache writes
complete before runtime shutdown.

`legacy-save-order.patch` backports that correction to official Geode 5.11.2
source commit `bc3fea78a5748427642c22ce01edb0b3174f29ae`. Apply it in a separate
checkout and build with the usual Windows CMake workflow:

```powershell
git apply --ignore-space-change /path/to/legacy-save-order.patch
```

This creates an unofficial patched Geode 5 loader, not a Geode 6 translation
layer. To register a separately verified patch build, pass its SHA-256 explicitly
to `Initialize-AlphaRuntimes.ps1 -LegacyLoaderSha256 <sha256>`. The default
continues to require the official loader. Runtime fingerprints remain enforced.

Local validation loaded the 38 copied user packages plus a test helper in
GD 2.2081 with the patched Geode 5 loader. Their Geode imports resolved; the
game reached its main menu and shut down without ERROR log entries. Silicate
retained its declared xDBot/CBF Extrapolate conflicts. Empty-keybind warnings
from Eclipse remain. Startup validation does not prove every mod feature works
or that Geode 5 DLLs can run inside the GD 2.2091 process.
