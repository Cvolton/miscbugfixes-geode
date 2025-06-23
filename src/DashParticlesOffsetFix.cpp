#include <Geode/modify/PlayerObject.hpp>
#include "_Utils.hpp"

using namespace geode::prelude;

STATIC_BOOL_SETTING(dashParticlesOffsetFix, dash-particles-offset-fix);

class $modify(PlayerObject) {
    $override void setPosition(const CCPoint& position) {
        PlayerObject::setPosition(position);
        // 2.2 changed this to the position of m_trailingParticles
        // which are used for ship and ufo click particles
        // this fix reverts this to the 2.113 behavior
        if (s_dashParticlesOffsetFix)
            m_dashParticles->setPosition(position);
    }
};
