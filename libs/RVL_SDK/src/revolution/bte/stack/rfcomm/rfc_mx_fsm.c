// Recovered RFCOMM multiplexer state-machine handlers.

#include <harness_catalog.h>
#include <string.h>

typedef struct BT_HDR BT_HDR;

typedef struct RfcMuxChannel RfcMuxChannel;
typedef struct RfcConfig RfcConfig;

typedef struct RfcControlBlock {
    u8 pad_0x000[0x414];
    u8 trace_level;
} RfcControlBlock;

struct RfcMuxChannel {
    u8 pad_0x00[0x62];
    u8 bd_addr[6];
    u16 field_0x68;
    u16 field_0x6a;
    u8 state;
    u8 field_0x6d;
    u8 field_0x6e;
    u8 field_0x6f;
    u8 field_0x70;
    u8 field_0x71;
};

struct RfcConfig {
    u16 result;
    u8 field_0x02;
    u8 pad_0x03;
    u16 field_0x04;
    u8 field_0x06;
    u8 pad_0x07[0x19];
    u8 field_0x20;
    u8 pad_0x21[0x1b];
};

extern RfcControlBlock rfc_cb;

extern void LogMsg_1(u32 level, const char* message, u32 p1);
extern void LogMsg_2(u32 level, const char* message, u32 p1, u32 p2);
extern u16 L2CA_ConnectReq(u16 psm, u8* bd_addr);
extern void L2CA_ConnectRsp(u8* bd_addr, u8 id, u16 lcid, u16 result, u16 status);
extern void L2CA_ConfigReq(u16 lcid, RfcConfig* config);
extern void L2CA_ConfigRsp(u16 lcid, RfcConfig* config);
extern void L2CA_DisconnectReq(u16 lcid);
extern void GKI_freebuf(BT_HDR* buffer);
extern void PORT_CloseInd(RfcMuxChannel* channel);
extern void PORT_StartCnf(RfcMuxChannel* channel);
extern void PORT_StartInd(RfcMuxChannel* channel);
extern void rfc_release_multiplexer_channel(RfcMuxChannel* channel);
extern void rfc_save_lcid_mcb(RfcMuxChannel* channel, ...);
extern void rfc_send_dm(RfcMuxChannel* channel, int poll, int command);
extern void rfc_send_disc(RfcMuxChannel* channel, int poll);
extern void rfc_send_sabme(RfcMuxChannel* channel, int poll);
extern void rfc_send_ua(RfcMuxChannel* channel, int poll);
extern void rfc_timer_start(RfcMuxChannel* channel, int timer);
extern void rfc_timer_stop(RfcMuxChannel* channel);

void rfc_mx_conf_cnf(RfcMuxChannel* channel, RfcConfig* config);
void rfc_mx_conf_ind(RfcMuxChannel* channel, RfcConfig* config);
void rfc_mx_sm_state_wait_sabme(RfcMuxChannel* channel, u16 event, u8* data);

void rfc_mx_sm_state_configure(RfcMuxChannel* channel, u16 event, u8* data) {
    if (rfc_cb.trace_level >= 4) {
        LogMsg_1(0x90003, "rfc_mx_sm_state_configure - evt:%d", event);
    }

    switch (event) {
    case 6:
    case 9:
        if (rfc_cb.trace_level >= 1) {
            LogMsg_2(0x90000, "Mx error state %d event %d", channel->state, event);
        }
        break;
    case 12:
        rfc_mx_conf_ind(channel, (RfcConfig*)data);
        break;
    case 11:
        rfc_mx_conf_cnf(channel, (RfcConfig*)data);
        break;
    case 14:
        channel->state = 0;
        PORT_CloseInd(channel);
        break;
    default:
    case 7:
    case 8:
    case 10:
    case 13:
        if (rfc_cb.trace_level >= 4) {
            LogMsg_2(0x90003, "rfc_mx_sm_state - evt:%d in state:%d", event, channel->state);
        }
        break;
    }
}
void rfc_mx_sm_state_connected(RfcMuxChannel* channel, u16 event, u8* data);

void rfc_mx_sm_state_disc_wait_ua(RfcMuxChannel* channel, u16 event, u8* data) {
    if (rfc_cb.trace_level >= 4) {
        LogMsg_1(0x90003, "rfc_mx_sm_state_disc_wait_ua - evt:%d", event);
    }

    switch (event) {
    case 1:
    case 2:
    case 5:
        L2CA_DisconnectReq(channel->field_0x68);
        if (channel->field_0x70 != 0) {
            u16 lcid = L2CA_ConnectReq(3, channel->bd_addr);
            channel->field_0x68 = lcid;
            if (lcid == 0) {
                PORT_StartCnf(channel);
                break;
            }
            rfc_save_lcid_mcb(channel);
            channel->field_0x70 = 0;
            channel->field_0x6e = 0;
            channel->field_0x6f = 0;
            channel->state = 1;
        } else {
            rfc_release_multiplexer_channel(channel);
        }
        break;
    case 3:
        rfc_send_ua(channel, 0);
        break;
    case 4:
        GKI_freebuf((BT_HDR*)data);
        rfc_send_dm(channel, 0, 0);
        break;
    case 6:
        channel->field_0x70 = 1;
        break;
    case 14:
        channel->state = 0;
        PORT_CloseInd(channel);
        break;
    case 8:
    case 13:
        break;
    default:
        if (rfc_cb.trace_level >= 4) {
            LogMsg_2(0x90003, "rfc_mx_sm_state - evt:%d in state:%d", event, channel->state);
        }
        break;
    }
}

void rfc_mx_sm_sabme_wait_ua(RfcMuxChannel* channel, u16 event, u8* data) {
    if (rfc_cb.trace_level >= 4) {
        LogMsg_1(0x90003, "rfc_mx_sm_sabme_wait_ua - evt:%d", event);
    }

    switch (event) {
    case 11:
        rfc_mx_conf_cnf(channel, (RfcConfig*)data);
        break;
    case 12:
        rfc_mx_conf_ind(channel, (RfcConfig*)data);
        break;
    case 14:
        channel->state = 0;
        PORT_CloseInd(channel);
        break;
    case 1:
        rfc_timer_stop(channel);
        channel->state = 5;
        channel->field_0x71 = 1;
        PORT_StartCnf(channel);
        break;
    case 2:
        rfc_timer_stop(channel);
        /* DM and timeout share the disconnect/error completion path. */
        /* fall through */
    case 5:
        channel->state = 0;
        L2CA_DisconnectReq(channel->field_0x68);
        PORT_StartCnf(channel);
        break;
    default:
        if (rfc_cb.trace_level >= 4) {
            LogMsg_2(0x90003, "rfc_mx_sm_state - evt:%d in state:%d", event, channel->state);
        }
        break;
    }
}

extern void rfc_mx_sm_state_idle(RfcMuxChannel* channel, u16 event, u8* data);
extern void rfc_mx_sm_state_wait_conn_cnf(RfcMuxChannel* channel, u16 event, u8* data);
extern void rfc_mx_sm_state_configure(RfcMuxChannel* channel, u16 event, u8* data);
extern void rfc_mx_sm_sabme_wait_ua(RfcMuxChannel* channel, u16 event, u8* data);
extern void rfc_mx_sm_state_disc_wait_ua(RfcMuxChannel* channel, u16 event, u8* data);

typedef void (*RfcMuxHandler)(RfcMuxChannel* channel, u16 event, u8* data);

void rfc_mx_sm_execute(RfcMuxChannel* channel, u16 event, u8* data) {
    static RfcMuxHandler handlers[7] = {
        rfc_mx_sm_state_idle,
        rfc_mx_sm_state_wait_conn_cnf,
        rfc_mx_sm_state_configure,
        rfc_mx_sm_sabme_wait_ua,
        rfc_mx_sm_state_wait_sabme,
        rfc_mx_sm_state_connected,
        rfc_mx_sm_state_disc_wait_ua,
    };

    if (channel->state <= 6) {
        handlers[channel->state](channel, event, data);
    }
}

void rfc_mx_sm_state_wait_conn_cnf(RfcMuxChannel* channel, u16 event, u8* data) {
    if (rfc_cb.trace_level >= 4) {
        LogMsg_1(0x90003, "rfc_mx_sm_state_wait_conn_cnf - evt:%d", event);
    }

    switch (event) {
    case 6:
        if (rfc_cb.trace_level >= 1) {
            LogMsg_2(0x90003, "Mx error state %d event %d", channel->state, event);
        }
        break;
    case 9:
        if (*(u16*)data != 0) {
            channel->state = 0;
            PORT_StartCnf(channel);
        } else {
            channel->state = 2;
            {
                RfcConfig config;
                memset(&config, 0, sizeof(config));
                config.field_0x02 = 1;
                config.field_0x04 = 0x69b;
                L2CA_ConfigReq(channel->field_0x68, &config);
            }
        }
        break;
    case 14:
        channel->state = 0;
        PORT_CloseInd(channel);
        break;
    case 5:
    case 7:
    case 8:
    case 10:
    case 11:
    case 12:
    case 13:
    default:
        if (rfc_cb.trace_level >= 4) {
            LogMsg_2(0x90003, "rfc_mx_sm_state - evt:%d in state:%d", event, channel->state);
        }
        break;
    }
}

void rfc_mx_sm_state_idle(RfcMuxChannel* channel, u16 event, u8* data) {
    if (rfc_cb.trace_level >= 4) {
        LogMsg_1(0x90003, "rfc_mx_sm_state_idle - evt:%d", event);
    }

    switch (event) {
    case 6: {
        u16 lcid;
        channel->field_0x6a = 0x29a;
        lcid = L2CA_ConnectReq(3, channel->bd_addr);
        channel->field_0x68 = lcid;
        if (lcid == 0) {
            PORT_StartCnf(channel);
        } else {
            rfc_save_lcid_mcb(channel);
            channel->state = 1;
        }
        break;
    }
    case 10:
        if (channel->field_0x6d != 0) {
            L2CA_ConnectRsp(channel->bd_addr, *data, channel->field_0x68, 1, 0);
        } else {
            rfc_timer_start(channel, 0x78);
            L2CA_ConnectRsp(channel->bd_addr, *data, channel->field_0x68, 0, 0);
            {
                RfcConfig config;
                memset(&config, 0, sizeof(config));
                config.field_0x02 = 1;
                config.field_0x04 = 0x69b;
                L2CA_ConfigReq(channel->field_0x68, &config);
            }
            channel->state = 2;
        }
        break;
    case 1:
    case 2:
    case 9:
    case 11:
    case 12:
        if (rfc_cb.trace_level >= 1) {
            LogMsg_2(0x90003, "Mx error state %d event %d", channel->state, event);
        }
        break;
    case 4:
        rfc_send_dm(channel, 0, 1);
        break;
    case 3:
        rfc_send_dm(channel, 0, 0);
        break;
    case 0:
    case 5:
    case 7:
    case 8:
    case 13:
    case 14:
    default:
        break;
    }
}

void rfc_mx_sm_state_wait_sabme(RfcMuxChannel* channel, u16 event, u8* data) {
    if (rfc_cb.trace_level >= 4) {
        LogMsg_1(0x90003, "rfc_mx_sm_state_wait_sabme - evt:%d", event);
    }

    if (event == 7) {
        if (*(u16*)data != 0) {
            rfc_send_dm(channel, 0, 1);
        } else {
            rfc_send_ua(channel, 0);
            channel->state = 5;
            channel->field_0x71 = 1;
        }
    } else if (event == 0) {
        PORT_StartInd(channel);
    } else if (event == 14) {
        channel->state = 0;
        PORT_CloseInd(channel);
    } else if (rfc_cb.trace_level >= 4) {
        LogMsg_2(0x90003, "rfc_mx_sm_state - evt:%d", event, channel->state);
    }
}

void rfc_mx_sm_state_connected(RfcMuxChannel* channel, u16 event, u8* data) {
    if (rfc_cb.trace_level >= 4) {
        LogMsg_1(0x90003, "rfc_mx_sm_state_connected - evt:%d", event);
    }

    /* Jump-table dispatch for events 3–13; out-of-range falls to default. */
    switch (event) {
    case 3:
        rfc_timer_start(channel, 3);
        channel->state = 6;
        rfc_send_disc(channel, 0);
        break;
    case 4:
        channel->state = 0;
        PORT_CloseInd(channel);
        break;
    case 7:
        rfc_send_ua(channel, 0);
        if (channel->field_0x6d != 0) {
            L2CA_DisconnectReq(channel->field_0x68);
        }
        PORT_CloseInd(channel);
        break;
    case 5:
    case 6:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
        break;
    default:
        if (rfc_cb.trace_level >= 4) {
            LogMsg_2(0x90003, "Mx error state %d event %d", channel->state, event);
        }
        break;
    }
}

void rfc_mx_conf_cnf(RfcMuxChannel* channel, RfcConfig* config) {
    if (rfc_cb.trace_level >= 4) {
        LogMsg_2(0x90003, "rfc_mx_conf_cnf p_cfg:%08x res:%d ", (u32)config,
                 config != 0 ? config->result : 0);
    }

    if (config->result != 0) {
        if (channel->field_0x6d != 0) {
            PORT_StartCnf(channel);
            L2CA_DisconnectReq(channel->field_0x68);
        }
        rfc_release_multiplexer_channel(channel);
    } else {
        channel->field_0x6e = 1;
        if (channel->state == 2 && channel->field_0x6f != 0) {
            if (channel->field_0x6d != 0) {
                channel->state = 3;
                rfc_send_sabme(channel, 0);
            } else {
                channel->state = 4;
            }
        }
    }
}

void rfc_mx_conf_ind(RfcMuxChannel* channel, RfcConfig* config) {
    if (rfc_cb.trace_level >= 4) {
        LogMsg_1(0x90003, "rfc_mx_conf_ind p_cfg:%0x", (u32)config);
    }

    if (config->field_0x02 != 0) {
        channel->field_0x6a = config->field_0x04 - 6;
    } else {
        channel->field_0x6a = 0x29a;
    }

    config->field_0x02 = 0;
    config->field_0x20 = 0;
    config->field_0x06 = 0;
    config->result = 0;
    L2CA_ConfigRsp(channel->field_0x68, config);
    channel->field_0x6f = 1;

    if (channel->state == 2 && channel->field_0x6e != 0) {
        if (channel->field_0x6d != 0) {
            channel->state = 3;
            rfc_send_sabme(channel, 0);
        } else {
            channel->state = 4;
        }
    }
}
