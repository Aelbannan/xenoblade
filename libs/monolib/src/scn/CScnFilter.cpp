// Decompiled for monolib/src/scn/CScnFilter

#include <types.h>
#include <monolib/scn/CScnFilter.hpp>

extern "C" {
    extern u32 lbl_eu_8056EB60[];   // CScnFilter vtable

}

extern "C" void __ct__CScnFilter(CScnFilter* self) {
    *(u32**)self = lbl_eu_8056EB60;
    self->mEnabled = 0;
    self->mUnk0C = 0;
    self->mIntensity = 0.0f;
    self->mUnk14 = 0;
    self->mFlags = (u32)self->mEnabled | 3;
}

CScnFilter::~CScnFilter() {
}

// Per-frame update payload; the delta at +0x0C is added to mIntensity.
struct CScnFilterUpdateData {
    u8 pad_00[0x0C];
    f32 field_0x0C;
};

struct CScnFilterHost {
    u8 field_0x00[0x84];
    CScnFilterUpdateData* field_0x84;
};

// Callback object invoked through vtable slot 0x0C when intensity overflows.
// The slot-2 virtual takes (host, filter) - retail preserves r4=host across
// the update and stages r5=self at entry for this call.
struct CScnFilterCallback {
    virtual void unk_04();
    virtual void unk_08(CScnFilterHost* host, CScnFilter* filter);
};

static BOOL check_over(CScnFilter* self);

extern "C" void FilterUpdateIntensity(CScnFilter* self, CScnFilterHost* host) {
    // proceed only while the filter is active and not yet saturated
    if (*(volatile u32*)&self->mUnk0C == 0) {
        return;
    }
    if (check_over(self)) {
        return;
    }
    // accumulate this frame's delta into the filter intensity
    f32 delta = host->field_0x84->field_0x0C;
    self->mIntensity += delta;
    if (check_over(self)) {
        if (self->mUnk14 != 0) {
            self->mUnk14->unk_08(host, self);
        }
    }}

static BOOL check_over(CScnFilter* self) {
    // intensity limit check; the direct (float)u32 cast emits MWCC's
    // 0x43300000 2^52 stack-trick (stw/stw/lfd/lfd/fsubs) inline.
    // Explicit if/else assignment => MWCC li-diamond, no accumulator preinit
    // (MWCC_CASES us-80374610 bool-diamond rule).
    BOOL over;
    if (self->mUnk0C != 0 && self->mIntensity > (float)self->mUnk0C) {
        over = TRUE;
    } else {
        over = FALSE;
    }
    return over;
}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.data] 0x8056EB60-0x8056EB78 (24B): CScnFilter vtable. The typeinfo pair
// __RTTI__10CScnFilter is owned by a foreign TU.
extern "C" u32 __RTTI__10CScnFilter;
extern "C" void __dt__10CScnFilterFv();
extern "C" u32 lbl_eu_8056EB60[6] = {
    (u32)&__RTTI__10CScnFilter, 0x00000000,
    (u32)&__dt__10CScnFilterFv,
    (u32)&FilterUpdateIntensity,
    0x00000000, 0x00000000,
};
