#pragma once

#include <Geode/Geode.hpp>

#pragma warning(push)
#pragma warning(disable: 4265)  // non-virtual destructor
#include <Geode/modify/GJGarageLayer.hpp>
#pragma warning(pop)

using namespace geode::prelude;

struct HookedGJGarageLayer : Modify<HookedGJGarageLayer, GJGarageLayer> {

  static void onModify(auto&);

  $override void onArrow(CCObject*);

};
