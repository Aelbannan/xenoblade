#pragma once

#include <types.h>
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/CItemBoxGrid_Color.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/CUICfManager_Res.hpp"


// Opaque item-object handle. `void*` is banned by TU lint; a typedef of
// void keeps MWCC's mangled form (`Pv`) and emitted bytes identical.
typedef void CMCCItemHandle;


// Item/crystal data header read by MCCrystal_ShowItem. word0 packs the item code
// (bits 20-31) and a type field (bits 12-15); byte 7 holds flags in bits 0-1.
struct CMCCItemData {
    u32 word0;   // 0x00
    u8 field04;  // 0x04
    u8 field05;  // 0x05
    u8 field06;  // 0x06
    u8 field07;  // 0x07
};

// Crystal result buffer written by MCCrystal_BuildItemBuf and consumed by
// MCCrystal_ShowSlots. Sized 0x24; the caller copies it wholesale.
struct CrystalItemBuf {
    u8 count;          // 0x00 number of matching crystal items
    u8 pad00[3];       // 0x01
    char* str;         // 0x04 description string
    char* names[4];    // 0x08 item name pointers (valid up to count)
    u32 pad18;         // 0x18
    u8 flags[4];       // 0x1c per-item flags
    u8 field20;        // 0x20
    u8 field21;        // 0x21 running count of stored entries
    u8 pad22[2];       // 0x22 (size 0x24)
};

// Item-impl virtual interface returned by CItem_initItemImplInstances (owner).
#include "kyoshin/cf/CItemImpl.hpp"

// View into the object returned by CDeviceFont::getFontInfo: vtable+0x24
// (declared index 7, no args) yields the u32 passed to ApplyFontToTxtPanes.
// All-pure so no vtable is emitted; a genuine virtual call makes MWCC emit
// the retail lwz r12 dispatch sequence.
class __declspec(novtable) CMCCrystalInfoFontView {
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

/* Crystal info for the makecrystal UI. Manages crystal information display
   state. Inherits from IWorkEvent for file-load callbacks. */
class CMCCrystalInfo : public IWorkEvent {
public:
    CMCCrystalInfo();
    virtual ~CMCCrystalInfo();
    virtual bool OnFileEvent(CEventFile* pEventFile);

    /* 0x00 */ // IWorkEvent base (vptr)
    /* 0x04 */ UnkClass_8045F564 mMemRegion1;
    /* 0x14 */ UnkClass_8045F564 mMemRegion2;
    /* 0x24 */ u32 mFileHandle1;
    /* 0x28 */ u32 mFileHandle2;
    /* 0x2C */ void* mArcResAccessor;     // nw4r::lyt::ArcResourceAccessor*
    /* 0x30 */ u32 mField30;
    /* 0x34 */ void* mLayout;              // nw4r::lyt::Layout*
    /* 0x38 */ void* mAnimTransform1;      // nw4r::lyt::AnimTransform*
    /* 0x3C */ void* mAnimTransform2;      // nw4r::lyt::AnimTransform*
    /* 0x40 */ void* mAnimTransform3;      // nw4r::lyt::AnimTransform*
    /* 0x44 */ void* mAnimTransform4;      // nw4r::lyt::AnimTransform*
    /* 0x48 */ u8 mEnabled;                // Set when layout is ready; gates proc/draw
    /* 0x4C */ s32 mState;                 // State (0=inactive, 1/2/4-8=active steps)
    /* 0x50 */ u8 mIsActive;               // Active flag; set when layout initialised
    u8 func_8021A8EC() const { return mIsActive; }
    /* 0x51 */ u8 mField51;                // Cleared when state transitions from 0->1
    /* 0x52 */ s16 mUnk52[4];              // not touched by this class's code (copied by
    /* 0x5A */ s16 mUnk5A[4];              // CMCCrystalBox's implicit assignment)
};

// Canonical declarations for the free helpers defined by CMCCrystalInfo.cpp.
// MWCC mangles these C++ names; callers must include this header rather than
// declaring flat extern "C" copies.
namespace nw4r { namespace lyt { class DrawInfo; } }

void MCCrystal_LoadLayouts(CMCCrystalInfo* self);
void MCCrystal_StepState(CMCCrystalInfo* self);
void MCCrystal_DrawLayout(CMCCrystalInfo* self, nw4r::lyt::DrawInfo* drawInfo);
void MCCrystal_Teardown(CMCCrystalInfo* self);
void MCCrystal_OpenFromClosed(CMCCrystalInfo* self);
void MCCrystal_PlayOutro(CMCCrystalInfo* self);
void MCCrystal_PlayAnim4(CMCCrystalInfo* self);
void MCCrystal_PlayAnim3A(CMCCrystalInfo* self);
void MCCrystal_PlayAnim3B(CMCCrystalInfo* self);
void MCCrystal_AlignPane(CMCCrystalInfo* self, CMCCrystalInfo* other, u32 paneIdx);

// ---------------------------------------------------------------------------
// Imports. All retail names here are unmangled, so plain global C++ (MWCC

// MWCC ctor/dtor/delete helpers below keep extern "C".
// ---------------------------------------------------------------------------
// Same-TU helpers; retail symbols are unmangled, so C linkage keeps both the
// definitions and the call relocs bound to the retail names.
extern "C" void MCCrystal_StepIntro(CMCCrystalInfo* self);
extern "C" void MCCrystal_BindIntro(CMCCrystalInfo* self);
extern "C" void MCCrystal_BindMain(CMCCrystalInfo* self);
extern "C" void MCCrystal_BindAnim3(CMCCrystalInfo* self);
extern "C" void MCCrystal_BindAnim4(CMCCrystalInfo* self);
extern "C" void MCCrystal_ClearTexts(CMCCrystalInfo* self);
extern "C" void MCCrystal_ShowSlots(CMCCrystalInfo* self, u16 arg2, CMCCItemHandle* item);

// Imports whose retail symbols are unmangled (plain global C++ would be
// mangled by MWCC, e.g. CUICfManager_getPackedFont9C__Fv) -- C linkage keeps the call relocs
// bound to the retail names.
// GXColorS10-style shared slot colours (4 s16 channels each); the alpha
// channel is refreshed from the layout panes during file load.
extern GXColorS10 sPane1Color0;
extern GXColorS10 sPane1Color1;
extern GXColorS10 sPane1Alpha0;
extern GXColorS10 sPane1Alpha1;
extern GXColorS10 sPane2Color0;
extern GXColorS10 sPane2Color1;
extern GXColorS10 sPane2Alpha0;
extern GXColorS10 sPane2Alpha1;

// Cross-TU imports with unmangled retail symbols (C linkage required so call
// relocs bind to the retail names, not MWCC-mangled C++ forms).
// CItem_initItemImplInstances: canonical decl from CfGameManager.hpp.
