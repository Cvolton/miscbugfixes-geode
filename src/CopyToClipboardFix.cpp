#include <Geode/Geode.hpp>

#ifdef GEODE_IS_ANDROID

#include <Geode/modify/InfoLayer.hpp>
#include <Geode/modify/ProfilePage.hpp>

using namespace geode::prelude;

class $modify(InfoLayer) {
    void onCopyLevelID(CCObject* sender) {
        static_assert(&InfoLayer::onCopyLevelID);

        clipboard::write(std::to_string(getID()));
    }
};

class $modify(ProfilePage) {
    void onCopyName(CCObject* sender) {
        static_assert(&ProfilePage::onCopyName);

        clipboard::write(this->m_score->m_userName);
    }
};

#endif