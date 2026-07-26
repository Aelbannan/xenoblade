// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/bta/sys/ptim
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void ptim_init(void* p_cb, unsigned long p1, unsigned char p2)
{
    GKI_init_timer_list(p_cb);
    *(unsigned long*)((char*)p_cb + 0xc) = p1;
    *((unsigned char*)p_cb + 0x10) = p2;
}

void ptim_timer_update() {}

void ptim_start_timer() {}

void ptim_stop_timer() {}
