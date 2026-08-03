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
    u8 pad_0x00[0x24];
    u8 port_inx[0x3E];
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

/* L2CAP application registration block embedded in rfc_cb at 0x14.
 * Retail layout: 10 callback pointers, 0x14..0x3B (ConnectInd, ConnectCfm,
 * NULL, ConfigInd, ConfigCfm, DisconnectInd, NULL, QoSViolationInd,
 * BufDataInd, CongestionStatusInd). */
struct RfcL2capApplInfo {
    void (*pL2CA_ConnectInd_Cb)(u8* bd_addr, u16 lcid, u16 psm, u8 id);
    void (*pL2CA_ConnectCfm_Cb)(u16 lcid, u16 result);
    void (*pL2CA_ConnectPnd_Cb)(void);
    void (*pL2CA_ConfigInd_Cb)(u16 lcid, u8* p_cfg);
    void (*pL2CA_ConfigCfm_Cb)(u16 lcid, u8* config);
    void (*pL2CA_DisconnectInd_Cb)(u16 lcid, u8 response);
    void (*pL2CA_DisconnectCfm_Cb)(void);
    void (*pL2CA_QoSViolationInd_Cb)(void);
    void (*pL2CA_DataInd_Cb)(u16 lcid, BT_HDR* buffer);
    void (*pL2CA_CongestionStatus_Cb)(u16 lcid, u8 congested);
};

struct RfcControlBlock {
    u8 dlci;
    u8 pad_0x01[1];
    u8 cr;
    u8 pad_0x03[1];
    u8 pf;
    u8 credit_based;
    u8 pad_0x06[0x0E];
    struct RfcL2capApplInfo l2cap_Appl_Info;
    RfcMuxChannel* mcb[10];
    u8 pad_0x64[0x3b0];
    u8 trace_level;
};

extern RfcControlBlock rfc_cb;

#define RFCOMM_PSM 3
#define L2CAP_CONN_NO_RESOURCES 4
#define RFC_MX_EVENT_CONN_IND 0x0a

extern void LogMsg_1(u32 level, const char* message, u32 value);
extern void LogMsg_2(u32 level, const char* message, u32 value1, u32 value2);
extern void GKI_freebuf(BT_HDR* buffer);
extern void L2CA_DisconnectRsp(u16 lcid);
extern u16 L2CA_Register(u16 psm, struct RfcL2capApplInfo* p_cb_info);
extern void L2CA_ConnectRsp(u8* bd_addr, u8 id, u16 lcid, u16 result, u16 status);
extern RfcMuxChannel* rfc_alloc_multiplexer_channel(u8* bd_addr, u8 initiator);
extern void rfc_mx_sm_execute(RfcMuxChannel* channel, u16 event, u8* data);
extern void rfc_process_mx_message(RfcMuxChannel* channel, BT_HDR* buffer);
extern u8 rfc_parse_data(RfcMuxChannel* channel, RfcControlBlock* control, BT_HDR* buffer);
extern RfcPort* port_find_mcb_dlci_port(RfcMuxChannel* channel, u8 dlci);
extern RfcPort* port_find_dlci_port(u8 dlci);
extern void rfc_send_dm(RfcMuxChannel* channel, int poll, int command);
extern void rfc_port_sm_execute(RfcPort* port, u8 event, BT_HDR* buffer);
extern void rfc_process_l2cap_congestion(RfcMuxChannel* channel, u8 congested);
extern void rfc_inc_credit(RfcPort* port, u8 credit);

static RfcMuxChannel* rfc_find_lcid_mcb(u16 lcid) {
    int index = (int)lcid - 0x40;
    RfcMuxChannel* channel;

    if (index >= 10) {
        if (rfc_cb.trace_level >= 1) {
            LogMsg_1(0x90000, "rfc_find_lcid_mcb LCID:0x%x", lcid);
        }
        return 0;
    }

    channel = rfc_cb.mcb[index];
    if (channel != 0 && channel->lcid != lcid) {
        if (rfc_cb.trace_level >= 2) {
            LogMsg_2(0x90001, "rfc_find_lcid_mcb LCID reused LCID:0x%x current:0x%x", lcid,
                     channel->lcid);
        }
        return 0;
    }
    return channel;
}

void RFCOMM_ConnectInd(u8* bd_addr, u16 lcid, u16 psm, u8 id);
void RFCOMM_ConnectCnf(u16 lcid, u16 result);
void RFCOMM_ConfigInd(u16 lcid, u8* p_cfg);
void RFCOMM_ConfigCnf(u16 lcid, u8* config);
void RFCOMM_DisconnectInd(u16 lcid, u8 response);
void RFCOMM_QoSViolationInd(void);
void RFCOMM_BufDataInd(u16 lcid, BT_HDR* buffer);
void RFCOMM_CongestionStatusInd(u16 lcid, u8 congested);

void rfcomm_l2cap_if_init() {
    struct RfcL2capApplInfo* p_info = &rfc_cb.l2cap_Appl_Info;

    p_info->pL2CA_ConnectInd_Cb = RFCOMM_ConnectInd;
    p_info->pL2CA_ConnectCfm_Cb = RFCOMM_ConnectCnf;
    p_info->pL2CA_ConnectPnd_Cb = 0;
    p_info->pL2CA_ConfigInd_Cb = RFCOMM_ConfigInd;
    p_info->pL2CA_ConfigCfm_Cb = RFCOMM_ConfigCnf;
    p_info->pL2CA_DisconnectInd_Cb = RFCOMM_DisconnectInd;
    p_info->pL2CA_DisconnectCfm_Cb = 0;
    p_info->pL2CA_QoSViolationInd_Cb = RFCOMM_QoSViolationInd;
    p_info->pL2CA_DataInd_Cb = RFCOMM_BufDataInd;
    p_info->pL2CA_CongestionStatus_Cb = RFCOMM_CongestionStatusInd;

    L2CA_Register(RFCOMM_PSM, p_info);
}

void RFCOMM_ConnectInd(u8* bd_addr, u16 lcid, u16 psm, u8 id) {
    RfcMuxChannel* p_mcb = rfc_alloc_multiplexer_channel(bd_addr, 0);

    rfc_cb.mcb[lcid - 0x40] = p_mcb;

    if (p_mcb == 0) {
        L2CA_ConnectRsp(bd_addr, id, lcid, L2CAP_CONN_NO_RESOURCES, 0);
        return;
    }

    p_mcb->lcid = lcid;

    rfc_mx_sm_execute(p_mcb, RFC_MX_EVENT_CONN_IND, (u8*)&id);
}

void RFCOMM_ConnectCnf(u16 lcid, u16 result) {
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
            LogMsg_1(0x90000, "RFCOMM_ConnectCnf LCID:0x%x", lcid);
        }
    } else {
        channel->lcid = lcid;
        rfc_mx_sm_execute(channel, 9, (u8*)&result);
    }
}

void RFCOMM_ConfigInd(u16 lcid, u8* p_cfg) {
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
            LogMsg_1(0x90000, "RFCOMM_ConfigInd LCID:0x%x", lcid);
        }
    } else {
        rfc_mx_sm_execute(channel, 0xc, p_cfg);
    }
}

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


void RFCOMM_BufDataInd(u16 lcid, BT_HDR* buffer) {
    RfcMuxChannel* channel = rfc_find_lcid_mcb(lcid);
    RfcPort* port = 0;
    u8 frame_type;

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

    port = port_find_mcb_dlci_port(channel, rfc_cb.dlci);
    if (port == 0 || port->mcb == 0) {
        if (frame_type != 0) {
            if ((channel->initiator != 0 && rfc_cb.cr == 0) ||
                (channel->initiator == 0 && rfc_cb.cr != 0)) {
                rfc_send_dm(channel, rfc_cb.dlci, rfc_cb.pf);
            }
            GKI_freebuf(buffer);
            return;
        }

        port = port_find_dlci_port(rfc_cb.dlci);
        if (port == 0) {
            rfc_send_dm(channel, rfc_cb.dlci, 1);
            GKI_freebuf(buffer);
            return;
        }
        channel->port_inx[rfc_cb.dlci] = port->dlci;
        port->mcb = channel;
    }

    if (frame_type == 4) {
        if (buffer->len != 0) {
            rfc_port_sm_execute(port, frame_type, buffer);
        } else {
            GKI_freebuf(buffer);
        }
        if (rfc_cb.credit_based != 0) {
            rfc_inc_credit(port, rfc_cb.credit_based);
        }
    } else {
        rfc_port_sm_execute(port, frame_type, 0);
        GKI_freebuf(buffer);
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
            LogMsg_1(0x90003,
                     "RFCOMM_CongestionStatusInd LCID:0x%x", lcid);
        }
        rfc_process_l2cap_congestion(channel, congested);
    }
}

void rfc_save_lcid_mcb(RfcMuxChannel* p_mcb, u16 lcid) {
    rfc_cb.mcb[lcid - 0x40] = p_mcb;
}
