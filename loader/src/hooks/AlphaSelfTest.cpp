#include <Geode/loader/GameEvent.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/binding/AppDelegate.hpp>
#include <Geode/binding/GameManager.hpp>
#include <Geode/binding/LocalLevelManager.hpp>
#include <ui/mods/settings/SettingNodeV3.hpp>

using namespace geode::prelude;

$on_game(TexturesLoaded) {
    if (!Loader::get()->getLaunchFlag("alpha-self-test") &&
        !Loader::get()->getLaunchFlag("private-self-test")) return;

    auto setting = std::dynamic_pointer_cast<BoolSettingV3>(
        Mod::get()->getSetting("disable-last-crashed-popup")
    );
    auto node = BoolSettingNodeV3::create(setting, 400.f);
    auto toggle = static_cast<CCMenuItemToggler*>(
        node->getButtonMenu()->getChildren()->objectAtIndex(0)
    );
    auto before = node->getValue();
    toggle->activate();
    log::info("[ALPHA TEST] Boolean setting activation: {}",
        node->getValue() != before ? "PASS" : "FAIL");

    matjson::Value json;
    json["id"] = "alpha.compatibility-test";
    json["name"] = "Alpha compatibility test";
    json["version"] = "1.0.0";
    json["developer"] = "Local test";
    json["geode"] = "5.0.0";
    json["gd"]["win"] = "2.2081";
    auto metadata = ModMetadata::create(json);
    auto experimental =
        Loader::get()->getLaunchFlag("allow-incompatible-mods");
    auto gameAllowed = metadata.checkGameVersion().isOk();
    auto sdkAllowed = metadata.checkGeodeVersion().isOk();
    log::info("[ALPHA TEST] Incompatible version gates: {}",
        !metadata.hasErrors() && gameAllowed == experimental &&
            sdkAllowed == experimental ? "PASS" : "FAIL");

    json["gd"]["win"] = "0.000";
    metadata = ModMetadata::create(json);
    log::info("[ALPHA TEST] Invalid platform rejected: {}",
        metadata.checkPlatformSupported().isErr() ? "PASS" : "FAIL");
}

$on_game(Loaded) {
    if (!Loader::get()->getLaunchFlag("alpha-self-test") &&
        !Loader::get()->getLaunchFlag("private-self-test")) return;

#ifdef GEODE_IS_WINDOWS
    auto manager = GameManager::get();
    std::string key = "gv_geode-alpha-binding-test";
    Ref<CCObject> previous = manager->m_valueKeeper->objectForKey(key);
    manager->m_valueKeeper->setObject(CCString::create("42"), key);
    auto value = manager->getIntGameVariable("geode-alpha-binding-test");
    if (previous) {
        manager->m_valueKeeper->setObject(previous, key);
    }
    else {
        manager->m_valueKeeper->removeObjectForKey(key);
    }
    log::info("[ALPHA TEST] Native integer variable: {}",
        value == 42 ? "PASS" : "FAIL");

    auto levels = LocalLevelManager::sharedState();
    auto levelsValid = levels && levels->m_fileName == "CCLocalLevels.dat";
    log::info("[ALPHA TEST] Native local level manager: {}",
        levelsValid ? "PASS" : "FAIL");

    auto token = Mod::get()->getSavedValue<int>("alpha-save-test-token") + 1;
    Mod::get()->setSavedValue("alpha-save-test-token", token);
    AppDelegate::get()->trySaveGame(true);
    log::info("[ALPHA TEST] Requested native save; expected token: {}", token);

    if (Loader::get()->getLaunchFlag("alpha-self-test-exit")) {
        Loader::get()->queueInMainThread([] {
            CCDirector::get()->end();
        });
    }
#endif
}
