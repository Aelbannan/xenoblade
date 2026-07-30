// Auto-scaffolded catalog TU for CriWare/src/sofdec/mwply/mwsfdrna
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void MWSFRNA_SetOutVol(void* self, u32 vol) { SFD_SetOutVol(*(void**)((u8*)self + 0x58), vol); }

void MWSFRNA_GetOutVol(void* self) { SFD_GetOutVol(*(void**)((u8*)self + 0x58)); }
