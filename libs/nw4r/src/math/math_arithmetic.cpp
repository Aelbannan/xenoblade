#include <nw4r/math.h>

namespace nw4r {
namespace math {
namespace {

struct ExpSample {
    f32 exp_val;   // at 0x0
    f32 exp_delta; // at 0x4
};

struct LogSample {
    f32 log_val;   // at 0x0
    f32 log_delta; // at 0x4
};

// Retail owns the exp/log interpolation tables in nw4r_data.s (.data):
// lbl_eu_80569CD0 = 33 exp entries, lbl_eu_80569DD8 = 257 log entries. This
// .text-only split references them rather than defining them.
extern "C" ExpSample lbl_eu_80569CD0[33];
extern "C" LogSample lbl_eu_80569DD8[257];

f32 FExpLn2(f32 x) {
    f32 fidx = (16 / NW4R_MATH_LN_2) * (NW4R_MATH_LN_2 + x);
    u16 whole = F32ToU16(fidx);
    f32 frac = fidx - U16ToF32(whole);

    return lbl_eu_80569CD0[whole].exp_val + frac * lbl_eu_80569CD0[whole].exp_delta;
}

f32 FLog1_2(f32 x) {
    f32 fidx = 256.0f * (x - 1.0f);
    u16 whole = F32ToU16(fidx);
    f32 frac = fidx - U16ToF32(whole);

    return lbl_eu_80569DD8[whole].log_val + frac * lbl_eu_80569DD8[whole].log_delta;
}

} // namespace

namespace detail {

f32 FExp(f32 x) {
    s16 k = F32ToS16((1 / NW4R_MATH_LN_2) * x);
    f32 kf = S16ToF32(k);
    f32 expxn = FExpLn2(x - NW4R_MATH_LN_2 * kf);
    u32 expx = F32AsU32(expxn);

    return U32AsF32(((k << 23) + expx) & 0x7FFFFFFF);
}

f32 FLog(f32 x) {
    s32 exp = FGetExpPart(x);
    f32 xn = FGetMantPart(x);
    f32 kf = S16ToF32(exp);
    f32 logxn = FLog1_2(xn);

    return logxn + NW4R_MATH_LN_2 * kf;
}

} // namespace detail

f32 FrSqrt(register f32 x) {
    register f32 rsqrt;
    register f32 c_half = 0.5f, c_three = 3.0f;
    register f32 work0, work1;

    ASM (
        // Estimate reciprocal square root
        frsqrte rsqrt, x
        
        // Refine estimate using Newton-Raphson method
        // y = 1 / sqrt(x)
        fmuls work0, rsqrt, rsqrt         // rsqrt^2
        fmuls work1, rsqrt, c_half        // rsqrt * 0.5
        fnmsubs work0, work0, x, c_three  // (3 - x * rsqrt^2)
        fmuls work1, work0, work1         // (3 - x * rsqrt^2) * (rsqrt * 0.5)
    )

    return work1;
}

namespace {

} // namespace
} // namespace math
} // namespace nw4r
