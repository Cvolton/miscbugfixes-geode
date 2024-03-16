#include <Geode/Geode.hpp>
#include <Geode/modify/CCMenuItem.hpp>
#include <Geode/modify/CCMenu.hpp>

using namespace geode::prelude;

class $modify(cocos2d::CCMenuItem) {
    void activate() {
        this->retain();
        cocos2d::CCMenuItem::activate();
        this->release();
    }
};

class $modify(cocos2d::CCMenu) {
    void ccTouchEnded(CCTouch* touch, CCEvent* event) {
        this->retain();
        cocos2d::CCMenu::ccTouchEnded(touch, event);
        this->release();
    }
};