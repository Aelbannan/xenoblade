// CriWare/src/sofdec/sfx/sfx_set - stream attribute setters/getters.
// These wrap simple field stores on the SFX set/handle state plus tag lookup
// against the shared Sofdec SDK tag table ("SFXZ" / "SFXINFE").
#include <harness_catalog.h>
#include "libs/CriWare/src/sofdec/sfx/sfx_types.h"

/* Shared Sofdec SDK tag table (.rodata): "SFXZ\0SFXINFE\0" */
extern u8 lbl_eu_8051CF28[];
/* SJ tag scanner (libs/CriWare/src/adx/sj/sj_utl.c) */
extern int SJ_SearchTag(u32*, const u8*, const u8*, u32*);

void SFX_SetCompoMode(SFXSetState* self, u32 val) {
    self->compoMode = val;
}

void SFX_SetOutBufSize(SFXSetState* self, u32 w, u32 h) {
    *(u32*)((u8*)self + 0x08) = w;
    *(u32*)((u8*)self + 0x0C) = h;
}

void SFX_SetUnitWidth(SFXSetState* self, u32 val) {
    self->unitWidth = val;
}

void SFX_SetTagInf(SFXHandleState* self, u32 val1, u32 val2) {
    void* zmv;
    u32 input[2];
    u32 output[2];
    self->tagVal1 = val1;
    zmv = self->zmv;
    self->tagVal2 = val2;
    input[0] = val1;
    input[1] = val2;
    s32 ret = SJ_SearchTag(input, lbl_eu_8051CF28, lbl_eu_8051CF28 + 5, output);
    if (ret == 0) {
        SFXZ_SetTagInf(zmv, 0, 0);
    } else {
        SFXZ_SetTagInf(zmv, output[0], output[1]);
    }
    /* volatile final store forces MWCC's scheduler to emit the retail LR-first
       epilogue restore order (see MWCC_CASES adx_mwii note) */
    *(volatile u32*)&self->tagFlag = 1;
}

void SFX_GetTagInf(SFXHandleState* self, u32* out1, u32* out2) {
    if ((s32)self->tagFlag != 1) {
        *out1 = 0;
        *out2 = 0;
    } else {
        *out1 = self->tagVal1;
        *out2 = self->tagVal2;
    }
}

/* ZMV frame-range query (libs/CriWare/src/sofdec/sfx/sfx_zmv.c) */
extern u32 SFXZ_GetZfrmRange(u32, u32);

void SFX_GetZfrmRange(SFXHandleState* self, SFXSetState* other) {
    SFXZ_GetZfrmRange((u32)self->zmv, *(u32*)((u8*)other + 0x4C));
}

u32 SFX_GetSplitField(SFXSetState* self) {
    return self->splitField;
}

u32 SFX_GetProgOut(SFXSetState* self) {
    return self->progOut;
}

u32 SFX_GetCnvBottomUp(SFXSetState* self) {
    return self->cnvBottomUp;
}

/* Advance the Y/Cb/Cr plane pointers by a pixel offset. Offsets are rounded
   toward zero and down to an even value so pointers move by whole 2x2 blocks:
   each plane gains evenOffset * stride + evenOffset elements, while the chroma
   line counts shrink by half the (rounded) Y offset. */
void SFX_ShiftYccPtrByPix(SFXStmInf* self, s32 pixX, s32 pixY) {
    s32 roundY = pixY + ((u32)pixY >> 31);   /* round toward zero */
    s32 roundX = pixX + ((u32)pixX >> 31);
    s32 halfY = ((roundY & ~1) + (s32)((u32)roundY >> 31)) >> 1;

    self->_04 += (roundY & ~1) * self->_08 + (roundX & ~1);
    self->_0C -= roundY & ~1;
    self->_14 += halfY * self->_18 + (((roundX & ~1) + (s32)((u32)roundX >> 31)) >> 1);
    self->_1C -= halfY;
    self->_24 += halfY * self->_28 + (((roundX & ~1) + (s32)((u32)roundX >> 31)) >> 1);
    self->_2C -= halfY;
}
