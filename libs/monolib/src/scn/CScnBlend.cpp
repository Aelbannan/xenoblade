// CScnBlend full-screen color-blend filter effect.
// Inherits CScnFilter (scene filter base) and adds a blend-rect region plus
// GX blend/colour settings. Decompiled for monolib/src/scn/CScnBlend.

#include <types.h>
#include <monolib/scn/CScnFilter.hpp>

// --- Cross-TU imports (declared here; .cpp-only TU, CScnFilter.hpp is read-only) ---
extern "C" void __ct__CScnFilter(CScnFilter* self);
extern void* __dl__FPv(void* p);
extern const f32 lbl_eu_8066AAE0;
extern const f32 lbl_eu_8066AAE4;
extern const f32 lbl_eu_8066AAE8;
extern char lbl_eu_8056E9E8[];   // CScnBlend vtable

// Region + tint as one anonymous aggregate (MWCC struct-copies via lwz/stw).
struct RectRegion {
    struct {
        f32 x;      // 0x38
        f32 y;      // 0x3C
        f32 w;      // 0x40
        f32 h;      // 0x44
        f32 tR;     // 0x48
        f32 tG;     // 0x4C
        f32 tB;     // 0x50
    };
};

struct CScnBlend : public CScnFilter {
    virtual ~CScnBlend();

    u8 pad_18[0x38 - 0x18];
    RectRegion mRect;   // 0x38
    u8 mFlags;      // 0x54
    u8 pad_55[3];
    u32 field_0x58;
    u32 field_0x5C;
    u32 field_0x60;
    u32 field_0x64;
    u32 field_0x68;
    u32 field_0x6C;
};

CScnBlend::~CScnBlend() {}

extern "C" void __ct__CScnBlend(CScnBlend* self) {
    __ct__CScnFilter(self);

    *((u32*)self) = (u32)(const void*)&lbl_eu_8056E9E8[0];

    RectRegion tmp;
    tmp.x = lbl_eu_8066AAE0;
    tmp.y = lbl_eu_8066AAE0;
    tmp.w = lbl_eu_8066AAE0;
    tmp.h = lbl_eu_8066AAE4;
    tmp.tR = lbl_eu_8066AAE8;
    tmp.tG = lbl_eu_8066AAE8;
    tmp.tB = lbl_eu_8066AAE8;
    self->mRect = tmp;

    self->mFlags = 3;
    self->field_0x58 = 4;
    self->field_0x5C = 1;
    self->field_0x60 = 4;
    self->field_0x64 = 1;
    self->field_0x68 = 0;
}

struct CScnBlendState {
    u8 _00[0x54];
    u8 flags;
};

void func_80498D98(void* r3, int r4) {
    CScnBlendState* state = (CScnBlendState*)r3;
    if (r4 != 0) {
        state->flags |= 1;
    } else {
        state->flags &= 0xFE;
    }
}

extern "C" void func_80498DC0(u8* self, u32 enable) {
    if (enable != 0) {
        *(u8*)((u8*)self + 0x54) |= 2;
    } else {
        *(u8*)((u8*)self + 0x54) &= ~2;
    }
}

void func_80498DE8(){}