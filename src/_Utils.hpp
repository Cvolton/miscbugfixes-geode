#pragma once

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