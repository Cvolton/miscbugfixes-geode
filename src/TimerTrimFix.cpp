#include <Geode/Geode.hpp>

using namespace geode::prelude;

void nopWindows(size_t address, const char* funcName) {
    if(auto patch = Mod::get()->patch(reinterpret_cast<void*>(base::get() + address), {0x90, 0x90, 0x90, 0x90, 0x90})) {
        log::info("Timer fix: Patched {}", funcName);
    } else {
        log::error("Timer fix: Failed to patch {}", funcName);
    }
}

void nopMac(size_t address, const char* funcName) {
    if(auto patch = Mod::get()->patch(reinterpret_cast<void*>(base::get() + address), {0x90, 0x90, 0x90, 0x90, 0x90, 0x90})) {
        log::info("Timer fix: Patched {}", funcName);
    } else {
        log::error("Timer fix: Failed to patch {}", funcName);
    }
}

void nopMac2(size_t address, const char* funcName) {
    if(auto patch = Mod::get()->patch(reinterpret_cast<void*>(base::get() + address), {0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90})) {
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
    #elif defined(GEODE_IS_MAC)
        static_assert(GEODE_COMP_GD_VERSION == 22000, "Wrong GD version detected");
        nopMac(0xcc88, "EditorUI::init");
        nopMac(0x52623, "FUN_1000520b0");
        nopMac(0x5366b, "FUN_100053640");
        nopMac(0x54801, "FUN_1000547c0");
        nopMac(0x70d68, "FUN_100070d40");
        nopMac(0x73151, "GameStatsManager::storeChallengeTime");
        nopMac(0x887e8, "FUN_1000887c0");
        nopMac(0xa5f7c, "PlayLayer::init");
        nopMac(0xaddea, "PlayLayer::levelComplete");
        nopMac(0xb4909, "PlayLayer::destroyPlayer");
        nopMac(0xb534f, "FUN_1000b5310");
        nopMac(0xb880b, "PlayLayer::pauseGame");
        nopMac(0xb8b6c, "PlayLayer::resume");
        nopMac(0xd98bc, "LevelEditorLayer::updateEditor");
        nopMac(0x1f949e, "RewardsPage::updateTimers");
        nopMac2(0x210086, "DailyLevelPage::init");
        nopMac(0x21075c, "DailyLevelPage::updateTimers");
        nopMac(0x21142d, "FUN_100211410");
        nopMac(0x2450f8, "FUN_1002450d0 (patch 1)");
        nopMac(0x245163, "FUN_1002450d0 (patch 2)");
        nopMac(0x245724, "FUN_100245700");
        nopMac(0x245817, "FUN_100245780");
        nopMac(0x2479d5, "FUN_100247920");
        nopMac(0x247b39, "FUN_100247a30");
        nopMac(0x247c74, "FUN_100247c50");
        nopMac(0x247ce1, "FUN_100247cc0");
        nopMac(0x35da8d, "FUN_10035da70");
        nopMac(0x35db0d, "FUN_10035daf0");
        nopMac(0x35db4d, "FUN_10035daf0");
        nopMac(0x35dc7f, "FUN_10035dc60");
        nopMac(0x35e082, "GameManager::likeFacebook");
        nopMac(0x35e0f2, "GameManager::followTwitter");
        nopMac(0x35e162, "GameManager::subYouTube");
        nopMac(0x35e1d2, "GameManager::followTwitch");
        nopMac(0x35e242, "GameManager::joinDiscord");
        nopMac(0x35e2b2, "GameManager::joinReddit");
        nopMac(0x3684c3, "GameManager::applicationWillEnterForeground");
        nopMac(0x37b406, "ChallengesPage::init");
        nopMac(0x37b68a, "ChallengesPage::updateTimers");
        nopMac(0x384bbc, "FUN_100384b40");
        nopMac(0x384fcf, "FUN_100384f50");
        nopMac(0x3a61fd, "FUN_1003a61d0 (DialogLayer)");
        nopMac(0x3a68b2, "DialogLayer::animateIn");
        nopMac(0x3e0ebf, "PlayerObject::updateJump");
        nopMac(0x3e4041, "FUN_1003e3fb0");
        nopMac(0x3e4273, "FUN_1003e4080");
        nopMac(0x46ffdc, "FUN_10046fe60");
        nopMac(0x509fdc, "GameLevelManager::makeTimeStamp");
        nopMac(0x50a27f, "GameLevelManager::isTimeValid");
        nopMac(0x50a7bf, "GameLevelManager::getTimeLeft");
        nopMac(0x526ffc, "GameLevelManager::storeDailyLevelState (patch 1)");
        nopMac(0x52704a, "GameLevelManager::storeDailyLevelState (patch 2)");
        nopMac(0x527095, "GameLevelManager::storeDailyLevelState (patch 3)");
        nopMac(0x61d386, "GJMultiplayerManager::onJoinLobbyCompleted");
        nopMac(0x61fc09, "AppDelegate::trySaveGame (patch 1)");
        nopMac(0x61fc75, "AppDelegate::trySaveGame (patch 1)");
    #endif
}