// L2CAP API functions - RVL retail slice.
// High-level C reconstruction of the Broadcom BTE l2c_api module.

#define REVOLUTION 1

#include <revolution/BTE/include/bt_target.h>
#include <revolution/BTE/stack/include/bt_types.h>
#include <revolution/BTE/gki/common/gki.h>
#include <string.h>

#include <revolution/BTE/stack/include/l2c_api.h>
#include <revolution/BTE/stack/include/l2cdefs.h>
#include <revolution/BTE/include/bt_trace.h>

/* Link pool size in the retail l2cb. */
#define L2C_MAX_LINKS               4

/* Link states in the retail L2CAP control block. */
typedef enum {
    LST_DISCONNECTED,
    LST_CONNECTING,
    LST_CONNECT_HOLDING,
    LST_CONNECTING_WAIT_SWITCH,
    LST_CONNECTED,
    LST_DISCONNECTING
} tL2C_LINK_STATE;

/*******************************************************************************
 * Internal types (normally in l2c_int.h, inlined here for this TU)
 ******************************************************************************/

/* Link control block (size 0x5C) - only the fields referenced by this TU. */
typedef struct t_l2c_linkcb tL2C_LCB;
typedef struct t_l2c_rcb    tL2C_RCB;

struct t_l2c_linkcb {
    BOOLEAN       in_use;                     /* 0x00 */
    UINT8         pad01[3];                   /* 0x01 */
    tL2C_LINK_STATE link_state;               /* 0x04 */
    UINT8         pad08[0x41 - 0x08];         /* 0x08..0x40 */
    BOOLEAN       cong_sent;                  /* 0x41 */
    UINT8         pad42[0x58 - 0x42];         /* 0x42..0x57 */
    UINT16        idle_timeout;               /* 0x58 */
    UINT8         pad5A[0x5C - 0x5A];         /* 0x5A..0x5B */
};

/* Retail tL2CAP_APPL_INFO holds 10 callback pointers (0x28 bytes); the
 * header extends it with pL2CA_TxComplete_Cb (0x2C).  The retail Register
 * copies only the first 10. */
typedef struct {
    void (*cb[10])(void);
} tL2C_APPL_INFO10;

/* Registration control block - one per registered PSM (size 0x2C). */
struct t_l2c_rcb {
    BOOLEAN        in_use;      /* 0x00 */
    UINT8          pad01;       /* 0x01 */
    UINT16         psm;         /* 0x02 */
    tL2C_APPL_INFO10 api;       /* 0x04: application callbacks (10) */
};

/* Channel control block - only the fields referenced by this TU. */
typedef struct t_l2c_ccb {
    UINT8     pad00[0x10];
    tL2C_LCB *p_lcb;                         /* 0x10 */
    UINT16    local_cid;                     /* 0x14 */
    UINT16    pad16;                         /* 0x16 */
    UINT8     pad18[0x30 - 0x18];            /* 0x18..0x2F */
    tL2C_RCB *p_rcb;                         /* 0x30 */
    UINT8     pad34[0x36 - 0x34];            /* 0x34..0x35 */
    UINT8     remote_id;                     /* 0x36 */
    UINT8     pad37[0x3A - 0x37];            /* 0x37..0x39 */
    UINT16    out_mtu;                       /* 0x3A */
    UINT8     pad3C[0x7C - 0x3C];            /* 0x3C..0x7B */
} tL2C_CCB;

/* Connection confirmation info passed to the channel state machine. */
typedef struct {
    BD_ADDR bd_addr;      /* 0x00 */
    UINT8   status;       /* 0x06 */
    UINT8   pad07;        /* 0x07 */
    UINT16  psm;          /* 0x08 */
    UINT16  l2cap_result; /* 0x0A */
    UINT16  l2cap_status; /* 0x0C */
    UINT16  remote_cid;   /* 0x0E */
} tL2C_CONN_INFO;

/* L2CAP control block (global `l2cb`). Only the fields referenced here are
 * named; the rest is opaque padding (must stay > 8 bytes so MWCC does not
 * treat `l2cb` as small data - retail references it via lis/@l, not @sda21). */
typedef struct {
    UINT8   l2cap_trace_level;              /* 0x000 */
    UINT8   pad001[0x008 - 0x001];          /* 0x001..0x007 */
    tL2C_LCB lcb_pool[L2C_MAX_LINKS];        /* 0x008..0x177 (4 x 0x5C) */
    UINT8   pad178[0x7BA - 0x178];          /* 0x178..0x7B9 */
    UINT16  idle_timeout;                   /* 0x7BA */
    UINT8   pad7BC[0x800 - 0x7BC];          /* 0x7BC..0x7FF */
} tL2C_CB;

/*******************************************************************************
 * External references
 ******************************************************************************/

extern tL2C_CB l2cb;

extern tL2C_RCB *l2cu_find_rcb_by_psm (UINT16 psm);
extern tL2C_RCB *l2cu_allocate_rcb (UINT16 psm);
extern void      l2cu_release_rcb (tL2C_RCB *p_rcb);
extern tL2C_CCB *l2cu_find_ccb_by_cid (tL2C_LCB *p_lcb, UINT16 cid);
extern tL2C_LCB *l2cu_find_lcb_by_bd_addr (BD_ADDR p_bd_addr);
extern tL2C_LCB *l2cu_allocate_lcb (BD_ADDR p_bd_addr);
extern tL2C_CCB *l2cu_allocate_ccb (tL2C_LCB *p_lcb);
extern BOOLEAN   l2cu_create_conn (tL2C_LCB *p_lcb);
extern BOOLEAN   BTM_IsDeviceUp (void);
extern void      l2c_csm_execute (tL2C_CCB *p_ccb, UINT16 event, void *p_data);

/* All-zero BD address (small-data const in the retail image). */
extern const unsigned char BT_BD_ANY[BD_ADDR_LEN];

/* Events fed to the channel state machine. */
#define L2CEVT_L2CA_CONNECT_REQ     20
#define L2CEVT_L2CA_CONNECT_RSP     21
#define L2CEVT_L2CA_CONNECT_RSP_NEG 22
#define L2CEVT_L2CA_CONFIG_REQ      23
#define L2CEVT_L2CA_CONFIG_RSP      24
#define L2CEVT_L2CA_CONFIG_RSP_NEG  25
#define L2CEVT_L2CA_DISCONNECT_REQ  26
#define L2CEVT_L2CA_DISCONNECT_RSP  27
#define L2CEVT_L2CA_DATA_WRITE      29

/*******************************************************************************
 * L2CAP API entry points
 ******************************************************************************/

UINT16 L2CA_Register (UINT16 psm, tL2CAP_APPL_INFO *p_cb_info)
{
    tL2C_RCB *p_rcb;

    L2CAP_TRACE_API1 ("L2CAP - L2CA_Register() called for PSM: 0x%04x", psm);

    if (!p_cb_info->pL2CA_ConfigCfm_Cb || !p_cb_info->pL2CA_ConfigInd_Cb ||
        !p_cb_info->pL2CA_DataInd_Cb || !p_cb_info->pL2CA_DisconnectInd_Cb)
    {
        L2CAP_TRACE_ERROR1 ("L2CAP - no cb registering PSM: 0x%04x", psm);
        return (FALSE);
    }

    if (L2C_INVALID_PSM (psm))
    {
        L2CAP_TRACE_ERROR1 ("L2CAP - invalid PSM value, PSM: 0x%04x", psm);
        return (FALSE);
    }

    p_rcb = l2cu_find_rcb_by_psm (psm);

    if (p_rcb == NULL)
    {
        p_rcb = l2cu_allocate_rcb (psm);

        if (p_rcb == NULL)
        {
            L2CAP_TRACE_WARNING1 ("L2CAP - no RCB available, PSM: 0x%04x", psm);
            return (FALSE);
        }
    }

    p_rcb->api = *(tL2C_APPL_INFO10 *)p_cb_info;

    return (TRUE);
}

void L2CA_Deregister (UINT16 psm)
{
    tL2C_RCB *p_rcb;

    L2CAP_TRACE_API1 ("L2CAP - L2CA_Deregister() called for PSM: 0x%04x", psm);

    p_rcb = l2cu_find_rcb_by_psm (psm);

    if (p_rcb != NULL)
        l2cu_release_rcb (p_rcb);
    else
        L2CAP_TRACE_WARNING1 ("L2CAP - PSM: 0x%04x not found for deregistration",
                              psm);
}

UINT16 L2CA_ConnectReq (UINT16 psm, BD_ADDR p_bd_addr)
{
    tL2C_LCB *p_lcb;
    tL2C_CCB *p_ccb;
    tL2C_RCB *p_rcb;

    L2CAP_TRACE_API1 ("L2CA_ConnectReq()  PSM: 0x%04x", psm);

    L2CAP_TRACE_API6 ("L2CA_ConnectReq()  BDA: %02x-%02x-%02x-%02x-%02x-%02x",
                      p_bd_addr[0], p_bd_addr[1], p_bd_addr[2], p_bd_addr[3],
                      p_bd_addr[4], p_bd_addr[5]);

    if (!BTM_IsDeviceUp ())
    {
        L2CAP_TRACE_WARNING0 ("L2CAP connect req - BTU not ready");
        return (0);
    }

    p_rcb = l2cu_find_rcb_by_psm (psm);

    if (p_rcb == NULL)
    {
        L2CAP_TRACE_WARNING1 ("L2CAP - no RCB for L2CA_conn_req, PSM: 0x%04x", psm);
        return (0);
    }

    p_lcb = l2cu_find_lcb_by_bd_addr (p_bd_addr);

    if (p_lcb == NULL)
    {
        p_lcb = l2cu_allocate_lcb (p_bd_addr);

        if (p_lcb == NULL)
        {
            L2CAP_TRACE_WARNING0 ("L2CAP - no LCB for L2CA_conn_req");
            return (0);
        }

        if (!l2cu_create_conn (p_lcb))
            return (0);
    }

    if (p_lcb->link_state == LST_DISCONNECTING)
    {
        L2CAP_TRACE_API0 ("L2CAP API - L2CA_conn_req rejected - link disconnecting");
        return (0);
    }

    p_ccb = l2cu_allocate_ccb (p_lcb);

    if (p_ccb == NULL)
    {
        L2CAP_TRACE_WARNING0 ("L2CAP - no CCB for L2CA_conn_req");
        return (0);
    }

    p_ccb->p_rcb = p_rcb;

    if (p_lcb->link_state == LST_CONNECTED)
        l2c_csm_execute (p_ccb, L2CEVT_L2CA_CONNECT_REQ, NULL);

    L2CAP_TRACE_API1 ("L2CAP - L2CA_conn_req() returned CID: 0x%04x",
                      p_ccb->local_cid);

    return (p_ccb->local_cid);
}

BOOLEAN L2CA_ConnectRsp (BD_ADDR p_bd_addr, UINT8 id, UINT16 lcid,
                         UINT16 result, UINT16 status)
{
    tL2C_LCB *p_lcb;
    tL2C_CCB *p_ccb;

    L2CAP_TRACE_API3 ("L2CA_ConnectRsp()  CID: 0x%04x  Result: %d  Status: %d",
                      lcid, result, status);

    L2CAP_TRACE_API6 ("L2CA_ConnectRsp()  BDA: %02x-%02x-%02x-%02x-%02x-%02x",
                      p_bd_addr[0], p_bd_addr[1], p_bd_addr[2], p_bd_addr[3],
                      p_bd_addr[4], p_bd_addr[5]);

    p_lcb = l2cu_find_lcb_by_bd_addr (p_bd_addr);

    if (p_lcb == NULL)
    {
        L2CAP_TRACE_WARNING0 ("L2CAP - no LCB for L2CA_conn_rsp");
        return (FALSE);
    }

    p_ccb = l2cu_find_ccb_by_cid (p_lcb, lcid);

    if (p_ccb == NULL)
    {
        L2CAP_TRACE_WARNING0 ("L2CAP - no CCB for L2CA_conn_rsp");
        return (FALSE);
    }

    if (p_ccb->remote_id != id)
    {
        L2CAP_TRACE_WARNING2 ("L2CAP - bad id in L2CA_conn_rsp. Exp: %d  Got: %d",
                              p_ccb->remote_id, id);
        return (FALSE);
    }

    if (result == L2CAP_CONN_OK)
    {
        l2c_csm_execute (p_ccb, L2CEVT_L2CA_CONNECT_RSP, NULL);
    }
    else
    {
        tL2C_CONN_INFO conn_info;

        conn_info.l2cap_result = result;
        conn_info.l2cap_status = status;

        if (result == L2CAP_CONN_PENDING)
            l2c_csm_execute (p_ccb, L2CEVT_L2CA_CONNECT_RSP, &conn_info);
        else
            l2c_csm_execute (p_ccb, L2CEVT_L2CA_CONNECT_RSP_NEG, &conn_info);
    }

    return (TRUE);
}

BOOLEAN L2CA_ConfigReq (UINT16 cid, tL2CAP_CFG_INFO *p_cfg)
{
    tL2C_CCB *p_ccb;

    L2CAP_TRACE_API1 ("L2CA_ConfigReq()  CID: 0x%04x", cid);

    p_ccb = l2cu_find_ccb_by_cid (NULL, cid);

    if (p_ccb == NULL)
    {
        L2CAP_TRACE_WARNING1 ("L2CAP - no CCB for L2CA_cfg_req, CID: %d", cid);
        return (FALSE);
    }

    p_cfg->fcr_present = 0;

    l2c_csm_execute (p_ccb, L2CEVT_L2CA_CONFIG_REQ, p_cfg);

    return (TRUE);
}

BOOLEAN L2CA_ConfigRsp (UINT16 cid, tL2CAP_CFG_INFO *p_cfg)
{
    tL2C_CCB *p_ccb;

    L2CAP_TRACE_API2 ("L2CA_ConfigRsp()  CID: 0x%04x  Result: %d", cid,
                      p_cfg->result);

    p_ccb = l2cu_find_ccb_by_cid (NULL, cid);

    if (p_ccb == NULL)
    {
        L2CAP_TRACE_WARNING1 ("L2CAP - no CCB for L2CA_cfg_rsp, CID: %d", cid);
        return (FALSE);
    }

    if (p_cfg->result == L2CAP_CFG_OK)
        l2c_csm_execute (p_ccb, L2CEVT_L2CA_CONFIG_RSP, p_cfg);
    else
        l2c_csm_execute (p_ccb, L2CEVT_L2CA_CONFIG_RSP_NEG, p_cfg);

    return (TRUE);
}

BOOLEAN L2CA_DisconnectReq (UINT16 cid)
{
    tL2C_CCB *p_ccb;

    L2CAP_TRACE_API1 ("L2CA_DisconnectReq()  CID: 0x%04x", cid);

    p_ccb = l2cu_find_ccb_by_cid (NULL, cid);

    if (p_ccb == NULL)
    {
        L2CAP_TRACE_WARNING1 ("L2CAP - no CCB for L2CA_disc_req, CID: %d", cid);
        return (FALSE);
    }

    l2c_csm_execute (p_ccb, L2CEVT_L2CA_DISCONNECT_REQ, NULL);

    return (TRUE);
}

BOOLEAN L2CA_DisconnectRsp (UINT16 cid)
{
    tL2C_CCB *p_ccb;

    L2CAP_TRACE_API1 ("L2CA_DisconnectRsp()  CID: 0x%04x", cid);

    p_ccb = l2cu_find_ccb_by_cid (NULL, cid);

    if (p_ccb == NULL)
    {
        L2CAP_TRACE_WARNING1 ("L2CAP - no CCB for L2CA_disc_rsp, CID: %d", cid);
        return (FALSE);
    }

    l2c_csm_execute (p_ccb, L2CEVT_L2CA_DISCONNECT_RSP, NULL);

    return (TRUE);
}

UINT8 L2CA_DataWrite (UINT16 cid, BT_HDR *p_data)
{
    tL2C_CCB *p_ccb;

    L2CAP_TRACE_API2 ("L2CA_DataWrite()  CID: 0x%04x  Len: %d", cid,
                      p_data->len);

    p_ccb = l2cu_find_ccb_by_cid (NULL, cid);

    if (p_ccb == NULL)
    {
        L2CAP_TRACE_WARNING1 ("L2CAP - no CCB for L2CA_DataWrite, CID: %d", cid);

        GKI_freebuf (p_data);
        return (L2CAP_DW_FAILED);
    }

    /* Check that the length will fit in the buffer. */
    if (p_data->len <= p_ccb->out_mtu)
    {
        l2c_csm_execute (p_ccb, L2CEVT_L2CA_DATA_WRITE, p_data);

        if (p_ccb->p_lcb->cong_sent)
            return (L2CAP_DW_CONGESTED);
    }
    else
    {
        L2CAP_TRACE_WARNING0 ("L2CAP - cannot send message bigger than peer's mtu size");

        GKI_freebuf (p_data);
        return (L2CAP_DW_FAILED);
    }

    return (L2CAP_DW_SUCCESS);
}

// L2CA_Ping is not present in Xenoblade retail .text; these named arrays
// reproduce its trace strings in the retail .data (between L2CA_DataWrite and
// L2CA_SetIdleTimeout).
char s_l2ca_ping_bda[0x30] =
    "L2CA_Ping()  BDA: %02x-%02x-%02x-%02x-%02x-%02x";
char s_l2ca_ping_no_lcb[0x1d] = "L2CAP - no LCB for L2CA_ping";
char s_l2ca_ping_reject2[0x22] = "L2CAP - rejected second L2CA_ping";
char s_l2ca_ping_reject_link[0x30] =
    "L2CAP - L2CA_ping rejected - link disconnecting";

BOOLEAN L2CA_SetIdleTimeout (UINT16 cid, UINT16 timeout, BOOLEAN is_global)
{
    tL2C_CCB *p_ccb;
    tL2C_LCB *p_lcb;

    if (is_global)
    {
        l2cb.idle_timeout = timeout;
    }
    else
    {
        p_ccb = l2cu_find_ccb_by_cid (NULL, cid);

        if (p_ccb == NULL)
        {
            L2CAP_TRACE_WARNING1 ("L2CAP - no CCB for L2CA_SetIdleTimeout, CID: %d",
                                  cid);
            return (FALSE);
        }

        p_lcb = p_ccb->p_lcb;

        if ((p_lcb != NULL) && (p_lcb->in_use) &&
            (p_lcb->link_state == LST_CONNECTED))
        {
            p_lcb->idle_timeout = timeout;
        }
        else
        {
            return (FALSE);
        }
    }

    return (TRUE);
}

BOOLEAN L2CA_SetIdleTimeoutByBdAddr (BD_ADDR bd_addr, UINT16 timeout)
{
    tL2C_LCB *p_lcb;

    if (memcmp (BT_BD_ANY, bd_addr, BD_ADDR_LEN) != 0)
    {
        p_lcb = l2cu_find_lcb_by_bd_addr (bd_addr);

        if (p_lcb && p_lcb->in_use && (p_lcb->link_state == LST_CONNECTED))
        {
            p_lcb->idle_timeout = timeout;
        }
        else
        {
            return (FALSE);
        }
    }
    else
    {
        int xx;
        tL2C_LCB *p_lcb = &l2cb.lcb_pool[0];

        for (xx = 0; xx < L2C_MAX_LINKS; xx++, p_lcb++)
        {
            if (p_lcb->in_use && (p_lcb->link_state == LST_CONNECTED))
            {
                p_lcb->idle_timeout = timeout;
            }
        }
    }

    return (TRUE);
}

UINT8 L2CA_SetTraceLevel (UINT8 trace_level)
{
    if (trace_level != 0xFF)
        l2cb.l2cap_trace_level = trace_level;

    return (l2cb.l2cap_trace_level);
}

void L2CA_RegisterCompression (void)
{
}

// L2CA_SetAclPriority/L2CA_SetCompression/L2CA_Flush/L2CA_GetNumQueuedBufs
// are not present in Xenoblade retail .text; these named arrays reproduce
// their trace strings in the retail .data tail.
char s_l2ca_acl_prio[0x38] =
    "L2CA_SetAclPriority()  bdaddr: %02x%02x%02x%02x%02x%02x";
char s_l2ca_acl_prio_no_lcb[0x27] = "L2CAP - no LCB for L2CA_SetAclPriority";
char s_l2ca_set_comp[0x60] =
    "L2CA_SetCompression() local cid %d, direction %d, pe_type %d, mem_level %d, wbits %d, enable %d";
char s_l2ca_flush_no_ccb[0x27] = "L2CAP - no CCB for L2CA_Flush, CID: %d";
char s_l2ca_flush[0x2d] = "L2CA_Flush()  CID: 0x%04x flushed %d buffers";
char s_l2ca_qbuf_abnormal[0x3d] =
    "L2CA_GetNumQueuedBufs()  CID: 0x%04x  abmormally returning 0";
char s_l2ca_qbuf[0x38] =
    "L2CA_GetNumQueuedBufs()  CID: 0x%04x  returning %d";
