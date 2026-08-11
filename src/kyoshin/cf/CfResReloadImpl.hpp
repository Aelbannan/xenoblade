#pragma once

#include <types.h>

namespace cf {

// Forward decl (needed by the vtable struct below).
struct CfResReloadImpl;

// Parent object referenced at +0x00 of CfResReloadImpl; the flag words at
// +0x68/+0x6C and the resource pointer at +0x70 are read/written here.
struct CfResReloadParent {
    u8 field_00[0x64];
    /* 0x64 */ u32 field_64;  // flags (bit 16 = 0x10000 tested by func_8016D2FC)
    /* 0x68 */ u32 field_68;  // flags (bit 26 = 0x04000000 tested by func_8016D390)
    /* 0x6C */ u32 field_6C;  // flags (bit 2 tested by func_8016DE68)
    /* 0x70 */ u8* field_70;  // resource pointer passed to func_eu_80063174
};

// Cast-only fake SI interface for the secondary vtable at +0x10 (see
// MWCC_REFERENCE "double-hop thunks"): the non-polymorphic Shift base puts the
// vptr at object+0x10, so a virtual call emits `lwz r12,0x10(r3);
// lwz r12,slot(r12); mtctr; bctr` with `this` staying at the object base.
struct CfResReloadShift {
    u8 field_00[0x10];
};

struct CfResReloadVtIf : CfResReloadShift {
    virtual void _v008();
    virtual void _v00C();
    virtual void _v010();
    virtual void _v014();
    virtual void _v018();
    virtual void _v01C();
    virtual void _v020();
    virtual void _v024();
    virtual void _v028();  // vtable offset 0x28 - dispatched by func_8016DE68
    virtual void _v02C();
    virtual void _v030();
    virtual int _v034(int);  // vtable offset 0x34 - called with (this, 1) by func_8016CF24
};

// Resource reload implementation.
// Manages reloading of resources with a timer and state tracking.
// Inherits from CfResImpl (base class defines fields 0x00-0x13).
struct CfResReloadImpl {
    /* 0x00 */ CfResReloadParent* field_00;   // parent/reference pointer
    /* 0x04 */ f32 field_04;                  // timer or delay float
    /* 0x08 */ u16 field_08;                  // type/category (also PMTF index in func_8016DE8C)
    /* 0x0A */ s16 field_0A;                  // state/param (-1 = invalid; lha by callers)
    /* 0x0C */ u16 field_0C;                  // counter
    /* 0x0E */ u16 field_0E;                  // state/param (-1 = invalid)
    /* 0x10 */ void* field_10;  // secondary vtable pointer (via CfResReloadVtIf cast)
    /* 0x14 */ u32 field_14[2];               // work buffer (2 words)
    /* 0x1C */ u16 field_1C;                  // reload count/state
    /* 0x1E */ u8 field_1E;                   // flags
    /* 0x1F */ u8 field_1F;                   // flags

    // vtable slot 6: returns a resource type identifier (12 for reload impl)
    int func_8016CF1C();

    // vtable slot 23: returns a resource sub-type identifier (1 for reload impl)
    int func_8016DECC();
};

// Entry returned by the CfRes table lookup func_80062EC4 (indexed by the
// state field); only +0x00 (flags) and +0x32 (attribute id) are read here.
struct CfResLookupEntry {
    /* 0x00 */ u32 field_00;  // flags (bit 0x800 = present)
    u8 field_04[0x2E];        // 0x04..0x31
    /* 0x32 */ u8 field_32;   // attribute id
};

// 0x10-byte reload-info struct written by initReloadInfoStruct and read back
// by func_8016E100.
struct ReloadInfo {
    /* 0x00 */ f32 field_00;
    /* 0x04 */ u16 field_04;
    /* 0x06 */ u16 field_06;
    /* 0x08 */ u16 field_08;
    /* 0x0A */ u16 field_0A;
    /* 0x0C */ u16 field_0C;
    /* 0x0E */ u16 field_0E;
};

} // namespace cf

// PMTF dispatch table (4 x 12-byte member pointers) selected by field_08 in
// func_8016DE8C. Declared at global scope so MWCC keeps the retail name
// unmangled; the (this->*table[idx])() call lowers to `bl __ptmf_scall`.
typedef void (cf::CfResReloadImpl::*CfResReloadImplPMF)();
extern CfResReloadImplPMF lbl_eu_80530FC0[4];

// Free-function imports used by this unit (defined in CfRes.cpp /
// CfObjectModel.cpp). extern "C" keeps the call-site reloc at the plain
// retail name - a C++ declaration makes MWCC emit a mangled __F<params>
// name (same convention as CfObjectModel.hpp).
extern "C" cf::CfResLookupEntry* func_80062EC4(int);
extern "C" int func_80062998(int, int, int);
extern "C" void func_eu_80063174(int, u8*);
extern "C" void func_800BAB64(cf::CfResReloadParent*);
// More C-ABI imports (defined in CfSoundMan.cpp / CfRes.cpp /
// code_801A929C.cpp); same extern "C" convention as above.
extern "C" void func_801BFE8C(u32 a, u32 b, u32 c);
extern "C" void func_801BFF04(int a, int b, int c, int d);
extern "C" int func_80063A60(int a);
extern "C" int func_801AAAA0(int a);

// Flag words defined in CUICfManager.cpp (.sbss); read by func_8016D2FC /
// func_8016E9CC.
extern u32 lbl_eu_80663E24;
extern u32 lbl_eu_80663E28;
// Delay/timer floats (.sdata2) selected by func_8016E9CC.
extern float lbl_eu_806676B4;
extern float lbl_eu_806676B8;

// Secondary-interface vtable stored at +0x10 by the constructor (.data).
extern void* lbl_eu_80530FF0[];
// Delay/timer float seeded by the constructor (.sdata2).
extern float lbl_eu_80667698;

// Reload-state globals owned by this unit (.sbss / .sdata2). Global-scope
// declarations keep the retail symbol names unmangled.
extern u16 lbl_eu_80664278;
extern u16 lbl_eu_8066427A;
extern u16 lbl_eu_8066427C;
extern u16 lbl_eu_8066427E;
extern u16 lbl_eu_80664280;
extern float lbl_eu_80664284;
extern u16 lbl_eu_80664288;
extern float lbl_eu_806676C8;
