#ifndef MTRAND_COMMON_H
#define MTRAND_COMMON_H

#include <types.h>

#define MTRAND_N 624
#define MTRAND_SEED 0x012BD6AAu
#define MTRAND_STATE1 0x82D2AB13u

typedef struct MTRand {
    u32 state[MTRAND_N];
    s32 left;
    BOOL initialized;
    u32* pNext;
    u32 unk9D0;
} MTRand;

extern s8 rb_lbl_eu_80665580;
extern f32 rb_lbl_eu_8066A1D0;
extern f64 rb_lbl_eu_8066A1D8;
extern f32 rb_lbl_eu_8066A1E0;
extern f32 dc_lbl_eu_8066A1D0;
extern f64 dc_lbl_eu_8066A1D8;
extern f32 dc_lbl_eu_8066A1E0;

void mtrand_init_lbl_constants(void);

#endif
