// Auto-scaffolded catalog TU for kyoshin/cf/CtrlRemote
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/cf/CBattleManagerApi.hpp"
#include "kyoshin/cf/CfMapItemManager.hpp"
// harness_catalog.hpp removed: it pulls CTaskGameEff.hpp ->
// CfObjectImplMove.hpp, whose func_804BE398 decl clashes with
// CtrlMoveBase.hpp (via CtrlPc.hpp). Nothing in this TU needs it.

#include "kyoshin/cf/CtrlRemote.hpp"
// CtrlPc.hpp scaffolds __ct__CtrlRemote as `void`; this TU declares the real
// returning signature (CtrlRemote.hpp). Hide the scaffold decl so the two do
// not conflict (the hidden name is never referenced in this TU).
#define __ct__CtrlRemote __ct__CtrlRemote_scaffold
#include "kyoshin/cf/CtrlPc.hpp"
#undef __ct__CtrlRemote
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CfPadTask.hpp"
#include "monolib/core/CPadManager.hpp"
#include "monolib/util/MemManager.hpp"

#include <stdlib.h>
#include <string.h>

cf::CtrlRemote* __ct__CtrlRemote(cf::CtrlRemote* obj, void* posObj, int arg5)
{
    __ct__cf_CtrlPc(reinterpret_cast<cf::CtrlPc*>(obj), posObj, (void*)arg5);
    CtrlRemoteLayout* self = reinterpret_cast<CtrlRemoteLayout*>(obj);
    // novtable class: store the retail vtable manually.
    self->vtbl = (void*)lbl_eu_80527F88;
    self->mField25C = 0;
    self->mField260 = 0;
    for (int i = 0; i < 32; i++) {
        self->mFloats1DC[i] = lbl_eu_80666730;
    }
    CPadManager::updateLongHoldTimerThreshold(0xa);
    return obj;
}

// Probe a control-data gate: refresh the player's current control word via
// the sub-object's vf30 and test it against `gate`. Inlined so the player
// pointer stays in a saved register across the vf30 call, matching retail.
static int probePlayerCtrl(cf::CtrlPc* self, u32* out, int gate);

// Target us-80099690. Per-frame remote-control update: when neither the
// 0x20 nor the 0x400000 global gate is set, reset the +0x04 flag word and
// the low byte of +0x2C, run the state virtual sweep, refresh the voice-
// owner 4-gate and mirror it into the 0x800 flag range, then (outside menu
// mode) re-aim via vf37 and push one sample into the 32-float ring buffer.
void func_80098CB8(CtrlPcVf44* self)
{
    if (((lbl_eu_80663E24 & 0x02000000) | (lbl_eu_80663E24 & 0x400)) != 0) {
        return;
    }

    // clear +0x04, keep only the low byte of +0x2C
    self->mField4 = 0;
    self->mField2C &= 0xFF;

    // State-refresh virtual sweep.
    self->vf41();
    self->vf33();
    self->vf35();
    self->vf36();
    self->vf39();
    self->vf42();
    self->vf43();
    self->vf44();
    self->vf20();
    self->vf34();
    self->vf21();
    self->vf40();

    // Gate 4 on the embedded voice-owner interface: arm the 0x800 range.
    if (self->mField5C->mSub3E9C.v01(4) != 0) {
        self->mField5C->mSub3ED4->vf10(0x800, 1);
    }

    cf::CfGameManager::getInstance();
    if (isGlobalCamFlagSet(0x4000000)) {
        return;
    }

    u32 mask = isClassicController__Q22cf13CfGameManagerFv(-1)
                   ? lbl_eu_80527F10[21]
                   : lbl_eu_80527E98[21];
    CtrlPcSub37Ae80* sub = self->vf37();
    if ((sub->mField4 & mask) != 0) {
        func_80133770();
    }

    // Push one sample into the float ring: the constant while the cutscene
    // bit is clear, else the current target angle; wrap at 32 entries.
    if ((self->mField4 & 0x200) == 0) {
        u32 i = self->mField25C;
        self->mField25C = i + 1;
        self->mFloats1DC[i] = lbl_eu_80666748;
    } else {
        u32 i = self->mField25C;
        self->mField25C = i + 1;
        self->mFloats1DC[i] = self->mFieldC;
    }
    if ((s32)self->mField25C >= 0x20) {
        self->mField25C = 0;
    }
}

// Target us-800998d0. Player-aim update: probe the player's control gates
// and battle-manager state, then derive the aim fields (mField8/mFieldC /
// mField10/mField14) from the voice-owner target positions, the stick
// bytes, and the 0x1000 flag-range state. The final pass mirrors the
// aim-sector bits into self->mField4 and re-aims at the battle target.
void func_80098EF8(CtrlPcVf38* self)
{
    cf::CfGameManager::getInstance();
    UnkClass_800821F8View* gm8 = (UnkClass_800821F8View*)getCameraDataBlock__Q22cf13CfGameManagerFv();
    CtrlVoiceHandle* actionSrc =
        (CtrlVoiceHandle*)findObjectById(func_800FE68C()->mField90E4);

    u32 v1a;    // probe 0x1a
    u32 v1;
    u32 v2;
    u32 v8;
    u32 v1000;
    u32 v4000;
    u32 v8b;
    u32 v3;
    u32 v6;
    u32 v801;
    u32 v803;
    u32 va;
    u32 v1c;
    u32 v1d;
    u32 v12;
    u32 v13;
    u32 v805;
    u32 v4000b;
    u32 v803b;
    u32 v19;
    u32 v3b;
    u32 v6b;
    u32 v9;
    u32 v806;
    u32 v801b;

    u32 vf14res = self->mField5C->mSub3ED4->vf14(0x800);
    u8 bs = ((CBattleManagerViewPc*)getInstance__Q22cf14CBattleManagerFv())->mField1AA;
    // Commuted second compare keeps MWCC from fusing the range test into
    // subi+cmpli; retail emits two separate cmplwi branches.
    int inBattle = (bs >= 1u && 0x18u >= bs) ? 1 : 0;
    int timerNZ =
        ((CBattleManagerViewPc*)getInstance__Q22cf14CBattleManagerFv())->mField20C8 != 0 ? 1 : 0;
    int probe1a = probePlayerCtrl((cf::CtrlPc*)self, &v1a, 0x1a);

    CtrlPlayerSub3F60* sub3f60 = self->mField5C->mField3F60;
    if (sub3f60 != NULL) {
        if (probe1a != 0) {
            if ((sub3f60->mField4EC & 0x10) == 0) {
                sub3f60->mField4EC |= 0x10;
                CVoicePos* pos = self->mField5C->mSub3E9C.getPosition();
                self->mField5C->mField3F60->mField510 = pos->f[1];
            }
        } else {
            cf::CfGameManager::getInstance();
            if (isGlobalCamFlagSet(0x4000000) == 0) {
                CtrlPlayerSub3F60* s = self->mField5C->mField3F60;
                u32 f4ec = s->mField4EC;
                if ((f4ec & 0x10) != 0) {
                    if ((s->mFieldC & 2) != 0 || (f4ec & 2) != 0) {
                        s->mField4EC &= ~0x10;
                    }
                }
            }
        }
    }

    // Player-state probe 0xb: re-aim the facing angle from the vf25 vector.
    if (func_80148778(&self->mField5C->mField8, 0xb) != 0) {
        float vec[3];
        if (self->vf25(vec) != 0 && (lbl_eu_80663E24 & 0x4000000) == 0) {
            f32 ang = Atan2FIdx__Q24nw4r4mathFff(vec[0], vec[2]);
            f32 v = ang * lbl_eu_8066674C;
            self->mField14 = lbl_eu_80666750;
            self->mFieldC = v;
            self->mField10 = v;
        } else {
            self->mField14 = lbl_eu_80666730;
        }
    }

    // Control-gate guard chain: any active gate/state routes to the
    // aim/fallback block below; the fully-clear path takes the voice-owner
    // sub-gate (DD0) branch.
    u32 f28 = lbl_eu_80663E28;
    if (probePlayerCtrl((cf::CtrlPc*)self, &v1, 1) != 0 ||
        probePlayerCtrl((cf::CtrlPc*)self, &v2, 2) != 0 ||
        probePlayerCtrl((cf::CtrlPc*)self, &v8, 8) != 0 ||
        probePlayerCtrl((cf::CtrlPc*)self, &v1000, 0x1000) != 0 ||
        probePlayerCtrl((cf::CtrlPc*)self, &v4000, 0x4000) != 0 ||
        func_80148778(&self->mField5C->mField8, 0xf) != 0 ||
        func_80148778(&self->mField5C->mField8, 0x9) != 0 ||
        func_80148778(&self->mField5C->mField8, 0xa) != 0 ||
        func_80148778(&self->mField5C->mField8, 0xb) != 0 ||
        (getUnk80664658()->mField214 & 2) != 0 ||
        func_8029EE58() != 0 ||
        (lbl_eu_80663E24 & 0x8429A000) != 0 ||
        (f28 & 0x20000) != 0 || (f28 & 0x30) != 0 ||
        cf::CfPadTask::func_801C1BC0() != 0 ||
        inBattle != 0 ||
        timerNZ != 0 ||
        (((CtrlVoiceOwnerC4View*)&self->mField5C->mSub3E9C)->mFieldC4 !=
             NULL &&
         ((((CtrlVoiceOwnerC4View*)&self->mField5C->mSub3E9C)
               ->mFieldC4->mField274 &
           0x100000) != 0) &&
         ((((CtrlVoiceOwnerC4View*)&self->mField5C->mSub3E9C)
               ->mFieldC4->mField270 &
           0x200000) == 0))) {
        // Fallback: clear the 0x800 aim range unless the aim timer already
        // ran out, and drop the facing angle when the 0x8 gate or menu state
        // is active.
        if (vf14res == 0 || self->mField14 > lbl_eu_80666730) {
            self->mField5C->mSub3ED4->vf10(0x800, 1);
            if (func_80148778(&self->mField5C->mField8, 0xb) == 0) {
                self->mField14 = lbl_eu_80666730;
            }
            if (probePlayerCtrl((cf::CtrlPc*)self, &v8b, 8) != 0 ||
                func_80148778(&self->mField5C->mField8, 0xf) != 0) {
                self->mField8 = lbl_eu_80666730;
                if (self->mField5C->mField3F60 != NULL) {
                    self->mField5C->mField3F60->mFieldC |= 0x400;
                }
            } else {
                cf::CfGameManager::getInstance();
                if (isGlobalCamFlagSet(0x4000000) != 0) {
                    self->mField8 = lbl_eu_80666730;
                    if (self->mField5C->mField3F60 != NULL) {
                        self->mField5C->mField3F60->mFieldC |= 0x400;
                    }
                }
            }
        }
        if (inBattle != 0 || timerNZ != 0) {
            self->vf22();
            goto a89c;
        }
        return;
    }

    // Skipped via goto when the battle/aim-timer path exits; scoped so no
    // declaration between here and the a89c label is jumped over (MWCC 10211).
    {
    // DD0: voice-owner chain gate - when the 0x800 aim range is set and the
    // battle chain is idle, clear the range flags.
    if (vf14res != 0 && self->mField5C->mSub3E9C.v01(4) == 0 &&
        probePlayerCtrl((cf::CtrlPc*)self, &v3, 3) == 0 &&
        probePlayerCtrl((cf::CtrlPc*)self, &v6, 6) == 0 &&
        probePlayerCtrl((cf::CtrlPc*)self, &v801, 0x801) == 0) {
    } else {
        self->mField5C->mSub3ED4->vf10(0x800, 0);
    }

    // Stick-byte sanity: when any of the four signed bytes exceeds +/-10, or
    // the menu mask is active, flag the aim-override bit.
    CtrlPcSub37Ext* sub = (CtrlPcSub37Ext*)self->vf37();
    if (abs((s8)sub->mField58) > 0xa || abs((s8)sub->mField59) > 0xa ||
        abs((s8)sub->mField5A) > 0xa || abs((s8)sub->mField5B) > 0xa) {
        self->mField4 |= 0x200;
    } else {
        u32 mask2 = isClassicController__Q22cf13CfGameManagerFv(-1)
                        ? lbl_eu_80527F10[2]
                        : lbl_eu_80527E98[2];
        if ((self->vf37()->mField4 & mask2) != 0) {
            self->mField4 |= 0x200;
        }
    }

    // 0x803 gate: engage the 0x1000 aim range toward the action source.
    if (probePlayerCtrl((cf::CtrlPc*)self, &v803, 0x803) != 0) {
        if (actionSrc != 0) {
            ml::CVec3 diff;
            CVoicePos* p1 = self->mField5C->mSub3E9C.getPosition();
            CVoicePos* p2 = (CVoicePos*)actionSrc->vf41();
            diff.x = p2->f[0] - p1->f[0];
            diff.y = p2->f[1] - p1->f[1];
            diff.z = p2->f[2] - p1->f[2];
            ml::CVec3 d = diff;
            f32 dist2 = d.x * d.x + d.y * d.y + d.z * d.z;
            if (dist2 < lbl_eu_80666754) {
                if (self->mField5C->mSub3ED4->vf14(0x1000) == 0) {
                    self->mField5C->mSub3ED4->vf10(0x1000, 1);
                }
            } else {
                if (self->mField5C->mSub3ED4->vf14(0x1000) != 0) {
                    self->mField5C->mSub3ED4->vf11(0x1000);
                }
            }
        } else {
            if (self->mField5C->mSub3ED4->vf14(0x1000) != 0) {
                self->mField5C->mSub3ED4->vf11(0x1000);
            }
        }
    } else {
        if (self->mField5C->mSub3ED4->vf14(0x1000) != 0) {
            self->mField5C->mSub3ED4->vf11(0x1000);
        }
    }

    // 0xa gate: aim-sector flags from the action-source bearing.
    if (probePlayerCtrl((cf::CtrlPc*)self, &va, 0xa) == 0) {
        if (actionSrc != 0 &&
            self->mField5C->mSub3ED4->vf14(0x1000) != 0) {
            ml::CVec3 diff;
            CVoicePos* p1 = self->mField5C->mSub3E9C.getPosition();
            CVoicePos* p2 = (CVoicePos*)actionSrc->vf41();
            diff.x = p2->f[0] - p1->f[0];
            diff.y = p2->f[1] - p1->f[1];
            diff.z = p2->f[2] - p1->f[2];
            ml::CVec3 d = diff;
            if ((self->mField4 & 0x200) != 0) {
                f32 f31v =
                    lbl_eu_8066A1F8 +
                    ((CtrlGm8Vf1C*)gm8)->vf05()->mField4 - self->mFieldC;
                f32 f1 = lbl_eu_8066674C * Atan2FIdx__Q24nw4r4mathFff(d.x, d.z);
                f32 f3 = f31v - f1;
                while (f3 >= lbl_eu_8066A1F8) {
                    f3 -= lbl_eu_8066A1FC;
                }
                while (f3 < -lbl_eu_8066A1F8) {
                    f3 += lbl_eu_8066A1FC;
                }
                f32 ang = f3 * lbl_eu_8066A20C;
                if (ang > lbl_eu_80666758 && ang < lbl_eu_8066675C) {
                    self->mField4 |= 0x800;
                } else {
                    self->mField4 &= ~0x800;
                }
                if (ang < lbl_eu_80666760 && ang > lbl_eu_80666764) {
                    self->mField4 |= 0x400;
                } else {
                    self->mField4 &= ~0x400;
                }
                if (ang >= lbl_eu_80666760 && ang <= lbl_eu_80666758) {
                    self->mField4 |= 0x2000;
                } else {
                    self->mField4 &= ~0x2000;
                }
                if (ang >= lbl_eu_8066675C || ang <= lbl_eu_80666764) {
                    self->mField4 |= 0x1000;
                } else {
                    self->mField4 &= ~0x1000;
                }
            }
            self->mField10 =
                lbl_eu_8066674C * Atan2FIdx__Q24nw4r4mathFff(d.x, d.z);
        } else if ((self->mField4 & 0x200) != 0) {
            self->mField4 |= 0x2000;
        }
    }

    // Stick-angle magnitudes: derive f31/f30 from the stick bytes when the
    // vf37 state byte is set.
    CtrlPcSub37Ext* s2 = (CtrlPcSub37Ext*)self->vf37();
    f32 f31;
    f32 f30;
    if (s2->mFieldED != 0) {
        f32 v = (f32)(s8)s2->mField58;
        if (v < lbl_eu_80666734) {
            v = lbl_eu_80666734;
        }
        if (v > lbl_eu_80666738) {
            v = lbl_eu_80666738;
        }
        f31 = v / lbl_eu_80666738;
    } else {
        f31 = lbl_eu_80666730;
    }
    CtrlPcSub37Ext* s3 = (CtrlPcSub37Ext*)self->vf37();
    if (s3->mFieldED != 0) {
        f32 v = (f32)(s8)s3->mField59;
        if (v < lbl_eu_80666734) {
            v = lbl_eu_80666734;
        }
        if (v > lbl_eu_80666738) {
            v = lbl_eu_80666738;
        }
        f30 = v / lbl_eu_80666738;
    } else {
        f30 = lbl_eu_80666730;
    }

    if (probePlayerCtrl((cf::CtrlPc*)self, &v1c, 0x1c) == 0 &&
        probePlayerCtrl((cf::CtrlPc*)self, &v1d, 0x1d) == 0 &&
        probePlayerCtrl((cf::CtrlPc*)self, &v12, 0x12) == 0 &&
        probePlayerCtrl((cf::CtrlPc*)self, &v13, 0x13) == 0 &&
        probePlayerCtrl((cf::CtrlPc*)self, &v805, 0x805) == 0) {
        if (f31 != lbl_eu_80666730 || f30 != lbl_eu_80666730) {
            f32 a1 = Atan2FIdx__Q24nw4r4mathFff(f31, f30);
            self->mFieldC = a1 * lbl_eu_8066674C;
        }
        if (f31 != lbl_eu_80666730 || f30 != lbl_eu_80666730) {
            f32 a2 = Atan2FIdx__Q24nw4r4mathFff(f31, f30);
            self->mField8 = a2 * lbl_eu_8066674C;
        }
        if (f31 * f31 + f30 * f30 < lbl_eu_80666730) {
            Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273,
                                        lbl_eu_80526300);
        }
        f32 d2 = f31 * f31 + f30 * f30;
        f32 dist;
        if (d2 > lbl_eu_80666730) {
            dist = d2 * FrSqrt__Q24nw4r4mathFf(d2);
        } else {
            dist = lbl_eu_80666730;
        }
        if (dist > lbl_eu_80666768) {
            dist = lbl_eu_80666768;
        }
        self->mField14 = dist;

        if (func_80148778(&self->mField5C->mField8, 0xcd) == 0 &&
            func_80148778(&self->mField5C->mField8, 0x6) == 0 &&
            probePlayerCtrl((cf::CtrlPc*)self, &v4000b, 0x4000) == 0) {
            // keep the computed stick distance
        } else {
            self->mField14 = lbl_eu_80666730;
            if (self->mField5C->mSub3ED4->vf14(0x1000) == 0) {
                CtrlVoiceOwnerC4View* vo =
                    (CtrlVoiceOwnerC4View*)&self->mField5C->mSub3E9C;
                if (vo->mFieldC4 != NULL && (vo->mFieldC4->mField530 & 1)) {
                    // still has a pending sub-target
                } else {
                    self->mField5C->mSub3E9C.v47(self->mFieldC);
                }
            }
        }
    }

    if (probePlayerCtrl((cf::CtrlPc*)self, &v803b, 0x803) != 0) {
        if (probePlayerCtrl((cf::CtrlPc*)self, &v19, 0x19) == 0 &&
            probe1a == 0) {
            self->mField14 *= lbl_eu_8066676C;
        }
    }

    self->mField14 *= ((CtrlPlayerVf23C*)self->mField5C)->vf141();
    }

a89c:
    if (probePlayerCtrl((cf::CtrlPc*)self, &v3b, 3) == 0 &&
        probePlayerCtrl((cf::CtrlPc*)self, &v6b, 6) == 0 &&
        probePlayerCtrl((cf::CtrlPc*)self, &v9, 9) == 0 &&
        probePlayerCtrl((cf::CtrlPc*)self, &v806, 0x806) == 0) {
        // battle-target re-aim below
    } else if (self->mField5C->mSub3E9C.v01(4) != 0) {
        void* other = func_8016FE34(findObjectById((s32)self->mField5C->mSub3E9C.v17()));
        if (other != 0) {
            ml::CVec3 diff;
            CVoicePos* p1 = self->mField5C->mSub3E9C.getPosition();
            CVoicePos* p2 = ((CVoiceOwnerIntfPc*)&((CtrlPlayerSweepView*)other)->mOwner3E9C)->getPosition();
            diff.x = p2->f[0] - p1->f[0];
            diff.y = p2->f[1] - p1->f[1];
            diff.z = p2->f[2] - p1->f[2];
            ml::CVec3 d = diff;
            f32 ang = Atan2FIdx__Q24nw4r4mathFff(d.x, d.z);
            self->mField14 = lbl_eu_80666730;
            self->mFieldC = ang * lbl_eu_8066674C;
            self->mField5C->mSub3E9C.v47(self->mFieldC);
        }
        self->mField5C->mSub3E9C.v02(4);
    }

    if (func_802799F0(&((CBattleManagerViewPc*)getInstance__Q22cf14CBattleManagerFv())->mField1A8,
                      self->mField5C) != 0 &&
        probePlayerCtrl((cf::CtrlPc*)self, &v801b, 0x801) != 0) {
        CtrlPlayerSub298Vf4* sub298 =
            (CtrlPlayerSub298Vf4*)self->mField5C->vf164();
        void* t = func_8016FE34(findObjectById((s32)sub298->mField4));
        if (t != 0 && t != self->mField5C) {
            ml::CVec3 diff;
            CVoicePos* p1 = self->mField5C->mSub3E9C.getPosition();
            CVoicePos* p2 = ((CVoiceOwnerIntfPc*)&((CtrlPlayerSweepView*)t)->mOwner3E9C)->getPosition();
            diff.x = p2->f[0] - p1->f[0];
            diff.y = p2->f[1] - p1->f[1];
            diff.z = p2->f[2] - p1->f[2];
            ml::CVec3 d = diff;
            f32 ang = Atan2FIdx__Q24nw4r4mathFff(d.x, d.z);
            self->mFieldC = ang * lbl_eu_8066674C;
            self->mField5C->mSub3E9C.v47(self->mFieldC);
        }
    }
}

float CActorParam_UnkVirtualFunc106__Q22cf11CActorParamFv(void* self) { return *(float*)((u8*)self + 0x1840); }

// Target us-8009abb4. Pad-handler gate: when the global "demo/idle" state
// word clears, run a battery of player-state probes; if none of them flag a
// blocking condition, mirror the current menu-state bitmask into self->mField4
// bit 0 (set when the arts/menu mask is active, clear otherwise).
// Probe a control-data gate: refresh the player's current control word via the
// sub-object's vf30 and test it against `gate`. Returns the raw func_80174C98
// result (nonzero = gate set). Inlined so the player pointer stays in a saved
// register across the vf30 call, matching retail.
static int probePlayerCtrl(cf::CtrlPc* self, u32* out, int gate)
{
    CtrlPlayerObj* p = self->mField5C;
    *out = *p->mField4->vf30();
    return func_80174C98(p, out, gate);
}

// Target us-8009abb4. Pad-handler gate: when the global "demo/idle" state
// word clears, run a battery of player-state probes; if none of them flag a
// blocking condition, mirror the current menu-state bitmask into self->mField4
// bit 0 (set when the arts/menu mask is active, clear otherwise).
void func_8009A1DC(cf::CtrlPc* self)
{
    bool flag = true;
    cf::CfGameManager::getInstance();
    if (isGlobalCamFlagSet(0x4000000) != 0) {
        return;
    }
    if (func_80148778(&self->mField5C->mField8, 0xf) != 0 ||
        func_80148778(&self->mField5C->mField8, 0x6) != 0) {
        flag = false;
    }
    u32 v1;
    u32 v2;
    if (probePlayerCtrl(self, &v1, 1) != 0 ||
        probePlayerCtrl(self, &v2, 2) != 0) {
        flag = false;
    }
    if (self->mField5C->mSub3ED4->vf14(0x40000) != 0) {
        flag = false;
    }
    if (func_800FEDF8() != 0) {
        flag = false;
    }
    if (self->mField5C->mSub3ED4->vf14(0x400) != 0) {
        flag = false;
    }
    u32 v3;
    u32 v4;
    if (probePlayerCtrl(self, &v3, 3) == 0 &&
        probePlayerCtrl(self, &v4, 4) == 0) {
        flag = false;
    }
    if (func_800FE68C()->mField90E4 != 0) {
        flag = false;
    }
    if (flag) {
        u32 mask = isClassicController__Q22cf13CfGameManagerFv(-1)
                       ? lbl_eu_80527F10[2]
                       : lbl_eu_80527E98[2];
        u32 mf4 = self->vf37()->mField4;
        if ((mf4 & mask) != 0) {
            if (isClassicController__Q22cf13CfGameManagerFv(-1) != 0 &&
                (self->vf37()->mField0 & 0x2000000)) {
                return;
            }
            u32 mask2 = isClassicController__Q22cf13CfGameManagerFv(-1)
                            ? lbl_eu_80527F10[2]
                            : lbl_eu_80527E98[2];
            u32 mf4b = self->vf37()->mField4;
            if ((mf4b & mask2) != 0) {
                self->mField4 |= 1;
            } else {
                self->mField4 &= ~1;
            }
        }
    }
}

// Target us-8009ae84. Control-gate sweep: probe the player's control word
// (gates 1, 2, 0x803, 0x802). When the 0x803 gate is active, run the
// menu/battle-state mask sweep that derives the aim/voice flags (flagA /
// flagB) and pushes them into the game-manager sub-object via
// func_8006BC1C / func_8006BBF4; the final pass mirrors the 0x400 flag
// range state into the 0x3 range.
void func_8009A4AC(CtrlPcVf38* self)
{
    cf::CfGameManager::getInstance();
    UnkClass_800821F8View* gm8 = (UnkClass_800821F8View*)getCameraDataBlock__Q22cf13CfGameManagerFv();
    CtrlVoiceHandle* actionSrc =
        (CtrlVoiceHandle*)findObjectById(func_800FE68C()->mField90E4);

    u32 v1;
    u32 v2;
    u32 v3;
    u32 v4;
    if (probePlayerCtrl((cf::CtrlPc*)self, &v1, 1) != 0 ||
        probePlayerCtrl((cf::CtrlPc*)self, &v2, 2) != 0) {
        return;
    }
    int flagA = 0;
    int flagB = 0;

    u32 mask = isClassicController__Q22cf13CfGameManagerFv(-1)
                   ? lbl_eu_80527F10[16]
                   : lbl_eu_80527E98[16];
    CtrlRemoteSubA0* sub = (CtrlRemoteSubA0*)self->vf38();
    self->mField5C->mSub3ED4->vf10(0x400, (sub->mField0 & mask) != 0);

    if (probePlayerCtrl((cf::CtrlPc*)self, &v3, 0x803) != 0) {
        if (isClassicController__Q22cf13CfGameManagerFv(-1) != 0) {
            u32 mask2 = isClassicController__Q22cf13CfGameManagerFv(-1)
                            ? lbl_eu_80527F10[18]
                            : lbl_eu_80527E98[18];
            CtrlRemoteSubA0* s2 = (CtrlRemoteSubA0*)self->vf38();
            if ((s2->mField10 & mask2) != 0) {
                if ((gm8->mField4 & 0x8000) != 0) {
                    flagA = 1;
                    func_8006BC1C(gm8, 0x100);
                    func_8006BC1C(gm8, 0x8000);
                } else {
                    if (self->mField5C->mSub3ED4->vf14(0x2000) == 0) {
                        flagA = 1;
                        func_8006BC1C(gm8, 0x100);
                    } else {
                        flagB = 1;
                        func_8006BBF4(gm8, 0x100, 1);
                        self->mField5C->mSub3ED4->vf11(0x2000);
                    }
                }
            } else {
                u32 mask3 = isClassicController__Q22cf13CfGameManagerFv(-1)
                                ? lbl_eu_80527F10[18]
                                : lbl_eu_80527E98[18];
                CtrlRemoteSubA0* s3 = (CtrlRemoteSubA0*)self->vf38();
                if ((s3->mFieldC & mask3) != 0) {
                    if ((gm8->mField4 & 0x8000) != 0) {
                        flagA = 1;
                        func_8006BC1C(gm8, 0x100);
                        func_8006BC1C(gm8, 0x8000);
                    } else {
                        if (self->mField5C->mSub3ED4->vf14(0x2000) == 0) {
                            flagB = 1;
                            func_8006BBF4(gm8, 0x100, 1);
                            self->mField5C->mSub3ED4->vf11(0x2000);
                        } else {
                            flagA = 1;
                            func_8006BC1C(gm8, 0x100);
                        }
                    }
                } else {
                    u32 mask4 = isClassicController__Q22cf13CfGameManagerFv(-1)
                                    ? lbl_eu_80527F10[29]
                                    : lbl_eu_80527E98[29];
                    CtrlRemoteSubA0* s4 = (CtrlRemoteSubA0*)self->vf38();
                    if ((s4->mFieldC & mask4) != 0) {
                        func_8006BBF4(gm8, 0x100, 1);
                        u32 mask5 = isClassicController__Q22cf13CfGameManagerFv(-1)
                                        ? lbl_eu_80527F10[18]
                                        : lbl_eu_80527E98[18];
                        CtrlRemoteSubA0* s5 = (CtrlRemoteSubA0*)self->vf38();
                        if ((s5->mField0 & mask5) == 0) {
                            self->mField5C->mSub3ED4->vf11(0x2000);
                        }
                    } else {
                        u32 mask6 = isClassicController__Q22cf13CfGameManagerFv(-1)
                                        ? lbl_eu_80527F10[18]
                                        : lbl_eu_80527E98[18];
                        CtrlRemoteSubA0* s6 = (CtrlRemoteSubA0*)self->vf38();
                        if ((s6->mField0 & mask6) != 0 ||
                            ((isClassicController__Q22cf13CfGameManagerFv(-1)
                                  ? lbl_eu_80527F10[29]
                                  : lbl_eu_80527E98[29]) &
                             self->vf38()->mField0) != 0) {
                            // Either menu-state word is set: clear the
                            // 0x2000 flag range unless the 0x8000 menu bit
                            // is already active.
                            u32 mf4 = gm8->mField4;
                            if ((mf4 & 0x8000) == 0) {
                                if ((mf4 & 0x10) != 0) {
                                    self->mField5C->mSub3ED4->vf10(0x2000, 1);
                                }
                                flagB = 1;
                                func_8006BC1C(gm8, 0x100);
                            }
                        } else {
                            if ((gm8->mField4 & 0x10) != 0) {
                                flagA = 1;
                            } else {
                                flagB = 1;
                            }
                        }
                    }
                }
            }
        } else {
            u32 mask7 = isClassicController__Q22cf13CfGameManagerFv(-1)
                            ? lbl_eu_80527F10[18]
                            : lbl_eu_80527E98[18];
            CtrlRemoteSubA0* s7 = (CtrlRemoteSubA0*)self->vf38();
            if ((s7->mField10 & mask7) != 0) {
                if (self->mField5C->mSub3ED4->vf14(0x2000) == 0) {
                    if ((gm8->mField4 & 0x10) != 0) {
                        flagB = 1;
                    } else {
                        flagA = 1;
                    }
                } else {
                    self->mField5C->mSub3ED4->vf11(0x2000);
                    func_8006BBF4(gm8, 0x100, 1);
                }
                func_8006BC1C(gm8, 0x8000);
            } else {
                u32 mask8 = isClassicController__Q22cf13CfGameManagerFv(-1)
                                ? lbl_eu_80527F10[18]
                                : lbl_eu_80527E98[18];
                CtrlRemoteSubA0* s8 = (CtrlRemoteSubA0*)self->vf38();
                if ((s8->mFieldC & mask8) != 0) {
                    if ((gm8->mField4 & 0x8000) == 0) {
                        self->mField5C->mSub3ED4->vf11(0x2000);
                        func_8006BBF4(gm8, 0x100, 1);
                    }
                    func_8006BC1C(gm8, 0x8000);
                } else {
                    u32 mf4 = gm8->mField4;
                    if ((mf4 & 0x10) != 0) {
                        u32 mask9 = isClassicController__Q22cf13CfGameManagerFv(-1)
                                        ? lbl_eu_80527F10[18]
                                        : lbl_eu_80527E98[18];
                        CtrlRemoteSubA0* s9 = (CtrlRemoteSubA0*)self->vf38();
                        if ((s9->mField0 & mask9) != 0) {
                            if ((gm8->mField4 & 0x8000) == 0) {
                                flagB = 1;
                                self->mField5C->mSub3ED4->vf10(0x2000, 1);
                            } else {
                                u32 maskA =
                                    isClassicController__Q22cf13CfGameManagerFv(-1)
                                        ? lbl_eu_80527F10[28]
                                        : lbl_eu_80527E98[28];
                                CtrlRemoteSubA0* sA =
                                    (CtrlRemoteSubA0*)self->vf38();
                                if ((sA->mField0 & maskA) != 0) {
                                    flagB = 1;
                                    self->mField5C->mSub3ED4->vf10(0x2000, 1);
                                    func_8006BC1C(gm8, 0x8000);
                                }
                            }
                        } else {
                            flagA = 1;
                        }
                    } else {
                        if ((mf4 & 0x8000) == 0) {
                            flagB = 1;
                        } else {
                            flagA = 1;
                        }
                        u32 maskB = isClassicController__Q22cf13CfGameManagerFv(-1)
                                        ? lbl_eu_80527F10[18]
                                        : lbl_eu_80527E98[18];
                        CtrlRemoteSubA0* sB = (CtrlRemoteSubA0*)self->vf38();
                        if ((sB->mField0 & maskB) != 0) {
                            u32 mf4b = gm8->mField4;
                            if ((mf4b & 0x8000) == 0) {
                                flagB = 1;
                                if ((mf4b & 0x100) != 0) {
                                    func_8006BC1C(gm8, 0x100);
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        // 0x803 gate clear: voice-owner state probe.
        if (self->mField5C->mSub3E9C.v01(0x100) == 0) {
            flagB = 1;
        }
        if ((gm8->mField4 & 0x100) != 0) {
            func_8006BC1C(gm8, 0x100);
        }
    }

    // Merge point: push the derived flags into the game-manager sub-object.
    if (actionSrc == 0) {
        flagB = 1;
    }
    if (flagB != 0 && (gm8->mField4 & 0x10) != 0) {
        func_8006BC1C(gm8, 0x10);
    }
    if (flagA != 0) {
        if ((gm8->mField4 & 0x100) != 0) {
            func_8006BC1C(gm8, 0x100);
        }
        if ((gm8->mField4 & 0x10) == 0 && actionSrc != 0) {
            func_8006BBF4(gm8, 0x10, 1);
        }
    }
    if ((gm8->mField4 & 0x10) != 0 && actionSrc != 0) {
        gm8->vf09(actionSrc->vf41());
    }

    // 0x802 gate: clear the 0x20 flag range when the mask-19 word is set
    // and neither 0x200 nor 0x20 is already active.
    if (probePlayerCtrl((cf::CtrlPc*)self, &v4, 0x802) != 0) {
        u32 maskC = isClassicController__Q22cf13CfGameManagerFv(-1)
                        ? lbl_eu_80527F10[19]
                        : lbl_eu_80527E98[19];
        CtrlRemoteSubA0* sC = (CtrlRemoteSubA0*)self->vf38();
        if ((sC->mField10 & maskC) != 0 && (gm8->mField4 & 0x220) == 0) {
            func_8006BBF4(gm8, 0x20, 1);
        }
    }

    // Mirror the 0x400 flag range state into the 0x3 range.
    func_8006BBF4(gm8, 0x3, self->mField5C->mSub3ED4->vf14(0x400) == 0);
}

// Target us-8009b858. Menu/battle-state sweep: probe the player control
// gates, then combine the vf37 menu-state words with the action-source
// presence to drive the 0x3ED4 flag ranges (0x400 / 0x4000), the +0x2C aim
// mirror bits, and the camera shake requests.
void func_8009AE80(CtrlPcVf37State* self)
{
    cf::CfGameManager::getInstance();
    getCameraDataBlock__Q22cf13CfGameManagerFv();
    CtrlVoiceHandle* actionSrc =
        (CtrlVoiceHandle*)findObjectById(
            (s32)((CfObjAe80*)func_800FE68C())->mField90E4);
    cf::CfGameManager::getInstance();
    if (isGlobalCamFlagSet(0x4000000) != 0) {
        return;
    }

    // Player pointer is re-read from self->mField5C at each block, matching
    // retail's reload pattern.
    u32 v1;
    u32 v2;
    if (probePlayerCtrl((cf::CtrlPc*)self, &v1, 1) != 0 ||
        probePlayerCtrl((cf::CtrlPc*)self, &v2, 2) != 0) {
        return;
    }
    if (func_800FEDF8() != 0) {
        return;
    }
    if (self->mField5C->mSub3ED4->vf14(0x400) != 0) {
        return;
    }

    int flag31 = 0;
    int flag30 = 0;

    // The pad-mask ternary is written inline at each use site: retail keeps
    // the selected mask in a callee-saved register and leaves the freshly
    // loaded menu-state word in r0, which only falls out of this expression
    // shape.
    #define PAD_MASK(idx) \
        (isClassicController__Q22cf13CfGameManagerFv(-1) ? lbl_eu_80527F10[(idx)] \
                                                   : lbl_eu_80527E98[(idx)])
    if (actionSrc == NULL) {
        // No action source: gate the 0x4000 range purely on the vf37 words.
        if ((self->vf37()->mField0 & PAD_MASK(7)) != 0) {
            int keep = 1;
            if ((self->vf37()->mField8 & PAD_MASK(8)) == 0) {
                if ((self->vf37()->mField8 & PAD_MASK(9)) == 0) {
                    keep = 0;
                }
            }
            if (isClassicController__Q22cf13CfGameManagerFv(-1) == 0 && keep == 0) {
                keep = 1;
                if ((self->vf37()->mField14 & PAD_MASK(7)) == 0) {
                    keep = 0;
                }
            }
            if (keep != 0 && self->mField5C->mSub3ED4->vf14(0x4000) == 0) {
                flag31 = 1;
            }
        } else {
            if ((self->vf37()->mField10 & PAD_MASK(7)) != 0) {
                flag31 = 1;
            } else {
                self->mField5C->mSub3ED4->vf11(0x4000);
            }
        }
    } else {
        // Action source present: the 0x10 menu word arms both flags.
        if ((self->vf37()->mField10 & PAD_MASK(7)) != 0) {
            if (isClassicController__Q22cf13CfGameManagerFv(-1) == 0) {
                flag30 = 1;
                flag31 = 1;
            } else if ((self->vf37()->mField0 & 0x10000000) != 0) {
                flag30 = 1;
                flag31 = 1;
            }
        }
        if ((self->vf37()->mField8 & PAD_MASK(5)) == 0) {
            if (self->mField5C->mSub3ED4->vf14(0x4000) == 0) {
                // Gate-3 probe plus the 0x04 menu word sets flag30.
                if ((self->vf37()->mField4 & PAD_MASK(1)) != 0) {
                    u32 v3;
                    if (probePlayerCtrl((cf::CtrlPc*)self, &v3, 3) != 0 &&
                        func_80148778(&self->mField5C->mField8, 0x11) == 0) {
                        if ((self->vf37()->mField0 & PAD_MASK(16)) == 0) {
                            flag30 = 1;
                        }
                    }
                }
            } else {
                // State probe 0x11 either sets flag30 or clears the 0x4000
                // range.
                if (func_80148778(&self->mField5C->mField8, 0x11) == 0) {
                    u32 v3;
                    if (probePlayerCtrl((cf::CtrlPc*)self, &v3, 3) != 0) {
                        flag30 = 1;
                    } else {
                        self->mField5C->mSub3ED4->vf11(0x4000);
                    }
                }
            }
        }
    }

    if (flag30 != 0) {
        // Arm the 0x4000 range and mirror the 0x400 bit into +0x2C unless the
        // voice-owner chain already holds the 0x100 flag.
        if (self->mField5C->mSub3E9C.v01(0x100) == 0) {
            self->mField5C->mSub3ED4->vf10(0x4000, 1);
            self->mField2C |= 0x400;
            if ((self->vf37()->mField0 & PAD_MASK(7)) == 0) {
                self->mField5C->mSub3ED4->vf11(0x4000);
            }
        }
    }
    if (flag31 != 0) {
        // Clear the 0x4000 range, swap +0x2C bit 0x400 for 0x200, and request
        // the camera shake matching the 0x803 gate state.
        self->mField5C->mSub3ED4->vf11(0x4000);
        self->mField2C = (self->mField2C & ~0x400) | 0x200;
        u32 v803;
        if (probePlayerCtrl((cf::CtrlPc*)self, &v803, 0x803) == 0) {
            func_800FE950(func_800FE68C(), 0x80000004, 0x4802, 0);
        } else {
            func_800FE950(func_800FE68C(), 0x80000003, 0x4802, 0);
        }
    }

    if ((((CfObjAe80*)func_800FE68C())->mFieldC180 & 1) != 0) {
        return;
    }
    if ((((CfObjAe80*)func_800FE68C())->mFieldC180 & 2) == 0) {
        return;
    }
    if ((self->vf37()->mField0 & PAD_MASK(7)) == 0) {
        return;
    }
    u32 v805;
    if (probePlayerCtrl((cf::CtrlPc*)self, &v805, 0x805) != 0 ||
        func_80148778(&self->mField5C->mField8, 0xf) != 0 ||
        func_80148778(&self->mField5C->mField8, 0x11) != 0) {
        func_800FE860(func_800FE68C(), 1);
    }
    if ((((CfObjAe80*)func_800FE68C())->mFieldC180 & 2) == 0) {
        return;
    }
    if (func_800FE910(func_800FE68C()) != 0) {
        return;
    }
    if (isClassicController__Q22cf13CfGameManagerFv(-1) != 0 &&
        (self->vf37()->mField0 & 0x10000000) != 0) {
        return;
    }
    // Mirror the 0x08 menu-word bits into +0x2C (mask idx 9 -> 0x80000,
    // mask idx 8 -> 0x100000).
    if ((self->vf37()->mField8 & PAD_MASK(9)) != 0) {
        self->mField2C |= 0x80000;
        return;
    }
    if ((self->vf37()->mField8 & PAD_MASK(8)) != 0) {
        self->mField2C |= 0x100000;
    }
}
#undef PAD_MASK

// Target us-8009c160. Remote-control command dispatch: after the 0x400
// global gate and the basic arts gates pass, branch on the 0x800 sub-object
// flag and the embedded voice-owner 0x100/0x200 flags to either trigger the
// camera enum-list sweep (0x20 mask), request voice-owner state changes, or
// mirror the menu-word bits into self->mField4 (0x4 camera / 0x8 sudden-commu).
void func_8009B788(CtrlPcVf38* self)
{
    cf::CfGameManager::getInstance();
    getCameraDataBlock__Q22cf13CfGameManagerFv();
    void* handle = findObjectById(func_800FE68C()->mField90E4);
    cf::CfGameManager::getInstance();
    if (isGlobalCamFlagSet(0x4000000)) {
        return;
    }

    u32 v1;
    u32 v2;
    if (probePlayerCtrl((cf::CtrlPc*)self, &v1, 1) != 0 ||
        probePlayerCtrl((cf::CtrlPc*)self, &v2, 2) != 0) {
        return;
    }

    if (func_800FEDF8() != 0) {
        return;
    }
    if (self->mField5C->mSub3ED4->vf14(0x800) != 0) {
        if (self->mField5C->mSub3E9C.v01(0x200) != 0) {
            self->mField4 |= 0x8;
        }
        return;
    }

    if (((CtrlVoiceOwnerC4View*)&self->mField5C->mSub3E9C)->mFieldC4 == NULL) {
        return;
    }

    if (self->mField5C->mSub3E9C.v01(0x100) != 0) {
        u32 v803;
        u32 v1f;
        if (probePlayerCtrl((cf::CtrlPc*)self, &v803, 0x803) == 0 ||
            probePlayerCtrl((cf::CtrlPc*)self, &v1f, 0x1f) != 0) {
            // Camera-sweep path: only when the voice/battle target exists,
            // its cutscene bit is clear, and the handle resolves.
            CtrlPlayerSub3F60* sub3f60 = self->mField5C->mField3F60;
            if (sub3f60 != NULL && (sub3f60->mField4EC & 0x2) == 0) {
                if (func_8016FE34(handle) != 0) {
                    CfEnumListHolder holder;
                    func_80043D90(&holder);
                    func_800F4A98(func_80043F18(&holder), 0x20, 1);
                    self->mField4 |= 0x4;
                    __dt__80043E88(&holder, -1);
                }
                return;
            }
        }
        self->mField5C->mSub3E9C.v02(0x100);
        return;
    }

    u32 v3;
    u32 ve;
    u32 v8;
    u32 v1fb;
    if (probePlayerCtrl((cf::CtrlPc*)self, &v3, 3) != 0 ||
        probePlayerCtrl((cf::CtrlPc*)self, &ve, 0xe) != 0 ||
        probePlayerCtrl((cf::CtrlPc*)self, &v8, 8) != 0 ||
        probePlayerCtrl((cf::CtrlPc*)self, &v1fb, 0x1f) != 0) {
        if (self->mField5C->mSub3ED4->vf14(0x400) != 0) {
            return;
        }
        CtrlPlayerSub3F60* sub3f60 = self->mField5C->mField3F60;
        if (sub3f60 == NULL) {
            return;
        }
        if (sub3f60->mField4EC & 0x2) {
            return;
        }
        if (self->mField5C->mSub3ED4->vf14(0x4000) != 0) {
            return;
        }
        ArtsSelStateViewPc* sel = CMenuArtsSelect_getSelectState();
        if (sel == NULL) {
            return;
        }
        if (sel->byte0) {
            return;
        }
        if (sel->byte1) {
            return;
        }
        if (self->mField5C->mSub3ED4->vf14(0x40000) != 0) {
            return;
        }
        if (func_8016FE34(handle) == 0) {
            return;
        }
        self->mField5C->mSub3E9C.v00(0x100);
        return;
    }

    if (self->mField5C->mSub3E9C.v01(0x200) != 0) {
        self->mField4 |= 0x8;
        return;
    }
    u32 v6;
    u32 v9;
    u32 v12;
    u32 v13;
    u32 v14;
    if (probePlayerCtrl((cf::CtrlPc*)self, &v6, 6) != 0 ||
        probePlayerCtrl((cf::CtrlPc*)self, &v9, 9) != 0 ||
        probePlayerCtrl((cf::CtrlPc*)self, &v12, 0x12) != 0 ||
        probePlayerCtrl((cf::CtrlPc*)self, &v13, 0x13) != 0 ||
        probePlayerCtrl((cf::CtrlPc*)self, &v14, 0x14) != 0) {
        ArtsSelStateViewPc* sel = CMenuArtsSelect_getSelectState();
        if (sel == NULL) {
            return;
        }
        if (sel->byte0) {
            return;
        }
        if (sel->byte1) {
            return;
        }
        self->mField4 |= 0x8;
        func_802A2CF0();
    }
}

void func_8009BD14(CtrlPcVf38* self)
{
    // Declared at function scope: MWCC creates the pseudo early, which pins
    // the counter's register ahead of the list-walk temps (retail r4).
    s32 count;
    void* handle = findObjectById(func_800FE68C()->mField90E4);
    if (handle == 0) {
        return;
    }
    CtrlPlayerObj* player = self->mField5C;
    u32 v = *player->mField4->vf30();
    if (func_80174C98(player, &v, 0x803) == 0) {
        return;
    }
    u32 bs = ((CBattleManagerViewPc*)getInstance__Q22cf14CBattleManagerFv())
        ->mField1AA;
    // Second comparison commuted: two same-operand unsigned compares with
    // the same branch target get fused by MWCC into a subi/cmpli range
    // check; commuting keeps retail's two cmplwi branches.
    int inBattle = bs >= 1u && 0x18u >= bs;
    if (inBattle != 0) {
        return;
    }
    if (((CBattleManagerViewPc*)getInstance__Q22cf14CBattleManagerFv())
            ->mField20C8 != 0) {
        return;
    }
    cf::CfGameManager::getInstance();
    if (isGlobalCamFlagSet(0x4000000) != 0) {
        return;
    }
    u32 mask = isClassicController__Q22cf13CfGameManagerFv(-1)
                   ? lbl_eu_80527F10[24]
                   : lbl_eu_80527E98[24];
    u32 mf0 = self->vf38()->mField0;
    if ((mf0 & mask) != 0) {
        // Count the battle-manager's circular actor list (+0x28 head); a solo
        // (or empty) party cannot trigger the menu-state sweeps below.
        // Head addressed via bm in both init and condition so the manager
        // stays in r3 across the head load (retail regalloc).
        CBattleManagerViewPc* bm =
            (CBattleManagerViewPc*)getInstance__Q22cf14CBattleManagerFv();
        count = 0;
        CtrlSweepNode* node =
            reinterpret_cast<CtrlBmSweepView*>(bm)->mField28->mNext;
        while (reinterpret_cast<CtrlBmSweepView*>(bm)->mField28 != node) {
            node = node->mNext;
            count++;
        }
        if (count <= 1) {
            return;
        }
        if (self->mField5C->mSub3ED4->vf14(0x400) != 0) {
            return;
        }
        if (func_801B0F8C() != 0) {
            return;
        }
        if (func_8017FD44() != 0) {
            return;
        }
        u32 mask1 = isClassicController__Q22cf13CfGameManagerFv(-1)
                        ? lbl_eu_80527F10[26]
                        : lbl_eu_80527E98[26];
        u32 mf1 = self->vf38()->mField4;
        if ((mf1 & mask1) != 0) {
            // Menu-state block 1: clear every actor accessor's combo word and the
            // 0x10 state bit, noting whether any accessor already had either set.
            CfEnumListHolder holder1;
            bool flag = false;
            func_80043D90(&holder1);
            CtrlEnumListSweep* list =
                reinterpret_cast<CtrlEnumListSweep*>(func_80043F18(&holder1));
            list->count = 0;
            list->mField3030 = 0;
            func_800F4A98(func_80043F18(&holder1), 0x20, 0);
            for (s32 i = 0;
                 i < reinterpret_cast<CtrlEnumListSweep*>(
                         func_80043F18(&holder1))
                         ->count;
                 i++) {
                CtrlAccSweepView* acc = reinterpret_cast<CtrlAccSweepView*>(
                    func_8016FE34(func_800F6EAC((CfMoveEnumList*)func_80043F18(&holder1), i)));
                if (acc->mField3E98 != 0 || (acc->mField3388 & 0x10) != 0) {
                    flag = true;
                }
                acc->mField3E98 = 0;
                acc->mField3388 &= ~0x10;
            }
            if (flag) {
                func_802A2BB0();
            }
            __dt__80043E88(&holder1, -1);
        } else {
            u32 mask2 = isClassicController__Q22cf13CfGameManagerFv(-1)
                            ? lbl_eu_80527F10[25]
                            : lbl_eu_80527E98[25];
            u32 mf2 = self->vf38()->mField4;
            if ((mf2 & mask2) != 0) {
                // Menu-state block 2: restore each accessor's combo word to the
                // voice handle's id, flagging accessors that were out of sync.
                CfEnumListHolder holder2;
                bool flag2 = false;
                func_80043D90(&holder2);
                CtrlEnumListSweep* list2 =
                    reinterpret_cast<CtrlEnumListSweep*>(func_80043F18(&holder2));
                list2->count = 0;
                list2->mField3030 = 0;
                func_800F4A98(func_80043F18(&holder2), 0x20, 0);
                for (s32 i = 0;
                     i < reinterpret_cast<CtrlEnumListSweep*>(
                             func_80043F18(&holder2))
                             ->count;
                     i++) {
                    CtrlAccSweepView* acc = reinterpret_cast<CtrlAccSweepView*>(
                        func_8016FE34(func_800F6EAC((CfMoveEnumList*)func_80043F18(&holder2), i)));
                    // Retail reloads the voice handle id from the handle each
                    // iteration (lwz r4, 0x74(r31) inside the loop body).
                    u32 hv = reinterpret_cast<CtrlVoiceSweepView*>(handle)->mField74;
                    if (acc->mField3E98 != hv || (acc->mField3388 & 0x10) != 0) {
                        flag2 = true;
                    }
                    acc->mField3E98 = hv;
                    acc->mField3388 &= ~0x10;
                }
                if (flag2) {
                    func_802A2B44();
                    // Conditional addi on the same register (null stays null),
                    // matching retail's cmpwi/beq/addi/mr sequence.
                    u8* arg = reinterpret_cast<u8*>(func_8016FE34(handle));
                    if (arg != NULL) {
                        arg = reinterpret_cast<CtrlAccSweepView*>(arg)->mOwner3E9C;
                    }
                    func_800451D8(0xBC, arg);
                }
                __dt__80043E88(&holder2, -1);
            } else {
                u32 mask3 = isClassicController__Q22cf13CfGameManagerFv(-1)
                                ? lbl_eu_80527F10[27]
                                : lbl_eu_80527E98[27];
                u32 mf3 = self->vf38()->mField4;
                if ((mf3 & mask3) != 0) {
                    // Menu-state block 3: zero every accessor's combo word and set
                    // the 0x10 state bit, then re-arm the voice chain.
                    CfEnumListHolder holder3;
                    bool flag3 = false;
                    func_80043D90(&holder3);
                    CtrlEnumListSweep* list3 =
                        reinterpret_cast<CtrlEnumListSweep*>(func_80043F18(&holder3));
                    list3->count = 0;
                    list3->mField3030 = 0;
                    func_800F4A98(func_80043F18(&holder3), 0x20, 0);
                    for (s32 i = 0;
                         i < reinterpret_cast<CtrlEnumListSweep*>(
                                 func_80043F18(&holder3))
                                 ->count;
                         i++) {
                        CtrlAccSweepView* acc = reinterpret_cast<CtrlAccSweepView*>(
                            func_8016FE34(func_800F6EAC((CfMoveEnumList*)func_80043F18(&holder3), i)));
                        acc->mField3E98 = 0;
                        flag3 = true;
                        acc->mField3388 |= 0x10;
                    }
                    if (flag3) {
                        func_802A2C1C();
                        u8* arg = reinterpret_cast<u8*>(self->mField5C);
                        if (arg != NULL) {
                            arg = reinterpret_cast<CtrlPlayerSweepView*>(arg)
                                      ->mOwner3E9C;
                        }
                        func_800451D8(0xBC, arg);
                    }
                    __dt__80043E88(&holder3, -1);
                }
            }
        }
    }
}

// Target us-8009cb94. Remote-control event pump: run the party-menu target
// search over the player's sub3ED4 state, bail on sudden-commu activity and
// on the menu-state word gate, apply the arts-select window-state actions,
// then (once the 0xA state gate passes) dispatch one of six voice/battle
// commands built from the player's +0x3F10 word; case 4 arms the chain
// timer instead.
void func_8009C1BC(CtrlPcVf38State* self)
{
    // Declaration order drives MWCC's saved-GPR assignment
    // (fd=r26, sub=r27, bmFlag=r28, mode=r29 in retail).
    Fd44State* fd;
    CtrlSub3ED4Remote* sub;
    u32 bmFlag;
    int mode;
    u32 mask;
    CtrlRemoteEaView* ea;
    s32 page;
    u32 gateVal;
    CtrlRemoteSub374View* obj374;
    CtrlRemoteCmd20 prm;

    sub = reinterpret_cast<CtrlSub3ED4Remote*>(self->mField5C->mSub3ED4);
    mode = func_80190940(&sub->mResult368, sub->mActor18, 0, 0);
    bmFlag = sub->mField370;

    if (func_801BA2C8((u8*)getInstance__Q22cf14CBattleManagerFv() + 0x216c) !=
        0) {
        return;
    }

    mask = isClassicController__Q22cf13CfGameManagerFv(-1)
                   ? lbl_eu_80527F10[22]
                   : lbl_eu_80527E98[22];
    u32 menuState = self->vf38()->mField4;
    if ((menuState & mask) == 0) {
        return;
    }

    fd = func_8017FD44();
    if (fd != NULL) {
        // Arts-select window-state dispatch: window value 1 drives the
        // select-state machine (slot 0xF4 closed / slots 0xF0 with the new
        // page plus a UI refresh pair); value 5 flags the enum-list object
        // once the select state sits in pages 1/2.
        u32 sel = fd->mFieldB8;
        if (sel == 1) {
            switch (func_8017FD4C(fd)) {
            case 0:
                sub->vf59();
                return;
            case 1:
                sub->vf58(0);
                func_80280ADC();
                func_8017FEF0(fd, 0);
                return;
            case 2:
                sub->vf58(1);
                func_80280ADC();
                func_8017FEF0(fd, 1);
                return;
            default:
                return;
            }
        }
        if (sel == 5) {
            ea =
                (CtrlRemoteEaView*)func_800EA444(getInstance__Q22cf14CBattleManagerFv());
            if (ea != NULL) {
                page = func_8017FD4C(fd);
                if ((u32)(page - 1) <= 1) {
                    ea->mField824 |= 0x00080000;
                }
            }
        }
    }

    gateVal = *self->mField5C->mField4->vf30();
    if (func_80174C98(self->mField5C, &gateVal, 0xA) != 0) {
        return;
    }

    obj374 = sub->mField374;
    memset(prm._04, 0, 0xE);
    memset(&prm, 0, sizeof(prm));

    switch (mode) {
    case 1:
        prm.w00 = obj374->mField3F10;
        prm.b0D = 0x35;
        prm.b0E = 100;
        prm.h12 = 0;
        prm.f14 = lbl_eu_80666730;
        prm.h10 = 0;
        prm.b06 = 0x25;
        if (self->mField5C->mField3F60 != NULL) {
            func_8004C5EC(self->mField5C->mField3F60);
            func_800BE12C(&self->mField5C->mSub3E9C, 1, 0, -1, 1);
        }
        break;
    case 2:
        prm.w00 = obj374->mField3F10;
        prm.b0D = 0x36;
        prm.b0E = 100;
        prm.h12 = 0;
        prm.f14 = lbl_eu_80666730;
        prm.h10 = 0;
        prm.b06 = 0x25;
        if (self->mField5C->mField3F60 != NULL) {
            func_8004C5EC(self->mField5C->mField3F60);
            func_800BE12C(&self->mField5C->mSub3E9C, 1, 0, -1, 1);
        }
        break;
    case 3:
        prm.w00 = obj374->mField3F10;
        prm.b0D = 0x37;
        prm.b0E = 100;
        prm.h12 = 0;
        prm.f14 = lbl_eu_80666730;
        prm.h10 = 0;
        prm.b06 = 0x25;
        if (self->mField5C->mField3F60 != NULL) {
            func_8004C5EC(self->mField5C->mField3F60);
            func_800BE12C(&self->mField5C->mSub3E9C, 1, 0, -1, 1);
        }
        break;
    case 7:
        prm.w00 = obj374->mField3F10;
        prm.b0D = 0x3B;
        prm.b0E = 100;
        prm.h12 = 0;
        prm.f14 = lbl_eu_80666730;
        prm.h10 = 0;
        prm.b06 = 0x25;
        if (self->mField5C->mField3F60 != NULL) {
            func_8004C5EC(self->mField5C->mField3F60);
            func_800BE12C(&self->mField5C->mSub3E9C, 1, 0, -1, 1);
        }
        break;
    case 8:
        prm.w00 = obj374->mField3F10;
        prm.b0D = 0x3C;
        prm.b0E = 100;
        prm.h12 = 0;
        prm.f14 = lbl_eu_80666730;
        prm.h10 = 0;
        prm.b06 = 0x25;
        if (self->mField5C->mField3F60 != NULL) {
            func_8004C5EC(self->mField5C->mField3F60);
            func_800BE12C(&self->mField5C->mSub3E9C, 1, 0, -1, 1);
        }
        break;
    case 4:
        if (bmFlag != 1) {
            return;
        }
        {
            u8* gaugeBase =
                (u8*)getInstance__Q22cf14CBattleManagerFv() + 0x219c;
            u32 chainId = *(u32*)func_800EA444(
                getInstance__Q22cf14CBattleManagerFv());
            cf::CChainState* chain =
                (cf::CChainState*)((u8*)getInstance__Q22cf14CBattleManagerFv() +
                                   0x20c8);
            if (func_8027DE44(chain,
                              (cf::CChainBattleObj*)self->mField5C,
                              (cf::CChainBattleObj*)obj374,
                              chainId) != 0) {
                func_802A201C(self->mField5C, obj374);
                gaugeBase[0x261a4] = 1;
                func_8018C820(
                    (u8*)getInstance__Q22cf14CBattleManagerFv() + 0x194,
                    -100);
            }
        }
        break;
    default:
        return;
    }

    // Common tail: re-arm the 0x800 flag range, install the command block
    // when a voice id was selected, then notify the voice owner.
    ((CtrlPlayerSub3ED4*)self->mField5C->mSub3ED4)->vf10(0x800, 1);
    if (prm.b0D != 0) {
        func_8014AC38(&self->mField5C->mField3380, &prm);
    }
    self->mField5C->mSub3E9C.v00(0x800);
    func_801B0E88();
}

void func_8009C6B4() {}

void func_8009C6B8() {}

// Target us-8009d094. Menu/voice state gate: when the global demo flag and
// the 0x04000000 game-manager flag are clear, mirror three probe results into
// the high bits of self->mField2C: voice-owner busy (bit 0x20000000), and two
// menu-state words read through vf37() gated by the 0x40000 actor-flag range
// (bits 0xA0000000 / 0x40000000).
void func_8009C6BC(CtrlPcVf38State* self)
{
    cf::CfGameManager::getInstance();
    if (isGlobalCamFlagSet(0x4000000) != 0) {
        return;
    }
    if (func_800FEDF8() != 0) {
        return;
    }
    u32 mask = isClassicController__Q22cf13CfGameManagerFv(-1)
                   ? lbl_eu_80527F10[16]
                   : lbl_eu_80527E98[16];
    CtrlRemoteSubA0* sub = (CtrlRemoteSubA0*)self->vf38();
    if ((sub->mField0 & mask) != 0) {
        return;
    }
    if (self->mField5C->mSub3E9C.v01(1) != 0) {
        self->mField2C |= 0x200000;
        return;
    }
    if (self->mField5C->mSub3ED4->vf14(0x40000) == 0) {
        return;
    }
    u32 mask2 = isClassicController__Q22cf13CfGameManagerFv(-1)
                    ? lbl_eu_80527F10[4]
                    : lbl_eu_80527E98[4];
    u32 mf4 = self->vf37()->mField4;
    if ((mf4 & mask2) != 0) {
        self->mField2C |= 0xA00000;
        return;
    }
    u32 mask3 = isClassicController__Q22cf13CfGameManagerFv(-1)
                    ? lbl_eu_80527F10[23]
                    : lbl_eu_80527E98[23];
    u32 mf4b = self->vf37()->mField4;
    if ((mf4b & mask3) != 0) {
        self->mField2C |= 0x400000;
    }
}

// Tail-call wrapper: report whether the [0x800,0x1000) flag range is set on
// the player actor's +0x3ED4 flag object (virtual slot 0x40).
u32 func_8009C860(cf::CtrlRemote* self)
{
    cf::CtrlPc* pc = reinterpret_cast<cf::CtrlPc*>(self);
    return pc->mField5C->mSub3ED4->vf14(0x800);
}

// Returns 1 when the 0x800 flag range is set, else 1 when the 0x1000 range is
// clear (inverse of the second query).
u32 func_8009C87C(cf::CtrlRemote* self)
{
    cf::CtrlPc* pc = reinterpret_cast<cf::CtrlPc*>(self);
    if (pc->mField5C->mSub3ED4->vf14(0x800) != 0) {
        return 1;
    }
    return !pc->mField5C->mSub3ED4->vf14(0x1000);
}

// Allocate a*b bytes (4-aligned) from the default heap, zero the first b
// bytes, and fall back to the MEM2 heap if the first allocation fails.
// `self` is unused - the retail function still takes it in r3.
void* func_8009C8F4(cf::CtrlRemote* self, u32 a, u32 b)
{
    u32 total = a * b;
    void* p = mtl::MemManager::allocate_head(func_80061FE8(), total, 4);
    if (p != NULL && b != 0) {
        memset(p, 0, b);
    }
    if (p == NULL) {
        p = mtl::MemManager::allocate_head(mtl::MemManager::getHandleMEM2(), total, 4);
    }
    return p;
}

// Free a heap buffer when non-null. The trailing `self` guard is a no-op
// (the function returns anyway) but keeps `this` live across the deallocate
// call, reproducing retail's r31 frame (mr r31, r3 / lwz r31).
void func_8009C980(cf::CtrlRemote* self, u8* ptr)
{
    if (ptr != NULL) {
        mtl::MemManager::deallocate(ptr);
    }
    if (self != NULL) {
        return;
    }
}

// Type-1 control-data parser: LZ-style decoder over the payload bytes.
// Flag bits are consumed LSB-first, one byte at a time (refilled every 8
// iterations): bit=1 copies one literal byte, bit=0 reads a two-byte LZSS
// token - 12-bit back-distance ((b1 & 0xF0) << 8 | b0), 0 terminates the
// stream - and a run length of (b1 & 0xF) + 3 bytes copied from out-dist.
// Returns the total number of bytes written.
u32 func_8009C9B8(cf::CtrlRemote* self, CtrlRemoteBuf* buf, u32 arg)
{
    u8* out = (u8*)self;
    u8* in = buf->mData;
    u32 flags = 0;
    u32 total = 0;
    s32 bits = 1;
    while (true) {
        bits--;
        flags >>= 1;
        if (bits <= 0) {
            flags = *in++;
            bits = 8;
        }
        if ((flags & 1) != 0) {
            *out++ = *in++;
            total++;
        } else {
            // LZSS-style token: 12-bit back-distance (low byte | high nibble
            // of second byte << 8), 0 terminates; run length is the second
            // byte's low nibble + 3.
            u32 dist = in[0] | ((in[1] & 0xF0) << 4);
            if (dist == 0) {
                break;
            }
            u8* src = out - dist;
            u32 count = (in[1] & 0xF) + 3;
            total += count;
            in += 2;
            for (; count != 0; count--) {
                *out++ = *src++;
            }
        }
    }
    return total;
}

// Type-2 control-data parser: build a 0x38-byte UnkClass_80460C34Ctx around
// the payload, initialize it via the retail stream ctor, then validate the
// stream state (v==0/1 accepted) before returning the payload size.
u32 func_8009CAAC(cf::CtrlRemote* self, CtrlRemoteBuf* buf, u32 arg)
{
    UnkClass_80460C34Ctx ctx;
    u32 size = buf->mSize;
    ctx.mSize = size;
    ctx.mArg = arg;
    ctx.mSelf = self;
    ctx.mAlloc = (void*)func_8009C8F4;
    ctx.mFree = (void*)func_8009C980;
    ctx.mZero = 0;
    ctx.mPayload = (u8*)buf + 8;
    if (isStateReady__17UnkClass_80460C34Fv(&ctx, lbl_eu_804FBC20, 0x38) != 0) {
        return 0;
    }
    s32 v = func_80460F58__17UnkClass_80460C34Fv(&ctx, 4);
    if (v != 1) {
        cleanupState__17UnkClass_80460C34Fv(&ctx);
        if (v != 0) {
            return 0;
        }
    }
    if (cleanupState__17UnkClass_80460C34Fv(&ctx) != 0) {
        return 0;
    }
    return size;
}

// Type-3 control-data parser: dictionary decoder over the payload.
//
// The payload is a sequence of blocks. Each block starts with a table-
// description section: bytes >0x7f accumulate extra index (idx += b - 0x7f),
// then (b + 1) pairs of bytes fill tblMain/tblSub side by side starting at
// idx (even-position bytes go to tblMain, odd-position bytes to tblSub);
// filling stops early for a position when its tblMain byte equals the
// position itself, or when idx reaches 0x100. The rest of the block is a
// 16-bit big-endian count of codes; each code byte is either a literal
// (byte == tblMain[byte]) written to the output, or expanded by pushing
// tblSub[byte] and tblMain[byte] onto a small LIFO queue that is drained
// before further input is consumed.
// Returns the number of output bytes produced.
u32 func_8009CB80(cf::CtrlRemote* self, CtrlRemoteBuf* buf, u32 arg)
{
    // Declared so MWCC's stack layout matches retail:
    // initTbl@0x228, tblMain@0x128, tblSub@0x28, queue@0x08.
    u8 initTbl[0x100]; // scratch identity table, copied over tblMain per block
    u8 tblMain[0x100]; // primary dictionary (literal/replacement map)
    u8 tblSub[0x100];  // secondary dictionary (expansion prefixes)
    u8 queue[0x20];    // pending expansion output (LIFO)
    u8* out = (u8*)self;
    u8* in = buf->mData;
    u32 size = buf->mSize;
    u32 pos = 0;
    u32 total = 0;
    u8* d1;
    u8* d2;
    s32 n;
    for (s32 i = 0; i < 0x100; i++) {
        initTbl[i] = i;
    }
    while (pos < size) {
        s32 b = in[pos++];
        memcpy(tblMain, initTbl, 0x100);
        s32 idx = 0;
        while (true) {
            if (b > 0x7f) {
                idx += b - 0x7f;
                b = 0;
            }
            if (idx == 0x100) {
                break;
            }
            // Runs b + 1 times; MWCC emits this as an mtctr countdown.
            d1 = &tblMain[idx];
            d2 = &tblSub[idx];
            n = b + 1;
            while (--n >= 0) {
                u8 c = in[pos++];
                *d1++ = c;
                if (idx != c) {
                    *d2++ = in[pos++];
                }
                idx++;
            }
            if (idx == 0x100) {
                break;
            }
            b = in[pos++];
        }
        u32 code = (in[pos] << 8) | in[pos + 1];
        pos += 2;
        const u8* p = &in[pos];
        s32 sp = 0;
        while (true) {
            u8 c;
            if (sp != 0) {
                c = queue[--sp];
            } else {
                if (code == 0) {
                    break;
                }
                code--;
                c = *p++;
                pos++;
            }
            u8 rep = tblMain[c];
            if (c == rep) {
                *out++ = c;
                total++;
            } else {
                queue[sp++] = tblSub[c];
                queue[sp++] = rep;
            }
        }
    }
    return total;
}

// Parse a serialized control buffer into this: type 1/2/3 dispatch to the
// typed parsers, otherwise the payload is memcpy'd and its size returned.
u32 func_8009CE14(cf::CtrlRemote* self, CtrlRemoteBuf* src, s32 type, u32 arg)
{
    if (type == 1) {
        return func_8009C9B8(self, src, arg);
    }
    if (type == 2) {
        return func_8009CAAC(self, src, arg);
    }
    if (type == 3) {
        return func_8009CB80(self, src, arg);
    }
    memcpy(self, src->mData, src->mSize);
    return src->mSize;
}

// Clear the shared control-data buffer (initializing it first if needed).
void func_8009CE88()
{
    s32 flag = (s8)lbl_eu_80663E80;
    if (flag == 0) {
        memset(lbl_eu_80571848, 0, 0x1214);
        memset(&lbl_eu_80571848[0x1234 / 4], 0, 0x20);
        memset(&lbl_eu_80571848[0x1214 / 4], 0, 0x20);
        lbl_eu_80663E80 = 1;
    }
    memset(lbl_eu_80571848, 0, 0x1214);
}

// Return a pointer to the shared control-data buffer, initializing it once.
u32* func_8009CF0C()
{
    s32 flag = (s8)lbl_eu_80663E80;
    if (flag == 0) {
        memset(lbl_eu_80571848, 0, 0x1214);
        memset(&lbl_eu_80571848[0x1234 / 4], 0, 0x20);
        memset(&lbl_eu_80571848[0x1214 / 4], 0, 0x20);
        lbl_eu_80663E80 = 1;
    }
    return lbl_eu_80571848;
}

int func_8009CF84(void* self) { return 4628; }

u32* func_8009D12C(u32* buffer, s32 index, s32* typeOut, s32* idxOut);

extern "C" void func_8009CF8C(void* arg)
{
	s32 flag = (s8)lbl_eu_80663E80;
	if (flag == 0) {
		memset(lbl_eu_80571848, 0, 0x1214);
		memset(&lbl_eu_80571848[0x1234 / 4], 0, 0x20);
		memset(&lbl_eu_80571848[0x1214 / 4], 0, 0x20);
		lbl_eu_80663E80 = 1;
	}
	func_8009D1F8(lbl_eu_80571848, (s32)arg);
}

// Set a control-data bit: initialize the shared buffer if needed, then write
// `value` at index `destination` via the retail bit-setter.
void func_8009D018(u32 destination, u32 value)
{
    s32 flag = (s8)lbl_eu_80663E80;
    if (flag == 0) {
        memset(lbl_eu_80571848, 0, 0x1214);
        memset(&lbl_eu_80571848[0x1234 / 4], 0, 0x20);
        memset(&lbl_eu_80571848[0x1214 / 4], 0, 0x20);
        lbl_eu_80663E80 = 1;
    }
    func_8009D2C8(lbl_eu_80571848, destination, value);
}

// Return a pointer to the shared control-data buffer, initializing it once.
u32* func_8009D0B4()
{
    s32 flag = (s8)lbl_eu_80663E80;
    if (flag == 0) {
        memset(lbl_eu_80571848, 0, 0x1214);
        memset(&lbl_eu_80571848[0x1234 / 4], 0, 0x20);
        memset(&lbl_eu_80571848[0x1214 / 4], 0, 0x20);
        lbl_eu_80663E80 = 1;
    }
    return lbl_eu_80571848;
}

// Control-index -> (word pointer, type, sub-index) mapper. The four ranges
// (bits, nibbles, bytes, bit-planes) live in successive regions of the shared
// buffer; each entry's sub-index and element size select the word and mask.
// Locals are ordered so MWCC keeps the scaled offset live in r4/r0 exactly as
// retail does.
u32* func_8009D12C(u32* buffer, s32 index, s32* typeOut, s32* idxOut)
{
	s32 v;

	if ((u32)index <= 0x1F) {
		*idxOut = 0;
		*typeOut = 4;
		return &buffer[index];
	}

	v = index - 0x20;
	if ((u32)v <= 0x1FF) {
		*idxOut = v;
		*typeOut = 3;
		return &buffer[v / 2 + 0x20];
	}

	v = index - 0x220;
	if ((u32)v <= 0x7FF) {
		*idxOut = v;
		*typeOut = 2;
		return &buffer[v / 4 + 0x120];
	}

	v = index - 0xA20;
	if ((u32)v <= 0x2CAF) {
		*idxOut = v;
		*typeOut = 1;
		return &buffer[v / 32 + 0x320];
	}

	*idxOut = 0;
	*typeOut = 0;
	return NULL;
}

// Set a control-data field: map `index` to a (word, mask, shift), splice
// `value` into that word's bit range, then notify every registered callback
// object with (index, clamped value, extracted old bits). The switch has no
// default: on an out-of-range type the mask/shift keep their entry values and
// the word write still happens (matching retail).
void func_8009D2C8(u32* buffer, u32 index, u32 value)
{
    s32 type;
    s32 idx;
    s32 shift = 0;
    u32 mask;
    u32* ptr;
    u32 word;
    u32 bits;
    s32 i;
    u32 newBits;

    ptr = func_8009D12C(buffer, index, &type, &idx);
    if (ptr != NULL) {
        switch (type) {
        case 4:
            mask = 0xFFFFFFFF;
            shift = 0;
            break;
        case 3:
            if (value > 0xFFFF) {
                value = 0xFFFF;
            }
            mask = lbl_eu_80661C70[idx & 1];
            shift = (idx & 1) << 4;
            break;
        case 2:
            if (value > 0xFF) {
                value = 0xFF;
            }
            mask = lbl_eu_80528048[idx & 3];
            shift = (idx & 3) << 3;
            break;
        case 1:
            if (value > 1) {
                value = 1;
            }
            mask = lbl_eu_80528058[idx & 0x1F];
            shift = idx & 0x1F;
            break;
        }

        // Splice the (masked, shifted) value into the target word.
        word = *ptr;
        newBits = value << shift;
        newBits &= mask;
        *ptr = (word & ~mask) | newBits;
        bits = (word & mask) >> shift;

        for (i = 0; i < 8; i++) {
            CtrlDataSlotObj* obj = (CtrlDataSlotObj*)buffer[0x1234 / 4 + i];
            if (obj != NULL) {
                obj->vf03(index, value, bits);
            }
        }
    }
}

extern "C" u32 func_8009D1F8(u32* buffer, s32 index)
{
	s32 shift;
	s32 type;
	s32 idx;
	u32 mask;
	u32* ptr;

	shift = 0;
	ptr = func_8009D12C(buffer, index, &type, &idx);
	if (ptr == NULL) {
		return 0;
	}

	switch (type) {
	case 4:
		mask = 0xFFFFFFFF;
		shift = 0;
		break;
	case 3:
		mask = lbl_eu_80661C70[idx & 1];
		shift = (idx & 1) << 4;
		break;
	case 2:
		mask = lbl_eu_80528048[idx & 3];
		shift = (idx & 3) << 3;
		break;
	case 1:
		mask = lbl_eu_80528058[idx & 0x1F];
		shift = idx & 0x1F;
		break;
	}

	return (*ptr & mask) >> shift;
}

// Register a control callback object into one of the 8 shared control-data
// slots (buf + 0x1234 + 4*i). The shared buffer is lazily zero-initialized
// once. NOTE: guard1 reads the global directly (not the local) - MWCC hoists
// that loop-invariant load into the preheader and coalesces it with the
// `flag` local, so the retail's pre-loop lbz + per-iteration extsb. + the
// `li r0,1` after the first init all come out naturally. Removing the local
// or reading it in the guard regresses the bytes.
void func_8009D414(void* obj)
{
    s8 flag = lbl_eu_80663E80;
    for (s32 i = 0; i < 8; i++) {
        if ((s8)lbl_eu_80663E80 == 0) {
            memset(lbl_eu_80571848, 0, 0x1214);
            memset(&lbl_eu_80571848[0x1234 / 4], 0, 0x20);
            memset(&lbl_eu_80571848[0x1214 / 4], 0, 0x20);
            lbl_eu_80663E80 = 1;
            flag = 1;
        }
        if (lbl_eu_80571848[0x1234 / 4 + i] == 0) {
            if ((s8)lbl_eu_80663E80 == 0) {
                memset(lbl_eu_80571848, 0, 0x1214);
                memset(&lbl_eu_80571848[0x1234 / 4], 0, 0x20);
                memset(&lbl_eu_80571848[0x1214 / 4], 0, 0x20);
                lbl_eu_80663E80 = 1;
                flag = 1;
            }
            lbl_eu_80571848[0x1234 / 4 + i] = (u32)obj;
            break;
        }
    }
}

// Clear a control-data slot: ensure the shared buffer is initialized once
// (the retail keeps the flag in a local across the whole loop, so both init
// guards survive), then find the slot holding `index` and clear it.
// Each lazy-init block re-reads the buffer base into its own local so the
// allocator keeps one pointer copy per block, as in retail.
void func_8009D514(u32 index)
{
    s32 flag = lbl_eu_80663E80;
    u32* slot = lbl_eu_80571848;
    for (s32 i = 0; i < 8; i++) {
        if ((s8)lbl_eu_80663E80 == 0) {
            u32* b = lbl_eu_80571848;
            memset(b, 0, 0x1214);
            memset(&b[0x1234 / 4], 0, 0x20);
            memset(&b[0x1214 / 4], 0, 0x20);
            lbl_eu_80663E80 = 1;
            flag = 1;
        }
        if (slot[0x1234 / 4] == index) {
            if ((s8)lbl_eu_80663E80 == 0) {
                u32* b = lbl_eu_80571848;
                memset(b, 0, 0x1214);
                memset(&b[0x1234 / 4], 0, 0x20);
                memset(&b[0x1214 / 4], 0, 0x20);
                lbl_eu_80663E80 = 1;
                flag = 1;
            }
            slot[0x1234 / 4] = 0;
        }
        slot += 1;
    }
}
