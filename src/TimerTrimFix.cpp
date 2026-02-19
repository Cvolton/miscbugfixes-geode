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
        static_assert(GEODE_COMP_GD_VERSION == 22081, "Wrong GD version detected");
        nopWindowsMulti({0x38be2c, 0x3a34a9, 0x3a35e9, 0x085a56, 0x03cf56, 0x149ce5, 0x0cf590, 0x149e6e, 0x169120, 0x169a88, 0x169adc, 0x2832a5, 0x149fb2, 0x3cd876});
    #elif defined(GEODE_IS_INTEL_MAC)
        static_assert(GEODE_COMP_GD_VERSION == 22074, "Wrong GD version detected");
        nopMacMulti({0x90a8, 0x4f40d, 0x503d0, 0x403059, 0x51701, 0x72125, 0x72d71, 0x8c688, 0xac3a0, 0xb3949, 0xba281, 0x3c84a2, 0xbf6dc, 0x538c74, 0x5383fc, 0xdd653, 0xfe5b3, 0x402d51, 0x402ff6, 0x5386d4, 0x21b10a, 0x23139c, 0x2320cd, 0x266f46, 0x266fb1, 0x267687, 0x267594, 0x269885, 0x2699e9, 0x269b24, 0x269b91, 0x39d8d7, 0x3807c2, 0x380832, 0x3808a2, 0x3800bd, 0x38013d, 0x38017d, 0x38036f, 0x380912, 0x380982, 0x3809f2, 0x38bbb3, 0x39db5a, 0x3a6f03, 0x3a730c, 0x3c7ded, 0x49436d, 0xbf33c, 0x559cfc, 0x559d4a, 0x559d95, 0x659bd9, 0x65c789, 0x65c7f5, 0xbb0ff});
        nopMac2(0x21edf6, "DailyLevelPage::init");
    #elif defined(GEODE_IS_ARM_MAC)
        static_assert(GEODE_COMP_GD_VERSION == 22074, "Wrong GD version detected");
        nopMacArmMulti({0xad9c, 0x09d88c, 0x047268, 0x048e88, 0x0657e0, 0x07fc84, 0x09cf28, 0x0aa31c, 0x340b8c, 0x0aafdc, 0x0add00, 0x371efc, 0x37215c, 0x47e1bc, 0x47e4bc, 0x1be304, 0x47eb1c, 0x1d24e4, 0x202484, 0x20257c, 0x2044c4, 0x204614, 0x204798, 0x319ca4, 0x2fe740, 0x2fe7c0, 0x2fe840, 0x2fe018, 0x2fe084, 0x2fe0cc, 0x2fe2dc, 0x2fe8c0, 0x2fe940, 0x2fe9c0, 0x308b7c, 0x32266c, 0x3404f4, 0x0ae054, 0x56ec38, 0x56ec98, 0x066398, 0x49c828, 0x49c880, 0x49c8d8, 0x0c5480, 0x201e9c, 0x201ef8, 0x319f68, 0x56c32c, 0x048120, 0x1d31c0, 0x204710, 0x3229f8, 0x3ef0a0});
        nopMacArm2(0x1d1b48, "DailyLevelPage::init");
    #elif defined(GEODE_IS_IOS)
        static_assert(GEODE_COMP_GD_VERSION == 22081, "Wrong GD version detected");
        if(Loader::get()->isPatchless()) return; // Jitless would crash here
        nopMacArmMulti({0x099558, 0x0993e0, 0x032124, 0x03235c, 0x122864, 0x21db04, 0x0871a0, 0x0875b4, 0x1159a0, 0x119d40, 0x12c594, 0x12c878, 0x122b64, 0x09984c, 0x1ba8bc, 0x1c46d0, 0x21d854, 0x21dab8, 0x3c4ec4, 0x31d3b8, 0x269754, 0x2697b4, 0x317274, 0x3172f0, 0x31736c, 0x3173e8, 0x317464, 0x3174e0, 0x333968, 0x3f8af0, 0x3fa758, 0x3331e8, 0x0ab8e8, 0x0ab92c, 0x0ab970, 0x358370, 0x11ec4c, 0x11f8f0, 0x1cec80, 0x367d5c, 0x3f98bc});
        nopMacArm2(0x1b9870, "DailyLevelPage::init");

        GEODE_MOD_STATIC_PATCH(0x099558, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x0993e0, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x032124, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x03235c, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x122864, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x21db04, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x0871a0, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x0875b4, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x1159a0, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x119d40, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x12c594, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x12c878, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x122b64, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x09984c, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x1ba8bc, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x1c46d0, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x21d854, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x21dab8, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x3c4ec4, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x31d3b8, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x269754, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x2697b4, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x317274, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x3172f0, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x31736c, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x3173e8, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x317464, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x3174e0, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x333968, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x3f8af0, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x3fa758, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x3331e8, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x0ab8e8, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x0ab92c, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x0ab970, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x358370, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x11ec4c, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x11f8f0, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x1cec80, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x367d5c, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x3f98bc, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x1b9870, {0x1F, 0x20, 0x03, 0xD5});
    #elif defined(GEODE_IS_ANDROID)
        //fix irrelevant
    #else
        static_assert(false, "Unsupported platform");
    #endif
}