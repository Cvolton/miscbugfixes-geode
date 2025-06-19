#include <Geode/modify/PlayerObject.hpp>
#include "_Utils.hpp"

using namespace geode::prelude;

STATIC_BOOL_SETTING(fixMiniSwing, fix-mini-swing);

class $modify(MyPlayerObject, PlayerObject) {
	void togglePlayerScale(bool p0, bool p1) {
		PlayerObject::togglePlayerScale(p0, p1);
		if (!m_isSwing || !m_defaultMiniIcon || !m_gameLayer || !s_fixMiniSwing || (this != m_gameLayer->m_player1 && this != m_gameLayer->m_player2)) return;
		PlayerObject::updatePlayerSwingFrame(GameManager::get()->getPlayerSwing());
	}
};