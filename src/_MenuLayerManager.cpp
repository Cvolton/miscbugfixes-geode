#include "_MenuLayerManager.hpp"
#include "_Utils.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

static std::vector<std::function<void(MenuLayer*)>> s_functionQueue;
static std::vector<std::function<void(MenuLayer*)>> s_functionQueueInThread;

void MenuLayerManager::queueFunction(std::function<void(MenuLayer*)> func) {
    s_functionQueue.push_back(func);
}

void MenuLayerManager::queueFunctionInThread(std::function<void(MenuLayer*)> func) {
    s_functionQueueInThread.push_back(func);
}

#include <Geode/modify/MenuLayer.hpp>
class $modify(MenuLayer) {
    bool init() {
        if(!MenuLayer::init()) return false;

        for(auto& func : s_functionQueue) {
            func(this);
        }
        s_functionQueue.clear();

        if(MiscBugfixes::isAmazon()) {
            auto alert = FLAlertLayer::create(
                "Amazon Bug",
                "This version of Geometry Dash is not supported by Geode. "
                "Please use the official version from the Google Play Store.",
                "OK"
            );
            alert->m_scene = this;
            alert->show();
        }

        //disabled for now
        /*std::thread([this] {
            thread::setName("MenuLayerManager");

            for(auto& func : s_functionQueueInThread) {
                func(this);
            }
            s_functionQueueInThread.clear();
        }).detach();*/

        return true;
    }
};