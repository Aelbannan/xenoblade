#include "kyoshin/cf/CBattleManagerApi.hpp"
#include "kyoshin/cf/object/CfObjectActor.hpp"
#include "kyoshin/code_802B8A3C.hpp"  // findObjectById imports
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

// LOCAL views of the battle-status helpers for this TU: the constructor call
// site uses the 5-arg table ABI of func_80174B4C while other TUs use the
// common 2-arg form (the two extern "C" signatures cannot coexist in one TU -
// MWCC error 10197), so both helpers are declared here instead of including
// kyoshin/cf/CfMapItemManager.hpp.
extern "C" s32 func_80174C98(void* obj, void* outFlags, u32 id);
extern "C" void func_80174B4C(void* actor, u32 flags, const void* a, const void* b, const void* c);
extern "C" void CActorParam_UnkVirtualFunc6__Q22cf11CActorParamFv(void* self, int val);

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
    sinkf = *(volatile float*)&lbl_eu_80667770;
    sinkf = *(volatile float*)&lbl_eu_80667774;
    sinkf = *(volatile float*)&lbl_eu_80667778;
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
        u8* grp = lbl_eu_8053109C;
        *(void**)this = (void*)grp;
        *(void**)((u8*)this + 0x8) = (void*)(grp + 0xC);
        *(void**)((u8*)this + 0x3380) = (void*)(grp + 0x36C);
        *(void**)((u8*)this + 0x3E9C) = (void*)(grp + 0x37C);
        func_80174B4C(this, 3, grp + 0x36C, grp + 0xC, grp);
        func_8014AA10(reinterpret_cast<u8*>(this) + 0x3380, reinterpret_cast<unsigned int>(this));
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
extern "C" void func_8016FF14(void* obj, void* dst) { (void)obj; (void)dst; }
extern "C" void func_80170AB0(void* self, void* dst) { (void)self; (void)dst; }
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
    CActorParam_addHate__Q22cf11CActorParamFv(
        self, arg, static_cast<int>(selfId), a, b, c);
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
            if (func_8027990C((u8*)bm + 0x1A8, self) != 0) {
                f = lbl_eu_80667740;
            }
        }
        reinterpret_cast<cf::CfActorParamFields*>(self)->field_0x17E8 = f;
        func_801A891C(self, 0);
    }
}
// Retail symbol is Fv; the real ABI passes (self, delta). Adds a signed
// delta to the gauge at 0x160C, clamping to [0, 0x160E]. Then, when the
// actor-id probe (func_80174C98, flag 0x802) succeeds and the gauge sits at
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
    if (func_80174C98(self, (int*)&id, 0x802) != 0) {
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
    if (func_80174C98(self, (int*)&id, 0x802) != 0) {
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
        func_8010989C((c & (0x80000000 | 0x200)) != 0);
        func_80109888(0);
        func_80109874(0);
    } else {
        func_8010989C((c & (0x80000000 | 0x100)) != 0);
        func_80109888((c & (0x80000000 | 0x400)) != 0);
        if ((c & 0x80500000) != 0) {
            func_80109874(1);
        } else if ((c & 0x80A00000) != 0) {
            func_80109874(2);
        } else {
            func_80109874(0);
        }
    }
    if (f < lbl_eu_80667738 && func_80148778((u8*)self + 8, 1) != 0) {
        f = lbl_eu_80667738;
    }
    // Status 0x33: adjust the gauge value inside the status entry. `ep` is
    // one register reused across every status block (retail keeps the entry
    // pointers in a single callee-saved register).
    cf::CfStatusEntry* ep = 0;
    if (func_80148778((u8*)self + 8, 0x33) != 0) {
        if ((c & 0x90000000) == 0) {
            ep = (cf::CfStatusEntry*)func_80149154((u8*)self + 8, 0x33);
            if (ep != 0) {
                if ((c & (0x80000000 | 0x8000)) != 0) {
                    func_80109784((void*)reinterpret_cast<cf::CfActorField3F10*>(self)->field_0x3F10, 3, 0x10);
                    if (f > lbl_eu_80667738) {
                        ep->field_0x14 = (s16)(s32)f;
                        func_801A891C(self, 0);
                        return;
                    }
                    static_cast<cf::CBattleState*>((cf::CBattleState*)((u8*)self + 8))->CBattleState_applyEventEntry((cf::CBattleStateEntry*)ep);
                    if (lbl_eu_80667738 == f) return;
                } else {
                    if (f <= lbl_eu_80667738) {
                        ep->field_0x14 = (s16)((float)(s16)ep->field_0x14 + f);
                        func_80109784((void*)reinterpret_cast<cf::CfActorField3F10*>(self)->field_0x3F10, 3, 0x10);
                        if ((s16)ep->field_0x14 > 0) {
                            func_801A891C(self, 0);
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
    if (f > lbl_eu_80667738 && func_80148778((u8*)self + 8, 0x120) != 0) return;
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
    func_8010975C((u8)a);
    func_80109770((u8)b);
    if (f <= lbl_eu_80667738) {
        func_80109734((void*)reinterpret_cast<cf::CfActorField3F10*>(self)->field_0x3F10, (u32)(s32)-f);
        func_80277A7C((u8*)getInstance__Q22cf14CBattleManagerFv() + 0x1A8, self, -f);
    } else {
        func_80109734((void*)reinterpret_cast<cf::CfActorField3F10*>(self)->field_0x3F10, (u32)(s32)f);
    }
    if (f <= lbl_eu_80667738) {
        // Status 0xC3: scale a ratio from the entry value.
        if (func_80148778((u8*)self + 8, 0xC3) != 0) {
            ep = (cf::CfStatusEntry*)func_80149154((u8*)self + 8, 0xC3);
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
        if (func_80148778((u8*)self + 8, 0x92) != 0) {
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
                if (func_80148778((u8*)self + 8, 0xA1) != 0) {
                    v27b = ((cf::CfStatusEntry*)func_80149154((u8*)self + 8, 0xA1))->field_0x10;
                }
                ep = (cf::CfStatusEntry*)func_80149154((u8*)self + 8, 0x92);
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
        if (func_80148778((u8*)self + 8, 0xFC) != 0) {
            ep = (cf::CfStatusEntry*)func_80149154((u8*)self + 8, 0xFC);
            if (ep != 0) {
                if (self->CActorParam_getHp() < lbl_eu_80667740) {
                    float g = self->CActorParam_getDamageScale();
                    f = lbl_eu_80667764 * ((float)(s32)ep->field_0x10 * g);
                    getInstance__Q22cf14CBattleManagerFv();
                    f *= func_800D81A8(0, self, 0);
                    self->CActorParam_addHp(f);
                    static_cast<cf::CBattleState*>((cf::CBattleState*)((u8*)self + 8))->CBattleState_clearStatusId(0xFC);
                    func_8018C820((u8*)getInstance__Q22cf14CBattleManagerFv() + 0x194, 0x32);
                }
            }
        }
        // Status 0x100: same drain block plus a decay add and flag.
        if (func_80148778((u8*)self + 8, 0x100) != 0) {
            ep = (cf::CfStatusEntry*)func_80149154((u8*)self + 8, 0x100);
            if (ep != 0) {
                if (self->CActorParam_getHp() < lbl_eu_80667740) {
                    float g = self->CActorParam_getDamageScale();
                    f = lbl_eu_80667764 * ((float)(s32)ep->field_0x10 * g);
                    getInstance__Q22cf14CBattleManagerFv();
                    f *= func_800D81A8(0, self, 0);
                    self->CActorParam_addHp(f);
                    static_cast<cf::CBattleState*>((cf::CBattleState*)((u8*)self + 8))->CBattleState_clearStatusId(0x100);
                    self->CActorParam_addArtsGauge(lbl_eu_80667768);
                    func_800F38E0(getInstance__Q22cf14CBattleManagerFv(), self, 0x93);
                }
            }
        }
    }
    if (self->CActorParam_getHp() < lbl_eu_80667740) {
        if (func_8027990C((u8*)getInstance__Q22cf14CBattleManagerFv() + 0x1A8, self) != 0) {
            self->CActorParam_setHp(lbl_eu_80667740);
        }
    }
    func_801A891C(self, 0);
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
    ::CActorParam_UnkVirtualFunc6__Q22cf11CActorParamFv(self, (int)arts);
    u32* idPtr = reinterpret_cast<u32*>(
        reinterpret_cast<cf::CObjectState*>(
            reinterpret_cast<cf::CActorState*>(self)->unk4)
            ->CObjectState_getStateData());
    u32 id = *idPtr;
    if (func_80174C98(self, (int*)&id, 0x1c) != 0) {
        CfObjectMove_setAnimModeArgs((u8*)self + 16028, 0x2f, 1, -1, 1);  // +0x3E9C: CfObjectMove subobject
    }
}

// .data tables (retail 0x1C) as typed pointer tables. Reloc sites read as
// zero in the .o (relocs skipped when bytes match), so these stay MATCH.
// lbl_eu_80531080 is the CfMapEffectManager vtable whose bytes live in this
// split range (cf. CfMapEffectManager.hpp:101 decl); its dtor/RTTI owners
// are not decompiled yet, so those slots extern-ref their retail symbols.
void func_8016FC98();
void func_801A4194();
extern "C" void __dt__Q22cf18CfMapEffectManagerFv(void* self);
extern u8 lbl_eu_806623F0[];
__declspec(section ".data") __attribute__((used))
const void* lbl_eu_80531080[4] = {
    lbl_eu_806623F0, 0,
    (const void*)__dt__Q22cf18CfMapEffectManagerFv,
    (const void*)func_8016FC98
};
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
__declspec(section ".sdata2") __attribute__((used))
const float lbl_eu_80667770 = 0.0f;
__declspec(section ".sdata2") __attribute__((used))
const float lbl_eu_80667774 = 0.9f;
__declspec(section ".sdata2") __attribute__((used))
const float lbl_eu_80667778 = 0.35f;
