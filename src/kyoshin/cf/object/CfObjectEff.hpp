#pragma once

#include <types.h>
#include "kyoshin/cf/object/CfObject.hpp"

// Retail data symbols (linker-provided)
// Retail data symbols (linker-provided)
extern const float lbl_eu_8066A210;  // final position scale (pi/2)
extern const float lbl_eu_8066A20C;  // effect position scale
extern const float lbl_eu_80666960;  // X component baked into the move vector (func_800ACD5C)
extern const float lbl_eu_80666964;  // scale passed to the vtable 0xB4 slot (func_800ACD5C)
extern u8 lbl_eu_80528870[];         // CfObjectEff vtable (retail data, 0x1A8 bytes)
extern u8 lbl_eu_805294E0[];         // CfObject vtable (retail .data at 0x805294E0) -
                                     // stored by the inlined base ctor
extern u8 __vt__Q22cf8CfObject[];    // compiler-generated name for the same CfObject
                                     // vtable; the devirtualized slot-0x5C call in the
                                     // ctor references it
extern const float lbl_eu_80666958;  // default position/scale fill (CfObjectEff ctor)
extern const float lbl_eu_8066695C;  // default mFloat60 fill (CfObjectEff ctor)

// C-ABI imports used by func_800AD060. These retail symbols are unmangled
// C names, so they are declared extern "C" (as CREvtEffect.hpp does) - a
// plain C++ declaration would make MWCC append a mangling suffix to the
// reloc. func_8006EF04__Fi is declared in kyoshin/cf/code_800F42AC.hpp
// (extern "C") and included by the cpp.
extern "C" {
    void* func_800630C8();
    void* func_80063038();
    void* func_80066E7C(void* self, u32 id);
    void* func_80066CF8(void* self);
    void* func_800BB340(void* obj);
    void* func_804E3CFC(void* effect);
    int CfRes_getD80Flag();
}

// RTTI typeinfos + C-ABI cast helper for func_800AD4B0 (same pair as
// CTaskGameEff/CBattleManager: cast the chain object from the type at
// lbl_eu_80661970 down to its base at lbl_eu_806618F0).
extern "C" const void* lbl_eu_80661970;
extern "C" const void* lbl_eu_806618F0;
extern "C" void* __dynamic_cast(void* obj, long offset, const void* src_type,
                                const void* dst_type, void* src2dst);
// C-linkage validity check used by func_800AD3A4/func_800AD68C (returns the
// object when it is still live, 0 when it has been released).
extern "C" void* func_800B8920(void* obj);

namespace cf {
class CfObjectEff;  // forward decl for the vtable-view structs below
}

// Pointer-to-member-function dispatch table used by func_800AD3A4: indexed by
// the u16 counter at +0x8E, each entry is a 12-byte ptmf (retail .data at
// lbl_eu_80528858, 0x18 bytes before the CfObjectEff vtable). Declared at
// global scope so MWCC keeps the C symbol name unmangled.
extern void (cf::CfObjectEff::*const lbl_eu_80528858[])();

namespace cf {

// Vtable view used by func_800AD3A4: slot 0x160 returns a status word whose
// bit 3 is synced into the child's u16 bit 1. Slot +0x160 is occupied by
// CfObjectEff::func_800AD4A4 (an override of the CfObject pure slot that
// CfObject.hpp declares with a placeholder `void()` signature - the retail
// function returns a status word), so the call cannot yet be spelled as a
// base-name virtual until CfObject.hpp's signature is corrected.
struct CfObjectEffVtable160If {
    virtual void _f08();
    virtual void _f0C();
    virtual void _f10();
    virtual void _f14();
    virtual void _f18();
    virtual void _f1C();
    virtual void _f20();
    virtual void _f24();
    virtual void _f28();
    virtual void _f2C();
    virtual void _f30();
    virtual void _f34();
    virtual void _f38();
    virtual void _f3C();
    virtual void _f40();
    virtual void _f44();
    virtual void _f48();
    virtual void _f4C();
    virtual void _f50();
    virtual void _f54();
    virtual void _f58();
    virtual void _f5C();
    virtual void _f60();
    virtual void _f64();
    virtual void _f68();
    virtual void _f6C();
    virtual void _f70();
    virtual void _f74();
    virtual void _f78();
    virtual void _f7C();
    virtual void _f80();
    virtual void _f84();
    virtual void _f88();
    virtual void _f8C();
    virtual void _f90();
    virtual void _f94();
    virtual void _f98();
    virtual void _f9C();
    virtual void _fA0();
    virtual void _fA4();
    virtual void _fA8();
    virtual void _fAC();
    virtual void _fB0();
    virtual void _fB4();
    virtual void _fB8();
    virtual void _fBC();
    virtual void _fC0();
    virtual void _fC4();
    virtual void _fC8();
    virtual void _fCC();
    virtual void _fD0();
    virtual void _fD4();
    virtual void _fD8();
    virtual void _fDC();
    virtual void _fE0();
    virtual void _fE4();
    virtual void _fE8();
    virtual void _fEC();
    virtual void _fF0();
    virtual void _fF4();
    virtual void _fF8();
    virtual void _fFC();
    virtual void _f100();
    virtual void _f104();
    virtual void _f108();
    virtual void _f10C();
    virtual void _f110();
    virtual void _f114();
    virtual void _f118();
    virtual void _f11C();
    virtual void _f120();
    virtual void _f124();
    virtual void _f128();
    virtual void _f12C();
    virtual void _f130();
    virtual void _f134();
    virtual void _f138();
    virtual void _f13C();
    virtual void _f140();
    virtual void _f144();
    virtual void _f148();
    virtual void _f14C();
    virtual void _f150();
    virtual void _f154();
    virtual void _f158();
    virtual void _f15C();
    virtual bool func160();  // slot 0x160 - returns a 0/1 status flag
};

// Vtable view used by func_800AD68C: slot 0xC on the mFieldB0 object receives
// the owning CfObjectEff. mFieldB0 is a foreign object (not part of the
// CfObjectEff tree), so this stays a small named iface on the owner.
struct CfObjectEffVtable0CIf {
    virtual void _f08();
    virtual void func0C(CfObjectEff* owner);  // slot 0xC
};

// Vtable view used by func_800AD68C: slot 0x1BC receives the owning
// CfObjectEff. The 0x9C/0xA0 partners are foreign CfObject-family objects,
// so this stays a small named iface on the owner.
struct CfObjectEffVtable1BCIf {
    virtual void _f08();
    virtual void _f0C();
    virtual void _f10();
    virtual void _f14();
    virtual void _f18();
    virtual void _f1C();
    virtual void _f20();
    virtual void _f24();
    virtual void _f28();
    virtual void _f2C();
    virtual void _f30();
    virtual void _f34();
    virtual void _f38();
    virtual void _f3C();
    virtual void _f40();
    virtual void _f44();
    virtual void _f48();
    virtual void _f4C();
    virtual void _f50();
    virtual void _f54();
    virtual void _f58();
    virtual void _f5C();
    virtual void _f60();
    virtual void _f64();
    virtual void _f68();
    virtual void _f6C();
    virtual void _f70();
    virtual void _f74();
    virtual void _f78();
    virtual void _f7C();
    virtual void _f80();
    virtual void _f84();
    virtual void _f88();
    virtual void _f8C();
    virtual void _f90();
    virtual void _f94();
    virtual void _f98();
    virtual void _f9C();
    virtual void _fA0();
    virtual void _fA4();
    virtual void _fA8();
    virtual void _fAC();
    virtual void _fB0();
    virtual void _fB4();
    virtual void _fB8();
    virtual void _fBC();
    virtual void _fC0();
    virtual void _fC4();
    virtual void _fC8();
    virtual void _fCC();
    virtual void _fD0();
    virtual void _fD4();
    virtual void _fD8();
    virtual void _fDC();
    virtual void _fE0();
    virtual void _fE4();
    virtual void _fE8();
    virtual void _fEC();
    virtual void _fF0();
    virtual void _fF4();
    virtual void _fF8();
    virtual void _fFC();
    virtual void _f100();
    virtual void _f104();
    virtual void _f108();
    virtual void _f10C();
    virtual void _f110();
    virtual void _f114();
    virtual void _f118();
    virtual void _f11C();
    virtual void _f120();
    virtual void _f124();
    virtual void _f128();
    virtual void _f12C();
    virtual void _f130();
    virtual void _f134();
    virtual void _f138();
    virtual void _f13C();
    virtual void _f140();
    virtual void _f144();
    virtual void _f148();
    virtual void _f14C();
    virtual void _f150();
    virtual void _f154();
    virtual void _f158();
    virtual void _f15C();
    virtual void _f160();
    virtual void _f164();
    virtual void _f168();
    virtual void _f16C();
    virtual void _f170();
    virtual void _f174();
    virtual void _f178();
    virtual void _f17C();
    virtual void _f180();
    virtual void _f184();
    virtual void _f188();
    virtual void _f18C();
    virtual void _f190();
    virtual void _f194();
    virtual void _f198();
    virtual void _f19C();
    virtual void _f1A0();
    virtual void _f1A4();
    virtual void _f1A8();
    virtual void _f1AC();
    virtual void _f1B0();
    virtual void _f1B4();
    virtual void _f1B8();
    virtual void func1BC(CfObjectEff* owner);  // slot 0x1BC
};

// Minimal view of the argument passed to func_800AD68C: only the +0x14 word
// (validity flag) is read.
struct CfObjectEffArg14View {
    u8 _pad00[0x14];
    u32 field_14;  // 0x14
};

// Raw u16 view of CfObjectEffChild's flag word at +0x0: func_800AD3A4 reads
// and writes the whole halfword (bit 14), not individual bitfields.
struct CfObjectEffChildFlagsView {
    u16 field_00;  // 0x00
};

// Minimal 3-component float vector used for the position/rotation copies.
struct CfObjectEffVec3 {
    float x;  // 0x00
    float y;  // 0x04
    float z;  // 0x08
};

// 2-word view for paired word copies (func_800ACCE4 copies x/y as one
// 8-byte struct assignment, producing lwz/lwz/stw/stw).
struct CfObjectEffU32Vec2 {
    u32 x;  // 0x00
    u32 y;  // 0x04
};

// 3-word vector used for raw word copies (func_800ACC94/func_800ACDA0): the
// destination float fields are copied as words, so MWCC emits lwz/stw.
struct CfObjectEffU32Vec3 {
    u32 x;  // 0x00
    u32 y;  // 0x04
    u32 z;  // 0x08
};

// Vtable view used by func_800ACD5C: slot 0xB4 receives a float-array
// pointer (r4) and a float scale (f1). Slot +0xB4 is filled by
// CfObjectEff::func_800ACCE4 (a CfObject-chain slot whose placeholder
// signature in CfObject.hpp does not take these arguments), so the call
// cannot yet be spelled as a base-name virtual. MWCC puts the Nth declared
// virtual at vtable offset (N+1)*4 (slots 0x0/0x4 reserved), so the 43
// fillers land moveB4 on 0xB4.
struct CfObjectEffMoveIf {
    virtual void _f08();
    virtual void _f0C();
    virtual void _f10();
    virtual void _f14();
    virtual void _f18();
    virtual void _f1C();
    virtual void _f20();
    virtual void _f24();
    virtual void _f28();
    virtual void _f2C();
    virtual void _f30();
    virtual void _f34();
    virtual void _f38();
    virtual void _f3C();
    virtual void _f40();
    virtual void _f44();
    virtual void _f48();
    virtual void _f4C();
    virtual void _f50();
    virtual void _f54();
    virtual void _f58();
    virtual void _f5C();
    virtual void _f60();
    virtual void _f64();
    virtual void _f68();
    virtual void _f6C();
    virtual void _f70();
    virtual void _f74();
    virtual void _f78();
    virtual void _f7C();
    virtual void _f80();
    virtual void _f84();
    virtual void _f88();
    virtual void _f8C();
    virtual void _f90();
    virtual void _f94();
    virtual void _f98();
    virtual void _f9C();
    virtual void _fA0();
    virtual void _fA4();
    virtual void _fA8();
    virtual void _fAC();
    virtual void _fB0();
    virtual void moveB4(const float* vec, float scale);  // vtable 0xB4
};

// View of the CfObjectEff tail used by func_800ACDA0: the 0x48-0x50 region
// (spanning mField4C) is one 12-byte vector copied as a struct.
struct CfObjectEffVec48View {
    u8 _pad00[0x48];
    CfObjectEffU32Vec3 vec48;  // 0x48
};

struct CfObjectEffChild {
    u16 unk0HighTop : 1; // bit 15
    u16 flag4000 : 1;    // bit 14 (0x4000) - synced with slot-0x160 status
    u16 unk0High : 3;    // bits 13-11
    u16 flag400 : 1;     // bit 10 (0x400) - assigned from a flag in func_800ACBCC
    u16 unk0Low : 10;    // bits 9-0
    u8 _pad02[0x14 - 0x02];
    u8* field_14;        // 0x14 - partner object (func_800ACF78)
    u8* field_18;        // 0x18 - resolved partner object (func_800ACFD8)
    u32 field_1C[3];     // 0x1C-0x27 - word-copied from a position vector (func_800ACC94)
    float field_28;      // 0x28
    float unk2C;         // 0x2C
    float field_30;      // 0x30
    u32 unk34[3];
    u32 unk40[4];
    u8 _pad50[0x59 - 0x50];
    u8 unk59;
    u8 _pad5A[0x5C - 0x5A];
    int unk5C;
};

// View of the CfObjectEffChild 0x34-0x3C region used by func_800ACEAC: the
// same float bits are word-copied into all three slots.
struct CfObjectEffChild34View {
    u8 _pad00[0x34];
    u32 field_34[3];  // 0x34-0x3C
};

// Minimal view of the CfObject-family sub-object checked by func_800AD860:
// the owner object starts 0x3E9C bytes before it, and only the +0x64 flag
// word (CfObject::unk64) is read (bit 4 = alive/enabled).
struct CfObjectEffSubView {
    u8 _pad64[0x64];
    u32 field_0x64;
};

// Byte-exact view of the CfObjectEff base region as the retail ctor chain
// clears it: 0x10 is a byte store, 0x30/0x34 words, the 0x3C-0x60 float
// block, and the 0x68-0x8E tail words/bytes. 0x64 is deliberately skipped.
struct CfObjectEffCtorView {
    u8 _pad00[0x04];
    u32 field_04;      // 0x04
    u32 field_08;      // 0x08
    u32 field_0C;      // 0x0C
    u8 field_10;       // 0x10 (byte store)
    u8 _pad11[0x30 - 0x11];
    u32 field_30;      // 0x30
    u32 field_34;      // 0x34
    u32 field_38;      // 0x38 (mSubObj38)
    float field_3C;    // 0x3C
    float field_40;    // 0x40
    float field_44;    // 0x44
    float field_48;    // 0x48
    float field_4C;    // 0x4C
    float field_50;    // 0x50
    float field_54;    // 0x54
    float field_58;    // 0x58
    float field_5C;    // 0x5C
    float field_60;    // 0x60 (mFloat60)
    u32 field_64;      // 0x64 (not cleared by the ctor)
    u32 field_68;      // 0x68 (mFlags68)
    u32 field_6C;      // 0x6C
    u32 field_70;      // 0x70
    u32 field_74;      // 0x74
    u8 field_78;       // 0x78 (byte store)
    u8 _pad79[0x88 - 0x79];
    u32 field_88;      // 0x88
    u16 field_8C;      // 0x8C
    u16 field_8E;      // 0x8E
};

// Vtable view for func_800AD060's owner-side dispatch: slots 0x3C (effect
// lookup result), 0x9C/0xBC (position/rotation vectors) and 0xDC (scale
// float) receive the shown arguments.
struct CfObjectEffOwnerIf {
    virtual void _f08();
    virtual void _f0C();
    virtual void _f10();
    virtual void _f14();
    virtual void _f18();
    virtual void _f1C();
    virtual void _f20();
    virtual void _f24();
    virtual void _f28();
    virtual void _f2C();
    virtual void _f30();
    virtual void _f34();
    virtual void _f38();
    virtual void vf3C(void* arg);   // slot 0x3C - effect lookup result
    virtual void _f40();
    virtual void _f44();
    virtual void _f48();
    virtual void _f4C();
    virtual void _f50();
    virtual void _f54();
    virtual void _f58();
    virtual void _f5C();
    virtual void _f60();
    virtual void _f64();
    virtual void _f68();
    virtual void _f6C();
    virtual void _f70();
    virtual void _f74();
    virtual void _f78();
    virtual void _f7C();
    virtual void _f80();
    virtual void _f84();
    virtual void _f88();
    virtual void _f8C();
    virtual void _f90();
    virtual void _f94();
    virtual void _f98();
    virtual void vf9C(void* arg);   // slot 0x9C
    virtual void _fA0();
    virtual void _fA4();
    virtual void _fA8();
    virtual void _fAC();
    virtual void _fB0();
    virtual void _fB4();
    virtual void _fB8();
    virtual void vfBC(void* arg);   // slot 0xBC
    virtual void _fC0();
    virtual void _fC4();
    virtual void _fC8();
    virtual void _fCC();
    virtual void _fD0();
    virtual void _fD4();
    virtual void _fD8();
    virtual void vfDC(float value); // slot 0xDC
};

// Minimal view of the effect-source objects (BDAT/CfRes entries) used by
// func_800AD060: the +0xC word gates the 0x10/0x11 types, and +0x2C is a
// sub-object whose vtable slot 0x40 validates the entry.
struct CfObjectEffSourceView {
    u8 _pad00[0xC];
    u32 field_0C;      // 0x0C
    u8 _pad10[0x2C - 0x10];
    void* field_2C;    // 0x2C
};

struct CfObjectEffSourceSubIf {
    virtual void _f08();
    virtual void _f0C();
    virtual void _f10();
    virtual void _f14();
    virtual void _f18();
    virtual void _f1C();
    virtual void _f20();
    virtual void _f24();
    virtual void _f28();
    virtual void _f2C();
    virtual void _f30();
    virtual void _f34();
    virtual void _f38();
    virtual void _f3C();
    virtual u32 func40(void* obj);  // slot 0x40
};

class __declspec(novtable) CfObjectEff : public CfObject {
public:
    u32 mField70;                // 0x70 - packed type/id (top 5 bits = type, func_800AD060)
    u8 _pad74[0x8C - 0x74];
    u16 mCount8C;                // 0x8C - effect count (func_800AD060 passes count-1)
    u16 mCount8E;                // 0x8E - ptmf-table dispatch counter (func_800AD3A4)
    u8* mSubObj90;               // 0x90 - secondary vtable (CfObjectEff vtable + 0x178)
    CfObjectEffChild* mChildEff;  // 0x94
    u8* mField98;            // 0x98 - object (cleared with mField9C in func_800ACA58)
    u8* mField9C;            // 0x9C - object (func_800ACF78)
    u8* mFieldA0;            // 0xA0 - target (func_800ACFD8)
    u16 mFlagsA4;
    u16 mCountA6;
    u8* mFieldA8;            // 0xA8 - source object (func_800AD060)
    u8* mFieldAC;            // 0xAC - partner (func_800ACF78)
    u8* mFieldB0;            // 0xB0 - object cleared by func_800AD68C (slot-0xC dispatch)
    u8 mFieldB4;             // 0xB4 - effect bound flag (func_800AD060)
    u8 _padB5[0xB8 - 0xB5];
    u8* mFieldB8;            // 0xB8 - detached target (func_800AD558)
    u8* mFieldBC;            // 0xBC - effect manager override (func_800AD060)

    CfObjectEff();
    void CfObject_UnkVirtualFunc29();
    bool func_800AC7CC();
    void func_800AC7FC();
    void func_800AC810();
    void func_800AC86C();
    void func_800AC990();
    void func_800ACA58();
    void func_800ACAE8();
    void func_800ACB08();
    void func_800ACBA4();
    void func_800ACBCC();
    void func_800ACC3C();
    void func_800ACC94();
    void func_800ACCD4();
    void func_800ACCE4();
    void func_800ACD4C();
    void func_800ACD5C();
    void func_800ACDA0();
    u8* func_800ACDE0();
    void func_800ACDFC();
    void func_800ACE60();
    float func_800ACE78();
    void func_800ACEAC();
    void func_800ACF50() const;
    void func_800AD3A4();
    int func_800AD4A4();
    void func_800AD68C();
    virtual ~CfObjectEff();
    // CfObjectEff-specific vtable slots beyond CfObject's last (0x174).
    virtual void _v178();
    virtual void _v17C();
    virtual void _v180();
    virtual void _v184();
    virtual u8* getSub188();    // vtable 0x188 - returns the active sub-object
    virtual void _v18C();
    virtual void destroy190();    // vtable 0x190 - tears down the sub-object
    void func_800AD818();
    void func_800AD830();
    void func_800AD840();
    void func_800ACE44();
    void func_800ACF34() const;
    void func_800AD850();
    void func_800AD858();
};

} // namespace cf
