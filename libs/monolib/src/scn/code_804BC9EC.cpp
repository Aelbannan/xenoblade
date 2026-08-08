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

// Additional extern function declarations
void func_804BC9B4(int* dest, int baseOffset, int* src);
void func_804B74F0(ScnResData* res, u8* data);
void func_804B7540(ScnResData* res, u8* data, u16 param);
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
// extern const f32 lbl_eu_8066AF10; // 1/3
// extern const f32 lbl_eu_8066AF14; // 0.0f
// extern const f32 lbl_eu_8066AF18; // 255.0f

// nw4r math functions
float FrSqrt__Q24nw4r4mathFf(float);
void Warning__Q24nw4r2dbFPCciPCce(const char*, int, const char*, ...);

// String constants for warning
extern const char lbl_eu_80526324[];
extern const char lbl_eu_80526300[];

extern u32 lbl_eu_80665968;
extern u8 lbl_eu_8065F418[0x10];
int func_804BC9EC__Fv(void* self) { return (int)(intptr_t)&lbl_eu_80665968; }

int func_804BC9F4(u32* outStruct, u32 data) {
    *outStruct = 0;
    func_804B7804(&lbl_eu_8065F32C);

    if (data == 0) {
        return 0;
    }

    u8* base = (u8*)data;
    u32 magic1 = *(u32*)base;

    // Check for "WP" container chunk: upper 16 bits = 0x5750, lower 16 bits <= 0x4F49
    if (magic1 - 0x57500000 <= 0x4F49) {
        base += *(u32*)(base + 8);
    }

    magic1 = *(u32*)base;
    // Check for "ID" data chunk: upper 16 bits = 0x4944, lower 16 bits <= 0x4445
    if (magic1 - 0x49440000 > 0x4445) {
        return 0;
    }

    if (*(u16*)(base + 4) != 0x3EA) {
        return 0;
    }

    ScnResDataEx* res = (ScnResDataEx*)&lbl_eu_8065F32C;
    res->field_0x14 = base;

    u16 entryCount = *(u16*)(base + 6);
    ScnResEntry* entries = (ScnResEntry*)(base + 8);

    int i = 0;
    goto check;

loop:
    switch (entries[i].type) {
    case 0: {
        u8* dataPtr = base + entries[i].offset;
        res->field_0x2C = dataPtr + 0x20;
        res->field_0x30 = dataPtr + 0x20;
        res->field_0x6C = entries[i].field_0x2;
        res->field_0x34 = dataPtr;
        break;
    }
    case 1:
        res->field_0x18 = base + entries[i].offset;
        break;
    case 2:
        res->field_0x20 = base + entries[i].offset;
        break;
    case 3:
        res->field_0x1C = base + entries[i].offset;
        break;
    case 4:
        res->field_0x28 = base + entries[i].offset;
        break;
    case 5:
        res->field_0x24 = base + entries[i].offset;
        break;
    case 6:
        res->field_0x38 = base + entries[i].offset;
        break;
    case 7:
        res->field_0x3C = base + entries[i].offset;
        res->field_0x70 = entries[i].field_0x2;
        break;
    case 8:
        res->field_0x40 = base + entries[i].offset;
        break;
    case 9:
        res->field_0x48 = base + entries[i].offset;
        break;
    case 10:
        func_804BC9B4((int*)res, (int)base, (int*)(base + entries[i].offset));
        break;
    case 11:
        func_804B74F0((ScnResData*)res, base + entries[i].offset);
        break;
    case 12:
        res->field_0x4C = base + entries[i].offset;
        break;
    case 13:
        func_804B7540((ScnResData*)res, base + entries[i].offset, entries[i].field_0x2);
        break;
    }
    i++;
check:
    if (i < entryCount) {
        goto loop;
    }

    return 1;
}

void func_804BCC10(){}

void func_804BCC1C(void* p) {
    *(u32*)p = 0;
    func_804B7804(&lbl_eu_8065F32C);
}

void func_804BCC30(){}

void func_804BCC3C(){}

void func_804BCC48(){}

void func_804BCC54(){}

void func_804BCC60(){}

void func_804BCC6C() {
    func_804B80CC(&lbl_eu_8065F32C);
}

void func_804BCC78(Mtx modelMtx, u8* viewData, u8* renderParams) {
    ScnResDataEx* res = (ScnResDataEx*)&lbl_eu_8065F32C;
    u8* base = res->field_0x14;
    if (base == NULL) {
        return;
    }

    f32* rp = (f32*)renderParams;
    f32* vf = (f32*)viewData;

    u16 entryCount = *(u16*)(base + 6);
    u8* entries = base + 8;

    Mtx mv;
    mv[0][0] = vf[0];  mv[0][1] = vf[1];  mv[0][2] = vf[2];  mv[0][3] = vf[3];
    mv[1][0] = vf[4];  mv[1][1] = vf[5];  mv[1][2] = vf[6];  mv[1][3] = vf[7];
    mv[2][0] = vf[8];  mv[2][1] = vf[9];  mv[2][2] = vf[10]; mv[2][3] = vf[11];

    Mtx44 projMtx;
    projMtx[0][0] = rp[0];  projMtx[0][1] = rp[1];  projMtx[0][2] = rp[2];  projMtx[0][3] = rp[3];
    projMtx[1][0] = rp[4];  projMtx[1][1] = rp[5];  projMtx[1][2] = rp[6];  projMtx[1][3] = rp[7];
    projMtx[2][0] = rp[8];  projMtx[2][1] = rp[9];  projMtx[2][2] = rp[10]; projMtx[2][3] = rp[11];
    projMtx[3][0] = rp[12]; projMtx[3][1] = rp[13]; projMtx[3][2] = rp[14]; projMtx[3][3] = rp[15];

    PSMTXInverse(mv, mv);
    GXSetProjection(projMtx, GX_ORTHOGRAPHIC);
    GXLoadPosMtxImm(mv, GX_PNMTX0);

    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetCullMode(GX_CULL_NONE);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGB, GX_RGB8, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetAlphaCompare(GX_ALWAYS, 0xFF, GX_AOP_AND, GX_ALWAYS, 0xFF);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);

    f32 camX = vf[3];
    f32 camY = vf[7];
    f32 camZ = vf[11];

    SceneVertex* vertBase = (SceneVertex*)res->field_0x18;

    // First pass: find type 0x10 and 0x08 entry data
    u8* colorData = NULL;
    u32* flagData = NULL;
    for (int i = 0; i < entryCount; i++) {
        ScnResEntry* e = &((ScnResEntry*)entries)[i];
        if (e->type == 0x10) {
            colorData = base + e->offset;
        } else if (e->type == 0x08) {
            flagData = (u32*)(base + e->offset);
        }
    }

    f32 oneThird = 1.0f / 3.0f;
    f32 zero = 0.0f;

    // Second pass: render type 1 entries  
    for (int i = 0; i < entryCount; i++) {
        ScnResEntry* e = &((ScnResEntry*)entries)[i];
        if (e->type != 1) continue;

        u8* data = base + e->offset;
        u16 triCount = *(u16*)(data + 2);
        SceneTriElem* tris = (SceneTriElem*)((u8*)data + 0x20);

        for (int j = 0; j < triCount; j++) {
            SceneTriElem* tri = &tris[j];
            SceneVertex* va = &vertBase[tri->indexA];
            SceneVertex* vb = &vertBase[tri->indexB];
            SceneVertex* vc = &vertBase[tri->indexC];

            f32 cx = (va->x + vb->x + vc->x) * oneThird;
            f32 cy = (va->y + vb->y + vc->y) * oneThird;
            f32 cz = (va->z + vb->z + vc->z) * oneThird;

            f32 dx0 = cx - va->x, dy0 = cy - va->y, dz0 = cz - va->z;
            f32 radiusSq = dx0 * dx0 + dy0 * dy0 + dz0 * dz0;

            f32 dx1 = cx - vb->x, dy1 = cy - vb->y, dz1 = cz - vb->z;
            f32 t2 = dx1 * dx1 + dy1 * dy1 + dz1 * dz1;
            if (t2 > radiusSq) radiusSq = t2;

            f32 dx2 = cx - vc->x, dy2 = cy - vc->y, dz2 = cz - vc->z;
            t2 = dx2 * dx2 + dy2 * dy2 + dz2 * dz2;
            if (t2 > radiusSq) radiusSq = t2;

            if (radiusSq != zero) {
                if (radiusSq < zero) {
                    Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
                }
                f32 radius = (radiusSq < zero) ? zero : FrSqrt__Q24nw4r4mathFf(radiusSq) * radiusSq;

                f32 dcamX = camX - cx, dcamY = camY - cy, dcamZ = camZ - cz;
                f32 dist2 = dcamX * dcamX + dcamY * dcamY + dcamZ * dcamZ;

                u32 flg = tri->flags;
                if (dist2 > radius * radius && !(flg & 0x20000)) {
                    continue;
                }
            }

            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 3);
            for (int k = 0; k < 3; k++) {
                const SceneVertex* v = (k == 0) ? va : ((k == 1) ? vb : vc);
                GXPosition3f32(v->x, v->y, v->z);

                u32 flg = tri->flags;
                if (flg & 0x20000) {
                    GXColor3u8(0xFF, 0, 0);
                } else if (flg & 0x400000) {
                    GXColor3u8(0, 0xFF, 0);
                } else if (flg & 0x40000) {
                    GXColor3u8(0, 0, 0xFF);
                } else if (colorData != NULL) {
                    f32* colors = (f32*)(colorData + tri->colorIdx * 0xC);
                    GXColor3u8((u8)(colors[0] * 255.0f), (u8)(colors[1] * 255.0f), (u8)(colors[2] * 255.0f));
                }
            }
        }
    }

    // Third pass: render type 0x0B entries
    u8* visData = NULL;
    u16 visCount = 0;
    u8* visGeomBase = NULL;
    for (int i = 0; i < entryCount; i++) {
        ScnResEntry* e = &((ScnResEntry*)entries)[i];
        if (e->type == 2) {
            visGeomBase = base + e->offset;
        } else if (e->type == 0x0B) {
            visData = base + e->offset;
            visCount = e->field_0x2;
        }
    }

    // Fourth pass: render type 0x13 entries
    for (int i = 0; i < entryCount; i++) {
        ScnResEntry* e = &((ScnResEntry*)entries)[i];
        if (e->type != 0x13) continue;

        u16 cullCount = e->field_0x2;
        u8* cullBase = base + e->offset;
        u8* cullRef = (u8*)res->field_0x60;

        for (int j = 0; j < cullCount; j++) {
            SceneCullElem* ce = (SceneCullElem*)(cullBase + j * 0x3C);

            u32 visIdx = ce->visIdx;
            u32* bits = (u32*)func_804B5A70();
            if (bits[visIdx >> 5] & (1 << (visIdx & 0x1F))) continue;

            f32* cf = (f32*)(cullRef + j * 0xE0);
            f32 sx = cf[5];  /* 0x14/4 */
            f32 sy = cf[9];  /* 0x24/4 */
            f32 sz = cf[13]; /* 0x34/4 */
            f32 dx = camX - sx, dy = camY - sy, dz = camZ - sz;
            f32 dist2 = dx * dx + dy * dy + dz * dz;

            Mtx localMtx;
            localMtx[0][0] = cf[2];  localMtx[0][1] = cf[3];  localMtx[0][2] = cf[4];  localMtx[0][3] = cf[5];
            localMtx[1][0] = cf[6];  localMtx[1][1] = cf[7];  localMtx[1][2] = cf[8];  localMtx[1][3] = cf[9];
            localMtx[2][0] = cf[10]; localMtx[2][1] = cf[11]; localMtx[2][2] = cf[12]; localMtx[2][3] = cf[13];

            Mtx concatMtx;
            PSMTXConcat(mv, localMtx, concatMtx);
            GXLoadPosMtxImm(concatMtx, GX_PNMTX0);

            u16 geomIdx = ce->index;
            u8* geomEntry = visGeomBase + geomIdx * 0xC;
            u16 triCount = *(u16*)(geomEntry + 2);
            u8* geomTris = base + *(u32*)(geomEntry + 8);

            GXBegin(GX_TRIANGLES, GX_VTXFMT0, triCount * 3);
            for (int t = 0; t < triCount; t++) {
                SceneTriElem* tri = (SceneTriElem*)(geomTris + t * 0x14);
                for (int k = 0; k < 3; k++) {
                    SceneVertex* v = &vertBase[(k == 0) ? tri->indexA : ((k == 1) ? tri->indexB : tri->indexC)];
                    GXPosition3f32(v->x, v->y, v->z);
                    if (colorData != NULL) {
                        f32* colors = (f32*)(colorData + tri->colorIdx * 0xC);
                        GXColor3u8((u8)(colors[0] * 255.0f), (u8)(colors[1] * 255.0f), (u8)(colors[2] * 255.0f));
                    }
                }
            }
        }
    }
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_804BD8A0() {
    lbl_eu_80665968 = 0;
    func_804B7804(&lbl_eu_8065F32C);
    void* const obj = &lbl_eu_80665968;
    void* const dtor = (void*)&__dt__804BD8E8;
    void* const reg = &lbl_eu_8065F418;
    __register_global_object(obj, dtor, reg);
}
