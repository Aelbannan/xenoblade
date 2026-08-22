#pragma once

#include <types.h>

#include "monolib/work/CProcess.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CTitleAHelp.hpp"
#include <nw4r/lyt.h>

class CScn;
class CMenuPassiveSkill;

extern "C" void Init__17CMenuPassiveSkillFv();
extern "C" void Move__17CMenuPassiveSkillFv();
extern "C" __declspec(noinline) CMenuPassiveSkill* __ct__CMenuPassiveSkill(CMenuPassiveSkill* self, u32 arg);
extern "C" void __ct__8CProcessFv(CProcess* self);
extern "C" CMenuPassiveSkill* __dt__17CMenuPassiveSkillFv(CMenuPassiveSkill* self, int flags);
extern "C" CMenuPassiveSkill* func_802638D0(CProcess* parent, u32 arg);
extern "C" unsigned long func_80263944();
extern "C" void func_80263954(CMenuPassiveSkill* self);
extern "C" void func_802639E4();
extern "C" void func_80263A34();
extern "C" void func_80263D3C();
extern "C" void func_80263D8C();
extern "C" void func_80263DE8();
extern "C" void func_80263E4C();

/*
 * Passive skill menu screen process.
 * IScnRender subobject is at offset 0x58 (secondary base).
 */
// Intermediate base pads the CProcess head out to 0x58 so the IScnRender
// secondary base lands at the retail offset (same trick as CMenuSkipTimer's
// CMenuSkipTimerBase).
class CMenuPassiveSkillBase : public CProcess {
public:
    u8 _pad40[0x1C]; // 0x40..0x5B
};

class CMenuPassiveSkill : public CMenuPassiveSkillBase, public IScnRender {
public:
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // IScnRender vtable this-adjusting thunks
    void func_80263EAC();
    void func_80263EB4();

    // --- member fields ---
    CScn* mScnRef;            // 0x5C - owning scene (removeRenderCB target)
    CBgTex mBgTex;            // 0x60
    CTitleAHelp mTitleAHelp;  // 0x80
    /* UI::CPassiveSkill sub-object at 0xB8 (opaque here; its full layout lives
     * in kyoshin/CPassiveSkill.hpp whose tail padding would swallow 0x2AC). */
    u8 mPassiveSkill[0x1F4];  // 0xB8..0x2AB
    u8 field_2AC;             // 0x2AC - widgets-ready flag set by func_80263954
    u8 _pad2AD[0x7];          // 0x2AD..0x2B3
};

// ---------------------------------------------------------------------------
// Retail-unmangled imports (US strips these member manglings; C linkage binds
// the verbatim symbol names).
// ---------------------------------------------------------------------------
extern "C" void removeRenderCB__4CScnFP10IScnRender(CScn* scn, IScnRender* render);
extern "C" void func_801C3D9C(CBgTex* self);
extern "C" u8 func_801C3E34(CBgTex* self);
extern "C" void func_801C3D7C(CBgTex* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void func_801C40A0(CTitleAHelp* self);
extern "C" u8 func_801C4114(CTitleAHelp* self);
extern "C" void func_801C412C(CTitleAHelp* self);
extern "C" void func_801C41E8(CTitleAHelp* self, u8 arg);
extern "C" void func_801C4080(CTitleAHelp* self, nw4r::lyt::DrawInfo* drawInfo);

// UI::CPassiveSkill helpers (first arg is the +0xB8 sub-object).
extern "C" void func_8026D920(u8* self);
extern "C" u8 func_8026D9AC(u8* self);
extern "C" void func_8026DA4C(u8* self);
extern "C" u8 func_8026DB74(u8* self);
extern "C" void func_8026D8FC(u8* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void __dt__Q22UI13CPassiveSkillFv(u8* self, int flags);

// Destructor retail symbols / game-side CProcess D2 wrapper (CMainMenu.cpp).
extern "C" void __dt__11CTitleAHelpFv(CTitleAHelp* self, int flags);
extern "C" void __dt__6CBgTexFv(CBgTex* self, int flags);
extern "C" void __dt__800FED0C(CProcess* self, int flags);

// cf pad enable/disable (retail pre-mangled name), sound-op helper.
extern "C" void func_8008294C__Q22cf13CfGameManagerFv(u8 enable);
extern "C" void func_80138078__FUl(u32 op);

// Scene-active gate + TaskGame queries used by cbRenderBefore.
extern "C" int func_8013BE50();
extern "C" void getInstance__9CTaskGameFv();
extern "C" int func_800426F0__9CTaskGameFv();

// DrawInfo setup helper; the C++ declaration mangles to the retail name
// func_80137250__FPQ34nw4r3lyt8DrawInfo.
void func_80137250(nw4r::lyt::DrawInfo* drawInfo);

// Singleton state: created-screen pointer (cleared by Term) and the shared
// mode bitfield (bit 21 / IBM bit 10 gates cbRenderBefore draws).
extern unsigned long lbl_eu_80664878;
extern u32 lbl_eu_80663E28;
