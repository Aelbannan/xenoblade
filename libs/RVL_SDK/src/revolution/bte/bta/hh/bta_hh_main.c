// Decompiled high-level C for libs/RVL_SDK/src/revolution/bte/bta/hh/bta_hh_main

#include <harness_catalog.h>
#include <string.h>

/* BTE data types must come before any BTE headers */
#include <revolution/BTE/gki/platform/data_types.h>
#include <revolution/BTE/stack/include/bt_types.h>
#include <revolution/BTE/bta/include/bd.h>
#include <revolution/BTE/bta/include/bta_hh_api.h>

/* Trace/logging function declarations (from bt_trace.h, isolated to avoid dependency chain) */
extern UINT8 appl_trace_level;
extern void LogMsg_1(UINT32 trace_set_mask, const char *fmt_str, UINT32 p1);
extern void LogMsg_2(UINT32 trace_set_mask, const char *fmt_str, UINT32 p1, UINT32 p2);
extern void LogMsg_3(UINT32 trace_set_mask, const char *fmt_str, UINT32 p1, UINT32 p2, UINT32 p3);

/* Device control block: 0x20 bytes each, state at offset 0x1c */
typedef struct {
    UINT8 _pad[0x1c];
    UINT8 state;
    UINT8 _pad2[3];
} tBTA_HH_DEV_CB;

/* Main control block at 0x805BC0C8, total 0x230 bytes */
typedef struct {
    UINT8 _pad1[0x10];
    tBTA_HH_DEV_CB dev_cb[16];
    UINT8 _pad2[4];
    UINT8 handle_to_idx[16];
    void (*cback)(UINT16, void *);
    UINT8 _pad3[8];
} tBTA_HH_CB;

extern tBTA_HH_CB bta_hh_cb;

/* rodata tables */
extern const UINT32 bta_hh_st_idle[];
extern const UINT32 bta_hh_st_w4_conn[];
extern const UINT32 bta_hh_st_connected[];
extern const UINT32 *bta_hh_st_tbl[];

/* Action function table */
extern void (*bta_hh_action[])(void *, void *);

/* Forward declarations */
extern void bta_hh_api_enable(void);
extern void bta_hh_api_disable(void);
extern void bta_hh_disc_cmpl(void);
extern void bta_hh_get_acl_q_info(void);
extern UINT8 bta_hh_find_cb(UINT8 *bda);
extern const char *bta_hh_evt_code(UINT16 event);

/* State name strings used in trace */
static const char *bta_hh_state_name(UINT8 state)
{
    switch (state) {
    case 0:
        return "BTA_HH_NULL_ST";
    case 1:
        return "BTA_HH_IDLE_ST";
    case 2:
        return "BTA_HH_W4_CONN_ST";
    case 3:
        return "BTA_HH_CONN_ST";
    default:
        return "unknown HID Host state";
    }
}

/* ---- bta_hh_sm_execute ------------------------------------------------ */
void bta_hh_sm_execute(void *p_cb, UINT16 event, void *p_data)
{
    const UINT8 (*state_table)[2];
    UINT8 action;
    UINT8 cback_data[0x20];
    UINT16 cback_event;
    UINT8 old_state;
    UINT8 new_state;
    const char *evt_str;
    const char *state_str;
    const char *new_state_str;

    cback_event = 0;
    memset(cback_data, 0, sizeof(cback_data));

    if (p_cb == NULL) {
        if (bta_hh_cb.cback) {
            switch (event) {
            case 0x1700:
                cback_event = 2;
                bdcpy((UINT8 *)cback_data, (UINT8 *)p_data + 8);
                cback_data[0x06] = 0x09;
                cback_data[0x07] = 0xFF;
                break;
            case 0x170A: {
                UINT16 sub_type;

                sub_type = *(UINT16 *)((UINT8 *)p_data + 0x10);
                cback_event = sub_type;
                if (sub_type == 0x0B) {
                    bdcpy((UINT8 *)cback_data, (UINT8 *)p_data + 8);
                    cback_data[0x06] = 0x09;
                    cback_data[0x07] = 0xFF;
                } else {
                    cback_data[0x06] = 0x0D;
                    cback_data[0x07] = *(UINT16 *)((UINT8 *)p_data + 6);
                }
                break;
            }
            case 0x1708: {
                UINT8 sub_type;

                sub_type = *(UINT8 *)((UINT8 *)p_data + 8);
                cback_event = sub_type;
                if (sub_type == 0x07 || sub_type == 0x05 || sub_type == 0x09) {
                    cback_data[0x00] = 0x0D;
                    cback_data[0x01] = *(UINT16 *)((UINT8 *)p_data + 6);
                } else if (sub_type != 0x0A && sub_type != 0x01) {
                    cback_data[0x01] = *(UINT16 *)((UINT8 *)p_data + 6);
                    cback_data[0x00] = 0x0D;
                }
                break;
            }
            case 0x1701:
                cback_data[0x00] = 0x0D;
                cback_event = 3;
                cback_data[0x01] = *(UINT16 *)((UINT8 *)p_data + 6);
                break;
            default:
                if (appl_trace_level >= 1) {
                    LogMsg_1(0x500, "wrong device handle: [%d]",
                             (UINT32)*(UINT16 *)((UINT8 *)p_data + 6));
                }
                break;
            }
            if (cback_event) {
                bta_hh_cb.cback(cback_event, cback_data);
            }
        }
        return;
    }

    old_state = *(UINT8 *)((UINT8 *)p_cb + 0x1c);

    if (appl_trace_level >= 4) {
        state_str = bta_hh_state_name(old_state);
        evt_str = bta_hh_evt_code(event);
        LogMsg_3(0x503, "bta_hh_sm_execute: State 0x%02x [%s], Event [%s]",
                 (UINT32)old_state, (UINT32)state_str, (UINT32)evt_str);
    }

    event &= 0xFF;
    state_table = (const UINT8 (*)[2])bta_hh_st_tbl[*(UINT8 *)((UINT8 *)p_cb + 0x1c) - 1];
    new_state = state_table[event][1];
    *(UINT8 *)((UINT8 *)p_cb + 0x1c) = new_state;

    if ((action = state_table[event][0]) != 0x0C) {
        bta_hh_action[action](p_cb, p_data);
    }

    if (old_state != *(UINT8 *)((UINT8 *)p_cb + 0x1c)) {
        if (appl_trace_level >= 5) {
            state_str = bta_hh_state_name(old_state);
            new_state_str = bta_hh_state_name(*(UINT8 *)((UINT8 *)p_cb + 0x1c));
            evt_str = bta_hh_evt_code(event);
            LogMsg_3(0x504, "HH State Change: [%s] -> [%s] after Event [%s]",
                     (UINT32)state_str, (UINT32)new_state_str, (UINT32)evt_str);
        }
    }
}

/* ---- bta_hh_hdl_event ------------------------------------------------- */
unsigned char bta_hh_hdl_event(BT_HDR *p_msg)
{
    void *p_cb;
    UINT8 dev_cb_idx;

    p_cb = NULL;
    dev_cb_idx = 0x10;

    switch (p_msg->event) {
    case 0x170C:
        bta_hh_api_enable();
        break;
    case 0x170D:
        bta_hh_api_disable();
        break;
    case 0x170F:
        bta_hh_disc_cmpl();
        break;
    case 0x170E:
        bta_hh_get_acl_q_info();
        break;
    default:
        if (p_msg->event == 0x1700) {
            dev_cb_idx = bta_hh_find_cb((UINT8 *)p_msg + 8);
        } else if (p_msg->event == 0x170A) {
            UINT16 sub_type;

            sub_type = *(UINT16 *)((UINT8 *)p_msg + 0x10);
            if (sub_type == 0x0B) {
                dev_cb_idx = bta_hh_find_cb((UINT8 *)p_msg + 8);
            } else {
                UINT16 handle;

                handle = *(UINT16 *)((UINT8 *)p_msg + 6);
                dev_cb_idx = bta_hh_cb.handle_to_idx[handle];
            }
        } else {
            UINT16 handle;

            handle = *(UINT16 *)((UINT8 *)p_msg + 6);
            if (handle < 0x10) {
                dev_cb_idx = bta_hh_cb.handle_to_idx[handle];
            }
        }

        if (dev_cb_idx != 0x10) {
            p_cb = &bta_hh_cb.dev_cb[dev_cb_idx];
        }

        if (appl_trace_level >= 5) {
            LogMsg_2(0x504, "bta_hh_hdl_event:: handle = %d dev_cb[%d] ",
                     (UINT32)*(UINT16 *)((UINT8 *)p_msg + 6), (UINT32)dev_cb_idx);
        }

        bta_hh_sm_execute(p_cb, p_msg->event, p_msg);
        break;
    }

    return 1;
}
/* ---- bta_hh_evt_code -------------------------------------------------- */
const char *bta_hh_evt_code(UINT16 event)
{
    switch (event) {
    case 0x170D: return "BTA_HH_API_DISABLE_EVT";
    case 0x170C: return "BTA_HH_API_ENABLE_EVT";
    case 0x1700: return "BTA_HH_API_OPEN_EVT";
    case 0x1701: return "BTA_HH_API_CLOSE_EVT";
    case 0x1702: return "BTA_HH_INT_OPEN_EVT";
    case 0x1703: return "BTA_HH_INT_CLOSE_EVT";
    case 0x1706: return "BTA_HH_INT_HANDSK_EVT";
    case 0x1704: return "BTA_HH_INT_DATA_EVT";
    case 0x1705: return "BTA_HH_INT_CTRL_DATA";
    case 0x1708: return "BTA_HH_API_WRITE_DEV_EVT";
    case 0x1707: return "BTA_HH_SDP_CMPL_EVT";
    case 0x170F: return "BTA_HH_DISC_CMPL_EVT";
    case 0x170A: return "BTA_HH_API_MAINT_DEV_EVT";
    case 0x1709: return "BTA_HH_API_GET_DSCP_EVT";
    case 0x170B: return "BTA_HH_OPEN_CMPL_EVT";
    case 0x170E: return "BTA_HH_API_GET_ACL_Q_EVT";
    default: return "unknown HID Host event code";
    }
}

