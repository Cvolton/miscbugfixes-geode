#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify(PlayLayer) {
	void resetLevel() {
		PlayLayer::resetLevel();
		if (!Mod::get()->getSettingValue<bool>("enforce-start-pos-mirror-mode") || !m_isTestMode || !m_startPosObject || !m_startPosObject->m_startSettings) return;
		PlayLayer::toggleFlipped(m_startPosObject->m_startSettings->m_mirrorMode, true);
	}
};