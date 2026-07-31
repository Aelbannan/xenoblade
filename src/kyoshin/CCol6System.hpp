#pragma once

#include <types.h>
#include "monolib/work/CProcess.hpp"
#include "kyoshin/CCol6CheckBat.hpp"
#include "kyoshin/CCol6Invite.hpp"
#include "kyoshin/CScrollBar.hpp"

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
    /* 0x124 */ u8 mCur18[0x18];           // CCur18
    /* 0x13C */ CScrollBar mScrollBar;
};

class CCol6System : public CProcess {
public:
    CCol6System();
    virtual ~CCol6System();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    /* 0x3C */ u8 _pad3C[0x74 - 0x3C];
    /* 0x74 */ u8 mMemRegion[0x10];       // UnkClass_8045F564
    /* 0x84 */ u8 _pad84[0xA8 - 0x84];
    /* 0xA8 */ u8 mCur1[0x18];            // CCur18
    /* 0xC0 */ u8 mCur2[0x18];            // CCur18
    /* 0xD8 */ u8 mSysWin1[0x3C];         // CSysWin
    /* 0x114 */ u8 mSysWin2[0x3C];        // CSysWin
};
