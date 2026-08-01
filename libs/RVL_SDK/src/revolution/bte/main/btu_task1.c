// High-level C reconstruction of RVL_SDK/src/revolution/bte/main/btu_task1
// Functions: btu_task_init, btu_task_msg_handler, btu_start_timer, btu_stop_timer

// NOTE: btu.h declares btu_hcif_process_event/send_cmd with (UINT8, BT_HDR *)
// but retail ASM shows they are called with single (BT_HDR *) argument — the
// controller_id parameter is absent in the actual implementations on this
// single-controller (BR/EDR only) build. We include btu.h for types, then
// override those three via function-pointer casts in the call sites.
#include "revolution/bte/stack/include/btu.h"
#include "revolution/os/OSTime.h"
#include "revolution/bte/bta/sys/bta_sys.h"

// Globals (in .sdata/.sbss)
extern UINT8 btu_count;
extern INT32 execute_btu;
extern UINT32 _btu_g_count;
extern UINT32 _btu_last_timer_tick;

// Forward declarations for functions in other TUs
void btu_init_core(void);
void BTE_InitStack(void);

void l2c_rcv_acl_data(BT_HDR *p_msg);
void l2c_link_segments_xmitted(BT_HDR *p_msg);
void l2c_process_timeout(TIMER_LIST_ENT *p_tle);
void btm_inq_rmt_name_failed(void);
void btm_discovery_timeout(void);
void btm_route_sco_data(BT_HDR *p_msg);
void btm_dev_timeout(TIMER_LIST_ENT *p_tle);
void btm_acl_timeout(TIMER_LIST_ENT *p_tle);
void sdp_conn_timeout(UINT32 param);
void rfcomm_process_timeout(TIMER_LIST_ENT *p_tle);
void hidh_proc_repage_timeout(TIMER_LIST_ENT *p_tle);

// The header declares these with (UINT8, BT_HDR*) but retail shows they
// are single-arg functions in this single-controller version. We use
// compatible pointer types for the call sites.
typedef void (*btu_hcif_process_event_fn_t)(BT_HDR *p_buf);
typedef void (*btu_hcif_send_cmd_fn_t)(BT_HDR *p_msg);
typedef void (*btu_hcif_cmd_timeout_fn_t)(void);

void btu_task_init(void)
{
    btu_count = 1;
    execute_btu = 1;
    _btu_g_count = 0;
    _btu_last_timer_tick = 0;
    btu_init_core();
    BTE_InitStack();
    bta_sys_init();
}

void btu_task_msg_handler(void)
{
    BT_HDR *p_msg;
    UINT16 event_type;
    TIMER_LIST_ENT *p_tle;
    UINT8 i;
    BOOLEAN found;
    UINT16 flags;
    BOOLEAN has_processed;

    has_processed = FALSE;
    OSGetTime();
    _btu_g_count++;

    GKI_disable();

    if (execute_btu) {
        execute_btu = FALSE;
        btu_count = 1;
    } else {
        btu_count++;
        GKI_enable();
        return;
    }
    GKI_enable();

    flags = 5;
    if (_btu_g_count > _btu_last_timer_tick + 500) {
        _btu_last_timer_tick = _btu_g_count;
        flags |= 0x30;
    }

    while (!has_processed) {
        has_processed = TRUE;

        /* Process mailbox 0 — HCI events/data */
        if (flags & 0x01) {
            while ((p_msg = (BT_HDR *)GKI_read_mbox(TASK_MBOX_0)) != NULL) {
                has_processed = FALSE;
                event_type = p_msg->event & 0xFF00;

                switch (event_type) {
                case 0x1100:
                    l2c_rcv_acl_data(p_msg);
                    break;
                case 0x1900:
                    l2c_link_segments_xmitted(p_msg);
                    break;
                case 0x1200:
                    btm_route_sco_data(p_msg);
                    break;
                case 0x1000:
                    ((btu_hcif_process_event_fn_t)btu_hcif_process_event)(p_msg);
                    GKI_freebuf(p_msg);
                    break;
                case 0x1600:
                    ((btu_hcif_send_cmd_fn_t)btu_hcif_send_cmd)(p_msg);
                    break;
                default:
                    i = 0;
                    found = FALSE;
                    for (; !found && i < BTU_MAX_REG_EVENT; i++) {
                        if (btu_cb.event_reg[i].event_cb == NULL) {
                            continue;
                        }

                        if (btu_cb.event_reg[i].event_range == event_type) {
                            if (btu_cb.event_reg[i].event_cb) {
                                (*btu_cb.event_reg[i].event_cb)(p_msg);
                                found = TRUE;
                            }
                        }
                    }
                    if (!found) {
                        GKI_freebuf(p_msg);
                    }
                    break;
                }
            }
        }

        /* Process expired timer queue */
        if (flags & 0x10) {
            GKI_update_timer_list(&btu_cb.quick_timer_queue, 1);
            flags = (UINT16)(flags & ~0x10);

            while (btu_cb.quick_timer_queue.p_first != NULL &&
                   btu_cb.quick_timer_queue.p_first->ticks == 0) {
                p_tle = btu_cb.quick_timer_queue.p_first;
                has_processed = FALSE;
                GKI_remove_from_timer_list(&btu_cb.quick_timer_queue, p_tle);

                switch (p_tle->event) {
                case 1:
                    btm_dev_timeout(p_tle);
                    break;
                case 9:
                    btm_acl_timeout(p_tle);
                    break;
                case 2:
                case 3:
                case 4:
                    l2c_process_timeout(p_tle);
                    break;
                case 5:
                    sdp_conn_timeout(p_tle->param);
                    break;
                case 10:
                    btm_inq_rmt_name_failed();
                    break;
                case 8:
                    btm_discovery_timeout();
                    break;
                case 11:
                case 12:
                    rfcomm_process_timeout(p_tle);
                    break;
                case 60:
                    ((btu_hcif_cmd_timeout_fn_t)btu_hcif_cmd_timeout)();
                    break;
                case 66:
                    hidh_proc_repage_timeout(p_tle);
                    break;
                case 22:
                    ((TIMER_CBACK *)(p_tle->param))(p_tle);
                    break;
                default:
                    i = 0;
                    found = FALSE;
                    for (; !found && i < BTU_MAX_REG_TIMER; i++) {
                        if (btu_cb.timer_reg[i].timer_cb == NULL) {
                            continue;
                        }

                        if (btu_cb.timer_reg[i].p_tle == p_tle) {
                            (*btu_cb.timer_reg[i].timer_cb)(p_tle);
                            found = TRUE;
                        }
                    }
                    break;
                }
            }
        }

        /* Process mailbox 2 — BTA events */
        if (flags & 0x04) {
            while ((p_msg = (BT_HDR *)GKI_read_mbox(TASK_MBOX_2)) != NULL) {
                has_processed = FALSE;
                bta_sys_event(p_msg);
            }
        }

        /* BTA timer update */
        if (flags & 0x20) {
            flags = (UINT16)(flags & ~0x20);
            bta_sys_timer_update();
        }

        if (flags & 0x8000) {
            break;
        }
    }

    execute_btu = TRUE;
}

void btu_start_timer(TIMER_LIST_ENT *p_tle, UINT16 type, UINT32 timeout)
{
    GKI_remove_from_timer_list(&btu_cb.quick_timer_queue, p_tle);
    p_tle->event = type;
    p_tle->ticks = (INT32)timeout;
    GKI_add_to_timer_list(&btu_cb.quick_timer_queue, p_tle);
}

void btu_stop_timer(TIMER_LIST_ENT *p_tle)
{
    GKI_remove_from_timer_list(&btu_cb.quick_timer_queue, p_tle);
}
