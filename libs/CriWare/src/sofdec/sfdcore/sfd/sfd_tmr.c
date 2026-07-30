#include <harness_catalog.h>

extern int UTY_IsTmrVoid(s32);
extern u64 UTY_GetTmrUnit(void);
extern u64 UTY_GetTmr(void);

extern u32 lbl_eu_80619BC8[2];  /* timer base (hi/lo) */
extern u32 lbl_eu_80606E38[];   /* SFD work area */

/* SFTMR_GetTmr: get current timer value as 64-bit */
u64 SFTMR_GetTmr(void *sfd) {
    u64 result;

    if (!UTY_IsTmrVoid(0)) {
        /* use UTY timer directly */
        u64 unit = UTY_GetTmrUnit();
        lbl_eu_80619BC8[0] = (u32)(unit >> 32);
        lbl_eu_80619BC8[1] = (u32)(unit);
        result = UTY_GetTmr();
        return result;
    }

    if (sfd != NULL) {
        u32 *p = (u32 *)sfd;
        if (p[0x54/4] != 0) {
            void (*gettime)(u32, u32 *, u32 *) = (void (*)(u32, u32 *, u32 *))p[0x107C/4];
            if (gettime != NULL) {
                u32 hi, lo;
                u32 arg = p[0x1090/4];
                gettime(arg, &lo, &hi);
                lbl_eu_80619BC8[0] = hi;
                lbl_eu_80619BC8[1] = lo;
                result = ((u64)hi << 32) | lo;
                return result;
            }
        }
    }

    /* fallback: use SFD work area frame timing */
    {
        u32 *sfd_work = lbl_eu_80606E38;
        u32 rate = sfd_work[0x19C/4];       /* frame rate */
        u32 frame_num_hi = sfd_work[0x1A8/4]; /* frame number hi */
        u32 frame_num_lo = sfd_work[0x1AC/4]; /* frame number lo */
        u32 product_lo = rate * frame_num_lo;
        lbl_eu_80619BC8[0] = frame_num_hi;
        lbl_eu_80619BC8[1] = frame_num_lo;
        result = ((u64)frame_num_hi << 32) | frame_num_lo;
        return result;
    }
}

/* SFTMR_GetTmrUnit: get timer unit/period as 64-bit */
u64 SFTMR_GetTmrUnit(void *sfd) {
    u64 result;

    if (lbl_eu_80619BC8[0] != 0 || lbl_eu_80619BC8[1] != 0) {
        /* already have a cached value */
        result = ((u64)lbl_eu_80619BC8[0] << 32) | lbl_eu_80619BC8[1];
        return result;
    }

    if (!UTY_IsTmrVoid(0)) {
        u64 unit = UTY_GetTmrUnit();
        lbl_eu_80619BC8[0] = (u32)(unit >> 32);
        lbl_eu_80619BC8[1] = (u32)(unit);
        result = UTY_GetTmr();
        return result;
    }

    if (sfd != NULL) {
        u32 *p = (u32 *)sfd;
        if (p[0x54/4] != 0) {
            void (*gettime)(u32, u32 *, u32 *) = (void (*)(u32, u32 *, u32 *))p[0x107C/4];
            if (gettime != NULL) {
                u32 hi, lo;
                u32 arg = p[0x1090/4];
                gettime(arg, &hi, &lo);
                lbl_eu_80619BC8[0] = lo;
                lbl_eu_80619BC8[1] = hi;
                result = ((u64)lo << 32) | hi;
                return result;
            }
        }
    }

    /* fallback */
    {
        u32 *sfd_work = lbl_eu_80606E38;
        u32 frame_num = sfd_work[0x1A8/4];
        lbl_eu_80619BC8[1] = frame_num;
        lbl_eu_80619BC8[0] = (s32)frame_num >> 31;
        u32 rate = sfd_work[0x19C/4];
        result = ((u64)(s32)frame_num >> 31 << 32) | frame_num;
        return result;
    }
}

/* SFTMR_InitTsum: initialize a timestamp accumulator struct */
void SFTMR_InitTsum(u32 *tsum) {
    u32 zero = 0;
    tsum[1] = zero;
    tsum[0] = zero;
    tsum[3] = (u32)-1;
    tsum[2] = 0x7FFFFFFF;
    tsum[5] = zero;
    tsum[4] = zero;
    tsum[6] = zero;
}

/* SFTMR_AddTsum: add a 64-bit timestamp delta to accumulator with clamping */
void SFTMR_AddTsum(u32 *tsum, s32 delta_hi, u32 delta_lo) {
    u32 a_lo = tsum[1];
    u32 a_hi = tsum[0];
    u32 max_lo = tsum[3];
    u32 max_hi = tsum[2];

    /* add delta to accumulator */
    u32 new_lo = a_lo + delta_lo;
    u32 new_hi = a_hi + delta_hi + (new_lo < a_lo ? 1 : 0);
    tsum[1] = new_lo;
    tsum[0] = new_hi;

    /* check if accumulator > max (signed 64-bit comparison) */
    {
        s32 sign_max = (s32)max_hi;
        s32 sign_new = (s32)new_hi;
        int exceeds = 0;
        if (sign_new > sign_max) exceeds = 1;
        else if (sign_new == sign_max && new_lo > max_lo) exceeds = 1;

        if (exceeds) {
            /* clamp to max */
            tsum[3] = delta_lo;
            tsum[2] = delta_hi;
        }
    }

    /* check if accumulator < min (signed 64-bit comparison) */
    {
        u32 min_hi = tsum[4];
        u32 min_lo = tsum[5];
        s32 sign_min = (s32)min_hi;
        s32 sign_new = (s32)new_hi;
        int below = 0;
        if (sign_new < sign_min) below = 1;
        else if (sign_new == sign_min && new_lo < min_lo) below = 1;
        else if ((s32)new_hi < 0 && (s32)min_hi >= 0) below = 1;

        if (!below) {
            /* no clamp needed */
            tsum[5] = min_lo;
            tsum[4] = min_hi;
        }
    }

    tsum[6]++; /* increment count */
}
