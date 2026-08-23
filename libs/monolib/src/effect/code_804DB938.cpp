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

// Retail sdata2 literals (named from the EU map).
extern f32 lbl_eu_8066B260; // 57.29578f (180/pi)
extern f32 lbl_eu_8066B270; // 1.0f
extern f32 lbl_eu_8066B274; // 0.0f
extern f32 lbl_eu_8066B278; // pi/180
extern f64 lbl_eu_8066B280; // 0x4330000080000000 (2^52+2^31), pool copy used
                            // by the table initializer's int->float path
extern f32 lbl_eu_8066A208; // 1e-6f

// 0x4330000080000000 (2^52 + 2^31): MWCC's signed int -> float conversion
// magic. Retail keeps it in .sdata2 at lbl_eu_8066B268 (same bytes as
// lbl_eu_8066A388); declared extern so loads emit the sda21 reloc.
extern f64 lbl_eu_8066B268;

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

// ---------------------------------------------------------------------------
// Cardinal axis rotation builders.
// ---------------------------------------------------------------------------

extern "C" {

// Cardinal axis rotation builders: build a 4x4 matrix rotating about the
// given axis by `rad` radians (table-interpolated sin/cos).
void func_804DD388(Mtx mtx, f32 rad) {
    CERotTrig(mtx, rad, 'x');
}

void func_804DD440(Mtx mtx, f32 rad) {
    CERotTrig(mtx, rad, 'y');
}

void func_804DD4F8(Mtx mtx, f32 rad) {
    CERotTrig(mtx, rad, 'z');
}

// mtx = RotY(rad) * mtx
void func_804DD5B0(ml::CMat34* mtx, f32 rad) {
    ml::CMat34 rot;
    ml::CMat34 result;
    CERotTrig(rot.mtx, rad, 'y');
    PSMTXConcat(rot.mtx, mtx->mtx, result.mtx);
    *mtx = result;
}

// ---------------------------------------------------------------------------
// Single-axis rotation from an "axis vector": exactly one non-zero component
// selects the axis, its value is the angle. Unit-length vectors mean "no
// rotation" (identity). Returns false when the vector is not single-axis.
// ---------------------------------------------------------------------------

int func_804DD0A0(Mtx mtx, const Vec* v) {
    f32 mag = PSVECMag(v);
    bool nearOne = ml::math::abs(mag - lbl_eu_8066B270) <= lbl_eu_8066A208;
    if (nearOne) {
        PSMTXIdentity(mtx);
        return 1;
    }
    if (v->x != lbl_eu_8066B274 && v->y == lbl_eu_8066B274 &&
        v->z == lbl_eu_8066B274) {
        CERotTrig(mtx, v->x, 'x');
        return 1;
    }
    if (v->x == lbl_eu_8066B274 && v->y != lbl_eu_8066B274 &&
        v->z == lbl_eu_8066B274) {
        CERotTrig(mtx, v->y, 'y');
        return 1;
    }
    if (v->x == lbl_eu_8066B274 && v->y == lbl_eu_8066B274 &&
        v->z != lbl_eu_8066B274) {
        CERotTrig(mtx, v->z, 'z');
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
void func_804DCA88(ml::CMat34* mtx, const Vec* ang) {
    if (func_804DD0A0(mtx->mtx, ang) != 0) {
        return;
    }
    CERotTrig(mtx->mtx, ang->x, 'x');
    ml::CMat34 rotY;
    CERotTrig(rotY.mtx, ang->y, 'y');
    ml::CMat34 work;
    PSMTXConcat(rotY.mtx, mtx->mtx, work.mtx);
    *mtx = work;
    ml::CMat34 rotZ;
    CERotTrig(rotZ.mtx, ang->z, 'z');
    ml::CMat34 work2;
    PSMTXConcat(rotZ.mtx, mtx->mtx, work2.mtx);
    *mtx = work2;
}

// Order Z, X, Y.
void func_804DCD94(ml::CMat34* mtx, const Vec* ang) {
    if (func_804DD0A0(mtx->mtx, ang) != 0) {
        return;
    }
    CERotTrig(mtx->mtx, ang->z, 'z');
    ml::CMat34 rotX;
    CERotTrig(rotX.mtx, ang->x, 'x');
    ml::CMat34 work;
    PSMTXConcat(rotX.mtx, mtx->mtx, work.mtx);
    *mtx = work;
    ml::CMat34 rotY;
    CERotTrig(rotY.mtx, ang->y, 'y');
    ml::CMat34 work2;
    PSMTXConcat(rotY.mtx, mtx->mtx, work2.mtx);
    *mtx = work2;
}

// All six euler rotation orders:
//   0: X,Y,Z   1: X,Z,Y   2: Y,X,Z   3: Y,Z,X   4: Z,X,Y   5: Z,Y,X
void func_804DB980(ml::CMat34* mtx, const Vec* ang, int order) {
    if (order == 0) {
        if (func_804DD0A0(mtx->mtx, ang) != 0) {
            return;
        }
        CERotTrig(mtx->mtx, ang->x, 'x');
        ml::CMat34 rotY;
        CERotTrig(rotY.mtx, ang->y, 'y');
        ml::CMat34 work;
        PSMTXConcat(rotY.mtx, mtx->mtx, work.mtx);
        *mtx = work;
        ml::CMat34 rotZ;
        CERotTrig(rotZ.mtx, ang->z, 'z');
        ml::CMat34 work2;
        PSMTXConcat(rotZ.mtx, mtx->mtx, work2.mtx);
        *mtx = work2;
    } else if (order == 1) {
        if (func_804DD0A0(mtx->mtx, ang) != 0) {
            return;
        }
        CERotTrig(mtx->mtx, ang->x, 'x');
        ml::CMat34 rotZ;
        CERotTrig(rotZ.mtx, ang->z, 'z');
        ml::CMat34 work;
        PSMTXConcat(rotZ.mtx, mtx->mtx, work.mtx);
        *mtx = work;
        ml::CMat34 rotY;
        CERotTrig(rotY.mtx, ang->y, 'y');
        ml::CMat34 work2;
        PSMTXConcat(rotY.mtx, mtx->mtx, work2.mtx);
        *mtx = work2;
    } else if (order == 2) {
        if (func_804DD0A0(mtx->mtx, ang) != 0) {
            return;
        }
        CERotTrig(mtx->mtx, ang->y, 'y');
        ml::CMat34 rotX;
        CERotTrig(rotX.mtx, ang->x, 'x');
        ml::CMat34 work;
        PSMTXConcat(rotX.mtx, mtx->mtx, work.mtx);
        *mtx = work;
        ml::CMat34 rotZ;
        CERotTrig(rotZ.mtx, ang->z, 'z');
        ml::CMat34 work2;
        PSMTXConcat(rotZ.mtx, mtx->mtx, work2.mtx);
        *mtx = work2;
    } else if (order == 3) {
        if (func_804DD0A0(mtx->mtx, ang) != 0) {
            return;
        }
        CERotTrig(mtx->mtx, ang->y, 'y');
        ml::CMat34 rotZ;
        CERotTrig(rotZ.mtx, ang->z, 'z');
        ml::CMat34 work;
        PSMTXConcat(rotZ.mtx, mtx->mtx, work.mtx);
        *mtx = work;
        ml::CMat34 rotX;
        CERotTrig(rotX.mtx, ang->x, 'x');
        ml::CMat34 work2;
        PSMTXConcat(rotX.mtx, mtx->mtx, work2.mtx);
        *mtx = work2;
    } else if (order == 4) {
        if (func_804DD0A0(mtx->mtx, ang) != 0) {
            return;
        }
        CERotTrig(mtx->mtx, ang->z, 'z');
        ml::CMat34 rotX;
        CERotTrig(rotX.mtx, ang->x, 'x');
        ml::CMat34 work;
        PSMTXConcat(rotX.mtx, mtx->mtx, work.mtx);
        *mtx = work;
        ml::CMat34 rotY;
        CERotTrig(rotY.mtx, ang->y, 'y');
        ml::CMat34 work2;
        PSMTXConcat(rotY.mtx, mtx->mtx, work2.mtx);
        *mtx = work2;
    } else if (order == 5) {
        if (func_804DD0A0(mtx->mtx, ang) != 0) {
            return;
        }
        CERotTrig(mtx->mtx, ang->z, 'z');
        ml::CMat34 rotY;
        CERotTrig(rotY.mtx, ang->y, 'y');
        ml::CMat34 work;
        PSMTXConcat(rotY.mtx, mtx->mtx, work.mtx);
        *mtx = work;
        ml::CMat34 rotX;
        CERotTrig(rotX.mtx, ang->x, 'x');
        ml::CMat34 work2;
        PSMTXConcat(rotX.mtx, mtx->mtx, work2.mtx);
        *mtx = work2;
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
    f32 toRad = lbl_eu_8066B278; // degrees-per-radian scale, hoisted into f31
    f32 prevSin;
    f32 prevCos;
    int i;
    for (i = 0; i < 360; i++) {
        lbl_eu_80660038[i].value = (f32)sin(toRad * (f32)i);
        lbl_eu_80660B78[i].value = (f32)cos(toRad * (f32)i);
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
