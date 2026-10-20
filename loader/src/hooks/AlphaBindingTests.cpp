#include <Geode/binding/AchievementManager.hpp>
#include <Geode/binding/BoomScrollLayer.hpp>
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/BrowseSmartTemplateLayer.hpp>
#include <Geode/binding/CCAnimateFrameCache.hpp>
#include <Geode/binding/CCCounterLabel.hpp>
#include <Geode/binding/CCMenuItemToggler.hpp>
#include <Geode/binding/CCSpriteWithHue.hpp>
#include <Geode/binding/ColorAction.hpp>
#include <Geode/binding/ColorChannelSprite.hpp>
#include <Geode/binding/CommentAdminDelegate.hpp>
#include <Geode/binding/CreateGuidelinesLayer.hpp>
#include <Geode/binding/EditorUI.hpp>
#include <Geode/binding/EditorWindowPopup.hpp>
#include <Geode/binding/FMODAudioEngine.hpp>
#include <Geode/binding/GameLevelManager.hpp>
#include <Geode/binding/GameManager.hpp>
#include <Geode/binding/GameStatsManager.hpp>
#include <Geode/binding/GameToolbox.hpp>
#include <Geode/binding/GJEffectManager.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/binding/GJKeybindManager.hpp>
#include <Geode/binding/GlobalKeybindSource.hpp>
#include <Geode/binding/GJLevelList.hpp>
#include <Geode/binding/GJMultiplayerManager.hpp>
#include <Geode/binding/GJOptionsLayer.hpp>
#include <Geode/binding/GJRequestCell.hpp>
#include <Geode/binding/GJRewardItem.hpp>
#include <Geode/binding/GJUserScore.hpp>
#include <Geode/binding/LocalLevelManager.hpp>
#include <Geode/binding/KeybindSet.hpp>
#include <Geode/binding/LevelUploadDelegate.hpp>
#include <Geode/binding/LabelGameObject.hpp>
#include <Geode/binding/LevelEditorLayer.hpp>
#include <Geode/binding/LevelSearchLayer.hpp>
#include <Geode/binding/MusicDownloadManager.hpp>
#include <Geode/binding/NumberInputLayer.hpp>
#include <Geode/binding/NumberInputDelegate.hpp>
#include <Geode/binding/ParentalGateLayer.hpp>
#include <Geode/binding/OBB2D.hpp>
#include <Geode/binding/PulseEffectAction.hpp>
#include <Geode/binding/PagedListPopup.hpp>
#include <Geode/binding/SpriteAnimationManager.hpp>
#include <Geode/binding/ScrollViewPopup.hpp>
#include <Geode/binding/SetTextPopup.hpp>
#include <Geode/binding/SetTextPopupDelegate.hpp>
#include <Geode/binding/CCTextInputNode.hpp>
#include <Geode/binding/VirtualRowSource.hpp>
#include <Geode/binding/UserInfoDelegate.hpp>
#include <Geode/binding/UploadActionPopup.hpp>
#include <Geode/cocos/particle_nodes/CCParticleSystemQuad.h>
#include <Geode/cocos/CCScheduler.h>
#include <Geode/cocos/cocoa/CCAutoreleasePool.h>
#include <Geode/loader/GameEvent.hpp>
#include <Geode/loader/Loader.hpp>

using namespace geode::prelude;

#ifdef GEODE_IS_WINDOWS
// ABI checks deliberately inspect polymorphic native class layouts.
#ifdef __clang__
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Winvalid-offsetof"
#endif
static_assert(sizeof(GJOptionsLayer) == 0x470);
static_assert(sizeof(PagedListPopup) == 0x308);
static_assert(std::is_abstract_v<PagedListPopup>);
static_assert(std::is_abstract_v<VirtualRowSource>);
static_assert(sizeof(KeybindSet) == 0x20);
static_assert(sizeof(EditorWindowPopup) == 0x480);
static_assert(sizeof(ScrollViewPopup) == 0x2a8);
static_assert(sizeof(BrowseSmartTemplateLayer) == 0x2c8);
static_assert(sizeof(CreateGuidelinesLayer) == 0x300);
static_assert(sizeof(SetTextPopup) == 0x2f8);
static_assert(sizeof(MusicDownloadManager) == 0x3d0);
static_assert(sizeof(LevelSearchLayer) == 0x1f8);
static_assert(sizeof(GJRequestCell) == 0x248);
static_assert(std::is_abstract_v<KeybindSource>);
static_assert(std::is_abstract_v<CCKeyboardActionManager>);
static_assert(std::is_abstract_v<EditorWindowPopup>);
static_assert(std::is_abstract_v<ScrollViewPopup>);
namespace {
    class AlphaNumberReceiver final : public NumberInputDelegate {
    public:
        NumberInputLayer* popup = nullptr;
        std::string text;
        void numberInputClosed(NumberInputLayer* value) override {
            popup = value;
            text = value->m_inputString;
        }
    };

    class AlphaRequestCanary final : public GJRequestCell {
    public:
        std::array<uint8_t, 32> guard;
        AlphaRequestCanary() : GJRequestCell("alpha-request-fixture", 320.f, 40.f) {
            // Absorb the former wrong-class init writes inside this allocation.
            std::memset(reinterpret_cast<uint8_t*>(this) + 0x230, 0, 0x18);
            guard.fill(0xa5);
        }
    };

    class AlphaTextReceiver final : public SetTextPopupDelegate {
    public:
        SetTextPopup* popup = nullptr;
        std::string text;
        void setTextPopupClosed(SetTextPopup* value, std::string result) override {
            popup = value;
            text = std::move(result);
        }
    };

    class AlphaEditorWindowFixture final : public EditorWindowPopup {
    public:
        void onBuildWindowContent() override {}
    };

    class AlphaScrollFixture final : public ScrollViewPopup {
    public:
        float buildContent(CCNode*, float width, float height) override {
            return width + height;
        }
    };

    class AlphaUserInfoFixture final : public UserInfoDelegate {
    public:
        int failedID = -1;
        void getUserInfoFailed(int id) override { failedID = id; }
    };

    class AlphaAdminFixture final : public CommentAdminDelegate {
    public:
        unsigned callbacks = 0;
        void adminQueueMuteFinished() override { callbacks |= 1; }
        void adminQueueMuteFailed() override { callbacks |= 2; }
        void adminQueueUnmuteFinished() override { callbacks |= 4; }
        void adminQueueUnmuteFailed() override { callbacks |= 8; }
    };

    class AlphaUploadFixture final : public LevelUploadDelegate {
    public:
        bool failed = false;
        GJGameLevel* level = nullptr;
        void levelUploadFailed(GJGameLevel* value) override {
            failed = true;
            level = value;
        }
    };

    class AlphaDelegateFields {
        uintptr_t* m_fields;
        std::array<uintptr_t, 32> m_previous;
    public:
        explicit AlphaDelegateFields(GameLevelManager* manager) {
            m_fields = reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(manager) + 0x548);
            std::copy_n(m_fields, m_previous.size(), m_previous.begin());
            std::fill_n(m_fields, m_previous.size(), 0);
        }
        ~AlphaDelegateFields() {
            std::copy(m_previous.begin(), m_previous.end(), m_fields);
        }
        AlphaDelegateFields(AlphaDelegateFields const&) = delete;
        AlphaDelegateFields& operator=(AlphaDelegateFields const&) = delete;
    };

    class AlphaPagedFixture final : public PagedListPopup {
    public:
        int requestPage(int page) override { return page + 17; }
    };

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

    template <class F>
    bool withNativeEditor(bool noUI, F test) {
        auto manager = GameManager::get();
        if (manager->m_levelEditorLayer || manager->m_playLayer) return false;
        AlphaTestField editorState(manager->m_levelEditorLayer, manager->m_levelEditorLayer);
        AlphaTestField sceneState(manager->m_sceneEnum, manager->m_sceneEnum);
        auto local = LocalLevelManager::get();
        AlphaTestField levels(local->m_localLevels, CCArray::create());
        auto music = MusicDownloadManager::sharedState();
        // Native init checks this library-request flag before making requests.
        auto& librariesRequested = *reinterpret_cast<bool*>(reinterpret_cast<uint8_t*>(music) + 0x205);
        AlphaTestField libraryState(librariesRequested, true);
        auto audio = FMODAudioEngine::sharedEngine();
        auto& editorAudio = *reinterpret_cast<bool*>(reinterpret_cast<uint8_t*>(audio) + 0x272);
        AlphaTestField audioState(editorAudio, editorAudio);
        bool valid = false;
        {
            auto pool = CCPoolManager::sharedPoolManager();
            pool->push();
            auto pop = [](CCPoolManager* value) { value->pop(); };
            std::unique_ptr<CCPoolManager, decltype(pop)> scope(pool, pop);
            auto level = GameLevelManager::sharedState()->createNewLevel();
            auto editor = level ? LevelEditorLayer::create(level, noUI) : nullptr;
            valid = editor && editor->m_level == level && editor->m_levelSettings &&
                editor->m_objects && editor->m_undoObjects && editor->m_redoObjects &&
                editor->m_savedPositions.size() == 10 && test(editor);
            if (editor) editor->cleanup();
            local->m_localLevels->removeAllObjects();
        }
        return valid && !manager->m_levelEditorLayer;
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

    runBindingTest("Native options containers", [] {
        std::unique_ptr<GJOptionsLayer> popup(new GJOptionsLayer());
        auto defaults = popup->m_gap == 48.f && popup->m_page == -1 &&
            popup->m_togglesPerPage == 10;
        AlphaTestField pages(popup->m_pageContainers, CCArray::create());
        AlphaTestField groups(popup->m_groupContainers, CCArray::create());
        auto page = popup->getPageContainer(2);
        auto group = popup->getGroupContainer(3);
        auto valid = page && group && popup->m_pageContainers->count() == 3 &&
            popup->m_groupContainers->count() == 4 &&
            popup->getPageContainer(2) == page && popup->getGroupContainer(3) == group;
        popup->GJOptionsLayer::didToggleGV("alpha-no-op-fixture");
        return defaults && valid;
    });

    runBindingTest("Native particle numeric records", [] {
        std::vector<std::string> tokens(80, "0");
        tokens[0] = "37";
        tokens[1] = "2.5";
        tokens[2] = "1.25";
        tokens[3] = "0.5";
        tokens[4] = "11";
        tokens[5] = "90";
        tokens[11] = "3";
        tokens[12] = "-4";
        tokens[21] = "0.25";
        std::string record;
        for (auto const& token : tokens) {
            if (!record.empty()) record += 'a';
            record += token;
        }
        ParticleStruct particle;
        GameToolbox::particleStringToStruct(record, particle);
        auto parsed = particle.TotalParticles == 37 && particle.Duration == 2.5f &&
            particle.Life == 1.25f && particle.LifeVar == 0.5f &&
            particle.EmissionRate == 11 && particle.Angle == 90 &&
            particle.GravityX == 3 && particle.GravityY == -4 &&
            std::abs(particle.StartColorR - 0.25f) < 0.001f;
        GameToolbox::particleStringToStruct("", particle);
        return parsed && particle.TotalParticles == 37;
    });

    runBindingTest("Native paged base construction", [] {
        std::unique_ptr<AlphaPagedFixture> popup(new AlphaPagedFixture());
        auto bytes = reinterpret_cast<uint8_t const*>(popup.get());
        auto defaults = *reinterpret_cast<int const*>(bytes + 0x2ac) == -1 &&
            *reinterpret_cast<uint32_t const*>(bytes + 0x300) == 0xff969696;
        PagedListPopup* base = popup.get();
        return defaults && base->requestPage(5) == 22;
    });

    runBindingTest("Native level upload callback", [] {
        auto manager = GameLevelManager::get();
        AlphaDelegateFields delegates(manager);
        AlphaTestField requests(manager->m_friendReqAndUserBlocks, CCDictionary::create());
        AlphaTestField localLevels(LocalLevelManager::get()->m_localLevels, CCArray::create());
        auto level = GJGameLevel::create();
        level->m_levelID = 717171;
        level->m_M_ID = 717171;
        LocalLevelManager::get()->m_localLevels->addObject(level);
        AlphaUploadFixture callback;
        manager->m_levelUploadDelegate = &callback;
        manager->handleIt(false, "", "717171", GJHttpType::UploadLevel);
        auto dispatched = callback.failed && callback.level == level;
        callback.failed = false;
        callback.level = nullptr;
        manager->onUploadLevelCompleted("-1", "717171");
        return dispatched && callback.failed && callback.level == level;
    });

    runBindingTest("Native user info callback", [] {
        auto manager = GameLevelManager::get();
        AlphaDelegateFields delegates(manager);
        AlphaTestField downloads(manager->m_downloadObjects, CCDictionary::create());
        AlphaUserInfoFixture callback;
        manager->m_userInfoDelegate = &callback;
        manager->handleIt(false, "", "alpha_717171", GJHttpType::GetGJUserInfo);
        auto dispatched = callback.failedID == 717171;
        callback.failedID = -1;
        manager->onGetGJUserInfoCompleted("-1", "alpha_717171");
        return dispatched && callback.failedID == 717171;
    });

    runBindingTest("Native admin queue callbacks", [] {
        auto manager = GameLevelManager::get();
        AlphaDelegateFields delegates(manager);
        AlphaTestField requests(manager->m_friendReqAndUserBlocks, CCDictionary::create());
        AlphaAdminFixture callback;
        manager->m_commentAdminDelegate = &callback;
        manager->handleIt(true, "1", "alpha-queue", GJHttpType::AdminQueueMute);
        manager->handleIt(false, "", "alpha-queue", GJHttpType::AdminQueueMute);
        manager->handleIt(true, "1", "alpha-queue", GJHttpType::AdminQueueUnmute);
        manager->handleIt(false, "", "alpha-queue", GJHttpType::AdminQueueUnmute);
        return callback.callbacks == 15;
    });

    runBindingTest("Native button background sizing", [] {
        auto icon = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
        if (!icon) return false;
        auto button = ButtonSprite::create(icon);
        if (!button || button->m_subSprite != icon || !button->m_BGSprite) return false;
        auto before = button->getContentSize();
        button->updateSpriteBGSize();
        auto after = button->getContentSize();
        return before.width > 0 && before.height > 0 &&
            std::abs(after.width - before.width) < 0.001f &&
            std::abs(after.height - before.height) < 0.001f;
    });

    runBindingTest("Native upload popup factory", [] {
        auto popup = UploadActionPopup::create(nullptr, "Alpha fixture");
        return popup && popup->m_textArea && popup->m_loadingCircle &&
            popup->m_closeButton && !popup->m_succeeded;
    });

    runBindingTest("Native local key bindings", [] {
        auto source = std::make_unique<KeybindSet>();
        KeybindSource* base = source.get();
        if (source->m_changed || !base->isLevelSet()) return false;
        base->bindChord(7, 42);
        base->bindChord(9, 42);
        base->bindChord(7, 43);
        auto queried = base->chordsForAction(42) == std::vector<int>{7, 9} &&
            base->actionsForChord(7) == std::vector<int>{42, 43};
        base->unbindChord(7, 42);
        return queried && source->m_changed &&
            base->chordsForAction(42) == std::vector<int>{9} &&
            base->actionsForChord(7) == std::vector<int>{43} &&
            base->chordsForAction(717171).empty();
    });

    runBindingTest("Native global key lookup dispatch", [] {
        GlobalKeybindSource source(geode::ZeroConstructor);
        KeybindSource* base = &source;
        auto manager = GJKeybindManager::sharedState();
        CCKeyboardActionManager* keyboard = manager;
        return !base->isLevelSet() &&
            base->chordsForAction(717171) == manager->chordsForAction(717171) &&
            base->actionsForChord(717171) == manager->actionsForChord(717171) &&
            keyboard->actionsForKey(KEY_A, false, false, false) ==
                manager->GJKeybindManager::actionsForKey(KEY_A, false, false, false);
    });

    runBindingTest("Native editor window base", [] {
        auto popup = std::make_unique<AlphaEditorWindowFixture>();
        auto bytes = reinterpret_cast<uint8_t const*>(popup.get());
        EditorWindowPopup* base = popup.get();
        base->onBuildWindowContent();
        return *reinterpret_cast<float const*>(bytes + 0x418) == 1.f &&
            *reinterpret_cast<int const*>(bytes + 0x424) == -1 &&
            *reinterpret_cast<float const*>(bytes + 0x458) == 100.f;
    });

    runBindingTest("Native scroll base ownership", [] {
        auto owned = CCNode::create();
        auto references = owned->retainCount();
        {
            auto popup = std::make_unique<AlphaScrollFixture>();
            auto field = reinterpret_cast<CCObject**>(reinterpret_cast<uintptr_t>(popup.get()) + 0x298);
            if (*field) return false;
            owned->retain();
            *field = owned;
            ScrollViewPopup* base = popup.get();
            if (base->buildContent(nullptr, 17.f, 23.f) != 40.f) return false;
        }
        return owned->retainCount() == references;
    });

    runBindingTest("Native template browser construction", [] {
        auto popup = std::make_unique<BrowseSmartTemplateLayer>();
        return !popup->m_template && !popup->m_pages && popup->m_page == 0 &&
            !popup->m_nextPageBtn && !popup->m_prevPageBtn && !popup->m_dotsArray;
    });

    runBindingTest("Native guidelines construction", [] {
        auto popup = std::make_unique<CreateGuidelinesLayer>();
        return !popup->m_delegate && popup->m_elapsed == 0.f &&
            !popup->m_recording && !popup->m_unk2b5 && popup->m_guidelineString.empty();
    });

    runBindingTest("Native text popup factory", [] {
        {
            auto sdkPopup = std::make_unique<SetTextPopup>();
            if (sdkPopup->m_input || sdkPopup->m_delegate ||
                sdkPopup->m_disableDelegate || sdkPopup->m_cancelled) return false;
            sdkPopup->m_value = std::string(80, 'a');
        }
        auto popup = SetTextPopup::create("fixture", "Enter text", 32,
            "Alpha fixture", "OK", false, 0.f, false, 0, 0);
        if (!popup || !popup->m_input || popup->m_input->getString() != "fixture" ||
            popup->m_value != "fixture" || popup->m_delegate) return false;
        AlphaTextReceiver receiver;
        popup->m_delegate = &receiver;
        popup->onClose(nullptr);
        return receiver.popup == popup && receiver.text == "fixture";
    });

    runBindingTest("Native local level ordering", [] {
        auto manager = LocalLevelManager::get();
        AlphaTestField levels(manager->m_localLevels, CCArray::create());
        auto first = GJGameLevel::create();
        auto second = GJGameLevel::create();
        auto third = GJGameLevel::create();
        first->m_levelIndex = second->m_levelIndex = third->m_levelIndex = 90;
        for (auto level : {first, second, third}) manager->m_localLevels->addObject(level);
        auto ordered = manager->updateLevelOrder() == 3 && first->m_levelIndex == 2 &&
            second->m_levelIndex == 1 && third->m_levelIndex == 0;
        manager->m_localLevels->removeAllObjects();
        return ordered && manager->updateLevelOrder() == 0;
    });

    runBindingTest("Native music resource cache", [] {
        auto manager = MusicDownloadManager::sharedState();
        auto offset = reinterpret_cast<uintptr_t>(&manager->m_resourceSongUnorderedSet) -
            reinterpret_cast<uintptr_t>(manager);
        // Reject an incorrect SDK layout before touching native STL storage.
        if (sizeof(MusicDownloadManager) != 0x3d0 || offset != 0x248) return false;
        AlphaTestField songs(manager->m_resourceSongUnorderedSet, std::unordered_set<int>{717171});
        return manager->isSongDownloaded(717171) && !manager->isSongDownloaded(2147483000);
    });

    runBindingTest("Native music manager construction", [] {
        auto original = MusicDownloadManager::sharedState();
        auto& singleton = *reinterpret_cast<MusicDownloadManager**>(base::get() + 0x90c070);
        AlphaTestField restoreSingleton(singleton, original);
        auto manager = std::make_unique<MusicDownloadManager>();
        return !manager->m_activeDownloads && !manager->m_musicObjects &&
            manager->m_customContentURL.empty() && manager->m_musicTags.empty() &&
            manager->m_resourceSfxUnorderedSet.empty() && manager->m_resourceSongUnorderedSet.empty();
    });

    runBindingTest("Native audio mixer restart", [] {
        auto engine = FMODAudioEngine::sharedEngine();
        // The first call also catches an unresolved wrapper before pausing.
        engine->start();
        if (!engine->m_system || engine->m_stopped) return false;
        engine->stop();
        auto scheduler = CCDirector::sharedDirector()->getScheduler();
        auto stopped = engine->m_stopped;
        engine->start();
        auto resumed = !engine->m_stopped && engine->m_lastResult == FMOD_OK &&
            !scheduler->isTargetPaused(engine);
        engine->start();
        return stopped && resumed && !engine->m_stopped;
    });

    runBindingTest("Native suspended audio channels", [] {
        auto engine = std::make_unique<FMODAudioEngine>();
        engine->m_allAudioPaused = true;
        engine->m_suspendedGlobalAudio = true;
        engine->m_suspendedChannels = {717171, 717172};
        engine->resumeSuspendedAudio();
        return !engine->m_suspendedGlobalAudio && engine->m_suspendedChannels.empty();
    });

    runBindingTest("Native level search filters", [] {
        auto layer = std::make_unique<LevelSearchLayer>();
        auto offset = reinterpret_cast<uintptr_t>(&layer->m_difficultyDict) -
            reinterpret_cast<uintptr_t>(layer.get());
        if (sizeof(LevelSearchLayer) != 0x1f8 || offset != 0x1e0) return false;
        AlphaTestField filters(layer->m_difficultyDict, CCDictionary::create());
        layer->m_difficultyDict->setObject(CCString::create("1"), "D2");
        layer->m_difficultyDict->setObject(CCString::create("1"), "T1");
        auto selected = layer->checkDiff(2) && layer->checkTime(1) &&
            !layer->checkDiff(3) && !layer->checkTime(5);
        layer->m_difficultyDict->setObject(CCString::create("0"), "D2");
        return selected && !layer->checkDiff(2);
    });

    runBindingTest("Native search page construction", [] {
        auto layer = LevelSearchLayer::create(1);
        return layer && layer->m_searchInput && layer->m_noStarsSprite &&
            layer->m_difficultySprites && layer->m_lengthSprites && layer->m_difficultyDict &&
            layer->m_searchInput->getParent() && layer->getChildrenCount() > 0;
    });

    runBindingTest("Native request cell boundary", [] {
        auto cell = std::make_unique<AlphaRequestCanary>();
        auto initialized = cell->GJRequestCell::init();
        return initialized && std::all_of(cell->guard.begin(), cell->guard.end(),
            [](uint8_t value) { return value == 0xa5; });
    });

    runBindingTest("Native score layout", [] {
        return sizeof(GJUserScore) == 0x3b8 &&
            offsetof(GJUserScore, m_scoreType) == 0x180 &&
            offsetof(GJUserScore, m_friendReqStatus) == 0x2f8 &&
            offsetof(GJUserScore, m_toggled) == 0x31d;
    });

    runBindingTest("Native score construction boundary", [] {
        alignas(GJUserScore) std::array<uint8_t, sizeof(GJUserScore) + 32> storage;
        storage.fill(0xa5);
        auto destroy = [](GJUserScore* score) { score->~GJUserScore(); };
        std::unique_ptr<GJUserScore, decltype(destroy)> score(
            new (storage.data()) GJUserScore(), destroy);
        bool valid = score->init() && score->m_color2 == 3 &&
            score->m_playerCube == 1 && score->m_playerStreak == 1 &&
            score->m_friendReqStatus == 0 && !score->m_toggled;
        for (auto field : { &score->m_userName, &score->m_userUDID,
            &score->m_youtubeURL, &score->m_twitterURL, &score->m_twitchURL,
            &score->m_discordUsername, &score->m_instagramURL, &score->m_tiktokURL,
            &score->m_customString, &score->m_unkString, &score->m_demonInfo,
            &score->m_starsInfo, &score->m_platformerInfo }) {
            *field = std::string(96, 'a');
        }
        score.reset();
        return valid && std::all_of(storage.begin() + sizeof(GJUserScore), storage.end(),
            [](uint8_t value) { return value == 0xa5; });
    });

    runBindingTest("Native score dictionary and request selection", [] {
        auto data = CCDictionary::create();
        for (auto [key, value] : std::initializer_list<std::pair<char const*, char const*>> {
            { "1", "AlphaFixture" }, { "2", "2323" }, { "3", "123" },
            { "46", "456" }, { "52", "789" }, { "9", "1" }, { "11", "3" },
            { "29", "1" }, { "31", "4" },
            { "38", "8" }, { "39", "9" }, { "40", "10" }
        }) {
            data->setObject(CCString::create(value), key);
        }
        auto score = GJUserScore::create(data);
        if (!score || score->m_userName != "AlphaFixture" || score->m_userID != 2323 ||
            score->m_stars != 123 || score->m_diamonds != 456 || score->m_moons != 789 ||
            score->m_friendReqStatus != 4 || score->m_newMsgCount != 8 ||
            score->m_friendReqCount != 9 || score->m_newFriendCount != 10) {
            return false;
        }
        auto cell = std::make_unique<AlphaRequestCanary>();
        auto references = score->retainCount();
        cell->loadFromScore(score);
        if (cell->m_score != score || !cell->m_toggler || !cell->m_requestStatusIsFour ||
            score->retainCount() <= references) {
            return false;
        }
        score->m_toggled = false;
        cell->onToggle(nullptr);
        cell->updateToggle();
        bool selected = score->m_toggled && cell->m_toggler->isToggled();
        cell->onToggle(nullptr);
        cell->updateToggle();
        bool cleared = !score->m_toggled && !cell->m_toggler->isToggled();
        // The fixture has account ID zero, so this native action opens no page.
        cell->onViewProfile(nullptr);
        bool boundary = std::all_of(cell->guard.begin(), cell->guard.end(),
            [](uint8_t value) { return value == 0xa5; });
        cell.reset();
        // Autoreleased native row controls can retain the score until the
        // current frame's pool is drained, after this fixture returns.
        return selected && cleared && boundary;
    });

    runBindingTest("Native reward stat keys", [] {
        std::array<char const*, 15> expected {
            "18", "19", "17", "16", "20", "21", "", "", "",
            "23", "24", "25", "26", "27", "43"
        };
        for (size_t index = 0; index < expected.size(); ++index) {
            if (GJRewardItem::rewardItemToStat(static_cast<SpecialRewardItem>(index + 1)) !=
                expected[index]) {
                return false;
            }
        }
        return GJRewardItem::rewardItemToStat(static_cast<SpecialRewardItem>(0)).empty() &&
            GJRewardItem::rewardItemToStat(static_cast<SpecialRewardItem>(16)).empty() &&
            GJRewardItem::rewardItemToStat(static_cast<SpecialRewardItem>(-1)).empty();
    });

    runBindingTest("Native cached user lists", [] {
        auto manager = GameLevelManager::sharedState();
        auto dictionary = CCDictionary::create();
        AlphaTestField cache(manager->m_storedLevels, dictionary);
        std::array<UserListType, 4> types {
            UserListType::Friends, UserListType::Blocked, UserListType::Following, UserListType::Muted
        };
        std::array<char const*, 4> keys { "get_friends", "get_blocked", "get_following", "get_muted" };
        std::array<CCArray*, 4> lists;
        for (size_t index = 0; index < types.size(); ++index) {
            lists[index] = CCArray::create();
            lists[index]->addObject(CCString::create(keys[index]));
            dictionary->setObject(lists[index], keys[index]);
        }
        auto unrelated = CCString::create("preserved");
        dictionary->setObject(unrelated, "alpha-unrelated-cache");
        if (manager->getStoredUserList(static_cast<UserListType>(2))) return false;
        manager->resetStoredUserList(static_cast<UserListType>(2));
        for (size_t index = 0; index < types.size(); ++index) {
            if (manager->getStoredUserList(types[index]) != lists[index]) return false;
            manager->resetStoredUserList(types[index]);
            if (manager->getStoredUserList(types[index])) return false;
            for (size_t remaining = index + 1; remaining < types.size(); ++remaining) {
                if (manager->getStoredUserList(types[remaining]) != lists[remaining]) return false;
            }
        }
        return dictionary->count() == 1 &&
            dictionary->objectForKey("alpha-unrelated-cache") == unrelated;
    });

    runBindingTest("Native editor layer layout", [] {
        return sizeof(LevelEditorLayer) == 0x3e08 &&
            offsetof(LevelEditorLayer, m_savedPositions) == 0x3b90 &&
            offsetof(LevelEditorLayer, m_removingAll) == 0x3c98 &&
            offsetof(LevelEditorLayer, m_delayedSpawnNodes) == 0x3cc0 &&
            offsetof(LevelEditorLayer, m_unk3ae0) == 0x3cf0;
    });

    runBindingTest("Native editor construction boundary", [] {
        alignas(LevelEditorLayer) std::array<uint8_t, sizeof(LevelEditorLayer) + 32> storage;
        storage.fill(0xa5);
        auto destroy = [](LevelEditorLayer* layer) { layer->~LevelEditorLayer(); };
        std::unique_ptr<LevelEditorLayer, decltype(destroy)> editor(
            new (storage.data()) LevelEditorLayer(), destroy);
        bool valid = !editor->m_editorUI && editor->m_lockedLayerCount == 0 &&
            editor->m_savedPositions.empty() && editor->m_sessionTime == 0 &&
            !editor->m_removingAll && editor->m_delayedSpawnNodes.empty() &&
            editor->m_delayedSpawnNodes2.empty() && editor->m_unk3ae0.empty();
        editor->m_savedPositions.assign(10, CCPoint { 3.f, 5.f });
        editor->m_blendingColors.assign(100, true);
        editor->m_blendingColors2.assign(100, false);
        editor->m_unk3ae0.emplace(17, 23);
        editor->m_levelString = std::string(96, 'a');
        {
            AlphaTestField objects(editor->m_objects, CCArray::create());
            valid = valid && editor->getLastObjectX() == 0.f;
            editor->removeObject(nullptr, true);
        }
        editor.reset();
        return valid && std::all_of(storage.begin() + sizeof(LevelEditorLayer), storage.end(),
            [](uint8_t value) { return value == 0xa5; });
    });

    runBindingTest("Native fresh editor construction", [] {
        return withNativeEditor(true, [](LevelEditorLayer* editor) {
            return !editor->m_editorUI;
        }) && withNativeEditor(false, [](LevelEditorLayer* editor) {
            return editor->m_editorUI && editor->m_editorUI->m_editorLayer == editor &&
                editor->m_editorUI->getChildrenCount() > 0;
        });
    });

    runBindingTest("Native editor object creation and removal", [] {
        return withNativeEditor(false, [](LevelEditorLayer* editor) {
            auto count = editor->m_objects->count();
            auto object = editor->createObject(1, CCPoint { 30.f, 30.f }, true);
            if (!object || editor->m_objects->count() != count + 1) return false;
            editor->removeSpecial(object);
            editor->addSpecial(object);
            bool position = editor->getLastObjectX() >= 30.f;
            editor->removeObject(object, true);
            return position && editor->m_objects->count() == count;
        });
    });

    runBindingTest("Native editor minimum X transform", [] {
        return withNativeEditor(false, [](LevelEditorLayer* editor) {
            auto layer = editor->m_objectLayer;
            auto before = layer->getPosition();
            auto first = editor->m_editorUI->getXMin(0);
            bool restored = layer->getPosition() == before;
            auto shifted = before + CCPoint { 111.f, 222.f };
            layer->setPosition(shifted);
            auto second = editor->m_editorUI->getXMin(200);
            restored = restored && layer->getPosition() == shifted;
            layer->setPosition(before);
            return restored && std::isfinite(first) && std::abs(first - second) < .001f;
        });
    });

    runBindingTest("Native multiplayer download cache removal", [] {
        auto manager = GJMultiplayerManager::sharedState();
        auto dictionary = CCDictionary::create();
        AlphaTestField downloads(manager->m_activeDownloads, dictionary);
        auto kept = CCString::create("preserved");
        manager->addDLToActive("alpha-kept", kept);
        manager->addDLToActive("alpha-download", CCString::create("fixture"));
        std::string longTag(96, 'a');
        manager->addDLToActive(longTag.c_str(), CCString::create("long fixture"));
        manager->removeDLFromActive("alpha-download");
        manager->removeDLFromActive(longTag.c_str());
        manager->removeDLFromActive("alpha-absent");
        return !manager->isDLActive("alpha-download") &&
            !manager->isDLActive(longTag.c_str()) && dictionary->count() == 1 &&
            manager->getDLObject("alpha-kept") == kept;
    });

    runBindingTest("Native number input layout", [] {
        return sizeof(NumberInputLayer) == 0x300 &&
            sizeof(ParentalGateLayer) == 0x308 &&
            offsetof(NumberInputLayer, m_okButton) == 0x2a8 &&
            offsetof(NumberInputLayer, m_inputString) == 0x2d8 &&
            offsetof(NumberInputLayer, m_delegate) == 0x2f8;
    });

    runBindingTest("Native number input construction boundary", [] {
        alignas(NumberInputLayer) std::array<uint8_t, sizeof(NumberInputLayer) + 32> storage;
        storage.fill(0xa5);
        auto destroy = [](NumberInputLayer* layer) { layer->~NumberInputLayer(); };
        std::unique_ptr<NumberInputLayer, decltype(destroy)> popup(
            new (storage.data()) NumberInputLayer(), destroy);
        bool valid = !popup->m_titleLabel && !popup->m_okButton && !popup->m_delegate &&
            popup->m_minimum == 4 && popup->m_maximum == 4 &&
            popup->m_inputPrefix.empty() && popup->m_inputString.empty();
        popup->m_inputPrefix = std::string(96, 'p');
        popup->m_inputString = std::string(96, '7');
        popup.reset();
        return valid && std::all_of(storage.begin() + sizeof(NumberInputLayer), storage.end(),
            [](uint8_t value) { return value == 0xa5; });
    });

    runBindingTest("Native number input controls and delegate", [] {
        auto popup = NumberInputLayer::create();
        if (!popup || !popup->m_inputLabel || !popup->m_titleLabel || !popup->m_okButton) return false;
        popup->m_minimum = 2;
        popup->m_maximum = 4;
        popup->updateNumberState();
        if (popup->m_okButton->isEnabled()) return false;
        auto sender = CCNode::create();
        sender->setTag(7);
        popup->onNumber(sender);
        if (popup->m_inputString != "7" || popup->m_okButton->isEnabled()) return false;
        sender->setTag(3);
        popup->onNumber(sender);
        if (popup->m_inputString != "73" || !popup->m_okButton->isEnabled()) return false;
        popup->inputNumber(5);
        sender->setTag(10);
        popup->onNumber(sender);
        if (popup->m_inputString != "73") return false;
        popup->deleteLast();
        if (popup->m_inputString != "7" || popup->m_okButton->isEnabled()) return false;
        popup->m_inputString = "7359";
        popup->m_inputPrefix = "ID: ";
        popup->updateNumberState();
        if (std::string(popup->m_inputLabel->getString()) != "ID: 7359" ||
            !popup->m_okButton->isEnabled()) return false;
        AlphaNumberReceiver receiver;
        popup->m_delegate = &receiver;
        sender->setTag(12);
        popup->onNumber(sender);
        popup->m_delegate = nullptr;
        return receiver.popup == popup && receiver.text == "7359";
    });

    runBindingTest("Native arithmetic gate construction", [] {
        auto popup = ParentalGateLayer::create();
        // Exercise construction only; do not submit or change any settings.
        return popup && popup->m_inputLabel && popup->m_titleLabel && popup->m_okButton &&
            !popup->m_inputPrefix.empty() &&
            std::string(popup->m_inputLabel->getString()) == popup->m_inputPrefix + popup->m_inputString;
    });

    runBindingTest("Native animated counter updates", [] {
        if (sizeof(CCCounterLabel) != 0x238 || offsetof(CCCounterLabel, m_stepSize) != 0x218) return false;
        auto label = CCCounterLabel::create(0, "bigFont.fnt", FormatterType::Integer);
        if (!label) return false;
        label->m_dontSchedule = true;
        label->setTargetCount(25);
        for (int frame = 0; frame < 20; ++frame) label->updateCounter(1.f / 30.f);
        if (label->m_currentCount != 25 || label->m_counterEnabled ||
            std::string(label->getString()) != "25") return false;
        label->setTargetCount(-7);
        for (int frame = 0; frame < 20; ++frame) label->updateCounter(1.f / 30.f);
        if (label->m_currentCount != -7 || label->m_counterEnabled ||
            std::string(label->getString()) != "-7") return false;
        label->m_targetCount = 99;
        label->updateCounter(1.f / 30.f);
        return label->m_currentCount == -7;
    });

    runBindingTest("Native gameplay counter labels", [] {
        return withNativeEditor(false, [](LevelEditorLayer* editor) {
            auto object = editor->createObject(1615, CCPoint { 30.f, 30.f }, true);
            auto label = typeinfo_cast<LabelGameObject*>(object);
            if (!label) return false;
            std::unordered_map<int, std::vector<LabelGameObject*>> fixture { { 717171, { label } } };
            AlphaTestField labels(editor->m_labelObjects, std::move(fixture));
            AlphaTestField update(label->m_updateLabel, false);
            label->m_labelDirty = false;
            editor->updateCounters(717171, 42);
            if (!label->m_labelDirty || label->m_labelString != "42") return false;
            editor->updateCounters(717171, 1000000000);
            if (label->m_labelString != "999999999") return false;
            editor->updateCounters(717171, -17);
            if (label->m_labelString != "-17") return false;
            label->m_updateLabel = true;
            editor->updateCounters(717171, 99);
            editor->updateCounters(717172, 11);
            return label->m_labelString == "-17" && editor->m_labelObjects.size() == 1;
        });
    });
}
#ifdef __clang__
    #pragma clang diagnostic pop
#endif
#endif
