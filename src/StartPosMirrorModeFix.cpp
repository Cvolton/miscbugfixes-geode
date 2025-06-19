#include <Geode/modify/PlayLayer.hpp>
#include "_Utils.hpp"

using namespace geode::prelude;

STATIC_BOOL_SETTING(enforceStartPosMirrorMode, enforce-start-pos-mirror-mode);

class $modify(PlayLayer) {
	void resetLevel() {
		PlayLayer::resetLevel();
		if (!s_enforceStartPosMirrorMode || !m_isTestMode || !m_startPosObject || !m_startPosObject->m_startSettings) return;
		PlayLayer::toggleFlipped(m_startPosObject->m_startSettings->m_mirrorMode, true);
	}
};