#include <Geode/Geode.hpp>

#include "GarageIconCountMultipleOf36Fix.hpp"

using namespace geode::prelude;

void HookedGJGarageLayer::onModify(auto& self) {
  // this is a reimplementation to fix a bug with robtop improperly calculating the pages, make sure it runs last
  Result<> result = self.setHookPriority("GJGarageLayer::onArrow", Priority::Replace + 1);
  if (!result) log::error("Failed to set hook priority, arrows may not work correctly.");
}

void HookedGJGarageLayer::onArrow(CCObject *sender) {
  CCMenuItemSpriteExtra *arrow = static_cast<CCMenuItemSpriteExtra *>(sender);
  // robtop forgot to subtract 1 here, that is the only change.
  // if the icon count is, say, 36, robtop would just divide by 36,
  // and get that the max page is 1, when, in fact, 36 icons fit onto page 0
  // so there would be a page 1 that is just blank
  int maxPage = (GameManager::get()->countForType(m_iconType) - 1)/36;
  int page = m_iconPages[m_iconType] + arrow->getTag();
  page = page > maxPage ? 0 : page < 0 ? maxPage : page;
  GJGarageLayer::setupPage(page, m_iconType);
}
