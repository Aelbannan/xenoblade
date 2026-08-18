// Auto-scaffolded catalog TU for kyoshin/cf/CtrlEnemy
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/cf/CtrlEnemy.hpp"

namespace cf {
class CtrlBase;
class CBattleManager;
class CActorParam;
}

// Forward decls: the UnkFunc helpers call each other (kept in cpp - unit-local
// retail symbols, symbol map links them).
void* CtrlEnemy_UnkFunc_80088620(cf::CtrlEnemy* self);
void* CtrlEnemy_UnkFunc_80087EEC(cf::CtrlEnemy* self);

// Retail __ct__cf_CtrlEnemy (0x80087F80): runs the base ctor (passing the
// parent through), installs the retail vtable, constructs the +0x84 enemy
// move-controller sub-object (passing the owner as its parent), then stores
// the func_8016FE34(parent) result at +0x80. Returns self.
void* __ct__cf_CtrlEnemy(cf::CtrlEnemy* self, void* parent) {
    __ct__800D10DC(self, parent);
    *(void**)self = (void*)lbl_eu_80527738;
    __ct__8008A104(&self->mSub84, self);
    self->field_0x80 = (cf::CtrlEnemyActor*)func_8016FE34(parent);
    return self;
}

cf::CtrlAct::~CtrlAct() {}

cf::CtrlBase::~CtrlBase() {}

// Retail __dt__80087688: deleting destructor of an unrecovered class. Free-
// function form (like __dt__800A75FC in code_800A75FC.cpp) - a local class
// with an out-of-line virtual dtor would make MWCC emit its __vt__ into
// .data, but the retail split for this unit has no .data section. The body
// reproduces the retail frame: saved self in r31, null-check + flag-gated
// operator delete, return self.
void* __dt__80087688(void* self, int flag) {
    if (self != 0 && flag > 0) {
        operator delete(self);
    }
    return self;
}

cf::CtrlEnemy::~CtrlEnemy() {
    // novtable class: restore the retail vtable manually (MWCC would emit
    // its own __vt__Q22cf9CtrlEnemy otherwise -> reloc name drift).
    *(void**)this = (void*)lbl_eu_80527738;
    // Destroy the +0x84 enemy move-controller sub-object (retail
    // func_8008B930, a 4-byte blr stub).
    func_8008B930(&mSub84);
}

// Retail CtrlEnemy_UnkFunc_8008772C (0x80088104): reset the +0x04 flag word,
// then, while the battle actor's embedded move sub-object reports a positive
// +0x8C gauge, run the self vtable slots 0x8C then 0x58.
void CtrlEnemy_UnkFunc_8008772C(cf::CtrlEnemy* self) {
    self->field_0x04 = 0;
    if (self->field_0x80->mSub.s08C() > lbl_eu_80666570) {
        self->v08C();
        self->v058();
    }
}

// Retail CtrlEnemy_UnkFunc_800877A8 (0x80088180): the enemy's main tick.
// Early-exit gates (global flag / battle-cmd bits / arts slots) reset the
// +0x14 counter via func_8008B580; otherwise the battle actor's embedded
// move sub-object and battle-cmd flags drive the follow-up work: swapping
// the drive target, re-selecting an action (sub slot 0x4C / the UnkFunc
// helpers / v2E8), then the final arts-category + distance wall checks.
void CtrlEnemy_UnkFunc_800877A8(cf::CtrlEnemy* self) {
    getInstance__Q22cf13CfGameManagerFv();
    if (func_8006EF04(0x1000000) != 0) goto fail;
    {
        cf::CtrlEnemyActor* p = self->field_0x80;
        u32 v = *p->field_04->b030();
        if (func_80174C98(p, &v, 0x1) != 0) goto fail;
    }
    {
        cf::CtrlEnemyActor* p = self->field_0x80;
        u32 v = *p->field_04->b030();
        if (func_80174C98(p, &v, 0x8) != 0) goto fail;
    }
    {
        cf::CtrlEnemyActor* p = self->field_0x80;
        u32 v = *p->field_04->b030();
        if (func_80174C98(p, &v, 0x18) != 0) goto fail;
    }
    if (func_80148778((u8*)self->field_0x80 + 0x8, 0xf) != 0) goto fail;
    if (func_80148778((u8*)self->field_0x80 + 0x8, 0x10) != 0) goto fail;
    if (func_80148778((u8*)self->field_0x80 + 0x8, 0x9) != 0) goto fail;
    cf::CtrlEnemyActor* p = self->field_0x80;
    if (p->field_3374 & 0x2000) goto fail;
fail:
    func_8008B580(&self->mSub84);
    self->field_0x14 = lbl_eu_80666570;
    goto end;
gateOk:
    // Attack-gauge reset when the actor's flag word shows it; then drive the
    // embedded move sub-object by battle-cmd state. (p survives the fail
    // block in r3 - retail reuses the pre-fail load for the +0x45CA read.)
    if (p->field_45CA & 0x6) self->field_0x14 = lbl_eu_80666570;
    if (self->field_0x80->mSub.s00C(0x200) == 0) goto L_8839C;
    if (self->field_0x80->field_3F60 == 0) {
        if (self->field_0x80->field_3374 & 0x08000000) {
            func_800D9CA0(getInstance__Q22cf14CBattleManagerFv(),
                          self->field_0x80);
        }
        self->field_0x80->mSub.s050(0);
        self->field_0x80->mSub.s010(0x200);
        self->field_0x80->field_3374 &= ~0x08000000;
        if (self->field_0x80->v2BC() == 0) {
            func_80174B4C(self->field_0x80, 0x3);
        }
    } else {
        self->field_0x04 |= 0x8;
    }
    goto L_88408;
L_8839C:
    if (self->field_0x80->mSub.s00C(0x100) != 0) {
        if (self->field_0x80->field_3374 & 0x08000000) {
            self->field_0x80->mSub.s010(0x100);
        } else if (self->field_0x80->v2BC() == 0) {
            self->field_0x04 |= 0x4;
        }
    }
L_88408:
    if (self->v088() == 0) goto L_88468;
    {
        cf::CtrlEnemyActor* p = self->field_0x80;
        u32 v = *p->field_04->b030();
        if (func_80174C98(p, &v, 0x3) != 0) goto end;
    }
    func_8008A2C8(&self->mSub84);
    goto end;
L_88468:
    {
        cf::CtrlEnemyActor* p = self->field_0x80;
        u32 v = *p->field_04->b030();
        if (func_80174C98(p, &v, 0x3) == 0) goto L_88600;
    }
    // Action re-selection: keep the current target while a new one resolves.
    {
        int kept = 0;
        if (func_8008B974(&self->mSub84) != 0) {
            void* enemy = self->field_0x1E0;
            kept = 1;
            goto L_8854C;
        }
        if (self->field_0x200 & 0x9000) goto L_8854C;
        void* enemy = self->field_0x80->mSub.s04C();
        if (enemy != 0) goto L_8852C;
        enemy = CtrlEnemy_UnkFunc_80087EEC(self);
        if (enemy != 0) goto L_88518;
        enemy = self->field_0x80->v2E8();
L_88518:
        if (enemy != 0) goto L_8852C;
        enemy = CtrlEnemy_UnkFunc_80088620(self);
L_8852C:
        if (enemy == 0) goto L_8854C;
        if (func_8008B934(&self->mSub84, enemy) != 0) kept = 1;
L_8854C:
        if (lbl_eu_80663E24 & 0x2000) {
            if (func_80085840__Q22cf13CfGameManagerFv() == 0) kept = 0;
        }
        if (kept != 0) {
            self->field_0x80->mSub.s050((u32)enemy);
            if (self->field_0x80->mSub.s00C(0x100) == 0) {
                self->field_0x80->mSub.s008(0x100);
                void* obj = func_8016FE34(func_800B708C((int)enemy));
                self->field_0x80->v2C4(obj, lbl_eu_80666574, lbl_eu_80666570,
                                       lbl_eu_80666570);
            }
            goto L_886DC;
        }
        func_8008A23C(&self->mSub84);
        goto L_886DC;
    }
L_88600:
    {
        cf::CtrlEnemyActor* p = self->field_0x80;
        u32 v = *p->field_04->b030();
        if (func_80174C98(p, &v, 0x5) != 0) {
            self->field_0x14 = lbl_eu_80666570;
            goto L_886D4;
        }
    }
    {
        cf::CtrlEnemyActor* p = self->field_0x80;
        u32 v = *p->field_04->b030();
        if (func_80174C98(p, &v, 0x19) != 0) {
            self->field_0x14 = lbl_eu_80666578;
            goto L_886D4;
        }
    }
    {
        cf::CtrlEnemyActor* p = self->field_0x80;
        u32 v = *p->field_04->b030();
        if (func_80174C98(p, &v, 0x800) != 0) goto L_886D4;
    }
    self->v060();
L_886D4:
    func_8008A2C8(&self->mSub84);
L_886DC:
    {
        cf::CtrlEnemyActor* p = self->field_0x80;
        u32 v = *p->field_04->b030();
        if (func_80174C98(p, &v, 0x4000) != 0) goto L_88744;
    }
    if (func_80148778((u8*)self->field_0x80 + 0x8, 0x6) != 0) goto L_88744;
    if (func_80148778((u8*)self->field_0x80 + 0x8, 0xcd) == 0) goto L_8874C;
L_88744:
    self->field_0x14 = lbl_eu_80666570;
L_8874C:
    {
        cf::CtrlEnemyActor* p = self->field_0x80;
        u32 v = *p->field_04->b030();
        if (func_80174C98(p, &v, 0x3) != 0) goto L_887F4;
    }
    {
        cf::CtrlEnemyActor* p = self->field_0x80;
        u32 v = *p->field_04->b030();
        if (func_80174C98(p, &v, 0x6) != 0) goto L_887F4;
    }
    {
        cf::CtrlEnemyActor* p = self->field_0x80;
        u32 v = *p->field_04->b030();
        if (func_80174C98(p, &v, 0x806) == 0) goto end;
    }
L_887F4:
    if (self->field_0x80->mSub.s00C(0x4) == 0) goto end;
    if (self->field_0x80->mSub.s140() == lbl_eu_80666570) {
        self->field_0x80->mSub.s010(0x4);
        goto end;
    }
    self->field_0x14 = lbl_eu_80666570;
    self->field_0x80->mSub.s0C4(self->field_0x0C);
    func_800BE12C(&self->field_0x80->mSub, 0x3, 0, -0x1, 0x1);
    self->field_0x80->mSub.s010(0x4);
end:
    return;
}

float cf::CfObjectMove::CfObject_UnkVirtualFunc60() {
    // Retail: lfs f1, 0x6ec(r3); blr - the getter returns the field (f1 is
    // the FP return register). The view class in CtrlEnemy.hpp declares the
    // float return so the definition compiles.
    return mField6EC;
}

// Placeholder until decompiled below.
void* CtrlEnemy_UnkFunc_80087EEC(cf::CtrlEnemy* self) { return 0; }

bool CBattleManager_CheckUnk84Flag(cf::CBattleManager* obj, unsigned int mask)
{
    return (*(unsigned int*)((char*)obj + 0x84) & mask) != 0;
}

// Retail CtrlEnemy_UnkFunc_80088620 (0x80088FF8): hunt the battle enum list
// for an enemy whose vtable-0xE0 id matches the battle actor's, and return
// the enemy's vtable-0x2E8 action object if found. Gate: the battle actor
// must report a nonzero +0x260 state and the battle manager must be idle
// (slot 0x28 with 0x1000). The list is filtered to 0x80000000 entries and
// the sub-object slot-0xAC position feeds the 800FB044 target ctor first.
void* CtrlEnemy_UnkFunc_80088620(cf::CtrlEnemy* self) {
    if (self->field_0x80->v260() == 0) return 0;
    if (((cf::CtrlEnemyBM*)getInstance__Q22cf14CBattleManagerFv())->m028(0x1000) !=
        0) {
        return 0;
    }
    cf::CtrlEnemyEnumHolder holder;
    func_80043D90(&holder);
    self->field_0x80->v258();
    func_800F4A98(func_80043F18(&holder), 0x80000000, 0);
    void* pos = self->field_0x80->mSub.s0AC();
    f32 gauge = *self->field_0x80->v268();
    __ct__800FB044(func_80043F18(&holder), gauge, pos, 0);
    for (u32 i = 0; i < func_80043F18(&holder)->field_620; i++) {
        cf::CtrlEnemyActor* obj = (cf::CtrlEnemyActor*)func_8016FE34(
            func_800B708C((int)func_800F6E98(func_80043F18(&holder), i)));
        if (obj == 0) continue;
        int objId = obj->v0E0();
        int battleId = self->field_0x80->v0E0();
        if (battleId != objId) continue;
        // Reuse objId's (now-dead) register web for the v2E8 result so it
        // keeps the same saved register across the dtor call (retail puts
        // both values in r31).
        objId = (int)obj->v2E8();
        __dt__80043E88(&holder, -1);
        return (void*)objId;
    }
    __dt__80043E88(&holder, -1);
    return 0;
}

// Retail CtrlEnemy_UnkFunc_80088800 (0x800891D8): the gate arg (r8) routes
// to the +0x84 move-controller helpers. With a nonzero gate the out-flag
// byte is zeroed, the horizontal direction is computed via func_80089398
// (which receives the flag arg) and 1 is returned; otherwise func_800890A8
// fills the out vector and returns its status.
int CtrlEnemy_UnkFunc_80088800(cf::CtrlEnemy* self, ml::CVec3* out, u8* outFlag,
                               const ml::CVec3* src, int flag, int gate) {
    if (gate != 0) {
        *outFlag = 0;
        func_80089398((cf::CCtrlMoveBase*)&self->mSub84, out, src, flag);
        return 1;
    }
    return func_800890A8((cf::CCtrlMoveBase*)&self->mSub84, out, outFlag, src, flag);
}

extern f32 lbl_eu_80666590;
extern "C" void func_800899C0(void* a, float f);
extern "C" void CtrlEnemy_UnkFunc_8008884C(void* self) { func_800899C0((char*)self + 132, lbl_eu_80666590); }

void cf::CtrlEnemy::delegateTo80089B24() { func_80089B24((char*)this + 0x84); }

void cf::CtrlEnemy::delegateTo80089E88() { func_80089E88((char*)this + 0x84); }

void cf::CtrlEnemy::delegateTo80089F68() { func_80089F68((char*)this + 0x84); }

int cf::CtrlEnemy::ret0_80088870() { return 0; }
int cf::CtrlEnemy::ret0_80088878() { return 0; }
int cf::CtrlEnemy::ret0_80088880() { return 0; }
int cf::CtrlEnemy::ret0_80088888() { return 0; }
int cf::CtrlEnemy::ret0_80088890() { return 0; }
int cf::CtrlEnemy::ret0_80088898() { return 0; }
int cf::CtrlEnemy::ret0_800888A0() { return 0; }
int cf::CtrlEnemy::ret0_800888A8() { return 0; }
int cf::CtrlEnemy::ret0_800888B0() { return 0; }
int cf::CtrlEnemy::ret0_800888B8() { return 0; }
int cf::CtrlEnemy::ret0_800888C0() { return 0; }
int cf::CtrlEnemy::ret0_800888C8() { return 0; }
int cf::CtrlEnemy::ret0_800888D0() { return 0; }
int cf::CtrlEnemy::ret0_800888D8() { return 0; }
int cf::CtrlEnemy::ret0_800888E0() { return 0; }
int cf::CtrlEnemy::ret0_800888E8() { return 0; }

extern "C" void CtrlEnemy_emptyStub_800888F0() {}

int cf::CtrlEnemy::ret1_800888F4() { return 1; }
int cf::CtrlEnemy::ret1_800888FC() { return 1; }

// Free-function aliases (C linkage) at 0x800892xx
extern "C" int CtrlEnemy_ret0_80088870() { return 0; }
extern "C" int CtrlEnemy_ret0_80088878() { return 0; }
extern "C" int CtrlEnemy_ret0_80088880() { return 0; }
extern "C" int CtrlEnemy_ret0_80088888() { return 0; }
extern "C" int CtrlEnemy_ret0_80088890() { return 0; }
extern "C" int CtrlEnemy_ret0_80088898() { return 0; }
extern "C" int CtrlEnemy_ret0_800888A0() { return 0; }
extern "C" int CtrlEnemy_ret0_800888A8() { return 0; }
extern "C" int CtrlEnemy_ret0_800888B0() { return 0; }
extern "C" int CtrlEnemy_ret0_800888B8() { return 0; }
extern "C" int CtrlEnemy_ret0_800888C0() { return 0; }
extern "C" int CtrlEnemy_ret0_800888C8() { return 0; }
extern "C" int CtrlEnemy_ret0_800888D0() { return 0; }
extern "C" int CtrlEnemy_ret0_800888D8() { return 0; }
extern "C" int CtrlEnemy_ret0_800888E0() { return 0; }
extern "C" int CtrlEnemy_ret0_800888E8() { return 0; }
extern "C" int CtrlEnemy_ret1_800888F4() { return 1; }
extern "C" int CtrlEnemy_ret1_800888FC() { return 1; }

extern "C" void CtrlEnemy_delegateTo80089B24(u8* self) { func_80089B24((char*)self + 0x84); }
extern "C" void CtrlEnemy_delegateTo80089E88(u8* self) { func_80089E88((char*)self + 0x84); }
extern "C" void CtrlEnemy_delegateTo80089F68(u8* self) { func_80089F68((char*)self + 0x84); }

extern "C" int CBattleManager_CheckUnk84Flag(u8* self, u32 mask) {
    return (*(u32*)((char*)self + 0x84) & mask) != 0;
}