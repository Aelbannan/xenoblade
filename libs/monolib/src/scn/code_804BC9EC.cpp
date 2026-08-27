// Auto-scaffolded catalog TU for monolib/src/scn/code_804BC9EC
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <monolib/scn/code_804BC9EC.hpp>
#include <revolution/GX.h>
#include <revolution/MTX.h>

// Entry in the top-level chunk list (8 bytes each)
struct ScnResEntry {
    u16 type;
    u16 field_0x2;
    u32 offset;  // relative to data base
};

// Extended ScnResData with named fields for offsets 0x00-0x60.
// Layout-compatible with ScnResData from the header.
struct ScnResDataEx {
    u8* field_0x00;  // set by func_804BC9B4 (base + src[1])
    u8* field_0x04;  // set by func_804BC9B4 (base + src[2])
    u8* field_0x08;  // set by func_804BC9B4 (base + src[3])
    u8 field_0x0C[0x14 - 0x0C];
    u8* field_0x14;  // base pointer to parsed data
    u8* field_0x18;  // entry type 1
    u8* field_0x1C;  // entry type 3
    u8* field_0x20;  // entry type 2
    u8* field_0x24;  // entry type 5
    u8* field_0x28;  // entry type 4
    u8* field_0x2C;  // entry type 0: data + 0x20
    u8* field_0x30;  // entry type 0: data + 0x20 (same as 0x2C)
    u8* field_0x34;  // entry type 0: data pointer
    u8* field_0x38;  // entry type 6
    u8* field_0x3C;  // entry type 7
    u8* field_0x40;  // entry type 8
    u8 field_0x44[0x48 - 0x44];
    u8* field_0x48;  // entry type 9
    u8* field_0x4C;  // entry type 12
    u8 field_0x50[0x60 - 0x50];
    u8* field_0x60;  // array of 0xE0-byte elements
    u8 field_0x64[0x6C - 0x64];
    u32 field_0x6C;    // entry type 0: u16 count
    u32 field_0x70;    // entry type 7: u16 value
    u8 field_0x74[0x78 - 0x74];
    int count;         // 0x78
};


// Additional extern function declarations (retail symbols are plain C
// names - no C++ mangling suffix on the retail reloc targets).
extern "C" void func_804BC9B4(int* dest, int baseOffset, int* src);
extern "C" void func_804B74F0(ScnResData* res, u8* data);
extern "C" void func_804B7540(ScnResData* res, u8* data, u16 param);
u32* func_804B5A70(void);

// Vertex: 12 bytes (3 floats: x, y, z)
struct SceneVertex {
    f32 x, y, z;
};

// Triangle element: 20 bytes (0x14)
struct SceneTriElem {
    u16 field_0x00;
    u16 field_0x02;
    u16 indexA;   // 0x04
    u16 indexB;   // 0x06
    u16 indexC;   // 0x08
    u8 colorIdx;  // 0x0C
    u8 field_0x0D;
    u8 field_0x0E;
    u8 field_0x0F;
    u16 field_0x10;
    u16 flags;    // 0x12: bit 17 = red?, bit 22 = green?, bit 18 = blue?
};

// Type 0x0B sub-element: 4 bytes
struct SceneVisElem {
    u16 field_0x00;
    u16 field_0x02;  // visibility bitfield index
};

// Type 0x13 element: 0x3C bytes (bounding sphere + transform)
struct SceneCullElem {
    u16 index;       // 0x00: index into geometry data
    u16 visIdx;      // 0x02: visibility bitfield index
    u16 field_0x04;
    u16 field_0x06;
    u16 field_0x08;
    u16 field_0x0A;
    f32 sphereX;     // 0x0C
    f32 sphereY;     // 0x10
    f32 sphereZ;     // 0x14
    f32 sphereR;     // 0x18
    f32 field_0x1C;
    f32 field_0x20;
    f32 field_0x24;
    f32 field_0x28;
    f32 field_0x2C;
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;
};

// Float constants from SDA2
// extern const f32 lbl_eu_8066AF10;
extern const f32 lbl_eu_80663AD0;  // 50.0f (pass-A cull margin)
extern const f32 lbl_eu_80663AD4;  // 20.0f (pass-B/C cull margin) // 1/3
// extern const f32 lbl_eu_8066AF14; // 0.0f
// extern const f32 lbl_eu_8066AF18; // 255.0f

// nw4r math functions
float FrSqrt__Q24nw4r4mathFf(float);
void Warning__Q24nw4r2dbFPCciPCce(const char*, int, const char*, ...);

// String constants for warning
extern const char lbl_eu_80526324[];
extern const char lbl_eu_80526300[];

extern u32 lbl_eu_80665968[2];
extern u8 lbl_eu_8065F418[0x10];

// Shared float pools referenced by the render path (retail loads them via
// lfs @sda21; local literals would grow .sdata2 past its 0-byte slice):
// lbl_eu_8066AF10 = 1/3, lbl_eu_8066AF14 = 0.0f, lbl_eu_8066AF18 = 255.0f.
extern const f32 lbl_eu_8066AF10;
extern const f32 lbl_eu_80663AD0;  // 50.0f (pass-A cull margin)
extern const f32 lbl_eu_80663AD4;  // 20.0f (pass-B/C cull margin)
extern const f32 lbl_eu_8066AF14;
extern const f32 lbl_eu_8066AF18;

// Dissolved monolibdata2 .data tail owned by this TU (0x8056F940-0x8056F9B2).
extern "C" u32 lbl_eu_80663AE0[2];
extern "C" u32 lbl_eu_80663AF8[2];
extern "C" u32 lbl_eu_80663B00[2];
extern "C" void __dt__6CLightFv(void* self, int flag);
extern "C" void __dt__reslist_IScnEnvCtl(void* self, int flag);
extern "C" void __dt___reslist_base_IScnEnvCtl(void* self, int flag);

int getScnHandle__Fv(u8* self) { return (int)(intptr_t)&lbl_eu_80665968; }

int func_804BC9F4(void* outStruct, u32 data) {
    *(u32*)outStruct = 0;
    func_804B7804(&lbl_eu_8065F32C);

    u8* base = (u8*)data;
    if (base != NULL) {
        // Optional "WP" wrapper chunk (single 32-bit tag compare, split hi/lo by MWCC):
        // skip its 12-byte header to reach the inner "ID" chunk.
        u32 tag = *(u32*)base;
        if (tag - 0x57500000 == 0x4F49) {
            base += *(u32*)(base + 8);
        }

        tag = *(u32*)base;
        // "ID" chunk tag and format version 0x3EA
        if (tag - 0x49440000 == 0x4445 && *(u16*)(base + 4) == 0x3EA) {
            ScnResDataEx* res = (ScnResDataEx*)&lbl_eu_8065F32C;
            res->field_0x14 = base;

            u16 entryCount = *(u16*)(base + 6);
            u8* entryList = base + 8;

            // Two induction variables (entry index + byte offset), matching
            // retail's addi/addi loop-bottom advance. Entry pointers are
            // re-derived inside each case body (retail emits one add per
            // body rather than hoisting a shared entry pointer).
            int i;
            u32 off;
            for (i = 0, off = 0; i < entryCount; i++, off += 8) {
                // Dispatch over chunk types 0x00-0x13 (retail jumptable is a
                // dense 20-entry table with holes falling to the loop bottom).
                switch (*(u16*)(entryList + off)) {
                case 1: {
                    u8* dataPtr = base + *(u32*)(entryList + off + 4);
                    res->field_0x2C = dataPtr + 0x20;
                    res->field_0x30 = dataPtr + 0x20;
                    res->field_0x6C = *(u16*)(entryList + off + 2);
                    res->field_0x34 = dataPtr;
                    break;
                }
                case 5:
                    res->field_0x18 = base + *(u32*)(entryList + off + 4);
                    break;
                case 6:
                    res->field_0x20 = base + *(u32*)(entryList + off + 4);
                    break;
                case 7:
                    res->field_0x1C = base + *(u32*)(entryList + off + 4);
                    break;
                case 8:
                    res->field_0x28 = base + *(u32*)(entryList + off + 4);
                    break;
                case 0x10:
                    res->field_0x24 = base + *(u32*)(entryList + off + 4);
                    break;
                case 2:
                    res->field_0x38 = base + *(u32*)(entryList + off + 4);
                    break;
                case 0xB:
                    res->field_0x3C = base + *(u32*)(entryList + off + 4);
                    res->field_0x70 = *(u16*)(entryList + off + 2);
                    break;
                case 0xC:
                    res->field_0x40 = base + *(u32*)(entryList + off + 4);
                    break;
                case 0xD:
                    res->field_0x48 = base + *(u32*)(entryList + off + 4);
                    break;
                case 0xA:
                    func_804BC9B4((int*)&lbl_eu_8065F32C, (int)base, (int*)(base + *(u32*)(entryList + off + 4)));
                    break;
                case 0x11:
                    func_804B74F0(&lbl_eu_8065F32C, base + *(u32*)(entryList + off + 4));
                    break;
                case 0x12:
                    res->field_0x4C = base + *(u32*)(entryList + off + 4);
                    break;
                case 0x13:
                    func_804B7540(&lbl_eu_8065F32C, base + *(u32*)(entryList + off + 4), *(u16*)(entryList + off + 2));
                    break;
                }
            }

            return 1;
        }
    }

    return 0;
}

// tail calls with &lbl_eu_8065F32C (retail: lis;addi;b callee); callees are
// extern "C" in monolib/scn/code_804BC9EC.hpp
extern "C" void func_804BCC10() { func_804BC9A0(&lbl_eu_8065F32C); }

void resetScnData(void* p) {
    *(u32*)p = 0;
    func_804B7804(&lbl_eu_8065F32C);
}

extern "C" void func_804BCC30(void* unused, int a) { func_804B7D9C((int)(uintptr_t)&lbl_eu_8065F32C, a); }

extern "C" void func_804BCC3C() { func_804B7DD4(&lbl_eu_8065F32C); }

extern "C" void func_804B7E0C(ScnResData* res);
extern "C" void func_804BCC48() { func_804B7E0C(&lbl_eu_8065F32C); }

extern "C" void func_804BCC54(void* unused, int a) { func_804B80A4(&lbl_eu_8065F32C, a); }

extern "C" void func_804BCC60(void* unused, int a) { func_804B8078(&lbl_eu_8065F32C, a); }

void func_804BCC6C() {
    func_804B80CC(&lbl_eu_8065F32C);
}

// Retail saves/restores GPRs via _savegpr_14/_restgpr_14 helpers here while
// func_804BC9F4 uses inline stmw/lmw - reproduce with a per-function pragma.
#pragma use_lmw_stmw off
void func_804BCC78(Mtx modelMtx, u8* viewData, u8* renderParams) {
    // Retail structure (fully decoded from build/us/asm reference; see
    // attempts.jsonl "func_804BCC78 ANALYZED"):
    //  - viewData (r4): view matrix, 12 f32 (cam pos at [3],[7],[11])
    //  - renderParams (r5): Mtx44 projection, 16 f32
    ScnResDataEx* res = (ScnResDataEx*)&lbl_eu_8065F32C;
    u8* base = res->field_0x14;
    if (base == NULL) {
        return;
    }

    u32* projSrc = (u32*)renderParams;
    u32* viewSrc = (u32*)viewData;
    Mtx44 proj;
    Mtx mv;
    for (int i = 0; i < 16; i++) {
        ((u32*)proj)[i] = projSrc[i];
    }
    for (int i = 0; i < 12; i++) {
        ((u32*)mv)[i] = viewSrc[i];
    }
    f32* vf = (f32*)viewData;
#define CAMX vf[3]
#define CAMY vf[7]
#define CAMZ vf[11]

    PSMTXInverse(mv, mv);
    GXSetProjection(proj, GX_PERSPECTIVE);
    GXLoadPosMtxImm(mv, GX_PNMTX0);

    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetZMode(GX_TRUE, GX_LESS, GX_TRUE);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT0, GX_DF_NONE, GX_AF_SPOT);
    GXSetCullMode(GX_CULL_NONE);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ONE, GX_CC_RASC, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGB, GX_RGB8, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetAlphaCompare(GX_ALWAYS, 0xFF, GX_AOP_AND, GX_GREATER, 0xFF);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);

    u16 entryCount = *(u16*)(base + 6);
    ScnResEntry* entries = (ScnResEntry*)(base + 8);
    SceneVertex* vertBase = (SceneVertex*)res->field_0x18;

    // Scan: type 0x10 -> color palette, type 8 -> per-triangle flag words.
    u8* colorData = NULL;
    u32* flagTable = NULL;
    for (int i = 0; i < entryCount; i++) {
        ScnResEntry* e = &entries[i];
        if (e->type == 0x10) {
            colorData = base + e->offset;
        } else if (e->type == 8) {
            flagTable = (u32*)(base + e->offset);
        }
    }

    // Pass A: type-1 entries - triangle soup with flag-word visibility and a
    // (radius + 50) camera-distance cull.
    for (int i = 0; i < entryCount; i++) {
        ScnResEntry* e = &entries[i];
        if (e->type != 1) continue;

        u16 triCount = e->field_0x2;
        SceneTriElem* tris = (SceneTriElem*)(base + e->offset + 0x20);

        for (int j = 0; j < triCount; j++) {
            SceneTriElem* tri = &tris[j];
            SceneVertex* va = &vertBase[tri->indexA];
            SceneVertex* vb = &vertBase[tri->indexB];
            SceneVertex* vc = &vertBase[tri->indexC];

            f32 vax = va->x, vay = va->y, vaz = va->z;
            f32 vbx = vb->x, vby = vb->y, vbz = vb->z;
            f32 vcx = vc->x, vcy = vc->y, vcz = vc->z;

            Vec cen;
            cen.x = (vax + vbx + vcx) * lbl_eu_8066AF10;
            cen.y = (vay + vby + vcy) * lbl_eu_8066AF10;
            cen.z = (vaz + vbz + vcz) * lbl_eu_8066AF10;

            f32 dx0 = cen.x - vax, dy0 = cen.y - vay, dz0 = cen.z - vaz;
            f32 radiusSq = dx0 * dx0 + dy0 * dy0 + dz0 * dz0;

            f32 dx1 = cen.x - vbx, dy1 = cen.y - vby, dz1 = cen.z - vbz;
            f32 t2 = dx1 * dx1 + dy1 * dy1 + dz1 * dz1;
            if (t2 > radiusSq) radiusSq = t2;

            f32 dx2 = cen.x - vcx, dy2 = cen.y - vcy, dz2 = cen.z - vcz;
            t2 = dx2 * dx2 + dy2 * dy2 + dz2 * dz2;
            if (t2 > radiusSq) radiusSq = t2;

            if (!(radiusSq > lbl_eu_8066AF14 || radiusSq == lbl_eu_8066AF14)) {
                Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
            }
            f32 radius;
            if (radiusSq == lbl_eu_8066AF14 || radiusSq < lbl_eu_8066AF14) {
                radius = lbl_eu_8066AF14;
            } else {
                radius = radiusSq * FrSqrt__Q24nw4r4mathFf(radiusSq);
            }

            f32 dcamX = CAMX - cen.x, dcamY = CAMY - cen.y, dcamZ = CAMZ - cen.z;
            f32 dist2 = dcamX * dcamX + dcamY * dcamY + dcamZ * dcamZ;
            f32 thresh = radius + lbl_eu_80663AD0;  // 50.0f pool constant
            thresh = thresh * thresh;

            u32 flags = flagTable[tri->flags];
            if (dist2 > thresh && !(flags & 0x4000)) {
                continue;
            }

            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 3);
            for (int k = 0; k < 3; k++) {
                SceneVertex* v = (k == 0) ? va : ((k == 1) ? vb : vc);
                f32* vf = &v->x;
                GXPosition3f32(vf[0], vf[1], vf[2]);
                if (flags & 0x4000) {
                    GXColor3u8(0xFF, 0, 0);
                } else if (flags & 0x200) {
                    GXColor3u8(0, 0xFF, 0);
                } else if (flags & 0x2000) {
                    GXColor3u8(0, 0, 0xFF);
                } else if (colorData != NULL) {
                    f32* colors = (f32*)(colorData + tri->colorIdx * 0xC);
                    GXColor3u8((u8)(colors[0] * lbl_eu_8066AF18), (u8)(colors[1] * lbl_eu_8066AF18), (u8)(colors[2] * lbl_eu_8066AF18));
                }
            }
        }
    }

    // Scan B: type 2 -> geometry entries, type 0x0B -> u16 selector list,
    // type 0x0C -> 0x80-byte cull records.
    u8* geomBase = NULL;
    u16* visList = NULL;
    u16 visCount = 0;
    u8* cullBase = NULL;
    for (int i = 0; i < entryCount; i++) {
        ScnResEntry* e = &entries[i];
        if (e->type == 2) {
            geomBase = base + e->offset;
        } else if (e->type == 0x0B) {
            visCount = e->field_0x2;
            visList = (u16*)(base + e->offset);
        } else if (e->type == 0x0C) {
            cullBase = base + e->offset;
        }
    }

    // Pass B: per selected cull record - visibility-bit test, expanding
    // (20 + |record vector|) camera-distance cull, per-record matrix concat,
    // then triangle draw with flag-word colors.
    for (int i = 0; i < visCount; i++) {
        u8* rec = cullBase + visList[i] * 0x80;
        u16 visHdr = *(u16*)(rec + 2);
        u32* visBits = func_804B5A70();
        if (visBits[(visHdr >> 3) & 0x1FFC] & (1 << (visHdr & 0x1F))) {
            continue;
        }

        f32 dx = CAMX - *(f32*)(rec + 0x10);
        f32 dy = CAMY - *(f32*)(rec + 0x20);
        f32 dz = CAMZ - *(f32*)(rec + 0x30);
        f32 dist2 = dx * dx + dy * dy + dz * dz;
        Vec magIn;
        magIn.x = *(f32*)(rec + 0x64);
        magIn.y = *(f32*)(rec + 0x68);
        magIn.z = *(f32*)(rec + 0x6C);
        f32 mag = PSVECMag(&magIn);
        f32 thresh = lbl_eu_80663AD4 + mag;
        thresh = thresh * thresh;
        if (dist2 > thresh) {
            continue;
        }

        Mtx recMtx;
        for (int r = 0; r < 3; r++) {
            for (int c = 0; c < 4; c++) {
                recMtx[r][c] = *(f32*)(rec + 4 + (r * 4 + c) * 4);
            }
        }
        Mtx concatMtx;
        PSMTXConcat(mv, recMtx, concatMtx);
        GXLoadPosMtxImm(concatMtx, GX_PNMTX0);

        u16 geomIdx = *(u16*)rec;
        u8* geomEntry = geomBase + geomIdx * 0xC;
        u16 triCount = *(u16*)(geomEntry + 2);
        u8* geomTris = base + *(u32*)(geomEntry + 8);

        GXBegin(GX_TRIANGLES, GX_VTXFMT0, triCount * 3);
        for (int j = 0; j < triCount; j++) {
            SceneTriElem* tri = (SceneTriElem*)(geomTris + j * 0x14);
            for (int k = 0; k < 3; k++) {
                SceneVertex* v = &vertBase[(k == 0) ? tri->indexA : ((k == 1) ? tri->indexB : tri->indexC)];
                f32* vf = &v->x;
                GXPosition3f32(vf[0], vf[1], vf[2]);
                u32 flags = flagTable[tri->flags];
                if (flags & 0x4000) {
                    GXColor3u8(0xFF, 0, 0);
                } else if (flags & 0x200) {
                    GXColor3u8(0, 0xFF, 0);
                } else if (flags & 0x2000) {
                    GXColor3u8(0, 0, 0xFF);
                } else if (colorData != NULL) {
                    f32* colors = (f32*)(colorData + tri->colorIdx * 0xC);
                    GXColor3u8((u8)(colors[0] * lbl_eu_8066AF18), (u8)(colors[1] * lbl_eu_8066AF18), (u8)(colors[2] * lbl_eu_8066AF18));
                }
            }
        }
    }

    // Pass C: type-0x13 entries - 0x3C-byte sub-entries paired with 0xE0-byte
    // transform records from res->field_0x60; same visibility-bit test and
    // (20 + mag) cull; colors always from the palette.
    for (int i = 0; i < entryCount; i++) {
        ScnResEntry* e = &entries[i];
        if (e->type != 0x13) continue;

        u16 subCount = e->field_0x2;
        u8* sub = base + e->offset;
        u8* rec = (u8*)res->field_0x60;

        for (int j = 0; j < subCount; j++) {
            u16 visHdr = *(u16*)(sub + 2);
            u32* visBits = func_804B5A70();
            if (visBits[(visHdr >> 3) & 0x1FFC] & (1 << (visHdr & 0x1F))) {
                sub += 0x3C;
                rec += 0xE0;
                continue;
            }

            f32 dx = CAMX - *(f32*)(rec + 0x14);
            f32 dy = CAMY - *(f32*)(rec + 0x24);
            f32 dz = CAMZ - *(f32*)(rec + 0x34);
            f32 dist2 = dx * dx + dy * dy + dz * dz;
            Vec magIn;
            magIn.x = *(f32*)(rec + 0xC8);
            magIn.y = *(f32*)(rec + 0xD0);
            magIn.z = *(f32*)(rec + 0xDC);
            f32 mag = PSVECMag(&magIn);
            f32 thresh = lbl_eu_80663AD4 + mag;
            thresh = thresh * thresh;
            if (dist2 > thresh) {
                sub += 0x3C;
                rec += 0xE0;
                continue;
            }

            Mtx recMtx;
            for (int r = 0; r < 3; r++) {
                for (int c = 0; c < 4; c++) {
                    recMtx[r][c] = *(f32*)(rec + 8 + (r * 4 + c) * 4);
                }
            }
            Mtx concatMtx;
            PSMTXConcat(mv, recMtx, concatMtx);
            GXLoadPosMtxImm(concatMtx, GX_PNMTX0);

            u16 geomIdx = *(u16*)sub;
            u8* geomEntry = geomBase + geomIdx * 0xC;
            u16 triCount = *(u16*)(geomEntry + 2);
            u8* geomTris = base + *(u32*)(geomEntry + 8);

            GXBegin(GX_TRIANGLES, GX_VTXFMT0, triCount * 3);
            for (int j = 0; j < triCount; j++) {
                SceneTriElem* tri = (SceneTriElem*)(geomTris + j * 0x14);
                for (int k = 0; k < 3; k++) {
                    SceneVertex* v = &vertBase[(k == 0) ? tri->indexA : ((k == 1) ? tri->indexB : tri->indexC)];
                    GXPosition3f32(v->x, v->y, v->z);
                    if (colorData != NULL) {
                        f32* colors = (f32*)(colorData + tri->colorIdx * 0xC);
                        GXColor3u8((u8)(colors[0] * lbl_eu_8066AF18), (u8)(colors[1] * lbl_eu_8066AF18), (u8)(colors[2] * lbl_eu_8066AF18));
                    }
                }
            }
            sub += 0x3C;
            rec += 0xE0;
        }
    }
}

#pragma use_lmw_stmw on

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_804BD8A0() {
    lbl_eu_80665968[0] = 0;
    func_804B7804(&lbl_eu_8065F32C);
    void* const obj = &lbl_eu_80665968;
    void* const dtor = (void*)&__dt__804BD8E8;
    void* const reg = &lbl_eu_8065F418;
    __register_global_object(obj, dtor, reg);
}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.bss] 0x8065F32C-0x8065F418 (0xEC, align 4): the scene-resource data
// block and its two tail objects (retail spans 0xC4 / 0xC / 0x1C). The 0xC4
// block (incl. its anonymous tail) is modeled entirely by ScnResData.
ScnResData lbl_eu_8065F32C;
u8 lbl_eu_8065F3F0[0xC];
u8 lbl_eu_8065F3FC[0x1C];

// [.sbss] 0x80665968-0x8066597C (0x14, align 8).
__declspec(align(8)) u32 lbl_eu_80665968[2];
extern "C" { u32 lbl_eu_80665970; u32 lbl_eu_80665974; u32 lbl_eu_80665978; }

// [.data] 0x8056F940-0x8056F9B2: dissolved monolibdata2 tail objects.
f32 lbl_eu_8056F940[4] = { 0.6f, 0.2f, 0.3f, 0.1f };
void* lbl_eu_8056F950[4] = {
    (void*)lbl_eu_80663AE0,
    NULL,
    (void*)&__dt__6CLightFv,
    NULL,
};
void* lbl_eu_8056F960[3] = {
    (void*)lbl_eu_80663AF8,
    NULL,
    (void*)&__dt__reslist_IScnEnvCtl,
};
void* lbl_eu_8056F96C[3] = {
    (void*)lbl_eu_80663B00,
    NULL,
    NULL,
};
void* lbl_eu_8056F978[4] = {
    (void*)lbl_eu_80663B00,
    NULL,
    (void*)&__dt___reslist_base_IScnEnvCtl,
    NULL,
};
char lbl_eu_8056F988[0x20] = "NW4R:Failed assertion IsValid()";
char lbl_eu_8056F9A8[0xA] = "g3d_fog.h";
// data: retail sections verified via run.py data diff (no bypass)
