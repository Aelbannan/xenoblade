#include <harness_catalog.h>

/* The RFCOMM callback block contains the multiplexer table followed by ports. */
typedef struct BT_HDR BT_HDR;
typedef struct RfcMuxChannel RfcMuxChannel;
typedef struct RfcPort RfcPort;
typedef struct RfcControlBlock RfcControlBlock;

struct BT_HDR {
    u16 event;
    u16 len;
    u16 offset;
    u16 layer_specific;
};

struct RfcMuxChannel {
    u8 pad_0x00[0x62];
    u8 bd_addr[6];
    u16 lcid;
    u16 field_0x6a;
    u8 state;
    u8 initiator;
    u8 field_0x6e;
    u8 field_0x6f;
    u8 field_0x70;
    u8 field_0x71;
};

struct RfcPort {
    u8 dlci;
    u8 pad_0x01[0x23];
    u8 field_0x24;
    u8 pad_0x25[0x47];
    RfcMuxChannel* mcb;
};

struct RfcControlBlock {
    u8 dlci;
    u8 pad_0x01[1];
    u8 field_0x02;
    u8 pad_0x03[1];
    u8 field_0x04;
    u8 credit_based;
    u8 pad_0x06[0x36];
    RfcMuxChannel* mcb[10];
    u8 pad_0x64[0x3b0];
    u8 trace_level;
};

extern RfcControlBlock rfc_cb;

extern void LogMsg_1(u32 level, const char* message, u32 value);
extern void LogMsg_2(u32 level, const char* message, u32 value1, u32 value2);
extern void GKI_freebuf(BT_HDR* buffer);
extern void L2CA_DisconnectRsp(u16 lcid);
extern void rfc_mx_sm_execute(RfcMuxChannel* channel, u16 event, u8* data);
extern void rfc_process_mx_message(RfcMuxChannel* channel, BT_HDR* buffer);
extern u8 rfc_parse_data(RfcMuxChannel* channel, RfcControlBlock* control, BT_HDR* buffer);
extern RfcPort* port_find_mcb_dlci_port(RfcMuxChannel* channel, u8 dlci);
extern RfcPort* port_find_dlci_port(u8 dlci);
extern void rfc_send_dm(RfcMuxChannel* channel, int poll, int command);
extern void rfc_port_sm_execute(RfcPort* port, u8 event, BT_HDR* buffer);
extern void rfc_process_l2cap_congestion(RfcMuxChannel* channel, u8 congested);
extern void rfc_inc_credit(RfcPort* port);

void rfcomm_l2cap_if_init() {}
void RFCOMM_ConnectInd() {}
void RFCOMM_ConnectCnf() {}
void RFCOMM_ConfigInd() {}
void RFCOMM_QoSViolationInd(void) {}

void RFCOMM_ConfigCnf(u16 lcid, u8* config) {
    int index = (int)lcid - 0x40;
    RfcMuxChannel* channel;

    if (index >= 10) {
        if (rfc_cb.trace_level >= 1) {
            LogMsg_1(0x90000, "rfc_find_lcid_mcb LCID:0x%x", lcid);
        }
        channel = 0;
    } else {
        channel = rfc_cb.mcb[index];
        if (channel != 0 && channel->lcid != lcid) {
            if (rfc_cb.trace_level >= 2) {
                LogMsg_2(0x90001, "rfc_find_lcid_mcb LCID reused LCID:0x%x current:0x%x", lcid,
                         channel->lcid);
            }
            channel = 0;
        }
    }

    if (channel == 0) {
        if (rfc_cb.trace_level >= 1) {
            LogMsg_1(0x90000, "RFCOMM_ConfigCnf LCID:0x%x", lcid);
        }
    } else {
        rfc_mx_sm_execute(channel, 0xb, config);
    }
}

void RFCOMM_DisconnectInd(u16 lcid, u8 response) {
    int index = (int)lcid - 0x40;
    RfcMuxChannel* channel;

    if (index >= 10) {
        if (rfc_cb.trace_level >= 1) {
            LogMsg_1(0x90000, "rfc_find_lcid_mcb LCID:0x%x", lcid);
        }
        channel = 0;
    } else {
        channel = rfc_cb.mcb[index];
        if (channel != 0 && channel->lcid != lcid) {
            if (rfc_cb.trace_level >= 2) {
                LogMsg_2(0x90001, "rfc_find_lcid_mcb LCID reused LCID:0x%x current:0x%x", lcid,
                         channel->lcid);
            }
            channel = 0;
        }
    }

    if (response != 0) {
        L2CA_DisconnectRsp(lcid);
    }

    if (channel == 0) {
        if (rfc_cb.trace_level >= 2) {
            LogMsg_1(0x90001, "RFCOMM_DisconnectInd LCID:0x%x", lcid);
        }
    } else {
        rfc_mx_sm_execute(channel, 0xe, 0);
    }
}

void RFCOMM_CongestionStatusInd(u16 lcid, u8 congested) {
    int index = (int)lcid - 0x40;
    RfcMuxChannel* channel;

    if (index >= 10) {
        if (rfc_cb.trace_level >= 1) {
            LogMsg_1(0x90000, "rfc_find_lcid_mcb LCID:0x%x", lcid);
        }
        channel = 0;
    } else {
        channel = rfc_cb.mcb[index];
        if (channel != 0 && channel->lcid != lcid) {
            if (rfc_cb.trace_level >= 2) {
                LogMsg_2(0x90001, "rfc_find_lcid_mcb LCID reused LCID:0x%x current:0x%x", lcid,
                         channel->lcid);
            }
            channel = 0;
        }
    }

    if (channel == 0) {
        if (rfc_cb.trace_level >= 1) {
            LogMsg_1(0x90000, "RFCOMM_CongestionStatusInd dropped LCID:0x%x", lcid);
        }
    } else {
        if (rfc_cb.trace_level >= 4) {
            LogMsg_1(0x90003, "RFCOMM_CongestionStatusInd LCID:0x%x", lcid);
        }
        rfc_process_l2cap_congestion(channel, congested);
    }
}

void RFCOMM_BufDataInd(u16 lcid, BT_HDR* buffer) {
    int index = (int)lcid - 0x40;
    RfcMuxChannel* channel;
    RfcPort* port;
    u8 frame_type;
    u8 dlci;

    if (index >= 10) {
        if (rfc_cb.trace_level >= 1) {
            LogMsg_1(0x90000, "rfc_find_lcid_mcb LCID:0x%x", lcid);
        }
    } else if (index >= 0) {
        channel = rfc_cb.mcb[index];
        if (channel != 0 && channel->lcid != lcid) {
            if (rfc_cb.trace_level >= 2) {
                LogMsg_2(0x90001, "rfc_find_lcid_mcb LCID reused LCID:0x%x current:0x%x", lcid,
                         channel->lcid);
            }
            channel = 0;
        }
    }

    if (channel == 0) {
        if (rfc_cb.trace_level >= 2) {
            LogMsg_1(0x90001, "RFCOMM_BufDataInd LCID:0x%x", lcid);
        }
        GKI_freebuf(buffer);
        return;
    }

    frame_type = rfc_parse_data(channel, &rfc_cb, buffer);
    if (frame_type == 0x32) {
        GKI_freebuf(buffer);
        return;
    }

    if (rfc_cb.dlci == 0) {
        if (frame_type == 4) {
            rfc_process_mx_message(channel, buffer);
        } else {
            rfc_mx_sm_execute(channel, frame_type, 0);
            GKI_freebuf(buffer);
        }
        return;
    }

    dlci = rfc_cb.dlci;
    port = port_find_mcb_dlci_port(channel, dlci);
    if (port == 0 || port->mcb == 0) {
        if (frame_type != 0) {
            if (channel->initiator != 0 || rfc_cb.field_0x02 == 0) {
                if (channel->initiator == 0 && rfc_cb.field_0x02 != 0) {
                    rfc_send_dm(channel, rfc_cb.dlci, rfc_cb.credit_based);
                }
            } else {
                rfc_send_dm(channel, rfc_cb.dlci, rfc_cb.credit_based);
            }
        }
        GKI_freebuf(buffer);
        return;
    }

    if (frame_type == 0) {
        port = port_find_dlci_port(rfc_cb.dlci);
        if (port == 0) {
            rfc_send_dm(channel, rfc_cb.dlci, 1);
            GKI_freebuf(buffer);
            return;
        }
        port->field_0x24 = port->dlci;
        port->mcb = channel;
    }

    if (frame_type == 4) {
        if (buffer->len != 0) {
            rfc_port_sm_execute(port, frame_type, buffer);
        } else {
            GKI_freebuf(buffer);
        }
        if (rfc_cb.credit_based != 0) {
            rfc_inc_credit(port);
        }
    } else {
        rfc_port_sm_execute(port, 0, 0);
        GKI_freebuf(buffer);
    }
}

void rfc_save_lcid_mcb() {}
