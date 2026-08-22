// CScnBlend full-screen color-blend filter effect.
// Inherits CScnFilter (scene filter base) and adds a blend-rect region plus
// GX blend/colour settings. Decompiled for monolib/src/scn/CScnBlend.

#include <types.h>
#include <monolib/scn/CScnFilter.hpp>

// --- Cross-TU imports (declared here; .cpp-only TU, CScnFilter.hpp is read-only) ---
extern "C" void __ct__CScnFilter(CScnFilter* self);
extern "C" void __dt__10CScnFilterFv(CScnFilter* self, int flag);
// Global operator delete — MWCC mangles this to __dl__FPv (defined in MemManager.cpp).
void operator delete(void* p) throw();
extern const f32 lbl_eu_8066AAE0;
extern const f32 lbl_eu_8066AAE4;
extern const f32 lbl_eu_8066AAE8;
extern "C" void func_8049C868();   // CScnBloom (foreign TU)
extern "C" void func_80498DE8();   // defined below
extern "C" u32 lbl_eu_806639E8;    // CScnBlend RTTI locator (owned by CScnItemCamera TU)
extern "C" u32 lbl_eu_806639F0;    // CScnFilter RTTI locator (owned by CScnItemCamera TU)

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

extern "C" void __dt__9CScnBlendFv(CScnBlend* self, int flag); // defined below

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.data] 0x8056E9E8-0x8056EA08 (32B): CScnBlend vtable (20B) + RTTI chain (12B).
// The vtable is installed by __ct__CScnBlend; writing the dtor as an extern "C"
// free function keeps MWCC from auto-emitting __vt__/__RTTI__ here (the retail
// RTTI locators lbl_eu_806639E8/lbl_eu_806639F0 live in the CScnItemCamera TU).
extern "C" u32 lbl_eu_8056E9E8[5] = {
    (u32)&lbl_eu_806639E8, 0x00000000, (u32)&__dt__9CScnBlendFv,
    (u32)&func_8049C868, (u32)&func_80498DE8,
};
extern "C" u32 lbl_eu_8056E9FC[3] = {
    (u32)&lbl_eu_806639F0, 0x00000000, 0x00000000,
};

// [.rodata] 0x805240A0-0x805240AC (12B): "CScnBlend"
extern "C" const u32 lbl_eu_805240A0[3] = {
    0x4353636E, 0x426C656E, 0x64000000,
};

// extern "C" free-function dtor (CFontLayer pattern): keeps MWCC from
// auto-emitting __vt__/__RTTI__ for CScnBlend in this TU (the retail vtable
// is the dissolved .data blob above).
extern "C" void __dt__9CScnBlendFv(CScnBlend* self, int flag) {
    if (self != 0) {
        __dt__10CScnFilterFv((CScnFilter*)self, 0);
        if (flag > 0) {
            operator delete(self);
        }
    }
}

// Retail __ct__CScnBlend (flat name): calls the base ctor, installs the
// CScnBlend vtable, fills the default blend rect/tint region and the
// blend-mode fields. Returns the object.
extern "C" CScnBlend* __ct__CScnBlend(CScnBlend* self) {
    __ct__CScnFilter(self);

    *(void**)self = (void*)lbl_eu_8056E9E8;

    // First 0-store is consumed by the flags OR (store-to-load forward); the
    // second (redundant) store keeps the physical stw at the end of the block.
    self->field_0x68 = 0;

    RectRegion tmp;
    tmp.x = lbl_eu_8066AAE0;
    tmp.y = lbl_eu_8066AAE0;
    tmp.w = lbl_eu_8066AAE0;
    tmp.h = lbl_eu_8066AAE4;
    tmp.tR = lbl_eu_8066AAE8;
    tmp.tG = lbl_eu_8066AAE8;
    tmp.tB = lbl_eu_8066AAE8;
    self->mRect = tmp;

    self->mFlags = (u8)((u32)self->field_0x68 | 3);
    self->field_0x58 = 4;
    self->field_0x5C = 1;
    self->field_0x60 = 4;
    self->field_0x64 = 1;
    self->field_0x68 = 0;
    return self;
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
