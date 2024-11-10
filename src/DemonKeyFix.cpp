#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

static bool s_orbFixApplied = false;

class $modify(MenuLayer) {
    bool init() {
        if(!MenuLayer::init()) return false;

        if(s_orbFixApplied) return true;

        auto total = GameStatsManager::sharedState()->getTotalCollectedCurrency();
        auto old = GameStatsManager::sharedState()->getStat("22");
        auto diff = total - old;

        auto differences = Mod::get()->getSavedValue<std::vector<int>>("differences");
        if(diff != 0) differences.push_back(diff);
        Mod::get()->setSavedValue("differences", differences);

        log::info("Adjusted collected orbs by: {} ({} -> {})", diff, old, total);
        
        GameStatsManager::sharedState()->setStat("22", total);

        s_orbFixApplied = true;

        return true;
    }
};