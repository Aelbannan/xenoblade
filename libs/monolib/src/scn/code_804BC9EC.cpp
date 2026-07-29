// Auto-scaffolded catalog TU for monolib/src/scn/code_804BC9EC
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <monolib/scn/code_804BC9EC.hpp>
#include <PowerPC_EABI_Support/Runtime/NMWException.h>
#include <revolution/gx/GXGeometry.h>
#include <revolution/gx/GXTransform.h>
#include <revolution/gx/GXTev.h>
#include <revolution/gx/GXPixel.h>
#include <revolution/gx/GXAttr.h>
#include <revolution/gx/GXLight.h>
#include <revolution/mtx/mtx.h>
#include <revolution/mtx/vec.h>

// Entry in the top-level chunk list (8 bytes each)
struct ScnResEntry {
    u16 type;
    u16 field_0x2;
    u32 offset;  // relative to data base
};

// Extended ScnResData with named fields for offsets 0x00-0x60.
// Layout-compatible with ScnResData from the header.
struct ScnResDataEx {
    void* field_0x00;  // set by func_804BC9B4 (base + src[1])
    void* field_0x04;  // set by func_804BC9B4 (base + src[2])
    void* field_0x08;  // set by func_804BC9B4 (base + src[3])
    u8 field_0x0C[0x14 - 0x0C];
    void* field_0x14;  // base pointer to parsed data
    void* field_0x18;  // entry type 1
    void* field_0x1C;  // entry type 3
    void* field_0x20;  // entry type 2
    void* field_0x24;  // entry type 5
    void* field_0x28;  // entry type 4
    void* field_0x2C;  // entry type 0: data + 0x20
    void* field_0x30;  // entry type 0: data + 0x20 (same as 0x2C)
    void* field_0x34;  // entry type 0: data pointer
    void* field_0x38;  // entry type 6
    void* field_0x3C;  // entry type 7
    void* field_0x40;  // entry type 8
    u8 field_0x44[0x48 - 0x44];
    void* field_0x48;  // entry type 9
    void* field_0x4C;  // entry type 12
    u8 field_0x50[0x60 - 0x50];
    void* field_0x60;  // array of 0xE0-byte elements
    u8 field_0x64[0x6C - 0x64];
    u32 field_0x6C;    // entry type 0: u16 count
    u32 field_0x70;    // entry type 7: u16 value
    u8 field_0x74[0x78 - 0x74];
    int count;         // 0x78
};

// Additional extern function declarations
void func_804BC9B4(int* dest, int baseOffset, int* src);
void func_804B74F0(ScnResData* res, void* data);
void func_804B7540(ScnResData* res, void* data, u16 param);
void* func_804B5A70(void);

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
    u16 count;       // 0x02: triangle count
    u16 field_0x04;
    u16 field_0x06;
    u16 field_0x08;
    u16 field_0x0A;
    f32 sphereX;     // 0x0C - bounding sphere center?
    f32 sphereY;     // 0x10
    f32 sphereZ;     // 0x14
    f32 sphereR;     // 0x18 - bounding sphere radius?
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
extern "C" {
float FrSqrt__Q24nw4r4mathFf(float);
void Warning__Q24nw4r2dbFPCciPCce(const char*, int, const char*, ...);
}

// String constants for warning
extern const char lbl_eu_80526324[];
extern const char lbl_eu_80526300[];

extern u32 lbl_eu_80665968;
extern "C" void __dt__804BD8E8();
extern u8 lbl_eu_8065F418[0x10];
int func_804BC9EC__Fv(void* self) { return (int)(intptr_t)&lbl_eu_80665968; }

int func_804BC9F4(void* outStruct, u32 data) {
    *(u32*)outStruct = 0;
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
    if (entries[i].type <= 0x13) {
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

void func_804BCC78(void* modelMtx, void* viewData, void* renderParams) {
    ScnResDataEx* res = (ScnResDataEx*)&lbl_eu_8065F32C;
    u8* base = (u8*)res->field_0x14;
    if (base == NULL) {
        return;
    }

    f32* rp = (f32*)renderParams;
    f32* vf = (f32*)viewData;

    u16 entryCount = *(u16*)(base + 6);
    u8* entries = base + 8;

    // Build modelView from viewData (3x4 matrix, 12 floats)
    f32 mv[12];
    for (int j = 0; j < 12; j++) {
        mv[j] = vf[j];
    }

    // Build projection matrix from renderParams (4x4, 16 floats at offsets 0x00-0x3C)
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

    // Camera position (viewData offsets 0x0C, 0x1C, 0x2C)
    f32 camX = vf[3];
    f32 camY = vf[7];
    f32 camZ = vf[11];

    // Vertex array from entry type 1
    SceneVertex* vertBase = (SceneVertex*)res->field_0x18;

    // First pass: find entry type 0x10 (tri data) and type 0x08 (flag data)
    u8* triData = NULL;
    u32* flagData = NULL;
    for (int i = 0; i < entryCount; i++) {
        ScnResEntry* e = &((ScnResEntry*)entries)[i];
        if (e->type == 0x10) {
            triData = base + e->offset;
        } else if (e->type == 0x08) {
            flagData = (u32*)(base + e->offset);
        }
    }

    f32 zero = 0.0f;
    f32 oneThird = 1.0f / 3.0f;
    f32 maxColor = 255.0f;

    // Second pass: render type 1 entries (triangle geometry with colors)
    for (int i = 0; i < entryCount; i++) {
        ScnResEntry* e = &((ScnResEntry*)entries)[i];
        if (e->type != 1) continue;

        u8* data = base + e->offset;
        u16 triCount = *(u16*)(data + 2);
        SceneTriElem* tris = (SceneTriElem*)(data + 0x20);

        for (int j = 0; j < triCount; j++) {
            SceneTriElem* tri = &tris[j];
            SceneVertex* va = &vertBase[tri->indexA];
            SceneVertex* vb = &vertBase[tri->indexB];
            SceneVertex* vc = &vertBase[tri->indexC];

            // Compute centroid
            f32 cx = (va->x + vb->x + vc->x) * oneThird;
            f32 cy = (va->y + vb->y + vc->y) * oneThird;
            f32 cz = (va->z + vb->z + vc->z) * oneThird;

            // Compute squared radius of bounding sphere
            f32 dx0 = cx - va->x, dy0 = cy - va->y, dz0 = cz - va->z;
            f32 r2 = dx0 * dx0 + dy0 * dy0 + dz0 * dz0;

            f32 dx1 = cx - vb->x, dy1 = cy - vb->y, dz1 = cz - vb->z;
            f32 t2 = dx1 * dx1 + dy1 * dy1 + dz1 * dz1;
            if (t2 > r2) r2 = t2;

            f32 dx2 = cx - vc->x, dy2 = cy - vc->y, dz2 = cz - vc->z;
            t2 = dx2 * dx2 + dy2 * dy2 + dz2 * dz2;
            if (t2 > r2) r2 = t2;

            // Check radius against zero (avoid sqrt of zero)
            if (r2 != zero) {
                if (r2 < zero) {
                    Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
                }
                f32 r;
                if (r2 < zero) {
                    r = zero;
                } else {
                    r = FrSqrt__Q24nw4r4mathFf(r2) * r2;
                }
                // Distance from camera to centroid
                f32 dcamX = camX - cx;
                f32 dcamY = camY - cy;
                f32 dcamZ = camZ - cz;
                f32 dist2 = dcamX * dcamX + dcamY * dcamY + dcamZ * dcamZ;

                // Check if triangle is within range, or flag allows rendering
                u32 flg = tri->flags;
                if (dist2 > r * r && !(flg & 0x20000)) {
                    continue;
                }
            }

            // Render triangle
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 3);
            for (int k = 0; k < 3; k++) {
                SceneVertex* v = (k == 0) ? va : ((k == 1) ? vb : vc);
                GXPosition3f32(v->x, v->y, v->z);

                u32 flg = tri->flags;
                if (flg & 0x20000) {
                    GXColor3u8(0xFF, 0x00, 0x00);
                } else if (flg & 0x400000) {
                    GXColor3u8(0x00, 0xFF, 0x00);
                } else if (flg & 0x40000) {
                    GXColor3u8(0x00, 0x00, 0xFF);
                } else {
                    u8* colors = (u8*)triData + tri->colorIdx * 12;
                    GXColor3u8((u8)(maxColor * ((f32*)colors)[0]), (u8)(maxColor * ((f32*)colors)[1]), (u8)(maxColor * ((f32*)colors)[2]));
                }
            }
        }
    }

    // Third pass: render type 0x0B entries (visibility-checked)
    u8* visData = NULL;
    u16 visCount = 0;
    u8* visElemBase = NULL;
    for (int i = 0; i < entryCount; i++) {
        ScnResEntry* e = &((ScnResEntry*)entries)[i];
        if (e->type == 0x0B) {
            visData = base + e->offset;
            visCount = e->field_0x2;
        } else if (e->type == 0x0C) {
            visElemBase = base + e->offset;
        }
    }

    if (visData != NULL) {
        for (int i = 0; i < visCount; i++) {
            SceneVisElem* ve = &((SceneVisElem*)visData)[i];
            if (ve->field_0x00 == 0) {
                u32 idx = ve->field_0x02;
                u32* bits = (u32*)func_804B5A70();
                if (bits[(idx >> 5)] & (1 << (idx & 0x1F))) {
                    continue;
                }

                // Check bounding sphere distance
                u8* sphereData = visElemBase + (idx * 128);
                f32 sx = ((f32*)sphereData)[6];   // offset 0x18? Actually let me check...
                // This is getting complex. Let me mark this as WIP.
            }
        }
    }

    // Fourth pass: render type 0x13 entries (culled geometry)
    u8* cullData = NULL;
    u16 cullCount = 0;
    for (int i = 0; i < entryCount; i++) {
        ScnResEntry* e = &((ScnResEntry*)entries)[i];
        if (e->type == 0x13) {
            cullData = base + e->offset;
            cullCount = e->field_0x2;
            break;
        }
    }

    if (cullData != NULL) {
        // TODO: render type 0x13 entries
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
