// CChainTime - chain time management

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/chain/CChainTime.hpp"
#include "kyoshin/cf/CBattleManager.hpp"

extern "C" void func_800EA484(cf::CBattleManager*, f32, int);
extern "C" void func_802A0818(int, int);

cf::CChainTime::CChainTime() {
    mTimer = 0.0f;
    mEnabled = 0;
    mLoop = 1;
    mPaused = 1;
}

cf::CChainTime::~CChainTime() {
    func_8027CE30();
}

void cf::CChainTime::func_8027CE30() {
    cf::CBattleManager* bm = cf::CBattleManager::getInstance();
    func_800EA484(bm, 0.0f, 0x13);
    
    int effId = (mLoop != 0) ? 0xB4 : 0xBB;
    func_802A0950(&mChainEffect, 0, effId, 0, 0, 0);
    
    mTimer = 0.0f;
    mEnabled = 0;
    mLoop = 1;
    mPaused = 1;
}

extern "C" void func_8027CEB0(cf::CChainTime* self, u8 val) {
    cf::CBattleManager* bm = cf::CBattleManager::getInstance();
    func_800EA484(bm, 0.0f, 0x13);
    
    int effId = (self->mLoop != 0) ? 0xB4 : 0xBB;
    func_802A0950(&self->mChainEffect, 0, effId, 0, 0, 0);
    
    self->mTimer = 0.0f;
    self->mEnabled = 0;
    self->mPaused = 1;
    self->mLoop = val;
}

extern "C" void func_8027CF3C(cf::CChainTime* self) {
    f32 timer = self->mTimer;
    
    if (timer != 0.0f) {
        if (self->mEnabled != 0) {
            if (-1.0f != timer || self->mChainEffect.unk4 != 0) {
                timer = self->mTimer;
                if (timer <= -1.0f) {
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
            if (timer <= -1.0f) {
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
            func_800EA484(bm, 0.0f, 0x13);
        }
        
        int effId = (self->mLoop != 0) ? 0xB4 : 0xBB;
        func_802A0950(&self->mChainEffect, 0, effId, 0, 0, 0);
    }
    
    self->mPaused = 0;
}
