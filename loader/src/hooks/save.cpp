#include <Geode/loader/Loader.hpp>
#include <Geode/loader/GameEvent.hpp>
#include <Geode/loader/ModEvent.hpp>
#include "Save.hpp"

using namespace geode::prelude;

#include <Geode/modify/AppDelegate.hpp>
#include <Geode/modify/CCApplication.hpp>

namespace {
    bool shuttingDown = false;
}

namespace geode {
    void saveModData() {
        // Native teardown can save again after Exiting has stopped async tasks.
        if (shuttingDown) return;
        log::info("Saving mod data...");
        log::NestScope nest;

        auto begin = std::chrono::high_resolution_clock::now();

        (void)Loader::get()->saveData();

        auto end = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
        log::info("Took {}s", static_cast<float>(time) / 1000.f);
    }
}

$on_mod(Loaded) {
    GameEvent(GameEventType::Exiting).listen([] {
        shuttingDown = true;
    }, Priority::First + 1).leak();
}

struct SaveLoader : Modify<SaveLoader, AppDelegate> {
    GEODE_FORWARD_COMPAT_DISABLE_HOOKS("save moved to CCApplication::gameDidSave()")
    void trySaveGame(bool p0) {
        saveModData();
        return AppDelegate::trySaveGame(p0);
    }
};

#ifdef GEODE_IS_WINDOWS

struct FallbackSaveLoader : Modify<FallbackSaveLoader, CCApplication> {
    GEODE_FORWARD_COMPAT_ENABLE_HOOKS("")
    void gameDidSave() {
        saveModData();
        return CCApplication::gameDidSave();
    }
};

#endif

#ifdef GEODE_IS_ANDROID

#include <Geode/modify/FileOperation.hpp>
#include <Geode/loader/Dirs.hpp>

// redirects the save path to what geode knows, in case launcher's fopen hook fails
struct FileOperationOverride : Modify<FileOperationOverride, FileOperation> {
    static std::string getFilePath() {
        return dirs::getSaveDir().string() + "/";
    }
};

#endif
