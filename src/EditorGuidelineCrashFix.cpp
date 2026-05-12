#include <Geode/Geode.hpp>
#include <Geode/modify/DrawGridLayer.hpp>

class $modify(DrawGridLayer) {
    void loadTimeMarkers(gd::string markersParam) {
        #ifdef GEODE_IS_ANDROID
            std::string markers = markersParam;
        #else
            std::string& markers = markersParam;
        #endif
        
        if (!markers.empty()) {
            auto count = std::count(markers.begin(), markers.end(), '~');
            auto leading = !markers.empty() && markers.back() == '~';
            if (count % 2 == 0) markers += "~0";
            else if (leading) markers += "0";
        }
        DrawGridLayer::loadTimeMarkers(markers);
    }
};