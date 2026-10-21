#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/binding/EnterEffectObject.hpp>
#include <algorithm>

#ifdef GEODE_IS_WINDOWS
using namespace geode::prelude;

class $modify(AlphaEnterEffects, GJBaseGameLayer) {
    void addCustomEnterEffect(EnterEffectObject* object, bool enter) {
        if (object->m_enterChannel >= 0 && object->m_enterChannel <= 100 && object->m_effectID > 0) {
            auto& channels = enter ? m_gameState.m_enterEffectInstanceVectors : m_gameState.m_exitEffectInstanceVectors;
            auto channel = channels.find(object->m_enterChannel);
            if (channel != channels.end()) {
                auto& effects = channel->second;
                // The native replacement loop starts two elements past begin,
                // which overruns a one-element vector. Erase replacements here
                // while retaining the native same-object value refresh path.
                std::erase_if(effects, [object](EnterEffectInstance const& effect) {
                    return effect.m_gameObject->m_effectID == object->m_effectID &&
                        (effect.m_gameObject->m_uniqueID != object->m_uniqueID ||
                            effect.m_targetID != object->m_targetGroupID ||
                            effect.m_centerID != object->m_centerGroupID);
                });
            }
        }
        GJBaseGameLayer::addCustomEnterEffect(object, enter);
    }
};
#endif
