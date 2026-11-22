#include <harness_catalog.h>

#include "revolution/BTE/stack/rfcomm/rfc_int.h"
#include "revolution/BTE/include/bt_trace.h"

static tRFC_MCB* rfc_find_lcid_mcb(UINT16 lcid) {
    tRFC_MCB* p_mcb;

    if (lcid - L2CAP_BASE_APPL_CID >= MAX_L2CAP_CHANNELS) {
        RFCOMM_TRACE_ERROR1("rfc_find_lcid_mcb LCID:0x%x", lcid);
        return NULL;
    }

    p_mcb = rfc_cb.rfc.p_rfc_lcid_mcb[lcid - L2CAP_BASE_APPL_CID];
    if (p_mcb != NULL && p_mcb->lcid != lcid) {
        RFCOMM_TRACE_WARNING2("rfc_find_lcid_mcb LCID reused LCID:0x%x current:0x%x", lcid, p_mcb->lcid);
        return NULL;
    }
    return p_mcb;
}

void RFCOMM_ConnectInd(u8* bd_addr, u16 lcid, u16 psm, u8 id);
void RFCOMM_ConnectCnf(u16 lcid, u16 result);
void RFCOMM_ConfigInd(u16 lcid, tL2CAP_CFG_INFO* p_cfg);
void RFCOMM_ConfigCnf(u16 lcid, tL2CAP_CFG_INFO* config);
void RFCOMM_DisconnectInd(u16 lcid, u8 response);
void RFCOMM_QoSViolationInd(BD_ADDR bd_addr);
void RFCOMM_BufDataInd(u16 lcid, BT_HDR* buffer);
void RFCOMM_CongestionStatusInd(u16 lcid, u8 congested);

void rfcomm_l2cap_if_init() {
    tL2CAP_APPL_INFO* p_info = &rfc_cb.rfc.reg_info;

    p_info->pL2CA_ConnectInd_Cb = RFCOMM_ConnectInd;
    p_info->pL2CA_ConnectCfm_Cb = RFCOMM_ConnectCnf;
    p_info->pL2CA_ConnectPnd_Cb = 0;
    p_info->pL2CA_ConfigInd_Cb = RFCOMM_ConfigInd;
    p_info->pL2CA_ConfigCfm_Cb = RFCOMM_ConfigCnf;
    p_info->pL2CA_DisconnectInd_Cb = RFCOMM_DisconnectInd;
    p_info->pL2CA_DisconnectCfm_Cb = 0;
    p_info->pL2CA_QoSViolationInd_Cb = RFCOMM_QoSViolationInd;
    p_info->pL2CA_DataInd_Cb = RFCOMM_BufDataInd;
    p_info->pL2CA_CongestionStatus_Cb = RFCOMM_CongestionStatusInd;

    L2CA_Register(BT_PSM_RFCOMM, p_info);
}

void RFCOMM_ConnectInd(u8* bd_addr, u16 lcid, u16 psm, u8 id) {
    tRFC_MCB* p_mcb = rfc_alloc_multiplexer_channel(bd_addr, 0);

    rfc_cb.rfc.p_rfc_lcid_mcb[lcid - L2CAP_BASE_APPL_CID] = p_mcb;

    if (p_mcb == 0) {
        L2CA_ConnectRsp(bd_addr, id, lcid, L2CAP_CONN_NO_RESOURCES, 0);
        return;
    }

    p_mcb->lcid = lcid;

    rfc_mx_sm_execute(p_mcb, RFC_MX_EVENT_CONN_IND, &id);
}

void RFCOMM_ConnectCnf(u16 lcid, u16 result) {
    tRFC_MCB* channel = rfc_find_lcid_mcb(lcid);

    if (channel == 0) {
        RFCOMM_TRACE_ERROR1("RFCOMM_ConnectCnf LCID:0x%x", lcid);
    } else {
        channel->lcid = lcid;
        rfc_mx_sm_execute(channel, RFC_MX_EVENT_CONN_CNF, &result);
    }
}

void RFCOMM_ConfigInd(u16 lcid, tL2CAP_CFG_INFO* p_cfg) {
    tRFC_MCB* channel = rfc_find_lcid_mcb(lcid);

    if (channel == 0) {
        RFCOMM_TRACE_ERROR1("RFCOMM_ConfigInd LCID:0x%x", lcid);
    } else {
        rfc_mx_sm_execute(channel, RFC_MX_EVENT_CONF_IND, p_cfg);
    }
}

void RFCOMM_ConfigCnf(u16 lcid, tL2CAP_CFG_INFO* config) {
    tRFC_MCB* channel = rfc_find_lcid_mcb(lcid);

    if (channel == 0) {
        RFCOMM_TRACE_ERROR1("RFCOMM_ConfigCnf LCID:0x%x", lcid);
    } else {
        rfc_mx_sm_execute(channel, RFC_MX_EVENT_CONF_CNF, config);
    }
}

void RFCOMM_QoSViolationInd(BD_ADDR bd_addr) {}

void RFCOMM_DisconnectInd(u16 lcid, u8 response) {
    tRFC_MCB* channel = rfc_find_lcid_mcb(lcid);

    if (response != 0) {
        L2CA_DisconnectRsp(lcid);
    }

    if (channel == 0) {
        RFCOMM_TRACE_WARNING1("RFCOMM_DisconnectInd LCID:0x%x", lcid);
    } else {
        rfc_mx_sm_execute(channel, RFC_MX_EVENT_DISC_IND, NULL);
    }
}


void RFCOMM_BufDataInd(u16 lcid, BT_HDR* buffer) {
    tRFC_MCB* channel = rfc_find_lcid_mcb(lcid);
    tPORT* port = 0;
    u8 frame_type;

    if (channel == 0) {
        RFCOMM_TRACE_WARNING1("RFCOMM_BufDataInd LCID:0x%x", lcid);
        GKI_freebuf(buffer);
        return;
    }

    frame_type = rfc_parse_data(channel, &rfc_cb.rfc.rx_frame, buffer);
    if (frame_type == RFC_EVENT_BAD_FRAME) {
        GKI_freebuf(buffer);
        return;
    }

    if (rfc_cb.rfc.rx_frame.dlci == 0) {
        if (frame_type == RFC_EVENT_UIH) {
            rfc_process_mx_message(channel, buffer);
        } else {
            rfc_mx_sm_execute(channel, frame_type, 0);
            GKI_freebuf(buffer);
        }
        return;
    }

    port = port_find_mcb_dlci_port(channel, rfc_cb.rfc.rx_frame.dlci);
    if (port == 0 || port->rfc.p_mcb == 0) {
        if (frame_type != RFC_EVENT_SABME) {
            if ((channel->is_initiator != 0 && rfc_cb.rfc.rx_frame.cr == 0) ||
                (channel->is_initiator == 0 && rfc_cb.rfc.rx_frame.cr != 0)) {
                rfc_send_dm(channel, rfc_cb.rfc.rx_frame.dlci, rfc_cb.rfc.rx_frame.pf);
            }
            GKI_freebuf(buffer);
            return;
        }

        port = port_find_dlci_port(rfc_cb.rfc.rx_frame.dlci);
        if (port == 0) {
            rfc_send_dm(channel, rfc_cb.rfc.rx_frame.dlci, 1);
            GKI_freebuf(buffer);
            return;
        }
        channel->port_inx[rfc_cb.rfc.rx_frame.dlci] = port->inx;
        port->rfc.p_mcb = channel;
    }

    if (frame_type == RFC_EVENT_UIH) {
        if (buffer->len != 0) {
            rfc_port_sm_execute(port, frame_type, buffer);
        } else {
            GKI_freebuf(buffer);
        }
        if (rfc_cb.rfc.rx_frame.credit != 0) {
            rfc_inc_credit(port, rfc_cb.rfc.rx_frame.credit);
        }
    } else {
        rfc_port_sm_execute(port, frame_type, 0);
        GKI_freebuf(buffer);
    }
}

void RFCOMM_CongestionStatusInd(u16 lcid, u8 congested) {
    tRFC_MCB* channel = rfc_find_lcid_mcb(lcid);

    if (channel == 0) {
        RFCOMM_TRACE_ERROR1("RFCOMM_CongestionStatusInd dropped LCID:0x%x", lcid);
    } else {
        RFCOMM_TRACE_EVENT1("RFCOMM_CongestionStatusInd LCID:0x%x", lcid);
        rfc_process_l2cap_congestion(channel, congested);
    }
}

void rfc_save_lcid_mcb(tRFC_MCB* p_mcb, u16 lcid) {
    rfc_cb.rfc.p_rfc_lcid_mcb[lcid - L2CAP_BASE_APPL_CID] = p_mcb;
}
