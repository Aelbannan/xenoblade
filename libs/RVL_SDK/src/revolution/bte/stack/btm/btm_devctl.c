// Decompiled btm_devctl.c - high-level C reconstruction (Wii RVL SDK).
// Retail symbol btm_cb is a .bss object at 0x805BC2F8 (size 0x27C4).
// btm_int.h's tBTM_CB/tBTM_VERSION_INFO layouts do not match the Wii
// binary, so we declare a local surrogate covering only the fields we
// touch (verified against build/us/asm/.../btm_devctl.s offsets).

#include "revolution/BTE/stack/include/bt_types.h"

/* ------------------------------------------------------------------ */
/*  Return codes / constants (from btm_api.h)                          */
/* ------------------------------------------------------------------ */
typedef UINT8 tBTM_STATUS;

#define BTM_SUCCESS   0
#define BTM_BUSY      2
#define BTM_DEV_RESET 12

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

#define BTM_CB_EVT_WRITE_STORED_LINK_KEYS   3
#define BTM_CB_EVT_DELETE_STORED_LINK_KEYS  4

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
    UINT8               _pad1[0x630 - 0x574];      /* 0x0574-0x062F */
    BD_ADDR             local_addr;                /* 0x0630 */
    BtmLocalVersionInfo local_version;             /* 0x0636 */
    BD_FEATURES         local_features;            /* 0x0640 */
    DEV_CLASS           dev_class;                 /* 0x0648 */
    UINT8               _pad2[0x64e - 0x64b];      /* 0x064B-0x064D */
    UINT8               state;                     /* 0x064E */
} BtmDevctlCb;

/* The real global */
extern BtmDevctlCb btm_cb;

void btm_dev_init() {}

void btm_db_reset() {}

void BTM_DeviceReset() {}

void BTM_SendHciReset() {}

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

void btm_read_local_name_complete() {}

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

void BTM_SetDeviceClass() {}

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

void btm_vsc_complete() {}

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

void btm_vendor_specific_evt() {}

void BTM_WritePageTimeout() {}

void BTM_ReadStoredLinkKey() {}

void BTM_WriteStoredLinkKey() {}

void BTM_DeleteStoredLinkKey() {}

void btm_read_stored_link_key_complete() {}

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

void btm_return_link_keys_evt() {}

/* ------------------------------------------------------------------ */
/*  btm_report_device_status - notify application of a device status  */
/*  change (up/down/command timeout).                                 */
/* ------------------------------------------------------------------ */
void btm_report_device_status(tBTM_DEV_STATUS status)
{
    if (btm_cb.p_dev_status_cb != NULL)
        btm_cb.p_dev_status_cb(status);
}
