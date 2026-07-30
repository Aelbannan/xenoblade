// Decompiled for monolib/src/scn/CScnFrame

#include <types.h>
#include "libs/monolib/src/scn/CScnFrame.hpp"

extern "C" {
    extern void __dl__FPv(void* ptr);
    extern u32 func_80448420__9CDeviceVIFv();
    extern const float lbl_eu_8066ABE0; // 1.0f
    extern const float lbl_eu_8066ABE4; // 0.5f
    extern const double lbl_eu_8066ABE8; // 0x4330000000000000
}

// The vtable is supplied by the retail data TU.
extern char lbl_eu_8056EC70[];

extern "C" void __ct__CScnFrame(CScnFrame* self, u32 param) {
    self->vtable = (void*)lbl_eu_8056EC70;
    self->mParam = param;
    self->mElapsed = lbl_eu_8066ABE0;
    self->mStep = lbl_eu_8066ABE0;
    self->mFlag = 0;
}

CScnFrame::~CScnFrame() {
}

void CScnFrame::update() {
    if (mFlag) {
        u32 tick = func_80448420__9CDeviceVIFv();
        f32 factor = lbl_eu_8066ABE4 * (f32)tick;
        mElapsed = mStep * factor;
    } else {
        mElapsed = mStep;
    }
}
