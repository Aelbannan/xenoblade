// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/stack/hid/hidh_api
// Replace stubs with high-level C/C++ during decomp.

/* bt_target.h defaults to 7; retail binary uses 16 */
#define HID_HOST_MAX_DEVICES 16

/* Security service registration (hiddefs.h / btm_api.h) */
#define HID_SERVICE_ID      0x20
#define BTM_SEC_PROTO_HID   6

#include <harness_catalog.h>
#include <revolution/bte/gki/common/gki.h>
#include <revolution/bte/stack/include/btu.h>
#include <revolution/bte/stack/include/sdp_api.h>
#include <revolution/bte/include/bt_trace.h>

extern BOOLEAN BTM_SetSecurityLevel (BOOLEAN is_originator, char *p_name,
                                     UINT8 service_id, UINT16 sec_level,
                                     UINT16 psm, UINT32 mx_proto_id,
                                     UINT32 mx_chan_id);

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

/* SDP device information extracted by hidh_search_callback (0x74 bytes).
   sdp_rec lives inside this struct so the memset in the search callback
   clears it together with the names/attributes. */
typedef struct {
    char    dev_name[32];      /* 0x00 HID_ATTR_DEVICE_NAME (0x100) */
    char    vendor_name[32];   /* 0x20 HID_ATTR_VENDOR_NAME (0x101) */
    char    product_name[32];  /* 0x40 HID_ATTR_PRODUCT_NAME (0x102) */
    u16     version;           /* 0x60 HID_ATTR_VERSION (0x200) */
    u16     product_id;        /* 0x62 HID_ATTR_PRODUCT_ID (0x201) */
    u8      subclass;          /* 0x64 HID_ATTR_SUBCLASS (0x202) */
    u8      country_code;      /* 0x65 HID_ATTR_COUNTRY_CODE (0x203) */
    u16     sdp_disable;       /* 0x66 HID_ATTR_SDP_DISABLE (0x20C) */
    u16     attr_mask;         /* 0x68 HID_ATTR_MASK */
    u8      *viral_cable;      /* 0x6C HID_ATTR_VIRTUAL_CABLE (0x206) */
    void    *sdp_rec;          /* 0x70 tSDP_DISC_REC* found by the search */
} tHID_DEV_SDP_INFO;

typedef void (tHID_SDP_CBACK)(u16 result, u16 attr_mask,
                              tHID_DEV_SDP_INFO *p_info);

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
    u8 pad_344[0x3C];       /* offset 0x344 */
    u8 sdp_busy;            /* offset 0x380: discovery in progress */
    u8 pad_381[3];          /* offset 0x381 */
    tHID_SDP_CBACK *sdp_cback; /* offset 0x384: search completion cb */
    void *sdp_db;           /* offset 0x388: tSDP_DISCOVERY_DB* */
    tHID_DEV_SDP_INFO sdp_info; /* offset 0x38C */
    u8 reg_flag;            /* offset 0x400: registration flag */
    u8 trace_level;         /* offset 0x401 */
    u8 pad_402[2];          /* offset 0x402 */
} tHID_HOST_CTB;

// Retail .bss is 0x408: 4 pad bytes after hh_cb. Declared before hh_cb so
// the reverse-order .bss emission places them after it.
#pragma sdata_threshold 0
unsigned char hidh_bss_pad4;
unsigned char hidh_bss_pad3;
unsigned char hidh_bss_pad2;
unsigned char hidh_bss_pad1;
#pragma sdata_threshold 8
tHID_HOST_CTB hh_cb;

void hidh_search_callback(u16 result, void *p_data);

tHID_STATUS HID_HostGetSDPRecord (BD_ADDR addr, tSDP_DISCOVERY_DB *p_db,
                                  UINT32 len, tHID_SDP_CBACK *p_cback)
{
    tSDP_UUID uuid;

    if (hh_cb.sdp_busy) {
        return (HID_ERR_SDP_BUSY);
    }

    hh_cb.sdp_db = p_db;

    uuid.len = LEN_UUID_16;
    uuid.uu.uuid16 = 0x1124;

    SDP_InitDiscoveryDb(p_db, len, 1, &uuid, 0, NULL);

    if (SDP_ServiceSearchRequest(addr, p_db,
                                 (tSDP_DISC_CMPL_CB *)hidh_search_callback)) {
        hh_cb.sdp_cback = p_cback;
        hh_cb.sdp_busy = TRUE;
        return (HID_SUCCESS);
    } else {
        return (HID_ERR_NO_RESOURCES);
    }
}

/* SDP service-search completion callback (registered via
   SDP_ServiceSearchRequest). Scans the discovery database for the HID
   service record and copies the device attributes into hh_cb.sdp_info before
   notifying the upper layer through the callback stored in hh_cb.sdp_cback. */
void hidh_search_callback(u16 result, void *p_data)
{
    tSDP_DISC_REC *p_rec;
    tSDP_DISC_ATTR *p_attr;
    tHID_DEV_SDP_INFO *p_sdp_info;
    tSDP_DISCOVERY_DB *p_db;
    u16 attr_mask = 0;
    u16 len;
    tBT_UUID uuid;

    p_sdp_info = &hh_cb.sdp_info;
    uuid.len = LEN_UUID_16;
    p_db = (tSDP_DISCOVERY_DB *)hh_cb.sdp_db;
    uuid.uu.uuid16 = 0x1124;

    hh_cb.sdp_busy = 0;
    if (result != 0) {
        hh_cb.sdp_cback(result, 0, NULL);
        return;
    }

    p_rec = SDP_FindServiceUUIDInDb(p_db, &uuid, NULL);
    if (p_rec == NULL) {
        hh_cb.sdp_cback(0x0C, 0, NULL);
        return;
    }

    memset(p_sdp_info, 0, sizeof(tHID_DEV_SDP_INFO));

    /* HID virtual cable: the attribute value is a data element sequence
       whose second element is the cable descriptor text string. */
    p_attr = SDP_FindAttributeInRec(p_rec, 0x206);
    if (p_attr == NULL ||
        SDP_DISC_ATTR_TYPE(p_attr->attr_len_type) != DATA_ELE_SEQ_DESC_TYPE ||
        (p_attr = p_attr->attr_value.v.p_sub_attr) == NULL ||
        SDP_DISC_ATTR_TYPE(p_attr->attr_len_type) != DATA_ELE_SEQ_DESC_TYPE ||
        (p_attr = p_attr->attr_value.v.p_sub_attr) == NULL ||
        (p_attr = p_attr->p_next_attr) == NULL ||
        SDP_DISC_ATTR_TYPE(p_attr->attr_len_type) != TEXT_STR_DESC_TYPE) {
        hh_cb.sdp_cback(HID_ERR_HOST_UNKNOWN, 0, NULL);
        return;
    }
    p_sdp_info->attr_mask = SDP_DISC_ATTR_LEN(p_attr->attr_len_type);
    if (p_sdp_info->attr_mask != 0) {
        p_sdp_info->viral_cable = p_attr->attr_value.v.array;
    }

    /* Supported feature flags (HID attribute presence). */
    p_attr = SDP_FindAttributeInRec(p_rec, 0x204);
    if (p_attr != NULL && p_attr->attr_value.v.u8 != 0) {
        attr_mask |= 0x01;
    }
    p_attr = SDP_FindAttributeInRec(p_rec, 0x205);
    if (p_attr != NULL && p_attr->attr_value.v.u8 != 0) {
        attr_mask |= 0x04;
    }
    p_attr = SDP_FindAttributeInRec(p_rec, 0x20D);
    if (p_attr != NULL && p_attr->attr_value.v.u8 != 0) {
        attr_mask |= 0x02;
    }
    p_attr = SDP_FindAttributeInRec(p_rec, 0x208);
    if (p_attr != NULL && p_attr->attr_value.v.u8 != 0) {
        attr_mask |= 0x08;
    }
    p_attr = SDP_FindAttributeInRec(p_rec, 0x209);
    if (p_attr != NULL && p_attr->attr_value.v.u8 != 0) {
        attr_mask |= 0x10;
    }
    p_attr = SDP_FindAttributeInRec(p_rec, 0x20A);
    if (p_attr != NULL && p_attr->attr_value.v.u8 != 0) {
        attr_mask |= 0x20;
    }

    /* Device / vendor / product names. */
    p_attr = SDP_FindAttributeInRec(p_rec, 0x100);
    if (p_attr != NULL) {
        len = SDP_DISC_ATTR_LEN(p_attr->attr_len_type);
        if (len < 0x20) {
            memcpy(p_sdp_info->dev_name, p_attr->attr_value.v.array, len);
            p_sdp_info->dev_name[len] = 0;
        } else {
            memcpy(p_sdp_info->dev_name, p_attr->attr_value.v.array, 0x1F);
            p_sdp_info->dev_name[0x20] = 0;
        }
    } else {
        p_sdp_info->dev_name[0] = 0;
    }

    p_attr = SDP_FindAttributeInRec(p_rec, 0x101);
    if (p_attr != NULL) {
        len = SDP_DISC_ATTR_LEN(p_attr->attr_len_type);
        if (len < 0x20) {
            memcpy(p_sdp_info->vendor_name, p_attr->attr_value.v.array, len);
            p_sdp_info->vendor_name[len] = 0;
        } else {
            memcpy(p_sdp_info->vendor_name, p_attr->attr_value.v.array, 0x1F);
            p_sdp_info->vendor_name[0x20] = 0;
        }
    } else {
        p_sdp_info->vendor_name[0] = 0;
    }

    p_attr = SDP_FindAttributeInRec(p_rec, 0x102);
    if (p_attr != NULL) {
        len = SDP_DISC_ATTR_LEN(p_attr->attr_len_type);
        if (len < 0x20) {
            memcpy(p_sdp_info->product_name, p_attr->attr_value.v.array, len);
            p_sdp_info->product_name[len] = 0;
        } else {
            memcpy(p_sdp_info->product_name, p_attr->attr_value.v.array, 0x1F);
            p_sdp_info->product_name[0x20] = 0;
        }
    } else {
        p_sdp_info->product_name[0] = 0;
    }

    p_attr = SDP_FindAttributeInRec(p_rec, 0x200);
    if (p_attr != NULL) {
        p_sdp_info->version = p_attr->attr_value.v.u16;
    }
    p_attr = SDP_FindAttributeInRec(p_rec, 0x203);
    if (p_attr != NULL) {
        p_sdp_info->country_code = p_attr->attr_value.v.u8;
    }
    p_attr = SDP_FindAttributeInRec(p_rec, 0x202);
    if (p_attr != NULL) {
        p_sdp_info->subclass = p_attr->attr_value.v.u8;
    }
    p_attr = SDP_FindAttributeInRec(p_rec, 0x201);
    if (p_attr != NULL) {
        p_sdp_info->product_id = p_attr->attr_value.v.u16;
    }
    p_attr = SDP_FindAttributeInRec(p_rec, 0x20C);
    if (p_attr != NULL) {
        attr_mask |= 0x40;
        p_sdp_info->sdp_disable = p_attr->attr_value.v.u16;
    }

    hh_cb.sdp_info.sdp_rec = p_rec;
    hh_cb.sdp_cback(0, attr_mask, &hh_cb.sdp_info);
}

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

tHID_STATUS HID_HostAddDev(BD_ADDR addr, UINT16 attr_mask, UINT8 *handle)
{
    int i;

    if (!hh_cb.reg_flag) {
        return HID_ERR_NOT_REGISTERED;
    }

    for (i = 0; i < HID_HOST_MAX_DEVICES; i++) {
        if (memcmp(addr, hh_cb.devices[i].addr, BD_ADDR_LEN) == 0) {
            break;
        }
    }

    if (i == HID_HOST_MAX_DEVICES) {
        for (i = 0; i < HID_HOST_MAX_DEVICES; i++) {
            if (!hh_cb.devices[i].in_use) {
                break;
            }
        }
    }

    if (i == HID_HOST_MAX_DEVICES) {
        return HID_ERR_NO_RESOURCES;
    }

    if (!hh_cb.devices[i].in_use) {
        hh_cb.devices[i].in_use = 1;
        memcpy(hh_cb.devices[i].addr, addr, BD_ADDR_LEN);
        hh_cb.devices[i].state = 0;
        hh_cb.devices[i].conn_tries = 0;
    }

    hh_cb.devices[i].attr_mask = attr_mask;
    *handle = i;

    return HID_SUCCESS;
}

tHID_STATUS HID_HostRemoveDev (UINT8 dev_handle)
{
    tHID_HOST_DEV_CTB *p_dev;

    if (!hh_cb.reg_flag) {
        return (HID_ERR_NOT_REGISTERED);
    }

    if ((dev_handle > HID_HOST_MAX_DEVICES) ||
        (!hh_cb.devices[dev_handle].in_use)) {
        return (HID_ERR_INVALID_PARAM);
    }

    p_dev = &hh_cb.devices[dev_handle];

    if (hh_cb.reg_flag && (dev_handle <= HID_HOST_MAX_DEVICES) &&
        hh_cb.devices[dev_handle].in_use) {
        hh_cb.devices[dev_handle].conn_tries = 1;
        btu_stop_timer(&hh_cb.devices[dev_handle].timer_entry);

        if (hh_cb.devices[dev_handle].state == HID_DEV_CONNECTED) {
            hh_cb.devices[dev_handle].conn_tries = 1;
            hidh_conn_disconnect(dev_handle);
        }
    }

    p_dev->in_use = 0;
    hh_cb.devices[dev_handle].conn_state = 0;
    hh_cb.devices[dev_handle].intr_cid = 0;
    hh_cb.devices[dev_handle].ctrl_cid = 0;

    return (HID_SUCCESS);
}

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

tHID_STATUS HID_HostSetSecurityLevel (char *p_name, UINT8 sec_lvl)
{
    if (!BTM_SetSecurityLevel(FALSE, p_name, HID_SERVICE_ID, sec_lvl,
                              HID_PSM_CONTROL, BTM_SEC_PROTO_HID, 1)) {
        HIDH_TRACE_ERROR0("Security Registration 1 failed");
        return (HID_ERR_NO_RESOURCES);
    }

    if (!BTM_SetSecurityLevel(TRUE, p_name, HID_SERVICE_ID, sec_lvl,
                              HID_PSM_CONTROL, BTM_SEC_PROTO_HID, 1)) {
        HIDH_TRACE_ERROR0("Security Registration 2 failed");
        return (HID_ERR_NO_RESOURCES);
    }

    if (!BTM_SetSecurityLevel(FALSE, p_name, HID_SERVICE_ID + 1, 0,
                              HID_PSM_CONTROL, BTM_SEC_PROTO_HID, 2)) {
        HIDH_TRACE_ERROR0("Security Registration 3 failed");
        return (HID_ERR_NO_RESOURCES);
    }

    if (!BTM_SetSecurityLevel(TRUE, p_name, HID_SERVICE_ID + 1, 0,
                              HID_PSM_CONTROL, BTM_SEC_PROTO_HID, 2)) {
        HIDH_TRACE_ERROR0("Security Registration 4 failed");
        return (HID_ERR_NO_RESOURCES);
    }

    if (!BTM_SetSecurityLevel(TRUE, p_name, HID_SERVICE_ID + 2, 0,
                              HID_PSM_INTERRUPT, BTM_SEC_PROTO_HID, 0)) {
        HIDH_TRACE_ERROR0("Security Registration 5 failed");
        return (HID_ERR_NO_RESOURCES);
    }

    if (!BTM_SetSecurityLevel(FALSE, p_name, HID_SERVICE_ID + 2, 0,
                              HID_PSM_INTERRUPT, BTM_SEC_PROTO_HID, 0)) {
        HIDH_TRACE_ERROR0("Security Registration 6 failed\x00\x00\x00\x00\x00");
        return (HID_ERR_NO_RESOURCES);
    }

    return (HID_SUCCESS);
}
