#include <Geode/Geode.hpp>
#include <Geode/modify/LevelListLayer.hpp>
#include <Geode/modify/LevelCell.hpp>

using namespace geode::prelude;

class $modify(LevelListLayer) {
    struct Fields {
        bool m_isBeingFixed = false;
        std::string m_key;
    };

    void loadLevelsFailed(char const* key, int type) {
        LevelListLayer::loadLevelsFailed(key, type);
        
        auto sv = std::string_view(key);
        if(!sv.starts_with("25")) return;

        log::info("LevelListLayer::loadLevelsFailed: key = {}, type = {}", key, type);
        log::info("levelsString: {}", m_levelList->m_levelsString);

        m_searchObject->m_searchType = SearchType::Type26;
        m_searchObject->m_searchQuery = m_levelList->m_levelsString;

        m_fields->m_isBeingFixed = true;
        m_fields->m_key = key;

        GameLevelManager::sharedState()->getOnlineLevels(m_searchObject);
    }

    void loadLevelsFinished(cocos2d::CCArray* levels, char const* key, int type) {
        auto sv = std::string_view(key);
        if(sv.starts_with("26") && m_fields->m_isBeingFixed) {
            m_fields->m_isBeingFixed = false;
            m_searchObject->m_searchType = SearchType::LevelListsOnClick;
            m_searchObject->m_searchQuery = std::to_string(m_levelList->m_listID);
            key = m_fields->m_key.c_str();
        }

        LevelListLayer::loadLevelsFinished(levels, key, type);
    }
};

//also fix get it in levelcell
class $modify(LevelCell) {
    void onClick(cocos2d::CCObject* sender) {
        LevelCell::onClick(sender);
        
        if(m_level->m_levelType != GJLevelType::Default) return;

        auto browserLayer = LevelBrowserLayer::scene(GJSearchObject::create(SearchType::Search, std::to_string(m_level->m_levelID)));
        auto transitionFade = CCTransitionFade::create(0.5, browserLayer);
        CCDirector::sharedDirector()->pushScene(transitionFade);
    }
};