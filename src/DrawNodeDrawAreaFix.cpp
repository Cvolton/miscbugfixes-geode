#include <Geode/Geode.hpp>
#include <Geode/modify/CCDrawNode.hpp>

using namespace geode::prelude;

class $modify(CCDrawNode) {
    // TODO: move to ctor when modify with it is fixed in geode
    $override bool init() {
        // and here we are with more uninitialized variables
        // this time robtop forgot to initialize m_bUseArea,
        // causing drawnodes to outright not work sometimes
        m_bUseArea = false;
        return CCDrawNode::init();
    }
};
