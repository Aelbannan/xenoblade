#include "kyoshin/cf/chain/CChainCombo.hpp"
#include "kyoshin/cf/object/CfObjectActor.hpp"
#include <monolib/math/Random.hpp>

// Forward declarations for functions called by the two decompiled functions.
// func_800B708C is declared in CAIAction.hpp (included transitively).
extern "C" void func_8013EAB0();
extern "C" void* func_8016FE34(void*);
extern "C" void func_802A07F4(int, void*);

// 3-entry table indexed by probability thresholds in func_80293EEC.
extern "C" int lbl_eu_80538988[3];

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

// Object returned by CActorParam_UnkVirtualFunc132 (vtable[0x2a4]).
struct CChainCombo_ArtsCategoryHolder {
    u8 pad[0x50];
    void* mArtsCategoryPtr; // 0x50
};

// Object with arts category byte at +0x3e.
struct CChainCombo_ArtsCategory {
    u8 pad[0x3e];
    u8 mArtsCategory; // 0x3e
};

// Sub-object at actor+0x3e9c (CfObjectMove base, vtable 3).
struct CChainCombo_MoveBase {
    void** mVtbl;
};

void func_80293E24(cf::CChainCombo* self, cf::CfObjectActor* actor) {
    // Call vtable[0x2a4] on actor, get a pointer to a sub-object.
    CChainCombo_ArtsCategoryHolder* holder =
        (CChainCombo_ArtsCategoryHolder*)actor->CActorParam_UnkVirtualFunc132();
    CChainCombo_ArtsCategory* category =
        (CChainCombo_ArtsCategory*)holder->mArtsCategoryPtr;
    u8 newArtsType = category->mArtsCategory;

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
        self->mPending = false;
        return;
    }

    // Call vtable[0x4c] on the sub-object at actor+0x3e9c.
    CChainCombo_MoveBase* moveBase = (CChainCombo_MoveBase*)((u8*)actor + 0x3e9c);
    int (*vfunc)(void*) = (int (*)(void*))moveBase->mVtbl[0x4c / 4];
    int result = vfunc(moveBase);

    void* obj = func_8016FE34(func_800B708C((BOOL)result));
    if (obj == nullptr) {
        self->mPending = false;
        return;
    }

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

    // Call vtable[0x184] on the object.
    CChainVObj* vobj = (CChainVObj*)obj;
    void (*vfunc2)(void*, int) = (void (*)(void*, int))vobj->mVtbl[0x184 / 4];
    vfunc2(vobj, value);
    func_802A07F4(0xbf, obj);

    self->mPending = false;
}
