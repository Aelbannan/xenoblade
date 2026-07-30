#include <harness_catalog.h>

s64 UTY_MulDivRound64(s64 a, s64 b, s64 c) {
    /* 64-bit multiply-divide: (a * b) / c with rounding */
    u32 neg = 0;
    u32 a_hi, a_lo, b_hi, b_lo;
    u64 a_abs, b_abs, c_abs;
    
    if (c == 0) {
        /* Divide by zero */
        if ((a < 0) != (b < 0))
            return (s64)0x8000000000000000ULL;
        else
            return (s64)0x7FFFFFFFFFFFFFFFULL;
    }
    
    a_abs = (a < 0) ? (u64)(-a) : (u64)(a);
    b_abs = (b < 0) ? (u64)(-b) : (u64)(b);
    c_abs = (c < 0) ? (u64)(-c) : (u64)(c);
    neg = ((a < 0) != (b < 0) != (c < 0)) ? 1 : 0;
    
    a_hi = (u32)(a_abs >> 32);
    a_lo = (u32)(a_abs);
    b_hi = (u32)(b_abs >> 32);
    b_lo = (u32)(b_abs);
    
    /* Compute (a_abs * b_abs) / c_abs as 128/64 division */
    /* 64x64 multiply: (a_hi<<32|a_lo) * (b_hi<<32|b_lo) */
    u64 mid_low = (u64)a_hi * (u64)b_lo + (u64)a_lo * (u64)b_hi;
    u64 low = (u64)a_lo * (u64)b_lo;
    u64 high = (u64)a_hi * (u64)b_hi + (mid_low >> 32);
    u64 mid_low_part = (mid_low << 32) + (u32)low;
    
    /* 128/64 division approximation */
    if (high == 0) {
        /* Result fits in 64 bits */
        s64 result = (s64)((u64)a_abs * (u64)b_abs / c_abs);
        return neg ? -result : result;
    }
    
    /* Result too large, use shift approximation */
    s64 result = (s64)(((u64)a_abs / c_abs) * (u64)b_abs + 
                      ((u64)a_abs % c_abs) * (u64)b_abs / c_abs);
    return neg ? -result : result;
}
