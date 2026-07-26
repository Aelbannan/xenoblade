// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfx/sfx_set
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void SFX_SetCompoMode(void* self, u32 val) { *(u32*)((u8*)self + 0x4) = val; }

void SFX_SetOutBufSize(void* self, u32 val1, u32 val2) { }

void SFX_SetUnitWidth(void* self, u32 val) { *(u32*)((u8*)self + 0x10) = val; }

void SFX_SetTagInf() {}

void SFX_GetTagInf() {}

void SFX_GetZfrmRange(void) {}

u32 SFX_GetSplitField(void* self) { return *(u32*)((u8*)self + 0x58); }

u32 SFX_GetProgOut(void* self) { return *(u32*)((u8*)self + 0x5c); }

u32 SFX_GetCnvBottomUp(void* self) { return *(u32*)((u8*)self + 0x64); }

void SFX_ShiftYccPtrByPix() {}
