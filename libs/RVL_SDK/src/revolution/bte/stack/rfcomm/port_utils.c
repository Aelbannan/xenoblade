// Decompiled: libs/RVL_SDK/src/revolution/bte/stack/rfcomm/port_utils.c
// High-level C reconstruction of port_utils functions.
// RVL_SDK BTE RFCOMM stack - port utility functions.

#include <harness_catalog.h>
#include <string.h>

/* ========================================================================
 * Forward type declarations
 * ======================================================================== */
typedef struct BT_HDR BT_HDR;

/* GKI buffer queue - 12 bytes on MWCC (padded to 4-byte alignment) */
typedef struct {
    void* p_first;
    void* p_last;
    unsigned short count;
} BUFFER_Q;

struct RfcMuxChannel;
struct RfcPort;

/* ========================================================================
 * Structure definitions - field positions derived from retail ASM offsets
 * ======================================================================== */

/*
 * RfcPort (tRFC_PORT) - size 0xA4 = 164 bytes
 * Stored at rfc_cb + 0x68 + i * 0xA4 for port i.
 */
typedef struct RfcPort {
    u8   dlci;              /* 0x00 */
    u8   in_use;            /* 0x01 - non-zero if this port slot is allocated */
    u8   state;             /* 0x02 - port state */
    u8   pad_03[3];         /* 0x03-0x05 */
    u8   bd_addr[6];        /* 0x06-0x0B - remote BD address */
    u8   pad_0c;            /* 0x0C */
    u8   dlci2;             /* 0x0D - DLCI */
    u8   pad_0e[4];         /* 0x0E-0x11 */
    u16  mtu;               /* 0x12 - max transmission unit */
    u8   pad_14[4];         /* 0x14-0x17 */
    BUFFER_Q tx_queue;      /* 0x18-0x23 (padded to 12 bytes) */
    u8   field_0x24;        /* 0x24 - in-use state for flow control */
    u8   flow_state;        /* 0x25 - flow control state (0 or 1) */
    u8   pad_26[2];         /* 0x26-0x27 */
    u32  field_0x28;        /* 0x28 - tx queue count / pending bytes */
    u8   pad_2c[4];         /* 0x2C-0x2F */
    BUFFER_Q rx_queue;      /* 0x30-0x3B (padded to 12 bytes) */
    u8   local_fc;          /* 0x3C - local flow control flag */
    u8   field_0x3D;        /* 0x3D - pending_credit flag */
    u8   pad_3e[2];         /* 0x3E-0x3F */
    u32  field_0x40;        /* 0x40 - queued bytes count */
    u8   pad_44[0x24];      /* 0x44-0x67 */
    u8   field_0x68;        /* 0x68 - rfc_port_closed flag */
    u8   pad_69[3];         /* 0x69-0x6B */
    struct RfcMuxChannel* mcb; /* 0x6C - parent multiplexer channel */
    u8   pad_70[0x18];      /* 0x70-0x87 */
    u32  signal_bits;       /* 0x88 - modem signal bits */
    u8   pad_8c[8];         /* 0x8C-0x93 */
    u32  field_0x94;        /* 0x94 - user-facing flow indicator */
    u8   pad_98[2];         /* 0x98-0x99 */
    u16  credit_rx;         /* 0x9A - received credits remaining */
    u16  credit_rx_max;     /* 0x9C - max credits to send */
    u16  credit_rx_low;     /* 0x9E - low credit threshold */
    u16  rx_buf_critical;   /* 0xA0 - rx buffer critical threshold */
} RfcPort;                  /* total: 0xA4 */

/*
 * RfcMuxChannel - RFCOMM multiplexer channel (per-connection).
 */
typedef struct RfcMuxChannel {
    u8   pad_00[0x24];      /* 0x00-0x23 */
    u8   dlci_port_map[0x3E]; /* 0x24-0x61 - DLCI => port index+1 */
    u8   bd_addr[6];        /* 0x62-0x67 */
    u16  lcid;              /* 0x68-0x69 */
    u16  field_0x6A;        /* 0x6A-0x6B */
    u8   state;             /* 0x6C */
    u8   initiator;         /* 0x6D */
    u8   field_0x6E;        /* 0x6E */
    u8   field_0x6F;        /* 0x6F */
    u8   field_0x70;        /* 0x70 */
    u8   field_0x71;        /* 0x71 - user flow active? */
    u8   field_0x72;        /* 0x72 - credit based flag (2=credit, else non-credit) */
} RfcMuxChannel;

/*
 * RfcControlBlock - top-level RFCOMM control block.
 */
typedef struct RfcControlBlock {
    u8   pad_00[0x68];      /* 0x00-0x67 */
    RfcPort port[5];        /* 0x68-0x39B - 5 ports */
    u8   pad_39C[0x78];     /* 0x39C-0x413 */
    u8   trace_level;       /* 0x414 */
} RfcControlBlock;

/* ========================================================================
 * Port array access macro
 * Port index i is at rfc_cb + 0x68 + i * 0xA4
 * ======================================================================== */
#define port_ptr(idx)  ((RfcPort*)((u8*)&rfc_cb + (idx) * 0xA4 + 0x68))

/* ========================================================================
 * External globals and functions
 * ======================================================================== */
extern RfcControlBlock rfc_cb;

extern unsigned short btm_get_max_packet_size(void* addr);

extern void LogMsg_0(unsigned int level, const char* message);
extern void LogMsg_1(unsigned int level, const char* message, unsigned int p1);
extern void LogMsg_3(unsigned int level, const char* message, unsigned int p1, unsigned int p2, unsigned int p3);

extern void* GKI_dequeue(BUFFER_Q* queue);
extern void GKI_freebuf(void* buffer);

extern void rfc_send_credit(struct RfcMuxChannel* mcb, u8 dlci, u8 credits);
extern void rfc_check_mcb_active(struct RfcMuxChannel* mcb);
extern void rfc_port_timer_stop(RfcPort* port);
extern void RFCOMM_FlowReq(struct RfcMuxChannel* mcb, u8 dlci, u8 enable);

/* ========================================================================
 * port_select_mtu - select MTU and compute credit parameters
 * ======================================================================== */
void port_select_mtu(RfcPort* port)
{
    u16 max_packet;
    u16 mtu_val;

    if (port->mtu != 0) {
        if (rfc_cb.trace_level >= 5) {
            LogMsg_1(0x90004, "port_select_mtu application selected %d", port->mtu);
        }
        goto done;
    }

    max_packet = btm_get_max_packet_size(port->bd_addr);
    max_packet = max_packet & 0xFFFF;

    if (max_packet == 0) {
        if (rfc_cb.trace_level >= 2) {
            LogMsg_0(0x90001, "port_select_mtu bad packet size");
        }
        port->mtu = 0x7F;
        goto done;
    }

    if (max_packet <= 0x69F) {
        port->mtu = (u16)((0x69F / max_packet) * max_packet - 0xA);
        if (rfc_cb.trace_level >= 5) {
            LogMsg_1(0x90004, "port_select_mtu selected %d based on connection speed", port->mtu);
        }
        goto done;
    }

    port->mtu = 0x695;
    if (rfc_cb.trace_level >= 5) {
        LogMsg_1(0x90004, "port_select_mtu selected %d based on l2cap PDU size", 0x695);
    }

done:
    mtu_val = port->mtu;
    port->credit_rx_max = 0x1F40 / mtu_val;
    port->credit_rx_low = 0x1388 / mtu_val;
    port->rx_buf_critical = 0x2EE0 / mtu_val;

    if (rfc_cb.trace_level >= 5) {
        LogMsg_3(0x90004,
                 "port_select_mtu credit_rx_max %d, credit_rx_low %d, rx_buf_critical %d",
                 port->credit_rx_max, port->credit_rx_low, port->rx_buf_critical);
    }
}

/* ========================================================================
 * port_release_port - release and clean up a port structure
 * ======================================================================== */
void port_release_port(RfcPort* port)
{
    BT_HDR* buf;

    /* Drain rx queue (at port+0x30) */
    while ((buf = (BT_HDR*)GKI_dequeue(&port->rx_queue)) != NULL) {
        GKI_freebuf(buf);
    }
    port->field_0x40 = 0;

    /* Drain tx queue (at port+0x18) */
    while ((buf = (BT_HDR*)GKI_dequeue(&port->tx_queue)) != NULL) {
        GKI_freebuf(buf);
    }

    port->field_0x28 = 0;
    port->state = 0;

    if (port->field_0x68 != 0) {
        return;
    }

    if (rfc_cb.trace_level >= 5) {
        LogMsg_0(0x90004, "rfc_port_closed DONE");
    }

    if (port->mcb != NULL) {
        u8* mcb_bytes = (u8*)port->mcb;
        mcb_bytes[0x24 + port->dlci2] = 0;
        rfc_check_mcb_active(port->mcb);
    }

    rfc_port_timer_stop(port);
    memset(port, 0, sizeof(RfcPort));
}

/* ========================================================================
 * port_find_mcb_dlci_port - find port given mcb + dlci
 * ======================================================================== */
RfcPort* port_find_mcb_dlci_port(RfcMuxChannel* mcb, u8 dlci)
{
    u8 port_idx_plus1;

    if (mcb == NULL) {
        return NULL;
    }
    if (dlci > 0x3D) {
        return NULL;
    }

    port_idx_plus1 = mcb->dlci_port_map[dlci];
    if (port_idx_plus1 == 0) {
        return NULL;
    }

    /* Compute port address: rfc_cb + (port_idx_plus1-1)*0xA4 + 0x68 */
    return port_ptr(port_idx_plus1 - 1);
}

/* ========================================================================
 * port_find_dlci_port - find a port by dlci (without mcb context)
 * ======================================================================== */
RfcPort* port_find_dlci_port(u8 dlci)
{
    u16 index;
    RfcPort* p_port;
    struct RfcMuxChannel* p_mcb;

    for (index = 0; index < 5; index++) {
        p_port = &rfc_cb.port[index];

        if (p_port->in_use) {
            p_mcb = p_port->mcb;

            if (p_mcb == NULL) {
                if (p_port->dlci2 == dlci) {
                    return p_port;
                }
                if ((dlci & 1) && (p_port->dlci2 == dlci - 1)) {
                    u8 new_dlci = p_port->dlci2 + 1;

                    p_port->dlci2 = new_dlci;
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
u32 port_flow_control_user(RfcPort* port)
{
    u8 flow = 0;

    if (port->field_0x24 != 0
        || port->mcb == NULL
        || port->mcb->field_0x71 == 0
        || port->field_0x28 > 0x1F40
        || port->tx_queue.count > 0x10)
    {
        flow = 1;
    }

    if (port->flow_state == flow)
        return 0;

    port->flow_state = flow;
    if (flow != 0)
        return 0x10000;
    return 0x30000;
}

/* ========================================================================
 * port_get_signal_changes - compute changed modem signal bits
 * r3=port, r4=prev_signals, r5=signals  (from ASM: xor r4, r5, r4)
 * ======================================================================== */
u32 port_get_signal_changes(RfcPort* port, u8 prev_signals, u8 signals)
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

    return port->signal_bits & result;
}

/* ========================================================================
 * port_flow_control_peer - handle peer-side flow control
 * ======================================================================== */
void port_flow_control_peer(RfcPort* port, u8 enable, u16 credits)
{
    if (port->mcb == NULL)
        return;

    if (port->mcb->field_0x72 == 2) {
        if (enable) {
            u16 old_rx = port->credit_rx;
            if (credits > old_rx) {
                port->credit_rx = 0;
            } else {
                port->credit_rx = old_rx - credits;
            }

            if (port->credit_rx <= port->credit_rx_low
                && port->field_0x3D == 0
                && port->credit_rx_max > port->credit_rx)
            {
                rfc_send_credit(port->mcb, port->dlci2,
                                (u8)(port->credit_rx_max - port->credit_rx));
                port->local_fc = 0;
                port->credit_rx = port->credit_rx_max;
            }
        } else {
            if (port->field_0x94 != 0) {
                port->local_fc = 1;
            } else if (port->tx_queue.count >= port->credit_rx_max) {
                port->local_fc = 1;
            }
        }
    } else {
        if (enable) {
            if (port->local_fc != 0
                && port->field_0x40 < 0x1388
                && port->tx_queue.count < 8
                && port->field_0x3D == 0)
            {
                port->local_fc = 0;
                RFCOMM_FlowReq(port->mcb, port->dlci2, 1);
            }
        } else {
            if (port->field_0x94 != 0) {
                port->local_fc = 1;
                RFCOMM_FlowReq(port->mcb, port->dlci2, 0);
            } else if (port->field_0x40 > 0x1F40 || port->tx_queue.count > 0x10) {
                if (port->local_fc == 0) {
                    if (rfc_cb.trace_level >= 4) {
                        LogMsg_0(0x90003,
                                 "PORT_DataInd Data reached HW. Sending FC set.");
                    }
                    port->local_fc = 1;
                    RFCOMM_FlowReq(port->mcb, port->dlci2, 0);
                }
            }
        }
    }
}
