// L2CAP utility functions - RVL retail slice.
// High-level C reconstruction of the Broadcom BTE l2c_utils module:
// link/channel/registration control block management and the L2CAP
// signalling packet builders.

#define REVOLUTION 1

#include <string.h>

#include <revolution/BTE/include/bt_target.h>
#include <revolution/BTE/stack/include/bt_types.h>
#include <revolution/BTE/gki/common/gki.h>
#include <revolution/BTE/stack/include/l2c_api.h>
#include <revolution/BTE/stack/include/l2cdefs.h>
#include <revolution/BTE/include/bt_trace.h>

/*******************************************************************************
 * Internal constants (normally in l2c_int.h)
 ******************************************************************************/

#define L2C_MAX_LINKS     4
#define L2C_MAX_CHANNELS  10
#define L2C_MAX_CLIENTS   8

#define L2CAP_MIN_MTU     0x30
#define L2CAP_MAX_MTU     0x69B

/* LCB (link) states used by the retail slice. */
enum {
    LST_DISCONNECTED        = 0,
    LST_CONNECTED           = 1,
    LST_CONNECT_WAIT_SWITCH = 2,
    LST_CONNECTING          = 3,
    LST_DISCONNECTING       = 4,
    LST_DISCONNECT_WAIT     = 5
};

/* Channel states referenced here (full enum in l2c_csm). */
enum {
    CST_W4_L2CAP_DISCONNECT_RSP = 7,
    CST_W4_L2CA_DISCONNECT_RSP  = 8
};

#define BTU_TTYPE_L2CAP_LINK        2

#define L2CAP_LINK_SWITCH_TOUT      10
#define L2CAP_LINK_CONN_TOUT        60
#define L2CAP_LINK_DISCONNECT_TOUT  30

/* Max ACL payload the echo responder will reflect. */
#define L2CAP_ECHO_MAX_DATA_SIZE    0x294

/* TRUE/FALSE trace strings — retail .sdata locals (lbl_80665948 / lbl_80665950).
   Non-const fixed-size arrays so MWCC keeps them in .sdata (sda21-accessed),
   matching the retail layout: TRUE at +0x0, FALSE at +0x8, 8 bytes each. */
static char lbl_80665948[8] = "TRUE";
static char lbl_80665950[8] = "FALSE";

/*******************************************************************************
 * Internal types (normally in l2c_int.h). Field offsets match the retail
 * layout observed in the disassembly.
 ******************************************************************************/

typedef struct t_l2c_linkcb tL2C_LCB;
typedef struct t_l2c_ccb    tL2C_CCB;

/* Registration control block - one per registered PSM (size 0x2C). The
 * retail tL2CAP_APPL_INFO holds 10 callback pointers (0x28 bytes). */
typedef struct {
    BOOLEAN in_use;      /* 0x00 */
    UINT8   pad1;        /* 0x01 */
    UINT16  psm;         /* 0x02 */
    UINT8   api[0x28];   /* 0x04: application callbacks */
} tL2C_RCB;

/* Link control block (size 0x5C). */
struct t_l2c_linkcb {
    BOOLEAN         in_use;          /* 0x00 */
    UINT8           conn_role;       /* 0x01 */
    UINT16          link_flags;      /* 0x02 */
    INT32           link_state;      /* 0x04 */
    tL2C_CCB       *p_first_ccb;     /* 0x08 */
    tL2C_CCB       *p_last_ccb;      /* 0x0C */
    TIMER_LIST_ENT  timer_entry;     /* 0x10 */
    UINT16          handle;          /* 0x28 */
    BD_ADDR         remote_bd_addr;  /* 0x2A */
    UINT8           switch_pending;  /* 0x30 */
    UINT8           tx_ident;        /* 0x31 */
    UINT16          flush_tout;      /* 0x32 */
    UINT16          pad34;           /* 0x34 */
    UINT16          pad36;           /* 0x36 */
    UINT16          allocated_bw;    /* 0x38 */
    UINT16          pad3a;           /* 0x3A */
    UINT16          pad3c;           /* 0x3C */
    UINT16          pad3e;           /* 0x3E */
    UINT32          pad40;           /* 0x40 */
    BUFFER_Q        held_data_q;     /* 0x44 */
    BT_HDR         *p_pending_data;  /* 0x50 */
    void          (*p_link_callback)(UINT8 event); /* 0x54 */
    UINT16          idle_timeout;    /* 0x58 */
    UINT16          pad5a;           /* 0x5A */
};

/* Channel control block (size 0x7C). */
struct t_l2c_ccb {
    BOOLEAN         in_use;          /* 0x00 */
    UINT8           pad1[3];         /* 0x01 */
    INT32           chnl_state;      /* 0x04 */
    tL2C_CCB       *p_next_ccb;      /* 0x08 */
    tL2C_CCB       *p_prev_ccb;      /* 0x0C */
    tL2C_LCB       *p_lcb;           /* 0x10 */
    UINT16          local_cid;       /* 0x14 */
    UINT16          remote_cid;      /* 0x16 */
    TIMER_LIST_ENT  timer_entry;     /* 0x18 */
    tL2C_RCB       *p_rcb;           /* 0x30 */
    BOOLEAN         config_done;     /* 0x34 */
    UINT8           local_id;        /* 0x35 */
    UINT8           remote_id;       /* 0x36 */
    BOOLEAN         rtry_flag;       /* 0x37 */
    UINT16          in_mtu;          /* 0x38 */
    UINT16          out_mtu;         /* 0x3A */
    UINT16          our_flush_to;    /* 0x3C */
    UINT16          pad3e;           /* 0x3E */
    FLOW_SPEC       peer_cfg_qos;    /* 0x40 */
    FLOW_SPEC       our_cfg_qos;     /* 0x58 */
    BUFFER_Q        xmit_hold_q;     /* 0x70 */
};

/* Configuration information (retail layout, size 0x30). */
typedef struct {
    UINT16          result;          /* 0x00: only used in confirm messages */
    BOOLEAN         mtu_present;     /* 0x02 */
    UINT8           pad3;            /* 0x03 */
    UINT16          mtu;             /* 0x04 */
    BOOLEAN         qos_present;     /* 0x06 */
    UINT8           pad7;            /* 0x07 */
    FLOW_SPEC       qos;             /* 0x08 */
    BOOLEAN         flush_to_present;/* 0x20 */
    UINT8           pad21;           /* 0x21 */
    UINT16          flush_to;        /* 0x22 */
    BOOLEAN         fcr_present;     /* 0x24 */
    UINT8           pad25;           /* 0x25 */
    tL2CAP_FCR_OPTS fcr;             /* 0x26 */
} tL2C_CFG_INFO;

/* Main L2CAP control block. */
typedef struct {
    UINT8       l2cap_trace_level;              /* 0x000 */
    UINT8       conn_role;                      /* 0x001 */
    UINT16      pad2;                           /* 0x002 */
    UINT16      total_bw;                       /* 0x004 */
    UINT16      pad6;                           /* 0x006 */
    tL2C_LCB    lcb_pool[L2C_MAX_LINKS];        /* 0x008 */
    tL2C_CCB    ccb_pool[L2C_MAX_CHANNELS];     /* 0x178 */
    tL2C_RCB    rcb_pool[L2C_MAX_CLIENTS];      /* 0x650 */
    struct {
        tL2C_CCB *p_first;                      /* 0x7B0 */
        tL2C_CCB *p_last;                       /* 0x7B4 */
    }           ccb_free_q;
    UINT16      num_links;                      /* 0x7B8 */
    UINT16      def_idle_timeout;               /* 0x7BA */
} tL2C_CB;

/*******************************************************************************
 * External references
 ******************************************************************************/

extern tL2C_CB l2cb;

/* btu - the vendored btu.h is a newer layout; declare the retail slice. */
typedef struct {
    UINT8   opaque[0x7C];
    UINT16  hcit_acl_data_size;   /* 0x7C */
    UINT16  hcit_acl_pkt_size;    /* 0x7E */
} tBTU_CB;

extern tBTU_CB btu_cb;

extern void btu_start_timer (TIMER_LIST_ENT *p_tle, UINT16 type, UINT32 timeout);
extern void btu_stop_timer (TIMER_LIST_ENT *p_tle);

/* btm */
typedef UINT8 tBTM_STATUS;
#define BTM_SUCCESS     0
#define BTM_CMD_STARTED 1

typedef struct {
    UINT16  clock_offset;           /* 0x00 */
    UINT8   pad02[9];               /* 0x02 */
    UINT8   page_scan_rep_mode;     /* 0x0B */
    UINT8   page_scan_period_mode;  /* 0x0C */
    UINT8   page_scan_mode;         /* 0x0D */
} tBTM_INQ_INFO;

typedef struct {
    UINT8   opaque[0x640];
    UINT8   role_switch_flags;      /* 0x640 */
} tBTM_CB;

extern tBTM_CB btm_cb;

extern tBTM_STATUS    btm_sec_disconnect (UINT16 handle, UINT8 reason);
extern void           btm_acl_removed (BD_ADDR bda);
extern void           btm_remove_sco_links (BD_ADDR bda);
extern BOOLEAN        btm_is_sco_active_by_bdaddr (BD_ADDR bda);
extern tBTM_STATUS    BTM_SwitchRole (BD_ADDR bda, UINT8 new_role, void *p_cb);
extern UINT8         *BTM_ReadLocalFeatures (void);
extern tBTM_INQ_INFO *BTM_InqDbRead (BD_ADDR bda);

/* hci */
extern BOOLEAN btsnd_hcic_create_conn (BD_ADDR bda, UINT16 packet_types,
                                       UINT8 page_scan_rep_mode, UINT8 page_scan_mode,
                                       UINT16 clock_offset, BOOLEAN allow_switch);
extern BOOLEAN btsnd_hcic_write_auto_flush_tout (BT_HDR *p_buf, UINT16 handle, UINT16 tout);

/* l2c_link */
extern void l2c_link_adjust_allocation (void);
extern void l2c_link_check_send_pkts (tL2C_LCB *p_lcb, tL2C_CCB *p_ccb, BT_HDR *p_buf);
extern void l2c_link_hci_disc_comp (UINT16 handle, UINT8 reason);

/* local prototypes */
BT_HDR *l2cu_build_header (UINT16 handle, UINT16 len, UINT8 code, UINT8 ident);
BOOLEAN        l2cu_create_conn_after_switch (tL2C_LCB *p_lcb);
void           l2cu_release_lcb (tL2C_LCB *p_lcb);
void           l2cu_release_ccb (tL2C_CCB *p_ccb);

/*******************************************************************************
 * Link control block (LCB) management
 ******************************************************************************/

tL2C_LCB *l2cu_allocate_lcb (BD_ADDR p_bd_addr)
{
    INT32     xx;
    tL2C_LCB *p_lcb = &l2cb.lcb_pool[0];

    for (xx = 0; xx < L2C_MAX_LINKS; xx++)
    {
        if (!p_lcb->in_use)
        {
            memset (p_lcb, 0, sizeof (tL2C_LCB));
            p_lcb->in_use = TRUE;
            memcpy (p_lcb->remote_bd_addr, p_bd_addr, BD_ADDR_LEN);
            p_lcb->link_state       = LST_DISCONNECTED;
            p_lcb->handle           = 0xFFFF;
            p_lcb->p_pending_data   = NULL;
            p_lcb->flush_tout       = L2CAP_NO_AUTOMATIC_FLUSH;
            p_lcb->timer_entry.param = (TIMER_PARAM_TYPE)p_lcb;
            p_lcb->allocated_bw     = 0;
            p_lcb->idle_timeout     = l2cb.def_idle_timeout;
            p_lcb->tx_ident         = 0;
            l2cb.num_links++;
            l2c_link_adjust_allocation ();
            return (p_lcb);
        }

        p_lcb++;
    }

    return (NULL);
}

void l2cu_release_lcb (tL2C_LCB *p_lcb)
{
    tL2C_CCB *p_ccb;

    p_lcb->in_use = FALSE;

    btu_stop_timer (&p_lcb->timer_entry);

    if (p_lcb->p_pending_data != NULL)
    {
        GKI_freebuf (p_lcb->p_pending_data);
        p_lcb->p_pending_data = NULL;
    }

    btm_remove_sco_links (p_lcb->remote_bd_addr);

    p_ccb = p_lcb->p_first_ccb;
    while (p_ccb != NULL)
    {
        l2cu_release_ccb (p_ccb);
        p_ccb = p_lcb->p_first_ccb;
    }

    if ((p_lcb->link_state == LST_DISCONNECTING) || (p_lcb->link_state == LST_DISCONNECT_WAIT))
        btm_acl_removed (p_lcb->remote_bd_addr);

    while (p_lcb->held_data_q.p_first != NULL)
        GKI_freebuf (GKI_dequeue (&p_lcb->held_data_q));

    if (l2cb.num_links >= 1)
        l2cb.num_links--;

    l2cb.total_bw += p_lcb->allocated_bw;
    l2c_link_adjust_allocation ();

    if (p_lcb->p_link_callback != NULL)
    {
        void (*p_cb)(UINT8) = p_lcb->p_link_callback;
        p_lcb->p_link_callback = NULL;
        (*p_cb)(1);
    }
}

tL2C_LCB *l2cu_find_lcb_by_bd_addr (BD_ADDR p_bd_addr)
{
    INT8      xx;
    tL2C_LCB *p_lcb;

    for (xx = 0, p_lcb = &l2cb.lcb_pool[0]; xx < L2C_MAX_LINKS; xx++, p_lcb++)
    {
        if (p_lcb->in_use && (memcmp (p_lcb->remote_bd_addr, p_bd_addr, BD_ADDR_LEN) == 0))
            return (p_lcb);
    }

    return (NULL);
}

tL2C_LCB *l2cu_find_lcb_by_handle (UINT16 handle)
{
    UINT8     xx;
    tL2C_LCB *p_lcb;

    for (xx = 0, p_lcb = &l2cb.lcb_pool[0]; xx < L2C_MAX_LINKS; xx++, p_lcb++)
    {
        if (p_lcb->in_use && (p_lcb->handle == handle))
            return (p_lcb);
    }

    return (NULL);
}

UINT8 l2cu_get_conn_role (void)
{
    return l2cb.conn_role;
}

/*******************************************************************************
 * Signalling packet construction
 ******************************************************************************/

BT_HDR *l2cu_build_header (UINT16 handle, UINT16 len, UINT8 code, UINT8 ident)
{
    BT_HDR *p_buf = (BT_HDR *)GKI_getpoolbuf (2);
    UINT8  *p;

    if (p_buf == NULL)
        return (NULL);

    p_buf->offset = 0;
    p_buf->len    = (UINT16)(len + 12);

    p = (UINT8 *)(p_buf + 1);
    UINT16_TO_STREAM (p, handle | 0x2000);
    UINT16_TO_STREAM (p, len + 8);
    UINT16_TO_STREAM (p, len + 4);
    UINT8_TO_STREAM (p, L2CAP_SIGNALLING_CID);
    UINT8_TO_STREAM (p, 0);
    UINT8_TO_STREAM (p, code);
    UINT8_TO_STREAM (p, ident);
    UINT16_TO_STREAM (p, len);

    return (p_buf);
}

void l2cu_send_peer_cmd_reject (tL2C_LCB *p_lcb, UINT16 reason, UINT8 local_id,
                                UINT16 data1, UINT16 data2)
{
    BT_HDR *p_buf;
    UINT8  *p;
    UINT16 rej_len;

    if (reason == L2CAP_CMD_REJ_MTU_EXCEEDED)
        rej_len = 2;
    else
        rej_len = (reason == L2CAP_CMD_REJ_INVALID_CID) ? 4 : 0;

    if ((p_buf = l2cu_build_header (p_lcb->handle, (UINT16)(rej_len + 2),
                                    L2CAP_CMD_REJECT, local_id)) == NULL)
    {
        L2CAP_TRACE_WARNING0 ("L2CAP - no buffer cmd_rej");
        return;
    }

    p = (UINT8 *)(p_buf + 1) + 12;
    UINT16_TO_STREAM (p, reason);
    if (rej_len >= 2)
        UINT16_TO_STREAM (p, data1);
    if (rej_len >= 4)
        UINT16_TO_STREAM (p, data2);

    l2c_link_check_send_pkts (p_lcb, NULL, p_buf);
}

void l2cu_send_peer_connect_req (tL2C_CCB *p_ccb)
{
    BT_HDR *p_buf;
    UINT8  *p;
    UINT8   ident;

    ident = ++p_ccb->p_lcb->tx_ident;
    p_ccb->local_id = ident;

    if ((p_buf = l2cu_build_header (p_ccb->p_lcb->handle, L2CAP_CONN_REQ_LEN,
                                    L2CAP_CMD_CONN_REQ, ident)) == NULL)
    {
        L2CAP_TRACE_WARNING0 ("L2CAP - no buffer for conn_req");
        return;
    }

    p = (UINT8 *)(p_buf + 1) + 12;
    UINT16_TO_STREAM (p, p_ccb->p_rcb->psm);
    UINT16_TO_STREAM (p, p_ccb->local_cid);

    l2c_link_check_send_pkts (p_ccb->p_lcb, NULL, p_buf);
}

void l2cu_send_peer_connect_rsp (tL2C_CCB *p_ccb, UINT16 result, UINT16 status)
{
    BT_HDR *p_buf;
    UINT8  *p;

    if ((p_buf = l2cu_build_header (p_ccb->p_lcb->handle, L2CAP_CONN_RSP_LEN,
                                    L2CAP_CMD_CONN_RSP, p_ccb->remote_id)) == NULL)
    {
        L2CAP_TRACE_WARNING0 ("L2CAP - no buffer for conn_req");
        return;
    }

    p = (UINT8 *)(p_buf + 1) + 12;
    UINT16_TO_STREAM (p, p_ccb->local_cid);
    UINT16_TO_STREAM (p, p_ccb->remote_cid);
    UINT16_TO_STREAM (p, result);
    UINT16_TO_STREAM (p, status);

    l2c_link_check_send_pkts (p_ccb->p_lcb, NULL, p_buf);
}

void l2cu_reject_connection (tL2C_LCB *p_lcb, UINT16 psm, UINT8 remote_id, UINT16 result)
{
    BT_HDR *p_buf;
    UINT8  *p;

    if ((p_buf = l2cu_build_header (p_lcb->handle, L2CAP_CONN_RSP_LEN,
                                    L2CAP_CMD_CONN_RSP, remote_id)) == NULL)
    {
        L2CAP_TRACE_WARNING0 ("L2CAP - no buffer for conn_req");
        return;
    }

    p = (UINT8 *)(p_buf + 1) + 12;
    UINT16_TO_STREAM (p, 0);
    UINT16_TO_STREAM (p, psm);
    UINT16_TO_STREAM (p, result);
    UINT16_TO_STREAM (p, 0);

    l2c_link_check_send_pkts (p_lcb, NULL, p_buf);
}

void l2cu_send_peer_config_req (tL2C_CCB *p_ccb, tL2C_CFG_INFO *p_cfg)
{
    BT_HDR *p_buf;
    UINT8  *p;
    UINT16  len = 0;

    p_ccb->local_id = ++p_ccb->p_lcb->tx_ident;

    if (p_cfg->mtu_present)
        len = 4;
    if (p_cfg->flush_to_present)
        len = (UINT16)(len + 4);
    if (p_cfg->qos_present)
        len = (UINT16)(len + L2CAP_CFG_QOS_OPTION_LEN + L2CAP_CFG_OPTION_OVERHEAD);
    if (p_cfg->fcr_present)
        len = (UINT16)(len + L2CAP_CFG_FCR_OPTION_LEN + L2CAP_CFG_OPTION_OVERHEAD);

    if ((p_buf = l2cu_build_header (p_ccb->p_lcb->handle, (UINT16)(len + 4),
                                    L2CAP_CMD_CONFIG_REQ, p_ccb->local_id)) == NULL)
    {
        L2CAP_TRACE_WARNING0 ("L2CAP - no buffer for conn_req");
        return;
    }

    p = (UINT8 *)(p_buf + 1) + 12;
    UINT16_TO_STREAM (p, p_ccb->remote_cid);
    UINT16_TO_STREAM (p, 0);

    if (p_cfg->mtu_present)
    {
        UINT8_TO_STREAM (p, L2CAP_CFG_TYPE_MTU);
        UINT8_TO_STREAM (p, L2CAP_CFG_MTU_OPTION_LEN);
        UINT16_TO_STREAM (p, p_cfg->mtu);
    }

    if (p_cfg->flush_to_present)
    {
        UINT8_TO_STREAM (p, L2CAP_CFG_TYPE_FLUSH_TOUT);
        UINT8_TO_STREAM (p, L2CAP_CFG_FLUSH_OPTION_LEN);
        UINT16_TO_STREAM (p, p_cfg->flush_to);
    }

    if (p_cfg->qos_present)
    {
        UINT8_TO_STREAM (p, L2CAP_CFG_TYPE_QOS);
        UINT8_TO_STREAM (p, L2CAP_CFG_QOS_OPTION_LEN);
        UINT8_TO_STREAM (p, p_cfg->qos.qos_flags);
        UINT8_TO_STREAM (p, p_cfg->qos.service_type);
        UINT32_TO_STREAM (p, p_cfg->qos.token_rate);
        UINT32_TO_STREAM (p, p_cfg->qos.token_bucket_size);
        UINT32_TO_STREAM (p, p_cfg->qos.peak_bandwidth);
        UINT32_TO_STREAM (p, p_cfg->qos.latency);
        UINT32_TO_STREAM (p, p_cfg->qos.delay_variation);
    }

    if (p_cfg->fcr_present)
    {
        UINT8_TO_STREAM (p, L2CAP_CFG_TYPE_FCR);
        UINT8_TO_STREAM (p, L2CAP_CFG_FCR_OPTION_LEN);
        UINT8_TO_STREAM (p, p_cfg->fcr.mode);
        UINT8_TO_STREAM (p, p_cfg->fcr.tx_win_sz);
        UINT8_TO_STREAM (p, p_cfg->fcr.max_transmit);
        UINT16_TO_STREAM (p, p_cfg->fcr.rtrans_tout);
        UINT16_TO_STREAM (p, p_cfg->fcr.mon_tout);
        UINT16_TO_STREAM (p, p_cfg->fcr.mps);
    }

    l2c_link_check_send_pkts (p_ccb->p_lcb, NULL, p_buf);
}

void l2cu_send_peer_config_rsp (tL2C_CCB *p_ccb, tL2C_CFG_INFO *p_cfg)
{
    BT_HDR *p_buf;
    UINT8  *p;
    UINT16  len = 0;

    if (p_cfg->mtu_present)
        len = 4;
    if (p_cfg->flush_to_present)
        len = (UINT16)(len + 4);
    if (p_cfg->qos_present)
        len = (UINT16)(len + L2CAP_CFG_QOS_OPTION_LEN + L2CAP_CFG_OPTION_OVERHEAD);
    if (p_cfg->fcr_present)
        len = (UINT16)(len + L2CAP_CFG_FCR_OPTION_LEN + L2CAP_CFG_OPTION_OVERHEAD);

    if ((p_buf = l2cu_build_header (p_ccb->p_lcb->handle, (UINT16)(len + 6),
                                    L2CAP_CMD_CONFIG_RSP, p_ccb->remote_id)) == NULL)
    {
        L2CAP_TRACE_WARNING0 ("L2CAP - no buffer for conn_req");
        return;
    }

    p = (UINT8 *)(p_buf + 1) + 12;
    UINT16_TO_STREAM (p, p_ccb->remote_cid);
    UINT16_TO_STREAM (p, 0);
    UINT16_TO_STREAM (p, p_cfg->result);

    if (p_cfg->mtu_present)
    {
        UINT8_TO_STREAM (p, L2CAP_CFG_TYPE_MTU);
        UINT8_TO_STREAM (p, L2CAP_CFG_MTU_OPTION_LEN);
        UINT16_TO_STREAM (p, p_cfg->mtu);
    }

    if (p_cfg->flush_to_present)
    {
        UINT8_TO_STREAM (p, L2CAP_CFG_TYPE_FLUSH_TOUT);
        UINT8_TO_STREAM (p, L2CAP_CFG_FLUSH_OPTION_LEN);
        UINT16_TO_STREAM (p, p_cfg->flush_to);
    }

    if (p_cfg->qos_present)
    {
        UINT8_TO_STREAM (p, L2CAP_CFG_TYPE_QOS);
        UINT8_TO_STREAM (p, L2CAP_CFG_QOS_OPTION_LEN);
        UINT8_TO_STREAM (p, p_cfg->qos.qos_flags);
        UINT8_TO_STREAM (p, p_cfg->qos.service_type);
        UINT32_TO_STREAM (p, p_cfg->qos.token_rate);
        UINT32_TO_STREAM (p, p_cfg->qos.token_bucket_size);
        UINT32_TO_STREAM (p, p_cfg->qos.peak_bandwidth);
        UINT32_TO_STREAM (p, p_cfg->qos.latency);
        UINT32_TO_STREAM (p, p_cfg->qos.delay_variation);
    }

    if (p_cfg->fcr_present)
    {
        UINT8_TO_STREAM (p, L2CAP_CFG_TYPE_FCR);
        UINT8_TO_STREAM (p, L2CAP_CFG_FCR_OPTION_LEN);
        UINT8_TO_STREAM (p, p_cfg->fcr.mode);
        UINT8_TO_STREAM (p, p_cfg->fcr.tx_win_sz);
        UINT8_TO_STREAM (p, p_cfg->fcr.max_transmit);
        UINT16_TO_STREAM (p, p_cfg->fcr.rtrans_tout);
        UINT16_TO_STREAM (p, p_cfg->fcr.mon_tout);
        UINT16_TO_STREAM (p, p_cfg->fcr.mps);
    }

    l2c_link_check_send_pkts (p_ccb->p_lcb, NULL, p_buf);
}

void l2cu_send_peer_config_rej (tL2C_CCB *p_ccb, UINT8 *p_data, UINT16 data_len, UINT16 rej_len)
{
    BT_HDR *p_buf;
    UINT8  *p, *p_start, *p_end;

    if ((p_buf = (BT_HDR *)GKI_getpoolbuf (2)) == NULL)
    {
        L2CAP_TRACE_WARNING0 ("L2CAP - no buffer for cfg_rej");
        return;
    }

    p_buf->offset = 0;
    p_end = p_data + data_len;
    p = (UINT8 *)(p_buf + 1);

    UINT16_TO_STREAM (p, p_ccb->p_lcb->handle | 0x2000);
    p_start = p;

    /* Signalling header; the ACL length field (p_start[0..1]) is filled in
     * below once the unknown options have been copied back. */
    p = p_start + 2;
    UINT16_TO_STREAM (p, rej_len + 10);
    UINT8_TO_STREAM  (p, L2CAP_SIGNALLING_CID);
    UINT8_TO_STREAM  (p, 0);
    UINT8_TO_STREAM  (p, L2CAP_CMD_CONFIG_RSP);
    UINT8_TO_STREAM  (p, p_ccb->remote_id);
    UINT16_TO_STREAM (p, rej_len + 6);
    UINT16_TO_STREAM (p, p_ccb->remote_cid);
    UINT16_TO_STREAM (p, 0);
    UINT16_TO_STREAM (p, L2CAP_CFG_UNKNOWN_OPTIONS);

    /* Copy the unknown options back to the peer. */
    while (p_data < p_end)
    {
        UINT8 opt_len  = p_data[1];
        UINT8 opt_type = p_data[0];

        switch (opt_type & 0x7F)
        {
            case L2CAP_CFG_TYPE_MTU:
            case L2CAP_CFG_TYPE_FLUSH_TOUT:
            case L2CAP_CFG_TYPE_QOS:
                p_data += opt_len + 2;
                continue;
            default:
                break;
        }

        {
            INT32 opt_size = opt_len + 2;

            if (opt_size <= data_len)
            {
                if (!(opt_type & 0x80))
                {
                    memcpy (p, p_data, opt_size);
                    p += opt_size;
                }
                p_data += opt_size;
            }
            else
            {
                p_data = p_end;
            }
        }
    }

    {
        UINT16 acl_len = (UINT16)(p - p_start - 2);
        p_start[0] = (UINT8)acl_len;
        p_start[1] = (UINT8)(acl_len >> 8);
        p_buf->len = (UINT16)(acl_len + 4);
    }

    l2c_link_check_send_pkts (p_ccb->p_lcb, p_ccb, p_buf);
}

void l2cu_send_peer_disc_req (tL2C_CCB *p_ccb)
{
    BT_HDR *p_buf;
    UINT8  *p;

    p_ccb->local_id = ++p_ccb->p_lcb->tx_ident;

    if ((p_buf = l2cu_build_header (p_ccb->p_lcb->handle, L2CAP_DISC_REQ_LEN,
                                    L2CAP_CMD_DISC_REQ, p_ccb->local_id)) == NULL)
    {
        L2CAP_TRACE_WARNING0 ("L2CAP - no buffer for conn_req");
        return;
    }

    p = (UINT8 *)(p_buf + 1) + 12;
    UINT16_TO_STREAM (p, p_ccb->remote_cid);
    UINT16_TO_STREAM (p, p_ccb->local_cid);

    p_buf->layer_specific = 0;
    GKI_enqueue (&p_ccb->p_lcb->held_data_q, p_buf);
    l2c_link_check_send_pkts (p_ccb->p_lcb, NULL, NULL);
}

void l2cu_send_peer_disc_rsp (tL2C_LCB *p_lcb, UINT8 remote_id, UINT16 local_cid, UINT16 remote_cid)
{
    BT_HDR *p_buf;
    UINT8  *p;

    if ((p_buf = l2cu_build_header (p_lcb->handle, L2CAP_DISC_RSP_LEN,
                                    L2CAP_CMD_DISC_RSP, remote_id)) == NULL)
    {
        L2CAP_TRACE_WARNING0 ("L2CAP - no buffer for conn_req");
        return;
    }

    p = (UINT8 *)(p_buf + 1) + 12;
    UINT16_TO_STREAM (p, local_cid);
    UINT16_TO_STREAM (p, remote_cid);

    p_buf->layer_specific = 0;
    GKI_enqueue (&p_lcb->held_data_q, p_buf);
    l2c_link_check_send_pkts (p_lcb, NULL, NULL);
}

void l2cu_send_peer_echo_req (tL2C_LCB *p_lcb, UINT8 *p_data, UINT16 data_len)
{
    BT_HDR *p_buf;
    UINT8  *p;

    if ((p_buf = l2cu_build_header (p_lcb->handle, data_len, L2CAP_CMD_ECHO_REQ,
                                    (UINT8)(++p_lcb->tx_ident))) == NULL)
    {
        L2CAP_TRACE_WARNING0 ("L2CAP - no buffer for echo_req");
        return;
    }

    p = (UINT8 *)(p_buf + 1) + 12;

    if (data_len)
    {
        ARRAY_TO_STREAM (p, p_data, data_len);
    }

    l2c_link_check_send_pkts (p_lcb, NULL, p_buf);
}

void l2cu_send_peer_echo_rsp (tL2C_LCB *p_lcb, UINT8 remote_id, UINT8 *p_data, UINT16 data_len)
{
    BT_HDR *p_buf;
    UINT8  *p;
    UINT16  max_len;

    if (btu_cb.hcit_acl_pkt_size < L2CAP_ECHO_MAX_DATA_SIZE)
        max_len = btu_cb.hcit_acl_data_size;
    else
        max_len = L2CAP_ECHO_MAX_DATA_SIZE;

    if (data_len > (UINT16)(max_len - 12))
        data_len = 0;

    if ((p_buf = l2cu_build_header (p_lcb->handle, data_len, L2CAP_CMD_ECHO_RSP,
                                    remote_id)) == NULL)
    {
        L2CAP_TRACE_WARNING0 ("L2CAP - no buffer for conn_req");
        return;
    }

    p = (UINT8 *)(p_buf + 1) + 12;

    if (data_len)
    {
        ARRAY_TO_STREAM (p, p_data, data_len);
    }

    l2c_link_check_send_pkts (p_lcb, NULL, p_buf);
}

void l2cu_send_peer_info_rsp (tL2C_LCB *p_lcb, UINT8 remote_id, UINT16 info_type)
{
    BT_HDR *p_buf;
    UINT8  *p;

    if ((p_buf = l2cu_build_header (p_lcb->handle, L2CAP_INFO_RSP_LEN,
                                    L2CAP_CMD_INFO_RSP, remote_id)) == NULL)
    {
        L2CAP_TRACE_WARNING0 ("L2CAP - no buffer for conn_req");
        return;
    }

    p = (UINT8 *)(p_buf + 1) + 12;
    UINT16_TO_STREAM (p, info_type);
    UINT16_TO_STREAM (p, 1);

    l2c_link_check_send_pkts (p_lcb, NULL, p_buf);
}

/*******************************************************************************
 * Channel control block (CCB) management
 ******************************************************************************/

tL2C_CCB *l2cu_allocate_ccb (tL2C_LCB *p_lcb)
{
    tL2C_CCB *p_ccb = l2cb.ccb_free_q.p_first;

    if (p_ccb == NULL)
        return (NULL);

    l2cb.ccb_free_q.p_first = p_ccb->p_next_ccb;

    p_ccb->in_use    = TRUE;
    p_ccb->local_cid = (UINT16)(p_ccb - l2cb.ccb_pool) + L2CAP_BASE_APPL_CID;
    p_ccb->p_lcb     = p_lcb;

    /* Link it to the LCB's channel queue. */
    if (p_lcb->p_first_ccb == NULL)
    {
        p_lcb->p_last_ccb   = p_ccb;
        p_lcb->p_first_ccb  = p_ccb;
        p_ccb->p_next_ccb   = NULL;
        p_ccb->p_prev_ccb   = NULL;
    }
    else
    {
        p_ccb->p_next_ccb = NULL;
        p_ccb->p_prev_ccb = p_lcb->p_last_ccb;
        p_lcb->p_last_ccb->p_next_ccb = p_ccb;
        p_lcb->p_last_ccb = p_ccb;
    }

    p_ccb->our_flush_to  = L2CAP_DEFAULT_FLUSH_TO;
    p_ccb->out_mtu       = L2CAP_DEFAULT_MTU;
    p_ccb->in_mtu        = L2CAP_DEFAULT_MTU;

    p_ccb->our_cfg_qos.service_type      = L2CAP_DEFAULT_SERV_TYPE;
    p_ccb->peer_cfg_qos.service_type     = L2CAP_DEFAULT_SERV_TYPE;
    p_ccb->our_cfg_qos.token_rate        = L2CAP_DEFAULT_TOKEN_RATE;
    p_ccb->peer_cfg_qos.token_rate       = L2CAP_DEFAULT_TOKEN_RATE;
    p_ccb->our_cfg_qos.token_bucket_size = L2CAP_DEFAULT_BUCKET_SIZE;
    p_ccb->peer_cfg_qos.token_bucket_size = L2CAP_DEFAULT_BUCKET_SIZE;
    p_ccb->our_cfg_qos.peak_bandwidth    = L2CAP_DEFAULT_PEAK_BANDWIDTH;
    p_ccb->peer_cfg_qos.peak_bandwidth   = L2CAP_DEFAULT_PEAK_BANDWIDTH;
    p_ccb->our_cfg_qos.latency           = L2CAP_DEFAULT_LATENCY;
    p_ccb->peer_cfg_qos.latency          = L2CAP_DEFAULT_LATENCY;
    p_ccb->our_cfg_qos.delay_variation   = L2CAP_DEFAULT_DELAY;
    p_ccb->peer_cfg_qos.delay_variation  = L2CAP_DEFAULT_DELAY;

    p_ccb->config_done = FALSE;
    p_ccb->chnl_state  = 0;
    p_ccb->rtry_flag   = TRUE;

    p_ccb->timer_entry.param = (TIMER_PARAM_TYPE)p_ccb;

    return (p_ccb);
}

void l2cu_release_ccb (tL2C_CCB *p_ccb)
{
    tL2C_LCB *p_lcb = p_ccb->p_lcb;

    if (!p_ccb->in_use)
        return;

    p_ccb->in_use = FALSE;

    btu_stop_timer (&p_ccb->timer_entry);

    while (p_ccb->xmit_hold_q.p_first != NULL)
        GKI_freebuf (GKI_dequeue (&p_ccb->xmit_hold_q));

    p_ccb->p_lcb = NULL;

    /* Unlink from the LCB's channel queue. */
    if (p_lcb->p_first_ccb == p_ccb)
    {
        p_lcb->p_first_ccb = p_ccb->p_next_ccb;
        if (p_lcb->p_first_ccb != NULL)
            p_lcb->p_first_ccb->p_prev_ccb = NULL;
    }
    else if (p_lcb->p_last_ccb == p_ccb)
    {
        p_lcb->p_last_ccb = p_ccb->p_prev_ccb;
        p_lcb->p_last_ccb->p_next_ccb = NULL;
    }
    else
    {
        p_ccb->p_prev_ccb->p_next_ccb = p_ccb->p_next_ccb;
        p_ccb->p_next_ccb->p_prev_ccb = p_ccb->p_prev_ccb;
    }

    /* Put it back on the free queue. */
    if (l2cb.ccb_free_q.p_first == NULL)
    {
        l2cb.ccb_free_q.p_first = p_ccb;
        l2cb.ccb_free_q.p_last  = p_ccb;
        p_ccb->p_next_ccb = NULL;
        p_ccb->p_prev_ccb = NULL;
    }
    else
    {
        p_ccb->p_next_ccb = NULL;
        p_ccb->p_prev_ccb = l2cb.ccb_free_q.p_last;
        l2cb.ccb_free_q.p_last->p_next_ccb = p_ccb;
        l2cb.ccb_free_q.p_last = p_ccb;
    }

    /* If this was the last channel on a disconnecting link, drop the link. */
    if ((p_lcb->in_use) && (p_lcb->link_state == LST_DISCONNECTING) && (p_lcb->p_first_ccb == NULL))
    {
        UINT16 tout = p_lcb->idle_timeout;

        if (tout == 0)
        {
            tBTM_STATUS status = btm_sec_disconnect (p_lcb->handle, HCI_ERR_PEER_USER);

            if (status == BTM_CMD_STARTED)
            {
                p_lcb->link_state = LST_DISCONNECT_WAIT;
                tout = L2CAP_LINK_DISCONNECT_TOUT;
            }
            else if (status == BTM_SUCCESS)
            {
                p_lcb->link_state = LST_DISCONNECT_WAIT;
                tout = L2CAP_NO_AUTOMATIC_FLUSH;
            }
            else
            {
                tout = 1;
            }
        }

        if (tout != L2CAP_NO_AUTOMATIC_FLUSH)
            btu_start_timer (&p_lcb->timer_entry, BTU_TTYPE_L2CAP_LINK, tout);
    }
}

tL2C_CCB *l2cu_find_ccb_by_cid (tL2C_LCB *p_lcb, UINT16 cid)
{
    tL2C_CCB *p_ccb = NULL;

    if (cid >= L2CAP_BASE_APPL_CID)
    {
        cid = (UINT16)(cid - L2CAP_BASE_APPL_CID);
        p_ccb = &l2cb.ccb_pool[cid];

        if (!p_ccb->in_use)
            p_ccb = NULL;
        else if ((p_lcb != NULL) && (p_lcb != p_ccb->p_lcb))
            p_ccb = NULL;
    }

    return (p_ccb);
}

/*******************************************************************************
 * Registration control block (RCB) management
 ******************************************************************************/

tL2C_RCB *l2cu_allocate_rcb (UINT16 psm)
{
    UINT8     xx;
    tL2C_RCB *p_rcb;

    for (xx = 0, p_rcb = &l2cb.rcb_pool[0]; xx < L2C_MAX_CLIENTS; xx++, p_rcb++)
    {
        if (!p_rcb->in_use)
        {
            p_rcb->in_use = TRUE;
            p_rcb->psm    = psm;
            return (p_rcb);
        }
    }

    return (NULL);
}

void l2cu_release_rcb (tL2C_RCB *p_rcb)
{
    p_rcb->in_use = FALSE;
    p_rcb->psm    = 0;
}

tL2C_RCB *l2cu_find_rcb_by_psm (UINT16 psm)
{
    UINT8     xx;
    tL2C_RCB *p_rcb;

    for (xx = 0, p_rcb = &l2cb.rcb_pool[0]; xx < L2C_MAX_CLIENTS; xx++, p_rcb++)
    {
        if (p_rcb->in_use && (p_rcb->psm == psm))
            return (p_rcb);
    }

    return (NULL);
}

/*******************************************************************************
 * Configuration processing
 ******************************************************************************/

BOOLEAN l2cu_process_peer_cfg_req (tL2C_CCB *p_ccb, tL2C_CFG_INFO *p_cfg)
{
    BOOLEAN mtu_ok   = TRUE;
    BOOLEAN qos_ok   = TRUE;
    BOOLEAN flush_ok = TRUE;
    BOOLEAN fcr_ok   = TRUE;
    BOOLEAN retval;

    if (p_cfg->mtu_present)
    {
        if (p_cfg->mtu >= L2CAP_MIN_MTU)
        {
            p_ccb->out_mtu = p_cfg->mtu;

            if (p_ccb->out_mtu > L2CAP_MAX_MTU)
                p_ccb->out_mtu = p_cfg->mtu = L2CAP_MAX_MTU;
        }
        else
        {
            p_cfg->mtu = L2CAP_MIN_MTU;
            mtu_ok = FALSE;
        }
    }

    if (p_cfg->flush_to_present)
    {
        if (p_cfg->flush_to == 0)
        {
            flush_ok = FALSE;
            p_cfg->flush_to = L2CAP_DEFAULT_FLUSH_TO;
        }
    }

    if (p_cfg->qos_present)
    {
        if (p_cfg->qos.service_type <= GUARANTEED)
        {
            p_ccb->peer_cfg_qos = p_cfg->qos;
        }
        else
        {
            p_cfg->qos.service_type = BEST_EFFORT;
            qos_ok = FALSE;
        }
    }

    retval = mtu_ok && flush_ok && qos_ok && fcr_ok;

    if (!retval)
    {
        p_cfg->result = L2CAP_CFG_UNACCEPTABLE_PARAMS;
        if (mtu_ok)
            p_cfg->mtu_present = FALSE;
        if (flush_ok)
            p_cfg->flush_to_present = FALSE;
        if (qos_ok)
            p_cfg->qos_present = FALSE;
        if (fcr_ok)
            p_cfg->fcr_present = FALSE;
    }

    return (retval);
}

void l2cu_process_peer_cfg_rsp (tL2C_CCB *p_ccb, tL2C_CFG_INFO *p_cfg)
{
    if (!p_cfg->qos_present)
        return;

    p_ccb->our_cfg_qos = p_cfg->qos;
}

void l2cu_process_our_cfg_req (tL2C_CCB *p_ccb, tL2C_CFG_INFO *p_cfg)
{
    tL2C_LCB *p_lcb;

    if (p_cfg->mtu_present)
    {
        UINT16 mtu = p_cfg->mtu;

        p_ccb->in_mtu = mtu;

        if (mtu > L2CAP_MAX_MTU)
        {
            p_cfg->mtu    = L2CAP_MAX_MTU;
            p_ccb->in_mtu = L2CAP_MAX_MTU;
        }
    }

    if (p_cfg->qos_present)
        p_ccb->our_cfg_qos = p_cfg->qos;

    if (p_cfg->flush_to_present && (p_cfg->flush_to != 0))
    {
        p_ccb->our_flush_to = p_cfg->flush_to;
        p_lcb = p_ccb->p_lcb;

        if (p_cfg->flush_to < p_lcb->flush_tout)
        {
            p_lcb->flush_tout = p_cfg->flush_to;

            if (p_cfg->flush_to <= 0x4FF)
            {
                UINT16  hci_flush_tout = (UINT16)((p_cfg->flush_to * 8 + 3) / 5);
                BT_HDR *p_buf = (BT_HDR *)GKI_getpoolbuf (2);

                if (p_buf != NULL)
                    btsnd_hcic_write_auto_flush_tout (p_buf, p_lcb->handle, hci_flush_tout);
            }
        }
    }
}

void l2cu_process_our_cfg_rsp (tL2C_CCB *p_ccb, tL2C_CFG_INFO *p_cfg)
{
    if (!p_cfg->qos_present)
        return;

    p_ccb->peer_cfg_qos = p_cfg->qos;
}

/*******************************************************************************
 * Device / connection management
 ******************************************************************************/

void l2cu_device_reset (void)
{
    INT8      xx;
    tL2C_LCB *p_lcb;

    for (xx = 0, p_lcb = &l2cb.lcb_pool[0]; xx < L2C_MAX_LINKS; xx++, p_lcb++)
    {
        if (p_lcb->in_use && (p_lcb->handle != 0xFFFF))
            l2c_link_hci_disc_comp (p_lcb->handle, 0xFF);
    }
}

BOOLEAN l2cu_create_conn (tL2C_LCB *p_lcb)
{
    INT8      xx;
    tL2C_LCB *p_lcb2;
    BOOLEAN   is_sco_active;

    p_lcb->link_state = LST_CONNECTING;

    for (xx = 0, p_lcb2 = &l2cb.lcb_pool[0]; xx < L2C_MAX_LINKS; xx++, p_lcb2++)
    {
        if (p_lcb2 == p_lcb)
            continue;
        if (!p_lcb2->in_use)
            continue;
        if (p_lcb2->switch_pending != 1)
            continue;

        is_sco_active = btm_is_sco_active_by_bdaddr (p_lcb2->remote_bd_addr);
        L2CAP_TRACE_API1 ("l2cu_create_conn - btm_is_sco_active_by_bdaddr() is_sco_active = %s",
                          (is_sco_active == TRUE) ? lbl_80665948 : lbl_80665950);
        if (is_sco_active == TRUE)
            continue;

        if (!(btm_cb.role_switch_flags & 0x20))
            continue;

        p_lcb->link_state     = LST_CONNECT_WAIT_SWITCH;
        p_lcb->switch_pending = 0;
        BTM_SwitchRole (p_lcb2->remote_bd_addr, HCI_ROLE_MASTER, NULL);
        btu_start_timer (&p_lcb->timer_entry, BTU_TTYPE_L2CAP_LINK, L2CAP_LINK_SWITCH_TOUT);
        return (TRUE);
    }

    return (l2cu_create_conn_after_switch (p_lcb));
}

BOOLEAN l2cu_create_conn_after_switch (tL2C_LCB *p_lcb)
{
    UINT8         *p_features;
    UINT8          allow_switch;
    tBTM_INQ_INFO *p_inq_info;
    UINT8          page_scan_rep_mode;
    UINT8          page_scan_mode;
    UINT16         clock_offset;

    p_features = BTM_ReadLocalFeatures ();
    allow_switch = (UINT8)((p_features[0] & 0x20) >> 5);
    p_lcb->link_state = LST_CONNECTING;

    p_inq_info = BTM_InqDbRead (p_lcb->remote_bd_addr);
    if (p_inq_info != NULL)
    {
        page_scan_rep_mode = p_inq_info->page_scan_rep_mode;
        page_scan_mode     = p_inq_info->page_scan_mode;
        clock_offset       = (UINT16)(p_inq_info->clock_offset | 0x8000);
    }
    else
    {
        page_scan_rep_mode = 1;
        page_scan_mode     = 0;
        clock_offset       = 0;
    }

    if (!btsnd_hcic_create_conn (p_lcb->remote_bd_addr, 0x18, page_scan_rep_mode,
                                 page_scan_mode, clock_offset, allow_switch))
    {
        L2CAP_TRACE_ERROR0 ("L2CAP - no buffer for l2cu_create_conn");
        l2cu_release_lcb (p_lcb);
        return (FALSE);
    }

    btu_start_timer (&p_lcb->timer_entry, BTU_TTYPE_L2CAP_LINK, L2CAP_LINK_CONN_TOUT);
    return (TRUE);
}

tL2C_LCB *l2cu_find_lcb_by_state (INT32 state)
{
    UINT8     xx;
    tL2C_LCB *p_lcb;

    for (xx = 0, p_lcb = &l2cb.lcb_pool[0]; xx < L2C_MAX_LINKS; xx++, p_lcb++)
    {
        if (p_lcb->in_use && (p_lcb->link_state == state))
            return (p_lcb);
    }

    return (NULL);
}

BOOLEAN l2cu_lcb_disconnecting (void)
{
    tL2C_LCB *p_lcb;
    tL2C_CCB *p_ccb;
    UINT8     xx;
    BOOLEAN   status = FALSE;

    p_lcb = &l2cb.lcb_pool[0];

    for (xx = 0; xx < L2C_MAX_LINKS; xx++, p_lcb++)
    {
        if (p_lcb->in_use)
        {
            if ((!p_lcb->p_first_ccb) || (p_lcb->link_state == LST_DISCONNECT_WAIT))
            {
                status = TRUE;
                break;
            }
            else if (p_lcb->p_first_ccb == p_lcb->p_last_ccb)
            {
                p_ccb = p_lcb->p_first_ccb;

                if ((p_ccb->in_use) &&
                    ((p_ccb->chnl_state == CST_W4_L2CAP_DISCONNECT_RSP) ||
                     (p_ccb->chnl_state == CST_W4_L2CA_DISCONNECT_RSP)))
                {
                    status = TRUE;
                    break;
                }
            }
        }
    }

    return (status);
}
