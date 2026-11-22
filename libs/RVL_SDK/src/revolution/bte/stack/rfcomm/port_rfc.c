// Decompiled: libs/RVL_SDK/src/revolution/bte/stack/rfcomm/port_rfc.c
// High-level C reconstruction of the RFCOMM port layer (Broadcom BTE/Bluedroid).
//
// These functions glue the RFCOMM multiplexer (tRFC_MCB) to the port objects
// (tPORT) held in the global rfc_cb control block.  Structure field offsets
// were recovered from the retail assembly and cross-checked against the
// already-decompiled sibling unit port_utils.c.
//
// PORT_* functions here are the callbacks invoked by the RFCOMM multiplexer
// finite-state machines (rfc_mx_fsm.c / rfc_port_fsm.c): start/parameter
// negotiation, DLC establishment/release, MSC control, line status and data
// delivery.  Events reported to the application through p_mgmt_cb mirror the
// serial-port event bits defined in port_api.c (PORT_EV_*).

#include <harness_catalog.h>
#include <string.h>

#include "revolution/BTE/stack/rfcomm/rfc_int.h"
#include "revolution/BTE/include/bt_trace.h"

/* Continuation of PORT_Open after the multiplexer start handshake. */
void port_open_continue(tPORT* p_port)
{
    RFCOMM_TRACE_EVENT0("port_open_continue");

    if (p_port->rfc.p_mcb == NULL) {
        RFCOMM_TRACE_WARNING0("port_open_continue no mx channel");
        return;
    }

    RFCOMM_PortNegReq(p_port->rfc.p_mcb, p_port->dlci, &p_port->peer_port_pars);
}

/* ========================================================================
 * PORT_StartCnf
 * ======================================================================== */
void PORT_StartCnf(tRFC_MCB* p_mcb, u16 result)
{
    tPORT* p_port;
    int i;
    int no_port = 1;

    RFCOMM_TRACE_EVENT1("PORT_StartCnf result:%d", result);

    for (i = 0, p_port = &rfc_cb.port.port[0]; i < MAX_RFC_PORTS; i++, p_port++) {
        if (p_port->rfc.p_mcb == p_mcb) {
            no_port = 0;
            if (result == 0) {
                RFCOMM_ParNegReq(p_mcb, p_port->dlci, p_port->mtu);
            } else {
                RFCOMM_TRACE_WARNING1("PORT_StartCnf failed result:%d", result);

                if (result == 4)
                    p_port->error = 0x18;
                else
                    p_port->error = 0x0C;

                rfc_release_multiplexer_channel(p_mcb);
                p_port->rfc.p_mcb = NULL;

                if (p_port->p_callback != NULL && (p_port->ev_mask & PORT_EV_CONNECT_ERR))
                    p_port->p_callback(PORT_EV_CONNECT_ERR, p_port->inx);

                if (p_port->p_mgmt_callback != NULL)
                    p_port->p_mgmt_callback(0x0C, p_port->inx);

                port_release_port(p_port);
            }
        }
    }

    if (no_port)
        rfc_check_mcb_active(p_mcb);
}

/* ========================================================================
 * PORT_StartInd
 * ======================================================================== */
void PORT_StartInd(tRFC_MCB* p_mcb)
{
    tPORT* p_port;
    int i;

    RFCOMM_TRACE_EVENT0("PORT_StartInd");

    for (i = 0, p_port = &rfc_cb.port.port[0]; i < MAX_RFC_PORTS; i++, p_port++) {
        if (p_port->rfc.p_mcb == NULL || p_port->rfc.p_mcb == p_mcb) {
            RFCOMM_StartRsp(p_mcb, 0);
            return;
        }
    }

    RFCOMM_StartRsp(p_mcb, 1);
}

/* ========================================================================
 * PORT_ParNegInd
 * ======================================================================== */
void PORT_ParNegInd(tRFC_MCB* p_mcb, u8 dlci, u16 mtu, u8 cl, u8 k)
{
    tPORT* p_port = port_find_mcb_dlci_port(p_mcb, dlci);
    u8 cl_rsp;
    u8 k_rsp;

    RFCOMM_TRACE_EVENT2("PORT_ParNegInd dlci:%d mtu:%d", dlci, mtu);

    if (p_port == NULL) {
        p_port = port_find_dlci_port(dlci);
        if (p_port == NULL) {
            rfc_send_dm(p_mcb, dlci, 0);
            rfc_check_mcb_active(p_mcb);
            RFCOMM_TRACE_EVENT0("PORT_ParNegInd: port not found");
            return;
        }
        p_mcb->port_inx[dlci] = p_port->inx;
    }

    memcpy(p_port->bd_addr, p_mcb->bd_addr, 6);
    port_select_mtu(p_port);
    p_port->rfc.p_mcb = p_mcb;

    if (p_port->mtu < mtu)
        mtu = p_port->mtu;
    p_port->mtu = mtu;
    p_port->peer_mtu = mtu;

    if (p_mcb->flow == 0) {
        if (cl == 0)
            p_mcb->flow = 1;
        else
            p_mcb->flow = 2;
    }

    if (cl == 0) {
        cl_rsp = 0;
        k_rsp = 0;
    } else {
        if (p_mcb->flow == 2) {
            p_port->credit_tx = k;
            if (k == 0)
                p_port->tx.peer_fc = 1;

            cl_rsp = 0xE0;
            k_rsp = (p_port->credit_rx_max < 7) ? p_port->credit_rx_max : 7;
            p_port->credit_rx = k_rsp;
        } else {
            cl_rsp = 0;
            k_rsp = 0;
        }
    }

    RFCOMM_ParNegRsp(p_mcb, dlci, p_port->mtu, cl_rsp, k_rsp);
}

/* ========================================================================
 * PORT_ParNegCnf
 * ======================================================================== */
void PORT_ParNegCnf(tRFC_MCB* p_mcb, u8 dlci, u16 mtu, u8 cl, u8 k)
{
    tPORT* p_port = port_find_mcb_dlci_port(p_mcb, dlci);

    RFCOMM_TRACE_EVENT4("PORT_ParNegCnf dlci:%d mtu:%d cl: %d k: %d", dlci, mtu, cl, k);

    if (p_port == NULL)
        return;

    if (p_mcb->flow == 0) {
        if (cl == 0xE0)
            p_mcb->flow = 2;
        else
            p_mcb->flow = 1;
    }

    if (p_port->mtu < mtu)
        mtu = p_port->mtu;
    p_port->mtu = mtu;
    p_port->peer_mtu = mtu;

    if (p_mcb->flow == 2) {
        p_port->credit_tx = k;
        if (k == 0)
            p_port->tx.peer_fc = 1;
    }

    if (p_port->state == PORT_STATE_OPENING)
        RFCOMM_DlcEstablishReq(p_mcb, p_port->dlci, p_port->mtu);
}

/* ========================================================================
 * PORT_DlcEstablishInd
 * ======================================================================== */
void PORT_DlcEstablishInd(tRFC_MCB* p_mcb, u8 dlci, u16 mtu)
{
    tPORT* p_port = port_find_mcb_dlci_port(p_mcb, dlci);

    RFCOMM_TRACE_EVENT2("PORT_DlcEstablishInd dlci:%d mtu:%d", dlci, mtu);

    if (p_port == NULL) {
        p_port = port_find_dlci_port(dlci);
        if (p_port == NULL) {
            RFCOMM_DlcEstablishRsp(p_mcb, dlci, 0, 1);
            return;
        }
        p_mcb->port_inx[dlci] = p_port->inx;
    }

    if (mtu != 0 && mtu < p_port->peer_mtu)
        p_port->peer_mtu = mtu;

    rfc_timer_stop(p_mcb);
    RFCOMM_DlcEstablishRsp(p_mcb, dlci, p_port->mtu, 0);

    if (p_port->p_callback != NULL && (p_port->ev_mask & 0x200))
        p_port->p_callback(0x200, p_port->inx);

    if (p_port->p_mgmt_callback != NULL)
        p_port->p_mgmt_callback(0, p_port->inx);

    p_port->state = PORT_STATE_OPENED;
}

/* ========================================================================
 * PORT_DlcEstablishCnf
 * ======================================================================== */
void PORT_DlcEstablishCnf(tRFC_MCB* p_mcb, u8 dlci, u16 mtu, u16 result)
{
    tPORT* p_port = port_find_mcb_dlci_port(p_mcb, dlci);

    RFCOMM_TRACE_EVENT3("PORT_DlcEstablishCnf dlci:%d mtu:%d result:%d", dlci, mtu, result);

    if (p_port == NULL)
        return;

    if (result != 0) {
        p_port->error = 0x0C;
        port_rfc_closed(p_port, 0x0C);
        return;
    }

    if (mtu != 0 && mtu < p_port->peer_mtu)
        p_port->peer_mtu = mtu;

    rfc_timer_stop(p_mcb);

    if (p_port->p_callback != NULL && (p_port->ev_mask & 0x200))
        p_port->p_callback(0x200, p_port->inx);

    if (p_port->p_mgmt_callback != NULL)
        p_port->p_mgmt_callback(0, p_port->inx);

    p_port->state = PORT_STATE_OPENED;

    if (p_port->uuid == 0x1103 || p_port->uuid == 0x1111)
        RFCOMM_PortNegReq(p_port->rfc.p_mcb, p_port->dlci, NULL);
    else
        RFCOMM_ControlReq(p_port->rfc.p_mcb, p_port->dlci, &p_port->local_ctrl);
}

/* ========================================================================
 * PORT_PortNegInd
 * ======================================================================== */
void PORT_PortNegInd(tRFC_MCB* p_mcb, u8 dlci, tPORT_STATE* p_port_ctrl, u16 request)
{
    tPORT* p_port = port_find_mcb_dlci_port(p_mcb, dlci);

    RFCOMM_TRACE_EVENT0("PORT_PortNegInd");

    if (p_port == NULL) {
        p_port = port_find_dlci_port(dlci);
        if (p_port == NULL) {
            p_port->peer_port_pars = *p_port_ctrl;
            RFCOMM_PortNegRsp(p_mcb, dlci, p_port_ctrl, 0);
            return;
        }
        p_mcb->port_inx[dlci] = p_port->inx;
    }

    p_port->peer_port_pars = *p_port_ctrl;
    RFCOMM_PortNegRsp(p_mcb, dlci, p_port_ctrl, request);
}

/* ========================================================================
 * PORT_PortNegCnf
 * ======================================================================== */
void PORT_PortNegCnf(tRFC_MCB* p_mcb, u8 dlci, tPORT_STATE* p_port_ctrl, u16 result)
{
    tPORT* p_port = port_find_mcb_dlci_port(p_mcb, dlci);

    RFCOMM_TRACE_EVENT0("PORT_PortNegCnf");

    if (p_port == NULL) {
        RFCOMM_TRACE_WARNING0("PORT_PortNegCnf no port");
        return;
    }

    if (result != 0) {
        p_port->error = 0x0E;
        RFCOMM_DlcReleaseReq(p_mcb, p_port->dlci);
        port_rfc_closed(p_port, 0x0E);
        return;
    }

    if ((p_port->port_ctrl & 1) == 0) {
        RFCOMM_ControlReq(p_port->rfc.p_mcb, p_port->dlci, &p_port->local_ctrl);
    } else {
        RFCOMM_TRACE_WARNING0("PORT_PortNegCnf Control Already sent");
    }
}

/* ========================================================================
 * PORT_ControlInd - remote MSC received.  Differs from the local signals,
 * forwards control info back if we have not done so yet, then reports any
 * signal changes / transmitted data to the application.
 *
 * ======================================================================== */
void PORT_ControlInd(tRFC_MCB* p_mcb, u8 dlci, tPORT_CTRL* p_port_ctrl)
{
    tPORT* p_port = port_find_mcb_dlci_port(p_mcb, dlci);
    u32 event;

    RFCOMM_TRACE_EVENT0("PORT_ControlInd");

    if (p_port == NULL)
        return;

    event = port_get_signal_changes(p_port, p_port->peer_ctrl.modem_signal, p_port_ctrl->modem_signal);

    p_port->peer_ctrl.modem_signal = p_port_ctrl->modem_signal;
    p_port->peer_ctrl.break_signal = p_port_ctrl->break_signal;
    p_port->peer_ctrl.discard_buffers = p_port_ctrl->discard_buffers;
    p_port->peer_ctrl.break_signal_seq = p_port_ctrl->break_signal_seq;
    p_port->peer_ctrl.fc = p_port_ctrl->fc;

    if ((p_port->port_ctrl & 1) == 0) {
        RFCOMM_ControlReq(p_port->rfc.p_mcb, p_port->dlci, &p_port->local_ctrl);
    } else {
        if ((p_port->port_ctrl & 4) == 0)
            event |= (p_port->ev_mask & 0x200);
        if (p_port->port_ctrl & 2)
            event |= port_rfc_send_tx_data(p_port);
    }

    p_port->port_ctrl |= 0x0C;

    if (p_port_ctrl->break_signal != 0)
        event |= (p_port->ev_mask & 0x40);

    if (event != 0 && p_port->p_callback != NULL)
        p_port->p_callback(event, p_port->inx);

    RFCOMM_TRACE_EVENT4("PORT_ControlInd DTR_DSR : %d, RTS_CTS : %d, RI : %d, DCD : %d",
                        ((p_port->peer_ctrl.modem_signal & 0x01) ? 1 : 0),
                        ((p_port->peer_ctrl.modem_signal & 0x02) ? 1 : 0),
                        ((p_port->peer_ctrl.modem_signal & 0x04) ? 1 : 0),
                        ((p_port->peer_ctrl.modem_signal & 0x08) ? 1 : 0));
}

/* ========================================================================
 * PORT_ControlCnf
 * ======================================================================== */
void PORT_ControlCnf(tRFC_MCB* p_mcb, u8 dlci, tPORT_CTRL* p_pars)
{
    tPORT* p_port = port_find_mcb_dlci_port(p_mcb, dlci);
    u32 event = 0;

    RFCOMM_TRACE_EVENT0("PORT_ControlCnf");

    if (p_port == NULL)
        return;

    if ((p_port->port_ctrl & 2) == 0) {
        p_port->port_ctrl |= 2;
        if (p_port->port_ctrl & 4)
            event = (p_port->ev_mask & 0x200);
    }

    if (p_port->port_ctrl & 4)
        event |= port_rfc_send_tx_data(p_port);

    if (event != 0 && p_port->p_callback != NULL)
        p_port->p_callback(event, p_port->inx);
}

/* ========================================================================
 * PORT_LineStatusInd
 * ======================================================================== */
void PORT_LineStatusInd(tRFC_MCB* p_mcb, u8 dlci, u8 line_status)
{
    tPORT* p_port = port_find_mcb_dlci_port(p_mcb, dlci);
    u32 event = 0;

    RFCOMM_TRACE_EVENT0("PORT_LineStatusInd");

    if (p_port == NULL)
        return;

    p_port->line_status |= line_status;

    if (line_status & 0x02)
        event |= 0x2000;

    if (line_status & 0x01)
        event |= 0x40;

    if (line_status & 0xFC)
        event |= 0x80;

    if (p_port->p_callback != NULL && (p_port->ev_mask & event) != 0)
        p_port->p_callback(p_port->ev_mask & event, p_port->inx);
}

/* ========================================================================
 * PORT_DlcReleaseInd
 * ======================================================================== */
void PORT_DlcReleaseInd(tRFC_MCB* p_mcb, u8 dlci)
{
    tPORT* p_port = port_find_mcb_dlci_port(p_mcb, dlci);

    RFCOMM_TRACE_EVENT0("PORT_DlcReleaseInd");

    if (p_port != NULL)
        port_rfc_closed(p_port, 0x13);
}

/* ========================================================================
 * PORT_CloseInd
 * ======================================================================== */
void PORT_CloseInd(tRFC_MCB* p_mcb)
{
    tPORT* p_port;
    int i;

    RFCOMM_TRACE_EVENT0("PORT_CloseInd");

    for (i = 0, p_port = &rfc_cb.port.port[0]; i < MAX_RFC_PORTS; i++, p_port++) {
        if (p_port->rfc.p_mcb == p_mcb)
            port_rfc_closed(p_port, 0x10);
    }

    rfc_release_multiplexer_channel(p_mcb);
}

/* ========================================================================
 * Port_TimeOutCloseMux
 * ======================================================================== */
void Port_TimeOutCloseMux(tRFC_MCB* p_mcb)
{
    tPORT* p_port;
    int i;

    RFCOMM_TRACE_EVENT0("Port_TimeOutCloseMux");

    for (i = 0, p_port = &rfc_cb.port.port[0]; i < MAX_RFC_PORTS; i++, p_port++) {
        if (p_port->rfc.p_mcb == p_mcb)
            port_rfc_closed(p_port, 0x12);
    }
}

/* ========================================================================
 * PORT_DataInd
 * ======================================================================== */
void PORT_DataInd(tRFC_MCB* p_mcb, u8 dlci, BT_HDR* p_buf)
{
    tPORT* p_port = port_find_mcb_dlci_port(p_mcb, dlci);
    u32 event = 0;

    RFCOMM_TRACE_EVENT1("PORT_DataInd with data length %d", p_buf->len);

    if (p_port == NULL) {
        GKI_freebuf(p_buf);
        return;
    }

    if (p_port->p_data_callback != NULL) {
        port_flow_control_peer(p_port, 1, 1);
        p_port->p_data_callback(p_port->inx, (UINT8 *)(p_buf + 1) + p_buf->offset, p_buf->len);
        GKI_freebuf(p_buf);
        return;
    }

    if (p_port->rx.queue_size + p_buf->len > 0x2EE0 ||
        p_port->rx.queue.count + 1 > p_port->rx_buf_critical) {
        RFCOMM_TRACE_EVENT0("PORT_DataInd. Buffer over run. Dropping the buffer");
        GKI_freebuf(p_buf);
        RFCOMM_LineStatusReq(p_mcb, dlci, 2);
        return;
    }

    if (p_port->user_port_pars.rx_char1 != 0 && (p_port->ev_mask & 0x2)) {
        u8* p = (UINT8 *)(p_buf + 1) + p_buf->offset;
        int n = p_buf->len;
        for (; n > 0; n--) {
            if (*p++ == p_port->user_port_pars.rx_char1) {
                event |= 0x2;
                break;
            }
        }
    }

    GKI_enqueue(&p_port->rx.queue, p_buf);
    p_port->rx.queue_size += p_buf->len;
    port_flow_control_peer(p_port, 0, 0);

    if (p_port->rx.user_fc != 0) {
        if (event & 0x2)
            p_port->rx_flag_ev_pending = 1;
    } else {
        event |= 0x1;
        event &= p_port->ev_mask;
        if (p_port->p_callback != NULL && event != 0)
            p_port->p_callback(event, p_port->inx);
    }
}

/* ========================================================================
 * PORT_FlowInd
 * ======================================================================== */
void PORT_FlowInd(tRFC_MCB* p_mcb, u8 dlci, u8 fc)
{
    tPORT* p_port = NULL;
    u32 event;
    int i;

    RFCOMM_TRACE_EVENT1("PORT_FlowInd fc:%d", fc);

    if (dlci == 0) {
        p_mcb->peer_ready = fc;
    } else {
        p_port = port_find_mcb_dlci_port(p_mcb, dlci);
        if (p_port == NULL)
            return;
        p_port->tx.peer_fc = (fc == 0);
    }

    for (i = 0; i < MAX_RFC_PORTS; i++) {
        if (dlci == 0) {
            p_port = &rfc_cb.port.port[i];
            if (p_port->in_use == 0 || p_port->rfc.p_mcb != p_mcb || p_port->rfc.state != 4)
                continue;
        }

        event = port_flow_control_user(p_port);
        event |= port_rfc_send_tx_data(p_port);
        event &= p_port->ev_mask;

        if (p_port->p_callback != NULL && event != 0)
            p_port->p_callback(event, p_port->inx);

        if (dlci != 0)
            break;
    }
}

/* ========================================================================
 * port_rfc_send_tx_data - drain queued buffers onto the multiplexer while
 * neither side has flow off.  Returns the PORT_EV_* mask of what happened.
 *
 * ======================================================================== */
u32 port_rfc_send_tx_data(tPORT* p_port)
{
    u32 event = 0;
    BT_HDR* p_buf;

    if (p_port->tx.queue_size != 0) {
        while (p_port->tx.peer_fc == 0 && p_port->rfc.p_mcb != NULL &&
               p_port->rfc.p_mcb->peer_ready != 0) {
            p_buf = (BT_HDR*)GKI_dequeue(&p_port->tx.queue);
            if (p_buf != NULL) {
                RFCOMM_TRACE_EVENT0("Sending RFCOMM_DataReq");

                {
                    u32 pending = p_port->tx.queue_size;
                    tRFC_MCB* mcb = p_port->rfc.p_mcb;
                    u8 dlci2 = p_port->dlci;

                    p_port->tx.queue_size = pending - p_buf->len;
                    RFCOMM_DataReq(mcb, dlci2, p_buf);
                }
                event |= 0x4000;

                if (p_port->tx.queue_size == 0) {
                    event |= 0x4;
                    break;
                }
            } else {
                event |= 0x4;
                break;
            }
        }

        event |= port_flow_control_user(p_port);
    }

    return event & p_port->ev_mask;
}

/* ========================================================================
 * port_rfc_closed - tear down the port's RFCOMM side.  While still OPENING
 * with a pending open, silently release back to the idle state instead of
 * reporting an error to the application.
 *
 * ======================================================================== */
void port_rfc_closed(tPORT* p_port, u8 result)
{
    u32 event = 0;
    tRFC_MCB* p_mcb = p_port->rfc.p_mcb;
    u8 state = p_port->state;

    if (state == PORT_STATE_OPENING && p_port->is_server != 0) {
        RFCOMM_TRACE_EVENT0("port_rfc_closed in OPENING state ignored");
        rfc_port_timer_stop(p_port);
        p_port->rfc.state = 0;
        if (p_mcb != NULL) {
            p_mcb->port_inx[p_port->dlci] = 0;
            rfc_check_mcb_active(p_mcb);
            p_port->rfc.p_mcb = NULL;
        }
        p_port->dlci &= 0xFE;
        return;
    }

    if (state != PORT_STATE_CLOSING && state != PORT_STATE_CLOSED) {
        u8 prev;
        p_port->line_status |= 0x10;
        prev = p_port->peer_ctrl.modem_signal;
        p_port->peer_ctrl.modem_signal = prev & ~0x0B;
        event = port_get_signal_changes(p_port, prev, p_port->peer_ctrl.modem_signal);
        if (p_port->ev_mask & PORT_EV_CONNECT_ERR)
            event |= PORT_EV_CONNECT_ERR;
    }

    RFCOMM_TRACE_EVENT2("port_rfc_closed state:%d sending events:%x", p_port->state, event);

    if (p_port->p_callback != NULL && event != 0)
        p_port->p_callback(event, p_port->inx);

    if (p_port->p_mgmt_callback != NULL)
        p_port->p_mgmt_callback(result, p_port->inx);

    p_port->rfc.state = 0;
    port_release_port(p_port);
}
