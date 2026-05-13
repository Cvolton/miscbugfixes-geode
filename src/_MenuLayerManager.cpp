#include "_MenuLayerManager.hpp"
#include "_Utils.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

static std::vector<std::function<void(MenuLayer*)>> s_functionQueue;

void MenuLayerManager::queueFunction(std::function<void(MenuLayer*)> func) {
    s_functionQueue.push_back(func);
}

#include <Geode/modify/MenuLayer.hpp>
class $modify(MenuLayer) {
    bool init() {
        if(!MenuLayer::init()) return false;

        for(auto& func : s_functionQueue) {
            func(this);
        }
        s_functionQueue.clear();

        return true;
    }
};