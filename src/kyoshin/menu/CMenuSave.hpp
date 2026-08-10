#pragma once

#include <types.h>
#include <monolib/work/CProcess.hpp>
#include <monolib/scn/IScnRender.hpp>

#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CSaveLoad.hpp"

/*
 * Menu save/load screen process.
 *
 * Layout (constructor / destructor / Term):
 *   0x00: CProcess          -- task-system base (0x3C) + vtable PMF data
 *   0x54: u8                -- phase/state flag (written by func_8028E768)
 *   0x58: IScnRender        -- render-callback subobject (member, vptr)
 *   0x5C: CProcess*         -- parent process reference
 *   0x60: CBgTex            -- background layout widget
 *   0x80: CTitleAHelp       -- title/help bar
 *   0xB8: CSaveLoad storage -- save/load data region (0x150 bytes; kept as an
 *                              opaque byte region because CSaveLoad's declared
 *                              sizeof is larger than its real retail extent)
 *  0x208: u8 mField208
 *  0x20A: u8 mField20A
 *  0x20B: u8 mState
 *  total: 0x20C             -- sizeof(CMenuSave) (allocate literal in func_8028E3B4)
 */
class CMenuSave : public CProcess {
public:
    virtual ~CMenuSave();
    virtual void Init();
    virtual void Term();
    virtual void Move();
    void cbRenderBefore();

    u32 ptmf0[3];                 // 0x3C-0x47: vtable pointer-to-member-function data
    u32 ptmf1[3];                 // 0x48-0x53: vtable pointer-to-member-function data
    u8 mField54;                  // 0x54: phase flag
    u8 mField55;                  // 0x55
    u8 _pad56[2];                 // 0x56-0x57
    IScnRender mIScnRender;       // 0x58
    CProcess* mParentRef;         // 0x5C
    CBgTex mBgTex;                // 0x60
    CTitleAHelp mTitleAHelp;      // 0x80
    u8 mSaveLoad[0x150];          // 0xB8-0x207: CSaveLoad storage
    u8 mField208;                 // 0x208
    u8 mField209;                 // 0x209
    u8 mField20A;                 // 0x20A
    u8 mState;                    // 0x20B
};

// Flat view of cf::CfPadData exposing only the flag words func_8028E530 reads:
// CPad::mPressedButtonFlags at +0x4 and CfPadData::mTurboPressButtonFlags at
// +0x104 (see kyoshin/cf/CfPadData.hpp for the full layout).
struct MenuSavePadData {
    u32 mPadHeldFlags;          // 0x00 - CPad::mHeldButtonFlags
    u32 mPadPressedFlags;       // 0x04 - CPad::mPressedButtonFlags
    u8 _pad[0xFC];              // 0x08-0x103 - rest of the pad data
    u32 mTurboPressButtonFlags; // 0x104
};

// IScnRender vtable this-adjusting thunks (retail: subi r3, r3, 0x58; b ...).
extern "C" void func_8028E7B8(void* self);
extern "C" void func_8028E7C0(void* self);

// Shared singleton state flag (cleared by Term, set by func_8028E3B4).
extern unsigned long lbl_eu_806649E8;

// Render-gate mode bitfield (.sbss; bit 21 = realtime event busy).
extern u32 lbl_eu_80663E28;

// vtable / PMF data (MWCC does not mangle global-scope data names).
extern u32 __ptmf_null[3];
extern char lbl_eu_8052BF70[];   // CMenuSave temporary vtable (pre-PMF copy)
extern char lbl_eu_80538710[];   // CMenuSave composite vtable
// Message table base used by Init's CTitleAHelp name lookup (+0xb string).
extern char lbl_eu_8050F7B0[];

// Retail-unmangled callee names (US strips mangling for these func_ helpers).
// isIdle/func_801C3E34/... are declared int (not u8) so callers compare with
// cmpwi directly (no byte mask), matching retail.
extern "C" int isIdle__11CTitleAHelpFv(CTitleAHelp* h);
extern "C" int func_801C3E34(CBgTex* self);
extern "C" void func_801C3D9C(CBgTex* self);
extern "C" int func_801C4114(CTitleAHelp* self);
extern "C" void func_801C40A0(CTitleAHelp* self);
extern "C" void func_801C412C(CTitleAHelp* self);
extern "C" void func_801C41E8(CTitleAHelp* self, u8 arg);
extern "C" int func_8028F664(CSaveLoad* self);
extern "C" int func_8028F5C4(CSaveLoad* self);
extern "C" u8 func_8028FFD4(CSaveLoad* self);
extern "C" void func_8028F4AC(CSaveLoad* self);
extern "C" void func_8028F6DC(CSaveLoad* self);
extern "C" void func_80240420();
extern "C" void func_8008294C__Q22cf13CfGameManagerFv(u8 enable);
extern "C" void func_80138078__FUl(u32 op);

// Additional C-ABI imports used by the Move/cbRenderBefore/Init handlers
// (retail symbols are unmangled / non-namespace, so C linkage is required).
extern "C" int func_8013BE50();
extern "C" void func_8028E450(CMenuSave* self);
extern "C" void func_8028E4E0(CMenuSave* self);
extern "C" void func_8028E530(CMenuSave* self);
extern "C" void func_8028E768(CMenuSave* self);
extern "C" void func_801C3D54(CBgTex* self);
extern "C" void func_801C3C14(CBgTex* self);
extern "C" void func_801C3D7C(CBgTex* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void func_801C3FF0(CTitleAHelp* self);
extern "C" void func_801C414C(CTitleAHelp* self);
extern "C" void func_801C4080(CTitleAHelp* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void CTitleAHelp_load(CTitleAHelp* self);
extern "C" void func_8028F2CC(CSaveLoad* self);
extern "C" void func_8028F3D4(CSaveLoad* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" int func_8028F66C(CSaveLoad* self);
extern "C" int func_8028FEC4(CSaveLoad* self);
extern "C" void func_8028FC18(CSaveLoad* self);
extern "C" void func_8028FE50(CSaveLoad* self);
extern "C" void func_8028F774(CSaveLoad* self);
extern "C" void func_8028FECC(CSaveLoad* self);
extern "C" void func_8028F7D0(CSaveLoad* self);
extern "C" void func_8028F904(CSaveLoad* self);
extern "C" void func_8028FA54(CSaveLoad* self);
extern "C" void func_8028FB20(CSaveLoad* self);
extern "C" void func_8028F23C__9CSaveLoadFv(CSaveLoad* self);
extern "C" int func_800FEDF8();
extern "C" void func_800FF914();
extern "C" int func_80086F9C__Q22cf13CfGameManagerFv(int arg);
extern "C" void __ct__8CProcessFv(CProcess* self);
extern "C" void __ct__CBgTex(void* self, int arg);
extern "C" void __ct__CTitleAHelp(void* self, char* name, int id);
extern "C" void __ct__CSaveLoad(void* self, u8 argA, u8 argB);
extern "C" void __ct__UnkClass_8011C974(void* dest, void* src);
extern "C" void func_80240360();
extern "C" char* func_80136190(char* a, char* b, int id);
extern "C" void func_8016742C(void* dest, void* src);
extern "C" void __ct__Q34nw4r3lyt8DrawInfoFv(void* self);
extern "C" void __dt__Q34nw4r3lyt8DrawInfoFv(void* self, int dealloc);
extern "C" void addRenderCB__4CScnFP10IScnRenderUlUl(CScn* self, IScnRender* cb, u32 prio, u32 flag);

// DrawInfo helper - retail emits the mangled nw4r form
// (func_80137250__FPQ34nw4r3lyt8DrawInfo), so it is declared as a normal C++
// global function to let MWCC mangle it - NOT inside the extern "C" block.
void func_80137250(nw4r::lyt::DrawInfo* drawInfo);

// Destructor retail symbols (kept mangled in US; the dtor D2 form addresses
// them by their literal names; __dt__800FED0C is the game-side CProcess dtor
// wrapper defined in CMainMenu.cpp).
extern "C" void cbRenderBefore__9CMenuSaveFv(void* self);
extern "C" void* __dt__800FED0C(void* self, int flags);
extern "C" CSaveLoad* __dt__9CSaveLoadFv(CSaveLoad* self, int flags);
extern "C" CTitleAHelp* __dt__11CTitleAHelpFv(CTitleAHelp* self, int flags);
extern "C" CBgTex* __dt__6CBgTexFv(CBgTex* self, int flags);
