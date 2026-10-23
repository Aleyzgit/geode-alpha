#include <Geode/binding/AppDelegate.hpp>
#include <Geode/binding/FMODAudioEngine.hpp>
#include <Geode/binding/GameManager.hpp>
#include <Geode/binding/PlayLayer.hpp>
#include <Geode/binding/PlayerObject.hpp>
#include <Geode/mapping/CCDirector.hpp>

#ifdef GEODE_IS_WINDOWS
bool AppDelegate::musicTest() {
    auto manager = GameManager::get();
    if (manager->m_loadingLevel) return true;
    auto scene = cocos2d::CCDirector::get()->getRunningScene();
    if (!scene) return false;
    auto play = manager->m_playLayer;
    if (play && (m_gamePaused || play->m_hasCompletedLevel || !play->m_started ||
        !play->m_player1 || play->m_player1->m_isDead)) return true;
    return scene->getTag() == 6;
}

void AppDelegate::resumeSound() {
    if (this->musicTest()) return;
    auto manager = GameManager::get();
    auto audio = FMODAudioEngine::sharedEngine();
    if (manager->m_playLayer || !manager->getGameVariable("0122")) audio->resumeAllMusic();
    if (audio->m_globalChannel) audio->m_globalChannel->setPaused(false);
}
#endif
