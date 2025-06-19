#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "_Utils.hpp"

using namespace geode::prelude;

STATIC_BOOL_SETTING(shouldApplyWaveTrailFix, other-wave-trail-fix);

class $modify(GJBaseGameLayer) {

    void toggleDualMode(GameObject* portal, bool state, PlayerObject* playerTouchingPortal, bool p4) {
        if (s_shouldApplyWaveTrailFix && !state && playerTouchingPortal == m_player2) {
            if (m_player2->m_isDart) {
                std::vector<CCPoint> points;
                for(auto pointNode : CCArrayExt<PointNode*>(m_player2->m_waveTrail->m_pointArray)) {
                    points.push_back(pointNode->m_point);
                }
                // the game will fade both of these trails out, blank the player2 trail so it's invisible
                m_player2->m_waveTrail->reset();
                GJBaseGameLayer::toggleDualMode(portal, state, playerTouchingPortal, p4);
                // now add back the points from the original player 2 trail
                m_player1->m_waveTrail->reset();
                for (auto& point : points) {
                    m_player1->m_waveTrail->addPoint(point);
                }
            } else {
                GJBaseGameLayer::toggleDualMode(portal, state, playerTouchingPortal, p4);
                m_player1->m_waveTrail->reset();
            }
        } else {
            GJBaseGameLayer::toggleDualMode(portal, state, playerTouchingPortal, p4);
        }
    }

};
