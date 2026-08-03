// High-level C reconstruction of RVL_SDK/src/revolution/bte/stack/rfcomm/rfc_ts_frames
//
// Broadcom BTE RFCOMM TS 07.10 frame handling (Apache-2.0).  The retail
// object matches the upstream BTE/Bluedroid rfc_ts_frames.c semantics with
// the older (pre-Bluedroid) MX_FRAME layout: the pn/msc/rpn/rls union
// members carry a leading dlci byte, and the NSC response is emitted
// inline at the end of rfc_process_mx_message.
//
// NOTE: this TU is compiled with `-ipa off`, which makes MWCC emit functions
// in REVERSE source order (and pool the .data string literals in that same
// reversed order).  The functions below are therefore written back-to-front
// (rfc_process_mx_message first, rfc_send_sabme last) so the emitted .text
// and string pool match the retail layout (same fix as l2c_csm.c).

#include <harness_catalog.h>

#include "revolution/bte/stack/include/rfcdefs.h"

/* ------------------------------------------------------------------ */
/*  Constants not present in the in-repo SDK headers                   */
/* ------------------------------------------------------------------ */
#define L2CAP_MIN_OFFSET            9
#define RFCOMM_CMD_POOL_ID          2

#define RFCOMM_MIN_MTU              23
#define RFCOMM_MAX_MTU              32767

#define PORT_FC_CREDIT              2

#define MODEM_SIGNAL_DTRDSR         0x01
#define MODEM_SIGNAL_RTSCTS         0x02
#define MODEM_SIGNAL_RI             0x04
#define MODEM_SIGNAL_DCD            0x08

#define BT_TRACE_LEVEL_ERROR        1
#define TRACE_LAYER_RFCOMM          0x00090000

/* RFCOMM multiplexer events returned by rfc_parse_data */
#define RFC_EVENT_SABME             0
#define RFC_EVENT_UA                1
#define RFC_EVENT_DM                2
#define RFC_EVENT_DISC              3
#define RFC_EVENT_UIH               4
#define RFC_EVENT_BAD_FRAME         50

/* FCS helpers (defined in rfc_utils.c) */
#define RFCOMM_SABME_FCS(p, cr, dlci)   rfc_calc_fcs(3, (p))
#define RFCOMM_UA_FCS(p, cr, dlci)      rfc_calc_fcs(3, (p))
#define RFCOMM_DM_FCS(p, cr, dlci)      rfc_calc_fcs(3, (p))
#define RFCOMM_DISC_FCS(p, cr, dlci)    rfc_calc_fcs(3, (p))
#define RFCOMM_UIH_FCS(p, dlci)         rfc_calc_fcs(2, (p))

/* ------------------------------------------------------------------ */
/*  Types                                                              */
/* ------------------------------------------------------------------ */
typedef struct {
    u16 event;
    u16 len;
    u16 offset;
    u16 layer_specific;
} BT_HDR;

/* Received TS 07.10 frame (retail layout, 0x14 bytes). */
typedef struct {
    u8 dlci;                    /* 0x00 */
    u8 type;                    /* 0x01 */
    u8 cr;                      /* 0x02 */
    u8 ea;                      /* 0x03 */
    u8 pf;                      /* 0x04 */
    u8 credit;                  /* 0x05 */
    u8 pad_06[2];
    union {
        struct {
            u8 dlci;            /* 0x08 */
            u8 frame_type;      /* 0x09 */
            u8 conv_layer;      /* 0x0A */
            u8 priority;        /* 0x0B */
            u8 t1;              /* 0x0C */
            u8 pad_0d;
            u16 mtu;            /* 0x0E */
            u8 n2;              /* 0x10 */
            u8 k;               /* 0x11 */
        } pn;
        struct {
            u8 ea;              /* 0x08 */
            u8 cr;              /* 0x09 */
            u8 type;            /* 0x0A */
        } nsc;
        struct {
            u8 dlci;            /* 0x08 */
            u8 signals;         /* 0x09 */
            u8 break_present;   /* 0x0A */
            u8 break_duration;  /* 0x0B */
        } msc;
        struct {
            u8 dlci;            /* 0x08 */
            u8 is_request;      /* 0x09 */
            u8 baud_rate;       /* 0x0A */
            u8 byte_size;       /* 0x0B */
            u8 stop_bits;       /* 0x0C */
            u8 parity;          /* 0x0D */
            u8 parity_type;     /* 0x0E */
            u8 fc_type;         /* 0x0F */
            u8 xon_char;        /* 0x10 */
            u8 xoff_char;       /* 0x11 */
            u16 param_mask;     /* 0x12 */
        } rpn;
        struct {
            u8 *p_data;         /* 0x08 */
            u16 data_len;       /* 0x0C */
        } test;
        struct {
            u8 dlci;            /* 0x08 */
            u8 line_status;     /* 0x09 */
        } rls;
    } u;
} MX_FRAME;

/* Multiplexer control block (only fields touched by this TU). */
typedef struct {
    u8 pad_00[0x68];
    u16 lcid;                   /* 0x68 */
    u8 pad_6a[3];
    u8 is_initiator;            /* 0x6D */
    u8 pad_6e[4];
    u8 flow;                    /* 0x72 */
} tRFC_MCB;

/* RFCOMM control block: rx_frame first, trace level at 0x414. */
typedef struct {
    MX_FRAME rx_frame;          /* 0x000 */
    u8 pad_014[0x400];
    u8 trace_level;             /* 0x414 */
} tRFC_CB;

/* Modem Status command parameters (rfc_send_msc). */
typedef struct {
    u8 modem_signal;            /* 0x00 */
    u8 break_signal;            /* 0x01 */
    u8 pad_02[2];
    u8 fc;                      /* 0x04 */
} tPORT_CTRL;

/* Remote Port Negotiation parameters (rfc_send_rpn). */
typedef struct {
    u8 baud_rate;               /* 0x00 */
    u8 byte_size;               /* 0x01 */
    u8 stop_bits;               /* 0x02 */
    u8 parity;                  /* 0x03 */
    u8 parity_type;             /* 0x04 */
    u8 fc_type;                 /* 0x05 */
    u8 pad_06;
    u8 xon_char;                /* 0x07 */
    u8 xoff_char;               /* 0x08 */
} tPORT_STATE;

/* ------------------------------------------------------------------ */
/*  Externals                                                          */
/* ------------------------------------------------------------------ */
extern tRFC_CB rfc_cb;

extern void *GKI_getpoolbuf(u8 pool_id);
extern void GKI_freebuf(void *p_buf);
extern u16 L2CA_DataWrite(u16 cid, BT_HDR *p_buf);

/* Short SDA trace strings: fixed-size externs keep MWCC on sda21 addressing
** and give the retail reloc names (lbl_80665958/lbl_80665960), same fix as
** btm_devctl.c's lbl_8066592C/34 (MWCC_REFERENCE §1a SDA globals). */
// Defined here (retail .sdata 0x0/0x8, 8 bytes each); non-const so they land
// in .sdata rather than .sdata2.
char lbl_80665958[8] = "Bad UA";
char lbl_80665960[8] = "Bad DM";

extern void LogMsg_0(u32 level, const char *msg);
extern void LogMsg_1(u32 level, const char *msg, u32 p1);
extern void LogMsg_2(u32 level, const char *msg, u32 p1, u32 p2);

extern u8 rfc_calc_fcs(u16 len, u8 *p);
extern u8 rfc_check_fcs(u16 len, u8 *p, u8 received_fcs);
extern void rfc_check_send_cmd(tRFC_MCB *p_mcb, BT_HDR *p_buf);

extern void rfc_process_pn(tRFC_MCB *p_mcb, u8 is_command, MX_FRAME *p_frame);
extern void rfc_process_test_rsp(tRFC_MCB *p_mcb, BT_HDR *p_buf);
extern void rfc_process_fcon(tRFC_MCB *p_mcb, u8 is_command);
extern void rfc_process_fcoff(tRFC_MCB *p_mcb, u8 is_command);
extern void rfc_process_msc(tRFC_MCB *p_mcb, u8 is_command, MX_FRAME *p_frame);
extern void rfc_process_nsc(tRFC_MCB *p_mcb, MX_FRAME *p_frame);
extern void rfc_process_rpn(tRFC_MCB *p_mcb, u8 is_command, u8 is_request, MX_FRAME *p_frame);
extern void rfc_process_rls(tRFC_MCB *p_mcb, u8 is_command, MX_FRAME *p_frame);

/* Forward declarations (functions defined later in this back-to-front file). */
void rfc_send_buf_uih(tRFC_MCB *p_mcb, u8 dlci, BT_HDR *p_buf);
void rfc_send_test(tRFC_MCB *p_mcb, u8 is_command, BT_HDR *p_buf);

#define RFCOMM_TRACE_ERROR0(m) \
    { if (rfc_cb.trace_level >= BT_TRACE_LEVEL_ERROR) LogMsg_0(TRACE_LAYER_RFCOMM, (m)); }
#define RFCOMM_TRACE_ERROR1(m, p1) \
    { if (rfc_cb.trace_level >= BT_TRACE_LEVEL_ERROR) LogMsg_1(TRACE_LAYER_RFCOMM, (m), (p1)); }
#define RFCOMM_TRACE_ERROR2(m, p1, p2) \
    { if (rfc_cb.trace_level >= BT_TRACE_LEVEL_ERROR) LogMsg_2(TRACE_LAYER_RFCOMM, (m), (p1), (p2)); }


/*******************************************************************************
**
** Function         rfc_process_mx_message
**
** Description      This function processes UIH frames received on the
**                  multiplexer control channel.
**
*******************************************************************************/
void rfc_process_mx_message(tRFC_MCB *p_mcb, BT_HDR *p_buf)
{
    u8 *p_data = (u8 *)(p_buf + 1) + p_buf->offset;
    MX_FRAME *p_rx_frame = &rfc_cb.rx_frame;
    u16 length = p_buf->len;
    u8 ea, cr, mx_len;
    u8 is_command;

    p_rx_frame->ea   = *p_data & RFCOMM_EA;
    p_rx_frame->cr   = (*p_data & RFCOMM_CR_MASK) >> RFCOMM_SHIFT_CR;
    p_rx_frame->type = *p_data++ & ~(RFCOMM_CR_MASK | RFCOMM_EA_MASK);

    if (!p_rx_frame->ea || !length)
    {
        RFCOMM_TRACE_ERROR2("Illegal MX Frame ea:%d len:%d", p_rx_frame->ea, length);
        GKI_freebuf(p_buf);
        return;
    }

    length--;

    is_command = p_rx_frame->cr;

    ea = *p_data & RFCOMM_EA;

    mx_len = *p_data++ >> RFCOMM_SHIFT_LENGTH1;
    length--;

    if (!ea)
    {
        mx_len += *p_data++ << RFCOMM_SHIFT_LENGTH2;
        length--;
    }

    if (mx_len != length)
    {
        RFCOMM_TRACE_ERROR0("Bad MX frame");
        GKI_freebuf(p_buf);
        return;
    }

    switch (p_rx_frame->type)
    {
    case RFCOMM_MX_PN:
        if (length != RFCOMM_MX_PN_LEN)
            break;

        p_rx_frame->dlci            = *p_data++ & RFCOMM_PN_DLCI_MASK;
        p_rx_frame->u.pn.frame_type = *p_data & RFCOMM_PN_FRAME_TYPE_MASK;
        p_rx_frame->u.pn.conv_layer = *p_data++ & RFCOMM_PN_CONV_LAYER_MASK;
        p_rx_frame->u.pn.priority   = *p_data++ & RFCOMM_PN_PRIORITY_MASK;
        p_rx_frame->u.pn.t1         = *p_data++;
        p_rx_frame->u.pn.mtu        = *p_data + (*(p_data + 1) << 8);
        p_data += 2;
        p_rx_frame->u.pn.n2         = *p_data++;
        p_rx_frame->u.pn.k          = *p_data++ & RFCOMM_PN_K_MASK;

        if (!p_rx_frame->dlci
         || !RFCOMM_VALID_DLCI(p_rx_frame->dlci)
         || (p_rx_frame->u.pn.mtu < RFCOMM_MIN_MTU)
         || (p_rx_frame->u.pn.mtu > RFCOMM_MAX_MTU))
        {
            RFCOMM_TRACE_ERROR0("Bad PN frame");
            break;
        }

        GKI_freebuf(p_buf);

        rfc_process_pn(p_mcb, is_command, p_rx_frame);
        return;

    case RFCOMM_MX_TEST:
        if (!length)
            break;

        p_rx_frame->u.test.p_data   = p_data;
        p_rx_frame->u.test.data_len = length;

        p_buf->offset += 2;
        p_buf->len    -= 2;

        if (is_command)
            rfc_send_test(p_mcb, FALSE, p_buf);
        else
            rfc_process_test_rsp(p_mcb, p_buf);
        return;

    case RFCOMM_MX_FCON:
        if (length != RFCOMM_MX_FCON_LEN)
            break;

        GKI_freebuf(p_buf);

        rfc_process_fcon(p_mcb, is_command);
        return;

    case RFCOMM_MX_FCOFF:
        if (length != RFCOMM_MX_FCOFF_LEN)
            break;

        GKI_freebuf(p_buf);

        rfc_process_fcoff(p_mcb, is_command);
        return;

    case RFCOMM_MX_MSC:
        ea               = *p_data & RFCOMM_EA;
        cr               = (*p_data & RFCOMM_CR_MASK) >> RFCOMM_SHIFT_CR;
        p_rx_frame->dlci = *p_data++ >> RFCOMM_SHIFT_DLCI;

        if (!ea || !cr || !p_rx_frame->dlci
         || !RFCOMM_VALID_DLCI(p_rx_frame->dlci))
        {
            RFCOMM_TRACE_ERROR0("Bad MSC frame");
            break;
        }

        p_rx_frame->u.msc.signals = *p_data++;

        if (mx_len == RFCOMM_MX_MSC_LEN_WITH_BREAK)
        {
            p_rx_frame->u.msc.break_present  = *p_data & RFCOMM_MSC_BREAK_PRESENT_MASK;
            p_rx_frame->u.msc.break_duration = (*p_data & RFCOMM_MSC_BREAK_MASK) >> RFCOMM_MSC_SHIFT_BREAK;
        }
        else
        {
            p_rx_frame->u.msc.break_present  = FALSE;
            p_rx_frame->u.msc.break_duration = 0;
        }
        GKI_freebuf(p_buf);

        rfc_process_msc(p_mcb, is_command, p_rx_frame);
        return;

    case RFCOMM_MX_NSC:
        if ((length != RFCOMM_MX_NSC_LEN) || !is_command)
            break;

        p_rx_frame->u.nsc.ea   = *p_data & RFCOMM_EA;
        p_rx_frame->u.nsc.cr   = (*p_data & RFCOMM_CR_MASK) >> RFCOMM_SHIFT_CR;
        p_rx_frame->u.nsc.type = *p_data++ >> RFCOMM_SHIFT_DLCI;

        GKI_freebuf(p_buf);

        rfc_process_nsc(p_mcb, p_rx_frame);
        return;

    case RFCOMM_MX_RPN:
        if ((length != RFCOMM_MX_RPN_REQ_LEN) && (length != RFCOMM_MX_RPN_LEN))
            break;

        ea               = *p_data & RFCOMM_EA;
        cr               = (*p_data & RFCOMM_CR_MASK) >> RFCOMM_SHIFT_CR;
        p_rx_frame->dlci = *p_data++ >> RFCOMM_SHIFT_DLCI;

        if (!ea || !cr || !p_rx_frame->dlci
         || !RFCOMM_VALID_DLCI(p_rx_frame->dlci))
        {
            RFCOMM_TRACE_ERROR0("Bad RPN frame");
            break;
        }

        p_rx_frame->u.rpn.is_request = (length == RFCOMM_MX_RPN_REQ_LEN);

        if (!p_rx_frame->u.rpn.is_request)
        {
            p_rx_frame->u.rpn.baud_rate   = *p_data++;
            p_rx_frame->u.rpn.byte_size   = (*p_data >> RFCOMM_RPN_BITS_SHIFT) & RFCOMM_RPN_BITS_MASK;
            p_rx_frame->u.rpn.stop_bits   = (*p_data >> RFCOMM_RPN_STOP_BITS_SHIFT) & RFCOMM_RPN_STOP_BITS_MASK;
            p_rx_frame->u.rpn.parity      = (*p_data >> RFCOMM_RPN_PARITY_SHIFT) & RFCOMM_RPN_PARITY_MASK;
            p_rx_frame->u.rpn.parity_type = (*p_data++ >> RFCOMM_RPN_PARITY_TYPE_SHIFT) & RFCOMM_RPN_PARITY_TYPE_MASK;

            p_rx_frame->u.rpn.fc_type     = *p_data++ & RFCOMM_FC_MASK;
            p_rx_frame->u.rpn.xon_char    = *p_data++;
            p_rx_frame->u.rpn.xoff_char   = *p_data++;
            p_rx_frame->u.rpn.param_mask  = (*p_data + (*(p_data + 1) << 8)) & RFCOMM_RPN_PM_MASK;
        }
        GKI_freebuf(p_buf);

        rfc_process_rpn(p_mcb, is_command, p_rx_frame->u.rpn.is_request, p_rx_frame);
        return;

    case RFCOMM_MX_RLS:
        if (length != RFCOMM_MX_RLS_LEN)
            break;

        ea = *p_data & RFCOMM_EA;
        cr = (*p_data & RFCOMM_CR_MASK) >> RFCOMM_SHIFT_CR;

        p_rx_frame->dlci              = *p_data++ >> RFCOMM_SHIFT_DLCI;
        p_rx_frame->u.rls.line_status = (*p_data & ~0x01);

        if (!ea || !cr || !p_rx_frame->dlci
         || !RFCOMM_VALID_DLCI(p_rx_frame->dlci))
        {
            RFCOMM_TRACE_ERROR0("Bad RPN frame");
            break;
        }

        GKI_freebuf(p_buf);

        rfc_process_rls(p_mcb, is_command, p_rx_frame);
        return;
    }

    GKI_freebuf(p_buf);

    /* Unsupported command: reply with NSC carrying the offending control
    ** field (rfc_send_nsc semantics, hand-inlined: MWCC emits every static
    ** function standalone, so the helper must not exist in source). */
    if (is_command)
    {
        BT_HDR *p_nsc;
        u8 *p_nsc_data;

        if ((p_nsc = (BT_HDR *)GKI_getpoolbuf(RFCOMM_CMD_POOL_ID)) != NULL)
        {
            p_nsc->offset = L2CAP_MIN_OFFSET + RFCOMM_CTRL_FRAME_LEN;
            p_nsc_data = (u8 *)(p_nsc + 1) + p_nsc->offset;

            *p_nsc_data++ = RFCOMM_EA | RFCOMM_I_CR(FALSE) | RFCOMM_MX_NSC;
            *p_nsc_data++ = RFCOMM_EA | (RFCOMM_MX_NSC_LEN << 1);

            *p_nsc_data = rfc_cb.rx_frame.ea |
                          (rfc_cb.rx_frame.cr << RFCOMM_SHIFT_CR) |
                          rfc_cb.rx_frame.type;

            /* Total length is size of NSC data + mx header 2 */
            p_nsc->len = RFCOMM_MX_NSC_LEN + 2;

            rfc_send_buf_uih(p_mcb, RFCOMM_MX_DLCI, p_nsc);
        }
    }
}


/*******************************************************************************
**
** Function         rfc_parse_data
**
** Description      This function processes data packet received from L2CAP
**
*******************************************************************************/
u8 rfc_parse_data(tRFC_MCB *p_mcb, MX_FRAME *p_frame, BT_HDR *p_buf)
{
    u8 ead, eal, fcs;
    u8 *p_data = (u8 *)(p_buf + 1) + p_buf->offset;
    u8 *p_start = p_data;
    u16 len;

    if (p_buf->len < RFCOMM_CTRL_FRAME_LEN)
    {
        RFCOMM_TRACE_ERROR1("Bad Length1: %d", p_buf->len);
        return (RFC_EVENT_BAD_FRAME);
    }

    RFCOMM_PARSE_CTRL_FIELD(ead, p_frame->cr, p_frame->dlci, p_data);
    RFCOMM_PARSE_TYPE_FIELD(p_frame->type, p_frame->pf, p_data);
    RFCOMM_PARSE_LEN_FIELD(eal, len, p_data);

    p_buf->len    -= (3 + !ead + !eal + 1);  /* Additional 1 for FCS */
    p_buf->offset += (3 + !ead + !eal);

    /* handle credit if credit based flow control */
    if ((p_mcb->flow == PORT_FC_CREDIT) && (p_frame->type == RFCOMM_UIH) &&
        (p_frame->dlci != RFCOMM_MX_DLCI) && (p_frame->pf == 1))
    {
        p_frame->credit = *p_data++;
        p_buf->len--;
        p_buf->offset++;
    }
    else
        p_frame->credit = 0;

    if (p_buf->len != len)
    {
        RFCOMM_TRACE_ERROR2("Bad Length2 %d %d", p_buf->len, len);
        return (RFC_EVENT_BAD_FRAME);
    }

    fcs = *(p_data + len);

    /* All control frames that we are sending are sent with P=1, expect */
    /* reply with F=1 */
    /* According to TS 07.10 spec invalid frames are discarded without */
    /* notification to the sender */
    switch (p_frame->type)
    {
    case RFCOMM_SABME:
        if (RFCOMM_FRAME_IS_RSP(p_mcb->is_initiator, p_frame->cr)
         || !p_frame->pf || len || !RFCOMM_VALID_DLCI(p_frame->dlci)
         || !rfc_check_fcs(RFCOMM_CTRL_FRAME_LEN, p_start, fcs))
        {
            RFCOMM_TRACE_ERROR0("Bad SABME");
            return (RFC_EVENT_BAD_FRAME);
        }
        else
            return (RFC_EVENT_SABME);

    case RFCOMM_UA:
        if (RFCOMM_FRAME_IS_CMD(p_mcb->is_initiator, p_frame->cr)
         || !p_frame->pf || len || !RFCOMM_VALID_DLCI(p_frame->dlci)
         || !rfc_check_fcs(RFCOMM_CTRL_FRAME_LEN, p_start, fcs))
        {
            RFCOMM_TRACE_ERROR0(lbl_80665958);
            return (RFC_EVENT_BAD_FRAME);
        }
        else
            return (RFC_EVENT_UA);

    case RFCOMM_DM:
        if (RFCOMM_FRAME_IS_CMD(p_mcb->is_initiator, p_frame->cr)
         || len || !RFCOMM_VALID_DLCI(p_frame->dlci)
         || !rfc_check_fcs(RFCOMM_CTRL_FRAME_LEN, p_start, fcs))
        {
            RFCOMM_TRACE_ERROR0(lbl_80665960);
            return (RFC_EVENT_BAD_FRAME);
        }
        else
            return (RFC_EVENT_DM);

    case RFCOMM_DISC:
        if (RFCOMM_FRAME_IS_RSP(p_mcb->is_initiator, p_frame->cr)
         || !p_frame->pf || len || !RFCOMM_VALID_DLCI(p_frame->dlci)
         || !rfc_check_fcs(RFCOMM_CTRL_FRAME_LEN, p_start, fcs))
        {
            RFCOMM_TRACE_ERROR0("Bad DISC");
            return (RFC_EVENT_BAD_FRAME);
        }
        else
            return (RFC_EVENT_DISC);

    case RFCOMM_UIH:
        if (!RFCOMM_VALID_DLCI(p_frame->dlci))
        {
            RFCOMM_TRACE_ERROR0("Bad UIH - invalid DLCI");
            return (RFC_EVENT_BAD_FRAME);
        }
        else if (!rfc_check_fcs(2, p_start, fcs))
        {
            RFCOMM_TRACE_ERROR0("Bad UIH - FCS");
            return (RFC_EVENT_BAD_FRAME);
        }
        else if (RFCOMM_FRAME_IS_RSP(p_mcb->is_initiator, p_frame->cr))
        {
            /* we assume that this is ok to allow bad implementations to work */
            RFCOMM_TRACE_ERROR0("Bad UIH - response");
            return (RFC_EVENT_UIH);
        }
        else
            return (RFC_EVENT_UIH);
    }

    return (RFC_EVENT_BAD_FRAME);
}


/*******************************************************************************
**
** Function         rfc_send_credit
**
** Description      This function sends a flow control credit in UIH frame.
**
*******************************************************************************/
void rfc_send_credit(tRFC_MCB *p_mcb, u8 dlci, u8 credit)
{
    BT_HDR *p_buf;
    u8 *p_data;
    u8 cr = RFCOMM_CR(p_mcb->is_initiator, TRUE);

    if ((p_buf = (BT_HDR *)GKI_getpoolbuf(RFCOMM_CMD_POOL_ID)) == NULL)
        return;

    p_buf->offset = L2CAP_MIN_OFFSET;
    p_data = (u8 *)(p_buf + 1) + p_buf->offset;

    *p_data++ = RFCOMM_EA | cr | (dlci << RFCOMM_SHIFT_DLCI);
    *p_data++ = RFCOMM_UIH | RFCOMM_PF;
    *p_data++ = RFCOMM_EA | 0;
    *p_data++ = credit;
    *p_data   = RFCOMM_UIH_FCS((u8 *)(p_buf + 1) + p_buf->offset, dlci);

    p_buf->len = 5;

    rfc_check_send_cmd(p_mcb, p_buf);
}


/*******************************************************************************
**
** Function         rfc_send_test
**
** Description      This function sends Test frame.
**
*******************************************************************************/
void rfc_send_test(tRFC_MCB *p_mcb, u8 is_command, BT_HDR *p_buf)
{
    u8 *p_data;
    u16 xx;
    u8 *p_src, *p_dest;

    /* Shift buffer to give space for header */
    if (p_buf->offset < (L2CAP_MIN_OFFSET + RFCOMM_MIN_OFFSET + 2))
    {
        p_src  = (u8 *)(p_buf + 1) + p_buf->offset + p_buf->len - 1;
        p_dest = (u8 *)(p_buf + 1) + L2CAP_MIN_OFFSET + RFCOMM_MIN_OFFSET + 2 + p_buf->len - 1;

        for (xx = 0; xx < p_buf->len; xx++)
            *p_dest-- = *p_src--;

        p_buf->offset = L2CAP_MIN_OFFSET + RFCOMM_MIN_OFFSET + 2;
    }

    /* Adjust offset by number of bytes we are going to fill */
    p_buf->offset -= 2;
    p_data = (u8 *)(p_buf + 1) + p_buf->offset;

    *p_data++ = RFCOMM_EA | RFCOMM_I_CR(is_command) | RFCOMM_MX_TEST;
    *p_data++ = RFCOMM_EA | (p_buf->len << 1);

    p_buf->len += 2;

    rfc_send_buf_uih(p_mcb, RFCOMM_MX_DLCI, p_buf);
}


/*******************************************************************************
**
** Function         rfc_send_rpn
**
** Description      This function sends Remote Port Negotiation Command
**
*******************************************************************************/
void rfc_send_rpn(tRFC_MCB *p_mcb, u8 dlci, u8 is_command, tPORT_STATE *p_pars, u16 mask)
{
    BT_HDR *p_buf;
    u8 *p_data;

    if ((p_buf = (BT_HDR *)GKI_getpoolbuf(RFCOMM_CMD_POOL_ID)) == NULL)
        return;

    p_buf->offset = L2CAP_MIN_OFFSET + RFCOMM_CTRL_FRAME_LEN;
    p_data = (u8 *)(p_buf + 1) + p_buf->offset;

    *p_data++ = RFCOMM_EA | RFCOMM_I_CR(is_command) | RFCOMM_MX_RPN;

    if (!p_pars)
    {
        *p_data++ = RFCOMM_EA | (RFCOMM_MX_RPN_REQ_LEN << 1);

        *p_data++ = RFCOMM_EA | RFCOMM_CR_MASK | (dlci << RFCOMM_SHIFT_DLCI);

        p_buf->len = RFCOMM_MX_RPN_REQ_LEN + 2;
    }
    else
    {
        *p_data++ = RFCOMM_EA | (RFCOMM_MX_RPN_LEN << 1);

        *p_data++ = RFCOMM_EA | RFCOMM_CR_MASK | (dlci << RFCOMM_SHIFT_DLCI);
        *p_data++ = p_pars->baud_rate;
        *p_data++ = (p_pars->byte_size << RFCOMM_RPN_BITS_SHIFT)
                  | (p_pars->stop_bits << RFCOMM_RPN_STOP_BITS_SHIFT)
                  | (p_pars->parity << RFCOMM_RPN_PARITY_SHIFT)
                  | (p_pars->parity_type << RFCOMM_RPN_PARITY_TYPE_SHIFT);
        *p_data++ = p_pars->fc_type;
        *p_data++ = p_pars->xon_char;
        *p_data++ = p_pars->xoff_char;
        *p_data++ = (mask & 0xFF);
        *p_data++ = (mask >> 8);

        /* Total length is size of RPN data + mx header 2 */
        p_buf->len = RFCOMM_MX_RPN_LEN + 2;
    }

    rfc_send_buf_uih(p_mcb, RFCOMM_MX_DLCI, p_buf);
}


/*******************************************************************************
**
** Function         rfc_send_rls
**
** Description      This function sends Remote Line Status Command Frame.
**
*******************************************************************************/
void rfc_send_rls(tRFC_MCB *p_mcb, u8 dlci, u8 is_command, u8 status)
{
    BT_HDR *p_buf;
    u8 *p_data;

    if ((p_buf = (BT_HDR *)GKI_getpoolbuf(RFCOMM_CMD_POOL_ID)) == NULL)
        return;

    p_buf->offset = L2CAP_MIN_OFFSET + RFCOMM_CTRL_FRAME_LEN;
    p_data = (u8 *)(p_buf + 1) + p_buf->offset;

    *p_data++ = RFCOMM_EA | RFCOMM_I_CR(is_command) | RFCOMM_MX_RLS;
    *p_data++ = RFCOMM_EA | (RFCOMM_MX_RLS_LEN << 1);

    *p_data++ = RFCOMM_EA | RFCOMM_CR_MASK | (dlci << RFCOMM_SHIFT_DLCI);
    *p_data++ = RFCOMM_RLS_ERROR | status;

    /* Total length is size of RLS data + mx header 2 */
    p_buf->len = RFCOMM_MX_RLS_LEN + 2;

    rfc_send_buf_uih(p_mcb, RFCOMM_MX_DLCI, p_buf);
}


/*******************************************************************************
**
** Function         rfc_send_msc
**
** Description      This function sends Modem Status Command Frame.
**
*******************************************************************************/
void rfc_send_msc(tRFC_MCB *p_mcb, u8 dlci, u8 is_command, tPORT_CTRL *p_pars)
{
    BT_HDR *p_buf;
    u8 *p_data;
    u8 signals;
    u8 break_duration;
    u8 len;

    signals        = p_pars->modem_signal;
    break_duration = p_pars->break_signal;

    if ((p_buf = (BT_HDR *)GKI_getpoolbuf(RFCOMM_CMD_POOL_ID)) == NULL)
        return;

    p_buf->offset = L2CAP_MIN_OFFSET + RFCOMM_CTRL_FRAME_LEN;
    p_data = (u8 *)(p_buf + 1) + p_buf->offset;

    if (break_duration)
        len = RFCOMM_MX_MSC_LEN_WITH_BREAK;
    else
        len = RFCOMM_MX_MSC_LEN_NO_BREAK;

    *p_data++ = RFCOMM_EA | RFCOMM_I_CR(is_command) | RFCOMM_MX_MSC;
    *p_data++ = RFCOMM_EA | (len << 1);

    *p_data++ = RFCOMM_EA | RFCOMM_CR_MASK | (dlci << RFCOMM_SHIFT_DLCI);
    *p_data++ = RFCOMM_EA |
                ((p_pars->fc)                    ? RFCOMM_MSC_FC  : 0) |
                ((signals & MODEM_SIGNAL_DTRDSR) ? RFCOMM_MSC_RTC : 0) |
                ((signals & MODEM_SIGNAL_RTSCTS) ? RFCOMM_MSC_RTR : 0) |
                ((signals & MODEM_SIGNAL_RI)     ? RFCOMM_MSC_IC  : 0) |
                ((signals & MODEM_SIGNAL_DCD)    ? RFCOMM_MSC_DV  : 0);

    if (break_duration)
    {
        *p_data++ = RFCOMM_EA | RFCOMM_MSC_BREAK_PRESENT_MASK |
                    (break_duration << RFCOMM_MSC_SHIFT_BREAK);
    }

    /* Total length is size of MSC data + mx header 2 */
    p_buf->len = len + 2;

    rfc_send_buf_uih(p_mcb, RFCOMM_MX_DLCI, p_buf);
}


/*******************************************************************************
**
** Function         rfc_send_fcoff
**
** Description      This function sends Flow Control Off Command.
**
*******************************************************************************/
void rfc_send_fcoff(tRFC_MCB *p_mcb, u8 is_command)
{
    BT_HDR *p_buf;
    u8 *p_data;

    if ((p_buf = (BT_HDR *)GKI_getpoolbuf(RFCOMM_CMD_POOL_ID)) == NULL)
        return;

    p_buf->offset = L2CAP_MIN_OFFSET + RFCOMM_CTRL_FRAME_LEN;
    p_data = (u8 *)(p_buf + 1) + p_buf->offset;

    *p_data++ = RFCOMM_EA | RFCOMM_I_CR(is_command) | RFCOMM_MX_FCOFF;
    *p_data++ = RFCOMM_EA | (RFCOMM_MX_FCOFF_LEN << 1);

    /* Total length is size of FCOFF data + mx header 2 */
    p_buf->len = RFCOMM_MX_FCOFF_LEN + 2;

    rfc_send_buf_uih(p_mcb, RFCOMM_MX_DLCI, p_buf);
}


/*******************************************************************************
**
** Function         rfc_send_fcon
**
** Description      This function sends Flow Control On Command.
**
*******************************************************************************/
void rfc_send_fcon(tRFC_MCB *p_mcb, u8 is_command)
{
    BT_HDR *p_buf;
    u8 *p_data;

    if ((p_buf = (BT_HDR *)GKI_getpoolbuf(RFCOMM_CMD_POOL_ID)) == NULL)
        return;

    p_buf->offset = L2CAP_MIN_OFFSET + RFCOMM_CTRL_FRAME_LEN;
    p_data = (u8 *)(p_buf + 1) + p_buf->offset;

    *p_data++ = RFCOMM_EA | RFCOMM_I_CR(is_command) | RFCOMM_MX_FCON;
    *p_data++ = RFCOMM_EA | (RFCOMM_MX_FCON_LEN << 1);

    /* Total length is size of FCON data + mx header 2 */
    p_buf->len = RFCOMM_MX_FCON_LEN + 2;

    rfc_send_buf_uih(p_mcb, RFCOMM_MX_DLCI, p_buf);
}


/*******************************************************************************
**
** Function         rfc_send_pn
**
** Description      This function sends DLC Parameters Negotiation Frame.
**
*******************************************************************************/
void rfc_send_pn(tRFC_MCB *p_mcb, u8 dlci, u8 is_command, u16 mtu, u8 cl, u8 k)
{
    BT_HDR *p_buf;
    u8 *p_data;

    if ((p_buf = (BT_HDR *)GKI_getpoolbuf(RFCOMM_CMD_POOL_ID)) == NULL)
        return;

    p_buf->offset = L2CAP_MIN_OFFSET + RFCOMM_CTRL_FRAME_LEN;
    p_data = (u8 *)(p_buf + 1) + p_buf->offset;

    *p_data++ = RFCOMM_EA | RFCOMM_I_CR(is_command) | RFCOMM_MX_PN;
    *p_data++ = RFCOMM_EA | (RFCOMM_MX_PN_LEN << 1);

    *p_data++ = dlci;
    *p_data++ = RFCOMM_PN_FRAM_TYPE_UIH | cl;

    /* It appeared that we need to reply with the same priority bits as we
    ** received.  We will use the fact that we reply in the same context so
    ** rx_frame can still be used.
    */
    if (is_command)
        *p_data++ = RFCOMM_PN_PRIORITY_0;
    else
        *p_data++ = rfc_cb.rx_frame.u.pn.priority;

    *p_data++ = RFCOMM_T1_DSEC;
    *p_data++ = mtu & 0xFF;
    *p_data++ = mtu >> 8;
    *p_data++ = RFCOMM_N2;
    *p_data   = k;

    /* Total length is size of PN data + mx header 2 */
    p_buf->len = RFCOMM_MX_PN_LEN + 2;

    rfc_send_buf_uih(p_mcb, RFCOMM_MX_DLCI, p_buf);
}


/*******************************************************************************
**
** Function         rfc_send_buf_uih
**
** Description      This function sends UIH frame.
**
*******************************************************************************/
void rfc_send_buf_uih(tRFC_MCB *p_mcb, u8 dlci, BT_HDR *p_buf)
{
    u8 *p_data;
    u8 cr = RFCOMM_CR(p_mcb->is_initiator, TRUE);
    u8 credits;

    p_buf->offset -= RFCOMM_CTRL_FRAME_LEN;
    if (p_buf->len > 127)
        p_buf->offset--;

    if (dlci)
        credits = (u8)p_buf->layer_specific;
    else
        credits = 0;

    if (credits)
        p_buf->offset--;

    p_data = (u8 *)(p_buf + 1) + p_buf->offset;

    /* UIH frame, command, PF = 0, dlci */
    *p_data++ = RFCOMM_EA | cr | (dlci << RFCOMM_SHIFT_DLCI);
    *p_data++ = RFCOMM_UIH | ((credits) ? RFCOMM_PF : 0);
    if (p_buf->len <= 127)
    {
        *p_data++ = RFCOMM_EA | (p_buf->len << 1);
        p_buf->len += 3;
    }
    else
    {
        *p_data++ = (p_buf->len & 0x7F) << 1;
        *p_data++ = p_buf->len >> RFCOMM_SHIFT_LENGTH2;
        p_buf->len += 4;
    }

    if (credits)
    {
        *p_data++ = credits;
        p_buf->len++;
    }

    p_data = (u8 *)(p_buf + 1) + p_buf->offset + p_buf->len++;

    *p_data = RFCOMM_UIH_FCS((u8 *)(p_buf + 1) + p_buf->offset, dlci);

    if (dlci == RFCOMM_MX_DLCI)
    {
        rfc_check_send_cmd(p_mcb, p_buf);
    }
    else
    {
        L2CA_DataWrite(p_mcb->lcid, p_buf);
    }
}


/*******************************************************************************
**
** Function         rfc_send_disc
**
** Description      This function sends DISC frame.
**
*******************************************************************************/
void rfc_send_disc(tRFC_MCB *p_mcb, u8 dlci)
{
    BT_HDR *p_buf;
    u8 *p_data;
    u8 cr = RFCOMM_CR(p_mcb->is_initiator, TRUE);

    if ((p_buf = (BT_HDR *)GKI_getpoolbuf(RFCOMM_CMD_POOL_ID)) == NULL)
        return;

    p_buf->offset = L2CAP_MIN_OFFSET;
    p_data = (u8 *)(p_buf + 1) + L2CAP_MIN_OFFSET;

    /* DISC frame, command, PF = 1, dlci */
    *p_data++ = RFCOMM_EA | cr | (dlci << RFCOMM_SHIFT_DLCI);
    *p_data++ = RFCOMM_DISC | RFCOMM_PF;
    *p_data++ = RFCOMM_EA | 0;

    *p_data = RFCOMM_DISC_FCS((u8 *)(p_buf + 1) + L2CAP_MIN_OFFSET, cr, dlci);

    p_buf->len = 4;

    rfc_check_send_cmd(p_mcb, p_buf);
}


/*******************************************************************************
**
** Function         rfc_send_dm
**
** Description      This function sends DM frame.
**
*******************************************************************************/
void rfc_send_dm(tRFC_MCB *p_mcb, u8 dlci, u8 pf)
{
    BT_HDR *p_buf;
    u8 *p_data;
    u8 cr = RFCOMM_CR(p_mcb->is_initiator, FALSE);

    if ((p_buf = (BT_HDR *)GKI_getpoolbuf(RFCOMM_CMD_POOL_ID)) == NULL)
        return;

    p_buf->offset = L2CAP_MIN_OFFSET;
    p_data = (u8 *)(p_buf + 1) + L2CAP_MIN_OFFSET;

    /* DM frame, response, PF = 1, dlci */
    *p_data++ = RFCOMM_EA | cr | (dlci << RFCOMM_SHIFT_DLCI);
    *p_data++ = RFCOMM_DM | ((pf) ? RFCOMM_PF : 0);
    *p_data++ = RFCOMM_EA | 0;

    *p_data = RFCOMM_DM_FCS((u8 *)(p_buf + 1) + L2CAP_MIN_OFFSET, cr, dlci);

    p_buf->len = 4;

    rfc_check_send_cmd(p_mcb, p_buf);
}


/*******************************************************************************
**
** Function         rfc_send_ua
**
** Description      This function sends UA frame.
**
*******************************************************************************/
void rfc_send_ua(tRFC_MCB *p_mcb, u8 dlci)
{
    BT_HDR *p_buf;
    u8 *p_data;
    u8 cr = RFCOMM_CR(p_mcb->is_initiator, FALSE);

    if ((p_buf = (BT_HDR *)GKI_getpoolbuf(RFCOMM_CMD_POOL_ID)) == NULL)
        return;

    p_buf->offset = L2CAP_MIN_OFFSET;
    p_data = (u8 *)(p_buf + 1) + L2CAP_MIN_OFFSET;

    /* ua frame, response, PF = 1, dlci */
    *p_data++ = RFCOMM_EA | cr | (dlci << RFCOMM_SHIFT_DLCI);
    *p_data++ = RFCOMM_UA | RFCOMM_PF;
    *p_data++ = RFCOMM_EA | 0;

    *p_data = RFCOMM_UA_FCS((u8 *)(p_buf + 1) + L2CAP_MIN_OFFSET, cr, dlci);

    p_buf->len = 4;

    rfc_check_send_cmd(p_mcb, p_buf);
}


/*******************************************************************************
**
** Function         rfc_send_sabme
**
** Description      This function sends SABME frame.
**
*******************************************************************************/
void rfc_send_sabme(tRFC_MCB *p_mcb, u8 dlci)
{
    BT_HDR *p_buf;
    u8 *p_data;
    u8 cr = RFCOMM_CR(p_mcb->is_initiator, TRUE);

    if ((p_buf = (BT_HDR *)GKI_getpoolbuf(RFCOMM_CMD_POOL_ID)) == NULL)
        return;

    p_buf->offset = L2CAP_MIN_OFFSET;
    p_data = (u8 *)(p_buf + 1) + L2CAP_MIN_OFFSET;

    /* SABME frame, command, PF = 1, dlci */
    *p_data++ = RFCOMM_EA | cr | (dlci << RFCOMM_SHIFT_DLCI);
    *p_data++ = RFCOMM_SABME | RFCOMM_PF;
    *p_data++ = RFCOMM_EA | 0;

    *p_data = RFCOMM_SABME_FCS((u8 *)(p_buf + 1) + L2CAP_MIN_OFFSET, cr, dlci);

    p_buf->len = 4;

    rfc_check_send_cmd(p_mcb, p_buf);
}

