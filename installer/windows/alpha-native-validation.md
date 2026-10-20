# Windows native validation

This port is tested against GD 2.2091 EXE SHA-256
`4eee06d5c2e2213b4ba2b1f6233dd426bbdd0e9b29d7c375e499516b6f8f69d5`
and engine SHA-256
`79c2f6567e5851225520803f460c6c5f6f49a2eac9afb8f3bd88d4c5697bef4e`.

`cmake/AlphaWindowsAddresses.cmake` overlays verified addresses, class layouts
and interfaces on the pinned upstream declarations. It verifies each expected
source declaration before patching the build copy, or accepts the exact same
correction when upstream already contains it. Game and engine RVAs use their
respective modules. The remaining 67 Windows address placeholders are in
GeometryDash.bro; the engine declarations have no remaining Windows address
placeholders. This count does not establish completeness of class layouts or
SDK implementations. Some removed native Windows classes, including
MoreOptionsLayer, have no usable Windows implementation in the current game.

The integrated upstream v6 revision is
`26cd9ca` and the bindings revision is
`d6f3312f0886b2c285fd0d2593ebabdc03b6ca0c`. The generated Windows build has
5,449 populated native wrappers, all pointing into executable sections of
their respective tested modules. That range audit does not verify semantics.

The updated upstream declarations include the current gameplay-layer state
and additional editor-window functions. Removed Windows classes such as
EditorOptionsLayer, GameOptionsLayer and GJWriteMessagePopup are marked missing.
This prevents their old declarations from masquerading as native Windows APIs;
it does not port mods that depend on those interfaces.

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
64 checks cover native constructors, level/list cache fixtures, fresh/expired
timers, easing, integer merges, robot creation, lightning drawing and properties,
invalid PVR header rejection, settings, version gates and asynchronous saving.
It requires normal shutdown and rejects new crash reports. Tests restore
temporary manager fields before saving. A loader test token is persisted in
the private profile. These checks do not exercise every game or mod feature.

GJRequestCell uses its native init at 0x158d20, with a score pointer at 0x230,
toggler at 0x238 and request-status flag at 0x240. Its former popup callback
slots and request-popup actions are absent from the current Windows interface;
they are excluded rather than replaced with unrelated addresses. A canary
fixture reproduced writes outside the cell before restoring its native init.

GJUserScore allocates 0x3b8 bytes; the previous SDK reserved only 0x398.
Social links start at 0x1e0, friend-request status is at 0x2f8 and selection
state is at 0x31d. The native dictionary factory is 0x26c000. Tests check these
positions, construction/destruction boundaries, long strings and native row
selection using dictionary data. Normal C++ destruction releases its strings
and CCNode base without calling the deleting destructor with missing flags.

Additional fixtures exercise achievement/download lookups, username and ID
caches, coin/completion namespaces, color interpolation, hue matrices, animation
priority, static editor offsets and scroll indicator placement. The native
GameLevelManager account-name map is exposed at 0x440 with its tested layout.

FMODAudioEngine's native constructor is 0x5bf90, with allocation size 0x8a8;
the previous SDK address constructed a different class. Its audio state starts
at 0x290, and its channel maps are at 0x668 and 0x730. The value-class
FMODAudioState initializes its containers through normal C++ construction and
zeros its timing fields, matching native 0x5bdc0 without constructing containers
twice. Audio tests include lookup misses and channel/effect stopping.

The SDK SpriteAnimationManager constructor previously left m_animateDict
uninitialized. A native create/destroy fixture reproduced a CCObject::release
access violation; initializing the owned pointer fixes that same fixture.
CCSpriteWithHue uses ordinary base destruction because its old Windows entry
was a deleting destructor requiring an additional flags argument. Its matrix
test constructs and destroys an actual SDK object.

EditorUI::offsetForKey is static in this game version: native 0x19f3e0 receives
the result buffer and object ID, whereas the older version also received this.
The generated API and tests use the current calling convention.

The native PagedListPopup base is 0x308 bytes and its requestPage entry is
pure virtual. VirtualRowSource likewise requires concrete implementations of
its count, height and three-argument mount methods. GJOptionsLayer is 0x470
bytes; native fixtures cover its constructor, page/group arrays and particle
record parsing.

New GameLevelManager delegate slots are interleaved with existing callbacks,
rather than appended at the end. Level upload is at 0x578, user info at 0x5c8,
and admin callbacks at 0x568. Four queue-mute/unmute methods precede user
history in CommentAdminDelegate. Offline tests drive the actual game's HTTP
dispatcher and verify receiver delivery, including both success/failure queue
paths. The corrected Windows GJHttpType enum is overlaid without changing
other platforms' enum values or modifying the fetched bindings repository.

The native keyboard manager has a pure actionsForKey vector query at slot
0x48 before handleGlobalAction at 0x50. KeybindSource has a virtual destructor,
four pure operations and isLevelSet. KeybindSet owns a map<int, set<int>> at
0x8 and a changed flag at 0x18, with total size 0x20. Tests bind/unbind isolated
chords, query both directions and dispatch through the keyboard base interface.

The image-button sizing routine requires an image-button instance. Its fixture
uses the same top-sprite setup as native callers. UploadActionPopup and
SetTextPopup factories are exercised without network requests. The latter's
value is at 0x2b8 and callback receiver at 0x2f0, with allocation size 0x2f8.
Its SDK constructor preserves the native defaults and ordinary C++ destruction
cleans up its string without calling a deleting destructor with missing flags.
Template-browser and guideline constructors also have native lifetime fixtures.

MusicDownloadManager's native allocation is 0x3d0. Its tag map is at 0x1d8,
and its resource SFX/song sets are at 0x208/0x248. Interleaved new native
storage previously left the SDK's library fields and resource sets at incorrect
offsets. Tests cover native construction/destruction, restored singleton state,
resource-cache hits and missing local files. Local-level ordering is checked
with an isolated three-level array. The text-popup fixture now also delivers
its value through the actual native close callback.

The native audio start routine resumes the FMOD mixer and clears the stopped
flag only on success. Tests suspend/resume the private game's mixer, verify
FMOD_OK and check a repeated start. The current engine has a suspended-global
flag at 0x271 and a vector<int> of suspended channels at 0x278; its
resumeSuspendedAudio routine clears that vector and flag after processing it.
This storage is represented directly instead of hidden in padding.

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

The profile-import tool was additionally exercised with the 38 copied user mod
packages, configuration, mod resources and private legacy saves: 136 files
were transferred and all 38 package hashes matched. This verifies data transfer,
not full behavioral compatibility in the new game process.

Profile-import tests on PowerShell 5.1 and 7 additionally inject a destination
write failure after copying begins. Existing destination files are restored,
new destination files are removed and source files remain unchanged.
