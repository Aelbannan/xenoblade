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
 */

#include <harness_catalog.h>

#include "revolution/BTE/stack/include/bt_types.h"
#include "revolution/BTE/stack/include/sdp_api.h"

/* ------------------------------------------------------------------ */
/* SDP control block (retail sdp_int.h layout)                        */
/* ------------------------------------------------------------------ */

typedef struct {
    UINT8 _pad[0x4630];
    UINT8 trace_level;          /* 0x4630 */
    UINT8 _pad2[3];
} tSDP_CB;

tSDP_CB sdp_cb;

/* ------------------------------------------------------------------ */
/* SDP connection control block (retail sdp_int.h layout)             */
/* ------------------------------------------------------------------ */

typedef struct {
    UINT8 con_state;            /* 0x000 */
    UINT8 con_flags;            /* 0x001 */
    UINT8 device_address[6];    /* 0x002 */
    UINT8 _pad0[0x18];          /* 0x008 TIMER_LIST_ENT slot */
    UINT16 rem_mtu_size;        /* 0x020 */
    UINT16 connection_id;       /* 0x022 */
    UINT8 _pad1[0x414 - 0x24];  /* 0x024 */
    tSDP_DISC_CMPL_CB *p_cb;    /* 0x414 */
    UINT8 _pad2[0x472 - 0x418]; /* 0x418 */
    UINT16 disconnect_reason;   /* 0x472 */
} tCONN_CB;

/* Connection states */
#define SDP_STATE_CONN_SETUP    1

/* ------------------------------------------------------------------ */
/* Trace helpers (retail BT trace encoding: TRACE_LAYER_SDP is        */
/* 0x000A0000; TRACE_TYPE_WARNING 0x1 / TRACE_TYPE_EVENT 0x3)         */
/* ------------------------------------------------------------------ */

#define SDP_TRACE_LEVEL_WARNING 2
#define SDP_TRACE_LEVEL_EVENT   4

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

/* ------------------------------------------------------------------ */
/* External helpers                                                   */
/* ------------------------------------------------------------------ */

extern void LogMsg_1(UINT32 trace_set_mask, const char *fmt_str, UINT32 p1);
extern void LogMsg_2(UINT32 trace_set_mask, const char *fmt_str, UINT32 p1,
                     UINT32 p2);
extern BOOLEAN L2CA_DisconnectReq(UINT16 cid);
extern tCONN_CB *sdpu_find_ccb_by_cid(UINT16 cid);
extern void sdpu_release_ccb(tCONN_CB *p_ccb);

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

/* ------------------------------------------------------------------ */
/* Remaining symbols are placeholders that will be matched in         */
/* follow-on sessions.  They are kept here as stubs so the unit       */
/* builds and the split .text budget is respected.                    */
/* ------------------------------------------------------------------ */

void sdp_init() {}

void sdp_connect_ind() {}

void sdp_connect_cfm() {}

void sdp_config_ind() {}

void sdp_config_cfm() {}

void sdp_disconnect_ind() {}

void sdp_data_ind() {}

void sdp_conn_originate() {}
