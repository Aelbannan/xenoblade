#pragma once

#include <types.h>
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

struct ResInfoEntry;  // full def in kyoshin/cf/IResInfo.hpp
class CScn;  // shared scene (CfObjectMove.hpp declares lbl_eu_80663E14 as CScn*)

namespace cf {

class CfObjectMove;  // fwd (also fwd-declared by CfGameManager.hpp)

struct CfResTboxImpl;
struct CfResTboxParent;
struct CfResTboxVtIf;
struct CfResTboxParentVtIf;
struct CfResTbox38ObjIf;
struct CfResTbox98ObjIf;

// Cast-only vtable view of the manual vtable pointer stored at
// CfResTboxImpl+0x10 (retail lbl_eu_80535204). The non-polymorphic
// CfResTboxShift base puts the vptr at object+0x10, so a virtual call emits
// `lwz r12,0x10(r3); lwz r12,slot(r12); mtctr; bctrl` with `this` staying at
// the object base (double-hop pattern, cf. CfResReloadVtIf).
struct CfResTboxShift {
    u8 field_00[0x10];
};

struct CfResTboxVtIf : CfResTboxShift {
    virtual void _v008();
    virtual void _v00C();
    virtual void _v010();
    virtual int _v014();                  // vtable offset 0x14 - open/in-use test
    virtual void _v018();
    virtual void _v01C();
    virtual void _v020();
    virtual void _v024();
    virtual void _v028();
    virtual void _v02C();
    virtual void _v030();
    virtual u32 _v034(int);                // vtable offset 0x34 - resource handle getter
};

// Cast-only vtable view of the parent object at CfResTboxImpl+0x00 (a
// CfObjectMove-derived instance; its vptr sits at +0x00). Dummies pin the
// slots dispatched by func_801F8EB0.
struct CfResTboxParentVtIf {
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
    virtual float _v0DC(float value);      // vtable offset 0xDC - anim setter (takes f1)
    virtual float _v0E0();                 // vtable offset 0xE0 - anim getter (returns f1)
    virtual void _v0E4(); virtual void _v0E8(); virtual void _v0EC(); virtual void _v0F0();
    virtual void _v0F4(); virtual void _v0F8(); virtual void _v0FC(); virtual void _v100();
    virtual void _v104(); virtual void _v108(); virtual void _v10C(); virtual void _v110();
    virtual void _v114(); virtual void _v118(); virtual void _v11C(); virtual void _v120();
    virtual void _v124(); virtual void _v128(); virtual void _v12C(); virtual void _v130();
    virtual void _v134(); virtual void _v138(); virtual void _v13C(); virtual void _v140();
    virtual void _v144(); virtual void _v148(); virtual void _v14C(); virtual void _v150();
    virtual void _v154(); virtual void _v158(); virtual void _v15C(); virtual void _v160();
    virtual void _v164();
    virtual void _v168(float value);       // vtable offset 0x168 - anim setter (takes f1)
    virtual void _v16C(); virtual void _v170(); virtual void _v174();
    virtual void _v178();                  // vtable offset 0x178 - dispatched by func_801F8EB0
    virtual void _v17C();                  // vtable offset 0x17C - dispatched by func_801F8EB0
};

// Cast-only vtable view of the sub-object at parent+0x38: slot +0xB0 is
// dispatched by func_801F8EB0 after the resource build.
struct CfResTbox38ObjIf {
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
    virtual void _v0A8(); virtual void _v0AC();
    virtual void _v0B0();                  // vtable offset 0xB0
};

// Cast-only vtable view of the sub-object at parent+0x98: slot +0x88 is
// dispatched with a 1 arg by func_801F8EB0.
struct CfResTbox98ObjIf {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
    virtual void _v038(); virtual void _v03C(); virtual void _v040(); virtual void _v044();
    virtual void _v048(); virtual void _v04C(); virtual void _v050(); virtual void _v054();
    virtual void _v058(); virtual void _v05C(); virtual void _v060(); virtual void _v064();
    virtual void _v068(); virtual void _v06C(); virtual void _v070(); virtual void _v074();
    virtual void _v078(); virtual void _v07C(); virtual void _v080(); virtual void _v084();
    virtual void _v088(int arg);           // vtable offset 0x88
};

// Parent object referenced at +0x00 of CfResTboxImpl (a CfObjectMove-derived
// instance; only the fields this unit touches are declared).
struct CfResTboxParent {
    u8 field_00[0x38];
    /* 0x38 */ CfResTbox38ObjIf* field_38;  // sub-object (vtable slot +0xB0)
    u8 field_3C[0x6C - 0x3C];
    /* 0x6C */ u32 field_6C;   // flags (bits 0x10 / 0x20 / 0x20000000 tested, 0x4/0x20000000 cleared)
    u8 field_70[0x90 - 0x70];
    /* 0x90 */ u8* field_90;   // resource handle (func_80066E7C / func_80062114 result)
    /* 0x94 */ u8* field_94;   // resource handle
    /* 0x98 */ CfResTbox98ObjIf* field_98;  // model object (vtable slot +0x88)
    /* 0x9C */ u8* field_9C;   // model handle (func_800584B8 result)
};

// Resource textbox implementation. The CfResObjImpl base handles offsets
// 0x00-0x0F; +0x10 is a manual vtable pointer (retail lbl_eu_80535204), not
// a C++ vptr - dispatch goes through the cast-only views above.
struct CfResTboxImpl {
    /* 0x00 */ CfResTboxParent* field_00;  // parent object
    u8 field_04[0x08 - 0x04];
    /* 0x08 */ u16 field_08;   // open/active counter (also PMTF dispatch index)
    u8 field_0A[0x10 - 0x0A];
    /* 0x10 */ u8* field_10;   // manual vtable pointer (set to lbl_eu_80535204)

    ~CfResTboxImpl();
};

} // namespace cf

// 3-word (12-byte) member-function-pointer storage view used for the
// word-wise __ptmf_null copy in func_801F91B4 (struct copy so MWCC emits the
// inline lwzu/stwu sequence).
struct CfPmf3 {
    u32 w0;
    u32 w1;
    u32 w2;
};

// PMTF dispatch table (3 x 12-byte member pointers) selected by field_08 in
// func_801F91B4; `(self->*table[idx])()` lowers to `bl __ptmf_scall`.
typedef void (cf::CfResTboxImpl::*CfResTboxImplPMF)();
extern CfResTboxImplPMF lbl_eu_805351E0[3];

// One-time PMTF-table init flag (.sdata).
extern char lbl_eu_80664660;

// Global null pointer-to-member-function constant (3 words).
extern u32 __ptmf_null[3];

// Retail vtable data for CfResTboxImpl (.data @0x80535204); stored at +0x10
// by the ctor.
extern u8 lbl_eu_80535204[];

// Shared scene pointer used as the archive handle by func_80489A60 (.sdata).

// Global mode flag (.sbss; bit 0x100000 gates the open path).

// sdata2 float literals (resource-anim timing values).
extern float lbl_eu_806681D0;
extern float lbl_eu_806681D4;

// Flag-byte setter used by the battle plugin (retail 0x801F92B0; defined in
// CfResTboxImpl.cpp as a plain C++ function - the symbol map carries the
// unmangled retail name).
void func_801F92B0(u8* base, int idx1, int idx2, int idx3);

// C-ABI imports (retail unmangled names - keep linkage/signatures verbatim).
extern "C" bool func_8008585C__Q22cf13CfGameManagerFv();
extern "C" char* func_80063080();
extern "C" u8* func_80066E7C(ResInfoEntry* self, u32 id);
extern "C" void* func_80062114(char* self, int index, void** out);
extern "C" u8* func_80489A60(u8* global, u8* handle, int a, int b, int c, int d);
extern "C" void func_800BBADC(cf::CfResTboxParent* parent, u8* handle);
extern "C" u8* func_800584B8(u32 global, u32 id, const char* name);
extern "C" int CfRes_getD80Flag();
extern "C" void func_800BC3B0(cf::CfObjectMove* player, float value);
