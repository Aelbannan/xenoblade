// Decompiled for monolib/src/scn/CScnFrame

#include <types.h>
#include "libs/monolib/src/scn/CScnFrame.hpp"

extern "C" {
    extern void __dl__FPv(void* ptr);
    extern u32 getTargetFramerate__9CDeviceVIFv();
    extern const float lbl_eu_8066ABE0; // 1.0f
    extern const float lbl_eu_8066ABE4; // 0.5f
    extern const double lbl_eu_8066ABE8; // 0x4330000000000000
}

// Foreign data label (RTTI base-list) referenced by the CScnFrame vtable.
extern "C" u32 lbl_eu_80663A70;
// Forward-decl the retail destructor mangled name (CScnFrame::~CScnFrame) so the
// vtable reloc resolves to __dt__9CScnFrameFv (separate identifier, no clash).
extern "C" void __dt__9CScnFrameFv();

// === .rodata size=0x10 align=8 ===
extern "C" __declspec(align(8)) const char lbl_eu_80524228[16] = {
    0x43,0x53,0x63,0x6E,0x46,0x72,0x61,0x6D,0x65,0x00, 0x00,0x00,0x00,0x00,0x00,0x00,
};

// === .data size=0x10 align=8 ===
// CScnFrame vtable (16B): typeinfo locator + dtor.
extern "C" u32 lbl_eu_8056EC70[4] = {
    (u32)&lbl_eu_80663A70, 0x00000000, (u32)&__dt__9CScnFrameFv, 0x00000000,
};

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
        u32 tick = getTargetFramerate__9CDeviceVIFv();
        f32 factor = lbl_eu_8066ABE4 * (f32)tick;
        mElapsed = mStep * factor;
    } else {
        mElapsed = mStep;
    }
}
