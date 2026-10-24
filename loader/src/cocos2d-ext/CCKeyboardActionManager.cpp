#include <Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardActionManager.h>

#ifdef GEODE_IS_WINDOWS
// The native constructor initializes CCObject and has no additional state.
cocos2d::CCKeyboardActionManager::CCKeyboardActionManager() = default;

bool cocos2d::CCKeyboardActionManager::handleGlobalAction(int, bool) {
    return false;
}
#endif
