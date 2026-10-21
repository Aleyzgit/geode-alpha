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
    // They occupy the verified 0x480-byte EditorWindowPopup base storage.
    static_assert(sizeof(EditorWindowPopup) == 0x480);
    auto window = reinterpret_cast<unsigned char*>(this);
    window[0x429] = 1;
    window[0x42a] = 1;
    *reinterpret_cast<int*>(window + 0x44c) = 2;
    *reinterpret_cast<float*>(window + 0x468) = 0.6f;
    *reinterpret_cast<float*>(window + 0x46c) = 1.f;
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
