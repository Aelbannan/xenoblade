#pragma once

#include <types.h>
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/work/IWorkEvent.hpp"

// vtable at lbl_eu_80535F80
extern "C" void* lbl_eu_80535F80[];

/* Crystal list for the makecrystal UI. Manages available crystals and their
   display state. Inherits from IWorkEvent for file-load callbacks. */
class CMCCrystalList : public IWorkEvent {
public:
    CMCCrystalList();
    virtual ~CMCCrystalList();
    virtual bool OnFileEvent(CEventFile* pEventFile);

    /* 0x00 */ // IWorkEvent base (vptr)
    /* 0x04 */ UnkClass_8045F564 mMemRegion1;
    /* 0x14 */ UnkClass_8045F564 mMemRegion2;
    /* 0x24 */ u32 mFileHandle1;
    /* 0x28 */ u32 mFileHandle2;
    /* 0x2C */ u32 mField2C;
    /* 0x30 */ u32 mField30;
    /* 0x34 */ u32 mField34;
    /* 0x38 */ u32 mField38;
    /* 0x3C */ u32 mField3C;
    /* 0x40 */ u32 mField40;
    /* 0x44 */ u32 mField44;
    /* 0x48 */ u32 mField48;
    /* 0x4C */ u8 mState;
    /* 0x4D */ u8 mStateIdx;
    /* 0x4E */ u8 mSlotStates[8];
    /* 0x56 */ u8 mDataFlag;
    /* 0x57 */ u8 mIsActive;
};
