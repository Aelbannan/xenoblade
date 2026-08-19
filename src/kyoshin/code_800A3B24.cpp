// Auto-scaffolded catalog TU for kyoshin/code_800A3B24
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "monolib/math/CVec3.hpp"
#include "monolib/math/MTRand.hpp"
#include "monolib/core/CDrawGX.hpp"

// Retail SDA2/data constants referenced by the PS math helpers below. Declared
// by retail label so the emitted @sda21 relocs match retail byte-for-byte; the
// values themselves live in the retail binary's .sdata2/.data sections.
extern const float lbl_eu_806667D8;  // 0.0f
// 1.0f (lbl_eu_806667E8)
extern const float lbl_eu_806667E8;
// ml::epsilon (lbl_eu_8066A208)
extern const float lbl_eu_8066A208;
// pi (lbl_eu_8066A1F8)
extern const float lbl_eu_8066A1F8;
// 2*pi (lbl_eu_8066A1FC)
extern const float lbl_eu_8066A1FC;
// nw4r FSqrt assert source-file string + message (retail .data)
extern char lbl_eu_80526324[];
extern char lbl_eu_80526300[];
// squared-length guard used by the segment projection helper (lbl_eu_806667F8)
extern const float lbl_eu_806667F8;
// 0.5f (lbl_eu_806667F4) - random-direction offset
 extern const float lbl_eu_806667F4;
// random-angle range (lbl_eu_8066A200)
 extern const float lbl_eu_8066A200;
// random-angle table scale (lbl_eu_806667D0)
 extern const float lbl_eu_806667D0;
// radians -> FIdx conversion factor (lbl_eu_806667D4)
 extern const float lbl_eu_806667D4;
// s32->f32 conversion magic double (0x4330000080000000), referenced by the
// builtin (float) cast in func_800A3B24. MWCC pools that magic as a TU-local
// @N label while retail references the shared .sdata2 blob, so the reloc name
// drifts (MWCC_CASES §7i); the extern documents the retail symbol and is
// the anchor a manual bit-pattern conversion would subtract (union + extern
// emits the correct reloc name but the double fsub/fmul instead of the
// builtin's fsubs, so the builtin is preferred here).
extern double lbl_eu_806667E0;
// random-angle offset table (lbl_eu_80572AA0); element 0 is never written
 extern float lbl_eu_80572AA0[4];
// 2.0f (lbl_eu_806667EC) and -2.0f (lbl_eu_806667F0), spline basis weights
 extern const float lbl_eu_806667EC;
 extern const float lbl_eu_806667F0;
// 0.3f (lbl_eu_80666804) - debug sphere alpha
 extern const float lbl_eu_80666804;
// double 2.0 (lbl_eu_80666808) - sphere arc multiplier
 extern const double lbl_eu_80666808;
// Reference directions used by the facing-matrix builder: (0,1,0) up and
// (1,0,1); each lazily initialized once (.data).
 extern ml::CVec3 lbl_eu_80572AB0;
 extern ml::CVec3 lbl_eu_80572ABC;
 extern s8 lbl_eu_80663E92;
 extern s8 lbl_eu_80663E93;
// Scene/view globals consumed by the debug-draw setup helper.
class CScn;
class CView;
 extern CScn* lbl_eu_80663E14;
 extern CView* lbl_eu_80663E10;
// random-angle table index / one-time init flag (.sbss bytes)
 extern u8 lbl_eu_80663E90;
 extern s8 lbl_eu_80663E91;
// capsule-axis constants for the collision probe helper (func_800A7094)
 extern const float lbl_eu_80666814;
 extern const float lbl_eu_80666818;
 extern const float lbl_eu_8066681C;
 extern const float lbl_eu_80666820;
// 0.25f (lbl_eu_806667FC) - sphere radius offset
 extern const float lbl_eu_806667FC;
// -1.0f (lbl_eu_80666800) - box face-direction constant
 extern const float lbl_eu_80666800;
// Box-face offset table {1.0f, 1.7f, 0.1f, 0.0f} (split1 .data) - Y raise
// offsets probed by the segment-vs-capsule helper func_800A4C48.
 extern float lbl_eu_80528178[4];

// Collision-probe object behind lbl_eu_80665958; only the +0x60..+0x68
// height-reference vector is read here.
struct ColiProbeObj {
    u8 field_0x00[0x60]; // 0x00
    float field_0x60;    // 0x60
    float field_0x64;    // 0x64
    float field_0x68;    // 0x68
};

// Global collision-probe object pointer (retail .sbss).
extern ColiProbeObj* lbl_eu_80665958;

// Segment ground-probe helper (monolib scn unit). Declared with five
// parameters: the retail call site in func_800A7094 only sets r3-r6 and f1
// (r4 = the second pointer argument), and the callee reads only the four GPR
// arguments (the trailing float is ignored).
extern "C" int func_804BE398(ml::CVec3* vec, ml::CVec3* a, int b, int c, f32 d, f32 e);
// Debug-draw setup: binds a CDrawGX to the scene + view (r5 = view, 0 = current).
extern "C" void func_8049034C(void* a, void* b, CView* c);
// Copy the 12-byte header / info block of scene-resource entry [index].
extern "C" void func_804BE4B4(ml::CVec3* dst, int index);
extern "C" void func_804BE4E0(ml::CVec3* dst, int index);
// Segment-query walker (monolib coli unit).
extern "C" int func_804B5088(ColiProbeObj* self, ml::CVec3* a, ml::CVec3* b,
                             int filter, int isFirst);

namespace cf {
class CfDebugDrawManager {
public:
    void renderCylinder();
    void renderCapsule();
    ml::CVec3 mPos;  // 0x00 - sphere/capsule center
};
} // namespace cf

// Collision object vtable interface used by func_800A50AC: position getter at
// vtable slot +0xAC (cf. CfCollCircleVTableIf) plus a reference vector at +0x54.
struct ColObjIf {
    virtual void _v008();
    virtual void _v00C();
    virtual void _v010();
    virtual void _v014();
    virtual void _v018();
    virtual void _v01C();
    virtual void _v020();
    virtual void _v024();
    virtual void _v028();
    virtual void _v02C();
    virtual void _v030();
    virtual void _v034();
    virtual void _v038();
    virtual void _v03C();
    virtual void _v040();
    virtual void _v044();
    virtual void _v048();
    virtual void _v04C();
    virtual void _v050();
    virtual void _v054();
    virtual void _v058();
    virtual void _v05C();
    virtual void _v060();
    virtual void _v064();
    virtual void _v068();
    virtual void _v06C();
    virtual void _v070();
    virtual void _v074();
    virtual void _v078();
    virtual void _v07C();
    virtual void _v080();
    virtual void _v084();
    virtual void _v088();
    virtual void _v08C();
    virtual void _v090();
    virtual void _v094();
    virtual void _v098();
    virtual void _v09C();
    virtual void _v0A0();
    virtual void _v0A4();
    virtual void _v0A8();
    virtual ml::CVec3* _v0AC();  // position getter (slot 0xAC/4)
    virtual void _v0B0();
    u8 field_0x04[0x50];  // 0x04
    ml::CVec3 mRef;       // 0x54
};

void func_800A3B24(ml::CVec3* out, int seed) {
    // Lazily fill the random-angle offset table (element 0 stays untouched).
    if (lbl_eu_80663E91 == 0) {
        lbl_eu_80572AA0[1] = lbl_eu_8066A1F8;
        lbl_eu_80572AA0[2] = lbl_eu_8066A200;
        lbl_eu_80572AA0[3] = lbl_eu_806667D0 * lbl_eu_8066A200;
        lbl_eu_80663E91 = 1;
    }
    ml::MTRand* rand = ml::MTRand::getInstance();
    // Random angle in [0, 8066A200) plus one of the four table offsets.
    float angle = lbl_eu_8066A200 * rand->randFloat() + lbl_eu_80572AA0[lbl_eu_80663E90];
    float rnd = rand->randFloat1();
    float fseed = (float)seed;
    lbl_eu_80663E90 = (lbl_eu_80663E90 + 1) & 3;
    // Radius scaled by the caller's seed; the point lies on the XZ plane.
    float radius = fseed * rnd;
    out->x = radius * nw4r::math::CosFIdx(lbl_eu_806667D4 * angle);
    out->z = radius * nw4r::math::SinFIdx(lbl_eu_806667D4 * angle);
    out->y = lbl_eu_806667D8;
}

ml::CVec3* func_800A3C48(ml::CVec3* v) {
    // Normalize v in place and return it. If v is (near-)zero it is left as-is;
    // if the squared length is exactly zero it is set to ml::CVec3::zero,
    // otherwise PSVECNormalize runs in place.
    bool result = false;
    bool temp = false;
    if (ml::math::abs(v->x) <= lbl_eu_8066A208 && ml::math::abs(v->y) <= lbl_eu_8066A208) {
        temp = true;
    }
    if (temp && ml::math::abs(v->z) <= lbl_eu_8066A208) {
        result = true;
    }
    if (!result) {
        if (v->x * v->x + v->y * v->y + v->z * v->z == lbl_eu_806667D8) {
            *v = ml::CVec3::zero;
        } else {
            PSVECNormalize(*v, *v);
        }
    }
    return v;
}

float func_800A3D34(ml::CVec3* v) {
    // Normalize v in place; returns the magnitude (0.0f if v is already
    // (near-)zero). Matches ml::CVec3::isZero + PSVECMag + VEC3Scale.
    bool result = false;
    bool temp = false;
    if (ml::math::abs(v->x) <= lbl_eu_8066A208 && ml::math::abs(v->y) <= lbl_eu_8066A208) {
        temp = true;
    }
    if (temp && ml::math::abs(v->z) <= lbl_eu_8066A208) {
        result = true;
    }
    if (result) {
        return lbl_eu_806667D8;
    }
    float mag = PSVECMag((const Vec*)v);
    float inv = lbl_eu_806667E8 / mag;
    nw4r::math::VEC3Scale(*v, *v, inv);
    return mag;
}

float func_800A3DF8(const ml::CVec3& v) {
    // XZ-plane length sqrt: len2 = x*x + z*z, with the nw4r FSqrt warning path
    // (negative len2) and a 0 return for (near-)zero vectors.
    bool temp = false;
    bool result = false;
    if (ml::math::abs(v.x) <= lbl_eu_8066A208 && ml::math::abs(v.y) <= lbl_eu_8066A208) {
        temp = true;
    }
    if (temp && ml::math::abs(v.z) <= lbl_eu_8066A208) {
        result = true;
    }
    if (result) return lbl_eu_806667D8;
    float len2 = v.x * v.x + v.z * v.z;
    if (!(len2 >= lbl_eu_806667D8)) {
        nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
    }
    return len2 <= lbl_eu_806667D8 ? lbl_eu_806667D8 : len2 * nw4r::math::FrSqrt(len2);
}

float func_800A3EF4(float x) {
    // Sqrt for positive inputs; warns on negatives (nw4r FSqrt assert) and
    // returns 0 for non-positive or near-zero inputs.
    if (ml::math::abs(x) > lbl_eu_8066A208) {
        if (!(x >= lbl_eu_806667D8)) {
            nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
        }
        return x <= lbl_eu_806667D8 ? lbl_eu_806667D8 : x * nw4r::math::FrSqrt(x);
    }
    return lbl_eu_806667D8;
}

void func_800A3F8C(ml::CVec3* v) {
    // Wrap each component into (-pi, pi] using the pi / 2*pi constants.
    float x = v->x;
    while (lbl_eu_8066A1F8 <= x) x -= lbl_eu_8066A1FC;
    while (x < -lbl_eu_8066A1F8) x += lbl_eu_8066A1FC;
    v->x = x;
    float y = v->y;
    while (lbl_eu_8066A1F8 <= y) y -= lbl_eu_8066A1FC;
    while (y < -lbl_eu_8066A1F8) y += lbl_eu_8066A1FC;
    v->y = y;
    float z = v->z;
    while (lbl_eu_8066A1F8 <= z) z -= lbl_eu_8066A1FC;
    while (z < -lbl_eu_8066A1F8) z += lbl_eu_8066A1FC;
    v->z = z;
}

bool func_800A4050(const ml::CVec3& a, const ml::CVec3& b, const ml::CVec3& c) {
    // True when the Y component of (b-a) x (c-a) is positive.
    ml::CVec3 ab;
    ml::CVec3 ac;
    ml::CVec3::sub(ab, b, a);
    ml::CVec3::sub(ac, c, a);
    return ab.z * ac.x - ab.x * ac.z > lbl_eu_806667D8;
}

void func_800A40E8(ml::CVec3* out, const ml::CVec3& a, const ml::CVec3& b, float t) {
    // Linear interpolation: out = a + (b - a) * t.
    // The nw4r VEC3Sub/Scale/Add PS kernels emit the retail psq_l/ps_sub/
    // ps_muls0/ps_add sequences; each result is copied out via set(), and the
    // final *out = res is a struct copy (lwz/stw). Declaration order is
    // reverse-stack (first declared gets the highest slot).
    ml::CVec3 res;
    ml::CVec3 scaled;
    ml::CVec3 diff;
    ml::CVec3 t1;
    ml::CVec3 t2;
    ml::CVec3 t3;
    nw4r::math::VEC3Sub(t1, b, a);
    diff.set(t1);
    nw4r::math::VEC3Scale(t2, diff, t);
    scaled.set(t2);
    nw4r::math::VEC3Add(t3, a, scaled);
    res.set(t3);
    *out = res;
}

void func_800A41BC(ml::CVec3* out, const ml::CVec3& p0, const ml::CVec3& p1,
                  const ml::CVec3& p2, const ml::CVec3& p3, float t) {
    // Cubic Hermite-ish spline segment evaluation: out = P1*H1 + (P2-P0)*0.5*H2
    // + (P3-P1)*0.5*H4 + P2*(-H3) with H1..H4 the Hermite basis.
    float t2 = t * t;
    float t3 = t2 * t;
    float c = lbl_eu_806667D0 * t2;                 // 3t^2
    float f2 = t3 - t2;                              // t^3 - t^2
    float e = t3 - lbl_eu_806667EC * t2;             // t^3 - 2t^2
    float g = lbl_eu_806667EC * t3 - c;              // 2t^3 - 3t^2
    float f13 = t + e;                               // t^3 - 2t^2 + t
    float f12 = lbl_eu_806667E8 + g;                 // 2t^3 - 3t^2 + 1
    float f1 = lbl_eu_806667F0 * t3 + c;             // 3t^2 - 2t^3

    ml::CVec3 h1b;
    ml::CVec3 h2b;
    ml::CVec3 h1;
    ml::CVec3 d1;
    ml::CVec3 h2;
    ml::CVec3 d2;
    ml::CVec3 sum3;
    ml::CVec3 sum2;
    ml::CVec3 sum1;
    ml::CVec3 scaled1;
    ml::CVec3 scaled2;
    ml::CVec3 scaled3;
    ml::CVec3 scaled4;
    ml::CVec3 tA;
    ml::CVec3 tC;
    ml::CVec3 tB;
    ml::CVec3 tD;
    ml::CVec3 tE;
    ml::CVec3 tF;
    ml::CVec3 tI;
    ml::CVec3 tG;
    ml::CVec3 tJ;
    ml::CVec3 tH;
    ml::CVec3 tK;
    nw4r::math::VEC3Sub(tA, p2, p0);
    d1.set(tA);
    nw4r::math::VEC3Sub(tB, p3, p1);
    d2.set(tB);
    nw4r::math::VEC3Scale(tC, d1, lbl_eu_806667F4);
    h1.set(tC);
    h1b = h1;
    nw4r::math::VEC3Scale(tD, d2, lbl_eu_806667F4);
    h2.set(tD);
    h2b = h2;
    nw4r::math::VEC3Scale(tE, p1, f12);
    scaled1.set(tE);
    nw4r::math::VEC3Scale(tF, h1b, f13);
    scaled2.set(tF);
    nw4r::math::VEC3Scale(tG, h2b, f2);
    scaled3.set(tG);
    nw4r::math::VEC3Scale(tH, p2, f1);
    scaled4.set(tH);
    nw4r::math::VEC3Add(tI, scaled1, scaled2);
    sum1.set(tI);
    nw4r::math::VEC3Add(tJ, sum1, scaled3);
    sum2.set(tJ);
    nw4r::math::VEC3Add(tK, sum2, scaled4);
    sum3.set(tK);
    *out = sum3;
}

extern "C" void func_800A44CC(ml::CMat34* out, const ml::CVec3* src, const ml::CVec3* trans) {
    // Build a facing matrix whose Z axis is the (normalized) src direction:
    // X/Y bases are derived from cross products with a reference direction.
    ml::CVec3 xAxis;
    ml::CVec3 axis;
    ml::CVec3 norm;
    ml::CVec3 copyA;
    ml::CVec3 copyB;
    ml::CVec3 copyC;
    ml::CVec3 copyD;
    norm.set(*src);
    bool result = false;
    bool temp = false;
    if (ml::math::abs(norm.x) <= lbl_eu_8066A208 && ml::math::abs(norm.y) <= lbl_eu_8066A208) {
        temp = true;
    }
    if (temp && ml::math::abs(norm.z) <= lbl_eu_8066A208) {
        result = true;
    }
    if (!result) {
        if (norm.x * norm.x + norm.y * norm.y + norm.z * norm.z == lbl_eu_806667D8) {
            norm = ml::CVec3::zero;
        } else {
            PSVECNormalize(norm, norm);
        }
    }

    // Lazily initialize the reference directions (0,1,0) and (1,0,1).
    if (lbl_eu_80663E92 == 0) {
        lbl_eu_80572AB0.x = lbl_eu_806667D8;
        lbl_eu_80572AB0.y = lbl_eu_806667E8;
        lbl_eu_80572AB0.z = lbl_eu_806667D8;
        lbl_eu_80663E92 = 1;
    }
    if (lbl_eu_80663E93 == 0) {
        lbl_eu_80572ABC.x = lbl_eu_806667E8;
        lbl_eu_80572ABC.y = lbl_eu_806667D8;
        lbl_eu_80572ABC.z = lbl_eu_806667E8;
        lbl_eu_80663E93 = 1;
    }

    if (ml::math::abs(lbl_eu_806667E8 - ml::math::abs(norm.y)) <= lbl_eu_8066A208) {
        // Near-vertical direction: derive from (1, 0, 1).
        ml::CVec3::cross(copyA, norm, lbl_eu_80572ABC);
        axis = copyA;
        ml::CVec3::cross(copyB, axis, norm);
        xAxis = copyB;
    } else {
        // Use world up (0, 1, 0).
        ml::CVec3::cross(copyC, lbl_eu_80572AB0, norm);
        xAxis = copyC;
        ml::CVec3::cross(copyD, norm, xAxis);
        axis = copyD;
    }

    out->m[0][0] = xAxis.x;
    out->m[1][0] = xAxis.y;
    out->m[2][0] = xAxis.z;
    out->m[0][1] = axis.x;
    out->m[1][1] = axis.y;
    out->m[2][1] = axis.z;
    out->m[0][2] = norm.x;
    out->m[1][2] = norm.y;
    out->m[2][2] = norm.z;
    out->m[0][3] = trans->x;
    out->m[1][3] = trans->y;
    out->m[2][3] = trans->z;
}

extern "C" float func_800A47C8(const ml::CVec3& a, const ml::CVec3& b, const ml::CVec3& c,
                                float* outT, ml::CVec3* out) {
    // Closest-point-on-segment helper: returns the squared distance from c to
    // the segment ab, writing the closest point and its parameter value.
    // NOTE: retail prologue hoists the segment loads before the frame setup.
    ml::CVec3 ab, ac, cb;
    ml::CVec3 sum, scaled;
    ml::CVec3 t1, t2, t3, t4, t5;
    nw4r::math::VEC3Sub(t1, b, a);
    ab.set(t1);
    nw4r::math::VEC3Sub(t2, c, a);
    ac.set(t2);
    nw4r::math::VEC3Sub(t3, c, b);
    cb.set(t3);
    float dot = nw4r::math::VEC3Dot(ac, ab);
    if (dot <= lbl_eu_806667D8) {
        *out = a;
        *outT = lbl_eu_806667D8;
        return nw4r::math::VEC3Dot(ac, ac);
    }
    float lenAbSq = nw4r::math::VEC3Dot(ab, ab);
    if (dot >= lenAbSq) {
        *out = b;
        *outT = lbl_eu_806667E8;
        return nw4r::math::VEC3Dot(cb, cb);
    }
    float t = dot / lenAbSq;
    *outT = t;
    nw4r::math::VEC3Scale(t4, ab, t);
    scaled.set(t4);
    nw4r::math::VEC3Add(t5, a, scaled);
    sum.set(t5);
    *out = sum;
    return nw4r::math::VEC3Dot(ac, ac) - dot * *outT;
}

bool func_800A49E4(const nw4r::math::VEC3& a, const nw4r::math::VEC3& b,
                  const nw4r::math::VEC3& c, float r2) {
    // Project c onto the XZ line through a-b and test whether the closest
    // point lies within the squared radius r2 (all vectors flattened to y=0).
    nw4r::math::VEC3 av, bv, cv;
    av.x = a.x;
    av.y = lbl_eu_806667D8;
    av.z = a.z;
    bv.x = b.x;
    bv.y = lbl_eu_806667D8;
    bv.z = b.z;
    cv.x = c.x;
    cv.y = lbl_eu_806667D8;
    cv.z = c.z;
    nw4r::math::VEC3 ab, ac, cb;
    nw4r::math::VEC3 t1, t2, t3;
    nw4r::math::VEC3Sub(&t1, &bv, &av);
    ab = t1;
    nw4r::math::VEC3Sub(&t2, &cv, &av);
    ac = t2;
    nw4r::math::VEC3Sub(&t3, &cv, &bv);
    cb = t3;
    float dot = nw4r::math::VEC3Dot(&ac, &ab);
    if (dot < lbl_eu_806667D8) return false;
    float len2 = nw4r::math::VEC3Dot(&ab, &ab);
    if (dot > len2) return false;
    float t = dot / len2;
    return nw4r::math::VEC3Dot(&ac, &ac) - dot * t <= r2;
}

float func_800A4B5C(const ml::CVec3& a, const ml::CVec3& b, const ml::CVec3& c) {
    // Project c onto the line ab: t = dot(a->b, a->c) / |ab|^2, guarded when
    // |ab|^2 is tiny, then clamped to [0, 1]. VEC3Sub/VEC3Dot are the nw4r PS
    // kernels; each sub result is copied out via set(). Declaration order is
    // reverse-stack (first declared gets the highest slot).
    ml::CVec3 ab;
    ml::CVec3 ac;
    ml::CVec3 t1;
    ml::CVec3 t2;
    nw4r::math::VEC3Sub(t1, b, a);
    ab.set(t1);
    nw4r::math::VEC3Sub(t2, c, a);
    ac.set(t2);
    float len2 = nw4r::math::VEC3Dot(ab, ab);
    float dot = nw4r::math::VEC3Dot(ac, ab);
    float t;
    if (len2 < lbl_eu_806667F8) {
        t = lbl_eu_806667D8;
    } else {
        t = dot / len2;
    }
    if (t < lbl_eu_806667D8) return lbl_eu_806667D8;
    if (t > lbl_eu_806667E8) return lbl_eu_806667E8;
    return t;
}

int func_800A4C48(ColObjIf* self, const ml::CVec3& a, const ml::CVec3& b, ml::CVec3* out,
                  float radius) {
    // Segment-vs-sphere probe: raise the object position's Y by each of three
    // table offsets (1.0/1.7/0.1) and test the closest point on segment ab
    // against the sphere radius; on a hit push the closest point out to the
    // sphere surface and write it to out. Returns whether any offset hit.
    ml::CVec3 d;       // raised center (0x78)
    ml::CVec3 closest; // closest point on ab from func_800A47C8 (0x6c)
    ml::CVec3 dir;     // d - closest (0x60)
    ml::CVec3 d2;      // pos - mRef (0x54)
    ml::CVec3 d3;      // d2 - closest (0x48)
    ml::CVec3 outTmp;  // 0x3c
    ml::CVec3 dirTmp;  // 0x30
    ml::CVec3 d2Tmp;   // 0x24
    ml::CVec3 d3Tmp;   // 0x18
    ml::CVec3 tAdd;    // 0xc
    d.set(*self->_v0AC());
    if (out != 0) *out = d;
    float R = lbl_eu_806667FC + radius;
    float R2 = R * R;
    float baseY = d.y;
    bool cond = false;
    float t;
    for (u32 i = 0; i < 3; i++) {
        d.y = baseY + lbl_eu_80528178[i];
        float dist2 = func_800A47C8(a, b, d, &t, &closest);
        cond = dist2 <= R2;
        if (cond) break;
    }
    if (cond && out != 0) {
        nw4r::math::VEC3Sub(dirTmp, d, closest);
        dir.set(dirTmp);
        bool zero = false;
        bool zeroTmp = false;
        if (ml::math::abs(dir.x) <= lbl_eu_8066A208 && ml::math::abs(dir.y) <= lbl_eu_8066A208) {
            zeroTmp = true;
        }
        if (zeroTmp && ml::math::abs(dir.z) <= lbl_eu_8066A208) {
            zero = true;
        }
        if (zero) {
            // Coincident centers: retry from the object's reference point.
            ml::CVec3* pos = self->_v0AC();
            nw4r::math::VEC3Sub(d2Tmp, *pos, self->mRef);
            d2.set(d2Tmp);
            nw4r::math::VEC3Sub(d3Tmp, d2, closest);
            d3.set(d3Tmp);
            dir = d3;
            bool zero2 = false;
            bool zero2Tmp = false;
            if (ml::math::abs(dir.x) <= lbl_eu_8066A208 && ml::math::abs(dir.y) <= lbl_eu_8066A208) {
                zero2Tmp = true;
            }
            if (zero2Tmp && ml::math::abs(dir.z) <= lbl_eu_8066A208) {
                zero2 = true;
            }
            if (zero2) return 1;
        }
        // Normalize the direction (exact-zero shortcut, then PSVECNormalize).
        if (dir.x * dir.x + dir.y * dir.y + dir.z * dir.z == lbl_eu_806667D8) {
            dir = ml::CVec3::zero;
        } else {
            PSVECNormalize(dir, dir);
        }
        float R2b = lbl_eu_806667FC + radius;
        d.x = dir.x * R2b;
        d.z = dir.z * R2b;
        d.y = self->_v0AC()->y - closest.y;
        nw4r::math::VEC3Add(tAdd, d, closest);
        outTmp.set(tAdd);
        *out = outTmp;
    }
    return cond;
}

bool func_800A5038(const nw4r::math::VEC3& a, const nw4r::math::VEC3& b, float r1, float r2) {
    // Circle-vs-circle overlap test on the XZ plane: true when the squared
    // distance between a and b (y treated as 0) is within (r1+r2)^2.
    nw4r::math::VEC3 dv;
    nw4r::math::VEC3 d;
    nw4r::math::VEC3Sub(&d, &a, &b);
    dv.x = d.x;
    dv.y = lbl_eu_806667D8;
    dv.z = d.z;
    float dist2 = nw4r::math::VEC3LenSq(&dv);
    return dist2 <= (r1 + r2) * (r1 + r2);
}

int func_800A50AC(ColObjIf* self, ml::CVec3* point, float radius, ml::CVec3* out) {
    // Radius test against the object's position (raised 1 unit), pushing the
    // point out to the sphere boundary when it is inside and out is given.
    ml::CVec3 d;
    ml::CVec3 dir;
    ml::CVec3 d2;
    ml::CVec3 d3;
    ml::CVec3 outTmp;
    ml::CVec3 dvTmp;
    ml::CVec3 dv;
    ml::CVec3 dirTmp;
    ml::CVec3 d2Tmp;
    ml::CVec3 d3Tmp;
    ml::CVec3 tAdd;
    ml::CVec3* pos = self->_v0AC();
    d.set(*pos);
    if (out != 0) *out = d;
    d.y += lbl_eu_806667E8;
    nw4r::math::VEC3Sub(dvTmp, d, *point);
    dv.x = dvTmp.x;
    dv.y = lbl_eu_806667D8;  // XZ-plane distance
    dv.z = dvTmp.z;
    float R = lbl_eu_806667FC + radius;
    bool cond = nw4r::math::VEC3LenSq(dv) <= R * R;
    if (cond && out != 0) {
        nw4r::math::VEC3Sub(dirTmp, d, *point);
        dir.set(dirTmp);
        dir.y = lbl_eu_806667D8;  // XZ-plane direction
        bool zero = false;
        bool zeroTmp = false;
        if (ml::math::abs(dir.x) <= lbl_eu_8066A208 && ml::math::abs(dir.y) <= lbl_eu_8066A208) {
            zeroTmp = true;
        }
        if (zeroTmp && ml::math::abs(dir.z) <= lbl_eu_8066A208) {
            zero = true;
        }
        if (zero) {
            // Coincident centers: retry from the object's reference point.
            pos = self->_v0AC();
            nw4r::math::VEC3Sub(d2Tmp, *pos, self->mRef);
            d2.set(d2Tmp);
            nw4r::math::VEC3Sub(d3Tmp, d2, *point);
            d3.set(d3Tmp);
            dir = d3;
            dir.y = lbl_eu_806667D8;
            bool zero2 = false;
            bool zero2Tmp = false;
            if (ml::math::abs(dir.x) <= lbl_eu_8066A208 && ml::math::abs(dir.y) <= lbl_eu_8066A208) {
                zero2Tmp = true;
            }
            if (zero2Tmp && ml::math::abs(dir.z) <= lbl_eu_8066A208) {
                zero2 = true;
            }
            if (zero2) return 1;
        }
        // Normalize the XZ direction (exact-zero shortcut, then PSVECNormalize).
        if (dir.x * dir.x + dir.y * dir.y + dir.z * dir.z == lbl_eu_806667D8) {
            dir = ml::CVec3::zero;
        } else {
            PSVECNormalize(dir, dir);
        }
        float R2 = lbl_eu_806667FC + radius;
        d.x = dir.x * R2;
        d.z = dir.z * R2;
        pos = self->_v0AC();
        d.y = pos->y - point->y;
        nw4r::math::VEC3Add(tAdd, d, *point);
        outTmp.set(tAdd);
        *out = outTmp;
    }
    return cond;
}

bool func_800A5488(const ml::CVec3& a, const ml::CVec3& b, ml::CVec3* out, float r1, float r2) {
    // Sphere-vs-sphere (XZ) push-apart helper. Returns whether the two circles
    // overlap; when they do and out is non-null, out = a pushed to the far
    // side of b along the (normalised) centre-to-centre direction.
    if (out != 0) *out = a;
    ml::CVec3 dir;         // centre-to-centre direction (a - b)
    nw4r::math::VEC3 dTmp; // d = b - a sub temp
    nw4r::math::VEC3 d;    // b - a
    nw4r::math::VEC3 dirTmp;
    nw4r::math::VEC3Sub(&dTmp, b, a);
    d = dTmp;
    float len2 = nw4r::math::VEC3Dot(&d, &d);
    bool overlap = len2 <= (r2 + r1) * (r2 + r1);
    if (overlap) {
        if (out != 0) {
            nw4r::math::VEC3Sub(&dirTmp, a, b);
            dir.x = dirTmp.x;
            dir.y = dirTmp.y;
            dir.z = dirTmp.z;
            // isZero() inlined (reads the retail epsilon label):
            bool zero = false;
            bool zeroTmp = false;
            if (ml::math::abs(dir.x) <= lbl_eu_8066A208 && ml::math::abs(dir.y) <= lbl_eu_8066A208) {
                zeroTmp = true;
            }
            if (zeroTmp && ml::math::abs(dir.z) <= lbl_eu_8066A208) {
                zero = true;
            }
            if (zero) {
                // Randomise the direction when the centres coincide.
                ml::MTRand* rand = ml::MTRand::getInstance();
                dir.x = rand->randFloat1() - lbl_eu_806667F4;
                dir.z = rand->randFloat1() - lbl_eu_806667F4;
            }
            // Re-check after randomisation:
            zero = false;
            zeroTmp = false;
            if (ml::math::abs(dir.x) <= lbl_eu_8066A208 && ml::math::abs(dir.y) <= lbl_eu_8066A208) {
                zeroTmp = true;
            }
            if (zeroTmp && ml::math::abs(dir.z) <= lbl_eu_8066A208) {
                zero = true;
            }
            if (!zero) {
                // normalizeSub() inlined: exact-zero check, then PSVECNormalize.
                if (dir.x * dir.x + dir.y * dir.y + dir.z * dir.z == lbl_eu_806667D8) {
                    dir.setZero();
                } else {
                    PSVECNormalize(dir, dir);
                }
            }
            out->x = dir.x * (r1 + r2) + b.x;
            out->z = dir.z * (r1 + r2) + b.z;
            out->y = a.y;
        }
    }
    return overlap;
}

int func_800A5738(ColObjIf* self, ml::CVec3* point, float radius, ml::CVec3* out) {
    // Push point out to the surface of the object's collision sphere (center
    // d = pos + (0,1,0), radius 0.25 + radius) when it is inside, writing the
    // pushed position to out. Returns whether the point was inside the sphere.
    // Full-3D variant of func_800A50AC (no XZ flattening).
    ml::CVec3 d;
    ml::CVec3 dir;
    ml::CVec3 d2;
    ml::CVec3 d3;
    ml::CVec3 outTmp;
    nw4r::math::VEC3 dvTmp;
    nw4r::math::VEC3 dv;
    ml::CVec3 dirTmp;
    ml::CVec3 d2Tmp;
    ml::CVec3 d3Tmp;
    ml::CVec3 tAdd;
    d.set(*self->_v0AC());
    if (out != 0) *out = d;
    d.y += lbl_eu_806667E8;
    nw4r::math::VEC3Sub(&dvTmp, *point, d);
    dv = dvTmp;
    bool cond = nw4r::math::VEC3Dot(&dv, &dv) <= (radius + lbl_eu_806667FC) * (radius + lbl_eu_806667FC);
    if (cond && out != 0) {
        nw4r::math::VEC3Sub(dirTmp, d, *point);
        dir.set(dirTmp);
        bool zero = false;
        bool zeroTmp = false;
        if (ml::math::abs(dir.x) <= lbl_eu_8066A208 && ml::math::abs(dir.y) <= lbl_eu_8066A208) {
            zeroTmp = true;
        }
        if (zeroTmp && ml::math::abs(dir.z) <= lbl_eu_8066A208) {
            zero = true;
        }
        if (zero) {
            // Coincident centers: retry from the object's reference point.
            nw4r::math::VEC3Sub(d2Tmp, *self->_v0AC(), self->mRef);
            d2.set(d2Tmp);
            nw4r::math::VEC3Sub(d3Tmp, d2, *point);
            d3.set(d3Tmp);
            dir = d3;
            bool zero2 = false;
            bool zero2Tmp = false;
            if (ml::math::abs(dir.x) <= lbl_eu_8066A208 && ml::math::abs(dir.y) <= lbl_eu_8066A208) {
                zero2Tmp = true;
            }
            if (zero2Tmp && ml::math::abs(dir.z) <= lbl_eu_8066A208) {
                zero2 = true;
            }
            if (zero2) return 1;
        }
        // Normalize the direction (exact-zero shortcut, then PSVECNormalize).
        if (dir.x * dir.x + dir.y * dir.y + dir.z * dir.z == lbl_eu_806667D8) {
            dir = ml::CVec3::zero;
        } else {
            PSVECNormalize(dir, dir);
        }
        float R2 = lbl_eu_806667FC + radius;
        d.x = dir.x * R2;
        d.z = dir.z * R2;
        d.y = self->_v0AC()->y - point->y;
        nw4r::math::VEC3Add(tAdd, d, *point);
        outTmp.set(tAdd);
        *out = outTmp;
    }
    return cond;
}

int func_800A5B18(ColObjIf* self, const ml::CVec3& point, ml::CVec3* out, float radius,
                  float h) {
    // Cylinder test: true when the raised center (pos + (0,1,0)) is within the
    // horizontal radius of point's XZ position AND point's Y is within h of the
    // raised center. On a hit the point is pushed out of the cylinder side and
    // written to out.
    bool cond2;
    bool cond1;
    ml::CVec3 d;
    ml::CVec3 dir;
    ml::CVec3 d2;
    ml::CVec3 d3;
    ml::CVec3 outTmp;
    ml::CVec3 dvTmp;
    ml::CVec3 dv;
    ml::CVec3 dirTmp;
    ml::CVec3 d2Tmp;
    ml::CVec3 d3Tmp;
    ml::CVec3 tAdd;
    d.set(*self->_v0AC());
    if (out != 0) *out = d;
    d.y += lbl_eu_806667E8;
    nw4r::math::VEC3Sub(dvTmp, d, point);
    dv.x = dvTmp.x;
    dv.y = lbl_eu_806667D8;  // XZ-plane distance
    dv.z = dvTmp.z;
    float R = lbl_eu_806667FC + radius;
    cond1 = nw4r::math::VEC3LenSq(dv) <= R * R;
    cond2 = point.y <= d.y && d.y <= point.y + h;
    if (cond1) {
        if (cond2 && out != 0) {
            if (cond1) {
            nw4r::math::VEC3Sub(dirTmp, d, point);
            dir.x = dirTmp.x;
            dir.y = lbl_eu_806667D8;  // XZ-plane direction
            dir.z = dirTmp.z;
            bool zero = false;
            bool zeroTmp = false;
            if (ml::math::abs(dir.x) <= lbl_eu_8066A208 && ml::math::abs(dir.y) <= lbl_eu_8066A208) {
                zeroTmp = true;
            }
            if (zeroTmp && ml::math::abs(dir.z) <= lbl_eu_8066A208) {
                zero = true;
            }
            if (zero) {
                // Coincident centers: retry from the object's reference point.
                ml::CVec3* pos = self->_v0AC();
                nw4r::math::VEC3Sub(d2Tmp, *pos, self->mRef);
                d2.set(d2Tmp);
                nw4r::math::VEC3Sub(d3Tmp, d2, point);
                d3.set(d3Tmp);
                dir = d3;
                dir.y = lbl_eu_806667D8;
                bool zero2 = false;
                bool zero2Tmp = false;
                if (ml::math::abs(dir.x) <= lbl_eu_8066A208 && ml::math::abs(dir.y) <= lbl_eu_8066A208) {
                    zero2Tmp = true;
                }
                if (zero2Tmp && ml::math::abs(dir.z) <= lbl_eu_8066A208) {
                    zero2 = true;
                }
                if (zero2) return 1;
            }
            // Normalize the XZ direction (exact-zero shortcut, then PSVECNormalize).
            if (dir.x * dir.x + dir.y * dir.y + dir.z * dir.z == lbl_eu_806667D8) {
                dir = ml::CVec3::zero;
            } else {
                PSVECNormalize(dir, dir);
            }
            float R2 = lbl_eu_806667FC + radius;
            d.x = dir.x * R2;
            d.z = dir.z * R2;
            d.y = self->_v0AC()->y - point.y;
            }
            nw4r::math::VEC3Add(tAdd, d, point);
            outTmp.set(tAdd);
            *out = outTmp;
        }
    }
    return cond1 && cond2;
}

extern "C" void func_800A5F54(const ml::CVec3* src, const ml::CVec3* min, const ml::CVec3* max, ml::CVec3* dst) {
    // Clamp a copy of src component-wise into the [min, max] AABB.
    *dst = *src;
    if (dst->x < min->x) {
        dst->x = min->x;
    } else if (dst->x > max->x) {
        dst->x = max->x;
    }
    if (dst->y < min->y) {
        dst->y = min->y;
    } else if (dst->y > max->y) {
        dst->y = max->y;
    }
    if (dst->z < min->z) {
        dst->z = min->z;
    } else if (dst->z > max->z) {
        dst->z = max->z;
    }
}

int func_800A5FE8(ColObjIf* self, const ml::CVec3& a, const ml::CVec3& b, const Mtx mat,
                  ml::CVec3* out) {
    // AABB test: the raised center d = pos + (0,1,0) (optionally transformed by
    // mat) is clamped into the box [a-0.5, b+0.5]; if the clamp moved it, the
    // point was inside and is pushed out onto the face plane facing the object's
    // reference direction (chosen by which face the reference direction
    // exceeds), then written to out with its Y replaced by the object's Y.
    ml::CVec3 clamped; // 0x100 - clamp result
    ml::CVec3 d;       // 0xf4 - raised center
    ml::CVec3 min;     // 0xe8
    ml::CVec3 max;     // 0xdc
    ml::CVec3 v0;      // 0xd0 - face normal
    ml::CVec3 v1;      // 0xc4 - face point (min/max corner)
    ml::CVec3 dir;     // 0xb8 - pos - mRef
    float dot;         // 0xb4
    ml::CVec3 tmp;     // 0xa8 - v0 copy
    ml::CVec3 outTmp;  // 0x5c
    ml::CVec3 tA;      // 0x50 - PSMTXMultVec result
    ml::CVec3 tB;      // 0x44 - distance diff temp
    ml::CVec3 diff;    // 0x38 - clamped - d
    ml::CVec3 dirTmp;  // 0x2c - pos - mRef sub result
    ml::CVec3 diff2;   // 0x20 - clamped - proj
    ml::CVec3 tC;      // 0x14 - VEC3Scale result
    ml::CVec3 proj;    // 0x8 - scaled direction
    d.set(*self->_v0AC());
    if (out != 0) *out = d;
    min.x = a.x - lbl_eu_806667F4;
    min.y = a.y - lbl_eu_806667F4;
    min.z = a.z - lbl_eu_806667F4;
    max.x = b.x + lbl_eu_806667F4;
    max.y = b.y + lbl_eu_806667F4;
    max.z = b.z + lbl_eu_806667F4;
    d.y += lbl_eu_806667E8;
    if (mat != 0) {
        PSMTXMultVec(mat, d, tA);
        d = tA;
    }
    func_800A5F54(&d, &min, &max, &clamped);
    // Inside the box iff the clamp left d unchanged (squared distance 0).
    nw4r::math::VEC3Sub(tB, clamped, d);
    diff.x = tB.x;
    diff.y = tB.y;
    diff.z = tB.z;
    float R = lbl_eu_806667D8;
    bool cond = nw4r::math::VEC3Dot(diff, diff) <= R * R;
    if (cond && out != 0) {
        nw4r::math::VEC3Sub(dirTmp, *self->_v0AC(), self->mRef);
        dir.x = dirTmp.x;
        dir.y = dirTmp.y;
        dir.z = dirTmp.z;
        // Pick the face the reference direction points at.
        if (dir.z <= min.z) {
            v0 = ml::CVec3(lbl_eu_806667D8, lbl_eu_806667D8, lbl_eu_80666800);
            v1 = min;
        } else if (dir.z >= max.z) {
            v0 = ml::CVec3(lbl_eu_806667D8, lbl_eu_806667D8, lbl_eu_806667E8);
            v1 = max;
        } else if (dir.x <= min.x) {
            v0 = ml::CVec3(lbl_eu_80666800, lbl_eu_806667D8, lbl_eu_806667D8);
            v1 = min;
        } else if (dir.x >= max.x) {
            v0 = ml::CVec3(lbl_eu_806667E8, lbl_eu_806667D8, lbl_eu_806667D8);
            v1 = max;
        } else {
            return cond;
        }
        // Project clamped onto the plane v0.x = v0.v1.
        dot = nw4r::math::VEC3Dot(v0, v1);
        tmp = v0;
        float t = nw4r::math::VEC3Dot(tmp, clamped) - dot;
        nw4r::math::VEC3Scale(tC, tmp, t);
        proj.set(tC);
        nw4r::math::VEC3Sub(diff2, clamped, proj);
        outTmp.set(diff2);
        *out = outTmp;
        out->y = self->_v0AC()->y;
    }
    return cond;
}

extern "C" void renderSphere__Q22cf18CfDebugDrawManagerFv(cf::CfDebugDrawManager* self, float radius) {
    CDrawGX draw;
    func_8049034C(lbl_eu_80663E14, &draw, lbl_eu_80663E10);

    ml::CCol4 col;
    col.r = lbl_eu_806667D8;
    col.g = lbl_eu_806667E8;
    col.b = lbl_eu_806667E8;
    col.a = lbl_eu_80666804;
    draw.setCol(col);

    ml::CMat34 mat;
    mat.m[0][0] = lbl_eu_806667E8;
    mat.m[0][1] = lbl_eu_806667D8;
    mat.m[0][2] = lbl_eu_806667D8;
    mat.m[0][3] = self->mPos.x;
    mat.m[1][0] = lbl_eu_806667D8;
    mat.m[1][1] = lbl_eu_806667E8;
    mat.m[1][2] = lbl_eu_806667D8;
    mat.m[1][3] = self->mPos.y;
    mat.m[2][0] = lbl_eu_806667D8;
    mat.m[2][1] = lbl_eu_806667D8;
    mat.m[2][2] = lbl_eu_806667E8;
    mat.m[2][3] = self->mPos.z;
    draw.setMatrix(mat);

    // Two nested loops of triangle-strip vertices (8x9 grid on the sphere).
    draw.begin(6, 0x90);
    double twoPi = lbl_eu_80666808 * lbl_eu_8066A1F8;
    for (int i = 0; i < 8; i++) {
        float a1 = (float)(twoPi * (double)i / 8);
        float s1 = nw4r::math::SinFIdx(lbl_eu_806667D4 * a1);
        float c1 = nw4r::math::CosFIdx(lbl_eu_806667D4 * a1);
        float r1 = radius * s1;
        float rc1 = radius * c1;
        int i1 = i + 1;
        float a2 = (float)(twoPi * (double)i1 / 8);
        float s2 = nw4r::math::SinFIdx(lbl_eu_806667D4 * a2);
        float c2 = nw4r::math::CosFIdx(lbl_eu_806667D4 * a2);
        float r2 = radius * s2;
        float rc2 = radius * c2;
        for (int j = 0; j <= 8; j++) {
            float a3 = (float)(twoPi * (double)j / 8);
            float s3 = nw4r::math::SinFIdx(lbl_eu_806667D4 * a3);
            float c3 = nw4r::math::CosFIdx(lbl_eu_806667D4 * a3);
            ml::CVec3 v1(rc1 * c3, rc1 * s3, r1);
            draw.add(v1);
            ml::CVec3 v2(rc2 * c3, rc2 * s3, r2);
            draw.add(v2);
        }
    }
    draw.end();
}

extern "C" void renderCylinder__Q22cf18CfDebugDrawManagerFb(cf::CfDebugDrawManager* self, const void* vec, const void* arg3, int mode);

void cf::CfDebugDrawManager::renderCylinder() {
    const void* vec;   // forwarded dangling r4 (retail: li r6,0; b Fb)
    const void* arg3;  // forwarded dangling r5
    renderCylinder__Q22cf18CfDebugDrawManagerFb(this, vec, arg3, 0);
}

int func_800A7094(ml::CVec3* a, ml::CVec3* b, ml::CVec3* c, float f, float g) {
    // Push b up along the +Y capsule axis until the segment probe against c
    // misses, snapping to the probe object's height reference when it does.
    *b = *a;
    b->y += lbl_eu_8066681C;
    ml::CVec3 tmp74;   // scratch for the resource-info probe (never read)
    ml::CVec3 tmp68(lbl_eu_806667D8, lbl_eu_80666818, lbl_eu_806667D8);
    ml::CVec3 tmp5c;   // probe-object height reference vector
    ml::CVec3 tmp50;   // b + tmp44
    ml::CVec3 tmp44;   // downward offset (0, -f, 0)
    ml::CVec3 tAdd;
    ml::CVec3 d2;      // b - tmp5c
    ml::CVec3 d2c;
    ml::CVec3 d1;      // b - tmp68
    ml::CVec3 d1c;
    int result = func_804BE398(b, c, 0, 0, -f, g);
    if (result != 0) {
        func_804BE4B4(&tmp68, 0);
        func_804BE4E0(&tmp74, 0);
    }
    if (result != 0) {
        tmp44.set(lbl_eu_806667D8, -f, lbl_eu_806667D8);
        nw4r::math::VEC3Add(tAdd, *b, tmp44);
        tmp50.set(tAdd);
        if (func_804B5088(lbl_eu_80665958, b, &tmp50, 1, 0) != 0) {
            tmp5c.set(lbl_eu_80665958->field_0x60, lbl_eu_80665958->field_0x64,
                      lbl_eu_80665958->field_0x68);
            nw4r::math::VEC3Sub(d1, *b, tmp68);
            d1c.set(d1);
            nw4r::math::VEC3Sub(d2, *b, tmp5c);
            d2c.set(d2);
            float lenObj = nw4r::math::VEC3Dot(d2c, d2c);
            float lenAxis = nw4r::math::VEC3Dot(d1c, d1c);
            if (lenObj >= lenAxis) {
                b->y = tmp68.z;
            } else {
                b->y = lbl_eu_80666820 + lbl_eu_80665958->field_0x64;
            }
            return 0;
        }
        b->y = tmp68.z;
        return 1;
    }
    b->y = a->y;
    return 0;
}

int func_800A72E0(ml::CVec3* self, ml::CVec3* arg2, ml::CVec3* arg3, float f1, float f2) {
    // Axis-aligned capsule/segment ground probe (X-axis variant of
    // func_800A7094): raise arg2, probe the scene twice, then pick the closer
    // reference height and walk the segment against the probe object.
    *arg2 = *self;
    arg2->y += lbl_eu_8066681C;
    ml::CVec3 tmp80;
    ml::CVec3 tmp74(lbl_eu_806667D8, lbl_eu_80666814, lbl_eu_806667D8);
    ml::CVec3 tmp68(lbl_eu_806667D8, lbl_eu_80666818, lbl_eu_806667D8);
    ml::CVec3 tmp5c;
    ml::CVec3 tmp50;
    ml::CVec3 tmp44;
    ml::CVec3 tAdd;
    ml::CVec3 d2;
    ml::CVec3 d2c;
    ml::CVec3 d1;
    ml::CVec3 d1c;
    int result1 = func_804BE398(arg2, arg3, 0, 0, f1, f2);
    if (result1 != 0) {
        func_804BE4B4(&tmp74, 0);
        func_804BE4E0(&tmp80, 0);
    }
    int result2 = func_804BE398(arg2, arg3, 0, 0, -f1, f2);
    if (result2 != 0) {
        func_804BE4B4(&tmp68, 0);
        func_804BE4E0(&tmp80, 0);
    }
    if (ml::math::abs(tmp74.y - self->y) < ml::math::abs(tmp68.y - self->y) && result1 != 0) {
        tmp68 = tmp74;
    }
    if (result2 != 0 || result1 != 0) {
        tmp44.set(lbl_eu_806667D8, -f1, lbl_eu_806667D8);
        nw4r::math::VEC3Add(tAdd, *arg2, tmp44);
        tmp50.set(tAdd);
        if (func_804B5088(lbl_eu_80665958, arg2, &tmp50, 1, 0) != 0) {
            tmp5c.set(lbl_eu_80665958->field_0x60, lbl_eu_80665958->field_0x64,
                      lbl_eu_80665958->field_0x68);
            nw4r::math::VEC3Sub(d1, *arg2, tmp68);
            d1c.set(d1);
            nw4r::math::VEC3Sub(d2, *arg2, tmp5c);
            d2c.set(d2);
            float lenObj = nw4r::math::VEC3Dot(d2c, d2c);
            float lenAxis = nw4r::math::VEC3Dot(d1c, d1c);
            if (lenObj >= lenAxis) {
                arg2->y = tmp68.y;
            } else {
                arg2->y = lbl_eu_80666820 + lbl_eu_80665958->field_0x64;
            }
            return 0;
        }
        arg2->y = tmp68.y;
        return 1;
    }
    arg2->y = self->y;
    return 0;
}

#pragma push
#pragma auto_inline off
extern "C" void renderCylinder__Q22cf18CfDebugDrawManagerFb(cf::CfDebugDrawManager* self, const void* vec, const void* arg3, int mode) {}
#pragma pop

void cf::CfDebugDrawManager::renderCapsule() {
    const void* vec;   // forwarded dangling r4 (retail: li r6,1; b Fb)
    const void* arg3;  // forwarded dangling r5
    renderCylinder__Q22cf18CfDebugDrawManagerFb(this, vec, arg3, 1);
}
