#pragma once

#include <types.h>
#include "monolib/work/CProcess.hpp"

// Local CRTP task base (mirrors monolib/work/CTTask.hpp) with out-of-line
// template specializations emitted in the unit cpp so they produce the retail
// Move__20CTTask<10CMenuTitle>Fv / Draw__20CTTask<10CMenuTitle>Fv symbols.
// Member-function-pointer callbacks are dispatched by MWCC through the retail
// __ptmf_test / __ptmf_scall machinery (r3=this, r12=&ptmf).
template <typename TDerived>
class CTTask : public CProcess {
public:
    CTTask() : mMoveFunc(nullptr), mDrawFunc(nullptr) {}
    virtual void Move();
    virtual void Draw();

protected:
    void (TDerived::*mMoveFunc)(); // 0x3C
    void (TDerived::*mDrawFunc)(); // 0x48
}; // size: 0x54

class CMenuTitle : public CTTask<CMenuTitle> {
public:
    CMenuTitle();

    // TODO: add fields
};

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

// sdata2 float constants referenced by the input handlers.
extern const f32 lbl_eu_80668FD0; // threshold / set-point for field_ec
extern const f32 lbl_eu_80668FD4;
extern const f32 lbl_eu_80668FD8; // per-frame increment for field_ec

