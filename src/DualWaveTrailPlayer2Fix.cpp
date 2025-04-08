#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>

using namespace geode::prelude;

// Global flag to enable/disable the fix
static bool s_shouldApplyWaveTrailFix = false;

$on_mod(Loaded) {
    listenForSettingChanges("other-wave-trail-fix", +[](bool value) {
        s_shouldApplyWaveTrailFix = value;
    });
}

$on_mod(DataLoaded) {
    s_shouldApplyWaveTrailFix = Mod::get()->getSettingValue<bool>("other-wave-trail-fix");
}

// Helper function to clear all points from a wave trail.
// This function assumes that the wave trail object has a public member m_pointArray
// that is a CCArray (or a similar container) with a method removeAllObjects()
void clearWaveTrailPoints(auto* waveTrail) {
    if (waveTrail && waveTrail->m_pointArray) {
        waveTrail->m_pointArray->removeAllObjects();
    }
}

class $modify(GJBaseGameLayer) {
public:
    void toggleDualMode(GameObject* portal, bool state, PlayerObject* playerTouchingPortal, bool p4) {
        if (s_shouldApplyWaveTrailFix && !state && playerTouchingPortal == m_player2) {
            if (m_player2->m_isDart) {
                // Save the points from player2's wave trail
                std::vector<CCPoint> points;
                for (auto pointNode : CCArrayExt<PointNode*>(m_player2->m_waveTrail->m_pointArray)) {
                    points.push_back(pointNode->m_point);
                }
                // Instead of calling reset(), clear the points manually.
                clearWaveTrailPoints(m_player2->m_waveTrail);
                // Call the original toggleDualMode to update mode.
                GJBaseGameLayer::toggleDualMode(portal, state, playerTouchingPortal, p4);
                // Clear player1's wave trail before restoring points.
                clearWaveTrailPoints(m_player1->m_waveTrail);
                // Restore the saved points into player1's wave trail.
                for (auto& point : points) {
                    m_player1->m_waveTrail->addPoint(point);
                }
            } else {
                GJBaseGameLayer::toggleDualMode(portal, state, playerTouchingPortal, p4);
                // Simply clear player1's wave trail without restoring any points.
                clearWaveTrailPoints(m_player1->m_waveTrail);
            }
        } else {
            // Default behavior if the fix is disabled or conditions don't match.
            GJBaseGameLayer::toggleDualMode(portal, state, playerTouchingPortal, p4);
        }
    }
};
