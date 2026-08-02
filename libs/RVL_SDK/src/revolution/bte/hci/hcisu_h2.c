// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/hci/hcisu_h2
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

#include <revolution/bte/gki/common/gki.h>

/* UUSB API (retail hcisu_h2.c declares UUSB_Write with a 4th p_buf arg that
   the driver body ignores; the shared uusb_ppc.h keeps the 3-arg form). */
void UUSB_Register(void *cb_arg);
void UUSB_Open(s32 fd, void (*close_cb)(int reason, s8 result));
u16 UUSB_Read(u8 channel, void *p_buf, u16 len);
s32 UUSB_Write(s32 type, void *data, u16 length, void *p_buf);
void UUSB_Close(void);
void UUSB_Unregister(void);

/* Retail GKI pool ids for the per-channel receive buffers: the binary uses
   pool 2 for HCI events, pool 3 for ACL and pool 1 for SCO (the header
   defaults in bt_target.h do not match the retail configuration). */

/* Header of each buffer used in the Bluetooth stack comes from
   bt_types.h (included via the GKI headers). */

/* HCI transport control block (hcisu.h). Layout mirrors the retail image:
   per-channel (channel-1) arrays followed by the shared open/task fields. */
typedef struct {
    BT_HDR *msg[3];        /* 0x00 buffers for channels 1..3 */
    UINT16  len[4];        /* 0x0C per-channel expected length (idx 3 = hci event) */
    UINT8   msgtype[3];    /* 0x14 per-channel HCI packet type (event/ACL/SCO) */
    UINT8   last_byte[3];  /* 0x17 per-channel last header byte */
    UINT8   state[4];      /* 0x1A per-channel receive state */
    UINT8   open;          /* 0x1E transport open flag */
    UINT8   task_id;       /* 0x1F BTU task id */
    UINT8   channel;       /* 0x20 current channel */
} tHCISU_H2_CB;

/* Receive state machine states (hcisu.h). */
#define HCISU_H2_STATE_IDLE   0
#define HCISU_H2_STATE_HEADER 1
#define HCISU_H2_STATE_DISC   2
#define HCISU_H2_STATE_DATA   3
#define HCISU_H2_STATE_BAD    4

/* Packet header sizes indexed by (msgtype - 1): event = 2, ACL = 4,
   SCO = 3. */
const UINT8 hcisu_preamble_table[8] = {3, 4, 3, 2, 0, 0, 0, 0};

/* HCI packet event codes indexed by (msgtype - 1) (bt_types.h). */
const UINT16 hcisu_msg_evt_table[4] = {0x0013, 0x0011, 0x0012, 0x0010};

extern unsigned char hcisu_h2_cb[];

/* BTU control block (btu.h): hcit_acl_data_size at 0x7C, hcit_acl_pkt_size at 0x7E. */
#include <revolution/bte/stack/include/btu.h>

/* USB configuration cookie passed to UUSB_Register/UUSB_Open; only the
   fields this TU writes are named. */
typedef struct {
    UINT8 _pad0[4];   /* 0x00 */
    UINT16 vid;       /* 0x04 */
    UINT16 pid;       /* 0x06 */
    UINT16 task_id;   /* 0x08 */
    UINT8 _pad1;      /* 0x0A */
    UINT8 cfg_byte;   /* 0x0B */
} tUUSB_CFG;

/* Externals from other BTE modules (bt_trace.h / l2cdefs.h). */
extern void LogMsg_0(UINT32 trace_set_mask, const char *p_str);
extern BT_HDR *l2cap_link_chk_pkt_start(BT_HDR *p_buf);
extern UINT8 l2cap_link_chk_pkt_end(void);

/* USB close-event callback: reason 4 (UUSB_CLOSE_REASON_CLOSED) means the
   link dropped; forward the sign-extended result to the BTA layer. */
void hcisu_h2_usb_cback(int type, s8 event) {
    if (type == 0x4) {
        bta_usb_close_evt(event);
    }
}

/* Receive loop for one HCI USB stream. Reads header bytes one at a time
   until the preamble length is collected, then streams the payload into the
   GKI buffer and forwards complete messages to the BTU task. */
UINT16 hcisu_h2_receive_msg(UINT16 hcisu_event, tHCISU_H2_CB *cb) {
    UINT16 total = 0;
    UINT16 ch;
    UINT32 payload_len = 0;
    UINT8 send_now = 0;
    UINT8 byte;
    UINT16 n;
    UINT16 mlen;
    BT_HDR *p_msg;

    if (cb->open != 2) {
        return 0;
    }

    ch = (UINT16)(hcisu_event + 1);

    for (;;) {
        n = UUSB_Read((UINT8)ch, &byte, 1);
        if (n == 0) {
            break;
        }

        send_now = 0;
        total++;

        switch (cb->state[hcisu_event]) {
        case HCISU_H2_STATE_IDLE:
            if (ch == 1) {
                cb->msg[hcisu_event] = GKI_getpoolbuf(2);
                cb->msgtype[hcisu_event] = 4;
            } else if (ch == 2) {
                cb->msg[hcisu_event] = GKI_getpoolbuf(3);
                cb->msgtype[hcisu_event] = 2;
            } else if (ch == 3) {
                cb->msg[hcisu_event] = GKI_getpoolbuf(1);
                cb->msgtype[hcisu_event] = 3;
            } else {
                break;
            }

            if (cb->msg[hcisu_event] != NULL) {
                cb->msg[hcisu_event]->len = 0;
                cb->msg[hcisu_event]->event =
                    hcisu_msg_evt_table[cb->msgtype[hcisu_event] - 1];
                cb->msg[hcisu_event]->offset = 0;
                cb->state[hcisu_event] = HCISU_H2_STATE_HEADER;
            } else {
                LogMsg_0(0x70000,
                         "HCIS: Unable to allocate buffer for incoming HCI "
                         "message.");
                cb->state[hcisu_event] = HCISU_H2_STATE_DISC;
            }
            cb->len[hcisu_event] =
                hcisu_preamble_table[cb->msgtype[hcisu_event] - 1];
            /* fall through to the header state */
        case HCISU_H2_STATE_HEADER:
            p_msg = cb->msg[hcisu_event];
            mlen = p_msg->len;
            ((UINT8 *)p_msg + BT_HDR_SIZE)[mlen] = byte;
            p_msg->len = mlen + 1;
            if (--cb->len[hcisu_event] == 0) {
                payload_len = byte;
                if (cb->msgtype[hcisu_event] == 2) {
                    payload_len =
                        (UINT16)((byte << 8) + cb->last_byte[hcisu_event]);
                    p_msg = l2cap_link_chk_pkt_start(cb->msg[hcisu_event]);
                    cb->msg[hcisu_event] = p_msg;
                    if (p_msg == NULL) {
                        cb->len[hcisu_event] = (UINT16)payload_len;
                        if (payload_len == 0) {
                            cb->state[hcisu_event] = HCISU_H2_STATE_IDLE;
                            break;
                        }
                        cb->state[hcisu_event] = HCISU_H2_STATE_BAD;
                        break;
                    }
                }
                cb->len[hcisu_event] = (UINT16)payload_len;
                if ((UINT32)(payload_len +
                             hcisu_preamble_table[cb->msgtype[hcisu_event] - 1] +
                             8) > (UINT32)GKI_get_buf_size(cb->msg[hcisu_event])) {
                    GKI_freebuf(cb->msg[hcisu_event]);
                    cb->msg[hcisu_event] = NULL;
                    cb->state[hcisu_event] = HCISU_H2_STATE_BAD;
                    LogMsg_0(0x70000,
                             "HCIS: Invalid length for incoming HCI message.");
                    break;
                }
                if (payload_len != 0) {
                    cb->state[hcisu_event] = HCISU_H2_STATE_DATA;
                    break;
                }
                cb->state[hcisu_event] = HCISU_H2_STATE_IDLE;
                send_now = 1;
                break;
            }
            cb->last_byte[hcisu_event] = byte;
            break;
        case HCISU_H2_STATE_DISC:
            if (--cb->len[hcisu_event] == 0) {
                payload_len = byte;
                if (cb->msgtype[hcisu_event] == 2) {
                    payload_len =
                        (UINT16)((byte << 8) + cb->last_byte[hcisu_event]);
                }
                cb->len[hcisu_event] = (UINT16)payload_len;
                cb->state[hcisu_event] = HCISU_H2_STATE_BAD;
                break;
            }
            cb->last_byte[hcisu_event] = byte;
            break;
        case HCISU_H2_STATE_DATA:
            p_msg = cb->msg[hcisu_event];
            mlen = p_msg->len;
            ((UINT8 *)p_msg + BT_HDR_SIZE)[mlen] = byte;
            p_msg->len = mlen + 1;
            cb->len[hcisu_event] -= 1;
            n = UUSB_Read((UINT8)(hcisu_event + 1),
                          (UINT8 *)cb->msg[hcisu_event] + BT_HDR_SIZE +
                              cb->msg[hcisu_event]->len,
                          cb->len[hcisu_event]);
            total += n;
            cb->msg[hcisu_event]->len += n;
            cb->len[hcisu_event] -= n;
            if (cb->len[hcisu_event] != 0) {
                break;
            }
            if (cb->msgtype[hcisu_event] == 2 && !l2cap_link_chk_pkt_end()) {
                cb->state[hcisu_event] = HCISU_H2_STATE_IDLE;
                break;
            }
            cb->state[hcisu_event] = HCISU_H2_STATE_IDLE;
            send_now = 1;
            break;
        case HCISU_H2_STATE_BAD:
            if (--cb->len[hcisu_event] != 0) {
                break;
            }
            cb->state[hcisu_event] = HCISU_H2_STATE_IDLE;
            break;
        }

        if (send_now) {
            GKI_send_msg(hcisu_h2_cb[0x1F], 0, cb->msg[hcisu_event]);
            cb->msg[hcisu_event] = NULL;
        }
    }

    return total;
}

#pragma push
#pragma auto_inline off
void hcisu_h2_send_msg_now(tHCISU_H2_CB *p_cb, BT_HDR *p_buf)
{
    UINT16 event;
    UINT16 h2type;
    UINT8 h2type_hi;
    UINT8 type;
    UINT8 *p_data;
    UINT8 *p;

    event = p_buf->event;
    p_data = (UINT8 *)p_buf + p_buf->offset + BT_HDR_SIZE;

    if (event == 0x2100) {
        type = 2;
    } else if (event == 0x2200) {
        type = 3;
    } else if (event == 0x2000) {
        type = 0;
    }

    if (event == 0x2100) {
        if (p_buf->len > btu_cb.hcit_acl_pkt_size) {
            h2type = (UINT16)(p_data[0] + ((UINT16)p_data[1] << 8));
            h2type = (UINT16)((h2type & 0xCFFF) | 0x1000);
            h2type_hi = (UINT8)(h2type >> 8);
            p_data += 2;

            while (p_buf->len > btu_cb.hcit_acl_pkt_size) {
                UUSB_Write(type, p_data, btu_cb.hcit_acl_pkt_size, p_buf);

                p_buf->offset = p_buf->offset + btu_cb.hcit_acl_data_size;
                p = (UINT8 *)(p_buf + 1) + p_buf->offset;
                p_buf->len = p_buf->len - btu_cb.hcit_acl_data_size;

                p[0] = (UINT8)h2type;
                p[1] = h2type_hi;
                p += 2;

                if (p_buf->len > btu_cb.hcit_acl_pkt_size) {
                    p[0] = (UINT8)btu_cb.hcit_acl_data_size;
                    p[1] = (UINT8)(btu_cb.hcit_acl_data_size >> 8);
                } else {
                    p[0] = (UINT8)(p_buf->len - 4);
                    p[1] = (UINT8)((p_buf->len - 4) >> 8);
                }
                p += 2;
                p_data = p;

                if (p_buf->layer_specific != 0) {
                    p_buf->layer_specific--;
                    if (p_buf->layer_specific == 0) {
                        p_buf->event = 0x1900;
                        GKI_send_msg(hcisu_h2_cb[0x1F], 0, p_buf);
                        return;
                    }
                }
            }
        }
    }

    UUSB_Write(type, p_data, p_buf->len, p_buf);
    GKI_freebuf(p_buf);
}
#pragma pop

void hcisu_h2_init(unsigned char arg0, unsigned char arg1, unsigned short arg2) {
    unsigned char *base = hcisu_h2_cb;
    base[0x1e] = 0;
    base[0x1a] = 0;
    base[0x1b] = 0;
    base[0x1c] = 0;
    base[0x1f] = arg0;
    base[0x20] = arg1;
    *((unsigned short *)(base + 0x12)) = arg2;
}

BOOLEAN hcisu_h2_open(UINT16 *p_bd_addr) {
    unsigned char *base = hcisu_h2_cb;
    tUUSB_CFG cfg;

    cfg.cfg_byte = 0;
    cfg.vid = p_bd_addr[0];
    cfg.pid = p_bd_addr[1];
    cfg.task_id = base[0x1F];
    base[0x1E] = 2;

    UUSB_Register(&cfg);
    UUSB_Open((s32)&cfg, hcisu_h2_usb_cback);

    return TRUE;
}

void hcisu_h2_close() {
    hcisu_h2_cb[0x1e] = 0;
    UUSB_Close();
    UUSB_Unregister();
}

UINT8 hcisu_h2_send(BT_HDR *p_buf) {
    hcisu_h2_send_msg_now((tHCISU_H2_CB *)hcisu_h2_cb, p_buf);
    return 1;
}

UINT16 hcisu_h2_handle_event(UINT16 hcisu_event) {
    UINT16 event;

    event = (UINT16)((hcisu_event - 8) - *(UINT16 *)&hcisu_h2_cb[0x12]);
    hcisu_h2_receive_msg(event, (tHCISU_H2_CB *)hcisu_h2_cb);
    return 0;
}

/* HCI transport interface table (retail .rodata, 6 pointers). Referenced by
   bte_main.c via `extern tHCI_IF hcisu_h2;`. */
const void *const hcisu_h2[6] = {
    (const void *)hcisu_h2_init,
    (const void *)hcisu_h2_open,
    (const void *)hcisu_h2_close,
    (const void *)hcisu_h2_send,
    (const void *)hcisu_h2_handle_event,
    NULL,
};
