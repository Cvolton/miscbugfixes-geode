#include <Geode/Geode.hpp>
#include <Geode/modify/AudioEffectsLayer.hpp>

using namespace geode::prelude;

class $modify(AudioEffectsLayer) {

	TodoReturn audioStep(float p0) {
		bool practiceStateOriginal = PlayLayer::get()->m_isPracticeMode;
		PlayLayer::get()->m_isPracticeMode = PlayLayer::get()->m_isPracticeMode && !GameStatsManager::sharedState()->isItemEnabled(UnlockType::GJItem, 0x11);
		AudioEffectsLayer::audioStep(p0);
		PlayLayer::get()->m_isPracticeMode = practiceStateOriginal;
	}
};