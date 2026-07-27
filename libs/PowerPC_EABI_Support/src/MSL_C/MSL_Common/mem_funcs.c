#include "PowerPC_EABI_Support/MSL_C/MSL_Common/mem_funcs.h"

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
    s = (unsigned char *)src;
    s--;
    d = (unsigned char *)dst;
    d--;

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

    s = (unsigned char *)(sw + 1);
    s--;
    d = (unsigned char *)(dw + 1);
    d--;

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
    unsigned char* srcChar;
    unsigned char* destChar;
    unsigned long* srcLong;
    unsigned long* destLong;

    i = (-(unsigned long)pDest) & 3;
    srcChar = (unsigned char*)pSrc;
    srcChar--;
    destChar = (unsigned char*)pDest;
    destChar--;

    if (i != 0) {
        len -= i;

        do {
            *++destChar = *++srcChar;
        } while(--i);
    }

    src = ((unsigned int)(srcChar + 1)) & 3;
    ls = src << 3;
    rs = 32 - ls;

    srcChar -= src;

    srcLong = (unsigned long*)(srcChar + 1) - 1;
    destLong = (unsigned long*)(destChar + 1) - 1;

    i = len >> 3;
    v1 = *++srcLong;

    do {
        v2 = *++srcLong;
        *++destLong = (v1 << ls) | (v2 >> rs);
        v1 = *++srcLong;
        *++destLong = (v2 << ls) | (v1 >> rs);
    } while(--i);

    if (len & 4) {
        v2 = *++srcLong;
        *++destLong = (v1 << ls) | (v2 >> rs);
    }

    srcChar = (unsigned char*)(srcLong + 1);
    srcChar--;
    destChar = (unsigned char*)(destLong + 1);
    destChar--;

    len &= 3;

    if (len != 0) {
        srcChar -= 4 - src;
        do {
            *++destChar = *++srcChar;
        } while(--len);
    }
}

void __copy_longs_rev_unaligned(void *pDest, const void *pSrc, unsigned long len) {
    unsigned long i, v1, v2;
    unsigned int src, ls, rs;
    unsigned char* srcChar;
    unsigned char* destChar;
    unsigned long* srcLong;
    unsigned long* destLong;

    srcChar = (unsigned char*)pSrc + len;
    destChar = (unsigned char*)pDest + len;
    i = ((unsigned long)pDest) & 3;

    if (i != 0) {
        len -= i;

        do {
            *--destChar = *--srcChar;
        } while(--i);
    }

    src = ((unsigned int)(srcChar)) & 3;
    ls = src << 3;
    rs = 32 - ls;

    srcChar += 4 - src;

    srcLong = (unsigned long*)srcChar;
    destLong = (unsigned long*)destChar;
    
    i = len >> 3;
    v1 = *--srcLong;

    do {
        v2 = *--srcLong;
        *--destLong = (v2 << ls) | (v1 >> rs);
        v1 = *--srcLong;
        *--destLong = (v1 << ls) | (v2 >> rs);
    } while(--i);

    if (len & 4) {
        v2 = *--srcLong;
        *--destLong = (v2 << ls) | (v1 >> rs);
    }

    len &= 3;

    srcChar = (unsigned char*)pSrc;
    destChar = (unsigned char*)pDest;

    if (len != 0) {
        srcChar += src;
        do {
            *--destChar = *--srcChar;
        } while(--len);
    }
}
