#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>

using namespace geode::prelude;

class $modify(GJBaseGameLayer) {

  void toggleDualMode(GameObject* portal, bool state, PlayerObject* playerTouchingPortal, bool p4) {
    if (!state && playerTouchingPortal == m_player2) {
      if (m_player2->m_isDart) {
        HardStreak* trailBackup = m_player2->m_waveTrail->createDuplicate();
        // the game will fade both of these trails out, blank the player2 trail so it's invisible
        m_player2->m_waveTrail->reset();
        GJBaseGameLayer::toggleDualMode(portal, state, playerTouchingPortal, p4);
        // now add back the points from the original player 2 trail
        m_player1->m_waveTrail->reset();
        CCArrayExt<PointNode*> pointNodeArray = trailBackup->m_pointArray;
        for (PointNode* pointNode : pointNodeArray) m_player1->m_waveTrail->addPoint(pointNode->m_point);
      } else {
        GJBaseGameLayer::toggleDualMode(portal, state, playerTouchingPortal, p4);
        m_player1->m_waveTrail->reset();
      }
    } else {
      GJBaseGameLayer::toggleDualMode(portal, state, playerTouchingPortal, p4);
    }
  }

};
