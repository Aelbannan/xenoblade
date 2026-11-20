// Decompiled for monolib/src/scn/CScnFilter

#include <types.h>
#include <monolib/scn/CScnFilter.hpp>

CScnFilter::CScnFilter() {
    mEnabled = 0;
    mUnk0C = 0;
    mIntensity = 0.0f;
    mUnk14 = 0;
    mFlags = (u32)mEnabled | 3;
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

void CScnFilter::updateIntensity(CScnFilterHost* host) {
    // proceed only while the filter is active and not yet saturated
    if (mUnk0C == 0) {
        return;
    }
    if (check_over(this)) {
        return;
    }
    // accumulate this frame's delta into the filter intensity
    f32 delta = host->field_0x84->field_0x0C;
    mIntensity += delta;
    if (check_over(this)) {
        if (mUnk14 != 0) {
            mUnk14->unk_08(host, this);
        }
    }}

static BOOL check_over(CScnFilter* self) {
    // intensity limit check; the direct (float)u32 cast emits MWCC's
    // 0x43300000 2^52 stack-trick (stw/stw/lfd/lfd/fsubs) inline.
    // Explicit if/else assignment => MWCC li-diamond, no accumulator preinit
    // (MWCC_CASES us-80374610 bool-diamond rule).
    // retail reads the volatile limit once per call and reuses it for both
    // the zero guard and the int-to-float compare.
    u32 limit = self->mUnk0C;
    BOOL over;
    if (limit != 0 && self->mIntensity > (float)limit) {
        over = TRUE;
    } else {
        over = FALSE;
    }
    return over;
}
