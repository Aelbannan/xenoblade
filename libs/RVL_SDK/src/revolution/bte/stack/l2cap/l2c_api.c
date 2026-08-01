// L2CAP API functions - RVL retail slice.
// High-level C reconstruction of the Broadcom BTE l2c_api module.

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

/* L2CAP control block (global `l2cb`). Only the fields referenced here are
 * named; the rest is opaque padding (must stay > 8 bytes so MWCC does not
 * treat `l2cb` as small data — retail references it via lis/@l, not @sda21). */
typedef struct {
    UINT8   l2cap_trace_level;              /* 0x000 */
    UINT8   pad001[0x7B9];                  /* 0x001..0x7B9 */
    UINT16  idle_timeout;                   /* 0x7BA */
    UINT8   pad7BC[0x800 - 0x7BC];          /* 0x7BC..0x7FF */
} tL2C_CB;

/* Link control block — only the fields referenced by this TU. */
typedef struct t_l2c_linkcb tL2C_LCB;
typedef struct t_l2c_rcb    tL2C_RCB;

struct t_l2c_linkcb {
    BOOLEAN in_use;                          /* 0x00 */
    UINT8   pad01[3];                        /* 0x01 */
    INT32   link_state;                      /* 0x04 */
    UINT8   pad08[0x58 - 0x08];              /* 0x08..0x57 */
    UINT16  idle_timeout;                    /* 0x58 */
};

/* Channel control block — only the fields referenced by this TU. */
typedef struct t_l2c_ccb {
    UINT8     pad00[0x10];
    tL2C_LCB *p_lcb;                         /* 0x10 */
    UINT8     pad14[0x7C - 0x14];
} tL2C_CCB;

/*******************************************************************************
 * External references
 ******************************************************************************/

extern tL2C_CB l2cb;

extern tL2C_RCB *l2cu_find_rcb_by_psm (UINT16 psm);
extern void      l2cu_release_rcb (tL2C_RCB *p_rcb);
extern tL2C_CCB *l2cu_find_ccb_by_cid (tL2C_LCB *p_lcb, UINT16 cid);
extern void      l2c_csm_execute (tL2C_CCB *p_ccb, UINT16 event, void *p_data);

/* Events fed to the channel state machine. */
#define L2CEVT_L2CA_CONFIG_REQ      23
#define L2CEVT_L2CA_DISCONNECT_REQ  26
#define L2CEVT_L2CA_DISCONNECT_RSP  27

/* Link state value checked by L2CA_SetIdleTimeout. */
#define LST_DISCONNECTING           4

/*******************************************************************************
 * L2CAP API entry points
 ******************************************************************************/

UINT16 L2CA_Register (UINT16 psm, tL2CAP_APPL_INFO *p_cb_info)
{
    return 0;
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
    return 0;
}

BOOLEAN L2CA_ConnectRsp (BD_ADDR p_bd_addr, UINT8 id, UINT16 lcid,
                         UINT16 result, UINT16 status)
{
    return FALSE;
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
    return FALSE;
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
    return 0;
}

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
            (p_lcb->link_state == LST_DISCONNECTING))
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
    return FALSE;
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
