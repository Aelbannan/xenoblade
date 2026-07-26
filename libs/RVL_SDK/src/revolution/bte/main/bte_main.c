// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/main/bte_main
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void BTUInterruptHandler() {
    extern void OSSwitchFiber(void *, void *);
    extern char __BTUInterruptHandlerStack[];
    extern void btu_task_msg_handler();
    OSSwitchFiber((void*)btu_task_msg_handler, (void*)(__BTUInterruptHandlerStack + 0x1000));
}

void BTA_Init() {}

void BTA_CleanUp() {}

void bta_usb_close_evt() {}
