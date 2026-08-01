// RVL_SDK/src/revolution/bte/bta/sys/ptim
// Protocol timer services (Broadcom BTE).

#include <harness_catalog.h>

#include "revolution/bte/bta/sys/bta_sys.h"
#include "revolution/bte/gki/common/gki.h"
#include "revolution/bte/stack/include/bt_types.h"

/* Periodic timer control block (retail layout):
 *   0x00  TIMER_LIST_Q timer_queue
 *   0x0C  UINT32       period      (ms, converted to GKI ticks / 10)
 *   0x10  UINT8        timer_id
 */
typedef struct {
    TIMER_LIST_Q timer_queue; /* 0x00 */
    INT32        period;      /* 0x0C */
    UINT8        timer_id;    /* 0x10 */
} tPTIM_CB;

void ptim_init(tPTIM_CB *p_cb, INT32 period, UINT8 timer_id) {
    GKI_init_timer_list(&p_cb->timer_queue);
    p_cb->period = period;
    p_cb->timer_id = timer_id;
}

void ptim_timer_update(tPTIM_CB *p_cb) {
    TIMER_LIST_ENT *p_tle;
    BT_HDR *p_msg;

    GKI_update_timer_list(&p_cb->timer_queue, p_cb->period);

    /* while there are expired timers */
    while (p_cb->timer_queue.p_first != NULL &&
           p_cb->timer_queue.p_first->ticks <= 0) {
        /* remove expired timer from list */
        p_tle = p_cb->timer_queue.p_first;
        GKI_remove_from_timer_list(&p_cb->timer_queue, p_tle);

        /* call timer callback */
        if (p_tle->p_cback != NULL) {
            (*p_tle->p_cback)(p_tle);
        } else if (p_tle->event != 0) {
            p_msg = (BT_HDR *)GKI_getbuf(sizeof(BT_HDR));
            if (p_msg != NULL) {
                p_msg->event = p_tle->event;
                bta_sys_sendmsg(p_msg);
            }
        }
    }

    /* if timer list is empty stop periodic GKI timer */
    if (p_cb->timer_queue.p_first == NULL) {
        GKI_stop_timer(p_cb->timer_id);
    }
}

void ptim_start_timer(tPTIM_CB *p_cb, TIMER_LIST_ENT *p_tle, UINT16 type,
                      INT32 timeout) {
    if (p_cb->timer_queue.p_first == NULL) {
        GKI_start_timer(p_cb->timer_id, p_cb->period / 10, TRUE);
    }

    GKI_remove_from_timer_list(&p_cb->timer_queue, p_tle);
    p_tle->event = type;
    p_tle->ticks = timeout;
    GKI_add_to_timer_list(&p_cb->timer_queue, p_tle);
}

void ptim_stop_timer(tPTIM_CB *p_cb, TIMER_LIST_ENT *p_tle) {
    GKI_remove_from_timer_list(&p_cb->timer_queue, p_tle);

    /* if timer list is empty stop periodic GKI timer */
    if (p_cb->timer_queue.p_first == NULL) {
        GKI_stop_timer(p_cb->timer_id);
    }
}
