#ifndef __CC_KEYBOARD_ACTION_MANAGER_H__
#define __CC_KEYBOARD_ACTION_MANAGER_H__

#include "../../cocoa/CCObject.h"
#ifdef GEODE_IS_WINDOWS
#include "CCKeyboardDelegate.h"
#include <vector>
#endif

NS_CC_BEGIN

class CC_DLL CCKeyboardActionManager : public CCObject
{
    GEODE_FRIEND_MODIFY

public:
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCKeyboardActionManager, CCObject)
#ifdef GEODE_IS_WINDOWS
    // Native Windows vtable: key lookup at 0x48, global actions at 0x50.
    virtual std::vector<int> actionsForKey(enumKeyCodes, bool, bool, bool) = 0;
#endif
    virtual bool handleGlobalAction(int, bool);
};

NS_CC_END

#endif  // __CC_KEYBOARD_ACTION_MANAGER_H__
