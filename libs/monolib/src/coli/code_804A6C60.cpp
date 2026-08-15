// Translation unit: monolib/src/coli/code_804A6C60
// Collision subsystem - method dispatch table handlers and utilities.
// Each stub below is a catalog placeholder pending decompilation.

#include <harness_catalog.h>
#include <PowerPC_EABI_Support/MSL_C/MSL_Common/rand.h>
#include <cstring>
#include <monolib/coli/code_804A6C60.hpp>
#include <monolib/coli/coli_types.hpp>
#include <monolib/lod/CLODCacheManagerS.hpp>
#include <monolib/math/CVec3.hpp>
#include <monolib/util/MemManager.hpp>
#include <nw4r/math.h>
#include <revolution/mtx/mtx.h>
#include <revolution/mtx/mtxvec.h>
#include <revolution/mtx/vec.h>

using ml::coli::CColiObject;
using nw4r::math::VEC3;
using nw4r::math::_VEC3;

// 3-word sub-spec copied raw into the +0x44 point; full definition appears
// below the ctor family that uses it.
struct CColiSubSpec804A7878;
// Embedded CColiProc base sub-object at +0x04; definition appears below.
struct CColiProcLocal;
extern "C" void __ct__CColiProc(CColiProcLocal* self);

// SDA2 zero constant used to fill the axis-helper vectors (retail @sda21 reloc).
extern const float lbl_eu_8066AE24;
// SDA2 1.0f used for the rotation-matrix diagonal (func_804A763C / func_804A70F8).
extern const float lbl_eu_8066AE20;
// radians -> FIdx scale (128/pi) fed to SinFIdx/CosFIdx (func_804A763C / func_804A70F8).
extern const float lbl_eu_8066AE28;
// ml::epsilon - plane-distance guard (func_804AD1E0 loop).
extern const float lbl_eu_8066A208;
// nw4r FSqrt assert strings (retail .data, shared with other TUs).
extern char lbl_eu_80526324[];
extern char lbl_eu_80526300[];
// SDA2 threshold/override constants (retail @sda21 relocs): 8066AE80 gates
// the per-axis AABB move in func_804B08A0, 8066AE88 overwrites v.y in
// func_804B24A4 when the flag byte is set. 8066AE40 scales the +0x60
// scalar in func_804A7C64; 8066AE8C/8066AE90 fill the axis block in
// func_804B0B54.
extern const float lbl_eu_8066AE80;
extern const float lbl_eu_8066AE88;
extern const float lbl_eu_8066AE40;
extern const float lbl_eu_8066AE8C;
extern const float lbl_eu_8066AE90;
extern const float lbl_eu_8066AE94;  // rotation angle scale (nw4r RAD_TO_FIDX)
extern const float lbl_eu_8066AE38;  // radius/angle gate scale (func_804A79B4)
extern const float lbl_eu_8066AE60;  // FIdx->angle scale (func_804ABF08)
extern const float lbl_eu_8066A1F8;  // pi in FIdx units (func_804ABF08 wrap bound)
extern const float lbl_eu_8066A1FC;  // 2*pi in FIdx units (func_804ABF08 wrap step)
extern const float lbl_eu_8066AE78;  // large radius bound (sweep tests)
extern const float lbl_eu_8066AEA0;  // reset value for the current-segment global
extern const float lbl_eu_8066AE58;  // sphere test gate scale (func_804AA504)
extern const float lbl_eu_8066AE54;  // normal scale constant (func_804AD8FC)
extern const float lbl_eu_8066AE6C;  // |component| < thresh -> treat as 0 (func_804ADD3C)
extern const double lbl_eu_8066AE48; // 0x4330000080000000 (s32->f32 magic)
extern const float lbl_eu_8066AE70;  // upper t bound init (func_804ADD3C)
extern const float lbl_eu_8066AE74;  // lower t bound init (func_804ADD3C)
extern const float lbl_eu_8066AE64;  // triangle half-space gate (func_804ACD9C)
extern const float lbl_eu_8066AE5C;  // near-parallel determinant gate (func_804B028C)
extern const float lbl_eu_8066AE68;  // exit-face reach init (func_804AD410)

// Data word written into the proc object's +0x08 slot at the start of the
// sweep/contact helpers (func_804AF09C / func_804AE11C).
extern u32 lbl_eu_80663A90;

// Current-segment global used by func_804B1DEC: the active segment point
// (+0x38) and scalar (+0x3c), plus a flag byte (+0x40). The +0x3c slot is
// also pointed at by the caller's object as its partner.
struct CColiGlobal805D0A0 {
    u8 _00[0x38];          // +0x00 .. +0x37
    VEC3* field_0x38;      // +0x38 current segment point
    f32 field_0x3c;        // +0x3c current segment scalar
    u8 field_0x40;         // +0x40 flag byte
};
extern CColiGlobal805D0A0 lbl_eu_8065D0A0;

// Registered-collision globals (sbss 0x65910..0x65938) written by
// func_804A7E18 / func_804A7EC8: the current collision object plus its
// sub-objects. Plain global-scope externs so MWCC emits the exact @sda21
// symbol names (namespace-scope declarations would be mangled).
extern ml::coli::CColiObject* lbl_eu_80665910;
extern ml::coli::CColiObject* lbl_eu_80665914;
extern ml::coli::CColiObject* lbl_eu_80665918;
extern ml::coli::CColiObject* lbl_eu_8066591C;
extern ml::coli::CColiObject* lbl_eu_80665920;
extern ml::coli::CColiObject* lbl_eu_80665924;
extern ml::coli::CColiObject* lbl_eu_80665928;
extern ml::coli::CColiObject* lbl_eu_8066592C;
extern ml::coli::CColiObject* lbl_eu_80665930;
extern u32 lbl_eu_80665938;

// Sweep-control record for func_804AB538: the +0x00 record count re-bases
// the 0x14-stride array (lbl_eu_80665924), +0x04 is the top record index
// and +0x08 the member count sizing the visited-bitset memset.
struct CColiSweepCtrl {
    u32 field_0x00;   // +0x00 record count
    u32 field_0x04;   // +0x04 top record index
    u32 field_0x08;   // +0x08 member count
};
extern CColiSweepCtrl* lbl_eu_80665934;

// Visited-id bitset shared by the sweep/recursion helpers: each id marks
// the sub-object as already processed (func_804AC9F4 / func_804AC624).
extern u32 lbl_eu_8065CFA0[];

/**
 * Collision object - layout recovered incrementally from retail access
 * patterns in this TU; unknown regions are kept as padding. field_0x00 is
 * the AABB corner pair + axis helpers; it is also read as u32 byte offsets
 * (func_804A7E18) and as a partner-object pointer (func_804ABCA4).
 * 0x04/0x08 are a kind/valid word pair (func_804B0B54, func_804B1BDC);
 * 0x60/0x64 are scalars (func_804A7C64); 0x310/0x314 tail fields.
 */
struct CColiObject {
    union {
        _VEC3 field_0x00[4];   // 0x00..0x2f corner pair + axes
        struct {
            CColiObject* field_0x00_obj; // 0x00 partner-object pointer
            u32 field_0x04;              // 0x04 kind/valid word
            u32 field_0x08;              // 0x08
            u8 _0c[4];                   // 0x0c
            union {
                u32 field_0x10;          // 0x10 sub-object byte offset
                f32 field_0x10_f;        // 0x10 radius view (func_804ABCA4)
            };
            u8 _14[4];                   // 0x14
            u32 field_0x18;              // 0x18
            u8 _1c[4];                   // 0x1c
            u32 field_0x20;              // 0x20
            u8 _24[4];                   // 0x24
            u32 field_0x28;              // 0x28
        };
    };
    union {
        _VEC3 field_0x30;     // 0x30..0x3b vector copied raw (lwz/stw)
        struct {
            u32 field_0x30_off;          // 0x30
            u8 _34[4];                   // 0x34
            u32 field_0x38;              // 0x38
        };
    };
    union {
        u8 field_0x3c[8];     // 0x3c..0x43
        struct {
            union {
                f32 field_0x3c_f;      // 0x3c (lfs view)
                u32 field_0x3c_u;      // 0x3c (stw view)
            };
            union {
                u32 field_0x40;        // 0x40 (lwz/stw view)
                u8 field_0x40_b;       // 0x40 (lbz view)
            };
        };
    };
    VEC3 field_0x44;          // 0x44
    VEC3 field_0x50;          // 0x50
    union {
        u8 _5c[0x150 - 0x5c];  // 0x5c..0x14f
        struct {
            f32 field_0x5c;            // 0x5c radius / scalar
            f32 field_0x60;            // 0x60 (func_804A7C64: sdata2 const * f2)
            f32 field_0x64;            // 0x64 (func_804A7C64: f3)
            VEC3 field_0x68;           // 0x68..0x73 segment diff (y forced to 0)
            VEC3 field_0x74;           // 0x74..0x7f (func_804AF09C / func_804AE11C gate point)
            u32 field_0x80;            // 0x80 (func_804ACD9C hit counter)
            u8 _84[0x8c - 0x84];       // 0x84..0x8b
            u32 field_0x8c;            // 0x8c
            union {
                _VEC3 field_0x90[16];   // 0x90..0x14f transformed point list
                struct {
                    _VEC3 field_0x90a[2];   // 0x90..0xa7
                    u32 field_0xa8;        // 0xa8 behaviour flags
                    u8 _ac[0xb0 - 0xac];   // 0xac..0xaf
                    _VEC3 field_0xb0[13];  // 0xb0..0x14b
                    f32 field_0x14c;       // 0x14c..0x14f
                };
            };
        };
    };
    VEC3 field_0x150[16];     // 0x150..0x20f transformed normal list
    u32 field_0x210;          // 0x210
    VEC3 field_0x214[16];     // 0x214..0x2d3 transformed point list
    u32 field_0x2d4;          // 0x2d4
    f32 field_0x2d8[3];       // 0x2d8 per-kind radius bounds (func_804AC624)
    f32 field_0x2e4[3];       // 0x2e4 (func_804AB538 transformed partner point)
    f32 field_0x2f0[3];       // 0x2f0 per-kind lower distance bounds (func_804AC9F4)
    f32 field_0x2fc[3];       // 0x2fc per-kind upper distance bounds (func_804AC9F4)
    f32 field_0x308;          // 0x308 horizontal squared distance
    f32 field_0x30c;          // 0x30c (func_804ACD9C sweep distance gate)
    f32 field_0x310;          // 0x310
    u32 field_0x314;          // 0x314
};

static inline f32 clamp01(f32 value) {
    if (value < lbl_eu_8066AE44) return lbl_eu_8066AE44;
    if (value > lbl_eu_8066AE3C) return lbl_eu_8066AE3C;
    return value;
}

// LOD draw-record view used by func_804A6C60 (retail stride 0x68; only the
// fields the matrix builder reads are declared).
struct CColiLodRecA6C60 {
    u16 field_0x0;       // 0x00 flags (bit 1 = concat self after dispatch)
    u8  field_0x2;       // 0x02 dispatch-table index
    u8  field_0x3;       // 0x03
    u16 field_0x4;       // 0x04
    u16 field_0x6;       // 0x06 LOD pair-table index
    u8  _08[0x14 - 0x08];  // 0x08..0x13
    f32 field_0x14[3];   // 0x14
    f32 field_0x20[3];   // 0x20
    Mtx field_0x2c;      // 0x2c..0x5b
};

// Per-record LOD matrix dispatch table (target func_804A6C60): the entry
// index is the record's field_0x2 byte; called with (out-matrix, buffer,
// record).
typedef void (*LodA6C60Fn)(f32*, f32*, CColiLodRecA6C60*);
extern LodA6C60Fn lbl_eu_8056F1B8[];

// LOD cache record (retail stride 0xC), defined in CLODCacheManagerS.cpp;
// the member lookup is called via `bl` from func_804A6C60.
namespace LOD {
struct CLODCacheManagerS {
    u16 field_0x0;   // 0x00 near distance
    u16 field_0x2;   // 0x02 far distance
    u16 field_0x4;   // 0x04 record id (dispatch-table index)
    u16 field_0x6;   // 0x06 entry count
    u32 field_0x8;   // 0x08 table index
    f32 func_8046323C();
    s32 func_80463590();
};
}  // namespace LOD

// LOD record-matrix builder: zero the 9-float work buffer, then overwrite
// slots 3..8 with the record's +0x14/+0x20 vectors, sweep the record's LOD
// pair list to write each cache record's distance value into
// buf[rec->field_0x4], dispatch through the per-record table at
// lbl_eu_8056F1B8 with (matrix, buf, record), and - when record bit 1 is
// set - premultiply self by the record's +0x2c matrix.
extern "C" void func_804A6C60(f32* self, CColiLodRecA6C60* obj) {
    u16 idx = obj->field_0x6;
    u32 entry = lbl_eu_8066574C[idx];
    u16* pair = &lbl_eu_80665750[entry];
    u16 count = *pair;
    f32 buf[9] = {0};
    s32 i = 0;
    buf[3] = obj->field_0x14[0];
    buf[4] = obj->field_0x14[1];
    buf[5] = obj->field_0x14[2];
    buf[6] = obj->field_0x20[0];
    buf[7] = obj->field_0x20[1];
    buf[8] = obj->field_0x20[2];
    while (i < count) {
        LOD::CLODCacheManagerS* rec = (LOD::CLODCacheManagerS*)(
            (u8*)lbl_eu_80665738 + (u32)pair[1] * 0xC);
        buf[rec->field_0x4] = rec->func_8046323C();
        pair++;
        i++;
    }
    lbl_eu_8056F1B8[obj->field_0x2](self, buf, obj);
    if (obj->field_0x0 & 0x2) {
        PSMTXConcat(obj->field_0x2c, (const f32 (*)[4])self,
                    (f32 (*)[4])self);
    }
}

// LOD cache-record lookup for the +4 index: entry table → pair table →
// record table, then tail-call the member distance-bin lookup (retail
// `b func_80463590`).
extern "C" s32 func_804A6D90(void* self) {
    u32 idx = *(u32*)((u8*)self + 4);
    u32 n = lbl_eu_8066574C[idx];
    u16 h = *(u16*)((u8*)lbl_eu_80665750 + n * 2 + 2);
    LOD::CLODCacheManagerS* rec =
        (LOD::CLODCacheManagerS*)((u8*)lbl_eu_80665738 + h * 0xC);
    return rec->func_80463590();
}

void func_804A6DC0(void){}

// --- func_804A6E20 defined below (after the CColiXformSrc structs) ---

// Euler transform source for the axis-matrix builders (func_804A763C /
// func_804A70F8): translation at +0x00, Euler rotation angles (radians,
// x/y/z) at +0x0c and scale at +0x18. The final matrix is composed as
// rotX * rotY * rotZ * scale via PSMTXConcat premultiplication.
struct CColiXformSrc {
    VEC3 field_0x00;   // +0x00 translation
    VEC3 field_0x0c;   // +0x0c Euler angles (x, y, z)
    VEC3 field_0x18;   // +0x18 scale
};

// Extra object for func_804A70F8: the +0x08 vector is added to the source
// translation to form the final matrix translation column.
struct CColiXformExtra804A70F8 {
    u8 _00[0x08];        // +0x00 .. +0x07
    VEC3 field_0x08;     // +0x08 translation offset
};

// Build the rotation-scale matrix for src into out: a diagonal scale matrix
// from src->field_0x18, then premultiply by the Z / Y / X rotation matrices
// whenever the corresponding Euler angle is non-zero.
void func_804A763C(f32* out, const CColiXformSrc* src) {
    f32 k = lbl_eu_8066AE24;
    f32 x = src->field_0x18.x;
    f32 y = src->field_0x18.y;
    f32 z = src->field_0x18.z;
    out[0] = x;
    out[1] = k;
    out[2] = k;
    out[3] = k;
    out[4] = k;
    out[5] = y;
    out[6] = k;
    out[7] = k;
    out[8] = k;
    out[9] = k;
    out[10] = z;
    out[11] = k;

    if (src->field_0x0c.z != lbl_eu_8066AE24) {
        f32 az = src->field_0x0c.z;
        f32 s = nw4r::math::SinFIdx(lbl_eu_8066AE28 * az);
        f32 c = nw4r::math::CosFIdx(lbl_eu_8066AE28 * az);
        f32 m[12] = {c, -s, lbl_eu_8066AE24, lbl_eu_8066AE24, s, c, lbl_eu_8066AE24, lbl_eu_8066AE24, lbl_eu_8066AE24, lbl_eu_8066AE24, lbl_eu_8066AE20, lbl_eu_8066AE24};
        PSMTXConcat((const f32 (*)[4])m, (const f32 (*)[4])out, (f32 (*)[4])out);
    }
    if (src->field_0x0c.y != lbl_eu_8066AE24) {
        f32 ay = src->field_0x0c.y;
        f32 s = nw4r::math::SinFIdx(lbl_eu_8066AE28 * ay);
        f32 c = nw4r::math::CosFIdx(lbl_eu_8066AE28 * ay);
        f32 m[12] = {c, lbl_eu_8066AE24, s, lbl_eu_8066AE24, lbl_eu_8066AE24, lbl_eu_8066AE20, lbl_eu_8066AE24, lbl_eu_8066AE24, -s, lbl_eu_8066AE24, c, lbl_eu_8066AE24};
        PSMTXConcat((const f32 (*)[4])m, (const f32 (*)[4])out, (f32 (*)[4])out);
    }
    if (src->field_0x0c.x != lbl_eu_8066AE24) {
        f32 ax = src->field_0x0c.x;
        f32 s = nw4r::math::SinFIdx(lbl_eu_8066AE28 * ax);
        f32 c = nw4r::math::CosFIdx(lbl_eu_8066AE28 * ax);
        f32 m[12] = {lbl_eu_8066AE20, lbl_eu_8066AE24, lbl_eu_8066AE24, lbl_eu_8066AE24, lbl_eu_8066AE24, c, -s, lbl_eu_8066AE24, lbl_eu_8066AE24, s, c, lbl_eu_8066AE24};
        PSMTXConcat((const f32 (*)[4])m, (const f32 (*)[4])out, (f32 (*)[4])out);
    }
}

// Same rotation-scale matrix build as func_804A763C, followed by the
// translation column: out[3/7/11] = src->field_0x00 + extra->field_0x08.
void func_804A70F8(f32* out, const CColiXformSrc* src,
                   const CColiXformExtra804A70F8* extra) {
    f32 k = lbl_eu_8066AE24;
    f32 x = src->field_0x18.x;
    f32 y = src->field_0x18.y;
    f32 z = src->field_0x18.z;
    out[0] = x;
    out[1] = k;
    out[2] = k;
    out[3] = k;
    out[4] = k;
    out[5] = y;
    out[6] = k;
    out[7] = k;
    out[8] = k;
    out[9] = k;
    out[10] = z;
    out[11] = k;

    if (src->field_0x0c.z != lbl_eu_8066AE24) {
        f32 az = src->field_0x0c.z;
        f32 s = nw4r::math::SinFIdx(lbl_eu_8066AE28 * az);
        f32 c = nw4r::math::CosFIdx(lbl_eu_8066AE28 * az);
        f32 m[12] = {c, -s, lbl_eu_8066AE24, lbl_eu_8066AE24, s, c, lbl_eu_8066AE24, lbl_eu_8066AE24, lbl_eu_8066AE24, lbl_eu_8066AE24, lbl_eu_8066AE20, lbl_eu_8066AE24};
        PSMTXConcat((const f32 (*)[4])m, (const f32 (*)[4])out, (f32 (*)[4])out);
    }
    if (src->field_0x0c.y != lbl_eu_8066AE24) {
        f32 ay = src->field_0x0c.y;
        f32 s = nw4r::math::SinFIdx(lbl_eu_8066AE28 * ay);
        f32 c = nw4r::math::CosFIdx(lbl_eu_8066AE28 * ay);
        f32 m[12] = {c, lbl_eu_8066AE24, s, lbl_eu_8066AE24, lbl_eu_8066AE24, lbl_eu_8066AE20, lbl_eu_8066AE24, lbl_eu_8066AE24, -s, lbl_eu_8066AE24, c, lbl_eu_8066AE24};
        PSMTXConcat((const f32 (*)[4])m, (const f32 (*)[4])out, (f32 (*)[4])out);
    }
    if (src->field_0x0c.x != lbl_eu_8066AE24) {
        f32 ax = src->field_0x0c.x;
        f32 s = nw4r::math::SinFIdx(lbl_eu_8066AE28 * ax);
        f32 c = nw4r::math::CosFIdx(lbl_eu_8066AE28 * ax);
        f32 m[12] = {lbl_eu_8066AE20, lbl_eu_8066AE24, lbl_eu_8066AE24, lbl_eu_8066AE24, lbl_eu_8066AE24, c, -s, lbl_eu_8066AE24, lbl_eu_8066AE24, s, c, lbl_eu_8066AE24};
        PSMTXConcat((const f32 (*)[4])m, (const f32 (*)[4])out, (f32 (*)[4])out);
    }

    out[3] = src->field_0x00.x + extra->field_0x08.x;
    out[7] = src->field_0x00.y + extra->field_0x08.y;
    out[11] = src->field_0x00.z + extra->field_0x08.z;
}

// Euler rotation composition (Rz * Ry * Rx) with a translation column: each
// non-zero angle is applied by premultiplying the running matrix with the
// corresponding rotation matrix (PSMTXConcat), except the first non-zero angle
// which is written directly into out. When every angle is zero the rotation
// part is the identity. The final translation column is the sum of the source
// translation and the extra object's +0x08 vector.
void func_804A6E20(f32* out, const CColiXformSrc* src,
                   const CColiXformExtra804A70F8* extra) {
    int count = 0;
    if (src->field_0x0c.x != lbl_eu_8066AE24) {
        f32 ax = src->field_0x0c.x;
        f32 s = nw4r::math::SinFIdx(lbl_eu_8066AE28 * ax);
        f32 c = nw4r::math::CosFIdx(lbl_eu_8066AE28 * ax);
        f32 ns = -s;
        out[0] = lbl_eu_8066AE20;
        out[1] = lbl_eu_8066AE24;
        out[2] = lbl_eu_8066AE24;
        out[3] = lbl_eu_8066AE24;
        out[4] = lbl_eu_8066AE24;
        out[5] = c;
        out[6] = ns;
        out[7] = lbl_eu_8066AE24;
        out[8] = lbl_eu_8066AE24;
        out[9] = s;
        out[10] = c;
        out[11] = lbl_eu_8066AE24;
        count++;
    }
    if (src->field_0x0c.y != lbl_eu_8066AE24) {
        f32 ay = src->field_0x0c.y;
        f32 s = nw4r::math::SinFIdx(lbl_eu_8066AE28 * ay);
        f32 c = nw4r::math::CosFIdx(lbl_eu_8066AE28 * ay);
        f32 ns = -s;
        if (count != 0) {
            f32 m[12] = {c, lbl_eu_8066AE24, s, lbl_eu_8066AE24, lbl_eu_8066AE24, lbl_eu_8066AE20, lbl_eu_8066AE24, lbl_eu_8066AE24, ns, lbl_eu_8066AE24, c, lbl_eu_8066AE24};
            PSMTXConcat((const f32 (*)[4])m, (const f32 (*)[4])out,
                        (f32 (*)[4])out);
        } else {
            out[0] = c;
            out[1] = lbl_eu_8066AE24;
            out[2] = s;
            out[3] = lbl_eu_8066AE24;
            out[4] = lbl_eu_8066AE24;
            out[5] = lbl_eu_8066AE20;
            out[6] = lbl_eu_8066AE24;
            out[7] = lbl_eu_8066AE24;
            out[8] = ns;
            out[9] = lbl_eu_8066AE24;
            out[10] = c;
            out[11] = lbl_eu_8066AE24;
        }
        count++;
    }
    if (src->field_0x0c.z != lbl_eu_8066AE24) {
        f32 az = src->field_0x0c.z;
        f32 s = nw4r::math::SinFIdx(lbl_eu_8066AE28 * az);
        f32 c = nw4r::math::CosFIdx(lbl_eu_8066AE28 * az);
        f32 ns = -s;
        if (count != 0) {
            f32 m[12] = {c, ns, lbl_eu_8066AE24, lbl_eu_8066AE24, s, c, lbl_eu_8066AE24, lbl_eu_8066AE24, lbl_eu_8066AE24, lbl_eu_8066AE24, lbl_eu_8066AE20, lbl_eu_8066AE24};
            PSMTXConcat((const f32 (*)[4])m, (const f32 (*)[4])out,
                        (f32 (*)[4])out);
        } else {
            out[0] = c;
            out[1] = ns;
            out[2] = lbl_eu_8066AE24;
            out[3] = lbl_eu_8066AE24;
            out[4] = s;
            out[5] = c;
            out[6] = lbl_eu_8066AE24;
            out[7] = lbl_eu_8066AE24;
            out[8] = lbl_eu_8066AE24;
            out[9] = lbl_eu_8066AE24;
            out[10] = lbl_eu_8066AE20;
            out[11] = lbl_eu_8066AE24;
        }
        count++;
    }
    if (count == 0) {
        PSMTXIdentity((f32 (*)[4])out);
    }
    out[3] = src->field_0x00.x + extra->field_0x08.x;
    out[7] = src->field_0x00.y + extra->field_0x08.y;
    out[11] = src->field_0x00.z + extra->field_0x08.z;
}

// Axis frame build (sibling of func_804A7834): the 3x4 matrix diagonal
// comes from src->field_0x00[2] (a direction vector), the zero constant
// fills the rest, and three axis slots get src's corner plus a mixed pair
// of other's axis vectors. FPR allocation follows local declaration order
// (z, y, x, then the sdata2 zero - matching retail's load sequence).
// The fills go through a _VEC3* so MWCC cannot alias-prove the add stores
// (written via self) against them - retail keeps all 12 fill stores and
// schedules the add loads in place after them.
// 12-float view of the field_0x00 matrix block used by func_804A732C's
// axis-slot writes (the retail keeps the three k-fills that are overwritten,
// so the add stores must look non-aliasing to MWCC's dead-store eliminator).
struct CColiM12f804A732C {
    f32 f[12];   // +0x00..+0x2f
};

// Axis frame build (sibling of func_804A7834): the 3x4 matrix diagonal
// comes from src->field_0x00[2] (a direction vector), the zero constant
// fills the rest, and three axis slots get src's corner plus a mixed pair
// of other's axis vectors. The four corner rows are written as whole VEC3
// copies so MWCC keeps all twelve stores (retail overwrites three with the
// axis slots instead of dropping the earlier k fills).
void func_804A732C(CColiObject* self, CColiObject* src, CColiObject* other) {
    f32 z = src->field_0x00[2].z;
    f32 y = src->field_0x00[2].y;
    f32 x = src->field_0x00[2].x;
    f32 k = lbl_eu_8066AE24;
    self->field_0x00[0].x = x;
    self->field_0x00[0].y = k;
    self->field_0x00[0].z = k;
    self->field_0x00[1].x = k;
    self->field_0x00[1].y = k;
    self->field_0x00[1].z = y;
    self->field_0x00[2].x = k;
    self->field_0x00[2].y = k;
    self->field_0x00[2].z = k;
    self->field_0x00[3].x = k;
    self->field_0x00[3].y = z;
    self->field_0x00[3].z = k;
    self->field_0x00[1].x = src->field_0x00[0].x + other->field_0x00[2].x;
    self->field_0x00[2].y = src->field_0x00[0].y + other->field_0x00[3].x;
    self->field_0x00[3].z = src->field_0x00[0].z + other->field_0x00[3].y;
}

// Euler rotation composition (Rx * Ry * Rz): each non-zero angle is applied
// by premultiplying the running matrix with the corresponding rotation matrix
// (PSMTXConcat), except the first non-zero angle which is written directly
// into out (retail's direct-write vs concat split). When every angle is zero
// the result is the identity matrix.
void func_804A73A0(f32* out, const CColiXformSrc* src) {
    int count = 0;
    if (src->field_0x0c.z != lbl_eu_8066AE24) {
        f32 az = src->field_0x0c.z;
        f32 s = nw4r::math::SinFIdx(lbl_eu_8066AE28 * az);
        f32 c = nw4r::math::CosFIdx(lbl_eu_8066AE28 * az);
        f32 ns = -s;
        out[0] = c;
        out[1] = ns;
        out[2] = lbl_eu_8066AE24;
        out[3] = lbl_eu_8066AE24;
        out[4] = s;
        out[5] = c;
        out[6] = lbl_eu_8066AE24;
        out[7] = lbl_eu_8066AE24;
        out[8] = lbl_eu_8066AE24;
        out[9] = lbl_eu_8066AE24;
        out[10] = lbl_eu_8066AE20;
        out[11] = lbl_eu_8066AE24;
        count++;
    }
    if (src->field_0x0c.y != lbl_eu_8066AE24) {
        f32 ay = src->field_0x0c.y;
        f32 s = nw4r::math::SinFIdx(lbl_eu_8066AE28 * ay);
        f32 c = nw4r::math::CosFIdx(lbl_eu_8066AE28 * ay);
        f32 ns = -s;
        if (count != 0) {
            f32 m[12] = {c, lbl_eu_8066AE24, s, lbl_eu_8066AE24, lbl_eu_8066AE24, lbl_eu_8066AE20, lbl_eu_8066AE24, lbl_eu_8066AE24, ns, lbl_eu_8066AE24, c, lbl_eu_8066AE24};
            PSMTXConcat((const f32 (*)[4])m, (const f32 (*)[4])out,
                        (f32 (*)[4])out);
        } else {
            out[0] = c;
            out[1] = lbl_eu_8066AE24;
            out[2] = s;
            out[3] = lbl_eu_8066AE24;
            out[4] = lbl_eu_8066AE24;
            out[5] = lbl_eu_8066AE20;
            out[6] = lbl_eu_8066AE24;
            out[7] = lbl_eu_8066AE24;
            out[8] = ns;
            out[9] = lbl_eu_8066AE24;
            out[10] = c;
            out[11] = lbl_eu_8066AE24;
        }
        count++;
    }
    if (src->field_0x0c.x != lbl_eu_8066AE24) {
        f32 ax = src->field_0x0c.x;
        f32 s = nw4r::math::SinFIdx(lbl_eu_8066AE28 * ax);
        f32 c = nw4r::math::CosFIdx(lbl_eu_8066AE28 * ax);
        f32 ns = -s;
        if (count != 0) {
            f32 m[12] = {lbl_eu_8066AE20, lbl_eu_8066AE24, lbl_eu_8066AE24, lbl_eu_8066AE24, lbl_eu_8066AE24, c, ns, lbl_eu_8066AE24, lbl_eu_8066AE24, s, c, lbl_eu_8066AE24};
            PSMTXConcat((const f32 (*)[4])m, (const f32 (*)[4])out,
                        (f32 (*)[4])out);
        } else {
            out[0] = lbl_eu_8066AE20;
            out[1] = lbl_eu_8066AE24;
            out[2] = lbl_eu_8066AE24;
            out[3] = lbl_eu_8066AE24;
            out[4] = lbl_eu_8066AE24;
            out[5] = c;
            out[6] = ns;
            out[7] = lbl_eu_8066AE24;
            out[8] = lbl_eu_8066AE24;
            out[9] = s;
            out[10] = c;
            out[11] = lbl_eu_8066AE24;
        }
        count++;
    }
    if (count == 0) {
        PSMTXIdentity((f32 (*)[4])out);
    }
}

void func_804A7834(CColiObject* self, const CColiObject* other) {
    // Diagonal 3x4 scale-matrix fill: other->field_0x00[2] (a direction
    // vector) placed on the main diagonal, zero constant elsewhere.
    // FPR allocation follows local declaration order: k->f0, x->f1, y->f2, z->f3.
    // MWCC hoists the sdata2 const load to slot 0 (retail loads z, y, x first);
    // the register map and input-load order match retail exactly.
    f32 k = lbl_eu_8066AE24;
    f32 x = other->field_0x00[2].x;
    f32 y = other->field_0x00[2].y;
    f32 z = other->field_0x00[2].z;
    self->field_0x00[0].x = x;
    self->field_0x00[0].y = k;
    self->field_0x00[0].z = k;
    self->field_0x00[1].x = k;
    self->field_0x00[1].y = k;
    self->field_0x00[1].z = y;
    self->field_0x00[2].x = k;
    self->field_0x00[2].y = k;
    self->field_0x00[2].z = k;
    self->field_0x00[3].x = k;
    self->field_0x00[3].y = z;
    self->field_0x00[3].z = k;
}

// Constructor with angular parameters: run the embedded CColiProc base ctor
// at +0x04, copy the 3-word sub-spec into the +0x44/+0x50 point slots (raw
// word copies; +0x48 is later re-written as the float sum), seed the +0x40
// sub-object offset (= 5) and the +0x5c/+0x60 angle-pair scalars. When the
// +0x60 bound exceeds const * +0x5c, set flag bit 3 and compute the +0x310
// gap from the +0x54 scalar. The +0x3c partner slot points at the caller's
// index or the embedded proc itself. Returns self.
CColiObject* func_804A79B4(CColiObject* self, const CColiSubSpec804A7878* spec,
                           f32 f1, f32 f2, u32 idx) {
    __ct__CColiProc((CColiProcLocal*)&self->field_0x04);
    const u32* s = (const u32*)spec;
    u32* d = (u32*)&self->field_0x44;
    d[1] = s[1];   // +0x48
    d[0] = s[0];   // +0x44
    d[2] = s[2];   // +0x4c
    d[3] = s[0];   // +0x50
    d[4] = s[1];   // +0x54
    d[5] = s[2];   // +0x58
    self->field_0x40 = 5;
    self->field_0x44.y += f1;
    self->field_0x5c = f1;
    self->field_0x60 = f2;
    if (f2 > lbl_eu_8066AE38 * f1) {
        self->field_0x8c |= 0x8;
        self->field_0x310 = self->field_0x50.y + f2 - f1;
    }
    self->field_0x8c = 0;
    self->field_0x210 = 0;
    self->field_0x2d4 = 0;
    if (idx != 0) {
        self->field_0x3c_u = idx;
    } else {
        self->field_0x3c_u = (u32)&self->field_0x04;
    }
    self->field_0x314 = 0;
    return self;
}

// Segment constructor: run the embedded CColiProc base ctor at +0x04, copy
// the 3-word sub-spec into the +0x44 point and the 3-word vector into the
// +0x50 point (raw word copies), then write the segment diff (b - a) at
// +0x68 and normalise it to unit length, storing the original length at
// +0x308. The +0x40 sub-object offset is 10; the +0x3c partner slot points
// at the caller's index or the embedded proc itself. Returns self.
CColiObject* func_804A7ACC(CColiObject* self, const CColiSubSpec804A7878* spec,
                           const VEC3* v, u32 idx) {
    __ct__CColiProc((CColiProcLocal*)&self->field_0x04);
    const u32* sa = (const u32*)spec;
    const u32* sv = (const u32*)v;
    u32* d = (u32*)&self->field_0x44;
    self->field_0x40 = 0xa;
    d[0] = sa[0];   // +0x44
    d[1] = sa[1];   // +0x48
    d[3] = sv[0];   // +0x50
    d[4] = sv[1];   // +0x54
    d[2] = sa[2];   // +0x4c
    d[5] = sv[2];   // +0x58
    VEC3Sub(&self->field_0x68, &self->field_0x50, &self->field_0x44);
    // mag holds the call result (double-modeled); the division re-singles it
    // via frsp while the raw f1 goes to the +0x308 store (retail's frsp f0,f1).
    double mag = PSVECMag((const Vec*)&self->field_0x68);
    f32 inv = lbl_eu_8066AE3C / (f32)mag;
    self->field_0x308 = (f32)mag;
    VEC3Scale(&self->field_0x68, &self->field_0x68, inv);
    self->field_0x8c = 0;
    self->field_0x210 = 0;
    self->field_0x2d4 = 0;
    if (idx != 0) {
        self->field_0x3c_u = idx;
    } else {
        self->field_0x3c_u = (u32)&self->field_0x04;
    }
    self->field_0x314 = 0;
    return self;
}

// Embedded CColiProc base sub-object at +0x04; only its constructor runs
// from this TU (retail symbol __ct__CColiProc, defined in the sibling
// CColiProc unit).
struct CColiProcLocal {
    u32 field_0x0;   // +0x0
    u32 field_0x4;   // +0x4
    u32 field_0x8;   // +0x8
    u32 field_0xc;   // +0xc
};
extern "C" void __ct__CColiProc(CColiProcLocal* self);

// Local proc seeded by func_804B25A4 and classified by func_804B2CBC (both
// defined in the sibling CColiProc TU; same-TU stubs are declared below).
extern "C" void func_804B25A4(CColiProcLocal* self, CColiObject* target,
                              u32 a, u32 b);
extern "C" int func_804B2CBC(CColiProcLocal* proc, CColiObject* obj);
// Local-proc classifier used by func_804B204C / func_804B21A8 (sibling
// CColiProc TU): classifies the linked object against the seeded proc.
extern "C" int func_804B2AA4(CColiProcLocal* proc, CColiObject* obj);
// Sibling CColiProc TU helpers used by func_804B102C: seed/classify the
// local proc (func_804B2590, func_804B25BC) and query its result bits
// (func_804B2F80, func_804B2FA8).
extern "C" void func_804B2590(CColiProcLocal* self, CColiObject* target,
                              u32 a);
extern "C" int func_804B25BC(CColiProcLocal* work, u16* outIndex,
                             void** outBuf, void* arg4);
extern "C" bool func_804B2F80(CColiProcLocal* self);
extern "C" bool func_804B2FA8(CColiProcLocal* self);
// Local-object ctor: the trailing params default so func_804B1BDC's retail
// no-setup call (`&local` only) still compiles.
extern "C" CColiObject* func_804A7D1C(CColiObject* self, const VEC3* a = 0,
                                      const VEC3* b = 0, f32 f = 0.0f);
extern "C" int func_804AD410(CColiObject* self, const f32* a, const VEC3* b,
                             const Mtx m, f32 f);
extern "C" void func_804B0EA0(CColiObject* self);
// Sibling CColiProc TU helpers used by func_804B0CE8 / func_804B19CC: the
// segment classifier (func_804B27EC), the local-object classifier
// (func_804B2E3C) and the node query helper (func_804B236C).
extern "C" void func_804B27EC(CColiProcLocal* proc, f32* a, f32* b, f32* c,
                              u16 d);
extern "C" int func_804B2E3C(CColiProcLocal* proc, CColiObject* obj);
extern "C" int func_804B236C(const void* a, const void* b, const void* c);

// u32 view of a CColiObject used by the func_804A7878 constructor: the
// sub-spec words are copied raw (lwz/stw) into the +0x44 point and the
// +0x40/+0x8c/+0x314 fields are seeded as integers.
struct CColiCtor804A7878 {
    u8 _00[0x04];
    u32 field_0x04;            // +0x04 embedded CColiProc base
    u8 _08[0x3c - 0x08];       // +0x08 .. +0x3b
    CColiObject* field_0x3c;   // +0x3c partner / self slot
    u32 field_0x40;            // +0x40 behaviour flags
    u32 field_0x44;            // +0x44 point (raw u32 copy)
    u32 field_0x48;            // +0x48
    u32 field_0x4c;            // +0x4c
    u8 _50[0x5c - 0x50];       // +0x50 .. +0x5b
    f32 field_0x5c;            // +0x5c scalar (func_804A790C radius)
    f32 field_0x60;            // +0x60 (func_804A7C64: sdata2 const * f2)
    f32 field_0x64;            // +0x64 (func_804A7C64: f3)
    u8 _68[0x8c - 0x68];       // +0x68 .. +0x8b
    u32 field_0x8c;            // +0x8c
    u8 _90[0x210 - 0x90];      // +0x90 .. +0x20f
    u32 field_0x210;           // +0x210
    u8 _214[0x2d4 - 0x214];    // +0x214 .. +0x2d3
    u32 field_0x2d4;           // +0x2d4
    u8 _2d8[0x314 - 0x2d8];    // +0x2d8 .. +0x313
    u32 field_0x314;           // +0x314
};

// Partner-sample helper (sibling of func_804A7878 / func_804A7BDC): run the
// embedded CColiProc base ctor at +0x04, copy the 3-word segment point into
// the +0x44 slot, seed the +0x5c scalar and clear the state fields. The
// +0x3c partner slot points at the caller-supplied index or at the embedded
// proc itself. auto_inline off: retail calls this via `bl`.
#pragma push
#pragma auto_inline off
CColiObject* func_804A790C(CColiObject* out, const VEC3* in, f32 d, u32 idx) {
    CColiCtor804A7878* s = (CColiCtor804A7878*)out;
    __ct__CColiProc((CColiProcLocal*)&s->field_0x04);
    const u32* src = (const u32*)in;
    s->field_0x44 = src[0];
    s->field_0x48 = src[1];
    s->field_0x4c = src[2];
    s->field_0x5c = d;
    s->field_0x40 = 0;
    s->field_0x8c = 0;
    s->field_0x210 = 0;
    s->field_0x2d4 = 0;
    if (idx != 0) {
        s->field_0x3c = (CColiObject*)idx;
    } else {
        s->field_0x3c = (CColiObject*)&s->field_0x04;
    }
    s->field_0x314 = 0;
    return out;
}
#pragma pop

// 3-word sub-object spec copied raw into the +0x44 point.
struct CColiSubSpec804A7878 {
    u32 field_0x00;   // +0x00
    u32 field_0x04;   // +0x04
    u32 field_0x08;   // +0x08
};

// CColiObject constructor: run the embedded CColiProc base ctor at +0x04,
// copy the 3-word sub-spec into the +0x44 point, seed the +0x40 behaviour
// flags (= 0xf) and clear +0x8c/+0x314; the +0x3c partner slot points at
// the caller-supplied parent object or at the embedded proc itself.
// C-linkage forward decl so the definition emits the unmangled retail name;
// auto_inline off keeps retail callers (func_804B19CC) emitting a real `bl`.
extern "C" CColiObject* func_804A7878(CColiObject* self,
                                       const CColiSubSpec804A7878* spec,
                                       CColiObject* parent);
#pragma push
#pragma auto_inline off
CColiObject* func_804A7878(CColiObject* self, const CColiSubSpec804A7878* spec,
                           CColiObject* parent) {
    CColiCtor804A7878* s = (CColiCtor804A7878*)self;
    __ct__CColiProc((CColiProcLocal*)&s->field_0x04);
    s->field_0x44 = spec->field_0x00;
    s->field_0x48 = spec->field_0x04;
    s->field_0x4c = spec->field_0x08;
    s->field_0x40 = 0xf;
    s->field_0x8c = 0;
    if (parent != 0) {
        s->field_0x3c = parent;
    } else {
        s->field_0x3c = (CColiObject*)&s->field_0x04;
    }
    s->field_0x314 = 0;
    return self;
}
#pragma pop

// CColiObject initialiser: run the embedded CColiProc base ctor at +0x04,
// copy the 3-word vector into the +0x44 point, then seed the +0x5c scalar,
// the +0x40 sub-object offset (= 0x14) and the cleared state fields.
// C-linkage so callers emit the unmangled retail symbol (bl func_804A7BDC).
extern "C" void func_804A7BDC(CColiObject* self, const VEC3* v, f32 f);
void func_804A7BDC(CColiObject* self, const VEC3* v, f32 f) {
    __ct__CColiProc((CColiProcLocal*)&self->field_0x10);
    self->field_0x44 = *v;
    self->field_0x5c = f;
    self->field_0x40 = 0x14;
    self->field_0x8c = 0;
    self->field_0x210 = 0;
    self->field_0x2d4 = 0;
    self->field_0x3c_u = 0;
    self->field_0x314 = 0;
}

// Constructor sibling of func_804A790C / func_804A7878: run the embedded
// CColiProc base ctor at +0x04, copy the 3-word sub-spec into the +0x44
// point, then seed the +0x5c/+0x60/+0x64 scalars (0x60 = sdata2 const *
// f2) and the +0x40 sub-object offset (= 0x19). Returns self.
// C linkage (retail symbol is unmangled) + auto_inline off: retail calls
// this via `bl` (separate TU in the original source).
#pragma push
#pragma auto_inline off
extern "C" CColiObject* func_804A7C64(CColiObject* self, const CColiSubSpec804A7878* spec,
                            f32 f1, f32 f2, f32 f3) {
    CColiCtor804A7878* s = (CColiCtor804A7878*)self;
    __ct__CColiProc((CColiProcLocal*)&s->field_0x04);
    s->field_0x44 = spec->field_0x00;
    s->field_0x48 = spec->field_0x04;
    s->field_0x4c = spec->field_0x08;
    s->field_0x5c = f1;
    s->field_0x60 = lbl_eu_8066AE40 * f2;
    s->field_0x64 = f3;
    s->field_0x40 = 0x19;
    s->field_0x8c = 0;
    return self;
}
#pragma pop

// Local-object constructor: run the embedded CColiProc base ctor at +0x04,
// copy the two 3-word points into the +0x44/+0x50 slots, then write the
// horizontal segment diff (b - a with y forced to zero) at +0x68 and its
// squared horizontal length at +0x308 (inverted to 1/length^2 when non-zero).
// auto_inline off: retail calls this via `bl` from func_804B1BDC.
#pragma push
#pragma auto_inline off
extern "C" CColiObject* func_804A7D1C(CColiObject* self, const VEC3* a,
                                       const VEC3* b, f32 f) {
    __ct__CColiProc((CColiProcLocal*)&self->field_0x04);
    // Raw word copies so MWCC emits lwz/stw for the point slots (retail).
    const u32* sa = (const u32*)a;
    const u32* sb = (const u32*)b;
    u32* da = (u32*)&self->field_0x44;
    u32* db = (u32*)&self->field_0x50;
    self->field_0x40 = 0x1e;
    da[0] = sa[0];
    da[1] = sa[1];
    db[0] = sb[0];
    db[1] = sb[1];
    da[2] = sa[2];
    db[2] = sb[2];
    VEC3* d = &self->field_0x68;
    VEC3Sub(d, &self->field_0x50, &self->field_0x44);
    d->y = lbl_eu_8066AE44;
    self->field_0x40 = 0x1e;
    self->field_0x5c = f;
    f32 sq = VEC3Dot(d, d);
    self->field_0x308 = sq;
    if (sq != lbl_eu_8066AE44) {
        self->field_0x308 = lbl_eu_8066AE3C / sq;
    }
    self->field_0x8c = 0;
    return self;
}
#pragma pop

// Register the collision object and its sub-objects with the global
// collision state (sbss 0x65910..0x65930). The first parameter is unused
// (retail keeps it in r3 and the object in r4). Each field at 0x10..0x40
// holds a byte offset to a sub-object relative to self; the chain is stored
// as absolute pointers, and the object itself is always registered last.
// The seven pointers are computed into locals first so MWCC hoists all the
// loads/adds ahead of the stores (retail's schedule).
void func_804A7E18(u32 unused, CColiObject* self) {
    if (self->field_0x10 != 0) {
        CColiObject* p0 = (CColiObject*)((u8*)self + self->field_0x10);
        CColiObject* p1 = (CColiObject*)((u8*)self + self->field_0x38);
        CColiObject* p2 = (CColiObject*)((u8*)self + self->field_0x28);
        CColiObject* p3 = (CColiObject*)((u8*)self + self->field_0x20);
        CColiObject* p4 = (CColiObject*)((u8*)self + self->field_0x18);
        CColiObject* p5 = (CColiObject*)((u8*)self + self->field_0x30_off);
        CColiObject* p6 = (CColiObject*)((u8*)self + self->field_0x40);
        lbl_eu_80665910 = p0;
        lbl_eu_80665914 = p1;
        lbl_eu_80665920 = p2;
        lbl_eu_80665918 = p3;
        lbl_eu_8066591C = p4;
        lbl_eu_80665928 = p5;
        lbl_eu_8066592C = p6;
    }
    lbl_eu_80665930 = self;
}

void func_804A7E7C(){}

u32 func_804A7EC8(u32 unused, u32 val) { lbl_eu_80665938 = val; return unused; }

// Object handed to the collision dispatch handlers: func_804A7F0C stores the
// caller parameter into field_0x00 and dispatches through the ptmf table at
// lbl_eu_8056F1D8 selected by (field_0x40 + lbl_eu_80665938).
struct CColiDispatchObj {
    u32 field_0x00;        // +0x00
    u8 _04[0x40 - 0x04];   // +0x04 .. +0x40
    u32 field_0x40;        // +0x40 dispatch index
};

// Member-function-pointer dispatch table (12-byte ptmf stride, 0x1A8 bytes).
// Each entry is invoked with the dispatch object as `this` and no arguments;
// MWCC lowers the member-fn-pointer call to __ptmf_scall.
typedef void (CColiDispatchObj::*CColiDispatchFn)();
extern CColiDispatchFn lbl_eu_8056F1D8[];

void func_804A7ED0(CColiObject* self, const VEC3* v) {
    // Add v to both embedded vectors; the nw4r VEC3Add helper reproduces
    // the retail paired-single (psq_l/ps_add/psq_st) sequence.
    VEC3Add(&self->field_0x44, &self->field_0x44, v);
    VEC3Add(&self->field_0x50, &self->field_0x50, v);
}

void func_804A7F0C(CColiDispatchObj* self, u32 val) {
    self->field_0x00 = val;
    u32 idx = self->field_0x40 + lbl_eu_80665938;
    (self->*lbl_eu_8056F1D8[idx])();
}

// Sphere test with contact write-back (defined at the end of the file).
extern "C" int func_804A7F50(CColiObject* self);

// No-op handler. Used as a default stub in collision dispatch tables for
// method slots that require no action (e.g. optional callbacks).
void func_804A822C() {}

// Object reached through CColiObject::field_0x3c (the proc): the computed
// contact point at +0x0c, the contact normal at +0x18 and the vtable in
// slot 0. Vtable slot 2 is the contact callback invoked with the +0x314
// argument after the normal is written.
struct CColiObj3C;
struct CColiVtbl3C {
    void (*field_0x00)(CColiObj3C* self);           // vtable slot 0
    void (*field_0x04)(CColiObj3C* self);           // vtable slot 1
    void (*field_0x08)(CColiObj3C* self, u32 arg);  // vtable slot 2
};
struct CColiObj3C {
    CColiVtbl3C* vtbl;      // +0x00 vtable pointer
    u32 field_0x04;         // +0x04
    u32 field_0x08;         // +0x08 (seeded by the sweep helpers)
    ml::CVec3 field_0x0c;   // +0x0c contact point
    ml::CVec3 field_0x18;   // +0x18 contact normal
    ml::CVec3 field_0x24;   // +0x24 normal scaled by the penetration
};

// Partner cylinder geometry reached through CColiObject::field_0x00: the
// centre point at +0x04, the radius at +0x10 and the y-extent bounds at
// +0x14 (upper) / +0x18 (lower). func_804AC4F4 uses the +0x14/+0x18 values
// as y-offsets added to the centre instead.
struct CColiCylinder804ABA68 {
    u8 _00[0x04];
    VEC3 field_0x04;   // +0x04 centre
    f32 field_0x10;    // +0x10 radius
    f32 field_0x14;    // +0x14 y upper bound / offset
    f32 field_0x18;    // +0x18 y lower bound / offset
    u8 _1c[0x28 - 0x1c];   // +0x1c .. +0x27
    f32 field_0x28;    // +0x28 scalar (func_804ABDD4 radius term)
};

// Sphere-vs-vertical-segment sweep (retail func_804A8230): test the sphere
// at self->field_0x44 (radius field_0x5c) against the partner's vertical
// segment (base partner->field_0x04, radius field_0x10, y-band
// [field_0x18, field_0x14]). Same contact/normal/epilogue pattern as
// func_804AE388 but with the segment read through field_0x00: the contact
// point is the partner base + radial normal * radius (y at the near
// endcap), the sphere must contain it, and the contact normal is the 3D
// offset from the contact point to the sphere centre.
extern "C" int func_804A8230(CColiObject* self) {
    CColiCylinder804ABA68* partner =
        (CColiCylinder804ABA68*)self->field_0x00_obj;
    f32 top = partner->field_0x14;
    f32 radius = self->field_0x5c;
    VEC3 diff;
    VEC3Sub(&diff, &self->field_0x44, &partner->field_0x04);
    if (diff.y > top + radius) return 0;
    if (diff.y < partner->field_0x18 - radius) return 0;
    f32 rsum = radius + partner->field_0x10;
    f32 dist2 = diff.z * diff.z + diff.x * diff.x;
    if (rsum * rsum < dist2) return 0;

    CColiObj3C* proc = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
    ml::CVec3* n = &proc->field_0x18;
    f32 pen = rsum;
    f32 dist2b;
    if (diff.y <= partner->field_0x14 && diff.y >= partner->field_0x18) {
        // Sphere centre vertically inside the segment: radial normal.
        if (dist2 != lbl_eu_8066AE44) {
            if (!(dist2 >= lbl_eu_8066AE44)) {
                nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
            }
            f32 dist = dist2 <= lbl_eu_8066AE44
                           ? lbl_eu_8066AE44
                           : dist2 * nw4r::math::FrSqrt(dist2);
            pen = rsum - dist;
            f32 inv = lbl_eu_8066AE3C / dist;
            n->x = diff.x * inv;
            n->z = diff.z * inv;
        } else {
            // Random horizontal direction when the centres coincide.
            f32 rz = (f32)(rand() % 200 - 100);
            f32 rx = (f32)(rand() % 200 - 100);
            n->x = rx;
            n->y = lbl_eu_8066AE44;
            n->z = rz;
            if (n->x == lbl_eu_8066AE44 && n->z == lbl_eu_8066AE44) {
                n->x = lbl_eu_8066AE44;
                n->y = lbl_eu_8066AE44;
                n->z = lbl_eu_8066AE3C;
            } else {
                f32 len2 = n->y * n->y + n->x * n->x + n->z * n->z;
                if (len2 == lbl_eu_8066AE44) {
                    *n = ml::CVec3::zero;
                } else {
                    PSVECNormalize((const Vec*)n, (Vec*)n);
                }
            }
        }
        n->y = lbl_eu_8066AE44;
        proc->field_0x0c.x = n->x * partner->field_0x10 + partner->field_0x04.x;
        proc->field_0x0c.z = n->z * partner->field_0x10 + partner->field_0x04.z;
        proc->field_0x0c.y = self->field_0x44.y;
    } else if (self->field_0x5c * self->field_0x5c >= dist2) {
        // Sphere centre above/below the segment but horizontally within
        // its own radius: contact along the segment axis.
        n->x = lbl_eu_8066AE44;
        n->z = lbl_eu_8066AE44;
        if (diff.y > lbl_eu_8066AE44) {
            n->y = lbl_eu_8066AE3C;
            pen = partner->field_0x14 + self->field_0x5c - diff.y;
            proc->field_0x0c.y = partner->field_0x04.y + partner->field_0x14;
        } else {
            n->y = lbl_eu_8066AE50;
            pen = diff.y + self->field_0x5c - partner->field_0x18;
            proc->field_0x0c.y = partner->field_0x04.y + partner->field_0x18;
        }
        proc->field_0x0c.x = self->field_0x44.x;
        proc->field_0x0c.z = self->field_0x44.z;
    } else {
        // Corner: the sphere centre is horizontally beyond its own radius
        // but within the sum of radii. The contact point sits on the
        // segment surface (base + horizontal normal * radius, y at the
        // near endcap); the sphere must contain it and the contact normal
        // is the (3D) offset from the contact point to the sphere centre.
        if (!(dist2 >= lbl_eu_8066AE44)) {
            nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
        }
        f32 dist = dist2 <= lbl_eu_8066AE44
                       ? lbl_eu_8066AE44
                       : dist2 * nw4r::math::FrSqrt(dist2);
        f32 inv = lbl_eu_8066AE3C / dist;
        f32 nx = diff.x * inv;
        f32 nz = diff.z * inv;
        proc->field_0x0c.x = nx * partner->field_0x10 + partner->field_0x04.x;
        proc->field_0x0c.z = nz * partner->field_0x10 + partner->field_0x04.z;
        if (diff.y > lbl_eu_8066AE44) {
            proc->field_0x0c.y = partner->field_0x04.y + partner->field_0x14;
        } else {
            proc->field_0x0c.y = partner->field_0x04.y + partner->field_0x18;
        }
        pen = self->field_0x5c;
        VEC3Sub(&diff, &self->field_0x44, (const VEC3*)&proc->field_0x0c);
        dist2b = VEC3LenSq(&diff);
        if (pen * pen < dist2b) return 0;
        if (dist2b == lbl_eu_8066AE44) {
            // Random direction when the sphere centre is on the axis.
            f32 rz = (f32)(rand() % 200 - 100);
            f32 rx = (f32)(rand() % 200 - 100);
            n->x = rx;
            n->y = lbl_eu_8066AE44;
            n->z = rz;
            if (n->x == lbl_eu_8066AE44 && n->z == lbl_eu_8066AE44) {
                n->x = lbl_eu_8066AE44;
                n->y = lbl_eu_8066AE44;
                n->z = lbl_eu_8066AE3C;
            } else {
                f32 len2 = n->y * n->y + n->x * n->x + n->z * n->z;
                if (len2 == lbl_eu_8066AE44) {
                    *n = ml::CVec3::zero;
                } else {
                    PSVECNormalize((const Vec*)n, (Vec*)n);
                }
            }
        } else {
            if (!(dist2b >= lbl_eu_8066AE44)) {
                nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
            }
            f32 distb = dist2b <= lbl_eu_8066AE44
                            ? lbl_eu_8066AE44
                            : dist2b * nw4r::math::FrSqrt(dist2b);
            f32 invb = lbl_eu_8066AE3C / distb;
            VEC3Scale((VEC3*)&proc->field_0x18, &diff, invb);
            pen = self->field_0x5c - distb;
        }
    }
    // Contact normal scaled by the penetration into +0x24, then the
    // vtable slot-2 callback with the +0x314 argument.
    VEC3Scale((VEC3*)&((CColiObj3C*)(uintptr_t)self->field_0x3c_u)->field_0x24,
              (const VEC3*)&((CColiObj3C*)(uintptr_t)self->field_0x3c_u)->field_0x18,
              pen);
    CColiObj3C* pc = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
    pc->vtbl->field_0x08(pc, self->field_0x314);
    return 1;
}

// Segment-sphere test with contact write-back (defined at the end of the file).
extern "C" int func_804A8850(CColiObject* self);

// Partner transform view used by func_804A8BE0 / func_804ABBF4: the +0x34
// matrix maps self's point into the local frame, +0x64/+0x68/+0x6c are the
// half-extents, +0x04 the partner point.
struct CColiXfrm804ABBF4 {
    u8 _00[0x04];        // +0x00
    VEC3 field_0x04;     // +0x04..+0x0f partner point (func_804A8BE0 arg)
    u8 _10[0x34 - 0x10]; // +0x10..+0x33
    Mtx field_0x34;      // +0x34..+0x63 transform
    f32 field_0x64;      // +0x64 half-extent x
    f32 field_0x68;      // +0x68 half-extent y
    f32 field_0x6c;      // +0x6c half-extent z
};

// Transform the (radius, 0, 0) normal of self (+0x5c) into the partner's
// frame (+0x34) and ask func_804AD410 to classify the point against the
// partner's half-extents (+0x64/+0x04/+0x34). On a miss, re-run the same
// check with self's +0x48 scalar temporarily swapped to +0x310 when flag
// bit 4 is set; the +0x48 scalar is restored before returning.
int func_804A8BE0(CColiObject* self) {
    CColiXfrm804ABBF4* xf = (CColiXfrm804ABBF4*)self->field_0x00_obj;
    VEC3 v;
    v.x = self->field_0x5c;
    v.y = lbl_eu_8066AE44;
    v.z = lbl_eu_8066AE44;
    nw4r::math::VEC3TransformNormal(
        &v, (const nw4r::math::MTX34*)xf->field_0x34, &v);
    f32 len = PSVECMag((const Vec*)&v);
    int result = func_804AD410(self, &xf->field_0x64, &xf->field_0x04,
                               xf->field_0x34, len);
    if (result == 0) {
        if (self->field_0x8c & 0x8) {
            f32 save = self->field_0x44.y;
            self->field_0x44.y = self->field_0x310;
            f32 len = PSVECMag((const Vec*)&v);
            result = func_804AD410(self, &xf->field_0x64, &xf->field_0x04,
                                   xf->field_0x34, len);
            self->field_0x44.y = save;
        } else {
            result = 0;
        }
    }
    return result;
}

void func_804A8CB0(){}

// s32 -> f32 through the shared signed magic double (lbl_eu_8066AE48,
// 0x4330000080000000 = 2^52 + 2^31): the value is stored as the low word of
// a 0x43300000 double with the sign bit flipped, then the magic is subtracted.
// Writing the words by hand keeps the named sdata2 reloc (MWCC would
// otherwise emit its own anonymous 2^52 literal).
union ConvF64_AE48 {
    f64 d;
    u32 w[2];
};

static inline f32 s32ToF_ae48(ConvF64_AE48& c, u32 v) {
    c.w[1] = v ^ 0x80000000u;
    return (f32)(c.d - lbl_eu_8066AE48);
}

// Sphere-vs-cylinder sweep: test the sphere at self->field_0x50 (radius
// field_0x5c, height field_0x60) against the partner cylinder (axis point
// partner->field_0x04, radius field_0x10). diff = partner centre - sphere
// centre. The sphere centre must lie in the band [-radius, height + radius]
// and within the sum of radii horizontally, otherwise 0. Inside the band the
// contact is radial on the side surface; above/below it the contact sits on
// the cap (or on the sphere surface when farther than the sphere radius).
// On success the contact point/normal are written into the +0x3c proc object
// (point +0x0c, normal +0x18, scaled normal +0x24) and its vtable slot-2
// callback fires with the +0x314 argument; returns 1.
int func_804A98C4(CColiObject* self) {
    CColiCylinder804ABA68* partner =
        (CColiCylinder804ABA68*)self->field_0x00_obj;
    // s32->f32 conversion doubles (retail stack 0x18/0x20): the 0x4330 high
    // words are seeded once so MWCC hoists the stores next to the partner
    // load (retail lis r0, 0x4330 / stw pair right after the prologue).
    VEC3 diff;
    ConvF64_AE48 convA, convB;
    f32 dist2, rsum;  // dist2 declared first -> f31, rsum/pen -> f30
    f32 pen;
    convA.w[0] = 0x43300000u;
    convB.w[0] = 0x43300000u;
    VEC3Sub(&diff, &partner->field_0x04, &self->field_0x50);
    if (diff.y > self->field_0x60 + partner->field_0x10) goto FAIL;
    if (diff.y < -partner->field_0x10) goto FAIL;
    rsum = self->field_0x5c + partner->field_0x10;
    dist2 = diff.z * diff.z + diff.x * diff.x;
    if (rsum * rsum < dist2) goto FAIL;

    // Derive the contact pointers from the proc object (the load stays
    // here and the proc pointer dies immediately -> scratch r4).
    CColiObj3C* proc = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
    ml::CVec3* n = &proc->field_0x18;
    ml::CVec3* point = &proc->field_0x0c;

    // Sphere centre inside the cylinder band (0 <= diff.y <= height): radial
    // contact on the side surface.
    if (diff.y > self->field_0x60 || diff.y < lbl_eu_8066AE44) goto BEYOND;

    if (dist2 != lbl_eu_8066AE44) {
        if (!(dist2 >= lbl_eu_8066AE44)) {
            nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
        }
        f32 dist = dist2 <= lbl_eu_8066AE44
                       ? lbl_eu_8066AE44
                       : dist2 * nw4r::math::FrSqrt(dist2);
        pen = rsum - dist;
        f32 inv = lbl_eu_8066AE3C / dist;
        n->x = -diff.x * inv;
        n->z = -diff.z * inv;
    } else {
        // Random horizontal direction when the centres coincide.
        f32 rz = s32ToF_ae48(convA, (u32)(rand() % 200 - 100));
        f32 rx = s32ToF_ae48(convB, (u32)(rand() % 200 - 100));
        n->x = rx;
        n->y = lbl_eu_8066AE44;
        n->z = rz;
        if (n->x == lbl_eu_8066AE44 && n->z == lbl_eu_8066AE44) {
            n->x = lbl_eu_8066AE44;
            n->y = lbl_eu_8066AE44;
            n->z = lbl_eu_8066AE3C;
        } else {
            f32 len2 = n->y * n->y + n->x * n->x + n->z * n->z;
            if (len2 == lbl_eu_8066AE44) {
                *n = ml::CVec3::zero;
            } else {
                PSVECNormalize((const Vec*)n, (Vec*)n);
            }
        }
        pen = rsum;
    }
    // Contact point on the cylinder side surface (y at the axis point).
    point->x = n->x * partner->field_0x10 + partner->field_0x04.x;
    point->z = n->z * partner->field_0x10 + partner->field_0x04.z;
    point->y = partner->field_0x04.y;
    n->y = lbl_eu_8066AE44;
    goto EPILOGUE;

BEYOND:
    // Sphere centre above/below the band: cap contact if the sphere centre
    // is horizontally within its own radius, else sphere-surface contact.
    if (self->field_0x5c * self->field_0x5c < dist2) goto FAR;
    point->x = partner->field_0x04.x;
    point->z = partner->field_0x04.z;
    if (self->field_0x8c & 0x2) {
        // Bit 1 set: normal keeps the sphere radius scale (cap / slanted
        // cap contact with y = 1 uplift).
        if (diff.y > lbl_eu_8066AE40 * self->field_0x60) {
            // Above the cylinder's top: straight-down normal on the top cap.
            n->x = lbl_eu_8066AE44;
            n->y = lbl_eu_8066AE50;
            n->z = lbl_eu_8066AE44;
            pen = self->field_0x60 + partner->field_0x10 - diff.y;
            point->y = partner->field_0x04.y - partner->field_0x10;
            goto EPILOGUE;
        }
        if (partner->field_0x10 * partner->field_0x10 > dist2) goto AXIS;
        if (dist2 != lbl_eu_8066AE44) {
            if (!(dist2 >= lbl_eu_8066AE44)) {
                nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
            }
            f32 dist = dist2 <= lbl_eu_8066AE44
                           ? lbl_eu_8066AE44
                           : dist2 * nw4r::math::FrSqrt(dist2);
            f32 inv = lbl_eu_8066AE3C / dist;
            n->x = -diff.x * inv;
            n->z = -diff.z * inv;
        } else {
            f32 rz = s32ToF_ae48(convA, (u32)(rand() % 200 - 100));
            f32 rx = s32ToF_ae48(convB, (u32)(rand() % 200 - 100));
            n->x = rx;
            n->y = lbl_eu_8066AE44;
            n->z = rz;
            if (n->x == lbl_eu_8066AE44 && n->z == lbl_eu_8066AE44) {
                n->x = lbl_eu_8066AE44;
                n->y = lbl_eu_8066AE44;
                n->z = lbl_eu_8066AE3C;
            } else {
                f32 len2 = n->y * n->y + n->x * n->x + n->z * n->z;
                if (len2 == lbl_eu_8066AE44) {
                    *n = ml::CVec3::zero;
                } else {
                    PSVECNormalize((const Vec*)n, (Vec*)n);
                }
            }
        }
        // Uplift the horizontal normal to y = 1 and re-normalise.
        n->y = lbl_eu_8066AE3C;
        {
            f32 len2 = n->y * n->y + n->x * n->x + n->z * n->z;
            if (len2 == lbl_eu_8066AE44) {
                *n = ml::CVec3::zero;
            } else {
                PSVECNormalize((const Vec*)n, (Vec*)n);
            }
        }
        goto NEAR_TAIL;
AXIS:
        // Sphere centre horizontally inside the cylinder radius: straight-up
        // axis contact on the top cap.
        n->x = lbl_eu_8066AE44;
        n->y = lbl_eu_8066AE3C;
        n->z = lbl_eu_8066AE44;
NEAR_TAIL:
        point->y = partner->field_0x04.y + partner->field_0x10;
        pen = diff.y + partner->field_0x10;
        self->field_0x8c |= 0x8000;
        goto EPILOGUE;
    } else {
        // Bit 1 clear: normal pre-scaled by 8066AE54 * sphere radius, then
        // length-normalised (penetration = pre-normalisation length).
        if (dist2 != lbl_eu_8066AE44) {
            if (!(dist2 >= lbl_eu_8066AE44)) {
                nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
            }
            f32 dist = dist2 <= lbl_eu_8066AE44
                           ? lbl_eu_8066AE44
                           : dist2 * nw4r::math::FrSqrt(dist2);
            f32 inv = lbl_eu_8066AE3C / dist;
            f32 s = lbl_eu_8066AE54 * (inv * self->field_0x5c);
            n->x = -diff.x * s;
            n->z = -diff.z * s;
        } else {
            f32 rz = s32ToF_ae48(convA, (u32)(rand() % 200 - 100));
            f32 rx = s32ToF_ae48(convB, (u32)(rand() % 200 - 100));
            n->x = rx;
            n->y = lbl_eu_8066AE44;
            n->z = rz;
            if (n->x == lbl_eu_8066AE44 && n->z == lbl_eu_8066AE44) {
                n->x = lbl_eu_8066AE44;
                n->y = lbl_eu_8066AE44;
                n->z = lbl_eu_8066AE3C;
            } else {
                f32 len2 = n->y * n->y + n->x * n->x + n->z * n->z;
                if (len2 == lbl_eu_8066AE44) {
                    *n = ml::CVec3::zero;
                } else {
                    PSVECNormalize((const Vec*)n, (Vec*)n);
                }
            }
            n->x *= lbl_eu_8066AE54 * self->field_0x5c;
            n->z *= lbl_eu_8066AE54 * self->field_0x5c;
        }
        if (diff.y > lbl_eu_8066AE40 * self->field_0x60) {
            n->y = -(self->field_0x60 + partner->field_0x10 - diff.y);
            point->y = partner->field_0x04.y - partner->field_0x10;
        } else {
            n->y = diff.y + partner->field_0x10;
            point->y = partner->field_0x04.y + partner->field_0x10;
        }
        double mag = PSVECMag((const Vec*)n);
        pen = (f32)mag;
        f32 inv = lbl_eu_8066AE3C / (f32)mag;
        VEC3Scale((VEC3*)n, (const VEC3*)n, inv);
        goto EPILOGUE;
    }

FAR:
    // Sphere centre farther than its own radius: pick a point on the sphere
    // surface toward the cylinder axis, then check it against the partner
    // radius (sphere-surface contact when inside).
    if (dist2 != lbl_eu_8066AE44) {
        if (!(dist2 >= lbl_eu_8066AE44)) {
            nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
        }
        f32 dist = dist2 <= lbl_eu_8066AE44
                       ? lbl_eu_8066AE44
                       : dist2 * nw4r::math::FrSqrt(dist2);
        f32 inv = lbl_eu_8066AE3C / dist;
        f32 t = inv * self->field_0x5c;
        diff.x = diff.x * t;
        diff.z = diff.z * t;
    } else {
        f32 rz = s32ToF_ae48(convA, (u32)(rand() % 200 - 100));
        f32 rx = s32ToF_ae48(convB, (u32)(rand() % 200 - 100));
        diff.y = lbl_eu_8066AE44;
        diff.z = rz;
        diff.x = rx;
        if (diff.x == lbl_eu_8066AE44 && diff.z == lbl_eu_8066AE44) {
            diff.x = lbl_eu_8066AE44;
            diff.y = lbl_eu_8066AE44;
            diff.z = lbl_eu_8066AE3C;
        } else {
            f32 len2 = diff.y * diff.y + diff.x * diff.x + diff.z * diff.z;
            if (len2 == lbl_eu_8066AE44) {
                *(ml::CVec3*)&diff = ml::CVec3::zero;
            } else {
                PSVECNormalize((const Vec*)&diff, (Vec*)&diff);
            }
        }
        diff.x *= self->field_0x5c;
        diff.z *= self->field_0x5c;
    }
    // Candidate contact point on the sphere surface, then re-measure the
    // distance from the cylinder axis point to it.
    if (diff.y > lbl_eu_8066AE40 * self->field_0x60) {
        diff.y = self->field_0x50.y + self->field_0x60;
    } else {
        diff.y = self->field_0x50.y;
    }
    diff.x += self->field_0x50.x;
    diff.z += self->field_0x50.z;
    VEC3Sub(&diff, &partner->field_0x04, &diff);
    f32 dist2b = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
    if (partner->field_0x10 * partner->field_0x10 < dist2b) return 0;
    if (dist2b != lbl_eu_8066AE44) {
        if (!(dist2b >= lbl_eu_8066AE44)) {
            nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
        }
        f32 distb = dist2b <= lbl_eu_8066AE44
                        ? lbl_eu_8066AE44
                        : dist2b * nw4r::math::FrSqrt(dist2b);
        f32 invb = lbl_eu_8066AE3C / distb;
        n->x = -diff.x * invb;
        n->y = -diff.y * invb;
        n->z = -diff.z * invb;
        pen = partner->field_0x10 - distb;
    } else {
        f32 rz = s32ToF_ae48(convA, (u32)(rand() % 200 - 100));
        f32 rx = s32ToF_ae48(convB, (u32)(rand() % 200 - 100));
        n->x = rx;
        n->y = lbl_eu_8066AE44;
        n->z = rz;
        if (n->x == lbl_eu_8066AE44 && n->z == lbl_eu_8066AE44) {
            n->x = lbl_eu_8066AE44;
            n->y = lbl_eu_8066AE44;
            n->z = lbl_eu_8066AE3C;
        } else {
            f32 len2 = n->y * n->y + n->x * n->x + n->z * n->z;
            if (len2 == lbl_eu_8066AE44) {
                *n = ml::CVec3::zero;
            } else {
                PSVECNormalize((const Vec*)n, (Vec*)n);
            }
        }
        pen = partner->field_0x10;
    }
    point->x = n->x * partner->field_0x10 + partner->field_0x04.x;
    point->y = n->y * partner->field_0x10 + partner->field_0x04.y;
    point->z = n->z * partner->field_0x10 + partner->field_0x04.z;
    if (self->field_0x8c & 0x2) self->field_0x8c |= 0x8000;
    goto EPILOGUE;

EPILOGUE:
    // Contact normal scaled by the penetration into +0x24, then the vtable
    // slot-2 callback with the +0x314 argument. The proc is re-read here so
    // the load stays in scratch (retail lwz r3, 0x3c(r28)).
    CColiObj3C* ep = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
    ep->field_0x24.x = n->x * pen;
    ep->field_0x24.y = n->y * pen;
    ep->field_0x24.z = n->z * pen;
    ep->vtbl->field_0x08(ep, self->field_0x314);
    return 1;

FAIL:
    return 0;
}

// Delegates to the segment-sphere contact test, describing the partner
// object's sphere from its centre (+4) and band/radius (+16) fields.
// Segment-sphere contact test (defined at the end of the file).
extern "C" int func_804AD8FC(CColiObject* self, const _VEC3* a, const _VEC3* b);
extern "C" int func_804AA4F4(CColiObject* self) {
    return func_804AD8FC(self, (const _VEC3*)((u8*)self->field_0x00_obj + 4),
                         (const _VEC3*)((u8*)self->field_0x00_obj + 16));
}

// Two-point clip sweep with contact write-back (defined at the end of the file).
extern "C" int func_804AA504(CColiObject* self);


// Partner geometry for func_804AA870: centre at +0x04 and radius at +0x10
// (subset of the CColiCylinder804ABA68 fields used here).
struct CColiCyl804AA870 {
    u8 _00[0x04];
    VEC3 field_0x04;   // +0x04 centre
    f32 field_0x10;    // +0x10 radius
};

// Segment-capsule collision with normal write-back: project self's point
// (+0x44) along its segment direction (+0x68) onto the partner sphere
// (+0x04 centre, +0x10 radius). On a hit, write the contact point
// (point + dir*t) and the contact normal (point - partner centre,
// normalised or zeroed) into the +0x3c proc object and fire its contact
// callback (vtable slot 2). The t parameter is the root of
// |point + dir*t - centre|^2 == r^2 along the segment.
bool func_804AA870(CColiObject* self) {
    CColiCyl804AA870* partner = (CColiCyl804AA870*)self->field_0x00_obj;
    VEC3 diff;
    VEC3Sub(&diff, &self->field_0x44, &partner->field_0x04);
    f32 dist2 = VEC3LenSq(&diff);
    f32 dot = VEC3Dot(&diff, &self->field_0x68);
    f32 r = partner->field_0x10;
    f32 x = dist2 - r * r;
    if (x >= lbl_eu_8066AE44 && dot >= lbl_eu_8066AE44) return false;
    f32 d = dot * dot - x;
    if (d <= lbl_eu_8066AE44) return false;
    if (!(d >= lbl_eu_8066AE44)) {
        nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
    }
    f32 sq = d <= lbl_eu_8066AE44 ? lbl_eu_8066AE44 : d * nw4r::math::FrSqrt(d);
    f32 t = -dot - sq;
    if (t < lbl_eu_8066AE44 || t > self->field_0x308) return false;

    // Contact point: point + dir * t, written into the proc object (reuses
    // the dead diff local, so it keeps the single sp+0x8 slot).
    CColiObj3C* obj = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
    VEC3Scale(&diff, &self->field_0x68, t);
    VEC3Add((VEC3*)&obj->field_0x0c, &diff, &self->field_0x44);

    // Contact normal: point - partner centre, then normalise (or zero it
    // when degenerate).
    obj = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
    VEC3Sub((VEC3*)&obj->field_0x18, (VEC3*)&obj->field_0x0c,
            &((CColiCyl804AA870*)self->field_0x00_obj)->field_0x04);
    ml::CVec3* n = &((CColiObj3C*)(uintptr_t)self->field_0x3c_u)->field_0x18;
    f32 len2 = n->y * n->y + n->x * n->x + n->z * n->z;
    if (len2 == lbl_eu_8066AE44) {
        *n = ml::CVec3::zero;
    } else {
        PSVECNormalize((const Vec*)n, (Vec*)n);
    }

    // Fire the contact callback (vtable slot 2) with the +0x314 arg.
    CColiObj3C* proc = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
    proc->vtbl->field_0x08(proc, self->field_0x314);
    return true;
}

// Cylinder clip with contact write-back (defined at the end of the file).
extern "C" int func_804AAA98(CColiObject* self);

void func_804AAD90(){}

// Segment-vs-segment clip (defined at the end of the file): passes the
// partner's +0x64 scalar list, +4 AABB matrix and +0x34 transform matrix.
extern "C" int func_804ADD3C(CColiObject* self, const f32* a, const Mtx b,
                             const Mtx c);
extern "C" int func_804AB524(CColiObject* self) {
    CColiObject* partner = self->field_0x00_obj;
    return func_804ADD3C(self, (const f32*)((u8*)partner + 0x64),
                         (const f32(*)[4])((u8*)partner + 0x04),
                         (const f32(*)[4])((u8*)partner + 0x34));
}

// 16-byte collision registration record: two sub-object ids, a point and a
// radius, plus the kind (+0x0c; 3 = group of sub-objects) and the group
// member count (+0x0e). Records live in a stride-16 array behind the
// lbl_eu_8066592C pointer; a group's member ids are u16s in the byte-offset
// array behind lbl_eu_80665930.
struct CColiRec16 {
    union {
        u32 field_0x00_off;   // +0x00 byte offset (group path)
        struct {
            u16 field_0x00;   // +0x00 sub-object id A
            u16 field_0x02;   // +0x02 sub-object id B
        };
    };
    f32 field_0x04;           // +0x04 point y
    f32 field_0x08;           // +0x08 radius
    u16 field_0x0c;           // +0x0c kind (3 = group)
    u16 field_0x0e;           // +0x0e group member count
};

// Sibling record handlers used by the sweep pass below.
extern "C" void func_804AD1E0(CColiObject* self, u16 id);
extern "C" void func_804AC9F4(CColiObject* self, CColiRec16* obj);

// Sweep pass (retail func_804AB538): transform both self points into the
// partner frame, fold them into per-kind min/max bounds, clear the visited
// bitset, re-base the 0x14-stride record array and run the top record's
// A/B child recursion (group records enumerate their members via
// func_804AD1E0, other records recurse via func_804AC9F4). Returns 2 when
// the sweep registered a hit (+0x80 counter), else 0.
extern "C" int func_804AB538(CColiObject* self) {
    PSMTXMultVec((const f32(*)[4])((u8*)self->field_0x00_obj + 0x34),
                 (const Vec*)&self->field_0x44, (Vec*)&self->field_0x2d8);
    PSMTXMultVec((const f32(*)[4])((u8*)self->field_0x00_obj + 0x34),
                 (const Vec*)&self->field_0x50, (Vec*)&self->field_0x2e4);

    // Fold the transformed point pair into per-axis min (0x2fc) / max
    // (0x2f0) bounds.
    if (self->field_0x2d8[0] > self->field_0x2e4[0]) {
        self->field_0x2f0[0] = self->field_0x2d8[0];
        self->field_0x2fc[0] = self->field_0x2e4[0];
    } else {
        self->field_0x2f0[0] = self->field_0x2e4[0];
        self->field_0x2fc[0] = self->field_0x2d8[0];
    }
    if (self->field_0x2d8[1] > self->field_0x2e4[1]) {
        self->field_0x2f0[1] = self->field_0x2d8[1];
        self->field_0x2fc[1] = self->field_0x2e4[1];
    } else {
        self->field_0x2f0[1] = self->field_0x2e4[1];
        self->field_0x2fc[1] = self->field_0x2d8[1];
    }
    if (self->field_0x2d8[2] > self->field_0x2e4[2]) {
        self->field_0x2f0[2] = self->field_0x2d8[2];
        self->field_0x2fc[2] = self->field_0x2e4[2];
    } else {
        self->field_0x2f0[2] = self->field_0x2e4[2];
        self->field_0x2fc[2] = self->field_0x2d8[2];
    }

    CColiSweepCtrl* ctrl = lbl_eu_80665934;
    memset(lbl_eu_8065CFA0, 0, ((ctrl->field_0x08 >> 5) + 1) * 4);
    lbl_eu_80665924 =
        (ml::coli::CColiObject*)((u8*)lbl_eu_80665920 + ctrl->field_0x00 * 0x14);
    VEC3Sub(&self->field_0x68, (const VEC3*)self->field_0x2e4,
            (const VEC3*)self->field_0x2d8);
    self->field_0x80 = 0;

    CColiRec16* rec = &((CColiRec16*)lbl_eu_8066592C)[ctrl->field_0x04];
    u16 kind = rec->field_0x0c;
    if (kind == 3) {
        // Group: mark each member id as visited and process it.
        u16* arr = (u16*)((u8*)lbl_eu_80665930 + rec->field_0x00_off);
        u16 count = rec->field_0x0e;
        for (int i = 0; i < count; i++) {
            u16 id = arr[i];
            u32 bit = 1u << (id & 31);
            if ((lbl_eu_8065CFA0[id >> 5] & bit) == 0) {
                lbl_eu_8065CFA0[id >> 5] |= bit;
                func_804AD1E0(self, id);
            }
        }
    } else {
        // A: the record's sphere within self's box at the record's kind.
        if (self->field_0x2fc[kind] <= rec->field_0x04 + rec->field_0x08 &&
            self->field_0x2f0[kind] >= rec->field_0x04 &&
            rec->field_0x00 != 0) {
            CColiRec16* sub = &((CColiRec16*)lbl_eu_8066592C)[rec->field_0x00];
            if (sub->field_0x0c == 3) {
                u16* arr = (u16*)((u8*)lbl_eu_80665930 + sub->field_0x00_off);
                u16 count = sub->field_0x0e;
                for (int i = 0; i < count; i++) {
                    u16 id = arr[i];
                    u32 bit = 1u << (id & 31);
                    if ((lbl_eu_8065CFA0[id >> 5] & bit) == 0) {
                        lbl_eu_8065CFA0[id >> 5] |= bit;
                        func_804AD1E0(self, id);
                    }
                }
            } else {
                u16 k1 = sub->field_0x0c;
                if (self->field_0x2fc[k1] <= sub->field_0x04 + sub->field_0x08 &&
                    self->field_0x2f0[k1] >= sub->field_0x04 &&
                    sub->field_0x00 != 0) {
                    func_804AC9F4(
                        self, &((CColiRec16*)lbl_eu_8066592C)[sub->field_0x00]);
                }
                if (self->field_0x2fc[k1] <= sub->field_0x04 &&
                    self->field_0x2f0[k1] >= sub->field_0x04 - sub->field_0x08 &&
                    sub->field_0x02 != 0) {
                    func_804AC9F4(
                        self, &((CColiRec16*)lbl_eu_8066592C)[sub->field_0x02]);
                }
            }
        }
        // B: the record's point within the shrunken box at its kind.
        if (self->field_0x2fc[kind] <= rec->field_0x04 &&
            self->field_0x2f0[kind] >= rec->field_0x04 - rec->field_0x08 &&
            rec->field_0x02 != 0) {
            CColiRec16* sub2 = &((CColiRec16*)lbl_eu_8066592C)[rec->field_0x02];
            if (sub2->field_0x0c == 3) {
                u16* arr = (u16*)((u8*)lbl_eu_80665930 + sub2->field_0x00_off);
                u16 count = sub2->field_0x0e;
                for (int i = 0; i < count; i++) {
                    u16 id = arr[i];
                    u32 bit = 1u << (id & 31);
                    if ((lbl_eu_8065CFA0[id >> 5] & bit) == 0) {
                        lbl_eu_8065CFA0[id >> 5] |= bit;
                        func_804AD1E0(self, id);
                    }
                }
            } else {
                u16 k2 = sub2->field_0x0c;
                if (self->field_0x2fc[k2] <= sub2->field_0x04 + sub2->field_0x08 &&
                    self->field_0x2f0[k2] >= sub2->field_0x04 &&
                    sub2->field_0x00 != 0) {
                    func_804AC9F4(
                        self, &((CColiRec16*)lbl_eu_8066592C)[sub2->field_0x00]);
                }
                if (self->field_0x2fc[k2] <= sub2->field_0x04 &&
                    self->field_0x2f0[k2] >= sub2->field_0x04 - sub2->field_0x08 &&
                    sub2->field_0x02 != 0) {
                    func_804AC9F4(
                        self, &((CColiRec16*)lbl_eu_8066592C)[sub2->field_0x02]);
                }
            }
        }
    }
    return self->field_0x80 != 0 ? 2 : 0;
}

void func_804ABA08(){}

// Cylinder-overlap test: true when the horizontal squared distance from
// self's point (+0x44) to the partner's centre (+0x04) is within the
// partner's squared radius (+0x10) and the vertical offset lies between
// the partner's y bounds (+0x14 upper, +0x18 lower). VEC3Sub lowers to
// the retail paired-single (psq_l/ps_sub/psq_st) sequence.
bool func_804ABA68(CColiObject* self) {
    CColiCylinder804ABA68* other = (CColiCylinder804ABA68*)self->field_0x00_obj;
    VEC3 diff;
    VEC3Sub(&diff, &self->field_0x44, &other->field_0x04);
    if (diff.y <= other->field_0x14 && diff.y >= other->field_0x18) {
        // Squared horizontal distance vs squared radius; the dist
        // expression is evaluated first so the r^2 fmuls lands between the
        // dx^2 and the fmadds (retail's schedule).
        f32 dist = diff.x * diff.x + diff.z * diff.z;
        if (other->field_0x10 * other->field_0x10 >= dist) {
            return true;
        }
    }
    return false;
}
// Segment/capsule partner geometry for func_804ABAF0: base point at +0x04,
// direction vector at +0x1c, radius at +0x28 and the direction length^2 at
// +0x2c (used to normalise the projection parameter).
struct CColiSegment804ABAF0 {
    u8 _00[0x04];
    VEC3 field_0x04;       // +0x04 base point
    u8 _10[0x1c - 0x10];   // +0x10..+0x1b
    VEC3 field_0x1c;       // +0x1c direction
    f32 field_0x28;        // +0x28 radius
    f32 field_0x2c;        // +0x2c direction length^2
};

// Capsule test: project self's point (+0x44) onto the partner's segment
// (base +0x04, direction +0x1c, param clamped to [0,1] via length^2 +0x2c),
// then compare the squared distance to the closest point against the squared
// radius (+0x28). VEC3Sub/VEC3Dot/VEC3Scale/VEC3Add/VEC3LenSq lower to the
// retail paired-single sequences.
bool func_804ABAF0(CColiObject* self) {
    CColiSegment804ABAF0* o = (CColiSegment804ABAF0*)self->field_0x00_obj;
    VEC3 v;
    VEC3 d;
    VEC3Sub(&d, &self->field_0x44, &o->field_0x04);
    f32 t = clamp01(VEC3Dot(&o->field_0x1c, &d) / o->field_0x2c);
    VEC3Scale(&v, &o->field_0x1c, t);
    VEC3Add(&v, &v, &o->field_0x04);
    VEC3Sub(&d, &self->field_0x44, &v);
    return o->field_0x28 * o->field_0x28 >= VEC3LenSq(&d);
}

// Partner transform AABB test: the partner's +0x34 matrix maps self's point
// (+0x44) into the local frame, then each axis of the result is checked
// against the partner's half-extents at +0x64/+0x68/+0x6c (|axis| <= extent).
bool func_804ABBF4(CColiObject* self) {
    CColiXfrm804ABBF4* obj = (CColiXfrm804ABBF4*)self->field_0x00_obj;
    VEC3 out;
    PSMTXMultVec(obj->field_0x34, self->field_0x44, out);
    obj = (CColiXfrm804ABBF4*)self->field_0x00_obj;
    if (obj->field_0x64 < out.x || -obj->field_0x64 > out.x) return false;
    if (obj->field_0x68 < out.y || -obj->field_0x68 > out.y) return false;
    if (obj->field_0x6c < out.z || -obj->field_0x6c > out.z) return false;
    return true;
}

// Partner collision record reached through CColiObject::field_0x00: the
// point at +0x04 and the radius at +0x10.
struct CColiSphere804ABCA4 {
    u8 _00[0x04];
    VEC3 field_0x04;   // +0x04 centre
    f32 field_0x10;    // +0x10 radius
};

// Sphere-overlap test: true when the squared distance between self's point
// (+0x44) and the partner's point (+0x04) is within the squared sum of the
// two radii (+0x10, +0x5c). VEC3Sub/VEC3LenSq lower to the retail
// paired-single sequence (psq_l/ps_sub/psq_st, ps_mul/ps_madd/ps_sum0).
bool func_804ABCA4(CColiObject* self) {
    CColiObject* other = self->field_0x00_obj;
    VEC3 diff;
    VEC3Sub(&diff, &self->field_0x44, (VEC3*)((u8*)other + 4));
    f32 r = other->field_0x10_f + self->field_0x5c;
    return r * r >= VEC3LenSq(&diff);
}

// Cylinder-capped sphere test: clamp self's point (+0x44) y-coordinate into
// the partner's [centre.y + lower, centre.y + upper] band, then test the
// squared distance against the squared sum of the partner radius (+0x10) and
// self's scalar (+0x5c). Mirrors func_804ABCA4 with the clamped point;
// VEC3Sub/VEC3LenSq lower to the retail paired-single sequence.
bool func_804ABD0C(CColiObject* self) {
    CColiCylinder804ABA68* obj = (CColiCylinder804ABA68*)self->field_0x00_obj;
    VEC3 v;
    VEC3 diff;
    v.x = obj->field_0x04.x;
    v.y = obj->field_0x04.y;
    v.z = obj->field_0x04.z;
    if (v.y + obj->field_0x14 <= self->field_0x44.y) {
        v.y = v.y + obj->field_0x14;
    } else {
        f32 lo = v.y + obj->field_0x18;
        if (lo >= self->field_0x44.y) {
            v.y = lo;
        } else {
            v.y = self->field_0x44.y;
        }
    }
    f32 r = ((CColiCylinder804ABA68*)self->field_0x00_obj)->field_0x10 +
            self->field_0x5c;
    VEC3Sub(&diff, &self->field_0x44, &v);
    return r * r >= VEC3LenSq(&diff);
}

// Sphere test against the partner's point pair: squared distance from
// self's point (+0x44) to the partner's +0x04 point, then to its +0x10
// point, each within the squared sum of the partner +0x28 scalar and
// self's +0x5c radius. Mirrors func_804ABCA4 with the second point.
bool func_804ABDD4(CColiObject* self) {
    CColiCylinder804ABA68* obj = (CColiCylinder804ABA68*)self->field_0x00_obj;
    f32 r5c = self->field_0x5c;
    VEC3 diff1;
    VEC3Sub(&diff1, &self->field_0x44, &obj->field_0x04);
    f32 r = obj->field_0x28 + r5c;
    f32 r2 = r * r;
    if (r2 >= VEC3LenSq(&diff1)) {
        return true;
    }
    VEC3 diff2;
    VEC3Sub(&diff2, &self->field_0x44, (VEC3*)&obj->field_0x10);
    return r2 >= VEC3LenSq(&diff2);
}

// Sphere/cone test against the cylinder partner: returns true when the
// squared distance from self's point (+0x44) to the partner centre (+0x04)
// is within the partner's squared radius (+0x10), or within the squared sum
// of the partner radius and self's scalar (+0x5c) AND the azimuth of the
// offset vector (Atan2FIdx of x/z) scaled by the FIdx constant, offset by
// +0x64 and wrapped into [-pi, pi], lies within the +0x60 bound.
bool func_804ABF08(CColiObject* self) {
    CColiCylinder804ABA68* other = (CColiCylinder804ABA68*)self->field_0x00_obj;
    VEC3 diff;
    VEC3Sub(&diff, &self->field_0x44, &other->field_0x04);
    f32 r = other->field_0x10;
    f32 r2 = r * r;
    f32 dist2 = VEC3LenSq(&diff);
    if (r2 >= dist2) return true;
    f32 rs = r + self->field_0x5c;
    if (rs * rs >= dist2) {
        f32 ang = lbl_eu_8066AE60 *
                      nw4r::math::Atan2FIdx(diff.x, diff.z) -
                  self->field_0x64;
        while (ang >= lbl_eu_8066A1F8) ang -= lbl_eu_8066A1FC;
        while (ang < -lbl_eu_8066A1F8) ang += lbl_eu_8066A1FC;
        if (-self->field_0x60 <= ang && ang <= self->field_0x60) return true;
    }
    return false;
}

// Cylinder-with-angle test: clamp self's point y (+0x48) into the partner's
// vertical band [centre.y + lower, centre.y + upper], then accept when the
// squared distance from the clamped point to self's point (+0x44) is within
// the partner's squared radius, or within the squared sum of the partner
// radius and self's scalar (+0x5c) AND the azimuth (Atan2FIdx of x/z scaled
// by the FIdx constant, offset by +0x64, wrapped into [-pi, pi]) lies within
// the +0x60 bound. Mirrors func_804ABF08 with the clamped point.
bool func_804AC020(CColiObject* self) {
    CColiCylinder804ABA68* other = (CColiCylinder804ABA68*)self->field_0x00_obj;
    VEC3 diff;
    VEC3 v;
    v.x = other->field_0x04.x;
    v.y = other->field_0x04.y;
    v.z = other->field_0x04.z;
    if (v.y + other->field_0x14 <= self->field_0x44.y) {
        v.y = v.y + other->field_0x14;
    } else {
        f32 lo = v.y + other->field_0x18;
        if (lo >= self->field_0x44.y) {
            v.y = lo;
        } else {
            v.y = self->field_0x44.y;
        }
    }
    VEC3Sub(&diff, &v, &self->field_0x44);
    f32 r = other->field_0x10;
    f32 r2 = r * r;
    f32 dist2 = VEC3LenSq(&diff);
    if (r2 >= dist2) return true;
    f32 rs = r + self->field_0x5c;
    if (rs * rs >= dist2) {
        f32 ang = lbl_eu_8066AE60 *
                      nw4r::math::Atan2FIdx(diff.x, diff.z) -
                  self->field_0x64;
        while (ang >= lbl_eu_8066A1F8) ang -= lbl_eu_8066A1FC;
        while (ang < -lbl_eu_8066A1F8) ang += lbl_eu_8066A1FC;
        if (-self->field_0x60 <= ang && ang <= self->field_0x60) return true;
    }
    return false;
}

// Angle gate for the sphere-or-cone test: wrap the FIdx-scaled azimuth into
// [-pi, pi] and test it against the +0x60 bound (inline; MWCC expands it at
// the condition site so the retail's bl Atan2FIdx lands inside the || chain).
static inline bool angleOK804AC198(CColiObject* self, const VEC3* d) {
    f32 ang = lbl_eu_8066AE60 * nw4r::math::Atan2FIdx(d->x, d->z) -
              self->field_0x64;
    while (ang >= lbl_eu_8066A1F8) ang -= lbl_eu_8066A1FC;
    while (ang < -lbl_eu_8066A1F8) ang += lbl_eu_8066A1FC;
    return -self->field_0x60 <= ang && ang <= self->field_0x60;
}

// Sphere-or-cone test against both partner points (+0x04 then +0x10): for
// each point, accept when the squared distance from self's point (+0x44) is
// within the partner's squared radius (+0x28), or within the squared sum of
// the partner radius and self's radius (+0x5c) AND the angle gate passes.
// The partner pointer is re-read from self->field_0x00 for each point so it
// never survives the Atan2FIdx call (retail reloads it in block 2).
bool func_804AC198(CColiObject* self) {
    CColiCylinder804ABA68* obj = (CColiCylinder804ABA68*)self->field_0x00_obj;

    VEC3 diff;
    VEC3Sub(&diff, &self->field_0x44, &obj->field_0x04);
    f32 r = obj->field_0x28;
    f32 r2 = r * r;
    f32 dist2 = VEC3LenSq(&diff);
    bool hit =
        r2 >= dist2 ||
        ((r + self->field_0x5c) * (r + self->field_0x5c) >= dist2 &&
         angleOK804AC198(self, &diff));
    if (hit) return true;

    obj = (CColiCylinder804ABA68*)self->field_0x00_obj;
    VEC3 diff2;
    VEC3Sub(&diff2, &self->field_0x44, (VEC3*)&obj->field_0x10);
    r = obj->field_0x28;
    r2 = r * r;
    dist2 = VEC3LenSq(&diff2);
    if (r2 >= dist2 ||
        ((r + self->field_0x5c) * (r + self->field_0x5c) >= dist2 &&
         angleOK804AC198(self, &diff2))) {
        return true;
    }
    return false;
}

// Contact-partner geometry view used by func_804AC3B0: point rows at
// +0x10/+0x20/+0x30 and the scalar at +0x70 (same fields CColiContactObj
// declares for the clip helper).
struct CColiContact804AC3B0 {
    u8 _00[0x10];            // +0x00 .. +0x10
    f32 field_0x10;          // +0x10
    u8 _14[0x20 - 0x14];     // +0x14 .. +0x20
    f32 field_0x20;          // +0x20
    u8 _24[0x30 - 0x24];     // +0x24 .. +0x30
    f32 field_0x30;          // +0x30
    u8 _34[0x70 - 0x34];     // +0x34 .. +0x70
    f32 field_0x70;          // +0x70
};

// Sphere/cone test against the contact partner: returns true when the
// squared distance from self's point (+0x44) to the partner's point rows
// (+0x10/+0x20/+0x30) is within the partner's squared scalar (+0x70), or
// within the squared sum of the partner scalar and self's radius (+0x5c)
// AND the azimuth of the offset vector (Atan2FIdx of x/z) scaled by the
// FIdx constant, offset by +0x64 and wrapped into [-pi, pi], lies within
// the +0x60 bound.
bool func_804AC3B0(CColiObject* self) {
    CColiContact804AC3B0* obj = (CColiContact804AC3B0*)self->field_0x00_obj;
    VEC3 v(obj->field_0x10, obj->field_0x20, obj->field_0x30);
    VEC3 diff;
    VEC3Sub(&diff, &v, &self->field_0x44);
    f32 r = obj->field_0x70;
    // r2 declared before the length call so MWCC schedules the fmuls early
    // (retail interleaves it with the VEC3Sub), mirroring func_804ABF08.
    f32 r2 = r * r;
    f32 dist2 = VEC3LenSq(&diff);
    if (r2 >= dist2) return true;
    f32 rs = r + self->field_0x5c;
    if (rs * rs >= dist2) {
        f32 ang = lbl_eu_8066AE60 *
                      nw4r::math::Atan2FIdx(diff.x, diff.z) -
                  self->field_0x64;
        while (ang >= lbl_eu_8066A1F8) ang -= lbl_eu_8066A1FC;
        while (ang < -lbl_eu_8066A1F8) ang += lbl_eu_8066A1FC;
        if (-self->field_0x60 <= ang && ang <= self->field_0x60) return true;
    }
    return false;
}

void func_804AC57C(){}

// Contact object sampled by func_804AC5D8 / func_804AF808: field_0x00 links
// the paired collision object; the VEC3s at +0x10/+0x20/+0x30 and the scalar
// at +0x70 describe the segment passed to the clip helper.
struct CColiContactObj {
    CColiContactObj* field_0x00; // +0x00 linked partner
    u8 _04[0x10 - 0x04];         // +0x04 .. +0x10
    f32 field_0x10;              // +0x10 (point rows are 0x10-stride)
    u8 _14[0x20 - 0x14];         // +0x14 .. +0x20
    f32 field_0x20;              // +0x20
    u8 _24[0x30 - 0x24];         // +0x24 .. +0x30
    f32 field_0x30;              // +0x30
    u8 _34[0x70 - 0x34];         // +0x34 .. +0x70
    f32 field_0x70;              // +0x70
};

extern "C" int func_804AF808(CColiContactObj* self, const VEC3* v, f32 f);

// Delegates to the contact clip helper, describing the partner object's
// segment from its centre (+4) and radius (+0x10) fields.
extern "C" int func_804AC4E4(CColiObject* self) {
    CColiObject* partner = self->field_0x00_obj;
    f32 radius = partner->field_0x10_f;
    const VEC3* seg = (const VEC3*)((u8*)partner + 4);
    return func_804AF808((CColiContactObj*)self, seg, radius);
}

// Sample the partner's point (+0x04) into v, clamp v.y so the caller's
// +0x48 scalar lies inside [v.y + field_0x18, v.y + field_0x14], then hand
// the clamped segment to the clip helper along with the partner radius
// (+0x10). The VEC3 fields are written component-wise so MWCC keeps v.y
// live in a register for the clamping adds (retail's f2).
void func_804AC4F4(CColiObject* self) {
    CColiCylinder804ABA68* obj = (CColiCylinder804ABA68*)self->field_0x00_obj;
    VEC3 v;
    v.x = obj->field_0x04.x;
    v.y = obj->field_0x04.y;
    v.z = obj->field_0x04.z;
    if (v.y + obj->field_0x14 <= self->field_0x44.y) {
        v.y = v.y + obj->field_0x14;
    } else {
        f32 hi = v.y + obj->field_0x18;
        if (hi >= self->field_0x44.y) {
            v.y = hi;
        } else {
            v.y = self->field_0x44.y;
        }
    }
    func_804AF808((CColiContactObj*)self, &v,
                  ((CColiCylinder804ABA68*)self->field_0x00_obj)->field_0x10);
}

// Sphere-overlap test against the contact partner: squared distance from
// self's point (+0x44) to the partner's point rows (+0x10/+0x20/+0x30) is
// within the squared sum of the partner scalar (+0x70) and self's radius
// (+0x5c). Mirrors func_804ABCA4 with the v built from the partner rows.
bool func_804ABE84(CColiObject* self) {
    CColiContactObj* obj = (CColiContactObj*)self->field_0x00_obj;
    VEC3 v(obj->field_0x10, obj->field_0x20, obj->field_0x30);
    VEC3 diff;
    VEC3Sub(&diff, &self->field_0x44, &v);
    f32 r = obj->field_0x70 + self->field_0x5c;
    return r * r >= VEC3LenSq(&diff);
}

extern "C" int func_804AF808(CColiContactObj* self, const VEC3* v, f32 f);

extern "C" int func_804AF808(CColiContactObj* self, const VEC3* v, f32 f);

void func_804AC5D8(CColiContactObj* self) {
    // Sample the partner's point rows (+0x10/+0x20/+0x30) plus the scalar at
    // +0x70, then ask the clip helper to classify the segment. The VEC3
    // 3-float ctor inlines to loads z,y,x into f2,f1,f0 then ascending stores.
    CColiContactObj* obj = self->field_0x00;
    VEC3 v(obj->field_0x10, obj->field_0x20, obj->field_0x30);
    func_804AF808(self, &v, obj->field_0x70);
}

// Default false-return handler. Used as a placeholder in collision method
// dispatch tables for boolean-returning virtual method slots that are
// not overridden (always returns 0 / false). The self pointer is unused.
int func_804AC61C(CColiObject* /*self*/) { return 0; }

// Enumerate a group record's members: each id not yet marked in the shared
// visited bitset is marked and handed to the per-id processor. (Inlined at
// each group site; retail keeps three separate copies.)

extern "C" void func_804AD1E0(CColiObject* self, u16 id);
extern "C" void func_804ACD9C(CColiObject* self, u16 id);

// Recurse the record tree: for each non-group record, the record's sphere
// (field_0x04 + field_0x08) must sit within self's per-kind box bounds
// (field_0x2fc upper / field_0x2f0 lower) for its id-A child to be
// processed, and its point (field_0x04) must sit within the shrunken box
// for the id-B child. Group children enumerate their members instead of
// recursing. Records are 16-byte entries behind lbl_eu_8066592C.
extern "C" void func_804AC9F4(CColiObject* self, CColiRec16* obj) {
    u16 kind = obj->field_0x0c;
    if (kind == 3) {
        // Group: mark each member id as visited and process it.
        u16 count = obj->field_0x0e;
        u16* arr = (u16*)((u8*)lbl_eu_80665930 + obj->field_0x00_off);
        int i;
        for (i = 0; i < count; i++) {
            u16 id = arr[i];
            u32 bit = 1u << (id & 31);
            if ((lbl_eu_8065CFA0[id >> 5] & bit) == 0) {
                lbl_eu_8065CFA0[id >> 5] |= bit;
                func_804AD1E0(self, id);
            }
        }
        return;
    }
    // A: the record's sphere within self's box at the record's kind index.
    if (self->field_0x2fc[kind] <= obj->field_0x04 + obj->field_0x08 &&
        self->field_0x2f0[kind] >= obj->field_0x04 && obj->field_0x00 != 0) {
        CColiRec16* sub =
            &((CColiRec16*)lbl_eu_8066592C)[obj->field_0x00];
        if (sub->field_0x0c == 3) {
            u16 count = sub->field_0x0e;
            u16* arr = (u16*)((u8*)lbl_eu_80665930 + sub->field_0x00_off);
            int i;
            for (i = 0; i < count; i++) {
                u16 id = arr[i];
                u32 bit = 1u << (id & 31);
                if ((lbl_eu_8065CFA0[id >> 5] & bit) == 0) {
                    lbl_eu_8065CFA0[id >> 5] |= bit;
                    func_804AD1E0(self, id);
                }
            }
        } else {
            u16 k1 = sub->field_0x0c;
            if (self->field_0x2fc[k1] <= sub->field_0x04 + sub->field_0x08 &&
                self->field_0x2f0[k1] >= sub->field_0x04 &&
                sub->field_0x00 != 0) {
                func_804AC9F4(self,
                             &((CColiRec16*)lbl_eu_8066592C)[sub->field_0x00]);
            }
            if (self->field_0x2fc[k1] <= sub->field_0x04 &&
                self->field_0x2f0[k1] >= sub->field_0x04 - sub->field_0x08 &&
                sub->field_0x02 != 0) {
                func_804AC9F4(self,
                             &((CColiRec16*)lbl_eu_8066592C)[sub->field_0x02]);
            }
        }
    }
    // B: the record's point within the shrunken box at its kind index.
    if (self->field_0x2fc[kind] <= obj->field_0x04 &&
        self->field_0x2f0[kind] >= obj->field_0x04 - obj->field_0x08 &&
        obj->field_0x02 != 0) {
        CColiRec16* sub2 =
            &((CColiRec16*)lbl_eu_8066592C)[obj->field_0x02];
        if (sub2->field_0x0c == 3) {
            u16 count = sub2->field_0x0e;
            u16* arr = (u16*)((u8*)lbl_eu_80665930 + sub2->field_0x00_off);
            int i;
            for (i = 0; i < count; i++) {
                u16 id = arr[i];
                u32 bit = 1u << (id & 31);
                if ((lbl_eu_8065CFA0[id >> 5] & bit) == 0) {
                    lbl_eu_8065CFA0[id >> 5] |= bit;
                    func_804AD1E0(self, id);
                }
            }
        } else {
            u16 k2 = sub2->field_0x0c;
            if (self->field_0x2fc[k2] <= sub2->field_0x04 + sub2->field_0x08 &&
                self->field_0x2f0[k2] >= sub2->field_0x04 &&
                sub2->field_0x00 != 0) {
                func_804AC9F4(self,
                             &((CColiRec16*)lbl_eu_8066592C)[sub2->field_0x00]);
            }
            if (self->field_0x2fc[k2] <= sub2->field_0x04 &&
                self->field_0x2f0[k2] >= sub2->field_0x04 - sub2->field_0x08 &&
                sub2->field_0x02 != 0) {
                func_804AC9F4(self,
                             &((CColiRec16*)lbl_eu_8066592C)[sub2->field_0x02]);
            }
        }
    }
}

// Sibling of func_804AC9F4 using the per-kind radius bounds at +0x2d8 with
// self's +0x5c scalar as the margin: the record's sphere must sit within
// [field_0x2d8[kind] - self->field_0x5c, field_0x2d8[kind] + radius +
// self->field_0x5c] for the id-A child, and the record's point within
// [field_0x2d8[kind] - radius - self->field_0x5c, field_0x2d8[kind] +
// self->field_0x5c] for the id-B child. Group records enumerate members
// (func_804ACD9C) instead of recursing.
extern "C" void func_804AC624(CColiObject* self, CColiRec16* obj) {
    u16 kind = obj->field_0x0c;
    if (kind == 3) {
        // Group: mark each member id as visited and process it.
        u16* arr = (u16*)((u8*)lbl_eu_80665930 + obj->field_0x00_off);
        u16 count = obj->field_0x0e;
        int i;
        for (i = 0; i < count; i++) {
            u16 id = arr[i];
            u32 bit = 1u << (id & 31);
            if ((lbl_eu_8065CFA0[id >> 5] & bit) == 0) {
                lbl_eu_8065CFA0[id >> 5] |= bit;
                func_804ACD9C(self, id);
            }
        }
        return;
    }
    // A: the record's sphere within self's box at the record's kind index.
    if (self->field_0x2d8[kind] >= obj->field_0x04 - self->field_0x5c &&
        self->field_0x2d8[kind] <= obj->field_0x04 + obj->field_0x08 +
                                       self->field_0x5c &&
        obj->field_0x00 != 0) {
        CColiRec16* sub =
            &((CColiRec16*)lbl_eu_8066592C)[obj->field_0x00];
        if (sub->field_0x0c == 3) {
            u16* arr = (u16*)((u8*)lbl_eu_80665930 + sub->field_0x00_off);
            u16 count = sub->field_0x0e;
            int i;
            for (i = 0; i < count; i++) {
                u16 id = arr[i];
                u32 bit = 1u << (id & 31);
                if ((lbl_eu_8065CFA0[id >> 5] & bit) == 0) {
                    lbl_eu_8065CFA0[id >> 5] |= bit;
                    func_804ACD9C(self, id);
                }
            }
        } else {
            u16 k1 = sub->field_0x0c;
            if (self->field_0x2d8[k1] >= sub->field_0x04 - self->field_0x5c &&
                self->field_0x2d8[k1] <= sub->field_0x04 + sub->field_0x08 +
                                           self->field_0x5c &&
                sub->field_0x00 != 0) {
                func_804AC624(self,
                             &((CColiRec16*)lbl_eu_8066592C)[sub->field_0x00]);
            }
            if (self->field_0x2d8[k1] <= sub->field_0x04 + self->field_0x5c &&
                self->field_0x2d8[k1] >= sub->field_0x04 - sub->field_0x08 -
                                           self->field_0x5c &&
                sub->field_0x02 != 0) {
                func_804AC624(self,
                             &((CColiRec16*)lbl_eu_8066592C)[sub->field_0x02]);
            }
        }
    }
    // B: the record's point within the shrunken box at its kind index.
    if (self->field_0x2d8[kind] <= obj->field_0x04 + self->field_0x5c &&
        self->field_0x2d8[kind] >= obj->field_0x04 - obj->field_0x08 -
                                      self->field_0x5c &&
        obj->field_0x02 != 0) {
        CColiRec16* sub2 =
            &((CColiRec16*)lbl_eu_8066592C)[obj->field_0x02];
        if (sub2->field_0x0c == 3) {
            u16* arr = (u16*)((u8*)lbl_eu_80665930 + sub2->field_0x00_off);
            u16 count = sub2->field_0x0e;
            int i;
            for (i = 0; i < count; i++) {
                u16 id = arr[i];
                u32 bit = 1u << (id & 31);
                if ((lbl_eu_8065CFA0[id >> 5] & bit) == 0) {
                    lbl_eu_8065CFA0[id >> 5] |= bit;
                    func_804ACD9C(self, id);
                }
            }
        } else {
            u16 k2 = sub2->field_0x0c;
            if (self->field_0x2d8[k2] >= sub2->field_0x04 - self->field_0x5c &&
                self->field_0x2d8[k2] <= sub2->field_0x04 + sub2->field_0x08 +
                                           self->field_0x5c &&
                sub2->field_0x00 != 0) {
                func_804AC624(self,
                             &((CColiRec16*)lbl_eu_8066592C)[sub2->field_0x00]);
            }
            if (self->field_0x2d8[k2] <= sub2->field_0x04 + self->field_0x5c &&
                self->field_0x2d8[k2] >= sub2->field_0x04 - sub2->field_0x08 -
                                           self->field_0x5c &&
                sub2->field_0x02 != 0) {
                func_804AC624(self,
                             &((CColiRec16*)lbl_eu_8066592C)[sub2->field_0x02]);
            }
        }
    }
}

extern "C" void func_804ACD9C(CColiObject* self, u16 id);

// Triangle record (0x14-byte entries behind lbl_eu_80665924) used by
// func_804ACD9C: the plane offset scalar at +0x00, three vertex ids
// (+0x04/+0x06/+0x08) into the point array (lbl_eu_80665910), a quad id
// (+0x0a) into the 0x24-byte record array (lbl_eu_8066591C) and a direction
// id (+0x10) into the 0xc-byte direction array (lbl_eu_80665918).
struct CColiRec14 {
    f32 field_0x00;   // +0x00 plane offset
    u16 field_0x04;   // +0x04 vertex id 1
    u16 field_0x06;   // +0x06 vertex id 2
    u16 field_0x08;   // +0x08 vertex id 3
    u16 field_0x0a;   // +0x0a quad id
    u16 field_0x0c;   // +0x0c
    u16 field_0x0e;   // +0x0e
    u16 field_0x10;   // +0x10 direction id
    u16 field_0x12;   // +0x12
};

// 0x24-byte record behind lbl_eu_8066591C: three VEC3 rows used as the
// per-vertex half-space vectors of the triangle.
struct CColiQuad {
    VEC3 field_0x00[3];
};

// Sweep/segment vs triangle test (retail func_804ACD9C): cull the segment
// against the triangle's plane (record offset, direction), then either store
// the projected point when it lies inside the triangle (all three half-space
// dot gates pass) or sweep the segment origin along the triangle edges,
// appending the transformed contact point/normal into the self lists.
extern "C" void func_804ACD9C(CColiObject* self, u16 id) {
    CColiRec14* rec = &((CColiRec14*)lbl_eu_80665924)[id];
    const VEC3* dir = &((const VEC3*)lbl_eu_80665918)[rec->field_0x10];

    // Plane cull: |dot(dir, self axis) - plane offset| >= reach.
    f32 d = VEC3Dot(dir, (const VEC3*)self->field_0x2d8) - rec->field_0x00;
    if ((f32)__fabs(d) >= self->field_0x308) return;

    // Project the axis point onto the plane (retail computes z, y, x with
    // interleaved per-component loads).
    f32 sz = self->field_0x2d8[2];
    f32 dz = dir->z;
    f32 sy = self->field_0x2d8[1];
    f32 dy = dir->y;
    f32 sx = self->field_0x2d8[0];
    f32 dx = dir->x;
    VEC3 P;
    P.z = sz - dz * d;
    P.y = sy - dy * d;
    P.x = sx - dx * d;

    const VEC3* base = (const VEC3*)lbl_eu_80665910;
    const VEC3* pts[4] = {&base[rec->field_0x04], &base[rec->field_0x06],
                          &base[rec->field_0x08], &base[rec->field_0x04]};
    VEC3 toSelf;
    VEC3 o[3];
    VEC3Sub(&o[0], &P, pts[0]);
    VEC3Sub(&o[1], &P, pts[1]);
    VEC3Sub(&o[2], &P, pts[2]);
    const VEC3* quad = &((const VEC3*)lbl_eu_8066591C)[rec->field_0x0a];

    int r28 = 0;
    if (VEC3Dot(&o[0], &quad[0]) > lbl_eu_8066AE64) {
        r28 = 1;
        if (VEC3Dot(&o[1], &quad[1]) > lbl_eu_8066AE64) {
            r28 = 2;
            if (VEC3Dot(&o[2], &quad[2]) > lbl_eu_8066AE64) {
                r28 = 3;
            }
        }
    }
    if (r28 != 3) {
        // Sweep the axis point along the triangle edges; P is dead here so
        // it doubles as the per-edge scratch (diff then closest point).
        for (int i = 0; i < 3; i++) {
            VEC3Sub(&P, pts[i + 1], pts[i]);
            f32 t = VEC3Dot(&P, &o[i]) / VEC3Dot(&P, &P);
            if (lbl_eu_8066AE44 > t) t = lbl_eu_8066AE44;
            if (lbl_eu_8066AE3C < t) t = lbl_eu_8066AE3C;
            VEC3Scale(&P, &P, t);
            VEC3Add(&P, pts[i], &P);
            toSelf.x = self->field_0x2d8[0] - P.x;
            toSelf.y = self->field_0x2d8[1] - P.y;
            toSelf.z = self->field_0x2d8[2] - P.z;
            if (VEC3Dot(&toSelf, &toSelf) < self->field_0x30c) {
                if (self->field_0x2d4 < 0x10) {
                    PSMTXMultVec((const f32(*)[4])((u8*)self->field_0x00_obj + 4),
                                 (const Vec*)&P,
                                 (Vec*)&self->field_0x214[self->field_0x2d4]);
                    self->field_0x2d4++;
                    self->field_0x80++;
                }
                return;
            }
        }
    } else {
        // P is inside the triangle: append the transformed point + normal.
        if (self->field_0x210 < 0x10) {
            PSMTXMultVec((const f32(*)[4])((u8*)self->field_0x00_obj + 4),
                         (const Vec*)&P,
                         (Vec*)&self->field_0x90[self->field_0x210]);
            VEC3TransformNormal(
                &self->field_0x150[self->field_0x210],
                (const nw4r::math::MTX34*)((u8*)self->field_0x00_obj + 4), dir);
            VEC3* n = &self->field_0x150[self->field_0x210];
            f32 len2 = n->y * n->y + n->x * n->x + n->z * n->z;
            if (len2 == lbl_eu_8066AE44) {
                *n = *(const VEC3*)(const void*)&ml::CVec3::zero;
            } else {
                PSVECNormalize((const Vec*)n, (Vec*)n);
            }
            self->field_0x210++;
            self->field_0x80++;
        }
    }
}

extern "C" void func_804AD1E0(CColiObject* self, u16 id) {}

#pragma push
#pragma auto_inline off
// Classify the partner point against self's half-extent block (retail
// func_804AD410): transform self's point (+0x44) by the matrix m, clamp it
// into the [-a, a] box and either (all three axes already inside) pick the
// closest exit face, or (edge case) clip the offset to the box and derive
// the contact along the sphere of reach f. The contact point and the
// axis-aligned/radial normal (scaled by self's +0x5c radius) are written
// into the proc object (+0x0c/+0x18) and its contact callback fires.
extern "C" int func_804AD410(CColiObject* self, const f32* a, const VEC3* b,
                             const Mtx m, f32 f) {
    VEC3 local;  // sp+0x20 transformed self point
    VEC3 out;    // sp+0x8
    f32 clamp[3];  // sp+0x14 clamped point
    PSMTXMultVec(m, (const Vec*)&self->field_0x44, (Vec*)&local);

    int count = 0;
    if (a[0] >= local.x) {
        if (-a[0] <= local.x) {
            clamp[0] = local.x;
            count = 1;
        } else {
            clamp[0] = -a[0];
        }
    } else {
        clamp[0] = a[0];
    }
    if (a[1] >= local.y) {
        if (-a[1] <= local.y) {
            clamp[1] = local.y;
            count++;
        } else {
            clamp[1] = -a[1];
        }
    } else {
        clamp[1] = a[1];
    }
    if (a[2] >= local.z) {
        if (-a[2] <= local.z) {
            clamp[2] = local.z;
            count++;
        } else {
            clamp[2] = -a[2];
        }
    } else {
        clamp[2] = a[2];
    }

    f32 pen = f;
    if (count == 3) {
        // All axes inside: pick the exit face with the smallest remaining
        // reach (a[i] - |local[i]|) and build the axis-aligned normal.
        f32 best = lbl_eu_8066AE68;
        f32 sign = lbl_eu_8066AE3C;
        int axis = 0;
        for (int i = 0; i < 3; i++) {
            f32 val;
            f32 s;
            if (((f32*)&local)[i] < lbl_eu_8066AE44) {
                val = a[i] + ((f32*)&local)[i];
                s = lbl_eu_8066AE50;
            } else {
                val = a[i] - ((f32*)&local)[i];
                s = lbl_eu_8066AE3C;
            }
            if (best > val) {
                best = val;
                axis = i;
                sign = s;
            }
        }
        out.x = lbl_eu_8066AE44;
        out.y = lbl_eu_8066AE44;
        out.z = lbl_eu_8066AE44;
        ((f32*)&out)[axis] = sign * (f + best);
    } else {
        // Edge case: offset from the clamped box; outside the reach sphere
        // the segment misses, otherwise normalise the offset and set the
        // penetration to the reach minus the distance.
        VEC3Sub(&out, &local, (const VEC3*)&clamp);
        f32 d2 = VEC3Dot(&out, &out);
        if (f * f < d2) return 0;
        if (d2 == lbl_eu_8066AE44) {
            // Degenerate offset: random horizontal direction.
            f32 rz = (f32)(rand() % 200 - 100);
            f32 rx = (f32)(rand() % 200 - 100);
            out.x = rx;
            out.y = lbl_eu_8066AE44;
            out.z = rz;
            f32 r2 = rx * rx + rz * rz;
            if (r2 == lbl_eu_8066AE44) {
                out = *(const VEC3*)(const void*)&ml::CVec3::zero;
            } else {
                PSVECNormalize((const Vec*)&out, (Vec*)&out);
            }
        } else {
            if (!(d2 >= lbl_eu_8066AE44)) {
                nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
            }
            f32 dist = d2 <= lbl_eu_8066AE44
                           ? lbl_eu_8066AE44
                           : d2 * nw4r::math::FrSqrt(d2);
            pen = f - dist;
            VEC3Scale(&out, &out, lbl_eu_8066AE3C / dist);
        }
        VEC3Scale(&out, &out, pen);
    }

    // Contact: transform the direction by the partner matrix, normalise it
    // into +0x18, then write self point - (normal * radius - +0x24) into
    // the +0x0c contact slot. The proc pointer is re-read before each call
    // so it never survives a call in a saved GPR (retail reloads it).
    CColiObj3C* proc = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
    VEC3TransformNormal((VEC3*)&proc->field_0x24,
                        (const nw4r::math::MTX34*)(const void*)b,
                        (const VEC3*)&out);
    proc = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
    PSVECNormalize((const Vec*)&proc->field_0x24,
                   (Vec*)&proc->field_0x18);
    proc = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
    VEC3Scale(&out, (const VEC3*)&proc->field_0x18, self->field_0x5c);
    proc = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
    VEC3Sub(&out, &out, (const VEC3*)&proc->field_0x24);
    proc = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
    VEC3Sub((VEC3*)&proc->field_0x0c, &self->field_0x44, &out);

    proc = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
    if (self->field_0x8c & 0x2) {
        self->field_0x8c |= 0x8000;
    } else if (proc->field_0x24.x == lbl_eu_8066AE44 &&
               proc->field_0x24.z == lbl_eu_8066AE44) {
        proc->field_0x24.x = lbl_eu_8066AE54 * self->field_0x5c;
    }
    proc = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
    proc->vtbl->field_0x08(proc, self->field_0x314);
    return 1;
}
#pragma pop

// Segment-sphere contact: cast self's segment (+0x50 point, +0x5c radius)
// against the sphere described by a (centre) and b (radius at b->x, y-band
// from b->y to b->z). The point offset from a is stored in v; when the
// squared horizontal distance is within the squared summed radius and the
// y-band gates pass, the contact normal is derived (axis-aligned when the
// +0x8c bit-1 flag is set, else radial/random) and written into the proc
// object along with the contact point and the normal scaled by the
// penetration. Returns 1 on a hit, else 0.
int func_804AD8FC(CColiObject* self, const _VEC3* a, const _VEC3* b) {
    f32 f7 = a->y + b->y;
    f32 f8 = self->field_0x50.y;
    if (f8 > f7) return 0;
    f32 f6 = f8 + self->field_0x60;
    f32 f5 = a->y + b->z;
    if (f6 < f5) return 0;
    f32 zz = self->field_0x50.z - a->z;
    f32 xx = self->field_0x50.x - a->x;
    f32 f31 = self->field_0x5c + b->x;
    VEC3 v;
    v.x = xx;
    f32 f27 = zz * zz + xx * xx;
    v.z = zz;
    if (f31 * f31 < f27) return 0;

    f32 f30 = f7 - f8;
    u32 flags = self->field_0x8c;
    f32 d = f5 - f6;
    CColiObj3C* proc = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
    ml::CVec3* n = &proc->field_0x18;
    f32 f28 = __fabs(f30);
    f32 f29 = __fabs(d);
    if ((flags & 0x2) && b->x * b->x >= f27) {
        self->field_0x8c = flags | 0x8000;
        if (f28 < f29) {
            proc->field_0x0c.x = self->field_0x44.x;
            proc->field_0x0c.y = f7;
            proc->field_0x0c.z = self->field_0x44.z;
        } else {
            f30 = d;
            proc->field_0x0c.x = self->field_0x44.x;
            proc->field_0x0c.y = f5;
            proc->field_0x0c.z = self->field_0x44.z;
        }
        n->x = lbl_eu_8066AE44;
        n->y = lbl_eu_8066AE3C;
        n->z = lbl_eu_8066AE44;
    } else {
        int nonzero = f27 != lbl_eu_8066AE44;
        if (f27 != lbl_eu_8066AE44) {
            if (!(f27 >= lbl_eu_8066AE44)) {
                nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
            }
            f32 dist = f27 <= lbl_eu_8066AE44
                           ? lbl_eu_8066AE44
                           : f27 * nw4r::math::FrSqrt(f27);
            f30 = f31 - dist;
            f32 inv = lbl_eu_8066AE3C / dist;
            n->x = v.x * inv;
            n->z = v.z * inv;
        } else {
            // Degenerate offset: random horizontal direction.
            f30 = f31;
            f32 rz = (f32)(rand() % 200 - 100);
            f32 rx = (f32)(rand() % 200 - 100);
            n->x = rx;
            n->y = lbl_eu_8066AE44;
            n->z = rz;
            if (n->x == lbl_eu_8066AE44 && n->z == lbl_eu_8066AE44) {
                n->x = lbl_eu_8066AE44;
                n->y = lbl_eu_8066AE44;
                n->z = lbl_eu_8066AE3C;
            } else {
                f32 len2 = n->y * n->y + n->x * n->x + n->z * n->z;
                if (len2 == lbl_eu_8066AE44) {
                    *n = ml::CVec3::zero;
                } else {
                    PSVECNormalize((const Vec*)n, (Vec*)n);
                }
            }
        }
        if (f28 < f29) {
            n->y = lbl_eu_8066AE3C;
        } else {
            f28 = f29;
            n->y = lbl_eu_8066AE50;
        }
        if (f28 < f30 && nonzero != 0) {
            // Scale the horizontal normal by the vertical reach, then
            // normalise the result.
            n->x *= lbl_eu_8066AE54 * self->field_0x5c;
            n->z *= lbl_eu_8066AE54 * self->field_0x5c;
            n->y *= f28;
            f32 mag = PSVECMag((const Vec*)n);
            f30 = mag;
            VEC3Scale((VEC3*)n, (VEC3*)n, lbl_eu_8066AE3C / mag);
        } else {
            n->y = lbl_eu_8066AE44;
        }
    }
    // Contact point and the penetration-scaled normal.
    proc->field_0x0c.y = self->field_0x44.y;
    proc->field_0x0c.x = n->x * b->x + a->x;
    proc->field_0x0c.z = n->z * b->x + a->z;
    nw4r::math::VEC3Scale((VEC3*)&proc->field_0x24, (const VEC3*)n, f30);
    proc->vtbl->field_0x08(proc, self->field_0x314);
    return 1;
}

// Ray-cast against the transformed AABB: transform self's two segment
// points (+0x44/+0x50) by c into the box frame, normalise the difference,
// then clip the ray against [-a, a] on each axis. The greatest positive
// interval lower bound tmax is the entry parameter; the contact point
// v1 + diff * tmax is transformed by b into the proc object's +0x0c slot
// and the axis-aligned normal (a +/-1 unit on the hit axis, transformed by
// b) goes to +0x18. Returns 1 on a hit, else 0.
int func_804ADD3C(CColiObject* self, const f32* a, const Mtx b, const Mtx c) {
    VEC3 v1;
    VEC3 v2;
    PSMTXMultVec(c, (const Vec*)&self->field_0x44, (Vec*)&v1);
    PSMTXMultVec(c, (const Vec*)&self->field_0x50, (Vec*)&v2);
    VEC3 diff;
    VEC3Sub(&diff, &v2, &v1);
    f32 mag = PSVECMag((const Vec*)&diff);
    f32 zero = lbl_eu_8066AE44;
    f32 thresh = lbl_eu_8066AE6C;
    f32 one = lbl_eu_8066AE3C;
    int any = 0;
    f32 inv = one / mag;
    VEC3Scale(&diff, &diff, inv);
    f32 vals[3];
    f32 ax = __fabs(diff.x);
    if (ax < thresh) {
        vals[0] = zero;
    } else {
        vals[0] = one / diff.x;
        any = 1;
    }
    f32 ay = __fabs(diff.y);
    if (ay < thresh) {
        vals[1] = zero;
    } else {
        vals[1] = one / diff.y;
        any = 1;
    }
    f32 az = __fabs(diff.z);
    if (az < thresh) {
        vals[2] = zero;
    } else {
        vals[2] = one / diff.z;
        any = 1;
    }
    if (any == 0) return 0;

    f32 tmin = lbl_eu_8066AE70;
    f32 tmax = lbl_eu_8066AE74;
    int hit = 0;
    int idx = 0;
    f32 sign;
    const f32* v1p = (const f32*)&v1;
    int i;
    for (i = 0; i < 3; i++) {
        f32 val = vals[i];
        if (val == lbl_eu_8066AE44) {
            if (v1p[i] < -a[i] || v1p[i] > a[i]) return 0;
        } else {
            f32 av = a[i];
            f32 vv = v1p[i];
            f32 t1 = val * (av - vv);
            f32 t0 = val * (-av - vv);
            f32 lo, hi;
            if (t0 > t1) {
                lo = t1;
                hi = t0;
                if (lo >= lbl_eu_8066AE44 && lo > tmax) {
                    tmax = lo;
                    sign = lbl_eu_8066AE3C;
                    idx = i;
                    hit = 1;
                }
                if (hi < tmin) tmin = hi;
            } else {
                lo = t0;
                hi = t1;
                if (lo >= lbl_eu_8066AE44 && lo > tmax) {
                    tmax = lo;
                    sign = lbl_eu_8066AE50;
                    idx = i;
                    hit = 1;
                }
                if (hi < tmin) tmin = hi;
            }
            if (tmax > tmin) return 0;
        }
    }
    if (tmax < lbl_eu_8066AE44) return 0;
    if (mag < tmax) return 0;
    if (hit == 0) return 0;

    // Contact point: v1 + diff * tmax, then transformed by b into the
    // proc object's +0x0c slot.
    VEC3 cp;
    cp.x = v1.x;
    cp.y = v1.y;
    cp.z = v1.z;
    VEC3Scale(&cp, &diff, tmax);
    VEC3Add(&cp, &cp, &v1);
    PSMTXMultVec(b, (const Vec*)&cp,
                 (Vec*)&((CColiObj3C*)(uintptr_t)self->field_0x3c_u)
                      ->field_0x0c);

    // Axis-aligned normal: +/-1 on the hit axis, transformed by b.
    cp.x = lbl_eu_8066AE44;
    cp.y = lbl_eu_8066AE44;
    cp.z = lbl_eu_8066AE44;
    ((f32*)&cp)[idx] = sign;
    nw4r::math::VEC3TransformNormal(
        (VEC3*)&((CColiObj3C*)(uintptr_t)self->field_0x3c_u)->field_0x18,
        (const nw4r::math::MTX34*)b, &cp);

    // Normalise the transformed normal (or zero it when degenerate).
    ml::CVec3* n = &((CColiObj3C*)(uintptr_t)self->field_0x3c_u)->field_0x18;
    f32 len2 = n->y * n->y + n->x * n->x + n->z * n->z;
    if (len2 == lbl_eu_8066AE44) {
        *n = ml::CVec3::zero;
    } else {
        PSVECNormalize((const Vec*)n, (Vec*)n);
    }
    CColiObj3C* proc = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
    proc->vtbl->field_0x08(proc, self->field_0x314);
    return 1;
}

extern "C" int func_804AE0D0(CColiObject* self, const VEC3* v, f32 f1,
                              f32 f2) { return 0; }

// Sweep/contact helpers (targets): func_804AE11C is a sphere-sweep contact test.
// func_804AF09C / func_804AF07C are the box-sweep variants (defined below).
extern "C" int func_804AE11C(CColiObject* self, const VEC3* arg4, f32 f1,
                             f32 f2) {
    // f6 is the sweep bound: the large constant when the +0x74 gate point is
    // inside the +0x74 radius, else zero.
    f32 f6 = lbl_eu_8066AE44;
    CColiObj3C* proc = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
    proc->field_0x08 = lbl_eu_80663A90;
    if (self->field_0x8c & 0x8) {
        f32 gy = self->field_0x74.y;
        f32 f1sq = f1 * f1;
        if (gy <= arg4->y + f2 && gy >= arg4->y &&
            f1sq >= (self->field_0x74.z - arg4->z) *
                        (self->field_0x74.z - arg4->z) +
                    (self->field_0x74.x - arg4->x) *
                        (self->field_0x74.x - arg4->x)) {
            f6 = lbl_eu_8066AE78;
        }
    }
    f32 py = self->field_0x50.y;
    if (py > arg4->y + f2) return 0;
    if (py + self->field_0x60 < arg4->y) return 0;
    f32 dz = self->field_0x50.z - arg4->z;
    f32 f29 = f1 - self->field_0x5c;
    f32 dx = self->field_0x50.x - arg4->x;
    if (f29 <= lbl_eu_8066AE44) return 0;
    f32 f30 = dz * dz + dx * dx;
    if (f29 * f29 >= f30) return 0;
    f32 g = f6 + self->field_0x5c + f1;
    if (g * g < f30) return 0;
    proc = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
    if (!(f30 >= lbl_eu_8066AE44)) {
        nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
    }
    f32 dist = f30 <= lbl_eu_8066AE44
                   ? lbl_eu_8066AE44
                   : f30 * nw4r::math::FrSqrt(f30);
    f32 nx = dx * (lbl_eu_8066AE3C / dist);
    f32 nz = dz * (lbl_eu_8066AE3C / dist);
    f32 scale = -(dist - f29);
    // Contact normal at +0x18 and contact point at +0x0c of the proc object.
    proc->field_0x18.x = nx;
    proc->field_0x18.z = nz;
    proc->field_0x18.y = lbl_eu_8066AE44;
    proc->field_0x0c.y = arg4->y;
    proc->field_0x0c.x = arg4->x + f1 * nx;
    proc->field_0x0c.z = arg4->z + f1 * nz;
    // Normal scaled by the penetration depth into +0x24 (paired-single ops).
    CColiObj3C* p2 = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
    nw4r::math::VEC3Scale((VEC3*)&p2->field_0x24, (VEC3*)&p2->field_0x18,
                          scale);
    CColiObj3C* p3 = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
    p3->vtbl->field_0x08(p3, self->field_0x314);
    return 1;
}
// Local-proc seed + classify used by func_804B1DEC (sibling CColiProc TU).
extern "C" int func_804B29EC(CColiProcLocal* proc, const VEC3* v,
                             CColiObject* obj, u32 bit9);

// Clip/classify helpers reached via `bl` from func_804B204C / func_804B21A8
// (retail bodies still to be decompiled; the signatures match the call
// shapes: partner object, work vector, then the two angle scalars or the
// +0x30/+0x3c/+0x6c axis block). auto_inline off keeps the call sites
// emitting real `bl`s.
#pragma push
#pragma auto_inline off
// Sphere-vs-vertical-segment sweep (retail func_804AE388): test the sphere
// at self->field_0x44 (radius field_0x5c) against the vertical segment
// [vec, vec + (0, y, 0)] of radius x. The contact normal at +0x18 is
// radial in the horizontal plane (or (0, +-1, 0) when the sphere centre is
// directly above/below the segment), the +0x0c contact point sits on the
// segment surface and the +0x24 slot gets the normal scaled by the
// penetration; the vtable slot-2 callback fires with the +0x314 argument.
extern "C" int func_804AE388(CColiObject* self, const _VEC3* vec, f32 x, f32 y) {
    CColiObj3C* proc0 = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
    proc0->field_0x08 = lbl_eu_80663A90;
    VEC3 diff;
    VEC3Sub(&diff, &self->field_0x44, (const VEC3*)vec);
    if (diff.y > y + self->field_0x5c) return 0;
    if (diff.y < -self->field_0x5c) return 0;
    f32 rsum = self->field_0x5c + x;
    f32 dist2 = diff.z * diff.z + diff.x * diff.x;
    if (rsum * rsum < dist2) return 0;

    CColiObj3C* proc = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
    ml::CVec3* n = &proc->field_0x18;
    f32 pen = rsum;
    f32 dist2b;
    if (diff.y <= y && diff.y >= lbl_eu_8066AE44) {
        // Sphere centre vertically inside the segment: radial normal.
        if (dist2 != lbl_eu_8066AE44) {
            if (!(dist2 >= lbl_eu_8066AE44)) {
                nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
            }
            f32 dist = dist2 <= lbl_eu_8066AE44
                           ? lbl_eu_8066AE44
                           : dist2 * nw4r::math::FrSqrt(dist2);
            pen = rsum - dist;
            f32 inv = lbl_eu_8066AE3C / dist;
            n->x = diff.x * inv;
            n->z = diff.z * inv;
        } else {
            // Random horizontal direction when the centres coincide.
            f32 rz = (f32)(rand() % 200 - 100);
            f32 rx = (f32)(rand() % 200 - 100);
            n->x = rx;
            n->y = lbl_eu_8066AE44;
            n->z = rz;
            if (n->x == lbl_eu_8066AE44 && n->z == lbl_eu_8066AE44) {
                n->x = lbl_eu_8066AE44;
                n->y = lbl_eu_8066AE44;
                n->z = lbl_eu_8066AE3C;
            } else {
                f32 len2 = n->y * n->y + n->x * n->x + n->z * n->z;
                if (len2 == lbl_eu_8066AE44) {
                    *n = ml::CVec3::zero;
                } else {
                    PSVECNormalize((const Vec*)n, (Vec*)n);
                }
            }
        }
        n->y = lbl_eu_8066AE44;
        proc->field_0x0c.x = vec->x + n->x * x;
        proc->field_0x0c.z = vec->z + n->z * x;
        proc->field_0x0c.y = self->field_0x44.y;
    } else if (self->field_0x5c * self->field_0x5c >= dist2) {
        // Sphere centre above/below the segment but horizontally within
        // its own radius: contact along the segment axis.
        n->x = lbl_eu_8066AE44;
        n->z = lbl_eu_8066AE44;
        if (diff.y > lbl_eu_8066AE40 * y) {
            n->y = lbl_eu_8066AE3C;
            pen = y + self->field_0x5c - diff.y;
            proc->field_0x0c.y = vec->y + y;
        } else {
            n->y = lbl_eu_8066AE50;
            pen = self->field_0x5c + diff.y;
            proc->field_0x0c.y = vec->y;
        }
        proc->field_0x0c.x = self->field_0x44.x;
        proc->field_0x0c.z = self->field_0x44.z;
    } else {
        // Corner: the sphere centre is horizontally beyond its own radius
        // but within the sum of radii. The contact point sits on the
        // segment surface (base + horizontal normal * x, y at the near
        // endcap); the sphere must contain it (r^2 >= |centre - contact|^2,
        // retail's second return-0 block) and the contact normal is the
        // (3D) offset from the contact point to the sphere centre
        // normalised by its length, with the penetration r minus it.
        if (!(dist2 >= lbl_eu_8066AE44)) {
            nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
        }
        f32 dist = dist2 <= lbl_eu_8066AE44
                       ? lbl_eu_8066AE44
                       : dist2 * nw4r::math::FrSqrt(dist2);
        f32 inv = lbl_eu_8066AE3C / dist;
        f32 nx = diff.x * inv;
        f32 nz = diff.z * inv;
        proc->field_0x0c.x = x * nx + vec->x;
        proc->field_0x0c.z = x * nz + vec->z;
        if (diff.y > lbl_eu_8066AE40 * y) {
            proc->field_0x0c.y = vec->y + y;
        } else {
            proc->field_0x0c.y = vec->y;
        }
        pen = self->field_0x5c;
        VEC3Sub(&diff, &self->field_0x44, (const VEC3*)&proc->field_0x0c);
        dist2b = VEC3LenSq(&diff);
        if (pen * pen < dist2b) return 0;
        if (dist2b == lbl_eu_8066AE44) {
            // Random direction when the sphere centre is on the axis.
            f32 rz = (f32)(rand() % 200 - 100);
            f32 rx = (f32)(rand() % 200 - 100);
            n->x = rx;
            n->y = lbl_eu_8066AE44;
            n->z = rz;
            if (n->x == lbl_eu_8066AE44 && n->z == lbl_eu_8066AE44) {
                n->x = lbl_eu_8066AE44;
                n->y = lbl_eu_8066AE44;
                n->z = lbl_eu_8066AE3C;
            } else {
                f32 len2 = n->y * n->y + n->x * n->x + n->z * n->z;
                if (len2 == lbl_eu_8066AE44) {
                    *n = ml::CVec3::zero;
                } else {
                    PSVECNormalize((const Vec*)n, (Vec*)n);
                }
            }
        } else {
            if (!(dist2b >= lbl_eu_8066AE44)) {
                nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
            }
            f32 distb = dist2b <= lbl_eu_8066AE44
                            ? lbl_eu_8066AE44
                            : dist2b * nw4r::math::FrSqrt(dist2b);
            f32 invb = lbl_eu_8066AE3C / distb;
            VEC3Scale((VEC3*)&proc->field_0x18, &diff, invb);
            pen = self->field_0x5c - distb;
        }
    }
    // Contact normal scaled by the penetration into +0x24, then the
    // vtable slot-2 callback with the +0x314 argument.
    VEC3Scale((VEC3*)&((CColiObj3C*)(uintptr_t)self->field_0x3c_u)->field_0x24,
              (const VEC3*)&((CColiObj3C*)(uintptr_t)self->field_0x3c_u)->field_0x18,
              pen);
    CColiObj3C* pc = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
    pc->vtbl->field_0x08(pc, self->field_0x314);
    return 1;
}
extern "C" int func_804AE9A4(CColiObject* self, const _VEC3* vec, f32 x, f32 y);
extern "C" int func_804AEC8C(CColiObject* self, const _VEC3* vec, f32 x, f32 y);
// Seed the proc's +0x08 with the sweep global, clear the +0x8c axis bit,
// then pass the axis-frame description through to the point/box classifier
// (func_804AD410) with self's +0x5c radius (retail tail-calls).
extern "C" int func_804AF2F0(CColiObject* self, const _VEC3* a, const _VEC3* b,
                             const _VEC3* c) {
    CColiObj3C* proc = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
    proc->field_0x08 = lbl_eu_80663A90;
    self->field_0x8c &= ~0x2u;
    return func_804AD410(self, (const f32*)a, (const VEC3*)b,
                         (const f32(*)[4])c, self->field_0x5c);
}
// Seed the proc's +0x08 with the sweep global, clear the +0x8c axis bits,
// then pass the segment description through to the segment-vs-segment clip
// (func_804ADD3C). The args travel unchanged, so retail tail-calls.
extern "C" int func_804AF310(CColiObject* self, const _VEC3* a, const _VEC3* b,
                             const _VEC3* c) {
    CColiObj3C* proc = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
    proc->field_0x08 = lbl_eu_80663A90;
    self->field_0x8c &= ~0x2u;
    return func_804ADD3C(self, (const f32*)a, (const f32(*)[4])b,
                         (const f32(*)[4])c);
}

// Segment-vs-AABB clip with contact write-back (retail func_804AF32C):
// transform both segment points (+0x50, +0x44) by the partner matrix c,
// check each axis of a against the local AABB, and when exactly one point
// is inside, find the entry t along the segment (per-axis inverse bounds
// with the zero-axis fallback), write the entry point into the proc's
// +0x0c contact slot and the axis-aligned normal (transformed by c) into
// +0x18, then fire the proc's contact callback. Returns 1 on a hit.
extern "C" int func_804AF32C(CColiObject* self, const _VEC3* a,
                             const _VEC3* b, const _VEC3* c) {
    VEC3 in;   // sp+0x44 transformed +0x50 point
    VEC3 out;  // sp+0x38 transformed +0x44 point
    PSMTXMultVec((const f32(*)[4])(const void*)c,
                 (const Vec*)&self->field_0x50, (Vec*)&in);
    int insideA = 1;
    if (a->x < in.x || -a->x > in.x) {
        insideA = 0;
    } else {
        if (a->y < in.y || -a->y > in.y) {
            insideA = 0;
        } else {
            if (a->z < in.z || -a->z > in.z) {
                insideA = 0;
            }
        }
    }
    PSMTXMultVec((const f32(*)[4])(const void*)c,
                 (const Vec*)&self->field_0x44, (Vec*)&out);
    int insideB = 1;
    if (a->x < out.x || -a->x > out.x) {
        insideB = 0;
    } else {
        if (a->y < out.y || -a->y > out.y) {
            insideB = 0;
        } else {
            if (a->z < out.z || -a->z > out.z) {
                insideB = 0;
            }
        }
    }
    if (insideA == insideB) return 0;

    if (insideA != 0) {
        // Swap so in holds the inside point and out the outside point.
        VEC3 t = in;
        in = out;
        out = t;
    }
    CColiObj3C* proc = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
    proc->field_0x08 = lbl_eu_80663A90;
    VEC3 dir;  // sp+0x20 outside - inside
    VEC3Sub(&dir, &out, &in);
    f32 mag = PSVECMag((const Vec*)&dir);
    f32 inv = lbl_eu_8066AE3C / mag;
    VEC3Scale(&dir, &dir, inv);

    f32 oneOver[3];  // sp+0x14 per-axis inverse
    int nonzero = 0;
    if ((f32)__fabs(dir.x) < lbl_eu_8066AE6C) {
        oneOver[0] = lbl_eu_8066AE44;
    } else {
        oneOver[0] = lbl_eu_8066AE3C / dir.x;
        nonzero = 1;
    }
    if ((f32)__fabs(dir.y) < lbl_eu_8066AE6C) {
        oneOver[1] = lbl_eu_8066AE44;
    } else {
        oneOver[1] = lbl_eu_8066AE3C / dir.y;
        nonzero = 1;
    }
    if ((f32)__fabs(dir.z) < lbl_eu_8066AE6C) {
        oneOver[2] = lbl_eu_8066AE44;
    } else {
        oneOver[2] = lbl_eu_8066AE3C / dir.z;
        nonzero = 1;
    }
    if (nonzero == 0) return 0;

    // Per-axis entry bounds along the segment; track the tightest lower
    // bound and the axis/sign that produced it.
    f32 tLo = lbl_eu_8066AE74;
    f32 tHi = lbl_eu_8066AE70;
    f32 axisSign = lbl_eu_8066AE44;
    int axis = 0;
    int found = 0;
    for (int i = 0; i < 3; i++) {
        f32 iv = oneOver[i];
        if (iv == lbl_eu_8066AE44) {
            // Zero direction on this axis: the inside point must already
            // lie within the AABB on it.
            if (((f32*)&in)[i] < -((const f32*)a)[i] ||
                ((f32*)&in)[i] > ((const f32*)a)[i]) {
                return 0;
            }
        } else {
            f32 t0 = iv * (((const f32*)a)[i] - ((f32*)&in)[i]);
            f32 t1 = iv * (-((const f32*)a)[i] - ((f32*)&in)[i]);
            if (t1 > t0) {
                if (t0 >= lbl_eu_8066AE44 && t0 > tLo) {
                    tLo = t0;
                    axisSign = lbl_eu_8066AE3C;
                    axis = i;
                    found = 1;
                }
                if (t1 < tHi) tHi = t1;
            } else {
                if (t1 >= lbl_eu_8066AE44 && t1 > tLo) {
                    tLo = t1;
                    axisSign = lbl_eu_8066AE50;
                    axis = i;
                    found = 1;
                }
                if (t0 < tHi) tHi = t0;
            }
            if (tLo > tHi) return 0;
        }
    }
    if (tLo < lbl_eu_8066AE44 || mag < tLo) return 0;
    if (found == 0) return 0;

    // Entry point: inside + dir * tLo, transformed into the proc contact.
    VEC3 hit;  // sp+0x8
    VEC3Scale(&hit, &dir, tLo);
    VEC3Add(&hit, &in, &hit);
    PSMTXMultVec((const f32(*)[4])(const void*)b, (const Vec*)&hit,
                 (Vec*)&proc->field_0x0c);
    // Axis-aligned normal in the partner frame.
    hit.x = lbl_eu_8066AE44;
    hit.y = lbl_eu_8066AE44;
    hit.z = lbl_eu_8066AE44;
    ((f32*)&hit)[axis] = -axisSign;
    VEC3TransformNormal((VEC3*)&proc->field_0x18,
                        (const nw4r::math::MTX34*)(const void*)b,
                        (const VEC3*)&hit);
    VEC3* n = (VEC3*)&proc->field_0x18;
    f32 len2 = n->y * n->y + n->x * n->x + n->z * n->z;
    if (len2 == lbl_eu_8066AE44) {
        *n = *(const VEC3*)(const void*)&ml::CVec3::zero;
    } else {
        PSVECNormalize((const Vec*)n, (Vec*)n);
    }
    proc->vtbl->field_0x08(proc, self->field_0x314);
    return 1;
}
#pragma pop

// Stub body (not yet decompiled): auto_inline off keeps callers emitting a
// real `bl` instead of inlining the empty body.
#pragma push
#pragma auto_inline off
// Clip helper for the contact object (retail body): accept when the squared
// distance from self's point (+0x44) or (+0x50) to v is within f^2, else
// project the horizontal offset onto the segment direction ((+0x68.x,
// +0x68.z) over +0x308), extend the point along it and test the enlarged
// squared distance against (f + +0x5c)^2. auto_inline off: retail callers
// (func_804AC4F4 / func_804AC5D8) call via `bl`.
extern "C" int func_804AF808(CColiContactObj* self, const VEC3* v, f32 f) {
    CColiObject* o = (CColiObject*)self;
    VEC3 w;
    VEC3 diff;
    f32 r2 = f * f;
    VEC3Sub(&diff, &o->field_0x44, v);
    if (r2 >= VEC3LenSq(&diff)) return 1;
    VEC3Sub(&diff, &o->field_0x50, v);
    if (r2 >= VEC3LenSq(&diff)) return 1;
    f32 t = clamp01(o->field_0x68.z * (v->y - o->field_0x44.z) /
                        o->field_0x308 +
                    o->field_0x68.x * (v->x - o->field_0x44.x));
    w.x = o->field_0x68.x * t;
    w.y = lbl_eu_8066AE44;
    w.z = o->field_0x68.z * t;
    VEC3Add(&w, &w, &o->field_0x44);
    VEC3Sub(&w, &w, v);
    f32 r = f + o->field_0x5c;
    if (r * r >= VEC3LenSq(&w)) return 1;
    return 0;
}
#pragma pop

// Cylinder test: the offset of self's point (+0x44) from other is within a
// vertical cylinder of radius a and height b around other's point (y in
// [0, b], horizontal squared distance <= a^2). The VEC3Sub + component
// reads reproduce the retail paired-single sequence.
bool func_804AF98C(CColiObject* self, const VEC3* other, f32 a, f32 b) {
    VEC3 diff;
    VEC3Sub(&diff, &self->field_0x44, other);
    if (diff.y <= b && diff.y >= lbl_eu_8066AE44 &&
        diff.x * diff.x + diff.z * diff.z <= a * a) {
        return true;
    }
    return false;
}

// Point-in-box + half-extent gate used by func_804B236C: when the object's
// point (+0x44) lies inside the box spanned by (boxA, boxB) - checked in
// x, z, y order to mirror the retail compare sequence - transform it by m
// and verify each axis of the result is within the half-extent vector.
int func_804AFA08(CColiObject* self, const VEC3* half, const Mtx m,
                  const VEC3* boxA, const VEC3* boxB) {
    Vec out;
    if (boxA->x >= self->field_0x44.x && boxB->x <= self->field_0x44.x &&
        boxA->z >= self->field_0x44.z && boxB->z <= self->field_0x44.z &&
        boxA->y >= self->field_0x44.y && boxB->y <= self->field_0x44.y) {
        PSMTXMultVec(m, (const Vec*)&self->field_0x44, &out);
        if (half->x < out.x || -half->x > out.x) return 0;
        if (half->y < out.y || -half->y > out.y) return 0;
        if (half->z < out.z || -half->z > out.z) return 0;
        return 1;
    }
    return 0;
}

// Clip the segment (pA, pB) against the frame spanned by (pC, pD): writes the
// entry/exit points of the portion of the segment that lies within the strip
// between the two parallel lines through pC and pD into pOutA/pOutB.
extern "C" void func_804AFB28(VEC3* pOutA, VEC3* pOutB, const VEC3* pA,
                              const VEC3* pB, const VEC3* pC, const VEC3* pD) {
    VEC3 v3, v1, v4;
    VEC3 scratch;
    VEC3 v2;
    VEC3Sub(&v1, pD, pC);
    VEC3Scale(&v2, &v1, lbl_eu_8066AE50);
    VEC3Sub(&v3, pB, pA);
    VEC3Sub(&v4, pC, pA);

    f32 a = VEC3Dot(&v1, &v1);
    f32 b = VEC3Dot(&v3, &v3);
    f32 c = VEC3Dot(&v3, &v2);
    f32 d = VEC3Dot(&v3, &v4);
    f32 e = VEC3Dot(&v2, &v4);

    f32 t = (b * e - c * d) / (a * b - c * c);
    f32 u = (d - c * t) / b;

    if ((u < lbl_eu_8066AE44 || u > lbl_eu_8066AE3C) &&
        (t < lbl_eu_8066AE44 || t > lbl_eu_8066AE3C)) {
        // both out of range
        f32 uc = clamp01(u);
        VEC3Scale(&scratch, &v3, uc);
        VEC3Add(pOutA, pA, &scratch);
        VEC3Sub(&v4, pOutA, pC);
        f32 s = VEC3Dot(&v1, &v4) / a;
        if (s < lbl_eu_8066AE44 || s > lbl_eu_8066AE3C) {
            f32 sc = clamp01(s);
            VEC3Scale(&scratch, &v1, sc);
            VEC3Add(pOutB, pC, &scratch);
            VEC3Sub(&v4, pOutB, pA);
            f32 s2 = VEC3Dot(&v3, &v4) / b;
            f32 sc2 = clamp01(s2);
            VEC3Scale(&scratch, &v3, sc2);
            VEC3Add(pOutA, pA, &scratch);
        } else {
            VEC3Scale(&scratch, &v1, s);
            VEC3Add(pOutB, pC, &scratch);
        }
    } else {
        if (u < lbl_eu_8066AE44 || u > lbl_eu_8066AE3C) {
            // u out, t in
            f32 uc = clamp01(u);
            VEC3Scale(&scratch, &v3, uc);
            VEC3Add(pOutA, pA, &scratch);
            VEC3Sub(&v4, pOutA, pC);
            f32 s = VEC3Dot(&v1, &v4) / a;
            f32 sc = clamp01(s);
            VEC3Scale(&scratch, &v1, sc);
            VEC3Add(pOutB, pC, &scratch);
        } else {
            if (t < lbl_eu_8066AE44 || t > lbl_eu_8066AE3C) {
                // t out, u in
                f32 tc = clamp01(t);
                VEC3Scale(&scratch, &v1, tc);
                VEC3Add(pOutB, pC, &scratch);
                VEC3Sub(&v4, pOutB, pA);
                f32 s2 = VEC3Dot(&v3, &v4) / b;
                f32 sc2 = clamp01(s2);
                VEC3Scale(&scratch, &v3, sc2);
                VEC3Add(pOutA, pA, &scratch);
            } else {
                // both in range
                VEC3Scale(&scratch, &v3, u);
                VEC3Add(pOutA, pA, &scratch);
                VEC3Scale(&scratch, &v1, t);
                VEC3Add(pOutB, pC, &scratch);
            }
        }
    }
}

// Clip the segment (a, b) against the strip spanned by the segment (c, d)
// with radius f (retail func_804B028C). The frame direction v0 = d - c, the
// offsets v1 = a - c and the segment direction v2 = b - a define three dots
// used for the endpoint gates; when the lines are near-parallel the
// intersection parameter comes from a projection, otherwise a quadratic
// determines the closest approach. On a hit the contact point a + v2*t is
// written into e and 1 is returned.
extern "C" int func_804B028C(VEC3* a, VEC3* b, const VEC3* c, const VEC3* d,
                             VEC3* e, f32 f) {
    VEC3 w[3];  // w[2] = d - c, w[1] = a - c, w[0] = b - a
    VEC3Sub(&w[2], d, c);
    VEC3Sub(&w[1], a, c);
    VEC3Sub(&w[0], b, a);
    // Declared in the retail's saved-FPR order (first-declared -> f31):
    // v1.v0, v2.v0, |v0|^2, |v2|^2, v1.v2, then the quadratic terms.
    f32 v1v0;
    f32 v2v0;
    f32 v0sq;
    f32 v2sq;
    f32 v1v2;
    f32 f26v;
    f32 f25v;
    f32 f24v;
    f32 f23v;
    f32 f4v;  // |v1|^2 (scratch f4)
    f32 f3v;
    v0sq = VEC3Dot(&w[2], &w[2]);  // |v0|^2
    v1v0 = VEC3Dot(&w[1], &w[2]);  // v1 . v0
    v2v0 = VEC3Dot(&w[0], &w[2]);  // v2 . v0
    // Endpoint gates: the whole segment before the frame start or past the
    // frame end misses.
    if (v1v0 < lbl_eu_8066AE44 && v1v0 + v2v0 < lbl_eu_8066AE44) return 0;
    if (v1v0 > v0sq && v1v0 + v2v0 > v0sq) return 0;

    v2sq = VEC3Dot(&w[0], &w[0]);  // |v2|^2
    f4v = VEC3Dot(&w[1], &w[1]);   // |v1|^2
    f32 sq30 = v2v0 * v2v0;
    f32 sq31 = v1v0 * v1v0;
    f26v = v0sq * v2sq - sq30;
    f25v = f4v - f * f;
    f3v = v0sq * f25v - sq31;
    v1v2 = VEC3Dot(&w[1], &w[0]);  // v1 . v2
    if ((f32)__fabs(f26v) < lbl_eu_8066AE5C) {
        // Near-parallel: project the frame-start offset onto the segment.
        if (f3v > lbl_eu_8066AE44) return 0;
        f32 t;
        if (v1v0 < lbl_eu_8066AE44) {
            t = -v1v2 / v2sq;
        } else if (v1v0 > v0sq) {
            t = (v2v0 - v1v2) / v2sq;
        } else {
            t = lbl_eu_8066AE44;
        }
        VEC3Scale(e, &w[0], t);
        VEC3Add(e, a, e);
        return 1;
    }
    {
        // General (non-parallel) intersection: quadratic closest approach.
        f24v = v0sq * v1v2 - v2v0 * v1v0;
        f23v = f24v * f24v - f26v * f3v;
        if (f23v < lbl_eu_8066AE44) return 0;
        if (!(f23v >= lbl_eu_8066AE44)) {
            nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
        }
        f32 sq = f23v <= lbl_eu_8066AE44
                     ? lbl_eu_8066AE44
                     : f23v * nw4r::math::FrSqrt(f23v);
        f32 t = (-f24v - sq) / f26v;
        f32 proj = t * v2v0 + v1v0;
        if (proj < lbl_eu_8066AE44) {
            // Closest approach before the frame start: clip at the start
            // line and test the quadratic there.
            if (v2v0 <= lbl_eu_8066AE44) return 0;
            f32 t2 = -v1v0 / v2v0;
            if (t2 * (lbl_eu_8066AE38 * v1v2 + t2 * v2sq) + f25v >
                lbl_eu_8066AE44) {
                return 0;
            }
            VEC3Scale(e, &w[0], t2);
            VEC3Add(e, a, e);
            return 1;
        }
        if (proj > v0sq) {
            // Closest approach past the frame end: clip at the end line.
            if (v2v0 < lbl_eu_8066AE44) return 0;
            f32 t3 = (v0sq - v1v0) / v2v0;
            if (t3 * (lbl_eu_8066AE38 * (v1v2 - v2v0) + t3 * v2sq) +
                    (f25v + v0sq - lbl_eu_8066AE38 * v1v0) >
                lbl_eu_8066AE44) {
                return 0;
            }
            VEC3Scale(e, &w[0], t3);
            VEC3Add(e, a, e);
            return 1;
        }
        // Closest approach within the frame: parameter must be interior.
        if (t > lbl_eu_8066AE44 && t < lbl_eu_8066AE3C) {
            VEC3Scale(e, &w[0], t);
            VEC3Add(e, a, e);
            return 1;
        }
        return 0;
    }
}

// Two-point clip against the vertical cylinder around the partner point:
// test self's +0x44 and +0x50 points against the (x-radius, y-height)
// cylinder; when exactly one is inside, clip the segment between them
// against the band [vec.y, vec.y + y] via func_804B028C and derive the
// contact normal (down/up for exact y matches, radial otherwise, or
// randomized when degenerate). Returns 1 on a hit, else 0.
extern "C" int func_804AEC8C(CColiObject* self, const _VEC3* vec, f32 x,
                             f32 y) {
    VEC3 diff;
    VEC3Sub(&diff, &self->field_0x44, (const VEC3*)vec);
    int flagA = 0;
    if (diff.y <= y && diff.y >= lbl_eu_8066AE44 &&
        diff.z * diff.z + diff.x * diff.x <= x * x) {
        flagA = 1;
    }
    VEC3Sub(&diff, &self->field_0x50, (const VEC3*)vec);
    int flagB = 0;
    if (diff.y <= y && diff.y >= lbl_eu_8066AE44 &&
        diff.z * diff.z + diff.x * diff.x <= x * x) {
        flagB = 1;
    }
    if (flagA == flagB) return 0;

    CColiObj3C* proc = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
    proc->field_0x08 = lbl_eu_80663A90;
    VEC3 cpA;
    VEC3 cpB;
    cpA.x = vec->x;
    cpA.y = vec->y;
    cpA.z = vec->z;
    cpB.x = vec->x;
    cpB.y = vec->y;
    cpB.z = vec->z;
    VEC3* pa;
    VEC3* pb;
    if (flagA != 0) {
        pa = &self->field_0x50;
        pb = &self->field_0x44;
    } else {
        pa = &self->field_0x44;
        pb = &self->field_0x50;
    }
    cpB.y += y;
    if (func_804B028C(pa, pb, &cpA, &cpB, (VEC3*)&proc->field_0x0c, x) == 0) {
        return 0;
    }
    proc = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
    if (cpA.y == proc->field_0x0c.y) {
        proc->field_0x18.x = lbl_eu_8066AE44;
        proc->field_0x18.y = lbl_eu_8066AE50;
        proc->field_0x18.z = lbl_eu_8066AE44;
    } else if (cpB.y == proc->field_0x0c.y) {
        proc->field_0x18.x = lbl_eu_8066AE44;
        proc->field_0x18.y = lbl_eu_8066AE3C;
        proc->field_0x18.z = lbl_eu_8066AE44;
    } else {
        f32 dz = cpB.z - proc->field_0x0c.z;
        f32 dx = cpB.x - proc->field_0x0c.x;
        f32 dist2 = dz * dz + dx * dx;
        if (dist2 != lbl_eu_8066AE44) {
            if (!(dist2 >= lbl_eu_8066AE44)) {
                nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
            }
            f32 dist = dist2 <= lbl_eu_8066AE44
                           ? lbl_eu_8066AE44
                           : dist2 * nw4r::math::FrSqrt(dist2);
            f32 inv = lbl_eu_8066AE3C / dist;
            CColiObj3C* p3 = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
            p3->field_0x18.x = dx * inv;
            p3->field_0x18.y = lbl_eu_8066AE44;
            p3->field_0x18.z = dz * inv;
        } else {
            // Random horizontal direction when the clip is degenerate.
            ml::CVec3* n = &proc->field_0x18;
            f32 rz = (f32)(rand() % 200 - 100);
            f32 rx = (f32)(rand() % 200 - 100);
            n->x = rx;
            n->y = lbl_eu_8066AE44;
            n->z = rz;
            if (n->x == lbl_eu_8066AE44 && n->z == lbl_eu_8066AE44) {
                n->x = lbl_eu_8066AE44;
                n->y = lbl_eu_8066AE44;
                n->z = lbl_eu_8066AE3C;
            } else {
                f32 len2 = n->y * n->y + n->x * n->x + n->z * n->z;
                if (len2 == lbl_eu_8066AE44) {
                    *n = ml::CVec3::zero;
                } else {
                    PSVECNormalize((const Vec*)n, (Vec*)n);
                }
            }
        }
    }
    proc = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
    proc->vtbl->field_0x08(proc, self->field_0x314);
    return 1;
}

// Write the segment (in - d, in + d) as the embedded AABB corner pair.
// auto_inline off: retail calls this via `bl` (separate TU in the original
// source), so callers must not inline the body.
#pragma push
#pragma auto_inline off
extern "C" void func_804B06FC(CColiObject* self, const VEC3* in, f32 d) {
    self->field_0x00[0].x = in->x + d;
    self->field_0x00[0].y = in->y + d;
    self->field_0x00[0].z = in->z + d;
    self->field_0x00[1].x = in->x - d;
    self->field_0x00[1].y = in->y - d;
    self->field_0x00[1].z = in->z - d;
}
#pragma pop

void func_804B073C(CColiObject* self, const VEC3* in, f32 a, f32 b, f32 c) {
    // Asymmetric AABB corner pair around in (half-extents a/b/c mixed axes).
    self->field_0x00[0].x = in->x + a;
    self->field_0x00[0].y = in->y + b;
    self->field_0x00[0].z = in->z + a;
    self->field_0x00[1].x = in->x - a;
    self->field_0x00[1].y = in->y + c;
    self->field_0x00[1].z = in->z - a;
}

// AABB corner pair from two objects: field_0x00[0] gets the component-wise
// maximum of the two points, field_0x00[1] the minimum.
void func_804B077C(CColiObject* out, const CColiObject* a, const CColiObject* b) {
    if (a->field_0x00[0].x > b->field_0x00[0].x) {
        out->field_0x00[0].x = a->field_0x00[0].x;
        out->field_0x00[1].x = b->field_0x00[0].x;
    } else {
        out->field_0x00[0].x = b->field_0x00[0].x;
        out->field_0x00[1].x = a->field_0x00[0].x;
    }
    if (a->field_0x00[0].y > b->field_0x00[0].y) {
        out->field_0x00[0].y = a->field_0x00[0].y;
        out->field_0x00[1].y = b->field_0x00[0].y;
    } else {
        out->field_0x00[0].y = b->field_0x00[0].y;
        out->field_0x00[1].y = a->field_0x00[0].y;
    }
    // z block: walk a f32* with *q++ so MWCC keeps a base register across
    // the branch (retail materialises addi r4, r3, 8 for the z pair).
    f32* q = &out->field_0x00[0].z;
    if (a->field_0x00[0].z > b->field_0x00[0].z) {
        *q++ = a->field_0x00[0].z;
        q++;
        q++;
        *q = b->field_0x00[0].z;
    } else {
        *q++ = b->field_0x00[0].z;
        q++;
        q++;
        *q = a->field_0x00[0].z;
    }
}

// Duplicate the source 12 bytes into both AABB corners (+0x00 and +0x0C):
// retail copies with lwz/stw (integer words), stores the +0x0C row first.
extern "C" void func_804B07F0(CColiObject* dst, const void* src) {
    const u32* s = (const u32*)src;
    u32* d = (u32*)dst;
    d[3] = s[0];
    d[4] = s[1];
    d[5] = s[2];
    d[0] = s[0];
    d[1] = s[1];
    d[2] = s[2];
}

// AABB-containment test: true when b's box (min corner at field_0x00[1],
// max corner at field_0x00[2]) lies inside a's box (min at field_0x00[1],
// max at field_0x00[0]). Component pairs are compared in x, z, y order to
// mirror the retail branch sequence. auto_inline off: retail calls this via
// `bl` (separate TU in the original source).
#pragma push
#pragma auto_inline off
extern "C" bool func_804B0818(const CColiObject* a, const CColiObject* b) {
    if (b->field_0x00[1].x >= a->field_0x00[1].x &&
        b->field_0x00[2].x <= a->field_0x00[0].x &&
        b->field_0x00[1].z >= a->field_0x00[1].z &&
        b->field_0x00[2].z <= a->field_0x00[0].z &&
        b->field_0x00[1].y >= a->field_0x00[1].y &&
        b->field_0x00[2].y <= a->field_0x00[0].y) {
        return true;
    }
    return false;
}
#pragma pop

// Move the embedded AABB corner pair (two 3-float rows) along v: components
// above the sdata2 threshold extend the max corner (row 0), the rest drag the
// min corner (row 1). The z block rebases the base pointer (self += 2 floats)
// so the two stores use 0/0xc displacements (retail's addi r3, r3, 8 shape).
void func_804B08A0(f32* self, const f32* v) {
    f32 threshold = lbl_eu_8066AE80;
    if (v[0] > threshold) {
        self[0] += v[0];
    } else {
        self[3] += v[0];
    }
    if (v[1] > threshold) {
        self[1] += v[1];
    } else {
        self[4] += v[1];
    }
    self += 2;
    if (v[2] > threshold) {
        self[0] += v[2];
    } else {
        self[3] += v[2];
    }
}

// Object whose destructor releases an owned buffer (retail __dt__804B095C).
// Only +0x04 (the MemManager-owned allocation) is touched by the teardown;
// the class identity is unknown (placeholder address name).
struct CColi804B095C {
    u8 _00[0x04];
    u8* field_0x4;   // +0x04 owned buffer (mtl::MemManager::deallocate)
};

void* __dt__804B095C(CColi804B095C* self, int deleting) {
    if (self != 0) {
        // Nested null-check on the buffer: MWCC CSEs the load/cmpwi but
        // emits both beqs (retail has the duplicated branch).
        if (self->field_0x4 != 0) {
            if (self->field_0x4 != 0) {
                mtl::MemManager::deallocate(self->field_0x4);
                self->field_0x4 = 0;
            }
        }
        if (deleting > 0) {
            delete self;
        }
    }
    return self;
}

// Collision list node built by the manager: two 3x4 transforms at
// +0x3c/+0x6c, a keyed doubly-linked list slot at +0x9c..+0xa8 and a
// 12-float axis/AABB block filled with the sdata2 constant. The constant
// is re-read after the identity calls because bl clobbers FPRs.
struct CColiNode804B09C8 {
    u32 field_0x00;                  // +0x00
    u8* field_0x04;                  // +0x04 owned collision-data buffer
    u32 field_0x08;                  // +0x08
    f32 field_0x0c[6];               // +0x0c..+0x23
    f32 field_0x24[6];               // +0x24..+0x3b
    Mtx field_0x3c;                  // +0x3c..+0x6b
    Mtx field_0x6c;                  // +0x6c..+0x9b
    CColiNode804B09C8* field_0x9c;   // +0x9c previous link
    CColiNode804B09C8* field_0xa0;   // +0xa0 next link
    f32 field_0xa4;                  // +0xa4
    u32 field_0xa8;                  // +0xa8 behaviour flags
    u32 field_0xac;                  // +0xac
    u16 field_0xb0;                  // +0xb0
    u16 field_0xb2;                  // +0xb2
};

// Clear the owned collision-data pointer, run the node's init, return self.
void func_804B09C8(CColiNode804B09C8* self);
extern "C" CColiNode804B09C8* func_804B0924(CColiNode804B09C8* self) {
    self->field_0x04 = 0;
    func_804B09C8(self);
    return self;
}

void func_804B09C8(CColiNode804B09C8* self) {
    f32 k = lbl_eu_8066AE88;
    self->field_0x00 = 0;
    self->field_0x08 = 0;
    self->field_0xa0 = 0;
    self->field_0x9c = 0;
    self->field_0xa4 = k;
    self->field_0xb2 = 1000;
    self->field_0xa8 = 0;
    self->field_0xb0 = 0;
    self->field_0x24[0] = k;
    self->field_0x24[1] = k;
    self->field_0x24[2] = k;
    self->field_0x24[3] = k;
    self->field_0x24[4] = k;
    self->field_0x24[5] = k;
    self->field_0x0c[0] = k;
    self->field_0x0c[1] = k;
    self->field_0x0c[2] = k;
    self->field_0x0c[3] = k;
    self->field_0x0c[4] = k;
    self->field_0x0c[5] = k;
    PSMTXIdentity(self->field_0x3c);
    PSMTXIdentity(self->field_0x6c);
    self->field_0xa4 = lbl_eu_8066AE88;
    self->field_0xac = 1;
}

extern "C" void func_804B102C(void* self);
extern "C" void func_804B0A6C(void* self, u32 val) { *(u32*)((u8*)self + 0) = val; func_804B102C(self); }

extern "C" void func_804B0A74(void* self, u32 val) { *(u32*)((u8*)self + 8) = val; func_804B102C(self); }

void func_804B0A7C(){}

void func_804B0AD4(CColiObject* self, int arg, f32 x, f32 y) {
    u32 flags = self->field_0xa8;
    self->field_0x30.x = x;
    flags = (flags & ~0x20) | 0x10;
    self->field_0x30.y = y;
    self->field_0xa8 = flags;
    if (arg != 0) {
        self->field_0xa8 |= 0x400;
    } else {
        self->field_0xa8 &= ~0x400;
    }
}

void func_804B0B0C(CColiObject* self, const _VEC3* v, int flag) {
    // Raw-copy the 3-word vector (VEC3 copy lowers to lwz/stw), then update
    // the behaviour flags: clear bit 4, set bit 5, and set/clear bit 10
    // according to the caller-supplied flag.
    u32 flags = self->field_0xa8;
    self->field_0x30 = *v;
    flags = (flags & ~0x10) | 0x20;
    self->field_0xa8 = flags;
    if (flag != 0) {
        self->field_0xa8 |= 0x400;
    } else {
        self->field_0xa8 &= ~0x400;
    }
}

// Position/state update: raw-copy the 3-word vector into the +0x24 slot and
// set flag bit 2. When bit 5 is set, copy the vector into the +0x3c matrix's
// translation column and invert it into +0x6c; when the object is a fresh
// one (kind 0) with bits 4..7 set, fill the +0x0c..+0x20 axis block with the
// two sdata2 constants. Hand the object to func_804B0EA0 last. The vector is
// copied as words (lwz/stw) so the +0x24 slot keeps the raw bit pattern.
void func_804B0B54(CColiObject* self, const _VEC3* v) {
    const u32* src = (const u32*)v;
    u32* dst = (u32*)&self->field_0x00[3];
    u32 flags = self->field_0xa8;
    dst[0] = src[0];
    u32 nf = flags | 0x4;
    self->field_0xa8 = nf;
    dst[1] = src[1];
    dst[2] = src[2];
    if (nf & 0x20) {
        CColiNode804B09C8* node = (CColiNode804B09C8*)self;
        node->field_0x3c[0][3] = self->field_0x00[3].x;
        node->field_0x3c[1][3] = self->field_0x00[3].y;
        node->field_0x3c[2][3] = self->field_0x00[3].z;
        PSMTXInverse(node->field_0x3c, node->field_0x6c);
    }
    if (self->field_0x04 == 0 && (self->field_0xa8 & 0xf0) != 0) {
        // Declared c2 first (c2->f0) but assigned c1 first (its load is
        // emitted first), matching retail's lfs f1,const1; lfs f0,const2.
        f32 c2;
        f32 c1;
        c1 = lbl_eu_8066AE8C;
        c2 = lbl_eu_8066AE90;
        self->field_0x00[1].x = c1;
        self->field_0x00[1].y = c1;
        self->field_0x00[1].z = c1;
        self->field_0x00[2].x = c2;
        self->field_0x00[2].y = c2;
        self->field_0x00[2].z = c2;
    }
    func_804B0EA0(self);
}

// Position/rotation update: raw-copy the 3-word vector into the +0x24 slot and
// set flag bit 2. When bit 5 is set, build the +0x3c matrix from the rotation
// vector scaled by the RAD_TO_FIDX constant, copy the +0x24 vector into its
// translation column and invert it into +0x6c; when the object is a fresh one
// (kind 0) with bits 4..7 set, fill the +0x0c..+0x20 axis block with the two
// sdata2 constants. Hand the object to func_804B0EA0 last.
void func_804B0C0C(CColiObject* self, const _VEC3* v, const _VEC3* rot) {
    const u32* src = (const u32*)v;
    u32* dst = (u32*)&self->field_0x00[3];
    u32 flags = self->field_0xa8;
    dst[0] = src[0];
    u32 nf = flags | 0x4;
    self->field_0xa8 = nf;
    dst[1] = src[1];
    dst[2] = src[2];
    if (nf & 0x20) {
        CColiNode804B09C8* node = (CColiNode804B09C8*)self;
        // k born first (f0) but loaded after z: declare it before z yet
        // assign it after z's load so the sdata2 load lands second.
        f32 k;
        f32 z = rot->z;
        k = lbl_eu_8066AE94;
        f32 y = rot->y;
        f32 x = rot->x;
        MTX34RotXYZFIdx((nw4r::math::MTX34*)node->field_0x3c, k * x, k * y,
                        k * z);
        node->field_0x3c[0][3] = self->field_0x00[3].x;
        node->field_0x3c[1][3] = self->field_0x00[3].y;
        node->field_0x3c[2][3] = self->field_0x00[3].z;
        PSMTXInverse(node->field_0x3c, node->field_0x6c);
    }
    if (self->field_0x04 == 0 && (self->field_0xa8 & 0xf0) != 0) {
        // Declared c2 first (c2->f0) but assigned c1 first (its load is
        // emitted first), matching retail's lfs f1,const1; lfs f0,const2.
        f32 c2;
        f32 c1;
        c1 = lbl_eu_8066AE8C;
        c2 = lbl_eu_8066AE90;
        self->field_0x00[1].x = c1;
        self->field_0x00[1].y = c1;
        self->field_0x00[1].z = c1;
        self->field_0x00[2].x = c2;
        self->field_0x00[2].y = c2;
        self->field_0x00[2].z = c2;
    }
    func_804B0EA0(self);
}

// Kind/behaviour record pointed at by CColiNode804B09C8::field_0x08: the
// word at +0x7a4 carries the collision flags (bits 1/16/26) that gate the
// axis-block refill in func_804B0CE8.
struct CColiKind804B0CE8 {
    u8 _00[0x7a4];
    u32 field_0x7a4;
};

// Behaviour-flag / axis-refill update: clears the bit-1 flag and returns
// early, otherwise clears bit 3 and - when the node is valid and either its
// kind record's +0x7a4 flags are in the active combination (bit 1 set, bits
// 16/26 clear) or behaviour bit 11 is set - refills the +0x0c axis block
// with the two sdata2 constants, re-seeds the local proc and classifies the
// segment via func_804B27EC, then runs func_804B0EA0 when behaviour bits
// 7-8 are set. Always ends by setting bit 1.
void func_804B0CE8(CColiNode804B09C8* self) {
    u32 flags = self->field_0xa8;
    if (flags & 0x1) {
        self->field_0xa8 = flags & ~0x1;
        return;
    }
    self->field_0xa8 = flags & ~0x8;
    if (self->field_0x00 != 0 && self->field_0x08 != 0 &&
        self->field_0x04 != 0) {
        u32 w = ((CColiKind804B0CE8*)self->field_0x08)->field_0x7a4;
        if (((w & 0x2) && !(w & 0x10000) && !(w & 0x4000000)) ||
            (self->field_0xa8 & 0x800)) {
            // Axis-block refill (8066AE8C x3, 8066AE90 x3); c2 declared
            // first but c1 assigned first so its load lands first (retail
            // lfs f1,const1; lfs f0,const2).
            f32 c2;
            f32 c1;
            c1 = lbl_eu_8066AE8C;
            c2 = lbl_eu_8066AE90;
            self->field_0x0c[0] = c1;
            self->field_0x0c[1] = c1;
            self->field_0x0c[2] = c1;
            self->field_0x0c[3] = c2;
            self->field_0x0c[4] = c2;
            self->field_0x0c[5] = c2;
            CColiProcLocal proc;
            func_804B25A4(&proc, (CColiObject*)self->field_0x00,
                          self->field_0x08, (u32)self->field_0x04);
            func_804B27EC(&proc, self->field_0x0c, self->field_0x24,
                          &self->field_0x24[3], self->field_0xb0);
            if (self->field_0xa8 & 0xC0) {
                func_804B0EA0((CColiObject*)self);
            }
            self->field_0xa8 |= 0xc;
        }
    }
    self->field_0xa8 |= 0x2;
}

// Re-sort self into the keyed list after its +0x18 key changed: walk from
// the head and relink self after the last node whose key is still greater.
// The tail updates the +0xa4 scalar from +0x0c, clamped up to the next
// node's value. Skipped entirely when the +0xa8 bit-1 flag is set.
void func_804B0DF4(CColiNode804B09C8* self) {
    if (!(self->field_0xa8 & 0x2)) return;
    CColiNode804B09C8* ins = 0;
    CColiNode804B09C8* oldNext = self->field_0xa0;
    CColiNode804B09C8* next = oldNext;
    if (oldNext != 0) {
        while (next != 0) {
            if (self->field_0x0c[3] >= next->field_0x0c[3]) break;
            ins = next;
            next = next->field_0xa0;
        }
    }
    if (ins != 0) {
        CColiNode804B09C8* prev = self->field_0x9c;
        if (prev != 0) {
            prev->field_0xa0 = oldNext;
        }
        self->field_0xa0->field_0x9c = prev;
        CColiNode804B09C8* n6 = ins->field_0xa0;
        if (n6 != 0) {
            n6->field_0x9c = self;
        }
        self->field_0xa0 = n6;
        self->field_0x9c = ins;
        ins->field_0xa0 = self;
    }
    CColiNode804B09C8* n = self->field_0xa0;
    self->field_0xa4 = self->field_0x0c[0];
    if (n != 0 && self->field_0x0c[0] < n->field_0xa4) {
        self->field_0xa4 = n->field_0xa4;
    }
}

#pragma push
#pragma auto_inline off
// AABB refresh (retail body): when behaviour bits 0xd0 are set, grow the
// embedded corner pair (field_0x00[1] max, field_0x00[2] min) to enclose the
// segment [pos - move, pos + move] (the y-min side only encloses pos); when
// bit 5 is set, transform the +0x30 vector by the +0x3c matrix and grow the
// box by its magnitude the same way. auto_inline off: retail callers
// (func_804B0B54 / func_804B0C0C / func_804B0CE8) call via `bl`.
extern "C" void func_804B0EA0(CColiObject* self) {
    if (self->field_0xa8 & 0xd0) {
        if (self->field_0x00[3].x + self->field_0x30.x > self->field_0x00[1].x) {
            self->field_0x00[1].x = self->field_0x00[3].x + self->field_0x30.x;
        }
        if (self->field_0x00[3].x - self->field_0x30.x < self->field_0x00[2].x) {
            self->field_0x00[2].x = self->field_0x00[3].x - self->field_0x30.x;
        }
        if (self->field_0x00[3].z + self->field_0x30.x > self->field_0x00[1].z) {
            self->field_0x00[1].z = self->field_0x00[3].z + self->field_0x30.x;
        }
        if (self->field_0x00[3].z - self->field_0x30.x < self->field_0x00[2].z) {
            self->field_0x00[2].z = self->field_0x00[3].z - self->field_0x30.x;
        }
        if (self->field_0x00[3].y + self->field_0x30.y > self->field_0x00[1].y) {
            self->field_0x00[1].y = self->field_0x00[3].y + self->field_0x30.y;
        }
        if (self->field_0x00[3].y < self->field_0x00[2].y) {
            self->field_0x00[2].y = self->field_0x00[3].y;
        }
    }
    if (self->field_0xa8 & 0x20) {
        VEC3 out;
        nw4r::math::VEC3TransformNormal(
            &out, (const nw4r::math::MTX34*)&self->field_0x3c,
            (const VEC3*)&self->field_0x30);
        f32 mag = PSVECMag((const Vec*)&out);
        if (self->field_0x00[1].x < mag + self->field_0x00[3].x) {
            self->field_0x00[1].x = mag + self->field_0x00[3].x;
        }
        if (self->field_0x00[2].x > self->field_0x00[3].x - mag) {
            self->field_0x00[2].x = self->field_0x00[3].x - mag;
        }
        if (self->field_0x00[1].y < mag + self->field_0x00[3].y) {
            self->field_0x00[1].y = mag + self->field_0x00[3].y;
        }
        if (self->field_0x00[2].y > self->field_0x00[3].y - mag) {
            self->field_0x00[2].y = self->field_0x00[3].y - mag;
        }
        if (self->field_0x00[1].z < mag + self->field_0x00[3].z) {
            self->field_0x00[1].z = mag + self->field_0x00[3].z;
        }
        if (self->field_0x00[2].z > self->field_0x00[3].z - mag) {
            self->field_0x00[2].z = self->field_0x00[3].z - mag;
        }
    }
}
#pragma pop

// Collision-node refresh: when the +0x00/+0x08 words are set, free the owned
// +0x04 buffer, clear the +0xa8 status bits, then rebuild the local proc and
// classify the node (func_804B25BC writes the selection index into +0xb0 and
// the new buffer into +0x04). The status bits are set from the classification
// result. auto_inline off: retail callers (func_804B0A6C/804B0A74) call via
// `b`.
#pragma push
#pragma auto_inline off
extern "C" void func_804B102C(void* self) {
    CColiNode804B09C8* node = (CColiNode804B09C8*)self;
    if (node->field_0x08 != 0 && node->field_0x00 != 0) {
        if (node->field_0x04 != 0) {
            if (node->field_0x04 != 0) {
                mtl::MemManager::deallocate(node->field_0x04);
                node->field_0x04 = 0;
            }
        }
        node->field_0xa8 &= 0xFFFFBB01;
        CColiProcLocal proc;
        func_804B2590(&proc, (CColiObject*)node->field_0x00, node->field_0x08);
        switch (func_804B25BC(&proc, &node->field_0xb0,
                              (void**)&node->field_0x04, &node->field_0x24[3])) {
        case 1:
            node->field_0xa8 |= 0x80;
            break;
        case 2:
            node->field_0xa8 |= 0x40;
            break;
        }
        node->field_0xa8 |= 0x1;
        if (func_804B2F80(&proc)) {
            node->field_0xa8 |= 0x2000;
        }
        if (func_804B2FA8(&proc)) {
            node->field_0xa8 |= 0x4000;
        }
    }
}
#pragma pop

// retail: build a {f,f,f} local vec from the SDA2 float and forward the
// caller's r3-r5 to func_804B1164 with the vec in r6 (passthrough wrapper).
extern "C" void __declspec(noinline) func_804B1164(void* a, void* b, u32 c, f32* v);  // defined below
void func_804B1130(void* a, void* b, u32 c) {
    f32 v[3] = { lbl_eu_8066AE88, lbl_eu_8066AE88, lbl_eu_8066AE88 };
    func_804B1164(a, b, c, v);
}

extern "C" void __declspec(noinline) func_804B1164(void* a, void* b, u32 c, f32* v){}

// Segment AABB handed to func_804B06FC / func_804B0818: two corner VEC3s.
struct CColiSeg804B192C {
    VEC3 field_0x00[2];   // +0x00..+0x17 max/min corner pair
};

extern "C" int func_804B204C(CColiObject* self, CColiObject* v, u32 flag);

// Move test: build the segment AABB around (in, d) at +0x8 and, when it is
// contained in self's box, sample the partner segment at +0x20 and process
// it, forwarding the helper's status. The seg local is 0x18 bytes so the
// second local lands at sp+0x20 exactly like the retail frame.
int func_804B192C(CColiObject* self, const VEC3* in, f32 d, u32 a4, u32 a5) {
    CColiSeg804B192C seg;
    func_804B06FC((CColiObject*)&seg, in, d);
    if (func_804B0818((CColiObject*)&seg, self)) {
        CColiObject local;
        func_804A790C(&local, in, d, a5);
        return func_804B204C(self, &local, a4);
    }
    return 0;
}

// Move-check gate (sibling of func_804B1AD8 with a proc-seeded variant):
// when the last argument is non-zero, require behaviour bit 14, apply the
// shared kind/flag gate, then - when the node is valid and behaviour bit 3
// is set - seed the local proc and classify the local object via
// func_804B2E3C, returning 1 on success. Otherwise seed a local object from
// the sub-spec and forward self/local/arg to func_804B236C, returning its
// status (retail tail block).
int func_804B19CC(CColiObject* self, const CColiSubSpec804A7878* spec,
                  const void* a5, u32 a6) {
    CColiProcLocal proc;   // +0x8
    CColiObject local;     // +0x330
    CColiObject local2;    // +0x18
    if (a6 == 0) goto tail;
    if (!(self->field_0xa8 & 0x4000)) goto ret_gate;
    func_804A7878(&local, spec, 0);
    {
        int t = 1;
        int w = 1;
        u32 flags = self->field_0xa8;
        if (!(flags & 0x100)) {
            if (flags & 0x2) {
                w = 0;
            }
        }
        if (w == 0) {
            if (flags & 0x4) {
                t = 0;
            }
        }
        if (t != 0) return 0;
        local.field_0x314 = (u32)self;
        if (self->field_0x04 != 0 && (self->field_0xa8 & 0x8)) {
            func_804B25A4(&proc, self->field_0x00_obj, self->field_0x08,
                          self->field_0x04);
            if (func_804B2E3C(&proc, &local) != 0) return 1;
        }
        return 0;
    }
ret_gate:
    return 0;
tail:
    func_804A7878(&local2, spec, 0);
    return func_804B236C(self, &local2, a5);
}

// Move-check gate: the object must have a non-zero kind and the +0xa8 flag
// combination (bit 8 set) or (bit 1 clear) or (bit 2 clear), with bit 4 set
// and bit 3 set. On success, build the segment AABB around (v, f), verify it
// is contained in self's box, construct a local object via func_804A7BDC,
// link it to self and forward both to func_804B2CBC, returning its status.
bool func_804B1AD8(CColiObject* self, const VEC3* v, f32 f) {
    CColiProcLocal proc;
    CColiSeg804B192C seg;
    CColiObject local;
    if (self->field_0x04 == 0) goto fail;
    int t = 1;
    int w = 1;
    u32 flags = self->field_0xa8;
    if (!(flags & 0x100)) {
        if (flags & 0x2) {
            w = 0;
        }
    }
    if (w == 0) {
        if (flags & 0x4) {
            t = 0;
        }
    }
    if (t != 0) return 0;
    if (!(flags & 0x8)) goto fail;

    func_804B06FC((CColiObject*)&seg, v, f);
    if (!func_804B0818((CColiObject*)&seg, self)) goto fail;
    func_804A7BDC(&local, v, f);
    local.field_0x314 = (u32)self;
    func_804B25A4(&proc, self->field_0x00_obj, self->field_0x08,
                  self->field_0x04);
    if (func_804B2CBC(&proc, &local) == 0) goto fail;
    return true;
fail:
    return false;
}

// State-machine check before segment processing: the object must have a
// non-zero kind, and the +0xa8 flag combination must be (bit 8 set) or
// (bit 1 clear) or (bit 2 clear), with bit 4 set. On success, construct a
// local object via func_804A7D1C, link it to self, seed a local proc and
// forward both to func_804B2CBC, returning its status.
bool func_804B1BDC(CColiObject* self) {
    CColiProcLocal proc;   // +0x8
    CColiObject local;     // +0x18 (0x318 bytes)
    if (self->field_0x04 == 0) goto fail;
    int v = 1;
    CColiObject* tgt;
    u32 a8;
    int w = 1;
    u32 flags = self->field_0xa8;
    u32 a4;
    if (!(flags & 0x100)) {
        if (flags & 0x2) {
            w = 0;
        }
    }
    if (w == 0) {
        if (flags & 0x4) {
            v = 0;
        }
    }
    if (v != 0) return false;
    if (!(flags & 0x8)) goto fail;

    func_804A7D1C(&local);
    local.field_0x314 = (u32)self;
    tgt = self->field_0x00_obj;
    a8 = self->field_0x08;
    a4 = self->field_0x04;
    func_804B25A4(&proc, tgt, a8, a4);
    if (func_804B2CBC(&proc, &local) == 0) goto fail;
    return true;
fail:
    return false;
}

// Segment move-check gate (sibling of func_804B1AD8): require a non-zero
// kind and the shared +0xa8 flag combination, then build the segment AABB
// around (v, f1), verify it is contained in self's box, construct a local
// object from the same sub-spec via func_804A7C64, link it to self and
// classify it through the local proc, returning the classifier's status.
int func_804B1C9C(CColiObject* self, const VEC3* v, f32 f1, f32 f2, f32 f3) {
    CColiProcLocal proc;   // +0x8
    CColiSeg804B192C seg;  // +0x18
    CColiObject local;     // +0x30
    if (self->field_0x04 == 0) goto fail;
    int t = 1;
    int w = 1;
    u32 flags = self->field_0xa8;
    if (!(flags & 0x100)) {
        if (flags & 0x2) {
            w = 0;
        }
    }
    if (w == 0) {
        if (flags & 0x4) {
            t = 0;
        }
    }
    if (t != 0) return 0;
    if (!(flags & 0x8)) goto fail;
    func_804B06FC((CColiObject*)&seg, v, f1);
    if (!func_804B0818((CColiObject*)&seg, self)) goto fail;
    func_804A7C64(&local, (const CColiSubSpec804A7878*)v, f1, f2, f3);
    local.field_0x314 = (u32)self;
    func_804B25A4(&proc, self->field_0x00_obj, self->field_0x08,
                  self->field_0x04);
    if (func_804B2CBC(&proc, &local) == 0) goto fail;
    return 1;
fail:
    return 0;
}

void func_804B1DC0(u8* self, int arg) {
    int* flags = (int*)((char*)self + 0xa8);
    if (arg != 0) {
        *flags &= ~0x100;
    } else {
        *flags = (*flags | 0x100) & ~0x0E;
    }
}

// Box-sweep contact helpers (target func_804AF09C; sibling func_804AF07C).
extern "C" int func_804AF09C(CColiObject* self, const VEC3* extents,
                             const Mtx m1, const Mtx m2);
extern "C" int func_804AF07C(CColiObject* self, const VEC3* extents,
                             const Mtx m1, const Mtx m2);

// Move/segment dispatcher: the shared t/w flag gate, then zero the output
// vector, link the object to self and - when the node is valid and behaviour
// bit 3 is set - seed the local proc and classify the segment
// (func_804B29EC). Then dispatch to the sphere-sweep contact
// (func_804AE11C / func_804AE0D0) or the box-sweep contact
// (func_804AF09C / func_804AF07C) depending on the +0xa8 bit combination,
// sharing the current-segment global (lbl_eu_8065D0A0) with the callees.
int func_804B1DEC(CColiObject* self, VEC3* outVec, CColiObject* obj, f32 f) {
    int result = 0;
    int w = 1;
    int t = 1;
    u32 flags = self->field_0xa8;
    if (!(flags & 0x100)) {
        if (flags & 0x2) {
            t = 0;
        }
    }
    if (t == 0) {
        if (flags & 0x4) {
            w = 0;
        }
    }
    if (w != 0) return 0;

    outVec->x = lbl_eu_8066AE88;
    outVec->y = lbl_eu_8066AE88;
    outVec->z = lbl_eu_8066AE88;
    obj->field_0x314 = (u32)self;

    if (self->field_0x04 != 0 && (self->field_0xa8 & 0x8)) {
        lbl_eu_8065D0A0.field_0x3c = lbl_eu_8066AEA0;
        CColiProcLocal proc;
        func_804B25A4(&proc, self->field_0x00_obj, self->field_0x08,
                      self->field_0x04);
        proc.field_0xc |= 0x2;
        result = (result | func_804B29EC(&proc, outVec, obj,
                                         (self->field_0xa8 >> 9) & 1)) != 0;
    }

    u32 fl = self->field_0xa8;
    if (!((fl & 0x50) || ((fl & 0x80) && (obj->field_0x8c & 0x4)))) {
        goto block60F4;
    }
    // Behaviour 0x50/0x80 gate: seed the partner slot with the current-segment
    // global (or the embedded proc) and run the sphere-sweep contact. The
    // global address is kept in a local so it survives the callee call (r27).
    CColiGlobal805D0A0* g = &lbl_eu_8065D0A0;
    if (&lbl_eu_8065D0A0 != 0) {
        obj->field_0x3c_u = (u32)&lbl_eu_8065D0A0;
    } else {
        obj->field_0x3c_u = (u32)((u8*)obj + 4);
    }
    g->field_0x3c = f;
    g->field_0x38 = outVec;
    if (self->field_0xa8 & 0x400) {
        result = (result | func_804AE11C(obj, (const VEC3*)&self->field_0x00[3],
                                         self->field_0x30.x,
                                         self->field_0x30.y)) != 0;
    } else {
        g->field_0x40 = 1;
        result = (result | func_804AE0D0(obj, (const VEC3*)&self->field_0x00[3],
                                         self->field_0x30.x,
                                         self->field_0x30.y)) != 0;
        g->field_0x40 = 0;
    }
    goto exit;
block60F4:
    if (fl & 0x20) {
        CColiGlobal805D0A0* g2 = &lbl_eu_8065D0A0;
        if (&lbl_eu_8065D0A0 != 0) {
            obj->field_0x3c_u = (u32)&lbl_eu_8065D0A0;
        } else {
            obj->field_0x3c_u = (u32)((u8*)obj + 4);
        }
        g2->field_0x3c = lbl_eu_8066AEA0;
        g2->field_0x38 = outVec;
        if (self->field_0xa8 & 0x400) {
            result = (result | func_804AF09C(
                          obj, (const VEC3*)&self->field_0x30,
                          (const f32 (*)[4])&self->field_0x3c,
                          (const f32 (*)[4])&self->field_0x68.y)) != 0;
        } else {
            result = (result | func_804AF07C(
                          obj, (const VEC3*)&self->field_0x30,
                          (const f32 (*)[4])&self->field_0x3c,
                          (const f32 (*)[4])&self->field_0x68.y)) != 0;
        }
    }
exit:
    return result;
}

// Box-sweep contact: when behaviour bit 4 is set, transform self's +0x74
// point by m2 and - if the result lies inside the extents box - widen the
// sweep bound. Then transform self's +0x44 point by m2 and walk the three
// axes: the axis must be at least the +0x5c radius away (else fail), the
// clamped axis is written into the result vector, and two per-axis counts
// gate the final contact (all three axes within the shrunk box fail; all
// three within the grown box pass). On success the result vector is
// transformed by m1, the contact offset (result - point) is written into the
// proc object and its vtable slot 2 fires with the +0x314 argument.
extern "C" int func_804AF09C(CColiObject* self, const VEC3* extents,
                             const Mtx m1, const Mtx m2) {
    f32 bound = lbl_eu_8066AE44;
    CColiObj3C* proc = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
    proc->field_0x08 = lbl_eu_80663A90;
    Vec out;
    if (self->field_0x8c & 0x10) {
        PSMTXMultVec(m2, (const Vec*)&self->field_0x74, &out);
        int c = 0;
        if (extents->x >= out.x && -extents->x <= out.x) c++;
        if (extents->y >= out.y && -extents->y <= out.y) c++;
        if (extents->z >= out.z && -extents->z <= out.z) c++;
        if (c == 3) bound = lbl_eu_8066AE78;
    }
    PSMTXMultVec(m2, (const Vec*)&self->field_0x44, &out);
    Vec res;
    const f32* ep = &extents->x;
    f32* op = &out.x;
    f32* rp = &res.x;
    int c1 = 0;
    int c2 = 0;
    for (int i = 0; i < 3; i++) {
        if (*ep < self->field_0x5c) return 0;
        f32 d = *ep - self->field_0x5c;
        if (d >= *op && -d <= *op) c1++;
        if (d < *op) {
            *rp = d;
        } else {
            if (-d > *op) {
                *rp = -d;
            } else {
                *rp = *op;
            }
        }
        f32 g = self->field_0x5c + *ep + bound;
        if (g >= *op && -g <= *op) c2++;
        ep++;
        op++;
        rp++;
    }
    if (c1 == 3) goto fail;
    if (c2 == 3) goto contact;
fail:
    return 0;
contact:
    {
        Vec tmp;
        PSMTXMultVec(m1, (const Vec*)&res, &tmp);
        CColiObj3C* p2 = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
        nw4r::math::VEC3Sub((VEC3*)&p2->field_0x24, (const VEC3*)&tmp,
                            (const VEC3*)&self->field_0x44);
        CColiObj3C* p3 = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
        p3->vtbl->field_0x08(p3, self->field_0x314);
        return 1;
    }
}

// Seed the proc's +0x08 with the sweep global, clear the +0x8c axis bit,
// then pass the box description through to the point/box classifier
// (func_804AD410) with self's +0x5c radius. The args travel unchanged, so
// retail tail-calls.
extern "C" int func_804AF07C(CColiObject* self, const VEC3* extents,
                             const Mtx m1, const Mtx m2) {
    CColiObj3C* proc = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
    proc->field_0x08 = lbl_eu_80663A90;
    self->field_0x8c &= ~0x2u;
    return func_804AD410(self, (const f32*)extents, (const VEC3*)m1, m2,
                         self->field_0x5c);
}

// Segment-processing helper (retail body still to be decompiled); returns
// a status code that func_804B192C forwards to its caller.
// auto_inline off: retail calls this via `bl`.
#pragma push
#pragma auto_inline off
// Segment-processing helper (sibling of func_804B21A8): the shared flag gate
// (bits 1/2/4 vs 8) and the proc-seed block run first; when the caller flag
// is non-zero, behaviour bit 0x50 dispatches to the 2-point clip
// (func_804AE388) and bit 0x20 to the axis-frame clip (func_804AF2F0).
// auto_inline off: retail callers (func_804B192C) call via `bl`.
extern "C" int func_804B204C(CColiObject* self, CColiObject* v, u32 flag) {
    CColiProcLocal proc;
    int t = 1;
    int w = 1;
    u32 flags = self->field_0xa8;
    if (!(flags & 0x100)) {
        if (flags & 0x2) {
            w = 0;
        }
    }
    if (w == 0) {
        if (flags & 0x4) {
            t = 0;
        }
    }
    if (t != 0) return 0;
    v->field_0x314 = (u32)self;
    int result = 0;
    if (self->field_0x04 != 0 && (self->field_0xa8 & 0x8)) {
        func_804B25A4(&proc, self->field_0x00_obj, self->field_0x08,
                      self->field_0x04);
        if (self->field_0xa8 & 0x1000) {
            proc.field_0xc |= 0x4;
        }
        // result is provably 0 here, so MWCC folds `result | r3` into the
        // or-with-zero (ori r3, r3, 0) before the neg/or/srwi booleanize.
        result = (result | func_804B2AA4(&proc, v)) != 0;
    }
    if (flag != 0) {
        if (self->field_0xa8 & 0x50) {
            result = (result | func_804AE388(v, &self->field_0x00[3],
                                             self->field_0x30.x,
                                             self->field_0x30.y)) != 0;
        }
        if (self->field_0xa8 & 0x20) {
            result = (result | func_804AF2F0(v, &self->field_0x30,
                                             (const _VEC3*)&self->field_0x3c,
                                             (const _VEC3*)&self->field_0x68.y)) != 0;
        }
    }
    return result;
}
#pragma pop

// Segment-processing helper (sibling of func_804B204C): same gate and
// proc-seed block; behaviour bit 10 selects the capsule clip pair
// (func_804AEC8C / func_804AF32C) vs the sphere clip pair (func_804AE9A4 /
// func_804AF310), each with the 0x50/0x20 dispatch inside.
int func_804B21A8(CColiObject* self, CColiObject* v, u32 flag) {
    CColiProcLocal proc;
    int t = 1;
    int w = 1;
    u32 flags = self->field_0xa8;
    if (!(flags & 0x100)) {
        if (flags & 0x2) {
            w = 0;
        }
    }
    if (w == 0) {
        if (flags & 0x4) {
            t = 0;
        }
    }
    if (t != 0) return 0;
    v->field_0x314 = (u32)self;
    int result = 0;
    if (self->field_0x04 != 0 && (self->field_0xa8 & 0x8)) {
        func_804B25A4(&proc, self->field_0x00_obj, self->field_0x08,
                      self->field_0x04);
        if (self->field_0xa8 & 0x1000) {
            proc.field_0xc |= 0x4;
        }
        // result is provably 0 here, so MWCC folds `result | r3` into the
        // or-with-zero (ori r3, r3, 0) before the neg/or/srwi booleanize.
        result = (result | func_804B2AA4(&proc, v)) != 0;
    }
    if (flag != 0) {
        u32 fl = self->field_0xa8;
        if (fl & 0x400) {
            if (fl & 0x50) {
                result = (result | func_804AEC8C(v, &self->field_0x00[3],
                                                 self->field_0x30.x,
                                                 self->field_0x30.y)) != 0;
            }
            if (self->field_0xa8 & 0x20) {
                result = (result | func_804AF32C(v, &self->field_0x30,
                                                 (const _VEC3*)&self->field_0x3c,
                                                 (const _VEC3*)&self->field_0x68.y)) != 0;
            }
        } else {
            if (fl & 0x50) {
                result = (result | func_804AE9A4(v, &self->field_0x00[3],
                                                 self->field_0x30.x,
                                                 self->field_0x30.y)) != 0;
            }
            if (self->field_0xa8 & 0x20) {
                result = (result | func_804AF310(v, &self->field_0x30,
                                                 (const _VEC3*)&self->field_0x3c,
                                                 (const _VEC3*)&self->field_0x68.y)) != 0;
            }
        }
    }
    return result;
}

void func_804B236C(){}

// Scale the +0x24 vector by the +0x3c scalar, optionally overwrite v.y with
// the sdata2 constant when the +0x40 flag byte is set, then add the result
// into the VEC3 pointed at by +0x38.
void func_804B24A4(CColiObject* self) {
    VEC3 v(self->field_0x00[3].x, self->field_0x00[3].y, self->field_0x00[3].z);
    VEC3Scale(&v, &v, self->field_0x3c_f);
    if (self->field_0x40_b != 0) {
        v.y = lbl_eu_8066AE88;
    }
    VEC3Add((VEC3*)self->field_0x38, (VEC3*)self->field_0x38, &v);
}

// ---------------------------------------------------------------------------
// Sphere test with contact write-back (target func_804A7F50): when the
// squared distance from self's point (+0x44) to the partner's centre (+0x04)
// is within the squared sum of the two radii (+0x5c, +0x10), the proc object
// (+0x3c) gets the contact point (normal * radius + centre) at +0x0c, the
// contact normal at +0x18 (from the point offset, or randomized when the
// centres coincide) and the normal scaled by the penetration at +0x24; the
// vtable slot-2 callback fires with the +0x314 argument. The partner and
// proc pointers are re-read per region so they die before the rand/sqrt
// calls (retail re-loads them instead of keeping callee-saved copies).
// ---------------------------------------------------------------------------
extern "C" int func_804A7F50(CColiObject* self) {
    CColiCyl804AA870* partner = (CColiCyl804AA870*)self->field_0x00_obj;
    VEC3 diff;
    VEC3Sub(&diff, &self->field_0x44, &partner->field_0x04);
    f32 rsum = self->field_0x5c + partner->field_0x10;
    f32 dist2 = VEC3LenSq(&diff);
    if (rsum * rsum < dist2) return 0;
    f32 pen = rsum;
    if (dist2 != lbl_eu_8066AE44) {
        if (!(dist2 >= lbl_eu_8066AE44)) {
            nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
        }
        f32 dist = dist2 <= lbl_eu_8066AE44
                       ? lbl_eu_8066AE44
                       : dist2 * nw4r::math::FrSqrt(dist2);
        pen = rsum - dist;
        f32 inv = lbl_eu_8066AE3C / dist;
        CColiObj3C* proc = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
        VEC3Scale((VEC3*)&proc->field_0x18, &diff, inv);
    } else {
        // Random horizontal direction (y = 0) when the centres coincide.
        CColiObj3C* proc = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
        ml::CVec3* n = &proc->field_0x18;
        f32 rz = (f32)(rand() % 200 - 100);
        f32 rx = (f32)(rand() % 200 - 100);
        n->x = rx;
        n->y = lbl_eu_8066AE44;
        n->z = rz;
        if (n->x == lbl_eu_8066AE44 && n->z == lbl_eu_8066AE44) {
            n->x = lbl_eu_8066AE44;
            n->y = lbl_eu_8066AE44;
            n->z = lbl_eu_8066AE3C;
        } else {
            f32 len2 = n->y * n->y + n->x * n->x + n->z * n->z;
            if (len2 == lbl_eu_8066AE44) {
                *n = ml::CVec3::zero;
            } else {
                PSVECNormalize((const Vec*)n, (Vec*)n);
            }
        }
    }
    // Contact point: normal * partner radius + partner centre; the normal
    // scaled by the penetration goes to +0x24.  The partner/proc pointers
    // are re-derived per statement so MWCC reloads them after each store
    // (retail re-loads self->field_0x00 / self->field_0x3c four times).
    VEC3Scale(
        (VEC3*)&((CColiObj3C*)(uintptr_t)self->field_0x3c_u)->field_0x0c,
        (const VEC3*)&((CColiObj3C*)(uintptr_t)self->field_0x3c_u)
                         ->field_0x18,
        ((CColiCyl804AA870*)self->field_0x00_obj)->field_0x10);
    VEC3Add((VEC3*)&((CColiObj3C*)(uintptr_t)self->field_0x3c_u)->field_0x0c,
            (VEC3*)&((CColiObj3C*)(uintptr_t)self->field_0x3c_u)->field_0x0c,
            &((CColiCyl804AA870*)self->field_0x00_obj)->field_0x04);
    VEC3Scale(
        (VEC3*)&((CColiObj3C*)(uintptr_t)self->field_0x3c_u)->field_0x24,
        (const VEC3*)&((CColiObj3C*)(uintptr_t)self->field_0x3c_u)
                         ->field_0x18,
        pen);
    CColiObj3C* pc = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
    pc->vtbl->field_0x08(pc, self->field_0x314);
    return 1;
}

// ---------------------------------------------------------------------------
// Segment-sphere test with contact write-back (target func_804A8850): the
// partner's segment (base +0x04, direction +0x1c, radius +0x28, length^2
// +0x2c) is projected from self's point (+0x44); the clamped projection
// parameter scales the direction to give the closest point. When the squared
// distance from the point to the closest point is within the squared sum of
// the two radii, the proc object gets the contact point (normal * radius +
// closest point) at +0x0c, the normal at +0x18 and the normal scaled by the
// penetration at +0x24; flag bit 15 is set from bit 1 and the vtable slot-2
// callback fires. The partner pointer is re-read where retail re-loads it.
// ---------------------------------------------------------------------------
extern "C" int func_804A8850(CColiObject* self) {
    CColiSegment804ABAF0* partner = (CColiSegment804ABAF0*)self->field_0x00_obj;
    VEC3 diff;
    VEC3 v;
    VEC3Sub(&diff, &self->field_0x44, &partner->field_0x04);
    f32 t = clamp01(VEC3Dot(&partner->field_0x1c, &diff) / partner->field_0x2c);
    VEC3Scale(&v, &partner->field_0x1c, t);
    VEC3Add(&v, &v, &partner->field_0x04);
    VEC3Sub(&diff, &self->field_0x44, &v);
    f32 dist2 = VEC3LenSq(&diff);
    f32 rsum = self->field_0x5c +
               ((CColiSegment804ABAF0*)self->field_0x00_obj)->field_0x28;
    if (rsum * rsum < dist2) return 0;
    f32 pen = rsum;
    if (dist2 != lbl_eu_8066AE44) {
        if (!(dist2 >= lbl_eu_8066AE44)) {
            nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
        }
        f32 dist = dist2 <= lbl_eu_8066AE44
                       ? lbl_eu_8066AE44
                       : dist2 * nw4r::math::FrSqrt(dist2);
        pen = rsum - dist;
        f32 inv = lbl_eu_8066AE3C / dist;
        CColiObj3C* proc = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
        VEC3Scale((VEC3*)&proc->field_0x18, &diff, inv);
    } else {
        // Random horizontal direction (y = 0) when the points coincide.
        CColiObj3C* proc = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
        ml::CVec3* n = &proc->field_0x18;
        f32 rz = (f32)(rand() % 200 - 100);
        f32 rx = (f32)(rand() % 200 - 100);
        n->x = rx;
        n->y = lbl_eu_8066AE44;
        n->z = rz;
        if (n->x == lbl_eu_8066AE44 && n->z == lbl_eu_8066AE44) {
            n->x = lbl_eu_8066AE44;
            n->y = lbl_eu_8066AE44;
            n->z = lbl_eu_8066AE3C;
        } else {
            f32 len2 = n->y * n->y + n->x * n->x + n->z * n->z;
            if (len2 == lbl_eu_8066AE44) {
                *n = ml::CVec3::zero;
            } else {
                PSVECNormalize((const Vec*)n, (Vec*)n);
            }
        }
    }
    // Contact point: normal * partner radius + closest point; the normal
    // scaled by the penetration goes to +0x24.  The partner/proc pointers
    // are re-derived per statement so MWCC reloads them like retail.
    VEC3Scale(
        (VEC3*)&((CColiObj3C*)(uintptr_t)self->field_0x3c_u)->field_0x0c,
        (const VEC3*)&((CColiObj3C*)(uintptr_t)self->field_0x3c_u)
                         ->field_0x18,
        ((CColiSegment804ABAF0*)self->field_0x00_obj)->field_0x28);
    VEC3Add((VEC3*)&((CColiObj3C*)(uintptr_t)self->field_0x3c_u)->field_0x0c,
            (VEC3*)&((CColiObj3C*)(uintptr_t)self->field_0x3c_u)->field_0x0c,
            &v);
    VEC3Scale(
        (VEC3*)&((CColiObj3C*)(uintptr_t)self->field_0x3c_u)->field_0x24,
        (const VEC3*)&((CColiObj3C*)(uintptr_t)self->field_0x3c_u)
                         ->field_0x18,
        pen);
    if (self->field_0x8c & 0x2) {
        self->field_0x8c |= 0x8000;
    }
    CColiObj3C* pc = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
    pc->vtbl->field_0x08(pc, self->field_0x314);
    return 1;
}

// Partner geometry for func_804AA504: two points at +0x04/+0x10 and the
// radius scalar at +0x28 (the segment the sweep is tested against).
struct CColiSeg2Pt804AA504 {
    u8 _00[0x04];
    VEC3 field_0x04;       // +0x04 point A
    VEC3 field_0x10;       // +0x10 point B
    u8 _1c[0x28 - 0x1c];   // +0x1c..+0x27
    f32 field_0x28;        // +0x28 radius
};

// ---------------------------------------------------------------------------
// Clip-sweep test with contact write-back (target func_804AA504): when the
// sdata2 gate scale * +0x5c is at least +0x60 the sphere test
// (func_804A8850) handles it; otherwise the partner's segment (+0x04/+0x10)
// is clipped against the vertical band [y + 0x5c, y + 0x60 - 0x5c] of
// self's +0x50 point (func_804AFB28) and the squared distance between the
// clip endpoints is tested against the squared sum of the radii (+0x28,
// +0x5c). On a hit the proc object gets the normal at +0x18, the normal
// scaled by the penetration at +0x24 and the contact point (normal * radius
// + clip start) at +0x0c; flag bit 15 is set from bit 1 and the vtable
// slot-2 callback fires.
// ---------------------------------------------------------------------------
extern "C" int func_804AA504(CColiObject* self) {
    if (lbl_eu_8066AE58 * self->field_0x5c >= self->field_0x60) {
        return func_804A8850(self);
    }
    CColiSeg2Pt804AA504* partner = (CColiSeg2Pt804AA504*)self->field_0x00_obj;
    f32 dy = self->field_0x60 - self->field_0x5c;
    VEC3 a(self->field_0x50.x, self->field_0x50.y + self->field_0x5c,
           self->field_0x50.z);
    VEC3 b(self->field_0x50.x, self->field_0x50.y + dy, self->field_0x50.z);
    VEC3 outA;
    VEC3 outB;
    func_804AFB28(&outA, &outB, &partner->field_0x10, &partner->field_0x04,
                  &a, &b);
    VEC3 diff;
    VEC3Sub(&diff, &outB, &outA);
    f32 dist2 = VEC3LenSq(&diff);
    f32 rsum = self->field_0x5c +
               ((CColiSeg2Pt804AA504*)self->field_0x00_obj)->field_0x28;
    if (rsum * rsum < dist2) return 0;
    f32 pen = rsum;
    if (dist2 != lbl_eu_8066AE44) {
        if (!(dist2 >= lbl_eu_8066AE44)) {
            nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
        }
        f32 dist = dist2 <= lbl_eu_8066AE44
                       ? lbl_eu_8066AE44
                       : dist2 * nw4r::math::FrSqrt(dist2);
        pen = rsum - dist;
        f32 inv = lbl_eu_8066AE3C / dist;
        CColiObj3C* proc = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
        VEC3Scale((VEC3*)&proc->field_0x18, &diff, inv);
    } else {
        // Random horizontal direction (y = 0) when the clip is degenerate.
        CColiObj3C* proc = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
        ml::CVec3* n = &proc->field_0x18;
        f32 rz = (f32)(rand() % 200 - 100);
        f32 rx = (f32)(rand() % 200 - 100);
        n->x = rx;
        n->y = lbl_eu_8066AE44;
        n->z = rz;
        if (n->x == lbl_eu_8066AE44 && n->z == lbl_eu_8066AE44) {
            n->x = lbl_eu_8066AE44;
            n->y = lbl_eu_8066AE44;
            n->z = lbl_eu_8066AE3C;
        } else {
            f32 len2 = n->y * n->y + n->x * n->x + n->z * n->z;
            if (len2 == lbl_eu_8066AE44) {
                *n = ml::CVec3::zero;
            } else {
                PSVECNormalize((const Vec*)n, (Vec*)n);
            }
        }
    }
    // Penetration-scaled normal first, then the contact point.  The
    // partner/proc pointers are re-derived per statement so MWCC reloads
    // them like retail.
    VEC3Scale(
        (VEC3*)&((CColiObj3C*)(uintptr_t)self->field_0x3c_u)->field_0x24,
        (const VEC3*)&((CColiObj3C*)(uintptr_t)self->field_0x3c_u)
                         ->field_0x18,
        pen);
    VEC3Scale(
        (VEC3*)&((CColiObj3C*)(uintptr_t)self->field_0x3c_u)->field_0x0c,
        (const VEC3*)&((CColiObj3C*)(uintptr_t)self->field_0x3c_u)
                         ->field_0x18,
        ((CColiSeg2Pt804AA504*)self->field_0x00_obj)->field_0x28);
    VEC3Add((VEC3*)&((CColiObj3C*)(uintptr_t)self->field_0x3c_u)->field_0x0c,
            (VEC3*)&((CColiObj3C*)(uintptr_t)self->field_0x3c_u)->field_0x0c,
            &outA);
    if (self->field_0x8c & 0x2) {
        self->field_0x8c |= 0x8000;
    }
    CColiObj3C* pc = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
    pc->vtbl->field_0x08(pc, self->field_0x314);
    return 1;
}

// ---------------------------------------------------------------------------
// Cylinder clip with contact write-back (target func_804AAA98): the
// partner's centre (+0x04) and its y-band offsets (+0x14 upper, +0x18
// lower) form the two clip segment points passed to func_804B028C along
// with self's +0x44/+0x50 points and the partner radius (+0x10). When the
// clip reports a hit the contact normal is derived from the clip point and
// the segment points (up/down for exact y matches, radial otherwise, or
// randomized when degenerate) and the vtable slot-2 callback fires.
// ---------------------------------------------------------------------------
extern "C" int func_804AAA98(CColiObject* self) {
    CColiCylinder804ABA68* partner =
        (CColiCylinder804ABA68*)self->field_0x00_obj;
    VEC3 base(partner->field_0x04.x,
              partner->field_0x04.y + partner->field_0x14,
              partner->field_0x04.z);
    VEC3 top(partner->field_0x04.x,
             partner->field_0x04.y + partner->field_0x18,
             partner->field_0x04.z);
    CColiObj3C* proc = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
    if (func_804B028C(&self->field_0x44, &self->field_0x50, &base, &top,
                      (VEC3*)&proc->field_0x0c, partner->field_0x10) == 0) {
        return 0;
    }
    proc = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
    if (base.y == proc->field_0x0c.y) {
        proc->field_0x18.x = lbl_eu_8066AE44;
        proc->field_0x18.y = lbl_eu_8066AE3C;
        proc->field_0x18.z = lbl_eu_8066AE44;
    } else if (top.y == proc->field_0x0c.y) {
        proc->field_0x18.x = lbl_eu_8066AE44;
        proc->field_0x18.y = lbl_eu_8066AE50;
        proc->field_0x18.z = lbl_eu_8066AE44;
    } else {
        f32 dz = proc->field_0x0c.z - top.z;
        f32 dx = proc->field_0x0c.x - top.x;
        f32 dist2 = dz * dz + dx * dx;
        if (dist2 != lbl_eu_8066AE44) {
            if (!(dist2 >= lbl_eu_8066AE44)) {
                nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
            }
            f32 dist = dist2 <= lbl_eu_8066AE44
                           ? lbl_eu_8066AE44
                           : dist2 * nw4r::math::FrSqrt(dist2);
            f32 inv = lbl_eu_8066AE3C / dist;
            CColiObj3C* p3 = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
            p3->field_0x18.x = dx * inv;
            p3->field_0x18.y = lbl_eu_8066AE44;
            p3->field_0x18.z = dz * inv;
        } else {
            // Random horizontal direction (y = 0) when the clip is degenerate.
            ml::CVec3* n = &proc->field_0x18;
            f32 rz = (f32)(rand() % 200 - 100);
            f32 rx = (f32)(rand() % 200 - 100);
            n->x = rx;
            n->y = lbl_eu_8066AE44;
            n->z = rz;
            if (n->x == lbl_eu_8066AE44 && n->z == lbl_eu_8066AE44) {
                n->x = lbl_eu_8066AE44;
                n->y = lbl_eu_8066AE44;
                n->z = lbl_eu_8066AE3C;
            } else {
                f32 len2 = n->y * n->y + n->x * n->x + n->z * n->z;
                if (len2 == lbl_eu_8066AE44) {
                    *n = ml::CVec3::zero;
                } else {
                    PSVECNormalize((const Vec*)n, (Vec*)n);
                }
            }
        }
    }
    proc = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
    proc->vtbl->field_0x08(proc, self->field_0x314);
    return 1;
}

// ---------------------------------------------------------------------------
// Sphere clip with contact write-back (target func_804AE9A4): the partner
// segment point (vec) and its +y top point form the two clip segment points
// passed to func_804B028C along with self's +0x44/+0x50 points and the
// caller's x scalar. When the clip reports a hit the contact normal is
// derived from the clip point and the segment points (up/down for exact y
// matches, radial otherwise, or randomized when degenerate) and the vtable
// slot-2 callback fires.
// ---------------------------------------------------------------------------
extern "C" int func_804AE9A4(CColiObject* self, const _VEC3* vec, f32 x,
                             f32 y) {
    CColiObj3C* proc = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
    proc->field_0x08 = lbl_eu_80663A90;
    VEC3 base(vec->x, vec->y, vec->z);
    VEC3 top(vec->x, vec->y + y, vec->z);
    if (func_804B028C(&self->field_0x44, &self->field_0x50, &base, &top,
                      (VEC3*)&proc->field_0x0c, x) == 0) {
        return 0;
    }
    proc = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
    if (base.y == proc->field_0x0c.y) {
        proc->field_0x18.x = lbl_eu_8066AE44;
        proc->field_0x18.y = lbl_eu_8066AE3C;
        proc->field_0x18.z = lbl_eu_8066AE44;
    } else if (top.y == proc->field_0x0c.y) {
        proc->field_0x18.x = lbl_eu_8066AE44;
        proc->field_0x18.y = lbl_eu_8066AE50;
        proc->field_0x18.z = lbl_eu_8066AE44;
    } else {
        f32 dz = proc->field_0x0c.z - top.z;
        f32 dx = proc->field_0x0c.x - top.x;
        f32 dist2 = dz * dz + dx * dx;
        if (dist2 != lbl_eu_8066AE44) {
            if (!(dist2 >= lbl_eu_8066AE44)) {
                nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
            }
            f32 dist = dist2 <= lbl_eu_8066AE44
                           ? lbl_eu_8066AE44
                           : dist2 * nw4r::math::FrSqrt(dist2);
            f32 inv = lbl_eu_8066AE3C / dist;
            CColiObj3C* p3 = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
            p3->field_0x18.x = dx * inv;
            p3->field_0x18.y = lbl_eu_8066AE44;
            p3->field_0x18.z = dz * inv;
        } else {
            // Random horizontal direction (y = 0) when the clip is degenerate.
            ml::CVec3* n = &proc->field_0x18;
            f32 rz = (f32)(rand() % 200 - 100);
            f32 rx = (f32)(rand() % 200 - 100);
            n->x = rx;
            n->y = lbl_eu_8066AE44;
            n->z = rz;
            if (n->x == lbl_eu_8066AE44 && n->z == lbl_eu_8066AE44) {
                n->x = lbl_eu_8066AE44;
                n->y = lbl_eu_8066AE44;
                n->z = lbl_eu_8066AE3C;
            } else {
                f32 len2 = n->y * n->y + n->x * n->x + n->z * n->z;
                if (len2 == lbl_eu_8066AE44) {
                    *n = ml::CVec3::zero;
                } else {
                    PSVECNormalize((const Vec*)n, (Vec*)n);
                }
            }
        }
    }
    proc = (CColiObj3C*)(uintptr_t)self->field_0x3c_u;
    proc->vtbl->field_0x08(proc, self->field_0x314);
    return 1;
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_804B2524(){}
