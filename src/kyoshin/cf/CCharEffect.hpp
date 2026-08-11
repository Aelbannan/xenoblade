#pragma once

#include <types.h>

/// Returns 1 (always-active sentinel or default state check)
int func_8015CB88();

/// Returns 0 (inactive/false default)
int func_8015CCB4();

// Retail C-ABI imports used by func_8015C2B0 (no shared kyoshin header owns
// them yet; same pattern as include/kyoshin/cf/CCharEffectEne.hpp).
extern "C" u32 func_80053F40(u8* p, u32 v);
extern "C" void func_80484EB0(u8* ptr);
extern "C" void func_800ACC50(u8* self);

// Retail C-ABI imports used by func_8015BB3C / func_8015C404 (retail symbols
// are unmangled C; func_80086B08 is called as a static __Fv even though the
// shared CfGameManager header declares it as a member - same pattern as
// CTaskGameEff.hpp's func_8008187C__Q22cf13CfGameManagerFv).
extern "C" void* func_80495FF0(void* scene);
extern "C" void func_800AA318(u32 packed, u32* out0, u32* out1, u32* out2, u32* out3);
extern "C" void func_800ACFD8(void* obj, void* target);
extern "C" void func_800ACC14(void* obj, s8 val);
extern "C" void func_804E3CDC(void* effect, f32 f1, f32 f2);
extern "C" void* func_80086B08__Q22cf13CfGameManagerFv();

// External data referenced by this unit (MWCC does not mangle global data).
extern u32 lbl_eu_80664228;     // default slot value when NULL is registered
// Name string compared via strstr by func_8015C2B0.
extern char lbl_eu_80501E38[];
// Per-type handler table (-1/1/2) indexed by the effect type byte.
extern s8 lbl_eu_80501DF8[];
// sdata2 scale constant used by func_8015C2B0.
extern f32 lbl_eu_80667530;

// CCharEffect's vtable, stored by __ct__CCharEffect. It is later
// overridden by containing-object ctors (e.g. lbl_eu_8052FE68 for the
// 80338 object), so CCharEffect instances are dispatched virtually in
// some contexts. Declared as data: MWCC does not mangle global data names.
extern u8 lbl_eu_8052FDB8[];

namespace cf {

// Sub-object at +0x04 of the battle object stored at CCharEffect::mBattleObj:
// with -RTTI on MWCC reserves two leading vtable slots, so the virtual at
// declared index N sits at vtable offset (N+2)*4. Declared index 10 is thus
// vtable offset 0x30; it returns a pointer whose first word is the id
// queried by func_80174C98.
class CCharEffectBattleObj4 {
public:
    virtual void bf00() = 0;  // index 0
    virtual void bf04() = 0;  // index 1
    virtual void bf08() = 0;  // index 2
    virtual void bf0C() = 0;  // index 3
    virtual void bf10() = 0;  // index 4
    virtual void bf14() = 0;  // index 5
    virtual void bf18() = 0;  // index 6
    virtual void bf1C() = 0;  // index 7
    virtual void bf20() = 0;  // index 8
    virtual void bf24() = 0;  // index 9
    virtual u32* bf30() = 0;  // index 10 -> vtable offset 0x30
};

// Battle object held at CCharEffect::mBattleObj (NULL when none).
struct CCharEffectBattleObj {
    u8 pad_00[0x4];
    CCharEffectBattleObj4* field_04;  // 0x04
};

} // namespace cf

// Effect-slot object held in CCharEffect::mSlots (never instantiated in
// this TU - only dispatched/cast). Declared virtual N lands at vtable byte
// offset (N+2)*4 (kyoshin builds with -RTTI on), so the method used here
// (index 84) sits at vtable offset 0x158. The slot also carries a pointer
// at +0x94 (func_eu_8015D258 follows it to a float holder).
class CCharEffectSlot {
public:
    virtual void v000() = 0; virtual void v001() = 0; virtual void v002() = 0; virtual void v003() = 0;
    virtual void v004() = 0; virtual void v005() = 0; virtual void v006() = 0; virtual void v007() = 0;
    virtual void v008() = 0; virtual void v009() = 0; virtual void v010() = 0; virtual void v011() = 0;
    virtual void v012() = 0; virtual void v013() = 0; virtual void v014() = 0; virtual void v015() = 0;
    virtual void v016() = 0; virtual void v017() = 0; virtual void v018() = 0; virtual void v019() = 0;
    virtual void v020() = 0; virtual void v021() = 0; virtual void v022() = 0; virtual void v023() = 0;
    virtual void v024() = 0; virtual void v025() = 0; virtual void v026() = 0; virtual void v027() = 0;
    virtual void v028() = 0; virtual void v029() = 0; virtual void v030() = 0; virtual void v031() = 0;
    virtual void v088() = 0; virtual void v033() = 0; // v088: index 32 -> vtable 0x88 virtual void v034() = 0; virtual void v035() = 0;
    virtual void v036() = 0; virtual void v037() = 0; virtual void v038() = 0; virtual void v039() = 0;
    virtual void v040() = 0; virtual void v041() = 0; virtual void v042() = 0; virtual void v043() = 0;
    virtual void v044() = 0; virtual void v045() = 0; virtual void v046() = 0; virtual void v047() = 0;
    virtual void v048() = 0; virtual void v049() = 0; virtual void v050() = 0; virtual void v051() = 0;
    virtual void v052() = 0; virtual void v053() = 0; virtual void v054() = 0; virtual void v055() = 0;
    virtual void v056() = 0; virtual void v057() = 0; virtual void v058() = 0; virtual void v059() = 0;
    virtual void v060() = 0; virtual void v061() = 0; virtual void v062() = 0; virtual void v063() = 0;
    virtual void v064() = 0; virtual void v065() = 0; virtual void v066() = 0; virtual void v067() = 0;
    virtual void v068() = 0; virtual void v069() = 0; virtual void v070() = 0; virtual void v071() = 0;
    virtual void v072() = 0; virtual void v073() = 0; virtual void v074() = 0; virtual void v075() = 0;
    virtual void v076() = 0; virtual void v077() = 0; virtual void v078() = 0; virtual void v079() = 0;
    virtual void v080() = 0; virtual void v081() = 0; virtual void v082() = 0; virtual void v083() = 0;
    virtual void v158(u32 value) = 0; // index 84 -> vtable offset 0x158

    u8 pad_04[0x64];        // 0x04..0x67
    u32 field_68;           // 0x68  flags word (bit 0x40 = "in use")
    u8 pad_6C[0x94 - 0x6C]; // 0x6C..0x93
    void* field_94;         // 0x94  sub-object pointer
    u8 pad_98[0xB0 - 0x98]; // 0x98..0xAF
    void* field_B0;         // 0xB0  owner back-pointer (cleared on removal)
    u8 field_B4;            // 0xB4  removal guard byte
};

// Target of CCharEffectSlot::field_94: float written at +0x4C, plus an
// id word at +0x14 matched against the manager's vtable-0xA8 result.
struct CCharEffectSlotSub {
    u8 pad_00[0x14];
    u32 field_14;           // 0x14  id compared by func_8015C8F4
    u8 pad_18[0x4C - 0x18];
    f32 field_4C;           // 0x4C
};

// Cast-only virtual interface for the manager/effect objects dispatched at
// retail vtable offsets 0x0A8 / 0x0DC / 0x194 (kyoshin -RTTI: declared
// index N lands at vtable byte offset (N+2)*4). Never instantiated.
class CCharEffectVTableIf {
public:
    virtual void v008() = 0; virtual void v00C() = 0; virtual void v010() = 0; virtual void v014() = 0;
    virtual CCharEffectSlot* v018(u32 idx, s16 val) = 0;   // index 4 -> vtable 0x18
    virtual void v01C(CCharEffectSlot* slot, u32 idx, u8 v) = 0; // index 5 -> vtable 0x1c
    virtual void v020() = 0; virtual void v024() = 0;
    virtual void v028() = 0; virtual void v02C() = 0; virtual void v030() = 0; virtual void v034() = 0;
    virtual void v038() = 0; virtual void v03C() = 0; virtual void v040() = 0; virtual void v044() = 0;
    virtual void v048() = 0; virtual void v04C() = 0; virtual void v050() = 0; virtual void v054() = 0;
    virtual void v058() = 0; virtual void v05C() = 0; virtual void v060() = 0; virtual void v064() = 0;
    virtual void v068() = 0; virtual void v06C() = 0; virtual void v070() = 0; virtual void v074() = 0;
    virtual void v078() = 0; virtual void v07C() = 0; virtual void v080() = 0; virtual void v084() = 0;
    virtual void v088() = 0; virtual void v08C() = 0; virtual void v090() = 0; virtual void v094() = 0;
    virtual void v098() = 0; virtual void v09C() = 0; virtual void v0A0() = 0; virtual void v0A4() = 0;
    virtual u32 v0A8() = 0;  // index 40 -> vtable 0x0A8
    virtual void v0AC() = 0; virtual void v0B0() = 0; virtual void v0B4() = 0;
    virtual void v0B8() = 0; virtual void v0BC() = 0; virtual void v0C0() = 0; virtual void v0C4() = 0;
    virtual void v0C8() = 0; virtual void v0CC() = 0; virtual void v0D0() = 0; virtual void v0D4() = 0;
    virtual void v0D8() = 0; virtual void v0DC(f32 v) = 0; // index 53 -> vtable 0x0DC
    virtual void v0E0() = 0; virtual void v0E4() = 0;
    virtual void v0E8() = 0; virtual void v0EC() = 0; virtual void v0F0() = 0; virtual void v0F4() = 0;
    virtual void v0F8() = 0; virtual void v0FC() = 0; virtual void v100() = 0; virtual void v104() = 0;
    virtual void v108() = 0; virtual void v10C() = 0; virtual void v110() = 0; virtual void v114() = 0;
    virtual void v118() = 0; virtual void v11C() = 0; virtual void v120() = 0; virtual void v124() = 0;
    virtual void v128() = 0; virtual void v12C() = 0; virtual void v130() = 0; virtual void v134() = 0;
    virtual void v138() = 0; virtual void v13C() = 0; virtual void v140() = 0; virtual void v144() = 0;
    virtual void v148() = 0; virtual void v14C() = 0; virtual void v150() = 0; virtual void v154() = 0;
    virtual void v158() = 0; virtual void v15C() = 0; virtual void v160() = 0; virtual void v164() = 0;
    virtual void v168() = 0; virtual void v16C() = 0; virtual void v170() = 0; virtual void v174() = 0;
    virtual void v178() = 0; virtual void v17C() = 0; virtual void v180() = 0; virtual void v184() = 0;
    virtual void v188() = 0; virtual void v18C() = 0; virtual void v190() = 0; virtual void v194(u32 v) = 0; // index 99 -> vtable 0x194
};

// Effect data object pointed to by CCharEffectMgr::field_98: virtual at
// vtable 0x18 returns a name string; f32 at 0x2E8 and byte at 0x304 are
// read by func_8015C2B0.
class CCharEffectData {
public:
    virtual void v008() = 0; virtual void v00C() = 0;
    virtual void v010() = 0; virtual void v014() = 0;
    virtual char* v018() = 0; // index 4 -> vtable 0x18

    u8 pad_04[0x2E8 - 0x04];
    f32 field_2E8;          // 0x2E8
    u8 pad_2EC[0x304 - 0x2EC];
    u8 field_304;           // 0x304
};

// Manager/owner object view (CfObjectMove-derived) for the two offsets this
// unit touches: effect data source at +0x98 and a data holder at +0xC4.
struct CCharEffectMgr {
    u8 pad_00[0x98];
    void* field_98;         // 0x98
    u8 pad_9C[0xC4 - 0x9C];
    u8* field_C4;           // 0xC4
};

// Minimal layout view of the CCharEffect object fields touched by this
// unit's helpers. mSlots[0] is the vtable slot (set by __ct__CCharEffect);
// the 44 effect slots live at mSlots[1..44] with a twin array at 0xB4.
struct CCharEffect {
    void* mSlots[45];        // 0x000..0x0B4 (mSlots[0] = vtable slot)
    void* mSlots2[44];       // 0x0B4..0x164  twin array
    s16 mField_164[44];      // 0x164..0x1BC  per-slot s16 table
    s16 mField_1BC[44];      // 0x1BC..0x214  per-slot s16 table
    u16 mField_214[44];      // 0x214..0x26C  per-slot u16 table (retail reads with lhz)
    u16 mFlags;              // 0x26C  bit1 = "changed" latch
    u8 mField_26E[44];       // 0x26E..0x29A  per-slot u8 table
    u8 pad_29A[2];           // 0x29A..0x29B
    void* mManager;          // 0x29C  manager/owner pointer
    cf::CCharEffectBattleObj* mBattleObj; // 0x2A0  battle object (NULL when none)
    u32 mField2A4;           // 0x2A4
    u32 mField2A8;           // 0x2A8
    u32 mField2AC;           // 0x2AC  clear-word, bits  0-31
    u32 mField2B0;           // 0x2B0  set-word,   bits  0-31
    u32 mField2B4;           // 0x2B4  set-word,   bits  0-31
    u32 mField2B8;           // 0x2B8
    u32 mField2BC;           // 0x2BC
    u32 mField2C0;           // 0x2C0  clear-word, bits 32-63
    u32 mField2C4;           // 0x2C4  set-word,   bits 32-63
    u32 mField2C8;           // 0x2C8  set-word,   bits 32-63
    void* mSlotList2[8];     // 0x2CC..0x2EC
    u8 pad_2EC[0x10];        // 0x2EC..0x2FB
    u32 mField2FC;           // 0x2FC
};

// External data referenced by func_8015C9A0 / func_8015C404 (MWCC does not
// mangle global data names).
extern void* lbl_eu_80663E14;        // .sbss scene/handle pointer (func_8015BB3C)
extern f32 lbl_eu_80667534;          // sdata2 float constant (func_8015C404)
extern f64 lbl_eu_80667538;          // sdata2 double constant 0x4330000080000000 (func_8015C404)
extern s8 lbl_eu_80501DCC[];         // per-type keep/release table (func_8015C404)
extern s8 lbl_eu_80501DA0[];         // per-type keep/release table (func_8015C404)
extern CCharEffectSlot* (*lbl_eu_8052FD08[])(CCharEffect*); // per-type handler table
