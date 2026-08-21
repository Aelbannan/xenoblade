// L2CAP link management - RVL retail slice.
// High-level C reconstruction of the Broadcom BTE l2c_link module.

#define REVOLUTION 1

#include <string.h>

#include <revolution/BTE/include/bt_target.h>
#include <revolution/BTE/stack/include/bt_types.h>
#include <revolution/BTE/gki/common/gki.h>
#include <revolution/BTE/stack/include/l2c_api.h>
#include <revolution/BTE/stack/include/l2cdefs.h>
#include <revolution/BTE/stack/include/btm_api.h>
#include <revolution/BTE/include/bt_trace.h>
#include <revolution/BTE/main/bte_hcisu.h>

/*******************************************************************************
 * Internal constants and types
 *******************************************************************************/

#define L2C_MAX_LINKS                    4
#define BTU_TTYPE_L2CAP_LINK             2
#define L2CAP_HIGH_PRIORITY_MIN_XMIT_QUOTA 3

/* Link states in the retail L2CAP control block. */
typedef enum {
    LST_DISCONNECTED,
    LST_CONNECTING,
    LST_CONNECT_HOLDING,
    LST_CONNECTING_WAIT_SWITCH,
    LST_CONNECTED,
    LST_DISCONNECTING
} tL2C_LINK_STATE;

/* Events consumed by l2c_csm_execute. */
enum {
    L2CEVT_LP_CONNECT_CFM        = 0,
    L2CEVT_LP_CONNECT_CFM_NEG    = 1,
    L2CEVT_LP_DISCONNECT_IND     = 3,
    L2CEVT_LP_QOS_VIOLATION_IND  = 6,
    L2CEVT_SEC_COMP               = 7,
    L2CEVT_SEC_COMP_NEG           = 8
};

typedef struct t_l2c_linkcb tL2C_LCB;
typedef struct t_l2c_ccb tL2C_CCB;

/* Registration control block.  The application callback table starts at 0x04. */
typedef struct {
    BOOLEAN          in_use;       /* 0x00 */
    UINT8            pad01;        /* 0x01 */
    UINT16           psm;          /* 0x02 */
    tL2CAP_APPL_INFO api;           /* 0x04 */
} tL2C_RCB;

/* Channel fields referenced by this translation unit. */
struct t_l2c_ccb {
    BOOLEAN          in_use;       /* 0x00 */
    UINT8            pad01[3];     /* 0x01 */
    INT32            chnl_state;   /* 0x04 */
    tL2C_CCB        *p_next_ccb;   /* 0x08 */
    tL2C_CCB        *p_prev_ccb;   /* 0x0C */
    tL2C_LCB        *p_lcb;        /* 0x10 */
    UINT16           local_cid;    /* 0x14 */
    UINT16           remote_cid;   /* 0x16 */
    TIMER_LIST_ENT   timer_entry;  /* 0x18 */
    tL2C_RCB        *p_rcb;        /* 0x30 */
};

/* Link control block.  Its size is 0x5C bytes in the retail image. */
struct t_l2c_linkcb {
    BOOLEAN          in_use;                /* 0x00 */
    UINT8            pad01[3];              /* 0x01 */
    tL2C_LINK_STATE  link_state;            /* 0x04 */
    tL2C_CCB        *p_first_ccb;            /* 0x08 */
    tL2C_CCB        *p_last_ccb;             /* 0x0C */
    TIMER_LIST_ENT   timer_entry;            /* 0x10 */
    UINT16           handle;                /* 0x28 */
    BD_ADDR          remote_bd_addr;         /* 0x2A */
    UINT8            link_role;              /* 0x30 */
    UINT8            pad31[3];              /* 0x31 */
    UINT16           link_xmit_quota;        /* 0x34 */
    UINT16           sent_not_acked;         /* 0x36 */
    UINT16           link_xmit_sent;         /* 0x38 */
    UINT16           congestion_start;       /* 0x3A */
    UINT16           congestion_end;         /* 0x3C */
    UINT16           congestion_discard;     /* 0x3E */
    BOOLEAN          partial_segment;        /* 0x40 */
    BOOLEAN          is_congested;           /* 0x41 */
    UINT8            pad42[2];              /* 0x42 */
    BUFFER_Q         link_xmit_data_q;       /* 0x44 */
    BT_HDR          *p_pending_data;         /* 0x50 */
    tL2CA_ECHO_RSP_CB *p_echo_rsp_cb;        /* 0x54 */
    UINT8            pad58[2];              /* 0x58 */
    UINT8            priority;               /* 0x5A */
    UINT8            pad5B;                 /* 0x5B */
};

/* Connection info blob passed to the CSM on link events.  The retail code
 * keeps the peer BD_ADDR at offset 0 with the status byte at offset 6. */
typedef struct {
    BD_ADDR bd_addr;                /* 0x00 */
    UINT8   status;                 /* 0x06 */
    UINT8   pad07[9];               /* 0x07..0x0F */
} tL2C_CONN_INFO;                   /* 0x10 */

/* Only the portions of l2cb used here are named. */
typedef struct {
    UINT8            l2cap_trace_level;      /* 0x000 */
    UINT8            pad001;                /* 0x001 */
    UINT16           num_lm_acl_bufs;        /* 0x002 */
    UINT16           acl_out_count;          /* 0x004 */
    UINT16           pad006;                /* 0x006 */
    tL2C_LCB         lcb_pool[L2C_MAX_LINKS]; /* 0x008 */
    UINT8            pad178[0x7B8 - 0x178]; /* 0x178 */
    UINT16           num_links;             /* 0x7B8 */
    UINT16           pad7BA;                /* 0x7BA */
    tL2C_LCB        *p_rcv_pending_lcb;      /* 0x7BC */
} tL2C_CB;

/* The vendored btu.h describes a newer control block; these are the retail
 * ACL size fields used by this module. */
typedef struct {
    UINT8            opaque[0x7C];
    UINT16           hcit_acl_data_size;     /* 0x7C */
    UINT16           hcit_acl_pkt_size;      /* 0x7E */
} tBTU_CB;

/* The retail btm_cb field used by the disconnect completion handler. */
typedef struct {
    UINT8            opaque[0x27BF];
    UINT8            disc_reason;            /* 0x27BF */
} tBTM_CB;

/* Only the fields of a security device record passed to btm_acl_created are
 * needed here. */
typedef struct {
    UINT8            opaque22[0x22];
    DEV_CLASS        dev_class;              /* 0x22 */
    UINT8            opaque25[0x10];
    BD_NAME          sec_bd_name;            /* 0x35 */
} tBTM_SEC_DEV_REC;

/*******************************************************************************
 * External references
 *******************************************************************************/

extern tL2C_CB l2cb;
extern tBTU_CB btu_cb;
extern tBTM_CB btm_cb;

extern tL2C_LCB *l2cu_find_lcb_by_bd_addr(BD_ADDR p_bd_addr);
extern tL2C_LCB *l2cu_find_lcb_by_handle(UINT16 handle);
extern tL2C_LCB *l2cu_find_lcb_by_state(INT32 state);
extern tL2C_LCB *l2cu_allocate_lcb(BD_ADDR p_bd_addr);
extern UINT8 l2cu_get_conn_role(void);
extern BOOLEAN l2cu_lcb_disconnecting(void);
extern BOOLEAN l2cu_create_conn(tL2C_LCB *p_lcb);
extern BOOLEAN l2cu_create_conn_after_switch(tL2C_LCB *p_lcb);
extern void l2cu_release_lcb(tL2C_LCB *p_lcb);
extern void l2cu_send_peer_echo_req(tL2C_LCB *p_lcb, UINT8 *p_data,
                                    UINT16 data_len);

extern void l2c_csm_execute(tL2C_CCB *p_ccb, UINT16 event, void *p_data);
extern void l2c_process_held_packets(BOOLEAN timed_out);

extern void btu_start_timer(TIMER_LIST_ENT *p_tle, UINT16 type, UINT32 timeout);
extern void btu_stop_timer(TIMER_LIST_ENT *p_tle);

extern void btsnd_hcic_accept_conn(BT_HDR *p_buf, BD_ADDR p_bd_addr,
                                   UINT8 role);
extern void btsnd_hcic_reject_conn(BT_HDR *p_buf, BD_ADDR p_bd_addr,
                                   UINT8 reason);

extern void bte_hcisu_send(HC_BT_HDR *p_msg, UINT16 event);

extern tBTM_SEC_DEV_REC *btm_find_dev(BD_ADDR bd_addr);
extern void btm_acl_created(BD_ADDR bda, DEV_CLASS dc, BD_NAME bdn,
                            UINT16 hci_handle, UINT8 link_role);
extern void btm_sco_acl_removed(BD_ADDR bda);
extern BOOLEAN btm_sec_is_bonding(UINT16 handle);
extern tBTM_STATUS btm_sec_disconnect(UINT16 handle, UINT8 reason);

/* The retail module has several mutually recursive/linkage-local calls. */
BOOLEAN l2c_link_hci_disc_comp(UINT16 handle, UINT8 reason);
void l2c_link_check_send_pkts(tL2C_LCB *p_lcb, tL2C_CCB *p_ccb,
                              BT_HDR *p_buf);

/*******************************************************************************
 * Link event handlers
 *******************************************************************************/

void l2c_link_hci_conn_req(BD_ADDR p_bd_addr)
{
    BT_HDR *p_buf;
    tL2C_LCB *p_lcb;
    UINT8 role;

    p_buf = (BT_HDR *)GKI_getpoolbuf(GKI_POOL_ID_2);
    if (p_buf == NULL)
        return;

    p_lcb = l2cu_find_lcb_by_bd_addr(p_bd_addr);
    if (p_lcb == NULL)
    {
        p_lcb = l2cu_allocate_lcb(p_bd_addr);
        if (p_lcb == NULL)
        {
            btsnd_hcic_reject_conn(p_buf, p_bd_addr, HCI_ERR_PEER_LOW_RESOURCES);
            L2CAP_TRACE_ERROR0("L2CAP failed to allocate LCB");
            return;
        }

        role = l2cu_get_conn_role();
        p_lcb->link_role = role;
        btsnd_hcic_accept_conn(p_buf, p_bd_addr, role);
        p_lcb->link_state = LST_CONNECTING_WAIT_SWITCH;
        btu_start_timer(&p_lcb->timer_entry, BTU_TTYPE_L2CAP_LINK, 60);
        return;
    }

    if ((p_lcb->link_state == LST_CONNECTING_WAIT_SWITCH) ||
        (p_lcb->link_state == LST_CONNECTING))
    {
        btsnd_hcic_accept_conn(p_buf, p_bd_addr, p_lcb->link_role);
        p_lcb->link_state = LST_CONNECTING_WAIT_SWITCH;
        return;
    }

    if (p_lcb->link_state == LST_DISCONNECTING)
    {
        btsnd_hcic_reject_conn(p_buf, p_bd_addr, HCI_ERR_PEER_POWER_OFF);
        return;
    }

    L2CAP_TRACE_ERROR0("L2CAP got conn_req while connected");
    GKI_freebuf(p_buf);
}

BOOLEAN l2c_link_hci_conn_comp(UINT8 status, UINT16 handle, BD_ADDR p_bd_addr)
{
    tL2C_CONN_INFO ci;
    tL2C_LCB *p_lcb;
    tL2C_CCB *p_ccb;
    tL2C_CCB *p_next_ccb;
    tBTM_SEC_DEV_REC *p_dev_rec;

    ci.status = status;
    memcpy(ci.bd_addr, p_bd_addr, BD_ADDR_LEN);

    p_lcb = l2cu_find_lcb_by_bd_addr(ci.bd_addr);
    if (p_lcb == NULL)
    {
        if (!btm_sec_is_bonding(handle))
            L2CAP_TRACE_WARNING0("L2CAP got conn_comp for unknown BD_ADDR");
        return FALSE;
    }

    if (p_lcb->link_state != LST_CONNECTING_WAIT_SWITCH)
    {
        L2CAP_TRACE_ERROR2(
            "L2CAP got conn_comp in bad state: %d  status: 0x%d",
            p_lcb->link_state, status);
        if (status != HCI_SUCCESS)
            l2c_link_hci_disc_comp(p_lcb->handle, status);
        return FALSE;
    }

    p_lcb->handle = handle;

    if (ci.status == HCI_SUCCESS)
    {
        p_lcb->link_state = LST_CONNECTED;

        p_dev_rec = btm_find_dev(p_bd_addr);
        if (p_dev_rec != NULL)
        {
            btm_acl_created(ci.bd_addr, p_dev_rec->dev_class,
                            p_dev_rec->sec_bd_name, handle,
                            p_lcb->link_role);
        }
        else
        {
            btm_acl_created(ci.bd_addr, NULL, NULL, handle,
                            p_lcb->link_role);
        }

        l2c_process_held_packets(FALSE);
        btu_stop_timer(&p_lcb->timer_entry);

        for (p_ccb = p_lcb->p_first_ccb; p_ccb != NULL;
             p_ccb = p_ccb->p_next_ccb)
        {
            l2c_csm_execute(p_ccb, L2CEVT_LP_CONNECT_CFM, ci.bd_addr);
        }

        if (p_lcb->p_echo_rsp_cb != NULL)
        {
            l2cu_send_peer_echo_req(p_lcb, NULL, 0);
            btu_start_timer(&p_lcb->timer_entry, BTU_TTYPE_L2CAP_LINK, 30);
        }
        else if (p_lcb->p_first_ccb == NULL)
        {
            btu_start_timer(&p_lcb->timer_entry, BTU_TTYPE_L2CAP_LINK, 60);
        }
    }
    else
    {
        if ((ci.status == HCI_ERR_MAX_NUM_OF_CONNECTIONS) &&
            l2cu_lcb_disconnecting())
        {
            p_lcb->link_state = LST_CONNECTING;
            p_lcb->handle = HCI_INVALID_HANDLE;
        }
        else
        {
            for (p_ccb = p_lcb->p_first_ccb; p_ccb != NULL;
                 p_ccb = p_next_ccb)
            {
                p_next_ccb = p_ccb->p_next_ccb;
                l2c_csm_execute(p_ccb, L2CEVT_LP_CONNECT_CFM_NEG,
                                ci.bd_addr);
            }
            l2cu_release_lcb(p_lcb);
        }
    }

    return TRUE;
}

void l2c_link_sec_comp(BD_ADDR p_bd_addr, UINT16 handle, UINT8 status)
{
    UINT16 event;
    tL2C_CONN_INFO ci;
    tL2C_LCB *p_lcb;
    tL2C_CCB *p_ccb;
    tL2C_CCB *p_next_ccb;

    (void)handle;
    ci.status = status;
    memcpy(ci.bd_addr, p_bd_addr, BD_ADDR_LEN);

    p_lcb = l2cu_find_lcb_by_bd_addr(p_bd_addr);
    if (p_lcb == NULL)
    {
        L2CAP_TRACE_WARNING0("L2CAP got sec_comp for unknown BD_ADDR");
        return;
    }

    if (status != 0)
        event = L2CEVT_SEC_COMP_NEG;
    else
        event = L2CEVT_SEC_COMP;

    for (p_ccb = p_lcb->p_first_ccb; p_ccb != NULL; p_ccb = p_next_ccb)
    {
        p_next_ccb = p_ccb->p_next_ccb;
        l2c_csm_execute(p_ccb, (UINT8)event, &ci);
    }
}

BOOLEAN l2c_link_hci_disc_comp(UINT16 handle, UINT8 reason)
{
    tL2C_LCB *p_lcb;
    BOOLEAN found = TRUE;
    UINT8 disconnect_reason = reason;
    tL2C_CCB *p_ccb;
    tL2C_CCB *p_next_ccb;

    p_lcb = l2cu_find_lcb_by_handle(handle);
    if (p_lcb == NULL)
    {
        found = FALSE;
    }
    else
    {
        if (btm_cb.disc_reason != HCI_ERR_HOST_REJECT_SECURITY)
            btm_cb.disc_reason = disconnect_reason;

        for (p_ccb = p_lcb->p_first_ccb; p_ccb != NULL; p_ccb = p_next_ccb)
        {
            p_next_ccb = p_ccb->p_next_ccb;
            l2c_csm_execute(p_ccb, L2CEVT_LP_DISCONNECT_IND,
                            &disconnect_reason);
        }

        btm_sco_acl_removed(p_lcb->remote_bd_addr);
        l2cu_release_lcb(p_lcb);
    }

    p_lcb = l2cu_find_lcb_by_state(LST_CONNECTING);
    if (p_lcb != NULL)
        l2cu_create_conn(p_lcb);

    return found;
}

BOOLEAN l2c_link_hci_qos_violation(UINT16 handle)
{
    tL2C_LCB *p_lcb;
    tL2C_CCB *p_ccb;

    p_lcb = l2cu_find_lcb_by_handle(handle);
    if (p_lcb == NULL)
        return FALSE;

    for (p_ccb = p_lcb->p_first_ccb; p_ccb != NULL;
         p_ccb = p_ccb->p_next_ccb)
    {
        if (p_ccb->p_rcb->api.pL2CA_QoSViolationInd_Cb != NULL)
            l2c_csm_execute(p_ccb, L2CEVT_LP_QOS_VIOLATION_IND, NULL);
    }

    return TRUE;
}

void l2c_link_timeout(tL2C_LCB *p_lcb)
{
    tL2C_CCB *p_ccb;
    tL2C_CCB *p_next_ccb;
    tL2CA_ECHO_RSP_CB *p_echo_cb;
    tBTM_STATUS status;
    UINT16 timeout;

    if ((p_lcb->link_state == LST_CONNECT_HOLDING) ||
        (p_lcb->link_state == LST_CONNECTING_WAIT_SWITCH) ||
        (p_lcb->link_state == LST_CONNECTING) ||
        (p_lcb->link_state == LST_DISCONNECTING))
    {
        for (p_ccb = p_lcb->p_first_ccb; p_ccb != NULL;
             p_ccb = p_next_ccb)
        {
            p_next_ccb = p_ccb->p_next_ccb;
            l2c_csm_execute(p_ccb, L2CEVT_LP_DISCONNECT_IND, NULL);
        }
        l2cu_release_lcb(p_lcb);
    }

    if (p_lcb->link_state != LST_CONNECTED)
        return;

    p_echo_cb = p_lcb->p_echo_rsp_cb;
    if (p_echo_cb != NULL)
    {
        p_lcb->p_echo_rsp_cb = NULL;
        (*p_echo_cb)(L2CAP_PING_RESULT_NO_RESP);
        L2CAP_TRACE_WARNING0("L2CAP - ping timeout");

        for (p_ccb = p_lcb->p_first_ccb; p_ccb != NULL;
             p_ccb = p_next_ccb)
        {
            p_next_ccb = p_ccb->p_next_ccb;
            l2c_csm_execute(p_ccb, L2CEVT_LP_DISCONNECT_IND, NULL);
        }
    }

    if (p_lcb->p_first_ccb == NULL)
    {
        status = btm_sec_disconnect(p_lcb->handle, HCI_ERR_PEER_USER);
        if (status == BTM_CMD_STARTED)
        {
            p_lcb->link_state = LST_DISCONNECTING;
            timeout = 30;
        }
        else if (status == BTM_SUCCESS)
        {
            p_lcb->link_state = LST_DISCONNECTING;
            timeout = 0xFFFF;
        }
        else
        {
            timeout = 1;
        }

        if (timeout != 0xFFFF)
            btu_start_timer(&p_lcb->timer_entry, BTU_TTYPE_L2CAP_LINK,
                            timeout);
    }
    else
    {
        l2c_link_check_send_pkts(p_lcb, NULL, NULL);
    }
}

BOOLEAN l2c_link_send_to_lower(tL2C_LCB *p_lcb, BT_HDR *p_buf)
{
    UINT16 num_segments;

    if (p_buf->len <= btu_cb.hcit_acl_pkt_size)
    {
        p_lcb->sent_not_acked--;
        p_lcb->link_xmit_sent++;
        l2cb.acl_out_count--;
        bte_hcisu_send((HC_BT_HDR *)p_buf, BT_EVT_TO_LM_HCI_ACL);
    }
    else
    {
        num_segments = (p_buf->len + btu_cb.hcit_acl_data_size - 5) /
                       btu_cb.hcit_acl_data_size;

        if (num_segments > l2cb.acl_out_count)
        {
            num_segments = l2cb.acl_out_count;
            p_buf->layer_specific = num_segments;
            p_lcb->partial_segment = TRUE;
        }

        if (num_segments > p_lcb->sent_not_acked)
        {
            num_segments = p_lcb->sent_not_acked;
            p_buf->layer_specific = num_segments;
            p_lcb->partial_segment = TRUE;
        }

        l2cb.acl_out_count -= num_segments;
        p_lcb->sent_not_acked -= num_segments;
        p_lcb->link_xmit_sent += num_segments;
        bte_hcisu_send((HC_BT_HDR *)p_buf, BT_EVT_TO_LM_HCI_ACL);
    }

    return TRUE;
}

void l2c_link_check_send_pkts(tL2C_LCB *p_lcb, tL2C_CCB *p_ccb,
                              BT_HDR *p_buf)
{
    tL2C_CCB *p_iter;

    (void)p_ccb;

    if (p_buf != NULL)
    {
        if ((p_lcb->link_xmit_data_q.count >= p_lcb->congestion_start) &&
            (p_lcb->is_congested == FALSE))
        {
            for (p_iter = p_lcb->p_first_ccb; p_iter != NULL;
                 p_iter = p_iter->p_next_ccb)
            {
                L2CAP_TRACE_WARNING1(
                    "L2CAP - Congested(), CID: 0x%04x, Congested",
                    p_iter->local_cid);

                if (p_iter->p_rcb->api.pL2CA_CongestionStatus_Cb != NULL)
                {
                    L2CAP_TRACE_API1(
                        "L2CAP - Calling CongestionStatus_Cb(), CID: 0x%04x, Congested",
                        p_iter->local_cid);
                    (*p_iter->p_rcb->api.pL2CA_CongestionStatus_Cb)(
                        p_iter->local_cid, TRUE);
                }
            }
            p_lcb->is_congested = TRUE;
        }

        if (p_lcb->link_xmit_data_q.count <= p_lcb->congestion_discard)
        {
            p_buf->layer_specific = 0;
            GKI_enqueue(&p_lcb->link_xmit_data_q, p_buf);
        }
        else
        {
            GKI_freebuf(p_buf);
            L2CAP_TRACE_WARNING0("!!! L2CAP - buffer dropped");
        }
    }

    if (p_lcb->partial_segment != FALSE)
        return;

    {
        tBTM_PM_MODE power_mode;

        if ((BTM_ReadPowerMode(p_lcb->remote_bd_addr, &power_mode) ==
             BTM_SUCCESS) &&
            (power_mode == BTM_PM_STS_PARK) &&
            (p_lcb->link_xmit_data_q.count != 0))
        {
            tBTM_PM_PWR_MD power_md;

            power_md.mode = BTM_PM_MD_ACTIVE;
            BTM_SetPowerMode(BTM_PM_SET_ONLY_ID, p_lcb->remote_bd_addr,
                             &power_md);
            btu_start_timer(&p_lcb->timer_entry, BTU_TTYPE_L2CAP_LINK, 1);
            return;
        }
    }

    while ((l2cb.acl_out_count != 0) &&
           (p_lcb->sent_not_acked != 0))
    {
        BT_HDR *p_head = (BT_HDR *)p_lcb->link_xmit_data_q.p_first;

        if ((p_head == NULL) || (p_head->layer_specific != 0))
            break;

        if (p_lcb->partial_segment != FALSE)
            return;

        p_buf = (BT_HDR *)GKI_dequeue(&p_lcb->link_xmit_data_q);
        if (p_buf == NULL)
            break;

        if (l2c_link_send_to_lower(p_lcb, p_buf) == FALSE)
            break;
    }

    if (p_lcb->partial_segment != FALSE)
        return;

    if ((p_lcb->link_xmit_data_q.count != 0) &&
        (p_lcb->sent_not_acked != 0))
    {
        btu_start_timer(&p_lcb->timer_entry, BTU_TTYPE_L2CAP_LINK, 2);
    }

    if (p_lcb->is_congested == FALSE)
        return;

    if (p_lcb->link_xmit_data_q.count > p_lcb->congestion_end)
        return;

    p_lcb->is_congested = FALSE;
    for (p_iter = p_lcb->p_first_ccb; p_iter != NULL;
         p_iter = p_iter->p_next_ccb)
    {
        if (p_iter->p_rcb->api.pL2CA_CongestionStatus_Cb != NULL)
        {
            L2CAP_TRACE_API1(
                "L2CAP - Calling CongestionStatus_Cb(), CID: 0x%04x, Uncongested",
                p_iter->local_cid);
            (*p_iter->p_rcb->api.pL2CA_CongestionStatus_Cb)(
                p_iter->local_cid, FALSE);

            if (p_lcb->is_congested != FALSE)
                return;
        }
    }
}

void l2c_link_adjust_allocation(void)
{
    UINT16 xx;
    tL2C_LCB *p_lcb;
    UINT16 quota;
    int num_links_active = 0;
    tL2C_CB *p_cb = &l2cb;

    if (p_cb->num_links == 0)
        return;

    if (p_cb->lcb_pool[0].in_use && (p_cb->lcb_pool[0].priority == TRUE))
        num_links_active = 1;
    if (p_cb->lcb_pool[1].in_use && (p_cb->lcb_pool[1].priority == TRUE))
        num_links_active++;
    if (p_cb->lcb_pool[2].in_use && (p_cb->lcb_pool[2].priority == TRUE))
        num_links_active++;
    if (p_cb->lcb_pool[3].in_use && (p_cb->lcb_pool[3].priority == TRUE))
        num_links_active++;

    quota = l2cb.num_lm_acl_bufs / l2cb.num_links + 1;

    for (xx = 0, p_lcb = l2cb.lcb_pool; xx < L2C_MAX_LINKS;
         xx++, p_lcb++)
    {
        if (p_lcb->in_use)
        {
            if ((num_links_active == 0) || (p_lcb->priority == TRUE))
            {
                p_lcb->link_xmit_quota = quota;
                p_lcb->sent_not_acked = quota;
            }
            else
            {
                p_lcb->link_xmit_quota = L2CAP_HIGH_PRIORITY_MIN_XMIT_QUOTA;
                p_lcb->sent_not_acked = L2CAP_HIGH_PRIORITY_MIN_XMIT_QUOTA;
            }

            p_lcb->congestion_start =
                (UINT16)(p_lcb->link_xmit_quota * 0x78 / 100);
            p_lcb->congestion_end =
                (p_lcb->congestion_start > 3) ?
                    (p_lcb->congestion_start - 3) : 0;
            p_lcb->congestion_discard = p_lcb->congestion_start + 2;

            L2CAP_TRACE_DEBUG6(
                "LCB %d Priority:%d XmitWindow:%d Congestion Start:%d End:%d Discard:%d",
                xx, p_lcb->priority, p_lcb->sent_not_acked,
                p_lcb->congestion_start, p_lcb->congestion_end,
                p_lcb->congestion_discard);
        }
    }
}

void l2c_link_process_num_completed_pkts(UINT8 *p_data)
{
    UINT8 num_handles = p_data[0];
    UINT8 xx;
    UINT8 *p = p_data + 1;

    for (xx = 0; xx < num_handles; xx++)
    {
        UINT16 handle = (UINT16)(p[0] + (p[1] << 8));
        UINT16 num_completed = (UINT16)(p[2] + (p[3] << 8));
        tL2C_LCB *p_lcb;

        p += 4;
        p_lcb = l2cu_find_lcb_by_handle(handle);
        if (p_lcb != NULL)
        {
            p_lcb->sent_not_acked += num_completed;
            if (p_lcb->sent_not_acked > p_lcb->link_xmit_quota)
                p_lcb->sent_not_acked = p_lcb->link_xmit_quota;

            if (p_lcb->link_xmit_sent > num_completed)
                p_lcb->link_xmit_sent -= num_completed;
            else
                p_lcb->link_xmit_sent = 0;

            l2cb.acl_out_count += num_completed;
            l2c_link_check_send_pkts(p_lcb, NULL, NULL);
        }
    }
}

void l2c_link_processs_num_bufs(UINT16 num_lm_acl_bufs)
{
    l2cb.num_lm_acl_bufs = num_lm_acl_bufs;
    l2cb.acl_out_count = num_lm_acl_bufs;
}

BT_HDR *l2cap_link_chk_pkt_start(BT_HDR *p_buf)
{
    UINT8 *p_data;
    UINT16 handle_info;
    UINT16 acl_len;
    tL2C_LCB *p_lcb;
    BT_HDR *p_pending;
    UINT8 packet_type;
    UINT16 pending_len;
    UINT8 *p_pending_data;
    UINT16 handle;


    p_data = (UINT8 *)p_buf + p_buf->offset;
    p_buf->layer_specific = 0;

    handle_info = (UINT16)(p_data[8] + (p_data[9] << 8));
    acl_len = (UINT16)(p_data[10] + (p_data[11] << 8));
    packet_type = (UINT8)((handle_info >> L2CAP_PKT_TYPE_SHIFT) &
                          L2CAP_PKT_TYPE_MASK);
    handle = handle_info & L2CAP_MASK_FLAG;
    l2cb.p_rcv_pending_lcb = NULL;

    p_lcb = l2cu_find_lcb_by_handle(handle);
    if (p_lcb == NULL)
        return p_buf;

    if (packet_type == L2CAP_PKT_START)
    {
        if (p_lcb->p_pending_data != NULL)
        {
            L2CAP_TRACE_WARNING0("L2CAP - dropping incomplete pkt");
            GKI_freebuf(p_lcb->p_pending_data);
        }

        p_lcb->p_pending_data = p_buf;
        l2cb.p_rcv_pending_lcb = p_lcb;
        p_pending = p_buf;
    }
    else
    {
        p_pending = p_lcb->p_pending_data;
        if (p_pending != NULL)
        {
            p_pending_data = (UINT8 *)(p_pending + 1) + p_pending->offset;
            pending_len = (UINT16)(p_pending_data[2] +
                                   (p_pending_data[3] << 8));

            if (pending_len + acl_len <= 0x69F)
            {
                l2cb.p_rcv_pending_lcb = p_lcb;
                if (p_buf->len > 4)
                {
                    memcpy((UINT8 *)(p_pending + 1) + p_pending->offset +
                               p_pending->len,
                           (UINT8 *)(p_buf + 1) + p_buf->offset + 4,
                           p_buf->len - 4);
                    p_pending->len += p_buf->len - 4;
                }

                GKI_freebuf(p_buf);
                pending_len += acl_len;
                p_pending_data[2] = (UINT8)pending_len;
                p_pending_data[3] = (UINT8)(pending_len >> 8);
            }
            else
            {
                L2CAP_TRACE_WARNING0("L2CAP - dropping too long pkt");
                GKI_freebuf(p_lcb->p_pending_data);
                p_lcb->p_pending_data = NULL;
                p_pending = NULL;
            }
        }
        else
        {
            p_pending = NULL;
        }
    }

    if (p_pending == NULL)
        GKI_freebuf(p_buf);

    return p_pending;
}

BOOLEAN l2cap_link_chk_pkt_end(void)
{
    tL2C_LCB *p_lcb;
    BT_HDR *p_buf;
    UINT8 *p_data;
    UINT16 packet_len;

    p_lcb = l2cb.p_rcv_pending_lcb;
    if (p_lcb == NULL || p_lcb->p_pending_data == NULL)
        return TRUE;

    p_buf = p_lcb->p_pending_data;
    p_data = (UINT8 *)p_buf + p_buf->offset;
    packet_len = (UINT16)(p_data[12] + (p_data[13] << 8));
    if (packet_len > p_buf->len - 8)
        return FALSE;

    p_lcb->p_pending_data = NULL;
    return TRUE;
}

void l2c_link_role_changed(BD_ADDR p_bd_addr, UINT8 role)
{
    tL2C_LCB *p_lcb;
    INT16 xx;

    p_lcb = l2cu_find_lcb_by_bd_addr(p_bd_addr);
    if (p_lcb != NULL)
        p_lcb->link_role = role;

    for (xx = 0, p_lcb = l2cb.lcb_pool; xx < L2C_MAX_LINKS;
         xx++, p_lcb++)
    {
        if (p_lcb->in_use && (p_lcb->link_state == LST_CONNECT_HOLDING))
            l2cu_create_conn_after_switch(p_lcb);
    }
}

void l2c_link_role_change_failed(void)
{
    tL2C_LCB *p_lcb;
    INT16 xx;

    for (xx = 0, p_lcb = l2cb.lcb_pool; xx < L2C_MAX_LINKS;
         xx++, p_lcb++)
    {
        if (p_lcb->in_use && (p_lcb->link_state == LST_CONNECT_HOLDING))
            l2cu_create_conn_after_switch(p_lcb);
    }
}

void l2c_link_segments_xmitted(BT_HDR *p_buf)
{
    UINT8 *p_data;
    UINT16 handle;
    tL2C_LCB *p_lcb;

    p_data = (UINT8 *)p_buf + p_buf->offset;
    handle = (UINT16)((p_data[8] + (p_data[9] << 8)) & L2CAP_MASK_FLAG);
    p_lcb = l2cu_find_lcb_by_handle(handle);

    if (p_lcb == NULL)
    {
        L2CAP_TRACE_WARNING1(
            "L2CAP - rcvd segment complete, unknown handle: %d", handle);
        GKI_freebuf(p_buf);
    }
    else if (p_lcb->link_state == LST_CONNECTED)
    {
        p_buf->layer_specific = 0;
        GKI_enqueue_head(&p_lcb->link_xmit_data_q, p_buf);
        p_lcb->partial_segment = FALSE;
        l2c_link_check_send_pkts(p_lcb, NULL, NULL);
    }
    else
    {
        GKI_freebuf(p_buf);
    }
}

void l2c_pin_code_request(BD_ADDR p_bd_addr)
{
    tL2C_LCB *p_lcb;

    p_lcb = l2cu_find_lcb_by_bd_addr(p_bd_addr);
    if ((p_lcb != NULL) && (p_lcb->p_first_ccb == NULL))
        btu_start_timer(&p_lcb->timer_entry, BTU_TTYPE_L2CAP_LINK, 120);
}
