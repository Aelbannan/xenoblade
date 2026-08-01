// Decompiled btm_devctl.c - high-level C reconstruction (Wii RVL SDK).
// Retail symbol btm_cb is a .bss object at 0x805BC2F8 (size 0x27C4).
// btm_int.h's tBTM_CB/tBTM_VERSION_INFO layouts do not match the Wii
// binary, so we declare a local surrogate covering only the fields we
// touch (verified against build/us/asm/.../btm_devctl.s offsets).

#include <string.h>
#include "revolution/BTE/stack/include/bt_types.h"

/* ------------------------------------------------------------------ */
/*  External SDK functions used by this unit.                          */
/* ------------------------------------------------------------------ */
extern void btu_start_timer(void *p_tle, UINT16 type, UINT32 timeout);
extern void btu_stop_timer(void *p_tle);
extern void *GKI_getpoolbuf(UINT8 pool_id);
extern void LogMsg_0(UINT32 trace_set_mask, const char *p_str);
extern void LogMsg_1(UINT32 trace_set_mask, const char *fmt_str, UINT32 p1);
extern int btsnd_hcic_reset(void);
extern void btsnd_hcic_write_dev_class(void *p, UINT8 *dev_class);
extern void btsnd_hcic_write_page_tout(void *p, UINT16 timeout);
extern void btm_acl_device_down(void);
extern void btm_db_reset(void);
extern void btm_inq_db_reset(void);
extern void btm_discovery_db_reset(void);

/* Default device class literal (retail bss symbol, sda21-accessed) */
extern UINT8 lbl_80665928[DEV_CLASS_LEN];

/* Trace level thresholds (from bt_trace.h) */
#define BT_TRACE_LEVEL_EVENT 4

/* ------------------------------------------------------------------ */
/*  Return codes / constants (from btm_api.h)                          */
/* ------------------------------------------------------------------ */
typedef UINT8 tBTM_STATUS;

#define BTM_SUCCESS      0
#define BTM_BUSY         2
#define BTM_NO_RESOURCES 3
#define BTM_DEV_RESET    12
#define BTM_DEV_STATE_WAIT_RESET  0
#define BTM_DEV_RESET_RETRY_NUM   5

/* ------------------------------------------------------------------ */
/*  Callback types (from btm_api.h)                                    */
/* ------------------------------------------------------------------ */
typedef UINT8 tBTM_DEV_STATUS;

typedef void (tBTM_DEV_STATUS_CB)(tBTM_DEV_STATUS status);
typedef void (tBTM_VS_EVT_CB)(UINT8 len, UINT8 *p);
typedef void (tBTM_CMPL_CB)(void *p1);

/* Stored link key result structures (from btm_api.h) */
typedef struct
{
    UINT8   event;
    UINT8   status;
    UINT8   num_keys;
} tBTM_WRITE_STORED_LINK_KEY_COMPLETE;

typedef struct
{
    UINT8   event;
    UINT8   status;
    UINT16  num_keys;
} tBTM_DELETE_STORED_LINK_KEY_COMPLETE;

/* Read stored link keys result (from btm_api.h) */
typedef struct
{
    UINT8   event;
    UINT8   status;
    UINT16  max_keys;
    UINT16  num_keys;
} tBTM_READ_STORED_LINK_KEY_COMPLETE;

/* Return link keys event (from btm_api.h) */
typedef struct
{
    UINT8   event;
    UINT8   num_keys;
} tBTM_RETURN_LINK_KEYS_EVT;

/* Vendor specific command complete event (local Wii layout) */
#define BTM_MAX_VSC_ARG_BYTES 0x10C

typedef struct
{
    UINT16  opcode;
    UINT16  param_len;
    UINT8   p_param_buf[BTM_MAX_VSC_ARG_BYTES];
} tBTM_VSC_CMPL;

typedef void (tBTM_VSC_CMPL_CB)(tBTM_VSC_CMPL *p);

#define BTM_CB_EVT_READ_STORED_LINK_KEYS   2
#define BTM_CB_EVT_WRITE_STORED_LINK_KEYS  3
#define BTM_CB_EVT_DELETE_STORED_LINK_KEYS 4

#define BTM_VSC_CMD_DONE 0

/* One returned link key record = BD_ADDR (6) + LINK_KEY (16) */
#define BTM_LINK_KEY_PAIR_LEN 22

/* ------------------------------------------------------------------ */
/*  Local version info - Wii layout is five UINT16s (unlike the       */
/*  UINT8/UINT16 packed tBTM_VERSION_INFO in btm_api.h).              */
/* ------------------------------------------------------------------ */
typedef struct
{
    UINT16  hci_version;
    UINT16  lmp_version;
    UINT16  manufacturer;
    UINT16  hci_sub_version;
    UINT16  lmp_sub_version;
} BtmLocalVersionInfo;

/* ------------------------------------------------------------------ */
/*  Local btm_cb surrogate - only the fields we touch.                */
/*  The real symbol is declared elsewhere; we supply our own layout.   */
/* ------------------------------------------------------------------ */
typedef struct
{
    UINT8               _pad0[0x568];              /* 0x0000-0x0567 */
    tBTM_DEV_STATUS_CB *p_dev_status_cb;           /* 0x0568 */
    tBTM_VS_EVT_CB     *p_vend_spec_cb;            /* 0x056C */
    tBTM_CMPL_CB       *p_stored_link_key_cmpl_cb; /* 0x0570 */
    UINT8               timer[0x10];               /* 0x0574-0x0583 dev reset timer */
    UINT32              reserved_584;              /* 0x0584 (init to 1) */
    UINT32              reserved_588;              /* 0x0588 */
    void               *p_reset_cmpl_cb;           /* 0x058C */
    UINT8               local_name_timer[0x10];    /* 0x0590-0x059F */
    UINT32              reserved_5A0;              /* 0x05A0 (init to 2) */
    UINT32              reserved_5A4;              /* 0x05A4 */
    tBTM_CMPL_CB       *p_local_name_cmpl_cb;      /* 0x05A8 */
    UINT32              reserved_5AC;              /* 0x05AC */
    UINT32              reserved_5B0;              /* 0x05B0 */
    UINT32              reserved_5B4;              /* 0x05B4 */
    UINT32              reserved_5B8;              /* 0x05B8 */
    UINT32              reserved_5BC;              /* 0x05BC (init to 4) */
    UINT8               _pad1[0x5C4 - 0x5C0];      /* 0x05C0-0x05C3 */
    tBTM_CMPL_CB       *reserved_5C4;              /* 0x05C4 */
    UINT8               _pad1b[0x5E0 - 0x5C8];     /* 0x05C8-0x05DF */
    tBTM_CMPL_CB       *reserved_5E0;              /* 0x05E0 */
    UINT8               _pad1c[0x61C - 0x5E4];     /* 0x05E4-0x061B */
    tBTM_VSC_CMPL_CB   *p_vsc_callback;            /* 0x061C */
    void               *p_send_hci_reset_cmpl_cb;  /* 0x0620 */
    UINT8               _pad2[0x630 - 0x624];      /* 0x0624-0x062F */
    BD_ADDR             local_addr;                /* 0x0630 */
    BtmLocalVersionInfo local_version;             /* 0x0636 */
    BD_FEATURES         local_features;            /* 0x0640 */
    DEV_CLASS           dev_class;                 /* 0x0648 */
    UINT8               _pad3;                     /* 0x064B */
    UINT16              page_timeout;              /* 0x064C */
    UINT8               state;                     /* 0x064E */
    UINT8               rst_retry;                 /* 0x064F */
    UINT8               rsp_pending;               /* 0x0650 */
    UINT8               _pad4[0x654 - 0x651];      /* 0x0651-0x0653 */
    UINT16              page_scan_interval;        /* 0x0654 */
    UINT16              page_scan_window;          /* 0x0656 */
    UINT8               _pad5[0x27BD - 0x658];     /* 0x0658-0x27BC */
    UINT8               afh_first;                 /* 0x27BD */
    UINT8               afh_last;                  /* 0x27BE */
    UINT8               _pad6[0x27C0 - 0x27BF];    /* 0x27BF */
    UINT8               trace_level;               /* 0x27C0 */
} BtmDevctlCb;

/* The real global */
extern BtmDevctlCb btm_cb;

/* ------------------------------------------------------------------ */
/*  btm_dev_init - initialize the device control block and issue the  */
/*  initial HCI reset.                                                */
/* ------------------------------------------------------------------ */
void btm_dev_init(void)
{
    /* set default device class */
    memcpy(btm_cb.dev_class, lbl_80665928, DEV_CLASS_LEN);

    btm_cb.page_timeout = 0x1400;
    btm_cb.reserved_584 = 1;
    btm_cb.reserved_5A0 = 2;
    btm_cb.reserved_5BC = 4;
    btm_cb.page_scan_interval = 0xCC18;
    btm_cb.page_scan_window = 0x3F;
    btm_cb.afh_first = 0xFF;
    btm_cb.afh_last = 0xFF;

    /* wait for the HCI reset; retry up to 5 times */
    btm_cb.state = BTM_DEV_STATE_WAIT_RESET;
    btm_cb.rst_retry = BTM_DEV_RESET_RETRY_NUM;
    btu_start_timer(&btm_cb.timer, 1, 4);
    btsnd_hcic_reset();
}

/* ------------------------------------------------------------------ */
/*  btm_db_reset - clear the inquiry/discovery databases and cancel   */
/*  any pending device callbacks.                                     */
/* ------------------------------------------------------------------ */
void btm_db_reset(void)
{
    UINT8 evt = 0xc;
    tBTM_CMPL_CB *p_cb;

    btm_inq_db_reset();
    btm_discovery_db_reset();

    if ((p_cb = btm_cb.p_local_name_cmpl_cb) != NULL)
    {
        btm_cb.p_local_name_cmpl_cb = NULL;
        if (p_cb)
            (*p_cb)(NULL);
    }

    if ((p_cb = (tBTM_CMPL_CB *)btm_cb.reserved_5C4) != NULL)
    {
        btm_cb.reserved_5C4 = NULL;
        if (p_cb)
            (*p_cb)(&evt);
    }

    if ((p_cb = (tBTM_CMPL_CB *)btm_cb.reserved_5E0) != NULL)
    {
        btm_cb.reserved_5E0 = NULL;
        if (p_cb)
            (*p_cb)(&evt);
    }
}

/* ------------------------------------------------------------------ */
/*  BTM_DeviceReset - reset the HCI device and notify the app.        */
/* ------------------------------------------------------------------ */
void BTM_DeviceReset(tBTM_CMPL_CB *p_cb)
{
    /* Check if the device is busy */
    if (btm_cb.p_reset_cmpl_cb != NULL && btm_cb.p_reset_cmpl_cb != p_cb)
        return;

    btm_acl_device_down();
    btm_db_reset();

    btm_cb.p_reset_cmpl_cb = p_cb;
    btm_cb.state = BTM_DEV_STATE_WAIT_RESET;
    btm_cb.rst_retry = BTM_DEV_RESET_RETRY_NUM;
    btu_start_timer(&btm_cb.timer, 1, 4);
    btsnd_hcic_reset();

    /* Notify the application that the device is going down */
    if (btm_cb.p_dev_status_cb != NULL)
        (*btm_cb.p_dev_status_cb)(1);
}

/* ------------------------------------------------------------------ */
/*  BTM_SendHciReset - send an HCI reset command to the controller.   */
/* ------------------------------------------------------------------ */
void BTM_SendHciReset(tBTM_CMPL_CB *p_cb)
{
    /* Check if the device is busy */
    if (btm_cb.p_reset_cmpl_cb != NULL && btm_cb.p_reset_cmpl_cb != p_cb
        && btm_cb.p_send_hci_reset_cmpl_cb != p_cb)
        return;

    btm_acl_device_down();
    btm_db_reset();

    btm_cb.p_send_hci_reset_cmpl_cb = p_cb;
    btm_cb.state = BTM_DEV_STATE_WAIT_RESET;
    btm_cb.rst_retry = BTM_DEV_RESET_RETRY_NUM;
    btu_start_timer(&btm_cb.timer, 1, 4);
    btsnd_hcic_reset();

    /* Notify the application that the device is going down */
    if (btm_cb.p_dev_status_cb != NULL)
        (*btm_cb.p_dev_status_cb)(1);
}

int BTM_IsDeviceUp(void) {
    return (btm_cb.state == 5) ? 1 : 0;
}

void BTM_SetAfhChannels() {}

void btm_dev_timeout() {}

void btm_reset_complete() {}

void btm_read_hci_buf_size_complete() {}

void btm_read_local_version_complete() {}

void btm_read_local_features_complete() {}

void BTM_SetLocalDeviceName() {}

/* ------------------------------------------------------------------ */
/*  btm_read_local_name_complete - HCI read local name complete.      */
/* ------------------------------------------------------------------ */
void btm_read_local_name_complete(UINT8 *p)
{
    tBTM_CMPL_CB *p_cb = btm_cb.p_local_name_cmpl_cb;

    btu_stop_timer(&btm_cb.local_name_timer);
    btm_cb.p_local_name_cmpl_cb = NULL;

    if (p_cb)
    {
        if (p[0] == 0)   /* HCI_SUCCESS */
            (*p_cb)(p + 1);
        else
            (*p_cb)(NULL);
    }
}

/* ------------------------------------------------------------------ */
/*  BTM_ReadLocalDeviceAddr - read the cached local BD address.       */
/* ------------------------------------------------------------------ */
tBTM_STATUS BTM_ReadLocalDeviceAddr(tBTM_CMPL_CB *p_cb)
{
    if (p_cb != NULL)
        (*p_cb)(btm_cb.local_addr);

    return BTM_SUCCESS;
}

/* ------------------------------------------------------------------ */
/*  btm_read_local_addr_complete - HCI read-local-address complete.   */
/*  The address arrives big-endian; store it reversed (little-endian  */
/*  order used by the stack).                                         */
/* ------------------------------------------------------------------ */
void btm_read_local_addr_complete(UINT8 *p)
{
    if (p[0] == 0)
    {
        btm_cb.local_addr[5] = p[1];
        btm_cb.local_addr[4] = p[2];
        btm_cb.local_addr[3] = p[3];
        btm_cb.local_addr[2] = p[4];
        btm_cb.local_addr[1] = p[5];
        btm_cb.local_addr[0] = p[6];
    }
}

/* ------------------------------------------------------------------ */
/*  BTM_ReadLocalVersion - return cached local version information.   */
/* ------------------------------------------------------------------ */
tBTM_STATUS BTM_ReadLocalVersion(BtmLocalVersionInfo *p_vers)
{
    /* Check if the device is ready */
    if (btm_cb.state < 4)
        return BTM_DEV_RESET;

    *p_vers = btm_cb.local_version;

    return BTM_SUCCESS;
}

/* ------------------------------------------------------------------ */
/*  BTM_SetDeviceClass - set the local device class and write it to   */
/*  the controller.                                                   */
/* ------------------------------------------------------------------ */
tBTM_STATUS BTM_SetDeviceClass(DEV_CLASS dev_class)
{
    UINT8 *p;

    memcpy(btm_cb.dev_class, dev_class, DEV_CLASS_LEN);

    if (btm_cb.state <= 1)
        return BTM_DEV_RESET;

    p = (UINT8 *)GKI_getpoolbuf(2);
    if (p != NULL)
    {
        btsnd_hcic_write_dev_class(p, dev_class);
        return BTM_SUCCESS;
    }

    return BTM_NO_RESOURCES;
}

/* ------------------------------------------------------------------ */
/*  BTM_ReadDeviceClass - return pointer to cached device class.      */
/* ------------------------------------------------------------------ */
UINT8 *BTM_ReadDeviceClass(void)
{
    return btm_cb.dev_class;
}

/* ------------------------------------------------------------------ */
/*  BTM_ReadLocalFeatures - return pointer to cached LMP features.    */
/* ------------------------------------------------------------------ */
UINT8 *BTM_ReadLocalFeatures(void)
{
    return btm_cb.local_features;
}

/* ------------------------------------------------------------------ */
/*  BTM_RegisterForDeviceStatusNotif - replace status callback;       */
/*  returns the previously registered callback.                       */
/* ------------------------------------------------------------------ */
tBTM_DEV_STATUS_CB *BTM_RegisterForDeviceStatusNotif(tBTM_DEV_STATUS_CB *p_cb)
{
    tBTM_DEV_STATUS_CB *p_old = btm_cb.p_dev_status_cb;

    btm_cb.p_dev_status_cb = p_cb;

    return p_old;
}

void BTM_VendorSpecificCommand() {}

/* ------------------------------------------------------------------ */
/*  btm_vsc_complete - HCI vendor specific command complete event.    */
/*  Package the result and invoke the registered callback.            */
/* ------------------------------------------------------------------ */
void btm_vsc_complete(UINT8 *p, UINT16 cc_opcode, UINT16 evt_len)
{
    tBTM_VSC_CMPL_CB *p_cb = btm_cb.p_vsc_callback;
    tBTM_VSC_CMPL evt_data;
    UINT8 *p_dst = evt_data.p_param_buf;

    btm_cb.p_vsc_callback = NULL;
    btm_cb.rsp_pending = BTM_VSC_CMD_DONE;

    if (p_cb)
    {
        evt_data.opcode = cc_opcode;
        evt_data.param_len = evt_len;
        memcpy(p_dst, p, evt_len);
        (*p_cb)(&evt_data);
    }
}

/* ------------------------------------------------------------------ */
/*  BTM_RegisterForVSEvents - register a vendor specific event        */
/*  callback (only one allowed at a time).                            */
/* ------------------------------------------------------------------ */
tBTM_STATUS BTM_RegisterForVSEvents(tBTM_VS_EVT_CB *p_cb)
{
    if (btm_cb.p_vend_spec_cb != NULL && p_cb != NULL)
        return BTM_BUSY;

    btm_cb.p_vend_spec_cb = p_cb;

    return BTM_SUCCESS;
}

/* ------------------------------------------------------------------ */
/*  btm_vendor_specific_evt - HCI vendor specific event received.     */
/*  Forward it to the application's vendor specific event callback.   */
/* ------------------------------------------------------------------ */
void btm_vendor_specific_evt(UINT8 *p, UINT8 evt_len)
{
    BtmDevctlCb *cb = &btm_cb;
    tBTM_VS_EVT_CB *p_cb;

    if (cb->local_version.hci_sub_version != 0x12
        && cb->trace_level >= BT_TRACE_LEVEL_EVENT)
        LogMsg_0(0xD0003, "BTM Event: Received a vendor specific event from controller");

    p_cb = cb->p_vend_spec_cb;
    cb->p_vsc_callback = NULL;
    cb->rsp_pending = BTM_VSC_CMD_DONE;

    if (p_cb)
        (*p_cb)(evt_len, p);
}

/* ------------------------------------------------------------------ */
/*  BTM_WritePageTimeout - set the page timeout in the controller.    */
/*  The timeout is converted from milliseconds to slot units.         */
/* ------------------------------------------------------------------ */
tBTM_STATUS BTM_WritePageTimeout(UINT16 timeout)
{
    UINT8 *p;

    if (btm_cb.trace_level >= BT_TRACE_LEVEL_EVENT)
        LogMsg_1(0xD0003, "BTM: BTM_WritePageTimeout: Timeout: %d.", timeout);

    btm_cb.page_timeout = (UINT16)((timeout * 625) / 1000);

    p = (UINT8 *)GKI_getpoolbuf(2);
    if (p != NULL)
    {
        btsnd_hcic_write_page_tout(p, timeout);
        return BTM_SUCCESS;
    }

    return BTM_NO_RESOURCES;
}

void BTM_ReadStoredLinkKey() {}

void BTM_WriteStoredLinkKey() {}

void BTM_DeleteStoredLinkKey() {}

/* ------------------------------------------------------------------ */
/*  btm_read_stored_link_key_complete - HCI read stored link key      */
/*  complete event.                                                   */
/* ------------------------------------------------------------------ */
void btm_read_stored_link_key_complete(UINT8 *p)
{
    tBTM_CMPL_CB *p_cb = btm_cb.p_stored_link_key_cmpl_cb;
    tBTM_READ_STORED_LINK_KEY_COMPLETE evt;

    btm_cb.p_stored_link_key_cmpl_cb = NULL;

    if (p_cb)
    {
        evt.event    = BTM_CB_EVT_READ_STORED_LINK_KEYS;
        evt.status   = p[0];
        evt.max_keys = 0 + ((UINT16)(*(p + 1)) << 0) + ((UINT16)(*(p + 2)) << 8);
        evt.num_keys = 0 + ((UINT16)(*(p + 3)) << 0) + ((UINT16)(*(p + 4)) << 8);
        (*p_cb)(&evt);
    }
}

/* ------------------------------------------------------------------ */
/*  btm_write_stored_link_key_complete - invoke the stored link key   */
/*  callback with the HCI write-stored-link-key result.               */
/* ------------------------------------------------------------------ */
void btm_write_stored_link_key_complete(UINT8 *p)
{
    tBTM_CMPL_CB *p_cb = btm_cb.p_stored_link_key_cmpl_cb;
    tBTM_WRITE_STORED_LINK_KEY_COMPLETE evt;

    btm_cb.p_stored_link_key_cmpl_cb = NULL;

    if (p_cb != NULL)
    {
        evt.event    = BTM_CB_EVT_WRITE_STORED_LINK_KEYS;
        evt.status   = p[0];
        evt.num_keys = p[1];
        (*p_cb)(&evt);
    }
}

/* ------------------------------------------------------------------ */
/*  btm_delete_stored_link_key_complete - invoke the stored link key  */
/*  callback with the HCI delete-stored-link-key result.              */
/* ------------------------------------------------------------------ */
void btm_delete_stored_link_key_complete(UINT8 *p)
{
    tBTM_CMPL_CB *p_cb = btm_cb.p_stored_link_key_cmpl_cb;
    tBTM_DELETE_STORED_LINK_KEY_COMPLETE evt;

    btm_cb.p_stored_link_key_cmpl_cb = NULL;

    if (p_cb != NULL)
    {
        evt.event    = BTM_CB_EVT_DELETE_STORED_LINK_KEYS;
        evt.status   = p[0];
        evt.num_keys = 0 + ((UINT16)(*(p + 1)) << 0) + ((UINT16)(*(p + 2)) << 8);
        (*p_cb)(&evt);
    }
}

/* ------------------------------------------------------------------ */
/*  btm_return_link_keys_evt - HCI return link keys event.  Each      */
/*  BD_ADDR + link key pair is converted from big to little endian.   */
/* ------------------------------------------------------------------ */
void btm_return_link_keys_evt(tBTM_RETURN_LINK_KEYS_EVT *result)
{
    tBTM_CMPL_CB *p_cb = btm_cb.p_stored_link_key_cmpl_cb;
    UINT8 i, *p, *p1;
    UINT8 bd_addr[BD_ADDR_LEN];
    UINT8 link_key[LINK_KEY_LEN];

    if (p_cb)
    {
        /* Change the BD addr and Link key in to big endian order */
        p = (UINT8 *)(result + 1);
        for (i = 0; i < result->num_keys; i++)
        {
            /* Initialize the backup pointer */
            p1 = p;

            /* Extract the BD Addr and Link Key */
            REVERSE_STREAM_TO_ARRAY(bd_addr, p1, BD_ADDR_LEN);
            REVERSE_STREAM_TO_ARRAY(link_key, p1, LINK_KEY_LEN);

            /* Write the BD Addr and Link Key back in big endian format */
            ARRAY_TO_STREAM(p, bd_addr, BD_ADDR_LEN);
            ARRAY_TO_STREAM(p, link_key, LINK_KEY_LEN);
        }
        (*p_cb)(result);
    }
}

/* ------------------------------------------------------------------ */
/*  btm_report_device_status - notify application of a device status  */
/*  change (up/down/command timeout).                                 */
/* ------------------------------------------------------------------ */
void btm_report_device_status(tBTM_DEV_STATUS status)
{
    if (btm_cb.p_dev_status_cb != NULL)
        btm_cb.p_dev_status_cb(status);
}
