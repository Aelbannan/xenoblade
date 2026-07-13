#pragma once

#include <types.h>
#include "monolib/math.hpp"

// CViewRectDataCore: viewport rectangle state at CView::unk1C8 (size 0x14).
class CViewRectDataCore {
public:
    CViewRectDataCore* func_80459270();
    void func_804592F0(const ml::CPnt16& size);
    void func_80459384(const ml::CPnt16& maxSize);

    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
};
