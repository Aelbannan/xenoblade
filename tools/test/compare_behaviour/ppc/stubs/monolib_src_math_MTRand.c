#include <string.h>

#include <types.h>

#include "mtrand_common.h"

s8 rb_lbl_eu_80665580;
f32 rb_lbl_eu_8066A1D0;
f64 rb_lbl_eu_8066A1D8;
f32 rb_lbl_eu_8066A1E0;
f32 dc_lbl_eu_8066A1D0;
f64 dc_lbl_eu_8066A1D8;
f32 dc_lbl_eu_8066A1E0;

__attribute__((aligned(8))) u8 rb_mtrand_singleton[0x9D0];

void mtrand_init_lbl_constants(void) {
    static const u64 kMagicD8 = 0x4330000080000000ULL;
    const f32 scale = 2.32830643653869628906e-10f;
    const f32 one = 1.0f;

    rb_lbl_eu_8066A1D0 = scale;
    dc_lbl_eu_8066A1D0 = scale;
    rb_lbl_eu_8066A1E0 = one;
    dc_lbl_eu_8066A1E0 = one;
    memcpy(&rb_lbl_eu_8066A1D8, &kMagicD8, sizeof(kMagicD8));
    memcpy(&dc_lbl_eu_8066A1D8, &kMagicD8, sizeof(kMagicD8));
}
