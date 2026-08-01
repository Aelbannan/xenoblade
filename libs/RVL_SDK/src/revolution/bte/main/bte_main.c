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

/* USB application info callback (set by BTA_Init via BTE_Init); NULL = none */
typedef void (*tBTE_APP_INFO_CB)(int);

extern tBTE_APP_INFO_CB _bte_app_info;

void bta_usb_close_evt(s8 result) {
    tBTE_APP_INFO_CB cb = _bte_app_info;

    if (cb) {
        if (result >= 0) {
            cb(0);
        } else {
            cb(1);
        }
    }
}
