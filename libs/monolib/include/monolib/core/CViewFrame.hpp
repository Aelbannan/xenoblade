#pragma once

#include <types.h>
#include "monolib/math.hpp"

class CWorkThread;
class CView;

namespace ml {
// Distinct from CRect16 in MWCC mangling (Q22ml5CRect); same 8-byte layout.
struct CRect {
    CPnt16 mPos;
    CPnt16 mSize;
};
} // namespace ml

// size: 0x5C
class CViewFrame {
public:
    bool render();
    void detachRenderWork(CWorkThread* pThread);

    void* mVtable; // 0x0
    CView* mOwner; // 0x4
    ml::CCol4 mFrameColor; // 0x8
    ml::CCol4 mColor18; // 0x18
    ml::CCol4 mColor28; // 0x28
    u32 unk38; // 0x38 — render flags / mode bits (1=border expand, 2=split)
    float unk3C; // 0x3C — possibly padding or unused alignment filler
    float unk40; // 0x40 — unused alignment padding to align mBorder siblings
    float unk44; // 0x44 — unused alignment padding
    float unk48; // 0x48 — unused alignment padding
    float unk4C; // 0x4C — unused alignment padding
    s16 unk50; // 0x50 — unused padding
    s16 unk52; // 0x52 — unused padding
    s16 mContentX; // 0x54 — client-area origin X (pixels from frame left edge to content)
    s16 mContentY; // 0x56 — client-area origin Y (pixels from frame top edge to content)
    s16 mBorder; // 0x58 — frame border thickness in pixels (used for expand/split sizing)
    s16 unk5A; // 0x5A — unused trailing padding; satisfies 0x5C sizeof
};

extern void getFrame2ViewOffset(ml::CRect16& rect, CViewFrame* r4);
