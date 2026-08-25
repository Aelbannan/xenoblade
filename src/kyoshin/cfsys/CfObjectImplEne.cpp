// Auto-scaffolded catalog TU for kyoshin/cfsys/CfObjectImplEne
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/cfsys/CfObjectImplEne.hpp"
#include "kyoshin/cf/CfMapItemManager.hpp"
#include "monolib/scn/CScnTimeApi.hpp"
#include "monolib/math/CVec3.hpp"
#include "kyoshin/realtimeevt/CREvtEffect.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)
#include "monolib/math/FloatUtils.hpp"  // H3 label-owner decl (lbl_eu_8066A208)

void __dt__Q22cf15CfObjectImplEneFv();

extern "C" int func_800D0C2C(cf::CfObjectImplEneObj* self, int flag);

// Runs the per-frame update: prepares via func_800CA948, dispatches vtable
// slot 0xe4, feeds the result + sub-fields into func_8015BB3C, pushes the
// +0x28 sub-object into func_802A0B8C, and finally triggers vtable slot 0x30
// when the +0x14 object's +0x70 token formats to the lbl_eu_80661D48 name.
void func_800CFFCC(cf::CfObjectImplEneObj* self) {
    func_800CA948(self);
    func_8015BB3C(self->vfE4(), self->field_14, self->field_18);
    func_802A0B8C(self->field_28, self->field_14);

    cf::CfObjectImplEneToken* p = (cf::CfObjectImplEneToken*)self->field_14;
    if (p != 0) {
        ml::FixStr<64> buf(true);
        if (func_800AA33C(buf, p->field_70, 0, 0) != 0) {
            if (strstr(buf.mString, lbl_eu_80661D48) != 0) {
                self->vf30(0x400, 1);
            }
        }
    }
}

void* func_800D0088(void* self) { return (void*)((u8*)self + 0x6c); }

// Releases the effect token at +0x36C (if set) and clears it, then forwards
// to func_800CAA44.
void func_800D0090(cf::CfObjectImplEneObj* self) {
    u32 v = self->field_36C;
    if (v != 0) {
        func_804E3CCC((void*)v);
        self->field_36C = 0;
    }
    func_800CAA44(self);
}

// Per-frame AI update: checks the battle-actor flag at +0x45CA (bit 1), then
// walks a 4-step guard chain (actor id / enemy state / sub-object / battle
// flag) into a "sync" flag passed to func_800CED64. Manages the effect token
// at +0x36C, mirrors the enemy angle into field_C4->+0x50C, dispatches the
// self vtable slots 0xe4/0x40, and finally runs the shared func_800D0C2C
// update with the player-controlled flag set.
void func_800D00DC(cf::CfObjectImplEneObj* self) {
    func_800CA964(self);
    if (self->field_14 == 0) return;

    cf::CfImplEneBattleObj* battleObj;
    int r29 = 0;
    int r28 = 0;
    int r27 = 0;
    int r26 = 0;
    cf::CfObjectImplEneFlag* flags = (cf::CfObjectImplEneFlag*)func_800AD860(self->field_14);
    if ((flags->field_45CA & 2) == 0) {
        battleObj = (cf::CfImplEneBattleObj*)self->field_18;
        int id = *(int*)battleObj->field_04->bf30();
        if (func_80174C98(battleObj, &id, 0x803) == 0) r26 = 1;
    }
    if (r26 != 0) {
        cf::CfImplEneBattleObj* battleObj = (cf::CfImplEneBattleObj*)self->field_18;
        if (battleObj->bhBC() == 0) r27 = 1;
    }
    if (r27 != 0) {
        cf::CfImplEneBattleObj* battleObj = (cf::CfImplEneBattleObj*)self->field_18;
        if (battleObj->mSub.sf4C() == 0) r28 = 1;
    }
    if (r28 != 0) {
        cf::CfImplEneBattleObj* battleObj = (cf::CfImplEneBattleObj*)self->field_18;
        if (!(battleObj->field_3374 & 0x08000000)) r29 = 1;
    }

    func_800CED64(self, r29);
    if (r29 != 0 || ((cf::CfObjectImplEne14*)self->field_14)->field_C4 == 0) {
        if (self->field_36C != 0) {
            func_804E3CCC((void*)self->field_36C);
            self->field_36C = 0;
        }
    }
    if (((cf::CfObjectImplEne14*)self->field_14)->field_C4 != 0) {
        cf::CfImplEneBattleObj* battleObj = (cf::CfImplEneBattleObj*)self->field_18;
        ((cf::CfObjectImplEneC4*)((cf::CfObjectImplEne14*)self->field_14)->field_C4)->field_50C =
            battleObj->bfD8();
    }
    cf::CfObjectImplEneE4* e4 = (cf::CfObjectImplEneE4*)self->vfE4();
    e4->e10();
    func_802A0E08(self->field_28);
    if (((cf::CfObjectImplEne14*)self->field_14)->field_90 != 0) {
        if (self->vf40(0x400) != 0) {
            ((cf::CfObjectImplEne98*)((cf::CfObjectImplEne14*)self->field_14)->field_98)->n5C(
                lbl_eu_80666CE0);
        }
    }
    func_800D0C2C(self, 1);
}

// Battle-actor per-frame logic: reacts to the actor's own state id (vtable
// 0x30 of the +0x4 sub-object) via func_80174C98 flag queries, issues battle-
// cmd removals (func_80174B4C) and action triggers, and performs the 0x968
// death handling by iterating the enum list and refreshing every unit.
void func_800D02D4(cf::CfObjectImplEneObj* self) {
    func_800CAB2C(self);
    cf::CfImplEneBattleObj* battleObj = (cf::CfImplEneBattleObj*)self->field_18;

    int r31 = 0;
    int r4 = 0;
    if (!(battleObj->field_3374 & 0x100) && battleObj->field_3F60 != 0) r4 = 1;
    if (r4 != 0 && (((cf::CfImplEneField3F60*)(u32)battleObj->field_3F60)->field_4EC & 0x80)) r31 = 1;
    if (r31 != 0) {
        battleObj->bg18(lbl_eu_80666CE4);
    }
    if (battleObj->bg2C() == lbl_eu_80666CE4) return;

    // Address-taken id slots (func_80174C98 out values); order matters for
    // the retail stack layout (0x30 down to 0x10).
    int idA;
    int idB;
    int idC;
    int idD;
    int idE;
    int idF;
    int idG;
    int idH;
    int idI;
    if (func_80174C98(battleObj, &idA, 0x1e) != 0) {
        if (*(float*)battleObj->bh34() > lbl_eu_80666CE4) {
            func_800BC3D8(&battleObj->mSub, lbl_eu_80666CE8);
        }
        f32 f31 = battleObj->mSub.sg6C();
        if (func_8006EF04(0x04000000) == 0 && f31 == lbl_eu_80666CEC &&
            battleObj->mSub.sh10(2) == 0) {
            battleObj->field_3F04 |= 0x40;
        }
        return;
    }

    if (func_80174C98(battleObj, &idB, 0x1c) != 0) {
        // Player: drive the actor by its battle-cmd flags.
        if (func_8006EF04(0x04000000) != 0) return;
        if (*(float*)battleObj->bh34() > lbl_eu_80666CEC) {
            if (func_80174C98(battleObj, &idC, 0x20000000) == 0) {
                if (battleObj->bi44() != 0) {
                    func_8013E2E0(battleObj->bi44(), 0, 0, 0, 0, 1, 0, 1, 0);
                }
                func_80174B4C(battleObj, 0x20000000);
            }
        }
        if (*(float*)battleObj->bh34() > lbl_eu_80666CE4) {
            if (func_80174C98(battleObj, &idD, 0x40000000) == 0) {
                func_80140E00(1, battleObj->field_3F28, 0);
                func_80174B4C(battleObj, 0x40000000);
            }
        }
        if (battleObj->mSub.sf0C(0x400) == 0) {
            cf::CfImplEneField3F60* f = (cf::CfImplEneField3F60*)(u32)battleObj->field_3F60;
            if (f != 0) {
                if (f->g80(0) != 0) {
                    battleObj->bh30();
                    battleObj->mSub.sf08(0x400);
                    if (func_80174C98(battleObj, &idE, 0x20000000) == 0) {
                        if (battleObj->bi44() != 0) {
                            func_8013E2E0(battleObj->bi44(), 0, 0, 0, 0, 1, 0, 1, 0);
                        }
                        func_80174B4C(battleObj, 0x20000000);
                    }
                }
            }
            return;
        }

        // Waiting: interrupt the current command when the timer is up.
        f32 f31 = *(float*)battleObj->bh34();
        if (battleObj->bfE0() == 1) {
            if (f31 > lbl_eu_80666CE4) func_80174B4C(battleObj, 0x1e);
        } else if (battleObj->bfE0() == 2) {
            if (f31 > lbl_eu_80666CE4) func_80174B4C(battleObj, 0x1e);
        } else {
            if (f31 > lbl_eu_80666CE4) func_80174B4C(battleObj, 0x1e);
        }
        return;
    }

    // Enemy: nothing is in progress - resolve the 0x968 death sequence.
    if (battleObj->bg28() > lbl_eu_80666CE4) return;
    if (func_80174C98(battleObj, &idF, 0x1c) != 0) return;
    if (func_80174C98(battleObj, &idG, 0x1a) != 0) return;
    if (func_80174C98(battleObj, &idH, 0x19) != 0) return;
    if (func_80174C98(battleObj, &idI, 0x04000000) != 0) {
        func_800BE12C(&battleObj->mSub, 5, 0, -1, 1);
        if (battleObj->field_3F60 == 0) func_80174B4C(battleObj, 0x1c);
        return;
    }
    func_80174B4C(battleObj, 0x04000000);
    func_800BE12C(&battleObj->mSub, 5, 0, -1, 1);
    if (battleObj->field_3F60 == 0) func_80174B4C(battleObj, 0x1c);

    // If a live vision target matches, run the removal sequence.
    cf::CfImplEneTarget* tgt =
        (cf::CfImplEneTarget*)cf::CBattleManager::getInstance()->func_800EA444();
    if (!(func_8006EF04(0x04000000) != 0 && tgt != 0 &&
          (tgt->field_04 == battleObj->field_3F10 || tgt->field_00 == battleObj->field_3F10))) {
        func_800BE824(&battleObj->mSub, 0);
        self->vf100();
        func_800F3C6C(cf::CBattleManager::getInstance(), battleObj->field_3F28);
        func_800E9B54(cf::CBattleManager::getInstance(), battleObj, 0, 0);
        func_800D9CA0(cf::CBattleManager::getInstance(), battleObj);
        func_80197BA4(battleObj, r31, ((battleObj->field_3374 >> 5) & 1) ^ 1);
    }

    // Death id 0x968: refresh every tracked unit in the enum list.
    if (battleObj->field_3F28 == 0x968) {
        u8 holder[8];
        func_80043D90(holder);
        func_800F4A98(func_80043F18(holder), 0x80000000, 0);
        for (u32 i = 0; i < ((cf::CfEnumList*)func_80043F18(holder))->field_620; i++) {
            void* obj = func_8016FE34(func_800F6EAC(func_80043F18(holder), i));
            if (obj != 0) {
                ((cf::CfImplEneBattleObj*)obj)->bg18(lbl_eu_80666CE4);
            }
        }
        __dt__80043E88(holder, -1);
    }
}

int func_800D0A58(void* self) { return func_800D0C2C((cf::CfObjectImplEneObj*)self, 0); }

// Plays a sound based on `kind` when the battle object's +0x3F60 id matches.
// Extra params p4/p5/p6 are live-in from the virtual call site; func_800CD5DC
// forwards them (all still in their argument registers).
void func_800D0A60(cf::CfObjectImplEneObj* self, u32 id, u32 kind, u32 p4, u32 p5, u32 p6) {
    if (id != ((cf::CfImplEneBattleObj*)self->field_18)->field_3F60) return;
    func_800CD5DC(self, id, kind, p4, p5, p6);
    switch (kind) {
    case 7:
    case 0xb:
    case 5:
    case 0x31:
    case 2:
        func_801BFE8C(0, 0x1bb, 0);
        break;
    case 0x11:
        cf::CfSoundMan::func_801BFC38(0, 0x1bb, 0, 0, lbl_eu_80666CEC);
        break;
    }
}

extern "C" void func_800CE544(void* self);
extern "C" void func_800CEA34(void* self);
extern "C" void func_800D0AFC(void* self) { func_800CE544(self); }
extern "C" void func_800D0B00(void* self) { func_800CEA34(self); }

// Scans the battle actor's up-to-16 timeline entries; for each entry not
// already tracked by the battle manager, applies the shared action from
// vtable[0x4C] of the own sub-object and counts it.
int func_800D0B04(cf::CfObjectImplEneObj* self) {
    void* base = self->field_18;
    if (base != 0) base = (u8*)base + 0x3e9c;
    void* obj = func_800AD860(base);

    cf::CfObjectImplEneActor* actor = (cf::CfObjectImplEneActor*)func_800B8A64(obj);
    int count = 0;
    if (actor != 0 && (actor->field_A0 & 1) != 0) {
        void* v = ((cf::CfImplEneBattleObj*)self->field_18)->mSub.sf4C();
        void* src = func_8016FE34(func_800B708C((int)v));
        for (int i = 0; i < 0x10; i++) {
            cf::CfImplEneBattleObj* p =
                (cf::CfImplEneBattleObj*)func_800AD860((void*)func_800B708C((int)func_801984E4(actor, i)));
            if (p == 0) continue;
            if (p == (cf::CfImplEneBattleObj*)self->field_18) continue;
            if (func_800DA06C(cf::CBattleManager::getInstance(), (unsigned int)p) != 0) continue;
            if (p->field_3F34 == 0) continue;
            if (p->field_3F60 == 0) continue;
            func_800D9978(cf::CBattleManager::getInstance(), p);
            p->bhC4((int)src, lbl_eu_80666CEC, lbl_eu_80666CE4, lbl_eu_80666CE4);
            count++;
        }
    }
    return count;
}

// Shared battle-object update driven by func_800D00DC: guards on the battle
// manager state and the enemy sub-object, builds a horizontal direction
// toward the current target, and spawns/updates an arrow effect token at
// +0x36C. Returns 1 while engaged, 0 when the guard chain fails.
int func_800D0C2C(cf::CfObjectImplEneObj* self, int flag) {
    if (func_80496288(lbl_eu_80663E14) == lbl_eu_80666CE4) return 0;

    cf::CfImplEneBattleObj* battleObj = (cf::CfImplEneBattleObj*)self->field_18;
    if (battleObj->bhBC() != 0) goto fail;
    if (battleObj->field_3374 & 0x800) goto fail;
    if (func_800DA06C(cf::CBattleManager::getInstance(), (unsigned int)battleObj) == 0) goto fail;

    cf::CfImplEneBattleObj* enemy = (cf::CfImplEneBattleObj*)func_8016FE34(
        func_800B708C((int)battleObj->mSub.sf4C()));
    if (enemy == 0) goto fail;
    if (enemy->field_3F34 == 0) goto fail;
    cf::CfObjectImplEne14* self14 = (cf::CfObjectImplEne14*)self->field_14;
    if (self14->field_98 == 0) goto fail;

    cf::CfImplEnePos* t1 = (cf::CfImplEnePos*)enemy->mSub.sg28();
    cf::CfImplEnePos* t2 = (cf::CfImplEnePos*)self14->v128();
    if (t1 == 0 || t2 == 0) goto fail;

    // Positions of both targets, then the horizontal direction t2 -> t1.
    ml::CVec3 t1pos(t1->field_0C, t1->field_1C, t1->field_2C);
    ml::CVec3 t2pos(t2->field_0C, t2->field_1C, t2->field_2C);
    ml::CVec3 diff(t1pos.x - t2pos.x, t1pos.y - t2pos.y, t1pos.z - t2pos.z);
    ml::CVec3 dir(diff.x, lbl_eu_80666CE4, diff.z);

    // isZero-style guard on the horizontal offset (epsilon from sdata2).
    bool temp = false;
    if (ml::math::abs(dir.x) <= lbl_eu_8066A208 && ml::math::abs(dir.y) <= lbl_eu_8066A208) {
        temp = true;
    }
    bool result = false;
    if (temp && ml::math::abs(dir.z) <= lbl_eu_8066A208) result = true;
    if (result) {
        dir.set(lbl_eu_80666CE4, lbl_eu_80666CE4, lbl_eu_80666CEC);
    } else {
        if (dir.x * dir.x + dir.y * dir.y + dir.z * dir.z == lbl_eu_80666CE4) {
            dir = ml::CVec3::zero;
        } else {
            PSVECNormalize(dir, dir);
        }
    }

    dir.x *= lbl_eu_80666CF0;
    dir.y *= lbl_eu_80666CF0;
    dir.z *= lbl_eu_80666CF0;

    ml::CVec3 v8(t1pos.x - dir.x, t1pos.y - dir.y, t1pos.z - dir.z);
    f32 rot[3] = {v8.x, v8.y, v8.z};
    f32 angle = lbl_eu_80666CF4 * nw4r::math::Atan2FIdx(dir.x, dir.z);

    // Create the direction-arrow effect once per engagement.
    if (self->field_36C == 0 && flag != 0 && self14->field_C4 != 0) {
        void* effect = func_804CC1F4(lbl_eu_8065FC18,
                                     func_80066E7C(func_800630C8(), 0x70100000),
                                     lbl_eu_80663E14, 0xa5, 0, 0);
        self->field_36C = (u32)effect;
        if (effect != 0) {
            void* parent = (u8*)self;
            if (self != 0) parent = (u8*)self + 0x68;
            func_804E3D0C(effect, parent);
        }
    }

    // While engaged, keep the arrow's rotation and mode in sync.
    if (self->field_36C != 0 && flag == 0) {
        CEffectInst* effect = (CEffectInst*)self->field_36C;
        f32 rotScaled[3] = {lbl_eu_80666CE4, angle, lbl_eu_80666CE4};
        effect->mMode =
            (s32)((((cf::CfImplEneField3F34*)(u32)battleObj->field_3F34)->field_7A4 >> 27) & 1);
        effect->mRot[0] = *(s32*)&rot[0];
        effect->mRot[1] = *(s32*)&rot[1];
        effect->mRot[2] = *(s32*)&rot[2];
        effect->mRotScaled[0] = *(s32*)&rotScaled[0];
        effect->mRotScaled[1] = *(s32*)&rotScaled[1];
        effect->mRotScaled[2] = *(s32*)&rotScaled[2];
        f32 dt = func_80484EB0((void*)battleObj->field_3F34);
        func_804E36DC((CSchedule*)effect, dt);
        func_804E3B08(effect);
    }
    return 1;

fail:
    if (self->field_36C != 0) {
        func_804E3CCC((void*)self->field_36C);
        self->field_36C = 0;
    }
    return 0;
}

void func_800D1020(cf::CfObjectImplEneObj* self, u32 arg) {
    u32 v = self->field_36C;
    if (v == arg) {
        void* p = (u8*)self;
        if (self != 0) p = (u8*)self + 0x68;
        func_804E3D48((void*)v, p);
        self->field_36C = 0;
    }
}

cf::CfObjectImplEne::~CfObjectImplEne() {}

void func_800D10B4(void* self) { ((void(*)(void*))func_800D0A58)((char*)self - 0xc); }

void func_800D10BC(void* self) { ((void(*)(void*))__dt__Q22cf15CfObjectImplEneFv)((char*)self - 0xc); }

void func_800D10C4(void* self) { ((void(*)(void*))__dt__Q22cf15CfObjectImplEneFv)((char*)self - 0x10); }

void func_800D10CC(void* self) { ((void(*)(void*))func_800D1020)((char*)self - 0x68); }

void func_800D10D4(void* self) { ((void(*)(void*))__dt__Q22cf15CfObjectImplEneFv)((char*)self - 0x68); }
