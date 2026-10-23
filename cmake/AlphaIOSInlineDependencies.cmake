# Several upstream iOS inline bodies need complete types which unity source
# ordering does not guarantee. Add their direct includes to the build copy.
if (NOT CMAKE_SYSTEM_NAME STREQUAL "iOS")
    return()
endif()

function(alpha_include_ios_inline class)
    set(path "${alpha_bindings_output}/inline/${class}.cpp")
    if (NOT EXISTS "${path}")
        message(FATAL_ERROR "Missing iOS inline source ${class}")
    endif()
    file(READ "${path}" source)
    set(includes "")
    foreach(header IN LISTS ARGN)
        string(APPEND includes "#include <${header}>\n")
    endforeach()
    file(WRITE "${path}" "${includes}${source}")
endfunction()

alpha_include_ios_inline(SongSelectNode Geode/binding/CustomSongLayer.hpp)
alpha_include_ios_inline(MoreVideoOptionsLayer Geode/binding/CCTextInputNode.hpp)
alpha_include_ios_inline(OptionsLayer Geode/binding/VideoOptionsLayer.hpp)
alpha_include_ios_inline(ParticlePreviewLayer Geode/cocos/platform/CCEGLView.h)
alpha_include_ios_inline(PlayLayer Geode/binding/GameStatsManager.hpp)
alpha_include_ios_inline(PlayerObject Geode/binding/GJActionManager.hpp)
alpha_include_ios_inline(EditorUI
    Geode/binding/ColorSelectLiveOverlay.hpp Geode/binding/HSVLiveOverlay.hpp)
foreach(class IN ITEMS CCSplitCols CCSplitRows)
    alpha_include_ios_inline("${class}"
        Geode/cocos/cocoa/CCZone.h Geode/cocos/CCDirector.h Geode/cocos/cocoa/CCGeometry.h)
endforeach()
alpha_include_ios_inline(CCSpriteFrameCache
    Geode/cocos/textures/CCTextureCache.h Geode/binding/CCContentManager.hpp)
alpha_include_ios_inline(CCStopGrid Geode/cocos/effects/CCGrid.h)
alpha_include_ios_inline(CCTextureAtlas Geode/cocos/textures/CCTextureCache.h)
alpha_include_ios_inline(CCTextureCache Geode/cocos/platform/CCFileUtils.h)
alpha_include_ios_inline(CCTransitionCrossFade Geode/cocos/misc_nodes/CCRenderTexture.h)
