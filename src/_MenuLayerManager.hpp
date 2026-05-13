#pragma once

#include <Geode/Geode.hpp>

namespace MenuLayerManager {
    void queueFunction(std::function<void(MenuLayer*)> func);
}