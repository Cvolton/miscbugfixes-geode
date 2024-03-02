#include <Geode/Geode.hpp>

using namespace geode::prelude;

$execute {
    auto mod = Loader::get()->getLoadedMod("geode.loader");
    auto value = mod->getSavedValue<std::string>("last-modified-auto-update-check");
    if (Loader::get()->getVersion() < VersionInfo::parse("2.0.0-beta.21")) {
        mod->setSavedValue<std::string>("last-modified-auto-update-check", "Sat, 25 Feb 2023 15:16:52 GMT");
    }
}