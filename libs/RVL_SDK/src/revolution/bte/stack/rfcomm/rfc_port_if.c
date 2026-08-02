// Decompiled: libs/RVL_SDK/src/revolution/bte/stack/rfcomm/rfc_port_if.c
// High-level C reconstruction of the RFCOMM port-interface layer.
// Field offsets recovered from the retail assembly and cross-checked against
// the sibling decomp units port_rfc.c / port_utils.c / rfc_port_fsm.c.

#include <harness_catalog.h>

typedef struct BT_HDR BT_HDR;
typedef struct RfcMuxChannel RfcMuxChannel;
typedef struct RfcPort RfcPort;

/* GKI buffer header. */
struct BT_HDR {
    u16 event;
    u16 len;
    u16 offset;
    u16 layer_specific;
};

/* Modem Status Command parameters (rfc_send_msc). */
typedef struct {
    u8 modem_signal;   /* 0x00 */
    u8 break_signal;   /* 0x01 */
    u8 pad_02[2];      /* 0x02 */
    u8 fc;             /* 0x04 */
} tPORT_CTRL;

/*
 * RfcPort - port object (0xA4 bytes, stored at rfc_cb + 0x68 + i * 0xA4).
 */
struct RfcPort {
    u8   dlci;            /* 0x00 - dlci reported to upper layers */
    u8   in_use;          /* 0x01 */
    u8   state;           /* 0x02 - port state */
    u8   pad_03[3];       /* 0x03-0x05 */
    u8   bd_addr[6];      /* 0x06-0x0B - remote BD address */
    u8   pad_0c;          /* 0x0C */
    u8   dlci2;           /* 0x0D - dlci used on the RFCOMM link */
    u8   field_0e;        /* 0x0E - last error code reported to upper layer */
    u8   line_status;     /* 0x0F */
    u8   pad_10[2];       /* 0x10-0x11 */
    u16  mtu;             /* 0x12 - max transmission unit */
    u16  peer_mtu;        /* 0x14 */
    u8   pad_16[0x44];    /* 0x16-0x59 */
    tPORT_CTRL port_ctrl; /* 0x5A-0x5E - control data sent with MSC */
    u8   modem_signal;    /* 0x5F */
    u8   pad_60[4];       /* 0x60-0x63 */
    u8   ctrl_flags;      /* 0x64 - MSC control already sent flag */
    u8   pad_65[3];       /* 0x65-0x67 */
    u8   rfc_state;       /* 0x68 - port state-machine state */
    u8   rfc_flags;       /* 0x69 - expected-response bits */
    u8   pad_6a[2];       /* 0x6A-0x6B */
    RfcMuxChannel* p_mcb; /* 0x6C - parent multiplexer channel */
    u8   pad_70[0x28];    /* 0x70-0x97 */
    u16  field_98;        /* 0x98 */
    u16  credit_rx;       /* 0x9A - received credits remaining */
    u16  credit_rx_max;   /* 0x9C - max credits to send */
    u16  credit_rx_low;   /* 0x9E */
    u16  rx_buf_critical; /* 0xA0 */
};                        /* 0xA4 */

/*
 * RfcMuxChannel - RFCOMM multiplexer channel (per-connection).
 */
struct RfcMuxChannel {
    u8   pad_00[0x24];    /* 0x00-0x23 */
    u8   port_inx[0x3E];  /* 0x24-0x61 - DLCI => port index+1 map */
    u8   bd_addr[6];      /* 0x62-0x67 */
    u16  lcid;            /* 0x68 */
    u16  field_6a;        /* 0x6A */
    u8   state;           /* 0x6C - multiplexer state-machine state */
    u8   initiator;       /* 0x6D */
    u8   field_6e;        /* 0x6E */
    u8   field_6f;        /* 0x6F */
    u8   field_70;        /* 0x70 */
    u8   field_71;        /* 0x71 */
    u8   field_72;        /* 0x72 - credit based flow flag (2=credit) */
};

/* Port states (p_port->state). */
#define PORT_STATE_OPENED       2

/* Port FSM states (p_port->rfc_state). */
#define RFC_STATE_OPENED        4

/* Multiplexer states (p_mcb->state). */
#define RFC_MX_STATE_CONNECTED  5

/* Port FSM events (rfc_port_sm_execute). */
#define PORT_EV_DLC_EST_REQ     9
#define PORT_EV_DLC_EST_RSP     11
#define PORT_EV_DLC_REL_REQ     12
#define PORT_EV_DATA_REQ        14

/* Expected-response bits in p_port->rfc_flags. */
#define RFC_FLAG_EXPECT_PN      0x01
#define RFC_FLAG_EXPECT_RPN     0x02
#define RFC_FLAG_EXPECT_RPN2    0x04
#define RFC_FLAG_EXPECT_MSC     0x08
#define RFC_FLAG_EXPECT_RLS     0x10

/* Port T1 timer timeout. */
#define RFC_PORT_T1_TIMEOUT     0x3C

extern RfcPort* port_find_mcb_dlci_port(RfcMuxChannel* mcb, u8 dlci);
extern void rfc_port_sm_execute(RfcPort* port, u8 event, BT_HDR* buffer);
extern void rfc_port_timer_start(RfcPort* port, u16 tout);
extern void rfc_send_pn(RfcMuxChannel* p_mcb, u8 dlci, u8 is_command, u16 mtu, u8 cl, u8 k);
extern void rfc_send_rpn(RfcMuxChannel* p_mcb, u8 dlci, u8 is_command, void* p_pars, u16 mask);
extern void rfc_send_msc(RfcMuxChannel* p_mcb, u8 dlci, u8 is_command, void* p_pars);
extern void rfc_send_rls(RfcMuxChannel* p_mcb, u8 dlci, u8 is_command, u8 status);
extern void PORT_DlcEstablishCnf(RfcMuxChannel* mcb, u8 dlci, u16 mtu, u16 result);
extern void PORT_DlcReleaseInd(RfcMuxChannel* mcb, u8 dlci);
extern void PORT_PortNegCnf(RfcMuxChannel* mcb, u8 dlci, u16 mtu, u16 result);

void RFCOMM_StartRsp(unsigned short param1, unsigned short param2) {
    unsigned short local = param2;
    rfc_mx_sm_execute(param1, 7, &local);
}

void RFCOMM_DlcEstablishReq(RfcMuxChannel* mcb, u8 dlci, u16 mtu) {
    RfcPort* port = port_find_mcb_dlci_port(mcb, dlci);

    if (mcb->state != RFC_MX_STATE_CONNECTED) {
        PORT_DlcEstablishCnf(mcb, dlci, 0, 1);
    } else {
        rfc_port_sm_execute(port, PORT_EV_DLC_EST_REQ, 0);
    }
}

void RFCOMM_DlcEstablishRsp(RfcMuxChannel* mcb, u8 dlci, u16 mtu, u16 result) {
    RfcPort* port = port_find_mcb_dlci_port(mcb, dlci);

    if (mcb->state != RFC_MX_STATE_CONNECTED && result == 0) {
        PORT_DlcReleaseInd(mcb, dlci);
    } else {
        rfc_port_sm_execute(port, PORT_EV_DLC_EST_RSP, (BT_HDR*)&result);
    }
}

void RFCOMM_ParNegReq(RfcMuxChannel* mcb, u8 dlci, u16 mtu) {
    RfcPort* port = port_find_mcb_dlci_port(mcb, dlci);
    u8 flow;
    u8 cl;
    u8 k;

    if (mcb->state != RFC_MX_STATE_CONNECTED) {
        port->field_0e = 0x0D;
        return;
    }

    flow = (mcb->field_72 == 0) ? 2 : mcb->field_72;
    if (flow == 2) {
        cl = 0xF0;
        k = (port->credit_rx_max >= 7) ? 7 : port->credit_rx_max;
        port->credit_rx = k;
    } else {
        cl = 0;
        k = 0;
    }

    port->rfc_flags |= RFC_FLAG_EXPECT_PN;
    rfc_send_pn(mcb, dlci, 1, mtu, cl, k);
    rfc_port_timer_start(port, RFC_PORT_T1_TIMEOUT);
}

void RFCOMM_ParNegRsp(RfcMuxChannel* mcb, u8 dlci, u16 mtu, u8 cl, u8 k) {
    if (mcb->state != RFC_MX_STATE_CONNECTED)
        return;

    rfc_send_pn(mcb, dlci, 0, mtu, cl, k);
}

void RFCOMM_PortNegReq(RfcMuxChannel* mcb, u8 dlci, void* p_port_ctrl) {
    RfcPort* port = port_find_mcb_dlci_port(mcb, dlci);

    if (mcb->state != RFC_MX_STATE_CONNECTED) {
        PORT_PortNegCnf(mcb, dlci, 0, 1);
        return;
    }

    if (p_port_ctrl == 0)
        port->rfc_flags |= RFC_FLAG_EXPECT_RPN;
    else
        port->rfc_flags |= RFC_FLAG_EXPECT_RPN2;

    rfc_send_rpn(mcb, dlci, 1, p_port_ctrl, 0x3F7F);
    rfc_port_timer_start(port, RFC_PORT_T1_TIMEOUT);
}

void RFCOMM_PortNegRsp(RfcMuxChannel* mcb, u8 dlci, void* p_port_ctrl, u16 request) {
    if (mcb->state != RFC_MX_STATE_CONNECTED)
        return;

    rfc_send_rpn(mcb, dlci, 0, p_port_ctrl, request);
}

void RFCOMM_ControlReq(RfcMuxChannel* mcb, u8 dlci, void* p_port_ctrl) {
    RfcPort* port = port_find_mcb_dlci_port(mcb, dlci);

    if (port->state != PORT_STATE_OPENED || port->rfc_state != RFC_STATE_OPENED)
        return;

    port->ctrl_flags |= 1;
    port->rfc_flags |= RFC_FLAG_EXPECT_MSC;
    rfc_send_msc(mcb, dlci, 1, p_port_ctrl);
    rfc_port_timer_start(port, RFC_PORT_T1_TIMEOUT);
}

void RFCOMM_FlowReq(RfcMuxChannel* mcb, u8 dlci, u8 enable) {
    tPORT_CTRL* p_ctrl;
    RfcPort* port = port_find_mcb_dlci_port(mcb, dlci);

    if (port->state != PORT_STATE_OPENED || port->rfc_state != RFC_STATE_OPENED)
        return;

    p_ctrl = &port->port_ctrl;
    p_ctrl->fc = (enable == 0);
    port->rfc_flags |= RFC_FLAG_EXPECT_MSC;
    rfc_send_msc(mcb, dlci, 1, p_ctrl);
    rfc_port_timer_start(port, RFC_PORT_T1_TIMEOUT);
}

void RFCOMM_LineStatusReq(RfcMuxChannel* mcb, u8 dlci, u8 status) {
    RfcPort* port = port_find_mcb_dlci_port(mcb, dlci);

    if (port->state != PORT_STATE_OPENED || port->rfc_state != RFC_STATE_OPENED)
        return;

    port->rfc_flags |= RFC_FLAG_EXPECT_RLS;
    rfc_send_rls(mcb, dlci, 1, status);
    rfc_port_timer_start(port, RFC_PORT_T1_TIMEOUT);
}

void RFCOMM_DlcReleaseReq(RfcMuxChannel* mcb, u8 dlci) {
    rfc_port_sm_execute(port_find_mcb_dlci_port(mcb, dlci), PORT_EV_DLC_REL_REQ, 0);
}

void RFCOMM_DataReq(RfcMuxChannel* mcb, u8 dlci, BT_HDR* p_buf) {
    rfc_port_sm_execute(port_find_mcb_dlci_port(mcb, dlci), PORT_EV_DATA_REQ, p_buf);
}
