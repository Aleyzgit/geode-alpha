#include <Geode/binding/LevelSettingsObject.hpp>

#ifdef GEODE_IS_WINDOWS
LevelSettingsObject* LevelSettingsObject::create() {
    auto settings = new LevelSettingsObject();
    if (settings->init()) {
        settings->autorelease();
        return settings;
    }
    delete settings;
    return nullptr;
}
#endif
