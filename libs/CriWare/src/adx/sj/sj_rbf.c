// Auto-scaffolded catalog TU for CriWare/src/adx/sj/sj_rbf
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern char lbl_eu_80518BC8[];
void SJRBF_Error(void) {
    SJERR_CallErr(lbl_eu_80518BC8);
}

void SJRBF_Init() {}

void SJRBF_Finish() {}

void SJRBF_Create() {}

void sjrbf_Create() {}

void SJRBF_Destroy() {}

void SJRBF_GetUuid() {}

void SJRBF_EntryErrFunc() {}

extern void SJCRS_Lock(void);
extern void SJCRS_Unlock(void);
extern void sjrbf_Reset(void*);

void SJRBF_Reset(void* self) {
    SJCRS_Lock();
    sjrbf_Reset(self);
    SJCRS_Unlock();
}

void sjrbf_Reset() {}

void fn_80397A74() {}

void SJRBF_GetChunk() {}

void sjrbf_GetChunk() {}

void SJRBF_PutChunk() {}

void sjrbf_PutChunk() {}

void SJRBF_UngetChunk() {}

void sjrbf_UngetChunk() {}

void SJRBF_IsGetChunk() {}

void sjrbf_IsGetChunk() {}

void SJRBF_GetBufPtr() {}

void SJRBF_GetBufSize() {}

void SJRBF_GetXtrSize() {}

void SJRBF_GetFlowCnt() {}
