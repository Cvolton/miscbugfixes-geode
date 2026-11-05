#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify(PlayerObject) {
	void stopDashing() {
		PlayerObject::stopDashing();
		if (this->m_isShip || this->m_isBird) {
			this->activateStreak();
		}
	}
};

class $modify(PlayLayer) {
	void resetLevel() {
		PlayLayer::resetLevel();
		if (this->m_isPracticeMode) {
			if (this->m_player1 && this->m_player1->m_isDart) {
				this->m_player1->m_waveTrail->addPoint(this->m_player1->getPosition());
			}
			if (this->m_player2 && this->m_player2->m_isDart) {
				this->m_player2->m_waveTrail->addPoint(this->m_player2->getPosition());
			}
		}
	}
};