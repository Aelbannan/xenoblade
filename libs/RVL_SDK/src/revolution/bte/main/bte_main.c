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
extern tHCI_CFG bte_hcisu_h2_cfg;
extern void bte_hcisu_task(int);
extern void bte_hcisu_close(void);
extern void btu_task_init(void);
extern void btu_task_msg_handler(void);
extern char __BTUInterruptHandlerStack[];

extern tBTE_APP_INFO_CB _bte_app_info;

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
