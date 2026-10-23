# GD 2.2091 Windows, EXE SHA-256
# 4eee06d5c2e2213b4ba2b1f6233dd426bbdd0e9b29d7c375e499516b6f8f69d5.
# Match complete declarations inside their class, never method names globally.
if (NOT WIN32)
    return()
endif()

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
    # Prefer an existing complete correction over its shorter source prefix.
    # Empty replacements remove declarations and cannot identify a fix.
    if (NOT "${after}" STREQUAL "")
        string(FIND "${block}" "${after}" already_patched)
        if (NOT already_patched LESS 0)
            return()
        endif()
    endif()
    string(FIND "${block}" "${before}" match)
    if (match LESS 0)
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

# Native comments reserve 0x1e0 bytes. New scalar state precedes the retained
# score at 0x1d8; the dictionary parser stores server key 15 at 0x1d4.
alpha_patch_class(GeometryDash GJComment "    GJUserScore* m_userScore;"
    "    int m_unk1cc;\n    bool m_unk1d0;\n    int m_key15;\n    GJUserScore* m_userScore;")
set(alpha_comment_inline "${alpha_bindings_output}/inline/GJComment.cpp")
file(READ "${alpha_comment_inline}" alpha_comment_source)
set(alpha_comment_before "    m_canDelete = false;")
string(FIND "${alpha_comment_source}" "${alpha_comment_before}" alpha_comment_match)
if (alpha_comment_match LESS 0)
    message(FATAL_ERROR "Unexpected GJComment constructor implementation")
endif()
string(REPLACE "${alpha_comment_before}"
    "${alpha_comment_before}\n    m_isFriend = false;\n    m_unk1cc = 0;\n    m_unk1d0 = false;\n    m_key15 = 0;"
    alpha_comment_source "${alpha_comment_source}")
file(WRITE "${alpha_comment_inline}" "${alpha_comment_source}")

# Native registration adds a password-strength label at 0x2e0. Its loading
# circle and input lock follow at 0x300/0x308, within a 0x310-byte allocation.
alpha_patch_class(GeometryDash AccountRegisterLayer
    "    cocos2d::CCLabelBMFont* m_passwordLabel;"
    "    cocos2d::CCLabelBMFont* m_passwordLabel;\n    cocos2d::CCLabelBMFont* m_passwordInfoLabel;")
alpha_patch_address(GeometryDash AccountRegisterLayer
    "void textChanged(CCTextInputNode* node)" 0x90140 inline)
alpha_patch_class(GeometryDash AccountRegisterLayer
    "    TodoReturn updatePasswordInfo(std::string);"
    "    void updatePasswordInfo(std::string password) = win 0x8f9e0;")
set(alpha_registration_inline "${alpha_bindings_output}/inline/AccountRegisterLayer.cpp")
file(READ "${alpha_registration_inline}" alpha_registration_source)
foreach(field IN ITEMS emailField passwordLabel emailLabel)
    if (field STREQUAL "emailField")
        set(extra "verifyEmailField")
    elseif(field STREQUAL "passwordLabel")
        set(extra "passwordInfoLabel")
    else()
        set(extra "verifyEmailLabel")
    endif()
    set(before "    m_${field} = nullptr;")
    string(FIND "${alpha_registration_source}" "${before}" match)
    if (match LESS 0)
        message(FATAL_ERROR "Unexpected AccountRegisterLayer constructor field ${field}")
    endif()
    string(REPLACE "${before}" "${before}\n    m_${extra} = nullptr;"
        alpha_registration_source "${alpha_registration_source}")
endforeach()
foreach(body IN ITEMS
    "void AccountRegisterLayer::textChanged(CCTextInputNode* node) {}"
    "bool AccountRegisterLayer::validPassword(std::string password) {\n    return password.size() > 5;\n}")
    string(FIND "${alpha_registration_source}" "${body}" match)
    if (match LESS 0)
        message(FATAL_ERROR "Unexpected AccountRegisterLayer inline implementation")
    endif()
    string(REPLACE "${body}" "" alpha_registration_source "${alpha_registration_source}")
endforeach()
file(READ "${CMAKE_CURRENT_LIST_DIR}/alpha-inline/AccountRegisterLayer.cpp" alpha_registration_helpers)
file(WRITE "${alpha_registration_inline}" "${alpha_registration_source}\n${alpha_registration_helpers}")

# Windows now resumes suspended audio in the engine. Preserve the older SDK
# helpers using the verified legacy policy and the current typed game state.
alpha_patch_address(GeometryDash AppDelegate "bool musicTest()" inline)
alpha_patch_address(GeometryDash AppDelegate "void resumeSound()" inline)
file(READ "${CMAKE_CURRENT_LIST_DIR}/alpha-inline/AppDelegate.cpp" alpha_app_audio_helpers)
file(APPEND "${alpha_bindings_output}/inline/AppDelegate.cpp" "\n${alpha_app_audio_helpers}")

# Native object setup and sprite creation call these shared implementations.
# Editor selection uses object ID, group and effective color-channel filters.
alpha_patch_address(GeometryDash GameObject "void commonSetup()" 0x28ae70)
alpha_patch_address(GeometryDash GameObject
    "cocos2d::CCSprite* addCustomColorChild(std::string frame)" 0x2944e0)
alpha_patch_address(GeometryDash GameObject
    "bool shouldBlendColor(GJSpriteColor* color, bool mainColor)" 0x28de70)
alpha_patch_address(GeometryDash EditorUI
    "bool canSelectObject(GameObject* object)" 0x1c8500)
alpha_patch_address(GeometryDash EditorUI "void updateZoom(float zoom)" 0x1b4c00)
alpha_patch_class(GeometryDash EditorUI
    "    void updateZoom(float, cocos2d::CCPoint) = win 0x1b4c70, m1 0x10464, imac 0x120c0;"
    "    void updateZoom(float zoom, cocos2d::CCPoint anchor) = win 0x1b4c70, m1 0x10464, imac 0x120c0;")
alpha_patch_address(GeometryDash LevelEditorLayer "void updateGridLayerParent()" 0x3ef7a0)

# Native slider callbacks read the snapped helper, not the raw-position helper.
# SDK thumb construction must initialize the new snapping controls as well.
alpha_patch_address(GeometryDash SliderThumb "float getValue()" 0x84020 0x83f90)
set(alpha_slider_thumb_inline "${alpha_bindings_output}/inline/SliderThumb.cpp")
file(READ "${alpha_slider_thumb_inline}" alpha_slider_thumb_source)
set(alpha_slider_thumb_before "    m_vertical = false;")
string(FIND "${alpha_slider_thumb_source}" "${alpha_slider_thumb_before}" alpha_slider_thumb_match)
if (alpha_slider_thumb_match LESS 0)
    message(FATAL_ERROR "Unexpected SliderThumb constructor implementation")
endif()
string(REPLACE "${alpha_slider_thumb_before}"
    "${alpha_slider_thumb_before}\n    m_snapValue = -1.f;\n    m_snapThreshold = 0.f;\n    m_snapStep = 0.f;"
    alpha_slider_thumb_source "${alpha_slider_thumb_source}")
file(WRITE "${alpha_slider_thumb_inline}" "${alpha_slider_thumb_source}")

# Native MusicBrowser construction reserves 0x398. Search controls, branding
# and view buttons were added between the older SDK's existing fields.
alpha_patch_class(GeometryDash MusicBrowser "    virtual ~MusicBrowser()"
    "    MusicBrowser() = win inline;\n    virtual ~MusicBrowser()")
alpha_patch_class(GeometryDash MusicBrowser "    CCMenuItemSpriteExtra* m_clearSearchButton;"
    "    CCMenuItemSpriteExtra* m_clearSearchButton;\n    CCTextInputNode* m_searchInput;\n    cocos2d::extension::CCScale9Sprite* m_searchBackground;\n    CCMenuItemSpriteExtra* m_clearSearchTextButton;\n    bool m_searchVisible;\n    cocos2d::CCSprite* m_brandSprite;")
alpha_patch_class(GeometryDash MusicBrowser "    CCMenuItemSpriteExtra* m_artistFilterButton;"
    "    CCMenuItemSpriteExtra* m_artistFilterButton;\n    CCMenuItemSpriteExtra* m_downloadedViewButton;\n    CCMenuItemSpriteExtra* m_favoriteViewButton;\n    CCMenuItemSpriteExtra* m_levelViewButton;")
alpha_patch_address(GeometryDash MusicBrowser "bool init(int songID, GJSongType songType)" 0x47a340 0xf6b60)
alpha_patch_address(GeometryDash MusicBrowser "void onUpdateLibrary(cocos2d::CCObject* sender)" 0x47c280)
alpha_patch_class(GeometryDash MusicDownloadManager "    void downloadMusicLibrary();"
    "    void downloadMusicLibrary() = win 0x48ca90;")
alpha_patch_address(GeometryDash MusicDownloadManager "void downloadSFXLibrary()" 0x48f820)
alpha_patch_class(GeometryDash MusicDownloadManager "    void tryUpdateMusicLibrary() = imac"
    "    void tryUpdateMusicLibrary() = win 0x48c6b0, imac")
alpha_patch_class(GeometryDash MusicDownloadManager "    void tryUpdateSFXLibrary();"
    "    void tryUpdateSFXLibrary() = win 0x48f410;")
file(READ "${CMAKE_CURRENT_LIST_DIR}/alpha-inline/MusicBrowser.cpp" alpha_music_browser_constructor)
file(APPEND "${alpha_bindings_output}/inline/MusicBrowser.cpp" "\n${alpha_music_browser_constructor}")
# The old Windows search-popup class was replaced by inline browser controls.
# Keep its SDK API on the current SetTextPopup and shared folder-search option.
alpha_patch_address(GeometryDash SearchSFXPopup "bool init(std::string query)" inline)
alpha_patch_address(GeometryDash SearchSFXPopup "void onSearchFolders(cocos2d::CCObject* sender)" inline)
file(READ "${CMAKE_CURRENT_LIST_DIR}/alpha-inline/SearchSFXPopup.cpp" alpha_sfx_search_popup)
file(APPEND "${alpha_bindings_output}/inline/SearchSFXPopup.cpp" "\n${alpha_sfx_search_popup}")

# SFXBrowser owns new search controls, a path string and native selection
# containers. Its native constructor/destructor initialize and release them.
alpha_patch_class(GeometryDash SFXBrowser "SFXBrowser() = inline" "SFXBrowser() = win 0x616230")
alpha_patch_address(GeometryDash SFXBrowser "~SFXBrowser()" 0x6164e0 inline)
alpha_patch_address(GeometryDash SFXBrowser "static SFXBrowser* create(int, int)" 0x6166e0 inline)
alpha_patch_address(GeometryDash SFXBrowser "bool init(int, int)" 0x616760)
alpha_patch_address(GeometryDash SFXBrowser "void onUpdateLibrary(cocos2d::CCObject* sender)" 0x618920)
alpha_patch_class(GeometryDash SFXBrowser [=[    SFXSearchResult* m_searchResult;
    cocos2d::CCArray* m_sfxObjects;
    GJCommentListLayer* m_listLayer;
    cocos2d::CCLabelBMFont* m_nameLabel;
    cocos2d::CCLabelBMFont* m_pathLabel;
    cocos2d::CCLabelBMFont* m_infoLabel;
    cocos2d::CCLabelBMFont* m_pageLabel;
    cocos2d::CCLabelBMFont* m_searchLabel;
    CCMenuItemSpriteExtra* m_prevButton;
    CCMenuItemSpriteExtra* m_nextButton;
    CCMenuItemSpriteExtra* m_backButton;
    CCMenuItemSpriteExtra* m_refreshButton;
    CCMenuItemSpriteExtra* m_clearSearchButton;
    LoadingCircleSprite* m_circleSprite;
    int m_sfxID;
    int m_libraryVersion;
    int m_cellAmount;
    bool m_gettingURL;
    bool m_selectedCell;
    bool m_snapToSelected;
    bool m_compactMode;
    AudioSortType m_sortType;
    SFXBrowserDelegate* m_delegate;
    bool m_autoUpdating;]=]
    [=[    SFXSearchResult* m_searchResult;
    cocos2d::CCArray* m_sfxObjects;
    GJCommentListLayer* m_listLayer;
    cocos2d::CCLabelBMFont* m_nameLabel;
    cocos2d::CCLabelBMFont* m_pathLabel;
    CCMenuItemSpriteExtra* m_searchSettingsButton;
    std::string m_folderPath;
    cocos2d::CCLabelBMFont* m_infoLabel;
    cocos2d::CCLabelBMFont* m_pageLabel;
    CCMenuItemSpriteExtra* m_prevButton;
    CCMenuItemSpriteExtra* m_nextButton;
    CCMenuItemSpriteExtra* m_backButton;
    CCMenuItemSpriteExtra* m_refreshButton;
    CCMenuItemSpriteExtra* m_clearSearchButton;
    CCTextInputNode* m_searchInput;
    cocos2d::extension::CCScale9Sprite* m_searchBackground;
    CCMenuItemSpriteExtra* m_clearSearchTextButton;
    cocos2d::CCArray* m_searchControls;
    bool m_searchVisible;
    std::array<CCMenuItemSpriteExtra*, 4> m_viewButtons;
    PAD = win 0x8;
    CCMenuItemSpriteExtra* m_importSetupButton;
    CCMenuItemSpriteExtra* m_replaceSetupButton;
    CCMenuItemSpriteExtra* m_shareSetupButton;
    PAD = win 0x8;
    CCMenuItemSpriteExtra* m_exportFileButton;
    CCMenuItemSpriteExtra* m_importFileButton;
    CCMenuItemSpriteExtra* m_newFolderButton;
    CCMenuItemSpriteExtra* m_moveToFolderButton;
    CCMenuItemSpriteExtra* m_batchDeleteButton;
    cocos2d::CCLabelBMFont* m_selectionLabel;
    PAD = win 0x48;
    LoadingCircleSprite* m_circleSprite;
    int m_sfxID;
    int m_libraryVersion;
    AudioSortType m_sortType;
    bool m_gettingURL;
    bool m_selectedCell;
    bool m_snapToSelected;
    bool m_compactMode;
    PAD = win 0x8;
    SFXBrowserDelegate* m_delegate;
    bool m_autoUpdating;]=])
file(WRITE "${alpha_bindings_output}/inline/SFXBrowser.cpp" "")
# Native SFX metadata destruction releases three strings, including caches at
# 0x70/0x90. Folder arrays follow the complete 0xb0-byte metadata base.
alpha_patch_class(GeometryDash SFXInfoObject "SFXInfoObject() = inline" "SFXInfoObject() = win 0x480b30")
alpha_patch_class(GeometryDash SFXInfoObject "    std::string m_name;"
    "    std::string m_name;\n    std::string m_lowerCaseName;\n    std::string m_unk090;")
alpha_patch_address(GeometryDash SFXInfoObject
    "bool init(int id, std::string name, int folderID, int size, int duration)" 0x4977a0 inline)
set(alpha_sfx_info_path "${alpha_bindings_output}/inline/SFXInfoObject.cpp")
file(READ "${alpha_sfx_info_path}" alpha_sfx_info_source)
set(alpha_sfx_info_old_constructor "SFXInfoObject::SFXInfoObject() {\n    m_sfxID = 0;\n    m_folderID = 0;\n    m_size = 0;\n    m_duration = 0;\n    m_folder = false;\n    m_unk049 = false;\n}")
string(FIND "${alpha_sfx_info_source}" "${alpha_sfx_info_old_constructor}" alpha_sfx_info_match)
if (alpha_sfx_info_match LESS 0)
    message(FATAL_ERROR "Unexpected SFXInfoObject constructor")
endif()
string(REPLACE "${alpha_sfx_info_old_constructor}" "" alpha_sfx_info_source "${alpha_sfx_info_source}")
string(REPLACE "#if defined(GEODE_IS_WINDOWS) || defined(GEODE_IS_IOS)" "#if defined(GEODE_IS_IOS)"
    alpha_sfx_info_source "${alpha_sfx_info_source}")
file(WRITE "${alpha_sfx_info_path}" "${alpha_sfx_info_source}")
set(alpha_sfx_folder_path "${alpha_bindings_output}/inline/SFXFolderObject.cpp")
file(READ "${alpha_sfx_folder_path}" alpha_sfx_folder_source)
set(alpha_sfx_folder_init "    if (!SFXInfoObject::init(id, name, folderID, 0, 0)) return false;")
string(FIND "${alpha_sfx_folder_source}" "${alpha_sfx_folder_init}" alpha_sfx_folder_match)
if (alpha_sfx_folder_match LESS 0)
    message(FATAL_ERROR "Unexpected SFXFolderObject initializer")
endif()
string(REPLACE "${alpha_sfx_folder_init}" "${alpha_sfx_folder_init}\n    m_folder = true;"
    alpha_sfx_folder_source "${alpha_sfx_folder_source}")
file(WRITE "${alpha_sfx_folder_path}" "${alpha_sfx_folder_source}")

# Library parsers inline construction of these records. Entries own an array
# at 0x48; music lines own a string at 0x40 after two integer values.
foreach(class IN ITEMS MusicChangelogEntry SFXChangelogEntry)
    alpha_patch_class(GeometryDash "${class}" "    virtual ~${class}()"
        "    ${class}() = win inline;\n    virtual ~${class}()")
    alpha_patch_class(GeometryDash "${class}" "    static ${class}* create(int, int, int);"
        "    static ${class}* create(int value1, int value2, int value3) = win inline;")
    alpha_patch_class(GeometryDash "${class}" "    bool init(int, int, int);"
        "    bool init(int value1, int value2, int value3) = win inline;\n\n    int m_value1;\n    int m_value2;\n    int m_value3;\n    cocos2d::CCArray* m_entries;")
endforeach()
alpha_patch_address(GeometryDash SFXChangelogEntry "~SFXChangelogEntry()" inline 0x480ca0)
alpha_patch_address(GeometryDash MusicChangelogEntry "~MusicChangelogEntry()" inline 0x480d50)
alpha_patch_class(GeometryDash MusicChangelogLine "    virtual ~MusicChangelogLine()"
    "    MusicChangelogLine() = win inline;\n    virtual ~MusicChangelogLine()")
alpha_patch_address(GeometryDash MusicChangelogLine "~MusicChangelogLine()" inline 0x480d00)
alpha_patch_class(GeometryDash MusicChangelogLine "    static MusicChangelogLine* create(int, std::string, int);"
    "    static MusicChangelogLine* create(int value1, std::string value, int value2) = win inline;")
alpha_patch_class(GeometryDash MusicChangelogLine "    bool init(int, std::string, int);"
    "    bool init(int value1, std::string value, int value2) = win inline;\n\n    int m_value1;\n    int m_value2;\n    std::string m_value;")
file(READ "${CMAKE_CURRENT_LIST_DIR}/alpha-inline/AudioChangelog.cpp" alpha_audio_changelog)
file(WRITE "${alpha_bindings_output}/inline/AudioChangelog.cpp" "${alpha_audio_changelog}")

# Native alert callbacks use this level deletion routine. Local editor levels
# are removed from their own array; downloaded levels retain earned progress.
alpha_patch_address(GeometryDash GameLevelManager
    "void deleteLevel(GJGameLevel* level)" 0x21c870)
# GameManager::dataLoaded applies achievement unlocks through this dictionary
# update. ItemInfoPopup's factory allocates 0x2a8 and calls its bound init.
alpha_patch_address(GeometryDash GameManager
    "void completedAchievement(std::string key)" 0x278140)
# Each async texture pair waits for both callbacks before reading the texture
# tag and tail-calling the shared ground or middleground completion routine.
alpha_patch_address(GeometryDash GameManager
    "void finishedLoadingGAsync1(cocos2d::CCObject* obj)" 0x27e1e0)
alpha_patch_address(GeometryDash GameManager
    "void finishedLoadingGAsync2(cocos2d::CCObject* obj)" 0x27e220)
alpha_patch_address(GeometryDash GameManager
    "void finishedLoadingMGAsync1(cocos2d::CCObject* obj)" 0x27df80)
alpha_patch_address(GeometryDash GameManager
    "void finishedLoadingMGAsync2(cocos2d::CCObject* obj)" 0x27dfc0)
# The native gold-chest action sequence schedules this two-second music fade.
alpha_patch_address(GeometryDash SecretRewardsLayer "void fadeInOutMusic()" 0x56d520)
alpha_patch_address(GeometryDash ItemInfoPopup
    "static ItemInfoPopup* create(int id, UnlockType type)" 0x3841e0)
alpha_patch_address(GeometryDash UIButtonConfig "void resetOneBtn()" 0x275970)
alpha_patch_address(GeometryDash GJUINode
    "static GJUINode* create(UIButtonConfig& config)" 0x673b50 0x66d6e0)
alpha_patch_address(GeometryDash GJUINode "void updateButtonPositions()" 0x674070)
alpha_patch_address(GeometryDash GJMGLayer "bool init(int index)" 0x387b80)
alpha_patch_address(GeometryDash GJMGLayer
    "void loadGroundSprites(int count, bool ground1)" 0x3880b0)
alpha_patch_address(GeometryDash LevelOptionsLayer
    "bool init(LevelSettingsObject* object)" 0x45d1a0)

# GJOptionsLayer's toggle builder increments its page count through this
# routine. The older SDK declaration had no Windows implementation at all.
alpha_patch_class(GeometryDash GJOptionsLayer "    void incrementCountForPage(int page);"
    "    void incrementCountForPage(int page) = win 0x39b850;")
alpha_patch_address(GeometryDash GJOptionsLayer "int countForPage(int page)" 0x39b760)
# Video and parental options share the native dictionary-count helper at
# 0x4cfb00, reading m_values at 0x2a8. Their page keys use page_%i.
foreach(class IN ITEMS MoreVideoOptionsLayer ParentalOptionsLayer)
    alpha_patch_address(GeometryDash "${class}" "int countForPage(int page)" 0x4cfb00)
endforeach()
alpha_patch_address(GeometryDash GameStatsManager "void trySelectActivePath()" 0x2da5f0)
alpha_patch_address(GeometryDash LevelBrowserLayer "void updatePageLabel()" 0x3e6a20)
# ProfilePage's native factory inlines initialization of its 0x388-byte object.
# SDK default construction must also initialize pointers released by its dtor.
alpha_patch_class(GeometryDash ProfilePage "    virtual ~ProfilePage()"
    "    ProfilePage() = win inline;\n    virtual ~ProfilePage()")
file(READ "${CMAKE_CURRENT_LIST_DIR}/alpha-inline/ProfilePage.cpp" alpha_profile_constructor)
file(APPEND "${alpha_bindings_output}/inline/ProfilePage.cpp" "\n${alpha_profile_constructor}")
alpha_patch_address(GeometryDash ProfilePage
    "void onNextPage(cocos2d::CCObject* sender)" 0x55a6d0)
alpha_patch_address(GeometryDash ProfilePage
    "void onPrevPage(cocos2d::CCObject* sender)" 0x55a6e0)

# Gameplay's pause factory allocates 0x2a0 and initializes these fields after
# the delegate bases. Its destructor retains ownership of the startpos array.
alpha_patch_class(GeometryDash PauseLayer
    "    bool m_tryingQuit;\n    bool m_uiHidden;\n    cocos2d::CCLabelBMFont* m_startposLabel;\n    int m_startPosIndex;\n    cocos2d::CCArray* m_startPositions;\n    bool m_isEditorLevel;\n    TimeSpeedControl* m_timeSpeedControls;\n    cocos2d::CCLayer* m_layer;\n    Slider* m_musicSlider;\n    Slider* m_sfxSlider;\n    cocos2d::CCMenu* m_showUIMenu;\n    CCMenuItemSpriteExtra* m_platformSettingsButton;"
    "    bool m_tryingQuit;\n    bool m_uiHidden;\n    cocos2d::CCLabelBMFont* m_startPosLabel;\n    int m_startPosIndex;\n    cocos2d::CCArray* m_startPosObjects;\n    bool m_startPosChanged;\n    TimeSpeedControl* m_timeSpeedControl;\n    cocos2d::CCLayer* m_mainLayer;\n    Slider* m_musicSlider;\n    Slider* m_sfxSlider;\n    cocos2d::CCMenu* m_hideMenu;\n    cocos2d::CCSprite* m_questsSprite;")
alpha_patch_class(GeometryDash PauseLayer "TodoReturn checkQuestsStatus()" "void checkQuestsStatus()")
alpha_patch_address(GeometryDash PauseLayer "void checkQuestsStatus()" inline)
alpha_patch_class(GeometryDash ChallengesPage "    TodoReturn canCollectQuests();"
    "    static bool canCollectQuests() = win 0x9e380;")
set(alpha_pause_inline "${alpha_bindings_output}/inline/PauseLayer.cpp")
file(READ "${alpha_pause_inline}" alpha_pause_content)
set(alpha_pause_old_constructor "PauseLayer::PauseLayer() {\n    m_unfocused = false;\n    m_tryingQuit = false;\n}")
string(FIND "${alpha_pause_content}" "${alpha_pause_old_constructor}" alpha_pause_match)
if (alpha_pause_match LESS 0)
    message(FATAL_ERROR "Unexpected PauseLayer constructor implementation")
endif()
string(REPLACE "${alpha_pause_old_constructor}" "" alpha_pause_content "${alpha_pause_content}")
file(READ "${CMAKE_CURRENT_LIST_DIR}/alpha-inline/PauseLayer.cpp" alpha_pause_constructor)
file(WRITE "${alpha_pause_inline}" "${alpha_pause_content}\n${alpha_pause_constructor}")

# The pause time-control factory allocates 0x160, with four child/state pointers
# after CCNode. Native slider callbacks update gameplay and audio speed floats.
alpha_patch_class(GeometryDash TimeSpeedControl "    virtual ~TimeSpeedControl()"
    "    TimeSpeedControl() = win inline;\n    virtual ~TimeSpeedControl()")
alpha_patch_class(GeometryDash TimeSpeedControl "    static TimeSpeedControl* create(GJBaseGameLayer*) = win 0x66d6e0;"
    "    static TimeSpeedControl* create(GJBaseGameLayer* layer) = win 0x66d6e0;")
alpha_patch_class(GeometryDash TimeSpeedControl "    bool init(GJBaseGameLayer*) = win 0x66d790;"
    "    bool init(GJBaseGameLayer* layer) = win 0x66d790;")
alpha_patch_class(GeometryDash TimeSpeedControl "    void onAudioTimeSpeedToggle(cocos2d::CCObject* sender);"
    "    void onAudioTimeSpeedToggle(cocos2d::CCObject* sender) = win 0x66ddd0;")
alpha_patch_class(GeometryDash TimeSpeedControl "    TodoReturn timeSliderChanged(cocos2d::CCObject*);"
    "    void timeSliderChanged(cocos2d::CCObject* sender) = win 0x66dc50;\n\n    GJBaseGameLayer* m_gameLayer;\n    Slider* m_slider;\n    cocos2d::CCLabelBMFont* m_label;\n    CCMenuItemToggler* m_audioToggle;")
alpha_patch_class(GeometryDash GJBaseGameLayer "    float m_unk3380;"
    "    float m_unk3380;\n    win {\n    float m_audioSpeed;\n    }")
file(READ "${CMAKE_CURRENT_LIST_DIR}/alpha-inline/TimeSpeedControl.cpp" alpha_time_speed_constructor)
file(WRITE "${alpha_bindings_output}/inline/TimeSpeedControl.cpp" "${alpha_time_speed_constructor}")
# GameManager::recountUserStats inlines this 43-level set in Windows 2.2091.
# Keep the SDK's owning return value instead of binding an interior address.
alpha_patch_address(GeometryDash LevelTools "static std::unordered_set<int> getLevelList()" inline)
file(APPEND "${alpha_bindings_output}/inline/LevelTools.cpp" [=[
#ifdef GEODE_IS_WINDOWS
std::unordered_set<int> LevelTools::getLevelList() {
    return {
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
        3001, 1001, 1002, 1003, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010,
        4001, 4002, 4003, 5001, 5002, 5003, 5004
    };
}
#endif
]=])
# The native timeline helper has seven arguments. Retain the SDK's older
# eight-argument overload as a forwarding helper, excluding its unused tail.
set(alpha_timeline_signature "static cocos2d::CCPoint posForTimeInternal(float time, cocos2d::CCArray* objects, int speed, bool platformer, bool ignoreMinorAxis, bool ignoreWarp, int& rotateChannel, int unused)")
alpha_patch_address(GeometryDash LevelTools "${alpha_timeline_signature}" inline 0x4676d0)
alpha_patch_class(GeometryDash LevelTools "    ${alpha_timeline_signature}"
    "    static cocos2d::CCPoint posForTimeInternal(float time, cocos2d::CCArray* objects, int speed, bool platformer, bool ignoreMinorAxis, bool ignoreWarp, int& rotateChannel) = win 0x4676d0;\n    ${alpha_timeline_signature}")
file(APPEND "${alpha_bindings_output}/inline/LevelTools.cpp" [=[
#ifdef GEODE_IS_WINDOWS
cocos2d::CCPoint LevelTools::posForTimeInternal(float time, cocos2d::CCArray* objects, int speed, bool platformer, bool ignoreMinorAxis, bool ignoreWarp, int& rotateChannel, int) {
    return posForTimeInternal(time, objects, speed, platformer, ignoreMinorAxis, ignoreWarp, rotateChannel);
}
#endif
]=])
foreach(class IN ITEMS MoreVideoOptionsLayer ParentalOptionsLayer)
    set(alpha_options_inline "${alpha_bindings_output}/inline/${class}.cpp")
    file(READ "${alpha_options_inline}" alpha_options_content)
    set(alpha_options_constructor "${class}::${class}() {\n")
    string(FIND "${alpha_options_content}" "${alpha_options_constructor}" alpha_options_match)
    if (alpha_options_match LESS 0)
        message(FATAL_ERROR "Unexpected ${class} constructor implementation")
    endif()
    # Failed initialization must not release uninitialized dictionary pointers.
    string(REPLACE "${alpha_options_constructor}"
        "${alpha_options_constructor}    m_values = nullptr;\n    m_variables = nullptr;\n    m_prevButton = nullptr;\n    m_nextButton = nullptr;\n"
        alpha_options_content "${alpha_options_content}")
    file(CONFIGURE OUTPUT "${alpha_options_inline}" CONTENT "${alpha_options_content}" @ONLY)
endforeach()

# Shader split targets are doubles: native tween actions 45/46 write 0x2b0
# and 0x2b8. The flags follow at 0x2c0; construction reserves 0x2d0.
alpha_patch_class(GeometryDash GJShaderState
    "GJShaderState() = win 0x5206b0" "GJShaderState() = win inline")
alpha_patch_class(GeometryDash GJShaderState
    "    float m_splitTargetRows;\n    float m_splitTargetCols;"
    "    double m_splitTargetRows;\n    double m_splitTargetCols;")
file(READ "${CMAKE_CURRENT_LIST_DIR}/alpha-inline/GJShaderState.cpp" alpha_shader_constructor)
file(APPEND "${alpha_bindings_output}/inline/GJShaderState.cpp" "\n${alpha_shader_constructor}")

# EffectGameObject::triggerObject calls these with the complete game-layer
# pointer. Their maps and channel vectors live in its GJGameState subobject.
alpha_patch_address(GeometryDash GJBaseGameLayer
    "void gameEventTriggered(GJGameEvent event, int material, int playerID)" 0x33ac80)
alpha_patch_address(GeometryDash GJBaseGameLayer
    "int generateEnterEasingBuffer(int easingType, float easingRate)" 0x314ac0)
alpha_patch_address(GeometryDash GJBaseGameLayer
    "void addCustomEnterEffect(EnterEffectObject* object, bool enter)" 0x314490)
alpha_patch_address(GeometryDash GJBaseGameLayer
    "void stopCustomEnterEffect(EnterEffectObject* object, bool enter)" 0x3147c0)
alpha_patch_address(GeometryDash GJBaseGameLayer
    "void updateExtraGameLayers()" 0x32b500)

# KeybindingsLayer stores settings, an input source and search controls after
# the 0x430-byte tabbed base. The old page dictionaries are absent.
alpha_patch_class(GeometryDash KeybindingsLayer
    "class KeybindingsLayer : TabbedPopupLayer {"
    "class KeybindingsLayer : TabbedPopupLayer {\n    KeybindingsLayer() = win 0x3d56d0;")
set(alpha_keybindings_old_fields [=[    int m_page;
    int m_keyCount;
    int m_maxPage;
    cocos2d::CCDictionary* m_values;
    cocos2d::CCDictionary* m_variables;
    CCMenuItemSpriteExtra* m_prevButton;
    CCMenuItemSpriteExtra* m_nextButton;]=])
set(alpha_keybindings_new_fields [=[    win {
        GameSettingsLayer* m_settings;
        KeybindSource* m_keybindSource;
        bool m_ownsKeybindSource;
        CCTextInputNode* m_searchInput;
        cocos2d::extension::CCScale9Sprite* m_searchBackground;
        CCMenuItemSpriteExtra* m_clearSearchButton;
        cocos2d::CCLayer* m_scrollLayer;
        cocos2d::CCMenu* m_scrollMenu;
        bool m_dragging;
        std::string m_searchQuery;
        int m_searchTab;
    }
    android, imac, m1, ios {
]=])
alpha_patch_class(GeometryDash KeybindingsLayer "${alpha_keybindings_old_fields}"
    "${alpha_keybindings_new_fields}${alpha_keybindings_old_fields}\n    }")
foreach(signature IN ITEMS
    "int countForPage(int page)"
    "void incrementCountForPage(int page)"
    "cocos2d::CCPoint nextPosition(int page)"
    "cocos2d::CCArray* objectsForPage(int page)"
    "void onInfo(cocos2d::CCObject* sender)"
    "void onToggle(cocos2d::CCObject* sender)")
    alpha_patch_class(GeometryDash KeybindingsLayer "    ${signature} = win "
        "    [[missing(win)]]\n    ${signature} = win ")
endforeach()
alpha_patch_class(GeometryDash KeybindingsLayer
    "int countForPage(int page) = win 0x9999999;" "int countForPage(int page);")
foreach(signature IN ITEMS
    "void incrementCountForPage(int page)"
    "cocos2d::CCArray* objectsForPage(int page)"
    "void onInfo(cocos2d::CCObject* sender)"
    "void onToggle(cocos2d::CCObject* sender)")
    alpha_patch_class(GeometryDash KeybindingsLayer "${signature} = win inline;" "${signature};")
endforeach()
alpha_patch_class(GeometryDash KeybindingsLayer
    "cocos2d::CCPoint nextPosition(int page) = win inline, m1"
    "cocos2d::CCPoint nextPosition(int page) = m1")
# Keep SDK key-formatting helpers; exclude the obsolete Windows functions
# that access page dictionaries no longer present in the native class.
file(WRITE "${alpha_bindings_output}/inline/KeybindingsLayer.cpp" [=[#include <Geode/binding/KeybindingsLayer.hpp>

#ifdef GEODE_IS_WINDOWS
const char* KeybindingsLayer::infoKey(int index) {
    return cocos2d::CCString::createWithFormat("info_%i", index)->getCString();
}

const char* KeybindingsLayer::layerKey(int page) {
    return cocos2d::CCString::createWithFormat("layer_%i", page)->getCString();
}

const char* KeybindingsLayer::objectKey(int page) {
    return cocos2d::CCString::createWithFormat("object_%i", page)->getCString();
}

const char* KeybindingsLayer::pageKey(int page) {
    return cocos2d::CCString::createWithFormat("page_%i", page)->getCString();
}
#endif
]=])

# Native settings callbacks return a vector of 0x28-byte menu entries, not
# void. A wrong return type omits the hidden result-buffer argument.
foreach(class IN ITEMS KeybindingsLayer LevelBindingsLayer)
    alpha_patch_class(GeometryDash "${class}"
        "virtual void settingsEntries()" "virtual std::vector<KeybindSettingsEntry> settingsEntries()")
endforeach()
set(alpha_extras_path "${alpha_bindings_output}/Extras.bro")
file(READ "${alpha_extras_path}" alpha_extras)
set(alpha_entry_before "class KeybindSettingsEntry {}")
string(FIND "${alpha_extras}" "${alpha_entry_before}" alpha_entry_match)
if (alpha_entry_match LESS 0)
    message(FATAL_ERROR "Unexpected KeybindSettingsEntry declaration")
endif()
string(REPLACE "${alpha_entry_before}" [=[[[depends(cocos2d::CCObject)]]
class KeybindSettingsEntry {
    char const* m_title;
    char const* m_background;
    cocos2d::CCObject* m_target;
    cocos2d::SEL_MenuHandler m_callback;
    bool m_closeAfterAction;
}]=] alpha_extras "${alpha_extras}")
file(CONFIGURE OUTPUT "${alpha_extras_path}" CONTENT "${alpha_extras}" @ONLY)
alpha_patch_class(GeometryDash KeybindingsSettingsPopup
    "class KeybindingsSettingsPopup : FLAlertLayer, FLAlertLayerProtocol {"
    "class KeybindingsSettingsPopup : FLAlertLayer, FLAlertLayerProtocol {\n    KeybindingsSettingsPopup() = win inline;")
alpha_patch_class(GeometryDash KeybindingsSettingsPopup
    "static KeybindingsSettingsPopup* create(KeybindingsLayer*, std::vector<KeybindSettingsEntry> const&);"
    "static KeybindingsSettingsPopup* create(KeybindingsLayer* layer, std::vector<KeybindSettingsEntry> const& entries) = win inline;")
alpha_patch_class(GeometryDash KeybindingsSettingsPopup
    "bool init(KeybindingsLayer*, std::vector<KeybindSettingsEntry> const&);"
    "bool init(KeybindingsLayer* layer, std::vector<KeybindSettingsEntry> const& entries) = win 0x3d8e80;")
alpha_patch_class(GeometryDash KeybindingsSettingsPopup
    "void onEntry(cocos2d::CCObject* sender);"
    "void onEntry(cocos2d::CCObject* sender) = win 0x3d93c0;")
alpha_patch_class(GeometryDash KeybindingsSettingsPopup
    "    void onReset(cocos2d::CCObject* sender);"
    [=[    void onReset(cocos2d::CCObject* sender);

    KeybindingsLayer* m_keybindingsLayer;
    std::vector<KeybindSettingsEntry> m_entries;
    int m_pendingAction;
    std::string m_pendingData;
    int m_pendingImportMode;]=])
file(WRITE "${alpha_bindings_output}/inline/KeybindingsSettingsPopup.cpp" [=[#include <Geode/binding/KeybindingsSettingsPopup.hpp>

#ifdef GEODE_IS_WINDOWS
KeybindingsSettingsPopup::KeybindingsSettingsPopup() :
    m_keybindingsLayer(nullptr), m_entries(), m_pendingAction(0), m_pendingData(), m_pendingImportMode(0) {}

KeybindingsSettingsPopup* KeybindingsSettingsPopup::create(KeybindingsLayer* layer, std::vector<KeybindSettingsEntry> const& entries) {
    auto popup = new KeybindingsSettingsPopup();
    if (popup->init(layer, entries)) {
        popup->autorelease();
        return popup;
    }
    delete popup;
    return nullptr;
}
#endif
]=])
set(alpha_geometry_path "${alpha_bindings_output}/GeometryDash.bro")
file(READ "${alpha_geometry_path}" alpha_geometry)
foreach(class IN ITEMS KeybindingsLayer KeybindingsSettingsPopup)
    set(alpha_attribute_before "[[link(android)]]\nclass ${class} ")
    string(FIND "${alpha_geometry}" "${alpha_attribute_before}" alpha_attribute_match)
    if (alpha_attribute_match LESS 0)
        message(FATAL_ERROR "Unexpected ${class} attributes")
    endif()
    string(REPLACE "${alpha_attribute_before}"
        "[[link(android), depends(KeybindSettingsEntry)]]\nclass ${class} "
        alpha_geometry "${alpha_geometry}")
endforeach()
file(CONFIGURE OUTPUT "${alpha_geometry_path}" CONTENT "${alpha_geometry}" @ONLY)

# AppDelegate's primary table is 0x719f88; its CCSceneDelegate subobject is
# at 0xd8 with table 0x719f78. These callbacks remain native Windows APIs.
if (WIN32)
    alpha_patch_class(GeometryDash AppDelegate
        "virtual void applicationDidEnterBackground();"
        "virtual void applicationDidEnterBackground() = win 0x98630;")
    alpha_patch_class(GeometryDash AppDelegate
        "virtual void applicationWillResignActive() = imac"
        "virtual void applicationWillResignActive() = win 0x985b0, imac")
    alpha_patch_class(GeometryDash AppDelegate
        "virtual void applicationWillTerminate();"
        "virtual void applicationWillTerminate() = win 0x985c0;")
    alpha_patch_class(GeometryDash AppDelegate
        "virtual void willSwitchToScene(cocos2d::CCScene* scene);"
        "virtual void willSwitchToScene(cocos2d::CCScene* scene) = win 0x98cc0;")
endif()

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
# Native factory allocates 0x328, initializes the robot animation and icon type.
alpha_patch_address(GeometryDash GJRobotSprite
    "static GJRobotSprite* create(int frame)" 0x3ac740 0x185e70)

# GameLevelManager's native constructor is 0x217310, allocation size 0x690.
# Upstream now declares its new containers and state. Retain typed delegates
# at the native offsets verified through the GLM HTTP dispatch table.
alpha_patch_class(GeometryDash GameLevelManager
    "    virtual ~GameLevelManager()"
    "    GameLevelManager() = win 0x217310;\n    virtual ~GameLevelManager()")
if (WIN32)
    # Native daily state begins at 0x3bc. Its content-state flag is at 0x3f0;
    # the constructor initializes the four preceding integers at 0x3e0..0x3ec.
    alpha_patch_class(GeometryDash GameLevelManager
        "    int m_dailyTimeLeft;" "    PAD = win 0x4;\n    int m_dailyTimeLeft;")
    alpha_patch_class(GeometryDash GameLevelManager
        "    int m_onGetContentStatusCompletedIdx4;\n" "")
    foreach(delegate IN ITEMS
        "m_levelControlsDelegate|LevelControlsDelegate"
        "m_adminDelegate|CommentAdminDelegate"
        "m_sendThreadMessageDelegate|ThreadMessageDelegate"
        "m_editorSoundUploadDelegate|EditorSoundUploadDelegate"
        "m_getEditorSoundsDelegate|EditorSoundListDelegate"
        "m_downloadEditorSoundDelegate|EditorSoundDownloadDelegate"
        "m_deleteEditorSoundDelegate|EditorSoundDeleteDelegate"
        "m_contentStatusDelegate|GJContentStatusDelegate"
        "m_songUsageDelegate|SongUsageDelegate")
        string(REPLACE "|" ";" parts "${delegate}")
        list(GET parts 0 field)
        list(GET parts 1 type)
        alpha_patch_class(GeometryDash GameLevelManager
            "    void* ${field};" "    ${type}* ${field};")
    endforeach()
endif()

# ZeroConstructor cannot initialize the new native containers.
set(alpha_manager_inline "${alpha_bindings_output}/inline/GameLevelManager.cpp")
file(READ "${alpha_manager_inline}" alpha_manager_content)
set(alpha_manager_old "ref = new GameLevelManager(geode::ZeroConstructor);")
string(FIND "${alpha_manager_content}" "${alpha_manager_old}" alpha_manager_match)
if (alpha_manager_match LESS 0)
    message(FATAL_ERROR "Unexpected GameLevelManager singleton implementation")
endif()

# Native GJUserScore construction/destruction allocates 0x3b8 bytes. The
# dictionary factory stores request status at 0x2f8 and toggles at 0x31d;
# new state precedes the social links and request flags. Preserve that state
# without guessing the names of fields not yet identified.
if (WIN32)
    # Upstream now names the social/request fields previously reserved here.
    # 0x218bc0 is a deleting destructor with an extra flags parameter. Its
    # body only destroys the declared strings and the CCNode base.
    alpha_patch_class(GeometryDash GJUserScore
        "    virtual ~GJUserScore() = win 0x218bc0;"
        "    virtual ~GJUserScore() = win inline;")
    file(APPEND "${alpha_bindings_output}/inline/GJUserScore.cpp"
        "\n#ifdef GEODE_IS_WINDOWS\nGJUserScore::~GJUserScore() {}\n#endif\n")
    alpha_patch_class(GeometryDash GJUserScore
        "static GJUserScore* create(cocos2d::CCDictionary* dict) = imac"
        "static GJUserScore* create(cocos2d::CCDictionary* dict) = win 0x26c000, imac")
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

# Upstream now includes the additional list state before its strings. Native
# create allocates 0x250, with m_levelsDict at 0x240 and m_M_ID at 0x24c.

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
alpha_patch_address(GeometryDash FMODAudioEngine
    "void triggerQueuedMusic(FMODQueuedMusic music)" 0x64620)
# Queued music calls the seven-argument native start helper. The SDK's older
# eight-argument declaration had no Windows implementation; its tail is unused.
set(alpha_start_music_signature "void startMusic(int start, int end, int fadeIn, int fadeOut, bool loop, int musicID, bool noResume, bool dontReset)")
alpha_patch_class(GeometryDash FMODAudioEngine
    "${alpha_start_music_signature} = win 0x63270, imac"
    "${alpha_start_music_signature} = win inline, imac")
alpha_patch_class(GeometryDash FMODAudioEngine "    ${alpha_start_music_signature}"
    "    void startMusic(int start, int end, int fadeIn, int fadeOut, bool loop, int musicID, bool noResume) = win 0x63270;\n    ${alpha_start_music_signature}")
file(APPEND "${alpha_bindings_output}/inline/FMODAudioEngine.cpp" [=[
#ifdef GEODE_IS_WINDOWS
void FMODAudioEngine::startMusic(int start, int end, int fadeIn, int fadeOut, bool loop, int musicID, bool noResume, bool) {
    startMusic(start, end, fadeIn, fadeOut, loop, musicID, noResume);
}
#endif
]=])
alpha_patch_class(GeometryDash FMODAudioEngine
    "    bool m_globalChannelPaused;\n    std::vector<int> m_stoppedChannelIDs;"
    "    bool m_suspendedGlobalAudio;\n    std::vector<int> m_suspendedChannels;")
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
    "    FMODAudioState() = win 0x5bdc0, inline;"
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
    "char const* getCoinKey(int coinNumber)" 0x264610 0x264630)
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
    "    cocos2d::CCPoint offsetForKey(int id) = win 0x19f3e0, m1 0x548e7, imac 0x603db0;"
    "    static cocos2d::CCPoint offsetForKey(int id) = win 0x19f3e0, m1 0x548e7, imac 0x603db0;")
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
# Upstream now declares the trailing toggle-button map on Windows.
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
alpha_patch_address(GeometryDash SetupTriggerPopup
    "void onToggleTriggerValue(cocos2d::CCObject* sender)" 0x60f240)
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
    "    // virtual int requestPage(int) = win 0x9999999;"
    "    inline virtual int requestPage(int) = 0;")
# The pinned upstream declarations now reserve its native 0x70-byte tail.
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
    "    virtual void mountVirtualRow(int, cocos2d::CCNode*, cocos2d::CCMenu*) = win 0x9999999;"
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
    "virtual TodoReturn onFieldEnter()" "virtual void onFieldEnter()")
alpha_patch_class(GeometryDash EditorWindowPopup
    "    virtual void onBuildWindowContent() = win 0x9999999;"
    "    inline virtual void onBuildWindowContent() = 0;")
# Upstream now declares the native window fields instead of reserved storage.

# EditorUI inlines the find window's initialization. Its final member is a
# 32-bit group ID, updated by valueDidChange at 0x1809e0, not a boolean.
alpha_patch_address(GeometryDash FindObjectPopup "bool init(EditorUI*)" inline)
alpha_patch_class(GeometryDash FindObjectPopup
    "    bool m_unknownBool;" "    int m_groupID;")
alpha_patch_class(GeometryDash FindObjectPopup
    "virtual TodoReturn onFieldEnter()" "virtual void onFieldEnter()")
alpha_patch_class(GeometryDash FindObjectPopup
    "    TodoReturn runFind(int);" "    void runFind(int direction) = win 0x1809f0;")
alpha_patch_class(GeometryDash FindObjectPopup
    "    void onFindStep(cocos2d::CCObject* sender);"
    "    void onFindStep(cocos2d::CCObject* sender) = win 0x180aa0;")
alpha_patch_class(GeometryDash FindObjectPopup
    "    void onSelect(cocos2d::CCObject* sender);"
    "    void onSelect(cocos2d::CCObject* sender) = win inline;")
alpha_patch_class(GeometryDash FindObjectPopup
    "    void onOptions(cocos2d::CCObject* sender);"
    "    void onOptions(cocos2d::CCObject* sender) = win 0x180ad0;")
file(READ "${CMAKE_CURRENT_LIST_DIR}/alpha-inline/FindObjectPopup.cpp" alpha_find_window)
file(WRITE "${alpha_bindings_output}/inline/FindObjectPopup.cpp" "${alpha_find_window}")

# ScrollViewPopup's native base has one retained object at 0x298, a further
# eight bytes of state, and a pure buildContent entry at vtable offset 0x438.
alpha_patch_class(GeometryDash ScrollViewPopup
    "    virtual ~ScrollViewPopup() = win 0x6218d0;"
    "    ScrollViewPopup() = win 0x47f7b0;\n    virtual ~ScrollViewPopup() = win inline;")
alpha_patch_class(GeometryDash ScrollViewPopup
    "    // virtual float buildContent(cocos2d::CCNode*, float, float) = win 0x9999999;"
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
# Upstream now names the extra scalar and emote fields before the delegate.
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

# Native MusicDownloadManager allocates 0x3d0. SFX parsing writes the object
# dictionary at 0x198, artists/changelog arrays at 0x1a0/0x1a8 and counters
# at 0x1b0/0x1b4. Preserve the native object preceding the dictionary.
alpha_patch_class(GeometryDash MusicDownloadManager
    "    cocos2d::CCDictionary* m_sfxObjects;"
    "    PAD = win 0x8;\n    cocos2d::CCDictionary* m_sfxObjects;")
alpha_patch_class(GeometryDash MusicDownloadManager
    "    cocos2d::CCArray* m_sfxArtists;\n    cocos2d::CCObject* m_unk1a0;\n    cocos2d::CCObject* m_unk1a8;"
    "    cocos2d::CCArray* m_sfxArtists;\n    cocos2d::CCArray* m_sfxChangelog;")
# The new upstream news tail extends past the native 0x3d0 allocation.
# Retain verified container fields and reserve the unverified news storage.
alpha_patch_class(GeometryDash MusicDownloadManager
    "    PAD = win 0x24;\n    int m_newsPages;\n    PAD = win 0x8;\n    bool m_updatingNewsFeed;\n    bool m_unkBool2;\n    bool m_requestedNewsFeedUpdate;\n    int m_unkInt5;"
    "    PAD = win 0x30;")
if (WIN32)
    alpha_patch_class(GeometryDash MusicDownloadManager
        "    virtual ~MusicDownloadManager()"
        "    MusicDownloadManager() = win 0x480540;\n    virtual ~MusicDownloadManager()")
endif()
alpha_patch_address(GeometryDash MusicDownloadManager "bool isSongDownloaded(int id)" 0x4845a0)

# The only native mixerResume caller sets m_stopped (0x270) back to false,
# resumes the suspended channel vector at 0x278 and restarts CCNode scheduling.
alpha_patch_address(GeometryDash FMODAudioEngine "void start()" 0x5dfc0)
if (WIN32)
    alpha_patch_class(GeometryDash FMODAudioEngine
        "    TodoReturn resumeSuspendedAudio();"
        "    void resumeSuspendedAudio() = win 0x5e030;")
endif()

# The current search page adds a no-stars sprite at 0x1c8. The native
# difficulty/length arrays move to 0x1d0/0x1d8 and their dictionary to 0x1e0.
alpha_patch_class(GeometryDash LevelSearchLayer
    "    cocos2d::CCSprite* m_noStarSprite;"
    "    cocos2d::CCSprite* m_noStarsSprite;")
set(alpha_search_inline "${alpha_bindings_output}/inline/LevelSearchLayer.cpp")
file(READ "${alpha_search_inline}" alpha_search_content)
set(alpha_search_before "    m_starsSprite = nullptr;")
string(FIND "${alpha_search_content}" "${alpha_search_before}" alpha_search_match)
if (alpha_search_match LESS 0)
    message(FATAL_ERROR "Unexpected LevelSearchLayer constructor implementation")
endif()
string(REPLACE "${alpha_search_before}"
    "${alpha_search_before}\n#ifdef GEODE_IS_WINDOWS\n    m_noStarsSprite = nullptr;\n#endif"
    alpha_search_content "${alpha_search_content}")
file(CONFIGURE OUTPUT "${alpha_search_inline}" CONTENT "${alpha_search_content}" @ONLY)
alpha_patch_address(GeometryDash LevelSearchLayer "bool checkDiff(int diff)" 0x43aaf0)
alpha_patch_address(GeometryDash LevelSearchLayer "bool checkTime(int time)" 0x43b120)

# Two native callers and the fifteen-entry SpecialRewardItem jump table agree.
# Shards/keys map to player-stat keys; orbs, diamonds and custom items return
# an empty string, as do values outside the native enum range.
alpha_patch_address(GeometryDash GJRewardItem
    "static std::string rewardItemToStat(SpecialRewardItem rewardItem)" 0x2f9190)
# Native cached user lists share GameLevelManager's dictionary at 0x480.
alpha_patch_class(GeometryDash GameLevelManager
    "cocos2d::CCArray* getStoredUserList(UserListType type);"
    "cocos2d::CCArray* getStoredUserList(UserListType type) = win 0x257b80;")

# Native editor allocation is 0x3e08. Its locked-layer count precedes the
# saved-position vector at 0x3b90, and new session state precedes the removal
# flag at 0x3c98. The delayed-spawn vectors are at 0x3cc0/0x3cd8. Preserve
# the additional native tail, including containers initialized by the ctor.
if (WIN32)
    alpha_patch_class(GeometryDash LevelEditorLayer
        "LevelEditorLayer() = imac" "LevelEditorLayer() = win 0x3ec2f0, imac")
    alpha_patch_class(GeometryDash LevelEditorLayer
        "    std::vector<cocos2d::CCPoint> m_savedPositions;"
        "    int m_lockedLayerCount;\n    PAD = win 0x4;\n    std::vector<cocos2d::CCPoint> m_savedPositions;")
    alpha_patch_class(GeometryDash LevelEditorLayer
        "    bool m_removingAll;" "    PAD = win 0x70;\n    bool m_removingAll;")
    alpha_patch_class(GeometryDash LevelEditorLayer
        "    std::vector<int> m_unk3b48;"
        "    std::vector<int> m_unk3b48;\n    PAD = win 0x98;")
endif()
# breakApartTextObject calls this removal path, which records optional undo
# data, removes the object from the base layer and unregisters its triggers.
alpha_patch_address(GeometryDash LevelEditorLayer
    "void removeObject(GameObject* object, bool noUndo)" 0x3f21b0 0x223320)
# EditorPauseLayer calls the same max-position scan as in the previous game.
alpha_patch_address(GeometryDash LevelEditorLayer
    "float getLastObjectX()" 0x3fd290 0x36d6b0)
# removeObject and twelve matched native callers unregister through this path.
alpha_patch_address(GeometryDash LevelEditorLayer
    "void removeSpecial(GameObject* object)" 0x3f5290)

# Native getXMin reads EditorUI's layer at 0x5e8, temporarily resets its
# object-layer position and restores it after the coordinate conversion.
# The integer parameter is unused in this Windows implementation.
alpha_patch_address(GeometryDash EditorUI "float getXMin(int offset)" 0x1bf270)
# Both mapped HTTP completion paths remove their tag from the active-download
# dictionary at 0x140 through this function, including heap-backed tag strings.
alpha_patch_address(GeometryDash GJMultiplayerManager
    "void removeDLFromActive(char const* tag)" 0x390a10)

# Native NumberInputLayer is 0x300: title label 0x2a0, OK button 0x2a8,
# display prefix 0x2b8, input 0x2d8 and delegate 0x2f8. onDone is the
# additional virtual slot at 0x438, also overridden by ParentalGateLayer.
if (WIN32)
    alpha_patch_class(GeometryDash NumberInputLayer
        "    NumberInputLayer() = inline;"
        "    NumberInputLayer() = win 0x4a1060, inline;")
    alpha_patch_class(GeometryDash NumberInputLayer
        "    void onDone(cocos2d::CCObject* sender);"
        "    virtual void onDone(cocos2d::CCObject* sender) = win 0x4a1b10;")
    # Use upstream's title/prefix fields and iOS-only inline implementations.
    # The native derived factory reserves one additional integer and alignment.
    alpha_patch_class(GeometryDash ParentalGateLayer
        "static ParentalGateLayer* create() = m1"
        "static ParentalGateLayer* create() = win 0x4d3ba0, m1")
    file(WRITE "${alpha_bindings_output}/inline/ParentalGateLayer.cpp"
        "#include <Geode/binding/ParentalGateLayer.hpp>\n\n#ifdef GEODE_IS_WINDOWS\nParentalGateLayer::ParentalGateLayer() : m_solution(0) {}\n#endif\n")
endif()
alpha_patch_address(GeometryDash NumberInputLayer
    "void onNumber(cocos2d::CCObject* sender)" 0x4a1820 0x2157c0)
alpha_patch_address(GeometryDash NumberInputLayer "void updateNumberState()" 0x4a1990)

# setTargetCount schedules this exact update callback at 30 Hz. The native
# body advances the count toward its target and disables completed counters.
alpha_patch_address(GeometryDash CCCounterLabel "void updateCounter(float dt)" 0x46ee0)
# Four matched callers update label objects through the map at 0x1118. Native
# formatting caps values at 999999999 and respects the label's update lock.
alpha_patch_address(GeometryDash GJBaseGameLayer
    "void updateCounters(int itemId, int value)" 0x33cb90)

# Native construction reserves 0x828 and initializes a new integer at 0x3f0.
# The two item-state sets start at 0x3f8/0x438; later trigger-ID storage is
# therefore at 0x560, not the previous SDK's 0x558.
if (WIN32)
    alpha_patch_class(GeometryDash GJEffectManager
        "    // GJEffectManager();" "    GJEffectManager() = win 0x362520;")
    # Upstream now names the native integer preceding the item-state sets.
    # The old native store method only inserts this unique pair. Supply the
    # equivalent SDK implementation against the corrected current container.
    alpha_patch_class(GeometryDash GJEffectManager
        "void storeTriggeredID(int objectUniqueID, int playerUniqueID) = imac"
        "void storeTriggeredID(int objectUniqueID, int playerUniqueID) = win inline, imac")
    file(APPEND "${alpha_bindings_output}/inline/GJEffectManager.cpp"
        "\n#ifdef GEODE_IS_WINDOWS\nvoid GJEffectManager::storeTriggeredID(int objectUniqueID, int playerUniqueID) {\n    m_unkMap498.emplace(objectUniqueID, playerUniqueID);\n}\n#endif\n")
endif()
alpha_patch_address(GeometryDash GJEffectManager
    "void removeTriggeredID(int objectUniqueID, int playerUniqueID)" 0x36c850)

# Compare the SDK member-pointer slots with native RTTI tables, not just the
# presence of an executable address. New callback groups moved older slots.
if (WIN32)
    alpha_patch_class(GeometryDash GManager [=[    virtual void dataLoaded(DS_Dictionary* dict) = win inline, imac 0x4a5bf0, m1 0x40cd80;
    virtual void firstLoad() = win inline, imac 0x4a5c00, m1 0x40cd84;
    virtual TodoReturn didEncodeForSave() = win 0x3f5c0;
    virtual TodoReturn saveFinished(bool) = win 0x3f5c0;]=]
        [=[    virtual void didEncodeForSave() = win 0x3f5c0;
    virtual void saveFinished(bool) = win 0x3f5c0;
    virtual void dataLoaded(DS_Dictionary* dict) = win inline, imac 0x4a5bf0, m1 0x40cd80;
    virtual void firstLoad() = win inline, imac 0x4a5c00, m1 0x40cd84;]=])
    foreach(class IN ITEMS GameManager LocalLevelManager)
        alpha_patch_class(GeometryDash "${class}"
            "virtual TodoReturn didEncodeForSave()" "virtual void didEncodeForSave()")
        alpha_patch_class(GeometryDash "${class}"
            "virtual TodoReturn saveFinished(bool)" "virtual void saveFinished(bool)")
    endforeach()

    # Upstream now places the hitbox callbacks before addToSection.
    alpha_patch_class(GeometryDash GJBaseGameLayer
        "virtual TodoReturn getHitboxTrailMax()" "virtual int getHitboxTrailMax()")
    foreach(class IN ITEMS LevelEditorLayer PlayLayer)
        foreach(method IN ITEMS shouldDrawDebugHitboxes shouldDrawHitboxTrail shouldRecordHitboxTrail)
            alpha_patch_class(GeometryDash "${class}"
                "virtual TodoReturn ${method}()" "virtual bool ${method}()")
        endforeach()
        alpha_patch_class(GeometryDash "${class}"
            "virtual TodoReturn getHitboxTrailMax()" "virtual int getHitboxTrailMax()")
    endforeach()

    alpha_patch_class(GeometryDash CustomSFXDelegate
        "    virtual bool overridePlaySFX(SFXInfoObject* object) = inline;\n    virtual int getActiveCustomSFXID() = inline;"
        "    virtual int getActiveCustomSFXID() = inline;\n    virtual bool overridePlaySFX(SFXInfoObject* object) = inline;")
    alpha_patch_class(GeometryDash TableViewCellDelegate [=[    virtual bool shouldSnapToSelected() = inline;
    virtual int getCellDelegateType() = inline;
    virtual TodoReturn getSelectedCellCustomIdx() = win 0x98e00;
    virtual TodoReturn allowsCustomSFXActions() = win 0x5a3a0;
    virtual bool isCustomIDSelected(int) = win 0x5a3a0;
    virtual TodoReturn inCustomSFXTree() = win 0x5a3a0;
    virtual TodoReturn showsSFXInfoButton() = win 0x5a3a0;]=]
        [=[    virtual int getSelectedCellCustomIdx() = win 0x98e00;
    virtual bool allowsCustomSFXActions() = win 0x5a3a0;
    virtual bool isCustomIDSelected(int) = win 0x5a3a0;
    virtual bool inCustomSFXTree() = win 0x5a3a0;
    virtual bool showsSFXInfoButton() = win 0x5a3a0;
    virtual bool shouldSnapToSelected() = inline;
    virtual int getCellDelegateType() = inline;]=])
    alpha_patch_class(GeometryDash SFXBrowser
        "virtual TodoReturn getSelectedCellCustomIdx()" "virtual int getSelectedCellCustomIdx()")
    foreach(method IN ITEMS allowsCustomSFXActions inCustomSFXTree showsSFXInfoButton)
        alpha_patch_class(GeometryDash SFXBrowser
            "virtual TodoReturn ${method}()" "virtual bool ${method}()")
    endforeach()
    alpha_patch_class(GeometryDash SFXBrowser
        "    virtual bool showsSFXInfoButton() = win 0x6202a0;"
        "    virtual bool showsSFXInfoButton() = win 0x6202a0;\n    virtual int getCellDelegateType() = win 0x98e00;")
endif()

# Native constructors reserve the additional tab state and browser pointers.
# Their derived delegates must follow the complete native base allocation.
if (WIN32)
    alpha_patch_class(GeometryDash TabbedPopupLayer
        "    virtual ~TabbedPopupLayer()"
        "    TabbedPopupLayer() = win 0x19cc00;\n    virtual ~TabbedPopupLayer()")
    alpha_patch_class(GeometryDash TabbedPopupLayer
        "    PAD = win 0x50, android64 0x48, android32 0x28;"
        "    PAD = win 0x48, android64 0x48, android32 0x28;")
    alpha_patch_class(GeometryDash GameSettingsLayer
        "    TodoReturn toggleVisibility(bool);"
        "    TodoReturn toggleVisibility(bool);\n    GJBaseGameLayer* m_gameLayer;\n    PAD = win 0x68;")
    # Upstream now declares the five native browser pointers at 0x320..0x340.

    # These native entry points are scalar deleting destructors. Pass zero
    # explicitly to retain the allocation, then use the SDK's destructor lock
    # protocol to avoid destroying native members and bases a second time.
    function(alpha_patch_deleting_destructor class address)
        alpha_patch_class(GeometryDash "${class}"
            "~${class}() = win ${address}" "~${class}() = win inline")
        set(alpha_destructor_source "#include <Geode/binding/${class}.hpp>\n#include <Geode/modify/Traits.hpp>\n\n#ifdef GEODE_IS_WINDOWS\n${class}::~${class}() {\n    if (!geode::DestructorLock::isLocked(this)) {\n        using FunctionType = void(*)(${class}*, unsigned int);\n        reinterpret_cast<FunctionType>(geode::base::get() + ${address})(this, 0);\n        new (this) ${class}(geode::CutoffConstructor, sizeof(${class}));\n        geode::DestructorLock::addLock(this);\n    }\n}\n#endif\n")
        set(alpha_destructor_existing "")
        if (EXISTS "${alpha_bindings_source}/inline/${class}.cpp" OR ARGV2 STREQUAL "PRESERVE_INLINE")
            file(READ "${alpha_bindings_output}/inline/${class}.cpp" alpha_destructor_existing)
        endif()
        file(WRITE "${alpha_bindings_output}/inline/${class}.cpp"
            "${alpha_destructor_existing}\n${alpha_destructor_source}")
    endfunction()
    alpha_patch_deleting_destructor(TabbedPopupLayer 0x19cd00)
    alpha_patch_deleting_destructor(GameSettingsLayer 0x2c2530)
    alpha_patch_deleting_destructor(KeybindingsSettingsPopup 0x3d8e30 PRESERVE_INLINE)
    alpha_patch_deleting_destructor(GJMGLayer 0x386c60)
    alpha_patch_deleting_destructor(FindObjectPopup 0x180260)
    alpha_patch_deleting_destructor(PauseLayer 0x4d6600)
    alpha_patch_deleting_destructor(TimeSpeedControl 0x47080 PRESERVE_INLINE)
    alpha_patch_deleting_destructor(MusicBrowser 0x479ee0)
    alpha_patch_deleting_destructor(SFXBrowser 0x6164e0)
    alpha_patch_deleting_destructor(SFXInfoObject 0x480bc0)
endif()

# Options factories allocate 0x78 and 0x300. The native row reads its count at
# 0x6c; filtering uses the popup's search string at 0x2d0, not a minimum count.
alpha_patch_address(GeometryDash OptionsObject
    "static OptionsObject* create(int id, bool enabled, std::string name, OptionsObjectDelegate* delegate)" 0x3a99e0)
alpha_patch_address(GeometryDash OptionsScrollLayer
    "void onClose(cocos2d::CCObject* sender)" 0x47f2d0)
if (WIN32)
    alpha_patch_class(GeometryDash OptionsObject
        "    virtual ~OptionsObject() = win 0x399c70;"
        "    OptionsObject() = win inline;\n    virtual ~OptionsObject() = win inline;")
    file(APPEND "${alpha_bindings_output}/inline/OptionsObject.cpp"
        "\n#ifdef GEODE_IS_WINDOWS\nOptionsObject::OptionsObject() : m_optionID(0), m_enabled(false), m_delegate(nullptr), m_type(0), m_count(0), m_startPosObject(nullptr) {}\nOptionsObject::~OptionsObject() {}\n#endif\n")

    alpha_patch_class(GeometryDash OptionsScrollLayer
        "    virtual ~OptionsScrollLayer()"
        "    OptionsScrollLayer() = win inline;\n    virtual ~OptionsScrollLayer()")
    alpha_patch_class(GeometryDash OptionsScrollLayer
        "static OptionsScrollLayer* create(cocos2d::CCArray*, bool, std::string, MusicSearchResult*) = m1"
        "static OptionsScrollLayer* create(cocos2d::CCArray*, bool, std::string, MusicSearchResult*) = win 0x47e380, m1")
    alpha_patch_class(GeometryDash OptionsScrollLayer
        "bool init(cocos2d::CCArray*, bool, std::string, MusicSearchResult*) = win inline"
        "bool init(cocos2d::CCArray*, bool, std::string, MusicSearchResult*) = win 0x47e520")
    alpha_patch_class(GeometryDash OptionsScrollLayer
        "cocos2d::CCArray* getRelevantObjects(cocos2d::CCArray* objects) = win inline;"
        "cocos2d::CCArray* getRelevantObjects(cocos2d::CCArray* objects) = win 0x47f400;")
    alpha_patch_class(GeometryDash OptionsScrollLayer
        "void onClearAll(cocos2d::CCObject* sender);"
        "void onClearAll(cocos2d::CCObject* sender) = win 0x47eef0;")
    alpha_patch_class(GeometryDash OptionsScrollLayer
        "void onClearSearchText(cocos2d::CCObject* sender);"
        "void onClearSearchText(cocos2d::CCObject* sender) = win 0x47efa0;")
    alpha_patch_class(GeometryDash OptionsScrollLayer
        "void onMatchToggle(cocos2d::CCObject* sender);"
        "void onMatchToggle(cocos2d::CCObject* sender) = win 0x47ee60;")
    alpha_patch_class(GeometryDash OptionsScrollLayer
        "    int m_minCount;"
        "    PAD = win 0x4;\n    CCTextInputNode* m_searchInput;\n    cocos2d::extension::CCScale9Sprite* m_searchBackground;\n    CCMenuItemSpriteExtra* m_clearSearchButton;\n    std::string m_searchQuery;\n    CCMenuItemSpriteExtra* m_matchButton;\n    MusicSearchResult* m_searchResult;")
    set(alpha_options_inline "${alpha_bindings_output}/inline/OptionsScrollLayer.cpp")
    file(READ "${alpha_options_inline}" alpha_options_content)
    set(alpha_options_filter [=[cocos2d::CCArray* OptionsScrollLayer::getRelevantObjects(cocos2d::CCArray* objects) {
    auto relevantObjects = cocos2d::CCArray::create();
    for (auto object : geode::cocos::CCArrayExt<OptionsObject, false>(objects)) {
        if (object->m_count > m_minCount || object->m_enabled) {
            relevantObjects->addObject(object);
        }
    }
    return relevantObjects;
}]=])
    set(alpha_options_init [=[// TODO: 2.209
bool OptionsScrollLayer::init(cocos2d::CCArray* objects, bool recreate, std::string p0, MusicSearchResult* p1) {
    if (!FLAlertLayer::init(150)) return false;
    m_noElasticity = true;
    m_recreateList = recreate;
    // m_minCount = minimum;
    if (objects) {
        m_optionObjects = objects;
        objects->retain();
    }
    auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();
    m_buttonMenu = cocos2d::CCMenu::create();
    m_mainLayer->addChild(m_buttonMenu, 10);
    m_joystickConnected = -1;
    auto closeButton = CCMenuItemSpriteExtra::create(cocos2d::CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png"), this, menu_selector(OptionsScrollLayer::onClose));
    closeButton->setSizeMult(1.6f);
    closeButton->setPosition(m_buttonMenu->convertToNodeSpace(winSize * .5f + cocos2d::CCPoint { -210.f, 135.f }));
    this->setupList(m_optionObjects);
    return true;
}]=])
    foreach(body IN ITEMS "${alpha_options_filter}" "${alpha_options_init}")
        string(FIND "${alpha_options_content}" "${body}" alpha_options_match)
        if (alpha_options_match LESS 0)
            message(FATAL_ERROR "Unexpected OptionsScrollLayer inline implementation")
        endif()
        string(REPLACE "${body}" "" alpha_options_content "${alpha_options_content}")
    endforeach()
    file(CONFIGURE OUTPUT "${alpha_options_inline}" CONTENT "${alpha_options_content}" @ONLY)
    file(APPEND "${alpha_options_inline}"
        "\n#ifdef GEODE_IS_WINDOWS\nOptionsScrollLayer::OptionsScrollLayer() : m_optionObjects(nullptr), m_listLayer(nullptr), m_recreateList(false), m_searchInput(nullptr), m_searchBackground(nullptr), m_clearSearchButton(nullptr), m_matchButton(nullptr), m_searchResult(nullptr) {}\n#endif\n")
    alpha_patch_deleting_destructor(OptionsScrollLayer 0x47a040)
    alpha_patch_deleting_destructor(OptionsCell 0x151070)
endif()

# MusicSearchResult::updateObjects calls artist filtering at 0x4981c0, tag
# filtering at 0x497d00 and future counts at 0x498460. The native constructor
# initializes the All/Any flag at 0x8e and view-filter mode at 0x94.
if (WIN32)
    alpha_patch_class(GeometryDash MusicSearchResult
        "    bool m_unk86;" "    bool m_matchAnyTags;")
    alpha_patch_class(GeometryDash MusicSearchResult
        "    int m_viewFilter;" "    int m_viewFilterMode;")
    alpha_patch_class(GeometryDash MusicSearchResult
        "applyArtistFilters(cocos2d::CCArray* objects) = win inline"
        "applyArtistFilters(cocos2d::CCArray* objects) = win 0x4981c0")
    alpha_patch_class(GeometryDash MusicSearchResult
        "updateFutureCount(cocos2d::CCArray*, cocos2d::CCArray*, cocos2d::CCArray*) = win inline"
        "updateFutureCount(cocos2d::CCArray*, cocos2d::CCArray*, cocos2d::CCArray*) = win 0x498460")
    alpha_patch_class(GeometryDash OptionsScrollLayer
        "    TodoReturn addTagMatchToggle();"
        "    void addTagMatchToggle() = win inline;")
    # MusicBrowser::onTags inlines this control with the standard button theme.
    file(APPEND "${alpha_bindings_output}/inline/OptionsScrollLayer.cpp" [=[
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/MusicSearchResult.hpp>
#ifdef GEODE_IS_WINDOWS
void OptionsScrollLayer::addTagMatchToggle() {
    if (!m_searchResult || m_matchButton) return;
    auto sprite = ButtonSprite::create(m_searchResult->m_matchAnyTags ? "Match: Any" : "Match: All",
        78, 0, 0.4f, true, "bigFont.fnt", "GJ_button_04.png", 22.f);
    if (!sprite) return;
    m_matchButton = CCMenuItemSpriteExtra::create(sprite, this, menu_selector(OptionsScrollLayer::onMatchToggle));
    m_buttonMenu->addChild(m_matchButton);
    auto size = cocos2d::CCDirector::sharedDirector()->getWinSize();
    m_matchButton->setPosition(m_buttonMenu->convertToNodeSpace({size.width / 2 + 131.f, size.height / 2 + 121.f}));
}
#endif
]=])
    set(alpha_search_inline "${alpha_bindings_output}/inline/MusicSearchResult.cpp")
    file(READ "${alpha_search_inline}" alpha_search_content)
    set(alpha_search_artist [=[cocos2d::CCArray* MusicSearchResult::applyArtistFilters(cocos2d::CCArray* objects) {
    if (objects->count() == 0) return objects;
    auto filterObjects = cocos2d::CCArray::create();
    m_artistFilter = false;
    for (auto object : geode::cocos::CCArrayExt<OptionsObject, false>(m_artistFilterObjects)) {
        if (object->m_enabled) {
            m_artistFilter = true;
            auto filtered = MusicDownloadManager::sharedState()->filterMusicByArtistID(object->m_optionID, objects);
            for (auto fobj : geode::cocos::CCArrayExt<OptionsObject, false>(filtered)) {
                filterObjects->addObject(fobj);
            }
        }
    }
    return m_artistFilter ? filterObjects : objects;
}
]=])
    set(alpha_search_counts [=[// TODO: 2.209
void MusicSearchResult::updateFutureCount(cocos2d::CCArray* objects, cocos2d::CCArray* allObjects, cocos2d::CCArray* p0) {
    auto count = objects->count();
    for (auto object : geode::cocos::CCArrayExt<OptionsObject, false>(m_tagFilterObjects)) {
        object->m_count = object->m_enabled ? MusicDownloadManager::sharedState()->filterMusicByTag(object->m_optionID, objects)->count() : count;
    }
    auto filtered = this->applyTagFilters(allObjects);
    for (auto object : geode::cocos::CCArrayExt<OptionsObject, false>(m_artistFilterObjects)) {
        object->m_count = MusicDownloadManager::sharedState()->filterMusicByArtistID(object->m_optionID, filtered)->count();
    }
}
]=])
    foreach(body IN ITEMS "${alpha_search_artist}" "${alpha_search_counts}")
        string(FIND "${alpha_search_content}" "${body}" alpha_search_match)
        if (alpha_search_match LESS 0)
            message(FATAL_ERROR "Unexpected MusicSearchResult inline implementation")
        endif()
        string(REPLACE "${body}" "" alpha_search_content "${alpha_search_content}")
    endforeach()
    file(CONFIGURE OUTPUT "${alpha_search_inline}" CONTENT "${alpha_search_content}" @ONLY)
    foreach(method IN ITEMS "filterMusicByArtistID" "filterMusicByTag")
        alpha_patch_class(GeometryDash MusicDownloadManager
            "${method}(int id, cocos2d::CCArray* songs) = win 0x9999999"
            "${method}(int id, cocos2d::CCArray* songs) = win inline")
    endforeach()
    # These single-ID helpers are inlined in the current native filters.
    # Preserve the old API's fresh array, order, duplicates and null handling.
    file(APPEND "${alpha_bindings_output}/inline/MusicDownloadManager.cpp" [=[
#ifdef GEODE_IS_WINDOWS
cocos2d::CCArray* MusicDownloadManager::filterMusicByArtistID(int id, cocos2d::CCArray* songs) {
    auto result = cocos2d::CCArray::create();
    if (songs) {
        for (unsigned int i = 0; i < songs->count(); ++i) {
            auto song = static_cast<SongInfoObject*>(songs->objectAtIndex(i));
            if (song->m_artistID == id || song->m_artistIDs.find(id) != song->m_artistIDs.end()) {
                result->addObject(song);
            }
        }
    }
    return result;
}

cocos2d::CCArray* MusicDownloadManager::filterMusicByTag(int id, cocos2d::CCArray* songs) {
    auto result = cocos2d::CCArray::create();
    if (songs) {
        for (unsigned int i = 0; i < songs->count(); ++i) {
            auto song = static_cast<SongInfoObject*>(songs->objectAtIndex(i));
            if (song->m_tags.find(id) != song->m_tags.end()) result->addObject(song);
        }
    }
    return result;
}
#endif
]=])
endif()
alpha_patch_address(GeometryDash MusicSearchResult
    "cocos2d::CCArray* applyTagFilters(cocos2d::CCArray* objects)" 0x497d00)

# Native list-row actions call the reorder function directly; coder loading
# calls the parser at 0x2728a0, not the unrelated old shader address.
alpha_patch_address(GeometryDash GJLevelList
    "void reorderLevel(int levelID, int newPosition)" 0x272d00)
alpha_patch_address(GeometryDash GJLevelList
    "void parseListLevels(std::string str)" 0x2728a0 0x5225c0)
# ReportQueuePopup's primary slot 0x420 reloads its current page through the
# common paged dispatch path, which invokes requestPage and clearPageContent.
alpha_patch_address(GeometryDash ReportQueuePopup "virtual void rebuildContent()" 0xc95b0)
if (WIN32)
    alpha_patch_class(GeometryDash PagedListPopup
        "    TodoReturn loadPage(int);" "    void loadPage(int page) = win 0xc3610;")
    alpha_patch_class(GeometryDash PagedListPopup
        "    TodoReturn showLoading();" "    void showLoading() = win 0xc3770;")
    alpha_patch_class(GeometryDash PagedListPopup
        "virtual TodoReturn clearPageContent()" "virtual void clearPageContent()")
    foreach(class IN ITEMS AccountReviewPopup AutoBackupPopup)
        alpha_patch_class(GeometryDash "${class}"
            "virtual TodoReturn clearPageContent()" "virtual void clearPageContent()")
    endforeach()

    # Upstream removed the obsolete Newgrounds alert and MusicBrowser text
    # callbacks. Their native primary tables end at 0x430.

    # Concrete native pickers return a CCArray pointer and its unsigned count.
    # Require SDK users to supply these interface callbacks, rather than
    # advertising a native implementation at an unresolved address.
    alpha_patch_class(GeometryDash FriendPickerDelegate
        "    virtual TodoReturn selectedRecipientCount() = win 0x9999999;"
        "    inline virtual unsigned int selectedRecipientCount() = 0;")
    alpha_patch_class(GeometryDash FriendPickerDelegate
        "    virtual TodoReturn selectedRecipients() = win 0x9999999;"
        "    inline virtual cocos2d::CCArray* selectedRecipients() = 0;")
    foreach(class IN ITEMS MemberListPopup MessageThreadsPage NewThreadPopup)
        alpha_patch_class(GeometryDash "${class}"
            "virtual TodoReturn selectedRecipientCount()" "virtual unsigned int selectedRecipientCount()")
        alpha_patch_class(GeometryDash "${class}"
            "virtual TodoReturn selectedRecipients()" "virtual cocos2d::CCArray* selectedRecipients()")
    endforeach()
    foreach(class IN ITEMS FriendPickerDelegate MemberListPopup)
        alpha_patch_class(GeometryDash "${class}"
            "virtual TodoReturn pickerRowState(int)" "virtual int pickerRowState(int)")
        alpha_patch_class(GeometryDash "${class}"
            "virtual TodoReturn pickerCountBase()" "virtual int pickerCountBase()")
    endforeach()
    alpha_patch_class(GeometryDash IDNameBrowserDelegate
        "    virtual void idNameBrowserClosed(IDNameBrowserPopup*, int) = win 0x9999999;"
        "    inline virtual void idNameBrowserClosed(IDNameBrowserPopup*, int) = 0;")
endif()

# GJRequestCell's native init is 0x158d20 (primary vtable 0x73cef0).
# Its allocation is 0x248: score at 0x230, toggler at 0x238 and the
# request-status-four flag at 0x240. The old popup callbacks have no native
# vtable slots in this version. Do not expose unrelated function addresses.
if (WIN32)
    foreach(obsolete IN ITEMS
        "    void markAsRead() = win 0x504490, imac 0x240820, m1 0x1eb810;"
        "    void onDeleteRequest(cocos2d::CCObject* sender);"
        "    void onViewFriendRequest(cocos2d::CCObject* sender) = win 0x1550d0;")
        alpha_patch_class(GeometryDash GJRequestCell "${obsolete}\n" "")
    endforeach()
    alpha_patch_class(GeometryDash GJRequestCell
        "    bool m_groupRequest;" "    bool m_requestStatusIsFour;")
    alpha_patch_class(GeometryDash GJRequestCell
        "void onToggle(cocos2d::CCObject* sender);"
        "void onToggle(cocos2d::CCObject* sender) = win 0x1592a0;")
    alpha_patch_class(GeometryDash GJRequestCell
        "void onViewProfile(cocos2d::CCObject* sender) = imac"
        "void onViewProfile(cocos2d::CCObject* sender) = win 0x157b80, imac")
    # Upstream also removed the obsolete popup ownership from the destructor.
endif()
