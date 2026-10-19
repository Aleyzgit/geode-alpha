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
Three Windows binding placeholders have been resolved; **209 remain** across
the pinned game and engine declarations (191 game and 18 engine addresses).
Unsupported hooks report an error instead of claiming successful activation.
Native class layouts and SDK interfaces also change between versions.

To include mods targeting other GD/Geode versions in the catalog and allow
experimental installation and loading, start the game with
`--geode:allow-incompatible-mods`. This bypasses version checks; it does not
convert old DLLs. Some mods need to be rebuilt or ported by their authors.
Platform checks and package hash checks remain enabled.

Build with Clang and the normal upstream CMake workflow. Bindings are pinned
to `cac7b26495124025212d0f5e4cf82bb43b8162e8`; the fork's verified native
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

Windows validation used Clang 21.1.8. Runtime checks passed for settings,
version gates, platform rejection, native integer variables, local-level
access, native level progress and rejection of unresolved hooks in both
compatibility modes. The native save path persisted a test
value across game runs. Dark Windows Titlebar 1.0.1 was rebuilt from its
unchanged C++ source against this SDK and loaded its window hook successfully.
Its published Geode 5 DLL cannot load unchanged because it imports three
EventCenterThreadLocal exports that the v6 SDK no longer provides. This is
one tested mod, not a general binary-compatibility guarantee.

**This fork contains code and documentation created with AI assistance.
Geode Alpha is unofficial and is not an official Geode release or endorsed
by the Geode team.**
