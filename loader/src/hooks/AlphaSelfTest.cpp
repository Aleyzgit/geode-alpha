#include <Geode/loader/GameEvent.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Hook.hpp>
#include <Geode/loader/ModEvent.hpp>
#include <Geode/utils/async.hpp>
#include <Geode/binding/AppDelegate.hpp>
#include <Geode/binding/GameManager.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/binding/LocalLevelManager.hpp>
#include <Geode/binding/GooglePlayManager.hpp>
#include <Geode/binding/EnhancedGameObject.hpp>
#include <Geode/binding/GJRobotSprite.hpp>
#include <Geode/binding/GameLevelManager.hpp>
#include <Geode/binding/GameToolbox.hpp>
#include <Geode/binding/GJLevelList.hpp>
#include <Geode/cocos/textures/CCTexturePVR.h>
#include <Geode/utils/file.hpp>
#include <asp/fs/fs.hpp>
#include <ui/mods/settings/SettingNodeV3.hpp>

using namespace geode::prelude;

#ifdef GEODE_IS_WINDOWS
// These ABI assertions intentionally inspect polymorphic native classes.
#ifdef __clang__
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Winvalid-offsetof"
#endif
static_assert(sizeof(GameLevelManager) == 0x690);
static_assert(sizeof(GJLevelList) == 0x250);
static_assert(offsetof(GameLevelManager, m_onlineLevels) == 0x358);
static_assert(offsetof(GameLevelManager, m_dailyID) == 0x3c0);
static_assert(offsetof(GameLevelManager, m_weeklyID) == 0x3cc);
static_assert(offsetof(GameLevelManager, m_eventID) == 0x3d8);
static_assert(offsetof(GameLevelManager, m_gauntletLevels) == 0x408);
static_assert(offsetof(GameLevelManager, m_timerDict) == 0x420);
static_assert(offsetof(GameLevelManager, m_storedLevels) == 0x480);
static_assert(offsetof(GameLevelManager, m_trueString) == 0x670);
#ifdef __clang__
    #pragma clang diagnostic pop
#endif

static bool testLevelManagerCaches() {
    auto manager = GameLevelManager::get();
    auto online = manager->m_onlineLevels;
    auto gauntlets = manager->m_gauntletLevels;
    auto stored = manager->m_storedLevels;
    auto timers = manager->m_timerDict;
    auto fixtureLevel = GJGameLevel::create();
    auto results = CCArray::create();
    results->addObject(fixtureLevel);
    manager->m_onlineLevels = CCDictionary::create();
    manager->m_gauntletLevels = CCDictionary::create();
    manager->m_storedLevels = CCDictionary::create();
    manager->m_timerDict = CCDictionary::create();
    manager->m_onlineLevels->setObject(fixtureLevel, "717171");
    manager->m_gauntletLevels->setObject(fixtureLevel, "717171");
    manager->m_storedLevels->setObject(results, "alpha-cache");
    auto now = std::chrono::duration<double>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
    manager->m_timerDict->setObject(
        CCString::create(fmt::format("{}", now)), "alpha-cache"
    );
    manager->m_timerDict->setObject(
        CCString::create(fmt::format("{}", now - 120.)), "alpha-expired"
    );
    auto onlineValid = manager->getSavedLevel(717171) == fixtureLevel &&
        !manager->getSavedLevel(717172);
    auto gauntletValid = manager->getSavedGauntletLevel(717171) == fixtureLevel;
    auto timeValid = manager->isTimeValid("alpha-cache", 60.f) &&
        !manager->isTimeValid("alpha-expired", 60.f) &&
        !manager->isTimeValid("alpha-missing", 60.f);
    auto storedValid = manager->getStoredOnlineLevels("alpha-cache") == results;
    manager->m_onlineLevels = online;
    manager->m_gauntletLevels = gauntlets;
    manager->m_storedLevels = stored;
    manager->m_timerDict = timers;
    auto local = LocalLevelManager::get();
    auto lists = local->m_localLists;
    auto fixtureList = GJLevelList::create();
    fixtureList->m_M_ID = 717171;
    local->m_localLists = CCArray::create();
    local->m_localLists->addObject(fixtureList);
    auto localValid = manager->getLocalLevelList(717171) == fixtureList &&
        !manager->getLocalLevelList(717172);
    local->m_localLists = lists;
    if (!onlineValid || !gauntletValid || !timeValid || !storedValid || !localValid) {
        log::info("[ALPHA TEST] Cache details: online={}, gauntlet={}, time={}, stored={}, local={}",
            onlineValid, gauntletValid, timeValid, storedValid, localValid);
    }
    return onlineValid && gauntletValid && timeValid && storedValid && localValid;
}

static bool testPVRLoading() {
    auto directory = Mod::get()->getTempDir() / "alpha-pvr-test";
    if (!utils::file::createDirectoryAll(directory)) return false;
    auto path = directory / "invalid.pvr";
    std::array<uint8_t, 52> data{};
    if (!utils::file::writeBinary(path, data)) return false;
    auto texture = new CCTexturePVR();
    auto emptyValid = texture->getWidth() == 0 && texture->getHeight() == 0 &&
        texture->getNumberOfMipmaps() == 0;
    auto version2Rejected = !texture->initWithContentsOfFile(path.string().c_str());
    // Valid PVR3 magic with an unsupported pixel format; reject before GL upload.
    data[0] = 0x50;
    data[1] = 0x56;
    data[2] = 0x52;
    data[3] = 0x03;
    std::fill(data.begin() + 8, data.begin() + 16, 0xff);
    auto written = utils::file::writeBinary(path, data).isOk();
    auto version3Rejected = written &&
        !texture->initWithContentsOfFile(path.string().c_str());
    texture->release();
    (void)asp::fs::removeFile(path);
    (void)asp::fs::remove(directory);
    return emptyValid && version2Rejected && version3Rejected;
}
#endif

$on_mod(DataSaved) {
    if (!Loader::get()->getLaunchFlag("alpha-self-test")) return;

    // Mods may persist caches asynchronously from their DataSaved listener.
    async::runtime().spawnBlocking<void>([] {
        log::info("[ALPHA TEST] Async mod save: PASS");
    });
}

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
    log::info("[ALPHA TEST] Native PVR loader rejects invalid headers: {}",
        testPVRLoading() ? "PASS" : "FAIL");

    auto originalManager = GameLevelManager::get();
    auto constructedManager = new GameLevelManager();
    auto constructedValid = constructedManager->m_queuedLists.empty() &&
        constructedManager->m_availableFilters.empty() &&
        !constructedManager->m_onlineLevels && !constructedManager->m_trueString;
    constructedManager->release();
    // The native destructor clears the singleton even for a temporary object.
    *reinterpret_cast<GameLevelManager**>(base::get() + 0x90bfb0) = originalManager;
    log::info("[ALPHA TEST] Native level-manager constructor: {}",
        constructedValid ? "PASS" : "FAIL");

    log::info("[ALPHA TEST] Native level-manager caches: {}",
        testLevelManagerCaches() ? "PASS" : "FAIL");

    auto eased = GameToolbox::getEasedValue(0.5f, 2, 2.f);
    auto linear = GameToolbox::getEasedValue(0.37f, 0, 2.f);
    log::info("[ALPHA TEST] Native easing: {}",
        std::abs(eased - 0.25f) < 0.0001f &&
        std::abs(linear - 0.37f) < 0.0001f ? "PASS" : "FAIL");
    auto to = CCDictionary::create();
    auto from = CCDictionary::create();
    to->setObject(CCString::create("7"), "keep");
    to->setObject(CCString::create("3"), "increase");
    from->setObject(CCString::create("4"), "keep");
    from->setObject(CCString::create("9"), "increase");
    from->setObject(CCString::create("5"), "new");
    GameToolbox::mergeDictsSaveLargestInt(to, from);
    log::info("[ALPHA TEST] Native integer dictionary merge: {}",
        to->valueForKey("keep")->intValue() == 7 &&
        to->valueForKey("increase")->intValue() == 9 &&
        to->valueForKey("new")->intValue() == 5 ? "PASS" : "FAIL");

    auto enhanced = new EnhancedGameObject();
    log::info("[ALPHA TEST] Native enhanced object constructor: {}",
        !enhanced->m_poweredOn && enhanced->m_animationSpeed == 1.f ?
            "PASS" : "FAIL");
    enhanced->release();

    auto robot = GJRobotSprite::create(1);
    log::info("[ALPHA TEST] Native robot factory: {}",
        robot && robot->m_iconType == IconType::Robot ? "PASS" : "FAIL");

    bool lightningValid = false;
    try {
        auto lightning = CCLightning::lightningWithStrikePoint(
            {0.f, 0.f}, {20.f, 20.f}, 0.1f
        );
        lightning->CCLightning::setOpacity(123);
        lightningValid = lightning->m_opacity == 123 &&
            lightning->CCLightning::getOpacity() == 123 &&
            lightning->CCLightning::getDisplayedOpacity() == 123;
        lightning->CCLightning::updateDisplayedOpacity(91);
        lightning->CCLightning::setOpacityModifyRGB(true);
        lightning->CCLightning::setCascadeOpacityEnabled(true);
        lightning->CCLightning::setCascadeColorEnabled(true);
        ccColor3B color = {17, 83, 201};
        ccColor3B displayed = {33, 44, 55};
        lightning->CCLightning::setColor(color);
        lightning->CCLightning::updateDisplayedColor(displayed);
        lightning->CCLightning::strike();
        // draw generates the point buffer; strike only schedules its actions.
        lightning->CCLightning::draw();
        lightningValid = lightningValid &&
            lightning->m_displayedOpacity == 91 &&
            lightning->CCLightning::isOpacityModifyRGB() &&
            lightning->CCLightning::isCascadeOpacityEnabled() &&
            lightning->CCLightning::isCascadeColorEnabled() &&
            lightning->CCLightning::getColor() == color &&
            lightning->CCLightning::getDisplayedColor() == displayed &&
            lightning->m_lightningPoints && lightning->m_numPoints > 0 &&
            lightning->m_numPoints <= lightning->m_lightningPoints->size();
    }
    catch (std::exception const& error) {
        log::info("[ALPHA TEST] Lightning binding error: {}", error.what());
    }
    log::info("[ALPHA TEST] Native lightning properties and strike: {}",
        lightningValid ? "PASS" : "FAIL");

    auto googlePlay = GooglePlayManager::sharedState();
    log::info("[ALPHA TEST] Native Google Play singleton: {}",
        googlePlay && googlePlay == GooglePlayManager::sharedState() ?
            "PASS" : "FAIL");

    auto missingHook = Hook::create(
        reinterpret_cast<void*>(geode::base::get() + 0x9999999),
        +[] {}, "Alpha unresolved binding test",
        tulip::hook::TulipConvention::Default
    );
    auto missingResult = missingHook->enable();
    log::info("[ALPHA TEST] Unresolved hook rejected: {}",
        missingResult.isErr() && !missingHook->isEnabled() ? "PASS" : "FAIL");

    // Use a temporary level; no player progress is changed by this check.
    auto level = GJGameLevel::create();
    bool levelValid = false;
    if (level) {
        level->savePercentage(37, false, 12, 4, false);
        level->savePercentage(79, true, 0, 0, false);
        levelValid = level->m_normalPercent.value() == 37 &&
            level->m_practicePercent == 79 && level->m_clicks.value() == 12;
    }
    log::info("[ALPHA TEST] Native level progress layout: {}",
        levelValid ? "PASS" : "FAIL");

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
