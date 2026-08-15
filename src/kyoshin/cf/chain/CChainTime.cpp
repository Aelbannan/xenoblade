// CChainTime - chain time management

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/chain/CChainTime.hpp"
#include "kyoshin/cf/CBattleManager.hpp"

extern const float lbl_eu_80668A88;
extern const float lbl_eu_80668A8C;

cf::CChainTime::CChainTime() {
    mTimer = lbl_eu_80668A88;
    mEnabled = 0;
    mLoop = 1;
    mPaused = 1;
}

void cf::CChainTime::func_8027CE30() {
    cf::CBattleManager* bm = cf::CBattleManager::getInstance();
    func_800EA484(bm, lbl_eu_80668A88, 0x13);

    int effId = (mLoop != 0) ? 0xB4 : 0xBB;
    func_802A0950(&mChainEffect, 0, effId, 0, 0, 0);

    mTimer = lbl_eu_80668A88;
    mEnabled = 0;
    mLoop = 1;
    mPaused = 1;
}

extern "C" void func_8027CEB0(cf::CChainTime* self, u8 val) {
    cf::CBattleManager* bm = cf::CBattleManager::getInstance();
    func_800EA484(bm, lbl_eu_80668A88, 0x13);

    int effId = (self->mLoop != 0) ? 0xB4 : 0xBB;
    func_802A0950(&self->mChainEffect, 0, effId, 0, 0, 0);

    self->mTimer = lbl_eu_80668A88;
    self->mEnabled = 0;
    self->mPaused = 1;
    self->mLoop = val;
}

extern "C" void func_8027CF3C(cf::CChainTime* self) {
    f32 timer = self->mTimer;

    if (lbl_eu_80668A88 != timer) {
        if (self->mEnabled != 0) {
            // Retail compares against lbl_eu_80668A8C (0.0f) here, not a -1.0f
            // literal — using the extern keeps the lfs reloc on the retail name
            // and avoids pooling a TU-local float constant.
            if (timer != lbl_eu_80668A8C || self->mChainEffect.unk4 != 0) {
                timer = self->mTimer;
                if (timer <= lbl_eu_80668A8C) {
                    func_802A0818(0xB8, 0);
                    func_802A0818(0xC1, 0);
                    func_802A0818(0xC2, 0);
                    func_802A0818(0xC9, 0);
                    func_802A0818(0xCA, 0);
                }
                cf::CBattleManager* bm = cf::CBattleManager::getInstance();
                func_800EA484(bm, timer, 0x13);
            }
        } else {
            if (timer <= lbl_eu_80668A8C) {
                func_802A0818(0xB8, 0);
                func_802A0818(0xC1, 0);
                func_802A0818(0xC2, 0);
                func_802A0818(0xC9, 0);
                func_802A0818(0xCA, 0);
            }
            cf::CBattleManager* bm = cf::CBattleManager::getInstance();
            func_800EA484(bm, timer, 0x13);
        }

        int effId = (self->mLoop != 0) ? 0xB4 : 0xBB;
        func_802A0950(&self->mChainEffect, self->mEnabled, effId, 0, 0, 0);
    } else {
        if (self->mPaused != 0) {
            cf::CBattleManager* bm = cf::CBattleManager::getInstance();
            func_800EA484(bm, lbl_eu_80668A88, 0x13);
        }

        int effId = (self->mLoop != 0) ? 0xB4 : 0xBB;
        func_802A0950(&self->mChainEffect, 0, effId, 0, 0, 0);
    }

    self->mPaused = 0;
}
