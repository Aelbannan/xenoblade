// Recovered RFCOMM multiplexer state-machine handlers.

#include <harness_catalog.h>
#include <string.h>

#include "revolution/BTE/stack/rfcomm/rfc_int.h"
#include "revolution/BTE/include/bt_trace.h"

/* L2CAP configuration block as laid out in the retail BTE (0x3C bytes).
 * DEBT: l2c_api.h still carries the newer 0x48-byte tL2CAP_CFG_INFO
 * (ext_flow_spec), so the request built on the stack here keeps the
 * retail-sized view until l2c_api.h is restored to the retail layout. */
typedef struct {
    UINT16 result;
    BOOLEAN mtu_present;
    UINT16 mtu;
    BOOLEAN qos_present;
    UINT8 pad_0x07[0x19];
    BOOLEAN flush_to_present;
    UINT8 pad_0x21[0x1b];
} RfcConfig;

static void rfc_mx_send_config_req(tRFC_MCB* p_mcb);

void rfc_mx_conf_cnf(tRFC_MCB* channel, tL2CAP_CFG_INFO* config);
void rfc_mx_conf_ind(tRFC_MCB* channel, tL2CAP_CFG_INFO* config);
void rfc_mx_sm_execute(tRFC_MCB* channel, u16 event, void* data);
void rfc_mx_sm_state_idle(tRFC_MCB* channel, u16 event, void* data);
void rfc_mx_sm_state_wait_conn_cnf(tRFC_MCB* channel, u16 event, void* data);
void rfc_mx_sm_state_configure(tRFC_MCB* channel, u16 event, void* data);
void rfc_mx_sm_sabme_wait_ua(tRFC_MCB* channel, u16 event, void* data);
void rfc_mx_sm_state_wait_sabme(tRFC_MCB* channel, u16 event, void* data);
void rfc_mx_sm_state_connected(tRFC_MCB* channel, u16 event, void* data);
void rfc_mx_sm_state_disc_wait_ua(tRFC_MCB* channel, u16 event, void* data);

void rfc_mx_sm_execute(tRFC_MCB* channel, u16 event, void* data) {
    switch (channel->state) {
    case RFC_MX_STATE_IDLE:
        rfc_mx_sm_state_idle(channel, event, data);
        break;
    case RFC_MX_STATE_WAIT_CONN_CNF:
        rfc_mx_sm_state_wait_conn_cnf(channel, event, data);
        break;
    case RFC_MX_STATE_CONFIGURE:
        rfc_mx_sm_state_configure(channel, event, data);
        break;
    case RFC_MX_STATE_SABME_WAIT_UA:
        rfc_mx_sm_sabme_wait_ua(channel, event, data);
        break;
    case RFC_MX_STATE_WAIT_SABME:
        rfc_mx_sm_state_wait_sabme(channel, event, data);
        break;
    case RFC_MX_STATE_CONNECTED:
        rfc_mx_sm_state_connected(channel, event, data);
        break;
    case RFC_MX_STATE_DISC_WAIT_UA:
        rfc_mx_sm_state_disc_wait_ua(channel, event, data);
        break;
    default:
        break;
    }
}
void rfc_mx_sm_state_idle(tRFC_MCB* channel, u16 event, void* data) {
    RFCOMM_TRACE_EVENT1("rfc_mx_sm_state_idle - evt:%d", event);

    switch (event) {
    case RFC_MX_EVENT_START_REQ: {
        u16 lcid;
        channel->peer_l2cap_mtu = L2CAP_DEFAULT_MTU - RFCOMM_MIN_OFFSET - 1;
        lcid = L2CA_ConnectReq(BT_PSM_RFCOMM, channel->bd_addr);
        channel->lcid = lcid;
        if (lcid == 0) {
            PORT_StartCnf(channel, 1);
        } else {
            rfc_save_lcid_mcb(channel, channel->lcid);
            channel->state = RFC_MX_STATE_WAIT_CONN_CNF;
        }
        break;
    }
    case RFC_MX_EVENT_START_RSP:
    case RFC_MX_EVENT_CONN_CNF:
    case RFC_MX_EVENT_CONF_CNF:
    case RFC_MX_EVENT_CONF_IND:
        RFCOMM_TRACE_ERROR2("Mx error state %d event %d", channel->state, event);
        break;

    case RFC_MX_EVENT_CONN_IND:
        if (channel->is_initiator != 0) {
            L2CA_ConnectRsp(channel->bd_addr, *(UINT8*)data, channel->lcid, 1,
                            0);
        } else {
            rfc_timer_start(channel, RFCOMM_CONN_TIMEOUT);
            L2CA_ConnectRsp(channel->bd_addr, *(UINT8*)data, channel->lcid, 0,
                            0);
            rfc_mx_send_config_req(channel);
            channel->state = RFC_MX_STATE_CONFIGURE;
        }
        break;
    case RFC_EVENT_UA:
    case RFC_EVENT_DM:
        return;
    case RFC_EVENT_DISC:
        rfc_send_dm(channel, 0, 1);
        break;
    case RFC_EVENT_UIH:
        rfc_send_dm(channel, 0, 0);
        break;
    case RFC_EVENT_SABME:
    default:
        RFCOMM_TRACE_EVENT2("RFCOMM MX ignored - evt:%d in state:%d", event, channel->state);
        break;
    }
}
void rfc_mx_sm_state_wait_conn_cnf(tRFC_MCB* channel, u16 event, void* data) {
    RFCOMM_TRACE_EVENT1("rfc_mx_sm_state_wait_conn_cnf - evt:%d", event);

    switch (event) {
    case RFC_MX_EVENT_START_REQ:
        RFCOMM_TRACE_ERROR2("Mx error state %d event %d", channel->state, event);
        break;
    case RFC_MX_EVENT_CONN_CNF:
        if (*(u16*)data != 0) {
            channel->state = RFC_MX_STATE_IDLE;
            PORT_StartCnf(channel, *(u16*)data);
        } else {
            channel->state = RFC_MX_STATE_CONFIGURE;
            rfc_mx_send_config_req(channel);
        }
        break;
    case RFC_MX_EVENT_DISC_IND:
        channel->state = RFC_MX_STATE_IDLE;
        PORT_CloseInd(channel);
        break;
    case RFC_EVENT_TIMEOUT:
        channel->state = RFC_MX_STATE_IDLE;
        L2CA_DisconnectReq(channel->lcid);
        PORT_CloseInd(channel);
        break;
    default:
        RFCOMM_TRACE_EVENT2("RFCOMM MX ignored - evt:%d in state:%d", event, channel->state);
        break;
    }
}
void rfc_mx_sm_state_configure(tRFC_MCB* channel, u16 event, void* data) {
    RFCOMM_TRACE_EVENT1("rfc_mx_sm_state_configure - evt:%d", event);

    switch (event) {
    case RFC_MX_EVENT_START_REQ:
    case RFC_MX_EVENT_CONN_CNF:
        RFCOMM_TRACE_ERROR2("Mx error state %d event %d", channel->state, event);
        break;
    case RFC_MX_EVENT_CONF_IND:
        rfc_mx_conf_ind(channel, (tL2CAP_CFG_INFO*)data);
        break;
    case RFC_MX_EVENT_CONF_CNF:
        rfc_mx_conf_cnf(channel, (tL2CAP_CFG_INFO*)data);
        break;
    case RFC_MX_EVENT_DISC_IND:
        channel->state = RFC_MX_STATE_IDLE;
        PORT_CloseInd(channel);
        break;
    default:
    case RFC_MX_EVENT_START_RSP:
    case RFC_MX_EVENT_CLOSE_REQ:
    case RFC_MX_EVENT_CONN_IND:
    case RFC_MX_EVENT_QOS_VIOLATION_IND:
        RFCOMM_TRACE_EVENT2("RFCOMM MX ignored - evt:%d in state:%d", event, channel->state);
        break;
    }
}
void rfc_mx_sm_sabme_wait_ua(tRFC_MCB* channel, u16 event, void* data) {
    RFCOMM_TRACE_EVENT1("rfc_mx_sm_sabme_wait_ua - evt:%d", event);

    switch (event) {
    case RFC_MX_EVENT_START_REQ:
    case RFC_MX_EVENT_CONN_CNF:
        RFCOMM_TRACE_ERROR2("Mx error state %d event %d", channel->state, event);
        break;
    case RFC_MX_EVENT_CONF_IND:
        rfc_mx_conf_ind(channel, (tL2CAP_CFG_INFO*)data);
        break;
    case RFC_MX_EVENT_CONF_CNF:
        rfc_mx_conf_cnf(channel, (tL2CAP_CFG_INFO*)data);
        break;
    case RFC_MX_EVENT_DISC_IND:
        channel->state = RFC_MX_STATE_IDLE;
        PORT_CloseInd(channel);
        break;
    case RFC_EVENT_UA:
        rfc_timer_stop(channel);
        channel->state = RFC_MX_STATE_CONNECTED;
        channel->peer_ready = 1;
        PORT_StartCnf(channel, 0);
        break;
    case RFC_EVENT_DM:
        rfc_timer_stop(channel);
        /* DM and timeout share the disconnect/error completion path. */
        /* fall through */
    case RFC_EVENT_TIMEOUT:
        channel->state = RFC_MX_STATE_IDLE;
        L2CA_DisconnectReq(channel->lcid);
        PORT_StartCnf(channel, 1);
        break;
    default:
        RFCOMM_TRACE_EVENT2("RFCOMM MX ignored - evt:%d in state:%d", event, channel->state);
        break;
    }
}
void rfc_mx_sm_state_wait_sabme(tRFC_MCB* channel, u16 event, void* data) {
    RFCOMM_TRACE_EVENT1("rfc_mx_sm_state_wait_sabme - evt:%d", event);

    switch (event) {
    case RFC_MX_EVENT_DISC_IND:
        channel->state = RFC_MX_STATE_IDLE;
        PORT_CloseInd(channel);
        break;
    case RFC_EVENT_SABME:
        PORT_StartInd(channel);
        break;
    case RFC_MX_EVENT_START_RSP:
        if (*(u16*)data != 0) {
            rfc_send_dm(channel, 0, 1);
        } else {
            rfc_send_ua(channel, 0);
            channel->state = RFC_MX_STATE_CONNECTED;
            channel->peer_ready = 1;
        }
        break;
    default:
        RFCOMM_TRACE_EVENT2("RFCOMM MX ignored - evt:%d in state:%d", event, channel->state);
        break;
    }
}
void rfc_mx_sm_state_connected(tRFC_MCB* channel, u16 event, void* data) {
    RFCOMM_TRACE_EVENT1("rfc_mx_sm_state_connected - evt:%d", event);

    /* Jump-table dispatch over the dense event range 3..14; every value in
     * the range must be an explicit case label for MWCC to build the table
     * (holes folded into default would lower to a compare chain). */
    switch (event) {
    case RFC_EVENT_TIMEOUT:
    case RFC_MX_EVENT_CLOSE_REQ:
        rfc_timer_start(channel, RFC_DISC_TIMEOUT);
        channel->state = RFC_MX_STATE_DISC_WAIT_UA;
        rfc_send_disc(channel, 0);
        break;
    case RFC_MX_EVENT_DISC_IND:
        channel->state = RFC_MX_STATE_IDLE;
        PORT_CloseInd(channel);
        break;
    case RFC_EVENT_DISC:
        rfc_send_ua(channel, 0);
        if (channel->is_initiator != 0) {
            L2CA_DisconnectReq(channel->lcid);
        }
        PORT_CloseInd(channel);
        break;
    default:
    case RFC_EVENT_UIH:
    case RFC_MX_EVENT_START_REQ:
    case RFC_MX_EVENT_START_RSP:
    case RFC_MX_EVENT_CONN_CNF:
    case RFC_MX_EVENT_CONN_IND:
    case RFC_MX_EVENT_CONF_CNF:
    case RFC_MX_EVENT_CONF_IND:
    case RFC_MX_EVENT_QOS_VIOLATION_IND:
        RFCOMM_TRACE_EVENT2("RFCOMM MX ignored - evt:%d in state:%d", event, channel->state);
        break;
    }
}
void rfc_mx_sm_state_disc_wait_ua(tRFC_MCB* channel, u16 event, void* data) {
    RFCOMM_TRACE_EVENT1("rfc_mx_sm_state_disc_wait_ua - evt:%d", event);

    switch (event) {
    case RFC_EVENT_UA:
    case RFC_EVENT_DM:
    case RFC_EVENT_TIMEOUT:
        L2CA_DisconnectReq(channel->lcid);
        if (channel->restart_required != 0) {
            u16 lcid = L2CA_ConnectReq(BT_PSM_RFCOMM, channel->bd_addr);
            channel->lcid = lcid;
            if (lcid == 0) {
                PORT_StartCnf(channel, 1);
                break;
            }
            rfc_save_lcid_mcb(channel, channel->lcid);
            channel->restart_required = 0;
            channel->local_cfg_sent = 0;
            channel->peer_cfg_rcvd = 0;
            channel->state = RFC_MX_STATE_WAIT_CONN_CNF;
        } else {
            rfc_release_multiplexer_channel(channel);
        }
        break;
    case RFC_EVENT_DISC:
        rfc_send_ua(channel, 0);
        break;
    case RFC_EVENT_UIH:
        GKI_freebuf(data);
        rfc_send_dm(channel, 0, 0);
        break;
    case RFC_MX_EVENT_START_REQ:
        channel->restart_required = 1;
        break;
    case RFC_MX_EVENT_DISC_IND:
        channel->state = RFC_MX_STATE_IDLE;
        PORT_CloseInd(channel);
        break;
    case RFC_MX_EVENT_CLOSE_REQ:
    case RFC_MX_EVENT_QOS_VIOLATION_IND:
        return;
    default:
        RFCOMM_TRACE_EVENT2("RFCOMM MX ignored - evt:%d in state:%d", event, channel->state);
        break;
    }
}
void rfc_mx_conf_cnf(tRFC_MCB* channel, tL2CAP_CFG_INFO* config) {
    RFCOMM_TRACE_EVENT2("rfc_mx_conf_cnf p_cfg:%08x res:%d ", (u32)config, config != 0 ? config->result : 0);

    if (config->result != 0) {
        if (channel->is_initiator != 0) {
            PORT_StartCnf(channel, config->result);
            L2CA_DisconnectReq(channel->lcid);
        }
        rfc_release_multiplexer_channel(channel);
        return;
    }

    channel->local_cfg_sent = 1;
    if (channel->state == RFC_MX_STATE_CONFIGURE && channel->peer_cfg_rcvd != 0) {
        if (channel->is_initiator != 0) {
            channel->state = RFC_MX_STATE_SABME_WAIT_UA;
            rfc_send_sabme(channel, 0);
        } else {
            channel->state = RFC_MX_STATE_WAIT_SABME;
        }
    }
}
void rfc_mx_conf_ind(tRFC_MCB* channel, tL2CAP_CFG_INFO* config) {
    RFCOMM_TRACE_EVENT1("rfc_mx_conf_ind p_cfg:%0x", (u32)config);

    if (config->mtu_present != 0) {
        channel->peer_l2cap_mtu = config->mtu - RFCOMM_MIN_OFFSET - 1;
    } else {
        channel->peer_l2cap_mtu = L2CAP_DEFAULT_MTU - RFCOMM_MIN_OFFSET - 1;
    }

    config->mtu_present = 0;
    config->flush_to_present = 0;
    config->qos_present = 0;
    config->result = 0;
    L2CA_ConfigRsp(channel->lcid, config);
    channel->peer_cfg_rcvd = 1;

    if (channel->state == RFC_MX_STATE_CONFIGURE && channel->local_cfg_sent != 0) {
        if (channel->is_initiator != 0) {
            channel->state = RFC_MX_STATE_SABME_WAIT_UA;
            rfc_send_sabme(channel, 0);
        } else {
            channel->state = RFC_MX_STATE_WAIT_SABME;
        }
    }
}

static void rfc_mx_send_config_req(tRFC_MCB* p_mcb) {
    RfcConfig cfg;

    RFCOMM_TRACE_EVENT0("rfc_mx_send_config_req");

    memset(&cfg, 0, sizeof(RfcConfig));

    cfg.mtu_present      = TRUE;
    cfg.mtu              = L2CAP_MTU_SIZE;
    cfg.flush_to_present = FALSE;
    cfg.qos_present      = FALSE;

    L2CA_ConfigReq(p_mcb->lcid, (tL2CAP_CFG_INFO*)&cfg);
}
