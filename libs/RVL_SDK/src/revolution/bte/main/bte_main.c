// Decompiled: bte_main.c - BTE (Bluetooth Embedded) main entry points
// High-level C reconstruction of RVL_SDK/src/revolution/bte/main/bte_main

#include <revolution/OS.h>

#include <revolution/BTE/gki/common/gki.h>
#include <revolution/BTE/stack/include/bt_types.h>
#include <revolution/BTE/stack/include/btu.h>
#include <revolution/BTE/bta/include/bta_api.h>
#include <revolution/BTE/rvl/uusb_ppc.h>
#include <revolution/bte/main/bte_hcisu.h>

/* USB application info callback (set by BTA_Init via BTE_Init); NULL = none */
typedef void (*tBTE_APP_INFO_CB)(int);

/* --- externs (defined in sibling bte units) --- */
extern tHCI_IF hcisu_h2;
extern void bte_hcisu_task(int);
extern void bte_hcisu_close(void);
extern void btu_task_init(void);
extern void btu_task_msg_handler(void);

// Defined here to reproduce the retail .bss/.sbss/.sdata allocations.
// bte_target_mode is never referenced in this unit, so MWCC places it first
// in the .sbss group (0x0..0x4), followed by _bte_app_info (0x4..0x8).
volatile u32 bte_target_mode;
tBTE_APP_INFO_CB _bte_app_info;
char __BTUInterruptHandlerStack[0x1000];

// retail .sdata symbol bte_hcisu_h2_cfg is 8 bytes; the header type tHCI_CFG
// is only 4, so back the symbol with an 8-byte array and alias the 4-byte view.
static u8 bte_hcisu_h2_cfg_data[8] = {0x0a, 0x5c, 0x21, 0x01, 0x00, 0x00, 0x00, 0x00};
#define bte_hcisu_h2_cfg (*(tHCI_CFG*)bte_hcisu_h2_cfg_data)

static OSAlarm _bte_alarm;

void BTUInterruptHandler(OSAlarm* pAlarm, OSContext* pCtx) {
    OSSwitchFiber((void*)btu_task_msg_handler,
                  (void*)(__BTUInterruptHandlerStack + 0x1000));
}

BOOLEAN BTA_Init(void) {
    _bte_app_info = NULL;
    GKI_init();
    GKI_enable();
    BTE_Init();
    p_hcisu_if = &hcisu_h2;
    p_hcisu_cfg = &bte_hcisu_h2_cfg;
    bte_hcisu_task(0);

    while (wait4hci) {
        GKI_delay(100);
    }

    btu_task_init();
    OSCreateAlarm(&_bte_alarm);
    OSSetPeriodicAlarm(&_bte_alarm, OSGetTime(), OS_MSEC_TO_TICKS(2),
                       BTUInterruptHandler);
    GKI_run(0);

    while (!BTA_DmIsDeviceUp()) {
        GKI_delay(2000);
    }

    return FALSE;
}

void BTA_CleanUp(tBTE_APP_INFO_CB cb) {
    _bte_app_info = cb;
    OSCancelAlarm(&_bte_alarm);
    bte_hcisu_close();
    GKI_shutdown();
}

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
