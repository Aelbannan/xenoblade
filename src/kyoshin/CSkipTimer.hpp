#pragma once

#include <types.h>
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"

/*
  Skip-timer widget backing data. Used by CMenuSkipTimer for cutscene skip UI.
  Inherits IWorkEvent for file-event dispatch; owns a memory region,
  nw4r layout/animation handles, state flags, a CSysWin panel (0x3C bytes at
  +0x34), and a nested CSkipTimer2 sub-controller (0x28 bytes at +0x70).
  Total size 0x98 bytes.
*/
class CSkipTimer : public IWorkEvent {
public:
    CSkipTimer();
    ~CSkipTimer();

    bool OnFileEvent(CEventFile* pEventFile);

    // Returns the skip-timer active flag at +0x30.
    u8 getActive() const { return mActive; }

    // --- member fields ---
    /* 0x04 */ UnkClass_8045F564 mMemRegion;   // scratch region for layout build
    /* 0x14 */ void* mParent;                   // parent handle, set at construction
    /* 0x18 */ void* mLayout;                   // nw4r::lyt::Layout*
    /* 0x1C */ void* mAnimTransform;            // nw4r::lyt::AnimTransform*
    /* 0x20 */ u32 mField20;                    // pointer/word, init 0
    /* 0x24 */ u32 mField24;                    // pointer/word, init 0
    /* 0x28 */ u8 mField28;                     // init 0
    /* 0x29 */ u8 mField29;                     // init 0
    /* 0x2A */ u8 mField2A;                     // init 0
    /* 0x2B */ u8 mField2B;                     // init 1
    /* 0x2C */ u16 mField2C;                    // init 0
    /* 0x2E */ u16 mField2E;                    // init 0
    /* 0x30 */ u8 mActive;                      // active flag; init 0, read by getActive
    /* 0x31 */ u8 pad31[3];
    /* 0x34 */ u8 mSysWinData[0x3C];            // CSysWin panel widget
    /* 0x70 */ u8 mSkipTimer2Data[0x28];        // CSkipTimer2 sub-controller
};
// Total size: 0x98 bytes (verified via CMenuSkipTimer mTimerData[0x98])
