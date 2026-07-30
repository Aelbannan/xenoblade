// Auto-scaffolded catalog TU for CriWare/src/adx/sj/sj_uni
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern char lbl_eu_80518E00[];
void SJUNI_Error(void) {
    SJERR_CallErr(lbl_eu_80518E00);
}

void SJUNI_Init() {}

void SJUNI_Finish() {}

void SJUNI_Create() {}

void sjuni_Create() {}

void SJUNI_Destroy() {}

void SJUNI_GetUuid() {}

void SJUNI_EntryErrFunc() {}

extern void SJCRS_Lock(void);
extern void SJCRS_Unlock(void);

void sjuni_Reset(void* self) {}

void SJUNI_Reset(void* self) {
    SJCRS_Lock();
    sjuni_Reset(self);
    SJCRS_Unlock();
}

void SJUNI_GetNumData() {}

void sjuni_GetNumData() {}

void SJUNI_GetChunk() {}

void sjuni_GetChunk() {}

void SJUNI_PutChunk() {}

void sjuni_PutChunk() {}

void SJUNI_UngetChunk() {}

void sjuni_UngetChunk() {}

void SJUNI_IsGetChunk() {}

void sjuni_IsGetChunk() {}

void SJUNI_GetNumChunk() {}
