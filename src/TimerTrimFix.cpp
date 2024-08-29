#include <Geode/Geode.hpp>
#include "_Utils.hpp"

using namespace geode::prelude;

void nopWindows(size_t address, const char* funcName) {
    auto result = patch(
        address,
        {0x25, 0xff, 0xff, 0x0f, 0x00},
        {0x90, 0x90, 0x90, 0x90, 0x90}
    );

    if(result.isErr()) {
        log::error("Timer fix: Failed to patch {} - {}", funcName, result.unwrapErr());
    } else {
        log::debug("Timer fix: Patched {} - {}", funcName, address);
    }
}

void nopWindowsMulti(std::initializer_list<size_t> addresses) {
    for(auto address : addresses) {
        nopWindows(address, "nopWindowsMulti");
    }
}

void nopWindows2(size_t address, const char* funcName) {
    auto result = patch(
        address,
        {0x81, 0xe1, 0xff, 0xff, 0x0f, 0x00},
        {0x90, 0x90, 0x90, 0x90, 0x90, 0x90}
    );

    if(result.isErr()) {
        log::error("Timer fix: Failed to patch {} - {}", funcName, result.unwrapErr());
    } else {
        log::debug("Timer fix: Patched {} - {}", funcName, address);
    }
}

void nopMac(size_t address, const char* funcName) {
    auto result = patch(
        address,
        {0x81, 0xe1, 0xff, 0xff, 0x0f, 0x00},
        {0x90, 0x90, 0x90, 0x90, 0x90, 0x90}
    );

    if(result.isErr()) {
        log::error("Timer fix: Failed to patch {} - {}", funcName, result.unwrapErr());
    } else {
        log::debug("Timer fix: Patched {} - {}", funcName, address);
    }
}

void nopMacMulti(std::initializer_list<size_t> addresses) {
    for(auto address : addresses) {
        nopMac(address, "nopMacMulti");
    }
}

void nopMac2(size_t address, const char* funcName) {
    auto result = patch(
        address,
        {0x41, 0x81, 0xe6, 0xff, 0xff, 0x0f, 0x00},
        {0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90}
    );

    if(result.isErr()) {
        log::error("Timer fix: Failed to patch {} - {}", funcName, result.unwrapErr());
    } else {
        log::debug("Timer fix: Patched {}", funcName);
    }
}

void nopMacArm(size_t address, const char* funcName) {
    auto result = patch(
        address,
        {0x29, 0x4d, 0x40, 0x92},
        {0x1F, 0x20, 0x03, 0xD5}
    );

    if(result.isErr()) {
        log::error("Timer fix: Failed to patch {} - {}", funcName, result.unwrapErr());
    } else {
        log::debug("Timer fix: Patched {} - {}", funcName, address);
    }
}

void nopMacArmMulti(std::initializer_list<size_t> addresses) {
    for(auto address : addresses) {
        nopMacArm(address, "nopMacArmMulti");
    }
}

void nopMacArm2(size_t address, const char* funcName) {
    auto result = patch(
        address,
        {0xc8, 0x4e, 0x40, 0x92},
        {0x1F, 0x20, 0x03, 0xD5}
    );

    if(result.isErr()) {
        log::error("Timer fix: Failed to patch {} - {}", funcName, result.unwrapErr());
    } else {
        log::debug("Timer fix: Patched {}", funcName);
    }
}

$execute {
    // This just nops out the AND that cuts the timestamp to the bottom 20 bits
    // so that the timer doesn't reset after 1048576 seconds

    /**
     * If you look into it, the reason why Rob initially added this makes sense:
     *
     * The timer was presumably stored in a 32-bit float, which would cause
     * a precision of 256 seconds. With the AND, the timer would have a precision
     * of 1/16th of a second, which is more than enough.
     *
     * However, the timer is now stored in a 64-bit double, which has a much higher
     * precision than the 32-bit float, so the AND is no longer necessary.
     *
     * This is proven by Android behavior, where this trim does not happen.
    */
    #ifdef GEODE_IS_WINDOWS
        static_assert(GEODE_COMP_GD_VERSION == 22060, "Wrong GD version detected");
        nopWindowsMulti({0x8436b, 0x369f7c, 0x16179c, 0x1617ed, 0x3c096, 0x3803f9, 0x380539, 0x2c509f, 0x14490e, 0x3a98c0, 0x144a52, 0x2710e5, 0xcc510, 0x144785});
        nopWindows2(0x161747, "GetGJDailyLevelState finished; its the callback in handleIt");
    #elif defined(GEODE_IS_INTEL_MAC)
        static_assert(GEODE_COMP_GD_VERSION == 22060, "Wrong GD version detected");
        nopMacMulti({0x0094f8, 0x40c1c9, 0x04fbd3, 0x050c1b, 0x051dd1, 0x0710b5, 0x073681, 0x08bba8, 0x0abe63, 0x0acaa9, 0x0bb623, 0x3cc002, 0x0bc00f, 0x0bf6ab, 0x0bfa0c, 0x53fb14, 0x53f2ec, 0x0e17dc, 0x40bec1, 0x40c166, 0x53f594, 0x215bce, 0x22cc70, 0x22d9dd, 0x262e5c, 0x262ec7, 0x2635e7, 0x2634f4, 0x265975, 0x265ad9, 0x265c14, 0x265c81, 0x3a08b6, 0x382082, 0x3820f2, 0x382162, 0x38196d, 0x3819ed, 0x381a2d, 0x381c0f, 0x3821d2, 0x382242, 0x3822b2, 0x38ce13, 0x3a0b3a, 0x3aa11c, 0x3aa52f, 0x3cb94d, 0x49d758, 0x560d2c, 0x560d7a, 0x560dc5, 0x666a90, 0x669959, 0x6699c5});
        nopMac2(0x22c586, "DailyLevelPage::init");
    #elif defined(GEODE_IS_ARM_MAC)
        static_assert(GEODE_COMP_GD_VERSION == 22060, "Wrong GD version detected");
        nopMacArmMulti({0x00b7e0, 0x384124, 0x047fe0, 0x049bec, 0x07ce9c, 0x09a3a0, 0x09ae08, 0x0a7370, 0x34eb78, 0x0a7fd8, 0x0aad20, 0x4913cc, 0x383e68, 0x3840c8, 0x49169c, 0x1c6840, 0x491c9c, 0x1da644, 0x20a9fc, 0x20aaf4, 0x20cb18, 0x20cc68, 0x20cdec, 0x327730, 0x30c18c, 0x30c20c, 0x30c28c, 0x30ba10, 0x30ba7c, 0x30bac4, 0x30bcd0, 0x30c30c, 0x30c38c, 0x30c40c, 0x315df4, 0x33023c, 0x34e4e0, 0x0ab04c, 0x5856f0, 0x585750, 0x065030, 0x066724, 0x4af684, 0x4af6dc, 0x4af734, 0x0c7a98, 0x20a3c4, 0x20a420, 0x3279fc, 0x582b54, 0x048ea0, 0x1db364, 0x20cd64, 0x3305cc, 0x401a94})
        nopMacArm2(0x1d9f9c, "DailyLevelPage::init");
    #elif defined(GEODE_IS_ANDROID)
        //fix irrelevant
    #else
        static_assert(false, "Unsupported platform");
    #endif
}