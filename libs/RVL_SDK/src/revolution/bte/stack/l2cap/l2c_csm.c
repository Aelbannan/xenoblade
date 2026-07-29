// L2CAP Channel State Machine — RVL retail slice (ogws/Petari donors adapted).
// High-level C reconstruction of the Broadcom bluedroid l2c_csm module.

#define REVOLUTION 1

#include <revolution/BTE/include/bt_target.h>
#include <revolution/BTE/stack/include/bt_types.h>
#include <revolution/BTE/gki/common/gki.h>
#include <revolution/BTE/stack/include/l2c_api.h>
#include <revolution/BTE/stack/include/l2cdefs.h>
#include <revolution/BTE/include/bt_trace.h>

/*******************************************************************************
 * Internal types (normally in l2c_int.h, inlined here for this TU)
 ******************************************************************************/

/* L2CAP channel (connection) states. */
typedef enum {
    CST_CLOSED,
    CST_ORIG_W4_SEC_COMP,
    CST_TERM_W4_SEC_COMP,
    CST_W4_L2CAP_CONNECT_RSP,
    CST_W4_L2CA_CONNECT_RSP,
    CST_CONFIG,
    CST_OPEN,
    CST_W4_L2CAP_DISCONNECT_RSP,
    CST_W4_L2CA_DISCONNECT_RSP,
} tL2C_CHNL_STATE;

/* Events fed to the channel state machine. */
enum {
    L2CEVT_LP_CONNECT_CFM        = 0,
    L2CEVT_LP_CONNECT_CFM_NEG    = 1,
    L2CEVT_LP_DISCONNECT_IND     = 3,
    L2CEVT_LP_QOS_VIOLATION_IND  = 6,
    L2CEVT_SEC_COMP             = 7,
    L2CEVT_SEC_COMP_NEG         = 8,
    L2CEVT_L2CAP_CONNECT_REQ    = 10,
    L2CEVT_L2CAP_CONNECT_RSP    = 11,
    L2CEVT_L2CAP_CONNECT_RSP_PND = 12,
    L2CEVT_L2CAP_CONNECT_RSP_NEG = 13,
    L2CEVT_L2CAP_CONFIG_REQ     = 14,
    L2CEVT_L2CAP_CONFIG_RSP     = 15,
    L2CEVT_L2CAP_CONFIG_RSP_NEG = 16,
    L2CEVT_L2CAP_DISCONNECT_REQ = 17,
    L2CEVT_L2CAP_DISCONNECT_RSP = 18,
    L2CEVT_L2CAP_DATA           = 19,
    L2CEVT_L2CA_CONNECT_REQ     = 20,
    L2CEVT_L2CA_CONNECT_RSP     = 21,
    L2CEVT_L2CA_CONNECT_RSP_NEG = 22,
    L2CEVT_L2CA_CONFIG_REQ      = 23,
    L2CEVT_L2CA_CONFIG_RSP      = 24,
    L2CEVT_L2CA_CONFIG_RSP_NEG  = 25,
    L2CEVT_L2CA_DISCONNECT_REQ  = 26,
    L2CEVT_L2CA_DISCONNECT_RSP  = 27,
    L2CEVT_L2CA_DATA_WRITE      = 29,
    L2CEVT_TIMEOUT              = 30,
    L2CEVT_SEC_RE_SEND_CMD      = 31,
    L2CEVT_ACK_TIMEOUT          = 32,
};

/* Registration control block — one per registered PSM. */
typedef struct {
    BOOLEAN             in_use;   /* 0x00 */
    /* 1 byte padding */
    UINT16              psm;      /* 0x02 */
    tL2CAP_APPL_INFO    api;      /* 0x04 */
} tL2C_RCB; /* size 0x2c */

/* Link control block. Only the type is needed here. */
typedef struct t_l2c_linkcb tL2C_LCB;

/* Channel control block. Field offsets must match the retail layout. */
typedef struct t_l2c_ccb {
    BOOLEAN             in_use;          /* 0x00 */
    /* 3 bytes padding */
    tL2C_CHNL_STATE     chnl_state;      /* 0x04 */
    struct t_l2c_ccb   *p_next_ccb;      /* 0x08 */
    struct t_l2c_ccb   *p_prev_ccb;      /* 0x0c */
    tL2C_LCB           *p_lcb;           /* 0x10 */
    UINT16              local_cid;       /* 0x14 */
    UINT16              remote_cid;       /* 0x16 */
    TIMER_LIST_ENT      timer_entry;     /* 0x18 */
    tL2C_RCB           *p_rcb;           /* 0x30 */
    UINT8               config_done;     /* 0x34 */
    UINT8               local_id;        /* 0x35 */
    UINT8               remote_id;       /* 0x36 */
    BOOLEAN             rtry_flag;       /* 0x37 */
    UINT16              in_mtu;          /* 0x38 */
    UINT16              out_mtu;         /* 0x3a */
    UINT16              our_flush_to;    /* 0x3c */
    /* remaining fields omitted — not referenced by this TU */
} tL2C_CCB;

/* L2CAP control block (global `l2cb`). Only the trace level is used here. */
typedef struct {
    UINT8   l2cap_trace_level;  /* 0x00 */
    /* remaining fields omitted — not referenced by this TU */
} tL2C_CB;

/*******************************************************************************
 * External references
 ******************************************************************************/

extern tL2C_CB l2cb;

extern void l2cu_release_ccb(tL2C_CCB *p_ccb);
extern void l2cu_send_peer_disc_rsp(tL2C_LCB *p_lcb, UINT8 remote_id,
                                    UINT16 local_cid, UINT16 remote_cid);

/* Forward declarations of the per-state handlers (defined below). */
void l2c_csm_closed(tL2C_CCB *p_ccb, UINT16 event, void *p_data);
void l2c_csm_orig_w4_sec_comp(tL2C_CCB *p_ccb, UINT16 event, void *p_data);
void l2c_csm_term_w4_sec_comp(tL2C_CCB *p_ccb, UINT16 event, void *p_data);
void l2c_csm_w4_l2cap_connect_rsp(tL2C_CCB *p_ccb, UINT16 event, void *p_data);
void l2c_csm_w4_l2ca_connect_rsp(tL2C_CCB *p_ccb, UINT16 event, void *p_data);
void l2c_csm_config(tL2C_CCB *p_ccb, UINT16 event, void *p_data);
void l2c_csm_open(tL2C_CCB *p_ccb, UINT16 event, void *p_data);
void l2c_csm_w4_l2cap_disconnect_rsp(tL2C_CCB *p_ccb, UINT16 event, void *p_data);
void l2c_csm_w4_l2ca_disconnect_rsp(tL2C_CCB *p_ccb, UINT16 event, void *p_data);

/*******************************************************************************
 * Channel state machine dispatcher
 ******************************************************************************/

/* Dispatch a channel event to the handler for the channel's current state. */
void l2c_csm_execute(tL2C_CCB *p_ccb, UINT16 event, void *p_data)
{
    switch (p_ccb->chnl_state)
    {
    case CST_CLOSED:
        l2c_csm_closed(p_ccb, event, p_data);
        break;
    case CST_ORIG_W4_SEC_COMP:
        l2c_csm_orig_w4_sec_comp(p_ccb, event, p_data);
        break;
    case CST_TERM_W4_SEC_COMP:
        l2c_csm_term_w4_sec_comp(p_ccb, event, p_data);
        break;
    case CST_W4_L2CAP_CONNECT_RSP:
        l2c_csm_w4_l2cap_connect_rsp(p_ccb, event, p_data);
        break;
    case CST_W4_L2CA_CONNECT_RSP:
        l2c_csm_w4_l2ca_connect_rsp(p_ccb, event, p_data);
        break;
    case CST_CONFIG:
        l2c_csm_config(p_ccb, event, p_data);
        break;
    case CST_OPEN:
        l2c_csm_open(p_ccb, event, p_data);
        break;
    case CST_W4_L2CAP_DISCONNECT_RSP:
        l2c_csm_w4_l2cap_disconnect_rsp(p_ccb, event, p_data);
        break;
    case CST_W4_L2CA_DISCONNECT_RSP:
        l2c_csm_w4_l2ca_disconnect_rsp(p_ccb, event, p_data);
        break;
    default:
        break;
    }
}

/*******************************************************************************
 * W4_L2CA_DISCONNECT_RSP state
 *  Waiting for the upper layer's L2CA_DisconnectRsp after we already sent a
 *  disconnect request to the peer. On most events the channel is released.
 ******************************************************************************/

void l2c_csm_w4_l2ca_disconnect_rsp(tL2C_CCB *p_ccb, UINT16 event, void *p_data)
{
    tL2CA_DISCONNECT_IND_CB *disconnect_ind =
        p_ccb->p_rcb->api.pL2CA_DisconnectInd_Cb;
    UINT16 local_cid = p_ccb->local_cid;

    L2CAP_TRACE_EVENT1("L2CAP - st: W4_L2CA_DISC_RSP evt: %d", event);

    switch (event)
    {
    case L2CEVT_LP_DISCONNECT_IND:
        L2CAP_TRACE_API1(
            "L2CAP - Calling Disconnect_Ind_Cb(), CID: 0x%04x  No Conf Needed",
            p_ccb->local_cid);
        l2cu_release_ccb(p_ccb);
        (*disconnect_ind)(local_cid, FALSE);
        break;

    case L2CEVT_TIMEOUT:
        l2cu_send_peer_disc_rsp(p_ccb->p_lcb, p_ccb->remote_id,
                                p_ccb->local_cid, p_ccb->remote_cid);
        L2CAP_TRACE_API1(
            "L2CAP - Calling Disconnect_Ind_Cb(), CID: 0x%04x  No Conf Needed",
            p_ccb->local_cid);
        l2cu_release_ccb(p_ccb);
        (*disconnect_ind)(local_cid, FALSE);
        break;

    case L2CEVT_L2CA_DISCONNECT_RSP:
        l2cu_send_peer_disc_rsp(p_ccb->p_lcb, p_ccb->remote_id,
                                p_ccb->local_cid, p_ccb->remote_cid);
        l2cu_release_ccb(p_ccb);
        break;

    case L2CEVT_L2CAP_DATA:
    case L2CEVT_L2CA_DATA_WRITE:
        GKI_freebuf(p_data);
        break;

    default:
        break;
    }
}

/*******************************************************************************
 * Stubs for the remaining per-state handlers (not yet decompiled).
 * Kept as out-of-line global definitions so the dispatcher's jump table
 * references local symbols and MWCC does not inline them away.
 ******************************************************************************/

void l2c_csm_closed(tL2C_CCB *p_ccb, UINT16 event, void *p_data)
{
    (void)p_ccb; (void)event; (void)p_data;
    l2cu_release_ccb(p_ccb);
}

void l2c_csm_orig_w4_sec_comp(tL2C_CCB *p_ccb, UINT16 event, void *p_data)
{
    (void)p_ccb; (void)event; (void)p_data;
}

void l2c_csm_term_w4_sec_comp(tL2C_CCB *p_ccb, UINT16 event, void *p_data)
{
    (void)p_ccb; (void)event; (void)p_data;
}

void l2c_csm_w4_l2cap_connect_rsp(tL2C_CCB *p_ccb, UINT16 event, void *p_data)
{
    (void)p_ccb; (void)event; (void)p_data;
}

void l2c_csm_w4_l2ca_connect_rsp(tL2C_CCB *p_ccb, UINT16 event, void *p_data)
{
    (void)p_ccb; (void)event; (void)p_data;
}

void l2c_csm_config(tL2C_CCB *p_ccb, UINT16 event, void *p_data)
{
    (void)p_ccb; (void)event; (void)p_data;
}

void l2c_csm_open(tL2C_CCB *p_ccb, UINT16 event, void *p_data)
{
    (void)p_ccb; (void)event; (void)p_data;
}

void l2c_csm_w4_l2cap_disconnect_rsp(tL2C_CCB *p_ccb, UINT16 event, void *p_data)
{
    (void)p_ccb; (void)event; (void)p_data;
}

void forward_peer_data(tL2C_CCB *p_ccb, BT_HDR *p_buf)
{
    (void)p_ccb; (void)p_buf;
}
