#include "kyoshin/cf/chain/CChainCombo.hpp"
#include "kyoshin/cf/object/CfObjectActor.hpp"
#include <monolib/math/Random.hpp>

namespace cf{
    CChainCombo::CChainCombo(){
        mVtbl = lbl_eu_80538994;
        initChainGauge__FPv(&mGauge);
        mArtsType = 0;
        mComboCount = 0;
        mPending = false;
        resetChainGauge__FPv(&mGauge);
        requestCancelChain__Fv();
    }

    void CChainCombo::func1(){
        mArtsType = 0;
        mComboCount = 0;
        mPending = false;
        resetChainGauge__FPv(&mGauge);
        requestCancelChain__Fv();
    }
}

void func_80293E24(cf::CChainCombo* self, cf::CfObjectActor* actor) {
    // Call vtable[0x2a4] on actor, get a pointer to a sub-object.
    CChainCombo_ArtsCategoryHolder* holder =
        (CChainCombo_ArtsCategoryHolder*)actor->CActorParam_UnkVirtualFunc132();
    CChainCombo_ArtsCategory* category = (CChainCombo_ArtsCategory*)holder->mArtsCategory;
    int newArtsType = category->mArtsCategory;
    int oldArtsType = self->mArtsType;

    // Reset combo count if arts type changed (but not to/from 8).
    if (newArtsType == 8) {
        oldArtsType = 0;
    } else if (oldArtsType == 8) {
        oldArtsType = 0;
    } else {
        oldArtsType = ((u32)(oldArtsType - newArtsType) | (u32)(newArtsType - oldArtsType)) >> 31;
    }
    if (oldArtsType != 0) {
        self->mComboCount = 0;
    }

    self->mArtsType = newArtsType;
    func_8013EAB0();

    if (self->mComboCount < 5) {
        self->mComboCount++;
        func_80294844(&self->mGauge, (float)self->mComboCount);
    }
}

void func_80293EEC(cf::CChainCombo* self, cf::CfObjectActor* actor) {
    if (self->mPending != 0) {
        // Call vtable[0x4c] on the +0x3e9c CfObjectMove sub-object, then chain
        // its id through findObjectById -> func_8016FE34.
        CChainVObj* vobjRaw = (CChainVObj*)func_8016FE34(findObjectById(
            ((CChainCombo_Vt4CIf*)((u8*)actor + 0x3E9C))->m4C()));

        if (vobjRaw != nullptr) {
            // Random selection from a 3-entry table based on probability thresholds.
            int rand = ml::math::mtRand(100);
            int value;
            if (rand < 5) {
                value = lbl_eu_80538988[0];
            } else if (rand < 25) {
                value = lbl_eu_80538988[1];
            } else {
                value = lbl_eu_80538988[2];
            }

            // Call vtable[0x184] (CfObjectModel_UnkVirtualFunc4) on the object.
            ((CChainCombo_Vt184If*)vobjRaw)->m184(value);
            func_802A07F4(0xbf, vobjRaw);
        }
    }
    // Volatile final store: makes MWCC schedule the epilogue with the LR
    // restore first (retail order).
    volatile bool* pending = &self->mPending;
    *pending = false;
}
