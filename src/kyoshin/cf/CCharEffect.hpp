#pragma once

#include <types.h>
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

/// Returns 1 (always-active sentinel or default state check)
int func_8015CB88();

/// Returns 0 (inactive/false default)
int func_8015CCB4();

// Retail C-ABI imports used by func_8015C2B0 (no shared kyoshin header owns
// them yet; same pattern as include/kyoshin/cf/CCharEffectEne.hpp).
extern "C" u32 func_80053F40(u8* p, u32 v);
extern "C" void func_80484EB0(u8* ptr);
extern "C" void setChildF50G_(u8* self);

// Retail C-ABI imports used by func_8015BB3C / func_8015C404 (retail symbols
// are unmangled C; getGimmickList is called as a static __Fv even though the
// shared CfGameManager header declares it as a member - same pattern as
// CTaskGameEff.hpp's createNpcActor__Q22cf13CfGameManagerFv).
extern "C" void* func_80495FF0(void* scene);
extern "C" void func_800AA318(u32 packed, u32* out0, u32* out1, u32* out2, u32* out3);
extern "C" void setTargetObj_(void* obj, void* target);
extern "C" void setChildB59__(void* obj, s8 val);
extern "C" void func_804E3CDC(void* effect, f32 f1, f32 f2);
extern "C" void* getGimmickList__Q22cf13CfGameManagerFv();

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
extern u8 lbl_eu_8052FDD8[];
extern u8 lbl_eu_8052FE08[];
extern u8 lbl_eu_8052FE38[];
extern u8 lbl_eu_8052FE68[];

struct CCharEffectSlot;
struct CCharEffectData;

namespace cf {
class CfObjectImplPcBattle4; // owner of the +0x04 battle sub-object (cfsys/CfObjectImplPc.hpp)
} // namespace cf

namespace cf {

// Battle object held at CCharEffect::mBattleObj (NULL when none). The +0x04
// sub-object's real class is cf::CfObjectImplPcBattle4 (cfsys owner iface:
// vtable 0x30 returns the id word holder); the former caller-local
// CCharEffectBattleObj4 pad is folded onto it.
struct CCharEffectBattleObj {
    u8 pad_00[0x4];
    cf::CfObjectImplPcBattle4* field_04;  // 0x04
};

// Real class tree for CCharEffect family (retail __vt__ dump from US split1.s):
// lbl_eu_8052FDB8 (CCharEffect, 0x20): RTTI 80662330, 0, __dt__Q22cf11CCharEffectFv, func_8015BF04, func_8015C9A0, func_800CEE7C, 0, func_8015C2B0
// lbl_eu_8052FDD8 (CCharEffectEne, 0x20): same but +0x18 = func_8015CD04
// lbl_eu_8052FE08 (Npc, 0x20): +0x18 = func_8015CD9C
// lbl_eu_8052FE38 (Obj, 0x20): +0x18 = func_8015CE44
// lbl_eu_8052FE68 (Pc, 0x20): +0x18 = func_8015CED0, +0x1C = func_8015CF90
// Hierarchy: CCharEffect is root, others derive directly. CHelp is the proven instance.
// novtable: do not emit __vt__ from this TU, ctor writes lbl_eu_... label explicitly.

class __declspec(novtable) CCharEffect {
public:
    virtual ~CCharEffect(); // vtable 0x08
    virtual void func_8015BF04(::CCharEffectSlot* p); // 0x0C
    virtual void func_8015C9A0(); // 0x10
    virtual void func_800CEE7C(); // 0x14
    virtual void* getObj(u32 idx, s32 mode) = 0; // 0x18 pure in base
    virtual void applyEffect(void* target, u32 type, u32 flags); // 0x1C
};

class __declspec(novtable) CCharEffectEne : public CCharEffect {
public:
    virtual void* getObj(u32 idx, s32 mode) override; // 0x18
};

class __declspec(novtable) CCharEffectNpc : public CCharEffect {
public:
    virtual void* getObj(u32 idx, s32 mode) override;
};

class __declspec(novtable) CCharEffectObj : public CCharEffect {
public:
    virtual void* getObj(u32 idx, s32 mode) override;
};

class __declspec(novtable) CCharEffectPc : public CCharEffect {
public:
    virtual void* getObj(u32 idx, s32 mode) override;
    virtual void applyEffect(void* target, u32 type, u32 flags) override;
};

} // namespace cf

// Effect-slot object held in CCharEffect::mSlots (never instantiated in
// this TU - only dispatched/cast). Slots are CfObject-family effect objects
// (getObj/factory results - cf::CfObjectEff* at creation sites); the former
// 84-dummy vtable pad is deleted and dispatch goes through the real owner
// slots (cf::CfObject::setPointEnabled at vtable offset 0x158).
// Plain layout view of the effect-relevant words (all inside CfObjectModel
// range: +0x68 CfObject::mFlags68, +0x94 sub-object pointer, +0xB0 owner
// back-pointer (cf::CfObjectModel::mSubObjB0), +0xB4 removal guard).
struct CCharEffectSlot {
    u8 pad_00[0x68];        // 0x00..0x67 (vptr at +0x00)
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

// Effect data object pointed to by CCharEffectMgr::field_98. Retail it is
// a CScnItemModel: vtable 0x18 (vfunc18) returns the name string compared
// by func_8015C2B0; f32 at 0x2E8 and the byte at 0x304 are read there too.
// Plain layout view (no virtuals); the name query goes via
// CScnItemModel::vfunc18, the fields stay at absolute offsets.
struct CCharEffectData {
    u8 pad_00[0x2E8];
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
extern f32 lbl_eu_80667534;          // sdata2 float constant (func_8015C404)
extern f64 lbl_eu_80667538;          // sdata2 double constant 0x4330000080000000 (func_8015C404)
extern s8 lbl_eu_80501DCC[];         // per-type keep/release table (func_8015C404)
extern s8 lbl_eu_80501DA0[];         // per-type keep/release table (func_8015C404)
extern CCharEffectSlot* (*lbl_eu_8052FD08[])(CCharEffect*); // per-type handler table
