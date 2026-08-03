// High-level C reconstruction of bta_sys_cfg.c (data-only TU).
// BTA system manager compile-time configuration.
// Values recovered from the retail object
// build/us/obj/RVL_SDK/src/revolution/bte/bta/sys/bta_sys_cfg.o

#include "revolution/bte/bta/sys/bta_sys.h"
#include "revolution/bte/gki/common/gki.h"

/* Local layout matching the retail build (8 bytes .sdata2). */
typedef struct
{
    UINT16  mbox_evt;       /* GKI mailbox event (TASK_MBOX_2_EVT_MASK = 4) */
    UINT8   mbox;           /* GKI mailbox id (TASK_MBOX_2 = 2) */
    UINT8   timer;          /* GKI timer id (TIMER_1 = 1) */
    UINT8   trace_level;    /* initial trace level (retail: 0) */
    UINT8   pad;            /* padding in retail build */
    UINT16  reserved;       /* padding in retail build */
} tBTA_SYS_CFG_RETAIL;

const tBTA_SYS_CFG_RETAIL bta_sys_cfg =
{
    TASK_MBOX_2_EVT_MASK,   /* mbox_evt    */
    TASK_MBOX_2,            /* mbox        */
    TIMER_1,                /* timer       */
    0,                      /* trace_level */
    0,                      /* pad         */
    0                       /* reserved    */
};

// Retail .sdata is 8 bytes: the pointer plus 4 zero pad bytes.
tBTA_SYS_CFG *p_bta_sys_cfg[2] = { (tBTA_SYS_CFG *)&bta_sys_cfg, NULL };
