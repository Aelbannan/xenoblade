// RVL_SDK/src/revolution/bte/bta/sys/bta_sys_main

#include <harness_catalog.h>

#include <string.h>

#include "revolution/bte/bta/sys/bta_sys.h"
#include "revolution/bte/gki/common/gki.h"
#include "revolution/bte/include/bt_trace.h"

/* Defined in bta_dm_act.c */
extern void bta_dm_immediate_disable(void);

/* Periodic-timer helpers (ptim.c) */
extern void ptim_timer_update(void *p_cb);
extern void ptim_start_timer(void *p_cb, void *p_tle, UINT16 type,
                              INT32 timeout);
extern void ptim_stop_timer(void *p_cb, void *p_tle);

/* bta_sys_cb is a BSS global (0x8C bytes) defined in this TU (retail bss
 * 0x805BBE58). Layout at the offsets used by this file:
 *   0x00: reg[26]   per-module registration table (const tBTA_SYS_REG*)
 *   0x68: ptim_cb   periodic timer control block
 *   0x7C: timer_in_use flag
 *   0x7D: task_id
 *   0x7E: immediate-disable flag
 *   0x80: rm_cback / pm_cback / compress_cback (used by bta_sys_conn.c)
 */
typedef struct {
    const tBTA_SYS_REG *reg[26]; /* 0x00 */
    UINT8 ptim_cb[0x14];         /* 0x68 */
    UINT8 timer_in_use;          /* 0x7C */
    UINT8 task_id;               /* 0x7D */
    UINT8 immediate_disable;     /* 0x7E */
    UINT8 _pad[0x8C - 0x7F];     /* 0x7F..0x8B */
} tBTA_SYS_CB;

tBTA_SYS_CB bta_sys_cb;

UINT8 appl_trace_level;

extern tBTA_SYS_CFG *p_bta_sys_cfg;
extern void ptim_init(void *p_cb, UINT16 period, UINT8 timer_id);

void bta_sys_init(void) {
    memset(&bta_sys_cb, 0, sizeof(tBTA_SYS_CB));
    /* retail config built with REVOLUTION defined: BTA_SYS_TIMER_PERIOD = 1000 */
    ptim_init(&bta_sys_cb.ptim_cb, 1000, p_bta_sys_cfg->timer);
    bta_sys_cb.task_id = GKI_get_taskid();
    appl_trace_level = p_bta_sys_cfg->trace_level;
}

void bta_sys_event(BT_HDR *p_msg) {
    UINT8 id;
    BOOLEAN freebuf = TRUE;

    if (appl_trace_level >= 4) {
        LogMsg_1(0x503, "BTA got event 0x%x", p_msg->event);
    }

    id = (UINT8)(p_msg->event >> 8);

    if (bta_sys_cb.immediate_disable) {
        if (p_msg->event == 0x101) {
            bta_dm_immediate_disable();
        }
        GKI_freebuf(p_msg);
        return;
    }

    if (id < BTA_ID_MAX && bta_sys_cb.reg[id] != NULL) {
        freebuf = (*bta_sys_cb.reg[id]->evt_hdlr)(p_msg);
    } else if (appl_trace_level >= 2) {
        LogMsg_1(0x501, "BTA got unregistered event id %d\0\0\0", id);
    }

    if (freebuf) {
        GKI_freebuf(p_msg);
    }
}

void bta_sys_timer_update(void) {
    if (bta_sys_cb.timer_in_use) {
        return;
    }
    ptim_timer_update(&bta_sys_cb.ptim_cb);
}

void bta_sys_register(UINT8 id, const tBTA_SYS_REG *p_reg) {
    bta_sys_cb.reg[id] = p_reg;
}

void bta_sys_sendmsg(void *p_msg) {
    GKI_send_msg(bta_sys_cb.task_id, p_bta_sys_cfg->mbox, p_msg);
}

void bta_sys_start_timer(TIMER_LIST_ENT *p_tle, UINT16 type, INT32 timeout) {
    ptim_start_timer(&bta_sys_cb.ptim_cb, p_tle, type, timeout);
}

void bta_sys_stop_timer(TIMER_LIST_ENT *p_tle) {
    ptim_stop_timer(&bta_sys_cb.ptim_cb, p_tle);
}

void bta_sys_disable(tBTA_SYS_HW_MODULE module) {
    int id;

    for (id = 0; id < BTA_ID_MAX; id++) {
        const tBTA_SYS_REG *p_reg = bta_sys_cb.reg[id];

        if (p_reg != NULL && p_reg->disable != NULL) {
            p_reg->disable();
        }
    }
}

void bta_sys_set_trace_level(u8 val) { appl_trace_level = val; }
