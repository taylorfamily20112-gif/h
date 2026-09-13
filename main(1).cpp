#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify(MoreCheckpointsPlayLayer, PlayLayer) {
    struct Fields {
        float m_timeSinceLastCheckpoint = 0.f;
        float m_logTimer = 0.f;
    };

    // Reset our timer whenever the level (re)starts, so checkpoints
    // don't fire immediately after a fresh attempt.
    void resetLevel() {
        PlayLayer::resetLevel();
        m_fields->m_timeSinceLastCheckpoint = 0.f;
    }

    void update(float dt) {
        PlayLayer::update(dt);

        bool enabled = Mod::get()->getSettingValue<bool>("enabled");
        bool dead = m_player1 && m_player1->m_isDead;
        auto interval = Mod::get()->getSettingValue<double>("checkpoint-interval");

        // Debug logging: prints roughly once a second so you can check
        // Geode's console and see exactly what the mod thinks is going on.
        m_fields->m_logTimer += dt;
        if (m_fields->m_logTimer >= 1.f) {
            m_fields->m_logTimer = 0.f;
            log::info(
                "[MoreCheckpoints] enabled={} practiceMode={} dead={} interval={} timer={}",
                enabled, m_isPracticeMode, dead, interval, m_fields->m_timeSinceLastCheckpoint
            );
        }

        if (!enabled) {
            return;
        }

        // Only auto-checkpoint in practice mode. Normal mode doesn't
        // support checkpoints in vanilla GD, and this keeps the mod
        // from interfering with runs/leaderboards.
        if (!m_isPracticeMode) {
            return;
        }

        // Don't place checkpoints while the player is dead/respawning.
        if (dead) {
            return;
        }

        m_fields->m_timeSinceLastCheckpoint += dt;

        if (m_fields->m_timeSinceLastCheckpoint >= interval) {
            m_fields->m_timeSinceLastCheckpoint = 0.f;
            log::info("[MoreCheckpoints] placing checkpoint now");
            this->createCheckpoint();
            this->trimOldCheckpoints();
        }
    }

    // Keeps m_checkpointArray from growing unbounded on long/slow levels
    // if the user set a max in settings.
    void trimOldCheckpoints() {
        int maxCheckpoints = Mod::get()->getSettingValue<int64_t>("max-checkpoints");
        if (maxCheckpoints <= 0) return; // 0 == unlimited

        if (m_checkpointArray && static_cast<int>(m_checkpointArray->count()) > maxCheckpoints) {
            m_checkpointArray->removeObjectAtIndex(0);
        }
    }
};
