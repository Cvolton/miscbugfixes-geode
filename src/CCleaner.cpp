#include <Geode/Geode.hpp>
#include <asp/fs.hpp>
#include "_MenuLayerManager.hpp"

using namespace geode::prelude;

void removeSubdirIfExists(const std::string& subdir, const std::string& friendlyName) {
    if(asp::fs::exists(dirs::getGeodeDir() / subdir)) {
        log::info("Cleaning up {}...", friendlyName);
        std::error_code ec;
        auto res = asp::fs::removeDirAll(dirs::getGeodeDir() / subdir);
        if(!res) {
            log::error("Failed to clean up {}: {}", friendlyName, res.unwrapErr());
        } else {
            log::info("Successfully cleaned up {}", friendlyName);
        }
    }
}

void cleanUpIndexCache() {
    removeSubdirIfExists("index", "old index cache");
}

void cleanUpUninstalledMods() {
    asp::Instant start = asp::Instant::now();

    log::info("Cleaning up uninstalled mods...");
    auto res = asp::fs::iterDir(dirs::getModRuntimeDir());
    if(res) for(auto entry : res.unwrap()) {
        if(!entry || !entry.unwrap().isDir()) continue;
        auto modDir = entry.unwrap().path;
        bool modInstalled = true;

        // not a mod if this fails, presumably user data saved on accident
        if(!asp::fs::exists(modDir / "mod.json") /*|| std::filesystem::is_empty(modDir, ec)) || ec*/) {
            //log::warn("Found non-mod directory in mods folder: {}", entry);
            continue;
        }

        // this should only have a possibility of throwing an exception on Windows
        modInstalled = Loader::get()->isModInstalled(modDir.filename().value_or("invalid-mod-id"));

        if(modInstalled) continue;
        //log::info("Cleaning up uninstalled mod: {}", modDir.filename().value_or("invalid-mod-id").string());
        
        auto res = asp::fs::removeDirAll(modDir);
        if(!res) {
            //log::error("Failed to clean up uninstalled mod {}: {}", modDir.filename().value_or("invalid-mod-id").string(), res.unwrapErr());
        } else {
            //log::info("Successfully cleaned up uninstalled mod: {}", modDir.filename().value_or("invalid-mod-id").string());
        }
    }
    log::info("Finished cleaning up uninstalled mods, took {}", start.elapsed());
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
            if(key.starts_with("should-load-geode_invalid") || key.starts_with("is-pinned-geode_invalid")) {
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

    MenuLayerManager::queueFunction([](MenuLayer *layer) {
        async::runtime().spawnBlocking<void>([] {
            cleanUpIndexCache();
            cleanUpStaleUpdate();
            cleanUpUninstalledMods();
        });
        cleanUpShouldLoadInvalidMods();
    });
}