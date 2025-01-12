#include <Geode/Geode.hpp>
#include "_MenuLayerManager.hpp"

using namespace geode::prelude;

$on_mod(Loaded) {
    MenuLayerManager::queueFunction([](MenuLayer*) {
        auto total = GameStatsManager::sharedState()->getTotalCollectedCurrency();
        auto old = GameStatsManager::sharedState()->getStat("22");
        auto diff = total - old;

        auto differences = Mod::get()->getSavedValue<std::vector<int>>("differences");
        if(diff != 0) differences.push_back(diff);
        Mod::get()->setSavedValue("differences", differences);

        log::info("Adjusted collected orbs by: {} ({} -> {})", diff, old, total);
        
        GameStatsManager::sharedState()->setStat("22", total);
    });
}