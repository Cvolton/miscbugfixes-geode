#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include "_Utils.hpp"

using namespace geode::prelude;

STATIC_BOOL_SETTING(showTriggerWaveParticlesFix, show-trigger-wave-particles-fix);

class $modify(PlayerObject) {
    $override void toggleVisibility(bool show) {
        bool willShowWave = show && m_isDart && m_isHidden && s_showTriggerWaveParticlesFix;
        PlayerObject::toggleVisibility(show);
        // the check in toggleVisibility gets into an "always has trail" branch
        // and it has m_trailingParticles->resetSystem() because all the game modes in it
        // until the wave was added (ship and ufo) also have trailing particles,
        // however it's not the case for wave and rob has a branch
        // specific to the wave inside the "always has trail" branch
        // but he forgot to check for wave for m_trailingParticles
        if (willShowWave)
            m_trailingParticles->stopSystem();
    }
};
