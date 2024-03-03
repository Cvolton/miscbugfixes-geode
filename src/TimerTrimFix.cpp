#include <Geode/Geode.hpp>

using namespace geode::prelude;

void nopWindows(size_t address, const char* funcName) {
    if(auto patch = Mod::get()->patch(reinterpret_cast<void*>(base::get() + address), {0x90, 0x90, 0x90, 0x90, 0x90})) {
        log::info("Timer fix: Patched {}", funcName);
    } else {
        log::error("Timer fix: Failed to patch {}", funcName);
    }
}

$execute {
    // This just nops out the AND that cuts the timestamp to the bottom 20 bits
    // so that the timer doesn't reset after 1048576 seconds

    /**
     * If you look into it, the reason why Rob initially added this makes sense:
     *
     * The timer was presumably stored in a 32-bit float, which would cause
     * a precision of 256 seconds. With the AND, the timer would have a precision
     * of 1/16th of a second, which is more than enough.
     *
     * However, the timer is now stored in a 64-bit double, which has a much higher
     * precision than the 32-bit float, so the AND is no longer necessary.
     *
     * This is proven by Android behavior, where this trim does not happen.
    */
    #ifdef GEODE_IS_WINDOWS
        static_assert(GEODE_COMP_GD_VERSION == 22040, "Wrong GD version detected");
        nopWindows(0x1f49a, "BoomScrollLayer::vfunction2");
        nopWindows(0x1f7e8, "BoomScrollLayer::vfunction4");
        nopWindows(0x5b7e3, "AppDelegate::trySaveGame (patch 1)");
        nopWindows(0x5b8d1, "AppDelegate::trySaveGame (patch 2)");
        nopWindows(0x5cc29, "ChallengesPage::init");
        nopWindows(0x5d4bc, "ChallengesPage::updateTimers");
        nopWindows(0x98083, "DailyLevelPage::init");
        nopWindows(0x98d16, "DailyLevelPage::getDailyTime");
        nopWindows(0x9b146, "FUN_0049b120");
        nopWindows(0x9b4f7, "DialogLayer::animateIn");
        nopWindows(0xa3503, "FUN_004a34e0");
        nopWindows(0xa42af, "EditorUI::init");
        nopWindows(0xda353, "EditorUI::keyDown");
        nopWindows(0xdb2b5, "EditorUI::scrollWheel");
        nopWindows(0xf8e03, "GameLevelManager::makeTimeStamp");
        nopWindows(0xf8f60, "GameLevelManager::isTimeValid");
        nopWindows(0xf90ac, "GameLevelManager::getTimeLeft");
        nopWindows(0x110bfc, "GameLevelManager::storeDailyLevelState (patch 1)");
        nopWindows(0x110c66, "GameLevelManager::storeDailyLevelState (patch 2)");
        nopWindows(0x110cd0, "GameLevelManager::storeDailyLevelState (patch 3)");
        nopWindows(0x126db5, "GameManager::likeFacebook");
        nopWindows(0x126e45, "GameManager::followTwitter");
        nopWindows(0x126ed5, "GameManager::subYouTube");
        nopWindows(0x126f65, "GameManager::followTwitch");
        nopWindows(0x126ff5, "GameManager::joinDiscord");
        nopWindows(0x127085, "GameManager::joinReddit");
        nopWindows(0x12defc, "GameManager::applicationWillEnterForeground");
        nopWindows(0x172942, "GameStatsManager::storeRewardState");
        nopWindows(0x172ec6, "GameStatsManager::storeChallengeTime");
        nopWindows(0x1fb540, "GJMultiplayerManager::onJoinLobbyCompleted");
        nopWindows(0x2c63a1, "PlayerObject::updateCheckpointTest");
        nopWindows(0x2dc696, "PlayLayer::init");
        nopWindows(0x2ddf88, "PlayLayer::levelComplete");
        nopWindows(0x2e767b, "PlayLayer::updateAttemptTime");
        nopWindows(0x2eaf02, "PlayLayer::pauseGame");
        nopWindows(0x2eb25b, "PlayLayer::resume");
        nopWindows(0x2f773a, "FUN_006f76d0");
    #endif
    //TODO: mac
}