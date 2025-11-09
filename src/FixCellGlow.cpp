#include <Geode/Geode.hpp>
#include <Geode/modify/GJScoreCell.hpp>
#include <Geode/modify/CommentCell.hpp>

using namespace geode::prelude;

class $modify(GJScoreCell) {
    void loadFromScore(GJUserScore* score) {
		GJScoreCell::loadFromScore(score);

		auto simplePlayer = m_mainLayer->getChildByType<SimplePlayer>(0);
		if (!simplePlayer)
			return;
        
        if (simplePlayer->m_hasGlowOutline) // GJUserScore::m_glowEnabled is always false, so this is the next best thing
		    simplePlayer->setGlowOutline(GameManager::get()->colorForIdx(score->m_color3));
	}
};

class $modify(CommentCell) {
    void loadFromComment(GJComment* comment) {
		CommentCell::loadFromComment(comment);
        
		auto simplePlayer = static_cast<SimplePlayer*>(this->getChildByIDRecursive("player-icon"));
		if (!simplePlayer)
			return;
		
        if (simplePlayer->m_hasGlowOutline)
            simplePlayer->setGlowOutline(GameManager::get()->colorForIdx(comment->m_userScore->m_color3));
	}
};