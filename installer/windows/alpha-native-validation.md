# Windows native validation

This port is tested against GD 2.2091 EXE SHA-256
`4eee06d5c2e2213b4ba2b1f6233dd426bbdd0e9b29d7c375e499516b6f8f69d5`
and engine SHA-256
`79c2f6567e5851225520803f460c6c5f6f49a2eac9afb8f3bd88d4c5697bef4e`.

`cmake/AlphaWindowsAddresses.cmake` overlays verified addresses, class layouts
and interfaces on the pinned upstream declarations. It verifies each expected
source declaration before patching the build copy, or accepts the exact same
correction when upstream already contains it. Game and engine RVAs use their
respective modules. The remaining 53 Windows address placeholders are in
GeometryDash.bro; the engine declarations have no remaining Windows address
placeholders. This count does not establish completeness of class layouts or
SDK implementations. Some removed native Windows classes, including
MoreOptionsLayer, have no usable Windows implementation in the current game.

The integrated upstream v6 revision is
`4457dd8` and the bindings revision is
`9a9af08340e446a970d434502f84cc5de52251e5`. The generated Windows build has
5,446 populated native wrappers, all pointing into executable sections of
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
93 checks cover native constructors, level/list cache fixtures, fresh/expired
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

GJRewardItem::rewardItemToStat uses native 0x2f9190; the fixture covers all
fifteen enum values and invalid values. GameLevelManager::getStoredUserList
uses 0x257b80 and the shared cache dictionary at 0x480. Windows UserListType
includes Following (3) and Muted (4), alongside Friends (0) and Blocked (1).
Tests verify lookups and selective resets across those four categories.

LevelEditorLayer reserves 0x3e08 bytes, rather than the previous SDK's 0x3cf8.
The native constructor is 0x3ec2f0. Its locked-layer count is at 0x3b88, saved
positions at 0x3b90, removal flag at 0x3c98 and delayed-spawn vectors at
0x3cc0/0x3cd8. Unknown session and tail storage is preserved. A fixture checks
the layout and construction/destruction boundaries with allocated position,
bit-vector, hash-map and string data. Native removeObject uses 0x3f21b0 and
getLastObjectX uses 0x3fd290; their old Windows entries pointed elsewhere.
Fresh-editor fixtures also initialize and destroy temporary local levels with
and without the editor UI. Another fixture creates a block, unregisters and
registers its special state through native removeSpecial (0x3f5290) and
addSpecial, checks the last-object position and removes it again. Existing
local-level arrays and manager fields are restored before saving. Library
requests are suppressed in the fixture. These checks do not establish that
every editor feature works.

EditorUI::getXMin uses 0x1bf270; its native Windows body does not use the
integer argument. Tests verify that translating the object layer does not
change the result and that the native temporary-position change is restored.
GJMultiplayerManager::removeDLFromActive uses 0x390a10 and its dictionary at
0x140. Tests remove short and long tags, allow an absent tag and preserve an
unrelated entry. The test uses no lobby or network request.

NumberInputLayer reserves 0x300 bytes instead of the previous SDK's 0x2d8.
Its title label is at 0x2a0, display prefix at 0x2b8, input string at 0x2d8
and delegate at 0x2f8. Native construction is 0x4a1060, digit/backspace actions
use 0x4a1820 and state updates use 0x4a1990. The additional virtual onDone
slot at 0x438 uses 0x4a1b10. Tests exercise long-string destruction, editing,
length bounds, label text and a temporary completion receiver.
ParentalGateLayer reserves 0x308 bytes and its native factory is 0x4d3ba0;
only construction is tested, without submitting an answer or changing settings.

CCCounterLabel::setTargetCount schedules native updateCounter at 0x46ee0.
The fixture checks increasing/decreasing counts, completed counters and an
explicitly disabled counter. GJBaseGameLayer::updateCounters uses 0x33cb90 and
the label map at 0x1118. An actual counter object created in a temporary editor
checks text updates, the native 999999999 upper limit, negative values, absent
item IDs and the label's update lock.

A local compiled-member-pointer audit compared 2,116 known mapped virtual
methods across 461 classes with native RTTI tables. All matched. An earlier
audit identified 84 slot or base-offset disagreements that are now corrected.
The smaller current sample reflects upstream removing 33 redundant or obsolete
declarations, with ReportQueuePopup::rebuildContent added to the audit.
GManager's save callbacks precede its
load callbacks; the gameplay hitbox callbacks precede section/group methods.
Custom SFX and table-cell callbacks use the current native order and bool/int
return types. Runtime fixtures check derived editor dispatch and typed delegate
callbacks. The audit excludes unmapped methods, overloads and classes without
available native RTTI; it does not establish complete SDK compatibility.

TabbedPopupLayer reserves 0x430 bytes, GameSettingsLayer 0x4a8,
LevelBrowserLayer 0x348 and LevelListLayer 0x408. Their inherited delegate
offsets follow these native base allocations. The settings layer's game pointer
is at 0x438. Opaque tab/browser state remains allocated without guessing its
container element types. TabbedPopupLayer and GameSettingsLayer invoke their
native deleting-destructor entry points with flags zero and follow the SDK's
destructor-lock protocol to prevent a second native destruction. Fixtures
exercise SDK-derived tab/browser construction and destruction boundaries, plus
native game-settings creation, tab selection and dropdown delegate dispatch.
Generated state-header dependencies are included explicitly so that changing
unity source order does not conceal missing map, array or cocos type includes.

The newer upstream manager fields replace the earlier opaque cache padding.
GameLevelManager's daily state still needs four bytes before m_dailyTimeLeft
at 0x3bc; removing the duplicated content-status integer keeps the native
current-ID fields at 0x3e0..0x3ec and content-status flag at 0x3f0. Existing
cache/delegate fixtures verify the retained offsets. Upstream's new browser
pointer fields and paged-popup storage are used without duplicate padding.
TabbedPopupLayer's Windows tail remains the verified 0x48 bytes.

A separate compiled-size comparison covers all 548 upstream Windows references.
It reports 142 differences requiring native verification; these are not a list
of proven defects because some reference sizes are inconsistent with native
construction. For example, GameManager's fields start before the reference's
claimed GManager size. No automatic padding is applied from this comparison.
This remaining investigation is another reason the port is not a full release.

The upstream Popup touch-priority correction registers (2, 0). A fixture checks
the target-priority change, absence of a new owned touch level and restoration
on destruction. ParentalGateLayer's SDK constructor initializes the native
solution integer after the corrected NumberInputLayer base; a guarded allocation
checks that construction/destruction stay within its 0x308 bytes.
The release placeholder counter ignores comments and quoted strings. Seven
CMake fixtures verify that commented-out old declarations do not block releases
and that real unresolved declarations, including after URL defaults, still count.

GJLevelList::reorderLevel uses 0x272d00, also called by native list-row actions;
tests cover moves in both directions, clamped positions, absent IDs, one-item
and empty lists, and the serialized level order. parseListLevels uses 0x2728a0,
as called by native coder loading, replacing an unrelated shader address.
Parsing tests cover short/long strings, replacement, duplicates and empty input.

PagedListPopup::loadPage (0xc3610) invokes requestPage and clearPageContent;
showLoading (0xc3770) replaces the owned indicator. Fixtures verify negative-page
rejection, successful dispatch, page state and repeated indicator replacement.
ReportQueuePopup::rebuildContent uses the native slot 0x420 at 0xc95b0. The full
report-queue UI and server requests are not exercised by this fixture.

FriendPickerDelegate now requires unsigned recipient counts and CCArray pointers;
all three mapped concrete native implementations use those signatures. Its
picker-state defaults return zero. IDNameBrowserDelegate likewise requires its
receiver callback. These are abstract SDK contracts, not invented native base
addresses. Typed fixture receivers verify dispatch and selection data. The
obsolete NewgroundsInfoLayer alert callback and MusicBrowser text-popup callback
are marked missing on Windows: neither has a native slot beyond 0x430 or the
corresponding delegate base. A boundary fixture verifies SDK-derived virtual
methods start at 0x438. These interface changes account for five of the seven
removed address placeholders; they do not port mods relying on removed APIs.

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
