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
    #elif defined(GEODE_IS_ANDROID)
        //fix irrelevant
    #else
        static_assert(false, "Unsupported platform");
    #endif
}