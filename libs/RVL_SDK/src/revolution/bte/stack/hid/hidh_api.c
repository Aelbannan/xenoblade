// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/stack/hid/hidh_api
// Replace stubs with high-level C/C++ during decomp.

/* bt_target.h defaults to 7; retail binary uses 16 */
#define HID_HOST_MAX_DEVICES 16

#include <harness_catalog.h>
#include <revolution/bte/gki/common/gki.h>
#include <revolution/bte/stack/include/btu.h>
#include <revolution/bte/include/bt_trace.h>

enum {
    HID_SUCCESS,
    HID_ERR_NOT_REGISTERED,
    HID_ERR_ALREADY_REGISTERED,
    HID_ERR_NO_RESOURCES,
    HID_ERR_NO_CONNECTION,
    HID_ERR_INVALID_PARAM,
    HID_ERR_UNSUPPORTED,
    HID_ERR_UNKNOWN_COMMAND,
    HID_ERR_CONGESTED,
    HID_ERR_CONN_IN_PROCESS,
    HID_ERR_ALREADY_CONN,
    HID_ERR_DISCONNECTING,
    HID_ERR_SET_CONNABLE_FAIL,
    HID_ERR_HOST_UNKNOWN,
    HID_ERR_L2CAP_FAILED,
    HID_ERR_AUTH_FAILED,
    HID_ERR_SDP_BUSY,
    HID_ERR_INVALID = 0xFF
};

typedef u8 tHID_STATUS;

enum {
    HID_DEV_NO_CONN,
    HID_DEV_CONNECTED,
};

/* Internal device control block (0x34 bytes per device) */
typedef struct {
    u8 in_use;              /* offset 0x00 */
    u8 addr[6];             /* offset 0x01: BD_ADDR */
    u8 pad_07;              /* offset 0x07 */
    u16 attr_mask;          /* offset 0x08 */
    u8 state;               /* offset 0x0a: connection state */
    u8 conn_substate;       /* offset 0x0b */
    u8 conn_tries;          /* offset 0x0c */
    u8 pad_0d[3];           /* offset 0x0d */
    /* tHID_CONN embedded at offset 0x10 */
    u8 conn_state;          /* offset 0x10 */
    u8 conn_flags;          /* offset 0x11 */
    u8 ctrl_id;             /* offset 0x12 */
    u8 pad_13;              /* offset 0x13 */
    u16 ctrl_cid;           /* offset 0x14 */
    u16 intr_cid;           /* offset 0x16 */
    u16 rem_mtu_size;       /* offset 0x18 */
    u16 disc_reason;        /* offset 0x1a */
    TIMER_LIST_ENT timer_entry; /* offset 0x1c: 0x18 bytes */
} tHID_HOST_DEV_CTB;

/* HID host control block (0x404 bytes total) */
typedef struct {
    tHID_HOST_DEV_CTB devices[HID_HOST_MAX_DEVICES]; /* offset 0x000 */
    void *dev_cback;        /* offset 0x340: tHID_HOST_DEV_CALLBACK* */
    u8 pad_344[0xBC];       /* offset 0x344 */
    u8 reg_flag;            /* offset 0x400: registration flag */
    u8 trace_level;         /* offset 0x401 */
    u8 pad_402[2];          /* offset 0x402 */
} tHID_HOST_CTB;

tHID_HOST_CTB hh_cb;

void HID_HostGetSDPRecord() {}

void hidh_search_callback() {}

extern tHID_STATUS hidh_conn_initiate(u8 dev_handle);
extern tHID_STATUS hidh_conn_reg(void);
extern void hidh_conn_dereg(void);
extern tHID_STATUS hidh_conn_disconnect(u8 dev_handle);
extern tHID_STATUS hidh_conn_snd_data(u8 dhandle, u8 trans_type, u8 param,
                                       u16 data, u8 rpt_id, BT_HDR *buf);

void HID_HostInit(void)
{
    memset(&hh_cb, 0, sizeof(tHID_HOST_CTB));
    hh_cb.trace_level = 0;
}

tHID_STATUS HID_HostRegister(struct tHID_HOST_DEV_CALLBACK *dev_cback)
{
    if (hh_cb.reg_flag) {
        return HID_ERR_ALREADY_REGISTERED;
    }
    if (!dev_cback) {
        return HID_ERR_INVALID_PARAM;
    }
    {
        tHID_STATUS status = hidh_conn_reg();
        if (status) {
            return status;
        }
    }
    hh_cb.dev_cback = dev_cback;
    hh_cb.reg_flag = 1;
    return HID_SUCCESS;
}

tHID_STATUS HID_HostDeregister(void)
{
    u8 i;

    if (!hh_cb.reg_flag) {
        return HID_ERR_NOT_REGISTERED;
    }

    for (i = 0; i < HID_HOST_MAX_DEVICES; i++) {
        if (hh_cb.reg_flag && i <= HID_HOST_MAX_DEVICES &&
            hh_cb.devices[i].in_use) {
            if (hh_cb.reg_flag && i <= HID_HOST_MAX_DEVICES &&
                hh_cb.devices[i].in_use) {
                hh_cb.devices[i].conn_tries = 1;
                btu_stop_timer(&hh_cb.devices[i].timer_entry);
                if (hh_cb.devices[i].state == HID_DEV_CONNECTED) {
                    hh_cb.devices[i].conn_tries = 1;
                    hidh_conn_disconnect(i);
                }
            }
            hh_cb.devices[i].in_use = 0;
            hh_cb.devices[i].conn_state = 0;
            hh_cb.devices[i].intr_cid = 0;
            hh_cb.devices[i].ctrl_cid = 0;
        }
    }

    hidh_conn_dereg();
    hh_cb.reg_flag = 0;
    return HID_SUCCESS;
}

void HID_HostAddDev() {}

void HID_HostRemoveDev() {}

tHID_STATUS HID_HostOpenDev(u8 dev_handle)
{
    if (!hh_cb.reg_flag) {
        return HID_ERR_NOT_REGISTERED;
    }
    if (dev_handle > 0x10 || !hh_cb.devices[dev_handle].in_use) {
        return HID_ERR_INVALID_PARAM;
    }
    if (hh_cb.devices[dev_handle].state != 0) {
        return HID_ERR_ALREADY_CONN;
    }
    hh_cb.devices[dev_handle].conn_tries = 1;
    return hidh_conn_initiate(dev_handle);
}

tHID_STATUS HID_HostWriteDev(u8 dev_handle, u8 t_type, u8 param, u16 data,
                             u8 report_id, BT_HDR *pbuf)
{
    tHID_STATUS status = HID_SUCCESS;

    if (!hh_cb.reg_flag) {
        HIDH_TRACE_ERROR0("HID_ERR_NOT_REGISTERED");
        status = HID_ERR_NOT_REGISTERED;
    }

    if (dev_handle > HID_HOST_MAX_DEVICES || !hh_cb.devices[dev_handle].in_use) {
        HIDH_TRACE_ERROR0("HID_ERR_INVALID_PARAM");
        status = HID_ERR_INVALID_PARAM;
    }

    if (hh_cb.devices[dev_handle].state != HID_DEV_CONNECTED) {
        HIDH_TRACE_ERROR1("HID_ERR_NO_CONNECTION dev_handle %d", dev_handle);
        status = HID_ERR_NO_CONNECTION;
    }

    if (status != HID_SUCCESS) {
        if (pbuf) {
            GKI_freebuf(pbuf);
        }
    } else {
        status = hidh_conn_snd_data(dev_handle, t_type, param, data, report_id,
                                    pbuf);
    }

    return status;
}

tHID_STATUS HID_HostCloseDev(u8 dev_handle)
{
    if (!hh_cb.reg_flag) {
        return HID_ERR_NOT_REGISTERED;
    }
    if (dev_handle > 0x10 || !hh_cb.devices[dev_handle].in_use) {
        return HID_ERR_INVALID_PARAM;
    }
    hh_cb.devices[dev_handle].conn_tries = 1;
    btu_stop_timer(&hh_cb.devices[dev_handle].timer_entry);
    if (hh_cb.devices[dev_handle].state != 1) {
        return HID_ERR_NO_CONNECTION;
    }
    hh_cb.devices[dev_handle].conn_tries = 1;
    return hidh_conn_disconnect(dev_handle);
}

void HID_HostSetSecurityLevel() {}
