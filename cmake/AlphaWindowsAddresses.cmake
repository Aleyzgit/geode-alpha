# GD 2.2091 Windows, EXE SHA-256
# 4eee06d5c2e2213b4ba2b1f6233dd426bbdd0e9b29d7c375e499516b6f8f69d5.
# Match complete declarations inside their class, never method names globally.
function(alpha_patch_class file class before after)
    set(path "${alpha_bindings_output}/${file}.bro")
    file(READ "${path}" content)
    string(FIND "${content}" "class ${class} :" start)
    if (start LESS 0)
        string(FIND "${content}" "class ${class} {" start)
    endif()
    if (start LESS 0)
        message(FATAL_ERROR "Missing binding class ${class}")
    endif()
    string(SUBSTRING "${content}" ${start} -1 tail)
    string(FIND "${tail}" "\n}" end)
    if (end LESS 0)
        message(FATAL_ERROR "Unterminated binding class ${class}")
    endif()
    string(SUBSTRING "${tail}" 0 ${end} block)
    string(FIND "${block}" "${before}" match)
    if (match LESS 0)
        # An upstream update may already contain this exact verified fix.
        string(FIND "${block}" "${after}" already_patched)
        if (NOT already_patched LESS 0)
            return()
        endif()
        message(FATAL_ERROR "Unexpected binding in ${class}: ${before}")
    endif()
    string(REPLACE "${before}" "${after}" patched "${block}")
    string(REPLACE "${block}" "${patched}" content "${content}")
    file(CONFIGURE OUTPUT "${path}" CONTENT "${content}" @ONLY)
endfunction()

function(alpha_patch_address file class signature address)
    set(expected 0x9999999)
    if (ARGC GREATER 4)
        set(expected "${ARGV4}")
    endif()
    alpha_patch_class("${file}" "${class}"
        "${signature} = win ${expected}" "${signature} = win ${address}")
endfunction()

# CCRGBAProtocol vtable at RVA 0x709498; all fourteen implementations have
# identical instructions and field offsets to the 2.2081 implementations.
alpha_patch_address(Cocos2d cocos2d::CCLightning
    "virtual bool isOpacityModifyRGB()" 0x486b0)
alpha_patch_address(Cocos2d cocos2d::CCLightning
    "virtual void setOpacityModifyRGB(bool)" 0x486c0)
alpha_patch_address(Cocos2d cocos2d::CCLightning
    "virtual unsigned char getOpacity()" 0x48660)
alpha_patch_address(Cocos2d cocos2d::CCLightning
    "virtual unsigned char getDisplayedOpacity()" 0x48670)
alpha_patch_address(Cocos2d cocos2d::CCLightning
    "virtual void setOpacity(unsigned char)" 0x48680)
alpha_patch_address(Cocos2d cocos2d::CCLightning
    "virtual void updateDisplayedOpacity(unsigned char)" 0x486d0)
alpha_patch_address(Cocos2d cocos2d::CCLightning
    "virtual bool isCascadeOpacityEnabled()" 0x486e0)
alpha_patch_address(Cocos2d cocos2d::CCLightning
    "virtual void setCascadeOpacityEnabled(bool)" 0x486f0)
alpha_patch_address(Cocos2d cocos2d::CCLightning
    "virtual cocos2d::ccColor3B const& getColor()" 0x48690)
alpha_patch_address(Cocos2d cocos2d::CCLightning
    "virtual cocos2d::ccColor3B const& getDisplayedColor()" 0x48700)
alpha_patch_address(Cocos2d cocos2d::CCLightning
    "virtual void setColor(cocos2d::ccColor3B const&)" 0x486a0)
alpha_patch_address(Cocos2d cocos2d::CCLightning
    "virtual void updateDisplayedColor(cocos2d::ccColor3B const&)" 0x48710)
alpha_patch_address(Cocos2d cocos2d::CCLightning
    "virtual bool isCascadeColorEnabled()" 0x48720)
alpha_patch_address(Cocos2d cocos2d::CCLightning
    "virtual void setCascadeColorEnabled(bool)" 0x48730)
# Primary vtable draw slot 0x2e0; initWithStrikePoint calls strike directly.
alpha_patch_address(Cocos2d cocos2d::CCLightning "virtual void draw()" 0x48430)
alpha_patch_address(Cocos2d cocos2d::CCLightning "void strike()" 0x485c0)

# Exact instruction matches, including the constraintContent tail-call target.
alpha_patch_address(GeometryDash CCScrollLayerExt "void constraintContent()" 0x180a90)
alpha_patch_address(GeometryDash GJWriteMessagePopup
    "virtual void textInputClosed(CCTextInputNode* node)" 0x1265d0)
# Constructor installs the GooglePlayManager RTTI vtable; two mapped callers
# and the complete 2.2081 instruction fingerprint agree on the singleton.
alpha_patch_address(GeometryDash GooglePlayManager
    "static GooglePlayManager* sharedState()" 0x77b90)
# Identical constructor structure and fields, called by both native factories.
alpha_patch_address(GeometryDash LevelOptionsLayer "LevelOptionsLayer()" 0x457900)
# EditorUI's ColorSelectDelegate vtable at subobject offset 0x1a0, slot zero.
# Clears the matching colour popup, refreshes buttons, then applies its change.
alpha_patch_address(GeometryDash EditorUI
    "virtual void colorSelectClosed(cocos2d::CCNode* popup)" 0x1bed60)

# These declarations already contained addresses, but their old/new exact
# fingerprints resolve elsewhere. Verify the expected input before replacing.
# Use the non-deleting destructor: the SDK wrapper passes no delete-flags arg.
alpha_patch_address(GeometryDash CCSpritePlus
    "~CCSpritePlus()" 0x4bdd0 0x49460)
# The object callback forwards its CCNode position to coinEnterFinished.
alpha_patch_address(GeometryDash EndLevelLayer
    "void coinEnterFinishedO(cocos2d::CCObject* sender)" 0x207180 0xdbab0)
alpha_patch_address(GeometryDash EndLevelLayer
    "void playCoinEffect(float duration)" 0x206f30 0x1a8090)
# Calls GameObject's constructor and installs EnhancedGameObject's RTTI tables.
alpha_patch_address(GeometryDash EnhancedGameObject
    "EnhancedGameObject()" 0x288660 0xccad0)
# Clears the three request-cell fields at 0x248, 0x250 and 0x258, returning true.
alpha_patch_address(GeometryDash GJRequestCell
    "virtual bool init()" 0x15ca80 0x158d20)
# Native factory allocates 0x328, initializes the robot animation and icon type.
alpha_patch_address(GeometryDash GJRobotSprite
    "static GJRobotSprite* create(int frame)" 0x3ac740 0x185e70)

# GameLevelManager's native constructor is 0x217310, allocation size 0x690.
# Preserve unknown storage, including the new containers before m_queuedLists.
# Confirmed with the constructor and the GLM_* dictionary serialization keys.
alpha_patch_class(GeometryDash GameLevelManager
    "    virtual ~GameLevelManager()"
    "    GameLevelManager() = win 0x217310;\n    virtual ~GameLevelManager()")
alpha_patch_class(GeometryDash GameLevelManager
    "    std::set<std::string> m_queuedLists;"
    "    PAD = win 0x1e8;\n    win {\n    LevelControlsDelegate* m_levelControlsDelegate;\n    }\n    PAD = win 0x8;\n    std::set<std::string> m_queuedLists;")
alpha_patch_class(GeometryDash GameLevelManager
    "    int m_dailyTimeLeft;" "    PAD = win 0x4;\n    int m_dailyTimeLeft;")
alpha_patch_class(GeometryDash GameLevelManager
    "    cocos2d::CCDictionary* m_gauntletLevels;"
    "    PAD = win 0x24;\n    cocos2d::CCDictionary* m_gauntletLevels;")
alpha_patch_class(GeometryDash GameLevelManager
    "    cocos2d::CCDictionary* m_storedLevels;"
    "    std::unordered_map<int, std::string> m_usernameForAccountID;\n    cocos2d::CCDictionary* m_storedLevels;")
alpha_patch_class(GeometryDash GameLevelManager
    "    std::string m_searchKey;"
    "    PAD = win 0x18;\n    std::string m_searchKey;")
alpha_patch_class(GeometryDash GameLevelManager
    "    LevelUploadDelegate* m_levelUploadDelegate;"
    "    win {\n    CommentAdminDelegate* m_commentAdminDelegate;\n    ThreadMessageDelegate* m_threadMessageDelegate;\n    }\n    LevelUploadDelegate* m_levelUploadDelegate;")
alpha_patch_class(GeometryDash GameLevelManager
    "    UserInfoDelegate* m_userInfoDelegate;"
    "    win {\n    EditorSoundUploadDelegate* m_editorSoundUploadDelegate;\n    EditorSoundListDelegate* m_editorSoundListDelegate;\n    EditorSoundDownloadDelegate* m_editorSoundDownloadDelegate;\n    EditorSoundDeleteDelegate* m_editorSoundDeleteDelegate;\n    }\n    UserInfoDelegate* m_userInfoDelegate;")
alpha_patch_class(GeometryDash GameLevelManager
    "    OnlineListDelegate* m_onlineListDelegate;"
    "    win {\n    GJContentStatusDelegate* m_contentStatusDelegate;\n    }\n    PAD = win 0x8;\n    OnlineListDelegate* m_onlineListDelegate;")
alpha_patch_class(GeometryDash GameLevelManager
    "    LevelRateInfoDelegate* m_levelRateInfoDelegate;"
    "    LevelRateInfoDelegate* m_levelRateInfoDelegate;\n    win {\n    SongUsageDelegate* m_songUsageDelegate;\n    }")

# ZeroConstructor cannot initialize the new native containers hidden in PAD.
set(alpha_manager_inline "${alpha_bindings_output}/inline/GameLevelManager.cpp")
file(READ "${alpha_manager_inline}" alpha_manager_content)
set(alpha_manager_old "ref = new GameLevelManager(geode::ZeroConstructor);")
string(FIND "${alpha_manager_content}" "${alpha_manager_old}" alpha_manager_match)
if (alpha_manager_match LESS 0)
    message(FATAL_ERROR "Unexpected GameLevelManager singleton implementation")
endif()
string(REPLACE "${alpha_manager_old}" "ref = new GameLevelManager();"
    alpha_manager_content "${alpha_manager_content}")
file(CONFIGURE OUTPUT "${alpha_manager_inline}"
    CONTENT "${alpha_manager_content}" @ONLY)

# GLM_03 online levels at 0x358; GLM_16 gauntlet levels at 0x408.
alpha_patch_address(GeometryDash GameLevelManager
    "GJGameLevel* getSavedLevel(int id)" 0x220700)
alpha_patch_address(GeometryDash GameLevelManager
    "GJGameLevel* getSavedGauntletLevel(int id)" 0x2208c0)
# Cached result arrays at 0x480, guarded by the native timer dictionary at 0x420.
alpha_patch_address(GeometryDash GameLevelManager
    "cocos2d::CCArray* getStoredOnlineLevels(char const* key)" 0x2217d0)
alpha_patch_address(GeometryDash GameLevelManager
    "bool isTimeValid(char const* key, float length)" 0x221d40)
# Native LocalLevelManager list array is at 0x178, list unique ID at 0x24c.
alpha_patch_address(GeometryDash GameLevelManager
    "GJLevelList* getLocalLevelList(int uniqueID)" 0x21cac0)
# Easing switch with 18 cases, and dictionary merge preserving larger integers.
alpha_patch_address(GeometryDash GameToolbox
    "static float getEasedValue(float value, int easingType, float easingRate)" 0x73230)
alpha_patch_address(GeometryDash GameToolbox
    "static void mergeDictsSaveLargestInt(cocos2d::CCDictionary* toDict, cocos2d::CCDictionary* fromDict)" 0x6eac0)

# Two additional native flags shift the strings and trailing list fields by
# eight bytes. create allocates 0x250; init writes m_levelsDict at 0x240 and
# m_M_ID at 0x24c, which getLocalLevelList compares against the requested ID.
alpha_patch_class(GeometryDash GJLevelList
    "    std::string m_creatorName;"
    "    PAD = win 0x2;\n    std::string m_creatorName;")

# libcocos2d.dll SHA-256
# 79c2f6567e5851225520803f460c6c5f6f49a2eac9afb8f3bd88d4c5697bef4e.
# These RVAs belong to the engine DLL, not the game EXE. The previous three
# populated addresses pointed into engine read-only data, outside .text.
# Constructor installs CCTexturePVR's RTTI vtable at 0x10b830.
# init dispatches PVR2 -> 0xadc60, PVR3 -> 0xadea0, then GL -> 0xad710.
alpha_patch_address(Cocos2d cocos2d::CCTexturePVR
    "CCTexturePVR()" 0xad670 0x143f80)
alpha_patch_address(Cocos2d cocos2d::CCTexturePVR
    "bool createGLTexture()" 0xad710 0x144020)
alpha_patch_address(Cocos2d cocos2d::CCTexturePVR
    "bool initWithContentsOfFile(char const*)" 0xad910)
alpha_patch_address(Cocos2d cocos2d::CCTexturePVR
    "bool unpackPVRv2Data(unsigned char*, unsigned int)" 0xadc60)
alpha_patch_address(Cocos2d cocos2d::CCTexturePVR
    "bool unpackPVRv3Data(unsigned char*, unsigned int)" 0xadea0 0x144880)

# The old implementations share a dictionary lookup at this + 0x148. The
# corresponding current callers and native fixtures confirm both aliases.
alpha_patch_address(GeometryDash AchievementManager
    "cocos2d::CCDictionary* getAchievementsWithID(char const* id)" 0x3c560)
alpha_patch_address(GeometryDash MusicDownloadManager
    "cocos2d::CCObject* getDLObject(char const* id)" 0x3c560)
alpha_patch_address(GeometryDash MusicDownloadManager
    "void removeDLFromActive(char const* tag)" 0x4843c0)
# Installs CCAnimateFrameCache's RTTI table and retains its three dictionaries.
alpha_patch_address(GeometryDash CCAnimateFrameCache
    "static CCAnimateFrameCache* sharedSpriteFrameCache()" 0x44fe0)
# Native local-list insertion, saved-list dictionary (0x370), download flags
# (0x378). GJLevelList's ID is at 0x158; its editor type is at 0x248.
alpha_patch_address(GeometryDash GameLevelManager
    "GJLevelList* createNewLevelList()" 0x21cb30)
alpha_patch_address(GeometryDash GameLevelManager
    "void saveLevelList(GJLevelList* list)" 0x22e290)
alpha_patch_address(GeometryDash GameLevelManager
    "bool hasDownloadedLevel(int id)" 0x233a80)
# Dictionary lookups at 0x2c8/0x2d0/0x2d8/0x360. Secret coins use the
# player-stat dictionary at 0x240 and the unique_%s prefix. The key builder
# preserves n_/c_/d_/g_/e_ namespaces for main/online/daily/gauntlet/event.
alpha_patch_address(GeometryDash GameStatsManager
    "std::string getLevelKey(int levelID, bool isOnline, bool isDaily, bool isGauntlet, bool isEvent)" 0x2e1530)
alpha_patch_address(GeometryDash GameStatsManager
    "bool hasCompletedLevel(GJGameLevel* level)" 0x2e17a0)
alpha_patch_address(GeometryDash GameStatsManager
    "bool hasUserCoin(char const* key)" 0x2e3630)
alpha_patch_address(GeometryDash GameStatsManager
    "bool hasPendingUserCoin(char const* key)" 0x2e37d0)
alpha_patch_address(GeometryDash GameStatsManager
    "bool hasSecretCoin(char const* key)" 0x2e38a0)
alpha_patch_address(GeometryDash GameStatsManager
    "bool isSecretChestUnlocked(int id)" 0x2eec60)
# SAT projections over native OBB corners/axes, and integer hash lookup with
# item IDs clamped to [0, 9999]. Both are exercised by native fixtures.
alpha_patch_address(GeometryDash OBB2D
    "bool overlaps1Way(OBB2D* other)" 0x7bb70)
alpha_patch_address(GeometryDash GJEffectManager
    "int countForItem(int id)" 0x36cb70)

# FMODAudioEngine's RTTI table is 0x70e200. Native singleton creation allocates
# 0x8a8 and calls 0x5bf90; 0x114ba0 constructs a different class. The new
# channel group precedes m_lastResult, and native state starts at 0x290.
alpha_patch_address(GeometryDash FMODAudioEngine
    "FMODAudioEngine()" 0x5bf90 0x114ba0)
alpha_patch_class(GeometryDash FMODAudioEngine
    "    FMOD_RESULT m_lastResult;"
    "    PAD = win 0x8;\n    FMOD_RESULT m_lastResult;")
alpha_patch_class(GeometryDash FMODAudioEngine
    "    FMODAudioState m_audioState;"
    "    PAD = win 0x18;\n    FMODAudioState m_audioState;")
# Native channel tables at 0x668 and 0x730; stop paths use those tables and
# the channel groups at 0x808/0x848. Queries are tested with isolated maps.
alpha_patch_address(GeometryDash FMODAudioEngine
    "FMOD::Channel* channelForChannelID(int id)" 0x60f50)
alpha_patch_address(GeometryDash FMODAudioEngine
    "int channelIDForUniqueID(int id)" 0x60e90)
alpha_patch_address(GeometryDash FMODAudioEngine
    "void stopAllEffects()" 0x62520)
alpha_patch_address(GeometryDash FMODAudioEngine
    "void stopChannel(int id, AudioTargetType type, bool loop, float delay)" 0x61010)

# This value class already constructs its C++ containers before the body.
# Native 0x5bdc0 initializes the same containers and zeros the first two floats;
# calling it from the body would construct containers twice and leak storage.
alpha_patch_class(GeometryDash FMODAudioState
    "    FMODAudioState() = win 0x9999999, inline;"
    "    FMODAudioState() = inline;")
set(alpha_audio_inline "${alpha_bindings_output}/inline/FMODAudioState.cpp")
file(READ "${alpha_audio_inline}" alpha_audio_content)
set(alpha_audio_before "#if defined(GEODE_IS_MACOS) || defined(GEODE_IS_ANDROID)")
string(FIND "${alpha_audio_content}" "${alpha_audio_before}" alpha_audio_match)
if (alpha_audio_match LESS 0)
    message(FATAL_ERROR "Unexpected FMODAudioState constructor implementation")
endif()
string(REPLACE "${alpha_audio_before}"
    "#if defined(GEODE_IS_MACOS) || defined(GEODE_IS_ANDROID) || defined(GEODE_IS_WINDOWS)"
    alpha_audio_content "${alpha_audio_content}")
file(CONFIGURE OUTPUT "${alpha_audio_inline}" CONTENT "${alpha_audio_content}" @ONLY)

# The two-argument thunk supplies the level's daily ID to the shared native
# three-argument formatter; calling 0x264630 directly leaves that ID undefined.
alpha_patch_address(GeometryDash GJGameLevel
    "char const* getCoinKey(int coinNumber)" 0x264610)
# Pulse strength/type/color fields at 0x18/0x20/0x1c; group colors are guarded
# by the native bit vector and use the pulse-action map. Checked with fixtures.
alpha_patch_address(GeometryDash GJEffectManager
    "cocos2d::ccColor3B colorForPulseEffect(cocos2d::ccColor3B const& color, PulseEffectAction* action)" 0x36bfb0)
alpha_patch_address(GeometryDash GJEffectManager
    "cocos2d::ccColor3B colorForGroupID(int id, cocos2d::ccColor3B const& color, bool mainColor)" 0x36c350)

# The old function takes (this, result, ID); current 0x19f3e0 takes only
# (result, ID). Both switch over object IDs and construct the output CCPoint.
# Preserve correct native parameter placement by declaring the current API static.
alpha_patch_class(GeometryDash EditorUI
    "    cocos2d::CCPoint offsetForKey(int id) = win 0x9999999;"
    "    static cocos2d::CCPoint offsetForKey(int id) = win 0x19f3e0;")
# Native factory installs BoomScrollLayer's RTTI tables and calls init 0x40b60.
# The dots updater uses its 0x1a0 array and page state, matching its eight callers.
alpha_patch_address(GeometryDash BoomScrollLayer
    "static BoomScrollLayer* create(cocos2d::CCArray* pages, int offset, bool looped, cocos2d::CCArray* dynamicObjects, DynamicScrollDelegate* delegate)" 0x40960)
alpha_patch_address(GeometryDash BoomScrollLayer
    "void updateDots(float dt)" 0x41290)

# Active-download dictionary at 0x4a8; username storage updates the account
# map at 0x440 and the known-user / reciprocal ID dictionaries at 0x428-0x438.
alpha_patch_address(GeometryDash GameLevelManager
    "void removeDLFromActive(char const* key)" 0x221b70)
alpha_patch_address(GeometryDash GameLevelManager
    "void storeUserName(int userID, int accountID, std::string userName)" 0x21d580)
# Alternating comma-delimited key/value tokens. Odd token counts return null.
alpha_patch_address(GeometryDash GameToolbox
    "static cocos2d::CCDictionary* stringSetupToDict(std::string const& str, char const* separator)" 0x6fd10)

# Native interpolation stops when paused/finished and updates RGB and opacity.
alpha_patch_address(GeometryDash ColorAction
    "void step(float dt)" 0x3633f0)
alpha_patch_address(GeometryDash ColorChannelSprite
    "void updateBlending(bool enabled)" 0x363180)
alpha_patch_address(GeometryDash ColorChannelSprite
    "void updateCopyLabel(int channelID, bool copyColorLoop)" 0x362e60)
alpha_patch_address(GeometryDash CCSpriteWithHue
    "void updateColorMatrix()" 0x4ca60)
# 0x4c570 is a deleting destructor with an extra EDX flags argument. Its
# non-deleting behavior only calls CCSprite's destructor, so use normal C++
# base destruction rather than invoking that entry with undefined flags.
alpha_patch_class(GeometryDash CCSpriteWithHue
    "    virtual ~CCSpriteWithHue() = win 0x4c570;"
    "    virtual ~CCSpriteWithHue() = win inline;")
file(APPEND "${alpha_bindings_output}/inline/CCSpriteWithHue.cpp"
    "\n#ifdef GEODE_IS_WINDOWS\nCCSpriteWithHue::~CCSpriteWithHue() {}\n#endif\n")
# Moves current -> next unless current is already the native not_used sentinel.
alpha_patch_address(GeometryDash SpriteAnimationManager
    "void overridePrio()" 0x87400)
# Its SDK constructor omitted an owned dictionary that its destructor releases.
# A bare create/destroy fixture reproduced CCObject::release on garbage memory.
set(alpha_animation_inline "${alpha_bindings_output}/inline/SpriteAnimationManager.cpp")
file(READ "${alpha_animation_inline}" alpha_animation_content)
set(alpha_animation_before "    m_soundDict = nullptr;\n    m_frameDict = nullptr;")
string(FIND "${alpha_animation_content}" "${alpha_animation_before}" alpha_animation_match)
if (alpha_animation_match LESS 0)
    message(FATAL_ERROR "Unexpected SpriteAnimationManager constructor implementation")
endif()
string(REPLACE "${alpha_animation_before}"
    "${alpha_animation_before}\n    m_animateDict = nullptr;"
    alpha_animation_content "${alpha_animation_content}")
file(CONFIGURE OUTPUT "${alpha_animation_inline}" CONTENT "${alpha_animation_content}" @ONLY)
# Native completion reset removes level, star and demon keys from 0x2c8.
alpha_patch_address(GeometryDash GameStatsManager
    "void uncompleteLevel(GJGameLevel* level)" 0x2e2450)
alpha_patch_address(GeometryDash GameStatsManager
    "std::string getPathRewardKey(int id)" 0x2ee970)

# RTTI identifies the options constructor at 0x1800b0. Its native deleting
# destructor allocates/frees 0x470, including the new trailing 0x40-byte map.
alpha_patch_address(GeometryDash GJOptionsLayer
    "GJOptionsLayer()" 0x1800b0)
alpha_patch_class(GeometryDash GJOptionsLayer
    "    float m_offset;" "    float m_offset;\n    PAD = win 0x40;")
# The native base didToggleGV implementation (0x98e10) only destroys its string
# argument. An empty C++ body performs that same argument cleanup automatically.
alpha_patch_class(GeometryDash GJOptionsLayer
    "    virtual void didToggleGV(std::string variable) = win 0x9999999, inline;"
    "    virtual void didToggleGV(std::string variable) = inline;")
file(APPEND "${alpha_bindings_output}/inline/GJOptionsLayer.cpp"
    "\n#ifdef GEODE_IS_WINDOWS\nvoid GJOptionsLayer::didToggleGV(std::string variable) {}\n#endif\n")
# Grow outer arrays at 0x380/0x390, returning a stable child array for the index.
alpha_patch_address(GeometryDash SetupTriggerPopup
    "cocos2d::CCArray* getPageContainer(int page)" 0x60b6f0)
alpha_patch_address(GeometryDash SetupTriggerPopup
    "cocos2d::CCArray* getGroupContainer(int group)" 0x60b970)
# Parse the 'a'-separated numeric particle record. Native writes at 0..0x104
# match ParticleStruct, and an empty input leaves the supplied struct unchanged.
alpha_patch_address(GeometryDash GameToolbox
    "static void particleStringToStruct(std::string const& str, cocos2d::ParticleStruct& particleStruct)" 0x71a40)

# Native PagedListPopup initializes 0x298..0x307 before derived delegate bases.
# Its primary RTTI table is 0x7214f8 and requestPage points to _purecall.
alpha_patch_class(GeometryDash PagedListPopup
    "    virtual ~PagedListPopup() = win 0xc2af0;"
    "    PagedListPopup() = win 0xabc50;\n    virtual ~PagedListPopup() = win inline;")
alpha_patch_class(GeometryDash PagedListPopup
    "    virtual int requestPage(int) = win 0x9999999;"
    "    inline virtual int requestPage(int) = 0;")
alpha_patch_class(GeometryDash PagedListPopup
    "    TodoReturn updatePageUI(int, int, int);"
    "    TodoReturn updatePageUI(int, int, int);\n    PAD = win 0x70;")
# Native non-deleting 0xc2af0 releases the object at 0x2b8, then destroys
# FLAlertLayer. Normal C++ base destruction avoids reconstructing an abstract
# PagedListPopup, which the generic generated destructor cannot do.
file(CONFIGURE OUTPUT "${alpha_bindings_output}/inline/PagedListPopup.cpp"
    CONTENT "#include <Geode/binding/PagedListPopup.hpp>\n\n#ifdef GEODE_IS_WINDOWS\nPagedListPopup::~PagedListPopup() {\n    auto owned = *reinterpret_cast<cocos2d::CCObject**>(reinterpret_cast<uintptr_t>(this) + 0x2b8);\n    CC_SAFE_RELEASE(owned);\n}\n#endif\n" @ONLY)
# VirtualRowSource is an interface. All concrete native row sources implement
# these three entries; mount takes index, node and menu, not an empty argument
# list. Keep C++ dispatch compatible and require concrete subclasses to supply it.
alpha_patch_class(GeometryDash VirtualRowSource
    "    virtual int virtualRowCount() = win 0x9999999;"
    "    inline virtual int virtualRowCount() = 0;")
alpha_patch_class(GeometryDash VirtualRowSource
    "    virtual int virtualRowHeight(int) = win 0x9999999;"
    "    inline virtual int virtualRowHeight(int) = 0;")
alpha_patch_class(GeometryDash VirtualRowSource
    "    virtual void mountVirtualRow() = win 0x9999999;"
    "    inline virtual void mountVirtualRow(int, cocos2d::CCNode*, cocos2d::CCMenu*) = 0;")

# Native queue mute/unmute responses dispatch through 0xd0/0xd8/0xe0/0xe8.
# These four interface entries precede history, shifting every later callback.
alpha_patch_class(GeometryDash CommentAdminDelegate
    "    virtual void adminUserHistoryFinished(int, cocos2d::CCArray*, int, int, int) = inline;"
    "    win {\n    virtual void adminQueueMuteFinished() = inline;\n    virtual void adminQueueMuteFailed() = inline;\n    virtual void adminQueueUnmuteFinished() = inline;\n    virtual void adminQueueUnmuteFailed() = inline;\n    }\n    virtual void adminUserHistoryFinished(int, cocos2d::CCArray*, int, int, int) = inline;")
file(APPEND "${alpha_bindings_output}/inline/CommentAdminDelegate.cpp"
    "\n#ifdef GEODE_IS_WINDOWS\nvoid CommentAdminDelegate::adminQueueMuteFinished() {}\nvoid CommentAdminDelegate::adminQueueMuteFailed() {}\nvoid CommentAdminDelegate::adminQueueUnmuteFinished() {}\nvoid CommentAdminDelegate::adminQueueUnmuteFailed() {}\n#endif\n")
# Native AccountReviewPopup secondary vtables: CommentAdmin at this+0x308,
# Dropdown at this+0x328. Known finished callbacks anchor all failed slots.
alpha_patch_address(GeometryDash AccountReviewPopup
    "virtual void dropdownSelectionChanged(Dropdown*, int)" 0xb34a0)
alpha_patch_address(GeometryDash AccountReviewPopup
    "virtual void adminReportedContentFailed(int)" 0xb37d0)
alpha_patch_address(GeometryDash AccountReviewPopup
    "virtual void adminAbuseInfoFailed(int)" 0xb39a0)
alpha_patch_address(GeometryDash AccountReviewPopup
    "virtual void adminUserHistoryFailed(int)" 0xb3b40)

# The native dispatch table forwards online levels to 0x22b650. The upstream
# Windows entry 0x243180 is an unrelated admin-reported-content handler.
alpha_patch_address(GeometryDash GameLevelManager
    "void onGetOnlineLevelsCompleted(std::string response, std::string tag)" 0x22b650 0x243180)
alpha_patch_class(GeometryDash GameLevelManager
    "void onUploadLevelCompleted(std::string response, std::string tag);"
    "void onUploadLevelCompleted(std::string response, std::string tag) = win 0x22a0b0;")
alpha_patch_class(GeometryDash GameLevelManager
    "void onGetGJUserInfoCompleted(std::string response, std::string tag) = imac"
    "void onGetGJUserInfoCompleted(std::string response, std::string tag) = win 0x23a830, imac")

alpha_patch_address(GeometryDash ButtonSprite "void updateSpriteBGSize()" 0x430e0)
alpha_patch_address(GeometryDash UploadActionPopup
    "static UploadActionPopup* create(UploadPopupDelegate* delegate, std::string str)" 0x3a0d60)

if (WIN32)
    # Native KeybindSource has a virtual destructor, four pure entries, then
    # isLevelSet. Chord/action queries return vectors of 32-bit integers.
    alpha_patch_class(GeometryDash KeybindSource
        "    virtual TodoReturn chordsForAction(int) = win 0x9999999;"
        "    inline virtual ~KeybindSource() {}\n    inline virtual std::vector<int> chordsForAction(int) = 0;")
    alpha_patch_class(GeometryDash KeybindSource
        "    virtual TodoReturn actionsForChord(int) = win 0x9999999;"
        "    inline virtual std::vector<int> actionsForChord(int) = 0;")
    foreach(method bindChord unbindChord)
        alpha_patch_class(GeometryDash KeybindSource
            "    virtual TodoReturn ${method}(int, int) = win 0x9999999;"
            "    inline virtual void ${method}(int, int) = 0;")
    endforeach()
    alpha_patch_class(GeometryDash KeybindSource
        "    bool isLevelSet() = win 0x5a3a0;"
        "    virtual bool isLevelSet() = win 0x5a3a0;")
    foreach(class GlobalKeybindSource KeybindSet)
        foreach(method chordsForAction actionsForChord)
            alpha_patch_class(GeometryDash "${class}"
                "virtual TodoReturn ${method}(int)" "virtual std::vector<int> ${method}(int)")
        endforeach()
        foreach(method bindChord unbindChord)
            alpha_patch_class(GeometryDash "${class}"
                "virtual TodoReturn ${method}(int, int)" "virtual void ${method}(int, int)")
        endforeach()
    endforeach()
    # Both populated destructor entries are deleting destructors taking flags.
    # Ordinary C++ destruction releases the owned map exactly once.
    alpha_patch_class(GeometryDash GlobalKeybindSource
        "    virtual ~GlobalKeybindSource() = win 0x3d51d0;"
        "    inline virtual ~GlobalKeybindSource() {}")
    alpha_patch_class(GeometryDash KeybindSet
        "    virtual ~KeybindSet() = win 0x44a370;"
        "    KeybindSet() = win inline;\n    inline virtual ~KeybindSet() {}")
    alpha_patch_class(GeometryDash KeybindSet
        "    bool isLevelSet() = win 0x3ff70;"
        "    virtual bool isLevelSet() = win 0x3ff70;\n    std::map<int, std::set<int>> m_bindings;\n    bool m_changed;\n    PAD = win 0x7;")
    file(CONFIGURE OUTPUT "${alpha_bindings_output}/inline/KeybindSet.cpp"
        CONTENT "#include <Geode/binding/KeybindSet.hpp>\n\nKeybindSet::KeybindSet() : KeybindSource(geode::ZeroConstructor), m_changed(false) {}\n" @ONLY)
    alpha_patch_class(GeometryDash GJKeybindManager
        "virtual TodoReturn actionsForKey(cocos2d::enumKeyCodes, bool, bool, bool)"
        "virtual std::vector<int> actionsForKey(cocos2d::enumKeyCodes, bool, bool, bool)")
    alpha_patch_class(GeometryDash GJKeybindManager
        "    static GJKeybindManager* sharedState();"
        "    static GJKeybindManager* sharedState() = win 0x388320;")
    alpha_patch_class(GeometryDash GJKeybindManager
        "    TodoReturn chordsForAction(int);"
        "    std::vector<int> chordsForAction(int) = win 0x388790;")
    alpha_patch_class(GeometryDash GJKeybindManager
        "    TodoReturn actionsForChord(int);"
        "    std::vector<int> actionsForChord(int) = win 0x388920;")
endif()

# Native editor window base allocates 0x480, initializes 0x3e8..0x478 and
# leaves onBuildWindowContent pure. Concrete editor popups provide the body.
alpha_patch_address(GeometryDash EditorWindowPopup "EditorWindowPopup()" 0x123530)
alpha_patch_class(GeometryDash EditorWindowPopup
    "    virtual void onBuildWindowContent() = win 0x9999999;"
    "    inline virtual void onBuildWindowContent() = 0;")
alpha_patch_class(GeometryDash EditorWindowPopup
    "    TodoReturn clampWindowCenter(cocos2d::CCPoint);"
    "    TodoReturn clampWindowCenter(cocos2d::CCPoint);\n    PAD = win 0x98;")

# ScrollViewPopup's native base has one retained object at 0x298, a further
# eight bytes of state, and a pure buildContent entry at vtable offset 0x438.
alpha_patch_class(GeometryDash ScrollViewPopup
    "    virtual ~ScrollViewPopup() = win 0x6218d0;"
    "    ScrollViewPopup() = win 0x47f7b0;\n    virtual ~ScrollViewPopup() = win inline;")
alpha_patch_class(GeometryDash ScrollViewPopup
    "    virtual float buildContent(cocos2d::CCNode*, float, float) = win 0x9999999;"
    "    inline virtual float buildContent(cocos2d::CCNode*, float, float) = 0;")
alpha_patch_class(GeometryDash ScrollViewPopup
    "    void onClose(cocos2d::CCObject* sender);"
    "    void onClose(cocos2d::CCObject* sender);\n    PAD = win 0x10;")
file(CONFIGURE OUTPUT "${alpha_bindings_output}/inline/ScrollViewPopup.cpp"
    CONTENT "#include <Geode/binding/ScrollViewPopup.hpp>\n\n#ifdef GEODE_IS_WINDOWS\nScrollViewPopup::~ScrollViewPopup() {\n    auto owned = *reinterpret_cast<cocos2d::CCObject**>(reinterpret_cast<uintptr_t>(this) + 0x298);\n    CC_SAFE_RELEASE(owned);\n}\n#endif\n" @ONLY)

# Native RTTI constructors and their deleting-destructor sizes agree with
# the SDK browser (0x2c8) and guidelines (0x300) layouts.
alpha_patch_address(GeometryDash BrowseSmartTemplateLayer "BrowseSmartTemplateLayer()" 0x5ec020)
alpha_patch_address(GeometryDash CreateGuidelinesLayer "CreateGuidelinesLayer()" 0x132b20)

# SetTextPopup's native factory inlines construction: input at 0x2a0, value
# string at 0x2b8, delegate at 0x2f0, allocation size 0x2f8. Its 0x399820
# destructor also takes deleting flags; normal C++ cleanup needs no such call.
alpha_patch_class(GeometryDash SetTextPopup
    "    SetTextPopup() = win 0x9999999, inline;"
    "    SetTextPopup() = inline;")
alpha_patch_class(GeometryDash SetTextPopup
    "    ~SetTextPopup() = win 0x399820;"
    "    ~SetTextPopup() = win inline;")
alpha_patch_class(GeometryDash SetTextPopup
    "    std::string m_value;" "    PAD = win 0x8;\n    std::string m_value;")
alpha_patch_class(GeometryDash SetTextPopup
    "    SetTextPopupDelegate* m_delegate;"
    "    PAD = win 0x18;\n    SetTextPopupDelegate* m_delegate;")
alpha_patch_class(GeometryDash SetTextPopup
    "static SetTextPopup* create(std::string, std::string, int, std::string, std::string, bool, float, bool, int, int) = imac"
    "static SetTextPopup* create(std::string, std::string, int, std::string, std::string, bool, float, bool, int, int) = win 0x3a6f70, imac")
alpha_patch_class(GeometryDash SetTextPopup
    "bool init(std::string, std::string, int, std::string, std::string, bool, float, bool, int, int);"
    "bool init(std::string, std::string, int, std::string, std::string, bool, float, bool, int, int) = win 0x3a71f0;")
file(APPEND "${alpha_bindings_output}/inline/SetTextPopup.cpp"
    "\n#ifdef GEODE_IS_WINDOWS\nSetTextPopup::SetTextPopup() {\n    m_input = nullptr;\n    m_disableDelegate = false;\n    m_cancelled = false;\n    m_delegate = nullptr;\n    std::memset(reinterpret_cast<uint8_t*>(this) + 0x2ac, 0, 8);\n    std::memset(reinterpret_cast<uint8_t*>(this) + 0x2d8, 0, 0x18);\n}\nSetTextPopup::~SetTextPopup() {\n    removeAllChildrenWithCleanup(true);\n}\n#endif\n")

alpha_patch_address(GeometryDash LocalLevelManager "int updateLevelOrder()" 0x46b690)
alpha_patch_class(GeometryDash SetTextPopup
    "void onClose(cocos2d::CCObject* sender);"
    "void onClose(cocos2d::CCObject* sender) = win 0x3a85a0;")

# Native MusicDownloadManager allocates 0x3d0. New storage precedes the SFX
# requested flag, the music-tag map and the music-library counters. Native
# construction puts tag map at 0x1d8 and resource sets at 0x208 / 0x248.
alpha_patch_class(GeometryDash MusicDownloadManager
    "    bool m_requestedSfxLibraryDownloaded;"
    "    PAD = win 0x10;\n    bool m_requestedSfxLibraryDownloaded;")
alpha_patch_class(GeometryDash MusicDownloadManager
    "    std::map<int, std::string> m_musicTags;"
    "    PAD = win 0x8;\n    std::map<int, std::string> m_musicTags;")
alpha_patch_class(GeometryDash MusicDownloadManager
    "    int m_musicLibraryVersion;"
    "    PAD = win 0x10;\n    int m_musicLibraryVersion;")
alpha_patch_class(GeometryDash MusicDownloadManager
    "    std::unordered_set<int> m_resourceSongUnorderedSet;"
    "    std::unordered_set<int> m_resourceSongUnorderedSet;\n    PAD = win 0x148;")
if (WIN32)
    alpha_patch_class(GeometryDash MusicDownloadManager
        "    virtual ~MusicDownloadManager()"
        "    MusicDownloadManager() = win 0x480540;\n    virtual ~MusicDownloadManager()")
endif()
alpha_patch_address(GeometryDash MusicDownloadManager "bool isSongDownloaded(int id)" 0x4845a0)
