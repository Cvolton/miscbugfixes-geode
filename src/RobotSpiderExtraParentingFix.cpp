#include <Geode/Geode.hpp>
#include <Geode/modify/GJRobotSprite.hpp>

using namespace geode::prelude;

class $modify(GJRobotSprite) {
    // TODO: move to ctor when modify with it is fixed in geode
    $override bool init(int p0, std::string p1) {
        // m_headSprite not being initialized sometimes causes a check in init to pass
        // causing the extra sprite to be attached to whatever was in m_headSprite
        // instead of the actual head sprite
        m_headSprite = nullptr;
        // m_unkArray is also uninitialized, i don't think it can cause issues
        // but initialize it anyway just in case
        m_unkArray = nullptr;
        return GJRobotSprite::init(p0, std::move(p1));
    }
};
