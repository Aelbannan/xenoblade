#pragma once

#include <types.h>
#include "monolib/work/CTTask.hpp"
#include "kyoshin/CTitle.hpp"

// CRTP task base - canonical monolib template (declared-only members so the
// unit cpp can emit the retail out-of-line Move__20CTTask<10CMenuTitle>Fv /
// Draw__20CTTask<10CMenuTitle>Fv symbols via explicit `template<>`
// specializations). Member-function-pointer callbacks are dispatched by MWCC
// through the retail __ptmf_test / __ptmf_scall machinery (r3=this, r12=&ptmf).

class CMenuTitle : public CTTask<CMenuTitle> {
public:
    CMenuTitle();

    // TODO: add fields
};

// Allocating factory ctor (retail unmangled symbol `__ct__CMenuTitle`): builds
// a fresh CMenuTitle object (0xF0 bytes), registers it under the passed parent
// and returns the stored singleton (or 0 when one already exists). C linkage
// matches retail; the definition in the .cpp inherits it from this declaration.
extern "C" CMenuTitle* __ct__CMenuTitle(CMenuTitle* _this, CProcess* parent, u32 arg2);

// Ctor-time view of the object layout built by __ct__CMenuTitle.
// 0x00-0x53 matches CTask<CMenuTitle> (CProcess base + two null ptmf slots).
struct CMenuTitleCtorShim {
    u8 _00[0x10];        // 0x00-0x0F CDoubleListNode
    void* vtable;        // 0x10
    u8 _14[0x28];        // 0x14-0x3B rest of CProcess
    u32 callbacks[6];    // 0x3C-0x53 null task callbacks (2x 12-byte ptmf)
    char* field54;       // 0x54 secondary-base vtable (final vtbl + 0x24)
    char* field58;       // 0x58 secondary-base vtable (final vtbl + 0xAC)
    CProcess* parent;    // 0x5C
    CTitle mTitle;       // 0x60 (0x88 bytes)
    u8 field_e8;         // 0xE8
    u8 field_e9;         // 0xE9
    u8 field_ea;         // 0xEA
    u8 _pad_eb;          // 0xEB
    f32 field_ec;        // 0xEC
}; // sizeof 0xF0

// Opaque sub-menu object embedded at offset 0x60 of the menu object handled by
// the func_802B5F58 / func_802B60CC controller handlers in this TU.
class CMenuTitleSub {};

// Layout of the menu object passed to the controller handlers.
// 0x60: sub-menu object (func_802B75B8/775C/75D8/7650/76D4 target).
// 0xE8/0xE9/0xEA: state bytes written/read by the handler.
struct CMenuTitleInput {
    u8 _pad00[0x60];           // 0x00-0x5F
    CMenuTitleSub mSub;        // 0x60 (empty class, sizeof 1)
    u8 _pad61[0xE8 - 0x61];    // 0x61-0xE7
    u8 field_e8;               // 0xE8
    u8 field_e9;               // 0xE9
    u8 field_ea;               // 0xEA
    f32 field_ec;              // 0xEC
};


// Sub-menu handler statics (plain C symbols) called on the +0x60 subobject.
extern "C" int func_802B7564(CMenuTitleSub* sub);
extern "C" int func_802B7590(CMenuTitleSub* sub);
extern "C" int func_802B775C(CMenuTitleSub* sub);
extern "C" void func_802B75D8(CMenuTitleSub* sub);
extern "C" void func_802B7630(CMenuTitleSub* sub);
extern "C" void func_802B7650(CMenuTitleSub* sub);
extern "C" void func_802B76D4(CMenuTitleSub* sub);
extern "C" void func_802B7800(CMenuTitleSub* sub);

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void __dt__10CMenuTitleFv(void* self);
extern "C" void cbRenderBefore__10CMenuTitleFv(void* self);

// Factory-ctor imports (retail symbol names).
extern "C" void __ct__8CProcessFv(CProcess* self);
extern "C" CTitle* __ct__CTitle(CTitle* self);
extern u32 __ptmf_null[3];
extern char lbl_eu_8053B1AC[]; // intermediate vtable (written first)
extern char lbl_eu_8053B0B8[]; // final CMenuTitle vtable
// Singleton pointer for the title menu (.sbss).
extern CMenuTitle* lbl_eu_80664C30;

// sdata2 float constants referenced by the input handlers.
extern const f32 lbl_eu_80668FD0; // threshold / set-point for field_ec
extern const f32 lbl_eu_80668FD4;
extern const f32 lbl_eu_80668FD8; // per-frame increment for field_ec

