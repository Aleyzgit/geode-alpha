<p align="center">
	<img src="/title.png" />
	<h3 align="center">
		<a href="https://geode-sdk.org">Home page</a>
	</h3>
</p>

<p align="center"><b>Geode</b> is a <a href="https://store.steampowered.com/app/322170/Geometry_Dash/">Geometry Dash</a> <b>mod loader</b> and <b>modding SDK</b> with a modern approach towards mod development.</p>

## Why Geode?

Unlike previous mod loaders, which merely inject the DLLs and let devs handle the rest, Geode aims to be a more comprehensive project that provides all the tools needed for creating mods in one package.

Geode's goal is to solve **mod incompatibility** - to ensure that mods work together without buttons getting misplaced or hooks mysteriously disappearing.

## "Hello World!" Example

Here's a **Hello World** mod in Geode:

```cpp
#include <Geode/Bindings.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

class $modify(MenuLayer) {
	void onMoreGames(CCObject*) {
		FLAlertLayer::create(
			"Geode",
			"Hello World from my Custom Mod!",
			"OK"
		)->show();
	}
};
```

This code modifies what happens when the "More Games" button is clicked on the home scene in Geometry Dash, showing a custom popup.

## Documentation

Detailed documentation, tutorials, and installation instructions on using Geode can be found [here](https://docs.geode-sdk.org).

New to modding GD? Geode's documentation also comes with a handy [tutorial book](https://docs.geode-sdk.org/#/handbook/chap0) that explains all the basics of GD modding!

## Contribution

For changes to this fork, open a [Pull Request](https://github.com/Aleyzgit/geode-alpha/pulls).
Follow the upstream code style and keep changes focused and tested.

## Questions, help, etc.

If you have any further questions, need help, or just want to share your love for catgirls, be sure to join [our Discord server](https://discord.gg/9e43WMKzhp)!

## Geode Alpha

This fork targets Windows Geometry Dash **2.2091** and follows Geode's
in-progress v6 port. It retains the upstream theme and mod package format.
It is an alpha build, not a completed port or a promise of immediate support
for future game updates.

The mod catalog, modal popup input, mod-data save hooks, GLFW error handlers,
Windows font imports and shutdown-thread location have been updated.
The Windows GameManager layout has also been corrected so native methods
and SDK field access use the same game-variable and scene storage.
The GJGameLevel layout and editor build statistics now use the newer upstream
bindings, including corrected level-browser, slider and secret-level methods.
**2 Windows native-address placeholders remain**
in the pinned game declarations. The engine declarations have no remaining
Windows placeholders. Additional unsafe or incorrect entries have been corrected,
including PVR functions that previously pointed outside executable memory.
GameLevelManager and GJLevelList now use corrected Windows layouts and native
construction. Mod data is saved before async shutdown, preventing a later
native save from invoking mods after their runtime has stopped.
The audio engine's constructor, state layout and channel lookups are corrected;
the animation manager initializes the dictionary released by its destructor.
The editor's object-offset helper now uses its native static calling convention.
Unsupported hooks report an error instead of claiming successful activation.
Native class layouts and SDK interfaces also change between versions.

To include mods targeting other GD/Geode versions in the catalog and allow
experimental installation and loading, start the game with
`--geode:allow-incompatible-mods`. This bypasses version checks; it does not
convert old DLLs. Some mods need to be rebuilt or ported by their authors.
Platform checks and package hash checks remain enabled.

Build with Clang and the normal upstream CMake workflow. Bindings are pinned
to `4a4044007192ae7fbbd72f0cd1e9f17094d9515b`; the fork's verified native
address corrections are applied in the build directory. To reject a build
while Windows placeholders remain, configure with
`-DGEODE_ALPHA_REQUIRE_COMPLETE_BINDINGS=ON`. This check is expected to fail
until the remaining native bindings have been ported. It also checks custom
`GEODE_BINDINGS_PATH` directories and the engine declarations. Passing this
address check alone does not prove mod or class-layout compatibility.

Loader auto-updates are disabled by default. Use the resources produced by
the same build as the loader. Run the optional runtime checks with
`--geode:alpha-self-test`; combine it with the compatibility flag to test the
experimental version-check behavior. Report fork-specific issues to
[Geode Alpha](https://github.com/Aleyzgit/geode-alpha/issues).

Windows validation used Clang 21.1.8. The 170 native checks passed in both
compatibility modes, including manager cache fixtures, local list lookup,
easing, dictionary merging, robot creation, lightning drawing and invalid PVR
header rejection. Additional fixtures exercise native audio state and channels,
identity caches, completion resets, coin namespaces, color interpolation, hue
matrices, animation priority, editor offsets and scroll indicators.
Fixtures also cover options/particle containers, abstract paged interfaces and
native upload, user-info and admin-queue callback dispatch. The Windows HTTP
request IDs and manager delegate layout match these native dispatch paths.
Key binding queries use typed integer vectors and the native virtual-method
order. Further fixtures exercise image-button sizing, popup factories, text
input state and construction/destruction of editor and scroll bases.
Search-page construction and difficulty/length filters use the current layout.
Request cells now use their native initialization address and toggler layout;
a regression fixture checks that initialization stays inside the object.
Score objects reserve the native 0x3b8 bytes and use the corrected social-link
and request-state offsets. Fixtures cover heap-backed string destruction,
native dictionary parsing and request selection through real row controls.
Four obsolete request-cell virtual callbacks and the removed request-popup
actions are excluded from the Windows SDK, since they have no matching native
interface. This removal reduces the placeholder count; it does not restore
those older APIs or establish binary compatibility with their users.
Native reward-stat keys and all four cached user-list categories are verified,
including selective cache invalidation and preservation of unrelated entries.
The editor layer reserves its native 0x3e08 bytes and aligns saved positions,
session state and delayed-spawn containers with native construction/destruction.
Two unrelated addresses for editor removal and last-object position are corrected.
Fresh editors initialize with and without their UI; a temporary level exercises
object creation, special-object registration and removal through native methods.
Editor minimum-X transforms restore the object layer's position, and native
multiplayer download removal preserves unrelated cached requests.
The number-input popup uses its current 0x300-byte layout and native digit and
completion callbacks; tests cover editing, limits, display prefixes and delegates.
Counter fixtures cover increasing/decreasing animations and updates to an actual
gameplay counter object, including value limits and its update lock.
Save-manager, gameplay and SFX callbacks use the current native virtual order
and typed return values. A local audit matched all 2,116 checked virtual methods
across 461 classes to native RTTI tables. An earlier audit identified 84 slot
or base-offset disagreements that are now corrected. Upstream removed several
redundant or obsolete declarations from the audited set. Tabbed settings and
level browsers reserve their full native
allocations; fixtures cover construction/destruction boundaries and actual
game-settings tab dispatch. This audit covers known mapped virtual methods,
not every SDK interface or mod.
An expanded structural audit covers 2,302 virtual entries across 467 classes,
including 2,120 mapped native targets, with no remaining disagreements in that
sample. It found a missing Windows monitor handle in CCApplication; restoring
the field fixes application setting offsets and AppDelegate's scene-delegate
base. Live tests compare the settings with exported engine getters. Four native
application/scene callbacks are also available for Windows hooks again.
Native list parsing and reordering handle replacement, position bounds and
empty lists. Paged reloads dispatch through their native request/clear callbacks.
Picker interfaces now require typed recipient-array/count and ID-name receivers.
Two obsolete Windows popup callbacks are excluded because their native vtables
have no corresponding slot; this does not restore those old APIs.
The current upstream v6 changes are integrated through `fed6cfb`, including
popup touch priority, particle draw cost and updated async dependencies. Fixtures verify balanced
popup priority registration and SDK arithmetic-popup construction.
A comparison against 548 upstream Windows size references still reports 127
differences to investigate; some reference sizes conflict with verified native
construction. The class-layout port remains incomplete.
The effect manager reserves its native 0x828 bytes, including new state before
the item-state sets. Trigger-ID fixtures exercise storing, lookup, duplicate
insertion, native removal and SDK reset, plus guarded native construction.
Options objects and their search popup use their native 0x78/0x300 layouts.
The incomplete popup initializer and minimum-count filter are replaced with
the native initialization and search functions. Fixtures exercise real option
rows, clearing selections/search, case-insensitive filtering and SDK lifetimes.
Music search uses native tag/artist filtering and future counts, including
All/Any tags and additional artists. The SDK also supplies the single-ID filter
helpers and missing Match: All/Any control. Fixtures verify local filtering,
counts and real button clicks through an isolated search model.
Object initialization, custom color sprites, reserved-channel blending and
editor selection filters now use their verified native implementations.
Fixtures exercise sprite replacement and object/group/color filters in a fresh
editor. Native level deletion preserves progress while removing downloaded
data; tests use temporary lists, levels and caches. Achievement unlocks and
item-info factories are also restored and tested with isolated unlock data.
Particle construction uses the native Windows 0x348-byte quad allocation;
the extra pointer used on other platforms is excluded from Windows. Guarded
fixtures exercise particle initialization, draw cost and single-button defaults.
Shader state uses double split targets and fully initialized SDK construction.
Native enter/exit effect registration and removal are available again; a loader
correction prevents the game's replacement loop from overrunning a one-element
effect vector. Fixtures exercise value refresh, replacement, selective removal
and independent channels. Extra game layers copy native object transforms.
Screen buttons use the correct native factory and position helper, with tests
for scale/radius changes, alternate mode and single-button configurations.
Option-page counters and the missing SDK increment helper use native routines.
Video/parental option constructors initialize their dictionary and button pointers;
guarded fixtures check initialization, release counts and allocation boundaries.
Native active-path selection is tested with temporary unlock/progress data,
including retaining unfinished paths and selecting the next eligible path.
The keybinding page uses its current 0x4a0-byte layout, native construction,
tabs and search controls. Six obsolete Windows page/dictionary helpers are
excluded because the native page no longer contains that data. This removes
one placeholder without restoring those old APIs. Keybinding settings now
return typed entries and reserve their full native popup allocation; fixtures
check actual test-button callbacks and guarded destruction.
Browser page/folder labels use the native update routine. Timeline positions
use the native seven-argument helper, with a forwarding overload for the older
SDK call. Fixtures compare portal timelines with the native editor and cover
empty lists, all start speeds and platformer behavior.
PauseLayer reserves its native 0x2a0 bytes, including start-position selection,
the time control and side-button sprites. Guarded construction and destruction
check array ownership; fixtures exercise queued start positions and all three
quest slots. The SDK quest badge follows the game's inlined setup behavior.
The time control also reserves its native 0x160 bytes. Tests exercise its actual
slider and audio toggle, speed rounding, label updates and SDK initialization.
Ground and middleground texture completion callbacks handle either arrival
order. The treasure-room music fade is restored and checked against real FMOD
fade points on isolated silent channels.
MusicBrowser reserves its native 0x398 bytes and uses the corrected initializer.
Fixtures create ordinary, NCS and Chompo browsers, including through the SDK,
and exercise refresh with isolated, already active requests. Library version
checks and full downloads use their separate native routines.
The former SFX search-popup API is supplied through the current text popup
and shared folder-search preference. Logger writes preserve messages across
buffer boundaries, and synchronous flush requests survive notification races.
SFXBrowser reserves its native 0x468 bytes and uses native construction and
destruction for its new controls and containers. Its tests check retained
objects, native/SDK initialization and refresh. SFX dictionary, artist and
changelog pointers and library counters now match the native download manager.
SFX metadata and folders reserve 0xb0/0xb8 bytes, including their extra strings.
Native initialization builds the lowercase search cache; SDK folder creation
sets the folder flag. Fixtures verify string/array lifetimes and SDK-created
metadata through actual native SFX search.
All 5,513 populated generated wrappers point into executable
sections of their respective tested modules; that audit does not prove their
semantics. The native save path persisted a test
value across game runs. Dark Windows Titlebar 1.0.1 was rebuilt from its
unchanged C++ source against this SDK and loaded its window hook successfully.
Its published Geode 5 DLL cannot load unchanged because it imports three
EventCenterThreadLocal exports that the v6 SDK no longer provides. This is
one tested mod, not a general binary-compatibility guarantee.

For existing Geode 5 mods, the optional [Windows runtime launcher](installer/windows/alpha-runtimes.md)
can select a separate GD 2.2081 / Geode 5.11.2 installation. It checks
the configured binaries and mod manifests before launch. This provides a
matching older game process; it does not complete the 2.2091 port or convert
old mods into Geode 6 mods.

The optional [mod repair and native validation tools](installer/windows/alpha-native-validation.md)
fix Prism's missing setting default and initialize absent Eclipse/BetterInfo
theme/cache files. A separately built Geode 5 backport fixes the same save-order
failure for legacy mods. The 38 copied mod packages reached the main menu and
clean shutdown in that runtime without ERROR log entries; Silicate's declared
conflicts remain. This does not establish compatibility with every mod feature.

**This fork contains code and documentation created with AI assistance.
Geode Alpha is unofficial and is not an official Geode release or endorsed
by the Geode team.**
