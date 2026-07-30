// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfx/sfx_set
#include <harness_catalog.h>
#include "libs/CriWare/src/sofdec/sfx/sfx_types.h"

void SFX_SetCompoMode(void* self, u32 val) { ((SFXSetState*)self)->compoMode = val; }
void SFX_SetOutBufSize(void* self, u32 val1, u32 val2) { *(u32*)((u8*)self + 0x08) = val1; *(u32*)((u8*)self + 0x0C) = val2; }
void SFX_SetUnitWidth(void* self, u32 val) { ((SFXSetState*)self)->unitWidth = val; }
void SFX_SetTagInf() {}
void SFX_GetTagInf(void* self, u32* out1, u32* out2) {
    if (*(s32*)((u8*)self + 0x14) == 1) goto if_body;
    *out1 = 0;
    *out2 = 0;
    return;
if_body:
    *out1 = *(u32*)((u8*)self + 0x18);
    *out2 = *(u32*)((u8*)self + 0x1C);
}
void SFX_GetZfrmRange(void) {}
u32 SFX_GetSplitField(void* self) { return ((SFXSetState*)self)->splitField; }
u32 SFX_GetProgOut(void* self) { return ((SFXSetState*)self)->progOut; }
u32 SFX_GetCnvBottomUp(void* self) { return ((SFXSetState*)self)->cnvBottomUp; }
void SFX_ShiftYccPtrByPix() {}
