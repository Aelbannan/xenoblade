#pragma once

#include <types.h>
#include <monolib/work/CProcess.hpp>
#include <monolib/scn/IScnRender.hpp>

#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CFade.hpp"
#include "kyoshin/CFloorMap.hpp"

// kyoshin/CMapSel.hpp cannot be included here: its global-scope CCur18View
// clashes with the one in CFloorMap.hpp (same repo-wide pattern - no TU
// includes both). Minimal layout-identical stand-in (0xB4 bytes, same as the
// real CMapSel); only addresses of the sub-object are used by this TU.
#ifndef KYOSHIN_CMAPSEL_HPP_INCLUDED
#define KYOSHIN_CMAPSEL_HPP_INCLUDED
struct CFileHandle;
class CEventFile;
class CMapSel {
public:
    CMapSel();
    ~CMapSel();
    u8 mVtbl[4];                                  // +0x00
    u8 mMemRegion[0x10];                          // +0x04
    CFileHandle* mFileHandle;                     // +0x14
    CFileHandle* mFileHandle2;                    // +0x18
    nw4r::lyt::ArcResourceAccessor* mArcAccessor; // +0x1C
    void* mAllocatedMem;                          // +0x20
    nw4r::lyt::Layout* mLayout;                   // +0x24
    nw4r::lyt::AnimTransform* mAnimTransform1;    // +0x28
    nw4r::lyt::AnimTransform* mAnimTransform2;    // +0x2C
    u8 field_0x30;                                // +0x30
    u8 mState;                                    // +0x31
    u8 field_0x32;                                // +0x32
    u8 mFlag33;                                   // +0x33
    u8 mScrollBar[0x40];                          // +0x34
    u8 mCursor[0x18];                             // +0x74
    s8 mSelY;                                     // +0x8C
    s8 mSelX;                                     // +0x8D
    u8 mGridData[0x22];                           // +0x8E
    float mTimer;                                 // +0xB0
};
#endif

// World map screen UI process.
// Inherits from CProcess (task system) and IScnRender (render callback).
// IScnRender subobject is at offset 0x58, requiring this-adjusting thunks.
//
// Layout (0x34E4 bytes total):
//   0x00-0x3B: CProcess (task system base)
//   0x3C-0x53: 6 PTMF callback slots (__ptmf_null copies)
//   0x54:      u8 mField54 (flag byte)
//   0x55:      u8 mField55 (flag byte)
//   0x56-0x57: padding
//   0x58:      IScnRender (vtable ptr, non-primary base)
//   0x5C:      CProcess* mParent (parent process reference)
//   0x60:      CBgTex (background layout widget)
//   0x80:      CTitleAHelp (title/help bar)
//   0xB8:      CMapSel (map selection widget)
//   0x16C:     CFade (fade overlay)
//   0x194:     CFloorMap (floor map widget)
//   0x34DC:    u8 mState (phase/state indicator)
//   0x34E0:    f32 mTimer (animation timer)
class CMenuMapSelect : public CProcess, public IScnRender {
public:
    // Implemented in code_802405F4.cpp
    virtual ~CMenuMapSelect();
    void cbRenderBefore();

    // IScnRender vtable this-adjusting thunk for cbRenderBefore
    // implemented as a free function in the .cpp (retail symbol func_8024309C).
    void func_802430A4();

    CMenuMapSelect();
    void func_80242368();

    // Fields
    u32 mCallbacks[6];                           // 0x3C-0x53 -- PTMF callback slots (__ptmf_null)
    u8 mField54;                                 // 0x54
    u8 mField55;                                 // 0x55
    // padding at 0x56-0x57
    // IScnRender at 0x58 (vtable ptr, empty base)

    CProcess* mParent;                           // 0x5C
    CBgTex mBgTex;                               // 0x60
    CTitleAHelp mTitleAHelp;                     // 0x80
    CMapSel mMapSel;                             // 0xB8
    CFade mFade;                                 // 0x16C
    CFloorMap mFloorMap;                         // 0x194
    u8 mState;                                   // 0x34DC
    f32 mTimer;                                  // 0x34E0
};

// size 0x34E4 verified via symbols