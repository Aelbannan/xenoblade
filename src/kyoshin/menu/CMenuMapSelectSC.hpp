#pragma once

#include <types.h>
#include <monolib/work/CProcess.hpp>
#include <monolib/scn/IScnRender.hpp>

#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CFade.hpp"
#include "kyoshin/CFloorMap.hpp"

// World map screen UI process (split-screen version).
// Inherits from CProcess (task system) and IScnRender (render callback).
// IScnRender subobject is at offset 0x58, requiring this-adjusting thunks.
//
// Layout (from retail ctor / dtor extab):
//   0x00: CProcess
//   0x3C-0x53: null PMF callback slots (2 groups of 3 words)
//   0x54/0x55: u8 phase flags
//   0x58: IScnRender (second base, vptr)
//   0x5C: CProcess* parent reference
//   0x60: CTitleAHelp
//   0x98: CFade
//   0xC0: CFloorMap (0x3348 bytes)
//  0x3408: u8 phase/state (Move()'s jumptable selector)
//  0x340C: f32 timer
class CMenuMapSelectSC : public CProcess, public IScnRender {
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
extern "C" int func_80244508(CFade* self);
extern "C" int func_80244510(CFade* self);
extern "C" void func_80244518(CFade* self);
extern "C" void func_8024CB94(CFloorMap* self);
extern "C" u16 func_8024F54C(CFloorMap* self);

// Retail symbol keeps its mangled form (CfGameManager static member); the
// call site passes (u16 value, 0) even though the name says Fv.
extern "C" void func_8008413C__Q22cf13CfGameManagerFv(u16 a, u32 b);

// D2-form destructor helper (explicit delete flag), referenced by the
// IScnRender dtor thunk so r4 (delete flag) passes through untouched.
extern "C" void __dt__16CMenuMapSelectSCFv(void* _this, int flags);
// cbRenderBefore referenced by its retail symbol from the IScnRender thunk.
extern "C" void cbRenderBefore__16CMenuMapSelectSCFv(void* self);

// IScnRender vtable this-adjusting thunks (retail: subi r3, r3, 0x58; b ...).
void func_80251DE8(IScnRender* self);
void func_80251DF0(IScnRender* self);

// World-map phase helpers (retail-unmangled callee names).
void func_80251D4C(CMenuMapSelectSC* self);
void func_80251D94(CMenuMapSelectSC* self);
void func_80251560(CMenuMapSelectSC* self);
