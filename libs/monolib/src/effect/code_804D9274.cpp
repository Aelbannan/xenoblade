// Effect random-vector helpers (monolib). Retail symbol names are unknown;
// these functions generate randomized directions/positions for the effect
// system:
//   func_804D9274 - tail-call wrapper: rotate a matrix +90 deg about X
//   func_804D927C - transpose the matrix embedded at obj+0xCC
//   func_804D928C - build an orientation matrix from a direction vector
//   func_804D9364 - random direction (yaw/pitch angle ranges) scaled by a
//                   rejection-sampled radius
//   func_804D9A10 - random box vector with inner-box rejection
//   func_804D9B70 - random cylinder vector
//   func_804D9F00 - random arc vector

#include <harness_catalog.h>
#include <revolution/MTX.h>
#include <nw4r/math.h>
#include <monolib/math.hpp>
#include <monolib/effect/CERand.hpp>

// Rotate `mtx` about the X axis by `angle` (code_804DB938.cpp).
extern "C" {
void func_804DD388(Mtx mtx, f32 angle);

// Shared sdata2 constants (retail linker names).
extern f32 lbl_eu_8066B1E8; // pi/2
extern f32 lbl_eu_8066B1F0; // 2^31
extern f32 lbl_eu_8066B1FC; // 180/pi
extern f32 lbl_eu_8066B210; // pi/180
extern f32 lbl_eu_8066A210; // pi/180

// Degree-indexed sin/cos tables: 360 entries of {value, slope} pairs,
// populated at runtime by monolib/src/core/code_804EE558.cpp.
struct CESinCosEntry {
    f32 value;
    f32 slope;
};
extern CESinCosEntry lbl_eu_80660038[360]; // sin table
extern CESinCosEntry lbl_eu_80660B78[360]; // cos table

// Retail keeps the FSqrt assert strings in split1.s (lbl_eu_80526300 =
// "FSqrt: Input is out of the domain.", lbl_eu_80526324 = "arithmetic.h");
// reference the named externs so no local .rodata string table is emitted.
extern "C" const char lbl_eu_80526300[];  // "FSqrt: Input is out of the domain."
extern "C" const char lbl_eu_80526324[];  // "arithmetic.h"

// Uniform random in [0, 1).
static inline f32 randF() {
    return (f32)ml::math::mtRand() / lbl_eu_8066B1F0;
}

// nw4r::math::FSqrt inlined, preserving the retail assert location
// (arithmetic.h:627).
static inline f32 fSqrt(f32 x) {
    if (!(x >= 0.0f)) {
        nw4r::db::Warning(lbl_eu_80526324, 627, lbl_eu_80526300);
    }
    return x <= 0.0f ? 0.0f : x * nw4r::math::FrSqrt(x);
}

// Table-interpolated sin/cos of an angle given in radians.
static inline f32 sinTbl(f32 rad) {
    f32 deg = rad * lbl_eu_8066B1FC;
    int i = (int)deg;
    int idx = i % 360;
    if (idx < 0) {
        idx += 360;
    }
    CESinCosEntry* entry = &lbl_eu_80660038[idx];
    return entry->value + (deg - (f32)i) * entry->slope;
}

static inline f32 cosTbl(f32 rad) {
    f32 deg = rad * lbl_eu_8066B1FC;
    int i = (int)deg;
    int idx = i % 360;
    if (idx < 0) {
        idx += 360;
    }
    CESinCosEntry* entry = &lbl_eu_80660B78[idx];
    return entry->value + (deg - (f32)i) * entry->slope;
}

// func_804D9274: load pi/2 and tail-call the X-axis rotation helper.
void func_804D9274(Mtx mtx) {
    func_804DD388(mtx, lbl_eu_8066B1E8);
}

// func_804D927C: transpose the matrix stored at obj+0xCC into mtx.
void func_804D927C(Mtx mtx, const void* obj) {
    PSMTXTranspose((const f32(*)[4])((const u8*)obj + 0xCC), mtx);
}

// func_804D928C: build an orientation matrix whose Z axis points along
// (obj[0xEC], 0, obj[0xF4]), then transpose it.
void func_804D928C(Mtx mtx, const u8* obj) {
    *(ml::CMat34*)mtx = ml::CMat34::identity;
    mtx[2][0] = *(const f32*)(obj + 0xEC);
    mtx[2][1] = 0.0f;
    mtx[2][2] = *(const f32*)(obj + 0xF4);
    mtx[2][3] = 0.0f;
    PSVECNormalize((Vec*)&mtx[2], (Vec*)&mtx[2]);
    PSVECCrossProduct((Vec*)&mtx[1], (Vec*)&mtx[2], (Vec*)&mtx[0]);
    mtx[0][3] = 0.0f;
    PSMTXTranspose(mtx, mtx);
}

// func_804D9364: build a random unit direction from yaw/pitch angle ranges
// (scale->y and scale->z, negated, in degrees; 0 means full range), then
// scale it by a rejection-sampled length along scale->x * dir->x.
void func_804D9364(ml::CVec3* out, const ml::CVec3* scale, const ml::CVec3* dir, f32 radius) {
    f32 sx = scale->x * dir->x;
    f32 sy = scale->y;
    f32 sz = scale->z;
    f32 minLen = sx * radius;

    f32 len = fSqrt(sx * (sx * randF()));
    if (0.0f < radius && radius < 1.0f) {
        while (len < minLen) {
            len = fSqrt(sx * (sx * randF()));
        }
    } else if (radius < 1.0f) {
        len = sx;
    }

    int yawRange = (int)(-sy);
    if (yawRange == 0) {
        yawRange = 360;
    }
    f32 yaw = (f32)yawRange * randF() * lbl_eu_8066A210;
    out->x = cosTbl(yaw);
    out->y = 0.0f;
    out->z = sinTbl(yaw);
    out->normalizeSub();

    int pitchRange = (int)sz;
    if (pitchRange == 0) {
        pitchRange = 180;
    }
    f32 pitch = (f32)pitchRange * randF() * lbl_eu_8066A210;
    out->x = out->x * sinTbl(pitch);
    out->y = cosTbl(pitch);

    f32 roll = (f32)pitchRange * randF() * lbl_eu_8066A210;
    out->y = out->y * cosTbl(roll);
    out->z = out->z * sinTbl(roll);
    out->normalizeSub();

    *out = *out * len;
}

// func_804D9A10: random unit box vector (CERand::randVec), re-rolled until at
// least one component escapes the inner |radius/2| box; for radius <= 0 a
// single random axis is pinned to +/-0.5 instead. The result is scaled
// component-wise by scale * dir.
void func_804D9A10(ml::CVec3* out, const ml::CVec3* scale, const ml::CVec3* dir, f32 radius) {
    f32 mx = scale->x * dir->x;
    f32 my = scale->y * dir->y;
    f32 mz = scale->z * dir->z;

    CERand::randVec(out);
    if (0.0f < radius && radius < 1.0f) {
        radius *= 0.5f;
        while (ml::math::abs(out->x) < radius &&
               ml::math::abs(out->y) < radius &&
               ml::math::abs(out->z) < radius) {
            CERand::randVec(out);
        }
    } else if (radius < 1.0f) {
        f32 sign = (ml::math::mtRand() % 2 != 0) ? 1.0f : -1.0f;
        f32 v = 0.5f * sign;
        (&out->x)[ml::math::mtRand(3)] = v;
    }

    out->x *= mx;
    out->y *= my;
    out->z *= mz;
}

// func_804D9B70: random cylinder vector: angle around Y from the full
// (int)(-scale->z) degree range, height jittered by scale->y * dir->y, radial
// length rejection-sampled against scale->x * dir->x.
void func_804D9B70(ml::CVec3* out, const ml::CVec3* scale, const ml::CVec3* dir, f32 radius) {
    f32 mx = scale->x * dir->x;
    f32 my = scale->y * dir->y;

    int range = (int)(-scale->z);
    if (range == 0) {
        range = 360;
    }
    f32 angle = (f32)range * randF() * lbl_eu_8066A210;

    f32 len = fSqrt(mx * (mx * randF()));
    if (0.0f < radius && radius < 1.0f) {
        while (len < mx * radius) {
            len = fSqrt(mx * (mx * randF()));
        }
    } else if (1.0f > radius) {
        len = mx;
    }

    out->x = len * cosTbl(angle);
    out->y = my * (randF() - 0.5f);
    out->z = len * sinTbl(angle);
}

// func_804D9F00: random arc vector in the X/Z plane over (int)(-scale->y)
// degrees, scaled by scale->x * dir->x.
void func_804D9F00(ml::CVec3* out, const ml::CVec3* scale, const ml::CVec3* dir) {
    ml::CVec3 temp(scale->x * dir->x, scale->y, 0.0f);

    int range = (int)(-temp.y);
    if (range == 0) {
        range = 360;
    }
    f32 angle = (f32)range * randF() * lbl_eu_8066B210;

    out->x = temp.x * cosTbl(angle);
    out->y = 0.0f;
    out->z = temp.x * sinTbl(angle);
}

} // extern "C"
