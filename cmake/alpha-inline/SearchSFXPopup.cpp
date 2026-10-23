#include <Geode/binding/GameManager.hpp>
#include <Geode/binding/GameToolbox.hpp>
#include <Geode/binding/SearchSFXPopup.hpp>
#include <Geode/mapping/CCDirector.hpp>

#ifdef GEODE_IS_WINDOWS
bool SearchSFXPopup::init(std::string query) {
    if (!SetTextPopup::init(query, "Search", 60, "Find SFX", "OK", false, 60.f, false, 0, 0)) return false;
    auto size = cocos2d::CCDirector::sharedDirector()->getWinSize();
    GameToolbox::createToggleButton("Folder Name", menu_selector(SearchSFXPopup::onSearchFolders),
        GameManager::get()->getGameVariable("0147"), m_buttonMenu,
        {size.width * .5f - 124.f, size.height * .5f + 10.f}, this, m_mainLayer,
        .8f, .3f, 0.f, {8.f, 0.f}, "bigFont.fnt", false, 0, nullptr);
    return true;
}

void SearchSFXPopup::onSearchFolders(cocos2d::CCObject* sender) {
    GameManager::get()->toggleGameVariable("0147");
}
#endif
