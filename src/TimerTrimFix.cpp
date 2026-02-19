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
        //nopWindows2(0x00000, "GetGJDailyLevelState finished; its the callback in handleIt");
    #elif defined(GEODE_IS_INTEL_MAC)
        static_assert(GEODE_COMP_GD_VERSION == 22074, "Wrong GD version detected");
        nopMacMulti({0x009748, 0x3f1439, 0x04f7ff, 0x05082b, 0x051a01, 0x071d95, 0x072951, 0x08c038, 0x0ac00e, 0x0aca99, 0x0bb276, 0x3b5592, 0x0bbcaf, 0x0bf30b, 0x0bf69c, 0x522b89, 0x5222bc, 0x0ddbfc, 0x3f1131, 0x3f13d6, 0x522599, 0x20960e, 0x21f5ac, 0x2202bd, 0x254f86, 0x254ff1, 0x2556c7, 0x2555d4, 0x257905, 0x257a69, 0x257ba4, 0x257c11, 0x38a71a, 0x36d0d2, 0x36d142, 0x36d1b2, 0x36c9fd, 0x36ca7d, 0x36cabd, 0x36ccaf, 0x36d222, 0x36d292, 0x36d302, 0x378663, 0x38a9aa, 0x393dcc, 0x3941df, 0x3b4edd, 0x48116d, 0x54383c, 0x54388a, 0x5438d5, 0x6438f9, 0x6464f9, 0x646565});
        nopMac2(0x21edf6, "DailyLevelPage::init");
    #elif defined(GEODE_IS_ARM_MAC)
        static_assert(GEODE_COMP_GD_VERSION == 22074, "Wrong GD version detected");
        nopMacArmMulti({0x00ad9c, 0x09d88c, 0x047268, 0x048e88, 0x0657e0, 0x07fc84, 0x09cf28, 0x0aa31c, 0x340b8c, 0x0aafdc, 0x0add00, 0x371efc, 0x37215c, 0x47e1bc, 0x47e4bc, 0x1be304, 0x47eb1c, 0x1d24e4, 0x202484, 0x20257c, 0x2044c4, 0x204614, 0x204798, 0x319ca4, 0x2fe740, 0x2fe7c0, 0x2fe840, 0x2fe018, 0x2fe084, 0x2fe0cc, 0x2fe2dc, 0x2fe8c0, 0x2fe940, 0x2fe9c0, 0x308b7c, 0x32266c, 0x3404f4, 0x0ae054, 0x56ec38, 0x56ec98, 0x066398, 0x49c828, 0x49c880, 0x49c8d8, 0x0c5480, 0x201e9c, 0x201ef8, 0x319f68, 0x56c32c, 0x048120, 0x1d31c0, 0x204710, 0x3229f8, 0x3ef0a0});
        nopMacArm2(0x1d1b48, "DailyLevelPage::init");
    #elif defined(GEODE_IS_IOS)
        static_assert(GEODE_COMP_GD_VERSION == 22074, "Wrong GD version detected");
        if(Loader::get()->isPatchless()) return; // Jitless would crash here
        nopMacArmMulti({0x09d10c, 0x09cf94, 0x1190d8, 0x036384, 0x0365bc, 0x125c08, 0x08b244, 0x08b65c, 0x118984, 0x123550, 0x125ee8, 0x09d400, 0x1bc87c, 0x1c53ec, 0x21e314, 0x21e568, 0x3bd424, 0x31d6bc, 0x2688c0, 0x26891c, 0x317450, 0x3174cc, 0x317548, 0x3175c4, 0x317640, 0x3176bc, 0x333990, 0x3f0bd8, 0x3f26e8, 0x333210, 0x0aedd0, 0x0aee14, 0x0aee58, 0x357c80, 0x12292c, 0x12f844, 0x12fb14, 0x1cf9fc, 0x3f19c4});
        nopMacArm2(0x1bb834, "DailyLevelPage::init");

        GEODE_MOD_STATIC_PATCH(0x09d10c, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x09cf94, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x1190d8, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x036384, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x0365bc, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x125c08, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x08b244, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x08b65c, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x118984, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x123550, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x125ee8, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x09d400, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x1bc87c, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x1c53ec, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x21e314, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x21e568, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x3bd424, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x31d6bc, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x2688c0, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x26891c, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x317450, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x3174cc, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x317548, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x3175c4, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x317640, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x3176bc, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x333990, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x3f0bd8, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x3f26e8, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x333210, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x0aedd0, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x0aee14, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x0aee58, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x357c80, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x12292c, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x12f844, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x12fb14, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x1cf9fc, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x3f19c4, {0x1F, 0x20, 0x03, 0xD5});
        GEODE_MOD_STATIC_PATCH(0x1bb834, {0x1F, 0x20, 0x03, 0xD5});
    #elif defined(GEODE_IS_ANDROID)
        //fix irrelevant
    #else
        static_assert(false, "Unsupported platform");
    #endif
}