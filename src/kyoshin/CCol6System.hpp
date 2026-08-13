#pragma once

#include <types.h>
#include "monolib/work/CProcess.hpp"
#include "kyoshin/CCol6CheckBat.hpp"
#include "kyoshin/CCol6Invite.hpp"
#include "kyoshin/CScrollBar.hpp"

#include "monolib/device/CDeviceVI.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "monolib/util/MemManager.hpp"
#include "kyoshin/code_80135FDC.hpp"

#include <nw4r/lyt.h>
#include <revolution/GX.h>

// Minimal CScn declaration (same guarded form as CTaskGameEff.hpp/CTaskGameEvt
// /CTaskGamePic): only the render-callback members used by CCol6System's
// Init/Term are needed. Declared with the real class tag so member calls emit
// the retail mangled symbols addRenderCB__4CScnFP10IScnRenderUlUl /
// removeRenderCB__4CScnFP10IScnRender.
#ifndef KYOSHIN_MINIMAL_CSCN_DECLARED
#define KYOSHIN_MINIMAL_CSCN_DECLARED
class CScn {
public:
    void addRenderCB(IScnRender* cb, u32 prio, u32 flag);
    void removeRenderCB(IScnRender* cb);
};
#endif

class CCol6Hint : public CProcess {
public:
    CCol6Hint();
    virtual ~CCol6Hint();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    /* 0x3C */ u8 mCallbackA[12];          // pointer-to-member-function null (3 words)
    /* 0x48 */ u8 mCallbackB[12];          // pointer-to-member-function null (3 words)
    /* 0x54 */ u32 mField54;               // init 0
    /* 0x58 */ u32 mField58;               // init 0
    /* 0x5C */ u32 mField5C;               // init 0
    /* 0x60 */ s32 mIndex;                 // init -1
    /* 0x64 */ u8 mFlag64;                 // byte flag, init 0
    /* 0x65 */ u8 mFlag65;                 // byte flag, init 0
    /* 0x66 */ u8 mFlag66;                 // byte flag, init 0
    /* 0x67 */ u8 mActive;                 // byte flag, init 1
    /* 0x68 */ u32 mField68;               // init 0
    /* 0x6C */ u32 mField6C;               // vtable-like ptr
    /* 0x70 */ u32 mField70;               // vtable-like ptr
    /* 0x74 */ u8 mMemRegion[0x10];        // UnkClass_8045F564
    /* 0x84 */ CProcess* mParentPtr;       // stored parent pointer
    /* 0x88 */ u32 mField88;               // CFileHandle*
    /* 0x8C */ u32 mField8C;               // ArcResourceAccessor*
    /* 0x90 */ u32 mField90;               // AnimTransform*
    /* 0x94 */ u32 mField94;               // AnimTransform*
    /* 0x98 */ u8 mFlag98;                 // byte flag, init 0
    /* 0x99 */ u8 mState;                  // state machine (0-5), init 0
    /* 0x9C */ u8 mUnk9C[0x80];            // 
    /* 0x11C */ u32 mField11C;             // init 0
    /* 0x120 */ u8 mField120;              // init 0
    /* 0x121 */ u8 mField121;              // init 0
    /* 0x122 */ u8 mPad122[2];             // retail pads to 0x124
    /* 0x124 */ u8 mCur18[0x18];           // CCur18
    /* 0x13C */ CScrollBar mScrollBar;
};

// Abstract view into the CCur18 cursor vtable embedded in CCol6System
// (MWCC reserves slots 0/1, so the first user virtuals land at vtable +0x08
// and +0x0C; slot 3 is the per-frame update invoked by Term).
class CCol6CursorView {
public:
    virtual void vf2(int) = 0;  // slot 2 (vtable + 0x08)
    virtual void vf3() = 0;     // slot 3 (vtable + 0x0C)
};

class CCol6System : public CProcess {
public:
    CCol6System();
    virtual ~CCol6System();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    /* 0x3C */ u8 mCallbackA[12];               // pointer-to-member-function null (3 words)
    /* 0x48 */ u8 mCallbackB[12];               // pointer-to-member-function null (3 words)
    /* 0x54 */ nw4r::lyt::Layout* mpLayout;     // layout (released in Term)
    /* 0x58 */ u32 mField58;                    // init 0
    /* 0x5C */ u32 mField5C;                    // init 0
    /* 0x60 */ s32 mIndex;                      // init -1
    /* 0x64 */ u8 mFlag64;                      // byte flag, init 0
    /* 0x65 */ u8 mFlag65;                      // byte flag, init 0
    /* 0x66 */ u8 mFlag66;                      // byte flag, init 0
    /* 0x67 */ u8 mFlag67;                      // byte flag, init 1
    /* 0x68 */ u32 mField68;                    // init 0
    /* 0x6C */ u32 mField6C;                    // vtable-like ptr
    /* 0x70 */ u8 mScnRender[4];                // IScnRender vptr slot (this-adjust target)
    /* 0x74 */ u8 mMemRegion[0x10];             // UnkClass_8045F564 scratch region
    /* 0x84 */ CScn* mScn;                      // owning scene (render-CB receiver)
    /* 0x88 */ CFileHandle* mFileHandle;        // loaded arc file handle
    /* 0x8C */ nw4r::lyt::ArcResourceAccessor* mArcAccessor; // arc accessor (Term release)
    /* 0x90 */ u32 mField90;
    /* 0x94 */ u32 mField94;
    /* 0x98 */ u8 mFlag98;                      // layout-1 ready flag (func_8015DB08)
    /* 0x99 */ u8 mState;                       // state byte
    /* 0x9A */ u8 mField9A;
    /* 0x9B */ u8 mField9B;
    /* 0x9C */ u32 mField9C;
    /* 0xA0 */ u8 mFlagA0;                      // layout-2 ready flag (cleared in Term)
    /* 0xA1 */ u8 mFlagA1;                      // draw-gate flag (cbRenderBefore)
    /* 0xA2 */ u8 mFieldA2;
    /* 0xA3 */ u8 mFieldA3;
    /* 0xA4 */ u32 mFieldA4;
    /* 0xA8 */ u8 mCur1[0x18];                  // CCur18 cursor 1
    /* 0xC0 */ u8 mCur2[0x18];                  // CCur18 cursor 2
    /* 0xD8 */ u8 mSysWin1[0x3C];               // CSysWin window 1
    /* 0x114 */ u8 mSysWin2[0x3C];              // CSysWin window 2
};

// C-linkage imports (retail symbols are unmangled - keep linkage verbatim)
extern "C" {
u8 CSysWin_isReady(void* syswin);                                    // CSysWin.cpp
void func_8022B7C8(void* syswin, nw4r::lyt::DrawInfo* drawInfo);     // CSysWin.cpp
void func_8022B7F4(void* syswin);                                    // CSysWin.cpp
void func_801D20B0(void* cursor, nw4r::lyt::DrawInfo* drawInfo);     // CCur.cpp
void func_8013D55C(char* msg, int a, int b);                         // CUICfManager.cpp
void func_8009D018(u32 destination, u32 value);                      // CfGameManager.cpp
}

// Split1 .rodata string pool (arc paths / layout region names / format strings).
extern char lbl_eu_80502050[];

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" const u32 lbl_eu_80667540;
extern "C" const u8 lbl_eu_80667544;
extern "C" u32 func_8009CF8C(u32 resourceId);
extern "C" void __dt__7CSysWinFv(void*, int);
extern "C" void __dt__6CCur18Fv(void*, int);
extern "C" void __dt__17UnkClass_8045F564Fv(void*, int);
