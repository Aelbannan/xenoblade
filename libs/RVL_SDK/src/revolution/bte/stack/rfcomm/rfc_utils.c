// High-level C reconstruction of RVL_SDK/src/revolution/bte/stack/rfcomm/rfc_utils

#include <harness_catalog.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/*  Forward declarations                                               */
/* ------------------------------------------------------------------ */
struct RfcMuxChannel;
struct RfcPortStruct;

/* ------------------------------------------------------------------ */
/*  rfc_crctable -- 256-byte CRC table at 0x80514E48                  */
/* ------------------------------------------------------------------ */
extern u8 rfc_crctable[256];

/* ------------------------------------------------------------------ */
/*  rfc_cb -- global control block at 0x805CFBA8, accessed as byte[]  */
/* ------------------------------------------------------------------ */
extern u8 rfc_cb[];

/* ------------------------------------------------------------------ */
/*  External functions called by this TU                               */
/* ------------------------------------------------------------------ */
extern void LogMsg_0(u32 level, const char *msg);
extern void LogMsg_1(u32 level, const char *msg, u32 p1);
extern void *GKI_dequeue(void *q);
extern void  GKI_enqueue(void *q, void *buf);
extern void  GKI_freebuf(void *buf);
extern void  GKI_init_q(void *q);
extern void btu_start_timer(void *tle, u16 type, u32 timeout);
extern void btu_stop_timer(void *tle);
extern void rfc_mx_sm_execute(struct RfcMuxChannel *mcb, u16 event, void *data);
extern void rfc_port_sm_execute(struct RfcPortStruct *port, u16 event, void *data);
extern void port_rfc_closed(struct RfcPortStruct *port, u8 result);
extern void PORT_FlowInd(struct RfcMuxChannel *mcb, u8 dlci, u8 flow);
extern u16  L2CA_DataWrite(u16 lcid, void *buf);

/* ------------------------------------------------------------------ */
/*  Minimal TIMER_LIST_ENT layout for field access.                    */
/* ------------------------------------------------------------------ */
typedef struct {
    u8  _pad[0x10];
    u32 param;       /* offset 0x10 */
    u16 event;       /* offset 0x14 */
    u8  in_use;      /* offset 0x16 */
} TimerEntry;

/* ------------------------------------------------------------------ */
/*  tRFC_MCB (0x78 bytes)                                              */
/* ------------------------------------------------------------------ */
struct RfcMuxChannel {
    u8  tle_0x00[0x18];              /* TIMER_LIST_ENT (24 B)    */
    u8  cmd_q[0x0C];                 /* BUFFER_Q (12 B)          */
    u8  port_inx[0x3E];              /* byte array [62]          */
    u8  bd_addr[6];                  /* offset 0x62              */
    u16 lcid;                        /* offset 0x68              */
    u16 peer_l2cap_mtu;              /* offset 0x6A              */
    u8  state;                       /* offset 0x6C              */
    u8  is_initiator;                /* offset 0x6D              */
    u8  local_cfg_sent;              /* offset 0x6E              */
    u8  peer_cfg_rcvd;               /* offset 0x6F              */
    u8  restart_required;            /* offset 0x70              */
    u8  peer_ready;                  /* offset 0x71              */
    u8  flow;                        /* offset 0x72              */
    u8  l2cap_congested;             /* offset 0x73              */
    u8  is_disc_initiator;           /* offset 0x74              */
    u16 pending_lcid;                /* offset 0x75              */
    u8  pending_id;                  /* offset 0x77              */
};

/* ------------------------------------------------------------------ */
/*  tPORT (only fields accessed by this TU)                            */
/* ------------------------------------------------------------------ */
struct RfcPortStruct {
    u8  pad_00[0x0D];
    u8  dlci;                        /* offset 0x0D              */
    u8  pad_0E[0x16];
    u8  state;                       /* offset 0x24              */
    u8  pad_25[0x47];
    struct RfcMuxChannel *p_mcb;     /* offset 0x6C              */
    u8  tle_0x70[0x18];              /* offset 0x70 (rfc.tle)    */
    u8  pad_88[0x10];
    u16 credit_tx;                   /* offset 0x98              */
};

/* ------------------------------------------------------------------ */
/*  rfc_cb layout constants                                            */
/* ------------------------------------------------------------------ */
#define MCB_ARRAY_OFFSET   0x39C
#define TRACE_LEVEL_OFFSET 0x414
#define LAST_MUX_OFFSET    0x065
#define RFCOMM_MAX_DLCI    61
#define MCB_SIZE 0x78

/* ------------------------------------------------------------------ */
/*  Helpers (macros to avoid out-of-line emission in C)                */
/* ------------------------------------------------------------------ */

#define MCB_PTR(idx)  ((struct RfcMuxChannel *)(rfc_cb + MCB_ARRAY_OFFSET + (idx) * MCB_SIZE))
#define TRACE_LEVEL() (rfc_cb[TRACE_LEVEL_OFFSET])

/* ------------------------------------------------------------------ */
/*                        FUNCTION IMPLEMENTATIONS                     */
/* ------------------------------------------------------------------ */

/* ================================================================== */
/*  rfc_calc_fcs                                                       */
/* ================================================================== */
u8 rfc_calc_fcs(u16 len, u8 *p)
{
    u8 fcs = 0xFF;

    while (len-- != 0) {
        fcs = rfc_crctable[fcs ^ *p++];
    }

    return (u8)(0xFF - fcs);
}

/* ================================================================== */
/*  rfc_check_fcs                                                      */
/* ================================================================== */
u8 rfc_check_fcs(u16 len, u8 *p, u8 received_fcs)
{
    u8 fcs = 0xFF;

    while (len-- != 0) {
        fcs = rfc_crctable[fcs ^ *p++];
    }

    return (rfc_crctable[fcs ^ received_fcs] == 0xCF);
}

/* ================================================================== */
/*  rfc_alloc_multiplexer_channel                                      */
/* ================================================================== */
struct RfcMuxChannel *rfc_alloc_multiplexer_channel(u8 *bd_addr, u8 is_initiator)
{
    u8 idx;
    struct RfcMuxChannel *p;
    TimerEntry *tle;

    /* Fast path: MCB[0] already active and matches bd_addr. */
    if (MCB_PTR(0)->state != 0
        && memcmp(MCB_PTR(0)->bd_addr, bd_addr, 6) == 0)
    {
        if (TRACE_LEVEL() >= 4) {
            LogMsg_0(0x90003, "rfc_timer_stop");
        }
        btu_stop_timer(MCB_PTR(0)->tle_0x00);
        return MCB_PTR(0);
    }

    /* Round-robin: try next index after the last one used. */
    idx = rfc_cb[LAST_MUX_OFFSET] + 1;
    if (idx < 1) {
        idx = 0;
    }

    p = MCB_PTR(idx);

    if (p->state == 0) {
        memset(p, 0, MCB_SIZE);
        memcpy(p->bd_addr, bd_addr, 6);
        GKI_init_q(p->cmd_q);
        p->is_initiator = is_initiator;

        if (TRACE_LEVEL() >= 4) {
            LogMsg_1(0x90003, "rfc_timer_start - timeout:%d", 60);
        }

        tle = (TimerEntry *)p->tle_0x00;
        tle->param = (u32)p;
        btu_start_timer(tle, 0x0B, 60);

        rfc_cb[LAST_MUX_OFFSET] = idx;
        return p;
    }

    return 0;
}

/* ================================================================== */
/*  rfc_release_multiplexer_channel                                    */
/* ================================================================== */
void rfc_release_multiplexer_channel(struct RfcMuxChannel *p)
{
    void *buf;

    if (TRACE_LEVEL() >= 4) {
        LogMsg_0(0x90003, "rfc_timer_stop");
    }

    btu_stop_timer(p->tle_0x00);

    while ((buf = GKI_dequeue(p->cmd_q)) != 0) {
        GKI_freebuf(buf);
    }

    memset(p, 0, MCB_SIZE);
    p->state = 0;
}

/* ================================================================== */
/*  rfc_timer_start                                                    */
/* ================================================================== */
void rfc_timer_start(struct RfcMuxChannel *p, u16 timeout)
{
    TimerEntry *tle = (TimerEntry *)p->tle_0x00;

    if (TRACE_LEVEL() >= 4) {
        LogMsg_1(0x90003, "rfc_timer_start - timeout:%d", timeout);
    }

    tle->param = (u32)p;
    btu_start_timer(tle, 0x0B, timeout);
}

/* ================================================================== */
/*  rfc_timer_stop                                                     */
/* ================================================================== */
void rfc_timer_stop(struct RfcMuxChannel *p)
{
    if (TRACE_LEVEL() >= 4) {
        LogMsg_0(0x90003, "rfc_timer_stop");
    }

    btu_stop_timer(p->tle_0x00);
}

/* ================================================================== */
/*  rfc_port_timer_start                                               */
/* ================================================================== */
void rfc_port_timer_start(struct RfcPortStruct *port, u16 tout)
{
    TimerEntry *tle = (TimerEntry *)((u8 *)port + 0x70);

    if (TRACE_LEVEL() >= 4) {
        LogMsg_1(0x90003, "rfc_port_timer_start - timeout:%d", tout);
    }

    tle->param = (u32)port;
    btu_start_timer(tle, 0x0C, tout);
}

/* ================================================================== */
/*  rfc_port_timer_stop                                                */
/* ================================================================== */
void rfc_port_timer_stop(struct RfcPortStruct *port)
{
    if (TRACE_LEVEL() >= 4) {
        LogMsg_0(0x90003, "rfc_port_timer_stop");
    }

    btu_stop_timer((u8 *)port + 0x70);
}

/* ================================================================== */
/*  rfc_check_mcb_active                                               */
/* ================================================================== */
void rfc_check_mcb_active(struct RfcMuxChannel *p)
{
    u16 ii;

    for (ii = 0; ii < RFCOMM_MAX_DLCI; ii++) {
        if (p->port_inx[ii] != 0) {
            p->is_disc_initiator = 0;
            return;
        }
    }

    if (p->is_disc_initiator != 0) {
        p->is_disc_initiator = 0;
        rfc_mx_sm_execute(p, 8, 0);
        return;
    }

    if (TRACE_LEVEL() >= 4) {
        LogMsg_1(0x90003, "rfc_timer_start - timeout:%d", 2);
    }

    ((TimerEntry *)p->tle_0x00)->param = (u32)p;
    btu_start_timer(p->tle_0x00, 0x0B, 2);
}

/* ================================================================== */
/*  rfcomm_process_timeout                                             */
/* ================================================================== */
void rfcomm_process_timeout(void *tle)
{
    TimerEntry *entry = (TimerEntry *)tle;

    switch (entry->event) {
    case 11:
        rfc_mx_sm_execute((struct RfcMuxChannel *)(u32)entry->param, 5, 0);
        break;
    case 12:
        rfc_port_sm_execute((struct RfcPortStruct *)(u32)entry->param, 5, 0);
        break;
    default:
        break;
    }
}

/* ================================================================== */
/*  rfc_sec_check_complete                                             */
/* ================================================================== */
void rfc_sec_check_complete(u8 *bd_addr, void *p_ref_data, u8 res)
{
    struct RfcPortStruct *port = (struct RfcPortStruct *)p_ref_data;

    /* Verify the port is still in use and waiting for security. */
    if (!*((u8 *)port + 1)
        || ((*((u8 *)port + 0x68) != 2) && (*((u8 *)port + 0x68) != 3)))
    {
        return;
    }

    rfc_port_sm_execute(port, 0x0F, &res);
}

/* ================================================================== */
/*  rfc_port_closed                                                    */
/* ================================================================== */
void rfc_port_closed(struct RfcPortStruct *port)
{
    struct RfcMuxChannel *p_mcb = port->p_mcb;

    if (TRACE_LEVEL() >= 5) {
        LogMsg_0(0x90004, "rfc_port_closed");
    }

    if (TRACE_LEVEL() >= 4) {
        LogMsg_0(0x90003, "rfc_port_timer_stop");
    }

    btu_stop_timer((u8 *)port + 0x70);

    /* Clear rfc.state. */
    *((u8 *)port + 0x68) = 0;

    if (p_mcb != 0) {
        u8 saved_dlci = port->dlci;
        u16 ii;

        p_mcb->port_inx[saved_dlci] = 0;
        port->dlci = 0;

        for (ii = 0; ii < RFCOMM_MAX_DLCI; ii++) {
            if (p_mcb->port_inx[ii] != 0) {
                p_mcb->is_disc_initiator = 0;
                goto done;
            }
        }

        if (p_mcb->is_disc_initiator != 0) {
            p_mcb->is_disc_initiator = 0;
            rfc_mx_sm_execute(p_mcb, 8, 0);
        } else {
            if (TRACE_LEVEL() >= 4) {
                LogMsg_1(0x90003, "rfc_timer_start - timeout:%d", 2);
            }
            ((TimerEntry *)p_mcb->tle_0x00)->param = (u32)p_mcb;
            btu_start_timer(p_mcb->tle_0x00, 0x0B, 2);
        }
    }

done:
    port_rfc_closed(port, 0x13);
}

/* ================================================================== */
/*  rfc_inc_credit                                                     */
/* ================================================================== */
void rfc_inc_credit(struct RfcPortStruct *port, u8 credit)
{
    if (port->p_mcb->flow == 2) {
        port->credit_tx += credit;

        if (TRACE_LEVEL() >= 4) {
            LogMsg_1(0x90003, "rfc_inc_credit:%d", port->credit_tx);
        }

        if (port->state == 1) {
            PORT_FlowInd(port->p_mcb, port->dlci, 1);
        }
    }
}

/* ================================================================== */
/*  rfc_dec_credit                                                     */
/* ================================================================== */
void rfc_dec_credit(struct RfcPortStruct *port)
{
    struct RfcMuxChannel *p_mcb = port->p_mcb;

    if (p_mcb->flow != 2) {
        return;
    }

    if (port->credit_tx != 0) {
        port->credit_tx--;
    }

    if (port->credit_tx == 0) {
        port->state = 1;
    }
}

/* ================================================================== */
/*  rfc_check_send_cmd                                                 */
/* ================================================================== */
void rfc_check_send_cmd(struct RfcMuxChannel *p, void *p_buf)
{
    if (p_buf != 0) {
        GKI_enqueue(p->cmd_q, p_buf);
    }

    while (!p->l2cap_congested) {
        void *buf = GKI_dequeue(p->cmd_q);
        if (buf == 0) {
            break;
        }
        L2CA_DataWrite(p->lcid, buf);
    }
}
