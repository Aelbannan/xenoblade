// Auto-scaffolded catalog TU for CriWare/src/adx/ahx/ahx_bsr
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void AHXBSR_Create() {}

void* memset(void* s, int c, size_t n);
void AHXBSR_Destroy(void* self) {
    if (self == NULL) return;
    memset(self, 0, 0x2c);
}

void AHXBSR_Restruct() {}

void ahxbsr_get_data() {}

void AHXBSR_IsDataAvailable() {}

void AHXBSR_GetBitStm() {}

u32 AHXBSR_Tell(void* self) { return *(u32*)((u8*)self + 0x10); }

void AHXBSR_SearchSync() {}
