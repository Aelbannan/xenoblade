#pragma once

#include <types.h>

// C-ABI retail symbols. functions.hpp (pulled in via monolib/scn.hpp if
// CTaskGame.hpp is included) declares the C++-mangled bool f(int) for
// func_8009CF8C, which would win the C-name resolution and emit a byte-masked
// return; CTaskGame.hpp is therefore not included here and the two CTaskGame
// statics are addressed by their literal retail names instead.
extern "C" u32 func_8009CF8C(u32);
class CTaskGame;
extern "C" CTaskGame* getInstance__9CTaskGameFv();
extern "C" int isFlag01Set__9CTaskGameFv();

#include <monolib/work/CProcess.hpp>
#include <monolib/scn/IScnRender.hpp>
#include <monolib/core/CPadManager.hpp>

#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CFade.hpp"
#include "kyoshin/CFloorMap.hpp"
#include "monolib/scn/CScn.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

// World map screen UI process (split-screen version).
// Inherits from CProcess (task system) and owns an IScnRender render-callback
// subobject (split-screen version of the CMenuOption layout).
// IScnRender subobject is at offset 0x58, requiring this-adjusting thunks.
//
// Layout (from retail ctor / dtor extab):
//   0x00: CProcess
//   0x3C-0x53: null PMF callback slots (2 groups of 3 words)
//   0x54/0x55: u8 phase flags
//   0x58: IScnRender (member subobject, vptr)
//   0x5C: CProcess* parent reference
//   0x60: CTitleAHelp
//   0x98: CFade
//   0xC0: CFloorMap (0x3348 bytes)
//  0x3408: u8 phase/state (Move()'s jumptable selector)
//  0x340C: f32 timer
class CMenuMapSelectSC : public CProcess {
public:
    virtual ~CMenuMapSelectSC();
    virtual void Init();
    virtual void Term();
    virtual void Move();
    void cbRenderBefore();

    CMenuMapSelectSC();

    u32 ptmf0[3];              // 0x3C-0x47: null PMF callback slot group 1
    u32 ptmf1[3];              // 0x48-0x53: null PMF callback slot group 2
    u8 mField54;               // 0x54
    u8 mField55;               // 0x55
    u8 _pad56[2];              // 0x56-0x57
    IScnRender mIScnRender;    // 0x58: render-callback subobject (vptr)
    CProcess* mParentRef;      // 0x5C
    CTitleAHelp mTitleAHelp;   // 0x60
    CFade mFade;               // 0x98
    CFloorMap mFloorMap;       // 0xC0
    u8 mState;                 // 0x3408
    u8 _3409[3];               // 0x3409-0x340B
    f32 mTimer;                // 0x340C
};

// Retail-unmangled callee names (US strips mangling for these func_ helpers);
// declared extern "C" so call sites emit the literal retail reloc names.
// Mirrors the CMenuOption/CMenuGCItem pattern for the same helpers. Return
// types are int (not u8) so callers compare with cmpwi directly (no byte
// mask), matching retail.
extern "C" int func_801C4114(CTitleAHelp* self);
extern "C" void func_801C4654(CTitleAHelp* self, u32 arg);
extern "C" void func_801C46B4(CTitleAHelp* self, char* name);
extern "C" void func_801C46DC(CTitleAHelp* self, u32 arg);
extern "C" void func_801C41E8(CTitleAHelp* self, u8 arg);
extern "C" void func_801C412C(CTitleAHelp* self);
extern "C" void func_801C3FF0(CTitleAHelp* self);
extern "C" int func_80244508(CFade* self);
extern "C" int func_80244510(CFade* self);
extern "C" void func_80244518(CFade* self);
extern "C" void func_80244538(CFade* self);
extern "C" void func_8024439C(CFade* self);
extern "C" void func_802443E8(CFade* self);
extern "C" void func_8024CB94(CFloorMap* self);
extern "C" u16 func_8024F54C(CFloorMap* self);
extern "C" int func_8024CE1C(CFloorMap* self);
extern "C" void func_8024BE1C(CFloorMap* self);
extern "C" void func_8024C1FC(CFloorMap* self);
extern "C" int func_8024F538(CFloorMap* self);
extern "C" int func_8024F6E0(CFloorMap* self);
extern "C" u8 func_8024F630(CFloorMap* self);
extern "C" u8 func_8024F6D8(CFloorMap* self);
extern "C" void func_8024F5C4(CFloorMap* self, u32 arg);
extern "C" u8 func_8024F6BC(CFloorMap* self);
extern "C" int func_8024F784(CFloorMap* self);
extern "C" void func_8024CE68(CFloorMap* self);
extern "C" void func_8024D23C(CFloorMap* self);
extern "C" void func_8024EE50(CFloorMap* self);
extern "C" u8 func_8024F554(CFloorMap* self);
extern "C" void func_8024F55C(CFloorMap* self);
extern "C" void func_8024D614(CFloorMap* self);
extern "C" void func_8024DA0C(CFloorMap* self);
extern "C" void func_8024DE08(CFloorMap* self);
extern "C" void func_8024E2BC(CFloorMap* self);
extern "C" void func_8024F658(CFloorMap* self);
extern "C" void func_8024F72C(CFloorMap* self);
extern "C" void func_8024E650(CFloorMap* self);
extern "C" void func_8024E828(CFloorMap* self);
extern "C" void func_8024EA00(CFloorMap* self);
extern "C" void func_8024EC24(CFloorMap* self);
extern "C" void func_8024F7CC(CFloorMap* self);

// Retail symbol keeps its mangled form (CfGameManager static member); the
// call site passes (u16 value, 0) even though the name says Fv.
extern "C" void func_8008413C__Q22cf13CfGameManagerFv(u16 a, u32 b);

// D2-form destructor helper (explicit delete flag), referenced by the
// IScnRender dtor thunk so r4 (delete flag) passes through untouched.
extern "C" CMenuMapSelectSC* __dt__16CMenuMapSelectSCFv(CMenuMapSelectSC* _this, int flags);
// cbRenderBefore referenced by its retail symbol from the IScnRender thunk
// (cbRenderBefore is virtual via IScnRender, so a direct member call would
// dispatch through the vtable instead of tail-calling).
extern "C" void cbRenderBefore__16CMenuMapSelectSCFv(char* self);

// IScnRender vtable this-adjusting thunks (retail: subi r3, r3, 0x58; b ...).
void func_80251DE8(IScnRender* self);
void func_80251DF0(IScnRender* self);

// World-map phase helpers (retail-unmangled callee names).
extern "C" void func_80251D4C(CMenuMapSelectSC* self);
extern "C" void func_80251D94(CMenuMapSelectSC* self);
extern "C" void func_80251560(CMenuMapSelectSC* self);
extern "C" void func_802515B8(CMenuMapSelectSC* self);
extern "C" void func_80251628(CMenuMapSelectSC* self);
extern "C" void func_802516DC(CMenuMapSelectSC* self);

// ---------------------------------------------------------------------------
// Additional retail-unmangled callee names for the world-map (SC) screen.
// ---------------------------------------------------------------------------
// CScn-side helpers: scene enable flag + render-callback management.
// int return (mismatched vs the (void*, u8) definition) blocks -ipa inlining
// of the 1-line body, keeping the retail `bl func_804962A0` call site.
extern "C" int func_804962A0(CScn* scn, int flag);
// CFloorMap helpers (retail unmangles these member helpers).
extern "C" void func_8024F1FC(CFloorMap* self, u8 mapIdx);
extern "C" void func_8024C104(CFloorMap* self);
extern "C" void func_8024C8F8(CFloorMap* self, nw4r::lyt::DrawInfo* drawInfo);
// CTitleAHelp / CFade draw + reset helpers.
extern "C" void func_801C40A0(CTitleAHelp* self);
extern "C" void func_801C4080(CTitleAHelp* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void func_80244460(CFade* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void func_8024448C(CFade* self);
// UI/sound + scene-active helpers.
extern "C" void func_8013EC6C(u32 a, u32 b);
extern "C" int func_8013BE50();
extern "C" void setPresentationFlag__Q22cf13CfGameManagerFv(bool enable);
// Retail constructor (unmangled global, ctor with explicit arg).
extern "C" CMenuMapSelectSC* __ct__CMenuMapSelectSC(CMenuMapSelectSC* self, CProcess* arg);
// Ctor/re-init copy helpers for the embedded widgets (retail-unmangled).
extern "C" void __ct__8CProcessFv(CProcess* self);
extern "C" void __ct__CTitleAHelp(CTitleAHelp* self, char* name, u8 arg);
extern "C" void __ct__CFade(CFade* self);
extern "C" void __ct__CFloorMap(CFloorMap* self);
extern "C" void __ct__UnkClass_8011C974(void* dest, void* src);
extern "C" void func_8016742C(void* dest, void* src);
extern "C" void CTitleAHelp_load(CTitleAHelp* self);
extern "C" char* func_80136190(char* a, char* b, u32 id);
// D2-form subobject/base destructor helpers (explicit delete flags), used by
// the free-function form of ~CMenuMapSelectSC. __dt__800FED0C is the CProcess
// base dtor retail symbol (the game-side D2 wrapper, distinct from the library
// D1 __dt__8CProcessFv).
extern "C" void __dt__9CFloorMapFv(CFloorMap* _this, int flags);
extern "C" void __dt__5CFadeFv(CFade* _this, int flags);
extern "C" void __dt__11CTitleAHelpFv(CTitleAHelp* _this, int flags);
extern "C" void __dt__800FED0C(CProcess* _this, int flags);

// C++-linkage helpers - retail emits the mangled forms
// playUISound__FUl / func_80137250__FPQ34nw4r3lyt8DrawInfo.
void playUISound(unsigned long op);
void func_80137250(nw4r::lyt::DrawInfo* pDrawInfo);
// nw4r DrawInfo pre-mangled ct/dt names (retail emits the direct calls; a C++
// local would virtual-dispatch its scope-exit destructor and bloat the body).
extern "C" void __ct__Q34nw4r3lyt8DrawInfoFv(nw4r::lyt::DrawInfo* drawInfo);
extern "C" void __dt__Q34nw4r3lyt8DrawInfoFv(nw4r::lyt::DrawInfo* drawInfo, int flags);

// Data imports (MWCC does not mangle global-scope data names).
extern u32 lbl_eu_80664184;   // current map id (bdat row) -> floor map select
extern u32 lbl_eu_806647C0;   // CMenuMapSelectSC singleton pointer (sbss)
extern u32 lbl_eu_80663E28;   // global mode bitfield (bit 0x400 = world-map gate)
extern u8 lbl_eu_8052BF70[];  // CMenuMapSelectSC temporary vtable (pre-PMF copy)
extern u8 lbl_eu_80537130[];  // CMenuMapSelectSC final vtable; IScnRender vtable at +0x24
extern u32 __ptmf_null[3];    // null pointer-to-member-function constant
extern char lbl_eu_8050C5A4[];  // world-map title/help string table base
extern const f32 lbl_eu_806687C0;  // 0.0f - world-map timer reset value
extern const f32 lbl_eu_806687C4;  // 1.0f - world-map timer increment per frame
extern const f32 lbl_eu_806687C8;  // 10.0f - world-map timer cap
