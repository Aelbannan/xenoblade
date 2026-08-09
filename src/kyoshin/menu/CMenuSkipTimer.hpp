#pragma once

#include "kyoshin/IUICf.hpp"
#include "monolib/scn/IScnRender.hpp"
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

// Skip-timer widget shown during cutscene skipping.
// Inherits from IScnRender for the +0x58 subobject layout (vtable + virtual dispatch).
// NOTE: Does NOT re-declare virtual overrides - the base class IScnRender provides
// the virtual slot; actual dispatch goes through the auto-generated thunks
// (func_8029F098 / func_8029F0A0, written as this-adjusting free functions).
class CMenuSkipTimer : public CMenuSkipTimerBase, public IScnRender {
public:
    CMenuSkipTimer(CProcess* parent);
    ~CMenuSkipTimer();

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

// IScnRender vtable this-adjusting thunks (retail: subi r3, r3, 0x58; b ...).
void func_8029F098(IScnRender* sub);
void func_8029F0A0(IScnRender* sub);

// Retail-unmangled same-TU helpers.
CMenuSkipTimer* func_8029EDE4(CProcess* self, CProcess* parent);
extern "C" void func_8029EE68(CMenuSkipTimer* self);
extern "C" void func_8029EEE0(CMenuSkipTimer* self);
extern "C" void func_8029F048(CMenuSkipTimer* self);

// Retail-unmangled callee names (US strips mangling for these func_ helpers).
extern "C" u8 func_8029FF00(CSkipTimer* self);
extern "C" void func_8029FE30(CSkipTimer* self);
extern "C" void func_801C3D9C(CBgTex* self);
extern "C" void func_801C40A0(CTitleAHelp* self);
extern "C" void func_8008294C__Q22cf13CfGameManagerFv(u8 enable);
extern "C" int isIdle__11CTitleAHelpFv(CTitleAHelp* h);

// Shared singleton pointer for the active skip-timer widget.
extern unsigned long lbl_eu_80664A48;

// Secondary mode bitfield (bit 21 / IBM bit 10 = realtime event busy).
extern u32 lbl_eu_80663E28;

// Retail-unmangled same-unit CSkipTimer / CMenuSkipTimer helpers referenced by
// Move/Init. Declared extern "C" so calls bind the literal (un)mangled retail
// symbol (US strips mangling for these func_ names).
extern "C" u8 func_802A01F0(CSkipTimer* self);
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
extern "C" void func_8029FBE0(CSkipTimer* self);
// String-table base for the skip-timer title/help name lookup.
extern char lbl_eu_80510540[];
// Retail-mangled CfGameManager helper that accepts a pad-channel arg (keeps the
// `li r3, -1` materialisation that retail emits; cf::CfGameManager's inline s16
// overload discards it). Declared extern "C" so the call binds the verbatim
// retail symbol.
extern "C" int func_80086F9C__Q22cf13CfGameManagerFv(int arg);