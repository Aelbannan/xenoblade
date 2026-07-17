#include "PowerPC_EABI_Support/MSL_C/MSL_Common/mem_funcs.h"

#define srcCharPtr  ((unsigned char*)pSrc)
#define destCharPtr ((unsigned char*)pDest)
#define srcLongPtr  ((unsigned long*)pSrc)
#define destLongPtr ((unsigned long*)pDest)

//unused
void __copy_mem(){
}

//unused
void __move_mem(){
}

void __copy_longs_aligned(void *dst, const void *src, unsigned long n) {
    unsigned long i;
    unsigned char *d, *s;
    unsigned long *dw, *sw;

    i = (-(unsigned long)dst) & 3;
    s = (unsigned char *)src - 1;
    d = (unsigned char *)dst - 1;

    if (i) {
        n -= i;
        do {
            *++d = *++s;
        } while (--i);
    }

    sw = (unsigned long *)(s + 1) - 1;
    dw = (unsigned long *)(d + 1) - 1;

    i = n >> 5;
    if (i) {
        do {
            *++dw = *++sw;
            *++dw = *++sw;
            *++dw = *++sw;
            *++dw = *++sw;
            *++dw = *++sw;
            *++dw = *++sw;
            *++dw = *++sw;
            *++dw = *++sw;
        } while (--i);
    }

    i = (n & 31) >> 2;
    if (i) {
        do {
            *++dw = *++sw;
        } while (--i);
    }

    s = (unsigned char *)(sw + 1) - 1;
    d = (unsigned char *)(dw + 1) - 1;

    n &= 3;
    if (n) {
        do {
            *++d = *++s;
        } while (--n);
    }
}

void __copy_longs_rev_aligned(void *pDest, const void *pSrc, unsigned long len) {
    unsigned long i;
    unsigned char *dest = (unsigned char *)pDest + len;
    const unsigned char *src = (const unsigned char *)pSrc + len;

    i = (unsigned long)dest & 3;
    if (i != 0) {
        len -= i;
        do {
            *--dest = *--src;
        } while (--i);
    }

    i = len >> 5;
    if (i != 0) {
        unsigned long *ldest = (unsigned long *)dest;
        const unsigned long *lsrc = (const unsigned long *)src;
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
        src = (const unsigned char *)lsrc;
    }

    i = (len & 31) >> 2;
    if (i != 0) {
        unsigned long *ldest = (unsigned long *)dest;
        const unsigned long *lsrc = (const unsigned long *)src;
        do {
            *--ldest = *--lsrc;
        } while (--i);
        dest = (unsigned char *)ldest;
        src = (const unsigned char *)lsrc;
    }

    len &= 3;
    if (len != 0) {
        do {
            *--dest = *--src;
        } while (--len);
    }
}

void __copy_longs_unaligned(void *pDest, const void *pSrc, unsigned long len) {
    unsigned long i, v1, v2;
    unsigned int src, ls, rs;

    i = (-(unsigned long)pDest) & 3;
    srcCharPtr = ((unsigned char*)pSrc) - 1;
    destCharPtr = ((unsigned char*)pDest) - 1;

    if (i != 0) {
        len -= i;

        do {
            *++destCharPtr = *++srcCharPtr;
        } while(--i);
    }

    src = ((unsigned int)(srcCharPtr + 1)) & 3;
    ls = src << 3;
    rs = 32 - ls;

    srcCharPtr -= src;

    srcLongPtr = ((unsigned long*)(srcCharPtr + 1)) - 1;
    destLongPtr = ((unsigned long*)(destCharPtr + 1)) - 1;

    i = len >> 3;
    v1 = *++srcLongPtr;

    do {
        v2 = *++srcLongPtr;
        *++destLongPtr = (v1 << ls) | (v2 >> rs);
        v1 = *++srcLongPtr;
        *++destLongPtr = (v2 << ls) | (v1 >> rs);
    } while(--i);

    if (len & 4) {
        v2 = *++srcLongPtr;
        *++destLongPtr = (v1 << ls) | (v2 >> rs);
    }

    srcCharPtr = ((unsigned char *)(srcLongPtr + 1)) - 1;
    destCharPtr = ((unsigned char *)(destLongPtr + 1)) - 1;

    len &= 3;

    if (len != 0) {
        srcCharPtr -= 4 - src;
        do {
            *++destCharPtr = *++srcCharPtr;
        } while(--len);
    }
}

void __copy_longs_rev_unaligned(void *pDest, const void *pSrc, unsigned long len) {
    unsigned long i, v1, v2;
    unsigned int src, ls, rs;

    srcCharPtr = ((unsigned char*)pSrc) + len;
    destCharPtr = ((unsigned char*)pDest) + len;
    i = ((unsigned long)pDest) & 3;

    if (i != 0) {
        len -= i;

        do {
            *--destCharPtr = *--srcCharPtr;
        } while(--i);
    }

    src = ((unsigned int)(srcCharPtr)) & 3;
    ls = src << 3;
    rs = 32 - ls;

    srcCharPtr += 4 - src;

    srcLongPtr = ((unsigned long*)srcCharPtr);
    destLongPtr = ((unsigned long*)destCharPtr);
    
    i = len >> 3;
    v1 = *--srcLongPtr;

    do {
        v2 = *--srcLongPtr;
        *--destLongPtr = (v2 << ls) | (v1 >> rs);
        v1 = *--srcLongPtr;
        *--destLongPtr = (v1 << ls) | (v2 >> rs);
    } while(--i);

    if (len & 4) {
        v2 = *--srcLongPtr;
        *--destLongPtr = (v2 << ls) | (v1 >> rs);
    }

    len &= 3;

    srcCharPtr = ((unsigned char*)pSrc);
    destCharPtr = ((unsigned char*)pDest);

    if (len != 0) {
        srcCharPtr += src;
        do {
            *--destCharPtr = *--srcCharPtr;
        } while(--len);
    }
}
