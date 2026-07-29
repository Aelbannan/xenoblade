#include "kyoshin/cf/chain/CChainCombo.hpp"
#include "kyoshin/cf/object/CfObjectActor.hpp"
#include <ml_math.h>

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

// Arts category byte loaded from the actor's sub-object.
struct CChainCombo_ArtsCategory {
    u8 pad[0x3e];
    u8 mArtsCategory; // 0x3e
};

// Sub-object at actor+0x50 that holds the arts category.
struct CChainCombo_ArtsCategoryHolder {
    u8 pad[0x50];
    CChainCombo_ArtsCategory* mArtsCategoryPtr; // 0x50
};

void func_80293E24(cf::CChainCombo* self, cf::CfObjectActor* actor) {
    // Virtual call at vtable[0x2a4] returns a pointer to a sub-object.
    CChainCombo_ArtsCategoryHolder* holder =
        (CChainCombo_ArtsCategoryHolder*)actor->CActorParam_UnkVirtualFunc132();
    u8 newArtsType = holder->mArtsCategoryPtr->mArtsCategory;

    // Reset combo count if arts type changed (but not to/from 8).
    int oldArtsType = self->mArtsType;
    int resetCombo;
    if (newArtsType == 8) {
        resetCombo = 0;
    } else if (oldArtsType == 8) {
        resetCombo = 0;
    } else {
        // (old - new) | (new - old) >> 31 — nonzero iff old != new.
        resetCombo = (int)((u32)(oldArtsType - newArtsType) | (u32)(newArtsType - oldArtsType)) >> 31;
    }
    if (resetCombo != 0) {
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
    if (!self->mPending) {
        return;
    }

    // Virtual call on the sub-object at actor+0x3e9c (vtable[0x4c]).
    int result = actor->field_0x3e9c->vtable[0x4c](actor->field_0x3e9c);
    void* obj = func_800B708C(func_8016FE34(result));
    if (obj == nullptr) {
        self->mPending = false;
        return;
    }

    // Random selection from a 3-entry table based on probability thresholds.
    int rand = ml::math::mtRand(100);
    int entry;
    if (rand < 5) {
        entry = 0;
    } else if (rand < 25) {
        entry = 1;
    } else {
        entry = 2;
    }
    int value = lbl_eu_80538988[entry];

    // Virtual call at vtable[0x184] on the object.
    obj->vtable[0x184](obj, value);
    func_802A07F4(0xbf, obj);

    self->mPending = false;
}
