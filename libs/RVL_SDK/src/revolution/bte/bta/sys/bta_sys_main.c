// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/bta/sys/bta_sys_main
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

#include "revolution/bte/bta/sys/bta_sys.h"

/* bta_sys_cb is a BSS global (0x8C bytes) defined in another TU.
 * Layout at the offsets used by this file:
 *   0x00: reg[26]   per-module registration table (const tBTA_SYS_REG*)
 *   0x68: ptim_cb   periodic timer control block
 *   0x7C: timer_in_use flag
 *   0x7D: task_id
 *   0x7E: immediate-disable flag
 */
typedef struct {
    const tBTA_SYS_REG *reg[26]; /* 0x00 */
    UINT8 ptim_cb[0x14];         /* 0x68 */
    UINT8 timer_in_use;          /* 0x7C */
    UINT8 task_id;               /* 0x7D */
    UINT8 immediate_disable;     /* 0x7E */
    UINT8 _pad[0x8C - 0x7F];     /* 0x7F..0x8B */
} tBTA_SYS_CB;

extern tBTA_SYS_CB bta_sys_cb;

void bta_sys_init(void) {}

void bta_sys_event(BT_HDR *p_msg) {}

void bta_sys_timer_update(void) {}

void bta_sys_register(UINT8 id, const tBTA_SYS_REG *p_reg) {
    bta_sys_cb.reg[id] = p_reg;
}

void bta_sys_sendmsg(void *p_msg) {
    extern unsigned char *p_bta_sys_cfg;
    GKI_send_msg(bta_sys_cb.task_id, p_bta_sys_cfg[2], p_msg);
}

void bta_sys_start_timer(TIMER_LIST_ENT *p_tle, UINT16 type, INT32 timeout) {}

void bta_sys_stop_timer(TIMER_LIST_ENT *p_tle) {}

void bta_sys_disable(tBTA_SYS_HW_MODULE module) {}

extern u8 appl_trace_level;
extern void bta_sys_set_trace_level(u8 val) { appl_trace_level = val; }
