#pragma once

#include <types.h>
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/work/IWorkEvent.hpp"

class CFileHandle;

namespace nw4r { namespace lyt {
class DrawInfo;
class Layout;
class AnimTransform;
class ArcResourceAccessor;
} }

// vtable at lbl_eu_80535F80
extern "C" void* lbl_eu_80535F80[];

/* Crystal list for the makecrystal UI. Manages available crystals and their
   display state. Inherits from IWorkEvent for file-load callbacks. */
class CMCCrystalList : public IWorkEvent {
public:
    CMCCrystalList();
    virtual ~CMCCrystalList();
    virtual bool OnFileEvent(CEventFile* pEventFile);

    u8 func_80222A50();
    u8 func_80222A58();
    void func_80222964(nw4r::lyt::DrawInfo* drawInfo);

    /* 0x00 */ // IWorkEvent base (vptr)
    /* 0x04 */ UnkClass_8045F564 mMemRegion1;
    /* 0x14 */ UnkClass_8045F564 mMemRegion2;
    /* 0x24 */ CFileHandle* mFileHandle1;
    /* 0x28 */ CFileHandle* mFileHandle2;
    /* 0x2C */ nw4r::lyt::ArcResourceAccessor* mArcResAccessor;
    /* 0x30 */ nw4r::lyt::ArcResourceAccessor* mArcResAccessor2;
    /* 0x34 */ nw4r::lyt::Layout* mLayout;
    /* 0x38 */ nw4r::lyt::AnimTransform* mAnimTrans1;
    /* 0x3C */ nw4r::lyt::AnimTransform* mAnimTrans2;
    /* 0x40 */ nw4r::lyt::AnimTransform* mAnimTrans3;
    /* 0x44 */ nw4r::lyt::AnimTransform* mAnimTrans4;
    /* 0x48 */ nw4r::lyt::AnimTransform* mAnimTrans5;
    /* 0x4C */ u8 mState;
    /* 0x4D */ u8 mStateIdx;
    /* 0x4E */ u8 mSlotStates[8];
    /* 0x56 */ u8 mDataFlag;
    /* 0x57 */ u8 mIsActive;
};
