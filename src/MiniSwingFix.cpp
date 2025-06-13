#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

class $modify(MyPlayerObject, PlayerObject) {
	void togglePlayerScale(bool p0, bool p1) {
		PlayerObject::togglePlayerScale(p0, p1);
		GJBaseGameLayer* gjbgl = GJBaseGameLayer::get();
		if (!gjbgl || (this != gjbgl->m_player1 && this != gjbgl->m_player2) || !this->m_isSwing) return;
		PlayerObject::updatePlayerSwingFrame(GameManager::get()->getPlayerSwing());
	}
};