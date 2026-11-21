#include <Geode/Geode.hpp>

#if GEODE_COMP_GD_VERSION > 22000

#include <Geode/modify/CCDrawNode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/GameObject.hpp>
#include "_Utils.hpp"

using namespace geode::prelude;

static bool s_insideDebugUpdate = false;

// init
STATIC_BOOL_SETTING(shouldApplyPhysicsFix, practice-mode-show-hitboxes);

// slopes physics change fix
class $modify(GJBaseGameLayer) {
    void updateDebugDraw() {
        s_insideDebugUpdate = (s_shouldApplyPhysicsFix && static_cast<GJBaseGameLayer*>(this) == PlayLayer::get());
        GJBaseGameLayer::updateDebugDraw();
        s_insideDebugUpdate = false;
    }
};

class $modify(GameObject) {
    void determineSlopeDirection() {
        if (s_insideDebugUpdate) return;
        GameObject::determineSlopeDirection();
    }
};

#endif