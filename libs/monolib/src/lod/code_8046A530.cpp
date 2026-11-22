// Translation unit for LOD::UnkClass_8046A530.
// Full layout information is in UnkClass_8046A530.hpp.

#include <harness_catalog.h>
#include <PowerPC_EABI_Support/MSL_C/MSL_Common/rand.h>
#include <revolution/MTX.h>
#include <revolution/mtx/vec.h>
#include "monolib/lod/UnkClass_8046A530.hpp"  // LOD::UnkClass_8046A530 definition
#include "monolib/lod/UnkClass_8046D144.hpp"  // enclosing LOD manager
#include "monolib/lod/LODMemMan.hpp"   // provides full LOD::LODMemMan definition
#include "monolib/core/code_804E36DC.hpp"  // declares operator delete(C ABI)
#include "monolib/math/CVec3.hpp"      // ml::CVec3 + nw4r math inlines (VEC3*, FrSqrt, Warning)

// .sdata2 constants are written as source literals; MWCC emits the TU's own
// literal pool in first-use order, matching the retail 0x68-byte pool.

// s32 -> f32 via a plain cast: MWCC lowers int-to-float through the shared
// signed magic double (0x4330000080000000) natively, which reproduces the
// two magic words at the retail pool's 0x8066A678/67c slots.
static inline f32 s32ToF_678(s32 v) {
    return v;
}
extern const f32 lbl_eu_8066A6B8;    // 2.0f, shared pool constant used by initLodState

// .sdata constants (defined below in the data block).
extern f32 sLodBatch0VelocityScale;    // 0.005f
extern f32 sLodBatch1VelocityScale;    // 0.005f
extern f32 sLodBatch2VelocityScales[2]; // 0.005f (updateLodBatch2 velocity constant)

// Warning() file / format rodata strings (nw4r db assert).
extern const char lbl_eu_80526324[];
extern const char lbl_eu_80526300[];

// Local mirror of the UnkClass_8046A530 field layout for the offsets used by
// this TU.  The shared header only exposes opaque padding up to 0xa44, so the
// recovered fields are declared here and the object is viewed through this
// struct instead of raw pointer arithmetic.
struct LODLayout_8046A530 {
    u32 field_0x00;                 // 0x000  flag word (checkLodFlagState)
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

// Trivially-copyable 3-float mirror with an init-list constructor: lets the
// batch-loop constant vectors be constructed DIRECTLY into the element
// (single-level store, like retail) instead of through the ml::CVec3
// ctor->set->op= chain that Wii/1.1 stages via a stack temporary.
struct Vec3Direct_8046A530 {
    f32 x, y, z;
    Vec3Direct_8046A530(f32 xx, f32 yy, f32 zz) : x(xx), y(yy), z(zz) {}
};

// Helper: access the LODMemMan embedded at offset 0xa44.
// Uses one-past-array addressing (well-defined C++) to avoid raw
// pointer arithmetic in callers.
LOD::LODMemMan& LOD::UnkClass_8046A530::GetMemMan() {
    return *static_cast<LOD::LODMemMan*>(
        static_cast<void*>(&mPad_00 + 1));
}

/**
 * Forwarder tail call into the embedded LODMemMan's buffer init (retail:
 * `addi r3, r3, 0xa44; b LODMemMan::initElementBuffers`).  Called from the
 * CTaskLOD factory with (handle, size); the LODMemMan is a member of the
 * enclosing manager, so this is a method of that class.
 */
void LOD::UnkClass_8046D144::initElementBuffers(u32 handle, u32 size) {
    mMemMan.initElementBuffers(handle, size);
}

// ---------------------------------------------------------------------------
// checkLodFlagState: OR 0x20000 into the flag word at 0x00 and store it.  If bit
// 0x40 was set in the result, stop; otherwise clear bit 0x40 and set bit
// 0x100.
// ---------------------------------------------------------------------------
void LOD::UnkClass_8046A530::checkLodFlagState() {
    LODLayout_8046A530* l = reinterpret_cast<LODLayout_8046A530*>(this);
    u32 v = l->field_0x00 | 0x20000;
    l->field_0x00 = v;
    if (!(v & 0x40)) {
        return;
    }
    l->field_0x00 = (v & ~0x40) | 0x100;
}

// ---------------------------------------------------------------------------
// Destructor (empty body): MWCC emits the deleting-dtor shape (operator
// delete when the pointer and the deletion flag are both non-zero) and the
// no-action extab record retail carries.
// ---------------------------------------------------------------------------
LOD::UnkClass_8046A530::~UnkClass_8046A530() {}

// ---------------------------------------------------------------------------
// Constructor (retail 8046E500, directly before the dtor).  The this+0x1d4 / this+0x1200 range check
// is leftover dead code in retail (its boolean result is computed into r0 and
// compared but never branched on); the only real work is the initLodState
// call.  initLodState is defined in another TU (it is not a stub here), so
// the call below cannot be inlined away and keeps its retail `bl` form.
//
// UNREPRODUCIBLE DEAD-BLOCK ARTIFACT (2026-08 sweep, 50+ shapes x GC 2.6/2.7/
// 3.0a3-3.0a5.2/Wii 1.0-1.7 x -O4,p/-O4,s x -ipa file/off): retail keeps
// `addi r0,r3,0x1200; cmpl r4,r0; bge; li r0,0; bgt; li r0,1; cmpi r0,0`
// (a bool (p1<=p2) materialized in r0 with the test branch folded). Every
// high-level shape folds the block away (MWCC DCEs empty nested ifs and
// simplifies `p1<p2 && p1<=p2` to `p1<p2`). The clamp body `if (p1<p2)
// {p1=p2;}` DOES keep the p2 addi + cmpl + branch (used by LOD CfSoundMan_ApplySlotGain
// sibling family) but changes r4 on the p1<p2 path, diverging from retail
// (retail passes p1 on both paths), so it is not an honest reconstruction.
// ---------------------------------------------------------------------------
// Retail initLodState is Fv-mangled but this caller passes the range start
// pointer in r4 (which the callee ignores); declare it with the exact name.
extern "C" void initLodState__Q23LOD17UnkClass_8046A530Fv(LOD::UnkClass_8046A530* self, u8* p1);

LOD::UnkClass_8046A530::UnkClass_8046A530() {
    u8* p1 = reinterpret_cast<u8*>(this) + 0x1d4;
    u8* p2 = reinterpret_cast<u8*>(this) + 0x1200;
    if (p1 < p2) {
        bool b = (p1 <= p2);
        if (b) {
            // dead in retail (cmpi kept, branch folded); unreproducible
            // from any high-level shape across all MWCC versions
        }
    }
    initLodState__Q23LOD17UnkClass_8046A530Fv(this, p1);
}


// ---------------------------------------------------------------------------
// initLodState: full LOD object init.  Zeroes the two 0x90-byte front
// sub-objects, then for each of the 24 animation elements: identity
// orientation matrices and a random y-axis direction (normalized), followed
// by the per-batch y positions (0,1,2 / 2,1,0 / 0,1,2) and the tail field
// defaults.
// ---------------------------------------------------------------------------
extern "C" void initLodState__Q23LOD17UnkClass_8046A530Fv(LOD::UnkClass_8046A530* self, u8* p1) {
    LODLayout_8046A530* l = reinterpret_cast<LODLayout_8046A530*>(self);
    LODElement_8046A530* elems =
        reinterpret_cast<LODElement_8046A530*>(reinterpret_cast<u8*>(self) + 0x120);

    // The two 0x90-byte front sub-objects: four 9-float groups each, all
    // zero except the 9th float of each group (the z axis) which is 1.0.
    for (int i = 0; i < 2; i++) {
        f32* p = reinterpret_cast<f32*>(reinterpret_cast<u8*>(self) + i * 0x90);
        for (int j = 0; j < 4; j++) {
            f32* q = p + j * 9;
            q[0] = 0.0f;
            q[1] = 0.0f;
            q[2] = 0.0f;
            q[3] = 0.0f;
            q[4] = 0.0f;
            q[5] = 0.0f;
            q[6] = 0.0f;
            q[7] = 0.0f;
            q[8] = 1.0f;
        }
    }

    // Identity matrices, a random direction, and a (0, 1, 2) y placement.
    for (int i = 0; i < 24; i++) {
        LODElement_8046A530* e = &elems[i];
        PSMTXIdentity(e->mtxA);
        PSMTXIdentity(e->mtxB);
        f32 a = 0.01f * s32ToF_678(rand() % 100) - 0.5f;
        f32 b = 0.01f * s32ToF_678(rand() % 100) - 0.5f;
        *reinterpret_cast<Vec3Direct_8046A530*>(&e->v[6]) = Vec3Direct_8046A530(b, 0.0f, a);
        if (e->v[6].x == 0.0f && e->v[6].y == 0.0f && e->v[6].z == 0.0f) {
            *reinterpret_cast<Vec3Direct_8046A530*>(&e->v[6]) = Vec3Direct_8046A530(0.0f, 0.0f, 1.0f);
        } else {
            f32 len2 = e->v[6].y * e->v[6].y + e->v[6].x * e->v[6].x + e->v[6].z * e->v[6].z;
            if (len2 == 0.0f) {
                e->v[6] = ml::CVec3::zero;
            } else {
                PSVECNormalize(e->v[6], e->v[6]);
            }
        }
    }

    for (int i = 0; i < 8; i++) {
        LODElement_8046A530* e = &elems[i];
        *reinterpret_cast<Vec3Direct_8046A530*>(&e->v[0]) = Vec3Direct_8046A530(0.0f, 0.0f, 0.0f);
        *reinterpret_cast<Vec3Direct_8046A530*>(&e->v[1]) = Vec3Direct_8046A530(0.0f, 1.0f, 0.0f);
        *reinterpret_cast<Vec3Direct_8046A530*>(&e->v[2]) = Vec3Direct_8046A530(0.0f, lbl_eu_8066A6B8, 0.0f);
        e->v[3] = ml::CVec3::zero;
        e->v[4] = ml::CVec3::zero;
        e->v[5] = ml::CVec3::zero;
    }
    for (int i = 0; i < 8; i++) {
        LODElement_8046A530* e = &elems[8 + i];
        *reinterpret_cast<Vec3Direct_8046A530*>(&e->v[0]) = Vec3Direct_8046A530(0.0f, lbl_eu_8066A6B8, 0.0f);
        *reinterpret_cast<Vec3Direct_8046A530*>(&e->v[1]) = Vec3Direct_8046A530(0.0f, 1.0f, 0.0f);
        *reinterpret_cast<Vec3Direct_8046A530*>(&e->v[2]) = Vec3Direct_8046A530(0.0f, 0.0f, 0.0f);
        e->v[3] = ml::CVec3::zero;
        e->v[4] = ml::CVec3::zero;
        e->v[5] = ml::CVec3::zero;
    }
    for (int i = 0; i < 8; i++) {
        LODElement_8046A530* e = &elems[16 + i];
        *reinterpret_cast<Vec3Direct_8046A530*>(&e->v[0]) = Vec3Direct_8046A530(0.0f, 0.0f, 0.0f);
        *reinterpret_cast<Vec3Direct_8046A530*>(&e->v[1]) = Vec3Direct_8046A530(0.0f, 1.0f, 0.0f);
        *reinterpret_cast<Vec3Direct_8046A530*>(&e->v[2]) = Vec3Direct_8046A530(0.0f, lbl_eu_8066A6B8, 0.0f);
        e->v[3] = ml::CVec3::zero;
        e->v[4] = ml::CVec3::zero;
        e->v[5] = ml::CVec3::zero;
    }

    // Tail fields: blend limits, frame step and the 10-frame reset counter.
    l->field_0x1200 = 0.3f;
    l->field_0x1204 = 0.1f;
    l->field_0x1208 = 0.6f;
    l->field_0x120C = 1.0f;
    l->field_0x1210 = 1.0f;
    l->field_0x1214 = 10.0f;
    l->field_0x1218 = 10.0f;
    l->field_0x121C = 0xa;
}

// ---------------------------------------------------------------------------
// Stubs for non-FULL_MATCH functions - marked NonMatching in configure.py.
// These compile but do not yet produce matching code.
// ---------------------------------------------------------------------------

void LOD::UnkClass_8046A530::isLodActive() { advanceLodAnim(); }

// ---------------------------------------------------------------------------
// updateLodPositions: record the type at 0x121c and set the 0x1200/0x1204/0x1208
// floats from per-type constants.  The retail symbol is Fv-mangled but the
// body reads r4 as its argument, so it is defined here with the exact
// mangled name and a real parameter list.
// ---------------------------------------------------------------------------
void setVariation__Q23LOD17UnkClass_8046A530Fv(LOD::UnkClass_8046A530* self, int type) {
    LODLayout_8046A530* l = reinterpret_cast<LODLayout_8046A530*>(self);
    l->field_0x121C = type;
    if (type == 0) {
        l->field_0x1200 = 0.0f;
        l->field_0x1204 = 0.0f;
        l->field_0x1208 = 0.0f;
        return;
    }
    if (type == 1) {
        l->field_0x1200 = 0.3f;
        l->field_0x1204 = 0.1f;
        l->field_0x1208 = 0.3f;
        return;
    }
    if (type == 2) {
        l->field_0x1200 = 0.5f;
        l->field_0x1204 = 0.1f;
        l->field_0x1208 = 0.5f;
        return;
    }
    l->field_0x1200 = 1.5f;
    l->field_0x1204 = 0.5f;
    l->field_0x1208 = 1.5f;
}

// ---------------------------------------------------------------------------
// advanceLodAnim: advance the LOD animation by scale: field_0x1210 accumulates
// the scaled step and field_0x1218 counts down.  When the counter drops to
// (or below) zero, re-randomize the counter and the 0x1208 element via rand()
// and the per-element update loops selected by the flags word.
//
// The retail symbol is Fv-mangled but the body reads r4 (flags) and f1
// (scale), so it is defined here with the exact mangled name and a real
// parameter list.  updateLodWalk / updateLodBatch0 / updateLodBatch1 /
// updateLodBatch2 are likewise Fv-mangled callees that the retail callers pass
// extra arguments to; declare them with the exact names so the call relocs
// match.
// ---------------------------------------------------------------------------

#pragma push
#pragma auto_inline off  // noinline-ok: retail keeps the updateLod* walk helpers out of line; -ipa off / -inline noauto both lose all 4 fns
extern "C" void advanceLodAnim__Q23LOD17UnkClass_8046A530Fv(LOD::UnkClass_8046A530* self, int flags, f32 scale) {
    LODLayout_8046A530* l = reinterpret_cast<LODLayout_8046A530*>(self);
    f32 step = l->field_0x120C * scale;
    l->field_0x1210 = step;
    f32 counter = l->field_0x1218 - step;
    l->field_0x1218 = counter;
    if (counter <= 0.0f) {
        // Randomize the countdown (0..999 / 1000 of the 0x1214 range) and the
        // 0x1208 element (blend 0x1200 / 0x1204 by a 0..19 / 20 factor).
        l->field_0x1218 = l->field_0x1214 * (s32ToF_678(rand() % 1000) / 1000.0f);
        l->field_0x1208 = l->field_0x1200 * (s32ToF_678(rand() % 20) / 20.0f) + l->field_0x1204;
    }
    if (flags & 0x8) {
        u8* p = reinterpret_cast<u8*>(self);
        for (int i = 0; i < 8; i++, p += 0x24) {
            self->updateLodWalk(p);
        }
    }
    if (flags & 0x10) {
        // i declared first (Rule A): retail colors the counter r28 and the
        // running element pointer r29 in this loop.
        int i;
        u8* p = reinterpret_cast<u8*>(self) + 0x120;
        for (i = 0; i < 8; i++, p += 0xb4) {
            self->updateLodBatch0(p, 0.4f);
        }
    }
    if (flags & 0x20) {
        for (int i = 0; i < 8; i++) {
            self->updateLodBatch1(
                reinterpret_cast<u8*>(self) + (i + 8) * 0xb4 + 0x120, 0.4f);
        }
    }
    if (flags & 0x40) {
        for (int i = 0; i < 8; i++) {
            self->updateLodBatch2(
                reinterpret_cast<u8*>(self) + (i + 16) * 0xb4 + 0x120, 0.4f);
        }
    }
}
#pragma pop

// ---------------------------------------------------------------------------
// updateLodWalk:
// rand() draws steer the walk direction v[2], then v[0] is damped and v[1]
// is pushed along v[0], with a repulsion term when v[1] gets too far from
// the element origin.
// ---------------------------------------------------------------------------
void LOD::UnkClass_8046A530::updateLodWalk(u8* elem) {
    LODLayout_8046A530* l = reinterpret_cast<LODLayout_8046A530*>(this);
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
    f31 = 0.11f * (l->field_0x1208 * (0.002f * s32ToF_678(rand() % 500)));
    f29 = l->field_0x1210 * (0.002f * s32ToF_678(rand() % 500) - 0.5f);
    f30 = l->field_0x1210 * (0.002f * s32ToF_678(rand() % 500) - 0.5f);
    f32 f0 = l->field_0x1210 * (0.002f * s32ToF_678(rand() % 500) - 0.5f);

    // v[2] += (f0, f30, f29)
    tmp.x = f0;
    tmp.y = f30;
    tmp.z = f29;
    nw4r::math::VEC3Add(e->v[2], e->v[2], tmp);

    // Normalize v[2] (or reset it when it is degenerate).
    f32 len2 = e->v[2].y * e->v[2].y + e->v[2].x * e->v[2].x + e->v[2].z * e->v[2].z;
    if (len2 == 0.0f) {
        e->v[2] = ml::CVec3::zero;
    } else {
        PSVECNormalize(e->v[2], e->v[2]);
    }

    // Push v[0] along v[2] scaled by f31 and the step, then damp v[0] and
    // move v[1].
    t.y = e->v[2].y * f31;
    t.x = e->v[2].x * f31;
    t.z = e->v[2].z * f31;
    nw4r::math::VEC3Scale(tmp3, t, 0.033333335f * l->field_0x1210);
    nw4r::math::VEC3Add(e->v[0], e->v[0], tmp3);

    // Repulsion: when -v[1] is long enough, push v[0] away from the origin.
    t.x = -e->v[1].x;
    t.y = -e->v[1].y;
    t.z = -e->v[1].z;
    f32 mag = PSVECMag(t);
    if (mag >= 0.0001f) {
        f32 inv = 1.0f / mag;
        f32 s = 0.02f * mag;
        nw4r::math::VEC3Scale(tmp3, t, l->field_0x1210 * (inv * s));
        nw4r::math::VEC3Add(e->v[0], e->v[0], tmp3);
    }

    // Integrate v[0] into v[1] and damp v[0].
    nw4r::math::VEC3Scale(tmp3, e->v[0], l->field_0x1210);
    nw4r::math::VEC3Add(e->v[1], e->v[1], tmp3);
    nw4r::math::VEC3Scale(e->v[0], e->v[0], 1.0f - 0.03f * l->field_0x1210);
}

// ---------------------------------------------------------------------------
// updateLodBatch0: per-frame update of one 0xb4 element (batches 0-7).
// Randomizes the orientation direction, runs two spring passes over the
// v[0..6] chain, then rebuilds the two orientation matrices from the
// v[2]-v[0] / v[1]-v[0] axes against the global Y axis.
// ---------------------------------------------------------------------------
void LOD::UnkClass_8046A530::updateLodBatch0(u8* elem, f32 c) {
    LODLayout_8046A530* l = reinterpret_cast<LODLayout_8046A530*>(this);
    LODElement_8046A530* e = reinterpret_cast<LODElement_8046A530*>(elem);

    f32 f31 = l->field_0x1210 * c;
    f32 f30 = l->field_0x1208 * (0.002f * s32ToF_678(rand() % 500));
    f32 a2 = l->field_0x1210 * (0.4f * (0.002f * s32ToF_678(rand() % 500) - 0.5f));
    f32 a3 = l->field_0x1210 * (0.4f * (0.002f * s32ToF_678(rand() % 500) - 0.5f));

    // Randomize the orientation direction (y stays 0) and normalize it.
    ml::CVec3 tmp;
    tmp.x = a3;
    tmp.y = 0.0f;
    tmp.z = a2;
    nw4r::math::VEC3Add(e->v[6], e->v[6], tmp);
    f32 len2 = e->v[6].y * e->v[6].y + e->v[6].x * e->v[6].x + e->v[6].z * e->v[6].z;
    if (len2 == 0.0f) {
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
        f32 m1 = mag - 1.0f;
        bool big = mag >= 0.0001f;
        f32 f6 = 0.4f * m1;
        ml::CVec3 tdiff;
        nw4r::math::VEC3Sub(tdiff, e->v[i + 3], e->v[i + 4]);
        f32 dot = nw4r::math::VEC3Dot(tdiff, diff);
        f32 f4 = big ? 1.0f / mag : 10000.0f;
        f32 s = f4 * (0.5f * dot);
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
        tmp2.y = s32ToF_678(i) - e->v[i].y;
        tmp2.z = -e->v[i].z;
        f32 mag = PSVECMag(tmp2);
        if (mag >= 0.0001f) {
            f32 inv = 1.0f / mag;
            f32 s = 0.13f * mag * f31;
            nw4r::math::VEC3Scale(tmp2, tmp2, inv);
            nw4r::math::VEC3Scale(tmp2, tmp2, s);
            nw4r::math::VEC3Add(e->v[i + 3], e->v[i + 3], tmp2);
        }
    }

    // Reuse the top temp for the random velocity like retail (saves a stack
    // slot - retail allocates 9 vector temps, not 10).
    nw4r::math::VEC3Scale(tmp, dir, 0.033333335f * f31);
    nw4r::math::VEC3Add(e->v[5], e->v[5], tmp);
tmp.x *= sLodBatch0VelocityScale * s32ToF_678((rand() % 100) + 100);
    tmp.y *= sLodBatch0VelocityScale * s32ToF_678((rand() % 100) + 100);
    tmp.z *= sLodBatch0VelocityScale * s32ToF_678((rand() % 100) + 100);

    // Integrate vel into v[4]/v[5] and damp both, clamping to +/- 0.3.
    nw4r::math::VEC3Add(e->v[4], e->v[4], tmp);
    nw4r::math::VEC3Scale(e->v[4], e->v[4], 1.0f - 0.02f * f31);
    if (e->v[4].x > 0.3f) e->v[4].x = 0.3f;
    if (e->v[4].y > 0.3f) e->v[4].y = 0.3f;
    if (e->v[4].z > 0.3f) e->v[4].z = 0.3f;
    if (e->v[4].x < -0.3f) e->v[4].x = -0.3f;
    if (e->v[4].y < -0.3f) e->v[4].y = -0.3f;
    if (e->v[4].z < -0.3f) e->v[4].z = -0.3f;
    nw4r::math::VEC3Scale(e->v[5], e->v[5], 1.0f - 0.02f * f31);
    if (e->v[5].x > 0.3f) e->v[5].x = 0.3f;
    if (e->v[5].y > 0.3f) e->v[5].y = 0.3f;
    if (e->v[5].z > 0.3f) e->v[5].z = 0.3f;
    if (e->v[5].x < -0.3f) e->v[5].x = -0.3f;
    if (e->v[5].y < -0.3f) e->v[5].y = -0.3f;
    if (e->v[5].z < -0.3f) e->v[5].z = -0.3f;

    // Advance v[1]/v[2] along v[4]/v[5].
    nw4r::math::VEC3Scale(tmp, e->v[4], f31);
    nw4r::math::VEC3Add(e->v[1], e->v[1], tmp);
    nw4r::math::VEC3Scale(tmp, e->v[5], f31);
    nw4r::math::VEC3Add(e->v[2], e->v[2], tmp);

    // Orientation matrix A from the (v[2] - v[0]) axis against the Y axis.
    ml::CVec3 axis;
    nw4r::math::VEC3Sub(axis, e->v[2], e->v[0]);
    f32 lenA = axis.y * axis.y + axis.x * axis.x + axis.z * axis.z;
    if (lenA == 0.0f) {
        axis = ml::CVec3::zero;
    } else {
        PSVECNormalize(axis, axis);
    }
    f32 dotA = nw4r::math::VEC3Dot(axis, ml::CVec3::unitY);
    Quaternion quat;
    if (dotA < -0.9999f) {
        quat.x = 0.0f;
        quat.y = 1.0f;
        quat.z = 0.0f;
        quat.w = 0.0f;
    } else {
        ml::CVec3 cross;
        PSVECCrossProduct(ml::CVec3::unitY, axis, cross);
        if (2.0f * (1.0f + dotA) < 0.0f) {
            nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
        }
        f32 w = 0.0f;
        f32 x = 2.0f * (1.0f + dotA);
        if (x > 0.0f) {
            w = x * nw4r::math::FrSqrt(x);
        }
        f32 inv = 1.0f / w;
        quat.x = cross.x * inv;
        quat.y = cross.y * inv;
        quat.z = cross.z * inv;
        quat.w = 0.5f * w;
    }
    PSMTXQuat(e->mtxA, &quat);

    // Orientation matrix B from the (v[1] - v[0]) axis against the Y axis.
    nw4r::math::VEC3Sub(axis, e->v[1], e->v[0]);
    f32 lenB = axis.y * axis.y + axis.x * axis.x + axis.z * axis.z;
    if (lenB == 0.0f) {
        axis = ml::CVec3::zero;
    } else {
        PSVECNormalize(axis, axis);
    }
    f32 dotB = nw4r::math::VEC3Dot(axis, ml::CVec3::unitY);
    if (dotB < -0.9999f) {
        quat.x = 0.0f;
        quat.y = 1.0f;
        quat.z = 0.0f;
        quat.w = 0.0f;
    } else {
        ml::CVec3 cross;
        PSVECCrossProduct(ml::CVec3::unitY, axis, cross);
        if (2.0f * (1.0f + dotB) < 0.0f) {
            nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
        }
        f32 w = 0.0f;
        f32 x = 2.0f * (1.0f + dotB);
        if (x > 0.0f) {
            w = x * nw4r::math::FrSqrt(x);
        }
        f32 inv = 1.0f / w;
        quat.x = cross.x * inv;
        quat.y = cross.y * inv;
        quat.z = cross.z * inv;
        quat.w = 0.5f * w;
    }
    PSMTXQuat(e->mtxB, &quat);

    e->v[3] = ml::CVec3::zero;
}

// ---------------------------------------------------------------------------
// updateLodBatch1: per-frame update of one 0xb4 element (batches 8-15).  Same
// skeleton as updateLodBatch0, but with a fourth rand() draw, anchors pulled
// toward (0, 2-i, 0), a different random velocity constant, and the
// orientation matrices built against a local (0, -1, 0) vector instead of
// the global Y axis.
// ---------------------------------------------------------------------------
void LOD::UnkClass_8046A530::updateLodBatch1(u8* elem, f32 c) {
    LODLayout_8046A530* l = reinterpret_cast<LODLayout_8046A530*>(this);
    LODElement_8046A530* e = reinterpret_cast<LODElement_8046A530*>(elem);

    f32 f31 = l->field_0x1210 * c;
    f32 f30 = l->field_0x1208 * (0.002f * s32ToF_678(rand() % 500));
    f32 a2 = l->field_0x1210 * (0.4f * (0.002f * s32ToF_678(rand() % 500) - 0.5f));
    f32 a3 = l->field_0x1210 * (0.4f * (0.002f * s32ToF_678(rand() % 500) - 0.5f));
    f32 a4 = l->field_0x1210 * (0.4f * (0.002f * s32ToF_678(rand() % 500) - 0.5f));

    ml::CVec3 tmp;
    tmp.x = a4;
    tmp.y = a3;
    tmp.z = a2;
    nw4r::math::VEC3Add(e->v[6], e->v[6], tmp);
    f32 len2 = e->v[6].y * e->v[6].y + e->v[6].x * e->v[6].x + e->v[6].z * e->v[6].z;
    if (len2 == 0.0f) {
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
        f32 m1 = mag - 1.0f;
        bool big = mag >= 0.0001f;
        f32 f6 = 0.4f * m1;
        ml::CVec3 tdiff;
        nw4r::math::VEC3Sub(tdiff, e->v[i + 3], e->v[i + 4]);
        f32 dot = nw4r::math::VEC3Dot(tdiff, diff);
        f32 f4 = big ? 1.0f / mag : 10000.0f;
        f32 s = f4 * (0.5f * dot);
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
        tmp2.y = s32ToF_678(2 - i) - e->v[i].y;
        tmp2.z = -e->v[i].z;
        f32 mag = PSVECMag(tmp2);
        if (mag >= 0.0001f) {
            f32 inv = 1.0f / mag;
            f32 s = 0.13f * mag * f31;
            nw4r::math::VEC3Scale(tmp2, tmp2, inv);
            nw4r::math::VEC3Scale(tmp2, tmp2, s);
            nw4r::math::VEC3Add(e->v[i + 3], e->v[i + 3], tmp2);
        }
    }

    // Reuse the top temp for the random velocity like retail (saves a stack
    // slot - retail allocates 9 vector temps, not 10).
    nw4r::math::VEC3Scale(tmp, dir, 0.033333335f * f31);
    nw4r::math::VEC3Add(e->v[5], e->v[5], tmp);
tmp.x *= sLodBatch1VelocityScale * s32ToF_678((rand() % 100) + 100);
    tmp.y *= sLodBatch1VelocityScale * s32ToF_678((rand() % 100) + 100);
    tmp.z *= sLodBatch1VelocityScale * s32ToF_678((rand() % 100) + 100);

    nw4r::math::VEC3Add(e->v[4], e->v[4], tmp);
    nw4r::math::VEC3Scale(e->v[4], e->v[4], 1.0f - 0.02f * f31);
    if (e->v[4].x > 0.3f) e->v[4].x = 0.3f;
    if (e->v[4].y > 0.3f) e->v[4].y = 0.3f;
    if (e->v[4].z > 0.3f) e->v[4].z = 0.3f;
    if (e->v[4].x < -0.3f) e->v[4].x = -0.3f;
    if (e->v[4].y < -0.3f) e->v[4].y = -0.3f;
    if (e->v[4].z < -0.3f) e->v[4].z = -0.3f;
    nw4r::math::VEC3Scale(e->v[5], e->v[5], 1.0f - 0.02f * f31);
    if (e->v[5].x > 0.3f) e->v[5].x = 0.3f;
    if (e->v[5].y > 0.3f) e->v[5].y = 0.3f;
    if (e->v[5].z > 0.3f) e->v[5].z = 0.3f;
    if (e->v[5].x < -0.3f) e->v[5].x = -0.3f;
    if (e->v[5].y < -0.3f) e->v[5].y = -0.3f;
    if (e->v[5].z < -0.3f) e->v[5].z = -0.3f;

    nw4r::math::VEC3Scale(tmp, e->v[4], f31);
    nw4r::math::VEC3Add(e->v[1], e->v[1], tmp);
    nw4r::math::VEC3Scale(tmp, e->v[5], f31);
    nw4r::math::VEC3Add(e->v[2], e->v[2], tmp);

    // Local "up" vector for the orientation rebuilds.
    ml::CVec3 up(0.0f, -1.0f, 0.0f);

    ml::CVec3 axis;
    nw4r::math::VEC3Sub(axis, e->v[2], e->v[0]);
    f32 lenA = axis.y * axis.y + axis.x * axis.x + axis.z * axis.z;
    if (lenA == 0.0f) {
        axis = ml::CVec3::zero;
    } else {
        PSVECNormalize(axis, axis);
    }
    f32 dotA = nw4r::math::VEC3Dot(up, axis);
    Quaternion quat;
    if (dotA < -0.9999f) {
        quat.x = 0.0f;
        quat.y = 1.0f;
        quat.z = 0.0f;
        quat.w = 0.0f;
    } else {
        ml::CVec3 cross;
        PSVECCrossProduct(up, axis, cross);
        if (2.0f * (1.0f + dotA) < 0.0f) {
            nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
        }
        f32 w = 0.0f;
        f32 x = 2.0f * (1.0f + dotA);
        if (x > 0.0f) {
            w = x * nw4r::math::FrSqrt(x);
        }
        f32 inv = 1.0f / w;
        quat.x = cross.x * inv;
        quat.y = cross.y * inv;
        quat.z = cross.z * inv;
        quat.w = 0.5f * w;
    }
    PSMTXQuat(e->mtxA, &quat);

    // Both orientation rebuilds reuse one axis temp like retail.
    nw4r::math::VEC3Sub(axis, e->v[1], e->v[0]);
    f32 lenB = axis.y * axis.y + axis.x * axis.x + axis.z * axis.z;
    if (lenB == 0.0f) {
        axis = ml::CVec3::zero;
    } else {
        PSVECNormalize(axis, axis);
    }
    f32 dotB = nw4r::math::VEC3Dot(up, axis);
    if (dotB < -0.9999f) {
        quat.x = 0.0f;
        quat.y = 1.0f;
        quat.z = 0.0f;
        quat.w = 0.0f;
    } else {
        ml::CVec3 cross;
        PSVECCrossProduct(up, axis, cross);
        if (2.0f * (1.0f + dotB) < 0.0f) {
            nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
        }
        f32 w = 0.0f;
        f32 x = 2.0f * (1.0f + dotB);
        if (x > 0.0f) {
            w = x * nw4r::math::FrSqrt(x);
        }
        f32 inv = 1.0f / w;
        quat.x = cross.x * inv;
        quat.y = cross.y * inv;
        quat.z = cross.z * inv;
        quat.w = 0.5f * w;
    }
    PSMTXQuat(e->mtxB, &quat);

    e->v[3] = ml::CVec3::zero;
}

// ---------------------------------------------------------------------------
// updateLodBatch2: per-frame update of one 0xb4 element (batches 16-23).
// Byte-identical skeleton to updateLodBatch0, but the orientation matrices are
// built against the global X axis instead of the Y axis.
// ---------------------------------------------------------------------------
void LOD::UnkClass_8046A530::updateLodBatch2(u8* elem, f32 c) {
    LODLayout_8046A530* l = reinterpret_cast<LODLayout_8046A530*>(this);
    LODElement_8046A530* e = reinterpret_cast<LODElement_8046A530*>(elem);

    f32 f31 = l->field_0x1210 * c;
    f32 f30 = l->field_0x1208 * (0.002f * s32ToF_678(rand() % 500));
    f32 a2 = l->field_0x1210 * (0.4f * (0.002f * s32ToF_678(rand() % 500) - 0.5f));
    f32 a3 = l->field_0x1210 * (0.4f * (0.002f * s32ToF_678(rand() % 500) - 0.5f));

    ml::CVec3 tmp;
    tmp.x = a3;
    tmp.y = 0.0f;
    tmp.z = a2;
    nw4r::math::VEC3Add(e->v[6], e->v[6], tmp);
    f32 len2 = e->v[6].y * e->v[6].y + e->v[6].x * e->v[6].x + e->v[6].z * e->v[6].z;
    if (len2 == 0.0f) {
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
        f32 m1 = mag - 1.0f;
        bool big = mag >= 0.0001f;
        f32 f6 = 0.4f * m1;
        ml::CVec3 tdiff;
        nw4r::math::VEC3Sub(tdiff, e->v[i + 3], e->v[i + 4]);
        f32 dot = nw4r::math::VEC3Dot(tdiff, diff);
        f32 f4 = big ? 1.0f / mag : 10000.0f;
        f32 s = f4 * (0.5f * dot);
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
        tmp2.y = s32ToF_678(i) - e->v[i].y;
        tmp2.z = -e->v[i].z;
        f32 mag = PSVECMag(tmp2);
        if (mag >= 0.0001f) {
            f32 inv = 1.0f / mag;
            f32 s = 0.13f * mag * f31;
            nw4r::math::VEC3Scale(tmp2, tmp2, inv);
            nw4r::math::VEC3Scale(tmp2, tmp2, s);
            nw4r::math::VEC3Add(e->v[i + 3], e->v[i + 3], tmp2);
        }
    }

    // Reuse the top temp for the random velocity like retail (saves a stack
    // slot - retail allocates 9 vector temps, not 10).
    nw4r::math::VEC3Scale(tmp, dir, 0.033333335f * f31);
    nw4r::math::VEC3Add(e->v[5], e->v[5], tmp);
tmp.x *= sLodBatch2VelocityScales[0] * s32ToF_678((rand() % 100) + 100);
    tmp.y *= sLodBatch2VelocityScales[0] * s32ToF_678((rand() % 100) + 100);
    tmp.z *= sLodBatch2VelocityScales[0] * s32ToF_678((rand() % 100) + 100);

    nw4r::math::VEC3Add(e->v[4], e->v[4], tmp);
    nw4r::math::VEC3Scale(e->v[4], e->v[4], 1.0f - 0.02f * f31);
    if (e->v[4].x > 0.3f) e->v[4].x = 0.3f;
    if (e->v[4].y > 0.3f) e->v[4].y = 0.3f;
    if (e->v[4].z > 0.3f) e->v[4].z = 0.3f;
    if (e->v[4].x < -0.3f) e->v[4].x = -0.3f;
    if (e->v[4].y < -0.3f) e->v[4].y = -0.3f;
    if (e->v[4].z < -0.3f) e->v[4].z = -0.3f;
    nw4r::math::VEC3Scale(e->v[5], e->v[5], 1.0f - 0.02f * f31);
    if (e->v[5].x > 0.3f) e->v[5].x = 0.3f;
    if (e->v[5].y > 0.3f) e->v[5].y = 0.3f;
    if (e->v[5].z > 0.3f) e->v[5].z = 0.3f;
    if (e->v[5].x < -0.3f) e->v[5].x = -0.3f;
    if (e->v[5].y < -0.3f) e->v[5].y = -0.3f;
    if (e->v[5].z < -0.3f) e->v[5].z = -0.3f;

    nw4r::math::VEC3Scale(tmp, e->v[4], f31);
    nw4r::math::VEC3Add(e->v[1], e->v[1], tmp);
    nw4r::math::VEC3Scale(tmp, e->v[5], f31);
    nw4r::math::VEC3Add(e->v[2], e->v[2], tmp);

    // Orientation matrices against the global X axis.
    ml::CVec3 axis;
    nw4r::math::VEC3Sub(axis, e->v[2], e->v[0]);
    f32 lenA = axis.y * axis.y + axis.x * axis.x + axis.z * axis.z;
    if (lenA == 0.0f) {
        axis = ml::CVec3::zero;
    } else {
        PSVECNormalize(axis, axis);
    }
    f32 dotA = nw4r::math::VEC3Dot(axis, ml::CVec3::unitX);
    Quaternion quat;
    if (dotA < -0.9999f) {
        quat.x = 0.0f;
        quat.y = 1.0f;
        quat.z = 0.0f;
        quat.w = 0.0f;
    } else {
        ml::CVec3 cross;
        PSVECCrossProduct(ml::CVec3::unitX, axis, cross);
        if (2.0f * (1.0f + dotA) < 0.0f) {
            nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
        }
        f32 w = 0.0f;
        f32 x = 2.0f * (1.0f + dotA);
        if (x > 0.0f) {
            w = x * nw4r::math::FrSqrt(x);
        }
        f32 inv = 1.0f / w;
        quat.x = cross.x * inv;
        quat.y = cross.y * inv;
        quat.z = cross.z * inv;
        quat.w = 0.5f * w;
    }
    PSMTXQuat(e->mtxA, &quat);

    // Both orientation rebuilds reuse one axis temp like retail.
    nw4r::math::VEC3Sub(axis, e->v[1], e->v[0]);
    f32 lenB = axis.y * axis.y + axis.x * axis.x + axis.z * axis.z;
    if (lenB == 0.0f) {
        axis = ml::CVec3::zero;
    } else {
        PSVECNormalize(axis, axis);
    }
    f32 dotB = nw4r::math::VEC3Dot(axis, ml::CVec3::unitX);
    if (dotB < -0.9999f) {
        quat.x = 0.0f;
        quat.y = 1.0f;
        quat.z = 0.0f;
        quat.w = 0.0f;
    } else {
        ml::CVec3 cross;
        PSVECCrossProduct(ml::CVec3::unitX, axis, cross);
        if (2.0f * (1.0f + dotB) < 0.0f) {
            nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
        }
        f32 w = 0.0f;
        f32 x = 2.0f * (1.0f + dotB);
        if (x > 0.0f) {
            w = x * nw4r::math::FrSqrt(x);
        }
        f32 inv = 1.0f / w;
        quat.x = cross.x * inv;
        quat.y = cross.y * inv;
        quat.z = cross.z * inv;
        quat.w = 0.5f * w;
    }
    PSMTXQuat(e->mtxB, &quat);

    e->v[3] = ml::CVec3::zero;
}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.sdata] 0x80663818-0x80663828 (16B): three 0.005f velocity constants +
// trailing pad word (non-const so MWCC places them in .sdata, not .sdata2).
// name-guess: .sdata 0x80663818, read only by updateLodBatch0 as its random
// velocity scale.
f32 sLodBatch0VelocityScale = 0.005f;
// name-guess: .sdata 0x8066381C, read only by updateLodBatch1 as its random
// velocity scale.
f32 sLodBatch1VelocityScale = 0.005f;
// name-guess: .sdata 0x80663820 (2 words), read only by updateLodBatch2 for
// its random velocity scale; the trailing word is the retail slot tail.
f32 sLodBatch2VelocityScales[2] = { 0.005f, 0.0f };
