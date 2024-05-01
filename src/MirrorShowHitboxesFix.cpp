#include <Geode/Geode.hpp>
#include <Geode/modify/CCDrawNode.hpp>

using namespace geode::prelude;

class $modify(cocos2d::CCDrawNode) {
    bool drawPolygon(CCPoint *verts, unsigned int count, const ccColor4F &fillColor, float borderWidth, const ccColor4F &borderColor) {
        return CCDrawNode::drawPolygon(verts, count, fillColor, std::abs(borderWidth), borderColor);
    }
};