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

void MWSTM_Create(void) {}

void MWSTM_Destroy(void) {
    ADXSTM_Destroy();
}

void MWSTM_SetFileRange() {}

void MWSTM_ReqStart(void) {
    ADXSTM_Start();
}

void MWSTM_ReqStop() {}

void MWSTM_GetStat(void) {
    ADXSTM_GetStat();
}
