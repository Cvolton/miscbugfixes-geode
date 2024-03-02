#include <Geode/Geode.hpp>

#ifdef GEODE_IS_ANDROID

#include <Geode/modify/InfoLayer.hpp>

using namespace geode::prelude;

class $modify(InfoLayer) {
    void onCopyLevelID(CCObject* sender) {
        clipboard::write(std::to_string(getID()));
    }
};

#endif