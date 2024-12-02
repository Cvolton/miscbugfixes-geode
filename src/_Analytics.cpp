#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "_Utils.hpp"

using namespace geode::prelude;

// https://stackoverflow.com/questions/25986331/how-to-determine-windows-version-in-future-proof-way
#ifdef GEODE_IS_WINDOWS
#pragma comment(lib, "ntdll.lib")

extern "C" {
typedef LONG NTSTATUS, *PNTSTATUS;
#define STATUS_SUCCESS (0x00000000)

// Windows 2000 and newer
NTSYSAPI NTSTATUS NTAPI RtlGetVersion(PRTL_OSVERSIONINFOEXW lpVersionInformation);
}
#endif

static std::optional<web::WebTask> s_requestTask = std::nullopt;

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
            else
            {
                return "";
            }
            return 0;

        #else
        return "";
        #endif
    }

    std::string getOSVersion() {
        #ifdef GEODE_IS_WINDOWS
            // https://stackoverflow.com/questions/25986331/how-to-determine-windows-version-in-future-proof-way
            RTL_OSVERSIONINFOEXW osVers;
            osVers.dwOSVersionInfoSize = sizeof(osVers);

            NTSTATUS status = RtlGetVersion(&osVers);

            return fmt::format("Windows%20{}.{}.{}", osVers.dwMajorVersion, osVers.dwMinorVersion, osVers.dwBuildNumber);
        #else
            return GEODE_PLATFORM_NAME;
        #endif
    }

    void onNoticesFailed(CCLayer* layer, const std::string& error = "") {
        log::warn("Fetching important notices failed: {}", error);
        s_requestTask = std::nullopt;
        layer->release();
    }

    std::string getUserAgent() {
        return fmt::format("Misc Bugfixes {} / Geode {}", Mod::get()->getVersion().toVString(true), Loader::get()->getVersion().toVString(true));
    }

    void loadImportantNotices(CCLayer* layer) {
        static bool hasBeenCalled = false;
        if(hasBeenCalled) return;
        hasBeenCalled = true;

        layer->retain();

        auto url = fmt::format("https://geometrydash.eu/mods/miscbugfixes/_api/importantNotices/?platform={}&version={}&loader={}&wine={}&os={}", GEODE_PLATFORM_NAME, Mod::get()->getVersion().toVString(true), Loader::get()->getVersion().toVString(true), getWineVersion(), getOSVersion());
        log::info("Fetching important notices from: {}", url);

        s_requestTask = web::WebRequest().userAgent(getUserAgent()).get(url).map(
            [layer](web::WebResponse* response) {
                if(!response->ok()) {
                    onNoticesFailed(layer, std::to_string(response->code()));
                    return *response;
                }

                auto result = response->json();
                if(!result) {
                    onNoticesFailed(layer, "Invalid JSON");
                    return *response;
                }

                auto info = result.unwrap();
                if(auto res = info["latest-geode"].asString()) {
                    resetUpdater(res.unwrap());
                }
                
                if(auto res = info["notice"].asString()) {
                    auto alert = FLAlertLayer::create(Mod::get()->getName().c_str(), res.unwrap(), "OK");
                    alert->m_scene = layer;
                    alert->show();
                    layer->release();
                }

                s_requestTask = std::nullopt;

                return *response;
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