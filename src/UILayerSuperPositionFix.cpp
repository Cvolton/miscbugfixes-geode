#include <Geode/Geode.hpp>
#include <Geode/modify/CCNode.hpp>

using namespace geode::prelude;

class $modify(CCNode) {

	void addChild(CCNode* child, int zOrder, int tag) {
		if (typeinfo_cast<EndLevelLayer*>(child)) {
			zOrder = 1000;
		}

		CCNode::addChild(child, zOrder, tag);
	}

};