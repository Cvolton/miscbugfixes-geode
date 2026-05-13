#include <Geode/Geode.hpp>
#include <Geode/modify/AudioEffectsLayer.hpp>
#include "_Utils.hpp"

using namespace geode::prelude;

STATIC_BOOL_SETTING(practiceMusicSyncPulseFix, practice-music-sync-pulse-fix);

class $modify(AudioEffectsLayer) {
    $override void audioStep(float p0) {
        if (!s_practiceMusicSyncPulseFix)
            return AudioEffectsLayer::audioStep(p0);
        auto* pl = PlayLayer::get();
        if (!pl)
            return AudioEffectsLayer::audioStep(p0);
        bool saved = pl->m_isPracticeMode;
        pl->m_isPracticeMode = pl->m_isPracticeMode && !pl->m_practiceMusicSync;
        AudioEffectsLayer::audioStep(p0);
        pl->m_isPracticeMode = saved;
    }
};
