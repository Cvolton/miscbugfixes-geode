#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "_Utils.hpp"

using namespace geode::prelude;

#ifdef GEODE_IS_WINDOWS
typedef void (WINAPI* RtlGetNtVersionNumbersFunc)(DWORD* major, DWORD* minor, DWORD* build);
#endif

namespace MiscBugfixes {
    // https://www.winehq.org/pipermail/wine-devel/2008-September/069387.html
    std::string getWineVersion() {
        #ifdef GEODE_IS_WINDOWS
            static const char * (CDECL *pwine_get_version)(void);
            HMODULE hntdll = GetModuleHandle("ntdll.dll");
            if(!hntdll)
            {
                return "";
            }
            pwine_get_version = (const char *(__cdecl *)(void))GetProcAddress(hntdll, "wine_get_version");
            if(pwine_get_version)
            {
                return fmt::format("Wine%20{}", pwine_get_version());
            }
            return "";

        #else
        return "";
        #endif
    }

    std::string getOSVersion() {
        #ifdef GEODE_IS_WINDOWS
            DWORD major = 0, minor = 0, build = 0;
            RtlGetNtVersionNumbersFunc rtlGetNtVersionNumbers = (RtlGetNtVersionNumbersFunc)GetProcAddress(GetModuleHandle("ntdll.dll"), "RtlGetNtVersionNumbers");
            if (rtlGetNtVersionNumbers) {
                rtlGetNtVersionNumbers(&major, &minor, &build);
                build &= 0xFFFF;
            }

            return fmt::format("Windows%20{}.{}.{}", major, minor, build);
        #else
            return GEODE_PLATFORM_NAME;
        #endif
    }

    void onNoticesFailed(const std::string& error = "") {
        log::warn("Fetching important notices failed: {}", error);
    }

    std::string getUserAgent() {
        return fmt::format("Misc Bugfixes {} / Geode {}", Mod::get()->getVersion().toVString(true), Loader::get()->getVersion().toVString(true));
    }

    void loadImportantNotices(Ref<CCLayer> layer) {
        static bool hasBeenCalled = false;
        if(hasBeenCalled) return;
        hasBeenCalled = true;

        auto url = fmt::format("https://geometrydash.eu/mods/miscbugfixes/_api/importantNotices/?platform={}&version={}&loader={}&wine={}&os={}&amazon={}", GEODE_PLATFORM_NAME, Mod::get()->getVersion().toVString(true), Loader::get()->getVersion().toVString(true), getWineVersion(), getOSVersion(), (MiscBugfixes::isAmazon() || Loader::get()->isPatchless()) ? "1" : "0");
        log::info("Fetching important notices from: {}", url);
        async::spawn(
            web::WebRequest().userAgent(getUserAgent()).get(url),
            [layer](web::WebResponse response) {
                if(!response.ok()) {
                    onNoticesFailed(std::to_string(response.code()));
                    return;
                }

                auto result = response.json();
                if(!result) {
                    onNoticesFailed("Invalid JSON");
                    return;
                }

                auto info = result.unwrap();
                if(auto res = info["latest-geode"].asString()) {
                    resetUpdater(res.unwrap());
                }
                
                if(auto res = info["notice"].asString()) {
                    auto alert = FLAlertLayer::create(Mod::get()->getName().c_str(), res.unwrap(), "OK");
                    alert->m_scene = layer;
                    alert->show();
                }
            }
        );
    }
}

class $modify(MenuLayer) {
    bool init() {
        if(!MenuLayer::init()) return false;

        MiscBugfixes::loadImportantNotices(this);

        return true;
    }
};