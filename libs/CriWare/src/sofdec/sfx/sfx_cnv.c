// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfx/sfx_cnv
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void SFX_IsMergeField() {}

void sfxcnv_MakeTable() {}

void SFXCNV_MakeCcirFromY() {}

void SFX_SetCnvFrmCbFunc(void* self, u32 val) { *(u32*)((u8*)self + 0x68) = val; }

void SFX_SetCopyAlphaCbFunc(void* self, u32 val) { *(u32*)((u8*)self + 0x6c) = val; }

void SFX_SetMakeLumiTableCbFunc(void* self, void* cb) {
    *(void**)((u8*)*(void**)((u8*)self + 0x30) + 0x18) = cb;
}

void SFX_SetMakeAlp3TableCbFunc(void* self, void* cb) {
    *(void**)((u8*)*(void**)((u8*)self + 0x30) + 0x20) = cb;
}

void SFX_SetMakeAlp3110TableCbFunc(void* self, void* cb) {
    *(void**)((u8*)*(void**)((u8*)self + 0x30) + 0x1c) = cb;
}

void SFX_SetMakeColAdjTableCbFunc(void* self, u32 val) { *(u32*)((u8*)self + 0x70) = val; }

void SFX_CnvFrmByCbFunc() {}

void sfxcnv_ExecCnvFrmByCbFunc() {}

void sfxcnv_MakeCftSrcBuf() {}

void sfxcnv_ExecCopyAlphaByCbFunc() {}

void SFX_SetBytePerPixelOutBuf(void* self, u32 val) { *(u32*)((u8*)self + 0x74) = val; }

void sfxcnv_MakeDstBufInf() {}

void SFX_Make2PlaneCftDstBuf() {}
