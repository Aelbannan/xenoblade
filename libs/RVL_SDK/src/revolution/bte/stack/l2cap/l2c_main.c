/******************************************************************************
 *
 *  L2CAP main processing — RVL retail slice reconstruction.
 *
 *  High-level C reconstruction of the Broadcom BTE l2c_main module:
 *  l2c_init, l2c_rcv_acl_data, process_l2cap_cmd, l2c_process_timeout
 *  and l2c_process_held_packets.
 *
 *  Internal types (normally in l2c_int.h) are declared here with the
 *  retail field offsets, as done in the sibling l2c_csm TU.
 *
 ******************************************************************************/

#define REVOLUTION 1

#include <string.h>

#include <revolution/BTE/include/bt_target.h>
#include <revolution/BTE/stack/include/bt_types.h>
#include <revolution/BTE/gki/common/gki.h>
#include <revolution/BTE/stack/include/l2c_api.h>
#include <revolution/BTE/stack/include/l2cdefs.h>
#include <revolution/BTE/include/bt_trace.h>

/*******************************************************************************
** Local constants
*******************************************************************************/

#ifndef MAX_L2C_CHANNELS
#define MAX_L2C_CHANNELS    10
#endif

/* Timer events routed to l2c_process_timeout() by the BTU task. */
#define BT_EVT_TO_L2C_LNKCTL    2       /* link control (LCB) timer   */
#define BT_EVT_TO_L2C_CHNL      3       /* channel (CCB) timer        */
#define BT_EVT_TO_L2C_HOLD      4       /* held ACL packet timer      */
#define BT_EVT_TO_L2C_INFO_RSP  0x49    /* unused in this build       */

/* Total ACL+L2CAP+HCI overhead subtracted when sizing echo responses. */
#define L2C_ECHO_OVERHEAD       (L2CAP_PKT_OVERHEAD + L2CAP_CMD_OVERHEAD + 4)

/*******************************************************************************
** Internal types (retail offsets)
*******************************************************************************/

/* Events fed to the channel state machine (l2c_csm). */
enum {
    L2CEVT_LP_DISCONNECT_IND     = 3,
    L2CEVT_L2CAP_CONNECT_REQ     = 10,
    L2CEVT_L2CAP_CONNECT_RSP     = 11,
    L2CEVT_L2CAP_CONNECT_RSP_PND = 12,
    L2CEVT_L2CAP_CONNECT_RSP_NEG = 13,
    L2CEVT_L2CAP_CONFIG_REQ      = 14,
    L2CEVT_L2CAP_CONFIG_RSP      = 15,
    L2CEVT_L2CAP_CONFIG_RSP_NEG  = 16,
    L2CEVT_L2CAP_DISCONNECT_REQ  = 17,
    L2CEVT_L2CAP_DISCONNECT_RSP  = 18,
    L2CEVT_L2CAP_DATA            = 19,
    L2CEVT_TIMEOUT               = 30,
};

typedef struct t_l2c_linkcb tL2C_LCB;
typedef struct t_l2c_rcb tL2C_RCB;

/* Link control block — only the fields referenced by this TU. */
struct t_l2c_linkcb {
    UINT8   pad00[0x28];
    UINT16  handle;                 /* 0x28 */
    UINT8   pad2A[0x54 - 0x2A];
    void  (*p_echo_cb)(UINT8 *);    /* 0x54 */
};

/* Channel control block — size must be 0x7C (ccb_pool element size). */
typedef struct t_l2c_ccb {
    BOOLEAN             in_use;         /* 0x00 */
    UINT8               pad01[3];       /* 0x01 */
    UINT32              chnl_state;     /* 0x04 */
    struct t_l2c_ccb   *p_next_ccb;     /* 0x08 */
    struct t_l2c_ccb   *p_prev_ccb;     /* 0x0C */
    tL2C_LCB           *p_lcb;          /* 0x10 */
    UINT16              local_cid;      /* 0x14 */
    UINT16              remote_cid;     /* 0x16 */
    TIMER_LIST_ENT      timer_entry;    /* 0x18 */
    tL2C_RCB           *p_rcb;          /* 0x30 */
    UINT8               config_done;    /* 0x34 */
    UINT8               local_id;       /* 0x35 */
    UINT8               remote_id;      /* 0x36 */
    UINT8               pad37[0x7C - 0x37];
} tL2C_CCB;

/* Parsed configuration options passed to the CSM config events. */
typedef struct {
    UINT16          result;             /* 0x00 */
    BOOLEAN         mtu_present;        /* 0x02 */
    UINT8           pad03;              /* 0x03 */
    UINT16          mtu;                /* 0x04 */
    BOOLEAN         qos_present;        /* 0x06 */
    UINT8           pad07;              /* 0x07 */
    FLOW_SPEC       qos;                /* 0x08 (24 bytes) */
    BOOLEAN         flush_to_present;   /* 0x20 */
    UINT8           pad21;              /* 0x21 */
    UINT16          flush_to;           /* 0x22 */
    BOOLEAN         fcr_present;        /* 0x24 */
    UINT8           pad25;              /* 0x25 */
    tL2CAP_FCR_OPTS fcr;                /* 0x26 (10 bytes) */
    UINT8           pad30[8];           /* 0x30 */
    UINT16          flags;              /* 0x38 */
} tL2C_CFG_INFO;

/* Connection event data passed to the CSM connect/disconnect events. */
typedef struct {
    UINT8   bd_addr[6];                 /* 0x00 */
    UINT16  handle;                     /* 0x06 */
    UINT16  psm;                        /* 0x08 */
    UINT16  result;                     /* 0x0A */
    UINT16  status;                     /* 0x0C */
    UINT16  rcid;                       /* 0x0E */
} tL2C_CONN_INFO;                       /* 0x10 */

/* L2CAP global control block (global `l2cb`, size 0x7E8). */
typedef struct {
    UINT8           l2cap_trace_level;              /* 0x000 */
    UINT8           is_register;                    /* 0x001 */
    UINT8           pad002[2];                      /* 0x002 */
    UINT8           lcb_pool[0x174];                /* 0x004 (3 LCBs x 0x7C) */
    tL2C_CCB        ccb_pool[MAX_L2C_CHANNELS];     /* 0x178 (10 CCBs x 0x7C) */
    UINT8           pad650[0x7B0 - 0x650];          /* 0x650 (RCB pool, etc.) */
    tL2C_CCB       *p_free_ccb_first;               /* 0x7B0 */
    tL2C_CCB       *p_free_ccb_last;                /* 0x7B4 */
    UINT16          num_links;                      /* 0x7B8 */
    UINT16          idle_timeout;                   /* 0x7BA */
    void           *round_robin_ccb;                /* 0x7BC */
    BUFFER_Q        hold_q;                         /* 0x7C0 */
    TIMER_LIST_ENT  hold_timer;                     /* 0x7CC */
    UINT32          pad7E4;                         /* 0x7E4 */
} tL2C_CB;

/* BTU control block — retail layout differs from btu.h; only the field
** used by this TU is needed, at offset 0x7E. */
typedef struct {
    UINT8   pad00[0x7E];
    UINT16  hcit_acl_pkt_size;          /* 0x7E */
} tBTU_CB;

/*******************************************************************************
** Globals and external references
*******************************************************************************/

extern tL2C_CB l2cb;
extern tBTU_CB btu_cb;

extern tL2C_LCB *l2cu_find_lcb_by_handle(UINT16 handle);
extern tL2C_CCB *l2cu_find_ccb_by_cid(tL2C_LCB *p_lcb, UINT16 cid);
extern tL2C_RCB *l2cu_find_rcb_by_psm(UINT16 psm);
extern tL2C_CCB *l2cu_allocate_ccb(tL2C_LCB *p_lcb);
extern void l2cu_reject_connection(tL2C_LCB *p_lcb, UINT16 src_cid, UINT8 rem_id,
                                   UINT16 reason);
extern void l2cu_send_peer_cmd_reject(tL2C_LCB *p_lcb, UINT8 reason, UINT8 rem_id,
                                      UINT16 local_cid, UINT16 remote_cid);
extern void l2cu_send_peer_config_rej(tL2C_CCB *p_ccb, UINT8 *p_data,
                                      UINT16 data_len, UINT16 rej_len);
extern void l2cu_send_peer_disc_rsp(tL2C_LCB *p_lcb, UINT8 rem_id,
                                    UINT16 local_cid, UINT16 remote_cid);
extern void l2cu_send_peer_echo_rsp(tL2C_LCB *p_lcb, UINT8 rem_id,
                                    UINT8 *p_data, UINT16 data_len);
extern void l2cu_send_peer_info_rsp(tL2C_LCB *p_lcb, UINT8 rem_id, UINT16 info_type);

extern BOOLEAN btm_sec_is_bonding(UINT16 handle);

extern void l2c_link_timeout(tL2C_LCB *p_lcb);
extern void l2c_csm_execute(tL2C_CCB *p_ccb, UINT16 event, void *p_data);

extern void btu_start_timer(TIMER_LIST_ENT *p_tle, UINT16 type, UINT32 timeout);
extern void btu_stop_timer(TIMER_LIST_ENT *p_tle);

void process_l2cap_cmd(tL2C_LCB *p_lcb, UINT8 *p, UINT16 pkt_len);
void l2c_process_held_packets(BOOLEAN timed_out);

/*******************************************************************************
**
** Function         l2c_init
**
** Description      Initialize the L2CAP control block: zero the block,
**                  chain the free CCB pool and set up the free queue.
**
*******************************************************************************/
void l2c_init(void)
{
    int i;

    memset(&l2cb, 0, sizeof(tL2C_CB));

    for (i = 0; i < MAX_L2C_CHANNELS - 1; i++)
    {
        l2cb.ccb_pool[i].p_next_ccb = &l2cb.ccb_pool[i + 1];
    }

    l2cb.p_free_ccb_first  = &l2cb.ccb_pool[0];
    l2cb.p_free_ccb_last   = &l2cb.ccb_pool[MAX_L2C_CHANNELS - 1];
    l2cb.is_register       = FALSE;
    l2cb.idle_timeout      = 2;
    l2cb.l2cap_trace_level = 0;
}

/*******************************************************************************
**
** Function         l2c_rcv_acl_data
**
** Description      Process an ACL data packet received from HCI: route
**                  signalling commands, connectionless traffic and channel
**                  data, holding packets for unknown links while bonding.
**
*******************************************************************************/
void l2c_rcv_acl_data(BT_HDR *p_msg)
{
    UINT8      *p;
    tL2C_LCB   *p_lcb;
    tL2C_CCB   *p_ccb = NULL;
    UINT16      handle;
    UINT16      hci_len;
    UINT16      l2cap_len;
    UINT16      rcv_cid;
    UINT8       pkt_type;

    p = (UINT8 *)(p_msg + 1) + p_msg->offset;

    /* Extract the handle and the packet boundary flag from the HCI header. */
    handle   = (UINT16)((p[0] + (p[1] << 8)) & 0x0FFF);
    pkt_type = (UINT8)(((p[0] + (p[1] << 8)) >> 12) & 0x03);

    if (pkt_type != 2)
    {
        L2CAP_TRACE_WARNING1("L2CAP - expected pkt start, got: %d", pkt_type);
        GKI_freebuf(p_msg);
        return;
    }

    /* Find the LCB based on the handle */
    if ((p_lcb = l2cu_find_lcb_by_handle(handle)) == NULL)
    {
        UINT16 cid    = (UINT16)(p[6] + (p[7] << 8));
        UINT8  opcode = p[8];

        /* While a bond is in progress, hold the first segment of signalling
        ** commands for the still-unknown link instead of dropping them. */
        if ((p_msg->layer_specific == 0) &&
            (cid == L2CAP_SIGNALLING_CID) &&
            ((opcode == L2CAP_CMD_INFO_REQ) || (opcode == L2CAP_CMD_CONN_REQ)) &&
            (!btm_sec_is_bonding(handle)))
        {
            L2CAP_TRACE_WARNING5(
                "L2CAP - holding ACL for unknown handle:%d ls:%d cid:%d opcode:%d cur count:%d",
                handle, p_msg->layer_specific, cid, opcode, l2cb.hold_q.count);

            p_msg->layer_specific = 2;
            GKI_enqueue(&l2cb.hold_q, p_msg);

            if (l2cb.hold_q.count == 1)
            {
                btu_start_timer(&l2cb.hold_timer, BT_EVT_TO_L2C_HOLD, 1);
            }
            return;
        }

        GKI_freebuf(p_msg);
        return;
    }

    /* Skip the HCI ACL header. */
    p_msg->offset += 4;
    hci_len   = (UINT16)(p[2] + (p[3] << 8));
    l2cap_len = (UINT16)(p[4] + (p[5] << 8));
    rcv_cid   = (UINT16)(p[6] + (p[7] << 8));

    /* For normal channels, find the CCB for this CID. */
    if (rcv_cid > L2CAP_CONNECTIONLESS_CID)
    {
        if ((p_ccb = l2cu_find_ccb_by_cid(p_lcb, rcv_cid)) == NULL)
        {
            L2CAP_TRACE_WARNING1("L2CAP - unknown CID: %d", rcv_cid);
            GKI_freebuf(p_msg);
            return;
        }
    }

    if (hci_len >= L2CAP_PKT_OVERHEAD)
    {
        p_msg->len = (UINT16)(hci_len - L2CAP_PKT_OVERHEAD);
        p_msg->offset += L2CAP_PKT_OVERHEAD;
    }
    else
    {
        L2CAP_TRACE_WARNING0("L2CAP - got incorrect hci header");
        GKI_freebuf(p_msg);
        return;
    }

    if (l2cap_len != p_msg->len)
    {
        L2CAP_TRACE_WARNING2("L2CAP - bad length in pkt. Exp: %d  Act: %d",
                             l2cap_len, p_msg->len);
        GKI_freebuf(p_msg);
        return;
    }

    if (rcv_cid == L2CAP_SIGNALLING_CID)
    {
        process_l2cap_cmd(p_lcb, p + 8, l2cap_len);
        GKI_freebuf(p_msg);
    }
    else if (rcv_cid == L2CAP_CONNECTIONLESS_CID)
    {
        p_msg->offset += L2CAP_BCST_OVERHEAD;
        p_msg->len = (UINT16)(hci_len - L2CAP_PKT_OVERHEAD - L2CAP_BCST_OVERHEAD);
        L2CAP_TRACE_DEBUG1("GOT CONNECTIONLESS DATA PSM:%d",
                           (UINT16)(p[8] + (p[9] << 8)));
        GKI_freebuf(p_msg);
    }
    else if (p_ccb == NULL)
    {
        GKI_freebuf(p_msg);
    }
    else
    {
        l2c_csm_execute(p_ccb, L2CEVT_L2CAP_DATA, p_msg);
    }
}

/*******************************************************************************
**
** Function         process_l2cap_cmd
**
** Description      Process one or more L2CAP signalling commands received
**                  on the signalling channel of a link.
**
*******************************************************************************/
void process_l2cap_cmd(tL2C_LCB *p_lcb, UINT8 *p, UINT16 pkt_len)
{
    UINT8          *p_cmd;
    UINT8          *p_end = p + pkt_len;
    UINT8           cmd_code;
    UINT8           id;
    UINT16          cmd_len;
    tL2C_CCB       *p_ccb;
    tL2C_RCB       *p_rcb;
    tL2C_CONN_INFO  conn_info;
    tL2C_CFG_INFO   cfg_info;

    while (p <= p_end - L2CAP_CMD_OVERHEAD)
    {
        p_cmd    = p;
        cmd_code = p_cmd[0];
        id       = p_cmd[1];
        cmd_len  = (UINT16)(p_cmd[2] + (p_cmd[3] << 8));
        p        = p_cmd + cmd_len + L2CAP_CMD_OVERHEAD;

        if (p > p_end)
        {
            L2CAP_TRACE_WARNING3("Command len bad  pkt_len: %d  cmd_len: %d  code: %d",
                                 pkt_len, cmd_len, cmd_code);
            return;
        }

        switch (cmd_code)
        {
        case L2CAP_CMD_REJECT:
        {
            UINT16 rej_reason = (UINT16)(p_cmd[4] + (p_cmd[5] << 8));

            p_cmd += 6;

            if (rej_reason == L2CAP_CMD_REJ_MTU_EXCEEDED)
            {
                UINT16 mtu = (UINT16)(p_cmd[0] + (p_cmd[1] << 8));

                L2CAP_TRACE_WARNING2("L2CAP - MTU rej Handle: %d MTU: %d",
                                     p_lcb->handle, mtu);
                p_cmd += 2;
            }
            else if (rej_reason == L2CAP_CMD_REJ_INVALID_CID)
            {
                UINT16 rcid = (UINT16)(p_cmd[0] + (p_cmd[1] << 8));
                UINT16 lcid = (UINT16)(p_cmd[2] + (p_cmd[3] << 8));

                L2CAP_TRACE_WARNING2("L2CAP - rej with CID invalid, LCID: %d RCID: %d",
                                     lcid, rcid);

                if ((p_ccb = l2cu_find_ccb_by_cid(p_lcb, lcid)) != NULL)
                {
                    if (p_ccb->remote_cid == rcid)
                    {
                        l2c_csm_execute(p_ccb, L2CEVT_LP_DISCONNECT_IND, NULL);
                    }
                }
            }
            break;
        }

        case L2CAP_CMD_CONN_REQ:
        {
            UINT16 psm     = (UINT16)(p_cmd[4] + (p_cmd[5] << 8));
            UINT16 src_cid = (UINT16)(p_cmd[6] + (p_cmd[7] << 8));

            if ((p_rcb = l2cu_find_rcb_by_psm(psm)) == NULL)
            {
                L2CAP_TRACE_WARNING1("L2CAP - rcvd conn req for unknown PSM: %d", psm);
                l2cu_reject_connection(p_lcb, src_cid, id, L2CAP_CONN_NO_PSM);
                break;
            }

            if ((p_ccb = l2cu_allocate_ccb(p_lcb)) == NULL)
            {
                L2CAP_TRACE_ERROR0("L2CAP - unable to allocate CCB");
                l2cu_reject_connection(p_lcb, src_cid, id, L2CAP_CONN_NO_RESOURCES);
                break;
            }

            p_ccb->remote_id  = id;
            p_ccb->p_rcb      = p_rcb;
            p_ccb->remote_cid = src_cid;
            l2c_csm_execute(p_ccb, L2CEVT_L2CAP_CONNECT_REQ, &conn_info);
            break;
        }

        case L2CAP_CMD_CONN_RSP:
        {
            UINT16 dst_cid = (UINT16)(p_cmd[4] + (p_cmd[5] << 8));
            UINT16 src_cid = (UINT16)(p_cmd[6] + (p_cmd[7] << 8));

            conn_info.rcid   = dst_cid;
            conn_info.result = (UINT16)(p_cmd[8] + (p_cmd[9] << 8));
            conn_info.status = (UINT16)(p_cmd[10] + (p_cmd[11] << 8));

            if ((p_ccb = l2cu_find_ccb_by_cid(p_lcb, src_cid)) == NULL)
            {
                L2CAP_TRACE_WARNING2("L2CAP - no CCB for conn rsp, LCID: %d RCID: %d",
                                     src_cid, dst_cid);
                break;
            }

            if (p_ccb->local_id != id)
            {
                L2CAP_TRACE_WARNING2("L2CAP - con rsp - bad ID. Exp: %d Got: %d",
                                     p_ccb->local_id, id);
                break;
            }

            if (conn_info.result == L2CAP_CONN_OK)
            {
                l2c_csm_execute(p_ccb, L2CEVT_L2CAP_CONNECT_RSP, &conn_info);
            }
            else if (conn_info.result == L2CAP_CONN_PENDING)
            {
                l2c_csm_execute(p_ccb, L2CEVT_L2CAP_CONNECT_RSP_PND, &conn_info);
            }
            else
            {
                l2c_csm_execute(p_ccb, L2CEVT_L2CAP_CONNECT_RSP_NEG, &conn_info);
            }
            break;
        }

        case L2CAP_CMD_CONFIG_REQ:
        {
            UINT16  local_cid   = (UINT16)(p_cmd[4] + (p_cmd[5] << 8));
            UINT8  *p_opt       = p_cmd + 8;
            UINT8  *p_opts      = p_opt;
            BOOLEAN bad_opt     = FALSE;
            UINT16  bad_opt_len = 0;

            cfg_info.flags           = (UINT16)(p_cmd[6] + (p_cmd[7] << 8));
            cfg_info.mtu_present     = FALSE;
            cfg_info.qos_present     = FALSE;
            cfg_info.flush_to_present = FALSE;
            cfg_info.fcr_present     = FALSE;

            while (p_opt < p)
            {
                UINT8 opt_type = p_opt[0];
                UINT8 opt_len  = p_opt[1];

                p_opt += 2;

                switch (opt_type & 0x7F)
                {
                case L2CAP_CFG_TYPE_MTU:
                    cfg_info.mtu_present = TRUE;
                    STREAM_TO_UINT16(cfg_info.mtu, p_opt);
                    break;

                case L2CAP_CFG_TYPE_FLUSH_TOUT:
                    cfg_info.flush_to_present = TRUE;
                    STREAM_TO_UINT16(cfg_info.flush_to, p_opt);
                    break;

                case L2CAP_CFG_TYPE_QOS:
                    cfg_info.qos_present = TRUE;
                    STREAM_TO_UINT8(cfg_info.qos.qos_flags, p_opt);
                    STREAM_TO_UINT8(cfg_info.qos.service_type, p_opt);
                    STREAM_TO_UINT32(cfg_info.qos.token_rate, p_opt);
                    STREAM_TO_UINT32(cfg_info.qos.token_bucket_size, p_opt);
                    STREAM_TO_UINT32(cfg_info.qos.peak_bandwidth, p_opt);
                    STREAM_TO_UINT32(cfg_info.qos.latency, p_opt);
                    STREAM_TO_UINT32(cfg_info.qos.delay_variation, p_opt);
                    break;

                case L2CAP_CFG_TYPE_FCR:
                    cfg_info.fcr_present = TRUE;
                    STREAM_TO_UINT8(cfg_info.fcr.mode, p_opt);
                    STREAM_TO_UINT8(cfg_info.fcr.tx_win_sz, p_opt);
                    STREAM_TO_UINT8(cfg_info.fcr.max_transmit, p_opt);
                    STREAM_TO_UINT16(cfg_info.fcr.rtrans_tout, p_opt);
                    STREAM_TO_UINT16(cfg_info.fcr.mon_tout, p_opt);
                    STREAM_TO_UINT16(cfg_info.fcr.mps, p_opt);
                    break;

                default:
                    if (opt_len + 2 > cmd_len)
                    {
                        p_opt = p;
                        break;
                    }
                    if (!(opt_type & 0x80))
                    {
                        bad_opt_len = (UINT16)(bad_opt_len + opt_len + 2);
                        bad_opt = TRUE;
                    }
                    p_opt += opt_len;
                    break;
                }
            }

            if ((p_ccb = l2cu_find_ccb_by_cid(p_lcb, local_cid)) == NULL)
            {
                l2cu_send_peer_cmd_reject(p_lcb, L2CAP_CMD_REJ_INVALID_CID, id, 0, 0);
            }
            else
            {
                p_ccb->remote_id = id;

                if (bad_opt)
                {
                    l2cu_send_peer_config_rej(p_ccb, p_opts,
                                              (UINT16)(cmd_len - L2CAP_CMD_OVERHEAD),
                                              bad_opt_len);
                }
                else
                {
                    l2c_csm_execute(p_ccb, L2CEVT_L2CAP_CONFIG_REQ, &cfg_info);
                }
            }
            break;
        }

        case L2CAP_CMD_CONFIG_RSP:
        {
            UINT16 local_cid = (UINT16)(p_cmd[4] + (p_cmd[5] << 8));
            UINT8 *p_opt     = p_cmd + 10;

            cfg_info.flags           = (UINT16)(p_cmd[6] + (p_cmd[7] << 8));
            cfg_info.result          = (UINT16)(p_cmd[8] + (p_cmd[9] << 8));
            cfg_info.mtu_present     = FALSE;
            cfg_info.qos_present     = FALSE;
            cfg_info.flush_to_present = FALSE;
            cfg_info.fcr_present     = FALSE;

            while (p_opt < p)
            {
                UINT8 opt_type = p_opt[0];
                UINT8 opt_len  = p_opt[1];

                p_opt += 2;

                switch (opt_type & 0x7F)
                {
                case L2CAP_CFG_TYPE_MTU:
                    cfg_info.mtu_present = TRUE;
                    STREAM_TO_UINT16(cfg_info.mtu, p_opt);
                    break;

                case L2CAP_CFG_TYPE_FLUSH_TOUT:
                    cfg_info.flush_to_present = TRUE;
                    STREAM_TO_UINT16(cfg_info.flush_to, p_opt);
                    break;

                case L2CAP_CFG_TYPE_QOS:
                    cfg_info.qos_present = TRUE;
                    STREAM_TO_UINT8(cfg_info.qos.qos_flags, p_opt);
                    STREAM_TO_UINT8(cfg_info.qos.service_type, p_opt);
                    STREAM_TO_UINT32(cfg_info.qos.token_rate, p_opt);
                    STREAM_TO_UINT32(cfg_info.qos.token_bucket_size, p_opt);
                    STREAM_TO_UINT32(cfg_info.qos.peak_bandwidth, p_opt);
                    STREAM_TO_UINT32(cfg_info.qos.latency, p_opt);
                    STREAM_TO_UINT32(cfg_info.qos.delay_variation, p_opt);
                    break;

                case L2CAP_CFG_TYPE_FCR:
                    cfg_info.fcr_present = TRUE;
                    STREAM_TO_UINT8(cfg_info.fcr.mode, p_opt);
                    STREAM_TO_UINT8(cfg_info.fcr.tx_win_sz, p_opt);
                    STREAM_TO_UINT8(cfg_info.fcr.max_transmit, p_opt);
                    STREAM_TO_UINT16(cfg_info.fcr.rtrans_tout, p_opt);
                    STREAM_TO_UINT16(cfg_info.fcr.mon_tout, p_opt);
                    STREAM_TO_UINT16(cfg_info.fcr.mps, p_opt);
                    break;

                default:
                    break;
                }
            }

            if ((p_ccb = l2cu_find_ccb_by_cid(p_lcb, local_cid)) == NULL)
            {
                L2CAP_TRACE_WARNING1("L2CAP - rcvd cfg rsp for unknown CID: %d",
                                     local_cid);
            }
            else if (p_ccb->local_id != id)
            {
                L2CAP_TRACE_WARNING2("L2CAP - cfg rsp - bad ID. Exp: %d Got: %d",
                                     p_ccb->local_id, id);
            }
            else if (cfg_info.result == L2CAP_CFG_OK)
            {
                l2c_csm_execute(p_ccb, L2CEVT_L2CAP_CONFIG_RSP, &cfg_info);
            }
            else
            {
                l2c_csm_execute(p_ccb, L2CEVT_L2CAP_CONFIG_RSP_NEG, &cfg_info);
            }
            break;
        }

        case L2CAP_CMD_DISC_REQ:
        {
            UINT16 dst_cid = (UINT16)(p_cmd[4] + (p_cmd[5] << 8));
            UINT16 src_cid = (UINT16)(p_cmd[6] + (p_cmd[7] << 8));

            if ((p_ccb = l2cu_find_ccb_by_cid(p_lcb, dst_cid)) == NULL)
            {
                l2cu_send_peer_disc_rsp(p_lcb, id, dst_cid, src_cid);
            }
            else if (p_ccb->remote_cid == src_cid)
            {
                p_ccb->remote_id = id;
                l2c_csm_execute(p_ccb, L2CEVT_L2CAP_DISCONNECT_REQ, &conn_info);
            }
            break;
        }

        case L2CAP_CMD_DISC_RSP:
        {
            UINT16 dst_cid = (UINT16)(p_cmd[4] + (p_cmd[5] << 8));
            UINT16 src_cid = (UINT16)(p_cmd[6] + (p_cmd[7] << 8));

            if ((p_ccb = l2cu_find_ccb_by_cid(p_lcb, src_cid)) != NULL)
            {
                if ((p_ccb->remote_cid == dst_cid) && (p_ccb->local_id == id))
                {
                    l2c_csm_execute(p_ccb, L2CEVT_L2CAP_DISCONNECT_RSP, &conn_info);
                }
            }
            break;
        }

        case L2CAP_CMD_ECHO_REQ:
            if (cmd_len < (btu_cb.hcit_acl_pkt_size - L2C_ECHO_OVERHEAD))
            {
                l2cu_send_peer_echo_rsp(p_lcb, id, p_cmd + L2CAP_CMD_OVERHEAD, cmd_len);
            }
            else
            {
                l2cu_send_peer_echo_rsp(p_lcb, id, NULL, 0);
            }
            break;

        case L2CAP_CMD_ECHO_RSP:
            if (p_lcb->p_echo_cb != NULL)
            {
                void (*p_echo_cb)(UINT8 *) = p_lcb->p_echo_cb;

                p_lcb->p_echo_cb = NULL;
                (*p_echo_cb)(NULL);
            }
            break;

        case L2CAP_CMD_INFO_REQ:
            l2cu_send_peer_info_rsp(p_lcb, id, (UINT16)(p_cmd[4] + (p_cmd[5] << 8)));
            break;

        case L2CAP_CMD_INFO_RSP:
            break;

        default:
            L2CAP_TRACE_WARNING1("L2CAP - bad cmd code: %d", cmd_code);
            l2cu_send_peer_cmd_reject(p_lcb, L2CAP_CMD_REJ_NOT_UNDERSTOOD, id, 0, 0);
            return;
        }
    }
}

/*******************************************************************************
**
** Function         l2c_process_timeout
**
** Description      Dispatch an expired BTU timer entry for L2CAP: link
**                  timers, channel timers and the held-packet timer.
**
*******************************************************************************/
void l2c_process_timeout(TIMER_LIST_ENT *p_tle)
{
    switch (p_tle->event)
    {
    case BT_EVT_TO_L2C_LNKCTL:
        l2c_link_timeout((tL2C_LCB *)p_tle->param);
        break;

    case BT_EVT_TO_L2C_CHNL:
        l2c_csm_execute((tL2C_CCB *)p_tle->param, L2CEVT_TIMEOUT, NULL);
        break;

    case BT_EVT_TO_L2C_HOLD:
        l2c_process_held_packets(TRUE);
        break;

    case BT_EVT_TO_L2C_INFO_RSP:
        break;

    default:
        break;
    }
}

/*******************************************************************************
**
** Function         l2c_process_held_packets
**
** Description      Retry (or release) ACL packets held for links that were
**                  unknown at receive time while bonding was in progress.
**
*******************************************************************************/
void l2c_process_held_packets(BOOLEAN timed_out)
{
    BT_HDR *p_buf;
    BT_HDR *p_next;

    if (l2cb.hold_q.count == 0)
    {
        return;
    }

    if (!timed_out)
    {
        btu_stop_timer(&l2cb.hold_timer);
        L2CAP_TRACE_WARNING0("L2CAP HOLD CONTINUE");
    }
    else
    {
        L2CAP_TRACE_WARNING0("L2CAP HOLD TIMEOUT");
    }

    for (p_buf = (BT_HDR *)GKI_getfirst(&l2cb.hold_q);
         p_buf != NULL;
         p_buf = p_next)
    {
        p_next = (BT_HDR *)GKI_getnext(p_buf);

        if ((!timed_out) ||
            (p_buf->layer_specific == 0) ||
            (--p_buf->layer_specific == 0))
        {
            GKI_remove_from_queue(&l2cb.hold_q, p_buf);
            p_buf->layer_specific = 0;
            l2c_rcv_acl_data(p_buf);
        }
    }

    if (l2cb.hold_q.count != 0)
    {
        btu_start_timer(&l2cb.hold_timer, BT_EVT_TO_L2C_HOLD, 1);
    }
}
