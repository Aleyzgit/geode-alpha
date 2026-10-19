# Keep native address corrections local to this fork and leave the fetched
# bindings repository untouched. Explicit custom binding paths take precedence.
if (NOT GEODE_GD_VERSION STREQUAL "2.2091" OR GEODE_BINDINGS_PATH)
    return()
endif()

set(alpha_bindings_source "${GEODE_BINDINGS_REPO_PATH}/bindings/2.2091")
set(alpha_bindings_output "${CMAKE_CURRENT_BINARY_DIR}/alpha-bindings/2.2091")
file(COPY "${alpha_bindings_source}/" DESTINATION "${alpha_bindings_output}")
file(READ "${alpha_bindings_source}/GeometryDash.bro" alpha_bindings)

# Verified against Windows EXE SHA-256
# 4eee06d5c2e2213b4ba2b1f6233dd426bbdd0e9b29d7c375e499516b6f8f69d5.
# AppDelegate::trySaveGame calls save on GameManager and LocalLevelManager.
string(REPLACE
    "void save(bool) = win 0x9999999,"
    "void save(bool) = win 0x77530,"
    alpha_bindings "${alpha_bindings}")
# The singleton installs LocalLevelManager's RTTI vtable and CCLocalLevels.dat.
string(REPLACE
    "static LocalLevelManager* sharedState() = win 0x9999999;"
    "static LocalLevelManager* sharedState() = win 0x46ac00;"
    alpha_bindings "${alpha_bindings}")
# Uses the same gv_ dictionary as getGameVariable, then CCString::intValue.
string(REPLACE
    "int getIntGameVariable(char const* key) = win 0x9999999;"
    "int getIntGameVariable(char const* key) = win 0x27ec70;"
    alpha_bindings "${alpha_bindings}")

# The constructor initializes additional Windows cache/state storage. Keep
# m_valueKeeper at 0x1b0, m_customObjectDict at 0x1d0 and m_playLayer at 0x230.
string(REPLACE
    "    bool m_switchModes;"
    "    PAD = win 0x18;\n    bool m_switchModes;"
    alpha_bindings "${alpha_bindings}")
string(REPLACE
    "    cocos2d::CCDictionary* m_unlockValueKeeper;"
    "    cocos2d::CCDictionary* m_unlockValueKeeper;\n    PAD = win 0x10;"
    alpha_bindings "${alpha_bindings}")

file(CONFIGURE OUTPUT "${alpha_bindings_output}/GeometryDash.bro"
    CONTENT "${alpha_bindings}" @ONLY)
set(GEODE_BINDINGS_PATH "${alpha_bindings_output}")

string(REGEX MATCHALL "win 0x9999999" alpha_missing_bindings "${alpha_bindings}")
list(LENGTH alpha_missing_bindings alpha_missing_count)
option(GEODE_ALPHA_REQUIRE_COMPLETE_BINDINGS
    "Reject builds with unresolved Windows native addresses" OFF)
if (alpha_missing_count GREATER 0)
    if (GEODE_ALPHA_REQUIRE_COMPLETE_BINDINGS)
        message(FATAL_ERROR "${alpha_missing_count} Windows bindings are unresolved")
    endif()
    message(WARNING
        "Geode Alpha: ${alpha_missing_count} Windows bindings remain unresolved")
endif()
