// L2CAP Channel State Machine - RVL retail slice (ogws/Petari donors adapted).
// High-level C reconstruction of the Broadcom bluedroid l2c_csm module.
//
// NOTE: function order in this file is REVERSED relative to the retail text
// layout: this TU is built with `-ipa off` (configure.py), which makes
// GC/3.0a5.2 emit functions into .text (and string pools into .data) in
// reverse source order. Writing the file back-to-front reproduces the retail
// .text / .data layout so string-pool reloc addends match byte-for-byte.

#define REVOLUTION 1

#include <revolution/BTE/include/bt_target.h>
#include <revolution/BTE/stack/include/bt_types.h>
#include <revolution/BTE/gki/common/gki.h>
#include <revolution/BTE/stack/include/btu.h>
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

/* Registration control block - one per registered PSM. */
typedef struct {
    BOOLEAN             in_use;   /* 0x00 */
    /* 1 byte padding */
    UINT16              psm;      /* 0x02 */
    tL2CAP_APPL_INFO    api;      /* 0x04 */
} tL2C_RCB; /* size 0x30 */

/* Link control block. Only the fields referenced by this TU are modelled;
 * offsets must match the retail layout (see l2c_utils.c). */
typedef struct t_l2c_ccb tL2C_CCB;
typedef struct t_l2c_linkcb tL2C_LCB;
struct t_l2c_linkcb {
    BOOLEAN             in_use;          /* 0x00 */
    UINT8               conn_role;       /* 0x01 */
    UINT16              link_flags;      /* 0x02 */
    INT32               link_state;      /* 0x04 */
    tL2C_CCB           *p_first_ccb;     /* 0x08 */
    tL2C_CCB           *p_last_ccb;      /* 0x0C */
    TIMER_LIST_ENT      timer_entry;     /* 0x10 */
    UINT16              handle;          /* 0x28 */
    BD_ADDR             remote_bd_addr;  /* 0x2A */
};

/* Channel control block. Field offsets must match the retail layout. */
typedef struct t_l2c_ccb {
    BOOLEAN             in_use;          /* 0x00 */
    /* 3 bytes padding */
    tL2C_CHNL_STATE     chnl_state;      /* 0x04 */
    struct t_l2c_ccb   *p_next_ccb;      /* 0x08 */
    struct t_l2c_ccb   *p_prev_ccb;      /* 0x0c */
    tL2C_LCB           *p_lcb;           /* 0x10 */
    UINT16              local_cid;       /* 0x14 */
    UINT16              remote_cid;      /* 0x16 */
    TIMER_LIST_ENT      timer_entry;     /* 0x18 */
    tL2C_RCB           *p_rcb;           /* 0x30 */
    UINT8               config_done;     /* 0x34 */
    UINT8               local_id;        /* 0x35 */
    UINT8               remote_id;       /* 0x36 */
    BOOLEAN             rtry_flag;       /* 0x37 */
    UINT16              in_mtu;          /* 0x38 */
    UINT16              out_mtu;         /* 0x3a */
    UINT16              our_flush_to;    /* 0x3c */
    UINT8               pad3e[0x32];     /* 0x3E .. 0x6F */
    BUFFER_Q            xmit_hold_q;     /* 0x70 - pending outbound data */
} tL2C_CCB;

/* Connection event data passed to the CSM connect events. Layout matches
 * the retail (see l2c_main.c process_l2cap_cmd). */
typedef struct {
    UINT8   bd_addr[6];                 /* 0x00 */
    UINT16  handle;                     /* 0x06 */
    UINT16  psm;                        /* 0x08 */
    UINT16  result;                     /* 0x0A */
    UINT16  status;                     /* 0x0C */
    UINT16  rcid;                       /* 0x0E */
} tL2C_CONN_INFO;                       /* 0x10 */

/* L2CAP control block (global `l2cb`). Full retail size 0x7E8 - MWCC picks
 * sda21 addressing for small extern types (assumes sbss), so the struct must
 * be declared at the full retail size to get the retail lis@ha/lbz@l pair
 * (see MWCC_CASES sda21/extern-size note). */
typedef struct {
    UINT8   l2cap_trace_level;  /* 0x000 */
    UINT8   is_register;        /* 0x001 */
    UINT8   pad[0x7E8 - 2];     /* remainder of the 0x7E8-byte control block */
} tL2C_CB;

/*******************************************************************************
 * Local constants (normally in l2c_int.h)
 ******************************************************************************/

#define L2CAP_CHNL_CONNECT_TOUT  60     /* 0x3c - L2CAP channel connect timeout */
#define L2CAP_CHNL_CFG_TOUT      30     /* 0x1e - L2CAP channel config timeout */
#define L2CAP_CHNL_RSP_TOUT     120     /* 0x78 - L2CAP channel rsp/pending timeout */
#define L2CAP_PKT_TYPE_CONNECT_REQ 9    /* first L2CAP signalling opcode */
#define L2CAP_CFG_DONE_OURS      0x01   /* our config response received */
#define L2CAP_CFG_DONE_PEER      0x02   /* peer config response received */
#define BTU_TTYPE_L2CAP_CHNL      3      /* timer type for L2CAP channels (btu.h) */

/*******************************************************************************
 * External references
 ******************************************************************************/

extern tL2C_CB l2cb;

extern void l2cu_release_ccb(tL2C_CCB *p_ccb);
extern void l2cu_send_peer_connect_req(tL2C_CCB *p_ccb);
extern void l2cu_send_peer_connect_rsp(tL2C_CCB *p_ccb, UINT16 result, UINT16 status);
extern void l2cu_send_peer_disc_rsp(tL2C_LCB *p_lcb, UINT8 remote_id,
                                    UINT16 local_cid, UINT16 remote_cid);
extern void l2cu_send_peer_disc_req(tL2C_CCB *p_ccb);
extern void l2c_link_check_send_pkts(tL2C_LCB *p_lcb, tL2C_CCB *p_ccb,
                                     BT_HDR *p_buf);
extern BOOLEAN l2cu_process_peer_cfg_req(tL2C_CCB *p_ccb, tL2CAP_CFG_INFO *p_cfg);
extern void l2cu_send_peer_config_rsp(tL2C_CCB *p_ccb, tL2CAP_CFG_INFO *p_cfg);
extern void l2cu_process_peer_cfg_rsp(tL2C_CCB *p_ccb, tL2CAP_CFG_INFO *p_cfg);
extern void l2cu_process_our_cfg_req(tL2C_CCB *p_ccb, tL2CAP_CFG_INFO *p_cfg);
extern void l2cu_send_peer_config_req(tL2C_CCB *p_ccb, tL2CAP_CFG_INFO *p_cfg);
extern void l2cu_process_our_cfg_rsp(tL2C_CCB *p_ccb, tL2CAP_CFG_INFO *p_cfg);
extern void btm_sec_abort_access_req(BD_ADDR bd_addr);
extern BOOLEAN btm_sec_l2cap_access_req(BD_ADDR bd_addr, UINT16 psm, UINT16 handle,
                                        BOOLEAN is_originator,
                                        void (*p_callback)(BD_ADDR, void *, UINT8));
extern void btu_start_timer(TIMER_LIST_ENT *p_tle, UINT16 type, UINT32 timeout);
extern void btu_stop_timer(TIMER_LIST_ENT *p_tle);
extern void l2c_link_sec_comp(BD_ADDR bd_addr, void *p_ref_data, UINT8 result);

/* Forward declarations of the per-state handlers (defined below). */
void l2c_csm_execute(tL2C_CCB *p_ccb, UINT16 event, void *p_data);
void l2c_csm_closed(tL2C_CCB *p_ccb, UINT16 event, void *p_data);
void l2c_csm_orig_w4_sec_comp(tL2C_CCB *p_ccb, UINT16 event, void *p_data);
void l2c_csm_term_w4_sec_comp(tL2C_CCB *p_ccb, UINT16 event, void *p_data);
void l2c_csm_w4_l2cap_connect_rsp(tL2C_CCB *p_ccb, UINT16 event, void *p_data);
void l2c_csm_w4_l2ca_connect_rsp(tL2C_CCB *p_ccb, UINT16 event, void *p_data);
void l2c_csm_config(tL2C_CCB *p_ccb, UINT16 event, void *p_data);
void l2c_csm_open(tL2C_CCB *p_ccb, UINT16 event, void *p_data);
void l2c_csm_w4_l2cap_disconnect_rsp(tL2C_CCB *p_ccb, UINT16 event, void *p_data);
void l2c_csm_w4_l2ca_disconnect_rsp(tL2C_CCB *p_ccb, UINT16 event, void *p_data);

/* Forward data packets to the upper layer (L2CAP data path helper). */
void forward_peer_data(tL2C_CCB *p_ccb, BT_HDR *p_buf);

#pragma auto_inline off

/* Forward queued upper-layer data to the peer: prepend the HCI ACL header
 * (handle + start flag) and the L2CAP header (length + remote CID) in the
 * space reserved at p_buf->offset, then hand the packet to the link layer.
 */
void forward_peer_data(tL2C_CCB *p_ccb, BT_HDR *p_buf)
{
    UINT8  *p;
    UINT16  event;

    if (p_buf && p_ccb->xmit_hold_q.count)
    {
        GKI_enqueue(&p_ccb->xmit_hold_q, p_buf);
        p_buf = (BT_HDR *)GKI_dequeue(&p_ccb->xmit_hold_q);
    }
    else if (p_buf == NULL)
    {
        p_buf = (BT_HDR *)GKI_dequeue(&p_ccb->xmit_hold_q);
    }

    while (p_buf != NULL)
    {
        event = p_buf->offset;

        if (event < L2CAP_PKT_TYPE_CONNECT_REQ)
        {
            L2CAP_TRACE_ERROR1("L2CAP - cannot send buffer, offset: %d\0", event);
            GKI_freebuf(p_buf);
            p_buf = (BT_HDR *)GKI_dequeue(&p_ccb->xmit_hold_q);
        }
        else
        {
            p_buf->offset = (UINT16)(event - 8);
            p_buf->len += 4;
            p = (UINT8 *)(p_buf + 1) + (UINT16)(event - 8);

            UINT16_TO_STREAM(p, p_ccb->p_lcb->handle | 0x2000);

            if (p_buf->len > btu_cb.hcit_acl_data_size)
            {
                UINT16_TO_STREAM(p, btu_cb.hcit_acl_data_size);
            }
            else
            {
                UINT16_TO_STREAM(p, p_buf->len);
            }

            UINT16_TO_STREAM(p, p_buf->len - 4);
            UINT16_TO_STREAM(p, p_ccb->remote_cid);
            p_buf->len += 4;

            l2c_link_check_send_pkts(p_ccb->p_lcb, NULL, p_buf);
            p_buf = (BT_HDR *)GKI_dequeue(&p_ccb->xmit_hold_q);
        }
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

void l2c_csm_w4_l2cap_disconnect_rsp(tL2C_CCB *p_ccb, UINT16 event, void *p_data)
{
    tL2CA_DISCONNECT_CFM_CB *p_disconnect_cfm_cb =
        p_ccb->p_rcb->api.pL2CA_DisconnectCfm_Cb;
    tL2CA_DISCONNECT_IND_CB *p_disconnect_ind_cb =
        p_ccb->p_rcb->api.pL2CA_DisconnectInd_Cb;
    UINT16 local_cid = p_ccb->local_cid;

    L2CAP_TRACE_EVENT1("L2CAP - st: W4_L2CAP_DISC_RSP evt: %d", event);

    switch (event)
    {
    case L2CEVT_LP_DISCONNECT_IND:      /* Link went down */
        L2CAP_TRACE_API1(
            "L2CAP - Calling Disconnect_Ind_Cb(), CID: 0x%04x  No Conf Needed",
            p_ccb->local_cid);
        l2cu_release_ccb(p_ccb);
        (*p_disconnect_ind_cb)(local_cid, FALSE);
        break;

    case L2CEVT_L2CAP_DISCONNECT_RSP:   /* Peer disconnect response */
        l2cu_release_ccb(p_ccb);
        if (p_disconnect_cfm_cb)
        {
            L2CAP_TRACE_API1("L2CAP - Calling DisconnectCfm_Cb(), CID: 0x%04x",
                             local_cid);
            (*p_disconnect_cfm_cb)(local_cid, 0);
        }
        break;

    case L2CEVT_L2CAP_DISCONNECT_REQ:   /* Peer disconnect request */
        l2cu_send_peer_disc_rsp(p_ccb->p_lcb, p_ccb->remote_id,
                                p_ccb->local_cid, p_ccb->remote_cid);
        l2cu_release_ccb(p_ccb);
        if (p_disconnect_cfm_cb)
        {
            L2CAP_TRACE_API1("L2CAP - Calling DisconnectCfm_Cb(), CID: 0x%04x",
                             local_cid);
            (*p_disconnect_cfm_cb)(local_cid, 0);
        }
        break;

    case L2CEVT_TIMEOUT:                /* Timeout */
        l2cu_release_ccb(p_ccb);
        if (p_disconnect_cfm_cb)
        {
            L2CAP_TRACE_API1("L2CAP - Calling DisconnectCfm_Cb(), CID: 0x%04x",
                             local_cid);
            (*p_disconnect_cfm_cb)(local_cid, L2CAP_CONN_TIMEOUT);
        }
        break;

    case L2CEVT_L2CAP_DATA:             /* Peer data */
    case L2CEVT_L2CA_DATA_WRITE:        /* Upper data */
        GKI_freebuf(p_data);
        break;
    }
}

void l2c_csm_open(tL2C_CCB *p_ccb, UINT16 event, void *p_data)
{
    tL2CA_DISCONNECT_IND_CB *p_disconnect_ind_cb =
        p_ccb->p_rcb->api.pL2CA_DisconnectInd_Cb;
    UINT16 local_cid = p_ccb->local_cid;

    L2CAP_TRACE_EVENT1("L2CAP - st: OPEN evt: %d", event);

    switch (event)
    {
    case L2CEVT_LP_DISCONNECT_IND:      /* Link went down */
        L2CAP_TRACE_API1(
            "L2CAP - Calling Disconnect_Ind_Cb(), CID: 0x%04x  No Conf Needed",
            p_ccb->local_cid);
        l2cu_release_ccb(p_ccb);
        (*p_disconnect_ind_cb)(local_cid, FALSE);
        break;

    case L2CEVT_LP_QOS_VIOLATION_IND:   /* QoS violation on the link */
        if (p_ccb->p_rcb->api.pL2CA_QoSViolationInd_Cb)
        {
            (*p_ccb->p_rcb->api.pL2CA_QoSViolationInd_Cb)(
                p_ccb->p_lcb->remote_bd_addr);
        }
        break;

    case L2CEVT_L2CAP_CONFIG_REQ:       /* Peer config request while open */
        {
            INT32 chnl_state = p_ccb->chnl_state;
            UINT8 config_done = p_ccb->config_done;

            p_ccb->chnl_state = CST_CONFIG;
            p_ccb->config_done = 0;
            btu_start_timer(&p_ccb->timer_entry, BTU_TTYPE_L2CAP_CHNL,
                            L2CAP_CHNL_CFG_TOUT);

            if (l2cu_process_peer_cfg_req(p_ccb,
                                          (tL2CAP_CFG_INFO *)p_data))
            {
                (*p_ccb->p_rcb->api.pL2CA_ConfigInd_Cb)(p_ccb->local_cid,
                                                         p_data);
            }
            else
            {
                btu_stop_timer(&p_ccb->timer_entry);
                p_ccb->chnl_state = chnl_state;
                p_ccb->config_done = config_done;
                l2cu_send_peer_config_rsp(p_ccb,
                                          (tL2CAP_CFG_INFO *)p_data);
            }
        }
        break;

    case L2CEVT_L2CAP_DISCONNECT_REQ:   /* Peer disconnect request */
        p_ccb->chnl_state = CST_W4_L2CA_DISCONNECT_RSP;
        btu_start_timer(&p_ccb->timer_entry, BTU_TTYPE_L2CAP_CHNL,
                        L2CAP_CHNL_CFG_TOUT);
        L2CAP_TRACE_API1(
            "L2CAP - Calling Disconnect_Ind_Cb(), CID: 0x%04x  Conf Needed",
            p_ccb->local_cid);
        (*p_ccb->p_rcb->api.pL2CA_DisconnectInd_Cb)(p_ccb->local_cid, TRUE);
        break;

    case L2CEVT_L2CAP_DATA:             /* Peer data */
        (*p_ccb->p_rcb->api.pL2CA_DataInd_Cb)(p_ccb->local_cid, p_data);
        break;

    case L2CEVT_L2CA_DISCONNECT_REQ:    /* Upper wants to disconnect */
        l2cu_send_peer_disc_req(p_ccb);
        p_ccb->chnl_state = CST_W4_L2CAP_DISCONNECT_RSP;
        btu_start_timer(&p_ccb->timer_entry, BTU_TTYPE_L2CAP_CHNL,
                        L2CAP_CHNL_CFG_TOUT);
        break;

    case L2CEVT_L2CA_DATA_WRITE:        /* Upper data to send */
        ((BT_HDR *)p_data)->event = p_ccb->local_cid;
        forward_peer_data(p_ccb, (BT_HDR *)p_data);
        break;

    case L2CEVT_L2CA_CONFIG_REQ:        /* Upper config request */
        l2cu_process_our_cfg_req(p_ccb, (tL2CAP_CFG_INFO *)p_data);
        l2cu_send_peer_config_req(p_ccb, (tL2CAP_CFG_INFO *)p_data);
        p_ccb->chnl_state = CST_CONFIG;
        p_ccb->config_done = 0;
        btu_start_timer(&p_ccb->timer_entry, BTU_TTYPE_L2CAP_CHNL,
                        L2CAP_CHNL_CFG_TOUT);
        break;
    }
}

void l2c_csm_config(tL2C_CCB *p_ccb, UINT16 event, void *p_data)
{
    tL2CA_DISCONNECT_IND_CB *p_disconnect_ind_cb =
        p_ccb->p_rcb->api.pL2CA_DisconnectInd_Cb;
    UINT16 local_cid = p_ccb->local_cid;

    L2CAP_TRACE_EVENT1("L2CAP - st: CONFIG evt: %d", event);

    switch (event)
    {
    case L2CEVT_LP_DISCONNECT_IND:      /* Link went down */
        L2CAP_TRACE_API1(
            "L2CAP - Calling Disconnect_Ind_Cb(), CID: 0x%04x  No Conf Needed",
            p_ccb->local_cid);
        l2cu_release_ccb(p_ccb);
        (*p_disconnect_ind_cb)(local_cid, FALSE);
        break;

    case L2CEVT_L2CAP_CONFIG_REQ:       /* Peer config request */
        if (l2cu_process_peer_cfg_req(p_ccb,
                                      (tL2CAP_CFG_INFO *)p_data))
        {
            L2CAP_TRACE_API1("L2CAP - Calling Config_Req_Cb(), CID: 0x%04x",
                             p_ccb->local_cid);
            (*p_ccb->p_rcb->api.pL2CA_ConfigInd_Cb)(p_ccb->local_cid, p_data);
        }
        else
        {
            l2cu_send_peer_config_rsp(p_ccb, (tL2CAP_CFG_INFO *)p_data);
        }
        break;

    case L2CEVT_L2CAP_CONFIG_RSP:       /* Peer config response */
        l2cu_process_peer_cfg_rsp(p_ccb, (tL2CAP_CFG_INFO *)p_data);
        p_ccb->config_done |= L2CAP_CFG_DONE_PEER;
        if (p_ccb->config_done & L2CAP_CFG_DONE_OURS)
        {
            p_ccb->chnl_state = CST_OPEN;
            btu_stop_timer(&p_ccb->timer_entry);
            forward_peer_data(p_ccb, NULL);
        }
        L2CAP_TRACE_API1("L2CAP - Calling Config_Rsp_Cb(), CID: 0x%04x",
                         p_ccb->local_cid);
        (*p_ccb->p_rcb->api.pL2CA_ConfigCfm_Cb)(p_ccb->local_cid, p_data);
        break;

    case L2CEVT_L2CAP_CONFIG_RSP_NEG:   /* Peer config response (negative) */
        btu_stop_timer(&p_ccb->timer_entry);
        L2CAP_TRACE_API2(
            "L2CAP - Calling Config_Rsp_Cb(), CID: 0x%04x, Failure: %d",
            p_ccb->local_cid,
            ((tL2CAP_CFG_INFO *)p_data)->result);
        (*p_ccb->p_rcb->api.pL2CA_ConfigCfm_Cb)(p_ccb->local_cid, p_data);
        break;

    case L2CEVT_L2CAP_DISCONNECT_REQ:   /* Peer disconnect request */
        btu_start_timer(&p_ccb->timer_entry, BTU_TTYPE_L2CAP_CHNL,
                        L2CAP_CHNL_CFG_TOUT);
        p_ccb->chnl_state = CST_W4_L2CA_DISCONNECT_RSP;
        L2CAP_TRACE_API1(
            "L2CAP - Calling Disconnect_Ind_Cb(), CID: 0x%04x  Conf Needed",
            p_ccb->local_cid);
        (*p_ccb->p_rcb->api.pL2CA_DisconnectInd_Cb)(p_ccb->local_cid, TRUE);
        break;

    case L2CEVT_L2CA_CONFIG_REQ:        /* Upper config request */
        l2cu_process_our_cfg_req(p_ccb, (tL2CAP_CFG_INFO *)p_data);
        l2cu_send_peer_config_req(p_ccb, (tL2CAP_CFG_INFO *)p_data);
        btu_start_timer(&p_ccb->timer_entry, BTU_TTYPE_L2CAP_CHNL,
                        L2CAP_CHNL_CFG_TOUT);
        break;

    case L2CEVT_L2CA_CONFIG_RSP:        /* Upper config response */
        l2cu_process_our_cfg_rsp(p_ccb, (tL2CAP_CFG_INFO *)p_data);
        p_ccb->config_done |= L2CAP_CFG_DONE_OURS;
        if (p_ccb->config_done & L2CAP_CFG_DONE_PEER)
        {
            p_ccb->chnl_state = CST_OPEN;
            btu_stop_timer(&p_ccb->timer_entry);
            forward_peer_data(p_ccb, NULL);
        }
        l2cu_send_peer_config_rsp(p_ccb, (tL2CAP_CFG_INFO *)p_data);
        break;

    case L2CEVT_L2CA_CONFIG_RSP_NEG:    /* Upper config response (negative) */
        l2cu_send_peer_config_rsp(p_ccb, (tL2CAP_CFG_INFO *)p_data);
        btu_start_timer(&p_ccb->timer_entry, BTU_TTYPE_L2CAP_CHNL,
                        L2CAP_CHNL_CFG_TOUT);
        break;

    case L2CEVT_L2CA_DISCONNECT_REQ:    /* Upper wants to disconnect */
        l2cu_send_peer_disc_req(p_ccb);
        p_ccb->chnl_state = CST_W4_L2CAP_DISCONNECT_RSP;
        btu_start_timer(&p_ccb->timer_entry, BTU_TTYPE_L2CAP_CHNL,
                        L2CAP_CHNL_CFG_TOUT);
        break;

    case L2CEVT_L2CAP_DATA:             /* Peer data */
        L2CAP_TRACE_API1("L2CAP - Calling DataInd_Cb(), CID: 0x%04x",
                         p_ccb->local_cid);
        (*p_ccb->p_rcb->api.pL2CA_DataInd_Cb)(p_ccb->local_cid, p_data);
        break;

    case L2CEVT_L2CA_DATA_WRITE:        /* Upper data to send */
        if (p_ccb->config_done & L2CAP_CFG_DONE_PEER)
        {
            GKI_enqueue(&p_ccb->xmit_hold_q, p_data);
        }
        else
        {
            GKI_freebuf(p_data);
        }
        break;

    case L2CEVT_TIMEOUT:                /* Timeout */
        l2cu_send_peer_disc_req(p_ccb);
        L2CAP_TRACE_API1(
            "L2CAP - Calling Disconnect_Ind_Cb(), CID: 0x%04x  No Conf Needed",
            p_ccb->local_cid);
        l2cu_release_ccb(p_ccb);
        (*p_disconnect_ind_cb)(local_cid, FALSE);
        break;
    }
}

void l2c_csm_w4_l2ca_connect_rsp(tL2C_CCB *p_ccb, UINT16 event, void *p_data)
{
    tL2CA_DISCONNECT_IND_CB *p_disconnect_ind_cb =
        p_ccb->p_rcb->api.pL2CA_DisconnectInd_Cb;
    UINT16 local_cid = p_ccb->local_cid;
    tL2C_CONN_INFO *p_ci = (tL2C_CONN_INFO *)p_data;

    L2CAP_TRACE_EVENT1("L2CAP - st: W4_L2CA_CON_RSP evt: %d", event);

    switch (event)
    {
    case L2CEVT_LP_DISCONNECT_IND:      /* Link went down */
        L2CAP_TRACE_API1(
            "L2CAP - Calling Disconnect_Ind_Cb(), CID: 0x%04x  No Conf Needed",
            p_ccb->local_cid);
        l2cu_release_ccb(p_ccb);
        (*p_disconnect_ind_cb)(local_cid, FALSE);
        break;

    case L2CEVT_L2CA_CONNECT_RSP:       /* Upper layer connect response */
        if (p_ci == NULL || p_ci->result == L2CAP_CONN_OK)
        {
            l2cu_send_peer_connect_rsp(p_ccb, L2CAP_CONN_OK, 0);
            p_ccb->chnl_state = CST_CONFIG;
            btu_start_timer(&p_ccb->timer_entry, BTU_TTYPE_L2CAP_CHNL,
                            L2CAP_CHNL_CFG_TOUT);
        }
        else
        {
            l2cu_send_peer_connect_rsp(p_ccb, p_ci->result,
                                       p_ci->status);
            btu_start_timer(&p_ccb->timer_entry, BTU_TTYPE_L2CAP_CHNL,
                            L2CAP_CHNL_RSP_TOUT);
        }
        break;

    case L2CEVT_L2CA_CONNECT_RSP_NEG:   /* Upper layer rejected */
        l2cu_send_peer_connect_rsp(p_ccb, p_ci->result,
                                   p_ci->status);
        l2cu_release_ccb(p_ccb);
        break;

    case L2CEVT_TIMEOUT:                /* Timeout */
        l2cu_send_peer_connect_rsp(p_ccb, L2CAP_CONN_NO_PSM, 0);
        L2CAP_TRACE_API1(
            "L2CAP - Calling Disconnect_Ind_Cb(), CID: 0x%04x  No Conf Needed",
            p_ccb->local_cid);
        l2cu_release_ccb(p_ccb);
        (*p_disconnect_ind_cb)(local_cid, FALSE);
        break;

    case L2CEVT_L2CAP_DATA:             /* Peer data */
    case L2CEVT_L2CA_DATA_WRITE:        /* Upper data */
        GKI_freebuf(p_data);
        break;

    case L2CEVT_L2CA_DISCONNECT_REQ:    /* Upper wants to disconnect */
        l2cu_send_peer_disc_req(p_ccb);
        p_ccb->chnl_state = CST_W4_L2CAP_DISCONNECT_RSP;
        btu_start_timer(&p_ccb->timer_entry, BTU_TTYPE_L2CAP_CHNL,
                        L2CAP_CHNL_CFG_TOUT);
        break;
    }
}

void l2c_csm_w4_l2cap_connect_rsp(tL2C_CCB *p_ccb, UINT16 event, void *p_data)
{
    tL2CA_DISCONNECT_IND_CB *p_disconnect_ind_cb =
        p_ccb->p_rcb->api.pL2CA_DisconnectInd_Cb;
    tL2CA_CONNECT_CFM_CB *p_connect_cfm_cb =
        p_ccb->p_rcb->api.pL2CA_ConnectCfm_Cb;
    UINT16 local_cid = p_ccb->local_cid;
    tL2C_CONN_INFO *p_ci = (tL2C_CONN_INFO *)p_data;

    L2CAP_TRACE_EVENT1("L2CAP - st: W4_L2CAP_CON_RSP evt: %d", event);

    switch (event)
    {
    case L2CEVT_LP_DISCONNECT_IND:      /* Link went down */
        p_ccb->chnl_state = CST_CLOSED;
        L2CAP_TRACE_API1(
            "L2CAP - Calling Disconnect_Ind_Cb(), CID: 0x%04x  No Conf Needed",
            p_ccb->local_cid);
        l2cu_release_ccb(p_ccb);
        (*p_disconnect_ind_cb)(local_cid, FALSE);
        break;

    case L2CEVT_L2CAP_CONNECT_RSP:      /* Peer connect response (OK) */
        p_ccb->remote_cid = p_ci->rcid;
        p_ccb->chnl_state = CST_CONFIG;
        btu_start_timer(&p_ccb->timer_entry, BTU_TTYPE_L2CAP_CHNL,
                        L2CAP_CHNL_CFG_TOUT);
        L2CAP_TRACE_API1(
            "L2CAP - Calling Connect_Cfm_Cb(), CID: 0x%04x, Success",
            p_ccb->local_cid);
        (*p_ccb->p_rcb->api.pL2CA_ConnectCfm_Cb)(p_ccb->local_cid,
                                                 L2CAP_CONN_OK);
        break;

    case L2CEVT_L2CAP_CONNECT_RSP_PND:  /* Peer connect response (pending) */
        btu_start_timer(&p_ccb->timer_entry, BTU_TTYPE_L2CAP_CHNL,
                        L2CAP_CHNL_RSP_TOUT);
        if (p_ccb->p_rcb->api.pL2CA_ConnectPnd_Cb)
        {
            L2CAP_TRACE_API0("L2CAP - Calling Connect_Pnd_Cb()");
            (*p_ccb->p_rcb->api.pL2CA_ConnectPnd_Cb)(0);
        }
        break;

    case L2CEVT_L2CAP_CONNECT_RSP_NEG:  /* Peer connect response (negative) */
        L2CAP_TRACE_API2(
            "L2CAP - Calling Connect_Cfm_Cb(), CID: 0x%04x, Failure Code: %d",
            p_ccb->local_cid, p_ci->result);
        l2cu_release_ccb(p_ccb);
        (*p_connect_cfm_cb)(local_cid, p_ci->result);
        break;

    case L2CEVT_TIMEOUT:                /* Timeout */
        L2CAP_TRACE_API1(
            "L2CAP - Calling Connect_Cfm_Cb(), CID: 0x%04x, Timeout",
            p_ccb->local_cid);
        l2cu_release_ccb(p_ccb);
        (*p_connect_cfm_cb)(local_cid, L2CAP_CONN_TIMEOUT);
        break;

    case L2CEVT_L2CA_DISCONNECT_REQ:    /* Upper wants to disconnect */
        l2cu_release_ccb(p_ccb);
        break;

    case L2CEVT_L2CAP_DATA:             /* Peer data */
    case L2CEVT_L2CA_DATA_WRITE:        /* Upper data */
        GKI_freebuf(p_data);
        break;
    }
}

#pragma auto_inline on

/*******************************************************************************
 * TERM_W4_SEC_COMP state
 *  Terminating side waiting for the security manager to finish before we can
 *  deliver the connect indication to the upper layer.
 ******************************************************************************/

void l2c_csm_term_w4_sec_comp(tL2C_CCB *p_ccb, UINT16 event, void *p_data)
{
    L2CAP_TRACE_EVENT1("L2CAP - st: TERM_W4_SEC_COMP evt: %d", event);

    switch (event)
    {
    case L2CEVT_LP_DISCONNECT_IND:      /* Link went down */
        btm_sec_abort_access_req(p_ccb->p_lcb->remote_bd_addr);
        l2cu_release_ccb(p_ccb);
        break;

    case L2CEVT_SEC_COMP:               /* Security done, send ConnectInd */
        p_ccb->chnl_state = CST_W4_L2CA_CONNECT_RSP;
        btu_start_timer(&p_ccb->timer_entry, BTU_TTYPE_L2CAP_CHNL,
                        L2CAP_CHNL_CONNECT_TOUT);
        L2CAP_TRACE_API1("L2CAP - Calling Connect_Ind_Cb(), CID: 0x%04x",
                         p_ccb->local_cid);
        (*p_ccb->p_rcb->api.pL2CA_ConnectInd_Cb)(p_ccb->p_lcb->remote_bd_addr,
                                                 p_ccb->local_cid,
                                                 p_ccb->p_rcb->psm,
                                                 p_ccb->remote_id);
        break;

    case L2CEVT_SEC_COMP_NEG:           /* Security failed, reject conn req */
        l2cu_send_peer_connect_rsp(p_ccb, L2CAP_CONN_SECURITY_BLOCK,
                                   L2CAP_CONN_OK);
        l2cu_release_ccb(p_ccb);
        break;

    case L2CEVT_L2CAP_DATA:             /* Peer data */
    case L2CEVT_L2CA_DATA_WRITE:        /* Upper data */
        GKI_freebuf(p_data);
        break;

    case L2CEVT_L2CA_DISCONNECT_REQ:    /* Upper wants to disconnect */
        l2cu_release_ccb(p_ccb);
        break;

    case L2CEVT_L2CAP_DISCONNECT_REQ:   /* Peer disconnect request */
        l2cu_send_peer_disc_rsp(p_ccb->p_lcb, p_ccb->remote_id,
                                p_ccb->local_cid, p_ccb->remote_cid);
        btm_sec_abort_access_req(p_ccb->p_lcb->remote_bd_addr);
        l2cu_release_ccb(p_ccb);
        break;
    }
}

/*******************************************************************************
 * ORIG_W4_SEC_COMP state
 *  Originating side waiting for the security manager to finish before we can
 *  send the L2CAP connect request to the peer.
 ******************************************************************************/

void l2c_csm_orig_w4_sec_comp(tL2C_CCB *p_ccb, UINT16 event, void *p_data)
{
    tL2CA_DISCONNECT_IND_CB *p_disconnect_ind_cb =
        p_ccb->p_rcb->api.pL2CA_DisconnectInd_Cb;
    tL2CA_CONNECT_CFM_CB *p_connect_cfm_cb =
        p_ccb->p_rcb->api.pL2CA_ConnectCfm_Cb;
    UINT16 local_cid = p_ccb->local_cid;

    L2CAP_TRACE_EVENT1("L2CAP - st: ORIG_W4_SEC_COMP evt: %d", event);

    switch (event)
    {
    case L2CEVT_LP_DISCONNECT_IND:      /* Link went down */
        L2CAP_TRACE_API1(
            "L2CAP - Calling Disconnect_Ind_Cb(), CID: 0x%04x  No Conf Needed",
            p_ccb->local_cid);
        l2cu_release_ccb(p_ccb);
        (*p_disconnect_ind_cb)(local_cid, FALSE);
        break;

    case L2CEVT_SEC_COMP:               /* Security done, start link connection */
        p_ccb->chnl_state = CST_W4_L2CAP_CONNECT_RSP;
        btu_start_timer(&p_ccb->timer_entry, BTU_TTYPE_L2CAP_CHNL,
                        L2CAP_CHNL_CONNECT_TOUT);
        l2cu_send_peer_connect_req(p_ccb);
        p_ccb->rtry_flag = FALSE;
        break;

    case L2CEVT_SEC_COMP_NEG:           /* Security failed */
        L2CAP_TRACE_API2(
            "L2CAP - Calling ConnectCfm_Cb(), CID: 0x%04x  Status: %d",
            p_ccb->local_cid, L2CAP_CONN_BAD_CTLR_ID);
        l2cu_release_ccb(p_ccb);
        (*p_connect_cfm_cb)(local_cid, L2CAP_CONN_BAD_CTLR_ID);
        break;

    case L2CEVT_L2CAP_DATA:             /* Peer data */
    case L2CEVT_L2CA_DATA_WRITE:        /* Upper data */
        GKI_freebuf(p_data);
        break;

    case L2CEVT_L2CA_DISCONNECT_REQ:    /* Upper wants to disconnect */
        btm_sec_abort_access_req(p_ccb->p_lcb->remote_bd_addr);
        l2cu_release_ccb(p_ccb);
        break;
    }
}

/*******************************************************************************
 * CLOSED state
 *  No connection exists. Link-down / connect-failure events notify the upper
 *  layer through the registered callbacks; connect requests start security.
 ******************************************************************************/

void l2c_csm_closed(tL2C_CCB *p_ccb, UINT16 event, void *p_data)
{
    tL2CA_DISCONNECT_IND_CB *p_disconnect_ind_cb =
        p_ccb->p_rcb->api.pL2CA_DisconnectInd_Cb;
    tL2CA_CONNECT_CFM_CB *p_connect_cfm_cb =
        p_ccb->p_rcb->api.pL2CA_ConnectCfm_Cb;
    UINT16 local_cid = p_ccb->local_cid;

    L2CAP_TRACE_EVENT1("L2CAP - st: CLOSED evt: %d", event);

    switch (event)
    {
    case L2CEVT_LP_DISCONNECT_IND:      /* Link went down */
        L2CAP_TRACE_API1(
            "L2CAP - Calling Disconnect_Ind_Cb(), CID: 0x%04x  No Conf Needed",
            p_ccb->local_cid);
        l2cu_release_ccb(p_ccb);
        (*p_disconnect_ind_cb)(local_cid, FALSE);
        break;

    case L2CEVT_LP_CONNECT_CFM:         /* Link came up */
        p_ccb->chnl_state = CST_ORIG_W4_SEC_COMP;
        btm_sec_l2cap_access_req(p_ccb->p_lcb->remote_bd_addr,
                                 p_ccb->p_rcb->psm, p_ccb->p_lcb->handle,
                                 TRUE, l2c_link_sec_comp);
        break;

    case L2CEVT_LP_CONNECT_CFM_NEG:     /* Link failed */
        L2CAP_TRACE_API2(
            "L2CAP - Calling ConnectCfm_Cb(), CID: 0x%04x  Status: %d",
            p_ccb->local_cid, ((UINT8 *)p_data)[6]);
        l2cu_release_ccb(p_ccb);
        (*p_connect_cfm_cb)(local_cid, ((UINT8 *)p_data)[6]);
        break;

    case L2CEVT_L2CA_CONNECT_REQ:       /* Upper connect request */
        if (btm_sec_l2cap_access_req(p_ccb->p_lcb->remote_bd_addr,
                                     p_ccb->p_rcb->psm, p_ccb->p_lcb->handle,
                                     TRUE, l2c_link_sec_comp) == TRUE)
        {
            p_ccb->chnl_state = CST_ORIG_W4_SEC_COMP;
        }
        break;

    case L2CEVT_SEC_COMP:               /* Security done, start link connection */
        l2cu_send_peer_connect_req(p_ccb);
        p_ccb->chnl_state = CST_W4_L2CAP_CONNECT_RSP;
        btu_start_timer(&p_ccb->timer_entry, BTU_TTYPE_L2CAP_CHNL,
                        L2CAP_CHNL_CONNECT_TOUT);
        break;

    case L2CEVT_SEC_COMP_NEG:           /* Security failed */
        L2CAP_TRACE_API2(
            "L2CAP - Calling ConnectCfm_Cb(), CID: 0x%04x  Status: %d",
            p_ccb->local_cid, L2CAP_CONN_TIMEOUT);
        l2cu_release_ccb(p_ccb);
        (*p_connect_cfm_cb)(local_cid, L2CAP_CONN_SECURITY_BLOCK);
        break;

    case L2CEVT_L2CAP_CONNECT_REQ:      /* Peer connect request */
        p_ccb->chnl_state = CST_TERM_W4_SEC_COMP;
        btm_sec_l2cap_access_req(p_ccb->p_lcb->remote_bd_addr,
                                 p_ccb->p_rcb->psm, p_ccb->p_lcb->handle,
                                 FALSE, l2c_link_sec_comp);
        break;

    case L2CEVT_TIMEOUT:                /* Timeout */
        L2CAP_TRACE_API2(
            "L2CAP - Calling ConnectCfm_Cb(), CID: 0x%04x  Status: %d",
            p_ccb->local_cid, L2CAP_CONN_TIMEOUT);
        l2cu_release_ccb(p_ccb);
        (*p_connect_cfm_cb)(local_cid, L2CAP_CONN_TIMEOUT);
        break;

    case L2CEVT_L2CAP_DATA:             /* Peer data */
    case L2CEVT_L2CA_DATA_WRITE:        /* Upper data */
        GKI_freebuf(p_data);
        break;

    case L2CEVT_L2CA_DISCONNECT_REQ:    /* Upper wants to disconnect */
        l2cu_release_ccb(p_ccb);
        break;
    }
}

/*******************************************************************************
 * Channel state machine dispatcher
 ******************************************************************************/

/* Dispatch a channel event to the handler for the channel's current state.
 * The retail lowers this to an unsigned range check + 9-entry jump table of
 * tail calls, so every state must be a case label and the operand unsigned. */
void l2c_csm_execute(tL2C_CCB *p_ccb, UINT16 event, void *p_data)
{
    switch ((UINT32)p_ccb->chnl_state)
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
    }
}
