// Auto-scaffolded catalog TU for kyoshin/cf/CtrlAct
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/cf/CBattleManagerApi.hpp"
#include "kyoshin/cf/object/CfObjectMoveApi.hpp"
#include "kyoshin/cf/CfMapItemManager.hpp"
#include "kyoshin/harness_catalog.hpp"
#include "monolib/scn/CScnTimeApi.hpp"

#include "kyoshin/cf/CtrlAct.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

// Polymorphic view of cf::CAttackParam (retail: 0x84 bytes of data, vptr at
// +0x84, virtual slot +0x0C = the gauge-max getter used by UnkVirtualFunc4).
// The shared CArtsSet.hpp declaration is non-virtual/void, which cannot
// reproduce the retail r12 ABI dispatch, so this TU uses its own view.
struct CAttackParamDataView {
    u8 _00[0x30];
    f32 unk30;                 // 0x30
    u8 _34[0x70 - 0x34];
    u8 unk70;                  // 0x70 count byte (unk6C[4])
    u8 _71[0x84 - 0x71];
};
namespace cf {
class CAttackParam : private CAttackParamDataView {
public:
    virtual void vf00();       // RTTI padding: lands at vtable +0x08
    virtual int getMax();      // +0x0C
    float CAttackParam_UnkVirtualFunc4();
};
} // namespace cf

// Target us-800d1bc4. CtrlActView-family base constructor: clear the state
// words, run the two vtable writes (base then derived, novtable convention),
// resolve the sub-object via func_800BBC0C and the player via func_8016FE34,
// zero the 0x2C position block and the tail bytes.
// Retail signature has a third (unused) argument; it affects MWCC's
// saved-register coloring (see CtrlPc.hpp declaration).
// Retail signature has a third (unused) argument. Dropping it does not
// change codegen (verified); kept for ABI fidelity with the CtrlPc.hpp /
// CtrlEnemy.hpp declarations.
CtrlActView* __ct__800D10DC(CtrlActView* self, void* posObj, void* arg5) {
    self->mField4 = 0;
    self->mField8 = lbl_eu_80666CF8;
    self->mFieldC = lbl_eu_80666CF8;
    self->mField10 = lbl_eu_80666CF8;
    self->mField14 = lbl_eu_80666CF8;
    self->mField18 = 0;
    self->mField1C = 2;
    self->mField24 = -1;
    *(void**)self = (void*)lbl_eu_80527BB0;   // base vtable
    self->mField28 = (CtrlActViewSub28*)func_800BBC0C(posObj);
    self->mField2C = 0;
    *(void**)self = (void*)lbl_eu_8052B080;   // derived vtable
    memset(&self->mPos30, 0, 0x2c);
    self->mPlayer = (CtrlActPlayerView*)func_8016FE34(posObj);
    self->mField78 = 0;
    self->mField79 = 0;
    self->mField74 = 0;
    self->mField76 = 0;
    self->mField7A = 0;
    self->mField7B = 0;
    self->mField70 = lbl_eu_80666CFC;
    return self;
}

// Target us-800d1c98. Player-facing action drive: gate on the battle-manager
// range and the voice-owner state, resolve the action source (the player
// itself when the 0x10 control bit and the 0x3388 bit 4 latch are live),
// scan the battle list for the lowest probe height, then blend the anchor
// position, pick the clearance margin and drive the facing/positioning
// probes (func_800D69D8 / func_800D755C / atan2 aim).
extern "C" void func_800D11B0(CtrlActView* self) {
    // Long-lived locals declared first so MWCC's saved-register coloring
    // matches retail: src->r31, r30->r30, r29->r29, self->r28 (param),
    // player-cache->r27. The gate flag below stays volatile (r0).
    CtrlActSrc* src;
    int r30;
    int r29;
    // Battle-state gate: when the battle-manager range byte is outside
    // [1, 0x18], the action is only kept alive while the voice-owner height
    // probe reads exactly 0 (otherwise the source resolution below runs).
    {
        u8 rangeByte =
            ((CtrlActBmView*)getInstance__Q22cf14CBattleManagerFv())
                ->mField1AA;
        if ((rangeByte >= 1 && rangeByte <= 0x18) == 0) {
            CtrlActVoiceOwnerView* vo =
                (CtrlActVoiceOwnerView*)&self->mPlayer->mSub3E9C;
            if (vo->vtbl->fn_0x8C(&self->mPlayer->mSub3E9C) ==
                lbl_eu_80666CF8) {
                self->mField14 = lbl_eu_80666CF8;
                return;
            }
        }
    }
    // Action-source resolution: the 0x10 control bit plus the player's
    // 0x3388 bit 4 latch make the source the player itself (r30=0), else the
    // source is resolved from the voice-owner handle and the latch cleared.
    // Retail initializes the flag register here (li r30,1 before the test).
    r30 = 1;
    if (self->mField74 & 0x10) {
        self->mField7A++;
        if ((self->mPlayer->mField3388 & 0x10) && (u8)self->mField7A <= 0x1e) {
            void* p = cf::CfGameManager::getPlayer(0);
            if (p != 0) {
                p = (u8*)p - 0x3E9C;
            }
            src = (CtrlActSrc*)p;
            r30 = 0;
        } else {
            src = (CtrlActSrc*)func_8016FE34(
                func_800B708C((int)(intptr_t)self->mPlayer->mSub3E9C.v17()));
            self->mField7A = 0;
            self->mField74 &= ~0x10;
        }
    } else {
        src = (CtrlActSrc*)func_8016FE34(
            func_800B708C((int)(intptr_t)self->mPlayer->mSub3E9C.v17()));
    }
    if (src == 0) {
        return;
    }
    r29 = 0;
    // Probe the player state words; any failure latches r29 (the "stuck"
    // flag that clears the state value at the end). Retail re-caches the
    // player pointer into r27 at each func_80174C98 probe site (lwz r27,
    // 0x5c(r28)) and uses a fresh self->mPlayer for the arts probes.
    {
        CtrlActPlayerView* player = self->mPlayer;
        u32 local18 = *player->mField4->vf30();
        if (func_80174C98(player, &local18, 0x4000) != 0) {
            r29 = 1;
        } else {
            CtrlActPlayerView* player2 = self->mPlayer;
            u32 local14 = *player2->mField4->vf30();
            if (func_80174C98(player2, &local14, 0xa) != 0) {
                r29 = 1;
            } else if (func_80148778(&self->mPlayer->mField8, 6) != 0) {
                r29 = 1;
            } else if (func_80148778(&self->mPlayer->mField8, 0xcd) != 0) {
                r29 = 1;
            }
        }
    }
    if (self->mPlayer->vtbl->fn_0x2BC(self->mPlayer) == 0
        && ((CtrlActVoiceOwnerView*)&self->mPlayer->mSub3E9C)
                   ->vtbl->fn_0x140(&self->mPlayer->mSub3E9C) !=
               lbl_eu_80666CF8) {
        // Height picker: scan the battle list for the lowest probe height,
        // falling back to the player's own height probes; the source radius
        // is added on later. Declaration order steers the FPR colors to
        // match retail: radius->f31, height->f30, pf temp->f29, mag->f28,
        // margin->f27.
        f32 f31 = src->mField44D8;
        f32 f30 = lbl_eu_80666D00;
        f32 pf;
        f32 f27;
        if (r30 != 0) {
            if (self->mPlayer->mField3594 != 0) {
                // Retail reuses the source-flag register (r30) as the battle
                // list loop counter, so both roles share one variable here.
                for (r30 = 0; r30 < self->mPlayer->mField3594; r30++) {
                    CtrlActBattleEntry* e =
                        (CtrlActBattleEntry*)((u8*)self->mPlayer->mField358C +
                            ((self->mPlayer->mField3590 + r30) %
                             self->mPlayer->mField3598) *
                                0x20);
                    if (e->mField18 != 0) {
                        if (e->mFieldD == 3) {
                            float* h = self->mPlayer->vtbl->fn_0x1C0(
                                self->mPlayer);
                            if (f30 > h[0]) {
                                f30 = self->mPlayer->vtbl->fn_0x1C0(
                                    self->mPlayer)[0];
                            }
                        } else {
                            f32 h =
                                ((CtrlActBattleEntryObj*)e->mField18)->mField60;
                            if (f30 > h) {
                                f30 = h;
                            }
                        }
                    } else {
                        u8 kind = e->mFieldD;
                        if ((u8)(kind + 0xcb) <= 2 || kind == 0x3b) {
                            f32 h = func_80190938();
                            if (f30 > h) {
                                f30 = func_80190938();
                            }
                        }
                    }
                }
            } else {
                float* v = self->mPlayer->vtbl->fn_0x1D0(self->mPlayer);
                if (v[0] != lbl_eu_80666CF8) {
                    f30 = self->mPlayer->vtbl->fn_0x1D0(self->mPlayer)[0];
                } else {
                    f30 = self->mPlayer->vtbl->fn_0x1C0(self->mPlayer)[0];
                }
            }
            if (f30 == lbl_eu_80666D00) {
                float* v = self->mPlayer->vtbl->fn_0x1D0(self->mPlayer);
                if (v[0] != lbl_eu_80666CF8) {
                    f30 = self->mPlayer->vtbl->fn_0x1D0(self->mPlayer)[0];
                } else {
                    f30 = self->mPlayer->vtbl->fn_0x1C0(self->mPlayer)[0];
                }
            }
            if (f30 < lbl_eu_80666CF8) {
                f30 = self->mPlayer->vtbl->fn_0x1C0(self->mPlayer)[0];
            }
        } else {
            void* p = cf::CfGameManager::getPlayer(1);
            if (p != 0) {
                p = (u8*)p - 0x3E9C;
            }
            if (p == (void*)self->mPlayer) {
                f30 = lbl_eu_80666D04;
            } else {
                f30 = lbl_eu_80666D08;
            }
        }
        // Anchor position: the 0x12C slot returns a position block when
        // present, else the player position; the y is then pulled toward the
        // player by the D0C blend so the facing math uses the offset origin.
        // pos40 holds the raw anchor, pos58 the blended copy (retail keeps
        // two stack slots at sp+0x40 and sp+0x58 with a copy between).
        ml::CVec3 pos40;
        ml::CVec3 pos58;
        CtrlActPosBlock* res =
            ((CtrlActVoiceOwnerView*)&self->mPlayer->mSub3E9C)
                ->vtbl->fn_0x12C(&self->mPlayer->mSub3E9C, 0x64);
        if (res != 0) {
            pos40.x = res->mFieldC;
            pos40.y = res->mField1C;
            pos40.z = res->mField2C;
        } else {
            pos40 = *(ml::CVec3*)self->mPlayer->mSub3E9C.getPosition();
        }
        pos58 = pos40;
        {
            CVoicePos* p = self->mPlayer->mSub3E9C.getPosition();
            f32 dy = pos58.y - p->f[1];
            pos58.y = pos58.y - dy * lbl_eu_80666D0C;
        }
        f32 f28 = pos58.y - self->mPlayer->mSub3E9C.getPosition()->f[1];
        if (f28 != lbl_eu_80666CF8) {
            f32 mag2 = f28 * f28 + f30 * f30;
            if (mag2 < lbl_eu_80666CF8) {
                nw4r::db::Warning((const char*)lbl_eu_80526324, 0x273,
                                  (const char*)lbl_eu_80526300);
            }
            if (mag2 <= lbl_eu_80666CF8) {
                mag2 = mag2 * nw4r::math::FrSqrt(mag2);
            }
            f30 = mag2;
        }
        // Distance from the (blended) anchor to the source; f27 is the
        // clearance margin = mag - scale * (height + radius).
        // Component-wise (no operator- temp): retail inlines the pair
        // subtract with psq_l/ps_sub, so no hidden return-slot local.
        CVoicePos* spos = src->mOwner3E9C.getPosition();
        ml::CVec3 diff;
        diff.x = spos->f[0] - pos58.x;
        diff.y = spos->f[1] - pos58.y;
        diff.z = spos->f[2] - pos58.z;
        ml::CVec3 magVec = diff;   // retail materializes a second slot
        f28 = PSVECMag((const Vec*)&magVec);   // f28 reused (retail fmr f28)
        if (self->mField74 & 0x8000) {
            s16 v = self->mField76;
            f27 = f28 - lbl_eu_80666D10 * (f30 + f31);   // fnmsubs first
            self->mField76 = v - 1;
            if (v <= 0) {
                self->mField74 &= ~0x8000;
                self->mField76 = 0;
            }
            r30 = 0;
        } else {
            f27 = f28 - lbl_eu_80666D14 * (f30 + f31);
        }
        // Battle-manager range gate again: while in range and the chain
        // start gate is open, f27 comes from the chain helper; then the
        // positive-f27 branch positions the action.
        {
            u8 rangeByte2 =
                ((CtrlActBmView*)getInstance__Q22cf14CBattleManagerFv())
                    ->mField1AA;
            if ((rangeByte2 >= 1 && rangeByte2 <= 0x18) != 0
                && lbl_eu_80666D18 > f28) {
                int r = func_80279778(
                    &((CtrlActBmView*)getInstance__Q22cf14CBattleManagerFv())
                         ->mField1A8,
                    self->mPlayer);
                // Retail lowers this via cntlzw/srwi: nonzero result picks
                // D1C, zero keeps CF8.
                f27 = (r != 0) ? lbl_eu_80666D1C : lbl_eu_80666CF8;
            }
        }
        if (f27 <= lbl_eu_80666CF8) {
            self->mField76 = 0x1e;
            self->mField74 &= ~0x8000;
            self->mField14 = lbl_eu_80666CF8;
        } else {
            u32 local10 = *self->mPlayer->mField4->vf30();
            if (func_80174C98(self->mPlayer, &local10, 0x1000) != 0) {
                self->mField14 = lbl_eu_80666CF8;
            } else {
                void* ppos = self->mPlayer->mSub3E9C.getPosition();
                if (func_804B19CC((u8*)src + 0x44A8, ppos, 0, 1) != 0) {
                    self->mField14 = lbl_eu_80666CF8;
                } else {
                    if (f27 < lbl_eu_80666D20) {
                        f27 = lbl_eu_80666D20;
                    }
                    if (f27 > lbl_eu_80666CFC) {
                        f27 = lbl_eu_80666CFC;
                    }
                    if (((u32)__cntlzw(
                             (u32)__cntlzw(self->mPlayer->mField3F00 & 2) >>
                             5) >> 5) !=
                        0) {
                        self->mField14 = lbl_eu_80666D24 * f27;
                    } else {
                        // mFlags58 bit 14/13 ladder: scale the player's
                        // height probe by the voice-owner 0x138 height and
                        // clamp, or blend the facing value into mField70.
                        if (self->mFlags58.mBit14) {
                            pf = self->mPlayer->vtbl->fn_0xF0(self->mPlayer);
                            float* hv =
                                ((CtrlActVoiceOwnerView*)
                                     &self->mPlayer->mSub3E9C)
                                    ->vtbl->fn_0x138(
                                        &self->mPlayer->mSub3E9C);
                            f32 f1;
                            if (self->mFlags58.mBit13) {
                                f1 = lbl_eu_80666D28 / hv[0] * pf;
                            } else {
                                f1 = lbl_eu_80666D2C / hv[0] * pf;
                            }
                            if (f1 < lbl_eu_80666D30) {
                                f1 = lbl_eu_80666D20;
                            }
                            self->mField14 = f1;
                            self->mField70 = f1;
                        } else {
                            // .L_800D2480: playerF0 is held in pf (f29)
                            // across the fn_0x138 call (retail fmr f29, f1).
                            pf = self->mPlayer->vtbl->fn_0xF0(self->mPlayer);
                            float* hv2 =
                                ((CtrlActVoiceOwnerView*)
                                     &self->mPlayer->mSub3E9C)
                                    ->vtbl->fn_0x138(
                                        &self->mPlayer->mSub3E9C);
                            f32 f1 = hv2[0] * pf;
                            if (f1 >= lbl_eu_80666D38) {
                                self->mField70 = f27;
                                self->mField14 = f27;
                            } else {
                                f32 sum = lbl_eu_80666D3C + f30 + f31;
                                if (f28 < sum) {
                                    f30 =
                                        self->mPlayer->vtbl->fn_0xF0(
                                            self->mPlayer);
                                    float* hv3 =
                                        ((CtrlActVoiceOwnerView*)
                                             &self->mPlayer->mSub3E9C)
                                            ->vtbl->fn_0x138(
                                                &self->mPlayer->mSub3E9C);
                                    f32 f1b = lbl_eu_80666D38 / hv3[0] * f30;
                                    if (f1b < lbl_eu_80666D30) {
                                        f1b = lbl_eu_80666D20;
                                    }
                                    f32 m70 = self->mField70;
                                    f27 = lbl_eu_80666D20 * (f1b - m70) + m70;
                                    self->mField70 = f27;
                                }
                                self->mField70 = f27;
                                self->mField14 = f27;
                            }
                        }
                    }
                    // .L_800D253C: restart the 0x8000 countdown.
                    if (r30 != 0) {
                        self->mField76 = 0x1e;
                        self->mField74 |= 0x8000;
                    }
                }
            }
        }
    }
    // .L_800D2580: bit 13 clear forces the state value to 0.
    if (self->mField74 & 0x2000) {
        self->mField14 = lbl_eu_80666CF8;
    }
    if ((self->mPlayer->mField3374 & 0x800) == 0) {
        int doAim = 0;
        if (self->mPlayer->mSub3E9C.v01(4) == 0) {
            u32 localC = *self->mPlayer->mField4->vf30();
            if (func_80174C98(self->mPlayer, &localC, 0x806) == 0) {
                if (self->mField14 != lbl_eu_80666CF8) {
                    doAim = 1;
                }
            }
        }
        if (doAim != 0) {
            if (self->mPlayer->mField3F00 & 2) {
                CVoicePos* p = src->mOwner3E9C.getPosition();
                func_800D69D8(self, (ml::CVec3*)p, src, r29);
            } else {
                CVoicePos* p = src->mOwner3E9C.getPosition();
                func_800D755C(self, (ml::CVec3*)p);
            }
        } else {
            u32 local8 = *self->mPlayer->mField4->vf30();
            if (func_80174C98(self->mPlayer, &local8, 0x1f) == 0) {
                if (self->mPlayer->mSub3E9C.v01(0x800) == 0) {
                    CVoicePos* pp = self->mPlayer->mSub3E9C.getPosition();
                    CVoicePos* sp = src->mOwner3E9C.getPosition();
                    ml::CVec3 diff2;
                    nw4r::math::VEC3Sub((nw4r::math::VEC3*)&diff2,
                                        (const nw4r::math::VEC3*)sp,
                                        (const nw4r::math::VEC3*)pp);
                    ml::CVec3 d2 = diff2;
                    f32 ang = nw4r::math::Atan2FIdx(d2.x, d2.z);
                    self->mFieldC = lbl_eu_80666D40 * ang;
                    self->mPlayer->mSub3E9C.v47(self->mFieldC);
                }
            }
        }
    }
    // .L_800D274C: decay the state value by the player's 0x23C scale; the
    // r29 "stuck" flag zeroes it, otherwise the 0x3F00 bit 2 path re-probes.
    {
        f32 cur = self->mField14;
        f32 scaled = cur * self->mPlayer->vtbl->fn_0x23C(self->mPlayer);
        self->mField14 = scaled;
        if (r29 != 0) {
            self->mField14 = lbl_eu_80666CF8;
        } else if (scaled != lbl_eu_80666CF8) {
            if (self->mPlayer->mField3F00 & 4) {
                func_800D64E8(self);
            }
        }
    }
}

// Target us-800d27e4. Player-facing action dispatch: gate on the voice-owner
// state (v01) and the 0x1000 control word; when both pass, resolve the
// action source from the voice handle, forward it to the voice-owner slot
// 0x1AC, and re-aim the owner at the resolved source unless the battle
// target is in a reserved state-page range (0x21..0x2A). The second
// 0x1000 gate then marks the owner with v02(1) + func_80174B4C.
void func_800D1CFC(CtrlActView* self) {
    // Initialized here so the interference range starts before the gate
    // blocks; the dead store itself is removed by the optimizer.
    CtrlActSrc* src = NULL;
    if (self->mPlayer->mSub3E9C.v01(1) == 0) {
        return;
    }
    {
        CtrlActPlayerView* player = self->mPlayer;
        u32 localC = *player->mField4->vf30();
        if (func_80174C98(player, &localC, 0x1000) != 0) {
            return;
        }
    }
    src = (CtrlActSrc*)func_800B708C(
        (int)(intptr_t)self->mPlayer->mSub3E9C.v17());
    if (src != NULL) {
        // Normalize the handle: if it points at our own +0x3E9C owner
        // region, pass NULL. Retail emits a copy plus guarded de-bias,
        // then a re-bias null check; MWCC shapes this ternary closest.
        void* arg = (src != NULL) ? (void*)((u8*)src - 0x3E9C) : (void*)src;
        if (arg != NULL) {
            arg = (u8*)arg + 0x3E9C;
        }
        ((CtrlActVoiceOwnerView*)&self->mPlayer->mSub3E9C)
            ->vtbl->fn_0x1AC(&self->mPlayer->mSub3E9C, arg,
                             lbl_eu_804FC81C);
        if (self->mPlayer->mField3F60 != 0) {
            int page = func_8004C5EC(self->mPlayer->mField3F60);
            if (page < 0x21 || page > 0x2a) {
                CVoicePos* p1 = self->mPlayer->mSub3E9C.getPosition();
                CVoicePos* p2 = (CVoicePos*)((CtrlVoiceHandle*)src)->vf41();
                ml::CVec3 diff;
                nw4r::math::VEC3Sub((nw4r::math::VEC3*)&diff,
                                    (const nw4r::math::VEC3*)p2,
                                    (const nw4r::math::VEC3*)p1);
                ml::CVec3 d = diff;
                f32 ang = nw4r::math::Atan2FIdx(d.x, d.z);
                self->mPlayer->mSub3E9C.v47(lbl_eu_80666D40 * ang);
                func_800BE12C((u8*)&self->mPlayer->mSub3E9C, 3, 0, -1, 1);
            }
        }
    }
    {
        CtrlActPlayerView* player = self->mPlayer;
        u32 local8 = *player->mField4->vf30();
        if (func_80174C98(player, &local8, 0x1000) != 0) {
            return;
        }
    }
    self->mPlayer->mSub3E9C.v02(1);
    func_80174B4C(self->mPlayer, 1);
}

// Target us-800d29f4. Per-frame action driver for the controlled actor.
// Gate phase: usable-gate virtual (slot 0x2BC), control word 0x2000, the
// action-container id probes 15/9 and the 0x805 command gate feeding the
// voice-height probe into func_8014B2EC. Then a cascade of func_80174C98
// command gates - any match (or a non-empty battle ring) runs the body. The
// body fetches a 0x20-byte action entry via func_8014B8BC and either rebuilds
// the +0x2A4 action-state block or dispatches the entry's kind byte through a
// 25-way switch; the no-entry path re-checks the current state instead.
void func_800D1F0C(CtrlActView* self) {
    // NOTE: no cached mPlayer local - retail reloads 0x5c(r30) at every use.
    // Typed virtuals keep retail's register flow (no temp-register copies).
    if (((CtrlActPlayerReal*)self->mPlayer)->vf173() != 0) {
        return;
    }
    if (self->mPlayer->mField3374 & 0x2000) {
        return;
    }

    if (func_80148778(&self->mPlayer->mField8, 0xf) == 0 &&
        func_80148778(&self->mPlayer->mField8, 9) == 0) {
        u32 cmd805 = *self->mPlayer->mField4->vf30();
        if (func_80174C98(self->mPlayer, &cmd805, 0x805) == 0) {
            f32 h = ((CtrlActVoiceHeightIntf*)&self->mPlayer->mSub3E9C)
                        ->getHeight();
            if (h != lbl_eu_80666CF8) {
                if (func_80496288(lbl_eu_80663E14) != lbl_eu_80666CF8) {
                    f32 scale = func_80496288(lbl_eu_80663E14);
                    f32 h2 = ((CtrlActVoiceHeightIntf*)&self->mPlayer->mSub3E9C)
                                 ->getHeight();
                    func_8014B2EC(&self->mPlayer->mField3380,
                                  h2 * scale / lbl_eu_80666D44);
                }
            }
        }
    }

    // Command gate cascade: evaluated sequentially like retail - any match
    // runs the body; otherwise only a non-empty battle ring does.
    // Branch-only cascade like retail (no materialized bool): any match
    // breaks to the body; falling through all gates with an empty ring
    // returns.
    {
        // Retail pins mPlayer in r29 across this whole cascade.
        CtrlActPlayerView* p = self->mPlayer;
        u32 gate9 = *p->mField4->vf30();
        if (func_80174C98(p, &gate9, 9) == 0) {
            u32 gateA = *p->mField4->vf30();
            if (func_80174C98(p, &gateA, 0xa) == 0) {
                u32 gateB = *p->mField4->vf30();
                if (func_80174C98(p, &gateB, 0xb) == 0) {
                    u32 gate6 = *p->mField4->vf30();
                    if (func_80174C98(p, &gate6, 6) == 0) {
                        u32 gate806 = *p->mField4->vf30();
                        if (func_80174C98(p, &gate806, 0x806) == 0) {
                            u32 gate14 = *p->mField4->vf30();
                            if (func_80174C98(p, &gate14, 0x14) == 0) {
                                u32 gate1F = *p->mField4->vf30();
                                if (func_80174C98(p, &gate1F, 0x1f) == 0 &&
                                    p->mField3594 == 0) {
                                    return;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    u8 buf4C[0xE];
    CtrlActActionEntry entry;
    u8 buf34[0xE];
    memset(buf4C, 0, sizeof(buf4C));
    memset(&entry, 0, sizeof(entry));
    memset(buf34, 0, sizeof(buf34));

    if (func_8014B8BC(&self->mPlayer->mField3380, &entry) == 0) {
        // No fresh entry: re-validate the current action state.
        if (!(entry.mFlags10 & 0x20)) {
            // L_3460: compare the owner's id probe against the stale entry.
            CtrlActVoiceOwnerView* vo =
                (CtrlActVoiceOwnerView*)&self->mPlayer->mSub3E9C;
            if (vo->vtbl->fn_0x4C(vo) != (int)entry.mField0) {
                if (entry.mFlags10 & 0x400) {
                    self->mPlayer->vtbl->fn_0x70(
                        self->mPlayer->mField3ED4,
                        (void*)(uintptr_t)entry.mField0);
                }
            }
            return;
        }
        u32 st = entry.mState;
        if ((st - 4 <= 2 || st == 0xa)) {
            // L_3370: cancel the action.
            self->mPlayer->mField3E6C &= ~0x20;
            ((CtrlActEntryObj*)entry.mPtr18)->mField7C = lbl_eu_80666CF8;
            func_800BE12C((u8*)&self->mPlayer->mSub3E9C, 0x31, 0, -1, 1);
            func_8014B2DC(&self->mPlayer->mField3380);
        } else if (st == 2) {
            // L_33B8: restart via command gates 0x806/0x10/0x18.
            u32 c1 = *self->mPlayer->mField4->vf30();
            if (func_80174C98(self->mPlayer, &c1, 0x806) == 0 &&
                func_80148778(&self->mPlayer->mField8, 0x10) == 0) {
                u32 c2 = *self->mPlayer->mField4->vf30();
                if (func_80174C98(self->mPlayer, &c2, 0x18) == 0) {
                    func_800BE12C((u8*)&self->mPlayer->mSub3E9C, 0x11, 0, -1, 1);
                }
            }
        }
        return;
    }

    if (entry.mFlags10 & 0x20) {
        if (entry.mState == 2) {
            return;
        }
        // Rebuild the +0x2A4 action-state block from the entry.
        CtrlActSub2A4* sub = ((CtrlActPlayerReal*)self->mPlayer)->vf167();
        sub->mField0 = 0;
        sub->mField4 = 0;
        sub->mField48 = 0;
        sub->mField4C = -1;
        sub->mField50 = 0;
        sub->mField54 = lbl_eu_80666CF8;
        sub->mField58 = lbl_eu_80666CF8;
        sub->mField5C = lbl_eu_80666CF8;
        sub->mField60 = lbl_eu_80666CF8;
        sub->mField64 = lbl_eu_80666CF8;
        sub->mField7C = 0;
        sub->mField80 = 0;
        sub->mFieldB8 = 0;
        sub->mField68 = lbl_eu_80666CF8;
        sub->mField6C = lbl_eu_80666CF8;
        sub->mField70 = 0;
        sub->mField72 = 0;
        memset((u8*)sub + 8, 0, 0x40);
        memset(sub->_84, 0, sizeof(sub->_84));
        sub->mField74 = 0;
        sub->mField78 = 0;
        sub->mField4 = entry.mField0;
        sub->mField50 = (u32)(uintptr_t)entry.mPtr18;

        CtrlActAtkParam* ap = (CtrlActAtkParam*)sub->mField50;
        if (self->mPlayer->mField3F00 & 2) {
            if (entry.mKind >= 5 && entry.mKind <= 0xc) {
                sub->mField48 = ap->mField76 + 7;
                sub->mField78 |= 0x40000800;
            } else {
                sub->mField48 = ap->mField76 + 0x10;
                if ((self->mPlayer->mField3F00 & 4) && ap->mField42 == 1) {
                    sub->mField78 |= 0x40000800;
                } else {
                    sub->mField78 |= 0x40000400;
                }
            }
        } else {
            sub->mField48 = ap->mField76 + 0x10;
            if ((self->mPlayer->mField3F00 & 4) && ap->mField42 == 1) {
                sub->mField78 |= 0x40000800;
            } else {
                sub->mField78 |= 0x40000400;
            }
        }
        if (ap->mField78 & 0x8000) {
            self->mPlayer->vtbl->fn_0x5C8(self->mPlayer, 1);
        }
        u32 g806 = *self->mPlayer->mField4->vf30();
        if (func_80174C98(self->mPlayer, &g806, 0x806) == 0) {
            func_800BE12C((u8*)&self->mPlayer->mSub3E9C, 0x11, 0, -1, 1);
            CtrlActEntryObj* eo = (CtrlActEntryObj*)entry.mPtr18;
            eo->mField7C = eo->mField2C;
            void* bm = getInstance__Q22cf14CBattleManagerFv();
            ((CtrlActBm2*)bm)->vtbl->fn2C(bm, self->mPlayer, sub);
        }
        return;
    }

    if (entry.mKind > 0x58) {
        return;
    }
    switch (entry.mKind) {
    case 0:
    case 1:
        ((CtrlActSelfView*)self)
            ->vtbl->fn_0x78(self, &entry);
        break;
    case 2:
    case 3:
        ((CtrlActSelfView*)self)
            ->vtbl->fn_0x7C(self, &entry);
        break;
    case 4:
        func_8016FE34(func_800B708C((int)entry.mField0));
        func_800BE12C((u8*)&self->mPlayer->mSub3E9C, 0x1b, 0, 4, 1);
        break;
    case 5: {
        void* src = func_8016FE34(func_800B708C((int)entry.mField0));
        func_800BE12C((u8*)&self->mPlayer->mSub3E9C, 0x1b, 0, 1, 1);
        func_802A29A4(self->mPlayer, src);
        break;
    }
    case 6: {
        void* src = func_8016FE34(func_800B708C((int)entry.mField0));
        func_800BE12C((u8*)&self->mPlayer->mSub3E9C, 0x1b, 0, 0, 1);
        func_802A2A0C(self->mPlayer, src);
        break;
    }
    case 7: {
        void* src = func_8016FE34(func_800B708C((int)entry.mField0));
        func_800BE12C((u8*)&self->mPlayer->mSub3E9C, 0x1b, 0, 2, 1);
        func_802A2ADC(self->mPlayer, src);
        break;
    }
    case 8:
        func_80174B4C(self->mPlayer, 0x80);
        func_800D5874(self, 1, 0);
        break;
    case 9:
        func_80174B4C(self->mPlayer, 0xc0);
        func_800D5874(self, 2, entry.mKind);
        break;
    case 10:
        func_80174B4C(self->mPlayer, 0x100);
        func_800D5874(self, 3, 0);
        break;
    case 11:
        func_80174B4C(self->mPlayer, 0x140);
        func_800D5874(self, 3, 5);
        break;
    case 12:
        func_80174B4C(self->mPlayer, 0x180);
        func_800D5874(self, 3, 0xa);
        break;
    case 13:
        func_80174B4C(self->mPlayer, 0x1c0);
        if (func_80148778(&self->mPlayer->mField8, 0xb) != 0) {
            func_800D5874(self, 0xa, 0);
        } else {
            func_800D5874(self, 4, 0);
        }
        break;
    case 14:
        func_80174B4C(self->mPlayer, 0x200);
        func_800D5874(self, 5, 0);
        break;
    case 15:
        func_80174B4C(self->mPlayer, 0x240);
        func_800D5874(self, 6, 0);
        break;
    case 16:
        func_80174B4C(self->mPlayer, 0x280);
        func_800D5874(self, 7, 0);
        break;
    case 17:
        func_80174B4C(self->mPlayer, 0x2c0);
        func_800D5874(self, 8, 0);
        break;
    case 18:
        func_80174B4C(self->mPlayer, 0x300);
        func_800D5874(self, 9, 0);
        break;
    case 19: {
        void* t = func_8016FE34(func_800B708C((int)entry.mField0));
        if (t != NULL) {
            ((CtrlActSrc*)t)->mField3388 |= 8;
        }
        break;
    }
    case 20: {
        CtrlActFxReq req;
        memset(&req, 0, sizeof(req));
        req.mField20 = lbl_eu_80666D48;
        req.mIdC = 0x111;
        void* bm = getInstance__Q22cf14CBattleManagerFv();
        func_800EC8FC(bm, self->mPlayer, &req, 0);
        break;
    }
    case 21: {
        CtrlActFxReq req;
        memset(&req, 0, sizeof(req));
        req.mField20 = lbl_eu_80666D48;
        req.mIdC = 0x112;
        void* bm = getInstance__Q22cf14CBattleManagerFv();
        func_800EC8FC(bm, self->mPlayer, &req, 0);
        break;
    }
    case 22: {
        void* base = self->mPlayer->vtbl->fn_0x27C(self->mPlayer);
        CtrlActArtsParam* p =
            (CtrlActArtsParam*)getArtsParamByIdx(base, entry.mArtsIdx);
        p->mField80 = p->vtbl->fn14(p);
        break;
    }
    case 23: {
        if ((self->mPlayer->mField3F00 & 4) == 0) {
            break;
        }
        void* arg =
            (self->mPlayer != NULL) ? (void*)&self->mPlayer->mSub3E9C : NULL;
        CtrlActChainObj* obj = (CtrlActChainObj*)func_800AD860(arg);
        void* o2 = func_80193670(obj);
        if (func_80193AB0(o2, obj->mField45C0) != NULL) {
            ((CtrlActChainObj*)o2)->mFieldA0 |= 2;
        }
        break;
    }
    case 24: {
        if ((self->mPlayer->mField3F00 & 4) == 0) {
            break;
        }
        void* bm = getInstance__Q22cf14CBattleManagerFv();
        func_8027936C(&((CtrlActBmView*)bm)->mField1A8, (int)self->mPlayer);
        break;
    }
    default:
        break;
    }
}

// Attack-param virtual #4: scale a per-frame effect by the count-scaled
// rate. Calls the +0x0C virtual on itself (retail r12 ABI dispatch). The
// (f32)(s32) product conversion emits MWCC's 2^52 trick
// (lis 0x4330 / xoris / lfd / fsubs); lbl_eu_80666D50 is that magic constant.
float cf::CAttackParam::CAttackParam_UnkVirtualFunc4() {
    int prod = unk70 * (getMax() - 1);
    float fprod = (float)prod;
    return unk30 * (lbl_eu_80666CFC - (fprod / lbl_eu_80666D4C));
}

// Target us-800d3544. Attack-action setup: gate on the player and source
// state words, then fill the player's +0x2A4 action block from the argument
// (attack index, actor id, kind flags 0x54/0x55 select the 0x78 flag bits).
// (gate helper experiments reverted: direct calls reproduce retail's
// stack-slot order)
int func_800D2A5C(CtrlActView* self, CtrlActAtkArg* arg) {
    // self->mPlayer is re-read at every use site (no cached local) so the
    // compiler reloads it per block like the retail code; the casts only
    // select real virtual dispatch (see CtrlActPlayerReal in CtrlAct.hpp).
    CtrlActSub2A4* sub = ((CtrlActPlayerReal*)self->mPlayer)->vf167();
    sub->mField4 = arg->mField0;
    int kind = 0;
    CtrlActPlayerReal* p;
    u8 b = arg->mFieldD;
    s16 atkIndex = arg->mField12;
    if (b == 2 || b == 3 || (u8)(b + 0xac) <= 1) {
        kind = b;
    }
    int ok = 1;
    {
        p = (CtrlActPlayerReal*)self->mPlayer;
        u32 local14;
        if (func_80174C98(p,
                          (u32*)(local14 = *p->mField4->vf30(), &local14),
                          9)
            == 0) {
            p = (CtrlActPlayerReal*)self->mPlayer;
            u32 local10;
            if (func_80174C98(p,
                              (u32*)(local10 = *p->mField4->vf30(),
                                     &local10),
                              6)
                == 0) {
                p = (CtrlActPlayerReal*)self->mPlayer;
                u32 localC;
                if (func_80174C98(p,
                                  (u32*)(localC = *p->mField4->vf30(),
                                         &localC),
                                  0x12)
                    == 0) {
                    return 0;
                }
            }
        }
    }
    CtrlActSrcReal* src =
        (CtrlActSrcReal*)func_8016FE34(func_800B708C((int)arg->mField0));
    if (src == NULL) {
        return 0;
    }
    if (src->vf173() != 0) {
        return 0;
    }
    u32 local8 = *src->mField4->vf30();
    if (func_80174C98(src, &local8, 0x100000) == 0) {
        return 0;
    }
    if (src == (CtrlActSrcReal*)self->mPlayer) {
        ((CtrlActPlayerReal*)self->mPlayer)->mSub3E9C.v18(0);
        return 0;
    }
    if (atkIndex >= 0) {
        self->mField4 |= 2;
        void* base = ((CtrlActPlayerReal*)self->mPlayer)->vf160();
        CtrlActAtkParam* atk =
            (CtrlActAtkParam*)getAtkParam(base, atkIndex);
        CtrlActSub2A4* state = ((CtrlActPlayerReal*)self->mPlayer)->vf167();
        state->mField0 = 0;
        // Local pin: forces the single lbl_eu_80666CF8 load (fused with the
        // -1 in r0 as the sda21 base) to sit here like retail.
        f32 fCon = lbl_eu_80666CF8;
        state->mField4 = 0;
        state->mField48 = 0;
        state->mField4C = -1;
        state->mField50 = 0;
        state->mField54 = fCon;
        state->mField58 = fCon;
        state->mField5C = fCon;
        state->mField60 = fCon;
        state->mField64 = fCon;
        state->mField7C = 0;
        state->mField80 = 0;
        state->mFieldB8 = 0;
        state->mField68 = fCon;
        state->mField6C = fCon;
        state->mField70 = 0;
        state->mField72 = 0;
        memset((u8*)state + 8, 0, 0x40);
        memset((u8*)state + 0x84, 0, 0x34);
        state->mField74 = 0;
        state->mField78 = 0;
        state->mField50 = arg->mField18;
        state->mField4 = arg->mField0;
        int atkCount = atk->mField76 + 1;
        self->mField18 = atkCount;
        state->mField48 = atkCount;
        // kind selects extra bits in the 0x78 flag word (0x54 -> 0x02000000,
        // 0x55 -> 0x01000000).
        state->mField78 |= 0x40002000;
        if (kind == 0x54) {
            state->mField78 |= 0x02000000;
        } else if (kind == 0x55) {
            state->mField78 |= 0x01000000;
        }
    } else {
        ok = 0;
    }
    return ok == 1;
}

// Target us-800d384c. Attack-action request: gate on the arts-param record,
// the action source and the battle-manager state, then fill the player's
// +0x2A4 action block from the argument (count from the arts-param, kind bits
// into the 0x78 flag word). The block is snapshotted to the stack while the
// arts container and battle state are probed, restored, and the facing
// virtuals run before the block is committed.
extern "C" int func_800D2D64(CtrlActView* self, CtrlActAtkArg* arg) {
    s16 atkIndex = arg->mField12;
    CtrlActSrc* src = (CtrlActSrc*)func_8016FE34(func_800B708C((int)arg->mField0));
    // Each arts-set lookup reloads self->mPlayer fresh (scoped locals):
    // retail issues two separate lwz 0x5c(r29) + slot-0x27C dispatches here,
    // so the player load must not be CSE'd across the two statements.
    void* arts;
    {
        CtrlActPlayerView* p = self->mPlayer;
        arts = func_80153CAC(p->vtbl->fn_0x27C(p), atkIndex);
    }
    CtrlActAtkParam* atk;
    {
        CtrlActPlayerView* p2 = self->mPlayer;
        atk = (CtrlActAtkParam*)getArtsParamByIdx(
            p2->vtbl->fn_0x27C(p2), atkIndex);
    }
    if (arts == 0 || src == 0) {
        return 0;
    }
    {
        u32 local = *src->mField4->vf30();
        if (func_80174C98(src, &local, 0x100000) == 0) {
            return 0;
        }
    }
    void* bm = getInstance__Q22cf14CBattleManagerFv();
    if (func_800EA444(bm) != 0) {
        CtrlActBattleSubView* bs = (CtrlActBattleSubView*)func_800EA444(bm);
        if ((bs->mField824 & 0x100000) == 0
            && func_801A6A7C(&bs->mField219C, self->mPlayer) != 0) {
            return 0;
        }
    }
    CtrlActSub2A4* sub = self->mPlayer->vtbl->fn_0x2A4(self->mPlayer);
    bm = getInstance__Q22cf14CBattleManagerFv();
    if (func_800EA444(bm) != 0
        && ((CtrlActBattleSubView*)func_800EA444(bm))->mField0
               == self->mPlayer->mField3F10) {
        // battle target matches: keep the existing block
    } else if ((sub->mField78 & 0x10000000) == 0) {
        // fresh state block
        sub->mField0 = 0;
        // Local pin: forces the single lbl_eu_80666CF8 load (fused with the
        // -1 in r0 as the sda21 base) to sit here like retail.
        f32 fCon = lbl_eu_80666CF8;
        sub->mField4 = 0;
        sub->mField48 = 0;
        sub->mField4C = -1;
        sub->mField50 = 0;
        sub->mField54 = fCon;
        sub->mField58 = fCon;
        sub->mField5C = fCon;
        sub->mField60 = fCon;
        sub->mField64 = fCon;
        sub->mField7C = 0;
        sub->mField80 = 0;
        sub->mFieldB8 = 0;
        sub->mField68 = fCon;
        sub->mField6C = fCon;
        sub->mField70 = 0;
        sub->mField72 = 0;
        memset((u8*)sub + 8, 0, 0x40);
        memset((u8*)sub + 0x84, 0, 0x34);
        sub->mField74 = 0;
        sub->mField78 = 0;
    }
    sub->mField50 = arg->mField18;
    sub->mField4 = arg->mField0;
    if ((self->mPlayer->mField3F00 & 2) && arg->mFieldD >= 5
        && arg->mFieldD <= 0xc) {
        int count = atk->mField76 + 7;
        self->mField18 = count;
        sub->mField48 = count;
        sub->mField78 |= 0x40008000;
    } else {
        int count = atk->mField76 + 0x10;
        self->mField18 = count;
        sub->mField48 = count;
        if ((self->mPlayer->mField3F00 & 4) && atk->mField42 == 1) {
            sub->mField78 |= 0x40008000;
        } else {
            sub->mField78 |= 0x40004000;
        }
    }
    // Snapshot the block while probing the arts container and battle state.
    // Snapshot: written as explicit member-wise assignments (not a whole-
    // struct assign) because retail lowers the save/restore per member with
    // natural typed accesses (lha/lhz/lfs), which MWCC's block-move lowering
    // would never produce.
    // Snapshot the block while probing the arts container and battle state.
    // Snapshot: written as explicit member-wise assignments because MWCC's
    // whole-struct block-move lowering diverges from retail's spill shape.
    CtrlActSub2A4 snapshot;
    snapshot.mFieldB8 = sub->mFieldB8;
    snapshot.mField80 = sub->mField80;
    snapshot.mField7C = sub->mField7C;
    snapshot.mField78 = sub->mField78;
    snapshot.mField74 = sub->mField74;
    snapshot.mField72 = sub->mField72;
    snapshot.mField70 = sub->mField70;
    snapshot.mField6C = sub->mField6C;
    snapshot.mField68 = sub->mField68;
    snapshot.mField64 = sub->mField64;
    snapshot.mField60 = sub->mField60;
    snapshot.mField5C = sub->mField5C;
    snapshot.mField58 = sub->mField58;
    snapshot.mField54 = sub->mField54;
    snapshot.mField50 = sub->mField50;
    snapshot.mField4C = sub->mField4C;
    snapshot.mField48 = sub->mField48;
    snapshot.mField44 = sub->mField44;
    snapshot.mField40 = sub->mField40;
    snapshot.mField3C = sub->mField3C;
    snapshot.mField38 = sub->mField38;
    snapshot.mField34 = sub->mField34;
    snapshot.mField30 = sub->mField30;
    snapshot.mField2C = sub->mField2C;
    snapshot.mField28 = sub->mField28;
    snapshot.mField24 = sub->mField24;
    snapshot.mField20 = sub->mField20;
    snapshot.mField1C = sub->mField1C;
    snapshot.mField18 = sub->mField18;
    snapshot.mField14 = sub->mField14;
    snapshot.mField10 = sub->mField10;
    snapshot.mFieldC = sub->mFieldC;
    snapshot.mField8 = sub->mField8;
    snapshot.mField4 = sub->mField4;
    snapshot.mField0 = sub->mField0;
    if (func_80148778(&self->mPlayer->mField8, 0xeb) != 0) {
        ((CtrlActArtsVtbl*)self->mPlayer->mField8)
            ->fn_0x20(&self->mPlayer->mField8, 0xeb);
        return 1;
    }
    if (func_80145C00(atk->mField48) != 0) {
        if (self->mPlayer->mField1530 != 0) {
            u8 v = ((CtrlActBmView*)getInstance__Q22cf14CBattleManagerFv())
                      ->mField1AA;
            if (v < 1 || v > 0x18) {
                if ((self->mPlayer->mField3E6C & 0x1000) == 0
                    && self->mPlayer->mField1530 == atk->mField48) {
                    ((CtrlActArtsVtbl*)self->mPlayer->mField8)->fn_0x20(
                        &self->mPlayer->mField8, self->mPlayer->mField1530);
                    return 1;
                }
            }
            if (self->mPlayer->mField1530 == 0xea
                && atk->mField48 == 0xea) {
                ((CtrlActArtsVtbl*)self->mPlayer->mField8)->fn_0x20(
                    &self->mPlayer->mField8, self->mPlayer->mField1530);
                return 1;
            }
            ((CtrlActArtsVtbl*)self->mPlayer->mField8)->fn_0x20(
                &self->mPlayer->mField8, self->mPlayer->mField1530);
        }
    }
    sub->mField0 = snapshot.mField0;
    sub->mField4 = snapshot.mField4;
    sub->mField8 = snapshot.mField8;
    sub->mFieldC = snapshot.mFieldC;
    sub->mField10 = snapshot.mField10;
    sub->mField14 = snapshot.mField14;
    sub->mField18 = snapshot.mField18;
    sub->mField1C = snapshot.mField1C;
    sub->mField20 = snapshot.mField20;
    sub->mField24 = snapshot.mField24;
    sub->mField28 = snapshot.mField28;
    sub->mField2C = snapshot.mField2C;
    sub->mField30 = snapshot.mField30;
    sub->mField34 = snapshot.mField34;
    sub->mField38 = snapshot.mField38;
    sub->mField3C = snapshot.mField3C;
    sub->mField40 = snapshot.mField40;
    sub->mField44 = snapshot.mField44;
    sub->mField48 = snapshot.mField48;
    sub->mField4C = snapshot.mField4C;
    sub->mField50 = snapshot.mField50;
    sub->mField54 = snapshot.mField54;
    sub->mField58 = snapshot.mField58;
    sub->mField5C = snapshot.mField5C;
    sub->mField60 = snapshot.mField60;
    sub->mField64 = snapshot.mField64;
    sub->mField68 = snapshot.mField68;
    sub->mField6C = snapshot.mField6C;
    sub->mField70 = snapshot.mField70;
    sub->mField72 = snapshot.mField72;
    sub->mField74 = snapshot.mField74;
    sub->mField78 = snapshot.mField78;
    sub->mField7C = snapshot.mField7C;
    sub->mField80 = snapshot.mField80;
    sub->mFieldB8 = snapshot.mFieldB8;
    if (self->vf30() == 0) {
        return 0;
    }
    self->vf31();
    self->mField4 |= 2;
    return 1;
}

extern "C" int func_800D34D4(CtrlActView* self) {
    // Positive-condition wrap (MWCC_CASES §798): retail lays the guard as
    // beq -> shared tail and the body inline; the early-return form inverts
    // the first branch.
    if (self->mFlags58.mKind != 0) {
    cf::CfGameManager::getInstance();
    // NOTE: retail materializes this mask with lis -> the real value is
    // 0x04000000, not 0x400.
    if (func_8006EF04(0x04000000) != 0) {
        self->mField14 = lbl_eu_80666CF8;
        return 1;
    }
    if (((CtrlActVoiceHeightIntf*)&self->mPlayer->mSub3E9C)->getHeight()
        == lbl_eu_80666CF8) {
        self->mField14 = lbl_eu_80666CF8;
        return 1;
    }
    {
        // Retail colors this gate's player cache into r30 (or r3,r30,r30
        // move before the call).
        CtrlActPlayerView* player = self->mPlayer;
        u32 local = *player->mField4->vf30();
        if (func_80174C98(player, &local, 0x1000) != 0) {
            return 0;
        }
    }
    if (self->mPlayer->mSub3E9C.v01(4) != 0) {
        return 0;
    }
    self->mField74 &= 0xEC3F;
    if (self->mField74 & 0x2000) {
        self->mField14 = lbl_eu_80666CF8;
        memset(&self->mPos30, 0, 0x2c);
        func_80174C24(self->mPlayer, 0x40);
        return 0;
    }
    // Action-kind dispatch: kinds 0-7 run their own handler, kind 8 clears
    // the state block, and kinds 9/0xA decay the action timer (resetting on
    // expiry).
    switch (self->mFlags58.mKind) {
    case 0:
        func_800D3998(self);
        break;
    case 1:
        func_800D3D34(self);
        break;
    case 2:
        func_800D3FFC(self);
        break;
    case 3:
        func_800D4834(self);
        break;
    case 4:
        func_800D56F0(self);
        break;
    case 5:
        func_800D49F4(self);
        break;
    case 6:
        func_800D4F30(self);
        break;
    case 7:
        func_800D5308(self);
        break;
    case 8:
        memset(&self->mPos30, 0, 0x2c);
        func_80174C24(self->mPlayer, 0x40);
        break;
    case 9:
    case 0xa: {
        u32 local = *self->mPlayer->mField4->vf30();
        if (func_80174C98(self->mPlayer, &local, 0x807) == 0) {
            memset(&self->mPos30, 0, 0x2c);
            func_80174C24(self->mPlayer, 0x40);
        } else {
            self->mField7A = 0;
            self->mField74 |= 0x10;
            f32 dt = func_80496288(lbl_eu_80663E14);
            self->mField54 = self->mField54 + lbl_eu_80666D58 * dt;
            if (self->mField54 >= lbl_eu_80666D04) {
                memset(&self->mPos30, 0, 0x2c);
                func_80174C24(self->mPlayer, 0x40);
            }
        }
        return 0;
    }
    default:
        break;
    }
    // Common post-dispatch: measure the squared distance from the recorded
    // anchor to the player; when the scaled height probe says the action is
    // close (kinds 4/0xA exempt), clear the state block and give up.
    int close = 0;
    CVoicePos* pos = self->mPlayer->mSub3E9C.getPosition();
    ml::CVec3 diff;
    nw4r::math::VEC3Sub((nw4r::math::VEC3*)&diff,
                        (const nw4r::math::VEC3*)pos,
                        (const nw4r::math::VEC3*)&self->mField3C);
    ml::CVec3 d = diff;
    // Multiply order (x, z, y) is what makes MWCC emit retail's
    // paired-single ps_mul/ps_madd/ps_sum0 magnitude sequence.
    f32 mag2 = d.x * d.x + d.z * d.z + d.y * d.y;
    f32 f138;
    if (self->mField28 != 0) {
        f138 = self->mField28->getFloats()[0];
    } else {
        f138 = lbl_eu_80666CF8;
    }
    f32 thresh = lbl_eu_80666D5C * f138 * self->mField14 * lbl_eu_80666D0C;
    thresh = thresh * thresh;
    u32 kind = self->mFlags58.mKind;
    if (kind != 4 && kind != 0xa && mag2 < thresh) {
        close = 1;
    }
    CVoicePos* pos2 = self->mPlayer->mSub3E9C.getPosition();
    self->mField3C = pos2->f[0];
    self->mField40 = pos2->f[1];
    self->mField44 = pos2->f[2];
    if (close != 0) {
        memset(&self->mPos30, 0, 0x2c);
        func_80174C24(self->mPlayer, 0x40);
        return 0;
    }
    // Facing-clearance gate: 0x3F00 bit 1 picks the height-clearance probe,
    // otherwise the facing-clearance probe; either failure clears the state.
    u32 flags = self->mPlayer->mField3F00;
    if (flags & 2) {
        if (func_800D6720(self, 0) != 0) {
            self->mField14 = lbl_eu_80666CF8;
            memset(&self->mPos30, 0, 0x2c);
            func_80174C24(self->mPlayer, 0x40);
        }
    } else {
        if (func_800D64E8(self) != 0) {
            memset(&self->mPos30, 0, 0x2c);
            func_80174C24(self->mPlayer, 0x40);
        }
    }
    {
        u32 local = *self->mPlayer->mField4->vf30();
        if (func_80174C98(self->mPlayer, &local, 0x4000) == 0
            && func_80148778(&self->mPlayer->mField8, 6) == 0
            && func_80148778(&self->mPlayer->mField8, 0xcd) == 0) {
            // arts container clear
        } else {
            self->mField14 = lbl_eu_80666CF8;
        }
    }
    }
    return 0;
}


extern "C" void func_800D3998(CtrlActView* self) {
    // Saved-register coloring (retail): src->r31, s (param copy) ->r30,
    // gate player cache ->r29. Every access goes through the local copy so
    // the raw parameter itself is never colored; clear blocks deliberately
    // re-read s->mPlayer after memset (retail reloads 0x5c(r30) there).
    CtrlActSrc* src;
    CtrlActView* s = self;
    CtrlActPlayerView* player = s->mPlayer;
    u32 local = *player->mField4->vf30();
    if (func_80174C98(player, &local, 0x807) == 0) {
        memset(&s->mPos30, 0, 0x2c);
        func_80174C24(s->mPlayer, 0x40);
        return;
    }
    void* t = func_8016FE34(
        func_800B708C((int)(intptr_t)s->mPlayer->mSub3E9C.v17()));
    if (t != 0) {
        src = (CtrlActSrc*)t;
    } else {
        src = 0;
    }
    if (src == 0) {
        memset(&s->mPos30, 0, 0x2c);
        func_80174C24(s->mPlayer, 0x40);
        return;
    }
    switch (s->mFlags58.mPhase) {
    case 0: {
        // Random-duration action setup: pick the aim/fx state from the
        // player's 0x3F00 bit 1, roll a timer in [D60/D44, 2*D60/D44) and a
        // facing magnitude, then snapshot the source position/facing.
        u32 flags = s->mPlayer->mField3F00;
        if (((u32)__cntlzw((u32)__cntlzw(flags & 2) >> 5) >> 5) != 0) {
            s->mField14 = lbl_eu_80666D0C;
        } else {
            s->mField14 = lbl_eu_80666CFC;
        }
        F64Conv conv;
        conv.w[1] = ml::math::mtRand(0x65) ^ 0x80000000;
        conv.w[0] = 0x43300000;
        f32 t = (f32)(conv.d - lbl_eu_80666D50) / lbl_eu_80666D4C;
        f32 f30 = lbl_eu_80666D60 * (lbl_eu_80666CFC + t) / lbl_eu_80666D44;
        F64Conv conv2;
        conv2.w[1] = ml::math::mtRand(0x65) ^ 0x80000000;
        conv2.w[0] = 0x43300000;
        f32 t2 = (f32)(conv2.d - lbl_eu_80666D50);
        // Retail order: subtract, multiply by D64, divide by D4C, add D64.
        f32 f31 =
            (lbl_eu_80666D64 + lbl_eu_80666D64 * t2 / lbl_eu_80666D4C) * lbl_eu_8066A210;
        CVoicePos* pos = src->mOwner3E9C.getPosition();
        s->mPos30 = *(ml::CVec3*)pos;
        s->mField48 = src->vtbl->fn_0x5B4(src);
        s->mField54 = f30;
        s->mField50 = f31;
        CVoicePos* ppos = s->mPlayer->mSub3E9C.getPosition();
        ml::CVec3 diff;
        nw4r::math::VEC3Sub((nw4r::math::VEC3*)&diff,
                            (const nw4r::math::VEC3*)ppos,
                            (const nw4r::math::VEC3*)&s->mPos30);
        ml::CVec3 d = diff;
        f32 ang = nw4r::math::Atan2FIdx(d.x, d.z);
        s->mField4C = lbl_eu_80666D40 * ang;
        func_800D5A2C(s);
        s->mFlags58.mPhase = s->mFlags58.mPhase + 1;
        break;
    }
    case 1: {
        // Decay the action timer; while live, re-aim at the source and bail
        // out when the heading delta exceeds the facing magnitude.
        f32 dt = func_80496288(lbl_eu_80663E14);
        s->mField54 -= lbl_eu_80666D58 * dt;
        if (s->mField54 <= lbl_eu_80666CF8) {
            s->mField14 = lbl_eu_80666CF8;
            memset(&s->mPos30, 0, 0x2c);
            func_80174C24(s->mPlayer, 0x40);
        } else {
            func_800D5F98(s, src);
            CVoicePos* ppos = s->mPlayer->mSub3E9C.getPosition();
            ml::CVec3 diff;
            nw4r::math::VEC3Sub((nw4r::math::VEC3*)&diff,
                                (const nw4r::math::VEC3*)ppos,
                                (const nw4r::math::VEC3*)&s->mPos30);
            ml::CVec3 d = diff;
            f32 ang = nw4r::math::Atan2FIdx(d.x, d.z);
            f32 delta = lbl_eu_80666D40 * ang - s->mField4C;
            if (delta < lbl_eu_80666CF8) {
                delta *= lbl_eu_80666D68;
            }
            if (delta > s->mField50) {
                s->mField14 = lbl_eu_80666CF8;
                memset(&s->mPos30, 0, 0x2c);
                func_80174C24(s->mPlayer, 0x40);
            }
        }
        break;
    }
    default:
        break;
    }
}

// Target us-800d481c. Action-phase update: gate on the player state, resolve
// the action source, then run the phase: phase 0 positions the action at the
// source (voice position snapshot, timer from the attack param, aim at the
// source, angle-state latch + phase+1); phase 1 decays the action timer and
// refreshes the facing helper. Timer expiry resets the 0x30 state block.
extern "C" void func_800D3D34(CtrlActView* self) {
    // Cached player pointer: MWCC keeps it in a saved register across the
    // state gate.
    CtrlActPlayerView* player = self->mPlayer;
    u32 local = *player->mField4->vf30();
    if (func_80174C98(player, &local, 0x807) == 0) {
        memset(&self->mPos30, 0, 0x2c);
        func_80174C24(self->mPlayer, 0x40);
        return;
    }
    CtrlActSrc* src;
    void* t = func_8016FE34(
        func_800B708C((int)(intptr_t)self->mPlayer->mSub3E9C.v17()));
    if (t != 0) {
        src = (CtrlActSrc*)t;
    } else {
        src = 0;
    }
    if (src == 0) {
        memset(&self->mPos30, 0, 0x2c);
        func_80174C24(self->mPlayer, 0x40);
        return;
    }
    if (self->mFlags58.mPhase == 0) {
        u32 flags = self->mPlayer->mField3F00;
        if (((u32)__cntlzw((u32)__cntlzw(flags & 2) >> 5) >> 5) != 0) {
            self->mField14 = lbl_eu_80666D6C;
        } else {
            self->mField14 = lbl_eu_80666D0C;
        }
        f32 f138;
        if (self->mField28 != 0) {
            f138 = self->mField28->getFloats()[0];
        } else {
            f138 = lbl_eu_80666CF8;
        }
        f32 denom = lbl_eu_80666D70 * f138 * self->mField14;
        // Cached zero spans the voice-owner calls (retail keeps it in an
        // FP reg across them).
        const f32 zero8 = lbl_eu_80666CF8;
        F64Conv conv;
        conv.w[0] = 0x43300000;
        conv.w[1] = self->mFlags58.mParam;
        f32 timer = (f32)(conv.d - lbl_eu_80666D78) / denom;
        CVoicePos* pos = src->mOwner3E9C.getPosition();
        self->mPos30 = *(ml::CVec3*)pos;
        self->mField48 = src->vtbl->fn_0x5B4(src);
        self->mField54 = timer;
        self->mField50 = zero8;
        CVoicePos* ppos = self->mPlayer->mSub3E9C.getPosition();
        ml::CVec3 diff;
        nw4r::math::VEC3Sub((nw4r::math::VEC3*)&diff,
                            (const nw4r::math::VEC3*)ppos,
                            (const nw4r::math::VEC3*)&self->mPos30);
        ml::CVec3 d = diff;
        f32 ang = nw4r::math::Atan2FIdx(d.x, d.z);
        self->mField4C = lbl_eu_80666D40 * ang;
        self->mFlags58.mAngleState = 3;
        self->mFlags58.mPhase = self->mFlags58.mPhase + 1;
    } else if (self->mFlags58.mPhase == 1) {
        f32 dt = func_80496288(lbl_eu_80663E14);
        self->mField54 -= lbl_eu_80666D58 * dt;
        if (self->mField54 <= lbl_eu_80666CF8) {
            self->mField14 = lbl_eu_80666CF8;
            memset(&self->mPos30, 0, 0x2c);
            func_80174C24(self->mPlayer, 0x40);
        } else {
            func_800D5F98(self, src);
        }
    }
}

// Target us-800d4ae4. Charge-action placement update: gate on the player
// state, resolve the action source, then phase 0 places the action at a spot
// offset from the source toward the player by the mParam-scaled direction,
// rotated by a mtRand-driven angle, and latches the timer/facing; phase 1
// (only when mParam==0) bails out when the source gets too close to either
// the player or the stored anchor. The common tail walks the action toward
// the player while the timer is live and re-aims via vf26 + ground probe.
extern "C" void func_800D3FFC(CtrlActView* self) {
    u32 local = *self->mPlayer->mField4->vf30();
    if (func_80174C98(self->mPlayer, &local, 0x807) == 0) {
        memset(&self->mPos30, 0, 0x2c);
        func_80174C24(self->mPlayer, 0x40);
        return;
    }
    // Voice/battle-list resolve of this actor's action source.
    void* t = func_8016FE34(
        func_800B708C((int)(intptr_t)self->mPlayer->mSub3E9C.v17()));
    CtrlActSrc* src;
    if (t != 0) {
        src = (CtrlActSrc*)t;
    } else {
        src = 0;
    }
    if (src == 0) {
        memset(&self->mPos30, 0, 0x2c);
        func_80174C24(self->mPlayer, 0x40);
        return;
    }
    switch (self->mFlags58.mPhase) {
    case 0: {
        // Placement: walk from the source toward the player, scale by the
        // action parameter, rotate by a random angle and store as mPos30.
        CVoicePos* sPos = src->mOwner3E9C.getPosition();
        CVoicePos* pPos = self->mPlayer->mSub3E9C.getPosition();
        ml::CVec3 diff = *(ml::CVec3*)pPos - *(ml::CVec3*)sPos;
        ml::CVec3 dir = diff;
        if (dir.y * dir.y + dir.x * dir.x + dir.z * dir.z ==
            lbl_eu_80666CF8) {
            dir = ml::CVec3::zero;
        } else {
            PSVECNormalize((Vec*)&dir, (Vec*)&dir);
        }
        F64Conv conv;
        conv.w[0] = 0x43300000;
        conv.w[1] = self->mFlags58.mParam;
        dir *= (f32)(conv.d - lbl_eu_80666D78);
        F64Conv conv2;
        conv2.w[0] = 0x43300000;
        conv2.w[1] = ((int)ml::math::mtRand(0xc9) - 0x64) ^ 0x80000000;
        f32 rnd = (f32)(conv2.d - lbl_eu_80666D50);
        f32 ang =
            lbl_eu_80666D3C * rnd / lbl_eu_80666D4C * lbl_eu_8066A210;
        f32 fs = nw4r::math::SinFIdx(lbl_eu_80666D80 * ang);
        f32 fc = nw4r::math::CosFIdx(lbl_eu_80666D80 * ang);
        CtrlActRotFrame frame;
        frame.mOffset.x = dir.x * fc + dir.z * fs;
        frame.mOffset.y = dir.y * lbl_eu_80666CFC;
        frame.mOffset.z = dir.z * fc - dir.x * fs;
        frame.mAxisX.x = fc;
        frame.mAxisX.y = lbl_eu_80666CF8;
        frame.mAxisX.z = -fs;
        frame.mAxisY.x = lbl_eu_80666CF8;
        frame.mAxisY.y = lbl_eu_80666CFC;
        frame.mAxisY.z = lbl_eu_80666CF8;
        frame.mAxisZ.x = fs;
        frame.mAxisZ.y = lbl_eu_80666CF8;
        frame.mAxisZ.z = fc;
        CVoicePos* sPos2 = src->mOwner3E9C.getPosition();
        self->mPos30 = *(ml::CVec3*)sPos2 + frame.mOffset;
        // Timer from the distance to the player and the sub-28 height.
        CVoicePos* pPos2 = self->mPlayer->mSub3E9C.getPosition();
        ml::CVec3 dmag = self->mPos30 - *(ml::CVec3*)pPos2;
        f32 dist = PSVECMag((const Vec*)&dmag);
        u32 flags = self->mPlayer->mField3F00;
        if (((u32)__cntlzw((u32)__cntlzw(flags & 2) >> 5) >> 5) != 0) {
            self->mField14 = lbl_eu_80666D0C;
        } else {
            self->mField14 = lbl_eu_80666CFC;
        }
        f32 h;
        if (self->mField28 != 0) {
            h = self->mField28->getFloats()[0];
        } else {
            h = lbl_eu_80666CF8;
        }
        f32 denom = lbl_eu_80666D70 * h * self->mField14;
        self->mField54 = dist / denom;
        f32 v = lbl_eu_80666D40 * nw4r::math::Atan2FIdx(dmag.x, dmag.z);
        self->mFlags58.mPhase = self->mFlags58.mPhase + 1;
        self->mFieldC = v;
        self->mField10 = v;
        break;
    }
    case 1: {
        if (self->mFlags58.mParam == 0) {
            // Proximity abort: once the source closes on the player or the
            // stored anchor inside the probe radius, end the action.
            CVoicePos* sp = src->mOwner3E9C.getPosition();
            CVoicePos* pp = self->mPlayer->mSub3E9C.getPosition();
            ml::CVec3 d1 = *(ml::CVec3*)sp - *(ml::CVec3*)pp;
            ml::CVec3 m1 = d1;
            f32 dPlayer = PSVECMag((const Vec*)&m1);
            CVoicePos* sp2 = src->mOwner3E9C.getPosition();
            ml::CVec3 d2 = *(ml::CVec3*)sp2 - *(ml::CVec3*)&self->mField3C;
            ml::CVec3 m2 = d2;
            f32 dSelf = PSVECMag((const Vec*)&m2);
            if (dPlayer < dSelf) {
                f32 lim =
                    lbl_eu_80666D14 *
                    (self->mPlayer->vtbl->fn_0x1C0(self->mPlayer)[0] +
                     src->mField44D8);
                if (dPlayer < lim) {
                    self->mField14 = lbl_eu_80666CF8;
                    memset(&self->mPos30, 0, 0x2c);
                    func_80174C24(self->mPlayer, 0x40);
                    return;
                }
            }
        }
        break;
    }
    default:
        break;
    }
    // Common tail: decay the timer; while live steer toward the player via
    // vf26 and the ground probe, otherwise reset the state block.
    CVoicePos* pp3 = self->mPlayer->mSub3E9C.getPosition();
    ml::CVec3 diff3 = self->mPos30 - *(ml::CVec3*)pp3;
    ml::CVec3 dir3 = diff3;
    if (dir3.y * dir3.y + dir3.x * dir3.x + dir3.z * dir3.z ==
        lbl_eu_80666CF8) {
        dir3 = ml::CVec3::zero;
    } else {
        PSVECNormalize((Vec*)&dir3, (Vec*)&dir3);
    }
    f32 dt = func_80496288(lbl_eu_80663E14);
    self->mField54 -= lbl_eu_80666D58 * dt;
    if (self->mField54 <= lbl_eu_80666CF8) {
        self->mField14 = lbl_eu_80666CF8;
        memset(&self->mPos30, 0, 0x2c);
        func_80174C24(self->mPlayer, 0x40);
        return;
    }
    ml::CVec3 scopy = dir3 * lbl_eu_80666CFC;
    CVoicePos* pp4 = self->mPlayer->mSub3E9C.getPosition();
    ml::CVec3 target = *(ml::CVec3*)pp4 + scopy;
    bool doReset = true;
    if (self->vf26(&target, 1) == 0) {
        ml::CVec3 off(lbl_eu_80666CF8, lbl_eu_80666CFC, lbl_eu_80666CF8);
        ml::CVec3 probePos = target + off;
        ml::CVec3 probeArg = probePos;
        if (func_804BE398(&probeArg, 0x4a05, 0, 0, lbl_eu_80666D84,
                          lbl_eu_80666CF8) != 0) {
            if (((ml::CVec3*)func_804BE520(0))->y > lbl_eu_80666CF8) {
                doReset = false;
            }
        }
    } else {
        doReset = false;
    }
    if (doReset) {
        self->mField14 = lbl_eu_80666CF8;
        memset(&self->mPos30, 0, 0x2c);
        func_80174C24(self->mPlayer, 0x40);
        return;
    }
    f32 v2 = lbl_eu_80666D40 * nw4r::math::Atan2FIdx(dir3.x, dir3.z);
    self->mFieldC = v2;
    self->mField10 = v2;
}

// Target us-800d531c. Battle-facing action update: when the 0x807 control
// gate passes and a voice source resolves, decay the action timer and, while
// it is live, aim at the resolved source's position through the vf24/vf23
// slots and store the facing angle. Falling out of the timer or failing the
// vf24 probe resets the 0x30 state block.

extern "C" void func_800D4834(CtrlActView* self) {
    ml::CVec3 vec1c;
    ml::CVec3 vec10;
    f32 x8;
    CtrlActPlayerView* player = self->mPlayer;
    {
        u32 local = *player->mField4->vf30();
        if (func_80174C98(player, &local, 0x807) == 0) {
            memset(&self->mPos30, 0, 0x2c);
            func_80174C24(self->mPlayer, 0x40);
            return;
        }
    }
    void* t = func_8016FE34(func_800B708C((int)(intptr_t)self->mPlayer->mSub3E9C.v17()));
    void* other;
    if (t != 0) {
        other = t;
    } else {
        other = 0;
    }
    if (other == 0) {
        memset(&self->mPos30, 0, 0x2c);
        func_80174C24(self->mPlayer, 0x40);
        return;
    }
    f32 dt = func_80496288(lbl_eu_80663E14);
    self->mField54 -= lbl_eu_80666D58 * dt;
    if (self->mField54 > lbl_eu_80666CF8) {
        CVoicePos* p2 =
            ((CVoiceOwnerIntfPc*)&((CtrlActSweepView*)other)->mOwner3E9C)->getPosition();
        if (self->vf24(&vec1c, p2) != 0) {
            f32 x8;
            self->vf23(&vec10, &x8, &vec1c, 1, 0);
            f32 az = vec10.z;
            f32 ax = vec10.x;
            f32 ang = nw4r::math::Atan2FIdx(ax, az);
            f32 v = lbl_eu_80666D40 * ang;
            self->mField14 = lbl_eu_80666D0C;
            self->mFieldC = v;
            self->mField10 = v;
            return;
        }
    }
    self->mField14 = lbl_eu_80666CF8;
    memset(&self->mPos30, 0, 0x2c);
    func_80174C24(self->mPlayer, 0x40);
}



int func_800D49E4(void* self) { return 0; }

int func_800D49EC(void* self) { return 0; }

// Target us-800d54dc. Looming-action update (kind 5): gate on the player
// state, resolve the action source, then run the phase: phase 0 positions
// the action at the source (voice position snapshot, fixed timer, aim at
// the source, phase+1); phase 1 decays the timer and, while live, checks
// the heading delta against the source-facing reference offset by the two
// wobble bands (D88 then D8C) - being inside either band ends the action,
// otherwise the shared facing helper re-aims.
extern "C" void func_800D49F4(CtrlActView* self) {
    {
        CtrlActPlayerView* player = self->mPlayer;
        u32 local = *player->mField4->vf30();
        if (func_80174C98(player, &local, 0x807) == 0) {
            memset(&self->mPos30, 0, 0x2c);
            func_80174C24(self->mPlayer, 0x40);
            return;
        }
    }
    void* t = func_8016FE34(
        func_800B708C((int)(intptr_t)self->mPlayer->mSub3E9C.v17()));
    CtrlActSrc* src;
    if (t != 0) {
        src = (CtrlActSrc*)t;
    } else {
        src = 0;
    }
    if (src == 0) {
        memset(&self->mPos30, 0, 0x2c);
        func_80174C24(self->mPlayer, 0x40);
        return;
    }
    switch (self->mFlags58.mPhase) {
    case 0:
        // Fixed-duration setup: snapshot the source position/facing, set the
        // timer from the constants, pick the facing direction, then fall into
        // the phase-1 decay.
        {
            u32 flags = self->mPlayer->mField3F00;
            if (((u32)__cntlzw((u32)__cntlzw(flags & 2) >> 5) >> 5) != 0) {
                self->mField14 = lbl_eu_80666D0C;
            } else {
                self->mField14 = lbl_eu_80666CFC;
            }
            f32 f31;
            f32 f30;
            f30 = lbl_eu_80666CF8;
            f31 = lbl_eu_80666D3C;
            CVoicePos* pos = src->mOwner3E9C.getPosition();
            self->mPos30 = *(ml::CVec3*)pos;
            self->mField48 = src->vtbl->fn_0x5B4(src);
            self->mField54 = f31;
            self->mField50 = f30;
            CVoicePos* ppos = self->mPlayer->mSub3E9C.getPosition();
            ml::CVec3 diff;
            nw4r::math::VEC3Sub((nw4r::math::VEC3*)&diff,
                                (const nw4r::math::VEC3*)ppos,
                                (const nw4r::math::VEC3*)&self->mPos30);
            ml::CVec3 d = diff;
            f32 ang = nw4r::math::Atan2FIdx(d.x, d.z);
            self->mField4C = lbl_eu_80666D40 * ang;
            func_800D5D68(self);
            self->mFlags58.mPhase = self->mFlags58.mPhase + 1;
            // fallthrough
        }
    case 1: {
        f32 dt = func_80496288(lbl_eu_80663E14);
        self->mField54 -= lbl_eu_80666D58 * dt;
        if (self->mField54 <= lbl_eu_80666CF8) {
            self->mField14 = lbl_eu_80666CF8;
            memset(&self->mPos30, 0, 0x2c);
            func_80174C24(self->mPlayer, 0x40);
            break;
        }
        // First heading band: wrap the source-facing reference (base facing +
        // wobble D88) and the heading delta into (-pi, pi], then check the
        // delta is inside the [-f31, f31] band - if so the action ends.
        f32 f31 = lbl_eu_80666D60 * lbl_eu_8066A210;
        f32 f30 = lbl_eu_80666D88 * lbl_eu_8066A210;
        CVoicePos* ppos = self->mPlayer->mSub3E9C.getPosition();
        ml::CVec3 diff;
        nw4r::math::VEC3Sub((nw4r::math::VEC3*)&diff,
                            (const nw4r::math::VEC3*)ppos,
                            (const nw4r::math::VEC3*)&self->mPos30);
        ml::CVec3 d = diff;
        f32 ang = nw4r::math::Atan2FIdx(d.x, d.z);
        f32 f3 = lbl_eu_80666D40 * ang;
        f32 f2 = self->mField48 + f30;
        while (lbl_eu_8066A1F8 <= f2) f2 -= lbl_eu_8066A1FC;
        while (f2 < -lbl_eu_8066A1F8) f2 += lbl_eu_8066A1FC;
        f2 = f3 - f2;
        while (lbl_eu_8066A1F8 <= f2) f2 -= lbl_eu_8066A1FC;
        while (f2 < -lbl_eu_8066A1F8) f2 += lbl_eu_8066A1FC;
        int inRange;
        if ((-f31 <= f2 && f2 <= lbl_eu_80666CF8)
            || (lbl_eu_80666CF8 <= f2 && f2 <= f31)) {
            inRange = 1;
        } else {
            inRange = 0;
        }
        if (inRange != 0) {
            self->mField14 = lbl_eu_80666CF8;
            memset(&self->mPos30, 0, 0x2c);
            func_80174C24(self->mPlayer, 0x40);
            break;
        }
        // Second heading band: same test with the wider D8C wobble. Fresh
        // locals so MWCC keeps a distinct stack slot pair (retail frame 0x80).
        {
            f32 f31b = lbl_eu_80666D60 * lbl_eu_8066A210;
            f32 f30b = lbl_eu_80666D8C * lbl_eu_8066A210;
            CVoicePos* ppos2 = self->mPlayer->mSub3E9C.getPosition();
            ml::CVec3 diff2;
            nw4r::math::VEC3Sub((nw4r::math::VEC3*)&diff2,
                                (const nw4r::math::VEC3*)ppos2,
                                (const nw4r::math::VEC3*)&self->mPos30);
            ml::CVec3 d2 = diff2;
            f32 ang2 = nw4r::math::Atan2FIdx(d2.x, d2.z);
            f32 f3b = lbl_eu_80666D40 * ang2;
            f32 f2b = self->mField48 + f30b;
            while (lbl_eu_8066A1F8 <= f2b) f2b -= lbl_eu_8066A1FC;
            while (f2b < -lbl_eu_8066A1F8) f2b += lbl_eu_8066A1FC;
            f2b = f3b - f2b;
            while (lbl_eu_8066A1F8 <= f2b) f2b -= lbl_eu_8066A1FC;
            while (f2b < -lbl_eu_8066A1F8) f2b += lbl_eu_8066A1FC;
            int inRange2;
            if ((-f31b <= f2b && f2b <= lbl_eu_80666CF8)
                || (lbl_eu_80666CF8 <= f2b && f2b <= f31b)) {
                inRange2 = 1;
            } else {
                inRange2 = 0;
            }
            if (inRange2 != 0) {
                self->mField14 = lbl_eu_80666CF8;
                memset(&self->mPos30, 0, 0x2c);
                func_80174C24(self->mPlayer, 0x40);
                break;
            }
        }
        func_800D5F98(self, src);
        break;
    }
    default:
        break;
    }
}

extern "C" void func_800D4F30(CtrlActView* self) {
    {
        CtrlActPlayerView* player = self->mPlayer;
        u32 local = *player->mField4->vf30();
        if (func_80174C98(player, &local, 0x807) == 0) {
            memset(&self->mPos30, 0, 0x2c);
            func_80174C24(self->mPlayer, 0x40);
            return;
        }
    }
    void* t = func_8016FE34(
        func_800B708C((int)(intptr_t)self->mPlayer->mSub3E9C.v17()));
    CtrlActSrc* src;
    if (t != 0) {
        src = (CtrlActSrc*)t;
    } else {
        src = 0;
    }
    if (src == 0) {
        memset(&self->mPos30, 0, 0x2c);
        func_80174C24(self->mPlayer, 0x40);
        return;
    }
    switch (self->mFlags58.mPhase) {
    case 0:
        // Fixed-duration setup: snapshot the source position/facing, set the
        // timer from the constants, pick the facing direction, then fall into
        // the phase-1 decay.
        {
            u32 flags = self->mPlayer->mField3F00;
            if (((u32)__cntlzw((u32)__cntlzw(flags & 2) >> 5) >> 5) != 0) {
                self->mField14 = lbl_eu_80666D0C;
            } else {
                self->mField14 = lbl_eu_80666CFC;
            }
            f32 f31;
            f32 f30;
            f30 = lbl_eu_80666CF8;
            f31 = lbl_eu_80666D3C;
            CVoicePos* pos = src->mOwner3E9C.getPosition();
            self->mPos30 = *(ml::CVec3*)pos;
            self->mField48 = src->vtbl->fn_0x5B4(src);
            self->mField54 = f31;
            self->mField50 = f30;
            CVoicePos* ppos = self->mPlayer->mSub3E9C.getPosition();
            ml::CVec3 diff;
            nw4r::math::VEC3Sub((nw4r::math::VEC3*)&diff,
                                (const nw4r::math::VEC3*)ppos,
                                (const nw4r::math::VEC3*)&self->mPos30);
            ml::CVec3 d = diff;
            f32 ang = nw4r::math::Atan2FIdx(d.x, d.z);
            self->mField4C = lbl_eu_80666D40 * ang;
            func_800D5D68(self);
            self->mFlags58.mPhase = self->mFlags58.mPhase + 1;
            // fallthrough
        }
    case 1: {
        f32 dt = func_80496288(lbl_eu_80663E14);
        self->mField54 -= lbl_eu_80666D58 * dt;
        if (self->mField54 <= lbl_eu_80666CF8) {
            self->mField14 = lbl_eu_80666CF8;
            memset(&self->mPos30, 0, 0x2c);
            func_80174C24(self->mPlayer, 0x40);
            break;
        }
        f32 f31 = lbl_eu_80666D44 * lbl_eu_8066A210;
        f32 f30 = lbl_eu_80666D90 * lbl_eu_8066A210;
        CVoicePos* ppos = self->mPlayer->mSub3E9C.getPosition();
        ml::CVec3 diff;
        nw4r::math::VEC3Sub((nw4r::math::VEC3*)&diff,
                            (const nw4r::math::VEC3*)ppos,
                            (const nw4r::math::VEC3*)&self->mPos30);
        ml::CVec3 d = diff;
        f32 ang = nw4r::math::Atan2FIdx(d.x, d.z);
        f32 f3 = lbl_eu_80666D40 * ang;
        // Wrap the facing reference (base + wobble) and the heading delta
        // into (-pi, pi], then check the delta is within the wobble band.
        f32 f2 = self->mField48 + f30;
        while (lbl_eu_8066A1F8 <= f2) f2 -= lbl_eu_8066A1FC;
        while (f2 < -lbl_eu_8066A1F8) f2 += lbl_eu_8066A1FC;
        f2 = f3 - f2;
        while (lbl_eu_8066A1F8 <= f2) f2 -= lbl_eu_8066A1FC;
        while (f2 < -lbl_eu_8066A1F8) f2 += lbl_eu_8066A1FC;
        int inRange;
        if ((-f31 <= f2 && f2 <= lbl_eu_80666CF8)
            || (lbl_eu_80666CF8 <= f2 && f2 <= f31)) {
            inRange = 1;
        } else {
            inRange = 0;
        }
        if (inRange != 0) {
            self->mField14 = lbl_eu_80666CF8;
            memset(&self->mPos30, 0, 0x2c);
            func_80174C24(self->mPlayer, 0x40);
        } else {
            func_800D5F98(self, src);
        }
        break;
    }
    default:
        break;
    }
}

void func_800D5308(CtrlActView* self) {
    // Declared first so MWCC's saved-register coloring matches retail
    // (same idiom as func_800D3998/func_800D11B0).
    CtrlActSrc* src;
    int blocked;
    {
        u32 local = *self->mPlayer->mField4->vf30();
        if (func_80174C98(self->mPlayer, &local, 0x807) == 0) {
            memset(&self->mPos30, 0, 0x2c);
            func_80174C24(self->mPlayer, 0x40);
            return;
        }
    }
    void* t = func_8016FE34(
        func_800B708C((int)(intptr_t)self->mPlayer->mSub3E9C.v17()));
    if (t != 0) {
        src = (CtrlActSrc*)t;
    } else {
        src = 0;
    }
    if (src == 0) {
        memset(&self->mPos30, 0, 0x2c);
        func_80174C24(self->mPlayer, 0x40);
        return;
    }
    // Facing angle (retail f31) and sine (retail f30) stay live across every
    // probe below. The probe offset (0,0,D68) is materialized before the trig
    // calls, then rotated around Y by the facing and scaled so the ground
    // probe point tracks the terrain height at the source.
    f32 angle = src->vtbl->fn_0x5B4(src);
    ml::CVec3 v;
    v.x = lbl_eu_80666CF8;
    v.y = lbl_eu_80666CF8;
    v.z = lbl_eu_80666D68;
    f32 sin = nw4r::math::SinFIdx(lbl_eu_80666D80 * angle);
    f32 cos = nw4r::math::CosFIdx(lbl_eu_80666D80 * angle);
    // In-place Y-rotation of v: x' = -sin*x + cos*z, z' = cos*x + sin*z.
    f32 vx = v.x;
    f32 vz = v.z;
    v.x = -sin * vx + lbl_eu_80666CF8 * v.y + cos * vz;
    v.y = lbl_eu_80666CF8 * vx + lbl_eu_80666CFC * v.y + lbl_eu_80666CF8 * vz;
    v.z = cos * vx + lbl_eu_80666CF8 * v.y + sin * vz;
    float* h = self->mPlayer->vtbl->fn_0x1C0(self->mPlayer);
    f32 scale = lbl_eu_80666D14 * (h[0] + src->mField44D8);
    v.x = v.x * scale;
    v.y = v.y * scale;
    v.z = v.z * scale;
    CVoicePos* pos = src->mOwner3E9C.getPosition();
    ml::CVec3 sumA = *(ml::CVec3*)pos + v;
    ml::CVec3 point = sumA;
    int r0;
    if (self->vf27(&sumA) != 0) {
        blocked = 1;
        ml::CVec3 up;
        up.x = lbl_eu_80666CF8;
        up.y = lbl_eu_80666CFC;
        up.z = lbl_eu_80666CF8;
        ml::CVec3 probe = point + up;
        ml::CVec3 probeCopy = probe;
        if (func_804BE398(&probeCopy, 0x40004a05, 0, 0, lbl_eu_80666D84,
                          lbl_eu_8066AF20) != 0) {
            if (((ml::CVec3*)func_804BE520(0))->y > lbl_eu_80666CF8) {
                blocked = 0;
            }
        }
        if (blocked != 0) {
            r0 = 1;
        } else if (self->vf26(&point, 1) != 0) {
            r0 = 0;
        } else {
            r0 = 1;
        }
    } else {
        r0 = 0;
    }
    if (r0 != 0) {
        memset(&self->mPos30, 0, 0x2c);
        func_80174C24(self->mPlayer, 0x40);
    } else {
        CtrlActVoiceOwnerView* vo =
            (CtrlActVoiceOwnerView*)&self->mPlayer->mSub3E9C;
        vo->vtbl->fn_0x9C(&self->mPlayer->mSub3E9C, &point);
        vo->vtbl->fn_0xC8(&self->mPlayer->mSub3E9C, angle);
        memset(&self->mPos30, 0, 0x2c);
        func_80174C24(self->mPlayer, 0x40);
    }
}

// Target us-800d61d8. Idle-facing update: when the 0x807 control gate is
// clear, reset the 0x30 state block; otherwise decay the action timer and
// re-aim at the current facing target while the timer is live (vf25 probe),
// with the aim/fx state value written from either the success or failure
// path.
extern "C" void func_800D56F0(CtrlActView* self) {
    CtrlActPlayerView* player = self->mPlayer;
    u32 id = *player->mField4->vf30();
    u32 local = id;
    if (func_80174C98(player, &local, 0x807) == 0) {
        memset(&self->mPos30, 0, 0x2c);
        func_80174C24(self->mPlayer, 0x40);
    } else {
        f32 next = self->mField54 - lbl_eu_80666D58 * func_80496288(lbl_eu_80663E14);
        self->mField54 = next;
        if (next > lbl_eu_80666CF8) {
            ml::CVec3 vec;
            if (self->vf25(&vec) != 0) {
                f32 az = vec.z;
                f32 ax = vec.x;
                f32 ang = nw4r::math::Atan2FIdx(ax, az);
                f32 v = lbl_eu_80666D40 * ang;
                self->mField14 = lbl_eu_80666D0C;
                self->mFieldC = v;
                self->mField10 = v;
            } else {
                self->mField14 = lbl_eu_80666CF8;
            }
        } else {
            memset(&self->mPos30, 0, 0x2c);
            func_80174C24(self->mPlayer, 0x40);
            self->mField14 = lbl_eu_80666CF8;
        }
    }
}

int func_800D5814(void* self) { return 0; }

void func_800D581C(void* self) {
    memset((u8*)self + 0x30, 0, 0x2C);
    func_80174C24(*(void**)((u8*)self + 0x5C), 0x40);
}

unsigned long func_800D5860(void* self) {
    unsigned long v = *(unsigned long*)((char*)self + 0x58);
    return !((v >> 15) & 1);
}

// Target us-800d635c. Action setup: clear the 0x30 state block, record the
// action kind + 0x8000 flag, snapshot the player position into 0x3C-0x44
// (y offset by a constant), then apply the kind-specific parameter:
// kind 3 stores the raw parameter, kinds 4/0xA set the timer constant, and
// kind 2 maps the parameter through the 0x28..0x2C ladder.
void func_800D5874(CtrlActView* self, u32 kindParam, int param2) {
    memset(&self->mPos30, 0, 0x2c);
    self->mFlags58.mKind = kindParam;
    self->mFlags58.mBit15 = 1;
    self->mField3C = self->mPlayer->mSub3E9C.getPosition()->f[0];
    self->mField40 = self->mPlayer->mSub3E9C.getPosition()->f[1] - lbl_eu_80666D00;
    self->mField44 = self->mPlayer->mSub3E9C.getPosition()->f[2];
    // Kind dispatch: retail lowers this as two equality-cascade switches
    // over cached register subjects (kind, then param2).
    u32 kind = self->mFlags58.mKind;
    if (kind == 3) {
        self->mFlags58.mParam = param2;
    } else if (kind == 4 || kind == 0xa) {
        self->mField54 = lbl_eu_80666D04;
    } else if (kind == 2) {
        // Param ladder, ascending compare cascade like retail.
        if (param2 == 0x28) {
            self->mFlags58.mParam = 5;
        } else if (param2 == 0x29) {
            self->mFlags58.mParam = 0xa;
        } else if (param2 == 0x2a) {
            self->mFlags58.mParam = 0xf;
        } else if (param2 == 0x2b) {
            self->mFlags58.mParam = 0x14;
        } else if (param2 == 0x2c) {
            self->mFlags58.mParam = 0x19;
        } else {
            self->mFlags58.mParam = 5;
        }
    }
}

void func_800D59FC(void* obj) {
    struct __attribute__((packed)) Data70 {
        float f70;
        unsigned short u74;
        unsigned short u76;
        unsigned char u78;
        unsigned char u79;
        unsigned char u7a;
        unsigned char u7b;
    };
    Data70* data = (Data70*)((char*)obj + 0x70);
    unsigned short temp = data->u74;
    f32 v = lbl_eu_80666CFC;  // 1.0f (SDA2 float pool)
    data->u78 = 0;
    data->u7b = 0;
    data->f70 = v;
    data->u7a = 0;
    data->u79 = 0;
    data->u76 = 0;
    data->u74 = temp & 0x2000;
}

// Target us-800d6514. Facing picker: when the player's 0x3F00 bit 1 is set,
// scan three enum-list candidates for the actors nearest to the player (via
// the voice positions), aim at the nearer of the first two and latch the turn
// direction into mAngleState (wrapped into (-pi, pi]); otherwise - or with no
// candidates - pick a random turn.
extern "C" void func_800D5A2C(CtrlActView* self) {
    CtrlActPlayerView* player = self->mPlayer;
    if (((u32)__cntlzw((u32)__cntlzw(player->mField3F00 & 2) >> 5) >> 5) != 0) {
        void* list[3] = {0, 0};
        f32 mags[3] = {0.0f, 0.0f};
        u32 data[3] = {lbl_eu_804FC810[0], lbl_eu_804FC810[1],
                       lbl_eu_804FC810[2]};
        ml::CVec3 vecs[3];
        int count = 0;
        for (int i = 0; i < 3; i++) {
            void* candidate = 0;
            CfEnumListHolder holder;
            func_80043D90(&holder);
            CfEnumList* lst = func_80043F18(&holder);
            func_800F4A98(lst, data[i], 0);
            lst = func_80043F18(&holder);
            int id = (int)(intptr_t)func_800F6E08(lst);
            if (id != 0) {
                void* actor = func_800B708C(id);
                if (actor != 0) {
                    actor = (u8*)actor - 0x3E9C;
                }
                candidate = actor;
            }
            if (candidate != 0 && candidate != (void*)player) {
                list[count] = candidate;
                CVoicePos* playerPos = player->mSub3E9C.getPosition();
                CVoicePos* candPos =
                    ((CVoiceOwnerIntfPc*)&((CtrlActSweepView*)candidate)
                         ->mOwner3E9C)
                        ->getPosition();
                ml::CVec3 diff;
                nw4r::math::VEC3Sub((nw4r::math::VEC3*)&diff,
                                    (const nw4r::math::VEC3*)candPos,
                                    (const nw4r::math::VEC3*)playerPos);
                vecs[count] = diff;
                mags[count] = PSVECMag((const Vec*)&vecs[count]);
                count++;
            }
            __dt__80043E88(&holder, -1);
        }
        if (count == 0) {
            if (ml::math::mtRand(2) == 0) {
                self->mFlags58.mAngleState = 1;
            } else {
                self->mFlags58.mAngleState = 0;
            }
            return;
        }
        int sel = 0;
        if (count == 2 && mags[0] > mags[1]) {
            sel = 1;
        }
        CVoicePos* playerPos = player->mSub3E9C.getPosition();
        ml::CVec3 diff2;
        nw4r::math::VEC3Sub((nw4r::math::VEC3*)&diff2,
                            (const nw4r::math::VEC3*)&self->mPos30,
                            (const nw4r::math::VEC3*)playerPos);
        ml::CVec3 d2 = diff2;
        f32 a1 = nw4r::math::Atan2FIdx(d2.x, d2.z);
        f32 f31 = lbl_eu_80666D40 * a1;
        f32 a2 = nw4r::math::Atan2FIdx(vecs[sel].x, vecs[sel].z);
        f32 f1 = lbl_eu_80666D40 * a2;
        f32 delta = f31 - f1;
        while (delta >= lbl_eu_8066A1F8) delta -= lbl_eu_8066A1FC;
        while (delta < -lbl_eu_8066A1F8) delta += lbl_eu_8066A1FC;
        if (delta >= lbl_eu_80666CF8) {
            self->mFlags58.mAngleState = 0;
        } else {
            self->mFlags58.mAngleState = 1;
        }
        return;
    }
    if (ml::math::mtRand(2) == 0) {
        self->mFlags58.mAngleState = 1;
    } else {
        self->mFlags58.mAngleState = 0;
    }
}

// Target us-800d6850. Facing-angle update: derive the facing delta from the
// player position vs the 0x30 position block, then pick the facing-state
// table (kind 6/5) for the 0x48 base angle, wrap both deltas into (-pi, pi]
// and latch the chosen delta's sign into the 0x58 angle-state bits.
void func_800D5D68(CtrlActView* self) {
    CVoicePos* pos = self->mPlayer->mSub3E9C.getPosition();
    ml::CVec3 diff;
    nw4r::math::VEC3Sub((nw4r::math::VEC3*)&diff,
                        (const nw4r::math::VEC3*)pos,
                        (const nw4r::math::VEC3*)&self->mPos30);
    ml::CVec3 d = diff;
    f32 ang = nw4r::math::Atan2FIdx(d.x, d.z);
    u32 kind = self->mFlags58.mKind;
    f32 f3 = lbl_eu_80666D40 * ang;
    f32 f4, f2;
    if (kind == 6) {
        f4 = self->mField48 + lbl_eu_80663EF8[0];
        f2 = self->mField48 + lbl_eu_80663EF8[1];
    } else if (kind == 5) {
        f4 = self->mField48 + lbl_eu_80573A20[0];
        f2 = self->mField48 + lbl_eu_80573A20[1];
    }
    while (lbl_eu_8066A1F8 <= f4) f4 -= lbl_eu_8066A1FC;
    while (f4 < -lbl_eu_8066A1F8) f4 += lbl_eu_8066A1FC;
    while (lbl_eu_8066A1F8 <= f2) f2 -= lbl_eu_8066A1FC;
    while (f2 < -lbl_eu_8066A1F8) f2 += lbl_eu_8066A1FC;
    f4 = f3 - f4;
    f2 = f3 - f2;
    while (lbl_eu_8066A1F8 <= f4) f4 -= lbl_eu_8066A1FC;
    while (f4 < -lbl_eu_8066A1F8) f4 += lbl_eu_8066A1FC;
    while (lbl_eu_8066A1F8 <= f2) f2 -= lbl_eu_8066A1FC;
    while (f2 < -lbl_eu_8066A1F8) f2 += lbl_eu_8066A1FC;
    f32 s1 = f4;
    if (f4 < lbl_eu_80666CF8) {
        s1 = f4 * lbl_eu_80666D68;
    }
    f32 s2 = f2;
    if (f2 < lbl_eu_80666CF8) {
        s2 = f2 * lbl_eu_80666D68;
    }
    // Pick the delta with the larger wrapped magnitude.
    if (s1 >= s2) {
        f4 = f2;
    }
    if (f4 >= lbl_eu_80666CF8) {
        self->mFlags58.mAngleState = 1;
    } else {
        self->mFlags58.mAngleState = 0;
    }
}

extern "C" int func_800D5F98(CtrlActView* self, CtrlActSrc* src) {
    // Facing helper shared by the action handlers. Re-aims the target angle
    // at the action source, wraps the heading into (-pi, pi], latches the
    // heading-band flags into the 0x04 word, then projects a unit circle
    // along the facing and probes the collision height before deciding
    // whether the action state must be cleared (return 0) or kept (1).
    // Retail indexes the facing table with flag-word bits 18-19
    // (rlwinm rX, rX, 10, 28, 29) - the upper half of mParam.
    f32 f31 = lbl_eu_80573A20[(self->mFlags58.mParam >> 4) & 3];
    CVoicePos* ppos = self->mPlayer->mSub3E9C.getPosition();
    CVoicePos* spos = src->mOwner3E9C.getPosition();
    // Retail spills each difference twice (paired store + component
    // copies), reproduced here with an explicit intermediate.
    ml::CVec3 diff1 = *(ml::CVec3*)spos - *(ml::CVec3*)ppos;
    ml::CVec3 d1 = diff1;
    f32 ang = nw4r::math::Atan2FIdx(diff1.x, diff1.z);
    self->mField10 = lbl_eu_80666D40 * ang;
    CVoicePos* ppos2 = self->mPlayer->mSub3E9C.getPosition();
    ml::CVec3 diff2 = self->mPos30 - *(ml::CVec3*)ppos2;
    ml::CVec3 d2 = diff2;
    f32 ang2 = nw4r::math::Atan2FIdx(diff2.x, diff2.z);
    f32 f1 = lbl_eu_80666D40 * ang2;
    f32 f3 = f1 + f31;
    while (lbl_eu_8066A1F8 <= f3) f3 -= lbl_eu_8066A1FC;
    while (f3 < -lbl_eu_8066A1F8) f3 += lbl_eu_8066A1FC;
    int r6 = 0;
    int r5 = 0;
    int r4 = 0;
    int r3 = 0;
    f32 f1b = f31 * lbl_eu_8066A20C;
    self->mFieldC = f3;
    if (f1b > lbl_eu_80666D94 && f1b < lbl_eu_80666D98) r6 = 1;
    if (f1b < lbl_eu_80666D9C && f1b > lbl_eu_80666DA0) r5 = 1;
    if (f1b < lbl_eu_80666D9C && f1b >= lbl_eu_80666D94) r4 = 1;
    if (f1b < lbl_eu_80666D98 || f1b > lbl_eu_80666DA0) r3 = 1;
    if (r6) {
        self->mField4 |= 0x800;
    } else {
        self->mField4 &= ~0x800;
    }
    if (r5) {
        self->mField4 |= 0x400;
    } else {
        self->mField4 &= ~0x400;
    }
    if (r4) {
        self->mField4 |= 0x2000;
    } else {
        self->mField4 &= ~0x2000;
    }
    if (r3) {
        self->mField4 |= 0x1000;
    } else {
        self->mField4 &= ~0x1000;
    }
    if (r6 || r5) {
        self->mFlags58.mBit13 = 1;
    } else {
        self->mFlags58.mBit13 = 0;
    }
    // Project the unit circle along the stored facing: build the Y-rotation
    // from mFieldC, rotate the (0, 0, 1) local, then scale it and add the
    // player position for the collision probe.
    f32 f30 = self->mFieldC;
    f32 f1c = lbl_eu_80666D80 * f30;
    self->mFlags58.mBit14 = 1;
    ml::CVec3 v(lbl_eu_80666CF8, lbl_eu_80666CF8, lbl_eu_80666CFC);
    f32 sin = nw4r::math::SinFIdx(f1c);
    f32 cos = nw4r::math::CosFIdx(lbl_eu_80666D80 * f30);
    ml::CMat33 mat(cos, lbl_eu_80666CF8, sin,
                   lbl_eu_80666CF8, lbl_eu_80666CFC, lbl_eu_80666CF8,
                   -sin, lbl_eu_80666CF8, cos);
    v = ml::CVec3(mat.m[0][0] * v.x + mat.m[0][2] * v.z,
                  mat.m[1][1] * v.y,
                  mat.m[2][0] * v.x + mat.m[2][2] * v.z);
    ml::CVec3 scaled = v * lbl_eu_80666CFC;
    CVoicePos* ppos3 = self->mPlayer->mSub3E9C.getPosition();
    ml::CVec3 sum = *(ml::CVec3*)ppos3 + scaled;
    if (self->vf26(&sum, 1) == 0) {
        ml::CVec3 offset(lbl_eu_80666CF8, lbl_eu_80666CFC, lbl_eu_80666CF8);
        ml::CVec3 probe = sum + offset;
        int r29 = 1;
        if (func_804BE398(&probe, 0x4a05, 0, 0, lbl_eu_80666D84,
                          lbl_eu_8066AF20) != 0) {
            if (((ml::CVec3*)func_804BE520(0))->y > lbl_eu_80666CF8) {
                r29 = 0;
            }
        }
        if (r29 != 0) {
            self->mField14 = lbl_eu_80666CF8;
            memset(&self->mPos30, 0, 0x2c);
            func_80174C24(self->mPlayer, 0x40);
            return 0;
        }
    }
    return 1;
}

int func_800D64D8(void* self) { return 0; }

int func_800D64E0(void* self) { return 0; }

// Target us-800d6fd0. Facing-clearance probe: gate on the player's 0x3374
// bit 15 and a live battle target, project a unit circle along mFieldC at the
// player position, and scan the collision results for the highest point below
// the probe - if the tallest hit leaves a gap wider than the threshold the
// facing is clear (return 0).
extern "C" int func_800D64E8(CtrlActView* self) {
    if ((self->mPlayer->mField3374 & 0x10000) != 0) {
        CtrlActTargetView* target = self->mPlayer->mField3F60;
        if (target != 0 && target->mField4F8 > lbl_eu_80666CF8) {
            f32 cos = nw4r::math::CosFIdx(self->mFieldC * lbl_eu_80666D80);
            f32 sin = nw4r::math::SinFIdx(self->mFieldC * lbl_eu_80666D80);
            ml::CVec3 local(sin, lbl_eu_80666CF8, cos);
            ml::CVec3 pos =
                *(ml::CVec3*)self->mPlayer->mSub3E9C.getPosition();
            ml::CVec3 sum = pos + local;
            if (func_804BE398(&sum, 0x4a11, 0, 1, lbl_eu_80666DA4,
                              lbl_eu_8066AF20) != 0) {
                int best = -1;
                f32 bestY = lbl_eu_80666DA8;
                // Retail scans with an unsigned counter here but a signed
                // one in the second pass.
                for (u32 i = 0; i < (u32)func_804BE4AC(); i++) {
                    if (((ml::CVec3*)func_804BE520(i))->y
                        >= lbl_eu_80666CF8) {
                        if (bestY > ((ml::CVec3*)func_804BE50C(i))->y) {
                            bestY = ((ml::CVec3*)func_804BE50C(i))->y;
                            best = i;
                        }
                    }
                }
                if (best >= 0 && func_804BE5A4(0x40000, best) != 0) {
                    f32 low = bestY - lbl_eu_80666D4C;
                    for (int i = 0; i < func_804BE4AC(); i++) {
                        if (best != i && bestY > ((ml::CVec3*)func_804BE50C(i))->y
                            && low < ((ml::CVec3*)func_804BE50C(i))->y) {
                            low = ((ml::CVec3*)func_804BE50C(i))->y;
                        }
                    }
                    if (bestY - low > lbl_eu_80666D20) {
                        return 0;
                    }
                }
            }
        }
    } else {
        return 0;
    }
    self->mField14 = lbl_eu_80666CF8;
    return 1;
}

// Target us-800d7208. Height-clearance check for the facing direction:
// probe a unit circle along mFieldC at the player position, find the highest
// collision hit; depending on its height vs the player, gate on 0x40000/0x20000
// flags and the target's 0x4EC bit 8, and latch the result into mField74.
extern "C" int func_800D6720(CtrlActView* self, int flag) {
    self->mField74 &= 0xfff1;
    CtrlActTargetView* target = self->mPlayer->mField3F60;
    if (target != 0) {
    f32 cos = nw4r::math::CosFIdx(self->mFieldC * lbl_eu_80666D80);
    f32 z = self->mPlayer->mSub3E9C.getPosition()->f[2]
            + lbl_eu_80666DAC * cos;
    f32 y = self->mPlayer->mSub3E9C.getPosition()->f[1] + lbl_eu_80666D04;
    f32 sin = nw4r::math::SinFIdx(self->mFieldC * lbl_eu_80666D80);
    ml::CVec3 local;
    local.x = self->mPlayer->mSub3E9C.getPosition()->f[0]
              + lbl_eu_80666DAC * sin;
    local.y = y;
    local.z = z;
    if (func_804BE398(&local, 0x4a05, 0, 1, lbl_eu_80666DB0,
                      lbl_eu_8066AF20) != 0) {
        f32 bestY = lbl_eu_80666DA8;
        int best = -1;
        // Retail scans with an unsigned counter here.
        for (u32 i = 0; i < (u32)func_804BE4AC(); i++) {
            if (bestY < ((ml::CVec3*)func_804BE50C(i))->y) {
                bestY = ((ml::CVec3*)func_804BE50C(i))->y;
                best = i;
            }
        }
        if (best >= 0) {
            if (((ml::CVec3*)func_804BE520(best))->y < lbl_eu_80666CF8) {
                f32 py = self->mPlayer->mSub3E9C.getPosition()->f[1];
                if (bestY <= py) {
                    return 1;
                }
            }
            self->mField74 |= 2;
            if (func_804BE5A4(0x40000, best) != 0) {
                f32 low = bestY - lbl_eu_80666D4C;
                for (int i = 0; i < func_804BE4AC(); i++) {
                    if (best != i && bestY > ((ml::CVec3*)func_804BE50C(i))->y
                        && low < ((ml::CVec3*)func_804BE50C(i))->y) {
                        low = ((ml::CVec3*)func_804BE50C(i))->y;
                    }
                }
                if (bestY - low > target->mField508) {
                    self->mField74 |= 8;
                    return 1;
                }
            }
        }
        if (func_804BE5A4(0x20000, best) != 0) {
            if ((target->mField4EC & 0x100) == 0) {
                self->mField74 |= 4;
                return 1;
            }
        }
    } else {
        return 1;
    }
    }
    return 0;
}

void func_800D69D8(){}

// Target us-800d74c0. Voice/battle-target action: when the caller flag is
// set the action state is cleared; otherwise the 0x40/0x180 control bits run
// the probe-countdown and distance checks, the 0x200 bit runs the two-step
// facing loop (segment probe + vf26 walk), and the 0x40-clear branch
// resolves the facing from the player's base-facing angle. The common tail
// decrements the mParam countdown or re-aims through vf23.
extern "C" void func_800D69D8(CtrlActView* self, ml::CVec3* pos,
                              CtrlActSrc* src, int flag) {
    ml::CVec3* target = pos;
    if (flag != 0) {
        self->mField7B = 0;
        self->mField74 &= 0xF83F;
    } else if ((self->mField74 & 0x40) != 0) {
        target = &self->mPos60;
        if ((self->mField74 & 0x180) != 0) {
            // Probe countdown: every 16th tick past 30 casts an up-offset
            // segment probe from the mPos60 block and latches the result
            // into the countdown itself.
            self->mField7B++;
            if ((u8)self->mField7B >= 0x1e
                && ((u8)self->mField7B & 0xf) == 0xf) {
                ml::CVec3 sum =
                    self->mPos60 +
                    ml::CVec3(lbl_eu_80666CF8, lbl_eu_80666D0C,
                              lbl_eu_80666CF8);
                // Pre-compute the range object so the world-pointer load
                // lands last (right before the call) like retail.
                u8* rangeObj = (u8*)self->mPlayer + 0x44A8;
                if (func_804B54D4(lbl_eu_80665958, rangeObj, &sum, 0,
                                  0) != 0) {
                    self->mField7B = 0x78;
                }
            }
            if (self->mField7B >= 0x78) {
                self->mField7B = 0;
                self->mField74 &= 0xFC3F;
            } else {
                CVoicePos* pp = self->mPlayer->mSub3E9C.getPosition();
                ml::CVec3 diff = *(ml::CVec3*)pp - self->mPos60;
                if (diff.x * diff.x + diff.z * diff.z <= lbl_eu_80666DB4) {
                    self->mField7B = 0;
                    if (self->mField74 & 0x80) {
                        self->mField74 = (self->mField74 | 0x200) & ~0x80;
                    } else {
                        self->mField74 &= 0xFC3F;
                    }
                }
            }
        }
        if ((self->mField74 & 0x200) != 0) {
            // Two-step facing scan: for each of two candidate directions
            // (alternating +1/-1), probe the segment from the player
            // position and walk it with vf26; the first success commits the
            // target block into mPos60 and the 0x100 latch.
            u16 save455A = src->mField455A;
            src->mField455A = 1;
            f32 f30 = lbl_eu_80666D0C * lbl_eu_8066A204;
            f32 f29 = lbl_eu_8066A204;
            f32 f24 = lbl_eu_80666D80;
            f32 f26 = lbl_eu_80666CF8;
            f32 f27 = lbl_eu_80666CFC;
            f32 f28 = lbl_eu_80666DB8;
            f64 magic = lbl_eu_80666D50;
            u32 magicHi = 0x43300000;
            int r22 = 1;
            for (int i = 0; i < 2; i++) {
                f32 dir = (i & 1) ? lbl_eu_80666CFC : lbl_eu_80666D68;
                F64Conv conv;
                conv.w[0] = magicHi;
                conv.w[1] = (u32)(self->mField7B & 1) ^ 0x80000000;
                f32 t = (f32)(conv.d - magic);
                f32 f23 = dir * (f30 * t + f29) + self->mField6C;
                f32 cosv = nw4r::math::CosFIdx(f24 * f23);
                f32 sinv = nw4r::math::SinFIdx(f24 * f23);
                ml::CVec3 dirVec(sinv, f26, cosv);
                f32 pz = self->mPlayer->mSub3E9C.getPosition()->f[2];
                f32 py = lbl_eu_80666CFC +
                         self->mPlayer->mSub3E9C.getPosition()->f[1];
                f32 px = self->mPlayer->mSub3E9C.getPosition()->f[0];
                ml::CVec3 pos2(px, py, pz);
                ml::CVec3 scaled = dirVec * f28;
                ml::CVec3 sum = pos2 + scaled;
                ml::CVec3 probe = sum;
                u8* rangeObj = (u8*)self->mPlayer + 0x44A8;
                if (func_804B526C(lbl_eu_80665958, rangeObj, &pos2,
                                  &probe, 0, 0, 0) == 0) {
                    if (self->vf26(&probe, 1) != 0) {
                        self->mField7B = 0;
                        r22 = 0;
                        self->mField74 = (self->mField74 & ~0x200) | 0x100;
                        self->mPos60 = probe;
                        break;
                    }
                }
            }
            src->mField455A = save455A;
            if (r22 != 0) {
                self->mField7B++;
                if ((u8)self->mField7B >= 2) {
                    self->mField7B = 0;
                    self->mField74 &= 0xFC3F;
                }
                CVoicePos* pp = self->mPlayer->mSub3E9C.getPosition();
                ml::CVec3 diff = *(ml::CVec3*)pp - self->mPos60;
                if (diff.x * diff.x + diff.z * diff.z <= lbl_eu_80666D6C) {
                    target = pos;
                }
            }
        }
    } else {
        // Facing-resolve branch: probe along the player's base-facing
        // direction; on a hit, refresh the mPos60 block from the player
        // position plus the facing offset and walk it with vf26.
        u16 save455A = src->mField455A;
        src->mField455A = 1;
        f32 facing = self->mPlayer->vtbl->fn_0x5B4(self->mPlayer);
        f32 cosv = nw4r::math::CosFIdx(lbl_eu_80666D80 * facing);
        f32 sinv = nw4r::math::SinFIdx(lbl_eu_80666D80 * facing);
        ml::CVec3 dirVec(sinv, lbl_eu_80666CF8, cosv);
        f32 pz = self->mPlayer->mSub3E9C.getPosition()->f[2];
        f32 py =
            lbl_eu_80666CFC + self->mPlayer->mSub3E9C.getPosition()->f[1];
        f32 px = self->mPlayer->mSub3E9C.getPosition()->f[0];
        ml::CVec3 pos2(px, py, pz);
        ml::CVec3 scaled = dirVec * lbl_eu_80666D04;
        ml::CVec3 sum = pos2 + scaled;
        ml::CVec3 probe = sum;
        int hit = 0;
        u8* rangeObj = (u8*)self->mPlayer + 0x44A8;
        if (func_804B526C(lbl_eu_80665958, rangeObj,
                          &pos2, &probe, 0, 0, 0) == 0) {
            f32 scale =
                (self->mField74 & 0x400) ? lbl_eu_80666DBC
                                         : lbl_eu_80666DC0;
            ml::CVec3 scaled2 = dirVec * scale;
            pos2.y = pos2.y + lbl_eu_80666D0C;
            ml::CVec3 sum2 = pos2 + scaled2;
            ml::CVec3 probe2 = sum2;
            u8* rangeObj2 = (u8*)self->mPlayer + 0x44A8;
            if (func_804B4E10(lbl_eu_80665958, rangeObj2, &probe2, 0, 0,
                              0) != 0) {
                hit = 1;
            } else {
                if (self->mField74 & 0x400) {
                    self->mField74 &= ~0x400;
                } else {
                    self->mField74 |= 0x400;
                }
            }
        } else {
            hit = 1;
        }
        if (hit != 0) {
            self->mField7B++;
            if ((u8)self->mField7B >= 0x1e) {
                self->mField7B = 0;
                f32 facing2 = self->mPlayer->vtbl->fn_0x5B4(self->mPlayer);
                self->mField6C = facing2;
                f32 ang = facing2 + lbl_eu_8066A1F8;
                f32 cosv2 = nw4r::math::CosFIdx(lbl_eu_80666D80 * ang);
                f32 sinv2 = nw4r::math::SinFIdx(lbl_eu_80666D80 * ang);
                ml::CVec3 dir3(sinv2, lbl_eu_80666CF8, cosv2);
                ml::CVec3 scaled3 = dir3 * lbl_eu_80666D18;
                CVoicePos* pp = self->mPlayer->mSub3E9C.getPosition();
                ml::CVec3 sum3 = *(ml::CVec3*)pp + scaled3;
                ml::CVec3 newPos = sum3;
                self->mPos60 = newPos;
                if (self->vf26(&self->mPos60, 1) != 0) {
                    target = &self->mPos60;
                    self->mField74 |= 0xC0;
                }
            }
        } else {
            self->mField7B = 0;
        }
        src->mField455A = save455A;
    }
    // .L_800D7D50: mParam countdown, or re-aim through vf23 when the
    // countdown is clear.
    if (self->mField74 & 0x1000) {
        self->mField14 = lbl_eu_80666CF8;
        u8 param = self->mFlags58.mParam;
        if (param - 1 > 0) {
            self->mFlags58.mParam = param - 1;
        } else {
            self->mField74 &= ~0x1000;
        }
    } else {
        f32 x8;
        ml::CVec3 outE4;
        if (self->vf23(&outE4, &x8, target, 1, 0) != 0) {
            f32 f23 = lbl_eu_80666D40 *
                      nw4r::math::Atan2FIdx(outE4.x, outE4.z);
            self->mFieldC = f23;
            if (func_800D6720(self, 0) != 0) {
                if (self->mField74 & 0x8) {
                    self->mFieldC = f23;
                    self->mField14 = lbl_eu_80666CF8;
                    self->mField79 = 0;
                } else if (!(self->mField74 & 0x2)) {
                    u8 old = self->mField79;
                    self->mField79 = old + 1;
                    if ((s8)old >= 0x28) {
                        self->mField79 = 0x2D;
                        self->mFieldC = f23;
                        self->mField14 = lbl_eu_80666CF8;
                    }
                }
            } else {
                self->mField79 = 0;
            }
            if (*(u8*)&x8 != 0) {
                if (self->mField14 != lbl_eu_80666CF8) {
                    CVoicePos* pp = self->mPlayer->mSub3E9C.getPosition();
                    f32 dx = pos->x - pp->f[0];
                    CVoicePos* pp2 = self->mPlayer->mSub3E9C.getPosition();
                    f32 dz = pos->z - pp2->f[2];
                    f32 r = self->mPlayer->mField44D8;
                    if (dx * dx + dz * dz <= lbl_eu_80666D04 * r) {
                        self->mFlags58.mParam = 0x3C;
                        self->mField74 |= 0x1000;
                        self->mField14 = lbl_eu_80666CF8;
                    }
                }
            }
            self->mField10 = self->mFieldC;
        } else {
            self->mField79 = 0;
            f32 f23 = lbl_eu_80666D40 *
                      nw4r::math::Atan2FIdx(outE4.x, outE4.z);
            self->mFieldC = f23;
            if (func_800D6720(self, 0) != 0) {
                self->mFieldC = f23;
                self->mField14 = lbl_eu_80666CF8;
            }
            if (self->mField14 != lbl_eu_80666CF8) {
                CVoicePos* pp = self->mPlayer->mSub3E9C.getPosition();
                f32 dx = pos->x - pp->f[0];
                CVoicePos* pp2 = self->mPlayer->mSub3E9C.getPosition();
                f32 dz = pos->z - pp2->f[2];
                f32 r = self->mPlayer->mField44D8;
                if (dx * dx + dz * dz <= lbl_eu_80666D04 * r) {
                    self->mFlags58.mParam = 0x3C;
                    self->mField74 |= 0x1000;
                    self->mField14 = lbl_eu_80666CF8;
                }
            }
            self->mField10 = self->mFieldC;
        }
    }
}

extern "C" void func_800D755C(CtrlActView* self, ml::CVec3* pos) {
    int r31 = 0;
    ml::CVec3 out;
    f32 x8;
    if ((self->mPlayer->mField3374 & 0x100) != 0) {
        // Probe countdown: tick the 0x78 counter; on expiry reset it and cast
        // an up-offset segment probe between the player and the target.
        r31 = 1;
        u8 c = self->mField78;
        self->mField78 = c - 1;
        if ((s8)c > 0) {
            goto probe_done;
        }
        self->mField78 = 0x78;
        ml::CVec3 offset;
        offset.x = lbl_eu_80666CF8;
        offset.y = lbl_eu_80666DC4;
        offset.z = lbl_eu_80666CF8;
        CVoicePos* ppos = self->mPlayer->mSub3E9C.getPosition();
        ml::CVec3 playerProbe = *(ml::CVec3*)ppos + offset;
        ml::CVec3 targetProbe = *pos + offset;
        if (func_804BE348(&playerProbe, &targetProbe, 0x44A11, 0, 0) != 0) {
            self->mField74 &= ~1;
        } else {
            self->mField74 |= 1;
        }
    }
probe_done:
    // Dispatch: when the bit-0 flag is set, clear bit 5 and aim once;
    // otherwise aim through vf23 and, when it rejects, walk the probe
    // counters (0x7B) and clear the facing when the target is close.
    if (self->mField74 & 1) {
        self->mField74 &= ~0x20;
        self->vf23(&out, &x8, pos, 1, 1);
        goto finish;
    }
    if (self->vf23(&out, &x8, pos, 1, 0) != 0) {
        self->mField7B = 0;
        self->mField74 &= ~0x820;
        goto finish;
    }
    self->mField74 |= 0x20;
    if (r31 == 0) {
        f32 cos = nw4r::math::CosFIdx(self->mFieldC * lbl_eu_80666D80);
        CVoicePos* ppos = self->mPlayer->mSub3E9C.getPosition();
        f32 f29 = lbl_eu_80666D0C * cos + ppos->f[2];
        CVoicePos* ppos2 = self->mPlayer->mSub3E9C.getPosition();
        f32 f30 = lbl_eu_80666D04 + ppos2->f[1];
        f32 sin = nw4r::math::SinFIdx(self->mFieldC * lbl_eu_80666D80);
        CVoicePos* ppos3 = self->mPlayer->mSub3E9C.getPosition();
        f32 fx = lbl_eu_80666D0C * sin + ppos3->f[0];
        ml::CVec3 local2;
        local2.x = fx;
        local2.y = f30;
        local2.z = f29;
        if (func_804BE398(&local2, 0x4a11, 0, 0, lbl_eu_80666DB0,
                          lbl_eu_8066AF20) != 0) {
            u16 v = self->mField74;
            if (v & 0x400) {
                self->mField14 = lbl_eu_80666CF8;
                CtrlActTargetView* target = self->mPlayer->mField3F60;
                if (target != 0 && (target->mField4EC & 1) == 0) {
                    self->mField7B = self->mField7B + 1;
                } else {
                    self->mField7B = self->mField7B + 2;
                }
                if (self->mField7B >= 0x3C) {
                    self->mField7B = 0;
                    self->mField74 &= ~0x800;
                }
            } else {
                CtrlActTargetView* target = self->mPlayer->mField3F60;
                if (target != 0 && (target->mField4EC & 1) != 0) {
                    self->mField7B = self->mField7B + 1;
                    if (self->mField7B >= 0xF) {
                        self->mField7B = 0;
                        self->mField74 = v | 0x800;
                    }
                } else {
                    self->mField7B = 0;
                }
            }
        } else {
            self->mField14 = lbl_eu_80666CF8;
        }
    }
    {
        CVoicePos* ppos = self->mPlayer->mSub3E9C.getPosition();
        f32 dx = pos->x - ppos->f[0];
        CVoicePos* ppos2 = self->mPlayer->mSub3E9C.getPosition();
        f32 dz = pos->z - ppos2->f[2];
        f32 dist2 = dx * dx + dz * dz;
        f32 r2 =
            self->mPlayer->mField44D8 * self->mPlayer->mField44D8;
        if (dist2 <= r2) {
            self->mField14 = lbl_eu_80666CF8;
        }
    }
finish:
    f32 ang = nw4r::math::Atan2FIdx(out.x, out.z);
    f32 v = lbl_eu_80666D40 * ang;
    self->mFieldC = v;
    self->mPlayer->mSub3E9C.v47(v);
}


// --- hard-symbol stubs (scaffold_hard_symbols) ---
// Static init: fill the facing tables with scaled defaults -
// lbl_eu_80573A20[0..3] (index 2 untouched) and lbl_eu_80663EF8[0..1].
void sinit_800D79B4() {
    f32 a = lbl_eu_80666D88 * lbl_eu_8066A210;
    f32 b = lbl_eu_80666D8C * lbl_eu_8066A210;
    f32 c = lbl_eu_80666D90 * lbl_eu_8066A210;
    f32 d = lbl_eu_80666DC8 * lbl_eu_8066A210;
    f32 e = lbl_eu_80666DCC * lbl_eu_8066A210;
    lbl_eu_80573A20[0] = a;
    lbl_eu_80573A20[1] = b;
    lbl_eu_80573A20[3] = c;
    lbl_eu_80663EF8[0] = d;
    lbl_eu_80663EF8[1] = e;
}
