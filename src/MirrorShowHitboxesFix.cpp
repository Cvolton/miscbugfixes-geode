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

// mirrored hitboxes invisible fix
class $modify(cocos2d::CCDrawNode) {
    bool drawPolygon(CCPoint *verts, unsigned int count, const ccColor4F &fillColor, float borderWidth, const ccColor4F &borderColor) {
        if(s_insideDebugUpdate) {
            borderWidth = std::abs(borderWidth);
        }

        return CCDrawNode::drawPolygon(verts, count, fillColor, borderWidth, borderColor);
    }

    bool drawCircle(const cocos2d::CCPoint& position, float radius, const cocos2d::ccColor4F& color, float borderWidth, const cocos2d::ccColor4F& borderColor, unsigned int segments) {
        if(s_insideDebugUpdate) {
            borderWidth = std::abs(borderWidth);
            m_bUseArea = false;
        }

        return CCDrawNode::drawCircle(position, radius, color, borderWidth, borderColor, segments);
    }
};

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