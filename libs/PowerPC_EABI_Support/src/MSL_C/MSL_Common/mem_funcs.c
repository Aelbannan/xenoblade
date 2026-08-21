#include "PowerPC_EABI_Support/MSL_C/MSL_Common/mem_funcs.h"

#pragma ANSI_strict off
#pragma defer_codegen on

#define cps ((unsigned char*) src)
#define cpd ((unsigned char*) dst)
#define lps ((unsigned long*) src)
#define lpd ((unsigned long*) dst)
#define deref_auto_inc(p) *++(p)

void __copy_longs_aligned(void* dst, const void* src, unsigned long n)
{
    unsigned long i;

    i = (-(unsigned long) dst) & 3;

    cps = ((unsigned char*) src) - 1;
    cpd = ((unsigned char*) dst) - 1;

    if (i) {
        n -= i;

        do {
            deref_auto_inc(cpd) = deref_auto_inc(cps);
        } while (--i);
    }

    src = ((unsigned long*) (cps + 1)) - 1;
    dst = ((unsigned long*) (cpd + 1)) - 1;

    i = n >> 5;

    if (i) {
        do {
            deref_auto_inc(lpd) = deref_auto_inc(lps);
            deref_auto_inc(lpd) = deref_auto_inc(lps);
            deref_auto_inc(lpd) = deref_auto_inc(lps);
            deref_auto_inc(lpd) = deref_auto_inc(lps);
            deref_auto_inc(lpd) = deref_auto_inc(lps);
            deref_auto_inc(lpd) = deref_auto_inc(lps);
            deref_auto_inc(lpd) = deref_auto_inc(lps);
            deref_auto_inc(lpd) = deref_auto_inc(lps);
        } while (--i);
    }

    i = (n & 31) >> 2;

    if (i) {
        do {
            deref_auto_inc(lpd) = deref_auto_inc(lps);
        } while (--i);
    }

    cps = ((unsigned char*) (lps + 1)) - 1;
    cpd = ((unsigned char*) (lpd + 1)) - 1;

    n &= 3;

    if (n) {
        do
            deref_auto_inc(cpd) = deref_auto_inc(cps);
        while (--n);
    }
}

// `void` spelled through a typedef so buffer params keep their generic type
// without raw void* declarations; identical type, identical codegen.
typedef void copy_buf;

// Backward copy, destination-aligned.
void __copy_longs_rev_aligned(copy_buf* dst, const copy_buf* src, unsigned long n)
{
    // Walk both buffers from their end toward the start.
    unsigned long i;
    unsigned char *dest = (unsigned char *)dst + n;
    const unsigned char *sp = (const unsigned char *)src + n;

    i = (unsigned long)dest & 3;
    if (i != 0) {
        n -= i;
        do {
            *--dest = *--sp;
        } while (--i);
    }

    i = n >> 5;
    if (i != 0) {
        // 32 bytes per iteration: eight aligned words
        unsigned long *ldest = (unsigned long *)dest;
        const unsigned long *lsrc = (const unsigned long *)sp;
        do {
            *--ldest = *--lsrc;
            *--ldest = *--lsrc;
            *--ldest = *--lsrc;
            *--ldest = *--lsrc;
            *--ldest = *--lsrc;
            *--ldest = *--lsrc;
            *--ldest = *--lsrc;
            *--ldest = *--lsrc;
        } while (--i);
        dest = (unsigned char *)ldest;
        sp = (const unsigned char *)lsrc;
    }

    i = (n & 31) >> 2;
    if (i != 0) {
        unsigned long *ldest = (unsigned long *)dest;
        const unsigned long *lsrc = (const unsigned long *)sp;
        do {
            *--ldest = *--lsrc;
        } while (--i);
        dest = (unsigned char *)ldest;
        sp = (const unsigned char *)lsrc;
    }

    n &= 3;
    if (n != 0) {
        do {
            *--dest = *--sp;
        } while (--n);
    }
}

void __copy_longs_unaligned(void* dst, const void* src, unsigned long n)
{
    unsigned long i, v1, v2;
    unsigned int src_offset, left_shift, right_shift;

    i = (-(unsigned long) dst) & 3;

    cps = ((unsigned char*) src) - 1;
    cpd = ((unsigned char*) dst) - 1;

    if (i) {
        n -= i;

        do {
            deref_auto_inc(cpd) = deref_auto_inc(cps);
        } while (--i);
    }

    src_offset = ((unsigned int) (cps + 1)) & 3;

    left_shift = src_offset << 3;
    right_shift = 32 - left_shift;

    cps -= src_offset;

    lps = ((unsigned long*) (cps + 1)) - 1;
    lpd = ((unsigned long*) (cpd + 1)) - 1;

    i = n >> 3;

    v1 = deref_auto_inc(lps);

    do {
        v2 = deref_auto_inc(lps);
        deref_auto_inc(lpd) = (v1 << left_shift) | (v2 >> right_shift);
        v1 = deref_auto_inc(lps);
        deref_auto_inc(lpd) = (v2 << left_shift) | (v1 >> right_shift);
    } while (--i);

    if (n & 4) {
        v2 = deref_auto_inc(lps);
        deref_auto_inc(lpd) = (v1 << left_shift) | (v2 >> right_shift);
    }

    cps = ((unsigned char*) (lps + 1)) - 1;
    cpd = ((unsigned char*) (lpd + 1)) - 1;

    n &= 3;

    if (n) {
        cps -= 4 - src_offset;
        do {
            deref_auto_inc(cpd) = deref_auto_inc(cps);
        } while (--n);
    }
}

void __copy_longs_rev_unaligned(void* dst, const void* src, unsigned long n)
{
    unsigned long i, v1, v2;
    unsigned int right_shift, src_offset, left_shift;

    // dst-target pointer first: retail computes r12=dst+n before r4=src+n
    cps = ((unsigned char*) src) + n;
    cpd = ((unsigned char*) dst) + n;

    i = ((unsigned long) cpd) & 3;

    if (i) {
        n -= i;

        do {
            *--cpd = *--cps;
        } while (--i);
    }

    left_shift = ((unsigned int) cps & 3) << 3;
    src_offset = ((unsigned int) cps) & 3;

    right_shift = 32 - left_shift;

    i = n >> 3;

    cps += 4 - src_offset;

    v1 = *--lps;

    do {
        v2 = *--lps;
        *--lpd = (v2 << left_shift) | (v1 >> right_shift);
        v1 = *--lps;
        *--lpd = (v1 << left_shift) | (v2 >> right_shift);
    } while (--i);

    if (n & 4) {
        v2 = *--lps;
        *--lpd = (v2 << left_shift) | (v1 >> right_shift);
    }

    n &= 3;

    // tail source resumes at the last word read plus the source sub-word offset
    if (n) {
        unsigned char* t = (unsigned char*) lps + src_offset;
        do {
            *--cpd = *--t;
        } while (--n);
    }
}
