# Several upstream inline bodies need complete types which unity source
# ordering does not guarantee. Add their direct includes to the build copy.
function(alpha_include_inline class)
    set(path "${alpha_bindings_output}/inline/${class}.cpp")
    if (NOT EXISTS "${path}")
        message(FATAL_ERROR "Missing inline source ${class}")
    endif()
    file(READ "${path}" source)
    set(includes "")
    foreach(header IN LISTS ARGN)
        string(APPEND includes "#include <${header}>\n")
    endforeach()
    file(WRITE "${path}" "${includes}${source}")
endfunction()

if (WIN32)
    alpha_include_inline(GJLevelList Geode/binding/GJGameLevel.hpp)
    alpha_include_inline(LevelSelectLayer
        Geode/binding/GameManager.hpp Geode/binding/LevelPage.hpp algorithm)
    alpha_include_inline(CreatorLayer
        Geode/cocos/CCDirector.h Geode/cocos/layers_scenes_transitions_nodes/CCTransition.h)
endif()

if (NOT CMAKE_SYSTEM_NAME STREQUAL "iOS")
    return()
endif()

# The imported cocos bodies use the framework's point macros, graphics matrix
# API and concrete engine types. Its umbrella header supplies those contracts.
file(GLOB alpha_cocos_inline "${alpha_bindings_output}/inline/CC*.cpp")
foreach(path IN LISTS alpha_cocos_inline)
    get_filename_component(class "${path}" NAME_WE)
    alpha_include_inline("${class}" Geode/cocos/include/cocos2d.h)
endforeach()

alpha_include_inline(SongSelectNode Geode/binding/CustomSongLayer.hpp)
alpha_include_inline(MoreVideoOptionsLayer Geode/binding/CCTextInputNode.hpp)
alpha_include_inline(OptionsLayer Geode/binding/VideoOptionsLayer.hpp)
alpha_include_inline(ParticlePreviewLayer Geode/cocos/platform/CCEGLView.h)
alpha_include_inline(PlayLayer Geode/binding/GameStatsManager.hpp)
alpha_include_inline(PlayerObject Geode/binding/GJActionManager.hpp)
alpha_include_inline(EditorUI
    Geode/binding/ColorSelectLiveOverlay.hpp Geode/binding/HSVLiveOverlay.hpp)
