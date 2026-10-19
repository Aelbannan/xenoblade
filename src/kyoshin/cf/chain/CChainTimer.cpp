// CChainTimer - chain timer / battle-chain state management.
//
// Implements the chain-state helpers of the kyoshin chain system:
//   ChainTmr_Start_DE44 - start a chain (battle-manager gates)
//   ChainTmr_IsUsable_E070 - battle-object usability check
//   ChainTmr_Update_D20C - per-frame chain-state update (dead-object cleanup, wave)
//   ChainTmr_Advance_D8C4 - chain state-machine advance
//   ChainTmr_ScanUsable_E200 - arts-select usability scan
//
// The chain state object is the cf::UnkClass_800D8DBC member of the battle
// manager; the battle objects are CfImplEneBattleObj-style actors whose
// vtable slots are reached through the CChainBattleObj interface mirror.

#include "kyoshin/cf/chain/CChainTimer.hpp"
#include "kyoshin/cf/CfMapItemManager.hpp"
#include "kyoshin/cf/chain/UnkClass_800D8DBC.hpp"
#include "kyoshin/cf/CVision.hpp" // lbl_eu_80663DA0, aiActionClearBits0006/F0
#include "kyoshin/cf/CBattleManagerApi.hpp"
#include "kyoshin/cf/CArtsSet.hpp"
#include "kyoshin/menu/CMenuArtsSelect.hpp"
#include "kyoshin/cf/object/CfObjectMove.hpp"
#include "kyoshin/cf/CfGameManager.hpp" // cf::CfGameManager::isSceneLoading
#include "kyoshin/cf/chain/CChainActorList.hpp" // lbl_eu_80538338
#include "kyoshin/cf/chain/CChain.hpp" // extern-C checkSlotUsable (retail-unmangled)
#include <math.h>

// Offset-typed view of cf::CBattleManager for the chain-start gates (the full
// class header drags in a conflicting playActorSound declaration chain).
// Byte phase counter at 0x1AA (head of the embedded CChain), sudden-commu
// sub-object at 0x216C.
namespace cf {
struct CBattleManagerChainGate {
    u8 _pad0[0x1AA];
    u8 mChainPhase;                 // 0x1AA = CChain::unk0[2]
    u8 _pad1AB[0x216C - 0x1AB];
    u8 mSuddenCommu;                // 0x216C (address taken)
};
} // namespace cf

// Same-TU helpers (retail symbols ChainTmr_Advance_D8C4 / ChainTmr_SetActive_DB74 /
// ChainTmr_IsUsable_E070 / ChainTmr_ScanUsable_E200 are unmangled, so they are defined with C
// linkage to keep intra-TU call reloc names identical to retail).
// (memberized: ChainTmr_SetActive_DB74 -> CChainState::Tmr_SetActive; declaration moved into class CChainState)
// (memberized: ChainTmr_Advance_D8C4 -> CChainState::Tmr_Advance; declaration moved into class CChainState)
// (memberized: ChainTmr_IsUsable_E070 -> CChainState::Tmr_IsUsable; declaration moved into class CChainState)
// (memberized: ChainTmr_ScanUsable_E200 -> CChainState::Tmr_ScanUsable; declaration moved into class CChainState)

// The member sub-objects construct implicitly before this body (flusher
// vptr/zero, chain-temp memset, chain-time ctor call, chain-timer vptr).
// The body assigns the chain-temp manual vtable, then runs the inlined
// chain reset (ChainTmr_Reset_D1A4 semantics): unk0 is still uninitialized at
// that point, so the runtime branch against it survives like retail.
cf::UnkClass_800D8DBC::UnkClass_800D8DBC() {
    mChainTemp.mVTable = (u32)&lbl_eu_80538338;

    cf::CChainState* self = (cf::CChainState*)this;
    if (self->field_0 != 0) {
        lbl_eu_80663DA0 &= 0xFE;
        self->field_84.resetChainTime();
        self->field_10 = 0;
        self->field_14 = 0;
    }
    self->field_0 = 0;
    self->field_4 = 0;
    self->field_8 = 0;
    self->field_C = 0;
}

// Chain reset: if a chain is active, clear the global chain flag, reset the
// embedded chain timer and wave state; then zero the whole chain state.
extern "C" void ChainTmr_Reset_D1A4(cf::CChainState* self) {
    if (self->field_0 != 0) {
        lbl_eu_80663DA0 &= 0xFE;
        self->field_84.resetChainTime();
        self->field_10 = 0;
        self->field_14 = 0;
    }
    self->field_0 = 0;
    self->field_4 = 0;
    self->field_8 = 0;
    self->field_C = 0;
}

// Per-frame chain-state update. Clears dead battle objects out of the state,
// resets the chain when any died, otherwise advances the state machine and
// drives the chain wave effect.
void ChainTmr_Update_D20C(cf::CChainState* self) {
    if (cf::CfGameManager::isSceneLoading() != 0) return;
    if (self->field_0 == 0) return;

    int changed = 0;
    // Wave interpolation buffers, declared at function scope so their live
    // ranges match the retail allocation.
    f32 s;
    f32 src[3];
    f32 prodT[3];
    f32 dst[3];
    // False arm yields the same value as the true arm when field_4 is null,
    // so MWCC folds the ternary to the retail conditional-addi form.
    if (lookupWorkAtAddr(self->field_4 ? &self->field_4->mSub
                                    : (cf::CChainVoiceSub*)self->field_4) == 0) {
        self->field_4 = 0;
        changed = 1;
    }
    if (lookupWorkAtAddr(self->field_8 ? &self->field_8->mSub
                                    : (cf::CChainVoiceSub*)self->field_8) == 0) {
        self->field_8 = 0;
        changed = 1;
        self->field_10 = 0;
        self->field_14 = 0;
    }
    if (findObjectById((int)self->field_C) == 0) {
        self->field_C = 0;
        changed = 1;
    }

    if (changed) {
        self->Tmr_SetActive(0);
        if (self->field_0 != 0) {
            lbl_eu_80663DA0 &= 0xFE;
            self->field_84.resetChainTime();
            self->field_10 = 0;
            self->field_14 = 0;
        }
        self->field_0 = 0;
        self->field_4 = 0;
        self->field_8 = 0;
        self->field_C = 0;
    }
    if (!changed) {
        // Retail evaluates the chain-timer countdown before advancing the
        // state machine.
        if (self->field_9C.unk0 > 0) self->field_9C.unk0--;
        self->Tmr_Advance();

        if (self->field_10 != 0) {
            // Phase counter wraps at 0x23, then drives a sinusoidal
            // interpolation between the two 3-float colour/scale tables.
            self->field_14++;
            if (self->field_14 >= 0x23) self->field_14 = 0;

            s = sin(lbl_eu_80668A90 * self->field_14 / lbl_eu_80668A94);
            src[0] = lbl_eu_80656C40[0] * s;
            prodT[0] = lbl_eu_80656C64[0] * (lbl_eu_80668A98 - s);
            prodT[2] = lbl_eu_80656C64[2] * (lbl_eu_80668A98 - s);
            src[2] = lbl_eu_80656C40[2] * s;
            dst[2] = src[2] + prodT[2];
            prodT[1] = lbl_eu_80656C64[1] * (lbl_eu_80668A98 - s);
            src[1] = lbl_eu_80656C40[1] * s;
            dst[0] = src[0] + prodT[0];
            dst[1] = src[1] + prodT[1];
            CfModel_GetSpeedRate(&self->field_10->mSub, dst);
        }

        tickChainTimer(&self->field_84);
        if (self->field_4 != 0) self->field_4->v367(lbl_eu_80668A98);
        if (self->field_8 != 0) self->field_8->v367(lbl_eu_80668A98);
    }
}

// Chain-arts gauge advance for a battle object flagged as chain-active
// (field_3E6C bit 12). Adds a counted amount to the action gauge, scaled by
// the arts-slot rate, when the arts id is usable; the count/rate block is
// replicated per arts-id branch (0x4c/0x4d/0x41) exactly as retail lays it
// out.
void ChainTmr_CheckArts_D478(cf::CChainState* self, cf::CChainBattleObj* obj) {
    if ((obj->field_3E6C & 0x1000) == 0) return;

    cf::CChainGaugeAction* action = (cf::CChainGaugeAction*)obj->v164();
    bool artsUnusable = (isArtsUsable(action->field_50->field_48, 1) == 0);
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
            rate = lbl_eu_80668AC0 * (f32)IdTable_SumValues((void*)obj->v162(), 0x4c);
        } else {
            rate = lbl_eu_80668AA8;
        }
        f32 delta = (f32)count * rate;
        action->field_5C += delta;

        f32 rate2;
        if (obj->v162() != 0) {
            rate2 = lbl_eu_80668AC0 * (f32)IdTable_SumValues((void*)obj->v162(), 0x4c);
        } else {
            rate2 = lbl_eu_80668AA8;
        }
        action->field_58 += rate2;

        if (CtrlObjectParam_GetSlotTableBase()[1] == 1) {
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
                rate3 = lbl_eu_80668AC0 * (f32)IdTable_SumValues((void*)obj->v162(), 0x4d);
            } else {
                rate3 = lbl_eu_80668AA8;
            }
            f32 delta2 = (f32)count2 * rate3;
            action->field_5C += delta2;

            f32 rate4;
            if (obj->v162() != 0) {
                rate4 = lbl_eu_80668AC0 * (f32)IdTable_SumValues((void*)obj->v162(), 0x4d);
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
            rate5 = lbl_eu_80668AC0 * (f32)IdTable_SumValues((void*)obj->v162(), 0x41);
        } else {
            rate5 = lbl_eu_80668AA8;
        }
        f32 delta3 = (f32)count3 * rate5;
        action->field_5C += delta3;

        f32 rate6;
        if (obj->v162() != 0) {
            rate6 = lbl_eu_80668AC0 * (f32)IdTable_SumValues((void*)obj->v162(), 0x41);
        } else {
            rate6 = lbl_eu_80668AA8;
        }
        action->field_58 += rate6;
    }
}

// Chain state-machine advance. Revalidates the battle objects (voice actor,
// both chain members) then walks the 1/2/3 state transitions.
void cf::CChainState::Tmr_Advance() {
    int st = 0;
    cf::CChainBattleObj* voice =
        (cf::CChainBattleObj*)MapFx_ResolveOwner(findObjectById((int)this->field_C));
    if (voice == 0) st = 1;
    else if (voice->v173() != 0) st = 1;
    else if (this->field_4->v173() != 0) st = 1;
    else if (this->field_8->v173() != 0) st = 1;
    else if (this->Tmr_IsUsable(this->field_8) == 0) st = 1;
    else st = (this->Tmr_ScanUsable(this->field_8, 0) == 0);
    if (st != 0) this->field_0 = 3;

    resetBattleGauge__Q22cf13CfGameManagerFv();

    cf::CChainBattleObj* o1 = this->field_4;
    u32 id1 = *(u32*)o1->field_04->f30();
    if (testActorStatus(o1, (int*)&id1, 0x4000) == 0)
        dispatchItemLoad(this->field_4, 0x4000);
    cf::CChainBattleObj* o2 = this->field_8;
    u32 id2 = *(u32*)o2->field_04->f30();
    if (testActorStatus(o2, (int*)&id2, 0x4000) == 0)
        dispatchItemLoad(this->field_8, 0x4000);

    if (this->field_4->field_3F60 != 0)
        this->field_4->field_3F60->field_4EC |= 0x1000;
    if (this->field_8->field_3F60 != 0)
        this->field_8->field_3F60->field_4EC |= 0x1000;

    switch (this->field_0) {
    case 1:
        this->field_0++;
        break;
    case 2: {
        cf::CArtsSelectStateView* state =
            (cf::CArtsSelectStateView*)CMenuArtsSelect_getSelectState();
        if (state == 0) break;
        if (state->field_1 == -1) break;
        chainUnbindMatchingObjects(0x7a, (int)(u32)this->field_8);
        if (state->field_0 == 0 && state->field_1 == 5) {
            // Do not advance while the arts-select target battle object is
            // flagged as busy (field_3374 bits 14/15).
            int chk;
            if (this->field_8->field_3F28 != 1) {
                chk = 0;
            } else {
                if ((this->field_8->field_3374 & 0x4000) == 0 &&
                    (this->field_8->field_3374 & 0x8000) == 0) {
                    chk = 0;
                } else {
                    chk = 1;
                }
            }
            if (chk != 0) break;
        }
        this->field_0++;
        break;
    }
    case 3:
        this->Tmr_SetActive(0);
        lbl_eu_80663DA0 &= 0xFE;
        this->field_84.mTimer = lbl_eu_80668A98;
        this->field_84.mEnabled = 0;
        this->field_84.mPaused = 1;
        this->field_0 = 0;
        break;
    }
}

// Chain on/off switch for both chain members. Re-registers the vision/voice
// sub-objects (aiRebootAction/E0 on +0x3380, vtable calls on the embedded
// +0x3E9C voice), refreshes the wave target / arts music table, and toggles
// the arts-select menu.
void cf::CChainState::Tmr_SetActive(int val) {
    if (this->field_4 != 0) {
        if (val != 0) {
            dispatchItemLoad(this->field_4, 0x4000);
            aiRebootAction(&this->field_4->mField3380);
            if (this->field_4->field_3F60 != 0)
                this->field_4->field_3F60->field_4EC |= 0x1000;
        } else {
            aiActionClearBits0006(&this->field_4->mField3380);
            this->field_4->field_04->f06(0x4000);
        }
    }
    if (this->field_8 != 0) {
        if (val != 0) {
            ChainMusic_SaveSlots(&this->mChainTemp, this->field_8);
            this->field_10 = this->field_8;
            this->field_14 = 0;
            this->field_8->v042();
            requestItemLoad(this->field_8, 0x40);
            dispatchItemLoad(this->field_8, 0x4000);
            this->field_8->mSub8.s06(0xeb);
            aiRebootAction(&this->field_8->mField3380);

            // Warn when the voice actor drifts beyond the chain range: the
            // squared distance between the chain target and the voice actor's
            // positions (vtable slot 0xAC of the embedded voice sub-objects).
            // Spelled as VEC3Sub into a local plus a component-wise copy so
            // MWCC keeps both stack vectors and the paired-single square-sum.
            int actorId = (int)this->field_C;
            cf::CChainBattleObj* target = this->field_8;
            cf::CChainBattleObj* voice =
                (cf::CChainBattleObj*)MapFx_ResolveOwner(findObjectById(actorId));
            f32 distSq;
            if (voice == 0) {
                distSq = lbl_eu_80668AC4;
            } else {
                // Component copy into a second vector keeps both stack
                // vectors live so MWCC emits the paired-single square-sum.
                nw4r::math::VEC3 d;
                nw4r::math::VEC3 delta;
                nw4r::math::VEC3Sub(&delta, voice->mSub.v41(),
                                    target->mSub.v41());
                d.x = delta.x;
                d.y = delta.y;
                d.z = delta.z;
                distSq = nw4r::math::VEC3LenSq(&d);
            }
            if (distSq < lbl_eu_80668AC8)
                this->field_8->mSub.v18(this->field_C);
            if (this->field_8->field_3F28 != 5)
                this->field_8->v087();
            else
                this->field_8->v082(lbl_eu_80668AA8);
            if (this->field_8->field_3F60 != 0)
                this->field_8->field_3F60->field_4EC |= 0x1000;
        } else {
            aiActionClearBits0006(&this->field_8->mField3380);
            this->field_8->field_04->f06(0x4000);
            if (this->field_10 != 0)
                CfModel_GetSpeedRate(&this->field_10->mSub, lbl_eu_80656C40);
            this->field_10 = 0;
            this->field_14 = 0;
            ChainMusic_RestoreSlots(&this->mChainTemp, this->field_8);
        }
    }
    if (val != 0) {
        syncBattleState__Q22cf13CfGameManagerFv(this->field_8 ? &this->field_8->mSub : 0);
        CMenuArtsSelect_setDisabled();
        CMenuArtsSelect_ResetSlotAnims();
    } else {
        processFieldEffects__Q22cf13CfGameManagerFv();
        CMenuArtsSelect_setDisabled();
    }
}

// Starts a chain on the state object with the two battle objects and actor
// id, gated by the battle-manager state and the sudden-commu flag.
int ChainTmr_Start_DE44(cf::CChainState* self, cf::CChainBattleObj* p1,
                  cf::CChainBattleObj* p2, u32 p3) {
    int ok;
    if (self->field_0 != 0) {
        ok = 0;
    } else {
        // Battle-phase gate: phases 1..0x18 block chain starts.
        // NOTE (wall class 13): retail keeps the two standalone cmplwi
        // compares (cmplwi 1/blt + cmplwi 0x18/bgt); every Wii/1.1 spelling
        // tried so far folds to addi/rlwinm/cmpli ((u8)(v-1)<=0x17) - see
        // isTypeId1to24 / CamEvtBusyChk notes. Needs the configure.py
        // per-unit version split; switching this TU regresses its 5
        // Wii/1.1 FULL_MATCHes.
        // Retail calls the singleton getter once per gate (reload into r3).
        u8 v = ((cf::CBattleManagerChainGate*)getInstance__Q22cf14CBattleManagerFv())
                   ->mChainPhase;
        // Battle-phase gate: phases 1..0x18 block chain starts.
        // NOTE (wall class 13): retail keeps two standalone cmplwi compares;
        // Wii/1.1 folds every spelling tried to addi/rlwinm/cmpli -- needs the
        // configure.py GC-version split (cf. isTypeId1to24 notes).
        int inRange = 0;
        if (v >= 1) {
            if (v <= 0x18)
                inRange = 1;
        }
        if (inRange != 0) {
            ok = 0;
        } else {
            int commu = SuddenCommuIsStateActive(
                &((cf::CBattleManagerChainGate*)getInstance__Q22cf14CBattleManagerFv())
                     ->mSuddenCommu);
            ok = (commu == 0);
        }
    }
    if (ok == 0) return 0;

    self->field_0 = 1;
    self->field_4 = p1;
    self->field_8 = p2;
    self->field_C = p3;
    lbl_eu_80663DA0 |= 1;
    startChainTimer(&self->field_84, (p1->field_3F00 >> 1) & 1);
    self->field_84.mTimer = lbl_eu_80668AA8;
    self->field_84.mEnabled = 1;
    self->field_84.mPaused = 1;
    self->Tmr_SetActive(1);
    return 1;
}

// Chain-target arts gate: with an active chain, when flag==0 scan arts slot
// cnt - if it passes checkArtsFull and its id is not 2 the chain is usable;
// otherwise only fail while the chain state is still in state 2.
extern "C" int ChainTmr_GateArts_DF38(cf::CChainState* self, cf::CChainBattleObj* obj,
                             int flag, int cnt) {
    if (self->field_0 == 0) return 0;

    int res = 0;
    if (flag == 0) {
        if (cnt >= 0) {
            if (cnt < 8) {
                cf::CAttackParam* artsParam =
                    (cf::CAttackParam*)getArtsParamAtCnt(obj->v157(), cnt);
                if (checkArtsFull(artsParam, obj, -2) == 0)
                    res = (artsParam->unk5E == 2);
                else
                    res = 0;
            }
        }
        if (res != 0) return 1;
    }
    if (self->field_0 == 2) return 0;
    return 1;
}

// Chain-start usability gate: only allow starting a chain on obj when it
// passes the battle-object usability check.
extern "C" int ChainTmr_StartUsable_E018(cf::CChainState* self, cf::CChainBattleObj* obj) {
    if (self->Tmr_IsUsable(obj) == 0) return 0;
    return self->Tmr_ScanUsable(obj, 1);
}

// Battle-object usability check: chainable (vtable 0x2bc), owns the actor id
// flags (testActorStatus), and in arts-select mode is not on cooldown
// (vtable 0x158/0x15c timers).
int cf::CChainState::Tmr_IsUsable(cf::CChainBattleObj* obj) {
    if (obj->v173() != 0) return 0;
    u32 id1 = *(u32*)obj->field_04->f30();
    if (testActorStatus(obj, (int*)&id1, 0x803) == 0) return 0;
    u32 id2 = *(u32*)obj->field_04->f30();
    if (testActorStatus(obj, (int*)&id2, 0x1a) != 0) return 0;
    if (obj->field_3F28 == 5) {
        if (isBattleStatusActive((u8*)&obj->mSub8, 0xf0) != 0) return 0;
        if (isBattleStatusActive((u8*)&obj->mSub8, 0xf1) != 0) return 0;
        u32 id3 = *(u32*)obj->field_04->f30();
        if (testActorStatus(obj, (int*)&id3, 0xb) != 0) return 0;
        f32 a = obj->v084();
        f32 b = obj->v085();
        if (b <= a) return 0;
    }
    return 1;
}

// Arts-select usability scan: returns 1 when the first selectable arts slot
// (through the menu, or the battle object directly) passes all gates.
// Per-slot pipeline: menu slot check (state-2 path only), busy-slot skip
// (slot 8 while field_3F28 == 5), a pre-check that drops slots whose arts
// param is flagged (id == 2 via checkArtsFull -2), then the real usage check
// with the caller flag (slot 8 uses the RC arts param).
int cf::CChainState::Tmr_ScanUsable(cf::CChainBattleObj* obj, int flag) {
    // Declared first: MWCC pins it to r30 across both loops like retail.
    cf::CAttackParam* artsParam;
    int f = 0x22;
    if (flag) f |= 0x10;

    if (this->field_0 == 2) {
        CMenuArtsSelect* menu = CMenuArtsSelect_getInstance();
        if (menu == 0) return 1;
        for (int i = 0; i <= 8; i++) {
            if (checkSlotUsable(menu, i) != 0) continue;
            // Init-then-flag form: retail emits li r3,0 + conditional set.
            int menuSkip = 0;
            if (obj->field_3F28 == 5 && i == 8) menuSkip = 1;
            if (menuSkip != 0) continue;
            // Out-of-range arm spelled as if/else with ||. NOTE (wall class
            // 13): retail keeps two separate signed compares (cmpi 0/blt,
            // cmpi 8/blt); the Wii/1.1 compiler locked for this unit always
            // folds the range test to cmpli 7/bgt - cf. the ChainTmr_Start_DE44
            // version-split note above.
            int skip;
            if (i < 0 || i >= 8) {
                skip = 0;
            } else {
                artsParam = (cf::CAttackParam*)getArtsParamAtCnt(obj->v157(), i);
                if (checkArtsFull(artsParam, obj, -2) != 0)
                    skip = 0;
                else
                    skip = (artsParam->unk5E == 2);
            }
            if (skip != 0) continue;
            int res;
            if (i < 8) {
                res = checkArtsFull(
                    (cf::CAttackParam*)getArtsParamAtCnt(obj->v157(), i), obj, f);
            } else if (i == 8) {
                res = checkArtsFull(
                    (cf::CAttackParam*)getArtsParamRC(obj->v157(), 2, 0), obj, f);
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
        if (i < 0 || i >= 8) {
            skip = 0;
        } else {
            artsParam = (cf::CAttackParam*)getArtsParamAtCnt(obj->v157(), i);
            if (checkArtsFull(artsParam, obj, -2) != 0)
                skip = 0;
            else
                skip = (artsParam->unk5E == 2);
        }
        if (skip != 0) continue;
        int res;
        if (i < 8) {
            res = checkArtsFull(
                (cf::CAttackParam*)getArtsParamAtCnt(obj->v157(), i), obj, f);
        } else if (i == 8) {
            res = checkArtsFull(
                (cf::CAttackParam*)getArtsParamRC(obj->v157(), 2, 0), obj, f);
        } else {
            res = 0;
        }
        if (res == 0) return 1;
    }
    return 0;
}
