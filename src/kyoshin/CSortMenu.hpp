#pragma once

#include <types.h>
#include <nw4r/lyt.h>

class CFileHandle;
class CEventFile;
class CScrollBar;

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
    u8 _2C[0x40];               // +0x2C: CScrollBar
    s32 mArray[32];             // +0x6C: 32 entries
    u8 mCount;                  // +0xEC
    u8 mPage;                   // +0xED
    u8 mSubPage;                // +0xEE
};