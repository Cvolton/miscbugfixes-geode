
#include <Geode/Geode.hpp>
#include "_Utils.hpp"

using namespace geode::prelude;


static auto levelPatch = patch(0xB2A650, {0x01, 0x00, 0x80, 0x12}, {0x01, 0x00, 0x80, 0x52});

$on_mod(Loaded) {
    if(!levelPatch.isOk()) {
        log::error("Failed to patch zlib level: {}", levelPatch.unwrapErr());
    } else {
        log::debug("Patched zlib level to 1 for faster compression");
    }
}

#include <Geode/modify/MenuLayer.hpp>
class $modify(MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        std::vector<ByteVector> bytes = {
            {0x01, 0x00, 0x80, 0x52}, // zlib level 0
            {0x21, 0x00, 0x80, 0x52},  // zlib level 1
            {0x41, 0x00, 0x80, 0x52},  // zlib level 2
            {0x61, 0x00, 0x80, 0x52},  // zlib level 3
            {0x81, 0x00, 0x80, 0x52},  // zlib level 4
            {0xa1, 0x00, 0x80, 0x52},  // zlib level 5
            {0xc1, 0x00, 0x80, 0x52},  // zlib level 6
            {0xe1, 0x00, 0x80, 0x52},  // zlib level 7
            {0x01, 0x01, 0x80, 0x52},   // zlib level 8
            {0x21, 0x01, 0x80, 0x52}    // zlib level 9
        };
        
        size_t idx = 0;
        for(const auto& byteVec : bytes) {
            levelPatch.unwrap()->updateBytes(byteVec);

            auto now = asp::Instant::now();
            GameManager::get()->getSaveString();
            LocalLevelManager::get()->getSaveString();
            log::info("Saving with zlib level {} took {}", idx, now.elapsed());

            idx++;
        }

        return true;
    }
};