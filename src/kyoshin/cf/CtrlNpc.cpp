// Auto-scaffolded catalog TU for kyoshin/cf/CtrlNpc
#include "kyoshin/code_800A3B24.hpp"
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/object/CfObjectMoveApi.hpp"
#include "monolib/scn/CScn.hpp"

#include <cstdio>
#include <cmath>

#include <monolib/math/CVec3.hpp>
#include <revolution/mtx/vec.h>  // PSVECMag
#include <monolib/math/Random.hpp>
#include <nw4r/math/math_arithmetic.h>
#include <nw4r/math/math_triangular.h>

#include "kyoshin/cf/CtrlNpc.hpp"
#include "kyoshin/cf/CfTFile.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"
#include "kyoshin/plugin/ocBdat.hpp"
// CfT_PlayRateGet is declared in CfGameManager.hpp as extern "C" float.

// typed retail data (replaces old byte blobs): strings sized to retail gaps,
// sdata2 as one struct to freeze MWCC order and keep the leading 0.0 live.
__declspec(section ".rodata") __attribute__((used)) const char lbl_eu_804FBB00[0x0C] = "cf::CtrlNpc";
struct Sdata2_CtrlNpc {
    float f0,f1,f2,f3,f4,f5,f6,f7,f8,f9;
    double d0;
    float f10,f11,f12,f13,f14,f15,f16,f17;
    double d1;
    float f18,f19,f20,f21,f22,f23,f24,f25,f26,f27;
};
__declspec(section ".sdata2") __attribute__((used, aligned(8)))
const Sdata2_CtrlNpc sdata2_CtrlNpc = {
    0.0f, 10.0f, 4.0f, 1.0f, 0.1f, 0.024543693f, 3.6f, 0.8f, 0.01f, 180.0f,
    4503601774854144.0,
    1000.0f, 3600.0f, 50.0f, 9.0f, 5.9f, -0.21f, 2.0f, 100.0f,
    4503599627370496.0,
    8.0f, 0.0625f, 0.0121f, 0.3f, 5.0f, 0.09f, 0.040000003f, 0.2f, 0.5f, 360.0f
};
#define lbl_eu_80666698 sdata2_CtrlNpc.f0
#define lbl_eu_8066669C sdata2_CtrlNpc.f1
#define lbl_eu_806666A0 sdata2_CtrlNpc.f2
#define lbl_eu_806666A4 sdata2_CtrlNpc.f3
#define lbl_eu_806666A8 sdata2_CtrlNpc.f4
#define lbl_eu_806666AC sdata2_CtrlNpc.f5
#define lbl_eu_806666B0 sdata2_CtrlNpc.f6
#define lbl_eu_806666B4 sdata2_CtrlNpc.f7
#define lbl_eu_806666B8 sdata2_CtrlNpc.f8
#define lbl_eu_806666BC sdata2_CtrlNpc.f9
#define lbl_eu_806666C0 sdata2_CtrlNpc.d0
#define lbl_eu_806666C8 sdata2_CtrlNpc.f10
#define lbl_eu_806666CC sdata2_CtrlNpc.f11
#define lbl_eu_806666D0 sdata2_CtrlNpc.f12
#define lbl_eu_806666D4 sdata2_CtrlNpc.f13
#define lbl_eu_806666D8 sdata2_CtrlNpc.f14
#define lbl_eu_806666DC sdata2_CtrlNpc.f15
#define lbl_eu_806666E0 sdata2_CtrlNpc.f16
#define lbl_eu_806666E4 sdata2_CtrlNpc.f17
#define lbl_eu_806666E8 sdata2_CtrlNpc.d1
#define lbl_eu_806666F0 sdata2_CtrlNpc.f18
#define lbl_eu_806666F4 sdata2_CtrlNpc.f19
#define lbl_eu_806666F8 sdata2_CtrlNpc.f20
#define lbl_eu_806666FC sdata2_CtrlNpc.f21
#define lbl_eu_80666700 sdata2_CtrlNpc.f22
#define lbl_eu_80666704 sdata2_CtrlNpc.f23
#define lbl_eu_80666708 sdata2_CtrlNpc.f24
#define lbl_eu_8066670C sdata2_CtrlNpc.f25
#define lbl_eu_80666710 sdata2_CtrlNpc.f26
#define lbl_eu_80666714 sdata2_CtrlNpc.f27

// Retail ctor is a free function (MWCC_CASES §4193): stores the base
// vtable, initializes the base fields, creates the character object, then
// stores the final vtable and constructs the CCtrlMoveNpc sub-object at +0x30.
cf::CtrlNpc* __ct__CtrlNpc(cf::CtrlNpc* self, cf::CfObject* param) {
    self->field_04 = 0;
    self->field_08 = lbl_eu_80666698;
    self->field_0C = lbl_eu_80666698;
    self->field_10 = lbl_eu_80666698;
    self->field_14 = lbl_eu_80666698;
    self->field_18 = 0;
    self->field_1C = 2;
    self->field_24 = -1;
    *(void**)self = (void*)lbl_eu_80527BB0;   // base vtable (retail position)
    self->field_28 = (cf::CfObjectMove*)func_800BBC0C(param);
    self->field_2C = 0;
    *(void**)self = (void*)lbl_eu_80527B38;   // final vtable
    __ct__Q22cf12CCtrlMoveNpcFPv((cf::CCtrlMoveNpc*)self->_sub30, self);
    self->field_AC.f = lbl_eu_80666698;
    self->field_B0.f = lbl_eu_80666698;
    self->field_B4.f = lbl_eu_80666698;
    self->field_B8 = 0;
    self->field_BC = 0;
    self->field_BE = 0;
    self->field_C0 = 0;
    self->field_C2 = 0;
    self->field_C3 = 0;
    self->field_C4 = 0;
    self->field_D4 = 10.0f;
    self->field_DC = 0;
    self->field_DE = 0x1e;
    self->field_172 = 0;
    self->field_174 = 0;
    self->field_178 = lbl_eu_80666698;
    return self;
}

// 0x80094154: set the action id. When switching away from a movement action
// (4/5), report the current target/position offset to the character object
// and (on success) bump the page counter / set the done flags.
void CtrlNpcSetAction(cf::CtrlNpc* self, u32 param) {
    reinterpret_cast<cf::CObjectParam*>(self->field_28)->CObjectParam_getParamPtr();
    u16 c0 = self->field_C0;
    self->field_DC = 0;
    if (param != c0) {
        if (c0 == 4 || c0 == 5) {
            const ml::CVec3* posf =
                reinterpret_cast<const ml::CVec3*>(reinterpret_cast<cf::CfObject*>(self->field_28)->CfObject_getPosVector());
            const ml::CVec3* tgt =
                reinterpret_cast<const ml::CVec3*>(&self->field_AC);
            // Kernel output vector declared first; snapshot copy second.
            // VEC3Sub emits the paired-single subtraction kernel;
            // VEC3LenSq's register-bound param keeps &diff materialized in a
            // callee-saved register across the offset 0x160 slot call, like retail.
            ml::CVec3 v;
            nw4r::math::VEC3Sub(
                reinterpret_cast<nw4r::math::VEC3*>(&v),
                reinterpret_cast<const nw4r::math::VEC3*>(tgt),
                reinterpret_cast<const nw4r::math::VEC3*>(posf));
            ml::CVec3 diff = v;
            const nw4r::math::VEC3* pd =
                reinterpret_cast<const nw4r::math::VEC3*>(&diff);
            if (reinterpret_cast<cf::CfObject*>(self->field_28)->CfObject_checkSubReady(&v) != 0 &&
                (gmPauseFlags & 0x04000000) == 0) {
                f32 len2 = nw4r::math::VEC3LenSq(pd);
                if (len2 < 4.0f) {
                    self->field_DC += 1;
                    if (self->field_C0 == 4) {
reinterpret_cast<cf::CfObjectMove*>(self->field_28)->CfObjectMove_setTargetC4Arg(0);
                        self->field_2C |= 0x10;
                    } else if (self->field_C0 == 5) {
                        self->field_2C |= 0x8;
                    }
                }
            }
        }
    }
    if (param <= 3) {
        CfObjectMove_setAnimModeArgs((u8*)self->field_28, 1, 1, -1, 1);
    }
    self->field_174 = 0;
    self->field_172 = 0;
    self->field_C0 = (u16)param;
    self->field_14 = lbl_eu_80666698;
    self->field_BE = 0;
}

// TU-local view of the CfObjectMove +0xC4 target word written by
// CfObjectMove_UnkVirtualFunc6 (exact layout; the target class is unnamed).
struct CtrlNpcC4Target {
    u8 _pad[0x37C];            // 0x00-0x37B
    unsigned long field_37C;  // 0x37C-0x37F
};
void cf::CfObjectMove::CfObjectMove_setTargetC4Arg(u32 val) {
    void* ptr = this->mTargetC4;
    if (ptr != 0)
        reinterpret_cast<CtrlNpcC4Target*>(ptr)->field_37C = val;
}

// 0x80094310: clear field_04, then run the char-object virtuals at vtable
// slots +0x58/+0x5C.
void CtrlNpcCharInitState(cf::CCtrlNpcChar* self) {
    self->field_04 = 0;
    reinterpret_cast<cf::CfObject*>(self)->CfObject_initEventState();
    reinterpret_cast<cf::CfObject*>(self)->CfObject_syncEnableState();
}

extern "C" void CtrlNpcAdvanceBehavior(cf::CtrlNpc* self);

// 0x8009398C: per-frame NPC action-state dispatcher. Gates on the
// presentation flag probe and the character's busy/lock virtuals, then runs
// the per-action handler table (field_BE-indexed member-pointer tables) and
// the shared search-helper progress check (inlined per action, as in retail);
// finally, if the character reports a heading update is due, re-feeds
// field_0C into it.
void CtrlNpcUpdateAction(cf::CtrlNpc* self) {
    u16 act;
    // Singleton fetched but unused at this site (retail discards the result).
    getInstance__Q22cf13CfGameManagerFv();
    if (isGlobalCamFlagSet(0x1000000))
        goto gateFail;
    if (reinterpret_cast<cf::CObjectState*>(self->field_28)->checkStateFlags8(1) != 0)
        goto gateFail;
    if (reinterpret_cast<cf::CObjectState*>(self->field_28)->checkStateFlags8(0x4000) == 0)
        goto bodyStart;
gateFail:
    self->field_14 = lbl_eu_80666698;
    return;
bodyStart:
    {
            if ((NpcCharView(self->field_28)->field_68 & 0x00100000) == 0 &&
                reinterpret_cast<cf::CObjectState*>(self->field_28)->checkStateFlags(1) != 0) {
            reinterpret_cast<cf::CObjectState*>(self->field_28)->setStateBitFlag(1);
            reinterpret_cast<cf::CfObjectModel*>(self->field_28)->CfObjectModel_bindModelTo(nullptr, "JUhead");
        }

        if ((self->field_2C & 0x08) != 0) {
            self->field_14 = lbl_eu_80666698;
            // Search helper kept live across the calls (retail r30).
            cf::CCtrlNpcSearch* search = NpcCharView(self->field_28)->field_98;
            if (search == 0)
                goto mask08;
            if (simGetLeafAnimDist(search) < lbl_eu_806666A4)
                goto mask08;
            if (NpcCharView(self->field_28)->field_C4 != 0)
                goto tail;
        mask08:;
            self->field_2C &= ~0x08;
            if (search == 0)
                goto tail;
            if (self->field_170 < 1)
                goto tail;
            CfObjectMove_setAnimModeArgs((u8*)self->field_28, self->field_170, 1, -1, 1);
            self->field_2C |= 0x10;
            goto tail;
        }

        if ((self->field_2C & 0x10) != 0) {
            // retail keeps this as an int tested with cmpi, not a bool.
            int ready = reinterpret_cast<cf::CfObject*>(self->field_28)->CfObject_checkTargetState();
            if (ready == 0) {
                cf::CCtrlNpcSearch* s = NpcCharView(self->field_28)->field_98;
                if (s != 0 && (s->field_7A4 & 0x00010000) != 0)
                    ready = 1;
            }
            if (ready == 0)
                goto tail;
            self->field_2C &= ~0x10;
            NpcCharView(self->field_28)->field_6C4 = 1;
            goto tail;
        }

        act = self->field_C0;
        if ((u16)(act + 0xFFFF) <= 1) { // actions 1..2: movement/walk handlers
            if (self->field_28 == 0)
                goto noMove;
            // Offset query takes the action kind (retail reuses r4 holding
            // the halfword unchanged).
            if (reinterpret_cast<cf::CfObject*>(self->field_28)->CfObject_checkSubReady(reinterpret_cast<const ml::CVec3*>(act)) != 0 ||
                (NpcCharView(self->field_28)->field_68 & 0x2000) != 0) {
                int be = self->field_BE;
                if (be >= 0) {
                    if (be < 4)
                        ((void (*)(cf::CtrlNpc*))gNpcActionState1Handlers[be*3+2])(self);
                }
                goto tail;
            } else {
            noMove:
                self->field_14 = lbl_eu_80666698;
            }
            goto tail;
        }

        // Shared post-dispatch check (inlined per action in retail): when the
        // search helper's progress floats cross their thresholds, run its
        // settle helper.
        if (act == 3) {
            if (self->field_15C != 0) {
                int be = self->field_BE;
                if (be >= 0) {
                    if (be < 3)
                        ((void (*)(cf::CtrlNpc*))gNpcActionState2Handlers[be*3+2])(self);
                }
            }
            if (NpcCharView(self->field_28)->field_98 != 0) {
                cf::CfObjectMove* o = self->field_28;
                if (lbl_eu_80666698 >= reinterpret_cast<cf::CfObject*>(o)->CfObject_getProgressHigh())
                    goto chk3;
                if (reinterpret_cast<cf::CfObject*>(o)->CfObject_getProgressHigh() < lbl_eu_806666A4)
                    goto settle3;
            chk3:
                if (lbl_eu_80666698 >= reinterpret_cast<cf::CfObject*>(o)->CfObject_getProgressLow())
                    goto tail;
                if (reinterpret_cast<cf::CfObject*>(o)->CfObject_getProgressLow() >= lbl_eu_806666A4)
                    goto tail;
            settle3:
                scnImN4DynStart(reinterpret_cast<cf::CfObjectModelSub98*>(NpcCharView(o)->field_98));
            }
            goto tail;
        }
        if (act == 6) {
            CtrlNpcAdvanceBehavior(self);
            goto tail;
        }
        if (act == 4) {
            int be = self->field_BE;
            if (be >= 0) {
                if (be < 2)
                    ((void (*)(cf::CtrlNpc*))gNpcAnimMode16Handlers[be*3+2])(self);
            }
            if (NpcCharView(self->field_28)->field_98 != 0) {
                cf::CfObjectMove* o = self->field_28;
                if (lbl_eu_80666698 >= reinterpret_cast<cf::CfObject*>(o)->CfObject_getProgressHigh())
                    goto chk4;
                if (reinterpret_cast<cf::CfObject*>(o)->CfObject_getProgressHigh() < lbl_eu_806666A4)
                    goto settle4;
            chk4:
                if (lbl_eu_80666698 >= reinterpret_cast<cf::CfObject*>(o)->CfObject_getProgressLow())
                    goto tail;
                if (reinterpret_cast<cf::CfObject*>(o)->CfObject_getProgressLow() >= lbl_eu_806666A4)
                    goto tail;
            settle4:
                scnImN4DynStart(reinterpret_cast<cf::CfObjectModelSub98*>(NpcCharView(o)->field_98));
            }
            goto tail;
        }
        if (act == 5) {
            int be = self->field_BE;
            if (be >= 0) {
                if (be < 3)
                    ((void (*)(cf::CtrlNpc*))gNpcAnimModeHandlers[be*3+2])(self);
            }
            if (NpcCharView(self->field_28)->field_98 != 0) {
                cf::CfObjectMove* o = self->field_28;
                if (lbl_eu_80666698 >= reinterpret_cast<cf::CfObject*>(o)->CfObject_getProgressHigh())
                    goto chk5;
                if (reinterpret_cast<cf::CfObject*>(o)->CfObject_getProgressHigh() < lbl_eu_806666A4)
                    goto settle5;
            chk5:
                if (lbl_eu_80666698 >= reinterpret_cast<cf::CfObject*>(o)->CfObject_getProgressLow())
                    goto tail;
                if (reinterpret_cast<cf::CfObject*>(o)->CfObject_getProgressLow() >= lbl_eu_806666A4)
                    goto tail;
            settle5:
                scnImN4DynStart(reinterpret_cast<cf::CfObjectModelSub98*>(NpcCharView(o)->field_98));
            }
            goto tail;
        }
        self->field_BE = 0;
        self->field_14 = lbl_eu_80666698;
        goto tail;

    tail:
        if (reinterpret_cast<cf::CObjectState*>(self->field_28)->checkStateFlags(4) == 0)
            return;
        if (NpcCharView(self->field_28)->field_C4 == 0)
            return;
        self->field_14 = lbl_eu_80666698;
        reinterpret_cast<cf::CfObject*>(self->field_28)->CfObject_setMoveHeadAngle(self->field_0C);
        CfObjectMove_setAnimModeArgs((u8*)self->field_28, 3, 0, -1, 1);
        reinterpret_cast<cf::CObjectState*>(self->field_28)->setStateBitFlag(4);
            return;
    }
}

// 0x80094900: per-frame NPC talk/page controller update. Runs while the scene
// clock is running: adjusts the character's action flags, tracks the current
// event page against the saved heading, then (when the character is not busy)
// looks up the NPC's talk row in the bdat tables and drives the heading toward
// the talk target.
// Message buffer: text at 0x88..0x187 and length word at 0x188 (the struct's
// address escapes through strcpy / UIWin_CreateTalkWin, which keeps the length
// stores alive exactly like retail).
struct NpcMsgBuf {
    char text[0x100];   // 0x00
    u32 len;            // 0x100
};

void CtrlNpcUpdateTalk(cf::CtrlNpc* self) {
    // Frame homes mirror retail (top-down by first use): message buffer,
    // name buffer, heading-diff vectors, then the column slots (first-used
    // highest). Column unions stay memory-resident like retail.
    NpcMsgBuf msg;
    ml::CVec3 diff;
    ml::CVec3 diffCopy;
    NpcColNarrow v48, v44, v40, v3C, v38, v34, v30, v2C, v28, v24, v20, v1C,
        v18, v14, v10, v0C, v08;

    if (Scn_GetFrameDelta(gmScene) > lbl_eu_80666698) {
        msg.text[0] = 0;
        msg.len = 0;

        if (reinterpret_cast<cf::CObjectState*>(self->field_28)->checkStateFlags(8) != 0) {
            reinterpret_cast<cf::CObjectState*>(self->field_28)->applyStateFlags(0x2000);
        } else {
            reinterpret_cast<cf::CObjectState*>(self->field_28)->clearStateFlags8(0x2000);
        }

        if (reinterpret_cast<cf::CObjectState*>(self->field_28)->checkStateFlags8(1) != 0) {
            reinterpret_cast<cf::CObjectState*>(self->field_28)->setStateBitFlag(1);
        } else if (self->field_172 != 0) {
            f32 d = self->field_178 - reinterpret_cast<cf::CfObject*>(self->field_28)->CfObject_getMoveHeadAngle();
            if (ml::math::abs(d) <= 0.1f)
                self->field_172 = 0;
        }

        if (reinterpret_cast<cf::CObjectState*>(self->field_28)->checkStateFlags(1) != 0 &&
            reinterpret_cast<cf::CObjectState*>(self->field_28)->checkStateFlags8(0x1000) == 0) {
            cf::CfObject* target =
                (cf::CfObject*)findObjectById(reinterpret_cast<cf::CObjectParam*>(self->field_28)->CObjectParam_getSelfObjectId());
            if (target != 0 && NpcCharView(self->field_28)->field_C4 != 0) {
                int page = (int)getAnimModelId(NpcCharView(self->field_28)->field_C4);
                int flag = 1;
                u32 st = NpcCharView(self->field_28)->field_8C;
                if ((st == 1 || st == 8 || st == 0x13) && page >= 0x21 &&
                    page <= 0x26)
                    flag = 0;
                if (flag != 0) {
                    cf::CCtrlNpcSearch* search = NpcCharView(self->field_28)->field_98;
                    if (search != 0) {
                        // retail compares against 0xFFFFFFFF (unsigned ->
                        // addis+cmplwi form).
                        u32 found =
                            (u32)search->findNameIndex("JUhead");
                        if (found != 0xFFFFFFFF) {
                            char* t = (char*)target;
                            if (t != 0) t -= 0x3E9C;
                            if (t != 0) t += 0x3E9C;
                            reinterpret_cast<cf::CfObjectModel*>(self->field_28)->CfObjectModel_bindModelTo(reinterpret_cast<cf::CfObject*>(t), "JUhead");
                        }
                    }
                }
                if (page < 0x21 || page > 0x26) {
                    if (self->field_174 == 0)
                        self->field_174 = (u16)page;
                    if (self->field_172 == 0)
                        self->field_178 = reinterpret_cast<cf::CfObject*>(self->field_28)->CfObject_getMoveHeadAngle();
                    const ml::CVec3* selfPos = reinterpret_cast<
                        const ml::CVec3*>(reinterpret_cast<cf::CfObject*>(self->field_28)->CfObject_getPosVector());
                    const ml::CVec3* tgtPos = reinterpret_cast<
                        const ml::CVec3*>(target->CfObject_getPosVector());
                    // Paired-single kernel: diff = tgt - self, snapshot copy,
                    // then heading from the x/z components.
                    nw4r::math::VEC3Sub(
                        reinterpret_cast<nw4r::math::VEC3*>(&diff),
                        reinterpret_cast<const nw4r::math::VEC3*>(tgtPos),
                        reinterpret_cast<const nw4r::math::VEC3*>(selfPos));
                    diffCopy = diff;
                    // atan args come from the snapshot copy (retail reuses
                    // the copy's f1/f2 loads for the call arguments).
                    f32 h = 0.024543693f *
                            nw4r::math::Atan2FIdx(diffCopy.x, diffCopy.z);
                    reinterpret_cast<cf::CfObject*>(self->field_28)->CfObject_setMoveHeadAngle(h);
                    CfObjectMove_setAnimModeArgs((u8*)self->field_28, 3, 0, -1, 1);
                } else {
                    self->field_174 = 0;
                }
            }
            if (reinterpret_cast<cf::CObjectState*>(self->field_28)->checkStateFlags8(0x1000) == 0) {
                reinterpret_cast<cf::CObjectState*>(self->field_28)->setStateBitFlag(1);
                reinterpret_cast<cf::CObjectState*>(self->field_28)->applyStateFlags(1);
                if (target != 0)
                    self->field_172 = 1;
            }
        }

        // Not in the talk state: run the page cleanup/advance logic.
        if (reinterpret_cast<cf::CObjectState*>(self->field_28)->checkStateFlags(1) == 0) {
            if (reinterpret_cast<cf::CObjectState*>(self->field_28)->setStateBitMask0(1, 0) != 0 && self->field_174 != 0) {
                reinterpret_cast<cf::CfObject*>(self->field_28)->CfObject_setMoveHeadAngle(self->field_178);
                CfObjectMove_setAnimModeArgs((u8*)self->field_28, 3, 0, -1, 1);
                if (reinterpret_cast<cf::CObjectState*>(self->field_28)->checkStateFlags8(0x1000) == 0) {
                    CfObjectMove_setAnimModeArgs((u8*)self->field_28, self->field_174, 0, -1, 1);
                    self->field_174 = 0;
                }
            }
            if (self->field_174 != 0 && reinterpret_cast<cf::CObjectState*>(self->field_28)->checkStateFlags8(0x1000) == 0 &&
                reinterpret_cast<cf::CObjectState*>(self->field_28)->checkStateFlags8(1) == 0) {
                CfObjectMove_setAnimModeArgs((u8*)self->field_28, self->field_174, 0, -1, 1);
                self->field_174 = 0;
            }
        }

        Bdat_GetTable_AA34();
        Bdat_GetTable_AA34();
        void* fp1 = getFP("FLD_npclist");
        // Zeroed here (not at function top) so the 8-word inline clear lands
        // between the getFP call and sprintf, like retail.
        char sbuf[0x20] = {0};
        sprintf(sbuf, "autotalklist%02d%02d", gmGlobalFlag42,
                gmGlobalFlag44);
        Bdat_GetTable_AA34();
        void* fp2 = getFP(sbuf);

        if (reinterpret_cast<cf::CObjectState*>(self->field_28)->setStateBitMask0(0x2000, 1) != 0) {
            self->field_17C = 0;
            // Scan the 6 NPC-kind rows; each column gate must pass before the
            // message column is copied into strbuf and handed to the text
            // object.
            // Scan the 6 NPC-kind rows; each column gate must pass before the
            // message column is copied into strbuf and handed to the text
            // object.
            for (u32 i = 0; i < 6; i++) {
                v48.w = getBdatStringColumnValue(
                    fp1, gNpcAutotalkColumnNames[i], NpcCharView(self->field_28)->field_8C);
                u8 count = v48.b;
                if (count == 0)
                    continue;

                v44.w = getBdatStringColumnValue(
                    fp2, "scenario_s", count);
                v40.w = getBdatStringColumnValue(
                    fp2, "scenario_e", count);
                u32 cur = CtrlRemote_TouchBitByArg(UIWin_PackHiLo(0x200001, 0));
                if ((int)cur < (int)v44.h)
                    continue;
                if ((int)v40.h < (int)cur)
                    continue;

                const char* col2 =
                    gNpcTimeColumnNames[(u16)getControllerWordA33C__Q22cf13CfGameManagerFv() /
                                    3];
                v3C.w = getBdatStringColumnValue(fp2, col2, count);
                if (v3C.b == 0)
                    continue;

                v38.w = getBdatStringColumnValue(
                    fp2, "quest_no", count);
                if (v38.h != 0) {
                    u32 f = CtrlRemote_TouchBitByArg(UIWin_PackHiLo(0x2203E8, v38.h));
                    if (f != 0xFE && f != 0xFF)
                        continue;
                }

                v34.w = getBdatStringColumnValue(
                    fp2, "relate_no", count);
                if (v34.h != 0) {
                    u32 f =
                        CtrlRemote_TouchBitByArg(UIWin_PackHiLo(0x608190, v34.h));
                    v30.w = getBdatStringColumnValue(
                        fp2, "relate", count);
                    if (f != v30.b)
                        continue;
                }

                v2C.w = getBdatStringColumnValue(
                    fp2, "select_no", count);
                if (v2C.b != 0) {
                    u32 f = CtrlRemote_TouchBitByArg(UIWin_PackHiLo(0x798064, v2C.b));
                    v28.w = getBdatStringColumnValue(
                        fp2, "select", count);
                    if (f != v28.b)
                        continue;
                }

                v24.w = getBdatStringColumnValue(
                    fp2, "popular_no", count);
                if (v24.b != 0) {
                    u32 f = CtrlRemote_TouchBitByArg(UIWin_PackHiLo(0x210007, v24.b));
                    v20.w = getBdatStringColumnValue(
fp2, "popular", count);
                    if (f < v20.h)
                        continue;
                }

                v1C.w = getBdatStringColumnValue(
fp2, "npcmeet_no", count);
                if (v1C.h != 0) {
                    u32 f =
                        CtrlRemote_TouchBitByArg(UIWin_PackHiLo(0xA2012C, v1C.h));
                    if (f == 0)
                        continue;
                }

                v18.w = getBdatStringColumnValue(
                    fp2, "col6_h_no", count);
                if (v18.b != 0) {
                    u32 f = CtrlRemote_TouchBitByArg(UIWin_PackHiLo(0x7FC008, v18.b));
                    v14.w = getBdatStringColumnValue(
                        fp2, "col6_h", count);
                    if (f < v14.h)
                        continue;
                }

                v10.w = getBdatStringColumnValue(
                    fp2, "col6_k_no", count);
                if (v10.b != 0) {
                    u32 f = CtrlRemote_TouchBitByArg(UIWin_PackHiLo(0x25781E, v10.b));
                    v0C.w = getBdatStringColumnValue(
                        fp2, "col6_k", count);
                    if (f != v0C.b)
                        continue;
                }

                const char* text = (const char*)getBdatStringColumnValue(
                    fp2, "auto_mes", count);
                msg.len = strlen(text);
                strcpy(msg.text, text);
                UIWin_CreateTalkWin(NpcCharView(self->field_28)->field_74, msg.text, 0);
                self->field_17C = count;
                break;
            }
        } else if (reinterpret_cast<cf::CObjectState*>(self->field_28)->checkStateFlags8(0x2000) != 0 &&
                   self->field_17C != 0) {
            const char* col3 =
                gNpcTimeColumnNamesAlt[(u16)getControllerWordA33C__Q22cf13CfGameManagerFv() /
                                3];
            v08.w = getBdatStringColumnValue(fp2, col3, self->field_17C);
            if (v08.b == 0) {
                reinterpret_cast<cf::CObjectState*>(self->field_28)->clearStateFlags8(0x2000);
                reinterpret_cast<cf::CObjectState*>(self->field_28)->setStateBitFlag(8);
            }
        }

        if (reinterpret_cast<cf::CObjectState*>(self->field_28)->setStateBitMask0(0x2000, 1) != 0)
            UIWin_ShowTalkWindow(NpcCharView(self->field_28)->field_74);
    }
}

// 0x80095274: cf::CObjectState::applyStateFlags. The retail
// symbol keeps the Fv suffix, but the virtual is invoked with an int (see
// object/CObjectState.hpp and CfObjectNpc.cpp callers), so the body is emitted
// under the literal retail name with the caller's real arg shape.
extern "C" void CObjectState_applyStateFlags__Q22cf12CObjectStateFi(cf::CObjectState* self, int arg) {
    if ((u32)arg < 0x3f)
        self->clearStateFlags8(0x3f);
    self->unk8 |= (u32)arg;
}

// 0x800952D0: record the movement range (field_160/164) and duration, then
// copy `count` 12-byte elements from src into the +0xE0 target array. Bulk
// copies run in 8-element groups (retail unrolls each group into 24 word
// loads/stores).
void CtrlNpcInitMoveWindow(cf::CtrlNpc* self, u32 a, u32 b, int count,
                   const ml::CVec3* src, f32 f) {
    // Retail evaluates the frame-count expression before any stores.
    self->field_168 = (s16)((b - a) * 60);
    self->field_160 = a;
    self->field_164 = b;
    self->field_BE = 0;
    self->field_C0 = 3;
    self->field_C4 = 1;
    self->field_158 = f;
    self->field_15C = (s16)count;
    // Retail's 8-way unroll + overflow guard comes from MWCC itself here.
    for (int i = 0; i < count; i++) {
        ml::CVec3& dstElem = *reinterpret_cast<ml::CVec3*>(&self->field_E0[i]);
        dstElem = src[i];
    }
}

// 0x80095474: record the movement target words (current position from the
// character object + the new target vec), then compute the heading toward the
// a distance check picks a turn amount (jittered/continued), the
// heading is stored to field_0C, and the character is kicked if it is idle.
#undef lbl_eu_80666698
#undef lbl_eu_806666A4
#undef lbl_eu_806666A8
#undef lbl_eu_806666AC
#undef lbl_eu_806666B0
void __declspec(noinline) CtrlNpcSetupMoveAction(cf::CtrlNpc* self,
                                        const ml::CVec3* vec, f32 scale,
                                        f32 paramB) {
    cf::CtrlNpcVec3W* pos = reinterpret_cast<cf::CtrlNpcVec3W*>(reinterpret_cast<cf::CfObject*>(self->field_28)->CfObject_getPosVector());
    const u32* vw = reinterpret_cast<const u32*>(vec);
    u32 py = pos->y;
    u32 px = pos->x;
    self->field_E0w[1] = py;
    self->field_E0w[0] = px;
    self->field_E0w[2] = pos->z;
    self->field_E0w[3] = vw[0];
    self->field_E0w[4] = vw[1];
    self->field_E0w[5] = vw[2];
    self->field_158 = paramB;
    self->field_160 = getFieldD90Value__Q22cf13CfGameManagerFv();
    self->field_C4 = 1;
    self->field_15C = 2;
    const ml::CVec3* posf = reinterpret_cast<const ml::CVec3*>(pos);
    ml::CVec3 v;
    ml::CVec3 v2;
    nw4r::math::VEC3 diff;
    nw4r::math::VEC3 diff2;
    nw4r::math::VEC3Sub(&diff, reinterpret_cast<const nw4r::math::VEC3*>(vec),
                        reinterpret_cast<const nw4r::math::VEC3*>(posf));
    v.set(*(const ml::CVec3*)&diff);
    f32 len2 = v.x * v.x + v.z * v.z;
    if (!(len2 >= lbl_eu_80666698)) {
        nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
    }
    f32 len = (len2 <= lbl_eu_80666698) ? lbl_eu_80666698
                                        : len2 * nw4r::math::FrSqrt(len2);
    f32 f0 = (lbl_eu_806666B0 * len) / scale;
    f0 = f0 / *reinterpret_cast<const float*>(reinterpret_cast<cf::CfObject*>(self->field_28)->CfObject_getMoveRateScale());
    f32 f1 = f0 < lbl_eu_806666A8 ? lbl_eu_806666A8
             : (f0 > lbl_eu_806666A4 ? lbl_eu_806666A4 : f0);
    self->field_14 = lbl_eu_80666698;
    self->field_D8 = f1;
    nw4r::math::VEC3Sub(&diff2, reinterpret_cast<const nw4r::math::VEC3*>(vec),
                        reinterpret_cast<const nw4r::math::VEC3*>(posf));
    v2.set(*(const ml::CVec3*)&diff2);
    f32 heading = nw4r::math::Atan2FIdx(v2.x, v2.z) * lbl_eu_806666AC;
    self->field_0C = heading;
    if (self->field_28 != 0) {
        reinterpret_cast<cf::CfObject*>(self->field_28)->CfObject_setMoveHeadAngle(heading);
        if (reinterpret_cast<cf::CfObject*>(self->field_28)->CfObject_isMoveActiveNow() != 0)
            CfObjectMove_setAnimModeArgs((u8*)self->field_28, 3, 0, -1, 1);
    }
}
#define lbl_eu_80666698 sdata2_CtrlNpc.f0
#define lbl_eu_806666A4 sdata2_CtrlNpc.f3
#define lbl_eu_806666A8 sdata2_CtrlNpc.f4
#define lbl_eu_806666AC sdata2_CtrlNpc.f5
#define lbl_eu_806666B0 sdata2_CtrlNpc.f6

// 0x800956C0: store the action-setup fields, then dispatch: a zero action id
// arms the movement sub-object (CCtrlMoveNpc at +0x30), anything else runs
// the generic action setup.
void CtrlNpcSetupAction(cf::CtrlNpc* self, const ml::CVec3* vec, int val, f32 scale, f32 paramB) {
    u16 v = self->field_C6;
    v = (u16)__rlwimi(v, (u32)val, 8, 16, 23);
    self->field_C0 = 6;
    self->field_C6 = v;
    self->field_158 = paramB;
    self->field_BE = 1;
    if (val != 0)
        CtrlNpcSetupMoveAction(self, vec, scale, paramB);
    else
        beginNpcMove((cf::CCtrlMoveNpc*)self->_sub30, vec, scale, paramB);
}

// 0x800956F4: action-advance query - whether the NPC may advance from its
// current busy state (field_BE) given the action kind (field_C0).
int CtrlNpcCanAdvanceAction(const cf::CtrlNpc* self) {
    int result = 0;
    u32 c0 = self->field_C0;
    if (c0 == 3) {
        if ((gmPauseFlags & 0x04000000) != 0) {
            if (self->field_BE == 2)
                result = 1;
        } else {
            if (self->field_BE == 2) {
                if (reinterpret_cast<cf::CObjectState*>(self->field_28)->checkStateFlags8(1) != 0 || reinterpret_cast<cf::CObjectState*>(self->field_28)->checkStateFlags(1) != 0)
                    result = 0;
                else
                    result = 1;
            }
        }
    } else if (c0 == 6) {
        if (self->field_BE == 4)
            result = 1;
    } else {
        result = 1;
    }
    return result;
}

// 0x800957CC: store the target position words and flags, then activate the
// character object if it is in one of the "in action" states (0x21..0x2A).
void CtrlNpcStoreTargetActivate(cf::CtrlNpc* self, const cf::CtrlNpcVec3W* vec) {
    self->field_14 = lbl_eu_80666698;
    self->field_C8 = vec->x;
    self->field_CC = vec->y;
    self->field_D0 = vec->z;
    self->field_BE = 1;
    self->field_174 = 0;
    cf::CfObjectMove* obj = self->field_28;
    if (NpcCharView(obj)->field_6C4 < 0x21)
        return;
    if (NpcCharView(obj)->field_6C4 > 0x2a)
        return;
    NpcCharView(obj)->field_6C4 = 1;
}

// 0x8009581C: reset the movement target to the character's current position
// and arm the action fields (kind 4) with the given id.
#undef lbl_eu_80666698
void func_80094E44(cf::CtrlNpc* self, u32 val) {
    self->field_14 = lbl_eu_80666698;
#define lbl_eu_80666698 sdata2_CtrlNpc.f0
    self->field_BE = 0;
    self->field_C0 = 4;
    self->field_16C = val;
    self->field_16E = val;
    self->field_170 = val;
    cf::CtrlNpcVec3W* pos = reinterpret_cast<cf::CtrlNpcVec3W*>(reinterpret_cast<cf::CfObject*>(self->field_28)->CfObject_getPosVector());
    u32 x = pos->x;
    u32 y = pos->y;
    self->field_B0.u = y;
    self->field_AC.u = x;
    self->field_B4.u = pos->z;
    self->field_174 = 0;
    NpcCharView(self->field_28)->field_6C4 = val;
}

// 0x800958B4: reset the movement target to the character's current position
// and arm the action fields (kind 5); the 0x16E/0x170 ids are clamped into
// [0, 0x68) (out-of-range ids become 0).
#undef lbl_eu_80666698
void CtrlNpcArmMoveTarget5(cf::CtrlNpc* self, int animMain, int animAlt1, int animAlt2) {
    self->field_14 = lbl_eu_80666698;
#define lbl_eu_80666698 sdata2_CtrlNpc.f0
    self->field_BE = 0;
    self->field_C0 = 5;
    self->field_16C = animMain;
    int ok5 = (animAlt1 >= 0) && !(animAlt1 >= 0x68);
    self->field_16E = ok5 ? animAlt1 : 0;
    int ok6 = (animAlt2 >= 0) && !(animAlt2 >= 0x68);
    self->field_170 = ok6 ? animAlt2 : 0;
    cf::CtrlNpcVec3W* pos = reinterpret_cast<cf::CtrlNpcVec3W*>(reinterpret_cast<cf::CfObject*>(self->field_28)->CfObject_getPosVector());
    u32 y = pos->y;
    u32 x = pos->x;
    self->field_B0.u = y;
    self->field_AC.u = x;
    self->field_B4.u = pos->z;
    self->field_174 = 0;
    s16 state = self->field_16C != 0 ? self->field_16C : self->field_16E;
    NpcCharView(self->field_28)->field_6C4 = state;
}

// 0x800959A0: record the flag byte, then activate the character object if it
// is in one of the "in action" states (0x21..0x2A).
void CtrlNpcSetFlagActivate(cf::CtrlNpc* self, u8 val) {
    self->field_C2 = val;
    cf::CfObjectMove* obj = self->field_28;
    if (NpcCharView(obj)->field_6C4 < 0x21)
        return;
    if (NpcCharView(obj)->field_6C4 > 0x2a)
        return;
    NpcCharView(obj)->field_6C4 = 1;
}

// TU-local view of the CfObjectModel float at +0xA4 read by
// CfObject_getProgressHigh (exact layout; no named member exists yet).
struct CtrlNpcModelA4View {
    u8 _pad[0xA4];   // 0x00-0xA3
    float field_A4;  // 0xA4-0xA7
};
float cf::CfObjectModel::CfObject_getProgressHigh() { return reinterpret_cast<CtrlNpcModelA4View*>(this)->field_A4; }

float cf::CfObjectModel::CfObject_getProgressLow() { return static_cast<cf::CfObjectModel*>(this)->field_A0; }

// 0x800959D8: when the character object's field_C4 flag is set, feed the
// halfword field_16C into the battle-status helper and bump the busy counter.
void CtrlNpcFireAnimMode16C(cf::CtrlNpc* self) {
    cf::CfObjectMove* obj = self->field_28;
    if (NpcCharView(obj)->field_C4 != 0) {
        CfObjectMove_setAnimModeArgs((u8*)obj, self->field_16C, 0, -1, 1);
        self->field_BE += 1;
    }
}

// 0x80095A34: when field_C4 is set, dispatch the battle-status helper with
// field_16C (or field_16E when it is zero); the zero branch bumps the busy
// counter one extra time.
void CtrlNpcFireAnimMode16CorE(cf::CtrlNpc* self) {
    cf::CfObjectMove* obj = self->field_28;
    if (NpcCharView(obj)->field_C4 != 0) {
        if (self->field_16C != 0) {
            CfObjectMove_setAnimModeArgs((u8*)obj, self->field_16C, 1, -1, 1);
        } else {
            CfObjectMove_setAnimModeArgs((u8*)obj, self->field_16E, 1, -1, 1);
            self->field_BE += 1;
        }
        self->field_BE += 1;
    }
}

// 0x80095AC0: advance the NPC action - when the character object's field_C4
// flag is set and it is idle (or the page helper says so), feed field_16E
// into the battle-status helper and bump the busy counter.
void CtrlNpcFireAnimModeIdleE(cf::CtrlNpc* self) {
    cf::CfObjectMove* obj = self->field_28;
    cf::CCtrlNpcC4Object* flag = NpcCharView(obj)->field_C4;
    if (flag != 0 && (reinterpret_cast<cf::CfObject*>(obj)->CfObject_queryTargetState() != 0 || getAnimModelId(flag) == 1)) {
        if (self->field_16E != 0) {
            int zero = (reinterpret_cast<cf::CfObject*>(self->field_28)->CfObject_getNpcTargetAction() == 0);
            CfObjectMove_setAnimModeArgs((u8*)self->field_28, self->field_16E, zero, -1, 1);
        }
        self->field_BE += 1;
    }
}

void CtrlNpcNoop() {}

// 0x80095B78: pull the movement target from the character object's position
// getter (vtable +0xac), record it, and either bump the busy counter or reset
// the movement flag.
void CtrlNpcSyncTargetFromPos(cf::CtrlNpc* self) {
    cf::CtrlNpcVec3W* pos = reinterpret_cast<cf::CtrlNpcVec3W*>(reinterpret_cast<cf::CfObject*>(self->field_28)->CfObject_getPosVector());
    u32 x = pos->x;
    self->field_CC = pos->y;
    self->field_C8 = x;
    self->field_D0 = pos->z;
    self->field_BC = 0;
    if (NpcCharView(self->field_28)->field_C4 != 0)
        self->field_BE += 1;
    else
        self->field_14 = lbl_eu_80666698;
}

// 0x80095BFC: turn the NPC toward its current movement target (or wander when
// close enough). The heading is stored into field_0C; when far away it aims at
// the target with a random jitter, otherwise it picks a random turn (continuing
// the previous turn while the wander timer field_BC is still running).
void CtrlNpcTurnToTarget(cf::CtrlNpc* self) {
    cf::CfObjectMove* obj = self->field_28;
    if (NpcCharView(obj)->field_C4 != 0 && reinterpret_cast<cf::CObjectState*>(obj)->checkStateFlags8(0x1000) != 0)
        return;
    const ml::CVec3* posf =
        reinterpret_cast<const ml::CVec3*>(reinterpret_cast<cf::CfObject*>(self->field_28)->CfObject_getPosVector());
    const ml::CVec3* tgt = reinterpret_cast<const ml::CVec3*>(&self->field_C8);
    // Inline VEC3Sub into a stack temp, then a component copy, like retail.
    ml::CVec3* posm = const_cast<ml::CVec3*>(posf);
    ml::CVec3 v = *tgt - *posm;
    f32 f1;
    // Far: aim at the target; close: wander.
    if (v.x * v.x + v.z * v.z >=
        0.8f * (self->field_D4 * self->field_D4)) {
        // Jittered aim; the angle stays in a callee-saved float across the
        // rand call, and the rand int promotes straight to double like
        // retail's __cvt_s2d sequence.
        f32 ang = 0.024543693f * nw4r::math::Atan2FIdx(v.x, v.z);
        f32 jitA = ml::pi * ml::math::mtRand(-30, 30);
        self->field_BA = 0x10;
        f32 jitB = 0.01f * jitA;
        jitB += ang;
        f1 = jitB;
    } else {
        // Continue the current turn while the timer runs.
        if (self->field_BC != 0) {
            // Retail evaluates the rand int-to-double conversion before the
            // _vD8() virtual call (the value lives in f31 across the call).
            double rnd = ml::math::mtRand(60);
            f32 base = 180.0f + reinterpret_cast<cf::CfObject*>(self->field_28)->CfObject_getScaledAngle();
            f1 = (base + rnd) * ml::deg2rad;
        } else {
            f1 = ml::math::mtRand(360) * ml::deg2rad;
        }
        self->field_BA = 0;
    }
    self->field_0C = f1;
    if (self->field_28 != 0) {
        reinterpret_cast<cf::CfObject*>(self->field_28)->CfObject_setMoveHeadAngle(f1);
        if (reinterpret_cast<cf::CfObject*>(self->field_28)->CfObject_isMoveActiveNow() != 0)
            CfObjectMove_setAnimModeArgs((u8*)self->field_28, 3, 0, -1, 1);
    }
    self->field_BE = 2;
    self->field_B8 = (s16)(ml::math::mtRand(128) + 64);
}

// 0x80095E28: per-frame movement/action update - check the distance to the
// target, tick the action timers, and advance the busy state.
void CtrlNpcUpdateMovement(cf::CtrlNpc* self) {
    // CVec3::sub lowers to the VEC3Sub paired-single kernel into an inner
    // temp plus component copy; retail keeps both stack regions alive.
    const ml::CVec3* posf =
        reinterpret_cast<const ml::CVec3*>(reinterpret_cast<cf::CfObject*>(self->field_28)->CfObject_getPosVector());
    ml::CVec3 diff =
        *reinterpret_cast<const ml::CVec3*>(&self->field_C8) - *posf;
    f32 rangeSq = self->field_D4 * self->field_D4;
    if (diff.x * diff.x + diff.z * diff.z > rangeSq) {
        if (self->field_BA < 0)
            self->field_B8 = 0;
    }
    if (NpcCharView(self->field_28)->field_C4 != 0 &&
        reinterpret_cast<cf::CObjectState*>(self->field_28)->checkStateFlags8(0x1000) != 0) {
        self->field_14 = lbl_eu_80666698;
        return;
    }
    if (self->field_BC > 0)
        self->field_BC -= 1;
    cf::CfObjectMove* obj = self->field_28;
    if (NpcCharView(obj)->field_C4 != 0) {
        if ((NpcCharView(obj)->field_C4->field_4EC & 0x02000000) != 0) {
            if (self->field_BC <= 0) {
                self->field_B8 = 0;
                self->field_BA = 0;
                self->field_BC = 15;
            } else {
                self->field_B8 -= 1;
                self->field_BA -= 1;
            }
        } else {
            self->field_B8 -= 1;
            self->field_BA -= 1;
        }
    } else {
        self->field_B8 -= 1;
        self->field_BA -= 1;
    }
    self->field_14 = reinterpret_cast<cf::CfObject*>(self->field_28)->CfObject_getMoveFactor();
    if (self->field_B8 <= 0) {
        // Retail loads field_DE once and reuses it for both the store and
        // the zero test.
        s16 de = self->field_DE;
        self->field_BA = de;
        self->field_B8 = 0;
        self->field_BE += 1;
        self->field_14 = lbl_eu_80666698;
        if (self->field_C0 == 1 || de == 0) {
            self->field_BA = 0;
            self->field_BE = 1;
        }
    }
}

struct CtrlNpcData {
    u8 _pad00[0xBA];
    short field_BA;
    u8 _padBC[2];
    short field_BE;
};

void CtrlNpcTickTimer(char* p) {
    CtrlNpcData* data = reinterpret_cast<CtrlNpcData*>(p);
    short v = data->field_BA - 1;
    data->field_BA = v;
    if (v <= 0) {
        data->field_BE = 1;
    }
}

// 0x80096034: movement-timer body (not yet recovered); the stub keeps the
// retail arg shape and C linkage (from the header declaration) so
// CtrlNpcRestartTimerGated's call reloc matches retail. noinline so the call stays a bl.
// Same-TU forward decl (defined below).
void CtrlNpcAdvanceWaypoint(cf::CtrlNpc* self);

// 0x8009565C: movement-timer restart. Validates the current event window
// ([field_160, field_164)), rebuilds waypoint segment lengths, converts the
// game-clock delta into normalized travel progress, then either snaps to the
// final waypoint or interpolates along the containing segment, steering /
// teleporting the character accordingly.
void __declspec(noinline) CtrlNpcRestartMoveTimer(cf::CtrlNpc* self) {
    if (self->field_15C == 0 || self->field_168 == 0)
        return;

    self->field_14 = lbl_eu_80666698;
    u16 first;
    u16 second;
    getControllerValues__Q22cf13CfGameManagerFv(&first, &second);
    clearControllerState__Q22cf13CfGameManagerFv();
    u32 tick = getFieldD90Value__Q22cf13CfGameManagerFv();

    if (second < self->field_160 || second >= self->field_164)
        return;

    // Implicit int->bool conversion; MWCC expands it branchlessly
    // (neg/or/srwi), matching retail.
    bool flag20 = gmPauseFlags & 0x20;
    cf::CfObjectMove* ch = self->field_28;
    if (!flag20 &&
        (((u32)tick & 3) != ((u32)reinterpret_cast<u32>(NpcCharView(ch)->field_74) & 3)))
        return;

    reinterpret_cast<cf::CObjectParam*>(ch)->CObjectParam_getParamPtr();

    // Accumulate XZ lengths between consecutive waypoints.
    f32 total = lbl_eu_80666698;
    f32 lens[8];
    for (int i = 0; i < self->field_15C - 1; i++) {
        const ml::CVec3* ta =
            reinterpret_cast<const ml::CVec3*>(&self->field_E0[i]);
        const ml::CVec3* tb =
            reinterpret_cast<const ml::CVec3*>(&self->field_E0[i + 1]);
        ml::CVec3 d = *tb - *ta;
        lens[i] = VecMath_LengthXZ(d);
        total += lens[i];
    }

    u16 carry = 0;
    if (self->field_DC != 0) {
        self->field_DC = 0;
        carry = first;
    } else if (self->field_C3 != 0) {
        self->field_C3 = 0;
        carry = first;
    }
    if (self->field_C2 != 0) {
        self->field_C2 = 0;
        self->field_C3 = 1;
    } else {
        self->field_C3 = 0;
    }

    s16 elapsed;
    if (flag20) {
        elapsed = (second - self->field_160) * 60;
        self->field_C3 = 0;
    } else {
        elapsed = (self->field_160 - second) * 60 + (first - carry);
    }
    self->field_16A = elapsed;

    self->field_BC = 0;
    self->field_C6 = (u16)(self->field_C6 & 0xFF00);

    // Travel progress through the event window, clamped to [0, 1].
    f32 ratio = (f32)((f64)(int)self->field_16A / (f64)(int)self->field_168);
    if (ratio > lbl_eu_806666A4)
        ratio = lbl_eu_806666A4;
    else if (ratio < lbl_eu_80666698)
        ratio = lbl_eu_80666698;

    // Minimum progress implied by the character's movement rate.
    f32 scale = *reinterpret_cast<const float*>(reinterpret_cast<cf::CfObject*>(ch)->CfObject_getMoveRateScale());
    f32 prog = reinterpret_cast<cf::CfObject*>(ch)->CfObject_getMoveFactor();
    f32 adj = 1000.0f *
              (f32)((f64)(int)self->field_16A * (prog * scale)) /
              3600.0f;
    if (adj > ml::epsilon)
        adj = adj / total;
    else
        adj = lbl_eu_80666698;
    if (ratio < adj)
        ratio = adj;

    self->field_BE += 1;
    self->field_D4 = lbl_eu_80666698;
    ml::CVec3 pt;
    pt.x = *reinterpret_cast<const f32*>(&self->field_E0[0].x);
    pt.y = *reinterpret_cast<const f32*>(&self->field_E0[0].y);
    pt.z = *reinterpret_cast<const f32*>(&self->field_E0[0].z);

    f32 heading = reinterpret_cast<cf::CfObject*>(ch)->CfObject_getMoveHeadAngle();
    flag20 = gmPauseFlags & 0x20;

    if (!(ratio >= lbl_eu_806666A4)) {
        // Interpolate along the segments to find the current position.
        s16 nextWp = 1;
        for (int j = 0; j < self->field_15C - 1; j++) {
            f32 frac = lens[j] / total;
            if (ratio <= frac) {
                f32 f;
                if (frac > ml::epsilon)
                    f = ratio / frac;
                else
                    f = lbl_eu_806666A4;
                const ml::CVec3* ta =
                    reinterpret_cast<const ml::CVec3*>(&self->field_E0[j]);
                const ml::CVec3* tb = reinterpret_cast<const ml::CVec3*>(
                    &self->field_E0[j + 1]);
                ml::CVec3 seg = *tb - *ta;
                ml::CVec3 off = seg * f;
                pt = *ta + off;
                heading =
                    0.024543693f * nw4r::math::Atan2FIdx(seg.x, seg.z);
                nextWp = j + 1;
                break;
            }
            ratio -= frac;
        }

        const ml::CVec3* pos =
            reinterpret_cast<const ml::CVec3*>(reinterpret_cast<cf::CfObject*>(ch)->CfObject_getPosVector());
        ml::CVec3 d = pt - *pos;
        f32 lenSq = d.x * d.x + d.z * d.z;
        bool far = lenSq > 9.0f ||
                   (f32)__fabs((f64)d.y) >= 5.9f;
        if (far) {
            // Teleport / reposition near the interpolated point.
            u32 mgrA = getControllerWordA33C__Q22cf13CfGameManagerFv();
            getControllerWordA37C__Q22cf13CfGameManagerFv();
            u16 evt = gmGlobalFlag42;
            if (evt == 1 && NpcCharView(ch)->field_8C == 0x91 && (u16)mgrA == 4) {
                if (flag20) {
                    pt.y = -0.21f;
                    reinterpret_cast<cf::CfObject*>(ch)->CfObject_syncMoveTarget(&pt);
                } else {
                    pt.y = pt.y + 2.0f;
                    reinterpret_cast<cf::CfObject*>(ch)->CfObject_applyMoveOffset(&pt, 1000.0f);
                }
            } else if (evt == 0xa && NpcCharView(ch)->field_8C == 0x2fa) {
                pt.y = pt.y + 2.0f;
                ml::CVec3 out = pt;
                VecMath_GroundProbeDual(&pt, &out, 0x4004A09, 100.0f,
                              lbl_eu_80666698);
                reinterpret_cast<cf::CfObject*>(ch)->CfObject_syncMoveTarget(&out);
            } else {
                reinterpret_cast<cf::CfObject*>(ch)->CfObject_applyMoveOffset(&pt, 1000.0f);
            }
            self->field_C4 = nextWp;
            if (flag20) {
                reinterpret_cast<cf::CfObject*>(ch)->CfObject_setMoveYaw(heading);
            } else {
                self->field_0C = heading;
                if (ch != 0) {
                    reinterpret_cast<cf::CfObject*>(ch)->CfObject_setMoveHeadAngle(self->field_0C);
                    if (reinterpret_cast<cf::CfObject*>(ch)->CfObject_isMoveActiveNow() != 0)
                        CfObjectMove_setAnimModeArgs((u8*)ch, 3, 0, -1, 1);
                }
            }
        } else {
            // Close enough: only apply the heading.
            if (flag20) {
                reinterpret_cast<cf::CfObject*>(ch)->CfObject_setMoveYaw(heading);
            } else {
                self->field_0C = heading;
                if (ch != 0) {
                    reinterpret_cast<cf::CfObject*>(ch)->CfObject_setMoveHeadAngle(self->field_0C);
                    if (reinterpret_cast<cf::CfObject*>(ch)->CfObject_isMoveActiveNow() != 0)
                        CfObjectMove_setAnimModeArgs((u8*)ch, 3, 0, -1, 1);
                }
            }
            self->field_C4 = nextWp;
        }
    } else {
        // Progress complete: snap to the final waypoint and finish the move.
        int nn = self->field_15C;
        ml::CVec3 last =
            *reinterpret_cast<ml::CVec3*>(&self->field_E0[nn - 1]);
        const ml::CVec3* prev =
            reinterpret_cast<const ml::CVec3*>(&self->field_E0[nn - 2]);
        ml::CVec3 hd = last - *prev;
        nw4r::math::Atan2FIdx(hd.x, hd.z);
        reinterpret_cast<cf::CfObject*>(ch)->CfObject_applyMoveOffset(&last, 50.0f);
        ColiSetAxisBlockInverse((u8*)ch + 0x60C,
                      reinterpret_cast<const float*>(reinterpret_cast<cf::CfObject*>(ch)->CfObject_getPosVector()));

        f32 f158 = self->field_158;
        if ((int)f158 != 0x168) {
            self->field_0C = f158 * ml::deg2rad;
            reinterpret_cast<cf::CfObject*>(ch)->CfObject_setMoveYaw(self->field_0C);
        }
        self->field_BE += 1;
        CtrlNpcAdvanceWaypoint(self);
        if (flag20)
            self->field_D4 = lbl_eu_80666698;
        self->field_BE = 2;
        self->field_14 = lbl_eu_80666698;
        return;
    }

    f32 p = reinterpret_cast<cf::CfObject*>(ch)->CfObject_getMoveFactor();
    self->field_D8 = p;
    if (p < 0.1f)
        self->field_D8 = 0.1f;
}

// 0x80095F44: per-frame movement update. Steers the NPC toward its current
// waypoint (field_E0[field_C4]); on arrival bumps the waypoint counter, and
// when the last waypoint is reached kicks the character's end-of-move virtuals
// and advances the busy state. The elapsed-frame count from the game manager
// scales the arrival radius; close-range slowdown blends the character's
// movement progress into the turn speed.
void CtrlNpcAdvanceWaypoint(cf::CtrlNpc* self) {
    u32 idx = self->field_C4;
    ml::CVec3* curTgt =
        reinterpret_cast<ml::CVec3*>(&self->field_E0[idx]);
    const ml::CVec3* posf =
        reinterpret_cast<const ml::CVec3*>(reinterpret_cast<cf::CfObject*>(self->field_28)->CfObject_getPosVector());
    // Kernel diff + component copy (same shape as CtrlNpcUpdateTalk).
    ml::CVec3 diff = *curTgt - *posf;
    f32 distSq = diff.x * diff.x + diff.z * diff.z;

    cf::CfObjectMove* obj = self->field_28;
    if (NpcCharView(obj)->field_C4 != 0 && reinterpret_cast<cf::CObjectState*>(obj)->checkStateFlags8(0x1000) != 0) {
        self->field_14 = lbl_eu_80666698;
        self->field_C6 = (u16)((self->field_C6 & 0xFF00) | 1);
        return;
    }

    u16 c6 = self->field_C6;
    if ((c6 & 0xFF) != 0) {
        self->field_14 = self->field_D8;
        self->field_C6 = (u16)(c6 & 0xFF00);
    }

    u32 frames = clearControllerState__Q22cf13CfGameManagerFv();
    f32 cap = (f32)(f64)frames;
    if (!(cap > 8.0f)) {
        cap = (f32)(f64)frames;
    }

    // Base speed: shorter stride on the final leg.
    f32 speed = (self->field_15C - 1 == idx) ? 0.0121f
                                             : 0.0625f;
    if (self->field_D8 >= lbl_eu_806666A4)
        speed *= 2.0f;

    // While a forced-wait flag is set on the C4 status object and few
    // waypoints remain, creep forward until within the small threshold.
    cf::CCtrlNpcC4Object* flag = NpcCharView(self->field_28)->field_C4;
    if (flag != 0) {
        if ((flag->field_4EC & 0x02000000) != 0 && self->field_15C <= 2) {
            f32 mag = PSVECMag(
                reinterpret_cast<const Vec*>(CfObjectMove_getSubObj54(self->field_28)));
            s16 bc = self->field_BC;
            if (bc >= 0x1e || mag >= 0.1f) {
                distSq = lbl_eu_80666698;
            } else {
                self->field_BC = bc + 1;
            }
        } else {
            self->field_BC = 0;
        }
    }

    f32 threshold = cap * (speed * (f32)(f64)frames);
    if (!(distSq <= threshold) &&
        !((f32)__fabs((f64)(threshold - distSq)) < 0.3f)) {
        // Not arrived yet.
        f32 prog = reinterpret_cast<cf::CfObject*>(self->field_28)->CfObject_getMoveFactor();
        self->field_D8 = prog;
        if (self->field_15C - 1 != idx) {
            f32 f30 = 0.3f * reinterpret_cast<cf::CfObject*>(self->field_28)->CfObject_getMoveFactor();
            if (!(f30 >= 0.1f))
                f30 = 0.1f;
            if (reinterpret_cast<cf::CfObject*>(self->field_28)->CfObject_getMoveFactor() >= lbl_eu_806666A4) {
                f30 = 0.09f * reinterpret_cast<cf::CfObject*>(self->field_28)->CfObject_getMoveFactor();
                if (!(f30 >= 0.1f))
                    f30 = 0.1f;
                speed = 2.0f;
            }
            if (distSq < speed * speed) {
                f32 ratio =
                    (speed - (f32)VecMath_SafeSqrtF(distSq)) / speed;
                f32 newProg = lbl_eu_806666A4 - ratio;
                f32 prog2 = reinterpret_cast<cf::CfObject*>(self->field_28)->CfObject_getMoveFactor();
                self->field_D8 = (prog2 - f30) * newProg + f30;
            }
        }
    } else {
        // Arrived at this waypoint.
        u16 next = self->field_C4 + 1;
        self->field_C4 = next;
        if (next >= self->field_15C) {
            self->field_14 = lbl_eu_80666698;
            cf::CfObjectMove* obj2 = self->field_28;
            reinterpret_cast<cf::CfObject*>(obj2)->CfObject_snapMoveTarget(curTgt, 50.0f);
            reinterpret_cast<cf::CfObject*>(obj2)->CfObject_getScaledAngle();
            f32 f158 = self->field_158;
            if ((int)f158 != 0x168) {
                self->field_0C = f158 * ml::deg2rad;
                if (obj2 != 0) {
                    reinterpret_cast<cf::CfObject*>(obj2)->CfObject_setMoveHeadAngle(self->field_0C);
                    if (reinterpret_cast<cf::CfObject*>(obj2)->CfObject_isMoveActiveNow() != 0)
                        CfObjectMove_setAnimModeArgs((u8*)obj2, 3, 0, -1, 1);
                }
            }
            if ((gmPauseFlags & 0x20) != 0)
                self->field_D4 = lbl_eu_80666698;
            else
                self->field_D4 = 5.0f;
            self->field_BE += 1;
            return;
        }
        // More waypoints: recompute the diff against the next one for the
        // heading in the tail below. Copy-assignment (not re-init) makes
        // MWCC emit kernel + return-temp copy + assign-copy, matching the
        // three retail stack regions.
        const ml::CVec3* pos2 = reinterpret_cast<const ml::CVec3*>(
            reinterpret_cast<cf::CfObject*>(self->field_28)->CfObject_getPosVector());
        ml::CVec3* tgt2 = reinterpret_cast<ml::CVec3*>(
            &self->field_E0[self->field_C4]);
        diff = *tgt2 - *pos2;
    }

    // Common tail: face along the current diff, advance by speed*dt, and
    // snapshot the character position into the raw target words.
    f32 ang = nw4r::math::Atan2FIdx(diff.x, diff.z);
    self->field_0C = 0.024543693f * ang;
    self->field_14 = self->field_D8 *
                     (f32)(f64)frames;
    cf::CtrlNpcVec3W* posw = reinterpret_cast<cf::CtrlNpcVec3W*>(reinterpret_cast<cf::CfObject*>(self->field_28)->CfObject_getPosVector());
    u32 px = posw->x;
    u32 py = posw->y;
    self->field_B0.u = py;
    self->field_AC.u = px;
    self->field_B4.u = posw->z;
}

// 0x80096E60: query the game manager's current id halfword; when it falls
// outside [field_160, field_164), clear the busy flag and restart the
// movement timer.
void CtrlNpcRestartTimerGated(cf::CtrlNpc* self) {
    u16 id;
    u16 dummy;
    getControllerValues__Q22cf13CfGameManagerFv(&id, &dummy);
    clearControllerState__Q22cf13CfGameManagerFv();
    if (id < self->field_160 || id >= self->field_164) {
        self->field_BE = 0;
        CtrlNpcRestartMoveTimer(self);
    }
}

// 0x80096EC4: movement-approach update. When the horizontal distance to the
// movement target is large enough, steer the heading toward it and drive the
// move helper; otherwise decay the remaining-duration timer and advance the
// action state.
int CtrlNpcApproachTarget(cf::CtrlNpc* self) {
    ml::CVec3* posf = reinterpret_cast<ml::CVec3*>(reinterpret_cast<cf::CfObject*>(self->field_28)->CfObject_getPosVector());
    ml::CVec3* tgt = reinterpret_cast<ml::CVec3*>(&self->field_E0[1]);
    // operator- emits the paired-single diff kernel matching retail.
    ml::CVec3 diff = *tgt - *posf;
    ml::CVec3& v = diff;
    f32 len2 = v.x * v.x + v.z * v.z;
    if (len2 > 0.040000003f) {
        f32 target = self->field_D8;
        if (len2 < 4.0f) {
            // nw4r FSqrt: warn on negative input, else sqrt via FrSqrt.
            if (!(len2 >= lbl_eu_80666698)) {
                nw4r::db::Warning(lbl_eu_80526324, 0x273,
                                  lbl_eu_80526300);
            }
            f32 len = (len2 <= lbl_eu_80666698)
                          ? lbl_eu_80666698
                          : len2 * nw4r::math::FrSqrt(len2);
            f32 scaled = len - 0.2f;
            target = (self->field_D8 - 0.1f) *
                         (scaled * 0.5f) +
                     0.1f;
        }
        // Retail computes the angle first so 'target' stays live in a
        // callee-saved float across the Atan2FIdx call.
        // Leading scoped temp forces right-to-left arg evaluation (z first),
        // matching retail's load order.
        f32 vz = v.z;
        f32 idx = nw4r::math::Atan2FIdx(v.x, vz);
        self->field_14 = target;
        self->field_0C = 0.024543693f * idx;
        if (VecMath_CircleOverlapXZ(posf, tgt,
                          self->field_D8, 0.2f) == 0)
            return 1;
    }
    self->field_14 = lbl_eu_80666698;
    self->field_D4 = lbl_eu_80666698;
    if (self->field_158 < 360.0f) {
        self->field_0C = self->field_158 * ml::deg2rad;
        if (self->field_28 != 0) {
            reinterpret_cast<cf::CfObject*>(self->field_28)->CfObject_setMoveHeadAngle(self->field_0C);
            if (reinterpret_cast<cf::CfObject*>(self->field_28)->CfObject_isMoveActiveNow() != 0)
                CfObjectMove_setAnimModeArgs((u8*)self->field_28, 3, 0, -1, 1);
        }
    }
    return 0;
}

// 0x800970C0: per-frame NPC action update. Dispatches on the busy counter
// (field_BE): state 1 arms the movement/character action (depending on the
// packed action byte in field_C6), state 2 waits for the character to become
// idle, state 3 counts the movement timer down to 0.
#undef lbl_eu_80666698
#undef lbl_eu_80666700
#undef lbl_eu_80666714
void CtrlNpcAdvanceBehavior(cf::CtrlNpc* self) {
    if (self->field_BE == 1) {
        int armed;
        if (((self->field_C6 >> 8) & 0xFF) != 0)
            armed = CtrlNpcApproachTarget(self);
        else
            armed = NpcMove_DispatchState((cf::CCtrlMoveNpc*)self->_sub30);
        if (armed != 0) {
            self->field_BE = 1;
        } else if (self->field_158 < lbl_eu_80666714) {
            self->field_BE = 2;
        } else {
            self->field_D4 = lbl_eu_80666700;
            self->field_BE = 3;
        }
    } else if (self->field_BE == 2) {
        if (NpcCharView(self->field_28)->field_C4 == 0 ||
            reinterpret_cast<cf::CObjectState*>(self->field_28)->checkStateFlags8(0x1000) == 0) {
            self->field_D4 = lbl_eu_80666700;
            self->field_BE = 3;
        }
    } else if (self->field_BE == 3) {
        // CfT_PlayRateGet's float return is declared in CfTFile.hpp.
        f32 v = self->field_D4 - CfT_PlayRateGet();
        self->field_D4 = v;
        if (v <= lbl_eu_80666698) {
            self->field_D4 = lbl_eu_80666698;
            self->field_BE = 4;
        }
    }
}
#define lbl_eu_80666698 sdata2_CtrlNpc.f0
#define lbl_eu_80666700 sdata2_CtrlNpc.f22
#define lbl_eu_80666714 sdata2_CtrlNpc.f27

// 0x800971D0: returns whether the NPC is active - either the "busy" flag is
// set, or the character object's state query (vtable +0x24) reports active.
int CtrlNpcIsActive(cf::CtrlNpc* self) {
    if (self->field_174 != 0)
        goto ret1;
    if (reinterpret_cast<cf::CObjectState*>(self->field_28)->checkStateFlags8(0x1000) == 0)
        goto ret0;
ret1:
    return 1;
ret0:
    return 0;
}

extern "C" int CtrlNpcAlwaysTrue(u8* self) { return 1; }

extern "C" int CtrlNpcAlwaysFalse(u8* self) { return 0; }

cf::CtrlNpc::~CtrlNpc() {}

// absorb: split1 retail data sections - generated (pointer tables via relocs)
__declspec(section ".data") const char* gNpcAutotalkColumnNames[6] = {
    "autotalk1", "autotalk2", "autotalk3",
    "autotalk4", "autotalk5", "autotalk6"
};
__declspec(section ".data") const char* gNpcTimeColumnNames[8] = {
    "OCLOCK_0", "OCLOCK_3", "OCLOCK_6", "OCLOCK_9",
    "OCLOCK_12", "OCLOCK_15", "OCLOCK_18", "OCLOCK_21"
};
__declspec(section ".data") const char* gNpcTimeColumnNamesAlt[8] = {
    "OCLOCK_0", "OCLOCK_3", "OCLOCK_6", "OCLOCK_9",
    "OCLOCK_12", "OCLOCK_15", "OCLOCK_18", "OCLOCK_21"
};
// handler tables: member function pointers (0, -1, func)
__declspec(section ".data") const void* gNpcAnimMode16Handlers[] = {
    (void*)0, (void*)-1, (void*)CtrlNpcFireAnimMode16C,
    (void*)0, (void*)-1, (void*)CtrlNpcNoop
};
__declspec(section ".data") const void* gNpcAnimModeHandlers[] = {
    (void*)0, (void*)-1, (void*)CtrlNpcFireAnimMode16CorE,
    (void*)0, (void*)-1, (void*)CtrlNpcFireAnimModeIdleE,
    (void*)0, (void*)-1, (void*)CtrlNpcNoop,
    (void*)0
};
__declspec(section ".data") const void* gNpcActionState1Handlers[] = {
    (void*)0, (void*)-1, (void*)CtrlNpcSyncTargetFromPos,
    (void*)0, (void*)-1, (void*)CtrlNpcTurnToTarget,
    (void*)0, (void*)-1, (void*)CtrlNpcUpdateMovement,
    (void*)0, (void*)-1, (void*)CtrlNpcTickTimer
};
__declspec(section ".data") const void* gNpcActionState2Handlers[] = {
    (void*)0, (void*)-1, (void*)CtrlNpcRestartMoveTimer,
    (void*)0, (void*)-1, (void*)CtrlNpcAdvanceWaypoint,
    (void*)0, (void*)-1, (void*)CtrlNpcRestartTimerGated,
    (void*)0
};
// (typed .rodata/.sdata2 live at the top of this file)

