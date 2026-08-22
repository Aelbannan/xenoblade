// Decompiled: libs/RVL_SDK/src/revolution/bte/stack/rfcomm/port_rfc.c
// High-level C reconstruction of the RFCOMM port layer (Broadcom BTE/Bluedroid).
//
// These functions glue the RFCOMM multiplexer (tRFC_MCB) to the port objects
// (tPORT) held in the global rfc_cb control block.  Structure field offsets
// were recovered from the retail assembly and cross-checked against the
// already-decompiled sibling unit port_utils.c.

#include <harness_catalog.h>
#include <string.h>

/* ========================================================================
 * Trace helpers (mirror bt_trace.h: TRACE_LAYER_RFCOMM=0x90000,
 * TRACE_TYPE_WARNING=1 / EVENT=3; guards use rfc_cb.trace_level)
 * ======================================================================== */
#define RFCOMM_TRACE_WARNING0(m)       {if (rfc_cb.trace_level >= 2) LogMsg_0(0x90001, (m));}
#define RFCOMM_TRACE_WARNING1(m, p1)   {if (rfc_cb.trace_level >= 2) LogMsg_1(0x90001, (m), (u32)(p1));}
#define RFCOMM_TRACE_EVENT0(m)         {if (rfc_cb.trace_level >= 4) LogMsg_0(0x90003, (m));}
#define RFCOMM_TRACE_EVENT1(m, p1)     {if (rfc_cb.trace_level >= 4) LogMsg_1(0x90003, (m), (u32)(p1));}
#define RFCOMM_TRACE_EVENT2(m, p1, p2) {if (rfc_cb.trace_level >= 4) LogMsg_2(0x90003, (m), (u32)(p1), (u32)(p2));}
#define RFCOMM_TRACE_EVENT3(m, p1, p2, p3) \
    {if (rfc_cb.trace_level >= 4) LogMsg_3(0x90003, (m), (u32)(p1), (u32)(p2), (u32)(p3));}
#define RFCOMM_TRACE_EVENT4(m, p1, p2, p3, p4) \
    {if (rfc_cb.trace_level >= 4) LogMsg_4(0x90003, (m), (u32)(p1), (u32)(p2), (u32)(p3), (u32)(p4));}

/* ========================================================================
 * Types
 * ======================================================================== */
typedef struct RfcMuxChannel tRFC_MCB;
typedef struct RfcPort tPORT;

/* GKI buffer header: len@0x02, offset@0x04, payload@0x08 */
typedef struct {
    u16 event;
    u16 len;
    u16 offset;
    u16 layer_specific;
    u8  data[1];
} BT_HDR;

/* GKI buffer queue (12 bytes) */
typedef struct {
    void* p_first;
    void* p_last;
    u16   count;
} BUFFER_Q;

/* Port control block exchanged during PN negotiation (9 bytes) */
typedef struct {
    u8 fc;
    u8 xon_char;
    u8 xoff_char;
    u8 xon_chars;
    u8 xoff_chars;
    u8 rx_buf_size;
    u8 field_6;
    u8 field_7;
    u8 field_8;
} tPORT_CTRL;

/* Port callbacks */
typedef void (*tPORT_MGMT_CB)(u32 event, u8 dlci);
typedef void (*tPORT_CB)(u32 code, u8 dlci);
typedef int  (*tPORT_DATA_CB)(u8 dlci, u8* p_data, u16 len);

/*
 * tPORT - 0xA4 bytes, stored at rfc_cb + 0x68 + i * 0xA4.
 */
struct RfcPort {
    u8   dlci;             /* 0x00 - dlci reported to upper layers */
    u8   in_use;           /* 0x01 */
    u8   state;            /* 0x02 */
    u8   pad_03;           /* 0x03 */
    u16  uuid;             /* 0x04 */
    u8   bd_addr[6];       /* 0x06 */
    u8   field_0c;         /* 0x0C */
    u8   dlci2;            /* 0x0D - dlci used on the RFCOMM link */
    u8   field_0e;         /* 0x0E */
    u8   line_status;      /* 0x0F */
    u8   pad_10[2];        /* 0x10 */
    u16  mtu;              /* 0x12 */
    u16  peer_mtu;         /* 0x14 */
    u8   pad_16[2];        /* 0x16 */
    BUFFER_Q tx_queue;     /* 0x18 */
    u8   field_24;         /* 0x24 - peer tx flow off */
    u8   flow_state;       /* 0x25 */
    u8   pad_26[2];        /* 0x26 */
    u32  tx_pending;       /* 0x28 - bytes queued for tx */
    u8   pad_2c[4];        /* 0x2C */
    BUFFER_Q rx_queue;     /* 0x30 */
    u8   local_fc;         /* 0x3C */
    u8   field_3d;         /* 0x3D */
    u8   pad_3e[2];        /* 0x3E */
    u32  rx_queued;        /* 0x40 - bytes queued on rx */
    u8   pad_44[0x0A];     /* 0x44 */
    u8   field_4e;         /* 0x4E - rx flag character */
    u8   pad_4f[2];        /* 0x4F */
    tPORT_CTRL port_ctrl;  /* 0x51 (9 bytes) */
    u8   field_5a[5];      /* 0x5A - control info sent with MSC */
    u8   modem_signal;     /* 0x5F */
    u8   field_60;         /* 0x60 */
    u8   field_61;         /* 0x61 */
    u8   field_62;         /* 0x62 */
    u8   field_63;         /* 0x63 */
    u8   ctrl_flags;       /* 0x64 */
    u8   field_65;         /* 0x65 */
    u8   pad_66[2];        /* 0x66 */
    u8   field_68;         /* 0x68 */
    u8   pad_69[3];        /* 0x69 */
    tRFC_MCB* p_mcb;       /* 0x6C */
    u8   pad_70[0x18];     /* 0x70 */
    u32  mask;             /* 0x88 - registered event mask */
    tPORT_MGMT_CB p_mgmt_cb; /* 0x8C */
    tPORT_CB p_port_cb;    /* 0x90 */
    tPORT_DATA_CB p_data_cb; /* 0x94 */
    u16  field_98;         /* 0x98 */
    u16  credit_rx;        /* 0x9A */
    u16  credit_rx_max;    /* 0x9C */
    u16  credit_rx_low;    /* 0x9E */
    u16  rx_buf_critical;  /* 0xA0 */
    u8   pad_a2[2];        /* 0xA2 */
};                         /* 0xA4 */

/*
 * tRFC_MCB - RFCOMM multiplexer channel.
 */
struct RfcMuxChannel {
    u8   pad_00[0x24];     /* 0x00 */
    u8   port_inx[0x3E];   /* 0x24 - dlci -> port index+1 map */
    u8   bd_addr[6];       /* 0x62 */
    u16  lcid;             /* 0x68 */
    u16  field_6a;         /* 0x6A */
    u8   state;            /* 0x6C */
    u8   initiator;        /* 0x6D */
    u8   field_6e;         /* 0x6E */
    u8   field_6f;         /* 0x6F */
    u8   field_70;         /* 0x70 */
    u8   field_71;         /* 0x71 - mux level flow */
    u8   field_72;         /* 0x72 - credit based flow (2) */
};

/*
 * tRFC_CB - RFCOMM control block (ports at 0x68, trace level at 0x414).
 */
typedef struct {
    u8   pad_00[0x68];
    tPORT port[5];
    u8   pad_398[0x78];  /* 0x39C..0x414 */
    u8   trace_level;      /* 0x414 */
} tRFC_CB;

/* Port states */
#define PORT_STATE_CLOSED   0
#define PORT_STATE_OPENING  1
#define PORT_STATE_OPENED   2
#define PORT_STATE_CLOSING  3

/* Management events */
#define PORT_EV_ERR         0x00008000

/* ========================================================================
 * Externs
 * ======================================================================== */
extern tRFC_CB rfc_cb;

extern void LogMsg_0(u32 mask, const char* msg);
extern void LogMsg_1(u32 mask, const char* fmt, u32 p1);
extern void LogMsg_2(u32 mask, const char* fmt, u32 p1, u32 p2);
extern void LogMsg_3(u32 mask, const char* fmt, u32 p1, u32 p2, u32 p3);
extern void LogMsg_4(u32 mask, const char* fmt, u32 p1, u32 p2, u32 p3, u32 p4);

extern void* GKI_dequeue(BUFFER_Q* q);
extern void  GKI_enqueue(BUFFER_Q* q, void* buf);
extern void  GKI_freebuf(void* buf);

extern tPORT* port_find_mcb_dlci_port(tRFC_MCB* p_mcb, u8 dlci);
extern tPORT* port_find_dlci_port(u8 dlci);
extern void   port_select_mtu(tPORT* p_port);
extern void   port_release_port(tPORT* p_port);
extern void   port_flow_control_peer(tPORT* p_port, u8 enable, u16 credits);
extern u32    port_flow_control_user(tPORT* p_port);
extern u32    port_get_signal_changes(tPORT* p_port, u8 prev_signals, u8 signals);

extern void rfc_release_multiplexer_channel(tRFC_MCB* p_mcb);
extern void rfc_check_mcb_active(tRFC_MCB* p_mcb);
extern void rfc_timer_stop(tRFC_MCB* p_mcb);
extern void rfc_port_timer_stop(tPORT* p_port);
extern void rfc_send_dm(tRFC_MCB* p_mcb, u8 dlci, u8 cr);

extern void RFCOMM_StartRsp(tRFC_MCB* p_mcb, u16 result);
extern void RFCOMM_ParNegReq(tRFC_MCB* p_mcb, u8 dlci, u16 mtu);
extern void RFCOMM_ParNegRsp(tRFC_MCB* p_mcb, u8 dlci, u16 mtu, u8 cl, u8 k);
extern void RFCOMM_DlcEstablishReq(tRFC_MCB* p_mcb, u8 dlci, u16 mtu);
extern void RFCOMM_DlcEstablishRsp(tRFC_MCB* p_mcb, u8 dlci, u16 mtu, u16 result);
extern void RFCOMM_PortNegReq(tRFC_MCB* p_mcb, u8 dlci, void* p_port_ctrl);
extern void RFCOMM_PortNegRsp(tRFC_MCB* p_mcb, u8 dlci, void* p_port_ctrl, u16 request);
extern void RFCOMM_ControlReq(tRFC_MCB* p_mcb, u8 dlci, void* p_port_ctrl);
extern void RFCOMM_DlcReleaseReq(tRFC_MCB* p_mcb, u8 dlci);
extern void RFCOMM_LineStatusReq(tRFC_MCB* p_mcb, u8 dlci, u8 status);
extern void RFCOMM_DataReq(tRFC_MCB* p_mcb, u8 dlci, BT_HDR* p_buf);

/* Defined below; used across functions */
void port_rfc_closed(tPORT* p_port, u8 result);
u32  port_rfc_send_tx_data(tPORT* p_port);

/* ------------------------------------------------------------------ */
/*  String-pool orphans: the retail .data contains the format strings  */
/*  "port_open_continue" / "port_open_continue no mx channel" even    */
/*  though their function was eliminated from the retail .text.        */
/*  Reference them here (in retail pool order) so the .data string     */
/*  layout matches (pool base lands on "port_open_continue").          */
/* ------------------------------------------------------------------ */
char *const port_rfc_pool_orphan_strings[] = {
    "port_open_continue",
    "port_open_continue no mx channel",
};

/* ========================================================================
 * PORT_StartCnf
 * ======================================================================== */
void PORT_StartCnf(tRFC_MCB* p_mcb, u16 result)
{
    tPORT* p_port;
    int i;
    int no_port = 1;

    RFCOMM_TRACE_EVENT1("PORT_StartCnf result:%d", result);

    for (i = 0, p_port = &rfc_cb.port[0]; i < 5; i++, p_port++) {
        if (p_port->p_mcb == p_mcb) {
            no_port = 0;
            if (result == 0) {
                RFCOMM_ParNegReq(p_mcb, p_port->dlci2, p_port->mtu);
            } else {
                RFCOMM_TRACE_WARNING1("PORT_StartCnf failed result:%d", result);

                if (result == 4)
                    p_port->field_0e = 0x18;
                else
                    p_port->field_0e = 0x0C;

                rfc_release_multiplexer_channel(p_mcb);
                p_port->p_mcb = NULL;

                if (p_port->p_mgmt_cb != NULL && (p_port->mask & PORT_EV_ERR))
                    p_port->p_mgmt_cb(PORT_EV_ERR, p_port->dlci);

                if (p_port->p_port_cb != NULL)
                    p_port->p_port_cb(0x0C, p_port->dlci);

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

    for (i = 0, p_port = &rfc_cb.port[0]; i < 5; i++, p_port++) {
        if (p_port->p_mcb == NULL || p_port->p_mcb == p_mcb) {
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
        p_mcb->port_inx[dlci] = p_port->dlci;
    }

    memcpy(p_port->bd_addr, p_mcb->bd_addr, 6);
    port_select_mtu(p_port);
    p_port->p_mcb = p_mcb;

    if (p_port->mtu < mtu)
        mtu = p_port->mtu;
    p_port->mtu = mtu;
    p_port->peer_mtu = mtu;

    if (p_mcb->field_72 == 0) {
        if (cl == 0)
            p_mcb->field_72 = 1;
        else
            p_mcb->field_72 = 2;
    }

    if (cl == 0) {
        cl_rsp = 0;
        k_rsp = 0;
    } else {
        if (p_mcb->field_72 == 2) {
            p_port->field_98 = k;
            if (k == 0)
                p_port->field_24 = 1;

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

    if (p_mcb->field_72 == 0) {
        if (cl == 0xE0)
            p_mcb->field_72 = 2;
        else
            p_mcb->field_72 = 1;
    }

    if (p_port->mtu < mtu)
        mtu = p_port->mtu;
    p_port->mtu = mtu;
    p_port->peer_mtu = mtu;

    if (p_mcb->field_72 == 2) {
        p_port->field_98 = k;
        if (k == 0)
            p_port->field_24 = 1;
    }

    if (p_port->state == PORT_STATE_OPENING)
        RFCOMM_DlcEstablishReq(p_mcb, p_port->dlci2, p_port->mtu);
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
        p_mcb->port_inx[dlci] = p_port->dlci;
    }

    if (mtu != 0 && mtu < p_port->peer_mtu)
        p_port->peer_mtu = mtu;

    rfc_timer_stop(p_mcb);
    RFCOMM_DlcEstablishRsp(p_mcb, dlci, p_port->mtu, 0);

    if (p_port->p_mgmt_cb != NULL && (p_port->mask & 0x200))
        p_port->p_mgmt_cb(0x200, p_port->dlci);

    if (p_port->p_port_cb != NULL)
        p_port->p_port_cb(0, p_port->dlci);

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
        p_port->field_0e = 0x0C;
        port_rfc_closed(p_port, 0x0C);
        return;
    }

    if (mtu != 0 && mtu < p_port->peer_mtu)
        p_port->peer_mtu = mtu;

    rfc_timer_stop(p_mcb);

    if (p_port->p_mgmt_cb != NULL && (p_port->mask & 0x200))
        p_port->p_mgmt_cb(0x200, p_port->dlci);

    if (p_port->p_port_cb != NULL)
        p_port->p_port_cb(0, p_port->dlci);

    p_port->state = PORT_STATE_OPENED;

    if (p_port->uuid == 0x1103 || p_port->uuid == 0x1111)
        RFCOMM_PortNegReq(p_port->p_mcb, p_port->dlci2, NULL);
    else
        RFCOMM_ControlReq(p_port->p_mcb, p_port->dlci2, p_port->field_5a);
}

/* ========================================================================
 * PORT_PortNegInd
 * ======================================================================== */
void PORT_PortNegInd(tRFC_MCB* p_mcb, u8 dlci, tPORT_CTRL* p_port_ctrl, u16 request)
{
    tPORT* p_port = port_find_mcb_dlci_port(p_mcb, dlci);

    RFCOMM_TRACE_EVENT0("PORT_PortNegInd");

    if (p_port == NULL) {
        p_port = port_find_dlci_port(dlci);
        if (p_port == NULL) {
            p_port->port_ctrl = *p_port_ctrl;
            RFCOMM_PortNegRsp(p_mcb, dlci, p_port_ctrl, 0);
            return;
        }
        p_mcb->port_inx[dlci] = p_port->dlci;
    }

    p_port->port_ctrl = *p_port_ctrl;
    RFCOMM_PortNegRsp(p_mcb, dlci, p_port_ctrl, request);
}

/* ========================================================================
 * PORT_PortNegCnf
 * ======================================================================== */
void PORT_PortNegCnf(tRFC_MCB* p_mcb, u8 dlci, void* p_port_ctrl, u16 result)
{
    tPORT* p_port = port_find_mcb_dlci_port(p_mcb, dlci);

    RFCOMM_TRACE_EVENT0("PORT_PortNegCnf");

    if (p_port == NULL) {
        RFCOMM_TRACE_WARNING0("PORT_PortNegCnf no port");
        return;
    }

    if (result != 0) {
        p_port->field_0e = 0x0E;
        RFCOMM_DlcReleaseReq(p_mcb, p_port->dlci2);
        port_rfc_closed(p_port, 0x0E);
        return;
    }

    if ((p_port->ctrl_flags & 1) == 0) {
        RFCOMM_ControlReq(p_port->p_mcb, p_port->dlci2, p_port->field_5a);
    } else {
        RFCOMM_TRACE_WARNING0("PORT_PortNegCnf Control Already sent");
    }
}

/* ========================================================================
 * PORT_ControlInd
 * ======================================================================== */
void PORT_ControlInd(tRFC_MCB* p_mcb, u8 dlci, tPORT_CTRL* p_port_ctrl)
{
    tPORT* p_port = port_find_mcb_dlci_port(p_mcb, dlci);
    u32 event;

    RFCOMM_TRACE_EVENT0("PORT_ControlInd");

    if (p_port == NULL)
        return;

    event = port_get_signal_changes(p_port, p_port->modem_signal, p_port_ctrl->fc);

    p_port->modem_signal = p_port_ctrl->fc;
    p_port->field_60 = p_port_ctrl->xon_char;
    p_port->field_61 = p_port_ctrl->xoff_char;
    p_port->field_62 = p_port_ctrl->xon_chars;
    p_port->field_63 = p_port_ctrl->xoff_chars;

    if ((p_port->ctrl_flags & 1) == 0) {
        RFCOMM_ControlReq(p_port->p_mcb, p_port->dlci2, p_port->field_5a);
    } else {
        if ((p_port->ctrl_flags & 4) == 0)
            event |= (p_port->mask & 0x200);
        if (p_port->ctrl_flags & 2)
            event |= port_rfc_send_tx_data(p_port);
    }

    p_port->ctrl_flags |= 0x0C;

    if (p_port_ctrl->xon_char != 0)
        event |= (p_port->mask & 0x40);

    if (event != 0 && p_port->p_mgmt_cb != NULL)
        p_port->p_mgmt_cb(event, p_port->dlci);

    RFCOMM_TRACE_EVENT4("PORT_ControlInd DTR_DSR : %d, RTS_CTS : %d, RI : %d, DCD : %d",
                        p_port->modem_signal & 0x01,
                        (p_port->modem_signal >> 1) & 1,
                        (p_port->modem_signal >> 2) & 1,
                        (p_port->modem_signal >> 3) & 1);
}

/* ========================================================================
 * PORT_ControlCnf
 * ======================================================================== */
void PORT_ControlCnf(tRFC_MCB* p_mcb, u8 dlci)
{
    tPORT* p_port = port_find_mcb_dlci_port(p_mcb, dlci);
    u32 event = 0;

    RFCOMM_TRACE_EVENT0("PORT_ControlCnf");

    if (p_port == NULL)
        return;

    if ((p_port->ctrl_flags & 2) == 0) {
        p_port->ctrl_flags |= 2;
        if (p_port->ctrl_flags & 4)
            event = (p_port->mask & 0x200);
    }

    if (p_port->ctrl_flags & 4)
        event |= port_rfc_send_tx_data(p_port);

    if (event != 0 && p_port->p_mgmt_cb != NULL)
        p_port->p_mgmt_cb(event, p_port->dlci);
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

    if (p_port->p_mgmt_cb != NULL && (p_port->mask & event) != 0)
        p_port->p_mgmt_cb(p_port->mask & event, p_port->dlci);
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

    for (i = 0, p_port = &rfc_cb.port[0]; i < 5; i++, p_port++) {
        if (p_port->p_mcb == p_mcb)
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

    for (i = 0, p_port = &rfc_cb.port[0]; i < 5; i++, p_port++) {
        if (p_port->p_mcb == p_mcb)
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

    if (p_port->p_data_cb != NULL) {
        port_flow_control_peer(p_port, 1, 1);
        p_port->p_data_cb(p_port->dlci, p_buf->data + p_buf->offset, p_buf->len);
        GKI_freebuf(p_buf);
        return;
    }

    if (p_port->rx_queued + p_buf->len > 0x2EE0 ||
        p_port->rx_queue.count + 1 > p_port->rx_buf_critical) {
        RFCOMM_TRACE_EVENT0("PORT_DataInd. Buffer over run. Dropping the buffer");
        GKI_freebuf(p_buf);
        RFCOMM_LineStatusReq(p_mcb, dlci, 2);
        return;
    }

    if (p_port->field_4e != 0 && (p_port->mask & 0x2)) {
        u8* p = p_buf->data + p_buf->offset;
        int n = p_buf->len;
        for (; n > 0; n--) {
            if (*p++ == p_port->field_4e) {
                event |= 0x2;
                break;
            }
        }
    }

    GKI_enqueue(&p_port->rx_queue, p_buf);
    p_port->rx_queued += p_buf->len;
    port_flow_control_peer(p_port, 0, 0);

    if (p_port->field_3d != 0) {
        if (event & 0x2)
            p_port->field_65 = 1;
    } else {
        event |= 0x1;
        event &= p_port->mask;
        if (p_port->p_mgmt_cb != NULL && event != 0)
            p_port->p_mgmt_cb(event, p_port->dlci);
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
        p_mcb->field_71 = fc;
    } else {
        p_port = port_find_mcb_dlci_port(p_mcb, dlci);
        if (p_port == NULL)
            return;
        p_port->field_24 = (fc == 0);
    }

    for (i = 0; i < 5; i++) {
        if (dlci == 0) {
            p_port = &rfc_cb.port[i];
            if (p_port->in_use == 0 || p_port->p_mcb != p_mcb || p_port->field_68 != 4)
                continue;
        }

        event = port_flow_control_user(p_port);
        event |= port_rfc_send_tx_data(p_port);
        event &= p_port->mask;

        if (p_port->p_mgmt_cb != NULL && event != 0)
            p_port->p_mgmt_cb(event, p_port->dlci);

        if (dlci != 0)
            break;
    }
}

/* ========================================================================
 * port_rfc_send_tx_data
 * ======================================================================== */
u32 port_rfc_send_tx_data(tPORT* p_port)
{
    u32 event = 0;
    BT_HDR* p_buf;

    if (p_port->tx_pending != 0) {
        while (p_port->field_24 == 0 && p_port->p_mcb != NULL &&
               p_port->p_mcb->field_71 != 0) {
            p_buf = (BT_HDR*)GKI_dequeue(&p_port->tx_queue);
            if (p_buf != NULL) {
                RFCOMM_TRACE_EVENT0("Sending RFCOMM_DataReq");

                {
                    u32 pending = p_port->tx_pending;
                    tRFC_MCB* mcb = p_port->p_mcb;
                    u8 dlci2 = p_port->dlci2;

                    p_port->tx_pending = pending - p_buf->len;
                    RFCOMM_DataReq(mcb, dlci2, p_buf);
                }
                event |= 0x4000;

                if (p_port->tx_pending == 0) {
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

    return event & p_port->mask;
}

/* ========================================================================
 * port_rfc_closed
 * ======================================================================== */
void port_rfc_closed(tPORT* p_port, u8 result)
{
    u32 event = 0;
    tRFC_MCB* p_mcb = p_port->p_mcb;
    u8 state = p_port->state;

    if (state == PORT_STATE_OPENING && p_port->field_0c != 0) {
        RFCOMM_TRACE_EVENT0("port_rfc_closed in OPENING state ignored");
        rfc_port_timer_stop(p_port);
        p_port->field_68 = 0;
        if (p_mcb != NULL) {
            p_mcb->port_inx[p_port->dlci2] = 0;
            rfc_check_mcb_active(p_mcb);
            p_port->p_mcb = NULL;
        }
        p_port->dlci2 &= 0xFE;
        return;
    }

    if (state != PORT_STATE_CLOSING && state != PORT_STATE_CLOSED) {
        u8 prev;
        p_port->line_status |= 0x10;
        prev = p_port->modem_signal;
        p_port->modem_signal = prev & ~0x0B;
        event = port_get_signal_changes(p_port, prev, p_port->modem_signal);
        if (p_port->mask & PORT_EV_ERR)
            event |= PORT_EV_ERR;
    }

    RFCOMM_TRACE_EVENT2("port_rfc_closed state:%d sending events:%x", p_port->state, event);

    if (p_port->p_mgmt_cb != NULL && event != 0)
        p_port->p_mgmt_cb(event, p_port->dlci);

    if (p_port->p_port_cb != NULL)
        p_port->p_port_cb(result, p_port->dlci);

    p_port->field_68 = 0;
    port_release_port(p_port);
}
