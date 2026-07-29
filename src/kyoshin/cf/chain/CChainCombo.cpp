#include "kyoshin/cf/chain/CChainCombo.hpp"
#include "kyoshin/cf/object/CfObjectActor.hpp"
#include <monolib/math/Random.hpp>

// Forward declarations for functions called by the two decompiled functions.
extern "C" void func_8013EAB0();
extern "C" void func_802A07F4(int, void*);

// Function pointer type for CfObjectModel_UnkVirtualFunc4 (vtable[0x184]).
// Header declares it with no args, but retail passes an int.
typedef void (*CfObjectModel_UnkVirtualFunc4_t)(cf::CfObjectModel*, int);

namespace cf{
    CChainCombo::CChainCombo(){
        func_80294824__FPv(&mGauge);
        mArtsType = 0;
        mComboCount = 0;
        mPending = false;
        func_80294834__FPv(&mGauge);
        func_802AA338__Fv();
    }

    void CChainCombo::func1(){
        mArtsType = 0;
        mComboCount = 0;
        mPending = false;
        func_80294834__FPv(&mGauge);
        func_802AA338__Fv();
    }
}

void func_80293E24(cf::CChainCombo* self, cf::CfObjectActor* actor) {
    // Call vtable[0x2a4] on actor, get a pointer to a sub-object.
    CChainCombo_ArtsCategoryHolder* holder =
        (CChainCombo_ArtsCategoryHolder*)actor->CActorParam_UnkVirtualFunc132();
    // Load old arts type first (while r3 still holds category ptr),
    // then load new arts type into r3.
    int oldArtsType = self->mArtsType;
    int newArtsType = ((CChainCombo_ArtsCategory*)holder->mArtsCategory)->mArtsCategory;

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
    if (self->mPending) {
        // Call vtable[0x4c] (CObjectParam_UnkVirtualFunc5) on the CfObjectMove sub-object at actor+0x3e9c.
        cf::CfObjectMove* moveObj = static_cast<cf::CfObjectMove*>(actor);
        func_800B708C(moveObj->CObjectParam_UnkVirtualFunc5());

        CChainVObj* vobj = (CChainVObj*)func_8016FE34();
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

            // Call vtable[0x184] (CfObjectModel_UnkVirtualFunc4) on the object.
            CfObjectModel_UnkVirtualFunc4_t func = (CfObjectModel_UnkVirtualFunc4_t)vobj->mVtbl[0x184 / 4];
            func((cf::CfObjectModel*)vobj, value);
            func_802A07F4(0xbf, vobj);
        }
    }
    self->mPending = false;
}
