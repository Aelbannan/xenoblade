#include "kyoshin/cf/object/CfObject.hpp"
#include "monolib/scn/CScnTimeApi.hpp"
#include "kyoshin/cf/object/CfObjectPoint.hpp"
#include "kyoshin/cf/object/CObjectParam.hpp"
#include "kyoshin/code_801862C0.hpp"
#include "kyoshin/plugin/ocBdat.hpp"

#include <types.h>
#include <decomp.h>

namespace {
// Cast-only interfaces retain the retail virtual-call slots without adding
// storage or compiler-owned vtables to CfObjectPoint. novtable: these are
// never constructed or address-taken (only reinterpret_cast), so MWCC must
// not emit their vtables (retail has no .data in this unit).
struct __declspec(novtable) CfObjectPointParamIf {
    virtual void _v008();
    virtual void _v00C();
    virtual void _v010();
    virtual void _v014();
    virtual void _v018();
    virtual void _v01C();
    virtual void _v020();
    virtual void _v024();
    virtual void _v028();
    virtual void _v02C();
    virtual void _v030();
    virtual void _v034();
    virtual void _v038();
    virtual void setName(const char*);
};

struct __declspec(novtable) CfObjectPointChildIf {
    virtual void _v008(int);
    virtual void _v00C();
    virtual void _v010();
    virtual void _v014();
    virtual void _v018();
    virtual void _v01C();
    virtual void _v020();
    virtual void _v024();
    virtual void _v028();
    virtual void setOwner(void*);
    virtual void _v030();
    virtual void _v034();
    virtual void _v038();
    virtual void _v03C();
    virtual void _v040();
    virtual void _v044();
    virtual void _v048();
    virtual void _v04C();
    virtual void _v050();
    virtual void _v054();
    virtual void _v058();
    virtual void _v05C();
    virtual void _v060();
    virtual void _v064();
    virtual void _v068();
    virtual void _v06C();
    virtual void _v070();
    virtual void _v074();
    virtual void _v078();
    virtual void _v07C();
    virtual void _v080();
    virtual void _v084();
    virtual void _v088();
    virtual void _v08C();
    virtual void _v090();
    virtual void _v094();
    virtual void _v098();
    virtual void _v09C();
    virtual void afterAttach();
};

// Local tail view of the scene object (CScn): the ground-collision probe in
// func_800C171C reads the pointer at +0x74. The full CScn layout lives in
// per-TU copies across the codebase; this unit only needs this one slot.
struct CScnTailView {
    u8 _pad[0x74]; // 0x00-0x73
    u8* mPtr74;    // 0x74
};
}

namespace cf {

class CfObjectPoint; // forward declaration (field type below)

// Local vtable-layout replica of the retail CfObject base chain. The
// CObjectState/CObjectParam slots (0x08-0x50) and fields are inherited from
// the real headers; only the CfObject slots 0x54-0x174 are replicated here.
// Deliberately NOT the header's CfObject: MWCC would synthesise a base-dtor
// call for its user-provided dtor, and an inline copy would emit a 0x68
// orphan weak body the retail linker GC'd (over the unit split budget). The
// cfVt<offset> slot names below mirror the retail vtable exactly so virtual
// dispatch compiles identically. CfObjectBase declares no user-provided dtor
// (implicit trivial), so no standalone base-dtor body is ever emitted;
// ~CfObjectPoint writes the D2 phase by hand.
class __declspec(novtable) CfObjectBase : public CObjectParam {
public:
    // CfObject slots 0x54-0x174 (slot 0x54 is the base-dtor placeholder)
    virtual void cfVt54();                                   // 0x54
    virtual void cfVt58() = 0;                               // 0x58
    virtual void cfVt5C();                                   // 0x5C
    virtual void cfVt60() = 0;                               // 0x60
    virtual void cfVt64();                                   // 0x64
    virtual void cfVt68();                                   // 0x68
    virtual void cfVt6C() = 0;                               // 0x6C
    virtual void cfVt70() = 0;                               // 0x70
    virtual bool cfVt74();                                   // 0x74
    virtual void cfVt78();                                   // 0x78
    virtual void cfVt7C();                                   // 0x7C
    virtual void cfVt80();                                   // 0x80
    virtual void cfVt84();                                   // 0x84
    virtual u32 cfVt88(float value);                         // 0x88
    virtual void cfVt8C();                                   // 0x8C
    virtual u32 cfVt90(float value);                         // 0x90
    virtual void cfVt94();                                   // 0x94
    virtual void cfVt98();                                   // 0x98
    virtual void cfVt9C();                                   // 0x9C
    virtual void cfVtA0(float a, float b);                   // 0xA0
    virtual void cfVtA4();                                   // 0xA4
    virtual CfObject* cfVtA8();                              // 0xA8
    virtual u32 cfVtAC();                                    // 0xAC
    virtual void cfVtB0();                                   // 0xB0
    virtual void cfVtB4();                                   // 0xB4
    virtual void cfVtB8(u32 v, float amt);                   // 0xB8
    virtual void cfVtBC(u32 src);                            // 0xBC
    virtual void cfVtC0();                                   // 0xC0
    virtual void cfVtC4(float value);                        // 0xC4
    virtual void cfVtC8();                                   // 0xC8
    virtual float cfVtCC();                                  // 0xCC
    virtual void cfVtD0();                                   // 0xD0
    virtual void cfVtD4(float amount);                       // 0xD4
    virtual float cfVtD8();                                  // 0xD8
    virtual void cfVtDC();                                   // 0xDC
    virtual void cfVtE0();                                   // 0xE0
    virtual void cfVtE4();                                   // 0xE4
    virtual void cfVtE8();                                   // 0xE8
    virtual void cfVtEC();                                   // 0xEC
    virtual void cfVtF0();                                   // 0xF0
    virtual void cfVtF4();                                   // 0xF4
    virtual void cfVtF8();                                   // 0xF8
    virtual void cfVtFC();                                   // 0xFC
    virtual void cfVt100();                                  // 0x100
    virtual void cfVt104();                                  // 0x104
    virtual void cfVt108();                                  // 0x108
    virtual void cfVt10C();                                  // 0x10C
    virtual void cfVt110();                                  // 0x110
    virtual void cfVt114();                                  // 0x114
    virtual int cfVt118();                                   // 0x118
    virtual int cfVt11C();                                   // 0x11C
    virtual void cfVt120();                                  // 0x120
    virtual CfObject* cfVt124();                             // 0x124
    virtual void cfVt128();                                  // 0x128
    virtual void cfVt12C();                                  // 0x12C
    virtual float cfVt130();                                 // 0x130
    virtual void cfVt134();                                  // 0x134
    virtual u32* cfVt138();                                  // 0x138
    virtual void cfVt13C();                                  // 0x13C
    virtual void cfVt140();                                  // 0x140
    virtual void cfVt144();                                  // 0x144
    virtual void cfVt148();                                  // 0x148
    virtual void cfVt14C();                                  // 0x14C
    virtual void cfVt150(int flag);                          // 0x150
    virtual void cfVt154();                                  // 0x154
    virtual void cfVt158(int) = 0;                           // 0x158
    virtual void cfVt15C();                                  // 0x15C
    virtual void cfVt160() = 0;                              // 0x160
    virtual void cfVt164();                                  // 0x164
    virtual void cfVt168(float value);                       // 0x168
    virtual void cfVt16C();                                  // 0x16C
    virtual void cfVt170();                                  // 0x170
    virtual void cfVt174();                                  // 0x174

    // CfObject fields
    u8 field_0x28[0x10];   // 0x28-0x37
    CfObjectPoint* mSubObj38; // 0x38-0x3B
    float mPos3C;          // 0x3C-0x3F
    float mPos40;          // 0x40-0x43
    float mPos44;          // 0x44-0x47
    u8 _pad48[4];          // 0x48-0x4B
    float mField4C;        // 0x4C-0x4F
    u8 _pad50[4];          // 0x50-0x53
    CfObjectSub54 mSubObj54; // 0x54-0x5F
    float mFloat60;        // 0x60-0x63
    u32 field_0x64;        // 0x64-0x67
    u32 mFlags68;          // 0x68-0x6B
    u8 _pad6C[4];          // 0x6C-0x6F
};

class __declspec(novtable) CfObjectPoint : public CfObjectBase {
public:
    virtual ~CfObjectPoint();
    void func_800C1324();
    void func_800C136C();
    void func_800C1444();
    void func_800C14CC();
    void func_800C1638();

    u8* mPtr70;       // 0x70-0x73
    u8 _pad74[4];     // 0x74-0x77
    char mName[0x14]; // 0x78-0x8B
    u16 mIndex8C;     // 0x8C-0x8D
    u8 _pad8E[2];     // 0x8E-0x8F
    u8 mFlag90;       // 0x90
    u8 mFlag91;       // 0x91
};

} // namespace cf

// Genuine C-linkage function: the retail symbol is the bare unmangled name
// (no __Q2 class qualifier), so it must be defined with C linkage. Other TUs
// (CfGimmick, CfMapMineManager) call it through their own C-linkage
// declarations; the first parameter is the object pointer.
extern "C" void func_800C13FC(cf::CfObjectPoint* self, u32 a, u8 val) {
    self->CObjectParam_UnkVirtualFunc1(a, val);
    self->mFlag91 = val;
}

namespace cf {

CfObjectPoint::~CfObjectPoint() {
    // D1 vtable store is explicit (novtable class suppresses MWCC's implicit
    // store); the null guard and delete-flag guard are auto-generated. The
    // flags word is read before the vtable store, matching retail's schedule.
    u32 flags = mFlags68;
    *(void**)this = reinterpret_cast<void*>(lbl_eu_8052A3B0);
    if (flags & 0x40000000) {
        ((void* (*)(void*, void*))func_80186474)(func_801862C0(), this);
    }
    cfVt68();
    // D2 phase (base class): CfObjectBase keeps an implicit trivial dtor so
    // MWCC emits no standalone base-dtor body (a user-provided one would add
    // a 0x68 orphan weak copy the retail linker GC'd, over the split budget).
    // The null guard + CfObject vtable store + slot-0x68 destroy are written
    // out, reproducing retail's inlined D2 block exactly.
    if (this != 0) {
        *(void**)this = reinterpret_cast<void*>(lbl_eu_805294E0);
        cfVt68();
    }
}

void CfObjectPoint::func_800C1324() {
    cfVt158(1);
    mFlag91 = 0;
    mFlag90 = 0;
}

void CfObjectPoint::func_800C136C() {
    func_8003AA34();
    u8* fp = (u8*)getFP(mName);
    u32 name = getBdatStringColumnValue(fp, lbl_eu_804FC648, mIndex8C);
    // Slot 0x3C is called with one argument here (retail sets r4 only).
    reinterpret_cast<CfObjectPointParamIf*>(this)->setName(
        reinterpret_cast<const char*>(name));
    u32 val = getBdatStringColumnValue(fp, lbl_eu_804FC648 + 5, mIndex8C);
    mFlag91 = *(u8*)&val;
}

void CfObjectPoint::func_800C1444() {
    if (mSubObj38 != nullptr) {
        CfObjectPoint* child = mSubObj38;
        child->cfVtAC();
        if (mSubObj38 != nullptr) {
            // Redundant nested check on the reloaded value mirrors retail's
            // two beq targets (MWCC keeps both branches).
            if (mSubObj38 != nullptr) {
                reinterpret_cast<CfObjectPointChildIf*>(mSubObj38)->_v008(1);
            }
            mSubObj38 = nullptr;
        }
    }
    mPtr70 = nullptr;
    mFlags68 &= 0x40000000;
}

void CfObjectPoint::func_800C14CC() {
    CObjectState_UnkVirtualFunc13();

    if ((lbl_eu_80663E24 & 0x01000000) != 0) {
        if (CObjectState_UnkVirtualFunc8(1) != 0) {
            goto main_body;
        }
        if (CObjectState_UnkVirtualFunc2(1) == 0) {
            goto done;
        }
    }

main_body:
    {
        float dist = func_80496288(lbl_eu_80663E14);
        if (dist > lbl_eu_80666B48) {
            if (CObjectState_UnkVirtualFunc8(1) != 0) {
                if (CObjectState_UnkVirtualFunc2(1) == 0) {
                    if (func_8013EB90(1) == 0) {
                        CObjectState_UnkVirtualFunc7(1);
                    }
                }
            }
        }
    }

    if (CObjectState_UnkVirtualFunc2(1) != 0) {
        CObjectState_UnkVirtualFunc5(1);
        CObjectState_UnkVirtualFunc3(1);
    }

    if (mSubObj38 != nullptr) {
        ((CfObject*)mSubObj38)->CfObject_UnkVirtualFunc21();
    }

done:
    return;
}

void CfObjectPoint::func_800C1638() {
    if (mSubObj38 == nullptr) return;
    ((CfObject*)mSubObj38)->CfObject_UnkVirtualFunc22();
}

} // namespace cf

// Forced-name form: the retail symbols func_800C1658/16F4/171C end in a
// decompiler-guessed "Fv" (the bodies consume r4/f1/f2 arguments), and the
// symbols.txt entries cannot be corrected to the true signatures (FPv/Fi/Fff)
// from this TU's writable scope. A real member would mangle to the corrected
// name, so these stay C-linkage functions with the mangled retail symbol as
// the identifier to emit the exact symbol.
extern "C" void func_800C1658__Q22cf13CfObjectPointFv(
    cf::CfObjectPoint* self, cf::CfObjectPoint* child) {
    cf::CfObjectPoint* old = self->mSubObj38;
    if (old != nullptr) {
        if (old != nullptr) {
            reinterpret_cast<CfObjectPointChildIf*>(old)->_v008(1);
        }
        self->mSubObj38 = nullptr;
    }

    self->mSubObj38 = child;
    if (child != nullptr) {
        reinterpret_cast<CfObjectPointChildIf*>(child)->setOwner(self);
        reinterpret_cast<CfObjectPointChildIf*>(self->mSubObj38)->afterAttach();
    }
}

extern "C" void func_800C16F4__Q22cf13CfObjectPointFv(
    cf::CfObjectPoint* self, int enable) {
    if (enable != 0) {
        self->mFlags68 |= 0x00100000;
    } else {
        self->mFlags68 = DECOMP_PPC_RLWINM(self->mFlags68, 0, 12, 10);
    }
}

extern "C" void func_800C171C__Q22cf13CfObjectPointFv(
    cf::CfObjectPoint* self, float x, float z) {
    // The const-qualified SDA float (see CfObjectPoint.hpp) lets MWCC hoist
    // the lfs above the prologue LR store (retail schedule); a non-const
    // declaration pins it after the global-pointer load.
    float y = lbl_eu_80666B4C;
    float pos[3];
    float first[3];
    float second[3];
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;

    if (lbl_eu_80663E14 == nullptr) {
        u8* source = reinterpret_cast<CScnTailView*>(lbl_eu_80663E14)->mPtr74;
        if (func_8049E51C(source) != 0) {
            if (func_804BE398(pos, 0, 0, 0,
                              lbl_eu_80666B50, lbl_eu_80666B48) != nullptr) {
                func_804BE4B4(first, 0);
                func_804BE4E0(second, 0);
                ((u32*)pos)[0] = ((u32*)first)[0];
                ((u32*)pos)[1] = ((u32*)first)[1];
                ((u32*)pos)[2] = ((u32*)first)[2];
            }
        }
    }

    // Word-copy pos into the object's position fields; retail stores 0x40
    // before 0x3c (assignment order), holding pos[0] in a register across.
    u32 p0 = *(u32*)&pos[0];
    *(u32*)&self->mPos40 = *(u32*)&pos[1];
    *(u32*)&self->mPos3C = p0;
    *(u32*)&self->mPos44 = *(u32*)&pos[2];
}
