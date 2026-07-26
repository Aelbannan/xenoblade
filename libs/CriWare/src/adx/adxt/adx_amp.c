// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_amp
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void ADXAMP_Destroy(void* p) {
    if (!p) return;
    ADXCRS_Lock();
    memset(p, 0, 0x30);
    ADXCRS_Unlock();
}

void ADXAMP_Start() {}

void ADXAMP_Stop(void) {}

void ADXAMP_SetSfreq(void* self, u32 val) { *(u32*)((u8*)self + 0x20) = val; }
