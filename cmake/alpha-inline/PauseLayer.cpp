#include <Geode/binding/ChallengesPage.hpp>
#include <Geode/binding/PauseLayer.hpp>

#ifdef GEODE_IS_WINDOWS
PauseLayer::PauseLayer() :
    m_unfocused(false),
    m_tryingQuit(false),
    m_uiHidden(false),
    m_startPosLabel(nullptr),
    m_startPosIndex(0),
    m_startPosObjects(nullptr),
    m_startPosChanged(false),
    m_timeSpeedControl(nullptr),
    m_mainLayer(nullptr),
    m_musicSlider(nullptr),
    m_sfxSlider(nullptr),
    m_hideMenu(nullptr),
    m_questsSprite(nullptr) {}

void PauseLayer::checkQuestsStatus() {
    if (!m_questsSprite) return;
    m_questsSprite->removeChildByTag(99);
    if (ChallengesPage::canCollectQuests()) {
        auto badge = cocos2d::CCSprite::createWithSpriteFrameName("exMark_001.png");
        m_questsSprite->addChild(badge, 10, 99);
        badge->setScale(.3f);
        badge->setPosition({6.f, 22.f});
    }
}
#endif
