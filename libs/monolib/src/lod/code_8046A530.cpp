// Translation unit for LOD::UnkClass_8046A530.
// Full layout information is in UnkClass_8046A530.hpp.

#include <harness_catalog.h>
#include <PowerPC_EABI_Support/MSL_C/MSL_Common/rand.h>
#include <revolution/MTX.h>
#include <revolution/mtx/vec.h>
#include "monolib/lod/LODMemMan.hpp"   // provides full LOD::LODMemMan definition
#include "monolib/core/code_804E36DC.hpp"  // declares __dl__FPv (C ABI)
#include "monolib/math/CVec3.hpp"      // ml::CVec3 + nw4r math inlines (VEC3*, FrSqrt, Warning)

// .sdata2 constants referenced by this TU (retail labels).  Declared at
// global scope so MWCC emits the names unmangled.
extern const f32 lbl_eu_8066A658;    // 0.0f
extern const f32 lbl_eu_8066A65C;    // 1.0f
extern const f32 lbl_eu_8066A660;    // 0.01f
extern const f32 lbl_eu_8066A664;    // 0.5f
extern const f32 lbl_eu_8066A668;    // 0.3f
extern const f32 lbl_eu_8066A66C;    // 0.1f
extern const f32 lbl_eu_8066A670;    // 0.6f
extern const f32 lbl_eu_8066A674;    // 10.0f
extern const double lbl_eu_8066A678; // 2^52 + 2^31 (int->float conversion trick)
extern const f32 lbl_eu_8066A680;    // func_8046AADC default x/z
extern const f32 lbl_eu_8066A684;    // 1000.0f (rand() % 1000 divisor)
extern const f32 lbl_eu_8066A688;    // 20.0f   (rand() % 20 divisor)
extern const f32 lbl_eu_8066A68C;    // 0.4f
extern const f32 lbl_eu_8066A690;    // 0.11f
extern const f32 lbl_eu_8066A694;    // 0.002f
extern const f32 lbl_eu_8066A698;    // 1/30
extern const f32 lbl_eu_8066A69C;    // 0.0001f
extern const f32 lbl_eu_8066A6A0;    // 0.02f
extern const f32 lbl_eu_8066A6A4;    // 0.03f
extern const f32 lbl_eu_8066A6A8;    // 10000.0f
extern const f32 lbl_eu_8066A6AC;    // 0.13f
extern const f32 lbl_eu_8066A6B0;    // -0.3f
extern const f32 lbl_eu_8066A6B4;    // -0.9999f
extern const f32 lbl_eu_8066A6B8;    // 2.0f
extern const f32 lbl_eu_8066A6BC;    // -1.0f
// .sdata constants.
extern const f32 lbl_eu_80663818;    // 0.005f
extern const f32 lbl_eu_8066381C;    // 0.005f

// Warning() file / format rodata strings (nw4r db assert).
extern const char lbl_eu_80526324[];
extern const char lbl_eu_80526300[];

// Local mirror of the UnkClass_8046A530 field layout for the offsets used by
// this TU.  The shared header only exposes opaque padding up to 0xa44, so the
// recovered fields are declared here and the object is viewed through this
// struct instead of raw pointer arithmetic.
struct LODLayout_8046A530 {
    u32 field_0x00;                 // 0x000  flag word (func_8046CFB4)
    u8  mPad_04[0xa44 - 0x04];      // 0x004..0xa43
    u8  mPad_A44[0x1200 - 0xa44];   // 0xa44..0x11ff
    f32 field_0x1200;               // 0x1200
    f32 field_0x1204;               // 0x1204
    f32 field_0x1208;               // 0x1208
    f32 field_0x120C;               // 0x120c
    f32 field_0x1210;               // 0x1210
    f32 field_0x1214;               // 0x1214
    f32 field_0x1218;               // 0x1218
    u32 field_0x121C;               // 0x121c
};

/**
 * One 0xb4-byte LOD animation element.  24 elements live back-to-back at
 * owner offset 0x120.  The seven CVec3s are: v[0] velocity, v[1] position,
 * v[2] walk direction, v[3..6] spring/orientation vectors; mtxA/mtxB are the
 * per-frame orientation matrices.
 */
struct LODElement_8046A530 {
    ml::CVec3 v[7];     // 0x00..0x48
    Mtx mtxA;           // 0x54  orientation matrix A
    Mtx mtxB;           // 0x84  orientation matrix B
};

// Helper: access the LODMemMan embedded at offset 0xa44.
// Uses one-past-array addressing (well-defined C++) to avoid raw
// pointer arithmetic in callers.
LOD::LODMemMan& LOD::UnkClass_8046A530::GetMemMan() {
    return *static_cast<LOD::LODMemMan*>(
        static_cast<void*>(&mPad_00 + 1));
}

/**
 * Virtual thunk: adjust 'this' to point to the LODMemMan sub-object
 * embedded at offset 0xa44, then forward to LODMemMan::func_80471834().
 *
 * Equivalent to:
 *   addi r3, r3, 0xa44
 *   b func_80471834__Q23LOD9LODMemManFv
 */
void LOD::UnkClass_8046A530::func_8046CFD8() {
    GetMemMan().func_80471834();
}

// ---------------------------------------------------------------------------
// func_8046CFB4: OR 0x20000 into the flag word at 0x00 and store it.  If bit
// 0x40 was set in the result, stop; otherwise clear bit 0x40 and set bit
// 0x100.
// ---------------------------------------------------------------------------
void LOD::UnkClass_8046A530::func_8046CFB4() {
    LODLayout_8046A530* l = reinterpret_cast<LODLayout_8046A530*>(this);
    u32 v = l->field_0x00 | 0x20000;
    l->field_0x00 = v;
    if (!(v & 0x40)) {
        return;
    }
    l->field_0x00 = (v & ~0x40) | 0x100;
}

// ---------------------------------------------------------------------------
// Deleting destructor (address-anchored symbol __dt__8046A584): frees the
// object via operator delete when the pointer and the deletion flag are both
// non-zero, and returns the object pointer.
// ---------------------------------------------------------------------------
void* __dt__8046A584(LOD::UnkClass_8046A530* obj, int dealloc) {
    if (obj != nullptr && dealloc > 0) {
        __dl__FPv(obj);
    }
    return obj;
}

// ---------------------------------------------------------------------------
// func_8046A530: init entry point.  The this+0x1d4 / this+0x1200 range check
// is leftover dead code in retail (its boolean result is computed into r0 and
// compared but never branched on); the only real work is the func_8046A5C4
// call.  func_8046A5C4 is defined in another TU (it is not a stub here), so
// the call below cannot be inlined away and keeps its retail `bl` form.
//
// UNREPRODUCIBLE DEAD-BLOCK ARTIFACT (2026-08 sweep, 50+ shapes x GC 2.6/2.7/
// 3.0a3-3.0a5.2/Wii 1.0-1.7 x -O4,p/-O4,s x -ipa file/off): retail keeps
// `addi r0,r3,0x1200; cmpl r4,r0; bge; li r0,0; bgt; li r0,1; cmpi r0,0`
// (a bool (p1<=p2) materialized in r0 with the test branch folded). Every
// high-level shape folds the block away (MWCC DCEs empty nested ifs and
// simplifies `p1<p2 && p1<=p2` to `p1<p2`). The clamp body `if (p1<p2)
// {p1=p2;}` DOES keep the p2 addi + cmpl + branch (used by LOD func_801C17CC
// sibling family) but changes r4 on the p1<p2 path, diverging from retail
// (retail passes p1 on both paths), so it is not an honest reconstruction.
// ---------------------------------------------------------------------------
// Retail func_8046A5C4 is Fv-mangled but this caller passes the range start
// pointer in r4 (which the callee ignores); declare it with the exact name.
void func_8046A5C4__Q23LOD17UnkClass_8046A530Fv(LOD::UnkClass_8046A530* self, u8* p1);

LOD::UnkClass_8046A530* LOD::UnkClass_8046A530::func_8046A530() {
    u8* p1 = reinterpret_cast<u8*>(this) + 0x1d4;
    u8* p2 = reinterpret_cast<u8*>(this) + 0x1200;
    if (p1 < p2) {
        bool b = (p1 <= p2);
        if (b) {
            // dead in retail (cmpi kept, branch folded); unreproducible
            // from any high-level shape across all MWCC versions
        }
    }
    func_8046A5C4__Q23LOD17UnkClass_8046A530Fv(this, p1);
    return this;
}

// ---------------------------------------------------------------------------
// func_8046A5C4: full LOD object init.  Zeroes the two 0x90-byte front
// sub-objects, then for each of the 24 animation elements: identity
// orientation matrices and a random y-axis direction (normalized), followed
// by the per-batch y positions (0,1,2 / 2,1,0 / 0,1,2) and the tail field
// defaults.
// ---------------------------------------------------------------------------
void func_8046A5C4__Q23LOD17UnkClass_8046A530Fv(LOD::UnkClass_8046A530* self, u8* p1) {
    LODLayout_8046A530* l = reinterpret_cast<LODLayout_8046A530*>(self);
    LODElement_8046A530* elems =
        reinterpret_cast<LODElement_8046A530*>(reinterpret_cast<u8*>(self) + 0x120);

    // The two 0x90-byte front sub-objects: four 9-float groups each, all
    // zero except the 9th float of each group (the z axis) which is 1.0.
    for (int i = 0; i < 2; i++) {
        f32* p = reinterpret_cast<f32*>(reinterpret_cast<u8*>(self) + i * 0x90);
        for (int j = 0; j < 4; j++) {
            f32* q = p + j * 9;
            q[0] = lbl_eu_8066A658;
            q[1] = lbl_eu_8066A658;
            q[2] = lbl_eu_8066A658;
            q[3] = lbl_eu_8066A658;
            q[4] = lbl_eu_8066A658;
            q[5] = lbl_eu_8066A658;
            q[6] = lbl_eu_8066A658;
            q[7] = lbl_eu_8066A658;
            q[8] = lbl_eu_8066A65C;
        }
    }

    // Identity matrices, a random direction, and a (0, 1, 2) y placement.
    for (int i = 0; i < 24; i++) {
        LODElement_8046A530* e = &elems[i];
        PSMTXIdentity(e->mtxA);
        PSMTXIdentity(e->mtxB);
        f32 a = lbl_eu_8066A660 * (float)(rand() % 100) - lbl_eu_8066A664;
        f32 b = lbl_eu_8066A660 * (float)(rand() % 100) - lbl_eu_8066A664;
        e->v[6] = ml::CVec3(b, lbl_eu_8066A658, a);
        if (e->v[6].x == lbl_eu_8066A658 && e->v[6].y == lbl_eu_8066A658 && e->v[6].z == lbl_eu_8066A658) {
            e->v[6] = ml::CVec3(lbl_eu_8066A658, lbl_eu_8066A658, lbl_eu_8066A65C);
        } else {
            f32 len2 = e->v[6].y * e->v[6].y + e->v[6].x * e->v[6].x + e->v[6].z * e->v[6].z;
            if (len2 == lbl_eu_8066A658) {
                e->v[6] = ml::CVec3::zero;
            } else {
                PSVECNormalize(e->v[6], e->v[6]);
            }
        }
    }

    for (int i = 0; i < 8; i++) {
        LODElement_8046A530* e = &elems[i];
        e->v[0] = ml::CVec3(lbl_eu_8066A658, (float)0, lbl_eu_8066A658);
        e->v[1] = ml::CVec3(lbl_eu_8066A658, (float)1, lbl_eu_8066A658);
        e->v[2] = ml::CVec3(lbl_eu_8066A658, (float)2, lbl_eu_8066A658);
        e->v[3] = ml::CVec3::zero;
        e->v[4] = ml::CVec3::zero;
        e->v[5] = ml::CVec3::zero;
    }
    for (int i = 0; i < 8; i++) {
        LODElement_8046A530* e = &elems[8 + i];
        e->v[0] = ml::CVec3(lbl_eu_8066A658, (float)2, lbl_eu_8066A658);
        e->v[1] = ml::CVec3(lbl_eu_8066A658, (float)1, lbl_eu_8066A658);
        e->v[2] = ml::CVec3(lbl_eu_8066A658, (float)0, lbl_eu_8066A658);
        e->v[3] = ml::CVec3::zero;
        e->v[4] = ml::CVec3::zero;
        e->v[5] = ml::CVec3::zero;
    }
    for (int i = 0; i < 8; i++) {
        LODElement_8046A530* e = &elems[16 + i];
        e->v[0] = ml::CVec3(lbl_eu_8066A658, (float)0, lbl_eu_8066A658);
        e->v[1] = ml::CVec3(lbl_eu_8066A658, (float)1, lbl_eu_8066A658);
        e->v[2] = ml::CVec3(lbl_eu_8066A658, (float)2, lbl_eu_8066A658);
        e->v[3] = ml::CVec3::zero;
        e->v[4] = ml::CVec3::zero;
        e->v[5] = ml::CVec3::zero;
    }

    // Tail fields: blend limits, frame step and the 10-frame reset counter.
    l->field_0x1200 = lbl_eu_8066A668;
    l->field_0x1204 = lbl_eu_8066A66C;
    l->field_0x1208 = lbl_eu_8066A670;
    l->field_0x120C = lbl_eu_8066A65C;
    l->field_0x1210 = lbl_eu_8066A65C;
    l->field_0x1214 = lbl_eu_8066A674;
    l->field_0x1218 = lbl_eu_8066A674;
    l->field_0x121C = 0xa;
}

// ---------------------------------------------------------------------------
// Stubs for non-FULL_MATCH functions - marked NonMatching in configure.py.
// These compile but do not yet produce matching code.
// ---------------------------------------------------------------------------

void LOD::UnkClass_8046A530::func_8046AAD8() { func_8046AB54(); }

// ---------------------------------------------------------------------------
// func_8046AADC: record the type at 0x121c and set the 0x1200/0x1204/0x1208
// floats from per-type constants.  The retail symbol is Fv-mangled but the
// body reads r4 as its argument, so it is defined here with the exact
// mangled name and a real parameter list.
// ---------------------------------------------------------------------------
void func_8046AADC__Q23LOD17UnkClass_8046A530Fv(LOD::UnkClass_8046A530* self, int type) {
    LODLayout_8046A530* l = reinterpret_cast<LODLayout_8046A530*>(self);
    l->field_0x121C = type;
    if (type == 0) {
        l->field_0x1200 = lbl_eu_8066A658;
        l->field_0x1204 = lbl_eu_8066A658;
        l->field_0x1208 = lbl_eu_8066A658;
        return;
    }
    if (type == 1) {
        l->field_0x1200 = lbl_eu_8066A668;
        l->field_0x1204 = lbl_eu_8066A66C;
        l->field_0x1208 = lbl_eu_8066A668;
        return;
    }
    if (type == 2) {
        l->field_0x1200 = lbl_eu_8066A664;
        l->field_0x1204 = lbl_eu_8066A66C;
        l->field_0x1208 = lbl_eu_8066A664;
        return;
    }
    l->field_0x1200 = lbl_eu_8066A680;
    l->field_0x1204 = lbl_eu_8066A664;
    l->field_0x1208 = lbl_eu_8066A680;
}

// ---------------------------------------------------------------------------
// func_8046AB54: advance the LOD animation by scale: field_0x1210 accumulates
// the scaled step and field_0x1218 counts down.  When the counter drops to
// (or below) zero, re-randomize the counter and the 0x1208 element via rand()
// and the per-element update loops selected by the flags word.
//
// The retail symbol is Fv-mangled but the body reads r4 (flags) and f1
// (scale), so it is defined here with the exact mangled name and a real
// parameter list.  func_8046AD2C / func_8046B0AC / func_8046BAE0 /
// func_8046C580 are likewise Fv-mangled callees that the retail callers pass
// extra arguments to; declare them with the exact names so the call relocs
// match.
// ---------------------------------------------------------------------------
void func_8046AD2C__Q23LOD17UnkClass_8046A530Fv(LOD::UnkClass_8046A530* self, u8* elem);
void func_8046B0AC__Q23LOD17UnkClass_8046A530Fv(LOD::UnkClass_8046A530* self, u8* elem, f32 c);
void func_8046BAE0__Q23LOD17UnkClass_8046A530Fv(LOD::UnkClass_8046A530* self, u8* elem, f32 c);
void func_8046C580__Q23LOD17UnkClass_8046A530Fv(LOD::UnkClass_8046A530* self, u8* elem, f32 c);

#pragma push
#pragma auto_inline off
void func_8046AB54__Q23LOD17UnkClass_8046A530Fv(LOD::UnkClass_8046A530* self, int flags, f32 scale) {
    LODLayout_8046A530* l = reinterpret_cast<LODLayout_8046A530*>(self);
    f32 step = l->field_0x120C * scale;
    l->field_0x1210 = step;
    f32 counter = l->field_0x1218 - step;
    l->field_0x1218 = counter;
    if (counter <= lbl_eu_8066A658) {
        // Randomize the countdown (0..999 / 1000 of the 0x1214 range) and the
        // 0x1208 element (blend 0x1200 / 0x1204 by a 0..19 / 20 factor).
        l->field_0x1218 = l->field_0x1214 * ((float)(rand() % 1000) / lbl_eu_8066A684);
        l->field_0x1208 = l->field_0x1200 * ((float)(rand() % 20) / lbl_eu_8066A688) + l->field_0x1204;
    }
    if (flags & 0x8) {
        u8* p = reinterpret_cast<u8*>(self);
        for (int i = 0; i < 8; i++, p += 0x24) {
            func_8046AD2C__Q23LOD17UnkClass_8046A530Fv(self, p);
        }
    }
    if (flags & 0x10) {
        u8* p = reinterpret_cast<u8*>(self) + 0x120;
        for (int i = 0; i < 8; i++, p += 0xb4) {
            func_8046B0AC__Q23LOD17UnkClass_8046A530Fv(self, p, lbl_eu_8066A68C);
        }
    }
    if (flags & 0x20) {
        for (int i = 0; i < 8; i++) {
            func_8046BAE0__Q23LOD17UnkClass_8046A530Fv(
                self, reinterpret_cast<u8*>(self) + (i + 8) * 0xb4 + 0x120, lbl_eu_8066A68C);
        }
    }
    if (flags & 0x40) {
        for (int i = 0; i < 8; i++) {
            func_8046C580__Q23LOD17UnkClass_8046A530Fv(
                self, reinterpret_cast<u8*>(self) + (i + 16) * 0xb4 + 0x120, lbl_eu_8066A68C);
        }
    }
}
#pragma pop

// ---------------------------------------------------------------------------
// func_8046AD2C: random-walk update for the 0x24-byte sub-elements.  Four
// rand() draws steer the walk direction v[2], then v[0] is damped and v[1]
// is pushed along v[0], with a repulsion term when v[1] gets too far from
// the element origin.
// ---------------------------------------------------------------------------
void func_8046AD2C__Q23LOD17UnkClass_8046A530Fv(LOD::UnkClass_8046A530* self, u8* elem) {
    LODLayout_8046A530* l = reinterpret_cast<LODLayout_8046A530*>(self);
    LODElement_8046A530* e = reinterpret_cast<LODElement_8046A530*>(elem);

    // Working vectors.  MWCC assigns stack slots in reverse declaration
    // order, so the walk-direction temp (retail sp+8) is declared last.
    ml::CVec3 t;
    ml::CVec3 tmp3;
    ml::CVec3 tmp;

    // Four rand() draws, scaled by the per-frame step (0x1210) and element
    // blend (0x1208).  Declared in the order MWCC assigns callee-saved FPRs
    // (f31, f30, f29) so the 2nd draw lands in f29 and the 3rd in f30 like
    // retail.
    f32 f31, f30, f29;
    f31 = lbl_eu_8066A690 * (l->field_0x1208 * (lbl_eu_8066A694 * (float)(rand() % 500)));
    f29 = l->field_0x1210 * (lbl_eu_8066A694 * (float)(rand() % 500) - lbl_eu_8066A664);
    f30 = l->field_0x1210 * (lbl_eu_8066A694 * (float)(rand() % 500) - lbl_eu_8066A664);
    f32 f0 = l->field_0x1210 * (lbl_eu_8066A694 * (float)(rand() % 500) - lbl_eu_8066A664);

    // v[2] += (f0, f30, f29)
    tmp.x = f0;
    tmp.y = f30;
    tmp.z = f29;
    nw4r::math::VEC3Add(e->v[2], e->v[2], tmp);

    // Normalize v[2] (or reset it when it is degenerate).
    f32 len2 = e->v[2].y * e->v[2].y + e->v[2].x * e->v[2].x + e->v[2].z * e->v[2].z;
    if (len2 == lbl_eu_8066A658) {
        e->v[2] = ml::CVec3::zero;
    } else {
        PSVECNormalize(e->v[2], e->v[2]);
    }

    // Push v[0] along v[2] scaled by f31 and the step, then damp v[0] and
    // move v[1].
    t.y = e->v[2].y * f31;
    t.x = e->v[2].x * f31;
    t.z = e->v[2].z * f31;
    nw4r::math::VEC3Scale(tmp3, t, lbl_eu_8066A698 * l->field_0x1210);
    nw4r::math::VEC3Add(e->v[0], e->v[0], tmp3);

    // Repulsion: when -v[1] is long enough, push v[0] away from the origin.
    t.x = -e->v[1].x;
    t.y = -e->v[1].y;
    t.z = -e->v[1].z;
    f32 mag = PSVECMag(t);
    if (mag >= lbl_eu_8066A69C) {
        f32 inv = lbl_eu_8066A65C / mag;
        f32 s = lbl_eu_8066A6A0 * mag;
        nw4r::math::VEC3Scale(tmp3, t, l->field_0x1210 * (inv * s));
        nw4r::math::VEC3Add(e->v[0], e->v[0], tmp3);
    }

    // Integrate v[0] into v[1] and damp v[0].
    nw4r::math::VEC3Scale(tmp3, e->v[0], l->field_0x1210);
    nw4r::math::VEC3Add(e->v[1], e->v[1], tmp3);
    nw4r::math::VEC3Scale(e->v[0], e->v[0], lbl_eu_8066A65C - lbl_eu_8066A6A4 * l->field_0x1210);
}

// ---------------------------------------------------------------------------
// func_8046B0AC: per-frame update of one 0xb4 element (batches 0-7).
// Randomizes the orientation direction, runs two spring passes over the
// v[0..6] chain, then rebuilds the two orientation matrices from the
// v[2]-v[0] / v[1]-v[0] axes against the global Y axis.
// ---------------------------------------------------------------------------
void func_8046B0AC__Q23LOD17UnkClass_8046A530Fv(LOD::UnkClass_8046A530* self, u8* elem, f32 c) {
    LODLayout_8046A530* l = reinterpret_cast<LODLayout_8046A530*>(self);
    LODElement_8046A530* e = reinterpret_cast<LODElement_8046A530*>(elem);

    f32 f31 = l->field_0x1210 * c;
    f32 f30 = l->field_0x1208 * (lbl_eu_8066A694 * (float)(rand() % 500));
    f32 a2 = l->field_0x1210 * (lbl_eu_8066A68C * (lbl_eu_8066A694 * (float)(rand() % 500) - lbl_eu_8066A664));
    f32 a3 = l->field_0x1210 * (lbl_eu_8066A68C * (lbl_eu_8066A694 * (float)(rand() % 500) - lbl_eu_8066A664));

    // Randomize the orientation direction (y stays 0) and normalize it.
    ml::CVec3 tmp;
    tmp.x = a3;
    tmp.y = lbl_eu_8066A658;
    tmp.z = a2;
    nw4r::math::VEC3Add(e->v[6], e->v[6], tmp);
    f32 len2 = e->v[6].y * e->v[6].y + e->v[6].x * e->v[6].x + e->v[6].z * e->v[6].z;
    if (len2 == lbl_eu_8066A658) {
        e->v[6] = ml::CVec3::zero;
    } else {
        PSVECNormalize(e->v[6], e->v[6]);
    }

    // Direction that the rest of the chain follows.
    ml::CVec3 dir;
    dir.y = e->v[6].y * f30;
    dir.x = e->v[6].x * f30;
    dir.z = e->v[6].z * f30;

    // Spring pass 1: for each segment, pull the spring targets (v[3..6])
    // toward the segment direction with a (mag - 1) force and a dot-product
    // damping term.
    for (int i = 0; i < 3; i++) {
        ml::CVec3 diff;
        nw4r::math::VEC3Sub(diff, e->v[i], e->v[i + 1]);
        f32 mag = PSVECMag(diff);
        f32 m1 = mag - lbl_eu_8066A65C;
        bool big = mag >= lbl_eu_8066A69C;
        f32 f6 = lbl_eu_8066A68C * m1;
        ml::CVec3 tdiff;
        nw4r::math::VEC3Sub(tdiff, e->v[i + 3], e->v[i + 4]);
        f32 dot = nw4r::math::VEC3Dot(tdiff, diff);
        f32 f4 = big ? lbl_eu_8066A65C / mag : lbl_eu_8066A6A8;
        f32 s = f4 * (lbl_eu_8066A664 * dot);
        f32 t = -(f6 + s) * f31;
        ml::CVec3 tmp2;
        nw4r::math::VEC3Scale(tmp2, diff, f4);
        nw4r::math::VEC3Scale(tmp2, tmp2, t);
        nw4r::math::VEC3Add(e->v[i + 3], e->v[i + 3], tmp2);
        nw4r::math::VEC3Sub(e->v[i + 4], e->v[i + 4], tmp2);
    }

    // Spring pass 2: pull v[4]/v[5] toward the y-axis anchors (0,1,0)/(0,2,0).
    for (int i = 1; i < 3; i++) {
        ml::CVec3 tmp2;
        tmp2.x = -e->v[i].x;
        tmp2.y = (float)i - e->v[i].y;
        tmp2.z = -e->v[i].z;
        f32 mag = PSVECMag(tmp2);
        if (mag >= lbl_eu_8066A69C) {
            f32 inv = lbl_eu_8066A65C / mag;
            f32 s = lbl_eu_8066A6AC * mag * f31;
            nw4r::math::VEC3Scale(tmp2, tmp2, inv);
            nw4r::math::VEC3Scale(tmp2, tmp2, s);
            nw4r::math::VEC3Add(e->v[i + 3], e->v[i + 3], tmp2);
        }
    }

    // v[5] is dragged along by the direction, and the velocity vector vel is
    // randomized per-component before being applied to v[4].
    ml::CVec3 vel;
    nw4r::math::VEC3Scale(vel, dir, lbl_eu_8066A698 * f31);
    nw4r::math::VEC3Add(e->v[5], e->v[5], vel);
    vel.x *= lbl_eu_80663818 * (float)((rand() % 100) + 100);
    vel.y *= lbl_eu_80663818 * (float)((rand() % 100) + 100);
    vel.z *= lbl_eu_80663818 * (float)((rand() % 100) + 100);

    // Integrate vel into v[4]/v[5] and damp both, clamping to +/- 0.3.
    nw4r::math::VEC3Add(e->v[4], e->v[4], vel);
    nw4r::math::VEC3Scale(e->v[4], e->v[4], lbl_eu_8066A65C - lbl_eu_8066A6A0 * f31);
    if (e->v[4].x > lbl_eu_8066A668) e->v[4].x = lbl_eu_8066A668;
    if (e->v[4].y > lbl_eu_8066A668) e->v[4].y = lbl_eu_8066A668;
    if (e->v[4].z > lbl_eu_8066A668) e->v[4].z = lbl_eu_8066A668;
    if (e->v[4].x < lbl_eu_8066A6B0) e->v[4].x = lbl_eu_8066A6B0;
    if (e->v[4].y < lbl_eu_8066A6B0) e->v[4].y = lbl_eu_8066A6B0;
    if (e->v[4].z < lbl_eu_8066A6B0) e->v[4].z = lbl_eu_8066A6B0;
    nw4r::math::VEC3Scale(e->v[5], e->v[5], lbl_eu_8066A65C - lbl_eu_8066A6A0 * f31);
    if (e->v[5].x > lbl_eu_8066A668) e->v[5].x = lbl_eu_8066A668;
    if (e->v[5].y > lbl_eu_8066A668) e->v[5].y = lbl_eu_8066A668;
    if (e->v[5].z > lbl_eu_8066A668) e->v[5].z = lbl_eu_8066A668;
    if (e->v[5].x < lbl_eu_8066A6B0) e->v[5].x = lbl_eu_8066A6B0;
    if (e->v[5].y < lbl_eu_8066A6B0) e->v[5].y = lbl_eu_8066A6B0;
    if (e->v[5].z < lbl_eu_8066A6B0) e->v[5].z = lbl_eu_8066A6B0;

    // Advance v[1]/v[2] along v[4]/v[5].
    nw4r::math::VEC3Scale(tmp, e->v[4], f31);
    nw4r::math::VEC3Add(e->v[1], e->v[1], tmp);
    nw4r::math::VEC3Scale(tmp, e->v[5], f31);
    nw4r::math::VEC3Add(e->v[2], e->v[2], tmp);

    // Orientation matrix A from the (v[2] - v[0]) axis against the Y axis.
    ml::CVec3 axis;
    nw4r::math::VEC3Sub(axis, e->v[2], e->v[0]);
    f32 lenA = axis.y * axis.y + axis.x * axis.x + axis.z * axis.z;
    if (lenA == lbl_eu_8066A658) {
        axis = ml::CVec3::zero;
    } else {
        PSVECNormalize(axis, axis);
    }
    f32 dotA = nw4r::math::VEC3Dot(axis, ml::CVec3::unitY);
    Quaternion quat;
    if (dotA < lbl_eu_8066A6B4) {
        quat.x = lbl_eu_8066A658;
        quat.y = lbl_eu_8066A65C;
        quat.z = lbl_eu_8066A658;
        quat.w = lbl_eu_8066A658;
    } else {
        ml::CVec3 cross;
        PSVECCrossProduct(ml::CVec3::unitY, axis, cross);
        if (lbl_eu_8066A6B8 * (lbl_eu_8066A65C + dotA) < lbl_eu_8066A658) {
            nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
        }
        f32 w = lbl_eu_8066A658;
        f32 x = lbl_eu_8066A6B8 * (lbl_eu_8066A65C + dotA);
        if (x > lbl_eu_8066A658) {
            w = x * nw4r::math::FrSqrt(x);
        }
        f32 inv = lbl_eu_8066A65C / w;
        quat.x = cross.x * inv;
        quat.y = cross.y * inv;
        quat.z = cross.z * inv;
        quat.w = lbl_eu_8066A664 * w;
    }
    PSMTXQuat(e->mtxA, &quat);

    // Orientation matrix B from the (v[1] - v[0]) axis against the Y axis.
    nw4r::math::VEC3Sub(axis, e->v[1], e->v[0]);
    f32 lenB = axis.y * axis.y + axis.x * axis.x + axis.z * axis.z;
    if (lenB == lbl_eu_8066A658) {
        axis = ml::CVec3::zero;
    } else {
        PSVECNormalize(axis, axis);
    }
    f32 dotB = nw4r::math::VEC3Dot(axis, ml::CVec3::unitY);
    if (dotB < lbl_eu_8066A6B4) {
        quat.x = lbl_eu_8066A658;
        quat.y = lbl_eu_8066A65C;
        quat.z = lbl_eu_8066A658;
        quat.w = lbl_eu_8066A658;
    } else {
        ml::CVec3 cross;
        PSVECCrossProduct(ml::CVec3::unitY, axis, cross);
        if (lbl_eu_8066A6B8 * (lbl_eu_8066A65C + dotB) < lbl_eu_8066A658) {
            nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
        }
        f32 w = lbl_eu_8066A658;
        f32 x = lbl_eu_8066A6B8 * (lbl_eu_8066A65C + dotB);
        if (x > lbl_eu_8066A658) {
            w = x * nw4r::math::FrSqrt(x);
        }
        f32 inv = lbl_eu_8066A65C / w;
        quat.x = cross.x * inv;
        quat.y = cross.y * inv;
        quat.z = cross.z * inv;
        quat.w = lbl_eu_8066A664 * w;
    }
    PSMTXQuat(e->mtxB, &quat);

    e->v[3] = ml::CVec3::zero;
}

// ---------------------------------------------------------------------------
// func_8046BAE0: per-frame update of one 0xb4 element (batches 8-15).  Same
// skeleton as func_8046B0AC, but with a fourth rand() draw, anchors pulled
// toward (0, 2-i, 0), a different random velocity constant, and the
// orientation matrices built against a local (0, -1, 0) vector instead of
// the global Y axis.
// ---------------------------------------------------------------------------
void func_8046BAE0__Q23LOD17UnkClass_8046A530Fv(LOD::UnkClass_8046A530* self, u8* elem, f32 c) {
    LODLayout_8046A530* l = reinterpret_cast<LODLayout_8046A530*>(self);
    LODElement_8046A530* e = reinterpret_cast<LODElement_8046A530*>(elem);

    f32 f31 = l->field_0x1210 * c;
    f32 f30 = l->field_0x1208 * (lbl_eu_8066A694 * (float)(rand() % 500));
    f32 a2 = l->field_0x1210 * (lbl_eu_8066A68C * (lbl_eu_8066A694 * (float)(rand() % 500) - lbl_eu_8066A664));
    f32 a3 = l->field_0x1210 * (lbl_eu_8066A68C * (lbl_eu_8066A694 * (float)(rand() % 500) - lbl_eu_8066A664));
    f32 a4 = l->field_0x1210 * (lbl_eu_8066A68C * (lbl_eu_8066A694 * (float)(rand() % 500) - lbl_eu_8066A664));

    ml::CVec3 tmp;
    tmp.x = a4;
    tmp.y = a3;
    tmp.z = a2;
    nw4r::math::VEC3Add(e->v[6], e->v[6], tmp);
    f32 len2 = e->v[6].y * e->v[6].y + e->v[6].x * e->v[6].x + e->v[6].z * e->v[6].z;
    if (len2 == lbl_eu_8066A658) {
        e->v[6] = ml::CVec3::zero;
    } else {
        PSVECNormalize(e->v[6], e->v[6]);
    }

    ml::CVec3 dir;
    dir.y = e->v[6].y * f30;
    dir.x = e->v[6].x * f30;
    dir.z = e->v[6].z * f30;

    for (int i = 0; i < 3; i++) {
        ml::CVec3 diff;
        nw4r::math::VEC3Sub(diff, e->v[i], e->v[i + 1]);
        f32 mag = PSVECMag(diff);
        f32 m1 = mag - lbl_eu_8066A65C;
        bool big = mag >= lbl_eu_8066A69C;
        f32 f6 = lbl_eu_8066A68C * m1;
        ml::CVec3 tdiff;
        nw4r::math::VEC3Sub(tdiff, e->v[i + 3], e->v[i + 4]);
        f32 dot = nw4r::math::VEC3Dot(tdiff, diff);
        f32 f4 = big ? lbl_eu_8066A65C / mag : lbl_eu_8066A6A8;
        f32 s = f4 * (lbl_eu_8066A664 * dot);
        f32 t = -(f6 + s) * f31;
        ml::CVec3 tmp2;
        nw4r::math::VEC3Scale(tmp2, diff, f4);
        nw4r::math::VEC3Scale(tmp2, tmp2, t);
        nw4r::math::VEC3Add(e->v[i + 3], e->v[i + 3], tmp2);
        nw4r::math::VEC3Sub(e->v[i + 4], e->v[i + 4], tmp2);
    }

    // Anchors at (0, 2-i, 0) for i = 1, 2.
    for (int i = 1; i < 3; i++) {
        ml::CVec3 tmp2;
        tmp2.x = -e->v[i].x;
        tmp2.y = (float)(2 - i) - e->v[i].y;
        tmp2.z = -e->v[i].z;
        f32 mag = PSVECMag(tmp2);
        if (mag >= lbl_eu_8066A69C) {
            f32 inv = lbl_eu_8066A65C / mag;
            f32 s = lbl_eu_8066A6AC * mag * f31;
            nw4r::math::VEC3Scale(tmp2, tmp2, inv);
            nw4r::math::VEC3Scale(tmp2, tmp2, s);
            nw4r::math::VEC3Add(e->v[i + 3], e->v[i + 3], tmp2);
        }
    }

    ml::CVec3 vel;
    nw4r::math::VEC3Scale(vel, dir, lbl_eu_8066A698 * f31);
    nw4r::math::VEC3Add(e->v[5], e->v[5], vel);
    vel.x *= lbl_eu_8066381C * (float)((rand() % 100) + 100);
    vel.y *= lbl_eu_8066381C * (float)((rand() % 100) + 100);
    vel.z *= lbl_eu_8066381C * (float)((rand() % 100) + 100);

    nw4r::math::VEC3Add(e->v[4], e->v[4], vel);
    nw4r::math::VEC3Scale(e->v[4], e->v[4], lbl_eu_8066A65C - lbl_eu_8066A6A0 * f31);
    if (e->v[4].x > lbl_eu_8066A668) e->v[4].x = lbl_eu_8066A668;
    if (e->v[4].y > lbl_eu_8066A668) e->v[4].y = lbl_eu_8066A668;
    if (e->v[4].z > lbl_eu_8066A668) e->v[4].z = lbl_eu_8066A668;
    if (e->v[4].x < lbl_eu_8066A6B0) e->v[4].x = lbl_eu_8066A6B0;
    if (e->v[4].y < lbl_eu_8066A6B0) e->v[4].y = lbl_eu_8066A6B0;
    if (e->v[4].z < lbl_eu_8066A6B0) e->v[4].z = lbl_eu_8066A6B0;
    nw4r::math::VEC3Scale(e->v[5], e->v[5], lbl_eu_8066A65C - lbl_eu_8066A6A0 * f31);
    if (e->v[5].x > lbl_eu_8066A668) e->v[5].x = lbl_eu_8066A668;
    if (e->v[5].y > lbl_eu_8066A668) e->v[5].y = lbl_eu_8066A668;
    if (e->v[5].z > lbl_eu_8066A668) e->v[5].z = lbl_eu_8066A668;
    if (e->v[5].x < lbl_eu_8066A6B0) e->v[5].x = lbl_eu_8066A6B0;
    if (e->v[5].y < lbl_eu_8066A6B0) e->v[5].y = lbl_eu_8066A6B0;
    if (e->v[5].z < lbl_eu_8066A6B0) e->v[5].z = lbl_eu_8066A6B0;

    nw4r::math::VEC3Scale(tmp, e->v[4], f31);
    nw4r::math::VEC3Add(e->v[1], e->v[1], tmp);
    nw4r::math::VEC3Scale(tmp, e->v[5], f31);
    nw4r::math::VEC3Add(e->v[2], e->v[2], tmp);

    // Local "up" vector for the orientation rebuilds.
    ml::CVec3 up(lbl_eu_8066A658, lbl_eu_8066A6BC, lbl_eu_8066A658);

    ml::CVec3 axis;
    nw4r::math::VEC3Sub(axis, e->v[2], e->v[0]);
    f32 lenA = axis.y * axis.y + axis.x * axis.x + axis.z * axis.z;
    if (lenA == lbl_eu_8066A658) {
        axis = ml::CVec3::zero;
    } else {
        PSVECNormalize(axis, axis);
    }
    f32 dotA = nw4r::math::VEC3Dot(up, axis);
    Quaternion quat;
    if (dotA < lbl_eu_8066A6B4) {
        quat.x = lbl_eu_8066A658;
        quat.y = lbl_eu_8066A65C;
        quat.z = lbl_eu_8066A658;
        quat.w = lbl_eu_8066A658;
    } else {
        ml::CVec3 cross;
        PSVECCrossProduct(up, axis, cross);
        if (lbl_eu_8066A6B8 * (lbl_eu_8066A65C + dotA) < lbl_eu_8066A658) {
            nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
        }
        f32 w = lbl_eu_8066A658;
        f32 x = lbl_eu_8066A6B8 * (lbl_eu_8066A65C + dotA);
        if (x > lbl_eu_8066A658) {
            w = x * nw4r::math::FrSqrt(x);
        }
        f32 inv = lbl_eu_8066A65C / w;
        quat.x = cross.x * inv;
        quat.y = cross.y * inv;
        quat.z = cross.z * inv;
        quat.w = lbl_eu_8066A664 * w;
    }
    PSMTXQuat(e->mtxA, &quat);

    ml::CVec3 tmpB = e->v[1] - e->v[0];
    f32 lenB = tmpB.y * tmpB.y + tmpB.x * tmpB.x + tmpB.z * tmpB.z;
    if (lenB == lbl_eu_8066A658) {
        tmpB = ml::CVec3::zero;
    } else {
        PSVECNormalize(tmpB, tmpB);
    }
    f32 dotB = nw4r::math::VEC3Dot(up, tmpB);
    if (dotB < lbl_eu_8066A6B4) {
        quat.x = lbl_eu_8066A658;
        quat.y = lbl_eu_8066A65C;
        quat.z = lbl_eu_8066A658;
        quat.w = lbl_eu_8066A658;
    } else {
        ml::CVec3 cross;
        PSVECCrossProduct(up, tmpB, cross);
        if (lbl_eu_8066A6B8 * (lbl_eu_8066A65C + dotB) < lbl_eu_8066A658) {
            nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
        }
        f32 w = lbl_eu_8066A658;
        f32 x = lbl_eu_8066A6B8 * (lbl_eu_8066A65C + dotB);
        if (x > lbl_eu_8066A658) {
            w = x * nw4r::math::FrSqrt(x);
        }
        f32 inv = lbl_eu_8066A65C / w;
        quat.x = cross.x * inv;
        quat.y = cross.y * inv;
        quat.z = cross.z * inv;
        quat.w = lbl_eu_8066A664 * w;
    }
    PSMTXQuat(e->mtxB, &quat);

    e->v[3] = ml::CVec3::zero;
}

// ---------------------------------------------------------------------------
// func_8046C580: per-frame update of one 0xb4 element (batches 16-23).
// Byte-identical skeleton to func_8046B0AC, but the orientation matrices are
// built against the global X axis instead of the Y axis.
// ---------------------------------------------------------------------------
void func_8046C580__Q23LOD17UnkClass_8046A530Fv(LOD::UnkClass_8046A530* self, u8* elem, f32 c) {
    LODLayout_8046A530* l = reinterpret_cast<LODLayout_8046A530*>(self);
    LODElement_8046A530* e = reinterpret_cast<LODElement_8046A530*>(elem);

    f32 f31 = l->field_0x1210 * c;
    f32 f30 = l->field_0x1208 * (lbl_eu_8066A694 * (float)(rand() % 500));
    f32 a2 = l->field_0x1210 * (lbl_eu_8066A68C * (lbl_eu_8066A694 * (float)(rand() % 500) - lbl_eu_8066A664));
    f32 a3 = l->field_0x1210 * (lbl_eu_8066A68C * (lbl_eu_8066A694 * (float)(rand() % 500) - lbl_eu_8066A664));

    ml::CVec3 tmp;
    tmp.x = a3;
    tmp.y = lbl_eu_8066A658;
    tmp.z = a2;
    nw4r::math::VEC3Add(e->v[6], e->v[6], tmp);
    f32 len2 = e->v[6].y * e->v[6].y + e->v[6].x * e->v[6].x + e->v[6].z * e->v[6].z;
    if (len2 == lbl_eu_8066A658) {
        e->v[6] = ml::CVec3::zero;
    } else {
        PSVECNormalize(e->v[6], e->v[6]);
    }

    ml::CVec3 dir;
    dir.y = e->v[6].y * f30;
    dir.x = e->v[6].x * f30;
    dir.z = e->v[6].z * f30;

    for (int i = 0; i < 3; i++) {
        ml::CVec3 diff;
        nw4r::math::VEC3Sub(diff, e->v[i], e->v[i + 1]);
        f32 mag = PSVECMag(diff);
        f32 m1 = mag - lbl_eu_8066A65C;
        bool big = mag >= lbl_eu_8066A69C;
        f32 f6 = lbl_eu_8066A68C * m1;
        ml::CVec3 tdiff;
        nw4r::math::VEC3Sub(tdiff, e->v[i + 3], e->v[i + 4]);
        f32 dot = nw4r::math::VEC3Dot(tdiff, diff);
        f32 f4 = big ? lbl_eu_8066A65C / mag : lbl_eu_8066A6A8;
        f32 s = f4 * (lbl_eu_8066A664 * dot);
        f32 t = -(f6 + s) * f31;
        ml::CVec3 tmp2;
        nw4r::math::VEC3Scale(tmp2, diff, f4);
        nw4r::math::VEC3Scale(tmp2, tmp2, t);
        nw4r::math::VEC3Add(e->v[i + 3], e->v[i + 3], tmp2);
        nw4r::math::VEC3Sub(e->v[i + 4], e->v[i + 4], tmp2);
    }

    for (int i = 1; i < 3; i++) {
        ml::CVec3 tmp2;
        tmp2.x = -e->v[i].x;
        tmp2.y = (float)i - e->v[i].y;
        tmp2.z = -e->v[i].z;
        f32 mag = PSVECMag(tmp2);
        if (mag >= lbl_eu_8066A69C) {
            f32 inv = lbl_eu_8066A65C / mag;
            f32 s = lbl_eu_8066A6AC * mag * f31;
            nw4r::math::VEC3Scale(tmp2, tmp2, inv);
            nw4r::math::VEC3Scale(tmp2, tmp2, s);
            nw4r::math::VEC3Add(e->v[i + 3], e->v[i + 3], tmp2);
        }
    }

    ml::CVec3 vel;
    nw4r::math::VEC3Scale(vel, dir, lbl_eu_8066A698 * f31);
    nw4r::math::VEC3Add(e->v[5], e->v[5], vel);
    vel.x *= lbl_eu_80663818 * (float)((rand() % 100) + 100);
    vel.y *= lbl_eu_80663818 * (float)((rand() % 100) + 100);
    vel.z *= lbl_eu_80663818 * (float)((rand() % 100) + 100);

    nw4r::math::VEC3Add(e->v[4], e->v[4], vel);
    nw4r::math::VEC3Scale(e->v[4], e->v[4], lbl_eu_8066A65C - lbl_eu_8066A6A0 * f31);
    if (e->v[4].x > lbl_eu_8066A668) e->v[4].x = lbl_eu_8066A668;
    if (e->v[4].y > lbl_eu_8066A668) e->v[4].y = lbl_eu_8066A668;
    if (e->v[4].z > lbl_eu_8066A668) e->v[4].z = lbl_eu_8066A668;
    if (e->v[4].x < lbl_eu_8066A6B0) e->v[4].x = lbl_eu_8066A6B0;
    if (e->v[4].y < lbl_eu_8066A6B0) e->v[4].y = lbl_eu_8066A6B0;
    if (e->v[4].z < lbl_eu_8066A6B0) e->v[4].z = lbl_eu_8066A6B0;
    nw4r::math::VEC3Scale(e->v[5], e->v[5], lbl_eu_8066A65C - lbl_eu_8066A6A0 * f31);
    if (e->v[5].x > lbl_eu_8066A668) e->v[5].x = lbl_eu_8066A668;
    if (e->v[5].y > lbl_eu_8066A668) e->v[5].y = lbl_eu_8066A668;
    if (e->v[5].z > lbl_eu_8066A668) e->v[5].z = lbl_eu_8066A668;
    if (e->v[5].x < lbl_eu_8066A6B0) e->v[5].x = lbl_eu_8066A6B0;
    if (e->v[5].y < lbl_eu_8066A6B0) e->v[5].y = lbl_eu_8066A6B0;
    if (e->v[5].z < lbl_eu_8066A6B0) e->v[5].z = lbl_eu_8066A6B0;

    nw4r::math::VEC3Scale(tmp, e->v[4], f31);
    nw4r::math::VEC3Add(e->v[1], e->v[1], tmp);
    nw4r::math::VEC3Scale(tmp, e->v[5], f31);
    nw4r::math::VEC3Add(e->v[2], e->v[2], tmp);

    // Orientation matrices against the global X axis.
    ml::CVec3 axis;
    nw4r::math::VEC3Sub(axis, e->v[2], e->v[0]);
    f32 lenA = axis.y * axis.y + axis.x * axis.x + axis.z * axis.z;
    if (lenA == lbl_eu_8066A658) {
        axis = ml::CVec3::zero;
    } else {
        PSVECNormalize(axis, axis);
    }
    f32 dotA = nw4r::math::VEC3Dot(axis, ml::CVec3::unitX);
    Quaternion quat;
    if (dotA < lbl_eu_8066A6B4) {
        quat.x = lbl_eu_8066A658;
        quat.y = lbl_eu_8066A65C;
        quat.z = lbl_eu_8066A658;
        quat.w = lbl_eu_8066A658;
    } else {
        ml::CVec3 cross;
        PSVECCrossProduct(ml::CVec3::unitX, axis, cross);
        if (lbl_eu_8066A6B8 * (lbl_eu_8066A65C + dotA) < lbl_eu_8066A658) {
            nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
        }
        f32 w = lbl_eu_8066A658;
        f32 x = lbl_eu_8066A6B8 * (lbl_eu_8066A65C + dotA);
        if (x > lbl_eu_8066A658) {
            w = x * nw4r::math::FrSqrt(x);
        }
        f32 inv = lbl_eu_8066A65C / w;
        quat.x = cross.x * inv;
        quat.y = cross.y * inv;
        quat.z = cross.z * inv;
        quat.w = lbl_eu_8066A664 * w;
    }
    PSMTXQuat(e->mtxA, &quat);

    ml::CVec3 tmpB = e->v[1] - e->v[0];
    f32 lenB = tmpB.y * tmpB.y + tmpB.x * tmpB.x + tmpB.z * tmpB.z;
    if (lenB == lbl_eu_8066A658) {
        tmpB = ml::CVec3::zero;
    } else {
        PSVECNormalize(tmpB, tmpB);
    }
    f32 dotB = nw4r::math::VEC3Dot(tmpB, ml::CVec3::unitX);
    if (dotB < lbl_eu_8066A6B4) {
        quat.x = lbl_eu_8066A658;
        quat.y = lbl_eu_8066A65C;
        quat.z = lbl_eu_8066A658;
        quat.w = lbl_eu_8066A658;
    } else {
        ml::CVec3 cross;
        PSVECCrossProduct(ml::CVec3::unitX, tmpB, cross);
        if (lbl_eu_8066A6B8 * (lbl_eu_8066A65C + dotB) < lbl_eu_8066A658) {
            nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
        }
        f32 w = lbl_eu_8066A658;
        f32 x = lbl_eu_8066A6B8 * (lbl_eu_8066A65C + dotB);
        if (x > lbl_eu_8066A658) {
            w = x * nw4r::math::FrSqrt(x);
        }
        f32 inv = lbl_eu_8066A65C / w;
        quat.x = cross.x * inv;
        quat.y = cross.y * inv;
        quat.z = cross.z * inv;
        quat.w = lbl_eu_8066A664 * w;
    }
    PSMTXQuat(e->mtxB, &quat);

    e->v[3] = ml::CVec3::zero;
}
