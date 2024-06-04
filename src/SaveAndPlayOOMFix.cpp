//TODO: version specific patches
/*#include <Geode/Geode.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>
#include <Geode/modify/GJGameLoadingLayer.hpp>
#include <Geode/modify/GameManager.hpp>
#include "_Utils.hpp"

using namespace geode::prelude;

static Patch* s_disableSavePatch = nullptr;
static bool s_saveLater = false;

$execute {
    // Patch out GManager::save (or GManager::saveGMTo if inlined) in EditorPauseLayer::saveLevel
    // so we can call it ourselves later when there's less allocated memory
    #ifdef GEODE_IS_WINDOWS
        static_assert(GEODE_COMP_GD_VERSION == 22040, "Wrong GD version detected");
        auto result = patch(
            0xa2c20,
            {0x75, 0x34},
            {0xeb, 0x34}
        );
    #elif defined(GEODE_IS_MACOS)
        static_assert(GEODE_COMP_GD_VERSION == 22000, "Wrong GD version detected");
        auto result = patch(
            0x2729ce,
            {0xe8, 0x7d, 0x57, 0x21, 0x00},
            {0x90, 0x90, 0x90, 0x90, 0x90}
        );
    #elif defined(GEODE_IS_ANDROID32)
        static_assert(GEODE_COMP_GD_VERSION == 22050, "Wrong GD version detected");
        auto result = patch(
            0x3E9020,
            {0xfa, 0xf6, 0xa6, 0xfc},
            {0x00, 0xBF, 0x00, 0xBF}
        );
    #elif defined(GEODE_IS_ANDROID64)
        static_assert(GEODE_COMP_GD_VERSION == 22050, "Wrong GD version detected");
        auto result = patch(
            0x765FE8,
            {0xB2, 0x35, 0xF8, 0x97},
            {0x1F, 0x20, 0x03, 0xD5}
        );
    #else
        static_assert(false, "Unsupported platform");
    #endif

    if(result.isOk()) {
        s_disableSavePatch = result.value();
    } else {
        log::error("Failed to patch GManager::save call in EditorPauseLayer::saveLevel: {}", result.err());
    }

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

    void onSaveAndExit(CCObject* sender) {
        if(s_disableSavePatch) {
            (void)s_disableSavePatch->enable();
            s_saveLater = true;
        }

        EditorPauseLayer::onSaveAndExit(sender);
        if(s_disableSavePatch) (void)s_disableSavePatch->disable();
    }
};

class SaveGameLayer : public CCLayer {
public:
    GJGameLevel* m_level = nullptr;

    static CCScene* scene(GJGameLevel* level) {
        auto scene = CCScene::create();
        auto layer = SaveGameLayer::create(level);
        if(layer) {
            scene->addChild(layer);
            return scene;
        }
        return nullptr;
    }

    static SaveGameLayer* create(GJGameLevel* level) {
        auto ret = new SaveGameLayer();
        if(ret && ret->init(level)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool init(GJGameLevel* level) {
        if(!CCLayer::init()) return false;

        m_level = level;
        m_level->retain();

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        auto title = CCLabelBMFont::create("Saving...", "bigFont.fnt");
        title->setPosition({ 0, 50 });
        title->setAnchorPoint({1,0});
        title->setPosition({winSize.width - 20, 20});
        addChild(title);

        return true;
    }

    void onEnterTransitionDidFinish() {
        CCLayer::onEnterTransitionDidFinish();
        Loader::get()->queueInMainThread([this] {
            LocalLevelManager::sharedState()->save();
            s_saveLater = false;
            Loader::get()->queueInMainThread([this] {
                GameManager::sharedState()->returnToLastScene(m_level);
                m_level->release();
            });
        });
    }
};

class $modify(GameManager) {
    void returnToLastScene(GJGameLevel* level) {
        if(s_saveLater) {
            auto scene = SaveGameLayer::scene(level);
            if(scene) {
                CCDirector::sharedDirector()->replaceScene(scene);
                return;
            }
        }

        GameManager::returnToLastScene(level);
    }
};

class $modify(MBGJGameLoadingLayer, GJGameLoadingLayer) {
    struct Fields {
        CCLabelBMFont* m_loading = nullptr;
    }

    static void onModify(auto& self) {
        (void) self.setHookPriority("GJGameLoadingLayer::loadLevel", 0x100000);
    }

    static GJGameLoadingLayer* transitionToLoadingLayer(GJGameLevel* level, bool editor) {
        auto layer = static_cast<MBGJGameLoadingLayer>(GJGameLoadingLayer::transitionToLoadingLayer(level, editor));

        layer->m_fields->m_loading = getChildOfType<CCLabelBMFont>(this, 0);
        if(s_saveLater && layer->m_fields->m_loading) {
            layer->m_fields->m_loading->setString("Saving...");
        }

        return true;
    }

    /*bool init(GJGameLevel* level, bool editor) {
        if(!GJGameLoadingLayer::init(level, editor)) return false;

        m_fields->m_loading = getChildOfType<CCLabelBMFont>(this, 0);
        if(s_saveLater && m_fields->m_loading) {
            m_fields->m_loading->setString("Saving...");
        }

        return true;
    }*/

/*    void loadLevel() {
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
};*/
