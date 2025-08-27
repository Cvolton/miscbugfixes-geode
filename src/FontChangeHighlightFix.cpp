#include <Geode/Geode.hpp>
#include <Geode/modify/SelectFontLayer.hpp>

using namespace geode::prelude;

// fixes editor TextGameObject highlighting when changing fonts
class $modify(FontChangeHighlightFix, SelectFontLayer) {
    void onChangeFont(CCObject* sender) {
        SelectFontLayer::onChangeFont(sender);
        if (auto EUI = EditorUI::get()) {
            EUI->resetSelectedObjectsColor();
        }
    }
};