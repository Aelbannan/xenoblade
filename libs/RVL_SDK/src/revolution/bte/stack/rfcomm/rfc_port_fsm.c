// High-level C reconstruction of RVL_SDK/src/revolution/bte/stack/rfcomm/rfc_port_fsm
// RFCOMM port state machine (Broadcom BTE RFCOMM stack).

#include <harness_catalog.h>

// Last trace string; definition at file end keeps it last in .data.
extern char rfc_pn_disc_str[0x26];
#include <string.h>

#include "revolution/BTE/stack/rfcomm/rfc_int.h"
#include "revolution/BTE/stack/btm/btm_int.h"
#include "revolution/BTE/include/bt_trace.h"

/* ------------------------------------------------------------------ */
/*  Forward declarations                                               */
/* ------------------------------------------------------------------ */
void rfc_port_sm_state_closed(tPORT *p_port, u16 event, void *p_data);
void rfc_port_sm_sabme_wait_ua(tPORT *p_port, u16 event, void *p_data);
void rfc_port_sm_term_wait_sec_check(tPORT *p_port, u16 event, void *p_data);
void rfc_port_sm_orig_wait_sec_check(tPORT *p_port, u16 event, void *p_data);
void rfc_port_sm_opened(tPORT *p_port, u16 event, void *p_data);
void rfc_port_sm_disc_wait_ua(tPORT *p_port, u16 event, void *p_data);
void rfc_set_port_state(tPORT_STATE *port_pars, MX_FRAME *p_frame);

/* ================================================================== */
/*  rfc_port_sm_execute                                               */
/* ================================================================== */
void rfc_port_sm_execute(tPORT *p_port, u16 event, void *p_data)
{
    if (p_port == NULL) {
        RFCOMM_TRACE_WARNING1("NULL port event %d", event);
        return;
    }

    switch (p_port->rfc.state) {
    case RFC_STATE_CLOSED:
        rfc_port_sm_state_closed(p_port, event, p_data);
        break;
    case RFC_STATE_SABME_WAIT_UA:
        rfc_port_sm_sabme_wait_ua(p_port, event, p_data);
        break;
    case RFC_STATE_ORIG_WAIT_SEC_CHECK:
        rfc_port_sm_orig_wait_sec_check(p_port, event, p_data);
        break;
    case RFC_STATE_TERM_WAIT_SEC_CHECK:
        rfc_port_sm_term_wait_sec_check(p_port, event, p_data);
        break;
    case RFC_STATE_OPENED:
        rfc_port_sm_opened(p_port, event, p_data);
        break;
    case RFC_STATE_DISC_WAIT_UA:
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
    case RFC_EVENT_OPEN:               /* 9 - local connect request */
        p_port->rfc.state = RFC_STATE_ORIG_WAIT_SEC_CHECK;
        btm_sec_mx_access_request(p_port->rfc.p_mcb->bd_addr, 3, 1, 3,
                                  (u32)p_port->dlci >> 1, rfc_sec_check_complete, p_port);
        return;
    case RFC_EVENT_CLEAR:         /* 13 */
        return;
    case RFC_EVENT_DATA:                  /* 14 */
        GKI_freebuf(p_data);
        break;
    case RFC_EVENT_SABME:                     /* 0 - remote connect request */
        p_port->rfc.state = RFC_STATE_TERM_WAIT_SEC_CHECK;
        btm_sec_mx_access_request(p_port->rfc.p_mcb->bd_addr, 3, 0, 3,
                                  (u32)p_port->dlci >> 1, rfc_sec_check_complete, p_port);
        return;
    case RFC_EVENT_UA:                        /* 1 */
        return;
    case RFC_EVENT_DM:                        /* 2 */
        rfc_port_closed(p_port);
        return;
    case RFC_EVENT_UIH:                      /* 4 */
        GKI_freebuf(p_data);
        rfc_send_dm(p_port->rfc.p_mcb, p_port->dlci, 0);
        return;
    case RFC_EVENT_DISC:                      /* 3 */
        rfc_send_dm(p_port->rfc.p_mcb, p_port->dlci, 0);
        return;
    case RFC_EVENT_TIMEOUT:                   /* 5 */
        Port_TimeOutCloseMux(p_port->rfc.p_mcb);
        RFCOMM_TRACE_ERROR2("Port error state %d event %d", p_port->rfc.state, event);
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
    case RFC_EVENT_OPEN:               /* 9 */
    case RFC_EVENT_ESTABLISH_RSP:               /* 11 */
        RFCOMM_TRACE_ERROR2("Port error state %d event %d", p_port->rfc.state, event);
        return;
    case RFC_EVENT_CLOSE:               /* 12 - local abort */
        rfc_port_timer_start(p_port, 3);
        rfc_send_disc(p_port->rfc.p_mcb, p_port->dlci);
        p_port->rfc.expected_rsp = 0;
        p_port->rfc.state = RFC_STATE_DISC_WAIT_UA;
        return;
    case RFC_EVENT_CLEAR:         /* 13 */
        rfc_port_closed(p_port);
        return;
    case RFC_EVENT_DATA:                  /* 14 */
        GKI_freebuf(p_data);
        break;
    case RFC_EVENT_UA:                        /* 1 - connection established */
        rfc_port_timer_stop(p_port);
        p_port->rfc.state = RFC_STATE_OPENED;
        PORT_DlcEstablishCnf(p_port->rfc.p_mcb, p_port->dlci,
                             p_port->rfc.p_mcb->peer_l2cap_mtu, 0);
        return;
    case RFC_EVENT_DM:                        /* 2 - refused */
        PORT_DlcEstablishCnf(p_port->rfc.p_mcb, p_port->dlci,
                             p_port->rfc.p_mcb->peer_l2cap_mtu, 1);
        rfc_port_closed(p_port);
        return;
    case RFC_EVENT_DISC:                      /* 3 */
        rfc_send_ua(p_port->rfc.p_mcb, p_port->dlci);
        PORT_DlcEstablishCnf(p_port->rfc.p_mcb, p_port->dlci,
                             p_port->rfc.p_mcb->peer_l2cap_mtu, 1);
        rfc_port_closed(p_port);
        return;
    case RFC_EVENT_SABME:                     /* 0 - collision, respond UA */
        rfc_send_ua(p_port->rfc.p_mcb, p_port->dlci);
        return;
    case RFC_EVENT_UIH:                      /* 4 */
        GKI_freebuf(p_data);
        return;
    case RFC_EVENT_TIMEOUT:                   /* 5 */
        p_port->rfc.state = RFC_STATE_CLOSED;
        PORT_DlcEstablishCnf(p_port->rfc.p_mcb, p_port->dlci,
                             p_port->rfc.p_mcb->peer_l2cap_mtu, 1);
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
    case RFC_EVENT_SEC_COMPLETE:              /* 15 */
        if (*(u8 *)p_data != 0) {
            if (p_port->rfc.p_mcb != NULL) {
                rfc_send_dm(p_port->rfc.p_mcb, p_port->dlci, 1);
                port_rfc_closed(p_port, 0x0F);
            }
        } else {
            PORT_DlcEstablishInd(p_port->rfc.p_mcb, p_port->dlci,
                                 p_port->rfc.p_mcb->peer_l2cap_mtu);
        }
        return;
    case RFC_EVENT_OPEN:               /* 9 */
    case RFC_EVENT_CLOSE:               /* 12 */
        RFCOMM_TRACE_ERROR2("Port error state %d event %d", p_port->rfc.state, event);
        return;
    case RFC_EVENT_CLEAR:         /* 13 */
        btm_sec_abort_access_req(p_port->rfc.p_mcb->bd_addr);
        rfc_port_closed(p_port);
        return;
    case RFC_EVENT_DATA:                  /* 14 */
        RFCOMM_TRACE_ERROR0("Port error state Term Wait Sec event Data");
        GKI_freebuf(p_data);
        return;
    case RFC_EVENT_SABME:                     /* 0 */
        return;
    case RFC_EVENT_DISC:                      /* 3 - remote gave up */
        btm_sec_abort_access_req(p_port->rfc.p_mcb->bd_addr);
        p_port->rfc.state = RFC_STATE_CLOSED;
        rfc_send_ua(p_port->rfc.p_mcb, p_port->dlci);
        PORT_DlcReleaseInd(p_port->rfc.p_mcb, p_port->dlci);
        return;
    case RFC_EVENT_UIH:                      /* 4 */
        GKI_freebuf(p_data);
        return;
    case RFC_EVENT_ESTABLISH_RSP:               /* 11 - local app response */
        if (*(u8 *)p_data != 0) {
            if (p_port->rfc.p_mcb != NULL) {
                rfc_send_dm(p_port->rfc.p_mcb, p_port->dlci, 1);
            }
        } else {
            rfc_send_ua(p_port->rfc.p_mcb, p_port->dlci);
            p_port->rfc.state = RFC_STATE_OPENED;
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
    case RFC_EVENT_SEC_COMPLETE:              /* 15 */
        if (*(u8 *)p_data != 0) {
            PORT_DlcEstablishCnf(p_port->rfc.p_mcb, p_port->dlci, 0, 0x70);
            rfc_port_closed(p_port);
        } else {
            rfc_send_sabme(p_port->rfc.p_mcb, p_port->dlci);
            rfc_port_timer_start(p_port, 0x3C);
            p_port->rfc.state = RFC_STATE_SABME_WAIT_UA;
        }
        return;
    case RFC_EVENT_SABME:                     /* 0 */
    case RFC_EVENT_OPEN:               /* 9 */
        RFCOMM_TRACE_ERROR2("Port error state %d event %d", p_port->rfc.state, event);
        return;
    case RFC_EVENT_CLEAR:         /* 13 */
        btm_sec_abort_access_req(p_port->rfc.p_mcb->bd_addr);
        rfc_port_closed(p_port);
        return;
    case RFC_EVENT_DATA:                  /* 14 */
        RFCOMM_TRACE_ERROR0("Port error state Orig Wait Sec event Data");
        GKI_freebuf(p_data);
        return;
    case RFC_EVENT_UIH:                      /* 4 */
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
    case RFC_EVENT_OPEN:               /* 9 */
        RFCOMM_TRACE_ERROR2("Port error state %d event %d", p_port->rfc.state, event);
        return;
    case RFC_EVENT_CLOSE:               /* 12 - local close */
        rfc_port_timer_start(p_port, 3);
        rfc_send_disc(p_port->rfc.p_mcb, p_port->dlci);
        p_port->rfc.expected_rsp = 0;
        p_port->rfc.state = RFC_STATE_DISC_WAIT_UA;
        return;
    case RFC_EVENT_CLEAR:         /* 13 */
        rfc_port_closed(p_port);
        return;
    case RFC_EVENT_DATA:                  /* 14 - data with credit */
        if (p_port->rfc.p_mcb->flow == 2
            && ((BT_HDR *)p_data)->len < p_port->peer_mtu
            && p_port->rx.user_fc == 0
            && p_port->credit_rx_max > p_port->credit_rx) {
            ((BT_HDR *)p_data)->layer_specific =
                (u16)(u8)(p_port->credit_rx_max - p_port->credit_rx);
            p_port->credit_rx = p_port->credit_rx_max;
        } else {
            ((BT_HDR *)p_data)->layer_specific = 0;
        }
        rfc_send_buf_uih(p_port->rfc.p_mcb, p_port->dlci, (BT_HDR *)p_data);
        rfc_dec_credit(p_port);
        return;
    case RFC_EVENT_UA:                        /* 1 */
        return;
    case RFC_EVENT_SABME:                     /* 0 */
        rfc_send_ua(p_port->rfc.p_mcb, p_port->dlci);
        return;
    case RFC_EVENT_DM:                        /* 2 - remote disconnected */
        PORT_DlcReleaseInd(p_port->rfc.p_mcb, p_port->dlci);
        rfc_port_closed(p_port);
        return;
    case RFC_EVENT_DISC:                      /* 3 - remote disconnected */
        p_port->rfc.state = RFC_STATE_CLOSED;
        rfc_send_ua(p_port->rfc.p_mcb, p_port->dlci);
        PORT_DlcReleaseInd(p_port->rfc.p_mcb, p_port->dlci);
        return;
    case RFC_EVENT_UIH:                      /* 4 */
        PORT_DataInd(p_port->rfc.p_mcb, p_port->dlci, (BT_HDR *)p_data);
        return;
    case RFC_EVENT_TIMEOUT:                   /* 5 */
        Port_TimeOutCloseMux(p_port->rfc.p_mcb);
        RFCOMM_TRACE_ERROR2("Port error state %d event %d", p_port->rfc.state, event);
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
    case RFC_EVENT_OPEN:               /* 9 */
    case RFC_EVENT_ESTABLISH_RSP:               /* 11 */
        RFCOMM_TRACE_ERROR2("Port error state %d event %d", p_port->rfc.state, event);
        return;
    case RFC_EVENT_CLEAR:         /* 13 */
        rfc_port_closed(p_port);
        return;
    case RFC_EVENT_DATA:                  /* 14 */
        GKI_freebuf(p_data);
        return;
    case RFC_EVENT_UA:                        /* 1 */
        p_port->rfc.p_mcb->is_disc_initiator = 1;
        /* fall through */
    case RFC_EVENT_DM:                        /* 2 */
        rfc_port_closed(p_port);
        return;
    case RFC_EVENT_SABME:                     /* 0 */
        rfc_send_dm(p_port->rfc.p_mcb, p_port->dlci, 1);
        return;
    case RFC_EVENT_DISC:                      /* 3 */
        rfc_send_dm(p_port->rfc.p_mcb, p_port->dlci, 1);
        return;
    case RFC_EVENT_UIH:                      /* 4 */
        GKI_freebuf(p_data);
        rfc_send_dm(p_port->rfc.p_mcb, p_port->dlci, 0);
        return;
    case RFC_EVENT_TIMEOUT:                   /* 5 */
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
            RFCOMM_TRACE_WARNING0(rfc_pn_disc_str);
        }
    } else {
        /* Response */
        p_port = port_find_mcb_dlci_port(p_mcb, dlci);
        if ((p_port == NULL) || !(p_port->rfc.expected_rsp & RFC_RSP_PN)) {
            return;
        }
        p_port->rfc.expected_rsp &= 0xFE;
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
            rfc_set_port_state(&buf, p_frame);
            PORT_PortNegInd(p_mcb, p_frame->dlci, &buf, p_frame->u.rpn.param_mask);
        }
        return;
    }

    if (is_command && is_request) {
        /* Request for new parameters: reply with the current port state */
        buf = p_port->peer_port_pars;
        rfc_send_rpn(p_mcb, p_frame->dlci, 0, &p_port->peer_port_pars, 0);
    } else {
        /* Copy current state, then apply the received parameters */
        buf = p_port->peer_port_pars;
        rfc_set_port_state(&buf, p_frame);

        if (is_command) {
            PORT_PortNegInd(p_mcb, p_frame->dlci, &buf, p_frame->u.rpn.param_mask);
        } else {
            /* Response */
            p_port = port_find_mcb_dlci_port(p_mcb, p_frame->dlci);
            if ((p_port == NULL) || !(p_port->rfc.expected_rsp & (RFC_RSP_RPN_REPLY | RFC_RSP_RPN))) {
                return;
            }
            rfc_port_timer_stop(p_port);
            if (p_port->rfc.expected_rsp & RFC_RSP_RPN_REPLY) {
                p_port->rfc.expected_rsp &= ~RFC_RSP_RPN_REPLY;
                p_port->peer_port_pars = buf;
                if (buf.fc_type == 0x0C || buf.fc_type == 0x30) {
                    p_frame->u.rpn.param_mask = 0x3F7F;
                } else {
                    p_port->peer_port_pars.fc_type = 0x0C;
                    p_port->rfc.expected_rsp |= RFC_RSP_RPN;
                    rfc_send_rpn(p_mcb, p_frame->dlci, 1, &p_port->peer_port_pars, 0x0C00);
                    rfc_port_timer_start(p_port, 0x3C);
                    return;
                }
            } else {
                p_port->rfc.expected_rsp &= ~RFC_RSP_RPN;
            }

            if ((p_frame->u.rpn.param_mask & 0x0C00) == 0x0C00
                || (p_frame->u.rpn.param_mask & 0x3000) == 0x3000) {
                PORT_PortNegCnf(p_mcb, p_port->dlci, &buf, 0);
            } else if (p_port->peer_port_pars.fc_type == 0x0C) {
                p_port->peer_port_pars.fc_type = 0x30;
                p_port->rfc.expected_rsp |= RFC_RSP_RPN;
                rfc_send_rpn(p_mcb, p_frame->dlci, 1, &p_port->peer_port_pars, 0x3000);
                rfc_port_timer_start(p_port, 0x3C);
            } else if (p_port->peer_port_pars.fc_type == 0x30) {
                p_port->peer_port_pars.fc_type = 0;
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
    tPORT_CTRL pars;

    p_port = port_find_mcb_dlci_port(p_mcb, p_frame->dlci);
    if (p_port == NULL) {
        return;
    }

    /* Extract modem signals into the local control block */
    pars.modem_signal = 0;
    if (signals & 0x04) {
        pars.modem_signal |= 0x01;
    }
    if (signals & 0x08) {
        pars.modem_signal |= 0x02;
    }
    if (signals & 0x40) {
        pars.modem_signal |= 0x04;
    }
    if (signals & 0x80) {
        pars.modem_signal |= 0x08;
    }
    pars.fc = (signals >> 1) & 1;
    pars.break_signal = p_frame->u.msc.break_present
                                   ? p_frame->u.msc.break_duration : 0;
    pars.discard_buffers = 0;
    pars.break_signal_seq = 1;

    if (is_command) {
        /* Send the response and report the new state */
        rfc_send_msc(p_mcb, p_frame->dlci, 0, &pars);
        if (p_port->rfc.p_mcb->flow != 2) {
            u8 break_signal = pars.fc;
            p_port->peer_ctrl.fc = break_signal;
            if (break_signal != p_port->tx.peer_fc) {
                PORT_FlowInd(p_mcb, p_frame->dlci,
                             (BOOLEAN)!break_signal);
            }
        }
        PORT_ControlInd(p_mcb, p_frame->dlci, &pars);
    } else {
        /* Response */
        if (p_port->rfc.expected_rsp & RFC_RSP_MSC) {
            p_port->rfc.expected_rsp &= ~RFC_RSP_MSC;
            rfc_port_timer_stop(p_port);
            PORT_ControlCnf(p_port->rfc.p_mcb, p_port->dlci, &pars);
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
        if ((p_port == NULL) || !(p_port->rfc.expected_rsp & RFC_RSP_RLS)) {
            return;
        }
        p_port->rfc.expected_rsp &= ~RFC_RSP_RLS;
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
        rfc_cb.rfc.peer_rx_disabled = 0;
        rfc_send_fcon(p_mcb, 0);
        if (p_mcb->l2cap_congested == 0) {
            PORT_FlowInd(p_mcb, 0, 1);
        }
    }
}

void rfc_process_fcoff(tRFC_MCB *p_mcb, u8 is_command)
{
    if (is_command) {
        rfc_cb.rfc.peer_rx_disabled = 1;
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

    if (rfc_cb.rfc.peer_rx_disabled == 0) {
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
void rfc_set_port_state(tPORT_STATE *port_pars, MX_FRAME *p_frame)
{
    if (p_frame->u.rpn.param_mask & 0x0001) {
        port_pars->baud_rate = p_frame->u.rpn.baud_rate;
    }
    if (p_frame->u.rpn.param_mask & 0x0002) {
        port_pars->byte_size = p_frame->u.rpn.byte_size;
    }
    if (p_frame->u.rpn.param_mask & 0x0004) {
        port_pars->stop_bits = p_frame->u.rpn.stop_bits;
    }
    if (p_frame->u.rpn.param_mask & 0x0008) {
        port_pars->parity = p_frame->u.rpn.parity;
    }
    if (p_frame->u.rpn.param_mask & 0x0010) {
        port_pars->parity_type = p_frame->u.rpn.parity_type;
    }
    if (p_frame->u.rpn.param_mask & 0x3F00) {
        port_pars->fc_type = p_frame->u.rpn.fc_type;
    }
    if (p_frame->u.rpn.param_mask & 0x0020) {
        port_pars->xon_char = p_frame->u.rpn.xon_char;
    }
    if (p_frame->u.rpn.param_mask & 0x0040) {
        port_pars->xoff_char = p_frame->u.rpn.xoff_char;
    }
}

// Retail .data ends at 0x326: exact 0x26-byte trace string (no pad).
char rfc_pn_disc_str[0x26] = "***** MX PN while disconnecting *****";
