#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include "_MenuLayerManager.hpp"

using namespace geode::prelude;

// the following code would kill a victorian child

class ModPopup : public CCNode {};

bool isCorrectDownloadSprite(CCNode* node) {
    if(!node->getParent()) return false;

    if(!typeinfo_cast<geode::IconButtonSprite*>(node->getParent())) return false;

    if(!typeinfo_cast<CCMenuItem*>(node->getParent()->getParent())) return false;

    if(auto label = node->getParent()->getChildByType<CCLabelBMFont>(0)) {
        return std::string_view(label->getString()) == "Install";
    }

    return false;
}

CCNode* getChildBySpriteFrameNameRecursive(CCNode* node, const char* name) {
    if(auto sprite = geode::cocos::getChildBySpriteFrameName(node, name)) {
        if(isCorrectDownloadSprite(sprite)) return sprite;
    }

    for(auto child : CCArrayExt<CCNode*>(node->getChildren())) {
        if(auto sprite = getChildBySpriteFrameNameRecursive(child, name)) {
            if(isCorrectDownloadSprite(sprite)) return sprite;
        }
    }

    return nullptr;
}

static Ref<CCNode> s_popup = nullptr;

void showDelayedPopup(const std::string& mod) {
    std::thread([] {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        Loader::get()->queueInMainThread([] {
            geode::openInfoPopup("thesillydoggo.comment_emojis").listen([](bool sth){
                if(sth) {
                    Notification::create("  Please uninstall the mod and\n  then reinstall from the index\n  to get the fixed version.", NotificationIcon::Info, NOTIFICATION_LONG_TIME)->show();
                }

                return sth;
            });
        });
    }).detach();
}

$on_mod(Loaded) {
    MenuLayerManager::queueFunction([](MenuLayer *layer) {
        if(Mod::get()->getSavedValue<bool>("skipped-comment-emoji-fix", false)) return;

        if(auto mod = Loader::get()->getLoadedMod("thesillydoggo.comment_emojis")) if(mod->getVersion() == VersionInfo::parse("1.2.1").unwrapOrDefault()) {
            Loader::get()->queueInMainThread([] {
                geode::createQuickPopup("Misc Bugfixes", "Broken version of Emoji in Comments mod detected.\nSwitch to a fixed version?", "No", "Yes", 400.f, [](auto, bool btn2) {
                    if(!btn2) {
                        Mod::get()->setSavedValue("skipped-comment-emoji-fix", true);
                        return;
                    }

                    Loader::get()->queueInMainThread([] {
                        geode::openModsList();
                        std::thread([]{
                            //std::this_thread::sleep_for(std::chrono::seconds(1));
                            Loader::get()->queueInMainThread([] {
                                geode::openInfoPopup("thesillydoggo.comment_emojis").listen([](bool sth){
                                    if(!sth) {
                                        Notification::create("Failed to open Emoji in Comments mod info", NotificationIcon::Error)->show();
                                        if(s_popup->getParent()) s_popup->removeFromParent();
                                        s_popup = nullptr;
                                        return sth;
                                    }

                                    Loader::get()->queueInMainThread([] {
                                        if(auto sprite = getChildBySpriteFrameNameRecursive(s_popup, "GJ_downloadsIcon_001.png")) {
                                            if(auto btn = typeinfo_cast<CCMenuItem*>(sprite->getParent()->getParent())) {
                                                btn->activate();
                                            } else {
                                                showDelayedPopup("thesillydoggo.comment_emojis");
                                            }
                                        } else {
                                            showDelayedPopup("thesillydoggo.comment_emojis");
                                        }

                                        s_popup = nullptr;
                                    });

                                    return sth;
                                });
                                
                                s_popup = CCScene::get()->getChildByType<ModPopup*>(0);
                                if(s_popup) s_popup->setVisible(false);
                            });
                        }).detach();
                    });
                });
            });
        }
    });
}