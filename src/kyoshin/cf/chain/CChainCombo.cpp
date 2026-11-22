#include "kyoshin/cf/chain/CChainCombo.hpp"
#include "kyoshin/cf/object/CfObjectActor.hpp"
#include <monolib/math/Random.hpp>
#include "kyoshin/cf/chain/CChainEffect.hpp"

// name-guess: three-entry second-gauge payout table named from payoutChainCombo.
static int sChainComboPayoutValues[3] = {0x708, 0x4b0, 0x258};

namespace cf{
    CChainCombo::CChainCombo(){
        // (implicit vptr store to __vt__Q22cf11CChainCombo at +0x14)
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

void updateChainCombo(cf::CChainCombo* self, cf::CfObjectActor* actor) {
    // Call vtable[0x2a4] on actor, get a pointer to a sub-object.
    int newArtsType = ((CChainCombo_ArtsCategoryHolder*)actor->CActorParam_getMoveRecord())
                          ->mArtsCategory->mArtsCategory;
    int oldArtsType = self->mArtsType;

    // Reset combo count if arts type changed (but not to/from 8).
    bool changed;
    if (newArtsType == 8) {
        changed = false;
    } else if (oldArtsType == 8) {
        changed = false;
    } else {
        changed = oldArtsType != newArtsType;
    }
    if (changed) {
        self->mComboCount = 0;
    }

    self->mArtsType = newArtsType;
    UIWin_CreateAA2A0Win();

    if (self->mComboCount < 5) {
        self->mComboCount++;
        setChainGauge(&self->mGauge, (float)self->mComboCount);
    }
}

void payoutChainCombo(cf::CChainCombo* self, cf::CfObjectActor* actor) {
    if (self->mPending != 0) {
        // Slot +0x4C is CObjectParam_getSelfObjectId on the CfObjectMove
        // sub-object at actor+0x3E9C (same shape as CAIAction aiMoveBaseVt4C);
        // its id result resolves through findObjectById -> MapFx_ResolveOwner to
        // a CActorParam whose slot +0x184 is CActorParam_addSecondGauge.
        cf::CActorParam* vobj = (cf::CActorParam*)MapFx_ResolveOwner(findObjectById(
            ((cf::CObjectParam*)((u8*)actor + 0x3E9C))->CObjectParam_getSelfObjectId()));

        if (vobj != nullptr) {
            // Random selection from a 3-entry table based on probability thresholds.
            int rand = ml::math::mtRand(100);
            int value;
            if (rand < 5) {
                value = sChainComboPayoutValues[0];
            } else if (rand < 25) {
                value = sChainComboPayoutValues[1];
            } else {
                value = sChainComboPayoutValues[2];
            }

            // Slot +0x184 is CActorParam_addSecondGauge (takes the id).
            vobj->CActorParam_addSecondGauge(value);
            chainResolveMemberPtr(0xbf, (int)vobj);
        }
    }
    // The final store makes MWCC schedule the epilogue with the LR
    // restore first (retail order).
    bool* pending = &self->mPending;
    *pending = false;
}
