// monolib effect matrix utilities (retail EU: code_804DB938).
//
// Table-driven sin/cos trigonometry (1-degree entries with linear
// interpolation), axis/euler-angle -> rotation matrix builders for all six
// rotation orders, the sin/cos table initializer, an axis remap table and
// paired-single vector lerps.

#include <harness_catalog.h>
#include <revolution/MTX.h>
#include <monolib/math/CMat34.hpp>
#include <monolib/math/CVec4.hpp>
#include <nw4r/math.h>
#include <math.h>
#include "monolib/math/FloatUtils.hpp"  // H3 label-owner decl (lbl_eu_8066A208)

// ---------------------------------------------------------------------------
// Sin/cos lookup tables: 360 entries of {value, delta} in BSS, built at
// runtime by func_804DD754. Entry i holds sin/cos(i degrees) and the slope
// to entry i+1, so fractional degrees are linearly interpolated.
// ---------------------------------------------------------------------------
struct CESinCosEntry {
    f32 value;
    f32 delta;
};

extern "C" {
extern CESinCosEntry lbl_eu_80660038[360]; // sin table
extern CESinCosEntry lbl_eu_80660B78[360]; // cos table
}

// Retail sdata2 literals (named from the EU map). const so MWCC hoists the
// loads like retail (cf. MWCC_CASES extern-const float hoist note).
extern const f32 lbl_eu_8066B260; // 57.29578f (180/pi)
extern f32 lbl_eu_8066B270; // 1.0f
extern f32 lbl_eu_8066B274; // 0.0f
extern const f32 lbl_eu_8066B278; // pi/180
extern f64 lbl_eu_8066B280; // 0x4330000080000000 (2^52+2^31), pool copy used
                            // by the table initializer's int->float path

// 0x4330000080000000 (2^52 + 2^31): MWCC's signed int -> float conversion
// magic. Retail keeps copies in .sdata2 (lbl_eu_8066B268 / lbl_eu_8066B280);
// the table initializer's conversion binds to lbl_eu_8066B280.

// Table based sincos + rotation build.
static void CERotTrig(Mtx m, f32 rad, char axis) {
    f32 deg = lbl_eu_8066B260 * rad;
    int n = (int)deg;
    f32 frac = deg - (f32)n;
    int idx = n % 360;
    if (idx < 0) {
        idx += 360;
    }
    const CESinCosEntry* cosE = &lbl_eu_80660B78[idx];
    const CESinCosEntry* sinE = &lbl_eu_80660038[idx];
    PSMTXRotTrig(m, frac * sinE->delta + sinE->value,
                 frac * cosE->delta + cosE->value, axis);
}

// idx-first variant of CERotTrig: identical body, but declaring the %360
// result before the (float)n cast flips MWCC's scratch coloring for callers
// whose register context needs mulhw r3 / xoris r0 (func_804DD0A0).
static void CERotTrigIdxFirst(Mtx m, f32 rad, char axis) {
    f32 deg = lbl_eu_8066B260 * rad;
    int n = (int)deg;
    int idx = n % 360;
    f32 frac = deg - (f32)n;
    if (idx < 0) {
        idx += 360;
    }
    const CESinCosEntry* cosE = &lbl_eu_80660B78[idx];
    const CESinCosEntry* sinE = &lbl_eu_80660038[idx];
    PSMTXRotTrig(m, frac * sinE->delta + sinE->value,
                 frac * cosE->delta + cosE->value, axis);
}

// z-axis idx-first clone: distinct call site text so MWCC schedules the
// inlined body independently in func_804DCA88's tail.
static void CERotTrigIdxFirstZ(Mtx m, f32 rad) {
    f32 deg = lbl_eu_8066B260 * rad;
    int n = (int)deg;
    int idx = n % 360;
    f32 frac = deg - (f32)n;
    if (idx < 0) {
        idx += 360;
    }
    const CESinCosEntry* cosE = &lbl_eu_80660B78[idx];
    const CESinCosEntry* sinE = &lbl_eu_80660038[idx];
    PSMTXRotTrig(m, frac * sinE->delta + sinE->value,
                 frac * cosE->delta + cosE->value, 'z');
}

// y-axis idx-first clone: distinct call site text so MWCC schedules the
// inlined body independently in func_804DCD94's tail (cf. CERotTrigIdxFirstZ).
static void CERotTrigIdxFirstY(Mtx m, f32 rad) {
    f32 deg = lbl_eu_8066B260 * rad;
    int n = (int)deg;
    int idx = n % 360;
    f32 frac = deg - (f32)n;
    if (idx < 0) {
        idx += 360;
    }
    const CESinCosEntry* cosE = &lbl_eu_80660B78[idx];
    const CESinCosEntry* sinE = &lbl_eu_80660038[idx];
    PSMTXRotTrig(m, frac * sinE->delta + sinE->value,
                 frac * cosE->delta + cosE->value, 'y');
}

// x-axis idx-first clone: distinct call site text so MWCC schedules the
// inlined body independently (cf. CERotTrigIdxFirstZ/Y).
static void CERotTrigIdxFirstX(Mtx m, f32 rad) {
    f32 deg = lbl_eu_8066B260 * rad;
    int n = (int)deg;
    int idx = n % 360;
    f32 frac = deg - (f32)n;
    if (idx < 0) {
        idx += 360;
    }
    const CESinCosEntry* cosE = &lbl_eu_80660B78[idx];
    const CESinCosEntry* sinE = &lbl_eu_80660038[idx];
    PSMTXRotTrig(m, frac * sinE->delta + sinE->value,
                 frac * cosE->delta + cosE->value, 'x');
}

// Third-rotation variants for func_804DB980: commuted multiply (rad first in
// source, constant fetched last by MWCC's operand evaluator). Retail fetches
// the sdata2 constants before the angle only in the third rotation of each
// case; commuting the operands is the lever that reorders the float loads.
static void CERotTrigThirdZ(Mtx m, f32 rad) {
    f32 deg = rad * lbl_eu_8066B260;
    int n = (int)deg;
    int idx = n % 360;
    f32 frac = deg - (f32)n;
    if (idx < 0) {
        idx += 360;
    }
    const CESinCosEntry* cosE = &lbl_eu_80660B78[idx];
    const CESinCosEntry* sinE = &lbl_eu_80660038[idx];
    PSMTXRotTrig(m, frac * sinE->delta + sinE->value,
                 frac * cosE->delta + cosE->value, 'z');
}

static void CERotTrigThirdY(Mtx m, f32 rad) {
    f32 deg = rad * lbl_eu_8066B260;
    int n = (int)deg;
    int idx = n % 360;
    f32 frac = deg - (f32)n;
    if (idx < 0) {
        idx += 360;
    }
    const CESinCosEntry* cosE = &lbl_eu_80660B78[idx];
    const CESinCosEntry* sinE = &lbl_eu_80660038[idx];
    PSMTXRotTrig(m, frac * sinE->delta + sinE->value,
                 frac * cosE->delta + cosE->value, 'y');
}

static void CERotTrigThirdX(Mtx m, f32 rad) {
    f32 deg = rad * lbl_eu_8066B260;
    int n = (int)deg;
    int idx = n % 360;
    f32 frac = deg - (f32)n;
    if (idx < 0) {
        idx += 360;
    }
    const CESinCosEntry* cosE = &lbl_eu_80660B78[idx];
    const CESinCosEntry* sinE = &lbl_eu_80660038[idx];
    PSMTXRotTrig(m, frac * sinE->delta + sinE->value,
                 frac * cosE->delta + cosE->value, 'x');
}

// ---------------------------------------------------------------------------
// Cardinal axis rotation builders.
// ---------------------------------------------------------------------------

extern "C" {

// Cardinal axis rotation builders: build a 4x4 matrix rotating about the
// given axis by `rad` radians (table-interpolated sin/cos).
void func_804DD388(Mtx mtx, f32 rad) {
    f32 deg = lbl_eu_8066B260 * rad;
    int n = (int)deg;
    int idx = n % 360;
    f32 frac = deg - (f32)n;
    if (idx < 0) {
        idx += 360;
    }
    const CESinCosEntry* cosE = &lbl_eu_80660B78[idx];
    const CESinCosEntry* sinE = &lbl_eu_80660038[idx];
    PSMTXRotTrig(mtx, frac * sinE->delta + sinE->value,
                 frac * cosE->delta + cosE->value, 'x');
}

// Dedicated y-axis helper: MWCC's inliner schedules the body differently
// than textual source in func_804DD440.
static void CERotTrigY(Mtx m, f32 rad) {
    f32 deg = lbl_eu_8066B260 * rad;
    int n = (int)deg;
    int idx = n % 360;
    f32 frac = deg - (f32)n;
    if (idx < 0) {
        idx += 360;
    }
    const CESinCosEntry* cosE = &lbl_eu_80660B78[idx];
    const CESinCosEntry* sinE = &lbl_eu_80660038[idx];
    PSMTXRotTrig(m, frac * sinE->delta + sinE->value,
                 frac * cosE->delta + cosE->value, 'y');
}

void func_804DD440(Mtx mtx, f32 rad) {
    CERotTrigY(mtx, rad);
}

void func_804DD4F8(Mtx mtx, f32 rad) {
    // z-axis retail body orders the %360 before the (float)n cast, flipping
    // the mulhw/xoris scratch coloring vs the x/y builders.
    CERotTrigIdxFirst(mtx, rad, 'z');
}

// mtx = RotY(rad) * mtx
void func_804DD5B0(ml::CMat34* mtx, f32 rad) {
    ml::CMat34 rot;
    ml::CMat34 result;
    // idx-first variant: retail colors mulhw->r3 / xoris->r0 here.
    CERotTrigIdxFirst(rot.mtx, rad, 'y');
    PSMTXConcat(rot.mtx, mtx->mtx, result.mtx);
    *mtx = result;
}

// ---------------------------------------------------------------------------
// Single-axis rotation from an "axis vector": exactly one non-zero component
// selects the axis, its value is the angle. Unit-length vectors mean "no
// rotation" (identity). Returns false when the vector is not single-axis.
// ---------------------------------------------------------------------------

int func_804DD0A0(Mtx mtx, const Vec* v) {
    // NOTE: uses the idx-first trig variant (mulhw r3 / xoris r0 like
    // retail); shared CERotTrig stays frac-first for the cardinal builders.
    // func_804DCA88/DCD94 use it too.
    f32 mag = PSVECMag(v);
    bool nearOne = ml::math::abs(mag - lbl_eu_8066B270) <= lbl_eu_8066A208;
    if (nearOne) {
        PSMTXIdentity(mtx);
        return 1;
    }
    if (v->x != lbl_eu_8066B274 && v->y == lbl_eu_8066B274 &&
        v->z == lbl_eu_8066B274) {
        CERotTrigIdxFirst(mtx, v->x, 'x');
        return 1;
    }
    if (v->x == lbl_eu_8066B274 && v->y != lbl_eu_8066B274 &&
        v->z == lbl_eu_8066B274) {
        CERotTrigIdxFirst(mtx, v->y, 'y');
        return 1;
    }
    if (v->x == lbl_eu_8066B274 && v->y == lbl_eu_8066B274 &&
        v->z != lbl_eu_8066B274) {
        CERotTrigIdxFirst(mtx, v->z, 'z');
        return 1;
    }
    return 0;
}

// ---------------------------------------------------------------------------
// Euler angle -> matrix builders. The angle vector is first tried as a
// single-axis rotation; otherwise three cardinal rotations are accumulated:
//   mtx = R3(a3) * (R2(a2) * R1(a1))
// ---------------------------------------------------------------------------

// Order X, Y, Z.
// NOTE: idx-first trig variant — this caller's register context needs
// mulhw r3 / xoris r0 like retail (see func_804DD0A0).
void func_804DCA88(ml::CMat34* mtx, const Vec* ang) {
    // Retail frame slots run work(0x98) > rotY(0x68) > work2(0x38) > rotZ(0x8):
    // first-declared local takes the highest slot.
    ml::CMat34 work;
    ml::CMat34 rotY;
    ml::CMat34 work2;
    ml::CMat34 rotZ;
    if (func_804DD0A0(mtx->mtx, ang) != 0) {
        return;
    }
    CERotTrigIdxFirst(mtx->mtx, ang->x, 'x');
    CERotTrigIdxFirst(rotY.mtx, ang->y, 'y');
    PSMTXConcat(rotY.mtx, mtx->mtx, work.mtx);
    *mtx = work;
    f32 az = ang->z;
    CERotTrigIdxFirstZ(rotZ.mtx, az);
    PSMTXConcat(rotZ.mtx, mtx->mtx, work2.mtx);
    *mtx = work2;
}

// Order Z, X, Y.
// idx-first trig variant, same reason as func_804DCA88.
void func_804DCD94(ml::CMat34* mtx, const Vec* ang) {
    // Retail frame slots run work(0x98) > rotX(0x68) > work2(0x38) > rotY(0x8):
    // first-declared local takes the highest slot. Plain Mtx locals: copying
    // the raw array pairs load/store words in retail's order.
    ml::CMat34 work;
    ml::CMat34 rotX;
    ml::CMat34 work2;
    ml::CMat34 rotY;
    if (func_804DD0A0(mtx->mtx, ang) != 0) {
        return;
    }
    CERotTrigIdxFirst(mtx->mtx, ang->z, 'z');
    CERotTrigIdxFirst(rotX.mtx, ang->x, 'x');
    PSMTXConcat(rotX.mtx, mtx->mtx, work.mtx);
    *mtx = work;
    f32 ay = ang->y;
    // Third (y) rotation uses the idx-first trig variant like blocks 1-2.
    CERotTrigIdxFirstY(rotY.mtx, ay);
    PSMTXConcat(rotY.mtx, mtx->mtx, work2.mtx);
    *mtx = work2;
}

// All six euler rotation orders:
//   0: X,Y,Z   1: X,Z,Y   2: Y,X,Z   3: Y,Z,X   4: Z,X,Y   5: Z,Y-X
// Retail is a compare-chain dispatch (cmpwi/beq per case); each case builds
// R3 * (R2 * R1) with the idx-first table trig (mulhw r3 / xoris r0
// coloring, cf. func_804DCA88).
void func_804DB980(ml::CMat34* mtx, const Vec* ang, int order) {
    switch (order) {
    case 0: { // X,Y,Z
        if (func_804DD0A0(mtx->mtx, ang) != 0) {
            return;
        }
        CERotTrigIdxFirst(mtx->mtx, ang->x, 'x');
        // Slot order (first-declared = highest): rotZ 0x458, work2 0x428,
        // rotY 0x3f8, work 0x3c8.
        ml::CMat34 rotZ;
        ml::CMat34 work2;
        ml::CMat34 rotY;
        ml::CMat34 work;
        CERotTrigIdxFirst(rotY.mtx, ang->y, 'y');
        PSMTXConcat(rotY.mtx, mtx->mtx, work.mtx);
        *mtx = work;
        f32 az = ang->z;
        CERotTrigThirdZ(rotZ.mtx, az);
        PSMTXConcat(rotZ.mtx, mtx->mtx, work2.mtx);
        *mtx = work2;
        break;
    }
    case 1: { // X,Z,Y
        if (func_804DD0A0(mtx->mtx, ang) != 0) {
            return;
        }
        CERotTrigIdxFirst(mtx->mtx, ang->x, 'x');
        ml::CMat34 rotY;
        ml::CMat34 work2;
        ml::CMat34 rotZ;
        ml::CMat34 work;
        CERotTrigIdxFirst(rotZ.mtx, ang->z, 'z');
        PSMTXConcat(rotZ.mtx, mtx->mtx, work.mtx);
        *mtx = work;
        f32 ay = ang->y;
        CERotTrigThirdY(rotY.mtx, ay);
        PSMTXConcat(rotY.mtx, mtx->mtx, work2.mtx);
        *mtx = work2;
        break;
    }
    case 2: { // Y,X,Z
        if (func_804DD0A0(mtx->mtx, ang) != 0) {
            return;
        }
        CERotTrigIdxFirst(mtx->mtx, ang->y, 'y');
        ml::CMat34 rotZ;
        ml::CMat34 work2;
        ml::CMat34 rotX;
        ml::CMat34 work;
        CERotTrigIdxFirst(rotX.mtx, ang->x, 'x');
        PSMTXConcat(rotX.mtx, mtx->mtx, work.mtx);
        *mtx = work;
        f32 az = ang->z;
        CERotTrigThirdZ(rotZ.mtx, az);
        PSMTXConcat(rotZ.mtx, mtx->mtx, work2.mtx);
        *mtx = work2;
        break;
    }
    case 3: { // Y,Z,X
        if (func_804DD0A0(mtx->mtx, ang) != 0) {
            return;
        }
        CERotTrigIdxFirst(mtx->mtx, ang->y, 'y');
        ml::CMat34 rotX;
        ml::CMat34 work2;
        ml::CMat34 rotZ;
        ml::CMat34 work;
        CERotTrigIdxFirst(rotZ.mtx, ang->z, 'z');
        PSMTXConcat(rotZ.mtx, mtx->mtx, work.mtx);
        *mtx = work;
        f32 ax = ang->x;
        CERotTrigThirdX(rotX.mtx, ax);
        PSMTXConcat(rotX.mtx, mtx->mtx, work2.mtx);
        *mtx = work2;
        break;
    }
    case 4: { // Z,X,Y
        if (func_804DD0A0(mtx->mtx, ang) != 0) {
            return;
        }
        CERotTrigIdxFirst(mtx->mtx, ang->z, 'z');
        ml::CMat34 rotY;
        ml::CMat34 work2;
        ml::CMat34 rotX;
        ml::CMat34 work;
        CERotTrigIdxFirst(rotX.mtx, ang->x, 'x');
        PSMTXConcat(rotX.mtx, mtx->mtx, work.mtx);
        *mtx = work;
        f32 ay = ang->y;
        CERotTrigThirdY(rotY.mtx, ay);
        PSMTXConcat(rotY.mtx, mtx->mtx, work2.mtx);
        *mtx = work2;
        break;
    }
    case 5: { // Z,Y,X
        if (func_804DD0A0(mtx->mtx, ang) != 0) {
            return;
        }
        CERotTrigIdxFirst(mtx->mtx, ang->z, 'z');
        ml::CMat34 rotX;
        ml::CMat34 work2;
        ml::CMat34 rotY;
        ml::CMat34 work;
        CERotTrigIdxFirst(rotY.mtx, ang->y, 'y');
        PSMTXConcat(rotY.mtx, mtx->mtx, work.mtx);
        *mtx = work;
        f32 ax = ang->x;
        CERotTrigThirdX(rotX.mtx, ax);
        PSMTXConcat(rotX.mtx, mtx->mtx, work2.mtx);
        *mtx = work2;
        break;
    }
    }
}

// Axis order remap used by the effect system.
int func_804DD6E8(int val) {
    switch (val) {
        case 0: return 5;
        case 1: return 3;
        case 2: return 4;
        case 3: return 1;
        case 4: return 2;
        case 5: return 0;
        default: return 0;
    }
}

// ---------------------------------------------------------------------------
// Sin/cos table initializer: fill value = sin/cos(i degrees) for every
// integer degree and delta = value[i+1] - value[i] (wrapping 359 -> 0).
// ---------------------------------------------------------------------------

void func_804DD754(void) {
    f32 prevSin;
    f32 prevCos;
    int i;
    for (i = 0; i < 360; i++) {
        lbl_eu_80660038[i].value = (f32)sin(lbl_eu_8066B278 * (f32)i);
        lbl_eu_80660B78[i].value = (f32)cos(lbl_eu_8066B278 * (f32)i);
        if (i != 0) {
            lbl_eu_80660038[i - 1].delta = lbl_eu_80660038[i].value - prevSin;
            lbl_eu_80660B78[i - 1].delta = lbl_eu_80660B78[i].value - prevCos;
        }
        prevSin = lbl_eu_80660038[i].value;
        prevCos = lbl_eu_80660B78[i].value;
    }
    lbl_eu_80660038[359].delta = lbl_eu_80660038[0].value - prevSin;
    lbl_eu_80660B78[359].delta = lbl_eu_80660B78[0].value - prevCos;
}

// ---------------------------------------------------------------------------
// Paired-single vector lerps: out = a + (b - a) * t.  The retail bodies are
// psq_l/ps_sub/ps_madds0 kernels that MWCC cannot emit from scalar C++ (the
// auto-vectorizer keeps lfs/fsubs/fmadds), so the Wii/MWCC build ships the
// retail kernel via the isolated PS backend; the scalar lerp below is the
// PC/NONMATCHING fallback.
// ---------------------------------------------------------------------------
#include "monolib/effect/code_804DB938_ps.inl"

} // extern "C"
