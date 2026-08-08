// LOD terrain-render state manager (monolib/src/lod/UnkClass_8046368C).
//
// Private downstream fork decompilation.  Every entry point keeps its retail
// (shortened) "...Fv" mangled name via extern "C"; the real ABI arguments are
// passed explicitly, per docs/MWCC_REFERENCE.md "Fv ABI note" (Metrowerks
// passes extra registers even on Fv names; data tables reference the short
// names).
//
// The unit manages a large BSS state object (lbl_eu_80657FB0) holding basis
// vectors and transform matrices, plus a bank of sbss pointer/flag globals
// shared with the neighbouring LOD translation units.

#include <harness_catalog.h>
#include <revolution/GX.h>
#include <revolution/MTX.h>
#include <nw4r/math/math_triangular.h>
#include <PowerPC_EABI_Support/Runtime/MWCPlusLib.h>
#include "monolib/math/CVec3.hpp"

extern "C" {

// ---------------------------------------------------------------------------
// Shared sdata2 constants (named pool references for matching).
// ---------------------------------------------------------------------------
extern const f32 lbl_eu_8066A5E8; // 40.743664f: angle -> nw4r fidx scale
extern const f32 lbl_eu_8066A5F4; // 0.0f
extern const f32 lbl_eu_8066A5F8; // 1.0f
extern const f32 lbl_eu_8066A5FC; // 1000000.0f

// ---------------------------------------------------------------------------
// sbss state globals (small data, r13-relative).
// ---------------------------------------------------------------------------
extern u8* lbl_eu_80665738;      // cache records, stride 0xC
extern u32* lbl_eu_8066574C;     // index -> pair table
extern u8* lbl_eu_80665750;      // pair table, stride 2
extern u8* lbl_eu_80665768;      // vertex pool base
extern u8* lbl_eu_80665770;      // texcoord array (stride 8)
extern u8* lbl_eu_80665774;      // colour array (stride 4)
extern u8* lbl_eu_80665778;      // draw records, stride 0x18
extern u8* lbl_eu_8066577C;      // draw records, stride 0x48
extern u8* lbl_eu_80665780;      // current record + 8
extern u8* lbl_eu_80665784;      // object records, stride 0xC
extern u32 lbl_eu_80665788;
extern u32 lbl_eu_8066578C;
extern s32 lbl_eu_80665790;      // texcoord array high-water mark
extern s32 lbl_eu_80665798;      // cached matrix slot
extern const void* lbl_eu_8066579C; // cached matrix pointer
extern s32 lbl_eu_806657A0;
extern f32 lbl_eu_806657A4;
extern s32 lbl_eu_806657A8;
extern u8* lbl_eu_806657AC;      // matrix records, stride 0xB4
extern u32 lbl_eu_806657B0;      // dirty flags
extern u32 lbl_eu_806657B4;      // current record id
extern s32 lbl_eu_806657B8;      // distance limit
extern u32 lbl_eu_806657BC;
extern u32 lbl_eu_806657C0;
extern u32 lbl_eu_806657C4;
extern u16 lbl_eu_806657C8;      // normal stride in bytes
extern u8* lbl_eu_8066581C;
extern u8* lbl_eu_80665820;
extern u8* lbl_eu_80665824;

// ---------------------------------------------------------------------------
// BSS objects (far access).
// ---------------------------------------------------------------------------
extern f32 lbl_eu_80658038[3];   // fade/colour scalars
extern u8 lbl_eu_80658048[0x180]; // 8 x 0x30 work objects
extern u8 lbl_eu_806581C8[0x180]; // 8 x 0x30 work objects

// ---------------------------------------------------------------------------
// Large LOD state object at 0x80657FB0.
//
// Layout (from func_80463814 / func_80463C24):
//   0x000  v[10]   basis words: v0 @0x00, v1 @0x0C, translation @0x18
//                  (the matrix view reads words 0x00..0x28 with the fourth
//                  column zeroed)
//   0x028  mtx28   position matrix (retail aliases v[10]/v[11])
//   0x058  mtx58   base transform
//   0x218  nrm[3]  normal matrices
// ---------------------------------------------------------------------------
typedef struct LodGlobal {
    union {
        f32 v[10];
        u32 words[10];
    };
    Mtx mtx28;
    Mtx mtx58;
    u8 gap88[0x218 - 0x88];
    Mtx nrm[3];
} LodGlobal;

// Mtx is a raw C array; copy it via an aggregate wrapper (retail emits a
// plain 12-word copy, not PSMTXCopy).
typedef union LodMtxCopy {
    Mtx m;
    u32 words[12];
} LodMtxCopy;

extern LodGlobal lbl_eu_80657FB0;

// ---------------------------------------------------------------------------
// Retail dispatch tables (.data).
// ---------------------------------------------------------------------------
typedef void (*LodDrawFn)(void*, void*);
typedef s32 (*LodPickFn)(void*);
extern LodDrawFn lbl_eu_8056D710[3];
extern LodPickFn lbl_eu_8056D71C[3];

// ---------------------------------------------------------------------------
// External LOD helpers (other translation units, retail mangled names).
// ---------------------------------------------------------------------------
void func_804630C0__Q23LOD17CLODCacheManagerSFv(void*);
void* func_8046323C__Q23LOD17CLODCacheManagerSFv(void*);
void func_8046368C__Q23LOD17UnkClass_8046368CFv(void);
void func_8046369C__Q23LOD17UnkClass_8046368CFv(void);
void func_80465704(s32);
void func_80465718__Q23LOD17UnkClass_804645CCFv(void);
void func_80465730(f32);
void func_8046577C(s32);
void func_8006BEC0(void*);

// ---------------------------------------------------------------------------
// Small record layouts.
// ---------------------------------------------------------------------------
typedef struct LodRec18 {
    u32 id;      // 0x00
    u16 f04;     // 0x04
    u16 kind;    // 0x06
    u8 gap08[0x10];
} LodRec18; // stride 0x18

typedef struct LodCacheObj {
    u32 f00;
    u32 f04;
    u32 f08;
    u32 f0C;
    u32 f10;
    u32 f14;
    u32 f18;
    u32 f1C;
    u32 f20;
    u32 f24;
} LodCacheObj;

typedef struct LodDrawParam {
    u32 flags;   // 0x00
    u32 f04;     // 0x04
    u32 f08;     // 0x08
    s16 f0C;     // 0x0C
    s16 f0E;     // 0x0E
    u32 f10;     // 0x10
    u32 f14;     // 0x14
    s16 f18;     // 0x18
    s16 f1A;     // 0x1A
    u8 f1C;      // 0x1C
    u8 f1D;      // 0x1D
    u8 f1E;      // 0x1E
    u8 f1F;      // 0x1F
} LodDrawParam;

typedef struct LodPickObj {
    u32 off;     // 0x00
    u16 f04;     // 0x04
    u16 kind;    // 0x06
} LodPickObj;

typedef struct LodRangeObj {
    u8 gap[0x1E];
    u16 start;   // 0x1E
    u16 end;     // 0x20
    u8 gap22[2];
    u8 mode;     // 0x24
} LodRangeObj;

typedef struct LodRotObj {
    f32 f00;
    f32 f04;
    u8 gap08[8];
    f32 f10;
    f32 f14;
} LodRotObj;

typedef struct LodVtxPool {
    u32 f00;
    u32 version;
} LodVtxPool;

} // extern "C"

// ===========================================================================
// us-8046773c  func_8046376C  (rotation table entry, angle in f1)
// ===========================================================================
extern "C" void func_8046376C__Q23LOD17UnkClass_8046368CFv(LodRotObj* self, f32 angle) {
    f32 s = nw4r::math::SinFIdx(lbl_eu_8066A5E8 * angle);
    f32 c = nw4r::math::CosFIdx(lbl_eu_8066A5E8 * angle);
    self->f00 = c;
    self->f04 = -s;
    self->f10 = s;
    self->f14 = c;
}

// ===========================================================================
// us-804677b8  func_804637E8  (index lookup + tail call)
// ===========================================================================
extern "C" void* func_804637E8__Q23LOD17UnkClass_8046368CFv(s32 index) {
    u32 entry = lbl_eu_8066574C[index];
    u16 kind = *(const u16*)(lbl_eu_80665750 + entry * 2 + 2);
    return func_8046323C__Q23LOD17CLODCacheManagerSFv(lbl_eu_80665738 + kind * 0xC);
}

// ===========================================================================
// us-804677e4  func_80463814  (frame setup: matrices, vertex formats, GX)
// ===========================================================================
extern "C" void func_80463814__Q23LOD17UnkClass_8046368CFv(
    const Mtx* input, void* records78, void* records84, void* texBase70,
    void* clrBase74, void* matsAC, void* records7C, u32 val8C, u32 val88) {
    LodGlobal* g = &lbl_eu_80657FB0;

    lbl_eu_80665778 = (u8*)records78;
    *(LodMtxCopy*)g->mtx28 = *(const LodMtxCopy*)input;
    lbl_eu_80665784 = (u8*)records84;
    lbl_eu_80665770 = (u8*)texBase70;
    lbl_eu_80665774 = (u8*)clrBase74;
    lbl_eu_806657AC = (u8*)matsAC;
    lbl_eu_8066577C = (u8*)records7C;
    lbl_eu_80665788 = val88;
    lbl_eu_8066578C = val8C;

    PSMTXInverse(g->mtx28, g->mtx28);
    GXSetCurrentMtx(0);
    LodVtxPool* pool = (LodVtxPool*)lbl_eu_80665768;
    func_8046369C__Q23LOD17UnkClass_8046368CFv();

    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    if (pool->version >= 0x3EE) {
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_S16, 14);
        lbl_eu_806657C8 = 6;
        if (pool->version >= 0x3EF) {
            func_8046368C__Q23LOD17UnkClass_8046368CFv();
        }
    } else {
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_F32, 0);
        lbl_eu_806657C8 = 12;
    }

    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGB, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_TEX_ST, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX2, GX_TEX_ST, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX3, GX_TEX_ST, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX4, GX_TEX_ST, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX5, GX_TEX_ST, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX6, GX_TEX_ST, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX7, GX_TEX_ST, GX_F32, 0);

    GXSetZCompLoc(GX_FALSE);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_ALPHA0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT0, GX_DF_NONE, GX_AF_NONE);
    GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);

    // Snapshot the previous basis (rotation part) before resetting it.
    LodMtxCopy prev;
    prev.words[0] = g->words[0];
    prev.words[1] = g->words[1];
    prev.words[2] = g->words[2];
    prev.words[3] = *(const u32*)&lbl_eu_8066A5F4;
    prev.words[4] = g->words[4];
    prev.words[5] = g->words[5];
    prev.words[6] = g->words[6];
    prev.words[7] = *(const u32*)&lbl_eu_8066A5F4;
    prev.words[8] = g->words[8];
    prev.words[9] = g->words[9];
    prev.words[10] = ((const LodMtxCopy*)g->mtx28)->words[0];
    prev.words[11] = *(const u32*)&lbl_eu_8066A5F4;

    // Reset basis: unit x/y axes plus the input translation column.
    const LodMtxCopy* inputBits = (const LodMtxCopy*)input;
    g->words[6] = inputBits->words[3];
    g->words[7] = inputBits->words[7];
    g->words[8] = inputBits->words[11];
    g->v[0] = lbl_eu_8066A5F8;
    g->v[1] = lbl_eu_8066A5F4;
    g->v[2] = lbl_eu_8066A5F4;
    g->v[3] = lbl_eu_8066A5F4;
    g->v[4] = lbl_eu_8066A5F8;
    g->v[5] = lbl_eu_8066A5F4;

    // Re-derive the working axes from the previous basis and normalize
    // (zero-length axes fall back to ml::CVec3::zero).
    Vec* axis0 = reinterpret_cast<Vec*>(&g->v[0]);
    Vec* axis1 = reinterpret_cast<Vec*>(&g->v[3]);
    PSMTXMultVec(prev.m, axis0, axis0);
    PSMTXMultVec(prev.m, axis1, axis1);

    f32 sq0 = g->v[0] * g->v[0] + g->v[1] * g->v[1] + g->v[2] * g->v[2];
    if (sq0 == lbl_eu_8066A5F4) {
        *reinterpret_cast<ml::CVec3*>(&g->v[0]) = ml::CVec3::zero;
    } else {
        PSVECNormalize(axis0, axis0);
    }

    f32 sq1 = g->v[3] * g->v[3] + g->v[4] * g->v[4] + g->v[5] * g->v[5];
    if (sq1 == lbl_eu_8066A5F4) {
        *reinterpret_cast<ml::CVec3*>(&g->v[3]) = ml::CVec3::zero;
    } else {
        PSVECNormalize(axis1, axis1);
    }

    lbl_eu_806657B4 = (u32)-1;
    lbl_eu_806657A4 = lbl_eu_8066A5FC;
    GXLoadPosMtxImm(g->mtx28, 0);
}

// ===========================================================================
// us-80467bf4  func_80463C24  (matrix slot upload with 1-entry cache)
// ===========================================================================
void func_80463C24(s32 index, s32 offset, const Mtx* mtx) {
    LodGlobal* g = &lbl_eu_80657FB0;

    s32 slot = (index << 3) + ((lbl_eu_806657A0 + offset) & 7);
    if (!(lbl_eu_806657B0 & 0x4)) {
        goto update;
    }
    if (lbl_eu_80665798 != slot) {
        goto update;
    }
    if (lbl_eu_8066579C != mtx) {
        goto update;
    }
    return;

update:
    u8* record = lbl_eu_806657AC + slot * 0xB4;
    Mtx work[3];
    PSMTXConcat(g->mtx58, *mtx, work[0]);
    PSMTXConcat(work[0], *(const Mtx*)(record + 0x1A4), work[1]);
    PSMTXConcat(work[0], *(const Mtx*)(record + 0x174), work[2]);

    Mtx* workCursor = work;
    Mtx* normalCursor = g->nrm;
    u32 mtxId = 6;
    s32 i = 0;
    while (i < 3) {
        Mtx t;
        PSMTXConcat(g->mtx28, *workCursor, t);
        GXLoadPosMtxImm(t, mtxId);
        PSMTXInverse(t, *normalCursor);
        PSMTXTranspose(*normalCursor, t);
        GXLoadNrmMtxImm(t, mtxId);
        ++i;
        ++workCursor;
        ++normalCursor;
        mtxId += 3;
    }

    lbl_eu_80665798 = slot;
    lbl_eu_8066579C = mtx;
    lbl_eu_806657B0 |= 0x4;
}

// ===========================================================================
// us-80467d14  func_80463D44  (per-record draw dispatch)
// ===========================================================================
extern "C" void func_80463D44__Q23LOD17UnkClass_8046368CFv(LodDrawParam* p) {
    LodRec18* rec = (LodRec18*)(lbl_eu_80665778 + p->f18 * 0x18);

    if (lbl_eu_806657B4 != rec->id) {
        lbl_eu_806657B0 = (lbl_eu_806657B0 | 0x01) & ~0x82u;
    } else {
        lbl_eu_806657B0 &= ~0x81u;
    }

    lbl_eu_806657A0 = p->f1A;
    lbl_eu_806657A8 = p->f0C;
    func_8046577C(p->f0E);

    if (p->flags & 0x1000) {
        lbl_eu_806657BC = p->f1E;
        if (p->flags & 0x2000) {
            lbl_eu_806657C0 = p->f1F;
            lbl_eu_806657C4 = p->f1C;
            lbl_eu_806657B0 |= 0x40;
        }
        lbl_eu_8056D710[rec->kind - 2](lbl_eu_80665768 + rec->id,
                                       lbl_eu_8066577C + p->f1A * 0x48);
        if ((lbl_eu_806657B0 & 0x40) && !(lbl_eu_806657B0 & 0x80)) {
            p->flags &= ~0x2000u;
        }
    } else {
        lbl_eu_8056D710[rec->kind - 2](lbl_eu_80665768 + rec->id,
                                       lbl_eu_8066577C + p->f1A * 0x48);
    }

    lbl_eu_806657B4 = rec->id;
}

// ===========================================================================
// us-80467e80  func_80463EB0  (cache object bind + dispatch)
// ===========================================================================
extern "C" void func_80463EB0__Q23LOD17UnkClass_8046368CFv(u32 index, u32 value) {
    u32* rec = (u32*)lbl_eu_80665784 + index * 3;

    lbl_eu_806657B0 |= 0x3;
    lbl_eu_80665780 = (u8*)(rec + 2);

    LodCacheObj* obj = (LodCacheObj*)rec[3];
    u8* addr824 = (u8*)obj + obj->f24;
    u8* addr820 = (u8*)obj + obj->f0C;
    u8* addr81C = (u8*)obj + obj->f04;
    lbl_eu_8066581C = addr81C;
    lbl_eu_80665820 = addr820;
    lbl_eu_80665824 = addr824;

    func_804630C0__Q23LOD17CLODCacheManagerSFv((u8*)obj + obj->f20);

    lbl_eu_806657A0 = value;
    lbl_eu_8056D710[1]((u8*)obj + obj->f1C, lbl_eu_8066577C + value * 0x48);
}

// ===========================================================================
// us-80467f30  func_80463F60  (pick dispatch, tail call)
// ===========================================================================
extern "C" s32 func_80463F60__Q23LOD17UnkClass_8046368CFv(const LodPickObj* obj) {
    return lbl_eu_8056D71C[obj->kind - 2](lbl_eu_80665768 + obj->off);
}

// ===========================================================================
// us-80467f5c  func_80463F8C  (conditional scalar updates, args in f1..f3)
// ===========================================================================
void func_80463F8C(f32 a, f32 b, f32 c) {
    if (lbl_eu_8066A5F4 != a) {
        lbl_eu_80658038[0] = a;
        lbl_eu_806657B0 |= 0x8;
    }
    if (lbl_eu_8066A5F4 != b) {
        lbl_eu_806657B0 |= 0x10;
        lbl_eu_80658038[1] = b;
    }
    if (lbl_eu_8066A5F4 == c) {
        return;
    }
    lbl_eu_806657B0 |= 0x20;
    lbl_eu_80658038[2] = c;
}

// ===========================================================================
// us-80467fc8  func_80463FF8  (range crosses the distance limit?)
// ===========================================================================
extern "C" s32 func_80463FF8__Q23LOD17UnkClass_8046368CFv(const LodRangeObj* obj) {
    if (obj->mode & 0x2) {
        s32 a = (s32)obj->start * 60;
        s32 b = (s32)obj->end * 60;
        s32 lim;
        if (a <= b) {
            goto ordered;
        }

        lim = lbl_eu_806657B8;
        if (a <= lim) {
            return 1;
        }
        if (b < lim) {
            goto return_zero;
        }
        return 1;

ordered:
        lim = lbl_eu_806657B8;
        if (a > lim) {
            goto return_zero;
        }
        if (b < lim) {
            goto return_zero;
        }
        return 1;

return_zero:
        return 0;
    }
    return 1;
}

// ===========================================================================
// us-8046803c  func_8046406C  (register vertex arrays with GX)
// ===========================================================================
void func_8046406C(s32 count) {
    if (!(lbl_eu_806657B0 & 0x2)) {
        lbl_eu_806657B0 |= 0x2;
        GXSetArray(GX_VA_CLR0, lbl_eu_80665774, 4);
        s32 i = 0;
        while (i < count) {
            GXSetArray((GXAttr)(GX_VA_TEX0 + i), lbl_eu_80665770, 8);
            ++i;
        }
        lbl_eu_80665790 = count;
    } else {
        s32 i = lbl_eu_80665790;
        while (i < count) {
            GXSetArray((GXAttr)(GX_VA_TEX0 + i), lbl_eu_80665770, 8);
            ++i;
        }
        if (lbl_eu_80665790 < count) {
            lbl_eu_80665790 = count;
        }
    }
    GXSetCullMode(GX_CULL_NONE);
}

// ===========================================================================
// us-804680f8  func_80464128  (flag-driven fade update, returns "unchanged")
// ===========================================================================
s32 func_80464128(u32 arg) {
    if (arg & 0x4) {
        s32 v = lbl_eu_806657A8;
        if (v == 0) {
            return 1;
        }
        if (v != 0xFF) {
            func_80465704(v);
        }
    }

    u32 flags = lbl_eu_806657B0;
    if (flags & 0x38) {
        if (!(arg & 0x78)) {
            return 0;
        }
        if ((arg & 0x18) == 0x18) {
            f32 cur = lbl_eu_80658038[2];
            if (lbl_eu_8066A5F8 == cur) {
                return 1;
            }
            func_80465730(lbl_eu_8066A5F8 - cur);
            return 0;
        }
        if ((arg & 0x60) == 0x60) {
            f32 cur = lbl_eu_80658038[2];
            if (lbl_eu_8066A5F8 != cur) {
                func_80465730(cur);
            }
            return 0;
        }
        for (s32 i = 0; i < 2; ++i) {
            u32 bit = 8u << i;
            if (flags & bit) {
                if (arg & bit) {
                    f32 cur = lbl_eu_80658038[i];
                    if (lbl_eu_8066A5F8 == cur) {
                        return 1;
                    }
                    func_80465730(lbl_eu_8066A5F8 - cur);
                    return 0;
                }
                if (arg & (0x20u << i)) {
                    f32 cur = lbl_eu_80658038[i];
                    if (lbl_eu_8066A5F8 != cur) {
                        func_80465730(cur);
                    }
                    return 0;
                }
            } else if (arg & (0x20u << i)) {
                return 1;
            }
        }
        return 0;
    }

    if (arg & 0x60) {
        return 1;
    }
    return 0;
}

// ===========================================================================
// us-8046828c  func_804642BC  (fade ratio by group id)
// ===========================================================================
s32 func_804642BC(s32 a, u32 b) {
    if (lbl_eu_806657B0 & 0x40) {
        if ((s32)lbl_eu_806657BC == a) {
            if ((s32)lbl_eu_806657C4 >= (s32)b) {
                return 0;
            }
            if (b == 0) {
                return 0;
            }
            lbl_eu_806657B0 |= 0x80;
            func_80465730((f32)lbl_eu_806657C4 / (f32)b);
            return 0;
        }
        if ((s32)lbl_eu_806657C0 == a) {
            if ((s32)lbl_eu_806657C4 >= (s32)b) {
                return 1;
            }
            if (b == 0) {
                return 1;
            }
            lbl_eu_806657B0 |= 0x80;
            func_80465730(
                lbl_eu_8066A5F8 - (f32)lbl_eu_806657C4 / (f32)b);
            return 0;
        }
        return 1;
    }
    if ((s32)lbl_eu_806657BC == a) {
        return 0;
    }
    return 1;
}

// ===========================================================================
// us-804683a8  func_804643D8  (range fade with entry/exit interpolation)
// ===========================================================================
s32 func_804643D8(s32 a, s32 b, s32 da, s32 db) {
    s32 lim = lbl_eu_806657B8;

    if (a <= b) {
        goto ordered;
    }

    if (a <= lim) {
        if (da != 0 && a + da > lim) {
            s32 over = a + da - lim;
            func_80465704(255 - (over * 255) / da);
        } else {
            func_80465718__Q23LOD17UnkClass_804645CCFv();
        }
        return 0;
    }
    if (b < lim) {
        return 1;
    }
    if (db != 0 && b - db < lim) {
        s32 under = b - lim;
        func_80465704((under * 255) / db);
    } else {
        func_80465718__Q23LOD17UnkClass_804645CCFv();
    }
    return 0;

ordered:
    if (a > lim) {
        return 1;
    }
    if (b < lim) {
        return 1;
    }
    if (da != 0 && a + da > lim) {
        s32 over2 = a + da - lim;
        func_80465704(255 - (over2 * 255) / da);
        return 0;
    }
    if (db != 0 && b - db < lim) {
        s32 under2 = b - lim;
        func_80465704((under2 * 255) / db);
        return 0;
    }
    func_80465718__Q23LOD17UnkClass_804645CCFv();
    return 0;
}

// ===========================================================================
// us-804684ec  func_8046451C  (does [a,b] straddle the distance limit?)
// ===========================================================================
s32 func_8046451C(s32 a, s32 b) {
    s32 lim;
    if (a <= b) {
        goto ordered;
    }

    lim = lbl_eu_806657B8;
    if (a <= lim) {
        return 1;
    }
    if (b < lim) {
        goto return_zero;
    }
    return 1;

ordered:
    lim = lbl_eu_806657B8;
    if (a > lim) {
        goto return_zero;
    }
    if (b < lim) {
        goto return_zero;
    }
    return 1;

return_zero:
    return 0;
}

// ===========================================================================
// us-8046853c  sinit_8046456C  (construct the two 8x0x30 BSS work arrays)
// ===========================================================================
extern "C" void sinit_8046456C(void) {
    __construct_array(lbl_eu_80658048, (ConstructorDestructor)func_8006BEC0, NULL, 0x30, 8);
    __construct_array(lbl_eu_806581C8, (ConstructorDestructor)func_8006BEC0, NULL, 0x30, 8);
}

// Retail .ctors entry for this translation unit.
__declspec(section ".ctors") extern void (* const s_lodUnkClass8046368CInit)(void) = sinit_8046456C;
