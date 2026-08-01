// High-level C reconstruction of RVL_SDK/src/revolution/bte/stack/rfcomm/rfc_port_fsm
// RFCOMM port state machine (Broadcom BTE RFCOMM stack).
// Field offsets recovered from retail assembly and cross-checked against the
// already-decompiled sibling units port_rfc.c / port_utils.c / rfc_ts_frames.c.

#include <harness_catalog.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/*  Types                                                              */
/* ------------------------------------------------------------------ */
typedef struct RfcMuxChannel tRFC_MCB;
typedef struct RfcPort tPORT;

/* GKI buffer header. */
typedef struct {
    u16 event;            /* 0x00 */
    u16 len;              /* 0x02 */
    u16 offset;           /* 0x04 */
    u16 layer_specific;   /* 0x06 */
} BT_HDR;

/* Received TS 07.10 MX frame (union payload at 0x08). */
typedef struct {
    u8  dlci;             /* 0x00 */
    u8  type;             /* 0x01 */
    u8  cr;               /* 0x02 */
    u8  ea;               /* 0x03 */
    u8  pf;               /* 0x04 */
    u8  credit;           /* 0x05 */
    u8  pad_06[2];
    union {
        struct {
            u8  dlci;        /* 0x08 */
            u8  frame_type;  /* 0x09 */
            u8  conv_layer;  /* 0x0A */
            u8  priority;    /* 0x0B */
            u8  t1;          /* 0x0C */
            u8  pad_0d;
            u16 mtu;         /* 0x0E */
            u8  n2;          /* 0x10 */
            u8  k;           /* 0x11 */
        } pn;
        struct {
            u8  dlci;          /* 0x08 */
            u8  signals;       /* 0x09 */
            u8  break_present; /* 0x0A */
            u8  break_duration;/* 0x0B */
        } msc;
        struct {
            u8  dlci;          /* 0x08 */
            u8  is_request;    /* 0x09 */
            u8  baud_rate;     /* 0x0A */
            u8  byte_size;     /* 0x0B */
            u8  stop_bits;     /* 0x0C */
            u8  parity;        /* 0x0D */
            u8  parity_type;   /* 0x0E */
            u8  fc_type;       /* 0x0F */
            u8  xon_char;      /* 0x10 */
            u8  xoff_char;     /* 0x11 */
            u16 param_mask;    /* 0x12 */
        } rpn;
        struct {
            u8  dlci;          /* 0x08 */
            u8  line_status;   /* 0x09 */
        } rls;
    } u;
} MX_FRAME;

/* Port negotiation / modem status control block (9 bytes, matches tPORT port_ctrl). */
typedef struct {
    u8 baud_rate;         /* 0x00 */
    u8 byte_size;         /* 0x01 */
    u8 stop_bits;         /* 0x02 */
    u8 parity;            /* 0x03 */
    u8 parity_type;       /* 0x04 */
    u8 fc_type;           /* 0x05 */
    u8 pad_06;            /* 0x06 */
    u8 xon_char;          /* 0x07 */
    u8 xoff_char;         /* 0x08 */
} tPORT_STATE;

/* Local MSC processing block (5 bytes). */
typedef struct {
    u8 fc;                /* 0x00 */
    u8 break_duration;    /* 0x01 */
    u8 disc;              /* 0x02 */
    u8 dtr;               /* 0x03 */
    u8 break_signal;      /* 0x04 */
} tMSC_CTRL;

/* RFCOMM control block (flow @ 0x64, trace level @ 0x414). */
typedef struct {
    u8 pad_00[0x64];
    u8 flow;              /* 0x64 - flow control disabled flag */
    u8 pad_65[0x3AF];
    u8 trace_level;       /* 0x414 */
} tRFC_CB;

/* tRFC_MCB - RFCOMM multiplexer channel. */
struct RfcMuxChannel {
    u8  pad_00[0x62];
    u8  bd_addr[6];         /* 0x62 */
    u16 lcid;               /* 0x68 */
    u16 peer_l2cap_mtu;     /* 0x6A */
    u8  state;              /* 0x6C */
    u8  pad_6d[5];
    u8  flow;               /* 0x72 */
    u8  l2cap_congested;    /* 0x73 */
    u8  is_disc_initiator;  /* 0x74 */
};

/* tPORT - port object (only fields accessed by this TU). */
struct RfcPort {
    u8  pad_00[0x0D];
    u8  dlci;               /* 0x0D */
    u8  pad_0e[0x06];
    u16 peer_mtu;           /* 0x14 */
    u8  pad_16[0x0E];
    u8  field_24;           /* 0x24 - previous break/flow state */
    u8  pad_25[0x18];
    u8  field_3d;           /* 0x3D */
    u8  pad_3e[0x13];
    tPORT_STATE port_ctrl;  /* 0x51 (9 bytes) */
    u8  pad_5a[0x09];
    u8  field_63;           /* 0x63 */
    u8  pad_64[4];
    u8  rfc_state;          /* 0x68 - port state-machine state */
    u8  rfc_flags;          /* 0x69 - expected-response bits */
    u8  pad_6a[2];
    tRFC_MCB *p_mcb;        /* 0x6C */
    u8  pad_70[0x28];
    u16 credit_tx;          /* 0x98 */
    u16 credit_rx;          /* 0x9A */
    u16 credit_rx_max;      /* 0x9C */
};

/* ------------------------------------------------------------------ */
/*  Constants                                                          */
/* ------------------------------------------------------------------ */
/* Port states (p_port->rfc_state). */
#define PORT_STATE_CLOSED               0
#define PORT_STATE_OPENING              1
#define PORT_STATE_ORIG_WAIT_SEC_CHECK  2
#define PORT_STATE_TERM_WAIT_SEC_CHECK  3
#define PORT_STATE_OPENED               4
#define PORT_STATE_DISC_WAIT_UA         5

/* Port state-machine events. */
#define PORT_EV_SABME           0   /* SABME frame received */
#define PORT_EV_UA              1   /* UA frame received */
#define PORT_EV_DM              2   /* DM frame received */
#define PORT_EV_DISC            3   /* DISC frame received */
#define PORT_EV_DATA            4   /* UIH data frame received */
#define PORT_EV_TIMEOUT         5   /* T1 timeout */
#define PORT_EV_DLC_EST_REQ     9   /* local DlcEstablish request */
#define PORT_EV_DLC_EST_RSP     11  /* local DlcEstablish response */
#define PORT_EV_DLC_REL_REQ     12  /* local DlcRelease request */
#define PORT_EV_LCID_DISCONNECTED 13 /* L2CAP channel disconnected */
#define PORT_EV_DATA_REQ        14  /* local data request */
#define PORT_EV_SEC_COMPLETE    15  /* security check complete */

/* Expected-response bits in p_port->rfc_flags (0x69). */
#define RFC_FLAG_EXPECT_PN      0x01
#define RFC_FLAG_EXPECT_RPN     0x02
#define RFC_FLAG_EXPECT_RPN2    0x04
#define RFC_FLAG_EXPECT_MSC     0x08
#define RFC_FLAG_EXPECT_RLS     0x10

/* RFCOMM multiplexer states (p_mcb->state). */
#define RFC_MX_STATE_DISC_WAIT_UA 6

/* Trace helpers. */
#define RFCOMM_TRACE_ERROR0(m) \
    { if (rfc_cb.trace_level >= 1) LogMsg_0(0x90000, (m)); }
#define RFCOMM_TRACE_ERROR2(m, p1, p2) \
    { if (rfc_cb.trace_level >= 1) LogMsg_2(0x90000, (m), (u32)(p1), (u32)(p2)); }
#define RFCOMM_TRACE_WARNING1(m, p1) \
    { if (rfc_cb.trace_level >= 2) LogMsg_1(0x90001, (m), (u32)(p1)); }

/* ------------------------------------------------------------------ */
/*  Externs                                                            */
/* ------------------------------------------------------------------ */
extern tRFC_CB rfc_cb;

extern void LogMsg_0(u32 level, const char *msg);
extern void LogMsg_1(u32 level, const char *msg, u32 p1);
extern void LogMsg_2(u32 level, const char *msg, u32 p1, u32 p2);

extern void GKI_freebuf(void *p_buf);

extern void rfc_send_sabme(tRFC_MCB *p_mcb, u8 dlci);
extern void rfc_send_ua(tRFC_MCB *p_mcb, u8 dlci);
extern void rfc_send_dm(tRFC_MCB *p_mcb, u8 dlci, u8 pf);
extern void rfc_send_disc(tRFC_MCB *p_mcb, u8 dlci);
extern void rfc_send_buf_uih(tRFC_MCB *p_mcb, u8 dlci, BT_HDR *p_buf);
extern void rfc_send_pn(tRFC_MCB *p_mcb, u8 dlci, u8 is_command, u16 mtu, u8 cl, u8 k);
extern void rfc_send_fcon(tRFC_MCB *p_mcb, u8 is_command);
extern void rfc_send_fcoff(tRFC_MCB *p_mcb, u8 is_command);
extern void rfc_send_msc(tRFC_MCB *p_mcb, u8 dlci, u8 is_command, void *p_pars);
extern void rfc_send_rls(tRFC_MCB *p_mcb, u8 dlci, u8 is_command, u8 status);
extern void rfc_send_rpn(tRFC_MCB *p_mcb, u8 dlci, u8 is_command, void *p_pars, u16 mask);

extern void rfc_port_timer_start(tPORT *p_port, u16 tout);
extern void rfc_port_timer_stop(tPORT *p_port);
extern void rfc_port_closed(tPORT *p_port);
extern void rfc_check_send_cmd(tRFC_MCB *p_mcb, void *p_buf);
extern void rfc_dec_credit(tPORT *p_port);
extern void rfc_sec_check_complete(u8 *bd_addr, void *p_ref_data, u8 res);

extern tPORT *port_find_mcb_dlci_port(tRFC_MCB *p_mcb, u8 dlci);

extern void port_rfc_closed(tPORT *p_port, u8 result);

extern void PORT_DlcEstablishInd(tRFC_MCB *p_mcb, u8 dlci, u16 mtu);
extern void PORT_DlcEstablishCnf(tRFC_MCB *p_mcb, u8 dlci, u16 mtu, u16 result);
extern void PORT_ParNegInd(tRFC_MCB *p_mcb, u8 dlci, u16 mtu, u8 cl, u8 k);
extern void PORT_ParNegCnf(tRFC_MCB *p_mcb, u8 dlci, u16 mtu, u8 cl, u8 k);
extern void PORT_PortNegInd(tRFC_MCB *p_mcb, u8 dlci, void *p_ctrl, u16 request);
extern void PORT_PortNegCnf(tRFC_MCB *p_mcb, u8 dlci, void *p_ctrl, u16 result);
extern void PORT_ControlInd(tRFC_MCB *p_mcb, u8 dlci, void *p_ctrl);
extern void PORT_ControlCnf(tRFC_MCB *p_mcb, u8 dlci, void *p_ctrl);
extern void PORT_LineStatusInd(tRFC_MCB *p_mcb, u8 dlci, u8 line_status);
extern void PORT_DlcReleaseInd(tRFC_MCB *p_mcb, u8 dlci);
extern void PORT_DataInd(tRFC_MCB *p_mcb, u8 dlci, BT_HDR *p_buf);
extern void PORT_FlowInd(tRFC_MCB *p_mcb, u8 dlci, u8 fc);
extern void Port_TimeOutCloseMux(tRFC_MCB *p_mcb);

extern void btm_sec_mx_access_request(u8 *bd_addr, u16 psm, u8 is_originator,
                                      u32 mx_proto_id, u32 mx_chan_id,
                                      void (*p_callback)(u8 *, void *, u8),
                                      void *p_ref_data);
extern void btm_sec_abort_access_req(u8 *bd_addr);

/* ------------------------------------------------------------------ */
/*  Forward declarations                                               */
/* ------------------------------------------------------------------ */
void rfc_port_sm_state_closed(tPORT *p_port, u16 event, void *p_data);
void rfc_port_sm_sabme_wait_ua(tPORT *p_port, u16 event, void *p_data);
void rfc_port_sm_term_wait_sec_check(tPORT *p_port, u16 event, void *p_data);
void rfc_port_sm_orig_wait_sec_check(tPORT *p_port, u16 event, void *p_data);
void rfc_port_sm_opened(tPORT *p_port, u16 event, void *p_data);
void rfc_port_sm_disc_wait_ua(tPORT *p_port, u16 event, void *p_data);
void rfc_set_port_state(u8 *p_port_state, MX_FRAME *p_frame);

/* ================================================================== */
/*  rfc_port_sm_execute                                               */
/* ================================================================== */
void rfc_port_sm_execute(tPORT *p_port, u16 event, void *p_data)
{
    if (p_port == NULL) {
        if (rfc_cb.trace_level >= 2) {
            LogMsg_1(0x90001, "NULL port event %d", event);
        }
        return;
    }

    switch (p_port->rfc_state) {
    case PORT_STATE_CLOSED:
        rfc_port_sm_state_closed(p_port, event, p_data);
        break;
    case PORT_STATE_OPENING:
        rfc_port_sm_sabme_wait_ua(p_port, event, p_data);
        break;
    case PORT_STATE_ORIG_WAIT_SEC_CHECK:
        rfc_port_sm_orig_wait_sec_check(p_port, event, p_data);
        break;
    case PORT_STATE_TERM_WAIT_SEC_CHECK:
        rfc_port_sm_term_wait_sec_check(p_port, event, p_data);
        break;
    case PORT_STATE_OPENED:
        rfc_port_sm_opened(p_port, event, p_data);
        break;
    case PORT_STATE_DISC_WAIT_UA:
        rfc_port_sm_disc_wait_ua(p_port, event, p_data);
        break;
    }
}

/* ================================================================== */
/*  rfc_port_sm_state_closed                                          */
/* ================================================================== */
void rfc_port_sm_state_closed(tPORT *p_port, u16 event, void *p_data)
{
    switch (event) {
    case PORT_EV_DLC_EST_REQ:               /* 9 - local connect request */
        p_port->rfc_state = PORT_STATE_ORIG_WAIT_SEC_CHECK;
        btm_sec_mx_access_request(p_port->p_mcb->bd_addr, 3, 1, 3,
                                  (u32)p_port->dlci >> 1, rfc_sec_check_complete, p_port);
        return;
    case PORT_EV_LCID_DISCONNECTED:         /* 13 */
        return;
    case PORT_EV_DATA_REQ:                  /* 14 */
        GKI_freebuf(p_data);
        break;
    case PORT_EV_SABME:                     /* 0 - remote connect request */
        p_port->rfc_state = PORT_STATE_TERM_WAIT_SEC_CHECK;
        btm_sec_mx_access_request(p_port->p_mcb->bd_addr, 3, 0, 3,
                                  (u32)p_port->dlci >> 1, rfc_sec_check_complete, p_port);
        return;
    case PORT_EV_UA:                        /* 1 */
        return;
    case PORT_EV_DM:                        /* 2 */
        rfc_port_closed(p_port);
        return;
    case PORT_EV_DATA:                      /* 4 */
        GKI_freebuf(p_data);
        rfc_send_dm(p_port->p_mcb, p_port->dlci, 0);
        return;
    case PORT_EV_DISC:                      /* 3 */
        rfc_send_dm(p_port->p_mcb, p_port->dlci, 0);
        return;
    case PORT_EV_TIMEOUT:                   /* 5 */
        Port_TimeOutCloseMux(p_port->p_mcb);
        RFCOMM_TRACE_ERROR2("Port error state %d event %d", p_port->rfc_state, event);
        return;
    }
    RFCOMM_TRACE_WARNING1("Port state closed Event ignored %d", event);
}

/* ================================================================== */
/*  rfc_port_sm_sabme_wait_ua                                         */
/* ================================================================== */
void rfc_port_sm_sabme_wait_ua(tPORT *p_port, u16 event, void *p_data)
{
    switch (event) {
    case PORT_EV_DLC_EST_REQ:               /* 9 */
    case PORT_EV_DLC_EST_RSP:               /* 11 */
        RFCOMM_TRACE_ERROR2("Port error state %d event %d", p_port->rfc_state, event);
        return;
    case PORT_EV_DLC_REL_REQ:               /* 12 - local abort */
        rfc_port_timer_start(p_port, 3);
        rfc_send_disc(p_port->p_mcb, p_port->dlci);
        p_port->rfc_flags = 0;
        p_port->rfc_state = PORT_STATE_DISC_WAIT_UA;
        return;
    case PORT_EV_LCID_DISCONNECTED:         /* 13 */
        rfc_port_closed(p_port);
        return;
    case PORT_EV_DATA_REQ:                  /* 14 */
        GKI_freebuf(p_data);
        break;
    case PORT_EV_UA:                        /* 1 - connection established */
        rfc_port_timer_stop(p_port);
        p_port->rfc_state = PORT_STATE_OPENED;
        PORT_DlcEstablishCnf(p_port->p_mcb, p_port->dlci,
                             p_port->p_mcb->peer_l2cap_mtu, 0);
        return;
    case PORT_EV_DM:                        /* 2 - refused */
        PORT_DlcEstablishCnf(p_port->p_mcb, p_port->dlci,
                             p_port->p_mcb->peer_l2cap_mtu, 1);
        rfc_port_closed(p_port);
        return;
    case PORT_EV_DISC:                      /* 3 */
        rfc_send_ua(p_port->p_mcb, p_port->dlci);
        PORT_DlcEstablishCnf(p_port->p_mcb, p_port->dlci,
                             p_port->p_mcb->peer_l2cap_mtu, 1);
        rfc_port_closed(p_port);
        return;
    case PORT_EV_SABME:                     /* 0 - collision, respond UA */
        rfc_send_ua(p_port->p_mcb, p_port->dlci);
        return;
    case PORT_EV_DATA:                      /* 4 */
        GKI_freebuf(p_data);
        return;
    case PORT_EV_TIMEOUT:                   /* 5 */
        p_port->rfc_state = PORT_STATE_CLOSED;
        PORT_DlcEstablishCnf(p_port->p_mcb, p_port->dlci,
                             p_port->p_mcb->peer_l2cap_mtu, 1);
        return;
    }
    RFCOMM_TRACE_WARNING1("Port state sabme_wait_ua Event ignored %d", event);
}

/* ================================================================== */
/*  rfc_port_sm_term_wait_sec_check                                   */
/* ================================================================== */
void rfc_port_sm_term_wait_sec_check(tPORT *p_port, u16 event, void *p_data)
{
    switch (event) {
    case PORT_EV_SEC_COMPLETE:              /* 15 */
        if (*(u8 *)p_data != 0) {
            if (p_port->p_mcb != NULL) {
                rfc_send_dm(p_port->p_mcb, p_port->dlci, 1);
                port_rfc_closed(p_port, 0x0F);
            }
        } else {
            PORT_DlcEstablishInd(p_port->p_mcb, p_port->dlci,
                                 p_port->p_mcb->peer_l2cap_mtu);
        }
        return;
    case PORT_EV_DLC_EST_REQ:               /* 9 */
    case PORT_EV_DLC_REL_REQ:               /* 12 */
        RFCOMM_TRACE_ERROR2("Port error state %d event %d", p_port->rfc_state, event);
        return;
    case PORT_EV_LCID_DISCONNECTED:         /* 13 */
        btm_sec_abort_access_req(p_port->p_mcb->bd_addr);
        rfc_port_closed(p_port);
        return;
    case PORT_EV_DATA_REQ:                  /* 14 */
        RFCOMM_TRACE_ERROR0("Port error state Term Wait Sec event Data");
        GKI_freebuf(p_data);
        return;
    case PORT_EV_SABME:                     /* 0 */
        return;
    case PORT_EV_DISC:                      /* 3 - remote gave up */
        btm_sec_abort_access_req(p_port->p_mcb->bd_addr);
        p_port->rfc_state = PORT_STATE_CLOSED;
        rfc_send_ua(p_port->p_mcb, p_port->dlci);
        PORT_DlcReleaseInd(p_port->p_mcb, p_port->dlci);
        return;
    case PORT_EV_DATA:                      /* 4 */
        GKI_freebuf(p_data);
        return;
    case PORT_EV_DLC_EST_RSP:               /* 11 - local app response */
        if (*(u8 *)p_data != 0) {
            if (p_port->p_mcb != NULL) {
                rfc_send_dm(p_port->p_mcb, p_port->dlci, 1);
            }
        } else {
            rfc_send_ua(p_port->p_mcb, p_port->dlci);
            p_port->rfc_state = PORT_STATE_OPENED;
        }
        return;
    }
    RFCOMM_TRACE_WARNING1("Port state term_wait_sec_check Event ignored %d", event);
}

/* ================================================================== */
/*  rfc_port_sm_orig_wait_sec_check                                   */
/* ================================================================== */
void rfc_port_sm_orig_wait_sec_check(tPORT *p_port, u16 event, void *p_data)
{
    switch (event) {
    case PORT_EV_SEC_COMPLETE:              /* 15 */
        if (*(u8 *)p_data != 0) {
            PORT_DlcEstablishCnf(p_port->p_mcb, p_port->dlci, 0, 0x70);
            rfc_port_closed(p_port);
        } else {
            rfc_send_sabme(p_port->p_mcb, p_port->dlci);
            rfc_port_timer_start(p_port, 0x3C);
            p_port->rfc_state = PORT_STATE_OPENING;
        }
        return;
    case PORT_EV_SABME:                     /* 0 */
    case PORT_EV_DLC_EST_REQ:               /* 9 */
        RFCOMM_TRACE_ERROR2("Port error state %d event %d", p_port->rfc_state, event);
        return;
    case PORT_EV_LCID_DISCONNECTED:         /* 13 */
        btm_sec_abort_access_req(p_port->p_mcb->bd_addr);
        rfc_port_closed(p_port);
        return;
    case PORT_EV_DATA_REQ:                  /* 14 */
        RFCOMM_TRACE_ERROR0("Port error state Orig Wait Sec event Data");
        GKI_freebuf(p_data);
        return;
    case PORT_EV_DATA:                      /* 4 */
        GKI_freebuf(p_data);
        return;
    }
    RFCOMM_TRACE_WARNING1("Port state orig_wait_sec_check Event ignored %d", event);
}

/* ================================================================== */
/*  rfc_port_sm_opened                                                */
/* ================================================================== */
void rfc_port_sm_opened(tPORT *p_port, u16 event, void *p_data)
{
    switch (event) {
    case PORT_EV_DLC_EST_REQ:               /* 9 */
        RFCOMM_TRACE_ERROR2("Port error state %d event %d", p_port->rfc_state, event);
        return;
    case PORT_EV_DLC_REL_REQ:               /* 12 - local close */
        rfc_port_timer_start(p_port, 3);
        rfc_send_disc(p_port->p_mcb, p_port->dlci);
        p_port->rfc_flags = 0;
        p_port->rfc_state = PORT_STATE_DISC_WAIT_UA;
        return;
    case PORT_EV_LCID_DISCONNECTED:         /* 13 */
        rfc_port_closed(p_port);
        return;
    case PORT_EV_DATA_REQ:                  /* 14 - data with credit */
        if (p_port->p_mcb->flow == 2
            && ((BT_HDR *)p_data)->len < p_port->peer_mtu
            && p_port->field_3d == 0
            && p_port->credit_rx_max > p_port->credit_rx) {
            ((BT_HDR *)p_data)->layer_specific =
                (u16)(u8)(p_port->credit_rx_max - p_port->credit_rx);
            p_port->credit_rx = p_port->credit_rx_max;
        } else {
            ((BT_HDR *)p_data)->layer_specific = 0;
        }
        rfc_send_buf_uih(p_port->p_mcb, p_port->dlci, (BT_HDR *)p_data);
        rfc_dec_credit(p_port);
        return;
    case PORT_EV_UA:                        /* 1 */
        return;
    case PORT_EV_SABME:                     /* 0 */
        rfc_send_ua(p_port->p_mcb, p_port->dlci);
        return;
    case PORT_EV_DM:                        /* 2 - remote disconnected */
        PORT_DlcReleaseInd(p_port->p_mcb, p_port->dlci);
        rfc_port_closed(p_port);
        return;
    case PORT_EV_DISC:                      /* 3 - remote disconnected */
        p_port->rfc_state = PORT_STATE_CLOSED;
        rfc_send_ua(p_port->p_mcb, p_port->dlci);
        PORT_DlcReleaseInd(p_port->p_mcb, p_port->dlci);
        return;
    case PORT_EV_DATA:                      /* 4 */
        PORT_DataInd(p_port->p_mcb, p_port->dlci, (BT_HDR *)p_data);
        return;
    case PORT_EV_TIMEOUT:                   /* 5 */
        Port_TimeOutCloseMux(p_port->p_mcb);
        RFCOMM_TRACE_ERROR2("Port error state %d event %d", p_port->rfc_state, event);
        return;
    }
    RFCOMM_TRACE_WARNING1("Port state opened Event ignored %d", event);
}

/* ================================================================== */
/*  rfc_port_sm_disc_wait_ua                                          */
/* ================================================================== */
void rfc_port_sm_disc_wait_ua(tPORT *p_port, u16 event, void *p_data)
{
    switch (event) {
    case PORT_EV_DLC_EST_REQ:               /* 9 */
    case PORT_EV_DLC_EST_RSP:               /* 11 */
        RFCOMM_TRACE_ERROR2("Port error state %d event %d", p_port->rfc_state, event);
        return;
    case PORT_EV_LCID_DISCONNECTED:         /* 13 */
        rfc_port_closed(p_port);
        return;
    case PORT_EV_DATA_REQ:                  /* 14 */
        GKI_freebuf(p_data);
        return;
    case PORT_EV_UA:                        /* 1 */
        p_port->p_mcb->is_disc_initiator = 1;
        /* fall through */
    case PORT_EV_DM:                        /* 2 */
        rfc_port_closed(p_port);
        return;
    case PORT_EV_SABME:                     /* 0 */
        rfc_send_dm(p_port->p_mcb, p_port->dlci, 1);
        return;
    case PORT_EV_DISC:                      /* 3 */
        rfc_send_dm(p_port->p_mcb, p_port->dlci, 1);
        return;
    case PORT_EV_DATA:                      /* 4 */
        GKI_freebuf(p_data);
        rfc_send_dm(p_port->p_mcb, p_port->dlci, 0);
        return;
    case PORT_EV_TIMEOUT:                   /* 5 */
        rfc_port_closed(p_port);
        return;
    }
    RFCOMM_TRACE_WARNING1("Port state disc_wait_ua Event ignored %d", event);
}

/* ================================================================== */
/*  rfc_process_pn                                                    */
/* ================================================================== */
void rfc_process_pn(tRFC_MCB *p_mcb, u8 is_command, MX_FRAME *p_frame)
{
    tPORT *p_port;
    u8 dlci = p_frame->dlci;

    if (is_command) {
        /* Request */
        if (p_mcb->state != RFC_MX_STATE_DISC_WAIT_UA) {
            PORT_ParNegInd(p_mcb, dlci, p_frame->u.pn.mtu,
                           p_frame->u.pn.conv_layer, p_frame->u.pn.k);
        } else {
            /* We are disconnecting, send DM */
            rfc_send_dm(p_mcb, dlci, 0);
            if (rfc_cb.trace_level >= 2) {
                LogMsg_0(0x90001, "***** MX PN while disconnecting *****");
            }
        }
    } else {
        /* Response */
        p_port = port_find_mcb_dlci_port(p_mcb, dlci);
        if ((p_port == NULL) || !(p_port->rfc_flags & RFC_FLAG_EXPECT_PN)) {
            return;
        }
        p_port->rfc_flags &= 0xFE;
        rfc_port_timer_stop(p_port);
        PORT_ParNegCnf(p_mcb, dlci, p_frame->u.pn.mtu,
                       p_frame->u.pn.conv_layer, p_frame->u.pn.k);
    }
}

/* ================================================================== */
/*  rfc_process_rpn                                                   */
/* ================================================================== */
void rfc_process_rpn(tRFC_MCB *p_mcb, u8 is_command, u8 is_request, MX_FRAME *p_frame)
{
    tPORT *p_port;
    tPORT_STATE buf;

    p_port = port_find_mcb_dlci_port(p_mcb, p_frame->dlci);
    if (p_port == NULL) {
        if (is_command) {
            memset(&buf, 0, sizeof(buf));
            rfc_set_port_state((u8 *)&buf, p_frame);
            PORT_PortNegInd(p_mcb, p_frame->dlci, &buf, p_frame->u.rpn.param_mask);
        }
        return;
    }

    if (is_command && is_request) {
        /* Request for new parameters: reply with the current port state */
        buf = p_port->port_ctrl;
        rfc_send_rpn(p_mcb, p_frame->dlci, 0, &p_port->port_ctrl, 0);
    } else {
        /* Copy current state, then apply the received parameters */
        buf = p_port->port_ctrl;
        rfc_set_port_state((u8 *)&buf, p_frame);

        if (is_command) {
            PORT_PortNegInd(p_mcb, p_frame->dlci, &buf, p_frame->u.rpn.param_mask);
        } else {
            /* Response */
            p_port = port_find_mcb_dlci_port(p_mcb, p_frame->dlci);
            if ((p_port == NULL) || !(p_port->rfc_flags & (RFC_FLAG_EXPECT_RPN | RFC_FLAG_EXPECT_RPN2))) {
                return;
            }
            rfc_port_timer_stop(p_port);
            if (p_port->rfc_flags & RFC_FLAG_EXPECT_RPN) {
                p_port->rfc_flags &= ~RFC_FLAG_EXPECT_RPN;
                p_port->port_ctrl = buf;
                if (buf.fc_type == 0x0C || buf.fc_type == 0x30) {
                    p_frame->u.rpn.param_mask = 0x3F7F;
                } else {
                    p_port->port_ctrl.fc_type = 0x0C;
                    p_port->rfc_flags |= RFC_FLAG_EXPECT_RPN2;
                    rfc_send_rpn(p_mcb, p_frame->dlci, 1, &p_port->port_ctrl, 0x0C00);
                    rfc_port_timer_start(p_port, 0x3C);
                    return;
                }
            } else {
                p_port->rfc_flags &= ~RFC_FLAG_EXPECT_RPN2;
            }

            if ((p_frame->u.rpn.param_mask & 0x0C00) == 0x0C00
                || (p_frame->u.rpn.param_mask & 0x3000) == 0x3000) {
                PORT_PortNegCnf(p_mcb, p_port->dlci, &buf, 0);
            } else if (p_port->port_ctrl.fc_type == 0x0C) {
                p_port->port_ctrl.fc_type = 0x30;
                p_port->rfc_flags |= RFC_FLAG_EXPECT_RPN2;
                rfc_send_rpn(p_mcb, p_frame->dlci, 1, &p_port->port_ctrl, 0x3000);
                rfc_port_timer_start(p_port, 0x3C);
            } else if (p_port->port_ctrl.fc_type == 0x30) {
                p_port->port_ctrl.fc_type = 0;
                PORT_PortNegCnf(p_mcb, p_port->dlci, &buf, 0);
            }
        }
    }
}

/* ================================================================== */
/*  rfc_process_msc                                                   */
/* ================================================================== */
void rfc_process_msc(tRFC_MCB *p_mcb, u8 is_command, MX_FRAME *p_frame)
{
    tPORT *p_port;
    u8 signals = p_frame->u.msc.signals;
    tMSC_CTRL port_ctrl;

    p_port = port_find_mcb_dlci_port(p_mcb, p_frame->dlci);
    if (p_port == NULL) {
        return;
    }

    /* Extract modem signals into the local control block */
    port_ctrl.fc = 0;
    if (signals & 0x04) {
        port_ctrl.fc |= 0x01;
    }
    if (signals & 0x08) {
        port_ctrl.fc |= 0x02;
    }
    if (signals & 0x40) {
        port_ctrl.fc |= 0x04;
    }
    if (signals & 0x80) {
        port_ctrl.fc |= 0x08;
    }
    port_ctrl.break_signal = (signals >> 1) & 1;
    port_ctrl.break_duration = p_frame->u.msc.break_present
                                   ? p_frame->u.msc.break_duration : 0;
    port_ctrl.disc = 0;
    port_ctrl.dtr = 1;

    if (is_command) {
        /* Send the response and report the new state */
        rfc_send_msc(p_mcb, p_frame->dlci, 0, &port_ctrl);
        if (p_port->p_mcb->flow != 2) {
            u8 break_signal = port_ctrl.break_signal;
            p_port->field_63 = break_signal;
            if (break_signal != p_port->field_24) {
                PORT_FlowInd(p_mcb, p_frame->dlci,
                             (u8)((u32)__cntlzw(break_signal) >> 5));
            }
        }
        PORT_ControlInd(p_mcb, p_frame->dlci, &port_ctrl);
    } else {
        /* Response */
        if (p_port->rfc_flags & RFC_FLAG_EXPECT_MSC) {
            p_port->rfc_flags &= ~RFC_FLAG_EXPECT_MSC;
            rfc_port_timer_stop(p_port);
            PORT_ControlCnf(p_port->p_mcb, p_port->dlci, &port_ctrl);
        }
    }
}

/* ================================================================== */
/*  rfc_process_rls                                                   */
/* ================================================================== */
void rfc_process_rls(tRFC_MCB *p_mcb, u8 is_command, MX_FRAME *p_frame)
{
    tPORT *p_port;

    if (is_command) {
        /* Request: report line status and send the response */
        PORT_LineStatusInd(p_mcb, p_frame->dlci, p_frame->u.rls.line_status);
        rfc_send_rls(p_mcb, p_frame->dlci, 0, p_frame->u.rls.line_status);
    } else {
        /* Response */
        p_port = port_find_mcb_dlci_port(p_mcb, p_frame->dlci);
        if ((p_port == NULL) || !(p_port->rfc_flags & RFC_FLAG_EXPECT_RLS)) {
            return;
        }
        p_port->rfc_flags &= ~RFC_FLAG_EXPECT_RLS;
        rfc_port_timer_stop(p_port);
    }
}

/* ================================================================== */
/*  rfc_process_nsc / rfc_process_test_rsp                            */
/* ================================================================== */
void rfc_process_nsc(tRFC_MCB *p_mcb, MX_FRAME *p_frame)
{
}

void rfc_process_test_rsp(tRFC_MCB *p_mcb, BT_HDR *p_buf)
{
    GKI_freebuf(p_buf);
}

/* ================================================================== */
/*  rfc_process_fcon / rfc_process_fcoff                              */
/* ================================================================== */
void rfc_process_fcon(tRFC_MCB *p_mcb, u8 is_command)
{
    if (is_command) {
        rfc_cb.flow = 0;
        rfc_send_fcon(p_mcb, 0);
        if (p_mcb->l2cap_congested == 0) {
            PORT_FlowInd(p_mcb, 0, 1);
        }
    }
}

void rfc_process_fcoff(tRFC_MCB *p_mcb, u8 is_command)
{
    if (is_command) {
        rfc_cb.flow = 1;
        if (p_mcb->l2cap_congested == 0) {
            PORT_FlowInd(p_mcb, 0, 0);
        }
        rfc_send_fcoff(p_mcb, 0);
    }
}

/* ================================================================== */
/*  rfc_process_l2cap_congestion                                      */
/* ================================================================== */
void rfc_process_l2cap_congestion(tRFC_MCB *p_mcb, u8 is_congested)
{
    p_mcb->l2cap_congested = is_congested;
    if (!is_congested) {
        rfc_check_send_cmd(p_mcb, NULL);
    }

    if (rfc_cb.flow == 0) {
        if (!is_congested) {
            PORT_FlowInd(p_mcb, 0, 1);
        } else {
            PORT_FlowInd(p_mcb, 0, 0);
        }
    }
}

/* ================================================================== */
/*  rfc_set_port_state                                                */
/* ================================================================== */
void rfc_set_port_state(u8 *p_port_state, MX_FRAME *p_frame)
{
    if (p_frame->u.rpn.param_mask & 0x0001) {
        p_port_state[0] = p_frame->u.rpn.baud_rate;
    }
    if (p_frame->u.rpn.param_mask & 0x0002) {
        p_port_state[1] = p_frame->u.rpn.byte_size;
    }
    if (p_frame->u.rpn.param_mask & 0x0004) {
        p_port_state[2] = p_frame->u.rpn.stop_bits;
    }
    if (p_frame->u.rpn.param_mask & 0x0008) {
        p_port_state[3] = p_frame->u.rpn.parity;
    }
    if (p_frame->u.rpn.param_mask & 0x0010) {
        p_port_state[4] = p_frame->u.rpn.parity_type;
    }
    if (p_frame->u.rpn.param_mask & 0x3F00) {
        p_port_state[5] = p_frame->u.rpn.fc_type;
    }
    if (p_frame->u.rpn.param_mask & 0x0020) {
        p_port_state[7] = p_frame->u.rpn.xon_char;
    }
    if (p_frame->u.rpn.param_mask & 0x0040) {
        p_port_state[8] = p_frame->u.rpn.xoff_char;
    }
}
