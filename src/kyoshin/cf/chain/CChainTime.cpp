// CChainTime - chain time management

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/chain/CChainTime.hpp"
#include "kyoshin/cf/chain/CChainEffect.hpp"

#include "kyoshin/cf/CBattleManager.hpp"
#include "kyoshin/cf/chain/CChainImports.hpp"
#include "kyoshin/cf/CVision.hpp"

extern const float lbl_eu_80668A88;
extern const float lbl_eu_80668A8C;

cf::CChainTime::CChainTime() {
    mTimer = lbl_eu_80668A88;
    mEnabled = 0;
    mLoop = 1;
    mPaused = 1;
}

void cf::CChainTime::resetChainTime() {
    cf::CBattleManager* bm = cf::CBattleManager::getInstance();
    refreshBattleActors(bm, lbl_eu_80668A88, 0x13);

    int effId = (mLoop != 0) ? 0xB4 : 0xBB;
    chainBindEffectLink(&mChainEffect, 0, effId, 0, 0, 0);

    mTimer = lbl_eu_80668A88;
    mEnabled = 0;
    mLoop = 1;
    mPaused = 1;
}

extern "C" void startChainTimer(cf::CChainTime* self, u8 val) {
    cf::CBattleManager* bm = cf::CBattleManager::getInstance();
    refreshBattleActors(bm, lbl_eu_80668A88, 0x13);

    int effId = (self->mLoop != 0) ? 0xB4 : 0xBB;
    chainBindEffectLink(&self->mChainEffect, 0, effId, 0, 0, 0);

    self->mTimer = lbl_eu_80668A88;
    self->mEnabled = 0;
    self->mPaused = 1;
    self->mLoop = val;
}

extern "C" void tickChainTimer(cf::CChainTime* self) {
    f32 timer = self->mTimer;

    if (lbl_eu_80668A88 != timer) {
        if (self->mEnabled != 0) {
            // Retail compares against lbl_eu_80668A8C (0.0f) here, not a -1.0f
            // literal -- using the extern keeps the lfs reloc on the retail name
            // and avoids pooling a TU-local float constant.
            if (timer != lbl_eu_80668A8C || self->mChainEffect.unk4 != 0) {
                timer = self->mTimer;
                if (timer <= lbl_eu_80668A8C) {
                    chainUnbindMatchingObjects(0xB8, 0);
                    chainUnbindMatchingObjects(0xC1, 0);
                    chainUnbindMatchingObjects(0xC2, 0);
                    chainUnbindMatchingObjects(0xC9, 0);
                    chainUnbindMatchingObjects(0xCA, 0);
                }
                cf::CBattleManager* bm = cf::CBattleManager::getInstance();
                refreshBattleActors(bm, timer, 0x13);
            }
        } else {
            if (timer <= lbl_eu_80668A8C) {
                chainUnbindMatchingObjects(0xB8, 0);
                chainUnbindMatchingObjects(0xC1, 0);
                chainUnbindMatchingObjects(0xC2, 0);
                chainUnbindMatchingObjects(0xC9, 0);
                chainUnbindMatchingObjects(0xCA, 0);
            }
            cf::CBattleManager* bm = cf::CBattleManager::getInstance();
            refreshBattleActors(bm, timer, 0x13);
        }

        int effId = (self->mLoop != 0) ? 0xB4 : 0xBB;
        chainBindEffectLink(&self->mChainEffect, self->mEnabled, effId, 0, 0, 0);
    } else {
        if (self->mPaused != 0) {
            cf::CBattleManager* bm = cf::CBattleManager::getInstance();
            refreshBattleActors(bm, lbl_eu_80668A88, 0x13);
        }

        int effId = (self->mLoop != 0) ? 0xB4 : 0xBB;
        chainBindEffectLink(&self->mChainEffect, 0, effId, 0, 0, 0);
    }

    self->mPaused = 0;
}

// Retail .sdata2 (this TU's split 0x80668A88-0x80668A90): the two float
// constants the timer code loads (1.0f default, 0.0f compare). Defined
// here, not in another TU: no other TU owns them. Kept at file bottom so
// the earlier extern-view coloring of tickChainTimer is preserved.
const float lbl_eu_80668A88 = 1.0f;
const float lbl_eu_80668A8C = 0.0f;
