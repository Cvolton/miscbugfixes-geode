#include <Geode/Geode.hpp>

using namespace geode::prelude;

$execute {
    // This makes the game save user IDs for all undeleted saved levels
    // This prevents an issue where profiles would incorrectly show "-"
    // while being opened from there
    #ifdef GEODE_IS_WINDOWS
        static_assert(GEODE_COMP_GD_VERSION == 22040, "Wrong GD version detected");
        //patch JZ to jump by 0 bytes
        if(!Mod::get()->patch(reinterpret_cast<void*>(base::get() + 0x1165b1), {0x74, 0x00})) {
            log::error("Failed to apply user ID fix");
        }
    #endif
    //TODO: mac
    //TODO: android
}