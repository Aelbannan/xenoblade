// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_sfa
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

u32 sfa_init_cnt;

void SFA_Init(void) {
    sfa_init_cnt++;
}

void SFA_Finish(void) {
    sfa_init_cnt--;
}
