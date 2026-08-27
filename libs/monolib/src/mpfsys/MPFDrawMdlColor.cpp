// monolib model draw dispatchers (with and without vertex colors).
//
// The four draw routines are referenced from the draw-object tables in
// monolibdata2 and dispatched through cast function pointers, so the retail
// binary mangles them `Fv` (void parameters) even though the dispatcher
// passes (data, list) in r4/r5.  They are reconstructed here as extern "C"
// definitions carrying the retail symbol names with their real signatures,
// the same technique nw4r uses for e.g. GetTypeName__Q34nw4r3g3d6ScnObjCFv.
//
// Local variables are declared (uninitialized) at the top of each function
// in the order the retail register allocation implies: MWCC hands out
// callee-saved GPRs from r31 downwards in declaration order.

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

extern s8 lbl_eu_80665898;                       // MPFDrawMdlColor singleton init flag
extern mpfsys::MPFDrawMdlColor* lbl_eu_8066589C; // MPFDrawMdlColor singleton slot
extern u8* lbl_eu_80665840;                      // model data arena base
extern MPFDrawMdlData* lbl_eu_80665860;          // last draw data handed to the unit

// TEV / texture pipeline helpers implemented by mpfsys::UnkClass_80471EC8.
// Retail mangles them as Fv members but they are invoked with the arguments
// below (see the dispatch comment at the top of this file).
void bindTexture__Q26mpfsys17UnkClass_80471EC8Fif(u16 texIdx, const MPFDrawMdlMat* mat, f32 lodBias);
void setupGfxMode1__Q26mpfsys17UnkClass_80471EC8Fv(void);
void setupGfxMode2__Q26mpfsys17UnkClass_80471EC8Fv(void);
void setupGfxMode3__Q26mpfsys17UnkClass_80471EC8Fv(void);
void setupGfxMode4__Q26mpfsys17UnkClass_80471EC8Fv(void);
void enableAlphaBlend__Q26mpfsys17UnkClass_80471EC8Fv(void);
void disableAlphaBlend__Q26mpfsys17UnkClass_80471EC8Fv(void);
void func_80474B00__Q26mpfsys17UnkClass_80471EC8Fv(mpfsys::UnkClass_80471EC8* group, s32 tevStage, f32 scale, u8 mtxSelect);
void loadTevKColor__Q26mpfsys17UnkClass_80471EC8Fv(void);
void applyMatColor__Q26mpfsys17UnkClass_80471EC8Fv(mpfsys::UnkClass_80471EC8* group);
void applyTevColor__Q26mpfsys17UnkClass_80471EC8Fv(mpfsys::UnkClass_80471EC8* group);
void setKeyAlpha__Q26mpfsys17UnkClass_80471EC8Fv(f32 param);
void setFogIndex__Q26mpfsys17UnkClass_80471EC8Fv(u8 texMapId);
void resetAmbient__Q26mpfsys17UnkClass_80471EC8Fv(void);
void applyAmbient__Q26mpfsys17UnkClass_80471EC8Fv(void);
void disableZMode__Q26mpfsys17UnkClass_80471EC8Fv(void);
void enableZMode__Q26mpfsys17UnkClass_80471EC8Fv(void);

} // extern "C"

// Foreign base-list label referenced by both param blocks (MPFDrawBillboard).
extern "C" u32 lbl_eu_80663870;
// Forward decls of the in-TU draw helpers referenced by the dispatch tables.
extern "C" void drawAllNoColor__Q26mpfsys17MPFDrawMdlNoColorFv(mpfsys::MPFDrawMdlNoColor* self, MPFDrawMdlData* data, const MPFDrawMdlGroupList* list);
extern "C" void drawHalfNoColor__Q26mpfsys17MPFDrawMdlNoColorFv(mpfsys::MPFDrawMdlNoColor* self, MPFDrawMdlData* data, const MPFDrawMdlGroupPairList* list);
extern "C" void drawAllWithColor__Q26mpfsys15MPFDrawMdlColorFv(mpfsys::MPFDrawMdlColor* self, MPFDrawMdlData* data, const MPFDrawMdlGroupList* list);
extern "C" void drawHalfWithColor__Q26mpfsys15MPFDrawMdlColorFv(mpfsys::MPFDrawMdlColor* self, MPFDrawMdlData* data, const MPFDrawMdlGroupPairList* list);

// === .rodata size=0x38 align=8 ===
extern "C" __declspec(align(8)) const char lbl_eu_80523DC8[24] = {
    0x6D,0x70,0x66,0x73,0x79,0x73,0x3A,0x3A,0x4D,0x50,0x46,0x44,0x72,0x61,0x77,0x4D,
    0x64,0x6C,0x43,0x6F,0x6C,0x6F,0x72,0x00,
};
extern "C" __declspec(align(8)) const char lbl_eu_80523DE0[32] = {
    0x6D,0x70,0x66,0x73,0x79,0x73,0x3A,0x3A,0x4D,0x50,0x46,0x44,0x72,0x61,0x77,0x4D,
    0x64,0x6C,0x4E,0x6F,0x43,0x6F,0x6C,0x6F,0x72,0x00, 0x00,0x00,0x00,0x00,0x00,0x00,
};

// === .sdata size=0x10 align=8 ===
// RTTI locators: {name-ptr, base-list-in-data}.
extern "C" u32 lbl_eu_8056DBD0[4];
extern "C" u32 lbl_eu_8056DBF0[4];
extern "C" u32 lbl_eu_80663878[2] = { (u32)&lbl_eu_80523DC8, (u32)&lbl_eu_8056DBD0 };
extern "C" u32 lbl_eu_80663880[2] = { (u32)&lbl_eu_80523DE0, (u32)&lbl_eu_8056DBF0 };

// === .data size=0x40 align=8 ===
// MPFDrawMdlColor dispatch table (16B).
extern "C" u32 lbl_eu_8056DBC0[4] = {
    (u32)&lbl_eu_80663878, 0x00000000, (u32)&drawAllWithColor__Q26mpfsys15MPFDrawMdlColorFv, (u32)&drawHalfWithColor__Q26mpfsys15MPFDrawMdlColorFv,
};
// MPFDrawMdlColor base-list tail (16B).
extern "C" u32 lbl_eu_8056DBD0[4] = {
    (u32)&lbl_eu_80663870, 0x00000000, 0x00000000, 0x00000000,
};
// MPFDrawMdlNoColor dispatch table (16B).
extern "C" u32 lbl_eu_8056DBE0[4] = {
    (u32)&lbl_eu_80663880, 0x00000000, (u32)&drawAllNoColor__Q26mpfsys17MPFDrawMdlNoColorFv, (u32)&drawHalfNoColor__Q26mpfsys17MPFDrawMdlNoColorFv,
};
// MPFDrawMdlNoColor base-list tail (16B).
extern "C" u32 lbl_eu_8056DBF0[4] = {
    (u32)&lbl_eu_80663870, 0x00000000, 0x00000000, 0x00000000,
};

// === .sbss size=0x8 align=8 (zero-fill) ===
extern "C" s8 lbl_eu_80665898 = 0;
extern "C" mpfsys::MPFDrawMdlColor* lbl_eu_8066589C = 0;

// Instance body, defined by monolibdata2 (.data, 0x10 bytes).

namespace mpfsys {

MPFDrawMdlColor* MPFDrawMdlColor::getInstance(){
    if(!lbl_eu_80665898){
        lbl_eu_8066589C = (mpfsys::MPFDrawMdlColor*)lbl_eu_8056DBC0;
        lbl_eu_80665898 = 1;
    }
    return (mpfsys::MPFDrawMdlColor*)&lbl_eu_8066589C;
}

} // namespace mpfsys

// mpfsys::MPFDrawMdlNoColor::drawAllNoColor() - draw every group, no vertex
// colors.
extern "C" void drawAllNoColor__Q26mpfsys17MPFDrawMdlNoColorFv(mpfsys::MPFDrawMdlNoColor* self, MPFDrawMdlData* data, const MPFDrawMdlGroupList* list) {
    mpfsys::UnkClass_80471EC8* const* slot;
    const MPFDrawMdlPolyNoColor* poly;
    const MPFDrawMdlPolyNoColor* polys;
    const MPFDrawMdlMat* mats;
    s32 tevStage;
    u32 i;
    s32 j;

    polys = (const MPFDrawMdlPolyNoColor*)(lbl_eu_80665840 + data->polyListOff);
    mats = (const MPFDrawMdlMat*)(lbl_eu_80665840 + data->matListOff);
    lbl_eu_80665860 = data;
    GXSetCurrentMtx(3);

    if (data->flags & 2) {
        setupGfxMode3__Q26mpfsys17UnkClass_80471EC8Fv();
        tevStage = 3;
    } else {
        setupGfxMode1__Q26mpfsys17UnkClass_80471EC8Fv();
        tevStage = 0;
    }

    if (data->flags & 1) {
        enableAlphaBlend__Q26mpfsys17UnkClass_80471EC8Fv();
    } else {
        disableAlphaBlend__Q26mpfsys17UnkClass_80471EC8Fv();
    }

    disableZMode__Q26mpfsys17UnkClass_80471EC8Fv();

    slot = list->groups;
    for (i = 1; i < list->count; slot++, i++) {
        func_80474B00__Q26mpfsys17UnkClass_80471EC8Fv(*slot, tevStage, data->scale, data->mtxSelect);
        applyMatColor__Q26mpfsys17UnkClass_80471EC8Fv(*slot);

        poly = polys;
        for (j = 0; j < data->polyCount; poly++, j++) {
            setFogIndex__Q26mpfsys17UnkClass_80471EC8Fv(mats[poly->matIdx].texMapId);
            if (mats[poly->matIdx].texFlags & 8) {
                applyAmbient__Q26mpfsys17UnkClass_80471EC8Fv();
            } else {
                resetAmbient__Q26mpfsys17UnkClass_80471EC8Fv();
            }
            bindTexture__Q26mpfsys17UnkClass_80471EC8Fif(mats[poly->matIdx].texIdx, &mats[poly->matIdx], mats[poly->matIdx].lodBias);
            GXSetArray(GX_VA_POS, lbl_eu_80665840 + poly->posOff, 12);
            GXSetArray(GX_VA_TEX0, lbl_eu_80665840 + poly->texOff, 8);
            GXCallDisplayList(lbl_eu_80665840 + poly->dlOff, poly->dlSize);
        }
    }

    GXSetCurrentMtx(0);
}

// mpfsys::MPFDrawMdlNoColor::drawHalfNoColor() - draw the first half of the
// groups (pair list), no vertex colors.
extern "C" void drawHalfNoColor__Q26mpfsys17MPFDrawMdlNoColorFv(mpfsys::MPFDrawMdlNoColor* self, MPFDrawMdlData* data, const MPFDrawMdlGroupPairList* list) {
    const MPFDrawMdlPolyNoColor* poly;
    const MPFDrawMdlPolyNoColor* polys;
    const MPFDrawMdlMat* mats;
    s32 tevStage;
    s32 pairCount;
    const MPFDrawMdlGroupPair* pair;
    s32 i;
    s32 j;

    polys = (const MPFDrawMdlPolyNoColor*)(lbl_eu_80665840 + data->polyListOff);
    mats = (const MPFDrawMdlMat*)(lbl_eu_80665840 + data->matListOff);
    GXSetCurrentMtx(3);
    lbl_eu_80665860 = data;

    if (data->flags & 2) {
        setupGfxMode3__Q26mpfsys17UnkClass_80471EC8Fv();
        tevStage = 3;
    } else {
        setupGfxMode1__Q26mpfsys17UnkClass_80471EC8Fv();
        tevStage = 0;
    }

    enableZMode__Q26mpfsys17UnkClass_80471EC8Fv();
    enableAlphaBlend__Q26mpfsys17UnkClass_80471EC8Fv();

    pair = list->pairs;
    pairCount = list->count / 2;
    for (i = 0; i < pairCount; i++, pair++) {
        setKeyAlpha__Q26mpfsys17UnkClass_80471EC8Fv(pair->param);
        func_80474B00__Q26mpfsys17UnkClass_80471EC8Fv(pair->obj, tevStage, data->scale, data->mtxSelect);
        applyMatColor__Q26mpfsys17UnkClass_80471EC8Fv(pair->obj);

        poly = polys;
        for (j = 0; j < data->polyCount; poly++, j++) {
            setFogIndex__Q26mpfsys17UnkClass_80471EC8Fv(mats[poly->matIdx].texMapId);
            if (mats[poly->matIdx].texFlags & 8) {
                applyAmbient__Q26mpfsys17UnkClass_80471EC8Fv();
            } else {
                resetAmbient__Q26mpfsys17UnkClass_80471EC8Fv();
            }
            bindTexture__Q26mpfsys17UnkClass_80471EC8Fif(mats[poly->matIdx].texIdx, &mats[poly->matIdx], mats[poly->matIdx].lodBias);
            GXSetArray(GX_VA_POS, lbl_eu_80665840 + poly->posOff, 12);
            GXSetArray(GX_VA_TEX0, lbl_eu_80665840 + poly->texOff, 8);
            GXCallDisplayList(lbl_eu_80665840 + poly->dlOff, poly->dlSize);
        }
    }

    loadTevKColor__Q26mpfsys17UnkClass_80471EC8Fv();
    GXSetCurrentMtx(0);
}

// mpfsys::MPFDrawMdlColor::drawAllWithColor() - draw every group with vertex
// colors.
extern "C" void drawAllWithColor__Q26mpfsys15MPFDrawMdlColorFv(mpfsys::MPFDrawMdlColor* self, MPFDrawMdlData* data, const MPFDrawMdlGroupList* list) {
    mpfsys::UnkClass_80471EC8* const* slot;
    const MPFDrawMdlPolyColor* poly;
    const MPFDrawMdlPolyColor* polys;
    const MPFDrawMdlMat* mats;
    s32 tevStage;
    u32 i;
    s32 j;

    polys = (const MPFDrawMdlPolyColor*)(lbl_eu_80665840 + data->polyListOff);
    mats = (const MPFDrawMdlMat*)(lbl_eu_80665840 + data->matListOff);
    lbl_eu_80665860 = data;
    GXSetCurrentMtx(3);

    if (data->flags & 2) {
        setupGfxMode4__Q26mpfsys17UnkClass_80471EC8Fv();
        tevStage = 3;
    } else {
        setupGfxMode2__Q26mpfsys17UnkClass_80471EC8Fv();
        tevStage = 0;
    }

    disableZMode__Q26mpfsys17UnkClass_80471EC8Fv();

    if (data->flags & 1) {
        enableAlphaBlend__Q26mpfsys17UnkClass_80471EC8Fv();
    } else {
        disableAlphaBlend__Q26mpfsys17UnkClass_80471EC8Fv();
    }

    slot = list->groups;
    for (i = 1; i < list->count; slot++, i++) {
        func_80474B00__Q26mpfsys17UnkClass_80471EC8Fv(*slot, tevStage, data->scale, data->mtxSelect);
        applyTevColor__Q26mpfsys17UnkClass_80471EC8Fv(*slot);

        poly = polys;
        for (j = 0; j < data->polyCount; poly++, j++) {
            setFogIndex__Q26mpfsys17UnkClass_80471EC8Fv(mats[poly->matIdx].texMapId);
            if (mats[poly->matIdx].texFlags & 8) {
                applyAmbient__Q26mpfsys17UnkClass_80471EC8Fv();
            } else {
                resetAmbient__Q26mpfsys17UnkClass_80471EC8Fv();
            }
            bindTexture__Q26mpfsys17UnkClass_80471EC8Fif(mats[poly->matIdx].texIdx, &mats[poly->matIdx], mats[poly->matIdx].lodBias);
            GXSetArray(GX_VA_POS, lbl_eu_80665840 + poly->posOff, 12);
            GXSetArray(GX_VA_CLR0, lbl_eu_80665840 + poly->clrOff, 3);
            GXSetArray(GX_VA_TEX0, lbl_eu_80665840 + poly->texOff, 8);
            GXCallDisplayList(lbl_eu_80665840 + poly->dlOff, poly->dlSize);
        }
    }

    GXSetCurrentMtx(0);
}

// mpfsys::MPFDrawMdlColor::drawHalfWithColor() - draw the first half of the
// groups (pair list) with vertex colors.
extern "C" void drawHalfWithColor__Q26mpfsys15MPFDrawMdlColorFv(mpfsys::MPFDrawMdlColor* self, MPFDrawMdlData* data, const MPFDrawMdlGroupPairList* list) {
    const MPFDrawMdlPolyColor* poly;
    const MPFDrawMdlPolyColor* polys;
    const MPFDrawMdlMat* mats;
    s32 tevStage;
    s32 pairCount;
    const MPFDrawMdlGroupPair* pair;
    s32 i;
    s32 j;

    polys = (const MPFDrawMdlPolyColor*)(lbl_eu_80665840 + data->polyListOff);
    mats = (const MPFDrawMdlMat*)(lbl_eu_80665840 + data->matListOff);
    lbl_eu_80665860 = data;
    GXSetCurrentMtx(3);

    if (data->flags & 2) {
        setupGfxMode4__Q26mpfsys17UnkClass_80471EC8Fv();
        tevStage = 3;
    } else {
        setupGfxMode2__Q26mpfsys17UnkClass_80471EC8Fv();
        tevStage = 0;
    }

    enableZMode__Q26mpfsys17UnkClass_80471EC8Fv();
    enableAlphaBlend__Q26mpfsys17UnkClass_80471EC8Fv();

    pair = list->pairs;
    pairCount = list->count / 2;
    for (i = 0; i < pairCount; i++, pair++) {
        setKeyAlpha__Q26mpfsys17UnkClass_80471EC8Fv(pair->param);
        func_80474B00__Q26mpfsys17UnkClass_80471EC8Fv(pair->obj, tevStage, data->scale, data->mtxSelect);
        applyTevColor__Q26mpfsys17UnkClass_80471EC8Fv(pair->obj);

        poly = polys;
        for (j = 0; j < data->polyCount; poly++, j++) {
            setFogIndex__Q26mpfsys17UnkClass_80471EC8Fv(mats[poly->matIdx].texMapId);
            if (mats[poly->matIdx].texFlags & 8) {
                applyAmbient__Q26mpfsys17UnkClass_80471EC8Fv();
            } else {
                resetAmbient__Q26mpfsys17UnkClass_80471EC8Fv();
            }
            bindTexture__Q26mpfsys17UnkClass_80471EC8Fif(mats[poly->matIdx].texIdx, &mats[poly->matIdx], mats[poly->matIdx].lodBias);
            GXSetArray(GX_VA_POS, lbl_eu_80665840 + poly->posOff, 12);
            GXSetArray(GX_VA_CLR0, lbl_eu_80665840 + poly->clrOff, 3);
            GXSetArray(GX_VA_TEX0, lbl_eu_80665840 + poly->texOff, 8);
            GXCallDisplayList(lbl_eu_80665840 + poly->dlOff, poly->dlSize);
        }
    }

    loadTevKColor__Q26mpfsys17UnkClass_80471EC8Fv();
    GXSetCurrentMtx(0);
}
