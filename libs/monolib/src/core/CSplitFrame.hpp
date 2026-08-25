#pragma once

#include <types.h>
#include "monolib/work/CWorkThreadSystem.hpp" // WORK_ID

class CView;

namespace ml {
struct CRect16;
} // namespace ml

// CSplitFrame: manages a two-pane split-screen layout for CView children.
//
// Layout offsets (size: 0x18):
//   0x00  void*    mVtable      - vtable pointer
//   0x04  CView*   mParent      - owning parent view
//   0x08  u8       mVertical    - nonzero = vertical split (top/bottom)
//   0x09  u8       mPadding9    - alignment padding
//   0x0A  s16      mSplitX      - horizontal split position (pixels from left)
//   0x0C  s16      mSplitY      - vertical split position (pixels from top)
//   0x0E  u16      mPaddingE    - alignment padding
//   0x10  WORK_ID  mView1       - first child view work ID
//   0x14  WORK_ID  mView2       - second child view work ID
struct CSplitFrame {
    void* mVtable; // 0x0
    CView* mParent; // 0x4
    u8 mVertical; // 0x8  nonzero = vertical split
    u8 mPadding9;
    s16 mSplitX; // 0xa
    s16 mSplitY; // 0xc
    u16 mPaddingE;
    WORK_ID mView1; // 0x10
    WORK_ID mView2; // 0x14

    // Compute the scissor rectangle for the first pane.
    static void getScissorRect1(ml::CRect16& out, const CSplitFrame* self);

    // Compute the scissor rectangle for the second pane (beyond the border).
    static void getScissorRect2(ml::CRect16& out, const CSplitFrame* self);
};
