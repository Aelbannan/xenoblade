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

/* SFTMR_Tsum: 64-bit accumulate + min/max tracking used by SFTMR_AddTsum */
typedef struct SFTMR_Tsum {
    s64 tsum;
    s64 min;
    s64 max;
    u32 num;
} SFTMR_Tsum;

/* SFTMR_AddTsum: add a 64-bit delta to the accumulator and track min/max of deltas */
void SFTMR_AddTsum(SFTMR_Tsum *tsum, void *unused, s64 delta) {
    tsum->tsum += delta;

    tsum->min = (delta < tsum->min) ? delta : tsum->min;
    tsum->max = (tsum->max < delta) ? delta : tsum->max;
    tsum->num++;
}
