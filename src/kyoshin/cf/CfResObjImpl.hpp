#pragma once

#include <types.h>
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

// Minimal nw4r sound-object interface used by func_8016C888 (sound slot +0x00
// dereferenced to call SetPlayerPriority). Declared here at global scope
// rather than via snd_BasicSound.h; MWCC mangles the member call to the
// retail name SetPlayerPriority__Q44nw4r3snd6detail10BasicSoundFi (same
// pattern as CfResReloadImpl.hpp - a copy inside namespace cf would mangle
// the extra cf scope into the name).
namespace nw4r {
namespace snd {
namespace detail {
class BasicSound {
public:
    void SetPlayerPriority(int priority);
};
} // namespace detail
} // namespace snd
} // namespace nw4r

class CScnItemModel; // fwd; real owner of parent+0x98 model (hot header)

namespace cf {

class CfObjectMove;      // fwd; func_800BCFA0 operand (real def in object/CfObjectMove.hpp)
class CScn;              // fwd; shared scene (CfObjectMove.hpp declares lbl_eu_80663E14 as CScn*)
struct CfResObjImpl;     // fwd

// Slot entry returned by func_801BFAE4 (CfSoundMan slot lookup); +0x00 holds
// the active sound object, which func_8016C888 calls SetPlayerPriority on.
struct SoundSlotEntry {
    /* 0x00 */ nw4r::snd::detail::BasicSound* field_00;
};

class CfObject; // real owner of the +0x38 sub-object (vtable slot 0xB0 is CfObject_getPosTriple)

// Parent object referenced at +0x00 of CfResObjImpl (a CfObject-derived
// instance whose vptr sits at +0x00). Only the fields this unit touches are
// declared.
struct CfResObjParent {
    u8 field_00[0x38];
    /* 0x38 */ cf::CfObject* field_38;       // sub-object (vtable slot 0xB0 -> CfObject_getPosTriple)
    u8 field_3C[0x6C - 0x3C];
    /* 0x6C */ u32 field_6C;                 // flags (bits 0x10 / 0x20 tested)
    u8 field_70[0x74 - 0x70];
    /* 0x74 */ u8* field_74;                 // sound-related pointer passed to func_801BFE20
    u8 field_78[0x90 - 0x78];
    /* 0x90 */ u8* field_90;                 // resource handle (func_80066E7C result)
    /* 0x94 */ u8* field_94;                 // resource handle
    /* 0x98 */ ::CScnItemModel* field_98;  // model object (vtable slots 0x78(void*)/0x88(int) -> CScnItemModel::vfunc78/88)
    /* 0x9C */ u8* field_9C;                 // model handle (func_800584B8 result)
    u8 field_A0[0x60C - 0xA0];
    /* 0x60C */ u8 field_60C[0x8];           // sub-object passed to func_804B0A6C
    u8 field_614[0x6B4 - 0x614];
    /* 0x6B4 */ u32 field_6B4;
    /* 0x6B8 */ u32 field_6B8;
};

// Parent dispatch now via real cf::CfObjectModel / cf::CfObject (slots
// 0x178/0x17C = CfObjectModel func1/2, 0xDC/0xE0 =
// CfObject func35/36). Fake parent dispatch deleted.

// Real owner of the +0x38 sub-object is cf::CfObject (slot 0xB0
// is CfObject_getPosTriple). The fake pad is deleted.

// Fake model dispatch deleted: real CScnItemModel::vfunc78(void*)/vfunc88(int)
// now owned by CScnItemModel header (hot header, widened to retail arity).

// Non-virtual data view of the +0x98 sub-object: the +0x7A4 flag word is
// read by func_8016C98C.
struct CfResObjModel98Data {
    u8 field_00[0x7A4];
    /* 0x7A4 */ u32 field_7A4;
};

// View of the object returned by cf::CfGameManager::getGameSubManager: only the
// +0x98 pointer is read (the real class UnkClass_80083298 keeps +0x98 inside
// opaque padding).
struct CfResObjGm98View {
    u8 field_00[0x98];
    /* 0x98 */ ::CScnItemModel* field_98;  // same model ptr as CfResObjParent/+0x98
};

// Prefix pushing vptr to +0x10 (CHelp / CfResReloadImpl pattern):
// base holds 0x00-0x0E, vptr follows at +0x10, then the 0x14 tail.
struct CfResObjImplPrefix {
    /* 0x00 */ CfResObjParent* field_00; // parent/reference pointer
    /* 0x04 */ f32 field_04;
    /* 0x08 */ u16 field_08; // state (also PMTF dispatch index)
    /* 0x0A */ s16 field_0A; // state (-1 = invalid)
    /* 0x0C */ u16 field_0C;
    /* 0x0E */ s16 field_0E; // state (-1 = invalid)
};

// Local overlay: ctor writes retail lbl_eu_80530F44 at +0x10 (novtable).
struct CfResObjImplVtbl {
    u32 slots[27];
};

// Real class tree for cf::CfResObjImpl (retail lbl_eu_80530F44, 0x6C:
// RTTI + 0 + 25 slots). novtable: TU has no .data vtable, ctor writes
// lbl_eu_80530F44 at +0x10 like CToken / CHelp.
class __declspec(novtable) CfResObjImpl : public CfResObjImplPrefix {
public:
    CfResObjImpl(CfResObjParent* parent);
    virtual ~CfResObjImpl();                // 0x08
    virtual void func_8016CCE0();           // 0x0C
    virtual void func_800BEA34();           // 0x10
    virtual int func_8016C860();            // 0x14
    virtual void func_800BE9AC();           // 0x18
    virtual void func_8016CD64();           // 0x1C
    virtual int func_8016C880();            // 0x20
    virtual void func_800BC2DC();           // 0x24
    virtual void func_800BF2F4();           // 0x28
    virtual void func_eu_800BFC78();        // 0x2C
    virtual void func_8016CD68(int idx, int value); // 0x30
    virtual u32 func_8016CCBC(int idx);     // 0x34
    virtual int func_800BEA38();            // 0x38
    // Widened: Move UVF39/40/42/43 leave r4/r5 live into this slot.
    virtual void func_800BED64(u32 a, u32 b); // 0x3C
    virtual void func_800BEE30();           // 0x40
    virtual void func_800BC3AC();           // 0x44
    virtual void func_800BEC44();           // 0x48
    // Widened: Move UVF38 leaves r4 live into this slot.
    virtual void func_800BED5C(u32 arg);    // 0x4C
    virtual int func_8016CD5C();            // 0x50
    virtual void func_8016C888(int arg2, int arg3, int arg4, float f1, float f2); // 0x54
    virtual void func_800BF2C4();           // 0x58
    virtual void func_800BF2C8();           // 0x5C
    virtual int func_8016C950();            // 0x60
    virtual int func_8016CD54();            // 0x64
    virtual int func_800BF30C();            // 0x68

    CfResObjImplVtbl*& vtbl() {
        return *reinterpret_cast<CfResObjImplVtbl**>(reinterpret_cast<u8*>(this) + 0x10);
    }

    /* 0x14 */ u32 field_14[2];
};

} // namespace cf

// 3-word (12-byte) member-function-pointer storage view used for the
// word-wise __ptmf_null copy in func_8016CCE0 (struct copy so MWCC emits the
// inline lwzu/stwu sequence).
struct CfPmf3 {
    u32 w0;
    u32 w1;
    u32 w2;
};

// PMTF dispatch table (3 x 12-byte entries) selected by field_08 in
// func_8016CCE0. Stored as CfPmf3 words ({0,0,0} + two {0,-1,func} rows:
// the free-function targets are not expressible as PMFs in C++); the
// dispatch site casts back to CfResObjImplPMF so the call still lowers to
// `bl __ptmf_scall`. Declared at global scope so MWCC keeps the retail
// name unmangled.
typedef void (cf::CfResObjImpl::*CfResObjImplPMF)();
// Word/PMF dual view of one dispatch entry: initialized via w (raw words
// with relocs), dispatched via p (real PMF call -> bl __ptmf_scall).
union CfPmfEntry {
    CfPmf3 w;
    CfResObjImplPMF p;
};
extern CfPmfEntry lbl_eu_80530F20[3];

// One-time PMTF-table init flag (.sdata).
extern char lbl_eu_80664270;

// Global null pointer-to-member-function constant (3 words).
extern u32 __ptmf_null[3];

// Retail vtable data for CfResObjImpl (.data @0x80530F44); stored at +0x10
// by the ctor.
extern cf::CfResObjImplVtbl lbl_eu_80530F44;

// Float seed pair written to +0x04 by the ctor (.sdata2; two 0.0f words).
// Stored as one 8-byte scalar: MWCC drops unreferenced const scalars and
// BSS-optimizes zero aggregates into .sbss2, so neither float[2] nor a
// second static survives. Code reads the low word as float (single lfs).
extern const double lbl_eu_80667690;

// ---------------------------------------------------------------------------
// C-ABI imports (retail unmangled names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------

struct ResInfoEntry;     // fwd; full def in kyoshin/cf/IResInfo.hpp (global scope)

// func_80063038: return the manager's +0x170 region (entry-table base) if
// the CfRes manager exists (CfRes.cpp).
extern "C" char* func_80063038();
extern "C" u8* func_80066E7C(ResInfoEntry* entry, u32 id);
extern "C" u8* func_80489A60(u8* global, u8* handle, int a, int b, int c, int d);
extern "C" void func_800BBADC(cf::CfResObjParent* parent, u8* handle);
extern "C" u8* func_800584B8(u32 global, u32 id, const char* name);
extern "C" int CfRes_getD80Flag();
extern "C" void func_800BCFA0(cf::CfObjectMove* self);
extern "C" void func_800BE824(cf::CfResObjParent* parent, int flag);
extern "C" void func_804B0A6C(u8* subObj, u8* handle);
extern "C" int func_801BFE20(int a, int b, u8* c, float f1, float f2);
extern "C" cf::SoundSlotEntry* func_801BFAE4(u16 handle);

// Free-function vtable-slot targets owned by other TUs (retail .data
// lbl_eu_80530F44 references these unmangled names; the same-named virtuals
// above are scoped methods and do not collide).
extern "C" void func_800BEA34();
// Defined in CfObjectMove.cpp as void(void*) (CScnItemModel::vfunc14 hop).
extern "C" void func_800BE9AC(void* self);
extern "C" void func_800BC2DC();
extern "C" void func_800BF2F4();
extern "C" void func_eu_800BFC78();
extern "C" bool func_800BEA38();
extern "C" void func_800BED64();
extern "C" void func_800BEE30();
extern "C" void func_800BC3AC();
extern "C" void func_800BEC44();
extern "C" void func_800BED5C();
extern "C" void func_800BF2C4();
extern "C" void func_800BF2C8();
extern "C" int func_800BF30C();
// Compiler-generated deleting destructor (defined in this TU's .text).
extern "C" void __dt__Q22cf12CfResObjImplFv();
