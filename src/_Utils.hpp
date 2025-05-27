#pragma once

#ifdef GEODE_IS_WINDOWS
    #ifdef CVOLTON_MISCBUGFIXES_EXPORTING
        #define MISCBUGFIXES_DLL __declspec(dllexport)
    #else
        #define MISCBUGFIXES_DLL __declspec(dllimport)
    #endif
#else
    #define MISCBUGFIXES_DLL __attribute__((visibility("default")))
#endif

#include <Geode/Geode.hpp>

using namespace geode::prelude;

inline Result<Patch*> patch(size_t address, std::initializer_list<uint8_t> expected, std::initializer_list<uint8_t> replacement) {
    auto offset = reinterpret_cast<uint8_t*>(base::get() + address);
    for(size_t i = 0; i < expected.size(); i++) {
        if(offset[i] != *(expected.begin() + i)) {
            return Err("Failed to patch - instruction doesn't match!");
        }
    }

    return Mod::get()->patch(reinterpret_cast<void*>(base::get() + address), replacement);
}

// for Geode Updater Fix
inline void resetUpdater(const VersionInfo& version) {
    log::info("Checking if {} is older than {}", Loader::get()->getVersion(), version);
    auto mod = Loader::get()->getLoadedMod("geode.loader");
    if (Loader::get()->getVersion() < version) {
        mod->setSavedValue<std::string>("last-modified-auto-update-check", "Sat, 25 Feb 2023 15:16:52 GMT");
    }
}

inline void resetUpdater(const std::string& versionString) {
    auto info = VersionInfo::parse(versionString);
    if(info.isOk()) resetUpdater(info.unwrap());
}

namespace MiscBugfixes {
    bool MISCBUGFIXES_DLL isAmazon();
    void setAmazon(bool isAmazon);
}