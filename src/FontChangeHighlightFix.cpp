#include <Geode/Geode.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>

using namespace geode::prelude;

// fixes editor TextGameObject highlighting when changing fonts
class $modify(FontChangeHighlightFix, LevelEditorLayer) {
    void updateLevelFont(int idx) {
        LevelEditorLayer::updateLevelFont(idx);

        if (auto EUI = EditorUI::get()) {
            EUI->resetSelectedObjectsColor();
        }
    }
};