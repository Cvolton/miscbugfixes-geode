#include <Geode/Geode.hpp>
#include <Geode/modify/LevelBrowserLayer.hpp>

using namespace geode::prelude;

static int g_cnt = 0;

class $modify(LevelBrowserLayer) {
    // Fixes user's list search showing incorrect usernames
    // Also fixes the missing apostrophe in "x's Levels"
    gd::string getSearchTitle() {
        if(m_searchObject->m_searchType == SearchType::UsersLevels) {
            auto GLM = GameLevelManager::sharedState();

            if(auto res = utils::numFromString<int>(m_searchObject->m_searchQuery)) {

                if(m_searchObject->m_searchMode == 1) {
                    if(res.unwrap() == GJAccountManager::sharedState()->m_accountID) return LevelBrowserLayer::getSearchTitle();

                    if(auto userID = static_cast<CCString*>(GLM->m_accountIDtoUserIDDict->objectForKey(res.unwrap()))) {
                        return fmt::format("{}'s Lists", GLM->userNameForUserID(userID->intValue()));
                    }
                } else {
                    if(res.unwrap() == GameManager::sharedState()->m_playerUserID) return LevelBrowserLayer::getSearchTitle();

                    return fmt::format("{}'s Levels", GLM->userNameForUserID(res.unwrap()));
                }

            }
        }

        return LevelBrowserLayer::getSearchTitle();
    }
};