#include <Geode/Geode.hpp>
#include <Geode/modify/CCMenuItem.hpp>

using namespace geode::prelude;

class $modify(cocos2d::CCMenuItem) {
    void activate() {
        this->retain();
        cocos2d::CCMenuItem::activate();
        this->release();
    }
};