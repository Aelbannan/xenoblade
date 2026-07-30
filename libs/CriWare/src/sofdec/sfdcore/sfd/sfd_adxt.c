// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_adxt
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern void ADXT_SetOutPan(void*);
void SFADXT_SetOutPan(void* self) {
    ADXT_SetOutPan(*(void**)(*(void**)((u8*)self + 0x20ac)));
}

extern void ADXT_GetOutPan(void*);
void SFADXT_GetOutPan(void* self) {
    ADXT_GetOutPan(*(void**)(*(void**)((u8*)self + 0x20ac)));
}

extern void ADXT_SetOutVol(void*);
void SFADXT_SetOutVol(void* self) {
    ADXT_SetOutVol(*(void**)(*(void**)((u8*)self + 0x20ac)));
}

void ADXT_GetOutVol(void* a);
void SFADXT_GetOutVol(void* self) {
    ADXT_GetOutVol(*(void**)(*(void**)((u8*)self + 0x20ac)));
}

void SFADXT_SetSpeed() {}

extern u32 lbl_eu_80606DE8[7];

void SFD_SetAdxtPara(u32 *src) {
    u32 *dst = lbl_eu_80606DE8;
    dst[0] = src[0];
    dst[1] = src[1];
    u32 tmp = src[2] + 0x1f;
    dst[2] = tmp & ~0x1f;
    dst[3] = src[3];
    dst[4] = src[4];
    dst[5] = src[5];
    tmp = src[6] + 0x1f;
    dst[6] = tmp & ~0x1f;
}

extern void ADXT_Init(void);
extern void SFA_Init(void);
extern int UTY_MemsetDword(u32*, u32, int);

int SFADXT_Init(void) {
    ADXT_Init();
    SFA_Init();
    UTY_MemsetDword((u32*)lbl_eu_80606DE8, 0, 7);
    return 0;
}

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

extern void ADXT_Stop(void*);

u32 SFADXT_Stop(void* self) {
    void* p = *(void**)((u8*)self + 0x20ac);
    ADXT_Stop(*(void**)((u8*)p));
    return 0;
}

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
