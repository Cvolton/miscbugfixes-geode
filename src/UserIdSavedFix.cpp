#include <Geode/Geode.hpp>

using namespace geode::prelude;

$execute {
    // This makes the game save user IDs for all undeleted saved levels
    // This prevents an issue where profiles would incorrectly show "-"
    // while being opened from there
    //
    // Patched function: GJGameLevel::encodeWithCoder
    #ifdef GEODE_IS_WINDOWS
        static_assert(GEODE_COMP_GD_VERSION == 22040, "Wrong GD version detected");
        //patch JZ to jump by 0 bytes
        if(!Mod::get()->patch(reinterpret_cast<void*>(base::get() + 0x1165b1), {0x74, 0x00})) {
            log::error("Failed to apply user ID fix");
        }
    #elif defined(GEODE_IS_MACOS)
        static_assert(GEODE_COMP_GD_VERSION == 22000, "Wrong GD version detected");
        if(!Mod::get()->patch(reinterpret_cast<void*>(base::get() + 0x52bc83), {0x74, 0x00})) {
            log::error("Failed to apply user ID fix");
        }
    #elif defined(GEODE_IS_ANDROID32)
        static_assert(GEODE_COMP_GD_VERSION == 22050, "Wrong GD version detected");
        // NOP out CBZ
        if(!Mod::get()->patch(reinterpret_cast<void*>(base::get() + 0x3B162C), {0x00, 0xbf})) {
            log::error("Failed to apply user ID fix");
        }
    #elif defined(GEODE_IS_ANDROID64)
        static_assert(GEODE_COMP_GD_VERSION == 22050, "Wrong GD version detected");
        //patch CBNZ to B
        if(!Mod::get()->patch(reinterpret_cast<void*>(base::get() + 0x6E0050), {0x87, 0x01, 0x00, 0x14})) {
            log::error("Failed to apply user ID fix");
        }
    #else
        static_assert(false, "Unsupported platform");
    #endif
}