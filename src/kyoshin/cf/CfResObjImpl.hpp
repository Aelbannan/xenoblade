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

namespace cf {

class CfObjectMove;      // fwd; func_800BCFA0 operand (real def in object/CfObjectMove.hpp)
class CScn;              // fwd; shared scene (CfObjectMove.hpp declares lbl_eu_80663E14 as CScn*)
struct CfResObjImpl;     // fwd

// Slot entry returned by func_801BFAE4 (CfSoundMan slot lookup); +0x00 holds
// the active sound object, which func_8016C888 calls SetPlayerPriority on.
struct SoundSlotEntry {
    /* 0x00 */ nw4r::snd::detail::BasicSound* field_00;
};

struct CfResObj38If;        // fwd; vtable view of the +0x38 sub-object
struct CfResObjModel98If;   // fwd; vtable view of the +0x98 sub-object

// Parent object referenced at +0x00 of CfResObjImpl (a CfObject-derived
// instance whose vptr sits at +0x00). Only the fields this unit touches are
// declared.
struct CfResObjParent {
    u8 field_00[0x38];
    /* 0x38 */ CfResObj38If* field_38;       // sub-object (vtable slot +0xB0)
    u8 field_3C[0x6C - 0x3C];
    /* 0x6C */ u32 field_6C;                 // flags (bits 0x10 / 0x20 tested)
    u8 field_70[0x74 - 0x70];
    /* 0x74 */ u8* field_74;                 // sound-related pointer passed to func_801BFE20
    u8 field_78[0x90 - 0x78];
    /* 0x90 */ u8* field_90;                 // resource handle (func_80066E7C result)
    /* 0x94 */ u8* field_94;                 // resource handle
    /* 0x98 */ CfResObjModel98If* field_98;  // model object (vtable slots +0x78/+0x88)
    /* 0x9C */ u8* field_9C;                 // model handle (func_800584B8 result)
    u8 field_A0[0x60C - 0xA0];
    /* 0x60C */ u8 field_60C[0x8];           // sub-object passed to func_804B0A6C
    u8 field_614[0x6B4 - 0x614];
    /* 0x6B4 */ u32 field_6B4;
    /* 0x6B8 */ u32 field_6B8;
};

// Cast-only vtable view of the parent object (vptr at +0x00). Dummies pin
// the slots dispatched by func_8016C98C: +0x178/+0x17C (init), +0xDC (anim
// setter, takes f1) and +0xE0 (anim getter, returns f1). With -RTTI on, MWCC
// adds 2 leading vtable slots, so the first declared virtual lands at vtable
// offset +8.
struct CfResObjParentVtIf {
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
    virtual void _v164(); virtual void _v168(); virtual void _v16C(); virtual void _v170();
    virtual void _v174();
    virtual void _v178();                  // vtable offset 0x178 - dispatched by func_8016C98C
    virtual void _v17C();                  // vtable offset 0x17C - dispatched by func_8016C98C
};

// Cast-only vtable view of the sub-object at parent+0x38: slot +0xB0 is
// dispatched by func_8016C98C after the resource build.
struct CfResObj38If {
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

// Cast-only vtable view of the sub-object at parent+0x98: slot +0x78 is
// dispatched with the model handle by func_8016C98C, slot +0x88 with a flag.
struct CfResObjModel98If {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
    virtual void _v038(); virtual void _v03C(); virtual void _v040(); virtual void _v044();
    virtual void _v048(); virtual void _v04C(); virtual void _v050(); virtual void _v054();
    virtual void _v058(); virtual void _v05C(); virtual void _v060(); virtual void _v064();
    virtual void _v068(); virtual void _v06C(); virtual void _v070(); virtual void _v074();
    virtual void _v078(void* arg);         // vtable offset 0x78 - model sync
    virtual void _v07C(); virtual void _v080(); virtual void _v084();
    virtual void _v088(int arg);           // vtable offset 0x88
};

// Non-virtual data view of the +0x98 sub-object: the +0x7A4 flag word is
// read by func_8016C98C.
struct CfResObjModel98Data {
    u8 field_00[0x7A4];
    /* 0x7A4 */ u32 field_7A4;
};

// View of the object returned by cf::CfGameManager::func_80083298: only the
// +0x98 pointer is read (the real class UnkClass_80083298 keeps +0x98 inside
// opaque padding).
struct CfResObjGm98View {
    u8 field_00[0x98];
    /* 0x98 */ void* field_98;
};

// Non-polymorphic shift base pushing the vptr to +0x10 (double-hop pattern):
// casting self to CfResObjImplVtIf and calling a virtual emits
// `lwz r12,0x10(r3); lwz r12,slot(r12); mtctr; bctrl` with `this` staying at
// the object base.
struct CfResObjImplShift {
    u8 field_00[0x10];
};

// Cast-only vtable view of the manual vtable pointer stored at
// CfResObjImpl+0x10 (retail lbl_eu_80530F44). Slot +0x14 is the in-use test
// dispatched by func_8016C950; +0x34 is the resource handle getter and +0x60
// the sound-id getter (func_8016C98C / func_8016C888).
struct CfResObjImplVtIf : CfResObjImplShift {
    virtual void _v008();
    virtual void _v00C();
    virtual void _v010();
    virtual int _v014();  // vtable offset 0x14 - in-use test
    virtual void _v018();
    virtual void _v01C();
    virtual void _v020();
    virtual void _v024();
    virtual void _v028();
    virtual void _v02C();
    virtual void _v030();
    virtual u32 _v034(int);  // vtable offset 0x34 - resource handle getter
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
    virtual int _v060();  // vtable offset 0x60 - sound-id getter
};

struct CfResObjImpl {
    CfResObjImpl(CfResObjParent* parent);
    ~CfResObjImpl();

    /* 0x00 */ CfResObjParent* field_00;  // parent/reference object
    /* 0x04 */ f32 field_04;
    /* 0x08 */ u16 field_08;  // state (also PMTF dispatch index)
    /* 0x0A */ s16 field_0A;  // state (-1 = invalid)
    /* 0x0C */ u16 field_0C;
    /* 0x0E */ s16 field_0E;  // state (-1 = invalid)
    /* 0x10 */ u8* field_10;  // manual vtable pointer (lbl_eu_80530F44)
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

// PMTF dispatch table (3 x 12-byte member pointers) selected by field_08 in
// func_8016CCE0. Declared at global scope so MWCC keeps the retail name
// unmangled; the (self->*table[idx])() call lowers to `bl __ptmf_scall`.
typedef void (cf::CfResObjImpl::*CfResObjImplPMF)();
extern CfResObjImplPMF lbl_eu_80530F20[3];

// One-time PMTF-table init flag (.sdata).
extern char lbl_eu_80664270;

// Global null pointer-to-member-function constant (3 words).
extern u32 __ptmf_null[3];

// Retail vtable data for CfResObjImpl (.data @0x80530F44); stored at +0x10
// by the ctor.
extern u8 lbl_eu_80530F44[];

// Float seed written to +0x04 by the ctor (.sdata2; 0.0f).
extern const float lbl_eu_80667690;

// ---------------------------------------------------------------------------
// C-ABI imports (retail unmangled names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------

struct ResInfoEntry;     // fwd; full def in kyoshin/cf/IResInfo.hpp (global scope)

// func_80063038: return the manager's +0x170 region (entry-table base) if
// the CfRes manager exists (CfRes.cpp).
extern "C" char* func_80063038();
extern "C" u8* func_80066E7C(ResInfoEntry* self, u32 id);
extern "C" u8* func_80489A60(u8* global, u8* handle, int a, int b, int c, int d);
extern "C" void func_800BBADC(cf::CfResObjParent* parent, u8* handle);
extern "C" u8* func_800584B8(u32 global, u32 id, const char* name);
extern "C" int CfRes_getD80Flag();
extern "C" void func_800BCFA0(cf::CfObjectMove* self);
extern "C" void func_800BE824(cf::CfResObjParent* parent, int flag);
extern "C" void func_804B0A6C(u8* subObj, u8* handle);
extern "C" int func_801BFE20(int a, int b, u8* c, float f1, float f2);
extern "C" cf::SoundSlotEntry* func_801BFAE4(u16 handle);

// Model-layer global passed as the first func_80489A60 argument (.sbss).
// CScn* to stay compatible with CfObjectMove.hpp's declaration (both TUs
// include this header).
