
#include <Geode/Geode.hpp>
#include "_Utils.hpp"

using namespace geode::prelude;

#ifdef GEODE_IS_ANDROID64

static auto levelPatch = patch(0xB2A650, {0x01, 0x00, 0x80, 0x12}, {0x21, 0x00, 0x80, 0x52});

#endif