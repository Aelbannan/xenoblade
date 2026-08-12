#include "kyoshin/cf/object/CfObjectActor.hpp"
#include "kyoshin/code_802B8A3C.hpp"  // func_80174C98 / func_800B708C imports

namespace cf {
    /* TODO: find out what base class the static cast is
    casting down to */
    CfObjectActor::CfObjectActor() :
    CActorParam(static_cast<CObjectParam*>(this), nullptr) {
        
    }

float CfObjectActor::CfObjectActor_UnkVirtualFunc6() {
    // Float field at absolute offset 0x3EE8 within CfObjectActor
    // (0x4C within the CfObjectMove subobject at 0x3E9C)
    return *(float*)(reinterpret_cast<uintptr_t>(this) + 0x3EE8);
}
}

// Forward declarations for thunks
void __dt__Q22cf13CfObjectActorFv(void* self);
void CObjectParam_UnkVirtualFunc2__Q22cf12CfObjectMoveFv(void* self);

// Retail symbol is Fv; the real ABI passes (self, f1, r4). Delegates to the
// CActorParam_UnkVirtualFunc35 vtable slot (+0x120, takes (float, int, int,
// int)) with the float arg = value itself, passed through unchanged.
extern "C" void CfObjectActor_UnkVirtualFunc5__Q22cf13CfObjectActorFv(cf::CfObjectActor* self, float value, int arg) {
    if (value <= lbl_eu_80667738) {
        reinterpret_cast<cf::CfActorVt120*>(self)->m120(value, 0, 0, arg);
    } else {
        reinterpret_cast<cf::CfActorVt120*>(self)->m120(value, 3, 0, arg);
    }
}

float cf::CfObjectActor::CfObjectActor_UnkVirtualFunc8() {
    // Word at absolute offset 0x3E74 (CAIAction trailer area) is used as a
    // pointer to an object holding a float at +0x7C.
    cf::CfFloat7C* p = reinterpret_cast<cf::CfFloat7C*>(reinterpret_cast<cf::CfActorField3E74*>(this)->field_0x3E74);
    if (p == 0) return lbl_eu_8066776C;
    return p->field_0x7C;
}
void cf::CfObjectActor::CfObjectActor_UnkVirtualFunc9() {}
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

void cf::CfObjectActor::CfObjectActor_UnkVirtualFunc11(void* arg) {
    CfObjectActorData* data = reinterpret_cast<CfObjectActorData*>(this);
    void* p = data->mSomePtr;
    if (p != 0) {
        static_cast<CfObjectActorInner*>(p)->mArgStorage = arg;
    }
    data->mOtherPtr = arg;
}

void func_8016FF14(){}
void func_80170AB0(){}
void cf::CfObjectActor::CfObjectActor_UnkVirtualFunc3() {
    // Two virtual calls on the CBattleState subobject vtable (this+0x8),
    // slot +0x20 (CBattleState_UnkVirtualFunc7; the retail signature takes
    // an int). Raw offset cast: an upcast to the secondary base would make
    // MWCC emit a null guard for the adjusted this.
    reinterpret_cast<cf::CfBattleVt20*>((u8*)this + 8)->m20(0xf);
    reinterpret_cast<cf::CfBattleVt20*>((u8*)this + 8)->m20(0x10);
}
void cf::CfObjectActor::CfObjectActor_UnkVirtualFunc4() {
    // Two calls to the CBattleState subobject vtable slot +0x20 (retail
    // passes an int through the slot). Function-pointer form keeps the +8
    // subobject offset folded into the vtable load; a virtual call through a
    // cast would materialize this+0x8 into a register first.
    ((cf::CfBattleVt20Table*)((cf::CfActorBattleVtPtr*)(u8*)this)->vt)->fn20((u8*)this + 8, 0xf);
    ((cf::CfBattleVt20Table*)((cf::CfActorBattleVtPtr*)(u8*)this)->vt)->fn20((u8*)this + 0x8, 0x10);
}
float cf::CfObjectActor::CfObjectActor_UnkVirtualFunc7() {
    // Base height at 0x3EE8; if the move target (0x3F60) is set, add the
    // signed short value the CActParamData helper returns (s16 -> float via
    // MWCC's 2^52 double-trick, which emits the retail fsubs sequence).
    u8* p = reinterpret_cast<cf::CfActorField3F60*>(this)->field_0x3F60;
    if (p != 0) {
        float base = reinterpret_cast<cf::CfActorField3EE8*>(this)->field_0x3EE8;
        return base + (float)(s16)func_80055F94(p + 16, 0);
    }
    return reinterpret_cast<cf::CfActorField3EE8*>(this)->field_0x3EE8;
}

// Retail symbol is Fv; the real ABI passes (self, arg) and returns whether
// both actors share the CfObjectMove+0x64 flag-bit (2 or 4).
extern "C" int CfObjectActor_UnkVirtualFunc9__Q22cf13CfObjectActorFv(cf::CfObjectActor* self, cf::CfObjectActor* arg) {
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
extern "C" void CfObjectActor_UnkVirtualFunc10__Q22cf13CfObjectActorFv(cf::CfObjectActor* self, float value) {
    // Forward the move to the CfObjectMove subobject (+0x3E9C), then
    // re-dispatch this same virtual (slot +0x5C4) on the actor behind the
    // action-source handle when its +0x64 flags select it.
    cf::CfObjectMove* move = (cf::CfObjectMove*)((u8*)self + 16028);  // +0x3E9C
    move->CfObject_UnkVirtualFunc14(value);
    u8* p = reinterpret_cast<cf::CfActorField45B8*>(self)->field_0x45B8;
    if (p != 0) {
        cf::CfObjectMove* src = static_cast<cf::CfObjectMove*>(func_800B708C(reinterpret_cast<int>(p)));
        if (src != 0) {
            u32 flags = reinterpret_cast<cf::CfMoveFlags64*>(src)->field_0x64;
            cf::CfObjectActor* actor = ((flags & 2) != 0 || (flags & 4) != 0)
                ? (src != 0 ? (cf::CfObjectActor*)((u8*)src - 16028) : 0)
                : 0;
            if (actor != 0) {
                ((cf::CfActorVt5C4Table*)((cf::CfActorVtPtr*)(u8*)actor)->vt)->fn5C4((u8*)actor, value);
            }
        }
    }
}


struct IfE0 {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
    virtual void _v038(); virtual void _v03C(); virtual void _v040(); virtual void _v044();
    virtual void _v048(); virtual void _v04C(); virtual void _v050(); virtual void _v054();
    virtual void _v058(); virtual void _v05C(); virtual void _v060(); virtual void _v064();
    virtual void _v068(); virtual void _v06C(); virtual void _v070(); virtual void _v074();
    virtual void _v078(); virtual void _v07C(); virtual void _v080(); virtual void _v084();
    virtual void _v088(); virtual void _v08C(); virtual void _v090(); virtual void _v094();
    virtual void _v098(); virtual void _v09C(); virtual void _v0A0(); virtual void _v0A4();
    virtual void _v0A8(); virtual void _v0AC(); virtual void _v0B0(); virtual void _v0B4();
    virtual void _v0B8(); virtual void _v0BC(); virtual void _v0C0(); virtual void _v0C4();
    virtual void _v0C8(); virtual void _v0CC(); virtual void _v0D0(); virtual void _v0D4();
    virtual void _v0D8(); virtual void _v0DC();
    virtual void vfE0();
};
struct IfDC {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
    virtual void _v038(); virtual void _v03C(); virtual void _v040(); virtual void _v044();
    virtual void _v048(); virtual void _v04C(); virtual void _v050(); virtual void _v054();
    virtual void _v058(); virtual void _v05C(); virtual void _v060(); virtual void _v064();
    virtual void _v068(); virtual void _v06C(); virtual void _v070(); virtual void _v074();
    virtual void _v078(); virtual void _v07C(); virtual void _v080(); virtual void _v084();
    virtual void _v088(); virtual void _v08C(); virtual void _v090(); virtual void _v094();
    virtual void _v098(); virtual void _v09C(); virtual void _v0A0(); virtual void _v0A4();
    virtual void _v0A8(); virtual void _v0AC(); virtual void _v0B0(); virtual void _v0B4();
    virtual void _v0B8(); virtual void _v0BC(); virtual void _v0C0(); virtual void _v0C4();
    virtual void _v0C8(); virtual void _v0CC(); virtual void _v0D0(); virtual void _v0D4();
    virtual void _v0D8();
    virtual void vfDC();
};
extern "C" void CActorParam_UnkVirtualFunc23__Q22cf13CfObjectActorFv(cf::CfObjectActor* self) {
    reinterpret_cast<IfE0*>((u8*)self + 0x3e9c)->vfE0();
}

// CActorParam_UnkVirtualFunc21: store float then adjust-tail-call vt+0xDC
extern "C" void CActorParam_UnkVirtualFunc21__Q22cf13CfObjectActorFv(cf::CfObjectActor* self, float v) {
    *(float*)((u8*)self + 0x15e8) = v;
    reinterpret_cast<IfDC*>((u8*)self + 0x3e9c)->vfDC();
}


// Retail symbol is Fv; the real ABI passes (self, arg, f1, f2, f3). Compares
// the CfObjectModel.field_0x74 word (absolute offset 0x3F10) of both actors
// and tail-calls the base CActorParam implementation when they differ.
extern "C" void CActorParam_UnkVirtualFunc140__Q22cf13CfObjectActorFv(cf::CfObjectActor* self, cf::CfObjectActor* arg, float a, float b, float c) {
    if (arg == 0) return;
    if (reinterpret_cast<cf::CfActorField3F10*>(self)->field_0x3F10 == reinterpret_cast<cf::CfActorField3F10*>(arg)->field_0x3F10) return;
    CActorParam_UnkVirtualFunc140__Q22cf11CActorParamFv(self, arg, a, b, c);
}
// cf::CfObjectActor vtable thunks (retail: this-adjust + tail-branch)
extern "C" void CActorParam_UnkVirtualFunc1__Q22cf13CfObjectActorFv(void* self) {
    CObjectParam_UnkVirtualFunc2__Q22cf12CfObjectMoveFv((char*)self + 0x3e9c);
}
extern "C" void func_801725DC__Q22cf13CfObjectActorFv(void* self) {
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
        ((cf::CfObjectMove*)((u8*)self + 0x3E9C))->CfObject_UnkVirtualFunc70(lbl_eu_80667738);
        break;
    case 0x10: {
        cf::CfActorParamFields* f = reinterpret_cast<cf::CfActorParamFields*>(self);
        if ((f->field_0x3374 & 0x40000) != 0 && f->field_0x1634 != 0) {
            reinterpret_cast<cf::CfBattleVt20*>((u8*)self + 8)->m20((int)f->field_0x1634);
        }
        break;
    }
    default:
        break;
    }
    u8* obj3ED4 = reinterpret_cast<cf::CfActorField3ED4*>(self)->field_0x3ED4;
    reinterpret_cast<cf::CfSub3ED4Vt*>(obj3ED4)->m8C(arg);
}
// Retail symbol is Fv; the real ABI passes a float in f1. Rounds the input
// to the nearest int (fctiwz roundtrip), then - unless the presentation or
// mode flags are set - clamps the value up to lbl_eu_80667740 when the
// battle manager's +0x1A8 sub-object reports the actor, stores it at 0x17E8
// and releases a vision slot.
extern "C" void CActorParam_UnkVirtualFunc33__Q22cf13CfObjectActorFv(cf::CfObjectActor* self, float val) {
    // The direct cast makes MWCC synthesize the shared sdata2 magic double
    // (lbl_eu_80667748); the decomp pools an @N entry of the same value at
    // the same reloc site (name-only drift - accepted, cf. UnkVirtualFunc7).
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
extern "C" void CActorParam_UnkVirtualFunc54__Q22cf13CfObjectActorFv(cf::CfObjectActor* self, int delta) {
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
    u32* idPtr = reinterpret_cast<cf::CfActorUnk4Vt30*>(reinterpret_cast<cf::CfActorField04*>(self)->field_0x04)->vf30();
    u32 id = *idPtr;
    if (func_80174C98(self, &id, 0x802) != 0) {
        cf::CfActorParamVt168* vt = reinterpret_cast<cf::CfActorParamVt168*>(self);
        if (vt->m178() == vt->m174()) {
            vt->m168(vt->m178() - 1);
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
    int v = reinterpret_cast<cf::CfActorParamArg*>(arg)->field_0xC;
    switch (v) {
    case 0xE5:
        ((cf::CfObjectMove*)((u8*)self + 0x3E9C))->CfObject_UnkVirtualFunc70(lbl_eu_8066773C);
        break;
    case 0x10: {
        cf::CfActorParamFields* f = reinterpret_cast<cf::CfActorParamFields*>(self);
        if ((f->field_0x3374 & 0x40000) != 0 && f->field_0x1634 != 0) {
            cf::CfActor179Buf buf;
            std::memset(&buf, 0, 0x34);
            buf.field_0x00 = reinterpret_cast<cf::CfActorField3F10*>(self)->field_0x3F10;
            buf.field_0x04 = 0;
            buf.field_0x14 = (u16)f->field_0x1634;
            buf.field_0x18 = f->field_0x163C;
            buf.field_0x1C = (u16)f->field_0x1638;
            buf.field_0x1E = (u16)f->field_0x1640;
            buf.field_0x20 = (u16)f->field_0x1644;
            buf.field_0x28 = lbl_eu_80667738;
            buf.field_0x38 = 0x10001;
            reinterpret_cast<cf::CfBattleVt18*>((u8*)self + 8)->m18(&buf);
        }
        break;
    }
    default:
        break;
    }
    u8* obj3ED4 = reinterpret_cast<cf::CfActorField3ED4*>(self)->field_0x3ED4;
    reinterpret_cast<cf::CfSub3ED4Vt*>(obj3ED4)->m88(arg);
}
// Retail symbol is Fv; the real ABI passes (self, delta). Same gauge update
// as CActorParam_UnkVirtualFunc54 but on 0x1614/[0, 0x1616]; after the
// actor-id probe succeeds the update is applied a second time (the retail
// re-reads the fields), then if the gauge is at max (vf190 == vf18C) it is
// decremented via the vf180 slot.
extern "C" void CActorParam_UnkVirtualFunc60__Q22cf13CfObjectActorFv(cf::CfObjectActor* self, s16 delta) {
    cf::CfActorParamFields* f = reinterpret_cast<cf::CfActorParamFields*>(self);
    int d = (int)delta;
    s16 cur = f->field_0x1614;
    s16 max = f->field_0x1616;
    int sum = cur + d;
    f->field_0x1614 = (u16)sum;
    if ((s16)sum < 0) {
        f->field_0x1614 = 0;
    } else if ((s16)sum > max) {
        f->field_0x1614 = (u16)max;
    }
    u32* idPtr = reinterpret_cast<cf::CfActorUnk4Vt30*>(reinterpret_cast<cf::CfActorField04*>(self)->field_0x04)->vf30();
    u32 id = *idPtr;
    if (func_80174C98(self, &id, 0x802) != 0) {
        int d2 = (int)delta;
        s16 cur2 = f->field_0x1614;
        s16 max2 = f->field_0x1616;
        int sum2 = cur2 + d2;
        f->field_0x1614 = (u16)sum2;
        if ((s16)sum2 < 0) {
            f->field_0x1614 = 0;
        } else if ((s16)sum2 > max2) {
            f->field_0x1614 = (u16)max2;
        }
        cf::CfActorParamVt168* vt = reinterpret_cast<cf::CfActorParamVt168*>(self);
        if (vt->m190() == vt->m18C()) {
            vt->m180(vt->m190() - 1);
        }
    }
}
// Retail symbol is Fv; the real ABI passes the float in f1. Slot +0x120
// (CActorParam_UnkVirtualFunc35) takes (float, int, int, int); the float arg
// is value itself, passed through unchanged.
extern "C" void CActorParam_UnkVirtualFunc34__Q22cf13CfObjectActorFv(cf::CfObjectActor* self, float value) {
    if (value <= lbl_eu_80667738) {
        reinterpret_cast<cf::CfActorVt120*>(self)->m120(value, 0, 0, 0);
    } else {
        reinterpret_cast<cf::CfActorVt120*>(self)->m120(value, 3, 0, 0);
    }
}
extern "C" void CActorParam_UnkVirtualFunc35__Q22cf13CfObjectActorFv() {}

// CfObjectActor's override of the CActorParam virtual (slot 0xA4): queries
// the +0x04 sub-object's vtable slot +0x30 for the actor-id word, then pokes
// the move subobject's status chain when the id matches the query.
void cf::CfObjectActor::CActorParam_UnkVirtualFunc4() {
    CActorParam::CActorParam_UnkVirtualFunc6();
    u32* idPtr = reinterpret_cast<cf::CfActorUnk4Vt30*>(reinterpret_cast<cf::CfActorField04*>(this)->field_0x04)->vf30();
    u32 id = *idPtr;
    if (func_80174C98(this, &id, 0x1c) != 0) {
        func_800BE12C((u8*)this + 16028, 0x2f, 1, -1, 1);  // +0x3E9C: CfObjectMove subobject
    }
}

