#pragma once

#include <types.h>
#include "monolib/work/CProcess.hpp"
#include "monolib/scn/IScnRender.hpp"
#include <nw4r/lyt/lyt_drawInfo.h>

/*
 * System window process (singleton, created by func_80124AEC).
 *
 * Retail class layout (derived from the Init/Term/dtor ASM):
 *   0x00  CProcess primary base - destroyed via ~CProcess (offset 0) in dtor
 *   0x3C  IUIWindow/Move region - content not recovered; byte padding
 *   0x6C  IWorkEvent vtable slot (bare member; never destroyed in this TU)
 *   0x70  IScnRender vtable slot - the render-callback subobject address passed
 *         to CScn::addRenderCB/removeRenderCB
 *   0x74  CScn* owning scene (loaded at Term's removeRenderCB / Init's addRenderCB)
 *   0x78  CSysWin storage (0x3A bytes). Not a C++ member (retail controls its
 *         ctor/dtor via C-linkage __ct__CSysWin / __dt__7CSysWinFv), so it is a
 *         bare byte buffer here to avoid duplicate destruction in ~CSystemWindow.
 *   0xB5  byte passed as the arg to __ct__CSysWin during Init
 *   0x2B6 flag = (lbl_eu_80663E24 >> 1) & 1 (drives cf load-flag management)
 *   sizeof = 0x2B7
 *
 * CSystemWindow is deliberately NON-polymorphic (a plain struct, like the repo's
 * CMenuQstCnt idiom): the retail object lays the CProcess base out explicitly at
 * +0x00 and drives the CSysWin + CProcess destruction by hand in ~CSystemWindow,
 * so a C++ CProcess base would make MWCC emit an extra vptr store / duplicate
 * subobject destruction that the retail dtor does not have.
 */
struct CSystemWindow {
    u8    mProcess[0x3C];    // 0x00 CProcess storage (vtable at +0x10)
    // 0x3C IUIWindow/Move region (0x30 bytes). Two null ptmfs then control words.
    u32   ptmf0[3];          // 0x3C null pointer-to-member-function
    u32   ptmf1[3];          // 0x48 null pointer-to-member-function
    u32   field_54;          // 0x54
    u32   field_58;          // 0x58
    u32   field_5C;          // 0x5C
    u32   field_60;          // 0x60 (init to -1)
    u8    field_64;          // 0x64 (Move state 3 -> 1)
    u8    field_65;          // 0x65
    u8    field_66;          // 0x66
    u8    field_67;          // 0x67 (Move state 3 -> 0)
    u32   field_68;          // 0x68
    u32   mWorkEvent;        // 0x6C IWorkEvent vtable slot
    u32   mScnRender;        // 0x70 IScnRender vtable slot
    CScn* mScene;            // 0x74 owning scene
    u8    mSysWin[0x3A];     // 0x78 CSysWin storage
    u8    _B2[0xB4 - 0xB2];  // 0xB2
    u8    mState;            // 0xB4 (Move state machine)
    u8    field_B5;          // 0xB5
    char  mStr1[0x100];      // 0xB6
    char  mStr2[0x100];      // 0x1B6
    u8    mFlag2B6;          // 0x2B6

    ~CSystemWindow();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();
};

// Render-callback APIs on the owning scene (OOL in CScn, retail symbols
// addRenderCB__4CScnFP10IScnRenderUlUl / removeRenderCB__4CScnFP10IScnRender).
class CScn {
public:
    void addRenderCB(IScnRender* cb, u32 prio, u32 flag);
    void removeRenderCB(IScnRender* cb);
};

// Minimal CTaskGame decl (retail global-namespace class, symbols
// getInstance__9CTaskGameFv / func_800426F0__9CTaskGameFv).
class CTaskGame {
public:
    static CTaskGame* getInstance();
    static bool func_800426F0();
};

// C-ABI function imports / definitions: these retail symbols are unmangled
// (C-linkage), so they must be declared `extern "C"` to stop MWCC appending a
// __F<argtypes> suffix to the emitted reloc names.
extern "C" {
void func_8022B7F4(void* syswin);
void func_8022B7C8(void* syswin, nw4r::lyt::DrawInfo* drawInfo);
void func_80137250(nw4r::lyt::DrawInfo* drawInfo);
int func_8013BE50();
void func_8008294C__Q22cf13CfGameManagerFv(bool enable);
void __ct__CSysWin(void* syswin, int arg);
void __dt__7CSysWinFv(void* syswin, int flags);
void __ct__8CProcessFv(CProcess* self);

// CSysWin / system-window helper imports (C-linkage retail symbols).
u32 CSysWin_isReady(void* syswin);
int CSysWin_isActive(void* syswin);
void func_8022B9B4(void* syswin, void* str1, void* str2);
void func_8022BFC8(void* syswin, int kind);
void func_8022B8B8(void* syswin);
void func_8022B8E4(void* syswin);
void func_8022B748(void* syswin);
int func_80086F9C__Q22cf13CfGameManagerFv(int arg);
void func_80138078__FUl(u32 op);
}

// Global data imports (MWCC does not mangle global-scope data names).
extern CSystemWindow* lbl_eu_80663FD8;   // singleton instance pointer (.sbss)
extern u32 lbl_eu_80663E28;              // cbRenderBefore flag (.sbss)
extern u32 lbl_eu_80663E24;              // Init flag (.sbss)