// LOD terrain-render state manager (monolib/src/lod/UnkClass_8046368C).
//
// Private downstream fork decompilation.  Every entry point keeps its retail
// (shortened) "...Fv" mangled name via extern "C"; the real ABI arguments are
// passed explicitly, per docs/MWCC_CASES.md "Fv ABI note" (Metrowerks
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

extern const f32 lbl_eu_8066A5F4; // 0.0f (setLodFadeScalars pool label)

// sbss state globals (small data, r13-relative).
// ---------------------------------------------------------------------------
extern u8* sLodCacheRecords;      // cache records, stride 0xC
extern u32* sLodPairOffsets;     // index -> pair table
extern u8* sLodPairTable;      // pair table, stride 2
extern u8* sLodCacheTailFlag;      // vertex pool base
extern u8* lbl_eu_80665770;      // texcoord array (stride 8)
extern u8* lbl_eu_80665774;      // colour array (stride 4)
extern u8* LodDrawRecords18;      // draw records, stride 0x18
extern u8* LodDrawRecords48;      // draw records, stride 0x48
extern u8* lbl_eu_80665780;      // current record + 8
extern u8* LodObjectRecords;      // object records, stride 0xC
extern u32 LodMatrixIndexTable;
extern u32 LodMatrixDataTable;
extern s32 LodTexcoordHighWater;      // texcoord array high-water mark
extern s32 LodMatrixCacheSlot;      // cached matrix slot
extern const void* LodMatrixCachePtr; // cached matrix pointer
extern s32 LodMatrixSlotBias;
extern f32 LodCachedDistance;
extern s32 LodFadeAlpha;
extern u8* LodMatrixRecordBase;      // matrix records, stride 0xB4
extern u32 LodStateFlags;      // dirty flags
extern u32 LodCurrentRecordId;      // current record id
extern s32 LodDistanceLimit;      // distance limit
extern u32 LodFadeGroupA;
extern u32 LodFadeGroupB;
extern u32 LodFadeGroupValue;
extern u16 LodNormalStride;      // normal stride in bytes
extern u8* lbl_eu_8066581C;
extern u8* lbl_eu_80665820;
extern u8* lbl_eu_80665824;

// ---------------------------------------------------------------------------
// BSS objects (far access).
// ---------------------------------------------------------------------------
extern f32 LodFadeScalars[3];   // fade/colour scalars
extern u8 LodMatrixWorkArea[0x180]; // 8 x 0x30 work objects
extern u8 LodBillboardMatrixTable[0x180]; // 8 x 0x30 work objects

// ---------------------------------------------------------------------------
// Large LOD state object at 0x80657FB0.
//
// Layout (from setupLodFrame / uploadLodMatrix):
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
extern LodDrawFn LodDrawDispatchTable[3];
extern LodPickFn LodPickDispatchTable[3];
extern "C" void dispatchLodRecords__Q23LOD17UnkClass_80468434Fv();

// ---------------------------------------------------------------------------
// External LOD helpers (other translation units, retail mangled names).
// ---------------------------------------------------------------------------
void initLodTables__Q23LOD17CLODCacheManagerSFv(void*);
void* catmullRomBin__Q23LOD17CLODCacheManagerSFv(void*);
void selectCubicLookup__Q23LOD17UnkClass_8046368CFv(void);
void selectLinearLookup__Q23LOD17UnkClass_8046368CFv(void);
void setLodFadeAlpha__Fl(s32);
void resetLodFlags__Q23LOD17UnkClass_804645CCFv(void);
void scaleLodFadeAlpha__Ff(f32);
void setLodColorFactor__Fl(s32);
void cfCam_nopCtorSlot(void*);

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

extern "C" s32 isRangeActive__Q23LOD17UnkClass_8046368CFv(const LodRangeObj*);
// [.data] 0x8056D71C-0x8056D728 (12B): retail pick-dispatch table.  Retail
// split object's .data section is align 4; MWCC would otherwise emit align 8
// for a u32[3], so force __declspec(align(4)).
extern "C" __declspec(align(4)) LodPickFn LodPickDispatchTable[3] = {
    (LodPickFn)&isRangeActive__Q23LOD17UnkClass_8046368CFv,
    (LodPickFn)&dispatchLodRecords__Q23LOD17UnkClass_80468434Fv,
    (LodPickFn)&isRangeActive__Q23LOD17UnkClass_8046368CFv,
};


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
extern "C" void setLodRotation__Q23LOD17UnkClass_8046368CFv(LodRotObj* self, f32 angle) {
    f32 s = nw4r::math::SinFIdx(40.743664f * angle);
    f32 c = nw4r::math::CosFIdx(40.743664f * angle);
    self->f00 = c;
    self->f04 = -s;
    self->f10 = s;
    self->f14 = c;
}

// ===========================================================================
// us-804677b8  func_804637E8  (index lookup + tail call)
// ===========================================================================
extern "C" void* getLodCacheObject__Q23LOD17UnkClass_8046368CFv(s32 index) {
    u32 entry = sLodPairOffsets[index];
    u16 kind = *(const u16*)(sLodPairTable + entry * 2 + 2);
    return catmullRomBin__Q23LOD17CLODCacheManagerSFv(sLodCacheRecords + kind * 0xC);
}

// ===========================================================================
// us-804677e4  setupLodFrame  (frame setup: matrices, vertex formats, GX)
// ===========================================================================
extern "C" void setupLodFrame__Q23LOD17UnkClass_8046368CFv(
    const Mtx* input, void* records78, void* records84, void* texBase70,
    void* clrBase74, void* matsAC, void* records7C, u32 val8C, u32 val88) {
    LodGlobal* g = &lbl_eu_80657FB0;

    LodDrawRecords18 = (u8*)records78;
    *(LodMtxCopy*)g->mtx28 = *(const LodMtxCopy*)input;
    LodObjectRecords = (u8*)records84;
    lbl_eu_80665770 = (u8*)texBase70;
    lbl_eu_80665774 = (u8*)clrBase74;
    LodMatrixRecordBase = (u8*)matsAC;
    LodDrawRecords48 = (u8*)records7C;
    LodMatrixIndexTable = val88;
    LodMatrixDataTable = val8C;

    PSMTXInverse(g->mtx28, g->mtx28);
    GXSetCurrentMtx(0);
    LodVtxPool* pool = (LodVtxPool*)sLodCacheTailFlag;
    selectLinearLookup__Q23LOD17UnkClass_8046368CFv();

    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    if (pool->version >= 0x3EE) {
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_S16, 14);
        LodNormalStride = 6;
        if (pool->version >= 0x3EF) {
            selectCubicLookup__Q23LOD17UnkClass_8046368CFv();
        }
    } else {
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_F32, 0);
        LodNormalStride = 12;
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
    g->v[0] = 1.0f;
    g->v[1] = 0.0f;
    g->v[2] = 0.0f;
    g->v[3] = 0.0f;
    g->v[4] = 1.0f;
    g->v[5] = 0.0f;

    // Re-derive the working axes from the previous basis and normalize
    // (zero-length axes fall back to ml::CVec3::zero).
    Vec* axis0 = reinterpret_cast<Vec*>(&g->v[0]);
    Vec* axis1 = reinterpret_cast<Vec*>(&g->v[3]);
    PSMTXMultVec(prev.m, axis0, axis0);
    PSMTXMultVec(prev.m, axis1, axis1);

    f32 sq0 = g->v[0] * g->v[0] + g->v[1] * g->v[1] + g->v[2] * g->v[2];
    if (sq0 == 0.0f) {
        *reinterpret_cast<ml::CVec3*>(&g->v[0]) = ml::CVec3::zero;
    } else {
        PSVECNormalize(axis0, axis0);
    }

    f32 sq1 = g->v[3] * g->v[3] + g->v[4] * g->v[4] + g->v[5] * g->v[5];
    if (sq1 == 0.0f) {
        *reinterpret_cast<ml::CVec3*>(&g->v[3]) = ml::CVec3::zero;
    } else {
        PSVECNormalize(axis1, axis1);
    }

    LodCurrentRecordId = (u32)-1;
    LodCachedDistance = 1000000.0f;
    GXLoadPosMtxImm(g->mtx28, 0);
}

// ===========================================================================
// us-80467bf4  uploadLodMatrix  (matrix slot upload with 1-entry cache)
// ===========================================================================
void uploadLodMatrix(s32 index, s32 offset, const Mtx* mtx) {
    s32 slot = (index << 3) + ((LodMatrixSlotBias + offset) & 7);
    LodGlobal* g = &lbl_eu_80657FB0;

    if (!(LodStateFlags & 0x4)) {
        goto update;
    }
    if (LodMatrixCacheSlot != slot) {
        goto update;
    }
    if (LodMatrixCachePtr != mtx) {
        goto update;
    }
    return;

update:
    u8* record = LodMatrixRecordBase + slot * 0xB4;
    Mtx work[3];
    PSMTXConcat(g->mtx58, *mtx, work[0]);
    PSMTXConcat(work[0], *(const Mtx*)(record + 0x1A4), work[1]);
    PSMTXConcat(work[0], *(const Mtx*)(record + 0x174), work[2]);

    Mtx* normalCursor = g->nrm;
    Mtx* workCursor = work;
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

    LodMatrixCacheSlot = slot;
    LodMatrixCachePtr = mtx;
    LodStateFlags |= 0x4;
}

// ===========================================================================
// us-80467d14  dispatchLodDraw  (per-record draw dispatch)
// ===========================================================================
extern "C" void dispatchLodDraw__Q23LOD17UnkClass_8046368CFv(LodDrawParam* p) {
    LodRec18* rec = (LodRec18*)(LodDrawRecords18 + p->f18 * 0x18);

    if (LodCurrentRecordId != rec->id) {
        LodStateFlags = (LodStateFlags | 0x01) & ~0x82u;
    } else {
        LodStateFlags &= ~0x81u;
    }

    LodMatrixSlotBias = p->f1A;
    LodFadeAlpha = p->f0C;
    setLodColorFactor__Fl(p->f0E);

    if (p->flags & 0x1000) {
        LodFadeGroupA = p->f1E;
        if (p->flags & 0x2000) {
            LodFadeGroupB = p->f1F;
            LodFadeGroupValue = p->f1C;
            LodStateFlags |= 0x40;
        }
        LodDrawDispatchTable[rec->kind - 2](sLodCacheTailFlag + rec->id,
                                       LodDrawRecords48 + p->f1A * 0x48);
        if ((LodStateFlags & 0x40) && !(LodStateFlags & 0x80)) {
            p->flags &= ~0x2000u;
        }
    } else {
        LodDrawDispatchTable[rec->kind - 2](sLodCacheTailFlag + rec->id,
                                       LodDrawRecords48 + p->f1A * 0x48);
    }

    LodCurrentRecordId = rec->id;
}

// ===========================================================================
// us-80467e80  bindLodCache  (cache object bind + dispatch)
// ===========================================================================
extern "C" void bindLodCache__Q23LOD17UnkClass_8046368CFv(u32 index, u32 value) {
    u32* rec = (u32*)LodObjectRecords + index * 3;

    LodStateFlags |= 0x3;
    lbl_eu_80665780 = (u8*)(rec + 2);

    LodCacheObj* obj = (LodCacheObj*)rec[3];
    u8* addr824 = (u8*)obj + obj->f24;
    u8* addr820 = (u8*)obj + obj->f0C;
    u8* addr81C = (u8*)obj + obj->f04;
    lbl_eu_8066581C = addr81C;
    lbl_eu_80665820 = addr820;
    lbl_eu_80665824 = addr824;

    initLodTables__Q23LOD17CLODCacheManagerSFv((u8*)obj + obj->f20);

    LodMatrixSlotBias = value;
    LodDrawDispatchTable[1]((u8*)obj + obj->f1C, LodDrawRecords48 + value * 0x48);
}

// ===========================================================================
// us-80467f30  func_80463F60  (pick dispatch, tail call)
// ===========================================================================
extern "C" s32 dispatchLodPick__Q23LOD17UnkClass_8046368CFv(const LodPickObj* obj) {
    return LodPickDispatchTable[obj->kind - 2](sLodCacheTailFlag + obj->off);
}

// ===========================================================================
// us-80467f5c  func_80463F8C  (conditional scalar updates, args in f1..f3)
// ===========================================================================
void setLodFadeScalars(f32 a, f32 b, f32 c) {
    if (lbl_eu_8066A5F4 != a) {
        LodFadeScalars[0] = a;
        LodStateFlags |= 0x8;
    }
    if (lbl_eu_8066A5F4 != b) {
        LodStateFlags |= 0x10;
        LodFadeScalars[1] = b;
    }
    if (lbl_eu_8066A5F4 == c) {
        return;
    }
    LodStateFlags |= 0x20;
    LodFadeScalars[2] = c;
}

// ===========================================================================
// us-80467fc8  func_80463FF8  (range crosses the distance limit?)
// ===========================================================================
extern "C" s32 isRangeActive__Q23LOD17UnkClass_8046368CFv(const LodRangeObj* obj) {
    if (obj->mode & 0x2) {
        s32 lim;
        if ((s32)obj->start * 60 <= (s32)obj->end * 60) {
            goto ordered;
        }

        lim = LodDistanceLimit;
        if ((s32)obj->start * 60 <= lim) {
            return 1;
        }
        if ((s32)obj->end * 60 < lim) {
            goto return_zero;
        }
        return 1;

ordered:
        lim = LodDistanceLimit;
        if ((s32)obj->start * 60 > lim) {
            goto return_zero;
        }
        if ((s32)obj->end * 60 < lim) {
            goto return_zero;
        }
        return 1;

return_zero:
        return 0;
    }
    return 1;
}

// ===========================================================================
// us-8046803c  bindLodVertexArrays  (register vertex arrays with GX)
// ===========================================================================
void bindLodVertexArrays(s32 count) {
    if (!(LodStateFlags & 0x2)) {
        LodStateFlags |= 0x2;
        GXSetArray(GX_VA_CLR0, lbl_eu_80665774, 4);
        s32 i = 0;
        while (i < count) {
            GXSetArray((GXAttr)(GX_VA_TEX0 + i), lbl_eu_80665770, 8);
            ++i;
        }
        LodTexcoordHighWater = count;
    } else {
        s32 i = LodTexcoordHighWater;
        while (i < count) {
            GXSetArray((GXAttr)(GX_VA_TEX0 + i), lbl_eu_80665770, 8);
            ++i;
        }
        if (LodTexcoordHighWater < count) {
            LodTexcoordHighWater = count;
        }
    }
    GXSetCullMode(GX_CULL_NONE);
}

// ===========================================================================
// us-804680f8  updateLodFade  (flag-driven fade update, returns "unchanged")
// ===========================================================================
s32 updateLodFade(u32 arg) {
    if (arg & 0x4) {
        s32 v = LodFadeAlpha;
        if (v == 0) {
            return 1;
        }
        if (v != 0xFF) {
            setLodFadeAlpha__Fl(v);
        }
    }

    u32 flags = LodStateFlags;
    if (flags & 0x38) {
        if (!(arg & 0x78)) {
            return 0;
        }
        if ((arg & 0x18) == 0x18) {
            f32 cur = LodFadeScalars[2];
            if (1.0f == cur) {
                return 1;
            }
            scaleLodFadeAlpha__Ff(1.0f - cur);
            return 0;
        }
        if ((arg & 0x60) == 0x60) {
            f32 cur = LodFadeScalars[2];
            if (1.0f != cur) {
                scaleLodFadeAlpha__Ff(cur);
            }
            return 0;
        }
        for (s32 i = 0; i < 2; ++i) {
            u32 bit = 8u << i;
            if (flags & bit) {
                if (arg & bit) {
                    f32 cur = LodFadeScalars[i];
                    if (1.0f == cur) {
                        return 1;
                    }
                    scaleLodFadeAlpha__Ff(1.0f - cur);
                    return 0;
                }
                if (arg & (0x20u << i)) {
                    f32 cur = LodFadeScalars[i];
                    if (1.0f != cur) {
                        scaleLodFadeAlpha__Ff(cur);
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
// us-8046828c  getLodFadeRatio  (fade ratio by group id)
// ===========================================================================
s32 getLodFadeRatio(s32 a, u32 b) {
    if (LodStateFlags & 0x40) {
        if ((s32)LodFadeGroupA == a) {
            if ((s32)LodFadeGroupValue >= (s32)b) {
                return 0;
            }
            if (b == 0) {
                return 0;
            }
            LodStateFlags |= 0x80;
            scaleLodFadeAlpha__Ff((f32)(s32)LodFadeGroupValue / (f32)(s32)b);
            return 0;
        }
        if ((s32)LodFadeGroupB == a) {
            if ((s32)LodFadeGroupValue >= (s32)b) {
                return 1;
            }
            if (b == 0) {
                return 1;
            }
            LodStateFlags |= 0x80;
            scaleLodFadeAlpha__Ff(1.0f - (f32)(s32)LodFadeGroupValue / (f32)(s32)b);
            return 0;
        }
        return 1;
    }
    if ((s32)LodFadeGroupA == a) {
        return 0;
    }
    return 1;
}

// ===========================================================================
// us-804683a8  applyLodRangeFade  (range fade with entry/exit interpolation)
// ===========================================================================
s32 applyLodRangeFade(s32 a, s32 b, s32 da, s32 db) {
    s32 lim = LodDistanceLimit;

    if (a <= b) {
        goto ordered;
    }

    if (a <= lim) {
        if (da != 0 && a + da > lim) {
            s32 over = a + da - lim;
            setLodFadeAlpha__Fl(255 - (over * 255) / da);
        } else {
            resetLodFlags__Q23LOD17UnkClass_804645CCFv();
        }
        return 0;
    }
    if (b < lim) {
        return 1;
    }
    if (db != 0 && b - db < lim) {
        s32 under = b - lim;
        setLodFadeAlpha__Fl((under * 255) / db);
    } else {
        resetLodFlags__Q23LOD17UnkClass_804645CCFv();
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
        setLodFadeAlpha__Fl(255 - (over2 * 255) / da);
        return 0;
    }
    if (db != 0 && b - db < lim) {
        s32 under2 = b - lim;
        setLodFadeAlpha__Fl((under2 * 255) / db);
        return 0;
    }
    resetLodFlags__Q23LOD17UnkClass_804645CCFv();
    return 0;
}

// ===========================================================================
// us-804684ec  isLodRangeStraddlesLimit  (does [a,b] straddle the distance limit?)
// ===========================================================================
s32 isLodRangeStraddlesLimit(s32 a, s32 b) {
    s32 lim;
    if (a <= b) {
        goto ordered;
    }

    lim = LodDistanceLimit;
    if (a <= lim) {
        return 1;
    }
    if (b < lim) {
        goto return_zero;
    }
    return 1;

ordered:
    lim = LodDistanceLimit;
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
    __construct_array(LodMatrixWorkArea, (ConstructorDestructor)cfCam_nopCtorSlot, NULL, 0x30, 8);
    __construct_array(LodBillboardMatrixTable, (ConstructorDestructor)cfCam_nopCtorSlot, NULL, 0x30, 8);
}

// Retail .ctors entry for this translation unit.
__declspec(section ".ctors") extern void (* const s_lodUnkClass8046368CInit)(void) = sinit_8046456C;
