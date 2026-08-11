#pragma once

#include <types.h>
#include "monolib/work/CTTask.hpp"
#include "kyoshin/CSysWin.hpp"

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void cbRenderBefore__12CUIErrMesWinFv();
extern "C" void __dt__12CUIErrMesWinFv();

// CSysWin helper imports (retail C-linkage; same declarations as CSaveLoad.hpp).
extern "C" u32 CSysWin_isReady(void*);
extern "C" int CSysWin_isActive(void*);

// +0x60 sub-object advance helper (retail C-linkage, defined in CTitle.cpp).
extern "C" void func_802B744C(void*);

// CSysWin advance helper (retail C-linkage; same declaration as CSystemWindow.hpp).
extern "C" void func_8022B748(void*);

// CSysWin release helper (retail C-linkage; same declaration as CSystemWindow.hpp).
extern "C" void func_8022B7F4(void*);

// CMenuTitle +0x60 sub-object advance/release helpers (retail C-linkage, CTitle.cpp).
extern "C" void func_802B73D4(void*);
extern "C" void func_802B74F4(void*);

// cf::CfGameManager helper. Retail names it with the no-arg Fv suffix, but the
// call site passes three zero args; declare the retail symbol verbatim under
// C linkage so calls bind to it (same pattern as func_80086F9C in CSysWin.hpp).
extern "C" void func_80081E90__Q22cf13CfGameManagerFv(int, int, int);

// Global window-active flags cleared by the Term functions (.sbss words).
extern u32 lbl_eu_80664C28;
extern u32 lbl_eu_80664C30;

// Sub-object embedded at CErrMesEntry +0x09, managed by the func_802B5148 /
// func_802B515C / func_802B58A4 family. Its first byte is an active flag;
// the rest of the layout is opaque to this TU.
struct CErrMesSub {
    u8 field_0;  // +0x00 - active flag
};

// Error-message record (func_802B48A0 / func_802B48B8 family).
struct CErrMesEntry {
    /* 0x00 */ u32 field_0;
    /* 0x04 */ u32 field_4;
    /* 0x08 */ u8 field_8;
    /* 0x09 */ CErrMesSub sub;  // embedded sub-object
};

// State-dispatch ptmf tables (retail .data). Indexed by the window state byte
// (field_0x9C / field_0xE8); each entry is a 12-byte ptmf.
class CUIErrMesWin;
class CMenuTitle;
extern void (CUIErrMesWin::*const lbl_eu_8053AEB0[])();
extern void (CMenuTitle::*const lbl_eu_8053B040[])();

// Minimal CScn declaration (same idiom as CTaskGameEff.hpp): only the
// render-callback members used by Init/Term. Guarded so the kyoshin headers
// can be included together in one TU. Member calls emit the retail mangled
// symbols addRenderCB__4CScnFP10IScnRenderUlUl / removeRenderCB__4CScnFP10IScnRender.
#ifndef KYOSHIN_MINIMAL_CSCN_DECLARED
#define KYOSHIN_MINIMAL_CSCN_DECLARED
class IScnRender;
class CScn {
public:
    void addRenderCB(IScnRender* cb, u32 prio, u32 flag);
    void removeRenderCB(IScnRender* cb);
};
#endif

// CProcess/CTask base (0x00-0x53): CDoubleListNode, vtable at +0x10, Move/Draw
// ptmf hooks at +0x3C/+0x48. The retail object is CTTask<CUIErrMesWin>-derived
// (retail emits Move__22CTTask<12CUIErrMesWin>Fv / Draw over the +0x3C/+0x48
// hooks). The render-callback subobject sits at +0x58 (IScnRender vtable slot)
// with the owning scene pointer at +0x5C (same arrangement as CSystemWindow).
class CUIErrMesWin : public CTTask<CUIErrMesWin> {
public:
    virtual ~CUIErrMesWin();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    /* 0x54 */ u8 field_0x54[0x04];  // +0x54..+0x58 - unknown region
    /* 0x58 */ u32 mScnRender;       // +0x58 - IScnRender vtable slot (render-callback subobject)
    /* 0x5C */ CScn* mScene;         // +0x5C - owning scene
    /* 0x60 */ CSysWin mSysWin;      // system message window
    /* 0x9C */ u8 field_0x9C;        // window state byte (2=ready, 4=active)
};

class CMenuTitle {
public:
    virtual ~CMenuTitle();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    /* 0x04 */ u8 field_0x04[0x54];  // +0x04..+0x58 - unknown region
    /* 0x58 */ u32 mScnRender;       // +0x58 - IScnRender vtable slot (render-callback subobject)
    /* 0x5C */ CScn* mScene;         // +0x5C - owning scene
    /* 0x60 */ u8 field_0x60[0x88];  // +0x60..+0xE8 - opaque region (func_802B744C target)
    /* 0xE8 */ u8 field_0xE8;        // state byte (ptmf table index)
};
