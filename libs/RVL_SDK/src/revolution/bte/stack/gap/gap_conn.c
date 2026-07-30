// High-level C reconstruction of RVL_SDK BTE stack/gap/gap_conn.c
//
// GAP connection management: L2CAP callback handlers for the Generic
// Access Profile connection subsystem (Broadcom BTE stack, Nintendo
// RVL SDK variant).
//
// Retail layout notes (verified against build/us/asm/.../gap_conn.s):
//   * tGAP_CONN_CB stride is 0x60; ccb[8] starts at gap_cb+0xAC.
//   * gap_cb is 0x3AC bytes total and lives in the gap_utils.c BSS
//     split, so it is extern here.
//   * The retail tL2CAP_CFG_INFO embedded in each connection block is
//     only 0x3C bytes (smaller than the reconstructed header type),
//     so the block keeps it as a sized byte region and call sites cast.

#include <harness_catalog.h>
#include <string.h>

#include "revolution/BTE/include/bt_target.h"
#include "revolution/BTE/stack/include/bt_types.h"
#include "revolution/BTE/include/bt_trace.h"
#include "revolution/BTE/stack/include/l2c_api.h"
#include "revolution/BTE/gki/common/gki.h"

#define GAP_MAX_CONNECTIONS     8

/* Connection states */
#define GAP_CONN_STATE_IDLE         0
#define GAP_CONN_STATE_LISTENING    1
#define GAP_CONN_STATE_CONNECTING   2
#define GAP_CONN_STATE_CONFIGURING  3
#define GAP_CONN_STATE_CONNECTED    4

/* Configuration flags */
#define GAP_CFG_FLAGS_CFG_DONE_IND  0x02
#define GAP_CFG_FLAGS_CFG_DONE_CFM  0x04

/* Connection events delivered to the upper layer callback */
#define GAP_EVT_CONN_OPENED         0x0100
#define GAP_EVT_CONN_CLOSED         0x0101
#define GAP_EVT_CONN_DATA_AVAIL     0x0102
#define GAP_EVT_CONN_CONGESTED      0x0103
#define GAP_EVT_CONN_UNCONGESTED    0x0104

/* Largest MTU we are willing to accept from the peer */
#define GAP_DATA_MAX_MTU_SIZE       0x69B

typedef void (tGAP_CONN_CBACK)(UINT16 gap_handle, UINT16 event);

/* Connection control block. 0x60 bytes per entry. */
typedef struct
{
    UINT8   state;                  /* 0x00: connection state           */
    UINT8   cfg_flags;              /* 0x01: configuration flags        */
    UINT8   reserved1[2];           /* 0x02                             */
    UINT16  handle;                 /* 0x04: GAP handle                 */
    UINT16  cid;                    /* 0x06: L2CAP channel ID           */
    UINT8   bd_addr_any;            /* 0x08: zero = accept any address  */
    BD_ADDR bd_addr;                /* 0x09: remote BD address          */
    UINT8   reserved2;              /* 0x0F                             */
    UINT16  psm;                    /* 0x10: PSM for the connection     */
    UINT16  rem_mtu_size;           /* 0x12: MTU of the remote device   */
    BUFFER_Q tx_queue;              /* 0x14: transmit queue             */
    tGAP_CONN_CBACK *p_callback;    /* 0x20: upper layer callback       */
    UINT8   cfg[0x3C];              /* 0x24: tL2CAP_CFG_INFO (retail)   */
} tGAP_CONN_CB;

/* L2CAP registration info kept by the connection subsystem: the ten
   callback slots at gap_cb+0x84 (retail has no TxComplete slot). */
typedef struct
{
    tL2CA_CONNECT_IND_CB        *pL2CA_ConnectInd_Cback;
    tL2CA_CONNECT_CFM_CB        *pL2CA_ConnectCfm_Cback;
    tL2CA_CONNECT_PND_CB        *pL2CA_ConnectPnd_Cback;
    tL2CA_CONFIG_IND_CB         *pL2CA_ConfigInd_Cback;
    tL2CA_CONFIG_CFM_CB         *pL2CA_ConfigCfm_Cback;
    tL2CA_DISCONNECT_IND_CB     *pL2CA_DisconnectInd_Cback;
    tL2CA_DISCONNECT_CFM_CB     *pL2CA_DisconnectCfm_Cback;
    tL2CA_QOS_VIOLATION_IND_CB  *pL2CA_QoSViolationInd_Cback;
    tL2CA_DATA_IND_CB           *pL2CA_DataInd_Cback;
    tL2CA_CONGESTION_STATUS_CB  *pL2CA_CongestionStatus_Cback;
} tGAP_CONN_REG_INFO;

typedef struct
{
    tGAP_CONN_REG_INFO reg_info;                    /* gap_cb+0x84 */
    tGAP_CONN_CB ccb[GAP_MAX_CONNECTIONS];          /* gap_cb+0xAC */
} tGAP_CONN;

/* Slice of the GAP control block used by this module. The full block
   is 0x3AC bytes; trace_level sits at 0x28 (see bt_trace.h macros). */
typedef struct
{
    UINT8   reserved1[0x28];
    UINT8   trace_level;                /* 0x28 */
    UINT8   reserved2[0x84 - 0x29];
    tGAP_CONN conn;                     /* 0x84 */
} tGAP_CB;                              /* 0x3AC total */

extern tGAP_CB gap_cb;

/* Banner string used to frame warning messages. Exactly 8 bytes so
   MWCC places it in .sdata (retail references it via r13). */
static char gap_conn_banner[] = "*******";

void gap_connect_ind(BD_ADDR bd_addr, UINT16 lcid, UINT16 psm, UINT8 l2cap_id);
void gap_connect_cfm(UINT16 lcid, UINT16 result);
void gap_config_ind(UINT16 cid, tL2CAP_CFG_INFO *p_cfg);
void gap_config_cfm(UINT16 cid, tL2CAP_CFG_INFO *p_cfg);
void gap_disconnect_ind(UINT16 lcid, BOOLEAN ack_needed);
void gap_data_ind(UINT16 lcid, BT_HDR *p_buf);
void gap_congestion_ind(UINT16 lcid, BOOLEAN is_congested);

/*******************************************************************************
**
** Function         gap_conn_init
**
** Description      Install the GAP connection handlers into the L2CAP
**                  registration block.
**
*******************************************************************************/
void gap_conn_init(void)
{
    gap_cb.conn.reg_info.pL2CA_ConnectInd_Cback       = gap_connect_ind;
    gap_cb.conn.reg_info.pL2CA_ConnectCfm_Cback       = gap_connect_cfm;
    gap_cb.conn.reg_info.pL2CA_ConnectPnd_Cback       = NULL;
    gap_cb.conn.reg_info.pL2CA_ConfigInd_Cback        = gap_config_ind;
    gap_cb.conn.reg_info.pL2CA_ConfigCfm_Cback        = gap_config_cfm;
    gap_cb.conn.reg_info.pL2CA_DisconnectInd_Cback    = gap_disconnect_ind;
    gap_cb.conn.reg_info.pL2CA_DisconnectCfm_Cback    = NULL;
    gap_cb.conn.reg_info.pL2CA_QoSViolationInd_Cback  = NULL;
    gap_cb.conn.reg_info.pL2CA_DataInd_Cback          = gap_data_ind;
    gap_cb.conn.reg_info.pL2CA_CongestionStatus_Cback = gap_congestion_ind;
}

/*******************************************************************************
**
** Function         gap_connect_ind
**
** Description      Handles an inbound connection indication from L2CAP.
**                  This is the case where we are acting as a server.
**
*******************************************************************************/
void gap_connect_ind(BD_ADDR bd_addr, UINT16 lcid, UINT16 psm, UINT8 l2cap_id)
{
    UINT16 xx;
    tGAP_CONN_CB *p_ccb = &gap_cb.conn.ccb[0];

    /* See if we have a connection block listening for this PSM */
    for (xx = 0; xx < GAP_MAX_CONNECTIONS; xx++, p_ccb++)
    {
        if ((p_ccb->state == GAP_CONN_STATE_LISTENING)
         && (p_ccb->psm == psm)
         && ((p_ccb->bd_addr_any == 0)
          || (memcmp(bd_addr, p_ccb->bd_addr, BD_ADDR_LEN) == 0)))
        {
            break;
        }
    }

    if (xx == GAP_MAX_CONNECTIONS)
    {
        GAP_TRACE_WARNING0(gap_conn_banner);
        GAP_TRACE_WARNING0("WARNING: GAP Conn Indication for Unexpected Bd Addr...Disconnecting");
        GAP_TRACE_WARNING0(gap_conn_banner);

        L2CA_DisconnectReq(lcid);
        return;
    }

    /* Found a listening block: accept the connection */
    p_ccb->state = GAP_CONN_STATE_CONFIGURING;
    memcpy(p_ccb->bd_addr, bd_addr, BD_ADDR_LEN);
    p_ccb->cid = lcid;

    L2CA_ConnectRsp(bd_addr, l2cap_id, lcid, 0, 0);

    GAP_TRACE_EVENT1("GAP_CONN - Rcvd L2CAP conn ind, CID: 0x%x", p_ccb->cid);

    L2CA_ConfigReq(lcid, (tL2CAP_CFG_INFO *)p_ccb->cfg);
}

/*******************************************************************************
**
** Function         gap_connect_cfm
**
** Description      Handles a connection confirm from L2CAP. This is the
**                  case where we are acting as a client.
**
*******************************************************************************/
void gap_connect_cfm(UINT16 lcid, UINT16 result)
{
    UINT8 xx;
    UINT16 psm;
    tGAP_CONN_CB *p_ccb = &gap_cb.conn.ccb[0];
    tGAP_CONN_CB *p_scan;
    BT_HDR *p_buf;

    /* Find the connection block for this CID */
#pragma unroll 4
    for (xx = 0; xx < GAP_MAX_CONNECTIONS; xx++, p_ccb++)
    {
        if ((p_ccb->state != GAP_CONN_STATE_IDLE)
         && (p_ccb->cid == lcid))
        {
            break;
        }
    }

    if (xx == GAP_MAX_CONNECTIONS)
    {
        return;
    }

    if ((result == 0) && (p_ccb->state == GAP_CONN_STATE_CONNECTING))
    {
        /* Connection succeeded: start configuration */
        p_ccb->state = GAP_CONN_STATE_CONFIGURING;
        L2CA_ConfigReq(lcid, (tL2CAP_CFG_INFO *)p_ccb->cfg);
        return;
    }

    /* Connection failed */
    if (p_ccb->p_callback != NULL)
    {
        (*p_ccb->p_callback)(p_ccb->handle, GAP_EVT_CONN_CLOSED);
    }

    psm = p_ccb->psm;

    while ((p_buf = (BT_HDR *)GKI_dequeue(&p_ccb->tx_queue)) != NULL)
    {
        GKI_freebuf(p_buf);
    }

    p_ccb->state = GAP_CONN_STATE_IDLE;

    /* If no other connection is registered for the PSM, deregister it */
#pragma unroll 4
    for (xx = 0, p_scan = &gap_cb.conn.ccb[0]; xx < GAP_MAX_CONNECTIONS; xx++, p_scan++)
    {
        if ((p_scan->state != GAP_CONN_STATE_IDLE)
         && (p_scan->psm == psm))
        {
            return;
        }
    }

    L2CA_Deregister(psm);
}

/*******************************************************************************
**
** Function         gap_config_ind
**
** Description      Handles a configuration indication from L2CAP.
**
*******************************************************************************/
void gap_config_ind(UINT16 cid, tL2CAP_CFG_INFO *p_cfg)
{
    UINT8 xx;
    tGAP_CONN_CB *p_ccb = &gap_cb.conn.ccb[0];

#pragma unroll 4
    for (xx = 0; xx < GAP_MAX_CONNECTIONS; xx++, p_ccb++)
    {
        if ((p_ccb->state != GAP_CONN_STATE_IDLE)
         && (p_ccb->cid == cid))
        {
            break;
        }
    }

    if (xx == GAP_MAX_CONNECTIONS)
    {
        return;
    }

    if (p_cfg->mtu_present && (p_cfg->mtu <= GAP_DATA_MAX_MTU_SIZE))
    {
        p_ccb->rem_mtu_size = p_cfg->mtu;
    }
    else
    {
        p_ccb->rem_mtu_size = GAP_DATA_MAX_MTU_SIZE;
    }

    /* We do not request any other configuration options */
    p_cfg->flush_to_present = FALSE;
    p_cfg->mtu_present      = FALSE;
    p_cfg->result           = 0;

    L2CA_ConfigRsp(cid, p_cfg);

    p_ccb->cfg_flags |= GAP_CFG_FLAGS_CFG_DONE_IND;

    if (p_ccb->cfg_flags & GAP_CFG_FLAGS_CFG_DONE_CFM)
    {
        p_ccb->state = GAP_CONN_STATE_CONNECTED;
        (*p_ccb->p_callback)(p_ccb->handle, GAP_EVT_CONN_OPENED);
    }
}

/*******************************************************************************
**
** Function         gap_config_cfm
**
** Description      Handles a configuration confirm from L2CAP.
**
*******************************************************************************/
void gap_config_cfm(UINT16 cid, tL2CAP_CFG_INFO *p_cfg)
{
    UINT8 xx;
    UINT16 psm;
    tGAP_CONN_CB *p_ccb = &gap_cb.conn.ccb[0];
    tGAP_CONN_CB *p_scan;
    BT_HDR *p_buf;

#pragma unroll 4
    for (xx = 0; xx < GAP_MAX_CONNECTIONS; xx++, p_ccb++)
    {
        if ((p_ccb->state != GAP_CONN_STATE_IDLE)
         && (p_ccb->cid == cid))
        {
            break;
        }
    }

    if (xx == GAP_MAX_CONNECTIONS)
    {
        return;
    }

    if (p_cfg->result == 0)
    {
        p_ccb->cfg_flags |= GAP_CFG_FLAGS_CFG_DONE_CFM;

        if (p_ccb->cfg_flags & GAP_CFG_FLAGS_CFG_DONE_IND)
        {
            p_ccb->state = GAP_CONN_STATE_CONNECTED;
            (*p_ccb->p_callback)(p_ccb->handle, GAP_EVT_CONN_OPENED);
        }
        return;
    }

    /* Configuration failed: tear the connection down */
    (*p_ccb->p_callback)(p_ccb->handle, GAP_EVT_CONN_CLOSED);

    psm = p_ccb->psm;

    while ((p_buf = (BT_HDR *)GKI_dequeue(&p_ccb->tx_queue)) != NULL)
    {
        GKI_freebuf(p_buf);
    }

    p_ccb->state = GAP_CONN_STATE_IDLE;

#pragma unroll 4
    for (xx = 0, p_scan = &gap_cb.conn.ccb[0]; xx < GAP_MAX_CONNECTIONS; xx++, p_scan++)
    {
        if ((p_scan->state != GAP_CONN_STATE_IDLE)
         && (p_scan->psm == psm))
        {
            return;
        }
    }

    L2CA_Deregister(psm);
}

/*******************************************************************************
**
** Function         gap_disconnect_ind
**
** Description      Handles a disconnect indication from L2CAP.
**
*******************************************************************************/
void gap_disconnect_ind(UINT16 lcid, BOOLEAN ack_needed)
{
    UINT8 xx;
    UINT16 psm;
    tGAP_CONN_CB *p_ccb = &gap_cb.conn.ccb[0];
    tGAP_CONN_CB *p_scan;
    BT_HDR *p_buf;

    GAP_TRACE_EVENT1("GAP_CONN - Rcvd L2CAP disc, CID: 0x%x", lcid);

#pragma unroll 4
    for (xx = 0; xx < GAP_MAX_CONNECTIONS; xx++, p_ccb++)
    {
        if ((p_ccb->state != GAP_CONN_STATE_IDLE)
         && (p_ccb->cid == lcid))
        {
            break;
        }
    }

    if (xx == GAP_MAX_CONNECTIONS)
    {
        return;
    }

    if (ack_needed)
    {
        L2CA_DisconnectRsp(lcid);
    }

    (*p_ccb->p_callback)(p_ccb->handle, GAP_EVT_CONN_CLOSED);

    psm = p_ccb->psm;

    while ((p_buf = (BT_HDR *)GKI_dequeue(&p_ccb->tx_queue)) != NULL)
    {
        GKI_freebuf(p_buf);
    }

    p_ccb->state = GAP_CONN_STATE_IDLE;

#pragma unroll 4
    for (xx = 0, p_scan = &gap_cb.conn.ccb[0]; xx < GAP_MAX_CONNECTIONS; xx++, p_scan++)
    {
        if ((p_scan->state != GAP_CONN_STATE_IDLE)
         && (p_scan->psm == psm))
        {
            return;
        }
    }

    L2CA_Deregister(psm);
}

/*******************************************************************************
**
** Function         gap_data_ind
**
** Description      Handles incoming data from L2CAP.
**
*******************************************************************************/
void gap_data_ind(UINT16 lcid, BT_HDR *p_buf)
{
    UINT8 xx;
    tGAP_CONN_CB *p_ccb = &gap_cb.conn.ccb[0];

#pragma unroll 4
    for (xx = 0; xx < GAP_MAX_CONNECTIONS; xx++, p_ccb++)
    {
        if ((p_ccb->state != GAP_CONN_STATE_IDLE)
         && (p_ccb->cid == lcid))
        {
            break;
        }
    }

    if (xx == GAP_MAX_CONNECTIONS)
    {
        GKI_freebuf(p_buf);
        return;
    }

    if (p_ccb->state == GAP_CONN_STATE_CONNECTED)
    {
        GKI_enqueue(&p_ccb->tx_queue, p_buf);
        (*p_ccb->p_callback)(p_ccb->handle, GAP_EVT_CONN_DATA_AVAIL);
    }
    else
    {
        GKI_freebuf(p_buf);
    }
}

/*******************************************************************************
**
** Function         gap_congestion_ind
**
** Description      Handles a congestion status change from L2CAP.
**
*******************************************************************************/
void gap_congestion_ind(UINT16 lcid, BOOLEAN is_congested)
{
    UINT8 xx;
    tGAP_CONN_CB *p_ccb = &gap_cb.conn.ccb[0];

    GAP_TRACE_EVENT2("GAP_CONN - Rcvd L2CAP Is Congested (%d), CID: 0x%x", is_congested, lcid);

#pragma unroll 4
    for (xx = 0; xx < GAP_MAX_CONNECTIONS; xx++, p_ccb++)
    {
        if ((p_ccb->state != GAP_CONN_STATE_IDLE)
         && (p_ccb->cid == lcid))
        {
            break;
        }
    }

    if (xx == GAP_MAX_CONNECTIONS)
    {
        return;
    }

    (*p_ccb->p_callback)(p_ccb->handle,
                         (is_congested) ? GAP_EVT_CONN_CONGESTED : GAP_EVT_CONN_UNCONGESTED);
}
