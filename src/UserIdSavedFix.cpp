#include <Geode/Geode.hpp>
#include "_Utils.hpp"

using namespace geode::prelude;

$execute {
    // This makes the game save user IDs for all undeleted saved levels.
    // This prevents an issue where profiles would incorrectly show "-"
    // while being opened from there.
    //
    // Patched function: GJGameLevel::encodeWithCoder
    #ifdef GEODE_IS_WINDOWS
        static_assert(GEODE_COMP_GD_VERSION == 22074, "Wrong GD version detected");
        // Patch: Change JZ jump length to 0 bytes
        auto result = patch(
            0x16c6bf,
            {0x74, 0x1e},
            {0x74, 0x00}
        );
        if(result.isErr()) {
            log::error("Failed to apply user ID fix - {}", result.unwrapErr());
        }
    #elif defined(GEODE_IS_INTEL_MAC)
        static_assert(GEODE_COMP_GD_VERSION == 22074, "Wrong GD version detected");
        // Patch: Change JZ jump length to 0 bytes
        auto result = patch(
            0x5488a6,
            {0x74, 0x1d},
            {0x74, 0x00}
        );
        if(result.isErr()) {
            log::error("Failed to apply user ID fix - {}", result.unwrapErr());
        }
    #elif defined(GEODE_IS_ARM_MAC)
        static_assert(GEODE_COMP_GD_VERSION == 22074, "Wrong GD version detected");
        // Patch: NOP out CBZ instruction
        auto result = patch(
            0x4a1140,
            {0x15, 0x01, 0x00, 0x34},
            {0x1F, 0x20, 0x03, 0xD5}
        );
        if(result.isErr()) {
            log::error("Failed to apply user ID fix - {}", result.unwrapErr());
        }
    #elif defined(GEODE_IS_IOS)
        // For iOS the user ID issue does not occur.
        log::debug("User ID fix: iOS detected - user ID patch not necessary.");
    #elif defined(GEODE_IS_ANDROID32)
        static_assert(GEODE_COMP_GD_VERSION == 22074, "Wrong GD version detected");
        // Patch: NOP out CBZ instruction
        auto result = patch(
            0x3B8F48,
            {0x56, 0xB1},
            {0x00, 0xBF}
        );
        if(result.isErr()) {
            log::error("Failed to apply user ID fix - {}", result.unwrapErr());
        }
    #elif defined(GEODE_IS_ANDROID64)
        static_assert(GEODE_COMP_GD_VERSION == 22074, "Wrong GD version detected");
        // Patch: Change CBNZ to B
        auto result = patch(
            0x6EFF34,
            {0xF6, 0x30, 0x00, 0x35},
            {0x87, 0x01, 0x00, 0x14}
        );
        if(result.isErr()) {
            log::error("Failed to apply user ID fix - {}", result.unwrapErr());
        }
    #else
        static_assert(false, "Unsupported platform");
    #endif
}