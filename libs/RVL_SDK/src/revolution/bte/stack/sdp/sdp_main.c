/* sdp_main.c - SDP main state machine (BTE stack)
 *
 * High-level C reconstruction of the retail TU
 * RVL_SDK/src/revolution/bte/stack/sdp/sdp_main.
 *
 * Implements the SDP service-layer callbacks registered with L2CAP
 * (connect/config/disconnect/data indications) plus the connection
 * originate and timeout paths.  The retail binary was built from the
 * RVL SDK's private sdp_int.h, which is not shipped with this
 * repository, so the internal structures below mirror the retail
 * layout exactly (offsets recovered from the retail assembly).
 *
 * The retail was built with GC/3.0a5.2 and `-ipa off`, which emits the
 * TU's functions in REVERSE source order (and the .data string pool in
 * the same reversed per-function order).  To reproduce the retail
 * forward-ordered .text / string pool, the functions below are written
 * BACK-TO-FRONT: source order is the reverse of the retail .text order
 * (MWCC_REFERENCE l2c_csm.c note).
 */

#include <harness_catalog.h>
#include <string.h>

#include "revolution/BTE/include/bt_target.h"
#include "revolution/BTE/gki/common/gki.h"
#include "revolution/BTE/stack/include/bt_types.h"
#include "revolution/BTE/stack/include/sdpdefs.h"
#include "revolution/BTE/stack/include/sdp_api.h"
#include "revolution/BTE/stack/include/l2c_api.h"
#include "revolution/BTE/stack/include/btu.h"

/* ------------------------------------------------------------------ */
/* SDP control block (retail sdp_int.h layout)                        */
/* ------------------------------------------------------------------ */

/* L2CAP registration callback table stored at the tail of sdp_cb.
 * The retail layout has 10 slots (no TxComplete), unlike the shipped
 * tL2CAP_APPL_INFO which adds one more pointer. */
typedef struct {
    tL2CA_CONNECT_IND_CB *pL2CA_ConnectInd_Cb;      /* 0x4604 */
    tL2CA_CONNECT_CFM_CB *pL2CA_ConnectCfm_Cb;      /* 0x4608 */
    tL2CA_CONNECT_PND_CB *pL2CA_ConnectPnd_Cb;      /* 0x460c */
    tL2CA_CONFIG_IND_CB *pL2CA_ConfigInd_Cb;        /* 0x4610 */
    tL2CA_CONFIG_CFM_CB *pL2CA_ConfigCfm_Cb;        /* 0x4614 */
    tL2CA_DISCONNECT_IND_CB *pL2CA_DisconnectInd_Cb;/* 0x4618 */
    tL2CA_DISCONNECT_CFM_CB *pL2CA_DisconnectCfm_Cb;/* 0x461c */
    tL2CA_QOS_VIOLATION_IND_CB *pL2CA_QoSViolationInd_Cb; /* 0x4620 */
    tL2CA_DATA_IND_CB *pL2CA_DataInd_Cb;            /* 0x4624 */
    tL2CA_CONGESTION_STATUS_CB *pL2CA_CongestionStatus_Cb; /* 0x4628 */
} tSDP_REG_INFO;

typedef struct {
    tL2CAP_CFG_INFO cfg;            /* 0x000 default local config */
    UINT8 _pad0[0x4604 - sizeof(tL2CAP_CFG_INFO)]; /* 0x048 */
    tSDP_REG_INFO reg_info;         /* 0x4604 */
    UINT16 max_attr_list_size;      /* 0x462c */
    UINT16 max_recs_per_search;     /* 0x462e */
    UINT8 trace_level;              /* 0x4630 */
    UINT8 _pad1[3];                 /* 0x4631 */
} tSDP_CB;

tSDP_CB sdp_cb;

/* ------------------------------------------------------------------ */
/* SDP connection control block (retail sdp_int.h layout)             */
/* ------------------------------------------------------------------ */

typedef struct {
    UINT8 con_state;            /* 0x000 */
    UINT8 con_flags;            /* 0x001 */
    UINT8 device_address[6];    /* 0x002 */
    TIMER_LIST_ENT timer;       /* 0x008 */
    UINT16 rem_mtu_size;        /* 0x020 */
    UINT16 connection_id;       /* 0x022 */
    UINT8 _pad1[0x414 - 0x24];  /* 0x024 */
    tSDP_DISC_CMPL_CB *p_cb;    /* 0x414 */
    UINT8 _pad2[0x472 - 0x418]; /* 0x418 */
    UINT16 disconnect_reason;   /* 0x472 */
} tCONN_CB;

/* Connection states */
#define SDP_STATE_CONN_SETUP    1
#define SDP_STATE_CFG_SETUP     2
#define SDP_STATE_CONNECTED     3

/* Connection flags */
#define SDP_FLAGS_IS_ORIG       0x01
#define SDP_FLAGS_CFG_NEGOTIATED 0x02
#define SDP_FLAGS_CFG_DONE      0x04

/* Local MTU cap used when negotiating the remote SDP MTU. */
#define SDP_MAX_MTU_SIZE        0x100

/* SDP M2 timer: type + timeout passed to btu_start_timer(). */
#define SDP_M2_TIMEOUT          30

/* ------------------------------------------------------------------ */
/* Trace helpers (retail BT trace encoding: TRACE_LAYER_SDP is        */
/* 0x000A0000; TRACE_TYPE_WARNING 0x1 / TRACE_TYPE_EVENT 0x3)         */
/* ------------------------------------------------------------------ */

#define SDP_TRACE_LEVEL_WARNING 2
#define SDP_TRACE_LEVEL_EVENT   4

/* sdp_init gates its error traces at level 1 with a bare LAYER mask. */
#define SDP_TRACE_ERROR0_INIT(m)                                        \
    {                                                                   \
        if (sdp_cb.trace_level >= 1)                                    \
            LogMsg_0(0x000A0000, (m));                                  \
    }
#define SDP_TRACE_WARNING0(m)                                       \
    {                                                               \
        if (sdp_cb.trace_level >= SDP_TRACE_LEVEL_WARNING)          \
            LogMsg_0(0x000A0001, (m));                              \
    }
#define SDP_TRACE_EVENT0(m)                                         \
    {                                                               \
        if (sdp_cb.trace_level >= SDP_TRACE_LEVEL_EVENT)            \
            LogMsg_0(0x000A0003, (m));                              \
    }
#define SDP_TRACE_WARNING1(m, p1)                                   \
    {                                                               \
        if (sdp_cb.trace_level >= SDP_TRACE_LEVEL_WARNING)          \
            LogMsg_1(0x000A0001, (m), (UINT32)(p1));                \
    }
#define SDP_TRACE_EVENT1(m, p1)                                     \
    {                                                               \
        if (sdp_cb.trace_level >= SDP_TRACE_LEVEL_EVENT)            \
            LogMsg_1(0x000A0003, (m), (UINT32)(p1));                \
    }
#define SDP_TRACE_EVENT2(m, p1, p2)                                 \
    {                                                               \
        if (sdp_cb.trace_level >= SDP_TRACE_LEVEL_EVENT)            \
            LogMsg_2(0x000A0003, (m), (UINT32)(p1), (UINT32)(p2));  \
    }
#define SDP_TRACE_WARNING2(m, p1, p2)                               \
    {                                                               \
        if (sdp_cb.trace_level >= SDP_TRACE_LEVEL_WARNING)          \
            LogMsg_2(0x000A0001, (m), (UINT32)(p1), (UINT32)(p2));  \
    }

/* ------------------------------------------------------------------ */
/* External helpers                                                   */
/* ------------------------------------------------------------------ */

extern void LogMsg_0(UINT32 trace_set_mask, const char *p_str);
extern void LogMsg_1(UINT32 trace_set_mask, const char *fmt_str, UINT32 p1);
extern void LogMsg_2(UINT32 trace_set_mask, const char *fmt_str, UINT32 p1,
                     UINT32 p2);
extern BOOLEAN BTM_SetSecurityLevel(BOOLEAN is_originator, char *p_name,
                                    UINT8 service_id, UINT16 sec_level,
                                    UINT16 psm, UINT32 mx_proto_id,
                                    UINT32 mx_chan_id);

extern tCONN_CB *sdpu_allocate_ccb(void);
extern tCONN_CB *sdpu_find_ccb_by_cid(UINT16 cid);
extern void sdpu_release_ccb(tCONN_CB *p_ccb);
extern void sdp_disc_server_rsp(tCONN_CB *p_ccb, BT_HDR *p_msg);
extern void sdp_server_handle_client_req(tCONN_CB *p_ccb, BT_HDR *p_msg);
extern void GKI_freebuf(void *p_buf);

/*******************************************************************************
**
** Function         sdp_conn_timeout
**
** Description      This function handles a timeout.  Currently, this is
**                  only used during the connection setup state.
**
*******************************************************************************/
void sdp_conn_timeout(tCONN_CB *p_ccb)
{
    SDP_TRACE_EVENT2("SDP - CCB timeout in state: %d  CID: 0x%x",
                     p_ccb->con_state, p_ccb->connection_id);

    L2CA_DisconnectReq(p_ccb->connection_id);

    if (p_ccb->p_cb)
        (*p_ccb->p_cb)(SDP_CONN_FAILED);

    sdpu_release_ccb(p_ccb);
}

/*******************************************************************************
**
** Function         sdp_disconnect_cfm
**
** Description      This function handles a disconnect confirm from the
**                  L2CAP layer.
**
*******************************************************************************/
void sdp_disconnect_cfm(UINT16 cid)
{
    tCONN_CB *p_ccb = sdpu_find_ccb_by_cid(cid);

    if (p_ccb == NULL) {
        SDP_TRACE_WARNING1("SDP - Rcvd L2CAP disc cfm, unknown CID: 0x%x",
                           cid);
    } else {
        SDP_TRACE_EVENT1("SDP - Rcvd L2CAP disc cfm, CID: 0x%x", cid);

        if (p_ccb->p_cb)
            (*p_ccb->p_cb)(p_ccb->disconnect_reason);

        sdpu_release_ccb(p_ccb);
    }
}

/*******************************************************************************
**
** Function         sdp_disconnect
**
** Description      This function handles a disconnect indication from the
**                  L2CAP layer.  If the disconnect is for an active
**                  connection, then the callback is invoked.  If the
**                  disconnect is for a connection that is still in the
**                  process of being established, then the connection
**                  attempt is aborted.
**
*******************************************************************************/
void sdp_disconnect(tCONN_CB *p_ccb, UINT16 reason)
{
    SDP_TRACE_EVENT1("SDP - disconnect  CID: 0x%x", p_ccb->connection_id);

    /* Check ccb state */
    if (p_ccb->con_state != SDP_STATE_CONN_SETUP) {
        /* Send disconnect request to L2CAP */
        L2CA_DisconnectReq(p_ccb->connection_id);

        /* Save the reason */
        p_ccb->disconnect_reason = reason;
    } else {
        /* Connection attempt is being aborted */
        if (p_ccb->p_cb)
            (*p_ccb->p_cb)(reason);

        sdpu_release_ccb(p_ccb);
    }
}

/*******************************************************************************
**
** Function         sdp_conn_originate
**
** Description      This function is called by the SDP client to originate a
**                  connection to a peer device.  A CCB is allocated, the
**                  connection request is sent to L2CAP, and the CCB is
**                  returned to the caller once the request is accepted.
**
*******************************************************************************/
tCONN_CB *sdp_conn_originate(BD_ADDR bd_addr)
{
    tCONN_CB *p_ccb;
    UINT16 cid;

    /* Allocate a new CCB. Return if no memory. */
    if ((p_ccb = sdpu_allocate_ccb()) == NULL) {
        SDP_TRACE_WARNING0("SDP - no spare CCB for orig");
        return NULL;
    }

    SDP_TRACE_EVENT0("SDP - Originate started");

    /* We are the originator of this connection */
    p_ccb->con_flags |= SDP_FLAGS_IS_ORIG;
    memcpy(p_ccb->device_address, bd_addr, BD_ADDR_LEN);
    p_ccb->con_state = SDP_STATE_CONN_SETUP;

    /* Send the connect request to L2CAP */
    if ((cid = L2CA_ConnectReq(SDP_PSM, bd_addr)) != 0) {
        p_ccb->connection_id = cid;
        return p_ccb;
    }

    SDP_TRACE_WARNING0("SDP - Originate failed");

    sdpu_release_ccb(p_ccb);
    return NULL;
}

/*******************************************************************************
**
** Function         sdp_data_ind
**
** Description      This function handles a data indication from the L2CAP
**                  layer. This could be a server side request or a client
**                  side response.  If it's a client response, it could be
**                  a response to a search or to a service attribute request.
**
*******************************************************************************/
void sdp_data_ind(UINT16 cid, BT_HDR *p_msg)
{
    tCONN_CB *p_ccb;

    /* Find the CCB */
    if ((p_ccb = sdpu_find_ccb_by_cid(cid)) != NULL) {
        if (p_ccb->con_state == SDP_STATE_CONNECTED) {
            if (p_ccb->con_flags & SDP_FLAGS_IS_ORIG)
                sdp_disc_server_rsp(p_ccb, p_msg);
            else
                sdp_server_handle_client_req(p_ccb, p_msg);
        } else {
            SDP_TRACE_WARNING2("SDP - Ignored L2CAP data while in state: %d, CID: 0x%x",
                               p_ccb->con_state, cid);
        }
    } else {
        SDP_TRACE_WARNING1("SDP - Rcvd L2CAP data, unknown CID: 0x%x", cid);
    }

    GKI_freebuf(p_msg);
}

/*******************************************************************************
**
** Function         sdp_disconnect_ind
**
** Description      This function handles a disconnect indication from the
**                  L2CAP layer.
**
*******************************************************************************/
void sdp_disconnect_ind(UINT16 cid, BOOLEAN is_connected)
{
    tCONN_CB *p_ccb;

    /* Find the CCB */
    if ((p_ccb = sdpu_find_ccb_by_cid(cid)) == NULL) {
        SDP_TRACE_WARNING1("SDP - Rcvd L2CAP disc, unknown CID: 0x%x", cid);
        return;
    }

    if (is_connected)
        L2CA_DisconnectRsp(cid);

    SDP_TRACE_EVENT1("SDP - Rcvd L2CAP disc, CID: 0x%x", cid);

    if (p_ccb->p_cb)
        (*p_ccb->p_cb)(p_ccb->con_state == SDP_STATE_CONNECTED ? SDP_SUCCESS
                                                                : SDP_CONN_FAILED);

    sdpu_release_ccb(p_ccb);
}

/*******************************************************************************
**
** Function         sdp_config_cfm
**
** Description      This function handles a config confirm from the L2CAP
**                  layer.  If the configuration failed, the connection is
**                  dropped (or the pending connection attempt aborted).
**                  Otherwise the connection is promoted to CONNECTED once
**                  both sides have finished negotiating.
**
*******************************************************************************/
void sdp_config_cfm(UINT16 l2cap_cid, tL2CAP_CFG_INFO *p_cfg)
{
    tCONN_CB *p_ccb;

    SDP_TRACE_EVENT2("SDP - Rcvd cfg cfm, CID: 0x%x  Result: %d", l2cap_cid,
                     p_cfg->result);

    /* Find the CCB */
    if ((p_ccb = sdpu_find_ccb_by_cid(l2cap_cid)) == NULL) {
        SDP_TRACE_WARNING1("SDP - Rcvd L2CAP cfg ind, unknown CID: 0x%x",
                           l2cap_cid);
        return;
    }

    /* If the configuration succeeded and both sides are done, connect */
    if (p_cfg->result == 0) {
        if ((p_ccb->con_flags |= SDP_FLAGS_CFG_DONE) & SDP_FLAGS_CFG_NEGOTIATED) {
            p_ccb->con_state = SDP_STATE_CONNECTED;

            if (p_ccb->con_flags & SDP_FLAGS_IS_ORIG)
                sdp_disc_connected(p_ccb);
            else
                btu_start_timer(&p_ccb->timer, BTU_TTYPE_SDP, SDP_M2_TIMEOUT);
        }
    } else {
        /* Configuration failed - drop the connection */
        SDP_TRACE_EVENT1("SDP - disconnect  CID: 0x%x", p_ccb->connection_id);

        if (p_ccb->con_state != SDP_STATE_CONN_SETUP) {
            L2CA_DisconnectReq(p_ccb->connection_id);
            p_ccb->disconnect_reason = SDP_CFG_FAILED;
        } else {
            if (p_ccb->p_cb)
                (*p_ccb->p_cb)(SDP_CFG_FAILED);

            sdpu_release_ccb(p_ccb);
        }
    }
}

/*******************************************************************************
**
** Function         sdp_config_ind
**
** Description      This function handles a config indication from the
**                  L2CAP layer.  The remote MTU is validated against the
**                  SDP limit, the peer configuration is acknowledged, and
**                  the connection is promoted to CONNECTED once both sides
**                  have finished negotiating.
**
*******************************************************************************/
void sdp_config_ind(UINT16 l2cap_cid, tL2CAP_CFG_INFO *p_cfg)
{
    tCONN_CB *p_ccb;

    /* Find the CCB */
    if ((p_ccb = sdpu_find_ccb_by_cid(l2cap_cid)) == NULL) {
        SDP_TRACE_WARNING1("SDP - Rcvd L2CAP cfg ind, unknown CID: 0x%x",
                           l2cap_cid);
        return;
    }

    /* If the MTU is not present, or is too large, cap it */
    if (!p_cfg->mtu_present)
        p_ccb->rem_mtu_size = SDP_MAX_MTU_SIZE;
    else if (p_cfg->mtu > SDP_MAX_MTU_SIZE)
        p_ccb->rem_mtu_size = SDP_MAX_MTU_SIZE;
    else
        p_ccb->rem_mtu_size = p_cfg->mtu;

    /* Send a config response with the parameters cleared */
    p_cfg->flush_to_present = FALSE;
    p_cfg->mtu_present = FALSE;
    p_cfg->result = 0;

    L2CA_ConfigRsp(l2cap_cid, p_cfg);

    SDP_TRACE_EVENT1("SDP - Rcvd cfg ind, sent cfg cfm, CID: 0x%x", l2cap_cid);

    /* If both sides have finished config, the connection is established */
    if ((p_ccb->con_flags |= SDP_FLAGS_CFG_NEGOTIATED) & SDP_FLAGS_CFG_DONE) {
        p_ccb->con_state = SDP_STATE_CONNECTED;

        if (p_ccb->con_flags & SDP_FLAGS_IS_ORIG)
            sdp_disc_connected(p_ccb);
        else
            btu_start_timer(&p_ccb->timer, BTU_TTYPE_SDP, SDP_M2_TIMEOUT);
    }
}

/*******************************************************************************
**
** Function         sdp_connect_cfm
**
** Description      This function handles a connect confirm from the L2CAP
**                  layer.  If the connection succeeded and the ccb is still
**                  in the setup state, a configuration request is sent.
**                  Otherwise the failed connection attempt is reported to
**                  the discovery callback and the ccb is released.
**
*******************************************************************************/
void sdp_connect_cfm(UINT16 l2cap_cid, UINT16 result)
{
    tCONN_CB *p_ccb;

    /* Find the CCB */
    if ((p_ccb = sdpu_find_ccb_by_cid(l2cap_cid)) == NULL) {
        SDP_TRACE_WARNING1("SDP - Rcvd conn cnf for unknown CID 0x%x",
                           l2cap_cid);
        return;
    }

    /* If the connection succeeded, send a configuration request */
    if ((result == 0) && (p_ccb->con_state == SDP_STATE_CONN_SETUP)) {
        p_ccb->con_state = SDP_STATE_CFG_SETUP;

        L2CA_ConfigReq(l2cap_cid, &sdp_cb.cfg);

        SDP_TRACE_EVENT1("SDP - got conn cnf, sent cfg req, CID: 0x%x",
                         p_ccb->connection_id);
    } else {
        SDP_TRACE_WARNING2("SDP - Rcvd conn cnf with error: 0x%x  CID 0x%x",
                           result, p_ccb->connection_id);

        if (p_ccb->p_cb) {
            if ((result == 0x0E) || (result == 0x05) || (result == 0x18) ||
                (result == 0x29) || (result == 0x06))
                (*p_ccb->p_cb)(SDP_SECURITY_ERR);
            else if (result == 0x0F)
                (*p_ccb->p_cb)(SDP_CONN_REJECTED);
            else
                (*p_ccb->p_cb)(SDP_CONN_FAILED);
        }

        sdpu_release_ccb(p_ccb);
    }
}

/*******************************************************************************
**
** Function         sdp_connect_ind
**
** Description      This function handles an L2CAP connect indication event.
**
*******************************************************************************/
void sdp_connect_ind(BD_ADDR bd_addr, UINT16 l2cap_cid, UINT16 psm,
                     UINT8 l2cap_status)
{
    tCONN_CB *p_ccb;

    /* Allocate a new CCB. Return if no memory. */
    if ((p_ccb = sdpu_allocate_ccb()) == NULL)
        return;

    /* We accept the connection. */
    p_ccb->con_state = SDP_STATE_CFG_SETUP;
    memcpy(p_ccb->device_address, bd_addr, BD_ADDR_LEN);
    p_ccb->connection_id = l2cap_cid;

    /* Send response to the L2CAP layer. */
    L2CA_ConnectRsp(bd_addr, l2cap_status, l2cap_cid, 0, 0);

    /* Send a configuration request. */
    L2CA_ConfigReq(l2cap_cid, &sdp_cb.cfg);

    SDP_TRACE_EVENT1("SDP - Rcvd L2CAP conn ind, sent config req, CID 0x%x",
                     p_ccb->connection_id);
}

/*******************************************************************************
**
** Function         sdp_init
**
** Description      This function initializes the SDP service: it zeroes the
**                  control block, registers the service with the security
**                  manager (server and client) and registers the SDP
**                  callbacks with L2CAP.
**
*******************************************************************************/
void sdp_init(void)
{
    /* Zero out the control block and set the default settings */
    memset(&sdp_cb, 0, sizeof(sdp_cb));

    sdp_cb.cfg.mtu_present = TRUE;
    sdp_cb.cfg.mtu = SDP_MAX_MTU_SIZE;
    sdp_cb.cfg.flush_to_present = TRUE;
    sdp_cb.cfg.flush_to = 0xFFFF;
    sdp_cb.max_attr_list_size = 0xF0;
    sdp_cb.max_recs_per_search = 0x15;
    sdp_cb.trace_level = 0;

    /* Register the server side with the security manager */
    if (BTM_SetSecurityLevel(FALSE, "Service Discovery", 0, 0, SDP_PSM, 0, 0)
        == FALSE) {
        SDP_TRACE_ERROR0_INIT("Security Registration Server failed");
        return;
    }

    /* Register the client side with the security manager */
    if (BTM_SetSecurityLevel(TRUE, "Service Discovery", 0, 0, SDP_PSM, 0, 0)
        == FALSE) {
        SDP_TRACE_ERROR0_INIT("Security Registration for Client failed");
        return;
    }

    /* Register with L2CAP */
    sdp_cb.reg_info.pL2CA_ConnectInd_Cb = sdp_connect_ind;
    sdp_cb.reg_info.pL2CA_ConnectCfm_Cb = sdp_connect_cfm;
    sdp_cb.reg_info.pL2CA_ConnectPnd_Cb = NULL;
    sdp_cb.reg_info.pL2CA_ConfigInd_Cb = sdp_config_ind;
    sdp_cb.reg_info.pL2CA_ConfigCfm_Cb = sdp_config_cfm;
    sdp_cb.reg_info.pL2CA_DisconnectInd_Cb = sdp_disconnect_ind;
    sdp_cb.reg_info.pL2CA_DisconnectCfm_Cb =
        (tL2CA_DISCONNECT_CFM_CB *)sdp_disconnect_cfm;
    sdp_cb.reg_info.pL2CA_QoSViolationInd_Cb = NULL;
    sdp_cb.reg_info.pL2CA_DataInd_Cb = sdp_data_ind;
    sdp_cb.reg_info.pL2CA_CongestionStatus_Cb = NULL;

    if (L2CA_Register(SDP_PSM, (tL2CAP_APPL_INFO *)&sdp_cb.reg_info) == FALSE)
        SDP_TRACE_ERROR0_INIT("SDP Registration failed");
}
