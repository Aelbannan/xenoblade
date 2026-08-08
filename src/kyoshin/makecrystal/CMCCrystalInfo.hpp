#pragma once

#include <types.h>
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/work/IWorkEvent.hpp"

// vtable at lbl_eu_80535CF8
extern "C" void* lbl_eu_80535CF8[];

// 4 packed shorts returned in r3:r4 by func_801397AC.
struct FourShorts { s16 a, b, c, d; };

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
    /* 0x51 */ u8 mField51;                // Cleared when state transitions from 0→1
};

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------
extern "C" void func_8021B52C(CMCCrystalInfo* self);
extern "C" void func_8021B5B4();
extern "C" void func_8021B63C();
extern "C" void func_8021B6C4();
extern "C" u32 getHandleMEM2__Q23mtl10MemManagerFv();
extern "C" void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
extern "C" char lbl_eu_80508DF8[];
extern "C" u32 func_801355BC();
extern "C" FourShorts func_801397AC(void*, u32);
extern "C" void CopyVec4s(short* dst, const short* src);
extern "C" void* lbl_eu_806646D8;
extern "C" void* lbl_eu_806646E0;
extern "C" void* lbl_eu_806646E8;
extern "C" void* lbl_eu_806646F0;
extern "C" void* lbl_eu_806646F8;
extern "C" void* lbl_eu_80664700;
extern "C" void* lbl_eu_80664708;
extern "C" void* lbl_eu_80664710;
extern "C" void __ct__17UnkClass_8045F564Fv(UnkClass_8045F564*);
extern "C" void __dt__17UnkClass_8045F564Fv(void*, int);
extern "C" void __dl__FPv(void*);
