#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

class $modify(PlayerObject) {
    // if you start dashing as cube or ball, your cube scale is set to 0.9
    // if you stop dashing as cube or ball, your cube scale is reset to 1.0
    // all gamemode switches to cube except from robot and ball call resetPlayerIcon
    // which also resets the scale
    // so with these conditions, if you start dashing as cube or ball and stop dashing as robot,
    // your cube scale will end up still at 0.9
    // i assume robtop just forgot to call resetPlayerIcon in these two,
    // so lets hope calling it here doesnt break anything (:
    // update: it breaks something, no more resetPlayerIcon call :D

    $override void toggleRobotMode(bool on, bool start) {
        if (on || !m_isRobot)
            return PlayerObject::toggleRobotMode(on, start);
        PlayerObject::toggleRobotMode(on, start);
        m_iconSprite->setScale(1.0f);
        m_iconGlow->setScale(1.0f);
    }

    $override void toggleRollMode(bool on, bool start) {
        if (on || !m_isBall)
            return PlayerObject::toggleRollMode(on, start);
        PlayerObject::toggleRollMode(on, start);
        m_iconSprite->setScale(1.0f);
        m_iconGlow->setScale(1.0f);
    }
};
