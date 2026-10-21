#include <Geode/binding/AchievementManager.hpp>
#include <Geode/binding/AppDelegate.hpp>
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
#include <Geode/binding/CustomSFXDelegate.hpp>
#include <Geode/binding/DrawGridLayer.hpp>
#include <Geode/binding/EffectGameObject.hpp>
#include <Geode/binding/EditorUI.hpp>
#include <Geode/binding/EditorWindowPopup.hpp>
#include <Geode/binding/EnterEffectObject.hpp>
#include <Geode/binding/FMODAudioEngine.hpp>
#include <Geode/binding/FMODMusic.hpp>
#include <Geode/binding/FMODQueuedMusic.hpp>
#include <Geode/binding/FMODSoundState.hpp>
#include <Geode/binding/FriendPickerDelegate.hpp>
#include <Geode/binding/FindObjectPopup.hpp>
#include <Geode/binding/IDNameBrowserDelegate.hpp>
#include <Geode/binding/ItemInfoPopup.hpp>
#include <Geode/binding/GameLevelManager.hpp>
#include <Geode/binding/GameManager.hpp>
#include <Geode/binding/GameObject.hpp>
#include <Geode/binding/GameSettingsLayer.hpp>
#include <Geode/binding/GameStatsManager.hpp>
#include <Geode/binding/GameToolbox.hpp>
#include <Geode/binding/GJEffectManager.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/binding/GJKeybindManager.hpp>
#include <Geode/binding/GlobalKeybindSource.hpp>
#include <Geode/binding/GManager.hpp>
#include <Geode/binding/GJLevelList.hpp>
#include <Geode/binding/GJMultiplayerManager.hpp>
#include <Geode/binding/GJMGLayer.hpp>
#include <Geode/binding/GJOptionsLayer.hpp>
#include <Geode/binding/GJRequestCell.hpp>
#include <Geode/binding/GJRewardItem.hpp>
#include <Geode/binding/GJSpriteColor.hpp>
#include <Geode/binding/GJStoreItem.hpp>
#include <Geode/binding/GJShaderState.hpp>
#include <Geode/binding/GJSearchObject.hpp>
#include <Geode/binding/GJUINode.hpp>
#include <Geode/binding/GJUserScore.hpp>
#include <Geode/binding/LocalLevelManager.hpp>
#include <Geode/binding/KeybindSet.hpp>
#include <Geode/binding/KeybindingsLayer.hpp>
#include <Geode/binding/KeybindingsSettingsPopup.hpp>
#include <Geode/binding/KeybindSettingsEntry.hpp>
#include <Geode/binding/LevelUploadDelegate.hpp>
#include <Geode/binding/LabelGameObject.hpp>
#include <Geode/binding/LevelEditorLayer.hpp>
#include <Geode/binding/LevelBrowserLayer.hpp>
#include <Geode/binding/LevelListLayer.hpp>
#include <Geode/binding/LevelOptionsLayer.hpp>
#include <Geode/binding/LevelOptionsLayer2.hpp>
#include <Geode/binding/LevelSearchLayer.hpp>
#include <Geode/binding/LevelSettingsObject.hpp>
#include <Geode/binding/LevelTools.hpp>
#include <Geode/binding/MusicDownloadManager.hpp>
#include <Geode/binding/MusicBrowser.hpp>
#include <Geode/binding/MusicSearchResult.hpp>
#include <Geode/binding/MoreVideoOptionsLayer.hpp>
#include <Geode/binding/NewgroundsInfoLayer.hpp>
#include <Geode/binding/NumberInputLayer.hpp>
#include <Geode/binding/NumberInputDelegate.hpp>
#include <Geode/binding/OptionsCell.hpp>
#include <Geode/binding/OptionsObject.hpp>
#include <Geode/binding/OptionsObjectDelegate.hpp>
#include <Geode/binding/OptionsScrollLayer.hpp>
#include <Geode/binding/ParentalGateLayer.hpp>
#include <Geode/binding/ParentalOptionsLayer.hpp>
#include <Geode/binding/OBB2D.hpp>
#include <Geode/binding/PulseEffectAction.hpp>
#include <Geode/binding/PagedListPopup.hpp>
#include <Geode/binding/PlayLayer.hpp>
#include <Geode/binding/ReportQueuePopup.hpp>
#include <Geode/binding/SFXBrowser.hpp>
#include <Geode/binding/ShaderLayer.hpp>
#include <Geode/binding/SongInfoObject.hpp>
#include <Geode/binding/SpriteAnimationManager.hpp>
#include <Geode/binding/ScrollViewPopup.hpp>
#include <Geode/binding/SetTextPopup.hpp>
#include <Geode/binding/SetTextPopupDelegate.hpp>
#include <Geode/binding/CCTextInputNode.hpp>
#include <Geode/binding/VirtualRowSource.hpp>
#include <Geode/binding/UserInfoDelegate.hpp>
#include <Geode/binding/UploadActionPopup.hpp>
#include <Geode/binding/TableViewCellDelegate.hpp>
#include <Geode/binding/TabbedPopupLayer.hpp>
#include <Geode/binding/UIButtonConfig.hpp>
#include <Geode/cocos/particle_nodes/CCParticleSystemQuad.h>
#include <Geode/cocos/CCScheduler.h>
#include <Geode/cocos/cocoa/CCAutoreleasePool.h>
#include <Geode/ui/Popup.hpp>
#include <Geode/loader/GameEvent.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/utils/addresser.hpp>

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
    class AlphaTriggerToggleFixture final : public SetupTriggerPopup {
    public:
        std::vector<std::pair<int, bool>> updates;
        std::vector<std::pair<int, float>> changes;
        bool guarded = true;
        void updateToggleItem(int tag, bool toggled) override {
            guarded = guarded && m_disableTextDelegate;
            updates.emplace_back(tag, toggled);
            SetupTriggerPopup::updateToggleItem(tag, toggled);
        }
        void valueDidChange(int tag, float value) override {
            guarded = guarded && m_disableTextDelegate;
            changes.emplace_back(tag, value);
        }
    };

    class AlphaEventFixture final : public GJBaseGameLayer {
    public:
        struct Call {
            int group;
            bool ordered;
            double delay;
            std::vector<int> remap;
            int trigger;
            int control;
        };
        std::vector<Call> calls;
        void spawnGroup(int group, bool ordered, double delay, std::vector<int> const& remap,
            int trigger, int control) override {
            calls.push_back({group, ordered, delay, remap, trigger, control});
        }
    };

    class AlphaKeybindEntryReceiver final : public CCObject {
    public:
        unsigned calls = 0;
        CCObject* lastSender = nullptr;
        void onEntry(CCObject* sender) {
            ++calls;
            lastSender = sender;
        }
    };

    class AlphaOptionsReceiver final : public OptionsObjectDelegate {
    public:
        unsigned changes = 0;
        OptionsObject* last = nullptr;
        void stateChanged(OptionsObject* value) override {
            ++changes;
            last = value;
        }
    };

    OptionsCell* findOptionsRow(CCNode* node, OptionsObject* option) {
        if (auto row = typeinfo_cast<OptionsCell*>(node); row && row->m_optionsObject == option) {
            return row;
        }
        if (auto children = node->getChildren()) {
            for (unsigned index = 0; index < children->count(); ++index) {
                if (auto row = findOptionsRow(static_cast<CCNode*>(children->objectAtIndex(index)), option)) {
                    return row;
                }
            }
        }
        return nullptr;
    }

    class AlphaModalFixture final : public geode::Popup {
    public:
        bool initialize() { return Popup::init(120.f, 80.f); }
        ~AlphaModalFixture() override = default;
    };

    class AlphaMusicShape final : public MusicBrowser {
    public:
        virtual void boundary() {}
    };
    class AlphaNewgroundsShape final : public NewgroundsInfoLayer {
    public:
        virtual void boundary() {}
    };

    class AlphaPickerFixture final : public FriendPickerDelegate, public IDNameBrowserDelegate {
    public:
        CCArray* recipients = nullptr;
        int selectedID = -1;
        unsigned int selectedRecipientCount() override { return recipients ? recipients->count() : 0; }
        CCArray* selectedRecipients() override { return recipients; }
        void clearRecipients() override { if (recipients) recipients->removeAllObjects(); }
        void idNameBrowserClosed(IDNameBrowserPopup*, int id) override { selectedID = id; }
    };

    class AlphaPagedReloadFixture final : public PagedListPopup {
    public:
        int requested = -1;
        unsigned requests = 0;
        unsigned clears = 0;
        int requestPage(int page) override {
            requested = page;
            ++requests;
            return 1;
        }
        void clearPageContent() override { ++clears; }
    };

    class AlphaTabbedFixture final : public TabbedPopupLayer {};
    class AlphaBrowserFixture final : public LevelBrowserLayer {};
    class AlphaListFixture final : public LevelListLayer {};
    class AlphaMusicSearchFixture final : public MusicSearchResult {
    public:
        int updates = 0;
        AudioSortType lastSort = AudioSortType::NameAscending;
        void updateObjects(AudioSortType sort) override {
            ++updates;
            lastSort = sort;
        }
    };

    template <class T>
    bool checkConstructionBoundary() {
        alignas(T) std::array<uint8_t, sizeof(T) + 32> storage;
        storage.fill(0xa5);
        auto value = new (storage.data()) T();
        value->T::~T();
        return std::all_of(storage.begin() + sizeof(T), storage.end(),
            [](uint8_t byte) { return byte == 0xa5; });
    }

    class AlphaSFXReceiver final : public CustomSFXDelegate {
    public:
        bool played = false;
        int getActiveCustomSFXID() override { return 717171; }
        bool overridePlaySFX(SFXInfoObject*) override {
            played = true;
            return true;
        }
    };

    class AlphaTableSelection final : public TableViewCellDelegate {
    public:
        int getSelectedCellCustomIdx() override { return 717171; }
        bool allowsCustomSFXActions() override { return true; }
        bool isCustomIDSelected(int id) override { return id == 717171; }
        bool inCustomSFXTree() override { return true; }
        bool showsSFXInfoButton() override { return true; }
        bool shouldSnapToSelected() override { return false; }
        int getCellDelegateType() override { return -1; }
    };

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

    template <class Layer>
    bool checkOptionPageCounts(size_t valuesOffset) {
        auto popup = std::make_unique<Layer>();
        if (reinterpret_cast<uint8_t*>(&popup->m_values) - reinterpret_cast<uint8_t*>(popup.get()) != valuesOffset) {
            return false;
        }
        AlphaTestField values(popup->m_values, CCDictionary::create());
        auto counts = popup->m_values;
        counts->setObject(CCString::create("17"), "page_3");
        counts->setObject(CCString::create("-2"), "page_-7");
        counts->setObject(CCString::create("44"), "alpha-sentinel");
        if (popup->countForPage(3) != 17 || popup->countForPage(-7) != -2 ||
            popup->countForPage(123456789) != 0 || counts->count() != 3) return false;
        popup->incrementCountForPage(3);
        popup->incrementCountForPage(-7);
        if (popup->countForPage(3) != 18 || popup->countForPage(-7) != -1 ||
            counts->valueForKey("alpha-sentinel")->intValue() != 44 || counts->count() != 3) return false;
        counts->setObject(CCString::create("invalid"), "page_3");
        return popup->countForPage(3) == 0 && counts->count() == 3;
    }

    template <class F>
    bool withNativeMusicChannels(F test) {
        auto engine = FMODAudioEngine::sharedEngine();
        AlphaTestField music(engine->m_fmodMusic, std::unordered_map<int, FMODMusic>{});
        AlphaTestField channels(engine->m_channelIDToChannel, std::unordered_map<int, FMOD::Channel*>{});
        AlphaTestField state(engine->m_audioState, FMODAudioState{});
        AlphaTestField removed(engine->m_removedSounds, std::vector<FMOD::Sound*>{});
        AlphaTestField result(engine->m_lastResult, engine->m_lastResult);
        std::vector<short> pcm(44100 * 3, 0);
        FMOD_CREATESOUNDEXINFO info{};
        info.cbsize = sizeof(info);
        info.length = pcm.size() * sizeof(short);
        info.numchannels = 1;
        info.defaultfrequency = 44100;
        info.format = FMOD_SOUND_FORMAT_PCM16;
        FMOD::Sound* rawSound = nullptr;
        if (!engine->m_system || engine->m_system->createSound(reinterpret_cast<char const*>(pcm.data()),
            FMOD_OPENMEMORY | FMOD_OPENRAW, &info, &rawSound) != FMOD_OK || !rawSound) return false;
        auto release = [](FMOD::Sound* value) { value->release(); };
        std::unique_ptr<FMOD::Sound, decltype(release)> sound(rawSound, release);
        auto stop = [](FMOD::Channel* value) { value->stop(); };
        std::unique_ptr<FMOD::Channel, decltype(stop)> first(nullptr, stop);
        std::unique_ptr<FMOD::Channel, decltype(stop)> second(nullptr, stop);
        for (int index = 0; index < 2; ++index) {
            FMOD::Channel* channel = nullptr;
            if (engine->m_system->playSound(sound.get(), engine->m_backgroundMusicChannel, true, &channel) != FMOD_OK ||
                !channel) return false;
            (index == 0 ? first : second).reset(channel);
            if (channel->setVolume(0.f) != FMOD_OK) return false;
            auto id = 717171 + index;
            auto channelID = 818181 + index;
            FMODMusic entry{};
            entry.m_channelID = channelID;
            entry.m_sound = sound.get();
            entry.m_filePath = std::string(128, char('a' + index));
            engine->m_fmodMusic.emplace(id, entry);
            engine->m_channelIDToChannel.emplace(channelID, channel);
            auto& channelState = engine->m_audioState.m_soundStateForChannels[channelID];
            channelState.m_musicID = id;
            channelState.m_channelID = channelID;
            channelState.m_isMusic = true;
            engine->m_audioState.m_volumeForMusicChannels[id] = index == 0 ? .25f : .75f;
        }
        return test(engine, first.get(), second.get());
    }

    template <class Layer>
    bool checkOptionConstruction() {
        alignas(Layer) std::array<uint8_t, sizeof(Layer) + 32> storage;
        storage.fill(0xa5);
        auto popup = new (storage.data()) Layer();
        auto initialized = !popup->m_values && !popup->m_variables &&
            !popup->m_prevButton && !popup->m_nextButton;
        auto values = CCDictionary::create();
        auto variables = CCDictionary::create();
        values->retain();
        variables->retain();
        popup->m_values = values;
        popup->m_variables = variables;
        popup->~Layer();
        return initialized && values->retainCount() == 1 && variables->retainCount() == 1 &&
            std::all_of(storage.begin() + sizeof(Layer), storage.end(),
                [](uint8_t byte) { return byte == 0xa5; });
    }

    template <class F>
    bool withMusicFilters(F test) {
        auto pool = CCPoolManager::sharedPoolManager();
        pool->push();
        auto pop = [](CCPoolManager* value) { value->pop(); };
        std::unique_ptr<CCPoolManager, decltype(pop)> scope(pool, pop);
        auto search = MusicSearchResult::create(GJSongType::Music);
        auto songs = CCArray::create();
        for (int i = 0; i < 3; ++i) {
            auto song = SongInfoObject::create(717171 + i);
            if (!song) return false;
            song->m_artistID = 10 + i;
            song->m_artistIDs.clear();
            song->m_tags = i == 0 ? std::set<int>{1, 2} : std::set<int>{i};
            songs->addObject(song);
        }
        static_cast<SongInfoObject*>(songs->objectAtIndex(0))->m_artistIDs.insert(11);
        if (!search) return false;
        auto tags = CCArray::create();
        auto artists = CCArray::create();
        for (int i = 0; i < 3; ++i) {
            auto tag = OptionsObject::create(i + 1, false, "Alpha tag", nullptr);
            auto artist = OptionsObject::create(i + 10, false, "Alpha artist", nullptr);
            if (!tag || !artist) return false;
            tags->addObject(tag);
            artists->addObject(artist);
        }
        AlphaTestField tagState(search->m_tagFilterObjects, tags);
        AlphaTestField artistState(search->m_artistFilterObjects, artists);
        return test(search, songs, tags, artists);
    }

    template <class F>
    bool withLevelDeletion(F test) {
        auto manager = GameLevelManager::get();
        auto local = LocalLevelManager::get();
        AlphaTestField levels(local->m_localLevels, CCArray::create());
        AlphaTestField online(manager->m_onlineLevels, CCDictionary::create());
        AlphaTestField daily(manager->m_dailyLevels, CCDictionary::create());
        AlphaTestField deaths(manager->m_deathRelated1,
            std::unordered_map<int, std::string>{{717171, "deleted"}, {717172, "preserved"}});
        AlphaTestField related(manager->m_deathRelated2,
            std::unordered_map<int, int>{{717171, 1}, {717172, 2}});
        AlphaTestField data(manager->m_deathData, std::unordered_set<int>{717171, 717172});
        return test(manager, local);
    }
}

$on_game(Loaded) {
    if (!Loader::get()->getLaunchFlag("alpha-self-test")) return;

    runBindingTest("Native application scene delegate layout", [] {
        auto app = AppDelegate::get();
        if (!app || sizeof(CCApplication) != 0xd8 || sizeof(AppDelegate) != 0x100) return false;
        auto bytes = reinterpret_cast<uint8_t const*>(app);
        auto delegate = static_cast<CCSceneDelegate*>(app);
        auto table = *reinterpret_cast<uintptr_t const* const*>(delegate);
        return reinterpret_cast<uint8_t const*>(delegate) - bytes == 0xd8 &&
            table[0] == geode::base::get() + 0x98cc0 &&
            offsetof(AppDelegate, m_glViewSetup) == 0xe0 &&
            offsetof(AppDelegate, m_gamePaused) == 0xe1 &&
            offsetof(AppDelegate, m_runningScene) == 0xe8 &&
            offsetof(AppDelegate, m_loadingFinished) == 0xf0 &&
            offsetof(AppDelegate, m_saveTime) == 0xf8 && app->m_loadingFinished;
    });

    runBindingTest("Native application setting accessors", [] {
        if (sizeof(CCApplication) != 0xd8 ||
            offsetof(CCApplication, m_bVerticalSyncEnabled) != 0xd0) return false;
        auto app = CCApplication::sharedApplication();
        auto module = GetModuleHandleW(L"libcocos2d.dll");
        if (!app || !module) return false;
        struct Setting {
            bool CCApplication::* field;
            char const* getter;
        };
        Setting settings[] = {
            {&CCApplication::m_bVerticalSyncEnabled, "?getVerticalSyncEnabled@CCApplication@cocos2d@@QEBA_NXZ"},
            {&CCApplication::m_bControllerConnected, "?getControllerConnected@CCApplication@cocos2d@@QEBA_NXZ"},
            {&CCApplication::m_bSleepMode, "?getSleepMode@CCApplication@cocos2d@@QEBA_NXZ"},
            {&CCApplication::m_bForceTimer, "?getForceTimer@CCApplication@cocos2d@@QEBA_NXZ"},
            {&CCApplication::m_bSmoothFix, "?getSmoothFix@CCApplication@cocos2d@@QEBA_NXZ"},
            {&CCApplication::m_bFullscreen, "?getFullscreen@CCApplication@cocos2d@@QEBA_NXZ"},
            {&CCApplication::m_bBorderless, "?getBorderless@CCApplication@cocos2d@@QEBA_NXZ"},
        };
        for (auto const& setting : settings) {
            auto getter = reinterpret_cast<bool (*)(CCApplication const*)>(GetProcAddress(module, setting.getter));
            if (!getter) return false;
            AlphaTestField restore(app->*setting.field, false);
            if (getter(app)) return false;
            app->*setting.field = true;
            if (!getter(app)) return false;
        }
        return true;
    });

    runBindingTest("Native music search state layout", [] {
        auto search = MusicSearchResult::create(GJSongType::Music);
        return search && sizeof(MusicSearchResult) == 0xb0 &&
            offsetof(MusicSearchResult, m_tagFilterObjects) == 0x48 &&
            offsetof(MusicSearchResult, m_artistFilterObjects) == 0x50 &&
            offsetof(MusicSearchResult, m_matchAnyTags) == 0x8e &&
            offsetof(MusicSearchResult, m_viewFilterMode) == 0x94 &&
            offsetof(SongInfoObject, m_artistID) == 0x1a8 &&
            offsetof(SongInfoObject, m_artistIDs) == 0x218 &&
            offsetof(SongInfoObject, m_tags) == 0x2c8 &&
            !search->m_matchAnyTags && search->m_viewFilterMode == 0;
    });

    runBindingTest("Native music tag and artist filters", [] {
        return withMusicFilters([](MusicSearchResult* search, CCArray* songs, CCArray* tags, CCArray* artists) {
            if (search->applyTagFilters(songs) != songs || search->m_tagFilter ||
                search->applyArtistFilters(songs) != songs || search->m_artistFilter) return false;
            auto firstTag = static_cast<OptionsObject*>(tags->objectAtIndex(0));
            auto secondTag = static_cast<OptionsObject*>(tags->objectAtIndex(1));
            firstTag->m_enabled = secondTag->m_enabled = true;
            auto all = search->applyTagFilters(songs);
            if (all->count() != 1 || all->objectAtIndex(0) != songs->objectAtIndex(0) ||
                !search->m_tagFilter) return false;
            search->m_matchAnyTags = true;
            auto any = search->applyTagFilters(songs);
            if (any->count() != 3) return false;
            for (unsigned int i = 0; i < songs->count(); ++i) {
                if (any->objectAtIndex(i) != songs->objectAtIndex(i)) return false;
            }
            auto artist = static_cast<OptionsObject*>(artists->objectAtIndex(1));
            artist->m_enabled = true;
            auto selected = search->applyArtistFilters(songs);
            if (selected->count() != 2 || selected->objectAtIndex(0) != songs->objectAtIndex(0) ||
                selected->objectAtIndex(1) != songs->objectAtIndex(1) || !search->m_artistFilter) return false;
            search->m_matchAnyTags = false;
            auto empty = CCArray::create();
            return search->applyTagFilters(empty) == empty && search->m_tagFilter &&
                search->applyArtistFilters(empty) == empty && !search->m_artistFilter && songs->count() == 3;
        });
    });

    runBindingTest("Native music future filter counts", [] {
        return withMusicFilters([](MusicSearchResult* search, CCArray* songs, CCArray* tags, CCArray* artists) {
            auto first = static_cast<OptionsObject*>(tags->objectAtIndex(0));
            auto second = static_cast<OptionsObject*>(tags->objectAtIndex(1));
            auto absent = static_cast<OptionsObject*>(tags->objectAtIndex(2));
            first->m_enabled = second->m_enabled = true;
            auto all = search->applyTagFilters(songs);
            search->updateFutureCount(all, songs, songs);
            if (first->m_count != 1 || second->m_count != 1 || absent->m_count != 0 ||
                static_cast<OptionsObject*>(artists->objectAtIndex(0))->m_count != 1 ||
                static_cast<OptionsObject*>(artists->objectAtIndex(1))->m_count != 1 ||
                static_cast<OptionsObject*>(artists->objectAtIndex(2))->m_count != 0) return false;
            search->m_matchAnyTags = true;
            auto any = search->applyTagFilters(songs);
            search->updateFutureCount(any, songs, songs);
            return first->m_count == 2 && second->m_count == 2 && absent->m_count == 0 &&
                static_cast<OptionsObject*>(artists->objectAtIndex(0))->m_count == 1 &&
                static_cast<OptionsObject*>(artists->objectAtIndex(1))->m_count == 2 &&
                static_cast<OptionsObject*>(artists->objectAtIndex(2))->m_count == 1;
        });
    });

    runBindingTest("SDK single music filter helpers", [] {
        return withMusicFilters([](MusicSearchResult*, CCArray* songs, CCArray*, CCArray*) {
            auto manager = MusicDownloadManager::sharedState();
            auto duplicate = CCArray::create();
            duplicate->addObjectsFromArray(songs);
            duplicate->addObject(songs->objectAtIndex(0));
            auto artists = manager->filterMusicByArtistID(11, duplicate);
            auto tags = manager->filterMusicByTag(1, duplicate);
            if (artists->count() != 3 || tags->count() != 3) return false;
            for (unsigned int i = 0; i < 3; ++i) {
                auto expected = songs->objectAtIndex(i == 1 ? 1 : 0);
                if (artists->objectAtIndex(i) != expected || tags->objectAtIndex(i) != expected) return false;
            }
            return manager->filterMusicByArtistID(99, songs)->count() == 0 &&
                manager->filterMusicByTag(99, songs)->count() == 0 &&
                manager->filterMusicByArtistID(11, nullptr)->count() == 0 &&
                manager->filterMusicByTag(1, nullptr)->count() == 0 && duplicate->count() == 4;
        });
    });

    runBindingTest("Native music match control", [] {
        auto pool = CCPoolManager::sharedPoolManager();
        pool->push();
        auto pop = [](CCPoolManager* value) { value->pop(); };
        std::unique_ptr<CCPoolManager, decltype(pop)> scope(pool, pop);
        auto search = new AlphaMusicSearchFixture();
        search->autorelease();
        search->init(GJSongType::Music);
        auto option = OptionsObject::create(1, false, "Alpha match fixture", nullptr);
        if (!option) return false;
        option->m_count = 1;
        auto options = CCArray::create();
        options->addObject(option);
        auto popup = OptionsScrollLayer::create(options, true, "Alpha match fixture", search);
        if (!popup || popup->m_searchResult != search) return false;
        popup->addTagMatchToggle();
        auto button = popup->m_matchButton;
        if (!button || button->getParent() != popup->m_buttonMenu) return false;
        auto sprite = static_cast<ButtonSprite*>(button->getNormalImage());
        if (!sprite || std::string(sprite->m_label->getString()) != "Match: All") return false;
        auto size = CCDirector::sharedDirector()->getWinSize();
        auto position = popup->m_buttonMenu->convertToWorldSpace(button->getPosition());
        if (std::abs(position.x - size.width / 2 - 131.f) > 0.01f ||
            std::abs(position.y - size.height / 2 - 121.f) > 0.01f) return false;
        auto children = popup->m_buttonMenu->getChildrenCount();
        popup->addTagMatchToggle();
        if (popup->m_matchButton != button || popup->m_buttonMenu->getChildrenCount() != children) return false;
        button->activate();
        if (!search->m_matchAnyTags || search->updates != 1 || search->lastSort != search->m_sortType ||
            std::string(sprite->m_label->getString()) != "Match: Any") return false;
        button->activate();
        bool restored = !search->m_matchAnyTags && search->updates == 2 &&
            std::string(sprite->m_label->getString()) == "Match: All";
        popup->onClose(nullptr);
        return restored;
    });

    runBindingTest("Native achievement lookup", [] {
        auto manager = AchievementManager::sharedState();
        AlphaTestField fixture(manager->m_platformAchievements, CCDictionary::create());
        auto value = CCDictionary::create();
        manager->m_platformAchievements->setObject(value, "alpha-achievement");
        return manager->getAchievementsWithID("alpha-achievement") == value &&
            !manager->getAchievementsWithID("alpha-missing");
    });

    runBindingTest("Native particle allocation boundary", [] {
        if (sizeof(CCParticleSystem) != 0x318 || sizeof(CCParticleSystemQuad) != 0x348 ||
            offsetof(CCParticleSystemQuad, m_pQuads) != 0x318 ||
            offsetof(CCParticleSystemQuad, m_tTextureRect) != 0x330 ||
            offsetof(CCParticleSystemQuad, m_uOpacity) != 0x346) return false;
        alignas(CCParticleSystemQuad) std::array<uint8_t, sizeof(CCParticleSystemQuad) + 32> storage;
        storage.fill(0xa5);
        auto particles = new (storage.data()) CCParticleSystemQuad();
        auto destroy = [](CCParticleSystemQuad* value) { value->~CCParticleSystemQuad(); };
        std::unique_ptr<CCParticleSystemQuad, decltype(destroy)> lifetime(particles, destroy);
        if (!particles->initWithTotalParticles(4, false) || particles->getTotalParticles() != 4 ||
            !particles->m_pQuads || !particles->m_pIndices || particles->m_uOpacity != 255) return false;
        particles->m_particleDrawCost = 10.f;
        if (std::abs(particles->getParticleDrawCost() - 0.78539819f) > 0.00001f) return false;
        lifetime.reset();
        return std::all_of(storage.begin() + sizeof(CCParticleSystemQuad), storage.end(),
            [](uint8_t byte) { return byte == 0xa5; });
    });

    runBindingTest("Native shader construction and split tweens", [] {
        alignas(GJShaderState) std::array<uint8_t, sizeof(GJShaderState) + 32> storage;
        storage.fill(0xa5);
        auto state = new (storage.data()) GJShaderState();
        auto destroy = [](GJShaderState* value) { value->~GJShaderState(); };
        std::unique_ptr<GJShaderState, decltype(destroy)> lifetime(state, destroy);
        if (sizeof(GJShaderState) != 0x2d0 || offsetof(GJShaderState, m_splitActive) != 0x2c0 ||
            !state->m_tweenActions.empty() || !state->m_tweenTimes.empty() ||
            state->m_time != 0.0 || state->m_prevTime != -1.0 || state->m_startTime != 0.0 ||
            state->m_textureScaleX != 1.f || state->m_textureScaleY != 1.f ||
            state->m_cGSegmentHeight != 1.f || state->m_pixelateTargetX != 1.f ||
            state->m_pixelateTargetY != 1.f || state->m_lensCircleSize != 1.f ||
            state->m_motionBlurFollowEaseX != 1.f || state->m_motionBlurFollowEaseY != 1.f ||
            state->m_invertColorR != 1.f || state->m_invertColorG != 1.f ||
            state->m_invertColorB != 1.f || state->m_colorChangeCR != 1.f ||
            state->m_colorChangeCG != 1.f || state->m_colorChangeCB != 1.f ||
            state->m_splitActive || state->m_minBlendingLayer || state->m_usesShaders) return false;
        state->updateTweenAction(3.25f, 45);
        state->updateTweenAction(7.5f, 46);
        if (state->m_splitTargetRows != 3.25 || state->m_splitTargetCols != 7.5 ||
            state->m_splitActive || state->m_minBlendingLayer) return false;
        state->m_time = 12.0;
        state->tweenValue(0.f, 10.f, 1, 2.f, 0, 2.f);
        if (state->m_tweenActions.size() != 1 || state->m_tweenTimes.at(1) != 12.0) return false;
        state->updateTweenActions(1.f);
        if (std::abs(state->m_chromaticTargetX - 5.f) > 0.001f) return false;
        state->updateTweenActions(1.f);
        if (!state->m_tweenActions.empty() || state->m_chromaticTargetX != 10.f) return false;
        state->reset();
        lifetime.reset();
        return std::all_of(storage.begin() + sizeof(GJShaderState), storage.end(),
            [](uint8_t byte) { return byte == 0xa5; });
    });

    runBindingTest("Native trigger toggle controls and delegate guard", [] {
        auto popup = std::make_unique<AlphaTriggerToggleFixture>();
        if (!popup->init(nullptr, nullptr, 240.f, 120.f, 1, 0.f)) return false;
        for (int property : {7171, 7172}) {
            auto nodes = popup->createToggleValueControlAdvanced(property, "Alpha fixture", {120.f, 60.f},
                false, 0, 0, 0.5f, 0.3f, 100.f, {0.f, 0.f});
            if (!nodes || !nodes->count()) return false;
        }
        auto first = static_cast<CCMenuItemToggler*>(popup->m_valueToggles->objectForKey(7171));
        auto second = static_cast<CCMenuItemToggler*>(popup->m_valueToggles->objectForKey(7172));
        if (!first || !second || first->m_toggled || second->m_toggled) return false;
        popup->updates.clear();
        popup->changes.clear();
        popup->guarded = true;
        popup->m_disableTextDelegate = false;
        first->activate();
        if (!first->m_toggled || second->m_toggled || popup->m_disableTextDelegate ||
            popup->updates != std::vector<std::pair<int, bool>>{{7171, true}} ||
            popup->changes != std::vector<std::pair<int, float>>{{7171, 1.f}}) return false;
        popup->m_disableTextDelegate = true;
        first->activate();
        second->activate();
        return !first->m_toggled && second->m_toggled && popup->m_disableTextDelegate && popup->guarded &&
            popup->updates == std::vector<std::pair<int, bool>>{{7171, true}, {7171, false}, {7172, true}} &&
            popup->changes == std::vector<std::pair<int, float>>{{7171, 1.f}, {7171, 0.f}, {7172, 1.f}};
    });

    runBindingTest("Native find popup group value boundary", [] {
        alignas(FindObjectPopup) std::array<uint8_t, sizeof(FindObjectPopup) + 32> storage;
        storage.fill(0xa5);
        auto popup = new (storage.data()) FindObjectPopup();
        auto destroy = [](FindObjectPopup* value) { value->~FindObjectPopup(); };
        std::unique_ptr<FindObjectPopup, decltype(destroy)> lifetime(popup, destroy);
        auto& group = popup->m_groupID;
        if (sizeof(FindObjectPopup) != 0x488 || offsetof(FindObjectPopup, m_groupID) != 0x480 || group != 0) return false;
        popup->valueDidChange(51, 654321.f);
        if (group != 654321) return false;
        popup->valueDidChange(50, 7.f);
        if (group != 654321) return false;
        popup->valueDidChange(51, -23.75f);
        if (group != -23) return false;
        lifetime.reset();
        return std::all_of(storage.begin() + sizeof(FindObjectPopup), storage.end(),
            [](uint8_t byte) { return byte == 0xa5; });
    });

    runBindingTest("SDK find popup native window initialization", [] {
        return withNativeEditor(false, [](LevelEditorLayer* editor) {
            AlphaTestField variables(GameManager::get()->m_valueKeeper, CCDictionary::create());
            auto popup = std::make_unique<FindObjectPopup>();
            if (!popup->init(editor->m_editorUI) || !popup->m_mainLayer || !popup->m_inputNodes ||
                !popup->m_inputNodes->objectForKey(51)) return false;
            auto bytes = reinterpret_cast<uint8_t*>(popup.get());
            if (*reinterpret_cast<EditorUI**>(bytes + 0x3e8) != editor->m_editorUI ||
                *reinterpret_cast<int*>(bytes + 0x470) != 3 ||
                *reinterpret_cast<float*>(bytes + 0x410) != 200.f ||
                *reinterpret_cast<int*>(bytes + 0x480) != 0) return false;
            popup->valueDidChange(51, 4321.f);
            if (popup->m_groupID != 4321 || GameManager::get()->getGameVariable("0232")) return false;
            popup->onFindObjectID(nullptr);
            if (popup->m_groupID != 4321 || !GameManager::get()->getGameVariable("0232")) return false;
            popup->onFindObjectID(nullptr);
            popup->runFind(0);
            popup->onFieldEnter();
            popup->onSelect(nullptr);
            auto sender = CCString::create("");
            for (int tag : {1, 2}) {
                sender->setTag(tag);
                popup->onFindStep(sender);
            }
            return popup->m_groupID == 4321 && !GameManager::get()->getGameVariable("0232");
        });
    });

    runBindingTest("Native gameplay event dispatch and frame cache", [] {
        auto layer = std::make_unique<AlphaEventFixture>();
        auto& state = layer->m_gameState;
        if (offsetof(GJGameState, m_commandIndex) != 0x238 ||
            offsetof(GJGameState, m_unkMapPairGJGameEventIntVectorEventTriggerInstance) != 0x3e0 ||
            offsetof(GJGameState, m_unkMapPairGJGameEventIntInt) != 0x3f0) return false;
        auto& events = state.m_unkMapPairGJGameEventIntVectorEventTriggerInstance;
        auto& frames = state.m_unkMapPairGJGameEventIntInt;
        events.clear();
        frames.clear();
        auto event = GJGameEvent::NormalJump;
        auto add = [&](int key, int target, bool inactive) {
            EventTriggerInstance entry;
            entry.m_targetID = target;
            entry.m_uniqueID = target + 100;
            entry.m_controlID = target + 200;
            entry.m_inactive = inactive;
            entry.m_remapKeys = {31, 47};
            events[{event, key}].push_back(entry);
        };
        add(40001, 17, false);
        add(40001, 99, true);
        add(40000, 18, false);
        add(40002, 19, false);
        state.m_commandIndex = 71;
        layer->gameEventTriggered(event, 4, 1);
        if (layer->calls.size() != 2 || layer->calls[0].group != 17 || layer->calls[1].group != 18 ||
            frames.at({event, 40001}) != 71 || frames.at({event, 40000}) != 71) return false;
        layer->gameEventTriggered(event, 4, 1);
        if (layer->calls.size() != 2) return false;
        layer->gameEventTriggered(event, 4, 2);
        if (layer->calls.size() != 3 || layer->calls.back().group != 19 ||
            frames.at({event, 40002}) != 71) return false;
        state.m_commandIndex = 72;
        layer->gameEventTriggered(event, 4, 1);
        layer->gameEventTriggered(event, 4, 0);
        if (layer->calls.size() != 5 || frames.at({event, 40000}) != 72) return false;
        events.at({event, 40001})[0].m_inactive = true;
        state.m_commandIndex = 73;
        layer->gameEventTriggered(event, 4, 1);
        layer->gameEventTriggered(event, 5, 0);
        if (layer->calls.size() != 6 || layer->calls.back().group != 18 ||
            frames.size() != 4 || frames.at({event, 50000}) != 73) return false;
        return std::all_of(layer->calls.begin(), layer->calls.end(), [](auto const& call) {
            return !call.ordered && call.delay == 0.0 && call.remap == std::vector<int>{31, 47} &&
                call.trigger == call.group + 100 && call.control == call.group + 200;
        });
    });

    runBindingTest("Native level option initialization and callbacks", [] {
        return withNativeEditor(true, [](LevelEditorLayer* editor) {
            auto settings = editor->m_levelSettings;
            auto popup = std::make_unique<LevelOptionsLayer>();
            if (sizeof(LevelOptionsLayer) != 0x478 || offsetof(LevelOptionsLayer, m_settingsObject) != 0x470 ||
                !popup->init(settings) || popup->m_settingsObject != settings ||
                popup->m_gap != 40.f || popup->m_maxLabelScale != 0.4f ||
                !popup->m_values || !popup->m_variables || !popup->m_mainLayer) return false;
            auto mini = settings->m_startMini;
            popup->didToggle(0);
            if (settings->m_startMini == mini) return false;
            popup->didToggle(0);
            if (settings->m_startMini != mini) return false;
            popup->valueDidChange(13, 23.75f);
            if (offsetof(LevelSettingsObject, m_spawnGroup) != 0x1c4 ||
                settings->m_spawnGroup != 23 || popup->getValue(13) != 23.f) return false;
            popup->valueDidChange(-1, 91.f);
            if (popup->getValue(-1) != 0.f || settings->m_spawnGroup != 23) return false;
            auto legacy = std::make_unique<LevelOptionsLayer2>();
            return legacy->init(settings) && legacy->m_settingsObject == settings && legacy->m_mainLayer &&
                legacy->m_values && legacy->m_variables;
        });
    });

    runBindingTest("Native middleground construction boundary", [] {
        alignas(GJMGLayer) std::array<uint8_t, sizeof(GJMGLayer) + 32> storage;
        storage.fill(0xa5);
        auto layer = new (storage.data()) GJMGLayer();
        auto valid = sizeof(GJMGLayer) == 0x1e8 &&
            offsetof(GJMGLayer, m_ground1Sprite) == 0x198 &&
            offsetof(GJMGLayer, m_mg1BatchNode) == 0x1d0 &&
            !layer->m_ground1Sprite && !layer->m_ground2Sprite &&
            !layer->m_mg1BatchNode && !layer->m_mg2BatchNode &&
            !layer->m_showGround && layer->m_showGround1 && layer->m_showGround2;
        layer->~GJMGLayer();
        return valid && std::all_of(storage.begin() + sizeof(GJMGLayer), storage.end(),
            [](uint8_t byte) { return byte == 0xa5; });
    });

    runBindingTest("Native middleground initialization", [] {
        AlphaTestField loaded(GameManager::get()->m_loadedMG, GameManager::get()->m_loadedMG);
        for (int index : {1, 2, 3}) {
            auto layer = std::make_unique<GJMGLayer>();
            if (!layer->init(index) || !layer->m_ground1Sprite || !layer->m_ground2Sprite ||
                !layer->m_mg1BatchNode || !layer->m_mg2BatchNode || layer->m_textureWidth <= 0.f ||
                layer->m_ground1Sprite->getParent() != layer->m_mg1BatchNode ||
                layer->m_ground2Sprite->getParent() != layer->m_mg2BatchNode ||
                layer->m_ground2Offset != GJMGLayer::defaultYOffsetForBG2(index)) return false;
            auto expected = unsigned(std::ceil(CCDirector::sharedDirector()->getWinSize().width /
                layer->m_textureWidth) + 1.f);
            for (auto sprite : {layer->m_ground1Sprite, layer->m_ground2Sprite}) {
                if (sprite->getChildrenCount() != expected) return false;
            }
        }
        return true;
    });

    runBindingTest("Native middleground tile resizing", [] {
        auto layer = std::make_unique<GJMGLayer>();
        auto first = CCSprite::createWithSpriteFrameName("infoIcon_001.png");
        auto second = CCSprite::createWithSpriteFrameName("infoIcon_001.png");
        if (!first || !second) return false;
        layer->m_ground1Sprite = first;
        layer->m_ground2Sprite = second;
        layer->addChild(first);
        layer->addChild(second);
        for (bool ground1 : {true, false}) {
            auto selected = ground1 ? first : second;
            auto other = ground1 ? second : first;
            auto otherCount = other->getChildrenCount();
            for (int count : {4, 4, 1, 0, 3}) {
                layer->loadGroundSprites(count, ground1);
                if (selected->getChildrenCount() != count || other->getChildrenCount() != otherCount) return false;
                for (int i = 0; i < count; ++i) {
                    auto tile = static_cast<CCSprite*>(selected->getChildren()->objectAtIndex(i));
                    if (tile->getTexture() != selected->getTexture() || tile->getAnchorPoint() != CCPoint{0.f, 0.f} ||
                        tile->getPosition() != CCPoint{i * tile->getContentSize().width, 0.f}) return false;
                }
            }
        }
        layer->m_ground2Sprite = nullptr;
        layer->loadGroundSprites(5, false);
        return first->getChildrenCount() == 3 && second->getChildrenCount() == 3;
    });

    runBindingTest("Native enter easing buffers and cache", [] {
        return withNativeEditor(true, [](LevelEditorLayer* editor) {
            if (offsetof(GJBaseGameLayer, m_enterEasingValues) != 0x3100 ||
                offsetof(GJBaseGameLayer, m_enterEasingIndices) != 0x3118 ||
                offsetof(GJBaseGameLayer, m_enterEasingValuesIndex) != 0x3158) return false;
            auto& values = editor->m_enterEasingValues;
            auto& indices = editor->m_enterEasingIndices;
            values.clear();
            indices.clear();
            editor->m_enterEasingValuesIndex = 0;
            if (editor->generateEnterEasingBuffer(0, 2.f) != -1) return false;
            for (int type : {3, 7, 8, 9}) {
                if (editor->generateEnterEasingBuffer(type, 2.f) != -2) return false;
            }
            if (editor->generateEnterEasingBuffer(1, -200.f) != -10000 ||
                !values.empty() || !indices.empty() || editor->m_enterEasingValuesIndex != 0) return false;
            int expectedIndex = 0;
            for (int type = 1; type <= 18; ++type) {
                if (type == 3 || type == 7 || type == 8 || type == 9) continue;
                for (float rate : {2.f, 3.f}) {
                    auto index = editor->generateEnterEasingBuffer(type, rate);
                    if (index != expectedIndex || indices.at(type * 10000 + int(rate * 100.f)) != index ||
                        values.size() != index + 101 || editor->m_enterEasingValuesIndex != index + 101) return false;
                    for (int sample = 0; sample <= 100; ++sample) {
                        auto expected = GameToolbox::getEasedValue(sample / 100.f, type, rate);
                        if (std::abs(values[index + sample] - expected) > 0.00001f) return false;
                    }
                    auto count = indices.size();
                    if (editor->generateEnterEasingBuffer(type, rate) != index ||
                        editor->generateEnterEasingBuffer(type, rate + 0.001f) != index ||
                        values.size() != index + 101 || indices.size() != count ||
                        editor->m_enterEasingValuesIndex != index + 101) return false;
                    expectedIndex += 101;
                }
            }
            return values.size() == expectedIndex;
        });
    });

    runBindingTest("SDK enter easing buffer regeneration", [] {
        return withNativeEditor(true, [](LevelEditorLayer* editor) {
            auto objects = CCArray::create();
            AlphaTestField objectState(editor->m_objects, objects);
            auto first = EnterEffectObject::create("infoIcon_001.png");
            auto second = EnterEffectObject::create("infoIcon_001.png");
            auto excluded = EnterEffectObject::create("infoIcon_001.png");
            if (!first || !second || !excluded) return false;
            first->m_objectID = 3006;
            second->m_objectID = 3021;
            excluded->m_objectID = 3016;
            for (auto object : {first, second, excluded}) {
                object->m_easingInType = static_cast<EasingType>(1);
                object->m_easingInRate = 2.f;
                object->m_easingOutType = static_cast<EasingType>(2);
                object->m_easingOutRate = 3.f;
                object->m_easingInBuffer = object->m_easingOutBuffer = 717171;
                objects->addObject(object);
            }
            // The SDK helpers must share the native cache and discard stale entries.
            editor->generateEnterEasingBuffer(4, 2.f);
            editor->generateEnterEasingBuffers(first);
            if (first->m_easingInBuffer != 101 || first->m_easingOutBuffer != 202) return false;
            editor->regenerateEnterEasingBuffers();
            if (editor->m_enterEasingIndices.size() != 2 || editor->m_enterEasingValues.size() != 202 ||
                editor->m_enterEasingValuesIndex != 202) return false;
            for (auto object : {first, second}) {
                if (object->m_easingInBuffer != 0 || object->m_easingOutBuffer != 101) return false;
            }
            if (excluded->m_easingInBuffer != 717171 || excluded->m_easingOutBuffer != 717171) return false;
            first->m_easingInType = EasingType::None;
            first->m_easingOutType = static_cast<EasingType>(3);
            objects->removeObject(second);
            editor->regenerateEnterEasingBuffers();
            return first->m_easingInBuffer == -1 && first->m_easingOutBuffer == -2 &&
                editor->m_enterEasingIndices.empty() && editor->m_enterEasingValues.empty() &&
                editor->m_enterEasingValuesIndex == 0;
        });
    });

    runBindingTest("Native custom enter effect registration", [] {
        return withNativeEditor(true, [](LevelEditorLayer* editor) {
            if (sizeof(EnterEffectInstance) != 0xe8 ||
                offsetof(EnterEffectObject, m_enterChannel) != 0x800) return false;
            auto first = EnterEffectObject::create("infoIcon_001.png");
            auto second = EnterEffectObject::create("infoIcon_001.png");
            if (!first || !second) return false;
            auto& state = editor->m_gameState;
            if (!state.m_enterEffectInstanceVectors.empty() || !state.m_exitEffectInstanceVectors.empty()) return false;
            first->m_enterChannel = second->m_enterChannel = 3;
            first->m_targetGroupID = 17;
            first->m_centerGroupID = 29;
            first->m_length = 150;
            first->m_effectID = second->m_effectID = 41;
            editor->addCustomEnterEffect(first, true);
            auto& effects = state.m_enterEffectInstanceVectors.at(3);
            if (effects.size() != 1 || effects.front().m_gameObject != first ||
                effects.front().m_targetID != 17 || effects.front().m_centerID != 29 ||
                effects.front().m_length != 150.f) return false;
            first->m_length = 175;
            editor->addCustomEnterEffect(first, true);
            if (effects.size() != 1 || effects.front().m_length != 175.f) return false;
            editor->addCustomEnterEffect(second, true);
            if (effects.size() != 1 || effects.front().m_gameObject != second) return false;
            editor->addCustomEnterEffect(first, false);
            if (state.m_exitEffectInstanceVectors.at(3).size() != 1 ||
                state.m_exitEffectInstanceVectors.at(3).front().m_gameObject != first) return false;
            first->m_enterChannel = 101;
            editor->addCustomEnterEffect(first, true);
            return !state.m_enterEffectInstanceVectors.contains(101) && effects.size() == 1;
        });
    });

    runBindingTest("Native custom enter effect removal", [] {
        return withNativeEditor(true, [](LevelEditorLayer* editor) {
            auto first = EnterEffectObject::create("infoIcon_001.png");
            auto second = EnterEffectObject::create("infoIcon_001.png");
            if (!first || !second) return false;
            editor->stopCustomEnterEffect(first, true);
            auto& state = editor->m_gameState;
            if (state.m_enterChannelMap.size() < 101 || state.m_exitChannelMap.size() < 101) return false;
            first->m_enterChannel = second->m_enterChannel = 4;
            first->m_effectID = 41;
            second->m_effectID = 42;
            editor->addCustomEnterEffect(first, true);
            editor->addCustomEnterEffect(second, true);
            state.m_enterChannelMap[4] = -15;
            editor->stopCustomEnterEffect(first, true);
            auto& effects = state.m_enterEffectInstanceVectors.at(4);
            if (effects.size() != 1 || effects.front().m_gameObject != second ||
                state.m_enterChannelMap[4] != -15) return false;
            editor->stopCustomEnterEffect(second, true);
            if (!effects.empty() || state.m_enterChannelMap[4] != -2) return false;
            editor->addCustomEnterEffect(first, false);
            state.m_exitChannelMap[4] = -15;
            first->m_effectID = 0;
            editor->stopCustomEnterEffect(first, false);
            if (state.m_exitEffectInstanceVectors.contains(4) || state.m_exitChannelMap[4] != -2) return false;
            first->m_enterChannel = 0;
            editor->addCustomEnterEffect(first, true);
            first->m_enterChannel = -10;
            state.m_enterChannelMap[0] = -15;
            editor->stopCustomEnterEffect(first, true);
            return !state.m_enterEffectInstanceVectors.contains(0) && state.m_enterChannelMap[0] == -2;
        });
    });

    runBindingTest("Native extra game layer transforms", [] {
        return withNativeEditor(true, [](LevelEditorLayer* editor) {
            auto source = editor->m_objectLayer;
            auto inside = editor->m_inShaderObjectLayer;
            auto above = editor->m_aboveShaderObjectLayer;
            if (!source || !inside || !above) return false;
            source->setPosition({37.f, -53.f});
            source->setScaleX(0.75f);
            source->setScaleY(1.25f);
            editor->updateExtraGameLayers();
            for (auto target : {inside, above}) {
                if (target->getPosition() != source->getPosition() ||
                    target->getScaleX() != 0.75f || target->getScaleY() != 1.25f) return false;
            }
            return true;
        });
    });

    runBindingTest("Native screen button positioning", [] {
        auto config = std::make_unique<UIButtonConfig>();
        config->reset();
        auto node = GJUINode::create(*config);
        if (!node || !typeinfo_cast<GJUINode*>(node) || !node->m_firstSprite || !node->m_secondSprite) return false;
        node->m_firstSprite->setScale(0.75f);
        node->m_radius = 18.f;
        node->m_modeB = false;
        node->updateButtonPositions();
        float distance = node->m_firstSprite->getContentSize().width * 0.75f * 0.5f + 18.f;
        if (node->m_firstSprite->getPosition() != CCPoint(-distance, 0.f) ||
            node->m_secondSprite->getPosition() != CCPoint(distance, 0.f)) return false;
        node->updateButtonScale(1.5f);
        distance = node->m_firstSprite->getContentSize().width * 1.5f * 0.5f + 18.f;
        if (node->m_firstSprite->getPositionX() != -distance ||
            node->m_secondSprite->getPositionX() != distance) return false;
        node->updateDragRadius(32.f);
        distance += 14.f;
        if (node->m_firstSprite->getPositionX() != -distance ||
            node->m_secondSprite->getPositionX() != distance) return false;
        auto secondPosition = node->m_secondSprite->getPosition();
        node->m_modeB = true;
        node->updateButtonPositions();
        if (node->m_firstSprite->getPosition() != CCPoint(0.f, 0.f) ||
            node->m_secondSprite->getPosition() != secondPosition) return false;
        config->resetOneBtn();
        auto single = GJUINode::create(*config);
        if (!single || !single->m_firstSprite || single->m_secondSprite) return false;
        auto position = single->m_firstSprite->getPosition();
        single->updateButtonPositions();
        return single->m_firstSprite->getPosition() == position;
    });

    runBindingTest("Native single button configuration reset", [] {
        if (sizeof(UIButtonConfig) != 0x28) return false;
        alignas(UIButtonConfig) std::array<uint8_t, sizeof(UIButtonConfig) + 32> storage;
        storage.fill(0xa5);
        auto config = new (storage.data()) UIButtonConfig();
        auto defaults = [config] {
            return config->m_width == 200 && config->m_height == 200 && config->m_deadzone == 0.f &&
                config->m_scale == 1.f && config->m_opacity == 255 && config->m_radius == 10.f &&
                !config->m_modeB && !config->m_snap && config->m_position == CCPoint(95.f, 196.f) &&
                config->m_oneButton && !config->m_player2 && !config->m_split;
        };
        config->resetOneBtn();
        if (!defaults()) return false;
        config->m_width = 17;
        config->m_modeB = config->m_snap = config->m_player2 = config->m_split = true;
        config->m_position = CCPoint {-100.f, 200.f};
        config->resetOneBtn();
        auto valid = defaults();
        config->~UIButtonConfig();
        return valid && std::all_of(storage.begin() + sizeof(UIButtonConfig), storage.end(),
            [](uint8_t byte) { return byte == 0xa5; });
    });

    runBindingTest("Native achievement unlock application", [] {
        auto manager = GameManager::get();
        auto achievements = AchievementManager::sharedState();
        AlphaTestField settings(manager->m_valueKeeper, CCDictionary::create());
        AlphaTestField definitions(achievements->m_platformAchievements, CCDictionary::create());
        struct Unlock {
            char const* icon;
            int id;
            UnlockType type;
            IconType iconType;
        };
        Unlock unlocks[] = {
            {"icon_717171", 717171, UnlockType::Cube, IconType::Cube},
            {"ship_717172", 717172, UnlockType::Ship, IconType::Ship},
            {"color_7", 7, UnlockType::Col1, IconType::Cube},
            {"color2_8", 8, UnlockType::Col2, IconType::Cube},
        };
        for (auto const& unlock : unlocks) {
            std::string key = std::string(96, 'a') + unlock.icon;
            auto definition = CCDictionary::create();
            definition->setObject(CCString::create(unlock.icon), "icon");
            achievements->m_platformAchievements->setObject(definition, key);
            int id = 0;
            auto type = static_cast<UnlockType>(0);
            manager->getUnlockForAchievement(key, id, type);
            if (id != unlock.id || type != unlock.type) return false;
            manager->completedAchievement(key);
            auto unlockedKey = type == UnlockType::Col1 || type == UnlockType::Col2 ?
                manager->colorKey(id, type) : manager->iconKey(id, unlock.iconType);
            if (manager->m_valueKeeper->valueForKey(unlockedKey)->intValue() != 1) return false;
            auto count = manager->m_valueKeeper->count();
            manager->completedAchievement(key);
            if (manager->m_valueKeeper->count() != count) return false;
        }
        auto count = manager->m_valueKeeper->count();
        manager->completedAchievement("alpha-missing-unlock");
        return manager->m_valueKeeper->count() == count;
    });

    runBindingTest("Native item information factories", [] {
        if (sizeof(ItemInfoPopup) != 0x2a8) return false;
        for (auto type : {UnlockType::Cube, UnlockType::Ship, UnlockType::Col1}) {
            auto popup = ItemInfoPopup::create(1, type);
            if (!popup || popup->m_itemID != 1 || popup->m_unlockType != type ||
                !popup->m_mainLayer || !popup->m_buttonMenu || !popup->getChildrenCount()) return false;
        }
        return true;
    });

    runBindingTest("Native local level deletion", [] {
        return withLevelDeletion([](GameLevelManager* manager, LocalLevelManager* local) {
            auto removed = GJGameLevel::create();
            auto kept = GJGameLevel::create();
            if (!removed || !kept) return false;
            removed->m_levelID = 717171;
            kept->m_levelID = 717172;
            removed->m_levelType = kept->m_levelType = GJLevelType::Editor;
            local->m_localLevels->addObject(removed);
            local->m_localLevels->addObject(kept);
            manager->deleteLevel(removed);
            manager->deleteLevel(removed);
            return local->m_localLevels->count() == 1 && local->m_localLevels->objectAtIndex(0) == kept &&
                manager->m_deathData.size() == 2 && manager->m_deathRelated1.size() == 2 &&
                manager->m_deathRelated2.size() == 2;
        });
    });

    runBindingTest("Native cached level deletion", [] {
        return withLevelDeletion([](GameLevelManager* manager, LocalLevelManager*) {
            auto removed = GJGameLevel::create();
            auto kept = GJGameLevel::create();
            auto progress = GJGameLevel::create();
            auto daily = GJGameLevel::create();
            if (!removed || !kept || !progress || !daily) return false;
            removed->m_levelID = 717171;
            kept->m_levelID = 717172;
            progress->m_levelID = 717173;
            daily->m_levelID = 717174;
            for (auto level : {removed, kept, progress, daily}) {
                level->m_levelType = GJLevelType::Saved;
                level->m_dailyID = 0;
                level->m_normalPercent = 0;
                level->m_practicePercent = 0;
                level->m_levelString.assign(128, 'a');
            }
            progress->m_normalPercent = 25;
            daily->m_dailyID = 12;
            manager->m_onlineLevels->setObject(removed, "717171");
            manager->m_onlineLevels->setObject(kept, "717172");
            manager->m_onlineLevels->setObject(progress, "717173");
            manager->m_dailyLevels->setObject(daily, "12");
            manager->deleteLevel(removed);
            if (manager->m_onlineLevels->objectForKey("717171") ||
                manager->m_deathData.contains(717171) || manager->m_deathRelated1.contains(717171) ||
                manager->m_deathRelated2.contains(717171)) return false;
            manager->deleteLevel(progress);
            if (manager->m_onlineLevels->objectForKey("717173") != progress ||
                !progress->m_levelString.empty() || !progress->m_levelNotDownloaded ||
                progress->m_normalPercent.value() != 25) return false;
            manager->deleteLevel(daily);
            manager->deleteLevel(removed);
            return !manager->m_dailyLevels->objectForKey("12") &&
                manager->m_onlineLevels->objectForKey("717172") == kept &&
                kept->m_levelString.size() == 128 && manager->m_deathData.contains(717172) &&
                manager->m_deathRelated1.at(717172) == "preserved" && manager->m_deathRelated2.at(717172) == 2;
        });
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
        AlphaTestField accountNames(manager->m_accountIdToUsernameMap,
            std::unordered_map<int, std::string>{});
        AlphaTestField downloads(manager->m_downloadObjects, CCDictionary::create());
        manager->storeUserName(717171, 818181, "Alpha Native Fixture");
        auto valid = std::string(manager->m_knownUsers->valueForKey("717171")->getCString()) ==
                "Alpha Native Fixture" &&
            manager->m_accountIdToUsernameMap.at(818181) == "Alpha Native Fixture" &&
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

    runBindingTest("Native active path selection", [] {
        auto manager = GameStatsManager::sharedState();
        if (offsetof(GameStatsManager, m_activePath) != 0x3e4 || sizeof(GJStoreItem) != 0x178) return false;
        AlphaTestField store(manager->m_allStoreItems, CCDictionary::create());
        AlphaTestField chestItems(manager->m_allTreasureRoomChestItems, CCDictionary::create());
        AlphaTestField specialItems(manager->m_allSpecialChestItems, CCDictionary::create());
        AlphaTestField purchased(manager->m_purchasedItems, CCDictionary::create());
        AlphaTestField stats(manager->m_playerStats, CCDictionary::create());
        AlphaTestField dictionaryStats(manager->m_usePlayerStatsCCDictionary, true);
        AlphaTestField active(manager->m_activePath, 42);
        manager->trySelectActivePath();
        if (manager->m_activePath != 0) return false;
        for (int path : {30, 31, 39}) {
            auto item = GJStoreItem::create(717171 + path, path - 24, 12, 0, ShopType::Normal);
            if (!item) return false;
            manager->m_allStoreItems->setObject(item, manager->getItemKey(path - 24, 12));
        }
        if (manager->isItemUnlocked(UnlockType::GJItem, 6)) return false;
        manager->m_purchasedItems->setObject(CCString::create("1"), "717201");
        manager->m_purchasedItems->setObject(CCString::create("1"), "717202");
        if (!manager->isItemUnlocked(UnlockType::GJItem, 6) ||
            !manager->isItemUnlocked(UnlockType::GJItem, 7)) return false;
        manager->m_activePath = 31;
        manager->trySelectActivePath();
        if (manager->m_activePath != 31) return false;
        manager->m_playerStats->setObject(CCString::create("1000"), "31");
        manager->trySelectActivePath();
        if (manager->m_activePath != 30) return false;
        manager->m_playerStats->setObject(CCString::create("1000"), "30");
        manager->trySelectActivePath();
        if (manager->m_activePath != 0) return false;
        manager->m_purchasedItems->setObject(CCString::create("1"), "717210");
        manager->m_playerStats->setObject(CCString::create("999"), "39");
        manager->trySelectActivePath();
        if (manager->m_activePath != 39) return false;
        manager->m_playerStats->setObject(CCString::create("1000"), "39");
        manager->trySelectActivePath();
        return manager->m_activePath == 0;
    });

    runBindingTest("Native video option construction", [] {
        return checkOptionConstruction<MoreVideoOptionsLayer>();
    });

    runBindingTest("Native parental option construction", [] {
        return checkOptionConstruction<ParentalOptionsLayer>();
    });

    runBindingTest("Native option page counts", [] {
        return checkOptionPageCounts<GJOptionsLayer>(0x408);
    });

    runBindingTest("Native video page counts", [] {
        return checkOptionPageCounts<MoreVideoOptionsLayer>(0x2a8);
    });

    runBindingTest("Native parental option page counts", [] {
        return checkOptionPageCounts<ParentalOptionsLayer>(0x2a8);
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
        manager->m_adminDelegate = &callback;
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

    runBindingTest("Native queued music value ownership and playback", [] {
        return withNativeMusicChannels([](FMODAudioEngine* engine, FMOD::Channel* first, FMOD::Channel* second) {
            if (sizeof(FMODQueuedMusic) != 0x68 || offsetof(FMODQueuedMusic, m_stopMusic) != 0x44 ||
                offsetof(FMODQueuedMusic, m_channelID) != 0x48) return false;
            FMODQueuedMusic music{};
            music.m_filePath = std::string(128, 'q');
            music.m_musicID = 717171;
            music.m_channelID = 717172;
            engine->triggerQueuedMusic(music);
            bool firstPaused = true;
            bool secondPaused = false;
            return first->getPaused(&firstPaused) == FMOD_OK && !firstPaused &&
                second->getPaused(&secondPaused) == FMOD_OK && secondPaused &&
                music.m_filePath == std::string(128, 'q') && music.m_musicID == 717171 &&
                music.m_channelID == 717172 && !music.m_stopMusic;
        });
    });

    runBindingTest("Native queued music stop and channel swap", [] {
        return withNativeMusicChannels([](FMODAudioEngine* engine, FMOD::Channel* first, FMOD::Channel* second) {
            FMODQueuedMusic music{};
            music.m_filePath = std::string(128, 'q');
            music.m_musicID = 717171;
            music.m_channelID = 717172;
            music.m_stopMusic = true;
            engine->triggerQueuedMusic(music);
            bool paused = true;
            bool playing = true;
            auto stopped = second->isPlaying(&playing);
            return first->getPaused(&paused) == FMOD_OK && !paused &&
                (stopped == FMOD_ERR_INVALID_HANDLE || (stopped == FMOD_OK && !playing)) &&
                engine->m_fmodMusic.at(717172).m_channelID == 818181 &&
                engine->m_audioState.m_soundStateForChannels.at(818181).m_musicID == 717172 &&
                engine->m_audioState.m_soundStateForChannels.at(818182).m_musicID == 717171 &&
                engine->m_audioState.m_volumeForMusicChannels.at(717172) == .25f &&
                engine->m_audioState.m_volumeForMusicChannels.at(717171) == .75f &&
                music.m_musicID == 717171 && music.m_filePath == std::string(128, 'q');
        });
    });

    runBindingTest("Native music start and resume control", [] {
        return withNativeMusicChannels([](FMODAudioEngine* engine, FMOD::Channel* first, FMOD::Channel* second) {
            engine->startMusic(0, 0, 0, 0, false, 717171, true, false);
            bool paused = false;
            if (first->getPaused(&paused) != FMOD_OK || !paused) return false;
            engine->startMusic(0, 0, 0, 0, false, 717171, false, true);
            if (first->getPaused(&paused) != FMOD_OK || paused) return false;
            engine->startMusic(0, 0, 0, 0, false, 717172, true);
            if (second->getPaused(&paused) != FMOD_OK || !paused) return false;
            engine->startMusic(0, 0, 0, 0, false, 717172, false);
            if (second->getPaused(&paused) != FMOD_OK || paused) return false;
            second->setPaused(true);
            return second->getPaused(&paused) == FMOD_OK && paused;
        });
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

    runBindingTest("Native object common initialization", [] {
        auto object = GameObject::createWithFrame("GJ_infoIcon_001.png");
        if (!object || !object->m_baseColor) return false;
        auto color = object->m_baseColor;
        auto previousID = object->m_uniqueID;
        color->m_colorID = 23;
        color->m_defaultColorID = 24;
        object->setScaleX(2.f);
        object->setScaleY(3.f);
        object->commonSetup();
        return object->m_baseColor == color && color->m_colorID == 0 &&
            color->m_defaultColorID == 1004 && object->m_uniqueID != previousID &&
            object->getScaleX() == 1.f && object->getScaleY() == 1.f;
    });

    runBindingTest("Native object custom color child", [] {
        auto object = GameObject::createWithFrame("GJ_infoIcon_001.png");
        if (!object) return false;
        auto first = object->addCustomColorChild("GJ_infoIcon_001.png");
        if (!first || object->m_colorSprite != first || !object->m_detailColor ||
            object->m_detailColor->m_colorID != 0 || object->m_detailColor->m_defaultColorID != 1) return false;
        Ref<CCSprite> kept(first);
        auto references = first->retainCount();
        auto color = object->m_detailColor;
        auto second = object->addCustomColorChild("GJ_infoIcon_001.png");
        return second && second != first && object->m_colorSprite == second &&
            object->m_detailColor == color && first->retainCount() + 1 == references &&
            second->getContentSize().width > 0 && second->getContentSize().height > 0;
    });

    runBindingTest("Native object reserved color blending", [] {
        auto object = GameObject::createWithFrame("GJ_infoIcon_001.png");
        if (!object || !object->m_baseColor) return false;
        AlphaTestField editorState(object->m_editorEnabled, false);
        AlphaTestField suppressed(object->m_unk353, false);
        GJSpriteColor color;
        if (object->shouldBlendColor(&color, true)) return false;
        for (int id : {1005, 1006, 1007}) {
            color.m_colorID = id;
            if (!object->shouldBlendColor(&color, true) || !object->shouldBlendColor(&color, false)) return false;
        }
        color.m_colorID = 0;
        color.m_defaultColorID = 1007;
        if (!object->shouldBlendColor(&color, true)) return false;
        color.m_defaultColorID = 1012;
        object->m_baseColor->m_colorID = 1007;
        if (object->shouldBlendColor(&color, true) || !object->shouldBlendColor(&color, false)) return false;
        object->m_baseColor->m_colorID = 0;
        object->m_baseColor->m_defaultColorID = 0;
        if (object->shouldBlendColor(&color, false)) return false;
        color.m_defaultColorID = 1007;
        object->m_unk353 = true;
        return !object->shouldBlendColor(&color, true);
    });

    runBindingTest("Native editor object selection filters", [] {
        return withNativeEditor(false, [](LevelEditorLayer* editor) {
            auto manager = GameManager::get();
            AlphaTestField settings(manager->m_valueKeeper, CCDictionary::create());
            auto object = editor->createObject(1, {30.f, 30.f}, true);
            if (!object || !object->m_baseColor || !editor->m_editorUI->canSelectObject(object)) return false;
            auto variables = manager->m_valueKeeper;
            auto set = [variables](char const* key, int value) {
                variables->setObject(CCString::createWithFormat("%i", value), key);
            };
            set("gv_0064", 1);
            set("gv_0005", 3);
            set("gv_0006", 2);
            if (editor->m_editorUI->canSelectObject(object)) return false;
            set("gv_0006", 1);
            if (!editor->m_editorUI->canSelectObject(object)) return false;
            set("gv_0064", 0);
            set("gv_0133", 23);
            if (editor->m_editorUI->canSelectObject(object)) return false;
            object->addToGroup(23);
            if (!editor->m_editorUI->canSelectObject(object)) return false;
            set("gv_0133", 24);
            if (editor->m_editorUI->canSelectObject(object)) return false;
            set("gv_0133", 0);
            set("gv_0139", 1007);
            GJSpriteColor detail;
            AlphaTestField detailState(object->m_detailColor, &detail);
            object->m_baseColor->m_colorID = 0;
            object->m_baseColor->m_defaultColorID = 1007;
            if (!editor->m_editorUI->canSelectObject(object)) return false;
            object->m_baseColor->m_defaultColorID = 1005;
            if (editor->m_editorUI->canSelectObject(object)) return false;
            detail.m_colorID = 1007;
            return editor->m_editorUI->canSelectObject(object);
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
        bool valid = !popup->m_title && !popup->m_okButton && !popup->m_delegate &&
            popup->m_minimum == 4 && popup->m_maximum == 4 &&
            popup->m_prefixString.empty() && popup->m_inputString.empty();
        popup->m_prefixString = std::string(96, 'p');
        popup->m_inputString = std::string(96, '7');
        popup.reset();
        return valid && std::all_of(storage.begin() + sizeof(NumberInputLayer), storage.end(),
            [](uint8_t value) { return value == 0xa5; });
    });

    runBindingTest("Native number input controls and delegate", [] {
        auto popup = NumberInputLayer::create();
        if (!popup || !popup->m_inputLabel || !popup->m_title || !popup->m_okButton) return false;
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
        popup->m_prefixString = "ID: ";
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
        return popup && popup->m_inputLabel && popup->m_title && popup->m_okButton &&
            !popup->m_prefixString.empty() &&
            std::string(popup->m_inputLabel->getString()) == popup->m_prefixString + popup->m_inputString;
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

    runBindingTest("Native manager save callback slots", [] {
        using geode::addresser::getVirtualOffset;
        return getVirtualOffset(&GManager::didEncodeForSave) == 0x388 &&
            getVirtualOffset(&GManager::saveFinished) == 0x390 &&
            getVirtualOffset(&GManager::dataLoaded) == 0x398 &&
            getVirtualOffset(&GManager::firstLoad) == 0x3a0 &&
            getVirtualOffset(&GameManager::dataLoaded) == 0x398 &&
            getVirtualOffset(&LocalLevelManager::saveFinished) == 0x390;
    });

    runBindingTest("Native gameplay virtual slots", [] {
        using geode::addresser::getVirtualOffset;
        return getVirtualOffset(&GJBaseGameLayer::shouldDrawDebugHitboxes) == 0x488 &&
            getVirtualOffset(&GJBaseGameLayer::getHitboxTrailMax) == 0x4a0 &&
            getVirtualOffset(&GJBaseGameLayer::addToSection) == 0x4a8 &&
            getVirtualOffset(&LevelEditorLayer::addToGroup) == 0x4b0 &&
            getVirtualOffset(&PlayLayer::addToGroup) == 0x4b0;
    });

    runBindingTest("Native SFX delegate virtual slots", [] {
        using geode::addresser::getVirtualOffset;
        return getVirtualOffset(&CustomSFXDelegate::getActiveCustomSFXID) == 0x10 &&
            getVirtualOffset(&CustomSFXDelegate::overridePlaySFX) == 0x18 &&
            getVirtualOffset(&TableViewCellDelegate::getSelectedCellCustomIdx) == 0x10 &&
            getVirtualOffset(&TableViewCellDelegate::shouldSnapToSelected) == 0x38 &&
            getVirtualOffset(&TableViewCellDelegate::getCellDelegateType) == 0x40 &&
            getVirtualOffset(&SFXBrowser::inCustomSFXTree) == 0x28;
    });

    runBindingTest("Native gameplay virtual dispatch", [] {
        return withNativeEditor(false, [](LevelEditorLayer* editor) {
            auto base = static_cast<GJBaseGameLayer*>(editor);
            return base->getHitboxTrailMax() == 72000 &&
                editor->GJBaseGameLayer::getHitboxTrailMax() == 480 &&
                base->shouldDrawDebugHitboxes() == editor->LevelEditorLayer::shouldDrawDebugHitboxes() &&
                base->shouldDrawHitboxTrail() == editor->LevelEditorLayer::shouldDrawHitboxTrail() &&
                base->shouldRecordHitboxTrail() == editor->LevelEditorLayer::shouldRecordHitboxTrail();
        });
    });

    runBindingTest("Native typed SFX delegate callbacks", [] {
        AlphaSFXReceiver receiver;
        auto custom = static_cast<CustomSFXDelegate*>(&receiver);
        AlphaTableSelection selection;
        auto table = static_cast<TableViewCellDelegate*>(&selection);
        return custom->getActiveCustomSFXID() == 717171 && custom->overridePlaySFX(nullptr) &&
            receiver.played && table->getSelectedCellCustomIdx() == 717171 &&
            table->allowsCustomSFXActions() && table->isCustomIDSelected(717171) &&
            !table->isCustomIDSelected(717172) && table->inCustomSFXTree() &&
            table->showsSFXInfoButton() && !table->shouldSnapToSelected() &&
            table->getCellDelegateType() == -1 &&
            selection.TableViewCellDelegate::getSelectedCellCustomIdx() == 0 &&
            !selection.TableViewCellDelegate::allowsCustomSFXActions();
    });

    runBindingTest("Native tab and browser layouts", [] {
        using geode::addresser::getThunkOffset;
        return sizeof(TabbedPopupLayer) == 0x430 && sizeof(GameSettingsLayer) == 0x4a8 &&
            sizeof(LevelBrowserLayer) == 0x348 && sizeof(LevelListLayer) == 0x408 &&
            getThunkOffset(&GameSettingsLayer::dropDownLayerWillClose) == 0x430 &&
            getThunkOffset(&LevelListLayer::textChanged) == 0x348 &&
            getThunkOffset(&LevelListLayer::levelListDeleteFinished) == 0x360;
    });

    runBindingTest("Native tab and browser construction boundaries", [] {
        auto manager = GameManager::sharedState();
        AlphaTestField scene(manager->m_sceneEnum, manager->m_sceneEnum);
        return checkConstructionBoundary<AlphaTabbedFixture>() &&
            checkConstructionBoundary<AlphaBrowserFixture>() &&
            checkConstructionBoundary<AlphaListFixture>();
    });

    runBindingTest("SDK main level list and native metadata", [] {
        auto levels = LevelTools::getLevelList();
        if (levels.size() != 43) return false;
        for (int id : levels) {
            auto level = LevelTools::getLevel(id, true);
            // The native progress set includes other editions' IDs. This
            // executable's metadata getter falls back to level 1 for those.
            auto expectedID = id <= 22 || id == 3001 || (id >= 5001 && id <= 5004) ? id : 1;
            if (!level || level->m_levelID != expectedID || level->m_levelName.empty()) return false;
        }
        for (int id : {1, 22, 3001, 1001, 1003, 2001, 2010, 4001, 4003, 5001, 5004}) {
            if (!levels.contains(id)) return false;
        }
        for (int id : {-1, 0, 23, 1000, 1004, 2000, 2011, 3000, 3002, 4000, 4004, 5000, 5005}) {
            if (levels.contains(id)) return false;
        }
        levels.erase(1);
        levels.insert(-717171);
        auto independent = LevelTools::getLevelList();
        return independent.size() == 43 && independent.contains(1) && !independent.contains(-717171);
    });

    runBindingTest("Native empty timeline positions", [] {
        float speeds[] = {311.5801086425781f, 251.1600799560547f, 387.4201354980469f,
            468.0001525878906f, 576.0001831054688f};
        auto empty = CCArray::create();
        for (int speed = 0; speed < 5; ++speed) {
            for (bool platformer : {false, true}) {
                for (float time : {-1.f, 0.f, 0.5f, 2.f}) {
                    int channel = 717171;
                    auto direct = LevelTools::posForTimeInternal(time, nullptr, speed, platformer,
                        true, true, channel, 77);
                    auto expected = speeds[platformer ? 0 : speed] * time;
                    if (std::abs(direct.x - expected) > 0.001f || direct.y != 0.f || channel != 717171) return false;
                    auto typed = LevelTools::posForTimeInternal(time, nullptr, speed, platformer,
                        true, true, channel);
                    if (typed != direct || channel != 717171) return false;
                    auto wrapper = LevelTools::posForTime(time, empty, speed, platformer, channel);
                    if (direct != wrapper || channel != 717171) return false;
                }
            }
        }
        return true;
    });

    runBindingTest("Native timeline positions match editor", [] {
        return withNativeEditor(true, [](LevelEditorLayer* editor) {
            if (!editor->m_drawGridLayer || !editor->m_drawGridLayer->m_speedObjects ||
                offsetof(EffectGameObject, m_speedStart) != 0x708 ||
                offsetof(EffectGameObject, m_speedModType) != 0x706) return false;
            auto first = typeinfo_cast<EffectGameObject*>(editor->createObject(200, {90.f, 105.f}, false));
            auto second = typeinfo_cast<EffectGameObject*>(editor->createObject(203, {300.f, 105.f}, false));
            if (!first || !second) return false;
            auto grid = editor->m_drawGridLayer;
            grid->addToSpeedObjects(first);
            grid->addToSpeedObjects(second);
            grid->sortSpeedObjects();
            // Native sorting uses each portal's leading edge, not its center.
            if (grid->m_speedObjects->count() < 2 || first->m_speedStart.x <= 0.f ||
                first->m_speedStart.x >= first->getPositionX() ||
                second->m_speedStart.x <= first->m_speedStart.x ||
                second->m_speedStart.x >= second->getPositionX() ||
                first->m_speedModType == second->m_speedModType) return false;
            auto& warp = *reinterpret_cast<float*>(base::get() + 0x8fe4f8);
            auto& axis = *reinterpret_cast<bool*>(base::get() + 0x9097df);
            auto& reverse = *reinterpret_cast<bool*>(base::get() + 0x90bd0c);
            AlphaTestField warpState(warp, warp);
            AlphaTestField axisState(axis, axis);
            AlphaTestField reverseState(reverse, reverse);
            auto settings = editor->m_levelSettings;
            bool affectedByPortals = false;
            for (float time : {0.f, 0.125f, 0.75f, 2.f, 4.f}) {
                int channel = 717171;
                auto actual = LevelTools::posForTimeInternal(time, grid->m_speedObjects,
                    static_cast<int>(settings->m_startSpeed), settings->m_platformerMode,
                    false, false, channel, 77);
                auto expected = editor->posForTime(time);
                affectedByPortals = affectedByPortals || std::abs(expected.x - 311.5801086425781f * time) > 0.001f;
                if (std::abs(actual.x - expected.x) > 0.001f ||
                    std::abs(actual.y - expected.y) > 0.001f ||
                    channel != editor->m_gameState.m_rotateChannel) return false;
            }
            return affectedByPortals;
        });
    });

    runBindingTest("Native browser page and folder labels", [] {
        if (offsetof(LevelBrowserLayer, m_pageText) != 0x2a8 ||
            offsetof(LevelBrowserLayer, m_folderText) != 0x2b8 ||
            offsetof(GJSearchObject, m_page) != 0x1a8 ||
            offsetof(GJSearchObject, m_folder) != 0x1c4) return false;
        auto manager = GameManager::sharedState();
        AlphaTestField scene(manager->m_sceneEnum, manager->m_sceneEnum);
        auto browser = std::make_unique<LevelBrowserLayer>();
        auto search = GJSearchObject::create(SearchType::MyLevels);
        auto page = CCLabelBMFont::create("old page", "bigFont.fnt");
        auto folder = CCLabelBMFont::create("old folder", "bigFont.fnt");
        auto arrow = CCMenuItemSpriteExtra::create(CCSprite::create(), nullptr, nullptr);
        if (!search || !page || !folder || !arrow) return false;
        AlphaTestField searchField(browser->m_searchObject, search);
        AlphaTestField pageField(browser->m_pageText, page);
        AlphaTestField folderField(browser->m_folderText, folder);
        AlphaTestField arrowField(browser->m_lastBtn, arrow);
        AlphaTestField lastPage(browser->m_lastPage, 1);
        search->m_page = 42;
        search->m_folder = 17;
        browser->updatePageLabel();
        if (std::string(page->getString()) != "43" || std::string(folder->getString()) != "17" ||
            !arrow->isVisible()) return false;
        search->m_page = 0;
        search->m_folder = 0;
        browser->m_lastPage = 0;
        browser->updatePageLabel();
        if (std::string(page->getString()) != "1" || std::string(folder->getString()) != "0" ||
            arrow->isVisible()) return false;
        browser->m_searchObject = nullptr;
        page->setString("missing search");
        folder->setString("missing search");
        browser->updatePageLabel();
        if (std::string(page->getString()) != "1" || std::string(folder->getString()) != "0") return false;
        browser->m_pageText = nullptr;
        browser->m_lastPage = 1;
        folder->setString("unchanged");
        browser->updatePageLabel();
        if (std::string(folder->getString()) != "unchanged" || arrow->isVisible()) return false;
        browser->m_pageText = page;
        browser->m_folderText = nullptr;
        browser->updatePageLabel();
        return std::string(page->getString()) == "1" && arrow->isVisible();
    });

    runBindingTest("Native keybinding popup construction", [] {
        if (sizeof(KeybindingsLayer) != 0x4a0) return false;
        auto& savedTab = *reinterpret_cast<int*>(base::get() + 0x909798);
        auto& savedQuery = *reinterpret_cast<std::string*>(base::get() + 0x8fe4a8);
        AlphaTestField tab(savedTab, savedTab);
        AlphaTestField query(savedQuery, savedQuery);
        alignas(KeybindingsLayer) std::array<uint8_t, sizeof(KeybindingsLayer) + 32> storage;
        storage.fill(0xa5);
        auto popup = new (storage.data()) KeybindingsLayer();
        auto bytes = reinterpret_cast<uint8_t*>(popup);
        auto& search = *reinterpret_cast<std::string*>(bytes + 0x478);
        auto initialized = !*reinterpret_cast<void**>(bytes + 0x430) &&
            !*reinterpret_cast<void**>(bytes + 0x438) && !bytes[0x440] &&
            !*reinterpret_cast<void**>(bytes + 0x448) &&
            !*reinterpret_cast<void**>(bytes + 0x460) && search.empty() &&
            *reinterpret_cast<int*>(bytes + 0x498) == -1;
        search.assign(128, 'q');
        popup->~KeybindingsLayer();
        return initialized && std::all_of(storage.begin() + sizeof(KeybindingsLayer), storage.end(),
            [](uint8_t byte) { return byte == 0xa5; });
    });

    runBindingTest("Native keybinding tabs and search", [] {
        if (sizeof(KeybindingsLayer) != 0x4a0) return false;
        auto& savedTab = *reinterpret_cast<int*>(base::get() + 0x909798);
        auto& savedQuery = *reinterpret_cast<std::string*>(base::get() + 0x8fe4a8);
        AlphaTestField tab(savedTab, 0);
        AlphaTestField query(savedQuery, std::string());
        auto pool = CCPoolManager::sharedPoolManager();
        pool->push();
        auto pop = [](CCPoolManager* value) { value->pop(); };
        std::unique_ptr<CCPoolManager, decltype(pop)> scope(pool, pop);
        auto popup = KeybindingsLayer::create(nullptr);
        if (!popup || !popup->getChildrenCount()) return false;
        auto bytes = reinterpret_cast<uint8_t*>(popup);
        auto cleanup = [](KeybindingsLayer* value) {
            // Avoid persisting test scroll offsets; the child remains owned by its parent.
            *reinterpret_cast<void**>(reinterpret_cast<uint8_t*>(value) + 0x460) = nullptr;
            value->cleanup();
        };
        std::unique_ptr<KeybindingsLayer, decltype(cleanup)> cleanupScope(popup, cleanup);
        auto input = *reinterpret_cast<CCTextInputNode**>(bytes + 0x448);
        auto& search = *reinterpret_cast<std::string*>(bytes + 0x478);
        if (!input || !*reinterpret_cast<void**>(bytes + 0x438)) return false;
        popup->showTab(0);
        if (*reinterpret_cast<int*>(bytes + 0x418) != 0) return false;
        input->setString("alpha-no-match-717171");
        popup->textChanged(input);
        int searchTab = popup->m_searchTab;
        if (search != "alpha-no-match-717171" || searchTab < 0 ||
            *reinterpret_cast<int*>(bytes + 0x418) != searchTab) return false;
        popup->onClearSearch(nullptr);
        popup->textChanged(input);
        if (!input->getString().empty() || !search.empty()) return false;
        popup->rebuildContent();
        return popup->getChildrenCount() > 0 && *reinterpret_cast<int*>(bytes + 0x418) == searchTab;
    });

    runBindingTest("Native typed keybinding settings entries", [] {
        if (sizeof(KeybindSettingsEntry) != 0x28 || sizeof(SEL_MenuHandler) != 8) return false;
        auto& savedTab = *reinterpret_cast<int*>(base::get() + 0x909798);
        auto& savedQuery = *reinterpret_cast<std::string*>(base::get() + 0x8fe4a8);
        AlphaTestField tab(savedTab, savedTab);
        AlphaTestField query(savedQuery, savedQuery);
        auto popup = std::make_unique<KeybindingsLayer>();
        auto entries = popup->KeybindingsLayer::settingsEntries();
        KeybindingsLayer* native = popup.get();
        auto dispatched = native->settingsEntries();
        if (entries.size() != 3 || dispatched.size() != 3) return false;
        char const* titles[] = {"Export", "Import", "Reset"};
        for (size_t i = 0; i < entries.size(); ++i) {
            if (std::string(entries[i].m_title) != titles[i] || !entries[i].m_background ||
                entries[i].m_target || !entries[i].m_callback || entries[i].m_closeAfterAction ||
                entries[i].m_callback != dispatched[i].m_callback) return false;
        }
        return true;
    });

    runBindingTest("Native keybinding settings entry controls", [] {
        if (sizeof(KeybindingsSettingsPopup) != 0x2f0 ||
            offsetof(KeybindingsSettingsPopup, m_entries) != 0x2a8 ||
            offsetof(KeybindingsSettingsPopup, m_pendingData) != 0x2c8) return false;
        AlphaKeybindEntryReceiver receiver;
        std::vector<KeybindSettingsEntry> entries {
            {"Alpha first", "GJ_button_02.png", &receiver,
                menu_selector(AlphaKeybindEntryReceiver::onEntry), false},
            {"Alpha second", "GJ_button_01.png", &receiver,
                menu_selector(AlphaKeybindEntryReceiver::onEntry), false}
        };
        alignas(KeybindingsSettingsPopup) std::array<uint8_t, sizeof(KeybindingsSettingsPopup) + 32> storage;
        storage.fill(0xa5);
        auto popup = new (storage.data()) KeybindingsSettingsPopup();
        auto destroy = [](KeybindingsSettingsPopup* value) { value->~KeybindingsSettingsPopup(); };
        std::unique_ptr<KeybindingsSettingsPopup, decltype(destroy)> lifetime(popup, destroy);
        if (popup->m_keybindingsLayer || !popup->m_entries.empty() || !popup->m_pendingData.empty() ||
            !popup->init(nullptr, entries) || popup->m_entries.size() != 2 || !popup->m_buttonMenu) return false;
        auto first = typeinfo_cast<CCMenuItemSpriteExtra*>(popup->m_buttonMenu->getChildByTag(0));
        auto second = typeinfo_cast<CCMenuItemSpriteExtra*>(popup->m_buttonMenu->getChildByTag(1));
        if (!first || !second) return false;
        first->activate();
        if (receiver.calls != 1 || receiver.lastSender != first) return false;
        auto invalid = CCNode::create();
        invalid->setTag(-1);
        popup->onEntry(invalid);
        invalid->setTag(99);
        popup->onEntry(invalid);
        if (receiver.calls != 1) return false;
        second->activate();
        if (receiver.calls != 2 || receiver.lastSender != second) return false;
        popup->m_pendingData.assign(128, 'x');
        lifetime.reset();
        return std::all_of(storage.begin() + sizeof(KeybindingsSettingsPopup), storage.end(),
            [](uint8_t byte) { return byte == 0xa5; });
    });

    runBindingTest("Native game settings tabs", [] {
        return withNativeEditor(false, [](LevelEditorLayer* editor) {
            auto settings = GameSettingsLayer::create(editor, 0);
            if (!settings || settings->m_gameLayer != editor || !settings->getChildrenCount()) {
                return false;
            }
            auto tabs = static_cast<TabbedPopupLayer*>(settings);
            tabs->showTab(0);
            auto delegate = static_cast<GJDropDownLayerDelegate*>(settings);
            delegate->dropDownLayerWillClose(nullptr);
            return settings->m_gameLayer == editor && settings->getChildrenCount() > 0;
        });
    });

    runBindingTest("Native level list reordering", [] {
        auto list = GJLevelList::create();
        if (!list) return false;
        list->m_levels = {11, 22, 33, 44};
        list->reorderLevel(33, 0);
        if (list->m_levels != std::vector<int>{33, 11, 22, 44}) return false;
        list->reorderLevel(44, -10);
        if (list->m_levels != std::vector<int>{44, 33, 11, 22}) return false;
        list->reorderLevel(33, 200);
        if (list->m_levels != std::vector<int>{44, 11, 22, 33}) return false;
        list->reorderLevel(22, 2);
        list->reorderLevel(99, 0);
        if (list->m_levels != std::vector<int>{44, 11, 22, 33}) return false;
        list->reorderLevelStep(11, true);
        if (list->m_levels != std::vector<int>{11, 44, 22, 33} ||
            list->m_levelsString != "11,44,22,33") return false;
        list->m_levels = {11};
        list->reorderLevel(11, 200);
        if (list->m_levels != std::vector<int>{11} || list->m_levelsString != "11") return false;
        list->m_levels.clear();
        list->reorderLevel(11, 0);
        return list->m_levels.empty() && list->m_levelsString.empty();
    });

    runBindingTest("Native level list parsing", [] {
        auto list = GJLevelList::create();
        if (!list) return false;
        list->parseListLevels("11,22,33");
        if (list->m_levels != std::vector<int>{11, 22, 33} ||
            list->m_levelsString != "11,22,33") return false;
        std::string longList = "717171,717172,717173,717174,717175";
        list->parseListLevels(longList);
        if (list->m_levels != std::vector<int>{717171, 717172, 717173, 717174, 717175} ||
            list->m_levelsString != longList) return false;
        list->parseListLevels("11,11,0,-3");
        if (list->m_levels != std::vector<int>{11, 11, 0, -3}) return false;
        list->parseListLevels("");
        return list->m_levels.empty() && list->m_levelsString.empty();
    });

    runBindingTest("Native paged reload dispatch", [] {
        auto popup = std::make_unique<AlphaPagedReloadFixture>();
        popup->m_mainLayer = CCLayer::create();
        popup->addChild(popup->m_mainLayer);
        auto bytes = reinterpret_cast<uint8_t*>(popup.get());
        auto& page = *reinterpret_cast<int*>(bytes + 0x2a8);
        auto& pending = *reinterpret_cast<int*>(bytes + 0x2ac);
        PagedListPopup* base = popup.get();
        base->loadPage(-1);
        if (popup->requests || popup->clears) return false;
        base->loadPage(5);
        if (popup->requested != 5 || popup->requests != 1 || popup->clears != 1 ||
            page != 5 || pending != -1 || !*reinterpret_cast<CCObject**>(bytes + 0x2b8)) {
            return false;
        }
        // Replacing the loading indicator must release the previous native one.
        base->showLoading();
        return *reinterpret_cast<CCObject**>(bytes + 0x2b8) &&
            geode::addresser::getVirtualOffset(&ReportQueuePopup::rebuildContent) == 0x420;
    });

    runBindingTest("Native popup virtual boundaries", [] {
        return geode::addresser::getVirtualOffset(&AlphaMusicShape::boundary) == 0x438 &&
            geode::addresser::getVirtualOffset(&AlphaNewgroundsShape::boundary) == 0x438;
    });

    runBindingTest("Native typed picker contracts", [] {
        static_assert(std::is_abstract_v<FriendPickerDelegate>);
        static_assert(std::is_abstract_v<IDNameBrowserDelegate>);
        AlphaPickerFixture fixture;
        FriendPickerDelegate* picker = &fixture;
        IDNameBrowserDelegate* names = &fixture;
        if (picker->selectedRecipientCount() || picker->selectedRecipients()) return false;
        fixture.recipients = CCArray::create();
        fixture.recipients->addObject(CCString::create("recipient"));
        if (picker->selectedRecipientCount() != 1 ||
            picker->selectedRecipients() != fixture.recipients || picker->pickerRowState(7) != 0 ||
            picker->pickerCountBase() != 0) return false;
        picker->clearRecipients();
        names->idNameBrowserClosed(nullptr, 717171);
        return picker->selectedRecipientCount() == 0 && fixture.selectedID == 717171;
    });

    runBindingTest("Native popup touch priority lifetime", [] {
        auto dispatcher = CCTouchDispatcher::get();
        auto target = dispatcher->getTargetPrio();
        bool valid = false;
        {
            auto popup = std::make_unique<AlphaModalFixture>();
            if (!popup->initialize()) return false;
            int level = 0;
            valid = dispatcher->getTargetPrio() == target - 2 &&
                !dispatcher->ownerLevelOf(popup.get(), level);
        }
        return valid && dispatcher->getTargetPrio() == target;
    });

    runBindingTest("Native arithmetic gate SDK construction", [] {
        return sizeof(ParentalGateLayer) == 0x308 &&
            checkConstructionBoundary<ParentalGateLayer>();
    });

    runBindingTest("Native effect manager layout", [] {
        return sizeof(GJEffectManager) == 0x828 &&
            offsetof(GJEffectManager, m_unk3f0) == 0x3f8 &&
            offsetof(GJEffectManager, m_timerItemMap) == 0x478 &&
            offsetof(GJEffectManager, m_unkMap498) == 0x560 &&
            offsetof(GJEffectManager, m_spawnTriggerActions) == 0x5b0;
    });

    runBindingTest("Native triggered ID removal", [] {
        if (sizeof(GJEffectManager) != 0x828 ||
            offsetof(GJEffectManager, m_unkMap498) != 0x560) return false;
        auto effects = GJEffectManager::create();
        if (!effects || !effects->m_unkMap498.empty()) return false;
        for (auto pair : {std::pair{17, 23}, std::pair{17, 29}, std::pair{19, 23}, std::pair{17, 23}}) {
            effects->storeTriggeredID(pair.first, pair.second);
        }
        if (effects->m_unkMap498.size() != 3 || !effects->hasBeenTriggered(17, 23) ||
            effects->hasBeenTriggered(17, 24)) return false;
        effects->removeTriggeredID(17, 23);
        if (effects->hasBeenTriggered(17, 23) ||
            effects->m_unkMap498 != std::set<std::pair<int, int>>{{17, 29}, {19, 23}}) return false;
        effects->removeTriggeredID(99, 23);
        if (effects->m_unkMap498.size() != 2) return false;
        effects->removeTriggeredID(17, 29);
        if (effects->m_unkMap498 != std::set<std::pair<int, int>>{{19, 23}}) return false;
        effects->resetTriggeredIDs();
        effects->removeTriggeredID(19, 23);
        return effects->m_unkMap498.empty();
    });

    runBindingTest("Native effect manager SDK construction", [] {
        alignas(GJEffectManager) std::array<uint8_t, sizeof(GJEffectManager) + 32> storage;
        storage.fill(0xa5);
        auto destroy = [](GJEffectManager* value) { value->GJEffectManager::~GJEffectManager(); };
        std::unique_ptr<GJEffectManager, decltype(destroy)> effects(
            new (storage.data()) GJEffectManager(), destroy);
        bool valid = effects->m_unk3f0Value == 0 && effects->m_unk3f0.empty() &&
            effects->m_unk430.empty() && effects->m_unkMap498.empty() &&
            effects->m_timerItemMap.empty() && effects->m_spawnTriggerActions.empty();
        effects->m_unk3f0.insert(17);
        effects->m_unk430.insert(23);
        effects->m_unkMap498.emplace(17, 23);
        effects->removeTriggeredID(17, 23);
        valid = valid && effects->m_unk3f0.contains(17) && effects->m_unk430.contains(23) &&
            effects->m_unkMap498.empty();
        effects.reset();
        return valid && std::all_of(storage.begin() + sizeof(GJEffectManager), storage.end(),
            [](uint8_t byte) { return byte == 0xa5; });
    });

    runBindingTest("Native options data and popup layouts", [] {
        return sizeof(OptionsObject) == 0x78 && offsetof(OptionsObject, m_count) == 0x6c &&
            sizeof(OptionsScrollLayer) == 0x300;
    });

    runBindingTest("Native options factory and row", [] {
        if (sizeof(OptionsObject) != 0x78 || offsetof(OptionsObject, m_count) != 0x6c ||
            sizeof(OptionsScrollLayer) != 0x300) return false;
        AlphaOptionsReceiver receiver;
        auto pool = CCPoolManager::sharedPoolManager();
        pool->push();
        auto pop = [](CCPoolManager* value) { value->pop(); };
        std::unique_ptr<CCPoolManager, decltype(pop)> scope(pool, pop);
        std::string title(96, 'a');
        auto option = OptionsObject::create(717171, false, title, &receiver);
        if (!option || option->m_optionID != 717171 || option->m_enabled ||
            option->m_name != title || option->m_delegate != &receiver || option->m_count != 0) {
            return false;
        }
        option->m_count = 7;
        auto options = CCArray::create();
        options->addObject(option);
        auto popup = OptionsScrollLayer::create(options, false, "Alpha options fixture", nullptr);
        if (!popup || popup->m_optionObjects != options || !popup->m_listLayer) return false;
        Ref<OptionsCell> row = findOptionsRow(popup, option);
        if (!row) return false;
        row->onToggleOption(nullptr);
        bool toggled = option->m_enabled && receiver.changes == 1 && receiver.last == option;
        popup->onClearAll(nullptr);
        bool cleared = !option->m_enabled && receiver.changes == 2 && receiver.last == option;
        popup->onClearAll(nullptr);
        popup->onMatchToggle(nullptr);
        cleared = cleared && receiver.changes == 2;
        popup->onClose(nullptr);
        return toggled && cleared && !popup->m_listLayer;
    });

    runBindingTest("Native options SDK construction", [] {
        alignas(OptionsObject) std::array<uint8_t, sizeof(OptionsObject) + 32> objectStorage;
        objectStorage.fill(0xa5);
        auto option = new (objectStorage.data()) OptionsObject();
        bool valid = !option->m_enabled && !option->m_delegate && option->m_count == 0 &&
            option->m_name.empty();
        option->m_name.assign(128, 'a');
        option->OptionsObject::~OptionsObject();
        valid = valid && std::all_of(objectStorage.begin() + sizeof(OptionsObject), objectStorage.end(),
            [](uint8_t byte) { return byte == 0xa5; });
        alignas(OptionsScrollLayer) std::array<uint8_t, sizeof(OptionsScrollLayer) + 32> popupStorage;
        popupStorage.fill(0xa5);
        auto popup = new (popupStorage.data()) OptionsScrollLayer();
        valid = valid && !popup->m_optionObjects && !popup->m_listLayer &&
            !popup->m_searchInput && popup->m_searchQuery.empty() && !popup->m_searchResult;
        popup->m_searchQuery.assign(128, 'b');
        popup->OptionsScrollLayer::~OptionsScrollLayer();
        valid = valid && std::all_of(popupStorage.begin() + sizeof(OptionsScrollLayer), popupStorage.end(),
            [](uint8_t byte) { return byte == 0xa5; });
        auto rowOption = std::make_unique<OptionsObject>();
        rowOption->m_name = "Alpha SDK row fixture";
        rowOption->m_count = 7;
        alignas(OptionsCell) std::array<uint8_t, sizeof(OptionsCell) + 32> rowStorage;
        rowStorage.fill(0xa5);
        auto row = new (rowStorage.data()) OptionsCell("alpha-options-fixture", 320.f, 40.f);
        row->loadFromObject(rowOption.get());
        valid = valid && row->m_optionsObject == rowOption.get() && row->getChildrenCount() > 0;
        row->OptionsCell::~OptionsCell();
        return valid && std::all_of(rowStorage.begin() + sizeof(OptionsCell), rowStorage.end(),
            [](uint8_t byte) { return byte == 0xa5; });
    });

    runBindingTest("Native options search filters", [] {
        auto pool = CCPoolManager::sharedPoolManager();
        pool->push();
        auto pop = [](CCPoolManager* value) { value->pop(); };
        std::unique_ptr<CCPoolManager, decltype(pop)> scope(pool, pop);
        auto first = OptionsObject::create(1, false, "Alpha positive", nullptr);
        auto second = OptionsObject::create(2, false, "Beta zero", nullptr);
        auto third = OptionsObject::create(3, true, "Alpha enabled", nullptr);
        if (!first || !second || !third) return false;
        first->m_count = 3;
        auto options = CCArray::create();
        for (auto option : {first, second, third}) options->addObject(option);
        auto popup = std::make_unique<OptionsScrollLayer>();
        auto selected = popup->getRelevantObjects(options);
        if (!selected || selected->count() != 2 || selected->objectAtIndex(0) != first ||
            selected->objectAtIndex(1) != third) return false;
        popup->m_searchQuery = "BETA";
        selected = popup->getRelevantObjects(options);
        if (selected->count() != 1 || selected->objectAtIndex(0) != second ||
            popup->m_searchQuery != "BETA") return false;
        popup->m_searchQuery.assign(128, 'z');
        return popup->getRelevantObjects(options)->count() == 0 &&
            popup->getRelevantObjects(nullptr)->count() == 0 && options->count() == 3;
    });

    runBindingTest("Native options SDK initialization", [] {
        auto pool = CCPoolManager::sharedPoolManager();
        pool->push();
        auto pop = [](CCPoolManager* value) { value->pop(); };
        std::unique_ptr<CCPoolManager, decltype(pop)> scope(pool, pop);
        auto option = OptionsObject::create(1, false, "Beta fixture", nullptr);
        if (!option) return false;
        option->m_count = 4;
        auto options = CCArray::create();
        options->addObject(option);
        auto popup = std::make_unique<OptionsScrollLayer>();
        if (!popup->init(options, false, "Alpha options SDK fixture", nullptr) ||
            popup->m_optionObjects != options || !popup->m_searchInput ||
            !popup->m_searchBackground || !popup->m_clearSearchButton || !popup->m_listLayer) {
            return false;
        }
        popup->m_searchInput->setString("BETA");
        popup->textChanged(popup->m_searchInput);
        auto selected = popup->getRelevantObjects(options);
        bool filtered = popup->m_searchQuery == "BETA" && selected->count() == 1 &&
            selected->objectAtIndex(0) == option;
        popup->onClearSearchText(nullptr);
        bool cleared = popup->m_searchInput->getString().empty();
        popup->textChanged(popup->m_searchInput);
        cleared = cleared && popup->m_searchQuery.empty();
        popup->onClose(nullptr);
        return filtered && cleared && !popup->m_listLayer;
    });
}
#ifdef __clang__
    #pragma clang diagnostic pop
#endif
#endif
