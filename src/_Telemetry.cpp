#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/modify/MenuLayer.hpp>

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
                return fmt::format("Wine {}", pwine_get_version());
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

            return fmt::format("Windows {}.{}.{}", osVers.dwMajorVersion, osVers.dwMinorVersion, osVers.dwBuildNumber);
        #else
            return GEODE_PLATFORM_NAME;
        #endif
    }

    void loadImportantNotices(CCLayer* layer) {
        static bool hasBeenCalled = false;
        if(hasBeenCalled) return;
        hasBeenCalled = true;

        layer->retain();

        web::AsyncWebRequest().fetch(fmt::format("https://geometrydash.eu/mods/miscbugfixes/_api/importantNotices/?platform={}&version={}&loader={}&wine={}&os={}&notAlpha7=1", GEODE_PLATFORM_NAME, Mod::get()->getVersion().toString(true), Loader::get()->getVersion().toString(true), getWineVersion(), getOSVersion())).json().then([layer](const matjson::Value& info){
            auto notice = info.try_get("notice");
            if(notice == std::nullopt) return;
            
            if(info["notice"].is_string()) {
                auto alert = FLAlertLayer::create(Mod::get()->getName().c_str(), info["notice"].as_string(), "OK");
                alert->m_scene = layer;
                alert->show();
                layer->release();
            }
        }).expect([layer](const std::string& error){
            log::warn("Fetching important notices failed: {}", error);
            layer->release();
        });
    }
}

class $modify(MenuLayer) {
    bool init() {
        if(!MenuLayer::init()) return false;

        MiscBugfixes::loadImportantNotices(this);

        return true;
    }
};