// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/main/bte_init
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void BTE_InitStack() {
    RFCOMM_Init();
    GAP_Init();
    HID_DevInit();
    HID_HostInit();
}
