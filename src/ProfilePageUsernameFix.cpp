#include <Geode/Geode.hpp>
#include <Geode/modify/ProfilePage.hpp>

using namespace geode::prelude;

class $modify(ProfilePage) {
    void loadPageFromUserInfo(GJUserScore* score) {
        GameLevelManager::sharedState()->storeUserName(score->m_userID, score->m_accountID, score->m_userName);
        if(std::string_view(m_usernameLabel->getString()) != score->m_userName) {
            m_usernameLabel->setString(score->m_userName.c_str());
            m_usernameLabel->limitLabelWidth(score->m_modBadge > 0 ? 140.f : 160.0f, 0.8f, 0.0f);
        }
        
        ProfilePage::loadPageFromUserInfo(score);
    }
};