// Auto-scaffolded catalog TU for kyoshin/cf/CtrlRemote
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

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

void func_80098CB8(){}

// Target us-800998d0. Player-aim update: probe the player's control gates
// and battle-manager state, then derive the aim fields (mField8/mFieldC /
// mField10/mField14) from the voice-owner target positions, the stick
// bytes, and the 0x1000 flag-range state. The final pass mirrors the
// aim-sector bits into self->mField4 and re-aims at the battle target.
void func_80098EF8(CtrlPcVf38* self)
{
    cf::CfGameManager::getInstance();
    UnkClass_800821F8View* gm8 = func_800821F8__Q22cf13CfGameManagerFv();
    CtrlVoiceHandle* actionSrc =
        (CtrlVoiceHandle*)func_800B708C(func_800FE68C()->mField90E4);

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
    u8 bs = getInstance__Q22cf14CBattleManagerFv()->mField1AA;
    int inBattle = (bs >= 1 && bs <= 0x18) ? 1 : 0;
    int timerNZ =
        getInstance__Q22cf14CBattleManagerFv()->mField20C8 != 0 ? 1 : 0;
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
            if (func_8006EF04(0x4000000) == 0) {
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
                if (func_8006EF04(0x4000000) != 0) {
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
        u32 mask2 = func_80086F9C__Q22cf13CfGameManagerFv(-1)
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

a89c:
    if (probePlayerCtrl((cf::CtrlPc*)self, &v3b, 3) == 0 &&
        probePlayerCtrl((cf::CtrlPc*)self, &v6b, 6) == 0 &&
        probePlayerCtrl((cf::CtrlPc*)self, &v9, 9) == 0 &&
        probePlayerCtrl((cf::CtrlPc*)self, &v806, 0x806) == 0) {
        // battle-target re-aim below
    } else if (self->mField5C->mSub3E9C.v01(4) != 0) {
        void* other = func_8016FE34(func_800B708C((s32)self->mField5C->mSub3E9C.v17()));
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

    if (func_802799F0(&getInstance__Q22cf14CBattleManagerFv()->mField1A8,
                      self->mField5C) != 0 &&
        probePlayerCtrl((cf::CtrlPc*)self, &v801b, 0x801) != 0) {
        CtrlPlayerSub298Vf4* sub298 =
            (CtrlPlayerSub298Vf4*)self->mField5C->vf164();
        void* t = func_8016FE34(func_800B708C((s32)sub298->mField4));
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
    if (func_8006EF04(0x4000000) != 0) {
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
        u32 mask = func_80086F9C__Q22cf13CfGameManagerFv(-1)
                       ? lbl_eu_80527F10[2]
                       : lbl_eu_80527E98[2];
        u32 mf4 = self->vf37()->mField4;
        if ((mf4 & mask) != 0) {
            if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0 &&
                (self->vf37()->mField0 & 0x2000000)) {
                return;
            }
            u32 mask2 = func_80086F9C__Q22cf13CfGameManagerFv(-1)
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
    UnkClass_800821F8View* gm8 = func_800821F8__Q22cf13CfGameManagerFv();
    CtrlVoiceHandle* actionSrc =
        (CtrlVoiceHandle*)func_800B708C(func_800FE68C()->mField90E4);

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

    u32 mask = func_80086F9C__Q22cf13CfGameManagerFv(-1)
                   ? lbl_eu_80527F10[16]
                   : lbl_eu_80527E98[16];
    CtrlRemoteSubA0* sub = (CtrlRemoteSubA0*)self->vf38();
    self->mField5C->mSub3ED4->vf10(0x400, (sub->mField0 & mask) != 0);

    if (probePlayerCtrl((cf::CtrlPc*)self, &v3, 0x803) != 0) {
        if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
            u32 mask2 = func_80086F9C__Q22cf13CfGameManagerFv(-1)
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
                u32 mask3 = func_80086F9C__Q22cf13CfGameManagerFv(-1)
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
                    u32 mask4 = func_80086F9C__Q22cf13CfGameManagerFv(-1)
                                    ? lbl_eu_80527F10[29]
                                    : lbl_eu_80527E98[29];
                    CtrlRemoteSubA0* s4 = (CtrlRemoteSubA0*)self->vf38();
                    if ((s4->mFieldC & mask4) != 0) {
                        func_8006BBF4(gm8, 0x100, 1);
                        u32 mask5 = func_80086F9C__Q22cf13CfGameManagerFv(-1)
                                        ? lbl_eu_80527F10[18]
                                        : lbl_eu_80527E98[18];
                        CtrlRemoteSubA0* s5 = (CtrlRemoteSubA0*)self->vf38();
                        if ((s5->mField0 & mask5) == 0) {
                            self->mField5C->mSub3ED4->vf11(0x2000);
                        }
                    } else {
                        u32 mask6 = func_80086F9C__Q22cf13CfGameManagerFv(-1)
                                        ? lbl_eu_80527F10[18]
                                        : lbl_eu_80527E98[18];
                        CtrlRemoteSubA0* s6 = (CtrlRemoteSubA0*)self->vf38();
                        if ((s6->mField0 & mask6) != 0 ||
                            ((func_80086F9C__Q22cf13CfGameManagerFv(-1)
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
            u32 mask7 = func_80086F9C__Q22cf13CfGameManagerFv(-1)
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
                u32 mask8 = func_80086F9C__Q22cf13CfGameManagerFv(-1)
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
                        u32 mask9 = func_80086F9C__Q22cf13CfGameManagerFv(-1)
                                        ? lbl_eu_80527F10[18]
                                        : lbl_eu_80527E98[18];
                        CtrlRemoteSubA0* s9 = (CtrlRemoteSubA0*)self->vf38();
                        if ((s9->mField0 & mask9) != 0) {
                            if ((gm8->mField4 & 0x8000) == 0) {
                                flagB = 1;
                                self->mField5C->mSub3ED4->vf10(0x2000, 1);
                            } else {
                                u32 maskA =
                                    func_80086F9C__Q22cf13CfGameManagerFv(-1)
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
                        u32 maskB = func_80086F9C__Q22cf13CfGameManagerFv(-1)
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
        u32 maskC = func_80086F9C__Q22cf13CfGameManagerFv(-1)
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

void func_8009AE80(){}

void func_8009B788(){}

void func_8009BD14(CtrlPcVf38* self)
{
    void* handle = func_800B708C(func_800FE68C()->mField90E4);
    if (handle == 0) {
        return;
    }
    CtrlPlayerObj* player = self->mField5C;
    u32 v = *player->mField4->vf30();
    if (func_80174C98(player, &v, 0x803) == 0) {
        return;
    }
    u8 bs = reinterpret_cast<CtrlBmSweepView*>(
                getInstance__Q22cf14CBattleManagerFv())
                ->mField1AA;
    bool inBattle = bs >= 1 && bs <= 0x18;
    if (inBattle) {
        return;
    }
    if (reinterpret_cast<CtrlBmSweepView*>(
            getInstance__Q22cf14CBattleManagerFv())
            ->mField20C8 != 0) {
        return;
    }
    cf::CfGameManager::getInstance();
    if (func_8006EF04(0x4000000) != 0) {
        return;
    }
    u32 mask = func_80086F9C__Q22cf13CfGameManagerFv(-1)
                   ? lbl_eu_80527F10[24]
                   : lbl_eu_80527E98[24];
    u32 mf0 = self->vf38()->mField0;
    if ((mf0 & mask) != 0) {
        // Count the battle-manager's circular actor list (+0x28 head); a solo
        // (or empty) party cannot trigger the menu-state sweeps below.
        CtrlSweepNode* head = reinterpret_cast<CtrlBmSweepView*>(
                                  getInstance__Q22cf14CBattleManagerFv())
                                  ->mField28;
        s32 count = 0;
        CtrlSweepNode* node = head->mNext;
        while (node != head) {
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
        u32 mask1 = func_80086F9C__Q22cf13CfGameManagerFv(-1)
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
                    func_8016FE34(func_800F6EAC(func_80043F18(&holder1), i)));
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
            u32 mask2 = func_80086F9C__Q22cf13CfGameManagerFv(-1)
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
                u32 hv = reinterpret_cast<CtrlVoiceSweepView*>(handle)->mField74;
                for (s32 i = 0;
                     i < reinterpret_cast<CtrlEnumListSweep*>(
                             func_80043F18(&holder2))
                             ->count;
                     i++) {
                    CtrlAccSweepView* acc = reinterpret_cast<CtrlAccSweepView*>(
                        func_8016FE34(func_800F6EAC(func_80043F18(&holder2), i)));
                    if (acc->mField3E98 != hv || (acc->mField3388 & 0x10) != 0) {
                        flag2 = true;
                    }
                    acc->mField3E98 = hv;
                    acc->mField3388 &= ~0x10;
                }
                if (flag2) {
                    func_802A2B44();
                    CtrlAccSweepView* acc2 =
                        reinterpret_cast<CtrlAccSweepView*>(func_8016FE34(handle));
                    func_800451D8(0xBC,
                                  acc2 != 0 ? &acc2->mOwner3E9C : 0);
                }
                __dt__80043E88(&holder2, -1);
            } else {
                u32 mask3 = func_80086F9C__Q22cf13CfGameManagerFv(-1)
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
                            func_8016FE34(func_800F6EAC(func_80043F18(&holder3), i)));
                        acc->mField3E98 = 0;
                        flag3 = true;
                        acc->mField3388 |= 0x10;
                    }
                    if (flag3) {
                        func_802A2C1C();
                        CtrlPlayerSweepView* pl =
                            reinterpret_cast<CtrlPlayerSweepView*>(self->mField5C);
                        func_800451D8(0xBC,
                                      pl != 0 ? &pl->mOwner3E9C : 0);
                    }
                    __dt__80043E88(&holder3, -1);
                }
            }
        }
    }
}

void func_8009C1BC(){}

void func_8009C6B4() {}

void func_8009C6B8() {}

void func_8009C6BC(){}

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
    if (func_80460DCC__17UnkClass_80460C34Fv(&ctx, lbl_eu_804FBC20, 0x38) != 0) {
        return 0;
    }
    s32 v = func_80460F58__17UnkClass_80460C34Fv(&ctx, 4);
    if (v != 1) {
        func_80461FE0__17UnkClass_80460C34Fv(&ctx);
        if (v != 0) {
            return 0;
        }
    }
    if (func_80461FE0__17UnkClass_80460C34Fv(&ctx) != 0) {
        return 0;
    }
    return size;
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

u32* func_8009D12C(u32* buffer, s32 index, s32* typeOut, s32* idxOut)
{
	if ((u32)index <= 0x1F) {
		*idxOut = 0;
		*typeOut = 4;
		return &buffer[index];
	}

	s32 v = index - 0x20;
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
// Clear a control-data slot: ensure the shared buffer is initialized once
// (the retail keeps the flag in a local across the whole loop, so both init
// guards survive), then find the slot holding `index` and clear it.
void func_8009D514(u32 index)
{
    s32 flag = (s8)lbl_eu_80663E80;
    u32* buf = lbl_eu_80571848;
    for (s32 i = 0; i < 8; i++) {
        if (flag == 0) {
            memset(lbl_eu_80571848, 0, 0x1214);
            memset(&buf[0x1234 / 4], 0, 0x20);
            memset(&buf[0x1214 / 4], 0, 0x20);
            lbl_eu_80663E80 = 1;
            flag = 1;
        }
        if (buf[0x1234 / 4 + i] == index) {
            if (flag == 0) {
                memset(lbl_eu_80571848, 0, 0x1214);
                memset(&lbl_eu_80571848[0x1234 / 4], 0, 0x20);
                memset(&lbl_eu_80571848[0x1214 / 4], 0, 0x20);
                lbl_eu_80663E80 = 1;
                flag = 1;
            }
            buf[0x1234 / 4 + i] = 0;
        }
    }
}

// Read-only accessor for the global flag word at lbl_eu_80663E88.
u32 func_8009D5FC() { return lbl_eu_80663E88; }
