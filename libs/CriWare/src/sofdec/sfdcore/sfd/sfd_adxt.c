// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_adxt
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void SFADXT_SetOutPan(void) {}

void SFADXT_GetOutPan(void) {}

void SFADXT_SetOutVol(void) {}

void ADXT_GetOutVol(void* a);
void SFADXT_GetOutVol(void* self) {
    ADXT_GetOutVol(*(void**)(*(void**)((u8*)self + 0x20ac)));
}

void SFADXT_SetSpeed() {}

void SFD_SetAdxtPara() {}

void SFADXT_Init() {}

int SFADXT_Finish() { SFA_Finish(); ADXT_Finish(); return 0; }

void SFCON_WriteTotSmplQue(void* a, u32 b, u32 c);
void criware_803BD518(void* a, void* b, void* c, u32 d, u32 e) {
    SFCON_WriteTotSmplQue(a, e, d);
}

void SFADXT_ExecServer() {}

void sfadxt_CopyData() {}

void sfadxt_AdjustSync() {}

void sfadxt_ExcludeHdr() {}

void sfadxt_SearchAlign() {}

void sfadxt_ExcludeSilence() {}

void sfadxt_ChkTermFlg() {}

void sfadxt_SetAdxtHd() {}

void SFADXT_Create() {}

void sfadxt_InitInf() {}

void sfadxt_GetTime() {}

void SFADXT_Destroy() {}

int SFADXT_RequestStop(void) { return 0x0; }

void SFADXT_Start() {}

void SFADXT_Stop() {}

void SFADXT_Pause() {}

s32 SFLIB_SetErr(void* h, u32 err_code);
s32 SFADXT_GetWrite(void* h) {
    return SFLIB_SetErr(h, 0xff000c03);
}

s32 SFLIB_SetErr(void* h, u32 err_code);
s32 SFADXT_AddWrite(void* h) {
    return SFLIB_SetErr(h, 0xff000c03);
}

s32 SFLIB_SetErr(void* h, u32 err_code);
s32 SFADXT_GetRead(void* h) {
    return SFLIB_SetErr(h, 0xff000c03);
}

s32 SFLIB_SetErr(void* h, u32 err_code);
s32 SFADXT_AddRead(void* h) {
    return SFLIB_SetErr(h, 0xff000c03);
}

void SFADXT_Seek() {}
