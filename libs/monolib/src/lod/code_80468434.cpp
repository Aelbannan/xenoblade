#include <types.h>
#include <revolution/GX.h>
#include <revolution/MTX.h>
#include "monolib/lod/LODMemMan.hpp"
#include "monolib/lod/code_804645CC.hpp"
#include "libs/monolib/src/lod/code_80468434.hpp"

using namespace LOD;

// ===========================================================================
// us-8046cbac  func_80468BDC  (billboard record draw, dual normal + facing)
// Same guard chain as func_80469138; the billboard uses two transformed
// direction normals (X and Y axes) and each record may lean toward the
// matrix translation column by field_0x14 (a normalized offset added to the
// base position).  Returns the +0x1C record view.
// ===========================================================================
u16* LOD::UnkClass_80468434::func_80468BDC() {
    LodDrawHeader* h = (LodDrawHeader*)this;
    // Integer-sum form keeps (idx * 0x2C) as the add's first operand
    // (retail: add r4, r4, r3; addi r0, r4, 0x1C).
    u16* rec = (u16*)(h->field_0x2 * 0x2c + (u32)this + 0x1c);

    // Visibility guard chain (mirrors the cull helpers' checks).
    if ((h->field_0x1 & lbl_eu_8066A5F0) != 0) {
        if ((h->field_0x1 & 2) != 0) {
            if (func_804643D8(h->field_0x8 * 0x3c, h->field_0xA * 0x3c,
                              h->field_0xC * 0x3c, h->field_0xD * 0x3c) != 0) {
                return rec;
            }
        } else {
            func_80465718__Q23LOD17UnkClass_804645CCFv();
        }
        if (func_80464128(h->field_0x1) != 0) {
            return rec;
        }
        if ((h->field_0x1 & 0x80) != 0) {
            if (func_804642BC(h->field_0x10, h->field_0x12) != 0) {
                return rec;
            }
        }
    } else {
        func_80465718__Q23LOD17UnkClass_804645CCFv();
    }

    // Matrix slot + matrix select (u16 at +0x0E indexes lbl_eu_806581C8,
    // zero selects the fixed lbl_eu_806583E0 table).
    func_804683D0__Q23LOD17UnkClass_80466348Fv((h->field_0xE_u16 + 1) * 3);
    const nw4r::math::MTX34* mtx;
    if (h->field_0xE_u16 != 0) {
        mtx = (const nw4r::math::MTX34*)lbl_eu_806581C8[h->field_0xE_u16 - 1];
    } else {
        mtx = (const nw4r::math::MTX34*)&lbl_eu_806583E0[0][0][0];
    }

    func_80465314(h->field_0x6);
    func_8046534C__Q23LOD17UnkClass_804645CCFv(h->field_0x4);
    s32 texCount = h->field_0x6 >> 8;
    func_8046406C(texCount);

    u32 count = h->field_0x2;

    // X/Y axis direction normals + translation column of the billboard
    // matrix; both normals are normalized (or zeroed when degenerate).
    nw4r::math::VEC3 normalX(lbl_eu_8066A648, lbl_eu_8066A64C, lbl_eu_8066A64C);
    nw4r::math::VEC3 normalY(lbl_eu_8066A64C, lbl_eu_8066A648, lbl_eu_8066A64C);
    nw4r::math::VEC3 transCol(mtx->_03, mtx->_13, mtx->_23);
    nw4r::math::VEC3TransformNormal(&normalX, mtx, &normalX);
    nw4r::math::VEC3TransformNormal(&normalY, mtx, &normalY);

    f32 lenSqX = normalX.x * normalX.x + normalX.y * normalX.y +
                 normalX.z * normalX.z;
    if (lenSqX == lbl_eu_8066A64C) {
        normalX.x = ml::CVec3::zero.x;
        normalX.y = ml::CVec3::zero.y;
        normalX.z = ml::CVec3::zero.z;
    } else {
        PSVECNormalize((const Vec*)&normalX, (Vec*)&normalX);
    }
    f32 lenSqY = normalY.x * normalY.x + normalY.y * normalY.y +
                 normalY.z * normalY.z;
    if (lenSqY == lbl_eu_8066A64C) {
        normalY.x = ml::CVec3::zero.x;
        normalY.y = ml::CVec3::zero.y;
        normalY.z = ml::CVec3::zero.z;
    } else {
        PSVECNormalize((const Vec*)&normalY, (Vec*)&normalY);
    }

    // Record billboard loop (records start at this + 0x1C).
    GXBegin(GX_QUADS, GX_VTXFMT0, (u16)(count * 4));
    LodDrawRec* recs = (LodDrawRec*)((u8*)this + 0x1c);
    nw4r::math::VEC3 pos;
    nw4r::math::VEC3 diff;
    nw4r::math::VEC3 scaledA;
    nw4r::math::VEC3 scaledB;
    nw4r::math::VEC3 scaledC;
    nw4r::math::VEC3 corner[4];
    for (u32 i = 0; i < count; i++) {
        LodDrawRec* r = &recs[i];

        nw4r::math::VEC3Scale(&scaledA, &normalX, r->field_0xC);
        nw4r::math::VEC3Scale(&scaledB, &normalY, r->field_0x10);
        nw4r::math::VEC3Scale(&scaledC, &normalY, r->field_0x18);

        nw4r::math::VEC3Sub(&corner[3], &scaledB, &scaledA);
        nw4r::math::VEC3Add(&corner[0], &scaledA, &scaledB);
        nw4r::math::VEC3Sub(&corner[2], &scaledC, &scaledA);
        nw4r::math::VEC3Add(&corner[1], &scaledC, &scaledA);

        if ((r->field_0x2B & 1) != 0) {
            lbl_eu_8056D9B0[r->field_0x29](&pos, &corner[0], r, i);
        } else {
            pos.x = r->field_0x0;
            pos.y = r->field_0x4;
            pos.z = r->field_0x8;
        }
        if (r->field_0x14 != lbl_eu_8066A64C) {
            // Lean the billboard toward the matrix translation column.
            nw4r::math::VEC3Sub(&diff, &transCol, &pos);
            f32 lenSq = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
            if (lenSq == lbl_eu_8066A64C) {
                diff.x = ml::CVec3::zero.x;
                diff.y = ml::CVec3::zero.y;
                diff.z = ml::CVec3::zero.z;
            } else {
                PSVECNormalize((const Vec*)&diff, (Vec*)&diff);
            }
            nw4r::math::VEC3Scale(&diff, &diff, r->field_0x14);
            nw4r::math::VEC3Add(&pos, &pos, &diff);
        }

        for (u32 k = 0; k < 4; k++) {
            GXPosition3f32(pos.x + corner[k].x, pos.y + corner[k].y,
                           pos.z + corner[k].z);
            GXColor1u16(r->field_0x1C[k]);
            for (u32 t = 0; t < (u32)texCount; t++) {
                GXTexCoord1x8(r->field_0x24[k]);
            }
        }
    }
    return rec;
}

// ===========================================================================
// us-8046c404  func_80468434  (record render dispatch)
// Stores the object as current, selects the view matrix (from the matrix
// tables or the caller's argument), uploads it, assembles the per-record
// matrices, submits the display lists, registers the vertex arrays and
// dispatches the record's render functions through lbl_eu_8056D980.
// ===========================================================================
void func_80468434__Q23LOD17UnkClass_80468434Fv(UnkClass_80468434* self,
                                                u8* arg2) {
    LodHeader34* o = (LodHeader34*)self;
    u32 idx = lbl_eu_80665794;
    lbl_eu_8066576C = (u8*)self;
    u16 count = *(u16*)((u8*)self + idx * 2 + 6);
    if (count == 0) {
        return;
    }

    // Select the view matrix: from the shared matrix tables (bit 5 of the
    // u16 at arg2+0x44) or copied from the caller's 12 floats.
    if ((*(u16*)(arg2 + 0x44) & 0x20) != 0) {
        u8* p88 = *(u8**)((u8*)&lbl_eu_80665788 + lbl_eu_806657A0);
        u8* p8C = *(u8**)((u8*)&lbl_eu_8066578C + lbl_eu_806657A0 * 2);
        u8* src = p8C + *(u16*)(p88 + lbl_eu_806657A0 * 2) * 0x48;
        u32* d = (u32*)&lbl_eu_80658008[0][0][0];
        u32* s = (u32*)src;
        for (u32 i = 0; i < 12; i++) {
            d[i] = s[i];
        }
    } else {
        f32* d = &lbl_eu_80658008[0][0][0];
        const f32* s = (const f32*)arg2;
        for (u32 i = 0; i < 12; i++) {
            d[i] = s[i];
        }
    }

    // Concatenate the fixed view matrix with the shared one and upload both
    // position and normal matrices.
    Mtx mtxA;
    PSMTXConcat(lbl_eu_80657FD8[0], lbl_eu_80658008[0], mtxA);
    GXLoadPosMtxImm(mtxA, 3);
    PSMTXInverse(mtxA, lbl_eu_806583E0[0]);
    Mtx mtxB;
    PSMTXTranspose(lbl_eu_806583E0[0], mtxB);
    GXLoadNrmMtxImm(mtxB, 3);

    // Reassemble the per-record matrices when the object's bit 4 flag is set
    // and the matrix cache is stale (bit 0 of the dirty word) or the cached
    // distance value differs.
    if ((o->field_0x0 & 0x10) != 0) {
        if ((lbl_eu_806657B0 & 1) != 0 ||
            lbl_eu_806657A4 != lbl_eu_80665754) {
            LodRec1* records = (LodRec1*)(lbl_eu_8066576C + o->field_0x28);
            for (u32 i = 0; i < o->field_0x4; i++) {
                LodRec1* rec = &records[i];
                f32* mtx = &lbl_eu_80658048[i][0][0];
                func_804A6C60(mtx, rec);
                if (rec->field_0x4 != 0) {
                    PSMTXConcat(lbl_eu_80658048[rec->field_0x4 - 1],
                                (const f32 (*)[4])mtx, (f32 (*)[4])mtx);
                }
            }
            lbl_eu_806657A4 = lbl_eu_80665754;
        }
    }

    // Record loop: per-record matrix concat + GX matrix upload.
    LodRec1* records = (LodRec1*)(lbl_eu_8066576C + o->field_0x28);
    for (u32 i = 0; i < o->field_0x4; i++) {
        LodRec1* rec = &records[i];
        if ((rec->field_0x0 & 1) != 0) {
            Mtx mtxC;
            PSMTXConcat(lbl_eu_80658008[0], lbl_eu_80658048[i], mtxC);
            PSMTXConcat(mtxC, lbl_eu_80657FD8[0], mtxC);
            GXLoadPosMtxImm(mtxC, ((u8*)rec)[3] + 3);
            PSMTXInverse(mtxC, lbl_eu_806581C8[0]);
            PSMTXTranspose(lbl_eu_806581C8[0], mtxC);
            GXLoadNrmMtxImm(mtxC, ((u8*)rec)[3] + 3);
        }
    }

    // Register the vertex arrays.
    if (o->field_0x10 != 0) {
        GXSetArray(GX_VA_NRM, lbl_eu_8066576C + o->field_0x10,
                   (u8)lbl_eu_806657C8);
    }
    if (o->field_0x18 != 0) {
        GXSetArray(GX_VA_CLR0, lbl_eu_8066576C + o->field_0x18, 4);
    }
    if (o->field_0x14 != 0) {
        for (u32 i = 0; i < o->field_0x2; i++) {
            GXSetArray((GXAttr)(GX_VA_TEX0 + i),
                       lbl_eu_8066576C + o->field_0x14, 8);
        }
    }

    // Dispatch the record's render functions through the byte stream at
    // self + u32[self + idx*4 + 0x1C]; each callee advances the stream.
    lbl_eu_806657B0 &= ~2u;
    u8* p = (u8*)self + *(u32*)((u8*)self + idx * 4 + 0x1c);
    for (u32 i = 0; i < count; i++) {
        lbl_eu_8056D980[*p](p);
    }
}

// ===========================================================================
// us-8046e384  func_8046A3B4  (billboard matrix assembly + submit)
// Resolves the render object for the given type index through the LOD
// per-type pointer tables (stride 0x18), stores it as the current object,
// assembles the per-record matrices in lbl_eu_80658048, then copies the
// caller's view matrix and submits every record flagged with bit 2.
// ===========================================================================
void func_8046A3B4__Q23LOD17UnkClass_80468434Fv(u32 idx, const f32* srcMtx,
                                                u8* arg2) {
    // Resolve the object: pool base + the u32 record table entry at
    // byte offset idx*0x18 of the draw-records pointer.
    u32 off = idx * 0x18;
    u8* p78 = lbl_eu_80665778;
    u8* p68 = lbl_eu_80665768;
    u8* obj = p68 + *(u32*)(p78 + off);
    lbl_eu_8066576C = obj;

    LodObj1* o = (LodObj1*)obj;
    LodRec1* records = (LodRec1*)(obj + o->field_0x28);
    s32 i;
    f32* mtx;
    LodRec1* rec;

    // Loop 1: per-record setup; concatenate the parent matrix when the
    // record references one (parent index is 1-based).
    for (i = 0; i < o->field_0x4; i++) {
        mtx = &lbl_eu_80658048[i][0][0];
        rec = &records[i];
        func_804A6C60(mtx, rec);
        if (rec->field_0x4 != 0) {
            PSMTXConcat(lbl_eu_80658048[rec->field_0x4 - 1],
                        (const f32 (*)[4])mtx, (f32 (*)[4])mtx);
        }
    }

    // Cache the shared LOD distance value, then copy the caller's 3x4 view
    // matrix into a stack buffer (used for the billboard concatenation).
    lbl_eu_806657A4 = lbl_eu_80665754;

    // Loop 2: re-derive the record array from the current-object global and
    // submit the records flagged with bit 2 (view matrix concat + scene
    // submit through the ScnRes wrapper).
    LodRec1* rec2 = (LodRec1*)(lbl_eu_8066576C + o->field_0x28);

    Mtx work;
    f32* dst = &work[0][0];
    // Retail copies the caller matrix with a specific element permutation;
    // the loads are hoisted in reverse statement order into f11..f0.
    dst[11] = srcMtx[2];
    dst[10] = srcMtx[11];
    dst[9] = srcMtx[10];
    dst[8] = srcMtx[9];
    dst[7] = srcMtx[1];
    dst[6] = srcMtx[8];
    dst[5] = srcMtx[7];
    dst[4] = srcMtx[6];
    dst[3] = srcMtx[0];
    dst[2] = srcMtx[5];
    dst[1] = srcMtx[4];
    dst[0] = srcMtx[3];
    for (i = 0; i < o->field_0x4; i++) {
        if ((rec2[i].field_0x0 & 4) != 0) {
            PSMTXConcat(work, lbl_eu_80658048[i], work);
            func_804BCC48(func_804BC9EC__Fv(), arg2, (u8*)work);
        }
    }
}

// ===========================================================================
// us-8046db2c  func_80469B5C  (billboard record draw, +0x48 view)
// Guard chain identical to the cull helpers; on visible, uploads the matrix
// slot + texcoord state and emits the record's billboard quad.  The quad
// corners are the scaled direction normal spread around two base vectors
// (record +0x10 / +0x18 heights) and the position base is record +0x0.
// ===========================================================================
u16* func_80469B5C__Q23LOD17UnkClass_80468434Fv(UnkClass_80468434* self) {
    LodDrawHeader* h = (LodDrawHeader*)self;
    // Integer-sum form keeps (idx * 0x2C) as the add's first operand
    // (retail: add r4, r4, r3; addi r31, r4, 0x48).
    LodDrawRec* rec = (LodDrawRec*)(h->field_0x2 * 0x2c + (u32)self + 0x48);

    // Visibility guard chain (mirrors the cull helpers' checks).
    if ((h->field_0x1 & lbl_eu_8066A5F0) != 0) {
        if ((h->field_0x1 & 2) != 0) {
            if (func_804643D8(h->field_0x8 * 0x3c, h->field_0xA * 0x3c,
                              h->field_0xC * 0x3c, h->field_0xD * 0x3c) != 0) {
                return (u16*)rec;
            }
        } else {
            func_80465718__Q23LOD17UnkClass_804645CCFv();
        }
        if (func_80464128(h->field_0x1) != 0) {
            return (u16*)rec;
        }
        if ((h->field_0x1 & 0x80) != 0) {
            if (func_804642BC(h->field_0x10, h->field_0x12) != 0) {
                return (u16*)rec;
            }
        }
    } else {
        func_80465718__Q23LOD17UnkClass_804645CCFv();
    }

    // Visible: upload the matrix slot and set up colour/texcoord state.
    func_80463C24((s32)h->field_0xE >> 1, h->field_0xF,
                  (const f32 (*)[3][4])&h->field_0x18);
    func_80465314(h->field_0x6);
    func_8046534C__Q23LOD17UnkClass_804645CCFv(h->field_0x4);
    s32 texCount = h->field_0x6 >> 8;
    func_8046406C(texCount);

    u32 count = h->field_0x2;
    f32 zero = lbl_eu_8066A64C;
    f32 one = lbl_eu_8066A648;

    // Direction normals + translation columns of the shared billboard
    // matrix table; normalize (or zero) the transformed X axis.
    nw4r::math::VEC3 dirs[3];
    nw4r::math::VEC3 trans[3];
    nw4r::math::VEC3 in(one, zero, zero);
    for (u32 i = 0; i < 3; i++) {
        nw4r::math::VEC3TransformNormal(
            &dirs[i], (const nw4r::math::MTX34*)lbl_eu_806581C8[i], &in);
        f32 lenSq = dirs[i].x * dirs[i].x + dirs[i].y * dirs[i].y +
                    dirs[i].z * dirs[i].z;
        if (lenSq == zero) {
            dirs[i].x = ml::CVec3::zero.x;
            dirs[i].y = ml::CVec3::zero.y;
            dirs[i].z = ml::CVec3::zero.z;
        } else {
            PSVECNormalize((const Vec*)&dirs[i], (Vec*)&dirs[i]);
        }
        trans[i].x = lbl_eu_806581C8[i][0][3];
        trans[i].y = lbl_eu_806581C8[i][1][3];
        trans[i].z = lbl_eu_806581C8[i][2][3];
    }

    // Record billboard loop (records start at self + 0x48).
    LodDrawRec* recs = (LodDrawRec*)((u8*)self + 0x48);
    for (u32 i = 0; i < count; i++) {
        LodDrawRec* r = &recs[i];
        u8 b = r->field_0x2A;
        func_804683D0__Q23LOD17UnkClass_80466348Fv(b * 3 + 6);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);

        nw4r::math::VEC3 scaled;
        nw4r::math::VEC3Scale(&scaled, &dirs[b], r->field_0xC);

        nw4r::math::VEC3 base1(zero, r->field_0x10, zero);
        nw4r::math::VEC3 base2(zero, r->field_0x18, zero);
        nw4r::math::VEC3 corner[4];
        nw4r::math::VEC3Add(&corner[0], &scaled, &base1);
        nw4r::math::VEC3Add(&corner[1], &base2, &scaled);
        nw4r::math::VEC3Sub(&corner[2], &base2, &scaled);
        nw4r::math::VEC3Sub(&corner[3], &base1, &scaled);

        nw4r::math::VEC3 pos(r->field_0x0, r->field_0x4, r->field_0x8);
        if (r->field_0x14 != zero) {
            // (retail applies a billboard facing adjustment here; the
            // touched stack slots are the corner array and the position)
            corner[0].x += r->field_0x14;
        }

        for (u32 k = 0; k < 4; k++) {
            GXPosition3f32(pos.x + corner[k].x, pos.y + corner[k].y,
                           pos.z + corner[k].z);
            GXColor1u16(r->field_0x1C[k]);
            for (u32 t = 0; t < (u32)texCount; t++) {
                GXTexCoord1x8(r->field_0x24[k]);
            }
        }
    }
    return (u16*)rec;
}

namespace LOD {

// ===========================================================================
// us-8046d5f0  func_80469620  (billboard record draw, dual normal)
// Same guard chain as func_80469B5C; the quad uses two transformed direction
// normals (X and Y axes) so the billboard can orient freely.
// ===========================================================================
u16* func_80469620__Q23LOD17UnkClass_80468434Fv(UnkClass_80468434* self) {
    LodDrawHeader* h = (LodDrawHeader*)self;
    // Integer-sum form keeps (idx * 0x2C) as the add's first operand.
    LodDrawRec* rec = (LodDrawRec*)(h->field_0x2 * 0x2c + (u32)self + 0x48);

    // Visibility guard chain (mirrors the cull helpers' checks).
    if ((h->field_0x1 & lbl_eu_8066A5F0) != 0) {
        if ((h->field_0x1 & 2) != 0) {
            if (func_804643D8(h->field_0x8 * 0x3c, h->field_0xA * 0x3c,
                              h->field_0xC * 0x3c, h->field_0xD * 0x3c) != 0) {
                return (u16*)rec;
            }
        } else {
            func_80465718__Q23LOD17UnkClass_804645CCFv();
        }
        if (func_80464128(h->field_0x1) != 0) {
            return (u16*)rec;
        }
        if ((h->field_0x1 & 0x80) != 0) {
            if (func_804642BC(h->field_0x10, h->field_0x12) != 0) {
                return (u16*)rec;
            }
        }
    } else {
        func_80465718__Q23LOD17UnkClass_804645CCFv();
    }

    // Visible: upload the matrix slot and set up colour/texcoord state.
    func_80463C24((s32)h->field_0xE >> 1, h->field_0xF,
                  (const f32 (*)[3][4])&h->field_0x18);
    func_80465314(h->field_0x6);
    func_8046534C__Q23LOD17UnkClass_804645CCFv(h->field_0x4);
    s32 texCount = h->field_0x6 >> 8;
    func_8046406C(texCount);

    u32 count = h->field_0x2;
    f32 zero = lbl_eu_8066A64C;
    f32 one = lbl_eu_8066A648;

    // X/Y axis direction normals + translation columns of the shared
    // billboard matrix table.
    nw4r::math::VEC3 dirsA[3];
    nw4r::math::VEC3 dirsB[3];
    nw4r::math::VEC3 trans[3];
    nw4r::math::VEC3 inA(one, zero, zero);
    nw4r::math::VEC3 inB(zero, one, zero);
    for (u32 i = 0; i < 3; i++) {
        nw4r::math::VEC3TransformNormal(
            &dirsA[i], (const nw4r::math::MTX34*)lbl_eu_806581C8[i], &inA);
        nw4r::math::VEC3TransformNormal(
            &dirsB[i], (const nw4r::math::MTX34*)lbl_eu_806581C8[i], &inB);
        f32 lenSqA = dirsA[i].x * dirsA[i].x + dirsA[i].y * dirsA[i].y +
                     dirsA[i].z * dirsA[i].z;
        if (lenSqA == zero) {
            dirsA[i].x = ml::CVec3::zero.x;
            dirsA[i].y = ml::CVec3::zero.y;
            dirsA[i].z = ml::CVec3::zero.z;
        } else {
            PSVECNormalize((const Vec*)&dirsA[i], (Vec*)&dirsA[i]);
        }
        f32 lenSqB = dirsB[i].x * dirsB[i].x + dirsB[i].y * dirsB[i].y +
                     dirsB[i].z * dirsB[i].z;
        if (lenSqB == zero) {
            dirsB[i].x = ml::CVec3::zero.x;
            dirsB[i].y = ml::CVec3::zero.y;
            dirsB[i].z = ml::CVec3::zero.z;
        } else {
            PSVECNormalize((const Vec*)&dirsB[i], (Vec*)&dirsB[i]);
        }
        trans[i].x = lbl_eu_806581C8[i][0][3];
        trans[i].y = lbl_eu_806581C8[i][1][3];
        trans[i].z = lbl_eu_806581C8[i][2][3];
    }

    // Record billboard loop (records start at self + 0x48).
    LodDrawRec* recs = (LodDrawRec*)((u8*)self + 0x48);
    for (u32 i = 0; i < count; i++) {
        LodDrawRec* r = &recs[i];
        u8 b = r->field_0x2A;
        func_804683D0__Q23LOD17UnkClass_80466348Fv(b * 3 + 6);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);

        nw4r::math::VEC3 scaledA;
        nw4r::math::VEC3 scaledB;
        nw4r::math::VEC3 scaledC;
        nw4r::math::VEC3Scale(&scaledA, &dirsA[b], r->field_0xC);
        nw4r::math::VEC3Scale(&scaledB, &dirsB[b], r->field_0x10);
        nw4r::math::VEC3Scale(&scaledC, &dirsB[b], r->field_0x18);

        nw4r::math::VEC3 corner[4];
        nw4r::math::VEC3Add(&corner[0], &scaledA, &scaledB);
        nw4r::math::VEC3Sub(&corner[1], &scaledA, &scaledB);
        nw4r::math::VEC3Add(&corner[2], &scaledC, &scaledA);
        nw4r::math::VEC3Sub(&corner[3], &scaledC, &scaledA);

        nw4r::math::VEC3 pos(r->field_0x0, r->field_0x4, r->field_0x8);
        if (r->field_0x14 != zero) {
            corner[0].x += r->field_0x14;
        }

        for (u32 k = 0; k < 4; k++) {
            GXPosition3f32(pos.x + corner[k].x, pos.y + corner[k].y,
                           pos.z + corner[k].z);
            GXColor1u16(r->field_0x1C[k]);
            for (u32 t = 0; t < (u32)texCount; t++) {
                GXTexCoord1x8(r->field_0x24[k]);
            }
        }
    }
    return (u16*)rec;
}

// ===========================================================================
// us-8046d108  func_80469138  (billboard record draw, +0x1C view)
// Guard chain returns the +0x1C record view; the direction normal is the
// transformed X axis of a matrix selected by the u16 at header +0x0E, and
// each record either dispatches through lbl_eu_8056D9B0 (bit 0 of +0x2B) or
// copies its position base directly.
// ===========================================================================
u16* func_80469138__Q23LOD17UnkClass_80468434Fv(UnkClass_80468434* self) {
    LodDrawHeader* h = (LodDrawHeader*)self;
    // Integer-sum form keeps (idx * 0x2C) as the add's first operand.
    LodDrawRec* rec = (LodDrawRec*)(h->field_0x2 * 0x2c + (u32)self + 0x1c);

    // Visibility guard chain (mirrors the cull helpers' checks).
    if ((h->field_0x1 & lbl_eu_8066A5F0) != 0) {
        if ((h->field_0x1 & 2) != 0) {
            if (func_804643D8(h->field_0x8 * 0x3c, h->field_0xA * 0x3c,
                              h->field_0xC * 0x3c, h->field_0xD * 0x3c) != 0) {
                return (u16*)rec;
            }
        } else {
            func_80465718__Q23LOD17UnkClass_804645CCFv();
        }
        if (func_80464128(h->field_0x1) != 0) {
            return (u16*)rec;
        }
        if ((h->field_0x1 & 0x80) != 0) {
            if (func_804642BC(h->field_0x10, h->field_0x12) != 0) {
                return (u16*)rec;
            }
        }
    } else {
        func_80465718__Q23LOD17UnkClass_804645CCFv();
    }

    // Matrix slot + matrix select (u16 at +0x0E indexes lbl_eu_806581C8,
    // zero selects the fixed lbl_eu_806583E0 table).
    u16 e = *(u16*)&h->field_0xE;
    func_804683D0__Q23LOD17UnkClass_80466348Fv((e + 1) * 3);

    const nw4r::math::MTX34* mtx;
    if (e != 0) {
        mtx = (const nw4r::math::MTX34*)lbl_eu_806581C8[e - 1];
    } else {
        mtx = (const nw4r::math::MTX34*)&lbl_eu_806583E0[0][0][0];
    }

    func_80465314(h->field_0x6);
    func_8046534C__Q23LOD17UnkClass_804645CCFv(h->field_0x4);
    s32 texCount = h->field_0x6 >> 8;
    func_8046406C(texCount);

    u32 count = h->field_0x2;
    f32 zero = lbl_eu_8066A64C;
    f32 one = lbl_eu_8066A648;

    // Single direction normal: transform {1,0,0} by the selected matrix and
    // normalize (or zero); keep the translation column.
    nw4r::math::VEC3 normal(one, zero, zero);
    nw4r::math::VEC3 transCol(mtx->_03, mtx->_13, mtx->_23);
    nw4r::math::VEC3TransformNormal(&normal, mtx, &normal);
    f32 lenSq = normal.x * normal.x + normal.y * normal.y +
                normal.z * normal.z;
    if (lenSq == zero) {
        normal.x = ml::CVec3::zero.x;
        normal.y = ml::CVec3::zero.y;
        normal.z = ml::CVec3::zero.z;
    } else {
        PSVECNormalize((const Vec*)&normal, (Vec*)&normal);
    }

    // Record billboard loop (records start at self + 0x1C).
    GXBegin(GX_QUADS, GX_VTXFMT0, (u16)(count * 4));
    LodDrawRec* recs = (LodDrawRec*)((u8*)self + 0x1c);
    for (u32 i = 0; i < count; i++) {
        LodDrawRec* r = &recs[i];

        nw4r::math::VEC3 scaled;
        nw4r::math::VEC3Scale(&scaled, &normal, r->field_0xC);

        nw4r::math::VEC3 base1(zero, r->field_0x10, zero);
        nw4r::math::VEC3 base2(zero, r->field_0x18, zero);
        nw4r::math::VEC3 corner[4];
        nw4r::math::VEC3Add(&corner[0], &scaled, &base1);
        nw4r::math::VEC3Add(&corner[1], &base2, &scaled);
        nw4r::math::VEC3Sub(&corner[2], &base2, &scaled);
        nw4r::math::VEC3Sub(&corner[3], &base1, &scaled);

        nw4r::math::VEC3 pos;
        if ((r->field_0x2B & 1) != 0) {
            lbl_eu_8056D9B0[r->field_0x29](&pos, &corner[1], r, i);
        } else {
            pos.x = r->field_0x0;
            pos.y = r->field_0x4;
            pos.z = r->field_0x8;
        }
        if (r->field_0x14 != zero) {
            corner[0].x += r->field_0x14;
        }

        for (u32 k = 0; k < 4; k++) {
            GXPosition3f32(pos.x + corner[k].x, pos.y + corner[k].y,
                           pos.z + corner[k].z);
            GXColor1u16(r->field_0x1C[k]);
            for (u32 t = 0; t < (u32)texCount; t++) {
                GXTexCoord1x8(r->field_0x24[k]);
            }
        }
    }
    return (u16*)rec;
}

// ===========================================================================
// us-8046dffc  func_8046A02C  (record-A visibility cull)
// When the object's distance-cull flag (u16@0x16 bit 1) is set, the range
// [u16@0x2 * 60, u16@0x4 * 60] is tested against the shared LOD distance
// limit and *out is set on a hit; when the flag is clear the record is always
// visible (*out = 1).  Returns the render-record pointer at this+0x24.
// ===========================================================================
u8* func_8046A02C__Q23LOD17UnkClass_80468434Fv(UnkClass_80468434* self, u8* out) {
    u8* result = &self->records[0].field_0x24;
    if ((self->records[0].field_0x16 & 0x2) != 0) {
        if (func_8046451C(self->records[0].field_0x2 * 0x3c,
                          self->records[0].field_0x4 * 0x3c) != 0) {
            *out = 1;
        }
        return result;
    }
    *out = 1;
    return result;
}

// ===========================================================================
// us-8046e074  func_8046A0A4  (record-B visibility cull)
// Same shape as func_8046A02C; returns the render-record pointer at this+0x58.
// ===========================================================================
u8* func_8046A0A4__Q23LOD17UnkClass_80468434Fv(UnkClass_80468434* self, u8* out) {
    u8* result = &self->records[2].field_0x0;
    if ((self->records[0].field_0x16 & 0x2) != 0) {
        if (func_8046451C(self->records[0].field_0x2 * 0x3c,
                          self->records[0].field_0x4 * 0x3c) != 0) {
            *out = 1;
        }
        return result;
    }
    *out = 1;
    return result;
}

// ===========================================================================
// us-8046e0ec  func_8046A11C  (per-record visibility cull)
// The record index (u16@0x2) selects an entry of the 0x2C-byte record array;
// when the record-level cull flag (u8@0x1 bit 1) is set the range
// [u16@0x8 * 60, u16@0xA * 60] is tested against the LOD distance limit.
// Returns the render-record pointer at record+0x1C.
// ===========================================================================
u16* func_8046A11C__Q23LOD17UnkClass_80468434Fv(UnkClass_80468434* self, u8* out) {
    u16* result = (u16*)((u32)self + (self->records[0].field_0x2 * 0x2c + 0x1c));
    if ((self->records[0].field_0x1 & 0x2) != 0) {
        if (func_8046451C(self->records[0].field_0x8 * 0x3c,
                          self->records[0].field_0xA * 0x3c) != 0) {
            *out = 1;
        }
        return result;
    }
    *out = 1;
    return result;
}

// ===========================================================================
// us-8046e170  func_8046A1A0  (second per-record visibility cull)
// Same shape as func_8046A11C; returns the render-record pointer at
// record+0x48 (the next record's +0x1C).
// ===========================================================================
u16* func_8046A1A0__Q23LOD17UnkClass_80468434Fv(UnkClass_80468434* self, u8* out) {
    // record = this + idx * 0x2C; return &record->field_0x1C + 0x2C (the next
    // record's view).  Parenthesised (scaled + 0x48) unit keeps the scaled
    // value as the add's first operand (retail: add r5, r5, r3).
    u16* result = (u16*)((u32)self + (self->records[0].field_0x2 * 0x2c + 0x48));
    if ((self->records[0].field_0x1 & 0x2) != 0) {
        if (func_8046451C(self->records[0].field_0x8 * 0x3c,
                          self->records[0].field_0xA * 0x3c) != 0) {
            *out = 1;
        }
        return result;
    }
    *out = 1;
    return result;
}

// ===========================================================================
// us-8046e1f4  func_8046A224  (spawn-position copy)
// Retail is a nofralloc paired-single kernel: it loads the spawn bias/table/
// spread globals, computes a dead dir*scale value, then stores in.x -> out.x
// and in.z -> out.z (the earlier psq_st results at out+0/out+8 are
// overwritten).  ps_cmpo0 is not expressible in high-level C++, so the body
// ships as an isolated PS backend (PLAN 17.6) with a scalar fallback; see
// code_80468434_ps.inl.
// ===========================================================================
#include "monolib/lod/code_80468434_ps.inl"

// ===========================================================================
// us-8046e250  func_8046A280  (spawn-position transform + copy)
// Transforms vec[0] and vec[3] in place by the matrix stored at +0x174 of the
// matrix record selected by bit 2 of (idx + bias), then copies src into self.
// ===========================================================================
void func_8046A280__Q23LOD17UnkClass_80468434Fv(Vec* self, Vec* vec,
                                                const Vec* src, s32 idx) {
    u8* record = (u8*)lbl_eu_806657AC + ((idx + lbl_eu_806657A0) & 4) * 0xb4;
    nw4r::math::VEC3TransformNormal((nw4r::math::VEC3*)&vec[0],
                                    (nw4r::math::MTX34*)(record + 0x174),
                                    (nw4r::math::VEC3*)&vec[0]);
    nw4r::math::VEC3TransformNormal((nw4r::math::VEC3*)&vec[3],
                                    (nw4r::math::MTX34*)(record + 0x174),
                                    (nw4r::math::VEC3*)&vec[3]);
    float x, y, z;
    z = src->z;
    y = src->y;
    x = src->x;
    self->x = x;
    self->y = y;
    self->z = z;
}

// ===========================================================================
// us-8046e2e8  func_8046A318  (spawn-position transform + copy, record +8)
// Same shape as func_8046A280 but transforms vec[1]/vec[2] using matrix record
// ((idx + bias) & 4) + 8.
// ===========================================================================
void func_8046A318__Q23LOD17UnkClass_80468434Fv(Vec* self, Vec* vec,
                                                const Vec* src, s32 idx) {
    u8* record = (u8*)lbl_eu_806657AC + (((idx + lbl_eu_806657A0) & 4) + 8) * 0xb4;
    nw4r::math::VEC3TransformNormal((nw4r::math::VEC3*)&vec[1],
                                    (nw4r::math::MTX34*)(record + 0x174),
                                    (nw4r::math::VEC3*)&vec[1]);
    nw4r::math::VEC3TransformNormal((nw4r::math::VEC3*)&vec[2],
                                    (nw4r::math::MTX34*)(record + 0x174),
                                    (nw4r::math::VEC3*)&vec[2]);
    float x, y, z;
    z = src->z;
    y = src->y;
    x = src->x;
    self->x = x;
    self->y = y;
    self->z = z;
}

// ===========================================================================
// us-8046c8fc  func_8046892C  (record dispatch loop)
// Stores the object pointer, reads the count at u16[self + idx*2 + 6], then
// walks the byte stream at self + u32[self + idx*4 + 0x1C]: each byte selects
// a render function from lbl_eu_8056D998 which is called with (p, &flag) and
// whose return value becomes the next pointer.  Returns the flag byte.
// ===========================================================================
s32 func_8046892C__Q23LOD17UnkClass_80468434Fv(u8* self) {
    lbl_eu_8066576C = self;
    s32 n = *(u16*)(self + lbl_eu_80665794 * 2 + 6);
    if (n != 0) {
        u8 flag = 0;
        u8* p = self + *(u32*)(self + lbl_eu_80665794 * 4 + 0x1c);
        for (s32 i = 0; i < n; i++) {
            p = (u8*)lbl_eu_8056D998[*p](p, &flag);
        }
        return flag;
    }
    return 0;
}

// ===========================================================================
// us-8046c9a4  func_804689D4  (per-object visibility + render)
// When the visibility mask (lbl_eu_8066A5F0) has bit 1 of the u16 flag set,
// the distance/fade helpers gate an early return of self+0x24; otherwise the
// record's display list is submitted.  Bit 7 gates a group fade check.
// ===========================================================================
u8* func_804689D4__Q23LOD17UnkClass_80468434Fv(UnkClass_80468434* self) {
    u8* result = &self->records[0].field_0x24;
    if ((self->records[0].field_0x16 & lbl_eu_8066A5F0) != 0) {
        if ((self->records[0].field_0x16 & 2) != 0) {
            if (func_804643D8(self->records[0].field_0x2 * 0x3c,
                              self->records[0].field_0x4 * 0x3c,
                              self->records[0].field_0x6 * 0x3c,
                              self->records[0].field_0x7 * 0x3c) != 0) {
                return result;
            }
        } else {
            func_80465718__Q23LOD17UnkClass_804645CCFv();
        }
        if (func_80464128(self->records[0].field_0x16) != 0) {
            return result;
        }
        if ((self->records[0].field_0x16 & 0x80) != 0) {
            if (func_804642BC(*(u16*)&self->records[0].field_0x18,
                              self->records[0].field_0x1A) != 0) {
                return result;
            }
        }
    } else {
        func_80465718__Q23LOD17UnkClass_804645CCFv();
    }
    func_80465314(self->records[0].field_0x8);
    func_8046534C__Q23LOD17UnkClass_804645CCFv(self->records[0].field_0xA);
    func_804683D0__Q23LOD17UnkClass_80466348Fv(self->records[0].field_0x14 + 3);
    GXCallDisplayList(lbl_eu_8066576C + self->records[0].field_0xC,
                      self->records[0].field_0x10);
    return result;
}

// ===========================================================================
// us-8046caa0  func_80468AD0  (per-object visibility + render, record 2)
// Same guard chain as func_804689D4 but returns self+0x58, uses the fade
// group ids at records[1].field_0x20/0x22, uploads the matrix at self+0x1C
// from bytes at self+0x18/0x19, and resets the cached-matrix sentinel
// (lbl_eu_80665818) to -1 after submitting the display list.
// ===========================================================================
u8* func_80468AD0__Q23LOD17UnkClass_80468434Fv(UnkClass_80468434* self) {
    u8* result = &self->records[2].field_0x0;
    if ((self->records[0].field_0x16 & lbl_eu_8066A5F0) != 0) {
        if ((self->records[0].field_0x16 & 2) != 0) {
            if (func_804643D8(self->records[0].field_0x2 * 0x3c,
                              self->records[0].field_0x4 * 0x3c,
                              self->records[0].field_0x6 * 0x3c,
                              self->records[0].field_0x7 * 0x3c) != 0) {
                return result;
            }
        } else {
            func_80465718__Q23LOD17UnkClass_804645CCFv();
        }
        if (func_80464128(self->records[0].field_0x16) != 0) {
            return result;
        }
        if ((self->records[0].field_0x16 & 0x80) != 0) {
            if (func_804642BC(self->records[1].field_0x20,
                              self->records[1].field_0x22) != 0) {
                return result;
            }
        }
    } else {
        func_80465718__Q23LOD17UnkClass_804645CCFv();
    }
    func_80463C24((s32)self->records[0].field_0x18 >> 1,
                  self->records[0].field_0x19,
                  (const f32 (*)[3][4])&self->records[0].field_0x1C);
    func_80465314(self->records[0].field_0x8);
    func_8046534C__Q23LOD17UnkClass_804645CCFv(self->records[0].field_0xA);
    GXCallDisplayList(lbl_eu_8066576C + self->records[0].field_0xC,
                      self->records[0].field_0x10);
    lbl_eu_80665818 = (u32)-1;
    return result;
}

} // namespace LOD

// ===== Dissolved monolibdata2 data owned by this TU =====
// [.data] 0x8056D980-0x8056DAF8 (376B): a 16-word record-draw dispatch table
// (15 retail function pointers + 1 zero word) followed by the assertion-string
// block.  Pointer words are emitted as relocs (retail carries them the same
// way -> zero bytes + matching reloc set); strings are mutable char arrays so
// MWCC places them in .data (retail keeps them in .data, not .rodata).
// __declspec(align(8)) matches the retail .data section alignment.
namespace Blob34 {
extern "C" {
void func_804689D4__Q23LOD17UnkClass_80468434Fv();
void func_80468BDC__Q23LOD17UnkClass_80468434Fv();
void func_80469138__Q23LOD17UnkClass_80468434Fv();
void func_80468AD0__Q23LOD17UnkClass_80468434Fv();
void func_80469620__Q23LOD17UnkClass_80468434Fv();
void func_80469B5C__Q23LOD17UnkClass_80468434Fv();
void func_8046A02C__Q23LOD17UnkClass_80468434Fv();
void func_8046A11C__Q23LOD17UnkClass_80468434Fv();
void func_8046A0A4__Q23LOD17UnkClass_80468434Fv();
void func_8046A1A0__Q23LOD17UnkClass_80468434Fv();
void func_8046A224__Q23LOD17UnkClass_80468434Fv();
void func_8046A280__Q23LOD17UnkClass_80468434Fv();
void func_8046A318__Q23LOD17UnkClass_80468434Fv();
}
}
extern "C" __declspec(align(8)) u32 lbl_eu_8056D980[16] = {
    (u32)&Blob34::func_804689D4__Q23LOD17UnkClass_80468434Fv,
    (u32)&Blob34::func_80468BDC__Q23LOD17UnkClass_80468434Fv,
    (u32)&Blob34::func_80469138__Q23LOD17UnkClass_80468434Fv,
    (u32)&Blob34::func_80468AD0__Q23LOD17UnkClass_80468434Fv,
    (u32)&Blob34::func_80469620__Q23LOD17UnkClass_80468434Fv,
    (u32)&Blob34::func_80469B5C__Q23LOD17UnkClass_80468434Fv,
    (u32)&Blob34::func_8046A02C__Q23LOD17UnkClass_80468434Fv,
    (u32)&Blob34::func_8046A11C__Q23LOD17UnkClass_80468434Fv,
    (u32)&Blob34::func_8046A11C__Q23LOD17UnkClass_80468434Fv,
    (u32)&Blob34::func_8046A0A4__Q23LOD17UnkClass_80468434Fv,
    (u32)&Blob34::func_8046A1A0__Q23LOD17UnkClass_80468434Fv,
    (u32)&Blob34::func_8046A1A0__Q23LOD17UnkClass_80468434Fv,
    (u32)&Blob34::func_8046A224__Q23LOD17UnkClass_80468434Fv,
    (u32)&Blob34::func_8046A280__Q23LOD17UnkClass_80468434Fv,
    (u32)&Blob34::func_8046A318__Q23LOD17UnkClass_80468434Fv,
    0x00000000,
};

extern "C" char lbl_eu_8056D9C0[0x18] = {
    0x52,0x65,0x73,0x54,0x65,0x78,0x50,0x6C,0x74,0x74,0x49,0x6E,0x66,0x6F,0x4F,0x66,
    0x66,0x73,0x65,0x74,0x00,0x00,0x00,0x00,
};
extern "C" char lbl_eu_8056D9D8[0x20] = {
    0x25,0x73,0x3A,0x3A,0x25,0x73,0x3A,0x20,0x4F,0x62,0x6A,0x65,0x63,0x74,0x20,0x6E,
    0x6F,0x74,0x20,0x76,0x61,0x6C,0x69,0x64,0x2E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
extern "C" char lbl_eu_8056D9F8[0x18] = {
    0x67,0x33,0x64,0x5F,0x72,0x65,0x73,0x74,0x65,0x78,0x70,0x6C,0x74,0x74,0x69,0x6E,
    0x66,0x6F,0x5F,0x61,0x63,0x2E,0x68,0x00,
};
extern "C" char lbl_eu_8056DA10[0x20] = {
    0x25,0x73,0x3A,0x3A,0x25,0x73,0x3A,0x20,0x4F,0x62,0x6A,0x65,0x63,0x74,0x20,0x6E,
    0x6F,0x74,0x20,0x76,0x61,0x6C,0x69,0x64,0x2E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
extern "C" char lbl_eu_8056DA30[0x18] = {
    0x67,0x33,0x64,0x5F,0x72,0x65,0x73,0x74,0x65,0x78,0x70,0x6C,0x74,0x74,0x69,0x6E,
    0x66,0x6F,0x5F,0x61,0x63,0x2E,0x68,0x00,
};
extern "C" char lbl_eu_8056DA48[0x30] = {
    0x4E,0x57,0x34,0x52,0x3A,0x46,0x61,0x69,0x6C,0x65,0x64,0x20,0x61,0x73,0x73,0x65,
    0x72,0x74,0x69,0x6F,0x6E,0x20,0x69,0x64,0x78,0x20,0x3C,0x20,0x47,0x65,0x74,0x4E,
    0x75,0x6D,0x4F,0x66,0x66,0x73,0x65,0x74,0x28,0x29,0x00,0x00,0x00,0x00,0x00,0x00,
};
extern "C" char lbl_eu_8056DA78[0x18] = {
    0x67,0x33,0x64,0x5F,0x72,0x65,0x73,0x74,0x65,0x78,0x70,0x6C,0x74,0x74,0x69,0x6E,
    0x66,0x6F,0x5F,0x61,0x63,0x2E,0x68,0x00,
};
extern "C" char lbl_eu_8056DA90[0x28] = {
    0x4E,0x57,0x34,0x52,0x3A,0x46,0x61,0x69,0x6C,0x65,0x64,0x20,0x61,0x73,0x73,0x65,
    0x72,0x74,0x69,0x6F,0x6E,0x20,0x21,0x28,0x28,0x75,0x33,0x32,0x29,0x70,0x20,0x26,
    0x20,0x30,0x78,0x33,0x29,0x00,0x00,0x00,
};
extern "C" char lbl_eu_8056DAB8[0x10] = {
    0x67,0x33,0x64,0x5F,0x72,0x65,0x73,0x6D,0x61,0x74,0x5F,0x61,0x63,0x2E,0x68,0x00,
};
extern "C" char lbl_eu_8056DAC8[0x10] = {
    0x52,0x65,0x73,0x54,0x65,0x78,0x50,0x6C,0x74,0x74,0x49,0x6E,0x66,0x6F,0x00,0x00,
};
extern "C" char lbl_eu_8056DAD8[0x20] = {
    0x25,0x73,0x3A,0x3A,0x25,0x73,0x3A,0x20,0x4F,0x62,0x6A,0x65,0x63,0x74,0x20,0x6E,
    0x6F,0x74,0x20,0x76,0x61,0x6C,0x69,0x64,0x2E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

// [.bss] 0x80657FD8-0x80658458 (0x480, align 8): work matrices + record tables.
extern "C" {
__declspec(align(8)) f32 lbl_eu_80657FD8[3][4];
f32 lbl_eu_80658008[3][4];
u32 lbl_eu_80658038[4];
f32 lbl_eu_80658048[8][3][4];
f32 lbl_eu_806581C8[8][3][4];
u32 lbl_eu_80658348[4];
u32 lbl_eu_80658358[4];
u32 lbl_eu_80658368[3];
u32 lbl_eu_80658374[3];
f32 lbl_eu_80658380[3][4];
f32 lbl_eu_806583B0[3][4];
f32 lbl_eu_806583E0[3][4];
u32 lbl_eu_80658410[3];
u32 lbl_eu_8065841C[3];
f32 lbl_eu_80658428[3][4];
}

// [.sbss] 0x8066576C-0x80665828 (0xBC, align 4): scalar/pointer state.
extern "C" {
__declspec(align(4)) u8*  lbl_eu_8066576C;
u32  lbl_eu_80665770;
u32  lbl_eu_80665774;
u8*  lbl_eu_80665778;
u32  lbl_eu_8066577C;
u32  lbl_eu_80665780;
u32  lbl_eu_80665784;
u32  lbl_eu_80665788;
u32  lbl_eu_8066578C;
u32  lbl_eu_80665790;
s32  lbl_eu_80665794;
u32  lbl_eu_80665798;
u32  lbl_eu_8066579C;
s32  lbl_eu_806657A0;
f32  lbl_eu_806657A4;
u32  lbl_eu_806657A8;
LodDirEntry* lbl_eu_806657AC;
u32  lbl_eu_806657B0;
u32  lbl_eu_806657B4;
u32  lbl_eu_806657B8;
u32  lbl_eu_806657BC;
u32  lbl_eu_806657C0;
u32  lbl_eu_806657C4;
s16  lbl_eu_806657C8;
u16  lbl_eu_806657C8_hi;
u32  lbl_eu_806657C8_x;
s32  lbl_eu_806657D0;
u32  lbl_eu_806657D4;
u32  lbl_eu_806657D8;
u32  lbl_eu_806657DC;
u32  lbl_eu_806657E0;
f32  lbl_eu_806657E4;
u32  lbl_eu_806657E8;
u32  lbl_eu_806657EC;
u32  lbl_eu_806657F0;
u32  lbl_eu_806657F4;
u32  lbl_eu_806657F8;
u32  lbl_eu_806657FC;
f32  lbl_eu_80665800;
u32  lbl_eu_80665804;
f32  lbl_eu_80665808;
u32  lbl_eu_8066580C;
u32  lbl_eu_80665810;
u32  lbl_eu_80665814;
u32  lbl_eu_80665818;
u32  lbl_eu_8066581C;
u32  lbl_eu_80665820;
u32  lbl_eu_80665824;
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" void sinit_8046A52C() {}
