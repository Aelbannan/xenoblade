#pragma once

#include <types.h>
#include <nw4r/lyt.h>

class CFileHandle;
class CEventFile;
class CScrollBar;

// Mirror of the CScrollBar layout (0x40 bytes) for raw field access in
// CSortMenu; the real CScrollBar.hpp cannot be included here (its extern
// ctor stub conflicts with the member ctor from UnkClass_8045F564.hpp).
struct CScrollBarData {
    void* mVtbl;         // +0x00
    u32 mMemRegion[4];   // +0x04 (UnkClass_8045F564)
    void* mFileHandle;   // +0x14
    void* mAccessor;     // +0x18
    void* mLayout;       // +0x1C
    void* mAnimTrans;    // +0x20
    u8 mReady;           // +0x24
    u8 mVisible;         // +0x25
    u8 mState;           // +0x26
    u8 mActive;          // +0x27
    f32 mAnimOffset;     // +0x28
    f32 mScrollPosY;     // +0x2C
    f32 mScrollRatio;    // +0x30
    f32 mThumbHeight;    // +0x34
    f32 mContentHeight;  // +0x38
    u8 mDirection;       // +0x3C
};

// Mirror of nw4r::lyt::Pane's size fields (mSize @ +0x4C: width, height).
// nw4r hides these members (protected); used for raw field access in matching.
struct PaneSizeRegion {
    f32 width;   // +0x4C
    f32 height;  // +0x50
};

class CSortMenu {
public:
    CSortMenu();
    virtual ~CSortMenu();
    void OnFileEvent(CEventFile* event);
    u8 func_801D3320();
    u8 func_801D3328();
    void func_801D3518(int value);
    u8 func_801D37F4();
    u8 func_801D3808();
    u8 func_801D3810();

    // Layout (from retail ASM):
    // +0x00: vtable (IWorkEvent / CSortMenu shared)
    // +0x04: UnkClass_8045F564 sub-object (0x10 bytes)
    // +0x14: CFileHandle* mFileHandle
    // +0x18: ArcResourceAccessor* mArcResAcc
    // +0x1C: Layout* mpLayout
    // +0x20: AnimTransform* mpAnimTrans0
    // +0x24: AnimTransform* mpAnimTrans1
    // +0x28: u8 field_0x28 (active flag)
    // +0x29: u8 field_0x29 (layout loaded flag)
    // +0x2A: u8 field_0x2A (state)
    // +0x2B: u8 field_0x2B (button flag)
    // +0x2C: CScrollBar mScrollBar (0x40 bytes)
    // +0x6C: s32 mArray[32] (0x80 bytes)
    // +0xEC: u8 mCount
    // +0xED: u8 mPage
    // +0xEE: u8 mSubPage

    u8 _04[0x10];               // +0x04: UnkClass_8045F564
    CFileHandle* mFileHandle;   // +0x14
    nw4r::lyt::ArcResourceAccessor* mArcResAcc; // +0x18
    nw4r::lyt::Layout* mpLayout;                // +0x1C
    nw4r::lyt::AnimTransform* mpAnimTrans0;     // +0x20
    nw4r::lyt::AnimTransform* mpAnimTrans1;     // +0x24
    u8 field_0x28;              // +0x28
    u8 field_0x29;              // +0x29
    u8 field_0x2A;              // +0x2A
    u8 field_0x2B;              // +0x2B
    CScrollBarData mScrollBar;  // +0x2C: CScrollBar
    s32 mArray[32];             // +0x6C: 32 entries
    u8 mCount;                  // +0xEC
    u8 mPage;                   // +0xED
    u8 mSubPage;                // +0xEE
};


// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void* lbl_eu_805349D0[];  // CSortMenu vtable
extern "C" void* lbl_eu_805349B8[];  // String table for sort menu pane names

extern "C" void __dl__FPv(void*);
extern "C" void __ct__17UnkClass_8045F564Fv(void*);
extern "C" void __dt__17UnkClass_8045F564Fv(void*, int);
extern "C" void func_8045F778__17UnkClass_8045F564Fv(void*);
extern "C" void func_8045F810__17UnkClass_8045F564Fv(void*);
extern "C" void __ct__14Class_8045F858FP17UnkClass_8045F564(void*, void*);
extern "C" void __dt__14Class_8045F858Fv(void*, int);
extern "C" void createRegion__17UnkClass_8045F564FiiPCci(void*, int, int, const char*, int);

extern "C" void* getHandleMEM2__Q23mtl10MemManagerFv();
extern "C" void func_80434A4C__Q23mtl10MemManagerFb(bool);
extern "C" nw4r::lyt::ArcResourceAccessor* createArcResourceAccessor__10CLibLayoutFv();
extern "C" void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(unsigned long, const char*, void*, int, int);
extern "C" void setHandleFlag1__11CDeviceFileFP11CFileHandle(void*);
extern "C" void* func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32, nw4r::lyt::Layout*);
extern "C" u32 func_8015780C(int);

// CScrollBar functions
extern "C" void __ct__CScrollBar(void*, int);
extern "C" void __dt__10CScrollBarFv(void*, int);
extern "C" void func_801F34F4(void*);
extern "C" void func_801F3540(void*);
extern "C" void func_801F35B0(void*, void*);
extern "C" void func_801F35DC(void*);
extern "C" u8 CScrollBar_isVisible(void*);
extern "C" void func_801F3670(void*, void*);
extern "C" void func_801F36BC(void*, int, int);
extern "C" void func_801F367C(void*);
extern "C" void func_801F369C(void*);
extern "C" void func_801F3850(void*, int);
extern "C" void func_80137924(void*, void*, void*, void*);