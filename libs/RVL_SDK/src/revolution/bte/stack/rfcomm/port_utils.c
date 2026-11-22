// Decompiled: libs/RVL_SDK/src/revolution/bte/stack/rfcomm/port_utils.c
// High-level C reconstruction of port_utils functions.
// RVL_SDK BTE RFCOMM stack - port utility functions.

#include <harness_catalog.h>
#include <string.h>

#include "revolution/BTE/stack/rfcomm/rfc_int.h"
#include "revolution/BTE/stack/btm/btm_int.h"
#include "revolution/BTE/include/bt_trace.h"

/* ========================================================================
 * port_select_mtu - select MTU and compute credit parameters
 * ======================================================================== */
void port_select_mtu(tPORT* port)
{
    u16 max_packet;
    u16 mtu_val;

    if (port->mtu == 0) {
        max_packet = btm_get_max_packet_size(port->bd_addr);

        if (max_packet == 0) {
            RFCOMM_TRACE_WARNING0("port_select_mtu bad packet size");
            port->mtu = 0x7F;
            goto done;
        }

        if (max_packet <= 0x69F) {
            port->mtu = (u16)((0x69F / max_packet) * max_packet - 0xA);
            RFCOMM_TRACE_DEBUG1("port_select_mtu selected %d based on connection speed", port->mtu);
            goto done;
        }

        port->mtu = 0x695;
        RFCOMM_TRACE_DEBUG1("port_select_mtu selected %d based on l2cap PDU size", 0x695);
    } else {
        RFCOMM_TRACE_DEBUG1("port_select_mtu application selected %d", port->mtu);
    }

done:
    mtu_val = port->mtu;
    port->credit_rx_max = 0x1F40 / mtu_val;
    port->credit_rx_low = 0x1388 / mtu_val;
    port->rx_buf_critical = 0x2EE0 / mtu_val;

    RFCOMM_TRACE_DEBUG3("port_select_mtu credit_rx_max %d, credit_rx_low %d, rx_buf_critical %d",
                        port->credit_rx_max, port->credit_rx_low, port->rx_buf_critical);
}

/* ========================================================================
 * port_release_port - release and clean up a port structure
 * ======================================================================== */
void port_release_port(tPORT* port)
{
    BT_HDR* buf;

    /* Drain rx queue (at port+0x30) */
    while ((buf = (BT_HDR*)GKI_dequeue(&port->rx.queue)) != NULL) {
        GKI_freebuf(buf);
    }
    port->rx.queue_size = 0;

    /* Drain tx queue (at port+0x18) */
    while ((buf = (BT_HDR*)GKI_dequeue(&port->tx.queue)) != NULL) {
        GKI_freebuf(buf);
    }

    port->tx.queue_size = 0;
    port->state = 0;

    if (port->rfc.state != 0) {
        return;
    }

    RFCOMM_TRACE_DEBUG0("rfc_port_closed DONE");

    if (port->rfc.p_mcb != NULL) {
        port->rfc.p_mcb->port_inx[port->dlci] = 0;
        rfc_check_mcb_active(port->rfc.p_mcb);
    }

    rfc_port_timer_stop(port);
    memset(port, 0, sizeof(tPORT));
}

/* ========================================================================
 * port_find_mcb_dlci_port - find port given mcb + dlci
 * ======================================================================== */
tPORT* port_find_mcb_dlci_port(tRFC_MCB* mcb, u8 dlci)
{
    u8 port_idx_plus1;

    if (mcb == NULL) {
        return NULL;
    }
    if (dlci > RFCOMM_MAX_DLCI) {
        return NULL;
    }

    port_idx_plus1 = mcb->port_inx[dlci];
    if (port_idx_plus1 == 0) {
        return NULL;
    }

    /* Compute port address: rfc_cb + (port_idx_plus1-1)*0xA4 + 0x68 */
    return &rfc_cb.port.port[port_idx_plus1 - 1];
}

/* ========================================================================
 * port_find_dlci_port - find a port by dlci (without mcb context)
 * ======================================================================== */
tPORT* port_find_dlci_port(u8 dlci)
{
    u16 index;
    tPORT* p_port;
    tRFC_MCB* p_mcb;

    for (index = 0; index < MAX_RFC_PORTS; index++) {
        p_port = &rfc_cb.port.port[index];

        if (p_port->in_use) {
            p_mcb = p_port->rfc.p_mcb;

            if (p_mcb == NULL) {
                if (p_port->dlci == dlci) {
                    return p_port;
                }
                if ((dlci & 1) && (p_port->dlci == dlci - 1)) {
                    u8 new_dlci = p_port->dlci + 1;

                    p_port->dlci = new_dlci;
                    return p_port;
                }
            }
        }
    }

    return NULL;
}

/* ========================================================================
 * port_flow_control_user - determine user-side flow control state
 * Returns event code: 0x30000 if flow==0 (unblocked), 0x10000 if flow==1
 * (blocked), 0 if unchanged.
 * ======================================================================== */
u32 port_flow_control_user(tPORT* port)
{
    u8 flow = 0;

    if (port->tx.peer_fc != 0
        || port->rfc.p_mcb == NULL
        || port->rfc.p_mcb->peer_ready == 0
        || port->tx.queue_size > 0x1F40
        || port->tx.queue.count > 0x10)
    {
        flow = 1;
    }

    if (port->tx.user_fc == flow)
        return 0;

    port->tx.user_fc = flow;
    if (flow != 0)
        return 0x10000;
    return 0x30000;
}

/* ========================================================================
 * port_get_signal_changes - compute changed modem signal bits
 * r3=port, r4=prev_signals, r5=signals  (from ASM: xor r4, r5, r4)
 * ======================================================================== */
u32 port_get_signal_changes(tPORT* port, u8 prev_signals, u8 signals)
{
    u8 changed = signals ^ prev_signals;
    u32 result = 0;

    if (changed & 0x01) {
        result |= 0x10;
        if (signals & 0x01)
            result |= 0x800;
    }

    if (changed & 0x02) {
        result |= 0x08;
        if (signals & 0x02)
            result |= 0x400;
    }

    if (changed & 0x04) {
        result |= 0x100;
    }

    if (changed & 0x08) {
        result |= 0x20;
        if (signals & 0x08)
            result |= 0x1000;
    }

    return port->ev_mask & result;
}

/* ========================================================================
 * port_flow_control_peer - handle peer-side flow control
 * ======================================================================== */
void port_flow_control_peer(tPORT* port, u8 enable, u16 count)
{
    if (port->rfc.p_mcb == NULL)
        return;

    if (port->rfc.p_mcb->flow == 2) {
        if (enable) {
            if (count > port->credit_rx)
                port->credit_rx = 0;
            else
                port->credit_rx -= count;

            if (port->credit_rx <= port->credit_rx_low &&
                port->rx.user_fc == 0 && port->credit_rx_max > port->credit_rx) {
                rfc_send_credit(port->rfc.p_mcb, port->dlci,
                                port->credit_rx_max - port->credit_rx);

                port->credit_rx = port->credit_rx_max;

                port->rx.peer_fc = 0;
            }
        } else {
            if (port->p_data_callback != 0)
                port->rx.peer_fc = 1;
            else if (port->rx.queue.count >= port->credit_rx_max)
                port->rx.peer_fc = 1;
        }
    } else if (enable) {
        if (port->rx.peer_fc != 0 && port->rx.queue_size < 0x1388 &&
            port->rx.queue.count < 8) {
            port->rx.peer_fc = 0;

            if (port->rx.user_fc == 0)
                RFCOMM_FlowReq(port->rfc.p_mcb, port->dlci, 1);
        }
    } else if (port->p_data_callback != 0) {
        port->rx.peer_fc = 1;
        RFCOMM_FlowReq(port->rfc.p_mcb, port->dlci, 0);
    } else if ((port->rx.queue_size > 0x1F40 ||
                port->rx.queue.count > 0x10) &&
               port->rx.peer_fc == 0) {
        RFCOMM_TRACE_EVENT0("PORT_DataInd Data reached HW. Sending FC set.");

        port->rx.peer_fc = 1;
        RFCOMM_FlowReq(port->rfc.p_mcb, port->dlci, 0);
    }
}
