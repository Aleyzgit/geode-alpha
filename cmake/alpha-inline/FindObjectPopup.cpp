#include <Geode/binding/FindObjectPopup.hpp>
#include <Geode/binding/GameManager.hpp>

#ifdef GEODE_IS_WINDOWS
FindObjectPopup::FindObjectPopup() : m_groupID(0) {}

FindObjectPopup* FindObjectPopup::create(EditorUI* ui) {
    auto popup = new FindObjectPopup();
    if (popup->init(ui)) {
        popup->autorelease();
        return popup;
    }
    delete popup;
    return nullptr;
}

bool FindObjectPopup::init(EditorUI* ui) {
    // EditorUI's native window factory inlines these settings before initWindow.
    // The current upstream names cover the verified base storage.
    static_assert(sizeof(EditorWindowPopup) == 0x480);
    static_assert(offsetof(EditorWindowPopup, m_allowDraggingAnywhere) == 0x429);
    static_assert(offsetof(EditorWindowPopup, m_startCentered) == 0x42a);
    static_assert(offsetof(EditorWindowPopup, m_resizeMode) == 0x44c);
    static_assert(offsetof(EditorWindowPopup, m_minScale) == 0x468);
    static_assert(offsetof(EditorWindowPopup, m_maxScale) == 0x46c);
    m_allowDraggingAnywhere = true;
    m_startCentered = true;
    m_resizeMode = 2;
    m_minScale = 0.6f;
    m_maxScale = 1.f;
    return initWindow(ui, 200.f, 89.f, 3, 0);
}

void FindObjectPopup::onFindObjectID(cocos2d::CCObject*) {
    // Native runFind reads this preference; it is independent of the group ID.
    GameManager::get()->toggleGameVariable("0232");
}

void FindObjectPopup::onSelect(cocos2d::CCObject*) {
    runFind(0);
}
#endif
