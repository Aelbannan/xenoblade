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

/* State name strings used in trace */
static const char *bta_hh_state_name(UINT8 state)
{
    if (state == 2)
        return "BTA_HH_W4_CONN_ST";
    if (state > 2) {
        if (state >= 4)
            return "unknown HID Host state";
        return "BTA_HH_CONN_ST";
    }
    if (state == 0)
        return "BTA_HH_NULL_ST";
    return "BTA_HH_IDLE_ST";
}

/* ---- bta_hh_evt_code -------------------------------------------------- */
const char *bta_hh_evt_code(UINT16 event)
{
    /* Pool-order seeds: retail .data pools these sibling strings first
     * (sm_execute / state_name / hdl_event traces); referenced here so the
     * evt strings land at the retail offsets. Dead after pooling. */
    static const char *const s_pool[] = {
        "wrong device handle: [%d]",
        "BTA_HH_NULL_ST",
        "BTA_HH_IDLE_ST",
        "BTA_HH_W4_CONN_ST",
        "BTA_HH_CONN_ST",
        "unknown HID Host state",
        "bta_hh_sm_execute: State 0x%02x [%s], Event [%s]",
        "HH State Change: [%s] -> [%s] after Event [%s]",
        "bta_hh_hdl_event:: handle = %d dev_cb[%d] ",
    };
    (void)s_pool;

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

/* ---- bta_hh_sm_execute ------------------------------------------------ */
void bta_hh_sm_execute(void *p_cb, UINT16 event, void *p_data)
{
    UINT8 local[0x20];
    UINT16 action;
    UINT8 old_state;
    UINT8 evt;
    UINT8 *st_base;
    UINT8 new_state;
    const char *evt_str;
    const char *state_str;

    action = 0;
    memset(local, 0, sizeof(local));

    if (p_cb == NULL) {
        if (bta_hh_cb.cback) {
            if (event == 0x1708) {
                UINT8 sub_type;

                sub_type = *(UINT8 *)((UINT8 *)p_data + 8);
                action = sub_type;
                if (sub_type == 0x07 || sub_type == 0x05 || sub_type == 0x09) {
                    local[0x08] = 0x0D;
                    local[0x09] = *(UINT8 *)((UINT8 *)p_data + 6);
                } else {
                    if (sub_type != 0x0A && sub_type != 0x01) {
                        local[0x08] = 0x0D;
                        local[0x09] = *(UINT8 *)((UINT8 *)p_data + 6);
                    }
                }
            } else if (event > 0x1708) {
                if (event == 0x170A) {
                    UINT16 sub_type;

                    sub_type = *(UINT16 *)((UINT8 *)p_data + 0x10);
                    action = sub_type;
                    if (sub_type == 0x0B) {
                        bdcpy((UINT8 *)local, (UINT8 *)p_data + 8);
                        local[0x0E] = 0x09;
                        local[0x0F] = 0xFF;
                    } else {
                        local[0x0E] = 0x0D;
                        local[0x0F] = *(UINT8 *)((UINT8 *)p_data + 6);
                    }
                } else {
                    action = 0;
                    if (appl_trace_level >= 1) {
                        LogMsg_1(0x500, "wrong device handle: [%d]",
                                 (UINT32)*(UINT16 *)((UINT8 *)p_data + 6));
                    }
                }
            } else if (event == 0x1701) {
                local[0x08] = 0x0D;
                local[0x09] = *(UINT8 *)((UINT8 *)p_data + 6);
                action = 3;
            } else if (event >= 0x1700) {
                bdcpy((UINT8 *)local, (UINT8 *)p_data + 8);
                local[0x0E] = 0x09;
                local[0x0F] = 0xFF;
                action = 2;
            } else {
                action = 0;
                if (appl_trace_level >= 1) {
                    LogMsg_1(0x500, "wrong device handle: [%d]",
                             (UINT32)*(UINT16 *)((UINT8 *)p_data + 6));
                }
            }

            if ((UINT16)action != 0) {
                bta_hh_cb.cback(action, local);
            }
        }
        return;
    }

    old_state = *(UINT8 *)((UINT8 *)p_cb + 0x1c);

    if (appl_trace_level >= 4) {
        evt_str = bta_hh_evt_code(event);
        state_str = bta_hh_state_name(old_state);
        LogMsg_3(0x503, "bta_hh_sm_execute: State 0x%02x [%s], Event [%s]",
                 (UINT32)old_state, (UINT32)state_str, (UINT32)evt_str);
    }

    evt = (UINT8)event;
    st_base = (UINT8 *)bta_hh_st_tbl[old_state - 1];
    action = st_base[evt * 2];
    new_state = st_base[evt * 2 + 1];
    *(UINT8 *)((UINT8 *)p_cb + 0x1c) = new_state;

    if (action != 0x0C) {
        bta_hh_action[action](p_cb, p_data);
    }

    if (old_state != new_state) {
        if (appl_trace_level >= 5) {
            evt_str = bta_hh_evt_code(evt);
            LogMsg_3(0x504, "HH State Change: [%s] -> [%s] after Event [%s]",
                     (UINT32)bta_hh_state_name(old_state),
                     (UINT32)bta_hh_state_name(new_state),
                     (UINT32)evt_str);
        }
    }
}

/* ---- bta_hh_hdl_event ------------------------------------------------- */
unsigned char bta_hh_hdl_event(BT_HDR *p_msg)
{
    void *p_cb;
    UINT8 dev_cb_idx;
    UINT16 event;

    p_cb = NULL;
    dev_cb_idx = 0x10;
    event = p_msg->event;

    if (event == 0x170E) {
        bta_hh_get_acl_q_info();
        return 1;
    }

    if (event > 0x170E) {
        if (event >= 0x1710) {
            /* fall through */
        } else {
            bta_hh_disc_cmpl();
            return 1;
        }
    } else if (event == 0x170C) {
        bta_hh_api_enable();
        return 1;
    } else if (event >= 0x170C) {
        bta_hh_api_disable();
        return 1;
    }

    if (event == 0x1700) {
        dev_cb_idx = bta_hh_find_cb((UINT8 *)p_msg + 8);
    } else if (event == 0x170A) {
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

    if (dev_cb_idx < 0x10) {
        p_cb = &bta_hh_cb.dev_cb[dev_cb_idx];
    }

    if (appl_trace_level >= 5) {
        LogMsg_2(0x504, "bta_hh_hdl_event:: handle = %d dev_cb[%d] ",
                 (UINT32)*(UINT16 *)((UINT8 *)p_msg + 6), (UINT32)dev_cb_idx);
    }

    bta_hh_sm_execute(p_cb, event, p_msg);

    return 1;
}
