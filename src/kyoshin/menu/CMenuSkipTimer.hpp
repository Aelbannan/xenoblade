#pragma once

#include "kyoshin/IUICf.hpp"
#include "monolib/scn/CScn.hpp"
#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CSkipTimer.hpp"
#include <types.h>

// Intermediate base: adds two flag bytes after IUICf/CTTask<IUICf> (0x54 bytes).
class CMenuSkipTimerBase : public IUICf {
public:
    u8 mFlag1; // 0x54
    u8 mFlag2; // 0x55
    // 0x56-0x57: padding
};

// Render-callback interface subobject at +0x58 (secondary vtable).
// Deliberately declared WITHOUT a virtual destructor (same trick as
// CMenuFade.hpp): the retail composite vtable (lbl_eu_80539780) fills the
// dtor slot with the manual func_8029F0A0 adjuster, and a virtual dtor here
// would make MWCC emit an extra @88@__dt__14CMenuSkipTimerFv thunk that
// retail does not have. cbRenderBefore stays virtual so MWCC auto-emits the
// @88@cbRenderBefore__14CMenuSkipTimerFv this-adjusting thunk.
class IScnRenderCB {
public:
    virtual void cbRenderBefore() = 0;
};

// Skip-timer widget shown during cutscene skipping.
// Retail layout: CProcess/CTTask base (vptr @0x10), the render-callback
// subobject at +0x58, then the embedded widgets.
class CMenuSkipTimer : public CMenuSkipTimerBase, public IScnRenderCB {
public:
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // --- member fields ---
    CProcess* mParentRef;          // 0x5C - scn/parent reference (removeRenderCB target)
    CBgTex mBgTex;                 // 0x60
    CTitleAHelp mTitleAHelp;       // 0x80
    CSkipTimer mTimerData;         // 0xB8
    u8 mFlag3;                     // 0x150
};

// Retail-unmangled same-TU helpers (C linkage; the definitions live in
// CMenuSkipTimer.cpp and emit the verbatim retail symbols).
extern "C" CMenuSkipTimer* __ct__CMenuSkipTimer(CMenuSkipTimer* self, CProcess* parent);
extern "C" CMenuSkipTimer* __dt__14CMenuSkipTimerFv(CMenuSkipTimer* self, int flags);
extern "C" CMenuSkipTimer* func_8029EDE4(CProcess* self, CProcess* parent);
extern "C" void func_8029F0A0(IScnRenderCB* sub);
extern "C" void func_8029EE68(CMenuSkipTimer* self);
extern "C" void func_8029EEE0(CMenuSkipTimer* self);
extern "C" void func_8029F048(CMenuSkipTimer* self);

// Retail-unmangled callee names (US strips mangling for these func_ helpers).
// int returns so the caller's `!= 0` stays a plain cmpwi (retail emits no u8 mask).
extern "C" int func_8029FF00(CSkipTimer* self);
extern "C" void func_8029FE30(CSkipTimer* self);
extern "C" void func_801C3D9C(CBgTex* self);
extern "C" void func_801C40A0(CTitleAHelp* self);
extern "C" void setPresentationFlag__Q22cf13CfGameManagerFv(u8 enable);
extern "C" int isIdle__11CTitleAHelpFv(CTitleAHelp* h);

// Shared singleton pointer for the active skip-timer widget.
extern unsigned long lbl_eu_80664A48;

// Secondary mode bitfield (bit 21 / IBM bit 10 = realtime event busy).
extern u32 lbl_eu_80663E28;

// Retail-unmangled same-unit CSkipTimer / CMenuSkipTimer helpers referenced by
// Move/Init. Declared extern "C" so calls bind the literal (un)mangled retail
// symbol (US strips mangling for these func_ names). int returns keep the
// caller's `!= 0` a plain cmpwi (retail emits no u8 mask; the defining TU's u8
// return is ABI-compatible, value in r3).
extern "C" int func_802A01F0(CSkipTimer* self);
extern "C" void func_8029FCDC(CSkipTimer* self);
extern "C" void func_8029FBE0(CSkipTimer* self);
extern "C" void func_8029EF30(CMenuSkipTimer* self);
extern "C" void func_801C3D54(CBgTex* self);
extern "C" void func_801C3FF0(CTitleAHelp* self);
// Retail-unmangled CW helpers used by Init()'s temporary-object re-init pattern.
extern "C" void __ct__CBgTex(void*, int);
extern "C" void __dt__6CBgTexFv(void*, int);
extern "C" void __ct__UnkClass_8011C974(void*, void*);
extern "C" void __ct__CTitleAHelp(void*, char*, int);
extern "C" void __dt__11CTitleAHelpFv(void*, int);
extern "C" void __ct__CSkipTimer(void*);
extern "C" void __dt__10CSkipTimerFv(void*, int);
extern "C" char* func_80136190(char*, char*, int);
extern "C" void func_801C3C14(CBgTex* self);
extern "C" void CTitleAHelp_load(CTitleAHelp* self);
// Game-side CProcess dtor wrapper (D2 form; defined in CMainMenu.cpp).
extern "C" void __dt__800FED0C(CProcess* self, int flags);
// Draw/advance helpers for cbRenderBefore / func_8029EF30 (retail keeps the
// bare func_ names at these call sites; int returns keep `!= 0` as cmpwi).
extern "C" int func_8029FEBC(CSkipTimer* self);
extern "C" void func_802A005C(CSkipTimer* self);
extern "C" void func_802A0148(CSkipTimer* self);
extern "C" void func_8029FF24(CSkipTimer* self);
extern "C" void func_8029FF98(CSkipTimer* self);
extern "C" void func_802A0028(CSkipTimer* self);
extern "C" int func_8029FF1C(CSkipTimer* self);
extern "C" void func_801C414C(CTitleAHelp* self);
extern "C" void func_801C3D7C(CBgTex* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void func_8029FDBC(CSkipTimer* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void func_801C4080(CTitleAHelp* self, nw4r::lyt::DrawInfo* drawInfo);
// Raw-storage nw4r DrawInfo build/destroy for cbRenderBefore (pre-mangled names).
extern "C" void __ct__Q34nw4r3lyt8DrawInfoFv(nw4r::lyt::DrawInfo* drawInfo);
extern "C" void __dt__Q34nw4r3lyt8DrawInfoFv(nw4r::lyt::DrawInfo* drawInfo, int flags);
// DrawInfo setup helper; the C++ declaration mangles to the retail name
// func_80137250__FPQ34nw4r3lyt8DrawInfo.
void func_80137250(nw4r::lyt::DrawInfo* drawInfo);
// CProcess base ctor (C-ABI, retail C-linkage symbol name).
extern "C" void __ct__8CProcessFv(CProcess* self);
// Scene-active gate for HUD draws (retail-unmangled; code_80135FDC.cpp).
extern "C" int func_8013BE50();
// Vtable data / null-PTMF constant (defined in .data / .rodata splits).
extern char lbl_eu_8052BF70[];
extern char lbl_eu_80539780[];
extern u32 __ptmf_null[3];
// String-table base for the skip-timer title/help name lookup.
extern char lbl_eu_80510540[];
// Retail-mangled CfGameManager helper that accepts a pad-channel arg (keeps the
// `li r3, -1` materialisation that retail emits; cf::CfGameManager's inline s16
// overload discards it). Declared extern "C" so the call binds the verbatim
// retail symbol.
extern "C" int isClassicController__Q22cf13CfGameManagerFv(int arg);
