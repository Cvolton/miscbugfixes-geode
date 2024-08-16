#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/loader/SettingEvent.hpp>

using namespace geode::prelude;

static bool s_shouldApplyAnticheatFix = false;

$on_mod(Loaded) {
    listenForSettingChanges("practice-mode-ignore-damage", +[](bool value) {
        s_shouldApplyAnticheatFix = value;
    });
}

$on_mod(DataLoaded) {
    s_shouldApplyAnticheatFix = Mod::get()->getSettingValue<bool>("practice-mode-ignore-damage");
}

class $modify(PlayLayer) {
    void togglePracticeMode(bool toggle) {
        if (toggle && s_shouldApplyAnticheatFix) {
            PlayLayer::destroyPlayer(m_player1, m_anticheatSpike);
        }
        PlayLayer::togglePracticeMode(toggle);
    }
};