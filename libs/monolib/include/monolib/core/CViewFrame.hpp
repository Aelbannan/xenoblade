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
    u32 unk38; // 0x38
    float unk3C; // 0x3C
    float unk40; // 0x40
    float unk44; // 0x44
    float unk48; // 0x48
    float unk4C; // 0x4C
    s16 unk50; // 0x50
    s16 unk52; // 0x52
    s16 unk54; // 0x54 position / client origin x
    s16 unk56; // 0x56 position / client origin y
    s16 unk58; // 0x58 border thickness
    s16 unk5A; // 0x5A
};

extern void getFrame2ViewOffset(ml::CRect16& rect, CViewFrame* r4);
