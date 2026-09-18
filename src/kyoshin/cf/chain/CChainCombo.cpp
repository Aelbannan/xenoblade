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
        (CChainCombo_ArtsCategoryHolder*)actor->CActorParam_getMoveRecord();
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
    UIWin_CreateAA2A0Win();

    if (self->mComboCount < 5) {
        self->mComboCount++;
        setChainGauge(&self->mGauge, (float)self->mComboCount);
    }
}

void func_80293EEC(cf::CChainCombo* self, cf::CfObjectActor* actor) {
    if (self->mPending != 0) {
        // Slot +0x4C is CObjectParam_getSelfObjectId on the CfObjectMove
        // sub-object at actor+0x3E9C (same shape as CAIAction aiMoveBaseVt4C);
        // its id result resolves through findObjectById -> func_8016FE34 to
        // a CActorParam whose slot +0x184 is CActorParam_addSecondGauge.
        cf::CActorParam* vobj = (cf::CActorParam*)func_8016FE34(findObjectById(
            ((cf::CObjectParam*)((u8*)actor + 0x3E9C))->CObjectParam_getSelfObjectId()));

        if (vobj != nullptr) {
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

            // Slot +0x184 is CActorParam_addSecondGauge (takes the id).
            vobj->CActorParam_addSecondGauge(value);
            chainResolveMemberPtr(0xbf, vobj);
        }
    }
    // Volatile final store: makes MWCC schedule the epilogue with the LR
    // restore first (retail order).
    volatile bool* pending = &self->mPending;
    *pending = false;
}
