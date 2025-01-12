#include <Geode/Geode.hpp>
#include "_MenuLayerManager.hpp"

using namespace geode::prelude;

$on_mod(Loaded) {
    MenuLayerManager::queueFunction([](MenuLayer *layer) {
        auto GM = GameManager::sharedState();
        if(GM->getUGV("14") && GM->getUGV("15") && GM->getUGV("16") && !GM->getUGV("13")) {
            Loader::get()->queueInMainThread([layer, GM] {
                auto popup = createQuickPopup(
                    "Misc Bugfixes",
                    "Basement monster softlock detected!\nYou have collected all keys needed to free the basement monster but it's still stuck.\nFix the issue?",
                    "No",
                    "Yes",
                    400,
                    [GM](FLAlertLayer* me, bool btn2) {
                        if(btn2) {
                            GM->setUGV("13", 1);
                        }
                    },
                    false
                );
                popup->m_scene = layer;
                popup->show();
            });
        }
    });
}