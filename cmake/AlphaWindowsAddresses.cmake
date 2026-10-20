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
    "    PAD = win 0x1f8;\n    std::set<std::string> m_queuedLists;")
alpha_patch_class(GeometryDash GameLevelManager
    "    int m_dailyTimeLeft;" "    PAD = win 0x4;\n    int m_dailyTimeLeft;")
alpha_patch_class(GeometryDash GameLevelManager
    "    cocos2d::CCDictionary* m_gauntletLevels;"
    "    PAD = win 0x24;\n    cocos2d::CCDictionary* m_gauntletLevels;")
alpha_patch_class(GeometryDash GameLevelManager
    "    cocos2d::CCDictionary* m_storedLevels;"
    "    PAD = win 0x40;\n    cocos2d::CCDictionary* m_storedLevels;")
alpha_patch_class(GeometryDash GameLevelManager
    "    std::string m_searchKey;"
    "    PAD = win 0x18;\n    std::string m_searchKey;")
alpha_patch_class(GeometryDash GameLevelManager
    "    SearchType m_searchType;"
    "    PAD = win 0x48;\n    SearchType m_searchType;")

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
