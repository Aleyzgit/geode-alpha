#include <Geode/binding/AchievementManager.hpp>
#include <Geode/binding/BoomScrollLayer.hpp>
#include <Geode/binding/CCAnimateFrameCache.hpp>
#include <Geode/binding/CCSpriteWithHue.hpp>
#include <Geode/binding/ColorAction.hpp>
#include <Geode/binding/ColorChannelSprite.hpp>
#include <Geode/binding/EditorUI.hpp>
#include <Geode/binding/FMODAudioEngine.hpp>
#include <Geode/binding/GameLevelManager.hpp>
#include <Geode/binding/GameStatsManager.hpp>
#include <Geode/binding/GameToolbox.hpp>
#include <Geode/binding/GJEffectManager.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/binding/GJLevelList.hpp>
#include <Geode/binding/LocalLevelManager.hpp>
#include <Geode/binding/MusicDownloadManager.hpp>
#include <Geode/binding/OBB2D.hpp>
#include <Geode/binding/PulseEffectAction.hpp>
#include <Geode/binding/SpriteAnimationManager.hpp>
#include <Geode/loader/GameEvent.hpp>
#include <Geode/loader/Loader.hpp>

using namespace geode::prelude;

#ifdef GEODE_IS_WINDOWS
namespace {
    // Restore fixture fields even when an unresolved SDK wrapper throws.
    template <class T>
    class AlphaTestField {
        T& m_field;
        T m_previous;

    public:
        AlphaTestField(T& field, T value) : m_field(field), m_previous(field) {
            m_field = value;
        }
        ~AlphaTestField() { m_field = m_previous; }
        AlphaTestField(AlphaTestField const&) = delete;
        AlphaTestField& operator=(AlphaTestField const&) = delete;
    };

    template <class F>
    void runBindingTest(char const* name, F test) {
        bool passed = false;
        try {
            passed = test();
        }
        catch (std::exception const& error) {
            log::info("[ALPHA TEST] {} exception: {}", name, error.what());
        }
        log::info("[ALPHA TEST] {}: {}", name, passed ? "PASS" : "FAIL");
    }
}

$on_game(Loaded) {
    if (!Loader::get()->getLaunchFlag("alpha-self-test")) return;

    runBindingTest("Native achievement lookup", [] {
        auto manager = AchievementManager::sharedState();
        AlphaTestField fixture(manager->m_platformAchievements, CCDictionary::create());
        auto value = CCDictionary::create();
        manager->m_platformAchievements->setObject(value, "alpha-achievement");
        return manager->getAchievementsWithID("alpha-achievement") == value &&
            !manager->getAchievementsWithID("alpha-missing");
    });

    runBindingTest("Native animation cache", [] {
        auto cache = CCAnimateFrameCache::sharedSpriteFrameCache();
        return cache && cache == CCAnimateFrameCache::sharedSpriteFrameCache() &&
            cache->m_animateFrames && cache->m_unk040 && cache->m_animateDescriptions;
    });

    runBindingTest("Native download state", [] {
        auto music = MusicDownloadManager::sharedState();
        AlphaTestField downloads(music->m_activeDownloads, CCDictionary::create());
        auto value = CCString::create("fixture");
        music->m_activeDownloads->setObject(value, "alpha-download");
        auto found = music->getDLObject("alpha-download") == value &&
            !music->getDLObject("alpha-missing");
        music->removeDLFromActive("alpha-download");
        music->removeDLFromActive("alpha-missing");
        auto removed = !music->getDLObject("alpha-download");
        auto levels = GameLevelManager::get();
        AlphaTestField downloaded(levels->m_downloadedLevels, CCDictionary::create());
        levels->m_downloadedLevels->setObject(value, "717171");
        return found && removed && levels->hasDownloadedLevel(717171) &&
            !levels->hasDownloadedLevel(717172);
    });

    runBindingTest("Native saved lists", [] {
        auto local = LocalLevelManager::get();
        AlphaTestField lists(local->m_localLists, CCArray::create());
        auto manager = GameLevelManager::get();
        AlphaTestField favorites(manager->m_favoriteLists, CCDictionary::create());
        auto list = manager->createNewLevelList();
        if (!list || local->m_localLists->count() != 1 ||
            local->m_localLists->objectAtIndex(0) != list || !list->m_isEditable ||
            list->m_listType != GJLevelType::Editor) return false;
        list->m_listID = 717171;
        manager->saveLevelList(list);
        return manager->m_favoriteLists->objectForKey("717171") == list;
    });

    runBindingTest("Native user coin state", [] {
        auto stats = GameStatsManager::sharedState();
        AlphaTestField verified(stats->m_verifiedUserCoins, CCDictionary::create());
        AlphaTestField pending(stats->m_pendingUserCoins, CCDictionary::create());
        AlphaTestField player(stats->m_playerStats, CCDictionary::create());
        auto value = CCString::create("1");
        stats->m_verifiedUserCoins->setObject(value, "alpha-coin");
        stats->m_pendingUserCoins->setObject(value, "alpha-pending");
        stats->m_playerStats->setObject(value, "unique_alpha-secret");
        return stats->hasUserCoin("alpha-coin") && !stats->hasUserCoin("alpha-missing") &&
            stats->hasPendingUserCoin("alpha-pending") &&
            !stats->hasPendingUserCoin("alpha-missing") &&
            stats->hasSecretCoin("alpha-secret") && !stats->hasSecretCoin("alpha-missing");
    });

    runBindingTest("Native completed level keys", [] {
        auto stats = GameStatsManager::sharedState();
        AlphaTestField completed(stats->m_completedLevels, CCDictionary::create());
        if (stats->getLevelKey(717171, false, false, false, false) != "n_717171" ||
            stats->getLevelKey(717171, true, false, false, false) != "c_717171" ||
            stats->getLevelKey(717171, true, true, false, false) != "d_717171" ||
            stats->getLevelKey(717171, true, false, true, false) != "g_717171" ||
            stats->getLevelKey(717171, true, true, true, true) != "e_717171") return false;
        auto level = GJGameLevel::create();
        level->m_levelID = 717171;
        level->m_dailyID = 0;
        level->m_gauntletLevel = false;
        level->m_levelType = GJLevelType::Saved;
        auto absent = !stats->hasCompletedLevel(level);
        stats->m_completedLevels->setObject(CCString::create("1"), "c_717171");
        return absent && stats->hasCompletedLevel(level);
    });

    runBindingTest("Native secret chests", [] {
        auto stats = GameStatsManager::sharedState();
        AlphaTestField chests(stats->m_treasureRoomChests, CCDictionary::create());
        stats->m_treasureRoomChests->setObject(CCString::create("1"), "717171");
        return stats->isSecretChestUnlocked(717171) && !stats->isSecretChestUnlocked(717172);
    });

    runBindingTest("Native oriented bounds", [] {
        auto first = OBB2D::create({0.f, 0.f}, 4.f, 2.f, 0.f);
        auto overlapping = OBB2D::create({1.f, 0.f}, 4.f, 2.f, 0.f);
        auto distant = OBB2D::create({50.f, 50.f}, 4.f, 2.f, 45.f);
        if (!first || !overlapping || !distant) return false;
        return first->overlaps1Way(first) && first->overlaps1Way(overlapping) &&
            overlapping->overlaps1Way(first) && !first->overlaps1Way(distant) &&
            !distant->overlaps1Way(first);
    });

    runBindingTest("Native item counts", [] {
        auto effects = GJEffectManager::create();
        if (!effects) return false;
        effects->m_itemCountMap[23] = 71;
        effects->m_itemCountMap[0] = 19;
        effects->m_itemCountMap[9999] = 29;
        return effects->countForItem(23) == 71 && effects->countForItem(24) == 0 &&
            effects->countForItem(-1) == 19 && effects->countForItem(10000) == 29;
    });

    runBindingTest("Native audio state and channels", [] {
        FMODAudioState state;
        if (state.m_interval != 0.f || state.m_elapsed != 0.f ||
            !state.m_tweensForEffectChannels.empty() ||
            !state.m_queuedMusicForChannels1.empty() ||
            !state.m_soundStateForChannels.empty()) return false;
        std::unique_ptr<FMODAudioEngine> engine(new FMODAudioEngine());
        // Lookup-only sentinel: no FMOD method is called through this pointer.
        auto marker = reinterpret_cast<FMOD::Channel*>(uintptr_t(0x717171));
        engine->m_channelIDToChannel[717171] = marker;
        engine->m_effectIDToChannelID[818181] = 717171;
        auto valid = engine->channelForChannelID(717171) == marker &&
            !engine->channelForChannelID(717172) &&
            engine->channelIDForUniqueID(818181) == 717171 &&
            engine->channelIDForUniqueID(818182) == 0;
        engine->m_channelIDToChannel.clear();
        engine->m_effectIDToChannelID.clear();
        engine->stopChannel(717172, AudioTargetType::SFXChannel, false, 0.f);
        engine->stopAllEffects();
        return valid;
    });

    runBindingTest("Native coin key namespaces", [] {
        auto level = GJGameLevel::create();
        level->m_levelID = 717171;
        level->m_dailyID = 0;
        level->m_gauntletLevel = false;
        auto regular = std::string(level->getCoinKey(2)) == "717171_2";
        level->m_gauntletLevel = true;
        auto gauntlet = std::string(level->getCoinKey(2)) == "717171_2_g";
        level->m_dailyID = 818181;
        return regular && gauntlet &&
            std::string(level->getCoinKey(2)) == "717171_2_818181";
    });

    runBindingTest("Native pulse colors", [] {
        auto effects = GJEffectManager::create();
        if (!effects) return false;
        ccColor3B base = {20, 40, 60};
        ccColor3B target = {100, 120, 140};
        PulseEffectAction action{};
        action.m_color = target;
        action.m_pulseEffectType = PulseEffectType::Color;
        auto inactive = effects->colorForPulseEffect(base, &action) == base;
        action.m_currentValue = 1.f;
        auto active = effects->colorForPulseEffect(base, &action) == target;
        action.m_currentValue = 0.5f;
        auto blended = effects->colorForPulseEffect(base, &action) == ccColor3B{60, 80, 100};
        auto emptyGroup = effects->colorForGroupID(23, base, true) == base;
        effects->m_pulseEffectMap[23].push_back(action);
        effects->m_unkVector2f0[23] = true;
        auto group = effects->colorForGroupID(23, base, true) == ccColor3B{60, 80, 100};
        return inactive && active && blended && emptyGroup && group;
    });

    runBindingTest("Native editor object offsets", [] {
        return EditorUI::offsetForKey(0) == CCPoint{0.f, 0.f} &&
            EditorUI::offsetForKey(9) == CCPoint{0.f, -12.5f} &&
            EditorUI::offsetForKey(-1) == CCPoint{0.f, 0.f};
    });

    runBindingTest("Native scroll pages and dots", [] {
        auto pages = CCArray::create();
        pages->addObject(CCLayer::create());
        pages->addObject(CCLayer::create());
        auto scroll = BoomScrollLayer::create(pages, 0, false, nullptr, nullptr);
        if (!scroll || !scroll->m_pages || scroll->m_pages->count() != 2 ||
            !scroll->m_dots || scroll->m_dots->count() != 2) {
            log::info("[ALPHA TEST] Scroll fixture: created={}, pages={}, dots={}",
                scroll != nullptr, scroll && scroll->m_pages ? scroll->m_pages->count() : 0,
                scroll && scroll->m_dots ? scroll->m_dots->count() : 0);
            return false;
        }
        scroll->updateDots(0.f);
        auto first = static_cast<CCSprite*>(scroll->m_dots->objectAtIndex(0));
        auto second = static_cast<CCSprite*>(scroll->m_dots->objectAtIndex(1));
        scroll->m_dotPosition = CCPoint{140.f, 50.f};
        scroll->updateDots(0.f);
        return first && second &&
            std::abs(first->getPositionX() - 132.f) < 0.001f &&
            std::abs(second->getPositionX() - 148.f) < 0.001f &&
            std::abs(first->getPositionY() - 50.f) < 0.001f &&
            std::abs(second->getPositionY() - 50.f) < 0.001f;
    });

    runBindingTest("Native user identity caches", [] {
        auto manager = GameLevelManager::get();
        AlphaTestField names(manager->m_knownUsers, CCDictionary::create());
        AlphaTestField accounts(manager->m_accountIDtoUserIDDict, CCDictionary::create());
        AlphaTestField users(manager->m_userIDtoAccountIDDict, CCDictionary::create());
        AlphaTestField accountNames(manager->m_usernameForAccountID,
            std::unordered_map<int, std::string>{});
        AlphaTestField downloads(manager->m_downloadObjects, CCDictionary::create());
        manager->storeUserName(717171, 818181, "Alpha Native Fixture");
        auto valid = std::string(manager->m_knownUsers->valueForKey("717171")->getCString()) ==
                "Alpha Native Fixture" &&
            manager->m_usernameForAccountID.at(818181) == "Alpha Native Fixture" &&
            manager->m_accountIDtoUserIDDict->valueForKey(818181)->intValue() == 717171 &&
            manager->m_userIDtoAccountIDDict->valueForKey(717171)->intValue() == 818181;
        manager->m_downloadObjects->setObject(CCString::create("fixture"), "alpha-download");
        manager->removeDLFromActive("alpha-download");
        manager->removeDLFromActive("alpha-missing");
        return valid && !manager->m_downloadObjects->objectForKey("alpha-download");
    });

    runBindingTest("Native setup dictionaries", [] {
        auto dictionary = GameToolbox::stringSetupToDict("alpha,17,beta,29", ",");
        if (!dictionary || dictionary->count() != 2) return false;
        return dictionary->valueForKey("alpha")->intValue() == 17 &&
            dictionary->valueForKey("beta")->intValue() == 29 &&
            !GameToolbox::stringSetupToDict("alpha,17,orphan", ",");
    });

    runBindingTest("Native color interpolation", [] {
        auto action = ColorAction::create({20, 40, 60}, {100, 120, 140},
            1.f, 0., false, -1, 0.2f, 0.8f);
        if (!action) return false;
        action->step(0.5f);
        auto halfway = action->m_color == ccColor3B{60, 80, 100} &&
            std::abs(action->m_currentOpacity - 0.5f) < 0.001f && !action->m_stepFinished;
        action->m_paused = true;
        action->step(0.25f);
        auto paused = std::abs(action->m_deltaTime - 0.5f) < 0.001f;
        action->m_paused = false;
        action->step(0.5f);
        return halfway && paused && action->m_stepFinished &&
            action->m_color == ccColor3B{100, 120, 140} &&
            std::abs(action->m_currentOpacity - 0.8f) < 0.001f;
    });

    runBindingTest("Native color channel labels", [] {
        auto sprite = ColorChannelSprite::create();
        if (!sprite) return false;
        sprite->updateBlending(true);
        auto visible = sprite->m_blendingLabel && sprite->m_blendingLabel->isVisible();
        sprite->updateBlending(false);
        auto hidden = sprite->m_blendingLabel && !sprite->m_blendingLabel->isVisible();
        sprite->updateCopyLabel(23, false);
        auto copy = sprite->m_copyLabel && sprite->m_copyLabel->isVisible() &&
            std::string(sprite->m_copyLabel->getString()) == "23";
        sprite->updateCopyLabel(0, false);
        return visible && hidden && copy && !sprite->m_copyLabel->isVisible();
    });

    runBindingTest("Native hue color matrix", [] {
        std::unique_ptr<CCSpriteWithHue> sprite(new CCSpriteWithHue());
        sprite->m_hue = 0.f;
        sprite->updateColorMatrix();
        for (size_t row = 0; row < 3; ++row) {
            for (size_t column = 0; column < 3; ++column) {
                if (std::abs(sprite->m_colorMatrix[row][column] - (row == column ? 1.f : 0.f)) >
                    0.001f) return false;
            }
        }
        auto identity = sprite->m_colorMatrix;
        sprite->m_hue = 1.f;
        sprite->updateColorMatrix();
        auto changed = std::abs(sprite->m_colorMatrix[0][0] - identity[0][0]) > 0.01f;
        for (auto const& row : sprite->m_colorMatrix) {
            for (auto value : row) {
                if (!std::isfinite(value)) return false;
            }
        }
        return changed;
    });

    runBindingTest("Native animation priority", [] {
        std::unique_ptr<SpriteAnimationManager> manager(new SpriteAnimationManager());
        manager->m_currentAnimation = "alpha-current";
        manager->m_nextAnimation = "alpha-next";
        manager->overridePrio();
        auto moved = manager->m_currentAnimation == "not_used" &&
            manager->m_nextAnimation == "alpha-current";
        manager->overridePrio();
        return moved && manager->m_currentAnimation == "not_used" &&
            manager->m_nextAnimation == "alpha-current";
    });

    runBindingTest("Native completion reset", [] {
        auto stats = GameStatsManager::sharedState();
        AlphaTestField completed(stats->m_completedLevels, CCDictionary::create());
        auto value = CCString::create("1");
        for (auto key : {"c_717171", "star_717171", "demon_717171", "alpha-preserve"}) {
            stats->m_completedLevels->setObject(value, key);
        }
        auto level = GJGameLevel::create();
        level->m_levelID = 717171;
        level->m_dailyID = 0;
        level->m_gauntletLevel = false;
        level->m_levelType = GJLevelType::Saved;
        stats->uncompleteLevel(level);
        return !stats->hasCompletedLevel(level) &&
            !stats->m_completedLevels->objectForKey("star_717171") &&
            !stats->m_completedLevels->objectForKey("demon_717171") &&
            stats->m_completedLevels->objectForKey("alpha-preserve") == value &&
            stats->getPathRewardKey(23) == "pr_23";
    });
}
#endif
