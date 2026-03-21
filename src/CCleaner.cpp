#include <Geode/Geode.hpp>
#include "_MenuLayerManager.hpp"

using namespace geode::prelude;

void removeSubdirIfExists(const std::string& subdir, const std::string& friendlyName) {
    if(std::filesystem::exists(dirs::getGeodeDir() / subdir)) {
        log::info("Cleaning up {}...", friendlyName);
        std::error_code ec;
        std::filesystem::remove_all(dirs::getGeodeDir() / subdir, ec);
        if(ec) {
            log::error("Failed to clean up {}: {}", friendlyName, ec.message());
        } else {
            log::info("Successfully cleaned up {}", friendlyName);
        }
    }
}

void cleanUpIndexCache() {
    removeSubdirIfExists("index", "old index cache");
}

void cleanUpUninstalledMods() {
    std::error_code ec;

    log::info("Cleaning up uninstalled mods...");
    for(auto& entry : std::filesystem::directory_iterator(dirs::getModRuntimeDir(), ec)) {
        if(!std::filesystem::is_directory(entry, ec)) continue;

        auto modDir = entry.path();
        bool modInstalled = true;

        // not a mod if this fails, presumably user data saved on accident
        if(!(std::filesystem::exists(modDir / "mod.json", ec) || std::filesystem::is_empty(modDir, ec)) || ec) {
            try {
                log::warn("Found non-mod directory in mods folder: {}", modDir.filename());
            } catch(const std::exception& e) {
                log::error("Failed to log non-mod directory: {}", e.what());
            }
            continue;
        }

        // this should only have a possibility of throwing an exception on Windows
        try {
            modInstalled = ec || Loader::get()->isModInstalled(modDir.filename().string());
        } catch(const std::exception& e) {
            log::error("Failed to check if mod is installed: {}", e.what());
            continue;
        }

        if(modInstalled) continue;
        log::info("Cleaning up uninstalled mod: {}", modDir.filename());
        
        std::filesystem::remove_all(modDir, ec);
        if(ec) {
            log::error("Failed to clean up uninstalled mod: {}", ec.message());
        } else {
            log::info("Successfully cleaned up uninstalled mod: {}", modDir.filename());
        }
    }
    log::info("Finished cleaning up uninstalled mods");
}

void cleanUpStaleUpdate() {
    GEODE_ANDROID(removeSubdirIfExists("update", "stale Geode update binaries"));
}

void cleanUpShouldLoadInvalidMods() {
    log::info("Cleaning up should-load flags for invalid mods...");

    static constexpr std::string_view shouldLoadPrefix = "should-load-";
    if(auto geode = Loader::get()->getLoadedMod("geode.loader")) {
        auto& save = geode->getSaveContainer();
        std::vector<std::string> toErase;
        for(auto& [key, _] : save) {
            if(key.starts_with("should-load-geode_invalid")) {
                auto modID = key.substr(shouldLoadPrefix.size());
                if(!Loader::get()->isModInstalled(modID)) {
                    toErase.push_back(key);
                }
            }
        }

        for (const auto& key : toErase) {
            save.erase(key);
        }
    }

    log::info("Finished cleaning up should-load flags for invalid mods");
}

$on_mod(Loaded) {
    if(Mod::get()->getSettingValue<bool>("skip-ccleaner")) return;

    std::thread([] {
        thread::setName("CCleaner");

        cleanUpIndexCache();
        cleanUpStaleUpdate();
    }).detach();

    MenuLayerManager::queueFunction([](MenuLayer *layer) {
        cleanUpUninstalledMods();
        cleanUpShouldLoadInvalidMods();
    });
}