// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfx/sfx_set
#include <harness_catalog.h>
#include "libs/CriWare/src/sofdec/sfx/sfx_types.h"

void SFX_SetCompoMode(void* self, u32 val) { ((SFXSetState*)self)->compoMode = val; }
void SFX_SetOutBufSize(void* self, u32 val1, u32 val2) { *(u32*)((u8*)self + 0x08) = val1; *(u32*)((u8*)self + 0x0C) = val2; }
void SFX_SetUnitWidth(void* self, u32 val) { ((SFXSetState*)self)->unitWidth = val; }
extern u8 lbl_eu_8051CF28[];
extern int SJ_SearchTag(u32*, const u8*, const u8*, u32*);

void SFX_SetTagInf(void* self, u32 val1, u32 val2) {
    void* zmv;
    u32 input[2];
    u32 output[2];
    int found;

    zmv = *(void**)((u8*)self + 0x24);
    *(u32*)((u8*)self + 0x18) = val1;
    *(u32*)((u8*)self + 0x1c) = val2;

    input[0] = val1;
    input[1] = val2;
    found = SJ_SearchTag(input, lbl_eu_8051CF28, lbl_eu_8051CF28 + 5, output);

    if (found == 0) {
        SFXZ_SetTagInf(zmv, 0, 0);
    } else {
        SFXZ_SetTagInf(zmv, output[0], output[1]);
    }

    *(u32*)((u8*)self + 0x14) = 1;
}
void SFX_GetTagInf(void* self, u32* out1, u32* out2) {
    if (*(s32*)((u8*)self + 0x14) == 1) goto if_body;
    *out1 = 0;
    *out2 = 0;
    return;
if_body:
    *out1 = *(u32*)((u8*)self + 0x18);
    *out2 = *(u32*)((u8*)self + 0x1C);
}
extern u32 SFXZ_GetZfrmRange(u32, u32);
void SFX_GetZfrmRange(void* a, void* b) {
    SFXZ_GetZfrmRange(*(u32*)((u8*)a + 0x24), *(u32*)((u8*)b + 0x4c));
}
u32 SFX_GetSplitField(void* self) { return ((SFXSetState*)self)->splitField; }
u32 SFX_GetProgOut(void* self) { return ((SFXSetState*)self)->progOut; }
u32 SFX_GetCnvBottomUp(void* self) { return ((SFXSetState*)self)->cnvBottomUp; }
void SFX_ShiftYccPtrByPix() {}
