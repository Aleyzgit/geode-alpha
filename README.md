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
**62 Windows native-address placeholders remain**
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
to `d6f3312f0886b2c285fd0d2593ebabdc03b6ca0c`; the fork's verified native
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

Windows validation used Clang 21.1.8. Seventy-six native checks passed in both
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
All 5,459 populated generated wrappers point into executable
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
