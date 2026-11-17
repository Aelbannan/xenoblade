#include "kyoshin/cf/CBattleManagerApi.hpp"
#include "kyoshin/cf/object/CfObjectActor.hpp"
#include "kyoshin/code_802B8A3C.hpp"  // findObjectById imports
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

// LOCAL views of the battle-status helpers for this TU: the constructor call
// site uses the 5-arg table ABI of dispatchItemLoad while other TUs use the
// common 2-arg form (the two extern "C" signatures cannot coexist in one TU -
// MWCC error 10197), so both helpers are declared here instead of including
// kyoshin/cf/CfMapItemManager.hpp.
extern "C" s32 testActorStatus(void* obj, void* outFlags, u32 id);
extern "C" void dispatchItemLoad(void* actor, u32 flags, const void* a, const void* b, const void* c);
// CActorParam::CActorParam_UnkVirtualFunc6(int) is declared in CActorParam.hpp.

// Pool anchor FIRST in the TU so MWCC emits the .sdata2 consts below in
// retail declaration order (emission follows first use; code uses alone
// would hoist lbl_eu_8066776C second and permute the doubles).
extern "C" void capdatatouch_CfObjectActor(void) {
    // MWCC drops unreferenced const scalars, so volatile-read each label
    // in retail order (plain reads fold away). lbl_eu_80667748 (s16->float
    // magic) and the 67770/67774/67778 tail have no code uses.
    volatile float sinkf;
    sinkf = *(volatile float*)&lbl_eu_80667738;
    sinkf = *(volatile float*)&lbl_eu_8066773C;
    sinkf = *(volatile float*)&lbl_eu_80667740;
    volatile double sinkd;
    sinkd = *(volatile double*)&lbl_eu_80667748;
    sinkd = *(volatile double*)&lbl_eu_80667750;
    sinkd = *(volatile double*)&lbl_eu_80667758;
    sinkf = *(volatile float*)&lbl_eu_80667760;
    sinkf = *(volatile float*)&lbl_eu_80667764;
    sinkf = *(volatile float*)&lbl_eu_80667768;
    sinkf = *(volatile float*)&lbl_eu_8066776C;
    (void)sinkf;
    (void)sinkd;
}

namespace cf {
    /* TODO: find out what base class the static cast is
    casting down to */
    CfObjectActor::CfObjectActor() :
    // Retail passes (this ? this+0x3E9C : this, 0) to the base ctor.
    CActorParam(this != 0 ? reinterpret_cast<UNKTYPE*>(reinterpret_cast<u8*>(this) + 0x3E9C)
                          : reinterpret_cast<UNKTYPE*>(this),
                nullptr) {
        // Copy the four vtable pointers from the shared table group, hand the
        // same group to the battle-side initializer, then link the AI-action
        // subobject back to this actor and seed the tail fields.
        u8* grp = (u8*)__vt__Q22cf13CfObjectActor;
        *(void**)this = (void*)grp;
        *(void**)((u8*)this + 0x8) = (void*)(grp + 0xC);
        *(void**)((u8*)this + 0x3380) = (void*)(grp + 0x36C);
        *(void**)((u8*)this + 0x3E9C) = (void*)(grp + 0x37C);
        dispatchItemLoad(this, 3, grp + 0x36C, grp + 0xC, grp);
        aiActionStoreWordB14(reinterpret_cast<u8*>(this) + 0x3380, reinterpret_cast<unsigned int>(this));
        CfActorField45B8* tail = reinterpret_cast<CfActorField45B8*>(this);
        tail->field_0x45B8 = 0;
        tail->field_0x45BC = -1;
    }

float CfObjectActor::CfObjectActor_readFacingAngle() {
    // Float field at absolute offset 0x3EE8 within CfObjectActor
    // (0x4C within the CfObjectMove subobject at 0x3E9C)
    return *(float*)(reinterpret_cast<uintptr_t>(this) + 0x3EE8);
}
}

// Forward declarations for thunks (extern "C" so MWCC emits the retail
// unmangled symbol names exactly as written - no extra __FPv suffix).
extern "C" void __dt__Q22cf13CfObjectActorFv(void* self);
extern "C" const char* CObjectParam_UnkVirtualFunc2__Q22cf12CfObjectMoveFv(void* self);

// Retail symbol is Fv; the real ABI passes (self, f1, r4). Delegates to the
// CActorParam_applyDamage vtable slot (+0x120, takes (float, int, int,
// int)) with the float arg = value itself, passed through unchanged.
extern "C" void CfObjectActor_takeDamageValue__Q22cf13CfObjectActorFv(cf::CfObjectActor* self, float value, int arg) {
    if (value <= lbl_eu_80667738) {
        self->CActorParam_applyDamage(value, 0, 0, arg);
    } else {
        self->CActorParam_applyDamage(value, 3, 0, arg);
    }
}

float cf::CfObjectActor::CfObjectActor_readActionScale() {
    // Word at absolute offset 0x3E74 (CAIAction trailer area) is used as a
    // pointer to an object holding a float at +0x7C.
    cf::CfFloat7C* p = reinterpret_cast<cf::CfFloat7C*>(reinterpret_cast<cf::CfActorField3E74*>(this)->field_0x3E74);
    if (p == 0) return lbl_eu_8066776C;
    return p->field_0x7C;
}
struct CfObjectActorData {
    u8 _pad00[0x3f60];
    void* mSomePtr;     // 0x3f60
    u8 _pad3f64[0x658];
    void* mOtherPtr;    // 0x45bc
};

struct CfObjectActorInner {
    u8 _pad00[0x37c];
    void* mArgStorage;  // 0x37c
};

// Retail symbol is Fv; the real ABI passes (self, arg). Forced-name extern
// "C" wrapper (same pattern as CfObjectActor_pushRefreshValue): the class
// keeps the virtual decl for the +0x5C8 slot, this definition provides the
// retail symbol. Stores the arg at +0x45BC and, when the +0x3F60 target is
// set, into its +0x37C word.
extern "C" void CfObjectActor_storeActionSrcId__Q22cf13CfObjectActorFv(cf::CfObjectActor* self, void* arg) {
    CfObjectActorData* data = reinterpret_cast<CfObjectActorData*>(self);
    void* p = data->mSomePtr;
    if (p != 0) {
        static_cast<CfObjectActorInner*>(p)->mArgStorage = arg;
    }
    data->mOtherPtr = arg;
}

// Retail defs owned by this TU's split range (0x80171310/0x80171EAC); the
// bodies are CVision logic awaiting that wave's decomp. extern "C" + real
// arity per CVision.hpp so this TU provides the retail symbols (C++
// linkage would mangle them __Fv and leave CVision.o's refs dangling).
extern "C" void Vision_CopyEngageActor(void* obj, void* dst) { (void)obj; (void)dst; }
extern "C" void Vision_CopyFxActor(void* self, void* dst) { (void)self; (void)dst; }
void cf::CfObjectActor::CfObjectActor_clearStatusPair() {
    // Two virtual calls on the CBattleState subobject vtable (this+0x8),
    // slot +0x20 (CBattleState_clearStatusId; retail fake-Fv ABI passes
    // the status id in r4). Calling through `this` directly lets MWCC keep
    // `this` in r31 and recompute the +8 adjusted-this per call (retail
    // shape); the reinterpret_cast form CSEs this+8 into r31.
    this->CBattleState_clearStatusId(0xf);
    this->CBattleState_clearStatusId(0x10);
}
void cf::CfObjectActor::CfObjectActor_flushStatusPair() {
    // Two calls to the CBattleState subobject vtable slot +0x20 (retail
    // passes an int through the slot; fake-Fv ABI). Same shape as
    // CfObjectActor_clearStatusPair: calling through `this` keeps this in
    // r31 and recomputes the +8 adjusted-this per call.
    this->CBattleState_clearStatusId(0xf);
    this->CBattleState_clearStatusId(0x10);
}
float cf::CfObjectActor::CfObjectActor_getAdjustedFacing() {
    // Base height at 0x3EE8; if the move target (0x3F60) is set, add the
    // signed short value the CActParamData helper returns (s16 -> float via
    // MWCC's 2^52 double-trick, which emits the retail fsubs sequence).
    u8* p = reinterpret_cast<cf::CfActorField3F60*>(this)->field_0x3F60;
    if (p != 0) {
        float base = reinterpret_cast<cf::CfActorField3EE8*>(this)->field_0x3EE8;
        return base + (float)(s16)getParamSelS16(p + 16, 0);
    }
    return reinterpret_cast<cf::CfActorField3EE8*>(this)->field_0x3EE8;
}

// Retail symbol is Fv; the real ABI passes (self, arg) and returns whether
// both actors share the CfObjectMove+0x64 flag-bit (2 or 4).
extern "C" int CfObjectActor_sharesMoveFlags__Q22cf13CfObjectActorFv(cf::CfObjectActor* self, cf::CfObjectActor* arg) {
    if (arg == 0) return 0;
    if (arg == self) return 0;
    u32 selfFlags = reinterpret_cast<cf::CfActorField3F00*>(self)->field_0x3F00;
    if ((selfFlags & 2) != 0 && (reinterpret_cast<cf::CfActorField3F00*>(arg)->field_0x3F00 & 2) != 0) return 1;
    if ((selfFlags & 4) != 0 && (reinterpret_cast<cf::CfActorField3F00*>(arg)->field_0x3F00 & 4) != 0) return 1;
    return 0;
}

// Retail symbol is Fv; the real ABI passes a float in f1. Forwards the move
// to the CfObjectMove subobject, then re-dispatches this same virtual (slot
// +0x5C4) on the actor behind the action-source handle when its +0x64 flags
// select it.
extern "C" void CfObject_pushRefreshValue__Q22cf12CfObjectMoveFf(void* self, float value);

extern "C" void CfObjectActor_pushRefreshValue__Q22cf13CfObjectActorFv(cf::CfObjectActor* self, float value) {
    // Forward the move to the CfObjectMove subobject (+0x3E9C), then
    // re-dispatch this same virtual (slot +0x5C4) on the actor behind the
    // action-source handle when its +0x64 flags select it. The move forward is
    // a DIRECT call to the CfObjectMove non-virtual (the retail bl; the
    // member call would virtual-dispatch through the base's 0x88 slot).
    cf::CfObjectMove* move = (cf::CfObjectMove*)((u8*)self + 16028);  // +0x3E9C
    CfObject_pushRefreshValue__Q22cf12CfObjectMoveFf(move, value);
    u8* p = reinterpret_cast<cf::CfActorField45B8*>(self)->field_0x45B8;
    if (p != 0) {
        cf::CfObjectMove* src = static_cast<cf::CfObjectMove*>(findObjectById(reinterpret_cast<int>(p)));
        // Merged condition: both false paths share one zeroing block. The
        // true arm re-tests src (retail re-checks it before the guarded
        // subi) and does an explicit -0x3E9C downcast (the C++ layout puts
        // CfObjectMove later, so static_cast would emit subi 16080). The
        // re-dispatch is VIRTUAL through slot +0x5C4 (retail bcctrl with
        // f1 carried through); a direct call lets MWCC auto-inline and
        // duplicate the body.
        cf::CfObjectActor* actor;
        if (src != 0 &&
            ((reinterpret_cast<cf::CfMoveFlags64*>(src)->field_0x64 & 2) != 0 ||
             (reinterpret_cast<cf::CfMoveFlags64*>(src)->field_0x64 & 4) != 0)) {
            actor = src != 0 ? (cf::CfObjectActor*)((u8*)src - 0x3E9C) : 0;
        } else {
            actor = 0;
        }
        if (actor != 0) {
            actor->CfObjectActor_pushRefreshValue(value);
        }
    }
}


extern "C" float CActorParam_getScale__Q22cf13CfObjectActorFv(cf::CfObjectActor* self) {
    return static_cast<cf::CfObject*>((cf::CfObjectMove*)((u8*)self + 0x3e9c))->CfObject_getObjScale();
}

// CActorParam_setScale: store float then adjust-tail-call vt+0xDC
extern "C" void CActorParam_setScale__Q22cf13CfObjectActorFv(cf::CfObjectActor* self, float v) {
    *(float*)((u8*)self + 0x15e8) = v;
    static_cast<cf::CfObject*>((cf::CfObjectMove*)((u8*)self + 0x3e9c))->CfObject_setObjScale(v);
}


// Retail Fv ABI is (self, arg, f1, f2, f3) - no int. The lwz of field_0x3F10
// lands in r5; that live r5 is the hidden int the base Unk140 consumes.
// Declaring int here reserved r5 and forced the load into r6 (75% reg_swap).
extern "C" void CActorParam_addHate__Q22cf13CfObjectActorFv(
    cf::CfObjectActor* self, cf::CfObjectActor* arg, float a, float b, float c) {
    if (arg == 0) return;
    u32 selfId = reinterpret_cast<cf::CfActorField3F10*>(self)->field_0x3F10;
    if (selfId == reinterpret_cast<cf::CfActorField3F10*>(arg)->field_0x3F10) return;
    // Non-virtual call of the CActorParam base (retail: tail-branch).
    self->cf::CActorParam::CActorParam_addHate(arg, a, b, c);
}
// cf::CfObjectActor vtable thunks (retail: this-adjust + tail-branch)
extern "C" const char* CActorParam_UnkVirtualFunc1__Q22cf13CfObjectActorFv(void* self) {
    return (const char*)CObjectParam_UnkVirtualFunc2__Q22cf12CfObjectMoveFv((char*)self + 0x3e9c);
}
extern "C" void destroyActorParam__Q22cf13CfObjectActorFv(void* self) {
    __dt__Q22cf13CfObjectActorFv((char*)self - 0x3e9c);
}
extern "C" void CActorParam_UnkVirtualFunc167__Q22cf11CActorParamFv() {}
// Retail symbol is Fv; the real ABI passes (self, arg). Calls the base
// CActorParam implementation, then dispatches on the arg's u16 id at +0xC:
// 0xE5 -> forward the threshold to the CfObjectMove subobject vf70 slot;
// 0x10 -> (flag-gated) forward the 0x1634 word through the CBattleState
// subobject slot +0x20. Always ends by calling the +0x3ED4 sub-object's
// vtable slot +0x8C with the arg.
extern "C" void CActorParam_UnkVirtualFunc180__Q22cf13CfObjectActorFv(cf::CfObjectActor* self, void* arg) {
    CActorParam_UnkVirtualFunc180__Q22cf11CActorParamFv(self, arg);
    int v = reinterpret_cast<cf::CfActorParamArg*>(arg)->field_0xC;
    switch (v) {
    case 0xE5:
        ((cf::CfObjectMove*)((u8*)self + 0x3E9C))->CfObject_syncModelRate(lbl_eu_80667738);
        break;
    case 0x10: {
        cf::CfActorParamFields* f = reinterpret_cast<cf::CfActorParamFields*>(self);
        if ((f->field_0x3374 & 0x40000) != 0 && f->field_0x1634 != 0) {
            static_cast<cf::CBattleState*>((cf::CBattleState*)((u8*)self + 8))->CBattleState_clearStatusId((int)f->field_0x1634);
        }
        break;
    }
    default:
        break;
    }
    u8* obj3ED4 = reinterpret_cast<cf::CfActorField3ED4*>(self)->field_0x3ED4;
    reinterpret_cast<cf::CfObjectSub38*>(obj3ED4)->forwardActorEventEnd(arg);
}
// Retail symbol is Fv; the real ABI passes a float in f1. Rounds the input
// to the nearest int (fctiwz roundtrip), then - unless the presentation or
// mode flags are set - clamps the value up to lbl_eu_80667740 when the
// battle manager's +0x1A8 sub-object reports the actor, stores it at 0x17E8
// and releases a vision slot.
extern "C" void CActorParam_setHp__Q22cf13CfObjectActorFv(cf::CfObjectActor* self, float val) {
    // The direct cast makes MWCC synthesize the shared sdata2 magic double
    // (lbl_eu_80667748); the decomp pools an @N entry of the same value at
    // the same reloc site (name-only drift - accepted, cf. getAdjustedFacing).
    float f = (float)(int)val;
    if ((lbl_eu_80663E24 & 0x10000000) == 0 && (lbl_eu_80663E28 & 0x800) == 0) {
        if (f < lbl_eu_80667740) {
            void* bm = getInstance__Q22cf14CBattleManagerFv();
            if (CChain_ProbeActorReady((u8*)bm + 0x1A8, self) != 0) {
                f = lbl_eu_80667740;
            }
        }
        reinterpret_cast<cf::CfActorParamFields*>(self)->field_0x17E8 = f;
        releaseVisionSlot(self, 0);
    }
}
// Retail symbol is Fv; the real ABI passes (self, delta). Adds a signed
// delta to the gauge at 0x160C, clamping to [0, 0x160E]. Then, when the
// actor-id probe (testActorStatus, flag 0x802) succeeds and the gauge sits at
// its max (vf174 == vf178), decrements it by one via the vf168 slot.
extern "C" void CActorParam_addGauge__Q22cf13CfObjectActorFv(cf::CfObjectActor* self, int delta) {
    cf::CfActorParamFields* f = reinterpret_cast<cf::CfActorParamFields*>(self);
    s16 cur = f->field_0x160C;
    s16 max = f->field_0x160E;
    // Explicit (s16) truncation of the int param -> retail's extsh r0, r4
    // (an s16-typed param lets MWCC fold the sign-extension into the add).
    int sum = cur + (s16)delta;
    f->field_0x160C = (u16)sum;
    if ((s16)sum < 0) {
        f->field_0x160C = 0;
    } else if ((s16)sum > max) {
        f->field_0x160C = (u16)max;
    }
    u32* idPtr = reinterpret_cast<u32*>(
        reinterpret_cast<cf::CObjectState*>(
            reinterpret_cast<cf::CActorState*>(self)->unk4)
            ->CObjectState_getStateData());
    u32 id = *idPtr;
    if (testActorStatus(self, (int*)&id, 0x802) != 0) {
        // MWCC evaluates == right-to-left: retail calls 0x178 first, 0x174 second.
        if (self->CActorParam_getGauge() == self->CActorParam_getGaugeMax()) {
            int mx = self->CActorParam_getGaugeMax();
            self->CActorParam_setGauge(mx - 1);
        }
    }
}
// Retail symbol is Fv; the real ABI passes (self, arg). Calls the base
// CActorParam implementation, then dispatches on the arg's u16 id at +0xC:
// 0xE5 -> forward the threshold to the CfObjectMove subobject vf70 slot;
// 0x10 -> (flag-gated) build a 0x34-byte descriptor from the 0x1634-0x1644
// words and pass it through the CBattleState subobject slot +0x18. Always
// ends by calling the +0x3ED4 sub-object's vtable slot +0x88 with the arg.
extern "C" void CActorParam_UnkVirtualFunc179__Q22cf13CfObjectActorFv(cf::CfObjectActor* self, void* arg) {
    CActorParam_UnkVirtualFunc179__Q22cf11CActorParamFv(self, arg);
    switch (reinterpret_cast<cf::CfActorParamArg*>(arg)->field_0xC) {
    case 0xE5:
        ((cf::CfObjectMove*)((u8*)self + 0x3E9C))->CfObject_syncModelRate(lbl_eu_8066773C);
        break;
    case 0x10: {
        cf::CfActorParamFields* f = reinterpret_cast<cf::CfActorParamFields*>(self);
        if ((f->field_0x3374 & 0x40000) != 0 && f->field_0x1634 != 0) {
            cf::CfActor179Buf buf;
            std::memset(&buf, 0, 0x34);
            buf.field_0x00 = reinterpret_cast<cf::CfActorField3F10*>(self)->field_0x3F10;
            buf.field_0x04 = 0;
            buf.field_0x0C = (u16)f->field_0x1634;
            buf.field_0x10 = f->field_0x163C;
            buf.field_0x14 = (u16)f->field_0x1638;
            buf.field_0x16 = (u16)f->field_0x1640;
            buf.field_0x18 = (u16)f->field_0x1644;
            buf.field_0x20 = lbl_eu_80667738;
            buf.field_0x30 = 0x10001;
            static_cast<cf::CBattleState*>((cf::CBattleState*)((u8*)self + 8))->CBattleState_enterStatusEntry((cf::CBattleStateEntry*)&buf);
        }
        break;
    }
    default:
        break;
    }
    u8* obj3ED4 = reinterpret_cast<cf::CfActorField3ED4*>(self)->field_0x3ED4;
    reinterpret_cast<cf::CfObjectSub38*>(obj3ED4)->forwardActorEvent(arg);
}
// Retail symbol is Fv; the real ABI passes (self, delta). Same gauge update
// as CActorParam_addGauge but on 0x1614/[0, 0x1616]; after the
// actor-id probe succeeds the update is applied a second time (the retail
// re-reads the fields), then if the gauge is at max (vf190 == vf18C) it is
// decremented via the vf180 slot.
extern "C" void CActorParam_addSecondGauge__Q22cf13CfObjectActorFv(cf::CfObjectActor* self, int delta) {
    cf::CfActorParamFields* f = reinterpret_cast<cf::CfActorParamFields*>(self);
    s16 cur = f->field_0x1614;
    s16 max = f->field_0x1616;
    // Explicit (s16) truncation of the int param -> retail's extsh r0, r4
    // (an s16-typed param lets MWCC fold the sign-extension into the add).
    int sum = cur + (s16)delta;
    f->field_0x1614 = (u16)sum;
    if ((s16)sum < 0) {
        f->field_0x1614 = 0;
    } else if ((s16)sum > max) {
        f->field_0x1614 = (u16)max;
    }
    u32* idPtr = reinterpret_cast<u32*>(
        reinterpret_cast<cf::CObjectState*>(
            reinterpret_cast<cf::CActorState*>(self)->unk4)
            ->CObjectState_getStateData());
    u32 id = *idPtr;
    if (testActorStatus(self, (int*)&id, 0x802) != 0) {
        s16 cur2 = f->field_0x1614;
        s16 max2 = f->field_0x1616;
        int sum2 = cur2 + (s16)delta;
        f->field_0x1614 = (u16)sum2;
        if ((s16)sum2 < 0) {
            f->field_0x1614 = 0;
        } else if ((s16)sum2 > max2) {
            f->field_0x1614 = (u16)max2;
        }
        // MWCC evaluates == right-to-left: retail calls 0x190 first, 0x18C second.
        if (self->CActorParam_getSecondGauge() == self->CActorParam_getSecondGaugeMax()) {
            int mx = self->CActorParam_getSecondGaugeMax();
            self->CActorParam_setSecondGauge(mx - 1);
        }
    }
}
// Retail symbol is Fv; the real ABI passes the float in f1. Slot +0x120
// (CActorParam_applyDamage) takes (float, int, int, int); the float arg
// is value itself, passed through unchanged.
extern "C" void CActorParam_addHp__Q22cf13CfObjectActorFv(cf::CfObjectActor* self, float value) {
    if (value <= lbl_eu_80667738) {
        self->CActorParam_applyDamage(value, 0, 0, 0);
    } else {
        self->CActorParam_applyDamage(value, 3, 0, 0);
    }
}
// Retail symbol is Fv; the real ABI passes (self, value, a, b, c). Slot
// +0x120 (CActorParam_applyDamage): applies the rounded float delta to
// the 0x17E8 gauge and dispatches status-driven follow-ups (33/120/C3/92/
// FC/100) on the +8 sub-object plus battle-manager effects.
extern "C" void CActorParam_applyDamage__Q22cf13CfObjectActorFv(cf::CfObjectActor* self, float value, int a, int b, int c) {
    float f = (float)(int)value;   // f31: rounded gauge delta
    if ((lbl_eu_80663E24 & 0x10000000) != 0) return;
    if ((lbl_eu_80663E28 & 0x800) != 0) return;
    int v27 = 0;
    getInstance__Q22cf13CfGameManagerFv();
    if (isGlobalCamFlagSet__Fi(0x04000000) != 0) {
        cf::CfPtmf3 ptmf;
        ptmf.field_0 = lbl_eu_80531090[0];
        ptmf.field_4 = lbl_eu_80531090[1];
        ptmf.field_8 = lbl_eu_80531090[2];
        if (__ptmf_cmpr((u8*)getInstance__Q22cf14CBattleManagerFv() + 0x28354, &ptmf) == 0) {
            v27 = 1;
        }
    }
    if (v27 != 0) return;

    c &= ~3;   // low 2 flag bits are cleared in place (retail clrlwi r31, r31, 2)
    if (f > lbl_eu_80667738) {
        BtlDmg_SetField779((c & (0x80000000 | 0x200)) != 0);
        BtlDmg_SetByte778(0);
        BtlDmg_SetByte777(0);
    } else {
        BtlDmg_SetField779((c & (0x80000000 | 0x100)) != 0);
        BtlDmg_SetByte778((c & (0x80000000 | 0x400)) != 0);
        if ((c & 0x80500000) != 0) {
            BtlDmg_SetByte777(1);
        } else if ((c & 0x80A00000) != 0) {
            BtlDmg_SetByte777(2);
        } else {
            BtlDmg_SetByte777(0);
        }
    }
    if (f < lbl_eu_80667738 && isBattleStatusActive((u8*)self + 8, 1) != 0) {
        f = lbl_eu_80667738;
    }
    // Status 0x33: adjust the gauge value inside the status entry. `ep` is
    // one register reused across every status block (retail keeps the entry
    // pointers in a single callee-saved register).
    cf::CfStatusEntry* ep = 0;
    if (isBattleStatusActive((u8*)self + 8, 0x33) != 0) {
        if ((c & 0x90000000) == 0) {
            ep = (cf::CfStatusEntry*)findBattleStatusEntry((u8*)self + 8, 0x33);
            if (ep != 0) {
                if ((c & (0x80000000 | 0x8000)) != 0) {
                    BtlDmg_FilterNotifyDamage((void*)reinterpret_cast<cf::CfActorField3F10*>(self)->field_0x3F10, 3, 0x10);
                    if (f > lbl_eu_80667738) {
                        ep->field_0x14 = (s16)(s32)f;
                        releaseVisionSlot(self, 0);
                        return;
                    }
                    static_cast<cf::CBattleState*>((cf::CBattleState*)((u8*)self + 8))->CBattleState_applyEventEntry((cf::CBattleStateEntry*)ep);
                    if (lbl_eu_80667738 == f) return;
                } else {
                    if (f <= lbl_eu_80667738) {
                        ep->field_0x14 = (s16)((float)(s16)ep->field_0x14 + f);
                        BtlDmg_FilterNotifyDamage((void*)reinterpret_cast<cf::CfActorField3F10*>(self)->field_0x3F10, 3, 0x10);
                        if ((s16)ep->field_0x14 > 0) {
                            releaseVisionSlot(self, 0);
                            return;
                        }
                        f = (float)(s16)ep->field_0x14;
                        static_cast<cf::CBattleState*>((cf::CBattleState*)((u8*)self + 8))->CBattleState_applyEventEntry((cf::CBattleStateEntry*)ep);
                        if (lbl_eu_80667738 == f) return;
                    }
                }
            }
        }
    }
    if (f > lbl_eu_80667738 && isBattleStatusActive((u8*)self + 8, 0x120) != 0) return;
    // Round the gauge to the nearest int (half away from zero), then clamp.
    float v = reinterpret_cast<cf::CfActorParamFields*>(self)->field_0x17E8 + f;
    reinterpret_cast<cf::CfActorParamFields*>(self)->field_0x17E8 = v;
    double round;
    if (v > lbl_eu_80667738) {
        round = lbl_eu_80667750;
    } else {
        round = lbl_eu_80667758;
    }
    v = (float)(s32)((double)v + round);
    reinterpret_cast<cf::CfActorParamFields*>(self)->field_0x17E8 = v;
    if (v < lbl_eu_80667740) {
        reinterpret_cast<cf::CfActorParamFields*>(self)->field_0x17E8 = lbl_eu_80667738;
    }
    v = reinterpret_cast<cf::CfActorParamFields*>(self)->field_0x17E8;
    if (v < lbl_eu_80667738) {
        reinterpret_cast<cf::CfActorParamFields*>(self)->field_0x17E8 = lbl_eu_80667738;
    } else if (v > reinterpret_cast<cf::CfActorParamFields*>(self)->field_0x17F4) {
        reinterpret_cast<cf::CfActorParamFields*>(self)->field_0x17E8 = reinterpret_cast<cf::CfActorParamFields*>(self)->field_0x17F4;
    }
    BtlDmg_SetDamageType((u8)a);
    BtlDmg_SetDamageDir((u8)b);
    if (f <= lbl_eu_80667738) {
        BtlDmg_NotifyDamage((void*)reinterpret_cast<cf::CfActorField3F10*>(self)->field_0x3F10, (u32)(s32)-f);
        CChain_AccumGaugeDelta((u8*)getInstance__Q22cf14CBattleManagerFv() + 0x1A8, self, -f);
    } else {
        BtlDmg_NotifyDamage((void*)reinterpret_cast<cf::CfActorField3F10*>(self)->field_0x3F10, (u32)(s32)f);
    }
    if (f <= lbl_eu_80667738) {
        // Status 0xC3: scale a ratio from the entry value.
        if (isBattleStatusActive((u8*)self + 8, 0xC3) != 0) {
            ep = (cf::CfStatusEntry*)findBattleStatusEntry((u8*)self + 8, 0xC3);
            if (ep != 0) {
                f = lbl_eu_80667740;
                if (ep->field_0x10 != 0) {
                    float g = self->CActorParam_getDamageScale();
                    f = (float)(s32)ep->field_0x10 * (g / lbl_eu_80667760);
                }
                if (self->CActorParam_getHp() < f) {
                    self->CActorParam_setHp(f);
                }
            }
        }
        // Status 0x92: party-list length gated proc.
        if (isBattleStatusActive((u8*)self + 8, 0x92) != 0) {
            void* bm = getInstance__Q22cf14CBattleManagerFv();
            u8* base = *(u8**)((u8*)bm + 8);
            u8* node = *(u8**)base;
            int count = 0;
            while (node != base) {
                node = *(u8**)node;
                count++;
            }
            if (count != 0) {
                int v27b = 0;
                if (isBattleStatusActive((u8*)self + 8, 0xA1) != 0) {
                    v27b = ((cf::CfStatusEntry*)findBattleStatusEntry((u8*)self + 8, 0xA1))->field_0x10;
                }
                ep = (cf::CfStatusEntry*)findBattleStatusEntry((u8*)self + 8, 0x92);
                if (ep != 0) {
                    if (self->CActorParam_getHp() < lbl_eu_80667740) {
                        s32 val = ep->field_0x10;
                        if (ep->field_0x8 == 0x2000) val += v27b;
                        if ((c & 0xA0000000) != 0) {
                            self->CActorParam_setHp(lbl_eu_80667740);
                        } else if (rand() % 100 < val) {
                            self->CActorParam_setHp(lbl_eu_80667740);
                        }
                    }
                }
            }
        }
        // Status 0xFC: gauge drain block.
        if (isBattleStatusActive((u8*)self + 8, 0xFC) != 0) {
            ep = (cf::CfStatusEntry*)findBattleStatusEntry((u8*)self + 8, 0xFC);
            if (ep != 0) {
                if (self->CActorParam_getHp() < lbl_eu_80667740) {
                    float g = self->CActorParam_getDamageScale();
                    f = lbl_eu_80667764 * ((float)(s32)ep->field_0x10 * g);
                    getInstance__Q22cf14CBattleManagerFv();
                    f *= calcBattleDamage(0, self, 0);
                    self->CActorParam_addHp(f);
                    static_cast<cf::CBattleState*>((cf::CBattleState*)((u8*)self + 8))->CBattleState_clearStatusId(0xFC);
                    PartyGaugeAddClamped((u8*)getInstance__Q22cf14CBattleManagerFv() + 0x194, 0x32);
                }
            }
        }
        // Status 0x100: same drain block plus a decay add and flag.
        if (isBattleStatusActive((u8*)self + 8, 0x100) != 0) {
            ep = (cf::CfStatusEntry*)findBattleStatusEntry((u8*)self + 8, 0x100);
            if (ep != 0) {
                if (self->CActorParam_getHp() < lbl_eu_80667740) {
                    float g = self->CActorParam_getDamageScale();
                    f = lbl_eu_80667764 * ((float)(s32)ep->field_0x10 * g);
                    getInstance__Q22cf14CBattleManagerFv();
                    f *= calcBattleDamage(0, self, 0);
                    self->CActorParam_addHp(f);
                    static_cast<cf::CBattleState*>((cf::CBattleState*)((u8*)self + 8))->CBattleState_clearStatusId(0x100);
                    self->CActorParam_addArtsGauge(lbl_eu_80667768);
                    CBattleMan_ClearEventNotify(getInstance__Q22cf14CBattleManagerFv(), self, 0x93);
                }
            }
        }
    }
    if (self->CActorParam_getHp() < lbl_eu_80667740) {
        if (CChain_ProbeActorReady((u8*)getInstance__Q22cf14CBattleManagerFv() + 0x1A8, self) != 0) {
            self->CActorParam_setHp(lbl_eu_80667740);
        }
    }
    releaseVisionSlot(self, 0);
}

// CfObjectActor's override of the CActorParam virtual (slot 0xA4): queries
// the +0x04 sub-object's vtable slot +0x30 for the actor-id word, then pokes
// the move subobject's status chain when the id matches the query. Retail
// symbol is Fv; the real ABI passes (self, arts). Forced-name extern "C"
// wrapper (same pattern as the CActorParam base in CActorParam.cpp): the
// class keeps the member decl, this definition provides the retail symbol.
// All in-tree callers dispatch through CActorParam*, so no member-form
// definition is needed.
extern "C" void CActorParam_resetArtsStatus__Q22cf13CfObjectActorFv(cf::CfObjectActor* self, void* arts) {
    // Forwards the incoming arts word (always null at every call site) as
    // the base-slot int flag: retail has no `li r4,0`, r4 flows straight
    // into the direct bl. A member call would virtual-dispatch slot 0xAC.
    self->cf::CActorParam::CActorParam_UnkVirtualFunc6((int)arts);
    u32* idPtr = reinterpret_cast<u32*>(
        reinterpret_cast<cf::CObjectState*>(
            reinterpret_cast<cf::CActorState*>(self)->unk4)
            ->CObjectState_getStateData());
    u32 id = *idPtr;
    if (testActorStatus(self, (int*)&id, 0x1c) != 0) {
        CfObjectMove_setAnimModeArgs((u8*)self + 16028, 0x2f, 1, -1, 1);  // +0x3E9C: CfObjectMove subobject
    }
}

// .data table (retail 0xC) as a typed pointer table. Reloc sites read as
// zero in the .o (relocs skipped when bytes match), so it stays MATCH.
void func_801A4194();
__declspec(section ".data") __attribute__((used))
const u32 lbl_eu_80531090[3] = { 0, 0xFFFFFFFFu, (u32)func_801A4194 };

// .sdata2 pool (retail 0x44) as typed consts in retail declaration order.
__declspec(section ".sdata2") __attribute__((used))
const float lbl_eu_80667738 = 0.0f;
__declspec(section ".sdata2") __attribute__((used))
const float lbl_eu_8066773C = 0.5f;
__declspec(section ".sdata2") __attribute__((used))
const float lbl_eu_80667740 = 1.0f;
__declspec(section ".sdata2") __attribute__((used))
const double lbl_eu_80667748 = 4503601774854144.0;
__declspec(section ".sdata2") __attribute__((used))
const double lbl_eu_80667750 = 0.5;
__declspec(section ".sdata2") __attribute__((used))
const double lbl_eu_80667758 = -0.5;
__declspec(section ".sdata2") __attribute__((used))
const float lbl_eu_80667760 = 100.0f;
__declspec(section ".sdata2") __attribute__((used))
const float lbl_eu_80667764 = 0.01f;
__declspec(section ".sdata2") __attribute__((used))
const float lbl_eu_80667768 = 50.0f;
__declspec(section ".sdata2") __attribute__((used))
const float lbl_eu_8066776C = -1.0f;

// .data 0x5D4: the cf::CfObjectActor vtable group (primary vtable + the
// CBattleState / CAIAction / CfObjectMove secondary vtables; RTTI headers at
// +0x0, +0xC, +0x36C, +0x37C). Hand-built until CfObjectActor is a real
// polymorphic class; every slot relocates to a CfObjectActor-family member.
extern "C" {
extern void* __RTTI__Q22cf13CfObjectActor;
extern void CfObject_isMoveActiveNow__Q22cf12CfObjectMoveFv();
extern void CObjectState_clearStateFlags8__Q22cf12CObjectStateFv();
extern void CActorParam_UnkVirtualFunc176__Q22cf11CActorParamFv();
extern void CActorParam_UnkVirtualFunc22__Q22cf11CActorParamFv();
extern void CActorState_setObjStateBit3__Q22cf11CActorStateFv(), CBattleState_getOwner__Q22cf13CfObjectActorFv(), CBattleState_callOwnerObject__Q22cf13CfObjectActorFv(), CBattleState_getParamPtrName__Q22cf13CfObjectActorFv(), CBattleState_UnkVirtualFunc4__Q22cf12CBattleStateFv(), CBattleState_enterStatusEntry__Q22cf12CBattleStateFPQ22cf17CBattleStateEntry();
extern void CBattleState_addStagingEntry__Q22cf12CBattleStateFPQ22cf17CBattleStateEntry(), CBattleState_clearStatusId__Q22cf12CBattleStateFv(), CBattleState_applyEventEntry__Q22cf12CBattleStateFPQ22cf17CBattleStateEntry(), CBattleState_UnkVirtualFunc9__Q22cf12CBattleStateFv(), CBattleState_removeKeyedEntries__Q22cf12CBattleStateFPQ22cf17CBattleStateEntry(), CBattleState_clearEntriesByMask__Q22cf12CBattleStateFUl();
extern void CBattleState_UnkVirtualFunc12__Q22cf12CBattleStateFv(), CBattleState_getStatusSlot__Q22cf12CBattleStateFi(), CBattleState_UnkVirtualFunc14__Q22cf12CBattleStateFv(), CBattleState_UnkVirtualFunc15__Q22cf12CBattleStateFv(), CBattleState_UnkVirtualFunc16__Q22cf12CBattleStateFv(), CBattleState_notifyEntryUpdated__Q22cf13CfObjectActorFv();
extern void CBattleState_getLinkedActorId__Q22cf13CfObjectActorFv(), CBattleState_UnkVirtualFunc19__Q22cf12CBattleStateFv(), CBattleState_getEntryByIndex__Q22cf12CBattleStateFv(), CBattleState_fetchStatusEntry__Q22cf12CBattleStateFv(), CBattleState_getEventEntry__Q22cf12CBattleStateFv(), CBattleState_getStatusEntry__Q22cf12CBattleStateFv();
extern void CBattleState_copyStatusEntries__Q22cf12CBattleStateFv(), CBattleState_getEntryArray__Q22cf12CBattleStateFv(), CBattleState_applyArtsTable__Q22cf12CBattleStateFPCQ22cf20CBattleStateSrcEntry(), CBattleState_getStatusBlock__Q22cf12CBattleStateFv(), CBattleState_getStagingRecord__Q22cf12CBattleStateFv(), CBattleState_clearStagingEntries__Q22cf12CBattleStateFv();
extern void CBattleState_UnkVirtualFunc30__Q22cf12CBattleStateFv(), CBattleState_getEventMask__Q22cf12CBattleStateFUl(), CBattleState_UnkVirtualFunc32__Q22cf12CBattleStateFv(), CBattleState_getStatusMask__Q22cf12CBattleStateFUl(), CActorParam_UnkVirtualFunc2__Q22cf13CfObjectActorFv();
extern void CActorParam_UnkVirtualFunc3__Q22cf13CfObjectActorFv(), CActorParam_resetArtsStatus__Q22cf11CActorParamFv(), CActorParam_commitArtsStatus__Q22cf11CActorParamFv(), CActorParam_setupActorState__Q22cf11CActorParamFv(), CActorParam_resetArtsMatrix__Q22cf11CActorParamFv();
extern void CActorParam_UnkVirtualFunc9__Q22cf11CActorParamFv(), CActorParam_applyTargetEvent__Q22cf11CActorParamFv(), CActorParam_applyStatusHeal__Q22cf11CActorParamFPQ22cf16CActorParam11Arg(), CActorParam_applyEventStatus__Q22cf11CActorParamFv(), CActorParam_resetGaugesAndRefresh__Q22cf11CActorParamFv(), CActorParam_setStatByte__Q22cf11CActorParamFUc();
extern void CActorParam_UnkVirtualFunc15__Q22cf11CActorParamFv(), CActorParam_setStatScale__Q22cf11CActorParamFf(), CActorParam_UnkVirtualFunc17__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc18__Q22cf11CActorParamFv(), CActorParam_getActorType__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc20__Q22cf11CActorParamFv();
extern void CActorParam_UnkVirtualFunc24__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc25__Q22cf11CActorParamFv(), CActorParam_getArtsLevel__Q22cf11CActorParamFv();
extern void CActorParam_UnkVirtualFunc27__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc28__Q22cf11CActorParamFv(), CActorParam_getActorLevel__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc30__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc31__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc32__Q22cf11CActorParamFv();
extern void CActorParam_UnkVirtualFunc36__Q22cf11CActorParamFv(), CActorParam_getHp__Q22cf11CActorParamFv(), CActorParam_getDamageScale__Q22cf11CActorParamFv();
extern void CActorParam_UnkVirtualFunc39__Q22cf11CActorParamFv(), CActorParam_applyStoredHp__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc41__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc42__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc43__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc44__Q22cf11CActorParamFv();
extern void CActorParam_UnkVirtualFunc45__Q22cf11CActorParamFv(), CActorParam_applyStoredHitCharge__Q22cf11CActorParamFv(), CActorParam_setTensionPoints__Q22cf11CActorParamFv(), CActorParam_addArtsGauge__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc49__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc50__Q22cf11CActorParamFv();
extern void CActorParam_UnkVirtualFunc51__Q22cf11CActorParamFv(), CActorParam_applyStoredArtsGauge__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc53__Q22cf11CActorParamFv(), CActorParam_setGaugeFlagA__Q22cf11CActorParamFUs(), CActorParam_UnkVirtualFunc56__Q22cf11CActorParamFv();
extern void CActorParam_UnkVirtualFunc57__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc58__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc59__Q22cf11CActorParamFv(), CActorParam_setGaugeFlagB__Q22cf11CActorParamFUs(), CActorParam_UnkVirtualFunc62__Q22cf11CActorParamFv();
extern void CActorParam_UnkVirtualFunc63__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc64__Q22cf11CActorParamFv(), CActorParam_setGaugeRateA__Q22cf11CActorParamFf(), CActorParam_UnkVirtualFunc66__Q22cf11CActorParamFv(), CActorParam_resetGaugeBattleEntry__Q22cf11CActorParamFv(), CActorParam_setGaugeRateB__Q22cf11CActorParamFf();
extern void CActorParam_UnkVirtualFunc69__Q22cf11CActorParamFv(), CActorParam_resetSecondGaugeBattleEntry__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc71__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc72__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc73__Q22cf11CActorParamFv(), CActorParam_setMoveRate__Q22cf11CActorParamFf();
extern void CActorParam_UnkVirtualFunc75__Q22cf11CActorParamFv(), CActorParam_getMoveRate__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc77__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc78__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc79__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc80__Q22cf11CActorParamFv();
extern void CActorParam_setSpentCurrency__Q22cf11CActorParamFUl(), CActorParam_addSpentCurrency__Q22cf11CActorParamFUl(), CActorParam_addAccumCurrency__Q22cf11CActorParamFUl(), CActorParam_UnkVirtualFunc84__Q22cf11CActorParamFv(), CActorParam_getSpentCurrency__Q22cf11CActorParamFv(), CActorParam_getCurrencyBalance__Q22cf11CActorParamFv();
extern void CActorParam_UnkVirtualFunc87__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc88__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc89__Q22cf11CActorParamFv(), CActorParam_addSecondCurrency__Q22cf11CActorParamFUl(), CActorParam_UnkVirtualFunc91__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc92__Q22cf11CActorParamFv();
extern void CActorParam_UnkVirtualFunc93__Q22cf11CActorParamFv(), CActorParam_getArtsDataBlock__Q22cf11CActorParamFv(), CActorParam_getStatusSyncBlock__Q22cf11CActorParamFv(), CActorParam_getArtsSyncBlock__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc97__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc98__Q22cf11CActorParamFv();
extern void CActorParam_UnkVirtualFunc99__Q22cf11CActorParamFv(), CActorParam_getBattleParams__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc101__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc102__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc103__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc104__Q22cf11CActorParamFv();
extern void CActorParam_UnkVirtualFunc105__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc106__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc107__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc108__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc109__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc110__Q22cf11CActorParamFv();
extern void CActorParam_UnkVirtualFunc111__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc112__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc113__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc114__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc115__Q22cf11CActorParamFv(), CActorParam_setGaugeFloat__Q22cf11CActorParamFf();
extern void CActorParam_UnkVirtualFunc117__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc118__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc119__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc120__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc121__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc122__Q22cf11CActorParamFv();
extern void CActorParam_UnkVirtualFunc123__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc124__Q22cf11CActorParamFv(), CActorParam_getArtsSlotIds__Q22cf11CActorParamFv(), CActorParam_getCopyParam__Q22cf11CActorParamFv(), CActorParam_getStatusTable__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc128__Q22cf11CActorParamFv();
extern void CActorParam_getBattleStats__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc130__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc131__Q22cf11CActorParamFv(), CActorParam_getMoveRecord__Q22cf11CActorParamFv(), CActorParam_getStatsSlot__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc134__Q22cf11CActorParamFv();
extern void CActorParam_reinitBattleStats__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc136__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc137__Q22cf11CActorParamFv(), CActorParam_isBattleLocked__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc139__Q22cf11CActorParamFv();
extern void CActorParam_resetGaugeEntry__Q22cf11CActorParamFv(), CActorParam_resetAllGaugeEntries__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc143__Q22cf11CActorParamFv(), CActorParam_findGaugeEntry__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc145__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc146__Q22cf11CActorParamFv();
extern void CActorParam_UnkVirtualFunc147__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc148__Q22cf11CActorParamFv(), CActorParam_findMaxGaugeEntry__Q22cf11CActorParamFv(), CActorParam_findMinGaugeEntry__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc151__Q22cf11CActorParamFv(), CActorParam_getTensionStatus__Q22cf11CActorParamFv();
extern void CActorParam_accumulateTension__Q22cf11CActorParamFv(), CActorParam_raiseTensionValue__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc155__Q22cf11CActorParamFv(), CActorParam_resetTensionState__Q22cf11CActorParamFv(), CActorParam_getStatusCount__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc158__Q22cf11CActorParamFv();
extern void CActorParam_UnkVirtualFunc159__Q22cf11CActorParamFv(), CActorParam_initStatusCounter__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc161__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc162__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc163__Q22cf11CActorParamFv(), CActorParam_getActorModeFlags__Q22cf11CActorParamFv();
extern void CActorParam_getBattleHitFlags__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc166__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc168__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc169__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc170__Q22cf11CActorParamFv();
extern void CActorParam_UnkVirtualFunc171__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc172__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc173__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc174__Q22cf11CActorParamFv(), CActorParam_updateHateEntries__Q22cf11CActorParamFf();
extern void CActorParam_UnkVirtualFunc177__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc178__Q22cf11CActorParamFv(), CActorParam_UnkVirtualFunc181__Q22cf11CActorParamFv();
extern void CObjectState_setStateBitMask__Q22cf12CObjectStateFv(), CObjectState_checkStateFlags__Q22cf12CObjectStateFv(), CObjectState_setStateBitFlag__Q22cf12CObjectStateFv(), CObjectState_clearStateWord4__Q22cf12CObjectStateFv(), CObjectState_applyStateFlags__Q22cf12CObjectStateFv();
extern void CObjectState_clearStateWord8__Q22cf12CObjectStateFv(), CObjectState_UnkVirtualFunc8__Q22cf12CObjectStateFv(), CObjectState_UnkVirtualFunc9__Q22cf12CObjectStateFv(), CObjectState_setStateBitMask0__Q22cf12CObjectStateFv(), CObjectState_getStateData__Q22cf12CObjectStateFv();
extern void CObjectState_setStateBitMask2__Q22cf12CObjectStateFv(), CObjectState_setStateBitMask3__Q22cf12CObjectStateFv(), CObjectParam_UnkVirtualFunc1__Q22cf12CObjectParamFv(), CObjectParam_getParamPtr__Q22cf13CfObjectActorFv(), CObjectParam_hasObjectName__Q22cf12CObjectParamFv(), CObjectParam_UnkVirtualFunc4__Q22cf12CObjectParamFv();
extern void CObjectParam_getSelfObjectId__Q22cf12CObjectParamFv(), CObjectParam_signalActionEnd__Q22cf12CObjectParamFv(), CfObject_UnkVirtualFunc2__Q22cf13CfObjectModelFv(), CfObject_UnkVirtualFunc3__Q22cf8CfObjectFv(), CfObject_UnkVirtualFunc4__Q22cf12CfObjectMoveFv();
extern void CfObject_UnkVirtualFunc5__Q22cf12CfObjectMoveFv(), CfObject_UnkVirtualFunc6__Q22cf12CfObjectMoveFv(), CfObject_UnkVirtualFunc7__Q22cf12CfObjectMoveFv(), CfObject_notifyEventDone__Q22cf13CfObjectModelFv(), CfObject_forwardSubObject__Q22cf12CfObjectMoveFv();
extern void CfObject_UnkVirtualFunc11__Q22cf8CfObjectFv(), CfObject_UnkVirtualFunc12__Q22cf12CfObjectMoveFv(), CfObject_queryTargetState__Q22cf12CfObjectMoveFv(), CfObject_pushRefreshValue__Q22cf13CfObjectActorFf(), CfObject_getMoveSpeedRate__Q22cf12CfObjectMoveFv(), CfObject_pushRefreshExtra__Q22cf12CfObjectMoveFv();
extern void CfObject_readRefreshValue__Q22cf12CfObjectMoveFv(), CfObject_checkTargetState__Q22cf12CfObjectMoveFv(), CfObject_setMoveTargetVec__Q22cf12CfObjectMoveFv(), CfObject_UnkVirtualFunc20__Q22cf13CfObjectModelFv(), CfObject_UnkVirtualFunc21__Q22cf8CfObjectFv(), CfObject_UnkVirtualFunc22__Q22cf12CfObjectMoveFv();
extern void CfObject_getPosVector__Q22cf12CfObjectMoveFv(), CfObject_UnkVirtualFunc24__Q22cf13CfObjectModelFv(), CfObject_UnkVirtualFunc25__Q22cf12CfObjectMoveFv(), CfObject_UnkVirtualFunc26__Q22cf12CfObjectMoveFv(), CfObject_UnkVirtualFunc27__Q22cf12CfObjectMoveFPv(), CfObject_UnkVirtualFunc28__Q22cf13CfObjectModelFv();
extern void CfObject_setMoveHeadAngle__Q22cf12CfObjectMoveFv(), CfObject_UnkVirtualFunc30__Q22cf12CfObjectMoveFv(), CfObject_getMoveHeadAngle__Q22cf13CfObjectActorFv(), CfObject_UnkVirtualFunc32__Q22cf12CfObjectMoveFv(), CfObject_UnkVirtualFunc33__Q22cf12CfObjectMoveFv(), CfObject_UnkVirtualFunc34__Q22cf13CfObjectModelFv();
extern void CfObject_UnkVirtualFunc35__Q22cf13CfObjectModelFv(), CfObject_UnkVirtualFunc36__Q22cf13CfObjectModelFv(), CfObject_UnkVirtualFunc37__Q22cf12CfObjectMoveFv(), CfObject_UnkVirtualFunc38__Q22cf12CfObjectMoveFv(), CfObject_UnkVirtualFunc39__Q22cf12CfObjectMoveFv(), CfObject_UnkVirtualFunc40__Q22cf12CfObjectMoveFv();
extern void CfObject_UnkVirtualFunc41__Q22cf8CfObjectFv(), CfObject_UnkVirtualFunc42__Q22cf12CfObjectMoveFv(), CfObject_UnkVirtualFunc43__Q22cf12CfObjectMoveFv(), CfObject_UnkVirtualFunc44__Q22cf8CfObjectFv(), CfObject_UnkVirtualFunc45__Q22cf12CfObjectMoveFv(), CfObject_setMoveTargetPtr__Q22cf12CfObjectMoveFv();
extern void CfObject_createMoveTarget__Q22cf12CfObjectMoveFv(), CfObject_getCurrentTarget__Q22cf12CfObjectMoveFv(), CfObject_forwardNpcAction__Q22cf12CfObjectMoveFv(), CfObject_UnkVirtualFunc50__Q22cf12CfObjectMoveFv(), CfObject_UnkVirtualFunc51__Q22cf12CfObjectMoveFv(), CfObject_UnkVirtualFunc52__Q22cf13CfObjectModelFv();
extern void CfObject_UnkVirtualFunc53__Q22cf13CfObjectModelFv(), CfObject_UnkVirtualFunc54__Q22cf13CfObjectModelFv(), CfObject_UnkVirtualFunc55__Q22cf13CfObjectModelFv(), CfObject_UnkVirtualFunc56__Q22cf13CfObjectModelFv(), CfObject_UnkVirtualFunc57__Q22cf12CfObjectMoveFv(), CfObject_getMoveRateScale__Q22cf12CfObjectMoveFv();
extern void CfObject_UnkVirtualFunc59__Q22cf12CfObjectMoveFv(), CfObject_UnkVirtualFunc60__Q22cf12CfObjectMoveFv(), CfObject_setAnimSlotEntry__Q22cf12CfObjectMoveFv(), CfObject_UnkVirtualFunc62__Q22cf12CfObjectMoveFv(), CfObject_UnkVirtualFunc63__Q22cf13CfObjectModelFv(), CfObject_setMoveBusyState__Q22cf12CfObjectMoveFi();
extern void CfObject_setMoveReadyFlag__Q22cf12CfObjectMoveFv(), CfObject_UnkVirtualFunc66__Q22cf12CfObjectMoveFv(), CfObject_UnkVirtualFunc67__Q22cf13CfObjectModelFv(), CfObject_UnkVirtualFunc68__Q22cf13CfObjectModelFv(), CfObject_UnkVirtualFunc69__Q22cf13CfObjectModelFv(), CfObject_UnkVirtualFunc70__Q22cf12CfObjectMoveFv();
extern void CfObject_UnkVirtualFunc71__Q22cf13CfObjectModelFv(), CfObject_UnkVirtualFunc72__Q22cf13CfObjectModelFv(), CfObject_UnkVirtualFunc73__Q22cf13CfObjectModelFv(), CfObjectModel_UnkVirtualFunc1__Q22cf12CfObjectMoveFv(), CfObjectModel_UnkVirtualFunc2__Q22cf12CfObjectMoveFv(), CfObjectModel_UnkVirtualFunc3__Q22cf13CfObjectModelFv();
extern void CfObjectModel_UnkVirtualFunc4__Q22cf12CfObjectMoveFv(), CfObjectModel_UnkVirtualFunc5__Q22cf12CfObjectMoveFv(), CfObjectModel_UnkVirtualFunc6__Q22cf12CfObjectMoveFv(), CfObjectModel_UnkVirtualFunc7__Q22cf13CfObjectModelFv(), CfObjectModel_UnkVirtualFunc8__Q22cf13CfObjectModelFv(), CfObjectModel_UnkVirtualFunc9__Q22cf13CfObjectModelFv();
extern void CfObjectModel_UnkVirtualFunc10__Q22cf13CfObjectModelFv(), CfObjectModel_UnkVirtualFunc11__Q22cf13CfObjectModelFv(), CfObjectModel_UnkVirtualFunc12__Q22cf13CfObjectModelFv(), CfObjectModel_UnkVirtualFunc13__Q22cf13CfObjectModelFv(), CfObjectModel_UnkVirtualFunc14__Q22cf13CfObjectModelFv(), CfObjectModel_UnkVirtualFunc15__Q22cf13CfObjectModelFv();
extern void CfObjectModel_UnkVirtualFunc16__Q22cf13CfObjectModelFv(), CfObjectModel_UnkVirtualFunc17__Q22cf13CfObjectModelFv(), CfObjectModel_UnkVirtualFunc18__Q22cf12CfObjectMoveFv(), CfObjectModel_UnkVirtualFunc19__Q22cf12CfObjectMoveFv(), CfObjectModel_UnkVirtualFunc20__Q22cf13CfObjectModelFv(), CfObjectMove_dropMoveRequest__Q22cf12CfObjectMoveFv();
extern void CfObjectMove_detachModelList__Q22cf12CfObjectMoveFv(), CfObjectMove_releaseSlotById__Q22cf12CfObjectMoveFv(), CfObjectMove_recordMoveValue__Q22cf12CfObjectMoveFv(), CfObjectMove_getMovementRate__Q22cf12CfObjectMoveFv(), CfObjectMove_storeActionSrcId__Q22cf13CfObjectActorFv(), CfObjectMove_refreshNpcState__Q22cf12CfObjectMoveFv();
extern void CfObjectMove_updateNpcTarget__Q22cf12CfObjectMoveFv(), CfObjectMove_queryNpcAdvance__Q22cf12CfObjectMoveFv(), CfObjectMove_UnkVirtualFunc10__Q22cf12CfObjectMoveFv(), CfObjectMove_writeTargetField__Q22cf12CfObjectMoveFv(), CfObjectMove_advanceNpcTarget__Q22cf12CfObjectMoveFv(), CfObjectMove_commandNpcTarget__Q22cf12CfObjectMoveFv();
extern void CfObjectMove_restartNpcTarget__Q22cf12CfObjectMoveFv(), CfObjectMove_hasActorScale__Q22cf13CfObjectActorFv(), CfObjectMove_attachEffectSlot__Q22cf12CfObjectMoveFv(), CfObjectMove_setEffectSlotBit__Q22cf12CfObjectMoveFv(), CfObjectMove_transferSlotBits__Q22cf12CfObjectMoveFv(), CfObjectMove_isEffectSlotUsed__Q22cf12CfObjectMoveFv();
extern void CfObjectMove_signalEffectSlot__Q22cf12CfObjectMoveFv(), CfObjectMove_flushEffectSlots__Q22cf12CfObjectMoveFv(), CfObjectMove_setAllEffectBits__Q22cf12CfObjectMoveFv(), CfObjectMove_loadResourceById__Q22cf12CfObjectMoveFv(), CfObjectActor_hasActorScale__Q22cf13CfObjectActorFv();
extern void CfObjectActor_clearStatusPair__Q22cf13CfObjectActorFv(), CfObjectActor_flushStatusPair__Q22cf13CfObjectActorFv(), CfObjectActor_readFacingAngle__Q22cf13CfObjectActorFv(), CfObjectActor_getAdjustedFacing__Q22cf13CfObjectActorFv(), CfObjectActor_readActionScale__Q22cf13CfObjectActorFv();
extern void CfObjectActor_getActionSrcId__Q22cf13CfObjectActorFv(), CfObjectActor_clearActionSrcId__Q22cf13CfObjectActorFv();

__declspec(section ".data") __attribute__((used))
const void* __vt__Q22cf13CfObjectActor[373] = {
    // +0x0 CActorState secondary (RTTI +0x0)
    &__RTTI__Q22cf13CfObjectActor, 0, (void*)CActorState_setObjStateBit3__Q22cf11CActorStateFv, &__RTTI__Q22cf13CfObjectActor, // +0x0
    (void*)0xFFFFFFF8, (void*)CBattleState_getOwner__Q22cf13CfObjectActorFv, (void*)CBattleState_callOwnerObject__Q22cf13CfObjectActorFv, (void*)CBattleState_getParamPtrName__Q22cf13CfObjectActorFv, // +0x10
    (void*)CBattleState_UnkVirtualFunc4__Q22cf12CBattleStateFv, (void*)CBattleState_enterStatusEntry__Q22cf12CBattleStateFPQ22cf17CBattleStateEntry, (void*)CBattleState_addStagingEntry__Q22cf12CBattleStateFPQ22cf17CBattleStateEntry, (void*)CBattleState_clearStatusId__Q22cf12CBattleStateFv, // +0x20
    (void*)CBattleState_applyEventEntry__Q22cf12CBattleStateFPQ22cf17CBattleStateEntry, (void*)CBattleState_UnkVirtualFunc9__Q22cf12CBattleStateFv, (void*)CBattleState_removeKeyedEntries__Q22cf12CBattleStateFPQ22cf17CBattleStateEntry, (void*)CBattleState_clearEntriesByMask__Q22cf12CBattleStateFUl, // +0x30
    (void*)CBattleState_UnkVirtualFunc12__Q22cf12CBattleStateFv, (void*)CBattleState_getStatusSlot__Q22cf12CBattleStateFi, (void*)CBattleState_UnkVirtualFunc14__Q22cf12CBattleStateFv, (void*)CBattleState_UnkVirtualFunc15__Q22cf12CBattleStateFv, // +0x40
    (void*)CBattleState_UnkVirtualFunc16__Q22cf12CBattleStateFv, (void*)CBattleState_notifyEntryUpdated__Q22cf13CfObjectActorFv, (void*)CBattleState_getLinkedActorId__Q22cf13CfObjectActorFv, (void*)CBattleState_UnkVirtualFunc19__Q22cf12CBattleStateFv, // +0x50
    (void*)CBattleState_getEntryByIndex__Q22cf12CBattleStateFv, (void*)CBattleState_fetchStatusEntry__Q22cf12CBattleStateFv, (void*)CBattleState_getEventEntry__Q22cf12CBattleStateFv, (void*)CBattleState_getStatusEntry__Q22cf12CBattleStateFv, // +0x60
    (void*)CBattleState_copyStatusEntries__Q22cf12CBattleStateFv, (void*)CBattleState_getEntryArray__Q22cf12CBattleStateFv, (void*)CBattleState_applyArtsTable__Q22cf12CBattleStateFPCQ22cf20CBattleStateSrcEntry, (void*)CBattleState_getStatusBlock__Q22cf12CBattleStateFv, // +0x70
    (void*)CBattleState_getStagingRecord__Q22cf12CBattleStateFv, (void*)CBattleState_clearStagingEntries__Q22cf12CBattleStateFv, (void*)CBattleState_UnkVirtualFunc30__Q22cf12CBattleStateFv, (void*)CBattleState_getEventMask__Q22cf12CBattleStateFUl, // +0x80
    (void*)CBattleState_UnkVirtualFunc32__Q22cf12CBattleStateFv, (void*)CBattleState_getStatusMask__Q22cf12CBattleStateFUl, (void*)CActorParam_UnkVirtualFunc1__Q22cf13CfObjectActorFv, (void*)CActorParam_UnkVirtualFunc2__Q22cf13CfObjectActorFv, // +0x90
    (void*)CActorParam_UnkVirtualFunc3__Q22cf13CfObjectActorFv, (void*)CActorParam_resetArtsStatus__Q22cf11CActorParamFv, (void*)CActorParam_commitArtsStatus__Q22cf11CActorParamFv, (void*)CActorParam_resetArtsStatus__Q22cf13CfObjectActorFv, // +0xA0
    (void*)CActorParam_setupActorState__Q22cf11CActorParamFv, (void*)CActorParam_resetArtsMatrix__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc9__Q22cf11CActorParamFv, (void*)CActorParam_applyTargetEvent__Q22cf11CActorParamFv, // +0xB0
    (void*)CActorParam_applyStatusHeal__Q22cf11CActorParamFPQ22cf16CActorParam11Arg, (void*)CActorParam_applyEventStatus__Q22cf11CActorParamFv, (void*)CActorParam_resetGaugesAndRefresh__Q22cf11CActorParamFv, (void*)CActorParam_setStatByte__Q22cf11CActorParamFUc, // +0xC0
    (void*)CActorParam_UnkVirtualFunc15__Q22cf11CActorParamFv, (void*)CActorParam_setStatScale__Q22cf11CActorParamFf, (void*)CActorParam_UnkVirtualFunc17__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc18__Q22cf11CActorParamFv, // +0xD0
    (void*)CActorParam_getActorType__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc20__Q22cf11CActorParamFv, (void*)CActorParam_setScale__Q22cf13CfObjectActorFv, (void*)CActorParam_UnkVirtualFunc22__Q22cf11CActorParamFv, // +0xE0
    (void*)CActorParam_getScale__Q22cf13CfObjectActorFv, (void*)CActorParam_UnkVirtualFunc24__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc25__Q22cf11CActorParamFv, (void*)CActorParam_getArtsLevel__Q22cf11CActorParamFv, // +0xF0
    (void*)CActorParam_UnkVirtualFunc27__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc28__Q22cf11CActorParamFv, (void*)CActorParam_getActorLevel__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc30__Q22cf11CActorParamFv, // +0x100
    (void*)CActorParam_UnkVirtualFunc31__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc32__Q22cf11CActorParamFv, (void*)CActorParam_setHp__Q22cf13CfObjectActorFv, (void*)CActorParam_addHp__Q22cf13CfObjectActorFv, // +0x110
    (void*)CActorParam_applyDamage__Q22cf13CfObjectActorFv, (void*)CActorParam_UnkVirtualFunc36__Q22cf11CActorParamFv, (void*)CActorParam_getHp__Q22cf11CActorParamFv, (void*)CActorParam_getDamageScale__Q22cf11CActorParamFv, // +0x120
    (void*)CActorParam_UnkVirtualFunc39__Q22cf11CActorParamFv, (void*)CActorParam_applyStoredHp__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc41__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc42__Q22cf11CActorParamFv, // +0x130
    (void*)CActorParam_UnkVirtualFunc43__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc44__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc45__Q22cf11CActorParamFv, (void*)CActorParam_applyStoredHitCharge__Q22cf11CActorParamFv, // +0x140
    (void*)CActorParam_setTensionPoints__Q22cf11CActorParamFv, (void*)CActorParam_addArtsGauge__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc49__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc50__Q22cf11CActorParamFv, // +0x150
    (void*)CActorParam_UnkVirtualFunc51__Q22cf11CActorParamFv, (void*)CActorParam_applyStoredArtsGauge__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc53__Q22cf11CActorParamFv, (void*)CActorParam_addGauge__Q22cf13CfObjectActorFv, // +0x160
    (void*)CActorParam_setGaugeFlagA__Q22cf11CActorParamFUs, (void*)CActorParam_UnkVirtualFunc56__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc57__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc58__Q22cf11CActorParamFv, // +0x170
    (void*)CActorParam_UnkVirtualFunc59__Q22cf11CActorParamFv, (void*)CActorParam_addSecondGauge__Q22cf13CfObjectActorFv, (void*)CActorParam_setGaugeFlagB__Q22cf11CActorParamFUs, (void*)CActorParam_UnkVirtualFunc62__Q22cf11CActorParamFv, // +0x180
    (void*)CActorParam_UnkVirtualFunc63__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc64__Q22cf11CActorParamFv, (void*)CActorParam_setGaugeRateA__Q22cf11CActorParamFf, (void*)CActorParam_UnkVirtualFunc66__Q22cf11CActorParamFv, // +0x190
    (void*)CActorParam_resetGaugeBattleEntry__Q22cf11CActorParamFv, (void*)CActorParam_setGaugeRateB__Q22cf11CActorParamFf, (void*)CActorParam_UnkVirtualFunc69__Q22cf11CActorParamFv, (void*)CActorParam_resetSecondGaugeBattleEntry__Q22cf11CActorParamFv, // +0x1A0
    (void*)CActorParam_UnkVirtualFunc71__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc72__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc73__Q22cf11CActorParamFv, (void*)CActorParam_setMoveRate__Q22cf11CActorParamFf, // +0x1B0
    (void*)CActorParam_UnkVirtualFunc75__Q22cf11CActorParamFv, (void*)CActorParam_getMoveRate__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc77__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc78__Q22cf11CActorParamFv, // +0x1C0
    (void*)CActorParam_UnkVirtualFunc79__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc80__Q22cf11CActorParamFv, (void*)CActorParam_setSpentCurrency__Q22cf11CActorParamFUl, (void*)CActorParam_addSpentCurrency__Q22cf11CActorParamFUl, // +0x1D0
    (void*)CActorParam_addAccumCurrency__Q22cf11CActorParamFUl, (void*)CActorParam_UnkVirtualFunc84__Q22cf11CActorParamFv, (void*)CActorParam_getSpentCurrency__Q22cf11CActorParamFv, (void*)CActorParam_getCurrencyBalance__Q22cf11CActorParamFv, // +0x1E0
    (void*)CActorParam_UnkVirtualFunc87__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc88__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc89__Q22cf11CActorParamFv, (void*)CActorParam_addSecondCurrency__Q22cf11CActorParamFUl, // +0x1F0
    (void*)CActorParam_UnkVirtualFunc91__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc92__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc93__Q22cf11CActorParamFv, (void*)CActorParam_getArtsDataBlock__Q22cf11CActorParamFv, // +0x200
    (void*)CActorParam_getStatusSyncBlock__Q22cf11CActorParamFv, (void*)CActorParam_getArtsSyncBlock__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc97__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc98__Q22cf11CActorParamFv, // +0x210
    (void*)CActorParam_UnkVirtualFunc99__Q22cf11CActorParamFv, (void*)CActorParam_getBattleParams__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc101__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc102__Q22cf11CActorParamFv, // +0x220
    (void*)CActorParam_UnkVirtualFunc103__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc104__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc105__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc106__Q22cf11CActorParamFv, // +0x230
    (void*)CActorParam_UnkVirtualFunc107__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc108__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc109__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc110__Q22cf11CActorParamFv, // +0x240
    (void*)CActorParam_UnkVirtualFunc111__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc112__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc113__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc114__Q22cf11CActorParamFv, // +0x250
    (void*)CActorParam_UnkVirtualFunc115__Q22cf11CActorParamFv, (void*)CActorParam_setGaugeFloat__Q22cf11CActorParamFf, (void*)CActorParam_UnkVirtualFunc117__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc118__Q22cf11CActorParamFv, // +0x260
    (void*)CActorParam_UnkVirtualFunc119__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc120__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc121__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc122__Q22cf11CActorParamFv, // +0x270
    (void*)CActorParam_UnkVirtualFunc123__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc124__Q22cf11CActorParamFv, (void*)CActorParam_getArtsSlotIds__Q22cf11CActorParamFv, (void*)CActorParam_getCopyParam__Q22cf11CActorParamFv, // +0x280
    (void*)CActorParam_getStatusTable__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc128__Q22cf11CActorParamFv, (void*)CActorParam_getBattleStats__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc130__Q22cf11CActorParamFv, // +0x290
    (void*)CActorParam_UnkVirtualFunc131__Q22cf11CActorParamFv, (void*)CActorParam_getMoveRecord__Q22cf11CActorParamFv, (void*)CActorParam_getStatsSlot__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc134__Q22cf11CActorParamFv, // +0x2A0
    (void*)CActorParam_reinitBattleStats__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc136__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc137__Q22cf11CActorParamFv, (void*)CActorParam_isBattleLocked__Q22cf11CActorParamFv, // +0x2B0
    (void*)CActorParam_UnkVirtualFunc139__Q22cf11CActorParamFv, (void*)CActorParam_addHate__Q22cf13CfObjectActorFv, (void*)CActorParam_resetGaugeEntry__Q22cf11CActorParamFv, (void*)CActorParam_resetAllGaugeEntries__Q22cf11CActorParamFv, // +0x2C0
    (void*)CActorParam_UnkVirtualFunc143__Q22cf11CActorParamFv, (void*)CActorParam_findGaugeEntry__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc145__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc146__Q22cf11CActorParamFv, // +0x2D0
    (void*)CActorParam_UnkVirtualFunc147__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc148__Q22cf11CActorParamFv, (void*)CActorParam_findMaxGaugeEntry__Q22cf11CActorParamFv, (void*)CActorParam_findMinGaugeEntry__Q22cf11CActorParamFv, // +0x2E0
    (void*)CActorParam_UnkVirtualFunc151__Q22cf11CActorParamFv, (void*)CActorParam_getTensionStatus__Q22cf11CActorParamFv, (void*)CActorParam_accumulateTension__Q22cf11CActorParamFv, (void*)CActorParam_raiseTensionValue__Q22cf11CActorParamFv, // +0x2F0
    (void*)CActorParam_UnkVirtualFunc155__Q22cf11CActorParamFv, (void*)CActorParam_resetTensionState__Q22cf11CActorParamFv, (void*)CActorParam_getStatusCount__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc158__Q22cf11CActorParamFv, // +0x300
    (void*)CActorParam_UnkVirtualFunc159__Q22cf11CActorParamFv, (void*)CActorParam_initStatusCounter__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc161__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc162__Q22cf11CActorParamFv, // +0x310
    (void*)CActorParam_UnkVirtualFunc163__Q22cf11CActorParamFv, (void*)CActorParam_getActorModeFlags__Q22cf11CActorParamFv, (void*)CActorParam_getBattleHitFlags__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc166__Q22cf11CActorParamFv, // +0x320
    (void*)CActorParam_UnkVirtualFunc167__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc168__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc169__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc170__Q22cf11CActorParamFv, // +0x330
    (void*)CActorParam_UnkVirtualFunc171__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc172__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc173__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc174__Q22cf11CActorParamFv, // +0x340
    (void*)CActorParam_updateHateEntries__Q22cf11CActorParamFf, (void*)CActorParam_UnkVirtualFunc176__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc177__Q22cf11CActorParamFv, (void*)CActorParam_UnkVirtualFunc178__Q22cf11CActorParamFv, // +0x350
    (void*)CActorParam_UnkVirtualFunc179__Q22cf13CfObjectActorFv, (void*)CActorParam_UnkVirtualFunc180__Q22cf13CfObjectActorFv, (void*)CActorParam_UnkVirtualFunc181__Q22cf11CActorParamFv, &__RTTI__Q22cf13CfObjectActor, // +0x360
    (void*)0xFFFFCC80, (void*)CAIAction_UnkVirtualFunc1__Q22cf9CAIActionFv, (void*)CAIAction_UnkVirtualFunc2__Q22cf9CAIActionFv, &__RTTI__Q22cf13CfObjectActor, // +0x370
    (void*)0xFFFFC164, (void*)CObjectState_setStateBitMask__Q22cf12CObjectStateFv, (void*)CObjectState_checkStateFlags__Q22cf12CObjectStateFv, (void*)CObjectState_setStateBitFlag__Q22cf12CObjectStateFv, // +0x380
    (void*)CObjectState_clearStateWord4__Q22cf12CObjectStateFv, (void*)CObjectState_applyStateFlags__Q22cf12CObjectStateFv, (void*)CObjectState_clearStateWord8__Q22cf12CObjectStateFv, (void*)CObjectState_clearStateFlags8__Q22cf12CObjectStateFv, // +0x390
    (void*)CObjectState_UnkVirtualFunc8__Q22cf12CObjectStateFv, (void*)CObjectState_UnkVirtualFunc9__Q22cf12CObjectStateFv, (void*)CObjectState_setStateBitMask0__Q22cf12CObjectStateFv, (void*)CObjectState_getStateData__Q22cf12CObjectStateFv, // +0x3A0
    (void*)CObjectState_setStateBitMask2__Q22cf12CObjectStateFv, (void*)CObjectState_setStateBitMask3__Q22cf12CObjectStateFv, (void*)CObjectParam_UnkVirtualFunc1__Q22cf12CObjectParamFv, (void*)CObjectParam_getParamPtr__Q22cf13CfObjectActorFv, // +0x3B0
    (void*)CObjectParam_hasObjectName__Q22cf12CObjectParamFv, (void*)CObjectParam_UnkVirtualFunc4__Q22cf12CObjectParamFv, (void*)CObjectParam_getSelfObjectId__Q22cf12CObjectParamFv, (void*)CObjectParam_signalActionEnd__Q22cf12CObjectParamFv, // +0x3C0
    (void*)destroyActorParam__Q22cf13CfObjectActorFv, (void*)CfObject_UnkVirtualFunc2__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc3__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc4__Q22cf12CfObjectMoveFv, // +0x3D0
    (void*)CfObject_UnkVirtualFunc5__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc6__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc7__Q22cf12CfObjectMoveFv, (void*)CfObject_notifyEventDone__Q22cf13CfObjectModelFv, // +0x3E0
    (void*)CfObject_isMoveActiveNow__Q22cf12CfObjectMoveFv, (void*)CfObject_forwardSubObject__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc11__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc12__Q22cf12CfObjectMoveFv, // +0x3F0
    (void*)CfObject_queryTargetState__Q22cf12CfObjectMoveFv, (void*)CfObject_pushRefreshValue__Q22cf13CfObjectActorFf, (void*)CfObject_getMoveSpeedRate__Q22cf12CfObjectMoveFv, (void*)CfObject_pushRefreshExtra__Q22cf12CfObjectMoveFv, // +0x400
    (void*)CfObject_readRefreshValue__Q22cf12CfObjectMoveFv, (void*)CfObject_checkTargetState__Q22cf12CfObjectMoveFv, (void*)CfObject_setMoveTargetVec__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc20__Q22cf13CfObjectModelFv, // +0x410
    (void*)CfObject_UnkVirtualFunc21__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc22__Q22cf12CfObjectMoveFv, (void*)CfObject_getPosVector__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc24__Q22cf13CfObjectModelFv, // +0x420
    (void*)CfObject_UnkVirtualFunc25__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc26__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc27__Q22cf12CfObjectMoveFPv, (void*)CfObject_UnkVirtualFunc28__Q22cf13CfObjectModelFv, // +0x430
    (void*)CfObject_setMoveHeadAngle__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc30__Q22cf12CfObjectMoveFv, (void*)CfObject_getMoveHeadAngle__Q22cf13CfObjectActorFv, (void*)CfObject_UnkVirtualFunc32__Q22cf12CfObjectMoveFv, // +0x440
    (void*)CfObject_UnkVirtualFunc33__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc34__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc35__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc36__Q22cf13CfObjectModelFv, // +0x450
    (void*)CfObject_UnkVirtualFunc37__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc38__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc39__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc40__Q22cf12CfObjectMoveFv, // +0x460
    (void*)CfObject_UnkVirtualFunc41__Q22cf8CfObjectFv, (void*)CfObject_UnkVirtualFunc42__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc43__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc44__Q22cf8CfObjectFv, // +0x470
    (void*)CfObject_UnkVirtualFunc45__Q22cf12CfObjectMoveFv, (void*)CfObject_setMoveTargetPtr__Q22cf12CfObjectMoveFv, (void*)CfObject_createMoveTarget__Q22cf12CfObjectMoveFv, (void*)CfObject_getCurrentTarget__Q22cf12CfObjectMoveFv, // +0x480
    (void*)CfObject_forwardNpcAction__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc50__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc51__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc52__Q22cf13CfObjectModelFv, // +0x490
    (void*)CfObject_UnkVirtualFunc53__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc54__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc55__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc56__Q22cf13CfObjectModelFv, // +0x4A0
    (void*)CfObject_UnkVirtualFunc57__Q22cf12CfObjectMoveFv, (void*)CfObject_getMoveRateScale__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc59__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc60__Q22cf12CfObjectMoveFv, // +0x4B0
    (void*)CfObject_setAnimSlotEntry__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc62__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc63__Q22cf13CfObjectModelFv, (void*)CfObject_setMoveBusyState__Q22cf12CfObjectMoveFi, // +0x4C0
    (void*)CfObject_setMoveReadyFlag__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc66__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc67__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc68__Q22cf13CfObjectModelFv, // +0x4D0
    (void*)CfObject_UnkVirtualFunc69__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc70__Q22cf12CfObjectMoveFv, (void*)CfObject_UnkVirtualFunc71__Q22cf13CfObjectModelFv, (void*)CfObject_UnkVirtualFunc72__Q22cf13CfObjectModelFv, // +0x4E0
    (void*)CfObject_UnkVirtualFunc73__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc1__Q22cf12CfObjectMoveFv, (void*)CfObjectModel_UnkVirtualFunc2__Q22cf12CfObjectMoveFv, (void*)CfObjectModel_UnkVirtualFunc3__Q22cf13CfObjectModelFv, // +0x4F0
    (void*)CfObjectModel_UnkVirtualFunc4__Q22cf12CfObjectMoveFv, (void*)CfObjectModel_UnkVirtualFunc5__Q22cf12CfObjectMoveFv, (void*)CfObjectModel_UnkVirtualFunc6__Q22cf12CfObjectMoveFv, (void*)CfObjectModel_UnkVirtualFunc7__Q22cf13CfObjectModelFv, // +0x500
    (void*)CfObjectModel_UnkVirtualFunc8__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc9__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc10__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc11__Q22cf13CfObjectModelFv, // +0x510
    (void*)CfObjectModel_UnkVirtualFunc12__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc13__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc14__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc15__Q22cf13CfObjectModelFv, // +0x520
    (void*)CfObjectModel_UnkVirtualFunc16__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc17__Q22cf13CfObjectModelFv, (void*)CfObjectModel_UnkVirtualFunc18__Q22cf12CfObjectMoveFv, (void*)CfObjectModel_UnkVirtualFunc19__Q22cf12CfObjectMoveFv, // +0x530
    (void*)CfObjectModel_UnkVirtualFunc20__Q22cf13CfObjectModelFv, (void*)CfObjectMove_dropMoveRequest__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_detachModelList__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_releaseSlotById__Q22cf12CfObjectMoveFv, // +0x540
    (void*)CfObjectMove_recordMoveValue__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_getMovementRate__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_storeActionSrcId__Q22cf13CfObjectActorFv, (void*)CfObjectMove_refreshNpcState__Q22cf12CfObjectMoveFv, // +0x550
    (void*)CfObjectMove_updateNpcTarget__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_queryNpcAdvance__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_UnkVirtualFunc10__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_writeTargetField__Q22cf12CfObjectMoveFv, // +0x560
    (void*)CfObjectMove_advanceNpcTarget__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_commandNpcTarget__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_restartNpcTarget__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_hasActorScale__Q22cf13CfObjectActorFv, // +0x570
    (void*)CfObjectMove_attachEffectSlot__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_setEffectSlotBit__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_transferSlotBits__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_isEffectSlotUsed__Q22cf12CfObjectMoveFv, // +0x580
    (void*)CfObjectMove_signalEffectSlot__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_flushEffectSlots__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_setAllEffectBits__Q22cf12CfObjectMoveFv, (void*)CfObjectMove_loadResourceById__Q22cf12CfObjectMoveFv, // +0x590
    (void*)__dt__Q22cf13CfObjectActorFv, (void*)CfObjectActor_hasActorScale__Q22cf13CfObjectActorFv, (void*)CfObjectActor_clearStatusPair__Q22cf13CfObjectActorFv, (void*)CfObjectActor_flushStatusPair__Q22cf13CfObjectActorFv, // +0x5A0
    (void*)CfObjectActor_takeDamageValue__Q22cf13CfObjectActorFv, (void*)CfObjectActor_readFacingAngle__Q22cf13CfObjectActorFv, (void*)CfObjectActor_getAdjustedFacing__Q22cf13CfObjectActorFv, (void*)CfObjectActor_readActionScale__Q22cf13CfObjectActorFv, // +0x5B0
    (void*)CfObjectActor_sharesMoveFlags__Q22cf13CfObjectActorFv, (void*)CfObjectActor_pushRefreshValue__Q22cf13CfObjectActorFv, (void*)CfObjectActor_storeActionSrcId__Q22cf13CfObjectActorFv, (void*)CfObjectActor_getActionSrcId__Q22cf13CfObjectActorFv, // +0x5C0
    (void*)CfObjectActor_clearActionSrcId__Q22cf13CfObjectActorFv, // +0x5D0
};
} // extern "C"
