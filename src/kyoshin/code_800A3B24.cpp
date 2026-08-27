// Auto-scaffolded catalog TU for kyoshin/code_800A3B24
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "monolib/math/CVec3.hpp"
#include "monolib/math/MTRand.hpp"
#include "monolib/core/CDrawGX.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)
#include "monolib/math/FloatUtils.hpp"  // H3 label-owner decl (lbl_eu_8066A208)

// Retail SDA2/data constants referenced by the PS math helpers below. Declared
// by retail label so the emitted @sda21 relocs match retail byte-for-byte; the
// values themselves live in the retail binary's .sdata2/.data sections.
extern const float lbl_eu_806667D8;  // 0.0f
// 1.0f (lbl_eu_806667E8)
extern const float lbl_eu_806667E8;
// ml::epsilon (lbl_eu_8066A208)
// pi (lbl_eu_8066A1F8)
extern const float lbl_eu_8066A1F8;
// 2*pi (lbl_eu_8066A1FC)
extern const float lbl_eu_8066A1FC;
// nw4r FSqrt assert source-file string + message (retail .data)
extern const char lbl_eu_80526324[];
extern const char lbl_eu_80526300[];
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
// Radius scale applied by renderCylinder in non-capsule mode (lbl_eu_80666810)
 extern const float lbl_eu_80666810;

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
extern "C" int func_804BE398(void* vec, int a, int b, int c, f32 d, f32 e);
// Debug-draw setup: binds a CDrawGX to the scene + view (r5 = view, 0 = current).
extern "C" void func_8049034C(void* a, void* b, CView* c);
// Copy the 12-byte header / info block of scene-resource entry [index].
extern "C" void func_804BE4B4(void* dst, int index);
extern "C" void func_804BE4E0(void* dst, int index);
// Segment-query walker (monolib coli unit).
extern "C" int func_804B5088(ColiProbeObj* self, ml::CVec3* a, ml::CVec3* b,
                             int filter, int isFirst);

// Closest-point-on-segment helper: retail exports this with C linkage
// (unmangled symbol), so the definition below must be extern "C" too or the
// call-site reloc name drifts from retail.
extern "C" float func_800A47C8(const ml::CVec3& a, const ml::CVec3& b,
                              const ml::CVec3& c, float* outT, ml::CVec3* out);

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
    ml::CVec3 ab = b - a;
    ml::CVec3 ac = c - a;
    return ab.z * ac.x - ab.x * ac.z > lbl_eu_806667D8;
}

void func_800A40E8(ml::CVec3* out, const ml::CVec3& a, const ml::CVec3& b, float t) {
    // Linear interpolation: out = a + (b - a) * t.
    // The nw4r VEC3Sub/Scale/Add PS kernels emit the retail psq_l/ps_sub/
    // ps_muls0/ps_add sequences; each result is copied out via set(), and the
    // final *out = res is a struct copy (lwz/stw).
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
    // Catmull-Rom spline segment evaluation via the Hermite basis:
    // out = p1*b00 + (p2-p0)*h*b10 + (p3-p1)*h*b11 + p2*b01.
    // Each helper takes references, so every op materializes a kernel temp
    // plus a set() copy - matching the retail copy traffic.
    float t2 = t * t;
    float t3 = t2 * t;
    float c = lbl_eu_806667D0 * t2;                 // 3t^2
    float h10 = t3 - lbl_eu_806667EC * t2;           // t^3 - 2t^2
    float h00 = lbl_eu_806667EC * t3 - c;            // 2t^3 - 3t^2
    float b01 = lbl_eu_806667F0 * t3 + c;            // 3t^2 - 2t^3
    float b11 = t3 - t2;                             // t^3 - t^2
    float b10 = t + h10;                             // t^3 - 2t^2 + t
    float b00 = lbl_eu_806667E8 + h00;               // 2t^3 - 3t^2 + 1

    ml::CVec3 d1;
    ml::CVec3::sub(d1, p2, p0);
    ml::CVec3 m1;
    ml::CVec3::scale(m1, d1, lbl_eu_806667F4);
    ml::CVec3 m1b = m1;
    ml::CVec3 d2;
    ml::CVec3::sub(d2, p3, p1);
    ml::CVec3 m2;
    ml::CVec3::scale(m2, d2, lbl_eu_806667F4);
    ml::CVec3 m2b = m2;

    ml::CVec3 s1;
    ml::CVec3::scale(s1, p1, b00);
    ml::CVec3 s2;
    ml::CVec3::scale(s2, m1b, b10);
    ml::CVec3 s3;
    ml::CVec3::scale(s3, m2b, b11);
    ml::CVec3 s4;
    ml::CVec3::scale(s4, p2, b01);
    ml::CVec3 sum12;
    ml::CVec3::add(sum12, s1, s2);
    ml::CVec3 sum123;
    ml::CVec3::add(sum123, sum12, s3);
    ml::CVec3 res;
    ml::CVec3::add(res, sum123, s4);
    *out = res;
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

extern "C" float func_800A47C8(const ml::CVec3& a, const ml::CVec3& b,
                               const ml::CVec3& c, float* outT, ml::CVec3* out) {
    // Closest-point-on-segment helper: returns the squared distance from c to
    // the segment ab, writing the closest point and its parameter value.
    // Retail keeps every PS kernel inlined; each sub/scale/add result is
    // copied out component-wise (cf. func_800A49E4).
    ml::CVec3 ab;
    ml::CVec3 ac;
    ml::CVec3 cb;
    ml::CVec3 sum;
    ml::CVec3 scaled;
    ml::CVec3 tAb;
    ml::CVec3 tAc;
    ml::CVec3 tCb;
    ml::CVec3 tScaled;
    ml::CVec3 tSum;
    nw4r::math::VEC3Sub((nw4r::math::VEC3*)&tAb, (const nw4r::math::VEC3*)&b,
                        (const nw4r::math::VEC3*)&a);
    nw4r::math::VEC3Sub((nw4r::math::VEC3*)&tAc, (const nw4r::math::VEC3*)&c,
                        (const nw4r::math::VEC3*)&a);
    nw4r::math::VEC3Sub((nw4r::math::VEC3*)&tCb, (const nw4r::math::VEC3*)&c,
                        (const nw4r::math::VEC3*)&b);
    ab.x = tAb.x;
    ab.y = tAb.y;
    ab.z = tAb.z;
    ac.x = tAc.x;
    ac.y = tAc.y;
    ac.z = tAc.z;
    cb.x = tCb.x;
    cb.y = tCb.y;
    cb.z = tCb.z;
    float dot = nw4r::math::VEC3Dot((const nw4r::math::VEC3*)&ac,
                                    (const nw4r::math::VEC3*)&ab);
    if (dot <= lbl_eu_806667D8) {
        *out = a;
        *outT = lbl_eu_806667D8;
        return nw4r::math::VEC3Dot((const nw4r::math::VEC3*)&ac,
                                   (const nw4r::math::VEC3*)&ac);
    }
    float lenAbSq = nw4r::math::VEC3Dot((const nw4r::math::VEC3*)&ab,
                                        (const nw4r::math::VEC3*)&ab);
    if (dot >= lenAbSq) {
        *out = b;
        *outT = lbl_eu_806667E8;
        return nw4r::math::VEC3Dot((const nw4r::math::VEC3*)&cb,
                                   (const nw4r::math::VEC3*)&cb);
    }
    float t = dot / lenAbSq;
    *outT = t;
    nw4r::math::VEC3Scale((nw4r::math::VEC3*)&tScaled,
                          (const nw4r::math::VEC3*)&ab, t);
    scaled.x = tScaled.x;
    scaled.y = tScaled.y;
    scaled.z = tScaled.z;
    nw4r::math::VEC3Add((nw4r::math::VEC3*)&tSum, (const nw4r::math::VEC3*)&a,
                        (const nw4r::math::VEC3*)&scaled);
    sum.x = tSum.x;
    sum.y = tSum.y;
    sum.z = tSum.z;
    *out = sum;
    return nw4r::math::VEC3Dot((const nw4r::math::VEC3*)&ac,
                               (const nw4r::math::VEC3*)&ac) -
           dot * t;
}

bool func_800A49E4(const nw4r::math::VEC3& a, const nw4r::math::VEC3& b,
                  const nw4r::math::VEC3& c, float r2) {
    float dot, len2, t;
    // Project c onto the XZ line through a-b and test whether the closest
    // point lies within the squared radius r2 (all vectors flattened to y=0).
    // flat[0]/[1]/[2] hold the y-flattened copies of a/b/c.
    nw4r::math::VEC3 flat[3];
    flat[0].x = a.x;
    flat[0].y = lbl_eu_806667D8;
    flat[0].z = a.z;
    flat[1].x = b.x;
    flat[1].y = lbl_eu_806667D8;
    flat[1].z = b.z;
    flat[2].x = c.x;
    flat[2].y = lbl_eu_806667D8;
    flat[2].z = c.z;
    nw4r::math::VEC3 ab, ac, cb;
    nw4r::math::VEC3 t1, t2, t3;
    nw4r::math::VEC3Sub(&t1, &flat[1], &flat[0]);
    ab.x = t1.x;
    ab.y = t1.y;
    ab.z = t1.z;
    nw4r::math::VEC3Sub(&t2, &flat[2], &flat[0]);
    ac.x = t2.x;
    ac.y = t2.y;
    ac.z = t2.z;
    nw4r::math::VEC3Sub(&t3, &flat[2], &flat[1]);
    cb.x = t3.x;
    cb.y = t3.y;
    cb.z = t3.z;
    dot = nw4r::math::VEC3Dot(&ac, &ab);
    if (dot < lbl_eu_806667D8) return false;
    len2 = nw4r::math::VEC3Dot(&ab, &ab);
    if (dot > len2) return false;
    t = dot / len2;
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
            nw4r::math::VEC3Sub(d2Tmp, *self->_v0AC(), self->mRef);
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
    // Whole-struct copy keeps every field of d live (retail stores the full
    // pair + z); the y overwrite then replaces the copied value, leaving
    // dv = {dx, 0, dz} for the XZ-plane squared-distance check.
    dv = d;
    dv.y = lbl_eu_806667D8;
    float dist2 = nw4r::math::VEC3LenSq(&dv);
    return dist2 <= (r1 + r2) * (r1 + r2);
}

int func_800A50AC(ColObjIf* self, nw4r::math::VEC3* point, float radius, ml::CVec3* out) {
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
    float R = lbl_eu_806667FC + radius;
    d.y += lbl_eu_806667E8;
    dv.y = lbl_eu_806667D8;  // XZ-plane distance
    nw4r::math::VEC3Sub(dvTmp, d, point);
    dv.x = dvTmp.x;
    dv.z = dvTmp.z;
    bool cond = nw4r::math::VEC3LenSq(dv) <= R * R;
    if (cond && out != 0) {
        nw4r::math::VEC3Sub(dirTmp, d, point);
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
            nw4r::math::VEC3Sub(d2Tmp, *self->_v0AC(), self->mRef);
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
        d.y = self->_v0AC()->y - point->y;
        nw4r::math::VEC3Add(tAdd, d, point);
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
    ml::CVec3 dTmp;    // d = b - a sub temp
    ml::CVec3 d;       // b - a
    ml::CVec3 dir;     // centre-to-centre direction (a - b)
    ml::CVec3 dirTmp;
    nw4r::math::VEC3Sub(dTmp, b, a);
    d.set(dTmp);
    float len2 = ml::CVec3::dot(d, d);
    bool overlap = len2 <= (r2 + r1) * (r2 + r1);
    if (overlap) {
        if (out != 0) {
            ml::CVec3::sub(dir, a, b);
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
                    dir = ml::CVec3::zero;
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
    // Declaration order controls MWCC stack-slot allocation (reverse order).
    ml::CVec3 d;
    {
        // Scoped so the vcall result's live range ends before the body.
        ml::CVec3* pos = self->_v0AC();
        d.x = pos->x;
        d.y = pos->y;
        d.z = pos->z;
    }
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
    if (out != 0) *out = d;
    // Retail computes the squared test radius before the subtraction.
    float R = radius + lbl_eu_806667FC;
    d.y += lbl_eu_806667E8;
    nw4r::math::VEC3Sub(&dvTmp, *point, d);
    dv = dvTmp;
    bool cond = nw4r::math::VEC3Dot(&dv, &dv) <= R * R;
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

    // Inline temporary: the setCol argument setup interleaves between the
    // constant loads exactly like retail.
    draw.setCol(ml::CCol4(lbl_eu_806667D8, lbl_eu_806667E8, lbl_eu_806667E8, lbl_eu_80666804));

    // Position floats read into locals so the constant stores schedule first;
    // retail fetches E8/D8 before the three position loads.
    float px = self->mPos.x;
    float py = self->mPos.y;
    float pz = self->mPos.z;
    ml::CMat34 mat(
        lbl_eu_806667E8, lbl_eu_806667D8, lbl_eu_806667D8, px,
        lbl_eu_806667D8, lbl_eu_806667E8, lbl_eu_806667D8, py,
        lbl_eu_806667D8, lbl_eu_806667D8, lbl_eu_806667E8, pz);
    draw.setMatrix(mat);

    // Two nested loops of triangle-strip vertices (8x9 grid on the sphere).
    draw.begin(6, 0x90);
    // Sphere grid: two stacked rings (i, i+1) x 9 segment vertices; every
    // angle is recomputed naively as (2*pi*n)/8 in double, then converted to
    // FIdx degrees for the trig kernels - retail does no CSE here.
    double twoPi = lbl_eu_80666808 * lbl_eu_8066A1F8;
    // Segment count as a variable: MWCC const-propagates the value (li 8)
    // but keeps the int->double conversion intrinsic runtime-expanded, which
    // is exactly what retail emits per angle.
    int seg = 8;
    for (int i = 0; i < 8; i++) {
        float r1 = radius * nw4r::math::SinFIdx(lbl_eu_806667D4 * (float)(twoPi * (double)i / seg));
        float rc1 = radius * nw4r::math::CosFIdx(lbl_eu_806667D4 * (float)(twoPi * (double)i / seg));
        float r2 = radius * nw4r::math::SinFIdx(lbl_eu_806667D4 * (float)(twoPi * (double)(i + 1) / seg));
        float rc2 = radius * nw4r::math::CosFIdx(lbl_eu_806667D4 * (float)(twoPi * (double)(i + 1) / seg));
        for (int j = 0; j <= 8; j++) {
            float s3 = nw4r::math::SinFIdx(lbl_eu_806667D4 * (float)(twoPi * (double)j / seg));
            float c3 = nw4r::math::CosFIdx(lbl_eu_806667D4 * (float)(twoPi * (double)j / seg));
            // Multiply order mirrors retail: y1, x1, x2, y2.
            float y1 = rc1 * s3;
            float x1 = rc1 * c3;
            float x2 = rc2 * c3;
            float y2 = rc2 * s3;
            draw.add(ml::CVec3(x1, y1, r1));
            draw.add(ml::CVec3(x2, y2, r2));
        }
    }
    draw.end();
}

extern "C" void renderCylinder__Q22cf18CfDebugDrawManagerFb(cf::CfDebugDrawManager* self, const ml::CVec3* vec, const ml::CCol4* col, bool mode, float radius = 1.0f);

void cf::CfDebugDrawManager::renderCylinder() {
    const ml::CVec3* vec;   // forwarded dangling r4 (retail: li r6,0; b Fb)
    const ml::CCol4* arg3;  // forwarded dangling r5
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
    int result = func_804BE398(b, (int)(uintptr_t)c, 0, 0, -f, g);
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

int func_800A72E0(const ml::CVec3* self, ml::CVec3* arg2, ml::CVec3* arg3, float f1, float f2) {
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
    int result1 = func_804BE398(arg2, (int)(uintptr_t)arg3, 0, 0, f1, f2);
    if (result1 != 0) {
        func_804BE4B4(&tmp74, 0);
        func_804BE4E0(&tmp80, 0);
    }
    int result2 = func_804BE398(arg2, (int)(uintptr_t)arg3, 0, 0, -f1, f2);
    if (result2 != 0) {
        func_804BE4B4(&tmp68, 0);
        func_804BE4E0(&tmp80, 0);
    }
    if (ml::math::abs(tmp74.y - self->y) < ml::math::abs(tmp68.y - self->y) && result1 != 0) {
        tmp68 = tmp74;
    }
    // Single-exit form: retail materialises the default result (1) before
    // testing the probe flags and writes 0 at each failing site.
    int ret = 1;
    if (result2 != 0 || result1 != 0) {
        tmp44.set(lbl_eu_806667D8, -f1, lbl_eu_806667D8);
        nw4r::math::VEC3Add(tAdd, *arg2, tmp44);
        tmp50.set(tAdd);
        if (func_804B5088(lbl_eu_80665958, arg2, &tmp50, 1, 0) != 0) {
            tmp5c.set(lbl_eu_80665958->field_0x60, lbl_eu_80665958->field_0x64,
                      lbl_eu_80665958->field_0x68);
            // Both PS subtraction kernels stay adjacent so the scheduler can
            // interleave their paired-single loads; the component copies then
            // batch after (retail order).
            nw4r::math::VEC3Sub(d1, *arg2, tmp68);
            nw4r::math::VEC3Sub(d2, *arg2, tmp5c);
            d1c.set(d1);
            d2c.set(d2);
            float lenObj = nw4r::math::VEC3Dot(d2c, d2c);
            float lenAxis = nw4r::math::VEC3Dot(d1c, d1c);
            if (lenObj >= lenAxis) {
                arg2->y = tmp68.y;
            } else {
                arg2->y = lbl_eu_80666820 + lbl_eu_80665958->field_0x64;
            }
            ret = 0;
        } else {
            arg2->y = tmp68.y;
        }
    } else {
        arg2->y = self->y;
        ret = 0;
    }
    return ret;
}

#pragma push
#pragma auto_inline off
// Real body for the shared cylinder/capsule debug-draw routine. Retail passes
// the axis end point in r4, the draw color in r5, the capsule flag in r6 and
// the radius in f1; the default argument keeps the 0x8 tail-jump wrappers from
// materializing f1 (they forward a dangling radius like retail does).
//
// Twelve transformed-point buffers are declared at function scope: MWCC gives
// each draw.add site its own stack slot (retail frame layout), so they must
// never be merged into block-scoped temporaries.
extern "C" void renderCylinder__Q22cf18CfDebugDrawManagerFb(
        cf::CfDebugDrawManager* self, const ml::CVec3* endPt, const ml::CCol4* col,
        bool mode, float radius) {
    ml::CVec3 p01, p02, p03, p04, p05, p06;
    ml::CVec3 p07, p08, p09, p10, p11, p12;
    // Axis direction: end - start via operator- (inlined VEC3Sub paired-single
    // kernel into a temp, then component copy into dir).
    ml::CVec3 dir = *endPt - self->mPos;
    float len = PSVECMag(dir);

    // Facing matrix along the axis, translated to the cylinder base.
    ml::CMat34 mat;
    func_800A44CC(&mat, &dir, &self->mPos);

    if (mode == false) {
        radius = radius * lbl_eu_80666810;
    }

    CDrawGX draw;
    func_8049034C(lbl_eu_80663E14, &draw, lbl_eu_80663E10);
    draw.setCol(*col);

    // Cylinder wall drawn as two 18-vertex triangle strips. Every angle is
    // recomputed naively in double exactly like retail - no CSE between the
    // CosFIdx/SinFIdx calls.
    double twoPi = lbl_eu_80666808 * lbl_eu_8066A1F8;
    int seg = 8;
    draw.begin(6, 0x12);
    for (int i = 0; i <= seg; i++) {
        float c = radius * nw4r::math::CosFIdx(lbl_eu_806667D4 * (float)(twoPi * (double)i / seg));
        float s = radius * nw4r::math::SinFIdx(lbl_eu_806667D4 * (float)(twoPi * (double)i / seg));
        mat.mul(p01, ml::CVec3(c, s, lbl_eu_806667D8));
        draw.add(ml::CVec3(p01));
        mat.mul(p02, ml::CVec3(c, s, len));
        draw.add(ml::CVec3(p02));
    }
    draw.end();
    draw.begin(6, 0x12);
    for (int i = 0; i <= seg; i++) {
        float c = radius * nw4r::math::CosFIdx(lbl_eu_806667D4 * (float)(twoPi * (double)i / seg));
        float s = radius * nw4r::math::SinFIdx(lbl_eu_806667D4 * (float)(twoPi * (double)i / seg));
        mat.mul(p03, ml::CVec3(c, s, len));
        draw.add(ml::CVec3(p03));
        mat.mul(p04, ml::CVec3(c, s, lbl_eu_806667D8));
        draw.add(ml::CVec3(p04));
    }
    draw.end();

    if (mode != false) {
        // Capsule: negative hemisphere cap center then its rim ring.
        draw.begin(7, 0xa);
        mat.mul(p05, ml::CVec3(lbl_eu_806667D8, lbl_eu_806667D8, -radius));
        draw.add(ml::CVec3(p05));
        for (int i = 0; i <= seg; i++) {
            float c = radius * nw4r::math::CosFIdx(lbl_eu_806667D4 * (float)(twoPi * (double)i / seg));
            float s = radius * nw4r::math::SinFIdx(lbl_eu_806667D4 * (float)(twoPi * (double)i / seg));
            mat.mul(p06, ml::CVec3(c, s, lbl_eu_806667D8));
            draw.add(ml::CVec3(p06));
        }
        draw.end();

        // Positive cap: apex at len+radius, rim ring walked with descending
        // angles (seg - i) like retail.
        draw.begin(7, 0xa);
        mat.mul(p07, ml::CVec3(lbl_eu_806667D8, lbl_eu_806667D8, len + radius));
        draw.add(ml::CVec3(p07));
        for (int i = 0; i <= seg; i++) {
            float c = radius * nw4r::math::CosFIdx(lbl_eu_806667D4 * (float)(twoPi * (double)(seg - i) / seg));
            float s = radius * nw4r::math::SinFIdx(lbl_eu_806667D4 * (float)(twoPi * (double)(seg - i) / seg));
            mat.mul(p08, ml::CVec3(c, s, len));
            draw.add(ml::CVec3(p08));
        }
        draw.end();
    } else {
        // Plain cylinder caps: flat disc center + rim ring at each end.
        draw.begin(7, 0xa);
        mat.mul(p09, ml::CVec3(lbl_eu_806667D8, lbl_eu_806667D8, lbl_eu_806667D8));
        draw.add(ml::CVec3(p09));
        for (int i = 0; i <= seg; i++) {
            float c = radius * nw4r::math::CosFIdx(lbl_eu_806667D4 * (float)(twoPi * (double)i / seg));
            float s = radius * nw4r::math::SinFIdx(lbl_eu_806667D4 * (float)(twoPi * (double)i / seg));
            mat.mul(p10, ml::CVec3(c, s, lbl_eu_806667D8));
            draw.add(ml::CVec3(p10));
        }
        draw.end();

        draw.begin(7, 0xa);
        mat.mul(p11, ml::CVec3(lbl_eu_806667D8, lbl_eu_806667D8, len));
        draw.add(ml::CVec3(p11));
        for (int i = 0; i <= seg; i++) {
            float c = radius * nw4r::math::CosFIdx(lbl_eu_806667D4 * (float)(twoPi * (double)(seg - i) / seg));
            float s = radius * nw4r::math::SinFIdx(lbl_eu_806667D4 * (float)(twoPi * (double)(seg - i) / seg));
            mat.mul(p12, ml::CVec3(c, s, len));
            draw.add(ml::CVec3(p12));
        }
        draw.end();
    }
}
#pragma pop

void cf::CfDebugDrawManager::renderCapsule() {
    const ml::CVec3* vec;   // forwarded dangling r4 (retail: li r6,1; b Fb)
    const ml::CCol4* arg3;  // forwarded dangling r5
    renderCylinder__Q22cf18CfDebugDrawManagerFb(this, vec, arg3, 1);
}
