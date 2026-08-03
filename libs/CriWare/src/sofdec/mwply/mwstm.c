// Auto-scaffolded catalog TU for CriWare/src/sofdec/mwply/mwstm
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

int MWSTM_SetTrSct(void) { return 0x0; }

int MWSTM_IsFsStatErr(void) {
    extern int ADXSTM_GetStat(void);
    int stat = ADXSTM_GetStat();
    return (stat == 4) ? 1 : 0;
}

void ADXSTM_SetBufSize(void* a);
void MWSTM_SetFlowLimit(void* a) {
    if (a != NULL) {
        ADXSTM_SetBufSize(a);
    }
}

void MWSTM_GetReadFlg(void) {
    ADXSTM_IsOpenedFile();
}

extern void* ADXSTM_Create(void*, int);
void* MWSTM_Create(void* a) { return ADXSTM_Create(a, 0); }

void MWSTM_Destroy(void) {
    ADXSTM_Destroy();
}

extern void ADXSTM_SetEos(void* stm, s32 a);
extern s32 ADXSTM_BindFileNw(void* stm, s32 a, s32 b, s32 c, s64 d);
extern void ADXSTM_ReleaseFileNw(void*);

void MWSTM_SetFileRange(void* a, s32 b, s32 c, s32 d, s32 e) {
    ADXSTM_ReleaseFileNw(a);
    ADXSTM_BindFileNw(a, b, c, d, ((s64)e) << 11);
    ADXSTM_SetEos(a, e);
}

void MWSTM_ReqStart(void) {
    ADXSTM_Start();
}

extern void ADXSTM_StopNw(void*);
extern void ADXSTM_ReleaseFileNw(void*);

void MWSTM_ReqStop(void* self) {
    ADXSTM_StopNw(self);
    ADXSTM_ReleaseFileNw(self);
}

void MWSTM_GetStat(void) {
    ADXSTM_GetStat();
}
