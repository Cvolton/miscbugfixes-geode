#ifdef GEODE_IS_ANDROID64

#include <Geode/Geode.hpp>

using namespace geode::prelude;

void deflateInitHook(z_streamp strm, int  level, int  method,
                 int windowBits, int memLevel,
                 int strategy, const char *version,
                 int stream_size) {
    
    log::info("Deflate init called with level {}, method {}, windowBits {}, memLevel {}, strategy {}, version {}, stream_size {}",
        level, method, windowBits, memLevel, strategy, version, stream_size);
    level = 1;
    auto result = deflateInit2(strm, level, method, windowBits, memLevel, strategy);
}

$on_mod(Loaded) {
    Mod::get()->hook(
        reinterpret_cast<void*>(
            geode::addresser::getNonVirtual(
                geode::modifier::Resolve<z_streamp strm, int  level, int  method,
                                      int windowBits, int memLevel,
                                      int strategy, const char *version,
                                      int stream_size>::func(&deflateInit2_)
            )
        ),
        &deflateInitHook,
        "deflateInit2_",
        tulip::hook::TulipConvention::Cdecl
    );

}

#endif