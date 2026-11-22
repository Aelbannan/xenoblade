// Auto-scaffolded catalog TU for kyoshin/cf/CtrlEnemy
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/cf/CBattleManager.hpp"
#include "kyoshin/cf/CtrlMoveBase.hpp"
#include "kyoshin/cf/object/CfObjectMoveApi.hpp"
#include "kyoshin/cf/CfMapItemManager.hpp"
#include "kyoshin/cf/object/CBattleState.hpp"
#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/cf/CtrlEnemy.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (gmScene; gmGameFlags)
#include "kyoshin/CfEnumListHolder.hpp"

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
// the MapFx_ResolveOwner(parent) result at +0x80. Returns self.
void* __ct__cf_CtrlEnemy(cf::CtrlEnemy* self, void* parent) {
    __ct__800D10DC(self, parent);
    *(void**)self = (void*)lbl_eu_80527738;
    __ct__Q22cf11CtrlMoveEneFPQ22cf9CtrlEnemy(&self->mSub84, self);
    self->field_0x80 = (cf::CtrlEnemyActor*)MapFx_ResolveOwner(parent);
    return self;
}

cf::CtrlAct::~CtrlAct() {}

cf::CtrlBase::~CtrlBase() {}

cf::CtrlEnemy::~CtrlEnemy() {
    // novtable class: restore the retail vtable manually (MWCC would emit
    // its own __vt__Q22cf9CtrlEnemy otherwise -> reloc name drift).
    *(void**)this = (void*)lbl_eu_80527738;
    // Destroy the +0x84 enemy move-controller sub-object (retail
    // CtrlMoveEne_ForwardBaseRefresh, a 4-byte blr stub).
    CtrlMoveEne_ForwardBaseRefresh(&mSub84);
}

// Retail CtrlEnemy_UnkFunc_8008772C (0x80088104): reset the +0x04 flag word,
// then, while the battle actor's embedded move sub-object reports a positive
// +0x8C gauge, run the self vtable slots 0x8C then 0x58.
void CtrlEnemy_UnkFunc_8008772C(cf::CtrlEnemy* self) {
    self->field_0x04 = 0;
    if (self->field_0x80->mSub.s08C() > 0.0f) {
        self->v08C();
        self->v058();
    }
}

// Retail CtrlEnemy_UnkFunc_800877A8 (0x80088180): the enemy's main tick.
// Early-exit gates (global flag / battle-cmd bits / arts slots) reset the
// +0x14 counter via CtrlMoveEne_UpdateBattleMove; otherwise the battle actor's embedded
// move sub-object and battle-cmd flags drive the follow-up work: swapping
// the drive target, re-selecting an action (sub slot 0x4C / the UnkFunc
// helpers / v2E8), then the final arts-category + distance wall checks.
void CtrlEnemy_UnkFunc_800877A8(cf::CtrlEnemy* self) {
    // One distinct slot per testActorStatus probe, declared in retail first-use
    // order (they land at +0x34, +0x30, ... , +0x08).
    u32 w34;
    u32 w30;
    u32 w2C;
    u32 w28;
    u32 w24;
    u32 w20;
    u32 w1C;
    u32 w18;
    u32 w14;
    u32 w10;
    u32 w0C;
    u32 w08;
    cf::CtrlEnemyActor* actor;
    void* enemy;
    int kept;

    getInstance__Q22cf13CfGameManagerFv();
    if (isGlobalCamFlagSet(0x1000000) != 0) goto fail;
    actor = self->field_0x80;
    w34 = *actor->field_04->b030();
    if (testActorStatus(actor, &w34, 0x1) != 0) goto fail;
    actor = self->field_0x80;
    w30 = *actor->field_04->b030();
    if (testActorStatus(actor, &w30, 0x8) != 0) goto fail;
    actor = self->field_0x80;
    w2C = *actor->field_04->b030();
    if (testActorStatus(actor, &w2C, 0x18) != 0) goto fail;
    if (isBattleStatusActive((u8*)self->field_0x80 + 0x8, 0xf) != 0) goto fail;
    if (isBattleStatusActive((u8*)self->field_0x80 + 0x8, 0x10) != 0) goto fail;
    if (isBattleStatusActive((u8*)self->field_0x80 + 0x8, 0x9) != 0) goto fail;
    actor = self->field_0x80;
    if (actor->field_3374 & 0x2000) goto fail;
fail:
    CtrlMoveEne_UpdateBattleMove(&self->mSub84);
    self->field_0x14 = 0.0f;
    goto end;
gateOk:
    // Attack-gauge reset when the actor's flag word shows it; then drive the
    // embedded move sub-object by battle-cmd state. (actor survives the fail
    // block in r3 - retail reuses the pre-fail load for the +0x45CA read.)
    if (actor->field_45CA & 0x6) self->field_0x14 = 0.0f;
    if (self->field_0x80->mSub.s00C(0x200) == 0) goto L_8839C;
    if (self->field_0x80->field_3F60 == 0) {
        if (self->field_0x80->field_3374 & 0x08000000) {
            unregisterBattleActor(cf::CBattleManager::getInstance(),
                          self->field_0x80);
        }
        self->field_0x80->mSub.s050(0);
        self->field_0x80->mSub.s010(0x200);
        self->field_0x80->field_3374 &= ~0x08000000;
        if (self->field_0x80->v2BC() == 0) {
            dispatchItemLoad(self->field_0x80, 0x3);
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
    actor = self->field_0x80;
    w28 = *actor->field_04->b030();
    if (testActorStatus(actor, &w28, 0x3) != 0) goto end;
    CtrlMoveEne_UpdateBattleAI(&self->mSub84);
    goto end;
L_88468:
    actor = self->field_0x80;
    w24 = *actor->field_04->b030();
    if (testActorStatus(actor, &w24, 0x3) == 0) goto L_88600;
    // Action re-selection: keep the current target while a new one resolves.
    kept = 0;
    if (CtrlMoveEne_ConsumeTargetResolve(&self->mSub84) != 0) {
        enemy = self->field_0x1E0;
        kept = 1;
        goto L_8854C;
    }
    if (self->field_0x200 & 0x9000) goto L_8854C;
    enemy = self->field_0x80->mSub.s04C();
    if (enemy != 0) goto L_8852C;
    enemy = CtrlEnemy_UnkFunc_80087EEC(self);
    if (enemy != 0) goto L_88518;
    enemy = self->field_0x80->v2E8();
L_88518:
    if (enemy != 0) goto L_8852C;
    enemy = CtrlEnemy_UnkFunc_80088620(self);
L_8852C:
    if (enemy == 0) goto L_8854C;
    if (CtrlMoveEne_TryLatchTargetActor(&self->mSub84, enemy) != 0) kept = 1;
L_8854C:
        if (gmGameFlags & 0x2000) {
            if (isSceneReadyForInput__Q22cf13CfGameManagerFv() == 0) kept = 0;
        }
        if (kept != 0) {
            self->field_0x80->mSub.s050((u32)enemy);
            if (self->field_0x80->mSub.s00C(0x100) == 0) {
                self->field_0x80->mSub.s008(0x100);
                void* obj = MapFx_ResolveOwner(findObjectById((int)enemy));
                self->field_0x80->v2C4(obj, 10.0f, 0.0f,
                                       0.0f);
            }
            goto L_886DC;
        }
        CtrlMoveEne_DispatchMoveHook(&self->mSub84);
        goto L_886DC;
L_88600:
    actor = self->field_0x80;
    w20 = *actor->field_04->b030();
    if (testActorStatus(actor, &w20, 0x5) != 0) {
        self->field_0x14 = 0.0f;
        goto L_886D4;
    }
    actor = self->field_0x80;
    w1C = *actor->field_04->b030();
    if (testActorStatus(actor, &w1C, 0x19) != 0) {
        self->field_0x14 = 4.0f;
        goto L_886D4;
    }
    actor = self->field_0x80;
    w18 = *actor->field_04->b030();
    if (testActorStatus(actor, &w18, 0x800) != 0) goto L_886D4;
    self->v060();
L_886D4:
    CtrlMoveEne_UpdateBattleAI(&self->mSub84);
L_886DC:
    actor = self->field_0x80;
    w14 = *actor->field_04->b030();
    if (testActorStatus(actor, &w14, 0x4000) != 0) goto L_88744;
    if (isBattleStatusActive((u8*)self->field_0x80 + 0x8, 0x6) != 0) goto L_88744;
    if (isBattleStatusActive((u8*)self->field_0x80 + 0x8, 0xcd) == 0) goto L_8874C;
L_88744:
    self->field_0x14 = 0.0f;
L_8874C:
    actor = self->field_0x80;
    w10 = *actor->field_04->b030();
    if (testActorStatus(actor, &w10, 0x3) != 0) goto L_887F4;
    actor = self->field_0x80;
    w0C = *actor->field_04->b030();
    if (testActorStatus(actor, &w0C, 0x6) != 0) goto L_887F4;
    actor = self->field_0x80;
    w08 = *actor->field_04->b030();
    if (testActorStatus(actor, &w08, 0x806) == 0) goto end;
L_887F4:
    if (self->field_0x80->mSub.s00C(0x4) == 0) goto end;
    if (self->field_0x80->mSub.s140() == 0.0f) {
        self->field_0x80->mSub.s010(0x4);
        goto end;
    }
    self->field_0x14 = 0.0f;
    self->field_0x80->mSub.s0C4(self->field_0x0C);
    CfObjectMove_setAnimModeArgs((u8*)&self->field_0x80->mSub, 0x3, 0, -0x1, 0x1);
    self->field_0x80->mSub.s010(0x4);
end:
    return;
}

float cf::CfObjectMove::CfObject_getMoveFactor() {
    return mField6EC;
}

// Retail CtrlEnemy_UnkFunc_80087EEC (0x80087EEC): pick an enemy action
// target. Gates on the battle state word (vtable 0x258), the global gimmick
// flags and the battle manager, builds a state-filtered candidate list, then
// scans it for the nearest eligible enemy (distance / arts-power wall) and
// returns its id.
void* CtrlEnemy_UnkFunc_80087EEC(cf::CtrlEnemy* self) {
    // Anchor-position buffers. Declaration order mirrors the retail frame
    // (MWCC allocates locals downward): u32 cmdWord @0x08, holder @0x10,
    // diff @0x18, sumTmp @0x24, distSqTmp @0x30, lifted @0x3c, upVec @0x48,
    // probePos @0x54, anchor @0x60, powTmp @0x70.
    u32 cmdWord;
    f32 diff[3];
    f32 sumTmp[3];
    f32 distSqTmp[3];
    f32 lifted[3];
    f32 upVec[3];
    f32 probePos[3];
    f32 anchor[3];
    f64 powTmp;
    int i;
    int id;
    cf::CtrlEnemyActor* obj;
    void* sub;
    cf::CtrlEnemyActor* actor;

    if (*self->field_0x80->v258() == 0 ||
        getUnk80664658()->mFlags & 0x20000) {
        return 0;
    }
    if (((cf::CBattleManager*)cf::CBattleManager::getInstance())->CheckUnk84Flag(
            0x1000) != 0) {
        return 0;
    }
    {
        // Anchor position: slot 0x12C probe (arg 0x64) yields a ready
        // position block; otherwise lift the slot-0xAC position by (0,1,0).
        f32* src;
        void* probe = self->field_0x80->mSub.s12C(0x64);
        if (probe != 0) {
            cf::CtrlEnemyPos* pos = (cf::CtrlEnemyPos*)probe;
            probePos[0] = pos->x;
            probePos[1] = pos->y;
            probePos[2] = pos->z;
            src = &probePos[0];
        } else {
            upVec[0] = 0.0f;
            upVec[1] = 1.0f;
            upVec[2] = 0.0f;
            cf::CtrlEnemyPos4* base =
                (cf::CtrlEnemyPos4*)self->field_0x80->mSub.s0AC();
            sumTmp[0] = base->e0 + upVec[0];
            sumTmp[1] = base->e1 + upVec[1];
            sumTmp[2] = base->e2 + upVec[2];
            lifted[0] = sumTmp[0];
            lifted[1] = sumTmp[1];
            lifted[2] = sumTmp[2];
            src = &lifted[0];
        }
        anchor[0] = src[0];
        anchor[1] = src[1];
        anchor[2] = src[2];
    }
    CfEnumListHolder holder;
    switch (*self->field_0x80->v258()) {
    case 1:
        startEnumObjects(holder.get(), 0x20, 0x900);
        sub = self->field_0x80->mSub.s0AC();
        powTmp = self->field_0x80->v5B4();
        {
            f32 f270 = *self->field_0x80->v270();
            f32 f268 = *self->field_0x80->v268();
            rebuildByDistanceCone(holder.get(), sub, f268, f270,
                          (f32)powTmp, 0);
        }
        __ct__800FAE3C(holder.get(), anchor, 0);
        break;
    case 2:
        startEnumObjects(holder.get(), 0x20, 0xa00);
        sub = self->field_0x80->mSub.s0AC();
        {
            f32 f268 = *self->field_0x80->v268();
            __ct__800FB044(holder.get(), f268, sub, 0);
        }
        __ct__800FAE3C(holder.get(), anchor, 0);
        break;
    case 3:
        startEnumObjects(holder.get(), 0x20, 0x800);
        sub = self->field_0x80->mSub.s0AC();
        {
            f32 f268 = *self->field_0x80->v268();
            __ct__800FB044(holder.get(), f268, sub, 0);
        }
        __ct__800FD0B4(holder.get(), 0);
        __ct__800FAE3C(holder.get(), anchor, 0);
        break;
    case 4:
        startEnumObjects(holder.get(), 0x20, 0x800);
        sub = self->field_0x80->mSub.s0AC();
        {
            f32 f268 = *self->field_0x80->v268();
            __ct__800FB044(holder.get(), f268, sub, 0);
        }
        __ct__800FC4FC(holder.get(), 0.15f, 0);
        break;
    }
    // Scan candidates: nearest eligible enemy wins.
    for (i = 0; i < (int)((cf::CVisionEnumList*)holder.get())->count; i++) {
        id = (int)getObjectIdAt(holder.get(), i);
        obj = (cf::CtrlEnemyActor*)MapFx_ResolveOwner(findObjectById(id));
        if (obj->field_3F60 != 0 &&
            (((cf::CtrlEnemySubFlag*)obj->field_3F60)->field_530 & 1) != 0) {
            continue;
        }
        sub = (obj != 0) ? (void*)&obj->mSub : obj;
        if (sub != getPlayer__Q22cf13CfGameManagerFi(0)) {
            cmdWord = *(u32*)obj->field_04->b030();
            if (testActorStatus(obj, &cmdWord, 0x802) != 0) {
                continue;
            }
        }
        actor = self->field_0x80;
        if (actor->field_15F0 == 0 && obj->v108() - actor->v108() >= 6) {
            continue;
        }
        // Squared distance from the anchor to the candidate.
        {
            cf::CtrlEnemyPos4* pa = (cf::CtrlEnemyPos4*)actor->mSub.s0AC();
            cf::CtrlEnemyPos4* pb = (cf::CtrlEnemyPos4*)obj->mSub.s0AC();
            diff[0] = pb->e0 - pa->e0;
            diff[1] = pb->e1 - pa->e1;
            diff[2] = pb->e2 - pa->e2;
        }
        {
            u32 flags = actor->field_3374;
            // Arts-category probe picked from the actor's flags.
            int artId;
            if (flags & 0x10000) {
                artId = 0xba;
            } else if (flags & 0x100) {
                artId = 0xbb;
            } else {
                artId = 0xb9;
            }
            distSqTmp[0] = diff[0];
            distSqTmp[1] = diff[1];
            f32 distSq = diff[2];
            distSq = distSqTmp[0] * distSqTmp[0] + distSqTmp[1] * distSqTmp[1] +
                     distSq * distSq;
            if (isBattleStatusActive((u8*)obj + 8, artId) != 0) {
                cf::CBattleStateEntry* info =
                    (cf::CBattleStateEntry*)findBattleStatusEntry((u8*)obj + 8, artId);
                // Remaining-power ratio, squared, as a double wall.
                f32 scale = *actor->v268();
                powTmp = (f64)(0x64 - info->unk10);
                powTmp = powTmp * scale / 100.0f;
                powTmp = powTmp * powTmp;
                if (powTmp < (f64)distSq) {
                    continue;
                }
            }
        }
        if (obj->v2BC() != 0) {
            continue;
        }
        return (void*)id;
    }
    return 0;
}

// Retail CtrlEnemy_UnkFunc_80088620 (0x80088FF8): hunt the battle enum list
// for an enemy whose vtable-0xE0 id matches the battle actor's, and return
// the enemy's vtable-0x2E8 action object if found. Gate: the battle actor
// must report a nonzero +0x260 state and the battle manager must be idle
// (slot 0x28 with 0x1000). The list is filtered to 0x80000000 entries and
// the sub-object slot-0xAC position feeds the 800FB044 target ctor first.
void* CtrlEnemy_UnkFunc_80088620(cf::CtrlEnemy* self) {
    if (self->field_0x80->v260() == 0) return 0;
    if (((cf::CBattleManager*)cf::CBattleManager::getInstance())->CheckUnk84Flag(0x1000) !=
        0) {
        return 0;
    }
    CfEnumListHolder holder;
    self->field_0x80->v258();
    startEnumObjects(holder.get(), 0x80000000, 0);
    void* pos = self->field_0x80->mSub.s0AC();
    f32 gauge = *self->field_0x80->v268();
    __ct__800FB044(holder.get(), gauge, pos, 0);
    for (u32 i = 0; i < ((cf::CVisionEnumList*)holder.get())->count; i++) {
        cf::CtrlEnemyActor* obj = (cf::CtrlEnemyActor*)MapFx_ResolveOwner(
            findObjectById((int)getObjectIdAt(holder.get(), i)));
        if (obj == 0) continue;
        int objId = obj->v0E0();
        int battleId = self->field_0x80->v0E0();
        if (battleId != objId) continue;
        // Reuse objId's (now-dead) register web for the v2E8 result so it
        // keeps the same saved register across the dtor call (retail puts
        // both values in r31).
        objId = (int)obj->v2E8();
        return (void*)objId;
    }
    return 0;
}

// Retail CtrlEnemy_UnkFunc_80088800 (0x800891D8): the gate arg (r8) routes
// to the +0x84 move-controller helpers. With a nonzero gate the out-flag
// byte is zeroed, the horizontal direction is computed via aimMoveTarget
// (which receives the flag arg) and 1 is returned; otherwise moveBaseSeekTo
// fills the out vector and returns its status.
int CtrlEnemy_UnkFunc_80088800(cf::CtrlEnemy* self, ml::CVec3* out, u8* outFlag,
                               const ml::CVec3* src, int flag, int gate) {
    if (gate != 0) {
        *outFlag = 0;
        aimMoveTarget((cf::CCtrlMoveBase*)&self->mSub84, out, src, flag);
        return 1;
    }
    return moveBaseSeekTo((cf::CCtrlMoveBase*)&self->mSub84, out, outFlag, src, flag);
}

extern "C" void cachedMoveReq(void* a, float f);
extern "C" void CtrlEnemy_UnkFunc_8008884C(void* self) { cachedMoveReq(((cf::CtrlEnemy*)self)->mSub84, 30.0f); }

void cf::CtrlEnemy::delegateTo80089B24() { moveBaseSway(this->mSub84); }

void cf::CtrlEnemy::delegateTo80089E88() { moveBaseRoute(this->mSub84); }

void cf::CtrlEnemy::delegateTo80089F68() { moveBaseScnCmd((cf::CCtrlMoveBase*)(this->mSub84)); }

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

extern "C" void CtrlEnemy_delegateTo80089B24(u8* self) { moveBaseSway(((cf::CtrlEnemy*)self)->mSub84); }
extern "C" void CtrlEnemy_delegateTo80089E88(u8* self) { moveBaseRoute(((cf::CtrlEnemy*)self)->mSub84); }
extern "C" void CtrlEnemy_delegateTo80089F68(u8* self) { moveBaseScnCmd((cf::CCtrlMoveBase*)((cf::CtrlEnemy*)self)->mSub84); }

extern "C" int CBattleManager_CheckUnk84Flag(cf::CBattleManager* self, u32 mask) {
    return (self->unk84 & mask) != 0;
}

