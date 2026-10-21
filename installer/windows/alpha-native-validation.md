# Windows native validation

This port is tested against GD 2.2091 EXE SHA-256
`4eee06d5c2e2213b4ba2b1f6233dd426bbdd0e9b29d7c375e499516b6f8f69d5`
and engine SHA-256
`79c2f6567e5851225520803f460c6c5f6f49a2eac9afb8f3bd88d4c5697bef4e`.

`cmake/AlphaWindowsAddresses.cmake` overlays verified addresses, class layouts
and interfaces on the pinned upstream declarations. It verifies each expected
source declaration before patching the build copy, or accepts the exact same
correction when upstream already contains it. Game and engine RVAs use their
respective modules. The remaining 16 Windows address placeholders are in
GeometryDash.bro; the engine declarations have no remaining Windows address
placeholders. This count does not establish completeness of class layouts or
SDK implementations. Some removed native Windows classes, including
MoreOptionsLayer, have no usable Windows implementation in the current game.

The integrated upstream v6 revision is
`fed6cfb` and the bindings revision is
`4a4044007192ae7fbbd72f0cd1e9f17094d9515b`. The generated Windows build has
5,494 populated native wrappers, all pointing into executable sections of
their respective tested modules. That range audit does not verify semantics.

The Windows engine allocates 0x348 bytes for CCParticleSystemQuad. The new
particle draw-cost field puts the base size at 0x318; the additional tail
pointer used on other platforms would exceed that native Windows allocation.
A guarded fixture initializes four particles, checks their buffers and draw
cost, destroys the object and verifies the allocation boundary. Native
UIButtonConfig::resetOneBtn at 0x275970 restores the single-button size,
position, opacity and flags, including after changing those settings.
The newer NumberInputLayer fields are retained while its onDone virtual
callback remains corrected to 0x4a1b10; 0x4a1820 handles digit input.

GJShaderState reserves 0x2d0 bytes. Native tween actions 45 and 46 store doubles
at 0x2b0 and 0x2b8, with flags following at 0x2c0. The SDK constructor initializes
all 149 members without reconstructing already live unordered maps. A guarded
fixture checks defaults, native split writes and the real tween-update path.
EnterEffectInstance is 0xe8 bytes; its native add/remove methods at
0x314490/0x3147c0 operate on the game-state channel maps. Effect IDs select
replacement/removal, while unique ID and target/center groups select refresh.
The game's replacement branch starts its move loop two elements past begin;
replacing a sole effect reproduced an access violation at 0x359f62. The
Windows loader removes replaced entries before the native function runs,
retaining native initialization and same-object refresh. Regression fixtures
cover this crash, selective removal, enter/exit isolation and invalid channels.
The enter-easing buffer generator at 0x314ac0 shares the native value vector,
index map and sample counter at 0x3100/0x3118/0x3158. Fixtures compare all 101
samples for each buffered easing type at two rates against the native easing
helper, verify sentinel results and cache reuse, and exercise SDK regeneration
with included/excluded enter-object IDs and stale cached entries.
GJMGLayer::init at 0x387b80 initializes both middleground sprite batches;
loadGroundSprites at 0x3880b0 independently grows and shrinks their tile lists.
Fixtures cover all three native middleground indices, tile positions, texture
identity, repeated resizing and a missing optional second sprite. A guarded
constructor/destructor fixture checks the 0x1e8 allocation. Its scalar deleting
destructor at 0x386c60 now receives explicit deletion flags zero.
gameEventTriggered at 0x33ac80 dispatches through TriggerEffectDelegate using
event/material/player keys and m_commandIndex for per-frame suppression.
A derived SDK fixture receives the real native spawnGroup callbacks and checks
player-specific and general entries, inactive entries, remap vectors, trigger
and control IDs, repeat suppression and dispatch on the following frame.
LevelOptionsLayer::init at 0x45d1a0 initializes the native options controls.
A fresh editor fixture checks ordinary and legacy option pages, reversible
mini-mode toggling and the tag-13 spawn-group field at 0x1c4, including integer
conversion and unrelated tags. All changed level settings belong to that
disposable editor level.
SetupTriggerPopup::onToggleTriggerValue uses 0x60f240, also referenced by its
native toggle-control builder. A derived SDK fixture creates two controls,
clicks them and checks their independent states and native virtual callbacks.
It verifies that the text-delegate guard is enabled during updates and that
both prior guard states are restored afterward.
FindObjectPopup stores a 32-bit group ID at 0x480, not a boolean. Its SDK
constructor now initializes the complete value. Windows inlines its native
window setup in EditorUI; the SDK initializer reproduces those verified
settings and calls initWindow with width 200, height 89 and window ID 3.
The return types of both onFieldEnter declarations are void. Native search,
step and options callbacks use 0x1809f0, 0x180aa0 and 0x180ad0. The SDK
selection helper calls search with direction zero, and the object-ID mode
helper toggles the same 0232 preference read by native search without changing
the group ID. A guarded fixture tests integer values larger than one byte;
a fresh editor fixture constructs the window and exercises selection and
both step directions. Its preference dictionary is isolated. The scalar
deleting destructor at 0x180260 receives explicit deletion flags zero.
Native updateExtraGameLayers at 0x32b500 copies position and both scale axes.
GJUINode::create now uses 0x673b50, whose native allocation is 0x188; the old
0x66d6e0 address created an unrelated 0x160-byte node. The 0x674070 position
helper is tested for scale/radius changes, alternate mode and a missing second
button. Alternate mode centers the first sprite at the native zero point.

GJOptionsLayer::countForPage at 0x39b760 reads m_values at 0x408; the previously
unimplemented increment helper is 0x39b850. Video and parental option pages
share 0x4cfb00, reading m_values at 0x2a8. Fixtures cover absent, signed,
malformed and independent page keys and increment through the SDK/native path.
Both SDK option constructors now initialize dictionary and button pointers.
Before that correction, exception cleanup in MoreVideoOptionsLayer released an
uninitialized m_variables pointer and crashed in CCObject::release. Guarded
construction/destruction fixtures verify initialization and balanced releases.
GameStatsManager::trySelectActivePath uses 0x2da5f0 and m_activePath at 0x3e4.
Temporary store, purchase and chest dictionaries and dictionary-mode statistics
isolate the fixture from real progression. It checks locked paths, preservation
of an unfinished selected path, replacement of completed paths, the 999/1000
completion boundary and no remaining eligible path. All manager fields are
restored before the private profile is saved.

KeybindingsLayer is 0x4a0 bytes and uses its native constructor at 0x3d56d0.
Its current settings pointer/source are at 0x430/0x438, the search input at
0x448, scroll layer/menu at 0x460/0x468, query at 0x478 and search-tab index
at 0x498. The former page integers and dictionaries are absent. Six old
Windows helpers that would access those dictionaries are excluded; removing
their countForPage placeholder does not restore that older interface.
Fixtures check guarded SDK construction/destruction with a heap-backed query,
native creation, tab selection, searching, clearing and rebuilding. Native
search deliberately selects its dedicated search tab. Saved tab/query globals
are restored and test scroll offsets are not persisted.

KeybindSettingsEntry is 0x28 bytes: title/background pointers, callback target,
an eight-byte menu selector and close-after-action flag. The native global and
level keybinding methods return a vector of these entries; their old void
declarations omitted the required result buffer. A fixture checks native and
qualified SDK return paths for Export/Import/Reset metadata without invoking
those actions. KeybindingsSettingsPopup reserves 0x2f0 bytes, with its entry
vector at 0x2a8 and pending string at 0x2c8. Its native init/entry dispatch are
0x3d8e80/0x3d93c0. The native destructor at 0x3d8e30 is called with deletion
flags zero. A guarded fixture clicks two custom test entries, rejects invalid
indices and frees a heap-backed pending string. No user bindings are imported,
exported or reset by these checks.

LevelBrowserLayer::updatePageLabel at 0x3e6a20 updates page text at 0x2a8 and
folder text at 0x2b8 from its search object. Fixtures verify one-based pages,
folder numbers, last-page-button visibility and missing optional controls.
LevelTools::posForTimeInternal at 0x4676d0 has seven native arguments. The old
eight-argument SDK overload forwards to it without the unused tail argument;
the seven-argument overload provides the actual hook signature. Empty/null
timeline fixtures check all five native speeds, platformer behavior, negative
and zero time and preservation of an untouched rotation-channel output.
A fresh editor with two real speed portals supplies a separate native-call
oracle at several times. The fixture verifies that the portals affect travel,
using their native sorted leading edges rather than sprite-center positions.
Temporary timewarp and direction globals are restored after the comparison.
EditorUI::updateZoom uses 0x1b4c00 for centered zoom, also called by the native
mouse-wheel handler, and 0x1b4c70 for an explicit point. The latter's return
type is void. A fresh editor fixture verifies both fixed world-space anchors,
restoration of the editor parent's scale, SDK zoom-button behavior and its
0.1/4.0 limits. LevelEditorLayer::updateGridLayerParent at 0x3ef7a0 moves the
grid between object and above-shader layers according to m_state.m_usesShaders
at shader offset 0x466. Its fixture checks shader absence, repeated transitions,
stable reference counts and preservation of position and Z order.
LevelTools::getLevelList returns the 43 IDs embedded in the native
GameManager::recountUserStats routine. Windows inlines the set construction,
so the SDK provides an owning unordered_set instead of exposing an interior
instruction address. The fixture checks range boundaries and independent
return values, and compares native level metadata. The progress set includes
IDs from other game editions; the Windows metadata getter falls back to
Stereo Madness for those, while providing the 22 main levels, The Challenge
and four Tower levels directly. The SDK retains all IDs from the native set.

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
152 checks cover native constructors, level/list cache fixtures, fresh/expired
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

GameObject::commonSetup uses 0x28ae70; tests check color defaults, a fresh unique
ID, reset scale and reuse of its existing base-color allocation. Native custom
color-child creation at 0x2944e0 owns the sprite at 0x380 and detail-color state
at 0x458. A fixture replaces a real sprite and verifies the old reference is
released while the detail-color allocation is reused. shouldBlendColor uses
0x28de70; reserved channels, default-channel fallback, base-color recursion and
the suppressing flag are checked outside editor preview mode.

EditorUI::canSelectObject uses 0x1c8500. In a fresh editor, isolated game-variable
settings verify object-ID, group-ID and effective base/detail color filtering.
The inspected SDK fields match the native unique ID at 0x39c, object ID at
0x40c, base/detail colors at 0x450/0x458 and group storage at 0x490/0x498.
These fixtures fail with the original unresolved wrappers and pass with the
native bindings, rather than testing only that their addresses are executable.

Native level deletion at 0x21c870 distinguishes editor levels from downloaded
levels. Fixtures use temporary local arrays, online/daily dictionaries and all
three death-data containers. They verify repeated deletion, selective cache
removal, unrelated entries and preservation of completion metadata while the
downloaded level string is discarded. No user level is passed to this test.
GameManager::completedAchievement (0x278140) applies icon and both color unlock
types from synthetic achievement definitions into a temporary settings dictionary.
Tests cover long keys, repeated application and absent definitions. ItemInfoPopup's
factory at 0x3841e0 allocates 0x2a8 and initializes actual cube, ship and color
information popups. The seven repaired APIs remove seven address placeholders.

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

A broader structural audit checks 2,302 virtual entries across 467 classes,
including unmapped methods. All have a matching native subobject offset and
in-range slot; 2,120 also match their mapped native target. This found another
base-offset error: Windows CCApplication owns an HMONITOR at 0xc8, before the
settings at 0xd0..0xd6. Including it restores the native base size of 0xd8 and
AppDelegate's CCSceneDelegate offset of 0xd8. The running-scene pointer is at
0xe8, loading flag at 0xf0 and save time at 0xf8. Tests check the live delegate
table and compare all seven settings with exported engine getters, restoring
each temporary value immediately. Four native application/scene callbacks now
have verified Windows bindings. Structural slot checks alone do not establish
the signatures or behavior of unmapped methods.

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
It reports 133 differences requiring native verification; these are not a list
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

GJEffectManager's native constructor is 0x362520 and allocation is 0x828 bytes.
Its new integer at 0x3f0 moves the item-state sets to 0x3f8/0x438, timer map
to 0x478, triggered-ID set to 0x560 and spawn vector to 0x5b0. Earlier item
count storage remains at 0x330. A layout fixture failed before this correction.
Native removeTriggeredID (0x36c850) erases the requested object/player pair;
tests preserve neighboring pairs, allow absent pairs and remove from empty sets.
The SDK storeTriggeredID implementation inserts the same unique pair, matching
the earlier native method's set-insertion behavior. Fixtures cover duplicates,
SDK hasBeenTriggered/resetTriggeredIDs and guarded native construction with
populated containers, including preservation of unrelated item-state sets.

OptionsObject's native factory is 0x3a99e0, allocation is 0x78, and its count
is at 0x6c after additional state at 0x68. Its string remains at 0x40. SDK
construction initializes the same defaults; normal C++ destruction releases
the string and CCObject base without passing undefined deleting flags.

OptionsScrollLayer reserves 0x300 bytes: search input at 0x2b8, background at
0x2c0, clear button at 0x2c8, query string at 0x2d0, match button at 0x2f0
and retained MusicSearchResult pointer at 0x2f8. Its native factory (0x47e380)
and initializer (0x47e520) replace the unfinished SDK popup construction.
Native getRelevantObjects (0x47f400) replaces the obsolete minimum-count
implementation. Without a query, positive-count or enabled options remain;
with a query, matching is case-insensitive and includes otherwise zero-count
options. The old m_minCount field is absent from the Windows SDK.

Native close (0x47f2d0), clear-all (0x47eef0), search clear (0x47efa0) and
match toggle (0x47ee60) are exposed. Fixtures create a real popup and option
row, toggle selection through the native row, clear selections, clear search,
exercise filtering and initialize an SDK-constructed popup. A populated SDK
search model verifies real Match: All/Any button clicks, labels, placement,
sort-preserving virtual refresh and repeated control creation. The refresh is
overridden locally so that this fixture does not request a music library.
Guarded SDK construction/destruction covers both
objects and an OptionsCell containing a real native label. OptionsScrollLayer
and OptionsCell use their native deleting destructors with explicit flags zero
and the SDK destruction-lock protocol, preserving existing inline helpers.
All four such native destructor-helper addresses also pass the executable
section audit, separately from the generated-wrapper count.

MusicSearchResult exposes the native All/Any flag at 0x8e and view-filter mode
at 0x94 without changing its 0xb0 allocation. applyTagFilters (0x497d00),
applyArtistFilters (0x4981c0) and updateFutureCount (0x498460) replace the
unresolved tag entry and incomplete SDK filtering/counting bodies. Tag filters
support intersection and union; artist filters include primary and additional
artists. Tests cover unchanged input with no selections, empty input, both
tag modes, artist matching and future counts including additional artists.
The single-ID manager helpers are inlined in the current native functions;
equivalent SDK implementations retain the old API's fresh arrays, order,
duplicates and null handling. They are not presented as separate native
addresses. The missing addTagMatchToggle helper reconstructs the control
inlined by MusicBrowser::onTags using the same button style and coordinates.
These changes remove three Windows address placeholders.

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
triggerQueuedMusic uses 0x64620, referenced by the native queue/update paths.
FMODQueuedMusic is 0x68 bytes, with stop/channel fields at 0x44/0x48. Native
music start at 0x63270 has seven arguments; the older eight-argument SDK
declaration lacked a Windows implementation and now forwards without its
unused tail. Tests use two real FMOD channels playing generated silent PCM,
verify resume suppression and playback through both overloads, then exercise
queued playback and stop/swap behavior. They check music/channel IDs, sound
state and volume-map swaps, and ownership of a heap-backed by-value path.
The fixtures stop/release their channels and sound and restore the original
music, channel and audio-state maps before returning.

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
