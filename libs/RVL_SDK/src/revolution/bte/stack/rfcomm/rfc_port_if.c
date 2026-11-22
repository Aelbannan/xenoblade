// Decompiled: libs/RVL_SDK/src/revolution/bte/stack/rfcomm/rfc_port_if.c
// High-level C reconstruction of the RFCOMM port-interface layer.

#include <harness_catalog.h>

#include "revolution/BTE/stack/rfcomm/rfc_int.h"

tRFC_CB rfc_cb;

void RFCOMM_StartRsp(tRFC_MCB* p_mcb, UINT16 result) {
    rfc_mx_sm_execute(p_mcb, RFC_MX_EVENT_START_RSP, &result);
}

void RFCOMM_DlcEstablishReq(tRFC_MCB* mcb, u8 dlci, u16 mtu) {
    tPORT* port = port_find_mcb_dlci_port(mcb, dlci);

    if (mcb->state != RFC_MX_STATE_CONNECTED) {
        PORT_DlcEstablishCnf(mcb, dlci, 0, 1);
    } else {
        rfc_port_sm_execute(port, RFC_EVENT_OPEN, 0);
    }
}

void RFCOMM_DlcEstablishRsp(tRFC_MCB* mcb, u8 dlci, u16 mtu, u16 result) {
    tPORT* port = port_find_mcb_dlci_port(mcb, dlci);

    if (mcb->state != RFC_MX_STATE_CONNECTED && result == 0) {
        PORT_DlcReleaseInd(mcb, dlci);
    } else {
        rfc_port_sm_execute(port, RFC_EVENT_ESTABLISH_RSP, &result);
    }
}

void RFCOMM_ParNegReq(tRFC_MCB* mcb, u8 dlci, u16 mtu) {
    tPORT* port = port_find_mcb_dlci_port(mcb, dlci);
    u8 flow;
    u8 cl;
    u8 k;

    if (mcb->state != RFC_MX_STATE_CONNECTED) {
        port->error = 0x0D;
        return;
    }

    flow = (mcb->flow == 0) ? 2 : mcb->flow;
    if (flow == 2) {
        cl = 0xF0;
        k = (port->credit_rx_max >= 7) ? 7 : port->credit_rx_max;
        port->credit_rx = k;
    } else {
        cl = 0;
        k = 0;
    }

    port->rfc.expected_rsp |= RFC_RSP_PN;
    rfc_send_pn(mcb, dlci, 1, mtu, cl, k);
    rfc_port_timer_start(port, RFC_PORT_T1_TIMEOUT);
}

void RFCOMM_ParNegRsp(tRFC_MCB* mcb, u8 dlci, u16 mtu, u8 cl, u8 k) {
    if (mcb->state != RFC_MX_STATE_CONNECTED)
        return;

    rfc_send_pn(mcb, dlci, 0, mtu, cl, k);
}

void RFCOMM_PortNegReq(tRFC_MCB* mcb, u8 dlci, tPORT_STATE* p_port_ctrl) {
    tPORT* port = port_find_mcb_dlci_port(mcb, dlci);

    if (mcb->state != RFC_MX_STATE_CONNECTED) {
        PORT_PortNegCnf(mcb, dlci, 0, 1);
        return;
    }

    if (p_port_ctrl == 0)
        port->rfc.expected_rsp |= RFC_RSP_RPN_REPLY;
    else
        port->rfc.expected_rsp |= RFC_RSP_RPN;

    rfc_send_rpn(mcb, dlci, 1, p_port_ctrl, 0x3F7F);
    rfc_port_timer_start(port, RFC_PORT_T1_TIMEOUT);
}

void RFCOMM_PortNegRsp(tRFC_MCB* mcb, u8 dlci, tPORT_STATE* p_port_ctrl, u16 request) {
    if (mcb->state != RFC_MX_STATE_CONNECTED)
        return;

    rfc_send_rpn(mcb, dlci, 0, p_port_ctrl, request);
}

void RFCOMM_ControlReq(tRFC_MCB* mcb, u8 dlci, tPORT_CTRL* p_port_ctrl) {
    tPORT* port = port_find_mcb_dlci_port(mcb, dlci);

    if (port->state != PORT_STATE_OPENED || port->rfc.state != RFC_STATE_OPENED)
        return;

    port->port_ctrl |= 1;
    port->rfc.expected_rsp |= RFC_RSP_MSC;
    rfc_send_msc(mcb, dlci, 1, p_port_ctrl);
    rfc_port_timer_start(port, RFC_PORT_T1_TIMEOUT);
}

void RFCOMM_FlowReq(tRFC_MCB* mcb, u8 dlci, u8 enable) {
    tPORT* port = port_find_mcb_dlci_port(mcb, dlci);

    if (port->state != PORT_STATE_OPENED || port->rfc.state != RFC_STATE_OPENED)
        return;

    port->local_ctrl.fc = (enable == 0);
    port->rfc.expected_rsp |= RFC_RSP_MSC;
    rfc_send_msc(mcb, dlci, 1, &port->local_ctrl);
    rfc_port_timer_start(port, RFC_PORT_T1_TIMEOUT);
}

void RFCOMM_LineStatusReq(tRFC_MCB* mcb, u8 dlci, u8 status) {
    tPORT* port = port_find_mcb_dlci_port(mcb, dlci);

    if (port->state != PORT_STATE_OPENED || port->rfc.state != RFC_STATE_OPENED)
        return;

    port->rfc.expected_rsp |= RFC_RSP_RLS;
    rfc_send_rls(mcb, dlci, 1, status);
    rfc_port_timer_start(port, RFC_PORT_T1_TIMEOUT);
}

void RFCOMM_DlcReleaseReq(tRFC_MCB* mcb, u8 dlci) {
    rfc_port_sm_execute(port_find_mcb_dlci_port(mcb, dlci), RFC_EVENT_CLOSE, 0);
}

void RFCOMM_DataReq(tRFC_MCB* mcb, u8 dlci, BT_HDR* p_buf) {
    rfc_port_sm_execute(port_find_mcb_dlci_port(mcb, dlci), RFC_EVENT_DATA, p_buf);
}
