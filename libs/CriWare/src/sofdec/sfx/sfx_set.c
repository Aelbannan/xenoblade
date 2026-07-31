// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfx/sfx_set
#include <harness_catalog.h>
#include "libs/CriWare/src/sofdec/sfx/sfx_types.h"

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

extern u8 lbl_eu_8051CF28[];
extern int SJ_SearchTag(u32*, const u8*, const u8*, u32*);

void SFX_SetTagInf(SFXHandleState* self, u32 val1, u32 val2) {
    void* zmv;
    u32 input[2];
    u32 output[2];

    zmv = self->zmv;
    self->tagVal1 = val1;
    self->tagVal2 = val2;
    input[0] = val1;
    input[1] = val2;
    SJ_SearchTag(input, lbl_eu_8051CF28, lbl_eu_8051CF28 + 5, output);
    SFXZ_SetTagInf(zmv, output[0], output[1]);
    self->tagFlag = 1;
}

void SFX_GetTagInf(SFXHandleState* self, u32* out1, u32* out2) {
    if (self->tagFlag == 1) {
        *out1 = self->tagVal1;
        *out2 = self->tagVal2;
    } else {
        *out1 = 0;
        *out2 = 0;
    }
}

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

void SFX_ShiftYccPtrByPix(SFXHandleState* self, u32 pixX, u32 pixY) {
    s32 div2X = (s32)pixX / 2;
    s32 div2Y = (s32)pixY / 2;

    self->field_0x04 += div2X * self->width + pixX;
    self->height -= div2X;
    self->field_0x10 += div2Y * self->field_0x0C + pixY;
    self->field_0x1C -= div2Y;
    self->field_0x24 += div2Y * self->bufStride;
    self->field_0x2C -= div2Y;
}
