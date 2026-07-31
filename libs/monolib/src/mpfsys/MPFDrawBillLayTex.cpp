#include "monolib/mpfsys/MPFDrawBillLayTex.hpp"
#include "monolib/mpfsys/MPFDrawBillboard.hpp"

#include <revolution/GX.h>
#include <revolution/MTX.h>
#include <nw4r/math/math_types.h>

struct MPFDrawBillData {
    f32 halfWidth;             // 0x00
    f32 halfHeight;            // 0x04
    u8 gap08[4];               // 0x08
    s16 texIndex;              // 0x0C
    u8 texMap;                 // 0x0E
    u8 layerIndex;             // 0x0F
    u8 colors[4];              // 0x10
    u32 flags;                 // 0x14
    f32 texScale;              // 0x18
    f32 layerDepth;            // 0x1C
};

struct MPFDrawBillLayer {
    u8 gap00[0x54];
    Mtx matrix;
    u8 gap84[0x30];
};

static inline void addBillVec(nw4r::math::VEC3* out, const Vec* a, const Vec* b) {
    nw4r::math::VEC3Add(out,
                        reinterpret_cast<const nw4r::math::VEC3*>(a),
                        reinterpret_cast<const nw4r::math::VEC3*>(b));
}

struct MPFDrawBillIndexList {
    u32 count;
    u32 indices[1];
};

struct MPFDrawBillPair {
    u32 index;
    f32 parameter;
};

struct MPFDrawBillPairList {
    u32 count;
    MPFDrawBillPair pairs[1];
};

extern "C" {
    s8 lbl_eu_806658B0;
    mpfsys::MPFDrawBillLayTex* lbl_eu_806658B4;
    static mpfsys::MPFDrawBillLayTex lbl_eu_8056DC00;

    extern MPFDrawBillData* lbl_eu_80665860;
    extern void* lbl_eu_80665838;
    extern Mtx lbl_eu_80658428;
    extern Vec* lbl_eu_8066584C;
    extern u16* lbl_eu_80665850;

    extern const u32 lbl_eu_80523E00[4];
    extern const u32 lbl_eu_80523E10[4];

    extern s32 lbl_eu_8066A728;
    extern u32 lbl_eu_8066A72C;
    extern f32 lbl_eu_8066A730;
    extern f32 lbl_eu_8066A848;
    extern f32 lbl_eu_8066A84C;
    extern f32 lbl_eu_8066A850;

    Vec* func_804B5A68(void);

    MPFDrawBillLayer* func_804734F4__Q26mpfsys17UnkClass_80471EC8FUc(void*, u8);
    void func_804737CC__Q26mpfsys17UnkClass_80471EC8Fv(s16, f32);
    void func_804737F0__Q26mpfsys17UnkClass_80471EC8Fv(s16, f32);
    void func_804742BC__Q26mpfsys17UnkClass_80471EC8Fv(void);
    void func_80474A40__Q26mpfsys17UnkClass_80471EC8Fv(void);
    void func_80474AA0__Q26mpfsys17UnkClass_80471EC8Fv(void);
    void func_80474CC4__Q26mpfsys17UnkClass_80471EC8Fv(void);
    void func_80474DAC__Q26mpfsys17UnkClass_80471EC8Fv(f32);
    void func_80474DF8__Q26mpfsys17UnkClass_80471EC8Fv(u8);
    void func_80474E24__Q26mpfsys17UnkClass_80471EC8Fv(void);
    void func_80474E68__Q26mpfsys17UnkClass_80471EC8Fv(void);
    void func_80474F2C__Q26mpfsys17UnkClass_80471EC8Fv(void);
    void func_80474F54__Q26mpfsys17UnkClass_80471EC8Fv(void);
}

namespace mpfsys {

MPFDrawBillLayTex* MPFDrawBillLayTex::getInstance() {
    if (!lbl_eu_806658B0) {
        lbl_eu_806658B4 = &lbl_eu_8056DC00;
        lbl_eu_806658B0 = 1;
    }
    return (MPFDrawBillLayTex*)&lbl_eu_806658B4;
}

} // namespace mpfsys

extern "C" void func_80479F54(MPFDrawBillData* billboard, Vec* positions) {
    f32 height = billboard->halfHeight;
    if (billboard->flags & 4) {
        height = -height;
    }

    if (billboard->flags & 2) {
        Vec left;
        Vec right;
        left.x = -billboard->halfWidth;
        left.y = lbl_eu_8066A848;
        left.z = lbl_eu_8066A848;
        right.x = billboard->halfWidth;
        right.y = lbl_eu_8066A848;
        right.z = lbl_eu_8066A848;
        PSMTXMultVec(lbl_eu_80658428, &left, &positions[0]);
        PSMTXMultVec(lbl_eu_80658428, &right, &positions[1]);

        if (billboard->layerDepth != lbl_eu_8066A848) {
            MPFDrawBillLayer* layers = func_804734F4__Q26mpfsys17UnkClass_80471EC8FUc(lbl_eu_80665838, billboard->layerIndex);
            s32 count = lbl_eu_8066A728;
            f32 step = billboard->layerDepth * lbl_eu_8066A730;
            f32 v = lbl_eu_8066A850 * step + (lbl_eu_8066A84C - lbl_eu_8066A850 * billboard->layerDepth);
            f32 zero = lbl_eu_8066A848;
            for (s32 i = 0; i < count; i++) {
                f32 y = height * v;
                Vec layerLeft;
                Vec layerRight;
                layerRight.x = billboard->halfWidth;
                layerLeft.x = -billboard->halfWidth;
                layerLeft.y = y;
                layerLeft.z = zero;
                layerRight.y = y;
                layerRight.z = zero;
                PSMTXMultVec(lbl_eu_80658428, &layerLeft, &layerLeft);
                PSMTXMultVec(lbl_eu_80658428, &layerRight, &layerRight);
                PSMTXMultVec(layers->matrix, &layerLeft, &positions[2 + i * 2]);
                PSMTXMultVec(layers->matrix, &layerRight, &positions[3 + i * 2]);
                v += step;
                layers++;
            }
        } else {
            Vec layerLeft;
            Vec layerRight;
            layerRight.x = billboard->halfWidth;
            layerLeft.x = -billboard->halfWidth;
            layerLeft.y = height;
            layerLeft.z = lbl_eu_8066A848;
            layerRight.y = height;
            layerRight.z = lbl_eu_8066A848;
            PSMTXMultVec(lbl_eu_80658428, &layerLeft, &layerLeft);
            PSMTXMultVec(lbl_eu_80658428, &layerRight, &layerRight);
            MPFDrawBillLayer* layers = func_804734F4__Q26mpfsys17UnkClass_80471EC8FUc(lbl_eu_80665838, billboard->layerIndex);
            s32 count = lbl_eu_8066A728;
            for (s32 i = 0; i < count; i++) {
                PSMTXMultVec(layers->matrix, &layerLeft, &positions[2 + i * 2]);
                PSMTXMultVec(layers->matrix, &layerRight, &positions[3 + i * 2]);
                layers++;
            }
        }
    } else if (billboard->layerDepth != lbl_eu_8066A848) {
        Vec left;
        Vec right;
        left.x = -billboard->halfWidth;
        left.y = lbl_eu_8066A848;
        left.z = lbl_eu_8066A848;
        right.x = billboard->halfWidth;
        right.y = lbl_eu_8066A848;
        right.z = lbl_eu_8066A848;
        PSMTXMultVec(lbl_eu_80658428, &left, &positions[0]);
        PSMTXMultVec(lbl_eu_80658428, &right, &positions[1]);
        func_804734F4__Q26mpfsys17UnkClass_80471EC8FUc(lbl_eu_80665838, billboard->layerIndex);
        s32 count = lbl_eu_8066A728;
        f32 step = billboard->layerDepth * lbl_eu_8066A730;
        f32 v = lbl_eu_8066A850 * step + (lbl_eu_8066A84C - lbl_eu_8066A850 * billboard->layerDepth);
        f32 zero = lbl_eu_8066A848;
        for (s32 i = 0; i < count; i++) {
            f32 y = height * v;
            Vec layerLeft;
            Vec layerRight;
            layerRight.x = billboard->halfWidth;
            layerLeft.x = -billboard->halfWidth;
            layerLeft.y = y;
            layerLeft.z = zero;
            layerRight.y = y;
            layerRight.z = zero;
            PSMTXMultVec(lbl_eu_80658428, &layerLeft, &positions[2 + i * 2]);
            PSMTXMultVec(lbl_eu_80658428, &layerRight, &positions[3 + i * 2]);
            v += step;
        }
    } else {
        f32 zero = lbl_eu_8066A848;
        positions[0].x = -billboard->halfWidth;
        positions[0].y = zero;
        positions[0].z = zero;
        positions[1].x = billboard->halfWidth;
        positions[1].y = zero;
        positions[1].z = zero;
        positions[2].x = -billboard->halfWidth;
        positions[2].y = height;
        positions[2].z = zero;
        positions[3].x = billboard->halfWidth;
        positions[3].y = height;
        positions[3].z = zero;
        PSMTXMultVec(lbl_eu_80658428, &positions[0], &positions[0]);
        PSMTXMultVec(lbl_eu_80658428, &positions[1], &positions[1]);
        PSMTXMultVec(lbl_eu_80658428, &positions[2], &positions[2]);
        PSMTXMultVec(lbl_eu_80658428, &positions[3], &positions[3]);
    }
}

extern "C" void func_8047A330(MPFDrawBillData* billboard, Vec* positions, MPFDrawBillIndexList* list) {
    s32 mask = -1;
    if (!(billboard->flags & 2) && billboard->layerDepth == lbl_eu_8066A848) {
        mask = 0;
    }

    u32 count = list->count;
    GXBegin(GX_QUADS, GX_VTXFMT0, (count - 1) * 4);
    const u32* index = list->indices + 1;
    for (u32 i = 0; i < count - 1; i++, index++) {
        u32 value = *index;
        u32 first = ((value & lbl_eu_8066A72C) << 1 & mask) + 2;
        const Vec* corner = &lbl_eu_8066584C[value];
        nw4r::math::VEC3 vertex0;
        nw4r::math::VEC3 vertex1;
        nw4r::math::VEC3 vertex2;
        nw4r::math::VEC3 vertex3;
        nw4r::math::VEC3Add(&vertex1, (const nw4r::math::VEC3*)corner, (const nw4r::math::VEC3*)&positions[1]);
        nw4r::math::VEC3Add(&vertex0, (const nw4r::math::VEC3*)corner, (const nw4r::math::VEC3*)&positions[0]);
        nw4r::math::VEC3Add(&vertex2, (const nw4r::math::VEC3*)corner, (const nw4r::math::VEC3*)&positions[first]);
        nw4r::math::VEC3Add(&vertex3, (const nw4r::math::VEC3*)corner, (const nw4r::math::VEC3*)&positions[first + 1]);

        const u32* order = lbl_eu_80523E00;
        if (value & 1) {
            order = lbl_eu_80523E10;
        }
        GXPosition3f32(vertex0.x, vertex0.y, vertex0.z);
        GXTexCoord1u16(lbl_eu_80665850[value]);
        GXColor1x8(billboard->colors[order[0]]);
        GXPosition3f32(vertex2.x, vertex2.y, vertex2.z);
        GXTexCoord1u16(lbl_eu_80665850[value]);
        GXColor1x8(billboard->colors[order[1]]);
        GXPosition3f32(vertex3.x, vertex3.y, vertex3.z);
        GXTexCoord1u16(lbl_eu_80665850[value]);
        GXColor1x8(billboard->colors[order[2]]);
        GXPosition3f32(vertex1.x, vertex1.y, vertex1.z);
        GXTexCoord1u16(lbl_eu_80665850[value]);
        GXColor1x8(billboard->colors[order[3]]);
    }
}

extern "C" void func_8047A570(MPFDrawBillData* billboard, Vec* positions, MPFDrawBillPairList* list) {
    s32 mask = -1;
    if (!(billboard->flags & 2) && billboard->layerDepth != lbl_eu_8066A848) {
        mask = 0;
    }

    MPFDrawBillPair* pair = list->pairs;
    s32 count = (s32)(list->count >> 1);
    for (s32 i = 0; i < count; i++, pair++) {
        func_80474DAC__Q26mpfsys17UnkClass_80471EC8Fv(pair->parameter);

        u32 value = pair->index;
        const u32* order = (value & 1) ? lbl_eu_80523E10 : lbl_eu_80523E00;
        u32 base = (value & lbl_eu_8066A72C) << 1;
        base &= mask;
        u32 first = base + 2;
        const Vec* corner = &lbl_eu_8066584C[value];
        nw4r::math::VEC3 vertex0;
        nw4r::math::VEC3 vertex1;
        nw4r::math::VEC3 vertex2;
        nw4r::math::VEC3 vertex3;
        nw4r::math::VEC3Add(&vertex0, (const nw4r::math::VEC3*)corner, (const nw4r::math::VEC3*)&positions[0]);
        nw4r::math::VEC3Add(&vertex1, (const nw4r::math::VEC3*)corner, (const nw4r::math::VEC3*)&positions[first]);
        nw4r::math::VEC3Add(&vertex2, (const nw4r::math::VEC3*)corner, (const nw4r::math::VEC3*)&positions[first + 1]);
        nw4r::math::VEC3Add(&vertex3, (const nw4r::math::VEC3*)corner, (const nw4r::math::VEC3*)&positions[1]);

        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(vertex0.x, vertex0.y, vertex0.z);
        GXTexCoord1u16(lbl_eu_80665850[value]);
        GXColor1x8(billboard->colors[order[0]]);
        GXPosition3f32(vertex1.x, vertex1.y, vertex1.z);
        GXTexCoord1u16(lbl_eu_80665850[value]);
        GXColor1x8(billboard->colors[order[1]]);
        GXPosition3f32(vertex2.x, vertex2.y, vertex2.z);
        GXTexCoord1u16(lbl_eu_80665850[value]);
        GXColor1x8(billboard->colors[order[2]]);
        GXPosition3f32(vertex3.x, vertex3.y, vertex3.z);
        GXTexCoord1u16(lbl_eu_80665850[value]);
        GXColor1x8(billboard->colors[order[3]]);
    }
}

extern "C" void func_8047A7B0__Q26mpfsys16MPFDrawBillboardFv(mpfsys::MPFDrawBillboard*, MPFDrawBillData* billboard, MPFDrawBillIndexList* list) {
    Vec* positions = func_804B5A68();
    if (lbl_eu_80665860 != billboard) {
        func_804742BC__Q26mpfsys17UnkClass_80471EC8Fv();
        func_804737CC__Q26mpfsys17UnkClass_80471EC8Fv(billboard->texIndex, billboard->texScale);
        func_80479F54(billboard, positions);
        lbl_eu_80665860 = billboard;
    }
    if (billboard->flags & 1) {
        func_80474A40__Q26mpfsys17UnkClass_80471EC8Fv();
    } else {
        func_80474AA0__Q26mpfsys17UnkClass_80471EC8Fv();
    }
    func_80474F2C__Q26mpfsys17UnkClass_80471EC8Fv();
    func_80474DF8__Q26mpfsys17UnkClass_80471EC8Fv(billboard->texMap);
    if (billboard->flags & 8) {
        func_80474E68__Q26mpfsys17UnkClass_80471EC8Fv();
    } else {
        func_80474E24__Q26mpfsys17UnkClass_80471EC8Fv();
    }
    func_8047A330(billboard, positions, list);
}

extern "C" void func_8047A86C__Q26mpfsys16MPFDrawBillboardFv(mpfsys::MPFDrawBillboard*, MPFDrawBillData* billboard, MPFDrawBillPairList* list) {
    Vec* positions = func_804B5A68();
    if (lbl_eu_80665860 != billboard) {
        func_804742BC__Q26mpfsys17UnkClass_80471EC8Fv();
        func_804737CC__Q26mpfsys17UnkClass_80471EC8Fv(billboard->texIndex, billboard->texScale);
        func_80479F54(billboard, positions);
        lbl_eu_80665860 = billboard;
    }
    func_80474DF8__Q26mpfsys17UnkClass_80471EC8Fv(billboard->texMap);
    if (billboard->flags & 8) {
        func_80474E68__Q26mpfsys17UnkClass_80471EC8Fv();
    } else {
        func_80474E24__Q26mpfsys17UnkClass_80471EC8Fv();
    }
    func_80474F54__Q26mpfsys17UnkClass_80471EC8Fv();
    func_80474A40__Q26mpfsys17UnkClass_80471EC8Fv();
    func_8047A570(billboard, positions, list);
    func_80474CC4__Q26mpfsys17UnkClass_80471EC8Fv();
}

extern "C" void func_8047A918(void*, MPFDrawBillData* billboard, Vec* positions) {
    func_804737CC__Q26mpfsys17UnkClass_80471EC8Fv(billboard->texIndex, billboard->texScale);
    func_804742BC__Q26mpfsys17UnkClass_80471EC8Fv();

    if (billboard->flags & 2) {
        Vec vertex0;
        Vec vertex1;
        Vec vertex2;
        Vec vertex3;
        if (billboard->layerDepth != lbl_eu_8066A848) {
            MPFDrawBillLayer* layers = func_804734F4__Q26mpfsys17UnkClass_80471EC8FUc(lbl_eu_80665838, billboard->layerIndex);
            f32 step = billboard->layerDepth * lbl_eu_8066A730;
            f32 v = lbl_eu_8066A850 * step + (lbl_eu_8066A84C - lbl_eu_8066A850 * billboard->layerDepth);
            for (s32 i = 0; i < lbl_eu_8066A728; i++) {
                f32 y = billboard->halfHeight * v;
                vertex0.x = -billboard->halfWidth;
                vertex0.y = y;
                vertex0.z = lbl_eu_8066A848;
                vertex1.x = billboard->halfWidth;
                vertex1.y = y;
                vertex1.z = lbl_eu_8066A848;
                vertex2.x = lbl_eu_8066A848;
                vertex2.y = y;
                vertex2.z = -billboard->halfWidth;
                vertex3.x = lbl_eu_8066A848;
                vertex3.y = y;
                vertex3.z = billboard->halfWidth;
                PSMTXMultVec(layers->matrix, &vertex0, positions + 0);
                PSMTXMultVec(layers->matrix, &vertex1, positions + 1);
                PSMTXMultVec(layers->matrix, &vertex2, positions + 2);
                PSMTXMultVec(layers->matrix, &vertex3, positions + 3);
                v += step;
                layers++;
                positions += 4;
            }
        } else {
            vertex0.x = -billboard->halfWidth;
            vertex0.y = billboard->halfHeight;
            vertex0.z = lbl_eu_8066A848;
            vertex1.x = billboard->halfWidth;
            vertex1.y = billboard->halfHeight;
            vertex1.z = lbl_eu_8066A848;
            vertex2.x = lbl_eu_8066A848;
            vertex2.y = billboard->halfHeight;
            vertex2.z = -billboard->halfWidth;
            vertex3.x = lbl_eu_8066A848;
            vertex3.y = billboard->halfHeight;
            vertex3.z = billboard->halfWidth;
            MPFDrawBillLayer* layers = func_804734F4__Q26mpfsys17UnkClass_80471EC8FUc(lbl_eu_80665838, billboard->layerIndex);
            Vec* output = positions;
            for (s32 i = 0; i < lbl_eu_8066A728; i++) {
                PSMTXMultVec(layers->matrix, &vertex0, output + 0);
                PSMTXMultVec(layers->matrix, &vertex1, output + 1);
                PSMTXMultVec(layers->matrix, &vertex2, output + 2);
                PSMTXMultVec(layers->matrix, &vertex3, output + 3);
                layers++;
                output += 4;
            }
        }
    } else if (billboard->layerDepth != lbl_eu_8066A848) {
        Vec* output = positions;
        func_804734F4__Q26mpfsys17UnkClass_80471EC8FUc(lbl_eu_80665838, billboard->layerIndex);
        f32 step = billboard->layerDepth * lbl_eu_8066A730;
        f32 v = lbl_eu_8066A850 * step + (lbl_eu_8066A84C - lbl_eu_8066A850 * billboard->layerDepth);
        for (s32 i = 0; i < lbl_eu_8066A728; i++) {
            f32 y = billboard->halfHeight * v;
            output[0].x = -billboard->halfWidth;
            output[0].y = y;
            output[0].z = lbl_eu_8066A848;
            output[1].x = billboard->halfWidth;
            output[1].y = y;
            output[1].z = lbl_eu_8066A848;
            output[2].x = lbl_eu_8066A848;
            output[2].y = y;
            output[2].z = -billboard->halfWidth;
            output[3].x = lbl_eu_8066A848;
            output[3].y = y;
            output[3].z = billboard->halfWidth;
            v += step;
            output += 4;
        }
    } else {
        Vec* output = positions;
        output[0].x = -billboard->halfWidth;
        output[0].y = billboard->halfHeight;
        output[0].z = lbl_eu_8066A848;
        output[1].x = billboard->halfWidth;
        output[1].y = billboard->halfHeight;
        output[1].z = lbl_eu_8066A848;
        output[2].x = lbl_eu_8066A848;
        output[2].y = billboard->halfHeight;
        output[2].z = -billboard->halfWidth;
        output[3].x = lbl_eu_8066A848;
        output[3].y = billboard->halfHeight;
        output[3].z = billboard->halfWidth;
    }

    if (billboard->flags & 4) {
        for (s32 i = 0; i < lbl_eu_8066A728; i++) {
            positions[0].y = -positions[0].y;
            positions[1].y = -positions[1].y;
            positions[2].y = -positions[2].y;
            positions[3].y = -positions[3].y;
            positions += 4;
        }
    }
}

extern "C" void func_8047B1E8(void* self, MPFDrawBillData* billboard, MPFDrawBillIndexList* list) {
    Vec* positions = func_804B5A68();
    if (lbl_eu_80665860 != billboard) {
        func_8047A918(self, billboard, positions);
        lbl_eu_80665860 = billboard;
    }

    s32 mask = -1;
    if (!(billboard->flags & 2) && billboard->layerDepth != lbl_eu_8066A848) {
        mask = 0;
    }
    func_80474DF8__Q26mpfsys17UnkClass_80471EC8Fv(billboard->texMap);
    if (billboard->flags & 8) {
        func_80474E68__Q26mpfsys17UnkClass_80471EC8Fv();
    } else {
        func_80474E24__Q26mpfsys17UnkClass_80471EC8Fv();
    }
    func_80474F2C__Q26mpfsys17UnkClass_80471EC8Fv();
    if (billboard->flags & 1) {
        func_80474A40__Q26mpfsys17UnkClass_80471EC8Fv();
    } else {
        func_80474AA0__Q26mpfsys17UnkClass_80471EC8Fv();
    }

    GXBegin(GX_QUADS, GX_VTXFMT0, (list->count - 1) * 8);
    const u32* index = list->indices + 1;
    for (u32 i = 1; i < list->count; i++, index++) {
        u32 value = *index;
        u32 base = (value & lbl_eu_8066A72C) << 2;
        base &= mask;
        const Vec* corner = &lbl_eu_8066584C[value];
        Vec vertex0;
        Vec vertex1;
        Vec vertex2;
        Vec vertex3;
        Vec vertex4;
        Vec vertex5;
        Vec vertex6;
        Vec vertex7;
        vertex0.x = corner->x - billboard->halfWidth;
        vertex0.y = corner->y;
        vertex0.z = corner->z;
        nw4r::math::VEC3Add((nw4r::math::VEC3*)&vertex1, (const nw4r::math::VEC3*)corner, (const nw4r::math::VEC3*)&positions[base + 0]);
        nw4r::math::VEC3Add((nw4r::math::VEC3*)&vertex2, (const nw4r::math::VEC3*)corner, (const nw4r::math::VEC3*)&positions[base + 1]);
        vertex3.x = corner->x + billboard->halfWidth;
        vertex3.y = corner->y;
        vertex3.z = corner->z;
        vertex4.x = corner->x;
        vertex4.y = corner->y;
        vertex4.z = corner->z - billboard->halfWidth;
        nw4r::math::VEC3Add((nw4r::math::VEC3*)&vertex5, (const nw4r::math::VEC3*)corner, (const nw4r::math::VEC3*)&positions[base + 2]);
        nw4r::math::VEC3Add((nw4r::math::VEC3*)&vertex6, (const nw4r::math::VEC3*)corner, (const nw4r::math::VEC3*)&positions[base + 3]);
        vertex7.x = corner->x;
        vertex7.y = corner->y;
        vertex7.z = corner->z + billboard->halfWidth;

        GXPosition3f32(vertex0.x, vertex0.y, vertex0.z);
        GXTexCoord1u16(lbl_eu_80665850[value]);
        GXColor1x8(billboard->colors[0]);
        GXPosition3f32(vertex1.x, vertex1.y, vertex1.z);
        GXTexCoord1u16(lbl_eu_80665850[value]);
        GXColor1x8(billboard->colors[1]);
        GXPosition3f32(vertex2.x, vertex2.y, vertex2.z);
        GXTexCoord1u16(lbl_eu_80665850[value]);
        GXColor1x8(billboard->colors[2]);
        GXPosition3f32(vertex3.x, vertex3.y, vertex3.z);
        GXTexCoord1u16(lbl_eu_80665850[value]);
        GXColor1x8(billboard->colors[3]);
        GXPosition3f32(vertex4.x, vertex4.y, vertex4.z);
        GXTexCoord1u16(lbl_eu_80665850[value]);
        GXColor1x8(billboard->colors[0]);
        GXPosition3f32(vertex5.x, vertex5.y, vertex5.z);
        GXTexCoord1u16(lbl_eu_80665850[value]);
        GXColor1x8(billboard->colors[1]);
        GXPosition3f32(vertex6.x, vertex6.y, vertex6.z);
        GXTexCoord1u16(lbl_eu_80665850[value]);
        GXColor1x8(billboard->colors[2]);
        GXPosition3f32(vertex7.x, vertex7.y, vertex7.z);
        GXTexCoord1u16(lbl_eu_80665850[value]);
        GXColor1x8(billboard->colors[3]);
    }
}

extern "C" void func_8047B528(void* self, MPFDrawBillData* billboard, MPFDrawBillPairList* list) {
    Vec* positions = func_804B5A68();
    if (lbl_eu_80665860 != billboard) {
        func_8047A918(self, billboard, positions);
        lbl_eu_80665860 = billboard;
    }

    s32 mask = -1;
    if (!(billboard->flags & 2) && billboard->layerDepth != lbl_eu_8066A848) {
        mask = 0;
    }
    func_80474DF8__Q26mpfsys17UnkClass_80471EC8Fv(billboard->texMap);
    if (billboard->flags & 8) {
        func_80474E68__Q26mpfsys17UnkClass_80471EC8Fv();
    } else {
        func_80474E24__Q26mpfsys17UnkClass_80471EC8Fv();
    }
    func_80474F54__Q26mpfsys17UnkClass_80471EC8Fv();
    func_80474A40__Q26mpfsys17UnkClass_80471EC8Fv();

    MPFDrawBillPair* pair = list->pairs;
    s32 count = (s32)(list->count >> 1);
    for (s32 i = 0; i < count; i++, pair++) {
        func_80474DAC__Q26mpfsys17UnkClass_80471EC8Fv(pair->parameter);
        u32 value = pair->index;
        u32 base = (value & lbl_eu_8066A72C) << 2;
        base &= mask;
        const Vec* corner = &lbl_eu_8066584C[value];
        Vec vertex0;
        Vec vertex1;
        Vec vertex2;
        Vec vertex3;
        Vec vertex4;
        Vec vertex5;
        Vec vertex6;
        Vec vertex7;
        vertex0.x = corner->x - billboard->halfWidth;
        vertex0.y = corner->y;
        vertex0.z = corner->z;
        nw4r::math::VEC3Add((nw4r::math::VEC3*)&vertex1, (const nw4r::math::VEC3*)corner, (const nw4r::math::VEC3*)&positions[base + 0]);
        nw4r::math::VEC3Add((nw4r::math::VEC3*)&vertex2, (const nw4r::math::VEC3*)corner, (const nw4r::math::VEC3*)&positions[base + 1]);
        vertex3.x = corner->x + billboard->halfWidth;
        vertex3.y = corner->y;
        vertex3.z = corner->z;
        vertex4.x = corner->x;
        vertex4.y = corner->y;
        vertex4.z = corner->z - billboard->halfWidth;
        nw4r::math::VEC3Add((nw4r::math::VEC3*)&vertex5, (const nw4r::math::VEC3*)corner, (const nw4r::math::VEC3*)&positions[base + 2]);
        nw4r::math::VEC3Add((nw4r::math::VEC3*)&vertex6, (const nw4r::math::VEC3*)corner, (const nw4r::math::VEC3*)&positions[base + 3]);
        vertex7.x = corner->x;
        vertex7.y = corner->y;
        vertex7.z = corner->z + billboard->halfWidth;

        GXBegin(GX_QUADS, GX_VTXFMT0, 8);
        GXPosition3f32(vertex0.x, vertex0.y, vertex0.z);
        GXTexCoord1u16(lbl_eu_80665850[value]);
        GXColor1x8(billboard->colors[0]);
        GXPosition3f32(vertex1.x, vertex1.y, vertex1.z);
        GXTexCoord1u16(lbl_eu_80665850[value]);
        GXColor1x8(billboard->colors[1]);
        GXPosition3f32(vertex2.x, vertex2.y, vertex2.z);
        GXTexCoord1u16(lbl_eu_80665850[value]);
        GXColor1x8(billboard->colors[2]);
        GXPosition3f32(vertex3.x, vertex3.y, vertex3.z);
        GXTexCoord1u16(lbl_eu_80665850[value]);
        GXColor1x8(billboard->colors[3]);
        GXPosition3f32(vertex4.x, vertex4.y, vertex4.z);
        GXTexCoord1u16(lbl_eu_80665850[value]);
        GXColor1x8(billboard->colors[0]);
        GXPosition3f32(vertex5.x, vertex5.y, vertex5.z);
        GXTexCoord1u16(lbl_eu_80665850[value]);
        GXColor1x8(billboard->colors[1]);
        GXPosition3f32(vertex6.x, vertex6.y, vertex6.z);
        GXTexCoord1u16(lbl_eu_80665850[value]);
        GXColor1x8(billboard->colors[2]);
        GXPosition3f32(vertex7.x, vertex7.y, vertex7.z);
        GXTexCoord1u16(lbl_eu_80665850[value]);
        GXColor1x8(billboard->colors[3]);
    }
    func_80474CC4__Q26mpfsys17UnkClass_80471EC8Fv();
}

extern "C" void func_8047B85C__Q26mpfsys17MPFDrawBillLayTexFv(mpfsys::MPFDrawBillLayTex*, MPFDrawBillData* billboard, MPFDrawBillIndexList* list) {
    Vec* positions = func_804B5A68();
    if (lbl_eu_80665860 != billboard) {
        func_804737F0__Q26mpfsys17UnkClass_80471EC8Fv(billboard->texIndex, billboard->texScale);
        func_80479F54(billboard, positions);
        lbl_eu_80665860 = billboard;
    }
    if (billboard->flags & 1) {
        func_80474A40__Q26mpfsys17UnkClass_80471EC8Fv();
    } else {
        func_80474AA0__Q26mpfsys17UnkClass_80471EC8Fv();
    }
    func_80474F2C__Q26mpfsys17UnkClass_80471EC8Fv();
    func_80474DF8__Q26mpfsys17UnkClass_80471EC8Fv(billboard->texMap);
    if (billboard->flags & 8) {
        func_80474E68__Q26mpfsys17UnkClass_80471EC8Fv();
    } else {
        func_80474E24__Q26mpfsys17UnkClass_80471EC8Fv();
    }
    func_8047A330(billboard, positions, list);
    func_80474F54__Q26mpfsys17UnkClass_80471EC8Fv();
    func_80474A40__Q26mpfsys17UnkClass_80471EC8Fv();
    GXSetBlendMode((GXBlendMode)1, (GXBlendFactor)4, (GXBlendFactor)1, (GXLogicOp)5);
    GXSetTevOrder((GXTevStageID)0, (GXTexCoordID)0, (GXTexMapID)1, (GXChannelID)4);
    GXSetTevColorIn((GXTevStageID)0, (GXTevColorArg)0xF, (GXTevColorArg)0xC, (GXTevColorArg)8, (GXTevColorArg)0xF);
    GXSetTevColorOp((GXTevStageID)0, (GXTevOp)0, (GXTevBias)0, (GXTevScale)1, (GXBool)1, (GXTevRegID)0);
    func_80474DF8__Q26mpfsys17UnkClass_80471EC8Fv(0);
    func_8047A330(billboard, positions, list);
    func_80474AA0__Q26mpfsys17UnkClass_80471EC8Fv();
    GXSetTevColorIn((GXTevStageID)0, (GXTevColorArg)0xF, (GXTevColorArg)0xA, (GXTevColorArg)8, (GXTevColorArg)0xF);
    GXSetTevColorOp((GXTevStageID)0, (GXTevOp)0, (GXTevBias)0, (GXTevScale)2, (GXBool)1, (GXTevRegID)0);
    GXSetTevOrder((GXTevStageID)0, (GXTexCoordID)0, (GXTexMapID)0, (GXChannelID)4);
}

extern "C" void func_8047B9DC__Q26mpfsys17MPFDrawBillLayTexFv(mpfsys::MPFDrawBillLayTex*, MPFDrawBillData* billboard, MPFDrawBillPairList* list) {
    Vec* positions = func_804B5A68();
    if (lbl_eu_80665860 != billboard) {
        func_804737F0__Q26mpfsys17UnkClass_80471EC8Fv(billboard->texIndex, billboard->texScale);
        func_80479F54(billboard, positions);
        lbl_eu_80665860 = billboard;
    }
    func_80474DF8__Q26mpfsys17UnkClass_80471EC8Fv(billboard->texMap);
    if (billboard->flags & 8) {
        func_80474E68__Q26mpfsys17UnkClass_80471EC8Fv();
    } else {
        func_80474E24__Q26mpfsys17UnkClass_80471EC8Fv();
    }
    func_80474F54__Q26mpfsys17UnkClass_80471EC8Fv();
    func_80474A40__Q26mpfsys17UnkClass_80471EC8Fv();
    func_8047A570(billboard, positions, list);
    GXSetBlendMode((GXBlendMode)1, (GXBlendFactor)4, (GXBlendFactor)1, (GXLogicOp)5);
    GXSetTevOrder((GXTevStageID)0, (GXTexCoordID)0, (GXTexMapID)1, (GXChannelID)4);
    GXSetTevColorIn((GXTevStageID)0, (GXTevColorArg)0xF, (GXTevColorArg)0xC, (GXTevColorArg)8, (GXTevColorArg)0xF);
    GXSetTevColorOp((GXTevStageID)0, (GXTevOp)0, (GXTevBias)0, (GXTevScale)1, (GXBool)1, (GXTevRegID)0);
    func_80474DF8__Q26mpfsys17UnkClass_80471EC8Fv(0);
    func_8047A570(billboard, positions, list);
    GXSetBlendMode((GXBlendMode)1, (GXBlendFactor)4, (GXBlendFactor)5, (GXLogicOp)0);
    GXSetTevColorIn((GXTevStageID)0, (GXTevColorArg)0xF, (GXTevColorArg)0xA, (GXTevColorArg)8, (GXTevColorArg)0xF);
    GXSetTevColorOp((GXTevStageID)0, (GXTevOp)0, (GXTevBias)0, (GXTevScale)2, (GXBool)1, (GXTevRegID)0);
    GXSetTevOrder((GXTevStageID)0, (GXTexCoordID)0, (GXTexMapID)0, (GXChannelID)4);
    func_80474CC4__Q26mpfsys17UnkClass_80471EC8Fv();
}
