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

        auto differences = Mod::get()->getSavedValue<matjson::Array>("differences");
        differences.push_back(total - old);
        Mod::get()->setSavedValue("differences", differences);

        log::info("Adjusted collected orbs by: {} ({} -> {})", total - old, old, total);
        
        GameStatsManager::sharedState()->setStat("22", total);

        s_orbFixApplied = true;

        return true;
    }
};