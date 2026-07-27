// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfx/sfx_cnv
#include <harness_catalog.h>
#include "libs/CriWare/src/sofdec/sfx/sfx_types.h"

void SFX_IsMergeField() {}
void sfxcnv_MakeTable() {}
void SFXCNV_MakeCcirFromY() {}
void SFX_SetCnvFrmCbFunc(void* self, u32 val) { ((SFXConvertState*)self)->makeCnvFrameCallback = (void*)val; }
void SFX_SetCopyAlphaCbFunc(void* self, u32 val) { ((SFXConvertState*)self)->copyAlphaCallback = (void*)val; }
void SFX_SetMakeLumiTableCbFunc(void* self, void* cb) { ((SFXConvertState*)self)->alphaState->makeLumiTable = (SFXAlphaCallback)cb; }
void SFX_SetMakeAlp3TableCbFunc(void* self, void* cb) { ((SFXConvertState*)self)->alphaState->makeAlpTable = (SFXAlphaTableCallback)cb; }
void SFX_SetMakeAlp3110TableCbFunc(void* self, void* cb) { ((SFXConvertState*)self)->alphaState->makeAlp3110Table = (SFXAlphaTableCallback)cb; }
void SFX_SetMakeColAdjTableCbFunc(void* self, u32 val) { ((SFXConvertState*)self)->makeColorAdjustCallback = (void*)val; }
void SFX_CnvFrmByCbFunc() {}
void sfxcnv_ExecCnvFrmByCbFunc() {}
void sfxcnv_MakeCftSrcBuf() {}
void sfxcnv_ExecCopyAlphaByCbFunc() {}
void SFX_SetBytePerPixelOutBuf(void* self, u32 val) { ((SFXConvertState*)self)->bytesPerPixelOut = val; }
void sfxcnv_MakeDstBufInf() {}
void SFX_Make2PlaneCftDstBuf() {}
