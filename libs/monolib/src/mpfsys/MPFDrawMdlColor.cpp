// monolib model draw dispatchers (with and without vertex colors).
//
// The four draw routines are referenced from the draw-object tables in
// monolibdata2 and dispatched through cast function pointers, so the retail
// binary mangles them `Fv` (void parameters) even though the dispatcher
// passes (data, list) in r4/r5.  They are reconstructed here as extern "C"
// definitions carrying the retail symbol names with their real signatures,
// the same technique nw4r uses for e.g. GetTypeName__Q34nw4r3g3d6ScnObjCFv.

#include "monolib/mpfsys/MPFDrawMdlColor.hpp"
#include "monolib/mpfsys/MPFDrawMdlNoColor.hpp"
#include "monolib/mpfsys/UnkClass_80471EC8.hpp"

#include <revolution/GX.h>

// Draw data header handed to the model draw routines (dispatch arg r4).
struct MPFDrawMdlData {
    u32 polyListOff; // +0x00: arena offset of the polygon list
    u32 matListOff;  // +0x04: arena offset of the material table
    u16 polyCount;   // +0x08: polygons drawn per group
    u8 gap0A[4];     // +0x0A
    u8 mtxSelect;    // +0x0E: position-matrix select for the group setup
    u8 gap0F;        // +0x0F
    u32 flags;       // +0x10: bit1 selects the tev stage, bit0 the alpha mode
    f32 scale;       // +0x14: group setup scale
};

// Material table entry (0x10 bytes) at arena + matListOff.
struct MPFDrawMdlMat {
    u16 texIdx;    // +0x00: texture coordinate / matrix index
    u8 texMapId;   // +0x02: texture map id
    u8 texFlags;   // +0x03: bit3 selects the texture wrap mode
    f32 lodBias;   // +0x04
    u8 gap08[8];   // +0x08
};

// Polygon descriptor used by the no-color draws (0x10 bytes).
struct MPFDrawMdlPolyNoColor {
    u32 posOff; // +0x00: arena offset of the position array
    u32 texOff; // +0x04: arena offset of the texcoord array
    u32 dlOff;  // +0x08: arena offset of the display list
    u16 matIdx; // +0x0C
    u16 dlSize; // +0x0E: display list size in bytes
};

// Polygon descriptor used by the vertex-color draws (0x14 bytes).
struct MPFDrawMdlPolyColor {
    u32 posOff; // +0x00
    u32 texOff; // +0x04
    u32 clrOff; // +0x08: arena offset of the color array
    u32 dlOff;  // +0x0C
    u16 matIdx; // +0x10
    u16 dlSize; // +0x12
};

// Group object list for the "draw all groups" variants (894/BA0).
struct MPFDrawMdlGroupList {
    u32 count;                              // +0x00
    mpfsys::UnkClass_80471EC8* groups[1];   // +0x04
};

// Group entry for the "draw half" variants (A1C/D40): object + parameter.
struct MPFDrawMdlGroupPair {
    mpfsys::UnkClass_80471EC8* obj; // +0x00
    f32 param;                      // +0x04
};

struct MPFDrawMdlGroupPairList {
    u32 count;                     // +0x00
    MPFDrawMdlGroupPair pairs[1];  // +0x04
};

extern "C" {

s8 lbl_eu_80665898;                       // MPFDrawMdlColor singleton init flag
mpfsys::MPFDrawMdlColor* lbl_eu_8066589C; // MPFDrawMdlColor singleton slot
u8* lbl_eu_80665840;                      // model data arena base
MPFDrawMdlData* lbl_eu_80665860;          // last draw data handed to the unit

// TEV / texture pipeline helpers implemented by mpfsys::UnkClass_80471EC8.
// Retail mangles them as Fv members but they are invoked with the arguments
// below (see the dispatch comment at the top of this file).
void func_804737CC__Q26mpfsys17UnkClass_80471EC8Fv(u16 texIdx, const MPFDrawMdlMat* mat, f32 lodBias);
void func_804743E0__Q26mpfsys17UnkClass_80471EC8Fv(void);
void func_804744EC__Q26mpfsys17UnkClass_80471EC8Fv(void);
void func_8047466C__Q26mpfsys17UnkClass_80471EC8Fv(void);
void func_80474780__Q26mpfsys17UnkClass_80471EC8Fv(void);
void func_80474A40__Q26mpfsys17UnkClass_80471EC8Fv(void);
void func_80474AA0__Q26mpfsys17UnkClass_80471EC8Fv(void);
void func_80474B00__Q26mpfsys17UnkClass_80471EC8Fv(mpfsys::UnkClass_80471EC8* group, s32 tevStage, f32 scale, u8 mtxSelect);
void func_80474CC4__Q26mpfsys17UnkClass_80471EC8Fv(void);
void func_80474CF4__Q26mpfsys17UnkClass_80471EC8Fv(mpfsys::UnkClass_80471EC8* group);
void func_80474D50__Q26mpfsys17UnkClass_80471EC8Fv(mpfsys::UnkClass_80471EC8* group);
void func_80474DAC__Q26mpfsys17UnkClass_80471EC8Fv(f32 param);
void func_80474DF8__Q26mpfsys17UnkClass_80471EC8Fv(u8 texMapId);
void func_80474E24__Q26mpfsys17UnkClass_80471EC8Fv(void);
void func_80474E68__Q26mpfsys17UnkClass_80471EC8Fv(void);
void func_80474F2C__Q26mpfsys17UnkClass_80471EC8Fv(void);
void func_80474F54__Q26mpfsys17UnkClass_80471EC8Fv(void);

} // extern "C"

// Instance body, defined by monolibdata2 (.data, 0x10 bytes).
extern mpfsys::MPFDrawMdlColor lbl_eu_8056DBC0;

namespace mpfsys {

MPFDrawMdlColor* MPFDrawMdlColor::getInstance(){
    if(!lbl_eu_80665898){
        lbl_eu_8066589C = &lbl_eu_8056DBC0;
        lbl_eu_80665898 = 1;
    }
    return (mpfsys::MPFDrawMdlColor*)&lbl_eu_8066589C;
}

} // namespace mpfsys

// mpfsys::MPFDrawMdlNoColor::func_80479894() — draw every group, no vertex
// colors.
extern "C" void func_80479894__Q26mpfsys17MPFDrawMdlNoColorFv(mpfsys::MPFDrawMdlNoColor* self, MPFDrawMdlData* data, const MPFDrawMdlGroupList* list) {
    const MPFDrawMdlPolyNoColor* polys = (const MPFDrawMdlPolyNoColor*)(lbl_eu_80665840 + data->polyListOff);
    const MPFDrawMdlMat* mats = (const MPFDrawMdlMat*)(lbl_eu_80665840 + data->matListOff);
    lbl_eu_80665860 = data;
    GXSetCurrentMtx(3);

    s32 tevStage;
    if (data->flags & 2) {
        func_8047466C__Q26mpfsys17UnkClass_80471EC8Fv();
        tevStage = 3;
    } else {
        func_804743E0__Q26mpfsys17UnkClass_80471EC8Fv();
        tevStage = 0;
    }

    if (data->flags & 1) {
        func_80474A40__Q26mpfsys17UnkClass_80471EC8Fv();
    } else {
        func_80474AA0__Q26mpfsys17UnkClass_80471EC8Fv();
    }

    func_80474F2C__Q26mpfsys17UnkClass_80471EC8Fv();

    mpfsys::UnkClass_80471EC8* const* slot = list->groups;
    for (u32 i = 1; i < list->count; i++, slot++) {
        mpfsys::UnkClass_80471EC8* group = *slot;
        func_80474B00__Q26mpfsys17UnkClass_80471EC8Fv(group, tevStage, data->scale, data->mtxSelect);
        func_80474CF4__Q26mpfsys17UnkClass_80471EC8Fv(group);

        const MPFDrawMdlPolyNoColor* poly = polys;
        for (s32 j = 0; j < data->polyCount; j++, poly++) {
            func_80474DF8__Q26mpfsys17UnkClass_80471EC8Fv(mats[poly->matIdx].texMapId);
            if (mats[poly->matIdx].texFlags & 8) {
                func_80474E68__Q26mpfsys17UnkClass_80471EC8Fv();
            } else {
                func_80474E24__Q26mpfsys17UnkClass_80471EC8Fv();
            }
            func_804737CC__Q26mpfsys17UnkClass_80471EC8Fv(mats[poly->matIdx].texIdx, &mats[poly->matIdx], mats[poly->matIdx].lodBias);
            GXSetArray(GX_VA_POS, lbl_eu_80665840 + poly->posOff, 12);
            GXSetArray(GX_VA_TEX0, lbl_eu_80665840 + poly->texOff, 8);
            GXCallDisplayList(lbl_eu_80665840 + poly->dlOff, poly->dlSize);
        }
    }

    GXSetCurrentMtx(0);
}

// mpfsys::MPFDrawMdlNoColor::func_80479A1C() — draw the first half of the
// groups (pair list), no vertex colors.
extern "C" void func_80479A1C__Q26mpfsys17MPFDrawMdlNoColorFv(mpfsys::MPFDrawMdlNoColor* self, MPFDrawMdlData* data, const MPFDrawMdlGroupPairList* list) {
    const MPFDrawMdlPolyNoColor* polys = (const MPFDrawMdlPolyNoColor*)(lbl_eu_80665840 + data->polyListOff);
    const MPFDrawMdlMat* mats = (const MPFDrawMdlMat*)(lbl_eu_80665840 + data->matListOff);
    GXSetCurrentMtx(3);
    lbl_eu_80665860 = data;

    s32 tevStage;
    if (data->flags & 2) {
        func_8047466C__Q26mpfsys17UnkClass_80471EC8Fv();
        tevStage = 3;
    } else {
        func_804743E0__Q26mpfsys17UnkClass_80471EC8Fv();
        tevStage = 0;
    }

    func_80474F54__Q26mpfsys17UnkClass_80471EC8Fv();
    func_80474A40__Q26mpfsys17UnkClass_80471EC8Fv();

    const MPFDrawMdlGroupPair* pair = list->pairs;
    s32 pairCount = list->count / 2;
    for (s32 i = 0; i < pairCount; i++, pair++) {
        func_80474DAC__Q26mpfsys17UnkClass_80471EC8Fv(pair->param);
        func_80474B00__Q26mpfsys17UnkClass_80471EC8Fv(pair->obj, tevStage, data->scale, data->mtxSelect);
        func_80474CF4__Q26mpfsys17UnkClass_80471EC8Fv(pair->obj);

        const MPFDrawMdlPolyNoColor* poly = polys;
        for (s32 j = 0; j < data->polyCount; j++, poly++) {
            func_80474DF8__Q26mpfsys17UnkClass_80471EC8Fv(mats[poly->matIdx].texMapId);
            if (mats[poly->matIdx].texFlags & 8) {
                func_80474E68__Q26mpfsys17UnkClass_80471EC8Fv();
            } else {
                func_80474E24__Q26mpfsys17UnkClass_80471EC8Fv();
            }
            func_804737CC__Q26mpfsys17UnkClass_80471EC8Fv(mats[poly->matIdx].texIdx, &mats[poly->matIdx], mats[poly->matIdx].lodBias);
            GXSetArray(GX_VA_POS, lbl_eu_80665840 + poly->posOff, 12);
            GXSetArray(GX_VA_TEX0, lbl_eu_80665840 + poly->texOff, 8);
            GXCallDisplayList(lbl_eu_80665840 + poly->dlOff, poly->dlSize);
        }
    }

    func_80474CC4__Q26mpfsys17UnkClass_80471EC8Fv();
    GXSetCurrentMtx(0);
}

// mpfsys::MPFDrawMdlColor::func_80479BA0() — draw every group with vertex
// colors.
extern "C" void func_80479BA0__Q26mpfsys15MPFDrawMdlColorFv(mpfsys::MPFDrawMdlColor* self, MPFDrawMdlData* data, const MPFDrawMdlGroupList* list) {
    const MPFDrawMdlPolyColor* polys = (const MPFDrawMdlPolyColor*)(lbl_eu_80665840 + data->polyListOff);
    const MPFDrawMdlMat* mats = (const MPFDrawMdlMat*)(lbl_eu_80665840 + data->matListOff);
    lbl_eu_80665860 = data;
    GXSetCurrentMtx(3);

    s32 tevStage;
    if (data->flags & 2) {
        func_80474780__Q26mpfsys17UnkClass_80471EC8Fv();
        tevStage = 3;
    } else {
        func_804744EC__Q26mpfsys17UnkClass_80471EC8Fv();
        tevStage = 0;
    }

    func_80474F2C__Q26mpfsys17UnkClass_80471EC8Fv();

    if (data->flags & 1) {
        func_80474A40__Q26mpfsys17UnkClass_80471EC8Fv();
    } else {
        func_80474AA0__Q26mpfsys17UnkClass_80471EC8Fv();
    }

    mpfsys::UnkClass_80471EC8* const* slot = list->groups;
    for (u32 i = 1; i < list->count; i++, slot++) {
        mpfsys::UnkClass_80471EC8* group = *slot;
        func_80474B00__Q26mpfsys17UnkClass_80471EC8Fv(group, tevStage, data->scale, data->mtxSelect);
        func_80474D50__Q26mpfsys17UnkClass_80471EC8Fv(group);

        const MPFDrawMdlPolyColor* poly = polys;
        for (s32 j = 0; j < data->polyCount; j++, poly++) {
            func_80474DF8__Q26mpfsys17UnkClass_80471EC8Fv(mats[poly->matIdx].texMapId);
            if (mats[poly->matIdx].texFlags & 8) {
                func_80474E68__Q26mpfsys17UnkClass_80471EC8Fv();
            } else {
                func_80474E24__Q26mpfsys17UnkClass_80471EC8Fv();
            }
            func_804737CC__Q26mpfsys17UnkClass_80471EC8Fv(mats[poly->matIdx].texIdx, &mats[poly->matIdx], mats[poly->matIdx].lodBias);
            GXSetArray(GX_VA_POS, lbl_eu_80665840 + poly->posOff, 12);
            GXSetArray(GX_VA_CLR0, lbl_eu_80665840 + poly->clrOff, 3);
            GXSetArray(GX_VA_TEX0, lbl_eu_80665840 + poly->texOff, 8);
            GXCallDisplayList(lbl_eu_80665840 + poly->dlOff, poly->dlSize);
        }
    }

    GXSetCurrentMtx(0);
}

// mpfsys::MPFDrawMdlColor::func_80479D40() — draw the first half of the
// groups (pair list) with vertex colors.
extern "C" void func_80479D40__Q26mpfsys15MPFDrawMdlColorFv(mpfsys::MPFDrawMdlColor* self, MPFDrawMdlData* data, const MPFDrawMdlGroupPairList* list) {
    const MPFDrawMdlPolyColor* polys = (const MPFDrawMdlPolyColor*)(lbl_eu_80665840 + data->polyListOff);
    const MPFDrawMdlMat* mats = (const MPFDrawMdlMat*)(lbl_eu_80665840 + data->matListOff);
    lbl_eu_80665860 = data;
    GXSetCurrentMtx(3);

    s32 tevStage;
    if (data->flags & 2) {
        func_80474780__Q26mpfsys17UnkClass_80471EC8Fv();
        tevStage = 3;
    } else {
        func_804744EC__Q26mpfsys17UnkClass_80471EC8Fv();
        tevStage = 0;
    }

    func_80474F54__Q26mpfsys17UnkClass_80471EC8Fv();
    func_80474A40__Q26mpfsys17UnkClass_80471EC8Fv();

    const MPFDrawMdlGroupPair* pair = list->pairs;
    s32 pairCount = list->count / 2;
    for (s32 i = 0; i < pairCount; i++, pair++) {
        func_80474DAC__Q26mpfsys17UnkClass_80471EC8Fv(pair->param);
        func_80474B00__Q26mpfsys17UnkClass_80471EC8Fv(pair->obj, tevStage, data->scale, data->mtxSelect);
        func_80474D50__Q26mpfsys17UnkClass_80471EC8Fv(pair->obj);

        const MPFDrawMdlPolyColor* poly = polys;
        for (s32 j = 0; j < data->polyCount; j++, poly++) {
            func_80474DF8__Q26mpfsys17UnkClass_80471EC8Fv(mats[poly->matIdx].texMapId);
            if (mats[poly->matIdx].texFlags & 8) {
                func_80474E68__Q26mpfsys17UnkClass_80471EC8Fv();
            } else {
                func_80474E24__Q26mpfsys17UnkClass_80471EC8Fv();
            }
            func_804737CC__Q26mpfsys17UnkClass_80471EC8Fv(mats[poly->matIdx].texIdx, &mats[poly->matIdx], mats[poly->matIdx].lodBias);
            GXSetArray(GX_VA_POS, lbl_eu_80665840 + poly->posOff, 12);
            GXSetArray(GX_VA_CLR0, lbl_eu_80665840 + poly->clrOff, 3);
            GXSetArray(GX_VA_TEX0, lbl_eu_80665840 + poly->texOff, 8);
            GXCallDisplayList(lbl_eu_80665840 + poly->dlOff, poly->dlSize);
        }
    }

    func_80474CC4__Q26mpfsys17UnkClass_80471EC8Fv();
    GXSetCurrentMtx(0);
}
