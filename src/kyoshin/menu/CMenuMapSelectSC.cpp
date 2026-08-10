// Auto-scaffolded catalog TU for kyoshin/menu/CMenuMapSelectSC
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/menu/CMenuMapSelectSC.hpp"
#include "kyoshin/harness_catalog.hpp"

struct S {
    int dummy;
    int a;
    int b;
    int c;
    int d;
    unsigned char e;
    unsigned char f;
};

void __ct__CMenuMapSelectSC(){}

CMenuMapSelectSC::~CMenuMapSelectSC() {}

void CMenuMapSelectSC::Init() {}

void func_80251294(S* r3, const S* r4) {
    S* dst = r3;
    const S* src = r4;
    dst->a = src->a;
    dst->b = src->b;
    dst->c = src->c;
    dst->d = src->d;
    dst->e = src->e;
    dst->f = src->f;
}

void CMenuMapSelectSC::Term() {}

void CMenuMapSelectSC::Move() {}

void CMenuMapSelectSC::cbRenderBefore() {}

void func_802514D4(){}

void func_80251550(){}

// World-map phase 0: wait for the title/help bar and fade to be ready, then
// start the fade-in animation and advance to phase 1.
void func_80251560(CMenuMapSelectSC* self) {
    if (func_801C4114(&self->mTitleAHelp) && func_80244508(&self->mFade)) {
        func_80244518(&self->mFade);
        self->mState = 1;
    }
}

void func_802515B8(){}

void func_80251628(){}

void func_802516DC(){}

// World-map phase 6: wait for the fade to finish hiding, then reset the
// floor map and move to phase 7 (idle).
void func_80251D4C(CMenuMapSelectSC* self) {
    if (func_80244510(&self->mFade)) {
        func_8024CB94(&self->mFloorMap);
        self->mState = 7;
    }
}

// World-map phase 6 variant: after the fade completes, hand the selected
// floor-map cursor index to the game manager and move to phase 7 (idle).
void func_80251D94(CMenuMapSelectSC* self) {
    if (func_80244510(&self->mFade)) {
        func_8008413C__Q22cf13CfGameManagerFv(func_8024F54C(&self->mFloorMap), 0);
        self->mState = 7;
    }
}

// IScnRender vtable this-adjusting thunk for cbRenderBefore.
// IScnRender is a non-primary base at offset 0x58 within CMenuMapSelectSC;
// vtable dispatch passes 'this' pointing at +0x58, so the thunk subtracts
// it back and tail-calls the real implementation.
// Retail: subi r3, r3, 0x58; b cbRenderBefore__16CMenuMapSelectSCFv
void func_80251DE8(IScnRender* self) {
    ((void(*)(void*))cbRenderBefore__16CMenuMapSelectSCFv)((char*)self - 0x58);
}

// IScnRender vtable this-adjusting thunk for ~CMenuMapSelectSC.
// Same adjustment as func_80251DE8 but forwards to the destructor, leaving
// r4 (delete flag) as caller leftover.
// Retail: subi r3, r3, 0x58; b __dt__16CMenuMapSelectSCFv
void func_80251DF0(IScnRender* self) {
    ((void(*)(void*))__dt__16CMenuMapSelectSCFv)((char*)self - 0x58);
}
