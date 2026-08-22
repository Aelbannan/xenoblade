// CChainTimer - chain timer / battle-chain state management.
//
// Implements the chain-state helpers of the kyoshin chain system:
//   func_8027DE44 - start a chain (battle-manager gates)
//   func_8027E070 - battle-object usability check
//   func_8027D20C - per-frame chain-state update (dead-object cleanup, wave)
//   func_8027D8C4 - chain state-machine advance
//   func_8027E200 - arts-select usability scan
//
// The chain state object is the cf::UnkClass_800D8DBC member of the battle
// manager; the battle objects are CfImplEneBattleObj-style actors whose
// vtable slots are reached through the CChainBattleObj interface mirror.

#include "kyoshin/cf/chain/CChainTimer.hpp"
#include "kyoshin/cf/chain/UnkClass_800D8DBC.hpp"
#include "kyoshin/cf/CBattleManager.hpp"
#include "kyoshin/cf/CArtsSet.hpp"
#include "kyoshin/menu/CMenuArtsSelect.hpp"
#include "kyoshin/cf/object/CfObjectMove.hpp" // func_800829B8__Q22cf13CfGameManagerFv
#include <math.h>

// Same-TU helpers (retail symbols func_8027D8C4 / func_8027DB74 /
// func_8027E070 / func_8027E200 are unmangled, so they are defined with C
// linkage to keep intra-TU call reloc names identical to retail).
extern "C" void func_8027DB74(cf::CChainState* self, int val);
extern "C" void func_8027D8C4(cf::CChainState* self);
extern "C" int func_8027E070(cf::CChainState* self, cf::CChainBattleObj* obj);
extern "C" int func_8027E200(cf::CChainState* self, cf::CChainBattleObj* obj, int flag);

cf::UnkClass_800D8DBC::UnkClass_800D8DBC() {}

void func_8027D1A4(){}

// Per-frame chain-state update. Clears dead battle objects out of the state,
// resets the chain when any died, otherwise advances the state machine and
// drives the chain wave effect.
void func_8027D20C(cf::CChainState* self) {
    if (func_800829B8__Q22cf13CfGameManagerFv() != 0) return;
    if (self->field_0 == 0) return;

    int changed = 0;
    cf::CChainBattleObj* p = self->field_4;
    if (func_800B8920(p ? &p->mSub : 0) == 0) {
        self->field_4 = 0;
        changed = 1;
    }
    p = self->field_8;
    if (func_800B8920(p ? &p->mSub : 0) == 0) {
        self->field_8 = 0;
        changed = 1;
        self->field_10 = 0;
        self->field_14 = 0;
    }
    if (func_800B708C((int)self->field_C) == 0) {
        self->field_C = 0;
        changed = 1;
    }

    if (changed) {
        func_8027DB74(self, 0);
        if (self->field_0 != 0) {
            lbl_eu_80663DA0 &= 0xFE;
            self->field_84.func_8027CE30();
            self->field_10 = 0;
            self->field_14 = 0;
        }
        self->field_0 = 0;
        self->field_4 = 0;
        self->field_8 = 0;
        self->field_C = 0;
    } else {
        func_8027D8C4(self);
        if (self->field_9C.unk0 > 0) self->field_9C.unk0--;

        if (self->field_10 != 0) {
            // Phase counter wraps at 0x23, then drives a sinusoidal
            // interpolation between the two 3-float colour/scale tables.
            self->field_14++;
            if (self->field_14 >= 0x23) self->field_14 = 0;

            f32 s = sin(lbl_eu_80668A90 * (f32)self->field_14 / lbl_eu_80668A94);
            f32 t = lbl_eu_80668A98 - s;
            f32 wave[3];
            wave[0] = lbl_eu_80656C40[0] * s + lbl_eu_80656C64[0] * t;
            wave[1] = lbl_eu_80656C40[1] * s + lbl_eu_80656C64[1] * t;
            wave[2] = lbl_eu_80656C40[2] * s + lbl_eu_80656C64[2] * t;
            func_800BBA7C(&self->field_10->mSub, wave);
        }

        func_8027CF3C(&self->field_84);
        if (self->field_4 != 0) self->field_4->v367(lbl_eu_80668A98);
        if (self->field_8 != 0) self->field_8->v367(lbl_eu_80668A98);
    }
}

// Chain-arts gauge advance for a battle object flagged as chain-active
// (field_3E6C bit 12). Adds a counted amount to the action gauge, scaled by
// the arts-slot rate, when the arts id is usable; the count/rate block is
// replicated per arts-id branch (0x4c/0x4d/0x41) exactly as retail lays it
// out.
void func_8027D478(cf::CChainState* self, cf::CChainBattleObj* obj) {
    if ((obj->field_3E6C & 0x1000) == 0) return;

    cf::CChainGaugeAction* action = (cf::CChainGaugeAction*)obj->v164();
    bool artsUnusable = (func_80146300(action->field_50->field_48, 1) == 0);
    if (artsUnusable) {
        f32 sum = action->field_5C + action->field_60;
        int count;
        if (sum < lbl_eu_80668AA8) {
            count = 0;
        } else if ((action->field_74 & 0x80) != 0) {
            count = 0;
        } else {
            f64 bias = (sum > lbl_eu_80668AA8) ? lbl_eu_80668AB0 : lbl_eu_80668AB8;
            count = (int)(sum + bias);
        }
        f32 rate;
        if (obj->v162() != 0) {
            rate = lbl_eu_80668AC0 * (f32)func_8025FB10(obj->v162(), 0x4c);
        } else {
            rate = lbl_eu_80668AA8;
        }
        f32 delta = (f32)count * rate;
        action->field_5C += delta;

        f32 rate2;
        if (obj->v162() != 0) {
            rate2 = lbl_eu_80668AC0 * (f32)func_8025FB10(obj->v162(), 0x4c);
        } else {
            rate2 = lbl_eu_80668AA8;
        }
        action->field_58 += rate2;

        if (func_8009ECB0()[1] == 1) {
            f32 sum2 = action->field_5C + action->field_60;
            int count2;
            if (sum2 < lbl_eu_80668AA8) {
                count2 = 0;
            } else if ((action->field_74 & 0x80) != 0) {
                count2 = 0;
            } else {
                f64 bias2 = (sum2 > lbl_eu_80668AA8) ? lbl_eu_80668AB0 : lbl_eu_80668AB8;
                count2 = (int)(sum2 + bias2);
            }
            f32 rate3;
            if (obj->v162() != 0) {
                rate3 = lbl_eu_80668AC0 * (f32)func_8025FB10(obj->v162(), 0x4d);
            } else {
                rate3 = lbl_eu_80668AA8;
            }
            f32 delta2 = (f32)count2 * rate3;
            action->field_5C += delta2;

            f32 rate4;
            if (obj->v162() != 0) {
                rate4 = lbl_eu_80668AC0 * (f32)func_8025FB10(obj->v162(), 0x4d);
            } else {
                rate4 = lbl_eu_80668AA8;
            }
            action->field_58 += rate4;
        }
    } else {
        f32 sum3 = action->field_5C + action->field_60;
        int count3;
        if (sum3 < lbl_eu_80668AA8) {
            count3 = 0;
        } else if ((action->field_74 & 0x80) != 0) {
            count3 = 0;
        } else {
            f64 bias3 = (sum3 > lbl_eu_80668AA8) ? lbl_eu_80668AB0 : lbl_eu_80668AB8;
            count3 = (int)(sum3 + bias3);
        }
        f32 rate5;
        if (obj->v162() != 0) {
            rate5 = lbl_eu_80668AC0 * (f32)func_8025FB10(obj->v162(), 0x41);
        } else {
            rate5 = lbl_eu_80668AA8;
        }
        f32 delta3 = (f32)count3 * rate5;
        action->field_5C += delta3;

        f32 rate6;
        if (obj->v162() != 0) {
            rate6 = lbl_eu_80668AC0 * (f32)func_8025FB10(obj->v162(), 0x41);
        } else {
            rate6 = lbl_eu_80668AA8;
        }
        action->field_58 += rate6;
    }
}

// Chain state-machine advance. Revalidates the battle objects (voice actor,
// both chain members) then walks the 1/2/3 state transitions.
extern "C" void func_8027D8C4(cf::CChainState* self) {
    int st = 0;
    cf::CChainBattleObj* voice =
        (cf::CChainBattleObj*)func_8016FE34(func_800B708C((int)self->field_C));
    if (voice == 0) st = 1;
    else if (voice->v173() != 0) st = 1;
    else if (self->field_4->v173() != 0) st = 1;
    else if (self->field_8->v173() != 0) st = 1;
    else if (func_8027E070(self, self->field_8) == 0) st = 1;
    else st = (func_8027E200(self, self->field_8, 0) == 0);
    if (st != 0) self->field_0 = 3;

    func_80082A0C__Q22cf13CfGameManagerFv();

    cf::CChainBattleObj* o1 = self->field_4;
    u32 id1 = *(u32*)o1->field_04->f30();
    if (func_80174C98(o1, (int*)&id1, 0x4000) == 0)
        func_80174B4C(self->field_4, 0x4000);
    cf::CChainBattleObj* o2 = self->field_8;
    u32 id2 = *(u32*)o2->field_04->f30();
    if (func_80174C98(o2, (int*)&id2, 0x4000) == 0)
        func_80174B4C(self->field_8, 0x4000);

    if (self->field_4->field_3F60 != 0)
        self->field_4->field_3F60->field_4EC |= 0x1000;
    if (self->field_8->field_3F60 != 0)
        self->field_8->field_3F60->field_4EC |= 0x1000;

    switch (self->field_0) {
    case 1:
        self->field_0++;
        break;
    case 2: {
        cf::CArtsSelectStateView* state =
            (cf::CArtsSelectStateView*)CMenuArtsSelect_getSelectState();
        if (state == 0) break;
        if (state->field_1 == -1) break;
        func_802A0818(0x7a, (int)(u32)self->field_8);
        if (state->field_0 == 0 && state->field_1 == 5) {
            // Do not advance while the arts-select target battle object is
            // flagged as busy (field_3374 bits 14/15).
            int chk;
            if (self->field_8->field_3F28 != 1) {
                chk = 0;
            } else {
                if ((self->field_8->field_3374 & 0x4000) == 0 &&
                    (self->field_8->field_3374 & 0x8000) == 0) {
                    chk = 0;
                } else {
                    chk = 1;
                }
            }
            if (chk != 0) break;
        }
        self->field_0++;
        break;
    }
    case 3:
        func_8027DB74(self, 0);
        lbl_eu_80663DA0 &= 0xFE;
        self->field_84.mTimer = lbl_eu_80668A98;
        self->field_84.mEnabled = 0;
        self->field_84.mPaused = 1;
        self->field_0 = 0;
        break;
    }
}

// Chain on/off switch for both chain members. Re-registers the vision/voice
// sub-objects (func_801537F0/E0 on +0x3380, vtable calls on the embedded
// +0x3E9C voice), refreshes the wave target / arts music table, and toggles
// the arts-select menu.
extern "C" void func_8027DB74(cf::CChainState* self, int val) {
    if (self->field_4 != 0) {
        if (val != 0) {
            func_80174B4C(self->field_4, 0x4000);
            func_801537F0(&self->field_4->mField3380);
            if (self->field_4->field_3F60 != 0)
                self->field_4->field_3F60->field_4EC |= 0x1000;
        } else {
            func_801537E0(&self->field_4->mField3380);
            self->field_4->field_04->f06(0x4000);
        }
    }
    if (self->field_8 != 0) {
        if (val != 0) {
            func_8027CC3C(&self->mChainTemp, self->field_8);
            self->field_10 = self->field_8;
            self->field_14 = 0;
            self->field_8->v042();
            func_80174C24(self->field_8, 0x40);
            func_80174B4C(self->field_8, 0x4000);
            self->field_8->mSub8.s06(0xeb);
            func_801537F0(&self->field_8->mField3380);

            // Warn when the voice actor drifts beyond the chain range: the
            // squared distance between the chain target and the voice actor's
            // positions (vtable slot 0xAC of the embedded voice sub-objects).
            int actorId = (int)self->field_C;
            cf::CChainBattleObj* target = self->field_8;
            cf::CChainBattleObj* voice =
                (cf::CChainBattleObj*)func_8016FE34(func_800B708C(actorId));
            f32 distSq;
            if (voice == 0) {
                distSq = lbl_eu_80668AC4;
            } else {
                nw4r::math::VEC3* p1 = target->mSub.v41();
                nw4r::math::VEC3* p2 = voice->mSub.v41();
                // SDK inline VEC3Sub emits the retail paired-single
                // subtraction; the squared length is a plain scalar dot that
                // MWCC -O4 vectorizes.
                nw4r::math::VEC3 d;
                nw4r::math::VEC3Sub(&d, p2, p1);
                distSq = d.x * d.x + d.y * d.y + d.z * d.z;
            }
            if (distSq < lbl_eu_80668AC8)
                self->field_8->mSub.v18(self->field_C);
            if (self->field_8->field_3F28 != 5)
                self->field_8->v087();
            else
                self->field_8->v082(lbl_eu_80668AA8);
            if (self->field_8->field_3F60 != 0)
                self->field_8->field_3F60->field_4EC |= 0x1000;
        } else {
            func_801537E0(&self->field_8->mField3380);
            self->field_8->field_04->f06(0x4000);
            if (self->field_10 != 0)
                func_800BBA7C(&self->field_10->mSub, lbl_eu_80656C40);
            self->field_10 = 0;
            self->field_14 = 0;
            func_8027CD08(&self->mChainTemp, self->field_8);
        }
    }
    if (val != 0) {
        func_80082A7C__Q22cf13CfGameManagerFv(self->field_8 ? &self->field_8->mSub : 0);
        CMenuArtsSelect_setDisabled();
        func_801043BC();
    } else {
        func_80082B38__Q22cf13CfGameManagerFv();
        CMenuArtsSelect_setDisabled();
    }
}

// Starts a chain on the state object with the two battle objects and actor
// id, gated by the battle-manager state and the sudden-commu flag.
int func_8027DE44(cf::CChainState* self, cf::CChainBattleObj* p1,
                  cf::CChainBattleObj* p2, u32 p3) {
    int ok;
    if (self->field_0 != 0) {
        ok = 0;
    } else {
        int v = cf::CBattleManager::getInstance()->mChain.unk0[2];
        int inRange = 0;
        if (v >= 1 && v <= 0x18) inRange = 1;
        if (inRange != 0) {
            ok = 0;
        } else {
            ok = (func_801BA2C8(
                      &cf::CBattleManager::getInstance()->mSuddenCommu) == 0);
        }
    }
    if (ok == 0) return 0;

    self->field_0 = 1;
    self->field_4 = p1;
    self->field_8 = p2;
    self->field_C = p3;
    lbl_eu_80663DA0 |= 1;
    func_8027CEB0(&self->field_84, (u8)((p1->field_3F00 >> 1) & 1));
    self->field_84.mTimer = lbl_eu_80668AA8;
    self->field_84.mEnabled = 1;
    self->field_84.mPaused = 1;
    func_8027DB74(self, 1);
    return 1;
}

void func_8027DF38(){}

void func_8027E018(){}

// Battle-object usability check: chainable (vtable 0x2bc), owns the actor id
// flags (func_80174C98), and in arts-select mode is not on cooldown
// (vtable 0x158/0x15c timers).
extern "C" int func_8027E070(cf::CChainState* self, cf::CChainBattleObj* obj) {
    if (obj->v173() != 0) return 0;
    u32 id1 = *(u32*)obj->field_04->f30();
    if (func_80174C98(obj, (int*)&id1, 0x803) == 0) return 0;
    u32 id2 = *(u32*)obj->field_04->f30();
    if (func_80174C98(obj, (int*)&id2, 0x1a) != 0) return 0;
    if (obj->field_3F28 == 5) {
        if (func_80148778((u8*)obj + 8, 0xf0) != 0) return 0;
        if (func_80148778(&((u8*)obj)[8], 0xf1) != 0) return 0;
        u32 id3 = *(u32*)obj->field_04->f30();
        if (func_80174C98(obj, (int*)&id3, 0xb) != 0) return 0;
        f32 a = obj->v084();
        f32 b = obj->v085();
        if (b <= a) return 0;
    }
    return 1;
}

// Arts-select usability scan: returns 1 when the first selectable arts slot
// (through the menu, or the battle object directly) passes all gates.
extern "C" int func_8027E200(cf::CChainState* self, cf::CChainBattleObj* obj, int flag) {
    int f = 0x22;
    if (flag) f |= 0x10;

    if (self->field_0 == 2) {
        CMenuArtsSelect* menu = CMenuArtsSelect_getInstance();
        if (menu == 0) return 1;
        for (int i = 0; i <= 8; i++) {
            if (menu->func_80107C54(i) != 0) continue;
            int menuSkip = 0;
            if (obj->field_3F28 == 5 && i == 8) menuSkip = 1;
            if (menuSkip != 0) continue;
            int skip;
            if (i >= 0 && i < 8) {
                cf::CAttackParam* artsParam =
                    (cf::CAttackParam*)getArtsParamAtCnt(obj->v157(), i);
                if (func_80154280(artsParam, obj, -2) != 0)
                    skip = 0;
                else
                    skip = (artsParam->unk5E == 2);
            } else {
                skip = 0;
            }
            if (skip != 0) continue;
            int res;
            if (i < 8) {
                cf::CAttackParam* artsParam =
                    (cf::CAttackParam*)getArtsParamAtCnt(obj->v157(), i);
                res = func_80154280(artsParam, obj, f);
            } else if (i == 8) {
                cf::CAttackParam* artsParam =
                    (cf::CAttackParam*)getArtsParamRC(obj->v157(), 2, 0);
                res = func_80154280(artsParam, obj, f);
            } else {
                res = 0;
            }
            if (res == 0) return 1;
        }
        return 0;
    }

    for (int i = 0; i <= 8; i++) {
        int menuSkip = 0;
        if (obj->field_3F28 == 5 && i == 8) menuSkip = 1;
        if (menuSkip != 0) continue;
        int skip;
        if (i >= 0 && i < 8) {
            cf::CAttackParam* artsParam =
                (cf::CAttackParam*)getArtsParamAtCnt(obj->v157(), i);
            if (func_80154280(artsParam, obj, -2) != 0)
                skip = 0;
            else
                skip = (artsParam->unk5E == 2);
        } else {
            skip = 0;
        }
        if (skip != 0) continue;
        int res;
        if (i < 8) {
            cf::CAttackParam* artsParam =
                (cf::CAttackParam*)getArtsParamAtCnt(obj->v157(), i);
            res = func_80154280(artsParam, obj, f);
        } else if (i == 8) {
            cf::CAttackParam* artsParam =
                (cf::CAttackParam*)getArtsParamRC(obj->v157(), 2, 0);
            res = func_80154280(artsParam, obj, f);
        } else {
            res = 0;
        }
        if (res == 0) return 1;
    }
    return 0;
}
