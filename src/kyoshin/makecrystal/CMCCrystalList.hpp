#pragma once

#include <types.h>
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/work/IWorkEvent.hpp"

class CFileHandle;

namespace nw4r { namespace lyt {
class DrawInfo;
class Layout;
class AnimTransform;
class AnimResource;
class ArcResourceAccessor;
} }

// vtable at lbl_eu_80535F80
extern "C" void* lbl_eu_80535F80[];

// Anim-completion sentinel float passed to func_80137444/func_80137510
// (retail .sdata2 constants at 0x80668544 / 0x80668548).
extern const float lbl_eu_80668544;
extern const float lbl_eu_80668548;

// Layout text/pane bind helper (retail 0x80137924).
void func_80137924(void* out, void* paneA, void* paneB, void* paneC);

// Pane texture/anim bind helpers used by func_80223004.
extern "C" void func_8013BCD4(void* pane, void* animRes);
extern "C" void func_80137F88(void* pane, void* tex);

// Crystal-list UI strings (retail .rodata at 0x805092C0, size 0x288).
extern char lbl_eu_805092C0[];

// View into the object returned by CDeviceFont::getFontInfo: vtable+0x24
// (declared index 7, no args) yields the u32 passed to func_8013676C.
// All-pure so no vtable is emitted; a genuine virtual call makes MWCC emit
// the retail lwz r12 dispatch sequence.
class __declspec(novtable) CMCCrystalListFontView {
public:
    virtual void vf0() = 0; // index 0 -> +0x08
    virtual void vf1() = 0; // index 1 -> +0x0C
    virtual void vf2() = 0; // index 2 -> +0x10
    virtual void vf3() = 0; // index 3 -> +0x14
    virtual void vf4() = 0; // index 4 -> +0x18
    virtual void vf5() = 0; // index 5 -> +0x1C
    virtual void vf6() = 0; // index 6 -> +0x20
    virtual u32 vf7() = 0;  // index 7 -> +0x24
};

// Imports whose retail symbols are unmangled (plain global C++ would be
// mangled by MWCC, e.g. func_801355BC__Fv) - C linkage keeps the call relocs
// bound to the retail names.
extern "C" u32 func_801355BC();
extern "C" void func_80136FA0(nw4r::lyt::Layout*, nw4r::lyt::AnimResource**, nw4r::lyt::ArcResourceAccessor*, char*);

/* Crystal list for the makecrystal UI. Manages available crystals and their
   display state. Inherits from IWorkEvent for file-load callbacks.
   novtable: the ctor stores the retail vtable label explicitly. */
class __declspec(novtable) CMCCrystalList : public IWorkEvent {
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
    /* 0x48 */ nw4r::lyt::AnimResource* mAnimRes5; // root-pane animation resource (SetAnimationEnable(AnimResource&))
    /* 0x4C */ u8 mState;
    /* 0x4D */ u8 mStateIdx;
    /* 0x4E */ u8 mSlotStates[8];
    /* 0x56 */ u8 mDataFlag;
    /* 0x57 */ u8 mIsActive;
};
