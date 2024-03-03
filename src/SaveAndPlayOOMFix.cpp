#include <Geode/Geode.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>
#include <Geode/modify/GJGameLoadingLayer.hpp>

using namespace geode::prelude;

static Patch* s_disableSavePatch = nullptr;
static bool s_saveLater = false;

$execute {
    // Patch out GManager::save (or GManager::saveGMTo if inlined) in EditorPauseLayer::saveLevel
    // so we can call it ourselves later when there's less allocated memory
    #ifdef GEODE_IS_WINDOWS
        static_assert(GEODE_COMP_GD_VERSION == 22040, "Wrong GD version detected");
        //if(!Mod::get()->patch(reinterpret_cast<void*>(base::get() + 0xa2c51), {0x90, 0x90, 0x90, 0x90, 0x90})) {
        if(auto patch = Mod::get()->patch(reinterpret_cast<void*>(base::get() + 0xa2c20), {0xeb, 0x34})) {
            s_disableSavePatch = patch.value();
        } else {
            log::error("Failed to patch GManager::save call in EditorPauseLayer::saveLevel");
        }
    #endif
    //TODO: mac
    //TODO: android

    if(s_disableSavePatch) (void)s_disableSavePatch->disable();
}

class $modify(EditorPauseLayer) {
    void onSaveAndPlay(CCObject* sender) {
        if(s_disableSavePatch) {
            (void)s_disableSavePatch->enable();
            s_saveLater = true;
        }
        
        EditorPauseLayer::onSaveAndPlay(sender);
        if(s_disableSavePatch) (void)s_disableSavePatch->disable();
    }
};

class $modify(GJGameLoadingLayer) {
    CCLabelBMFont* m_loading = nullptr;

    static void onModify(auto& self) {
        (void) self.setHookPriority("GJGameLoadingLayer::loadLevel", 0x100000);
    }

    bool init(GJGameLevel* level, bool editor) {
        if(!GJGameLoadingLayer::init(level, editor)) return false;

        m_fields->m_loading = getChildOfType<CCLabelBMFont>(this, 0);
        if(s_saveLater && m_fields->m_loading) {
            m_fields->m_loading->setString("Saving...");
        }

        return true;
    }

    void loadLevel() {
        if(!s_saveLater) {
            GJGameLoadingLayer::loadLevel();
            return;
        }

        s_saveLater = false;
        LocalLevelManager::sharedState()->save();

        if(m_fields->m_loading) {
            m_fields->m_loading->setString("Loading...");
        }

        Loader::get()->queueInMainThread([this] {
            loadLevel();
        });
    }
};