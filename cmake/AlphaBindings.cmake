# Keep native address corrections local to this fork and leave the fetched
# bindings repository untouched. Explicit custom binding paths take precedence.
if (NOT GEODE_GD_VERSION STREQUAL "2.2091")
    return()
endif()

if (NOT GEODE_BINDINGS_PATH)
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

    file(CONFIGURE OUTPUT "${alpha_bindings_output}/GeometryDash.bro"
        CONTENT "${alpha_bindings}" @ONLY)
    include("${CMAKE_CURRENT_LIST_DIR}/AlphaWindowsAddresses.cmake")
    # Keep the fetched unversioned header intact. Windows request IDs changed
    # when native APIs were inserted into the shared HTTP dispatch enum.
    file(READ "${GEODE_BINDINGS_REPO_PATH}/bindings/include/Geode/Enums.hpp" alpha_enums)
    # Generated state headers use arrays, maps and cocos value types without
    # direct includes. Keep them valid independently of unity source ordering.
    set(alpha_enums "#include <array>\n#include <map>\n#include <Geode/cocos/include/ccTypes.h>\n${alpha_enums}")
    set(alpha_song_types "enum class GJSongType {\n    Music = -1,\n    NCS = 1,\n};")
    string(FIND "${alpha_enums}" "${alpha_song_types}" alpha_song_types_match)
    if (alpha_song_types_match LESS 0)
        message(FATAL_ERROR "Unexpected upstream GJSongType enum")
    endif()
    string(REPLACE "${alpha_song_types}"
        "enum class GJSongType {\n    Music = -1,\n    NCS = 1,\n#ifdef GEODE_IS_WINDOWS\n    Chompo = 2,\n#endif\n};"
        alpha_enums "${alpha_enums}")
    string(REGEX MATCH "enum class UserListType \\{[^}]*\\};" alpha_old_user_lists "${alpha_enums}")
    if (NOT alpha_old_user_lists)
        message(FATAL_ERROR "Missing upstream UserListType enum")
    endif()
    string(REPLACE "${alpha_old_user_lists}"
        "enum class UserListType {\n    Friends = 0,\n    Blocked = 1,\n#ifdef GEODE_IS_WINDOWS\n    Following = 3,\n    Muted = 4,\n#endif\n};"
        alpha_enums "${alpha_enums}")
    file(READ "${CMAKE_CURRENT_LIST_DIR}/AlphaWindowsHttpTypes.hpp" alpha_http_types)
    string(REGEX MATCH "enum class GJHttpType \\{[^}]*\\};" alpha_old_http_types "${alpha_enums}")
    if (NOT alpha_old_http_types)
        message(FATAL_ERROR "Missing upstream GJHttpType enum")
    endif()
    string(REPLACE "${alpha_old_http_types}"
        "#ifdef GEODE_IS_WINDOWS\n${alpha_http_types}\n#else\n${alpha_old_http_types}\n#endif"
        alpha_enums "${alpha_enums}")
    set(GEODE_ALPHA_ENUMS_INCLUDE "${CMAKE_CURRENT_BINARY_DIR}/alpha-include")
    file(CONFIGURE OUTPUT "${GEODE_ALPHA_ENUMS_INCLUDE}/Geode/Enums.hpp"
        CONTENT "${alpha_enums}" @ONLY)
    set(GEODE_BINDINGS_PATH "${alpha_bindings_output}")
endif()

# Audit custom bindings as well, including engine declarations. Overriding the
# source directory must not bypass the completeness check.
if (NOT EXISTS "${GEODE_BINDINGS_PATH}/GeometryDash.bro")
    message(FATAL_ERROR "Missing GeometryDash.bro in ${GEODE_BINDINGS_PATH}")
endif()
file(GLOB_RECURSE alpha_binding_files "${GEODE_BINDINGS_PATH}/*.bro")
include("${CMAKE_CURRENT_LIST_DIR}/AlphaBindingAudit.cmake")
set(alpha_missing_count 0)
foreach(alpha_binding_file IN LISTS alpha_binding_files)
    file(READ "${alpha_binding_file}" alpha_binding_content)
    alpha_count_windows_placeholders("${alpha_binding_content}" alpha_missing_in_file)
    math(EXPR alpha_missing_count
        "${alpha_missing_count} + ${alpha_missing_in_file}")
endforeach()
option(GEODE_ALPHA_REQUIRE_COMPLETE_BINDINGS
    "Reject builds with unresolved Windows native addresses" OFF)
# The Windows installer is a release artifact. Keep its completeness decision
# tied to the same audited binding copy used by the loader build.
if (NOT CMAKE_SCRIPT_MODE_FILE)
    file(CONFIGURE OUTPUT "${CMAKE_CURRENT_LIST_DIR}/../bin/nightly/alpha-release-state.nsh"
        CONTENT "!define ALPHA_UNRESOLVED_WINDOWS_BINDINGS @alpha_missing_count@\n" @ONLY)
endif()
if (alpha_missing_count GREATER 0)
    if (GEODE_ALPHA_REQUIRE_COMPLETE_BINDINGS)
        message(FATAL_ERROR "${alpha_missing_count} Windows bindings are unresolved")
    endif()
    message(WARNING
        "Geode Alpha: ${alpha_missing_count} Windows bindings remain unresolved")
endif()
