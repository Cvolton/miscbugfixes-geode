#include <Geode/Geode.hpp>
#include <Geode/modify/GameStatsManager.hpp>

using namespace geode::prelude;

class $modify(GameStatsManager) {
    void dataLoaded(DS_Dictionary* dict) {
        GameStatsManager::dataLoaded(dict);

        for(auto [key, quest] : CCDictionaryExt<gd::string, GJChallengeItem*>(this->m_activeChallenges)) {
            if(quest->m_count >= quest->m_goal) quest->m_canClaim = true;
        }
    }
};