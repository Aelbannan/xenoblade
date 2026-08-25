// Decompiled btm_devctl.c - high-level C reconstruction (Wii RVL SDK).
// Retail symbol btm_cb is a .bss object at 0x805BC2F8 (size 0x27C4).
// btm_int.h's tBTM_CB/tBTM_VERSION_INFO layouts do not match the Wii
// binary, so we declare a local surrogate covering only the fields we
// touch (verified against build/us/asm/.../btm_devctl.s offsets).

#include <string.h>
#include <decomp.h>
#include "revolution/BTE/stack/include/bt_types.h"
#include "revolution/BTE/stack/include/hcidefs.h"

/* LogMsg_2 / LogMsg_4 (varargs beyond p1) */
extern void LogMsg_2(UINT32 trace_set_mask, const char *fmt_str, UINT32 p1, UINT32 p2);
extern void LogMsg_4(UINT32 trace_set_mask, const char *fmt_str, UINT32 p1, UINT32 p2,
                     UINT32 p3, UINT32 p4);

/* ------------------------------------------------------------------ */
/*  External SDK functions used by this unit.                          */
/* ------------------------------------------------------------------ */
extern void btu_start_timer(void *p_tle, UINT16 type, UINT32 timeout);
extern void btu_stop_timer(void *p_tle);
extern void *GKI_getpoolbuf(UINT8 pool_id);
extern void LogMsg_0(UINT32 trace_set_mask, const char *p_str);
extern void LogMsg_1(UINT32 trace_set_mask, const char *fmt_str, const char *p1);
extern int btsnd_hcic_reset(void);
extern void btsnd_hcic_write_dev_class(void *p, UINT8 *dev_class);
extern void btsnd_hcic_write_page_tout(void *p, UINT16 timeout);
extern void btm_acl_device_down(void);
extern void btm_db_reset(void);
extern void btm_inq_db_reset(void);
extern void btm_discovery_db_reset(void);
extern void btm_pm_reset(void);
extern void l2cu_device_reset(void);
extern void l2c_link_processs_num_bufs(UINT16 num_lm_acl_bufs);
extern void BTM_SetPinType(UINT8 pin_type, UINT8 *pin_code, UINT8 pin_code_len);

extern void *GKI_getbuf(UINT16 size);

/* HCI command senders (hcicmds.c) */
extern UINT8 btsnd_hcic_set_afh_channels(UINT8 first, UINT8 last);
extern void btsnd_hcic_read_buffer_size(void *p_buf);
extern void btsnd_hcic_set_host_buf_size(UINT16 acl_buf_size, UINT16 sco_buf_size,
                                         UINT16 acl_pkt_count, UINT16 sco_pkt_count);
extern int  btsnd_hcic_read_local_ver(void);
extern int  btsnd_hcic_read_local_features(void);
extern int  btsnd_hcic_read_bd_addr(void);
extern void btsnd_hcic_change_name(void *p, UINT8 *p_name);
extern void btsnd_hcic_vendor_spec_cmd(void *p, UINT16 opcode, UINT8 param_len,
                                       UINT8 *p_param_buf);
extern void btm_sec_dev_reset(void);
extern UINT8 BTM_SetInquiryMode(UINT8 mode);
extern UINT8 BTM_SetPageScanType(UINT16 scan_type);
extern UINT8 BTM_SetInquiryScanType(UINT16 scan_type);
extern void btsnd_hcic_read_stored_key(void *p, UINT8 *bd_addr, UINT8 read_all);
extern void btsnd_hcic_write_stored_key(void *p, UINT8 num_keys, UINT8 *bd_addr,
                                        UINT8 *link_key);
extern UINT8 btsnd_hcic_delete_stored_key(UINT8 *bd_addr, UINT8 delete_all_flag);

/* "TRUE"/"FALSE" strings -- defined here (retail btm_devctl.o .sdata
   slice 0x80663000..0x80663018: dev-class bytes, "TRUE", "FALSE").
   Non-const so MWCC places them in .sdata (sda21-accessed), matching the
   retail lbl_8066592C/34 relocs. */
UINT8 lbl_80665928[DEV_CLASS_LEN] = {0x00, 0x1F, 0x00};
char lbl_8066592C[5] = "TRUE";   /* "TRUE"  */
/* Retail .sdata slice is 0x18 (0x80663000..0x80663018): the 3 objects with
   "FALSE" occupying 0xC..0x18 (6-byte string + 6 pad zeros); raise the small-
   data threshold so the 12-byte object stays in .sdata like retail. */
#pragma sdata_threshold 12
char lbl_80665934[12] = "FALSE";  /* "FALSE" */
#pragma sdata_threshold 8

/* BTU control block (Wii layout: hcit_acl_data_size at 0x7C) */
typedef struct
{
    UINT8   _pad[0x7C];
    UINT16  hcit_acl_data_size;   /* 0x7C */
    UINT16  hcit_acl_pkt_size;    /* 0x7E */
} BtuCbWii;
extern BtuCbWii btu_cb;

/* Wii-local timer list entry (retail layout: UINT32 event at 0x10) */
typedef struct
{
    UINT8   _pad[0x10];
    UINT32  event;                /* 0x10 */
} BtmTimerListEnt;

/* Trace level thresholds (from bt_trace.h) */
#define BT_TRACE_LEVEL_API    3
#define BT_TRACE_LEVEL_EVENT  4
#define BT_TRACE_LEVEL_DEBUG  5

/* ------------------------------------------------------------------ */
/*  Return codes / constants (from btm_api.h)                          */
/* ------------------------------------------------------------------ */
typedef UINT8 tBTM_STATUS;

/* Same-TU API functions called (and inlined) by btm_reset_complete */
tBTM_STATUS BTM_SetDeviceClass(DEV_CLASS dev_class);
tBTM_STATUS BTM_SetLocalDeviceName(char *p_name);

#define BTM_SUCCESS       0
#define BTM_CMD_STARTED   1
#define BTM_BUSY          2
#define BTM_NO_RESOURCES  3
#define BTM_MODE_UNSUPPORTED 4
#define BTM_ILLEGAL_VALUE 5
#define BTM_DEV_RESET     12
#define BTM_DEV_STATE_WAIT_RESET      0
#define BTM_DEV_STATE_WAIT_AFTER_RESET 1
#define BTM_DEV_STATE_WAIT_BUF_SIZE    2
#define BTM_DEV_STATE_WAIT_LOCAL_VER   3
#define BTM_DEV_STATE_WAIT_FEATURES    4
#define BTM_DEV_STATE_READY            5
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
/*  Local version info - matches tBTM_VERSION_INFO in btm_api.h:      */
/*  UINT8 hci_version, UINT16 hci_revision, UINT8 lmp_version,        */
/*  UINT16 manufacturer, UINT16 lmp_subversion.                       */
/* ------------------------------------------------------------------ */
typedef struct
{
    UINT8   hci_version;      /* 0x0636 */
    UINT16  hci_revision;     /* 0x0638 */
    UINT8   lmp_version;      /* 0x063A */
    UINT16  manufacturer;     /* 0x063C */
    UINT16  lmp_subversion;   /* 0x063E */
} BtmLocalVersionInfo;

/* ------------------------------------------------------------------ */
/*  Local btm_cb surrogate - only the fields we touch.                */
/*  The real symbol is declared elsewhere; we supply our own layout.   */
/* ------------------------------------------------------------------ */
typedef struct
{
    UINT8               bd_name[0x20];             /* 0x0000-0x001F local device name (32 bytes) */
    UINT8               pin_type;                  /* 0x0020 */
    UINT8               pin_code_len;              /* 0x0021 */
    UINT8               pin_code[0x10];            /* 0x0022-0x0031 */
    UINT8               _pad0[0x4C4 - 0x32];       /* 0x0032-0x04C3 */
    UINT16              acl_pkt_types_supported;   /* 0x04C4 */
    UINT8               _pad0b[0x568 - 0x4C6];     /* 0x04C6-0x0567 */
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
    tBTM_CMPL_CB       *p_send_hci_reset_cmpl_cb;  /* 0x0620 */
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
    UINT16              default_page_scan_interval; /* 0x0654 */
    UINT16              default_page_scan_window;   /* 0x0656 */
    UINT8               _pad5[0x169C - 0x658];     /* 0x0658-0x169B */
    UINT16              page_scan_window;          /* 0x169C */
    UINT16              page_scan_period;          /* 0x169E */
    UINT16              inq_scan_window;           /* 0x16A0 */
    UINT16              inq_scan_period;           /* 0x16A2 */
    UINT16              inq_scan_type;             /* 0x16A4 */
    UINT16              page_scan_type;            /* 0x16A6 */
    UINT8               _pad5b[0x1908 - 0x16A8];   /* 0x16A8-0x1907 */
    UINT8               page_scan_window_flag;     /* 0x1908 */
    UINT8               _pad5c[0x27BD - 0x1909];   /* 0x1909-0x27BC */
    UINT8               afh_first;                 /* 0x27BD */
    UINT8               afh_last;                  /* 0x27BE */
    UINT8               _pad6[0x27C0 - 0x27BF];    /* 0x27BF */
    UINT8               trace_level;               /* 0x27C0 */
} BtmDevctlCb;

/* The real global */
extern BtmDevctlCb btm_cb;

/* ------------------------------------------------------------------ */
/*  Device control block overlay (tBTM_DEVCB grouping, base btm_cb +   */
/*  0x568).  Used only by btm_read_local_features_complete so the      */
/*  retail's `p_devcb = &btm_cb.devcb` address shape is reproduced.    */
/* ------------------------------------------------------------------ */
typedef struct
{
    tBTM_DEV_STATUS_CB *p_dev_status_cb;           /* +0x000 = 0x0568 */
    tBTM_VS_EVT_CB     *p_vend_spec_cb;            /* +0x004 = 0x056C */
    tBTM_CMPL_CB       *p_stored_link_key_cmpl_cb; /* +0x008 = 0x0570 */
    UINT8               timer[0x10];               /* +0x00C = 0x0574 */
    UINT32              reserved_01C;              /* +0x01C = 0x0584 */
    UINT32              reserved_020;              /* +0x020 = 0x0588 */
    void               *p_reset_cmpl_cb;           /* +0x024 = 0x058C */
    UINT8               local_name_timer[0x10];    /* +0x028 = 0x0590 */
    UINT32              reserved_038;              /* +0x038 = 0x05A0 */
    UINT32              reserved_03C;              /* +0x03C = 0x05A4 */
    tBTM_CMPL_CB       *p_local_name_cmpl_cb;      /* +0x040 = 0x05A8 */
    UINT8               _pad[0xC8 - 0x44];         /* +0x044-0x0C7 */
    BD_ADDR             local_addr;                /* +0x0C8 = 0x0630 */
    BtmLocalVersionInfo local_version;             /* +0x0CE = 0x0636 */
    BD_FEATURES         local_features;            /* +0x0D8 = 0x0640 */
    DEV_CLASS           dev_class;                 /* +0x0E0 = 0x0648 */
    UINT8               _pad2;                     /* +0x0E3 = 0x064B */
    UINT16              page_timeout;              /* +0x0E4 = 0x064C */
    UINT8               state;                     /* +0x0E6 = 0x064E */
    UINT8               rst_retry;                 /* +0x0E7 = 0x064F */
    UINT8               rsp_pending;               /* +0x0E8 = 0x0650 */
    UINT8               _pad3[0xEC - 0xE9];        /* +0x0E9-0x0EB */
    UINT16              default_page_scan_interval;/* +0x0EC = 0x0654 */
    UINT16              default_page_scan_window;  /* +0x0EE = 0x0656 */
} BtmDevcbWii;

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
    btm_cb.default_page_scan_interval = 0xCC18;
    btm_cb.default_page_scan_window = 0x3F;
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

UINT8 BTM_IsDeviceUp(void) {
    return (btm_cb.state == 5);
}

/* ------------------------------------------------------------------ */
/*  BTM_SetAfhChannels - set the AFH channel map (disable channels).  */
/* ------------------------------------------------------------------ */
tBTM_STATUS BTM_SetAfhChannels(UINT8 first, UINT8 last)
{
    if (btm_cb.trace_level >= BT_TRACE_LEVEL_API)
        LogMsg_4(0xD0002, "BTM_SetAfhChannels first: %d (%d) last: %d (%d)",
                 first, btm_cb.afh_first, last, btm_cb.afh_last);

    /* Make sure the local device supports the feature before sending */
    if ((!HCI_LMP_AFH_CAP_MASTR_SUPPORTED(btm_cb.local_features)) &&
        (!HCI_LMP_AFH_CLASS_SLAVE_SUPPORTED(btm_cb.local_features)) &&
        (!HCI_LMP_AFH_CLASS_MASTR_SUPPORTED(btm_cb.local_features)))
        return BTM_MODE_UNSUPPORTED;

    if ((btm_cb.afh_first != first) || (btm_cb.afh_last != last))
    {
        if (btsnd_hcic_set_afh_channels(first, last))
        {
            btm_cb.afh_first = first;
            btm_cb.afh_last = last;
        }
        else
            return BTM_NO_RESOURCES;
    }
    return BTM_SUCCESS;
}

/* ------------------------------------------------------------------ */
/*  btm_dev_timeout - timer callback for the device reset / local     */
/*  name timers.                                                      */
/* ------------------------------------------------------------------ */
void btm_dev_timeout(BtmTimerListEnt *p_tle)
{
    UINT8 *p_name;
    void  *p_buf;
    void  *p_buf2;

    /* Check if this is the device reset timer */
    if (p_tle->event == 1)   /* BTU_TTYPE_BTM_DEV_RST */
    {
        switch (btm_cb.state)
        {
        case BTM_DEV_STATE_WAIT_RESET:              /* 0 */
            btm_cb.state = BTM_DEV_STATE_WAIT_RESET;
            btm_cb.rst_retry = BTM_DEV_RESET_RETRY_NUM;
            btu_start_timer(&btm_cb.timer, 1, 4);
            btsnd_hcic_reset();
            break;

        case BTM_DEV_STATE_WAIT_AFTER_RESET:        /* 1 */
            btm_cb.state = BTM_DEV_STATE_WAIT_BUF_SIZE;
            btm_cb.rst_retry = BTM_DEV_RESET_RETRY_NUM - 1;
            if (btm_cb.rst_retry == 0)
            {
                /* no more retries: restart the reset */
                btm_cb.rst_retry = BTM_DEV_RESET_RETRY_NUM - 1;
                btm_cb.state = BTM_DEV_STATE_WAIT_FEATURES;
                btu_start_timer(&btm_cb.timer, 1, 4);
                btsnd_hcic_reset();
            }
            else
            {
                /* Continue the device init sequence */
                btu_start_timer(&btm_cb.timer, 1, 1);
                p_buf = GKI_getpoolbuf(2);
                if (p_buf != NULL)
                    btsnd_hcic_read_buffer_size(p_buf);
            }

            /* Send the device class (inlined BTM_SetDeviceClass) */
            memcpy(btm_cb.dev_class, btm_cb.dev_class, DEV_CLASS_LEN);
            if (btm_cb.state != BTM_DEV_STATE_WAIT_RESET &&
                btm_cb.state != BTM_DEV_STATE_WAIT_AFTER_RESET)
            {
                p_buf = GKI_getpoolbuf(2);
                if (p_buf != NULL)
                    btsnd_hcic_write_dev_class(p_buf, btm_cb.dev_class);
            }

            /* Send the local device name if set */
            p_name = btm_cb.bd_name;
            if (p_name != NULL)
            {
                if (btm_cb.state != BTM_DEV_STATE_WAIT_RESET &&
                    btm_cb.state != BTM_DEV_STATE_WAIT_AFTER_RESET)
                {
                    p_buf2 = GKI_getpoolbuf(2);
                    if (p_buf2 != NULL)
                    {
                        if (btm_cb.bd_name != p_name)
                        {
                            memset(p_name, 0, 0x20);
                            strncpy((char *)p_name, (const char *)p_name, 0x1F);
                        }
                        btsnd_hcic_change_name(p_buf2, p_name);
                    }
                }
            }

            /* Send the pin type */
            BTM_SetPinType(btm_cb.pin_type, btm_cb.pin_code, btm_cb.pin_code_len);
            break;

        case BTM_DEV_STATE_WAIT_BUF_SIZE:           /* 2 */
            btm_cb.rst_retry--;
            if (btm_cb.rst_retry == 0)
            {
                /* no more retries: restart the reset */
                btm_cb.state = BTM_DEV_STATE_WAIT_RESET;
                btm_cb.rst_retry = BTM_DEV_RESET_RETRY_NUM;
                btu_start_timer(&btm_cb.timer, 1, 4);
                btsnd_hcic_reset();
            }
            else
            {
                btu_start_timer(&btm_cb.timer, 1, 1);
                p_buf = GKI_getpoolbuf(2);
                if (p_buf != NULL)
                    btsnd_hcic_read_buffer_size(p_buf);
            }
            break;

        case BTM_DEV_STATE_WAIT_LOCAL_VER:          /* 3 */
            btm_cb.rst_retry--;
            if (btm_cb.rst_retry == 0)
            {
                /* no more retries: restart the reset */
                btm_cb.state = BTM_DEV_STATE_WAIT_RESET;
                btm_cb.rst_retry = BTM_DEV_RESET_RETRY_NUM;
                btu_start_timer(&btm_cb.timer, 1, 4);
                btsnd_hcic_reset();
            }
            else
            {
                btu_start_timer(&btm_cb.timer, 1, 1);
                btsnd_hcic_read_local_ver();
                btsnd_hcic_read_bd_addr();
                btm_pm_reset();
            }
            break;
        }
    }
    else if (p_tle->event == 2)   /* BTU_TTYPE_BTM_LOCAL_NAME */
    {
        tBTM_CMPL_CB *p_cb = btm_cb.p_local_name_cmpl_cb;

        btm_cb.p_local_name_cmpl_cb = NULL;
        if (p_cb != NULL)
            (*p_cb)(NULL);
    }
}

/* ------------------------------------------------------------------ */
/*  btm_reset_complete - HCI reset command complete event.            */
/*  Kick off the rest of the init sequence (buffer sizes, device      */
/*  class, local name, pin type).                                     */
/* ------------------------------------------------------------------ */
void btm_reset_complete(void)
{
    UINT8 *p_name;
    void  *p_buf;
    void  *p_buf2;

    if (btm_cb.trace_level >= BT_TRACE_LEVEL_EVENT)
        LogMsg_0(0xD0003, "btm_reset_complete");

    /* Handle the case where BTM_SendHciReset initiated the reset */
    if (btm_cb.p_send_hci_reset_cmpl_cb != NULL)
    {
        tBTM_CMPL_CB *p_cb = btm_cb.p_send_hci_reset_cmpl_cb;
        btm_cb.p_send_hci_reset_cmpl_cb = NULL;
        (*p_cb)(NULL);
        return;
    }

    /* If the reset was initiated by the stack itself, continue the
       device init sequence. */
    if (btm_cb.state == BTM_DEV_STATE_WAIT_RESET)
    {
        /* Tell L2CAP that all connections are gone */
        l2cu_device_reset();

        /* Restore the scan parameters */
        btm_cb.inq_scan_window  = HCI_DEF_INQUIRYSCAN_WINDOW;
        btm_cb.inq_scan_period  = 0x800;
        btm_cb.inq_scan_type    = 0;
        btm_cb.page_scan_window = HCI_DEF_PAGESCAN_WINDOW;
        btm_cb.page_scan_period = 0x800;
        btm_cb.page_scan_type   = 0;

        btm_cb.state = BTM_DEV_STATE_WAIT_BUF_SIZE;
        btm_cb.rst_retry = BTM_DEV_RESET_RETRY_NUM - 1;

        if (btm_cb.rst_retry == 0)
        {
            /* no more retries: restart the reset */
            btm_cb.state = BTM_DEV_STATE_WAIT_RESET;
            btm_cb.rst_retry = BTM_DEV_RESET_RETRY_NUM - 1;
            btu_start_timer(&btm_cb.timer, 1, 4);
            btsnd_hcic_reset();
        }
        else
        {
            /* Read the buffer size and continue init */
            btu_start_timer(&btm_cb.timer, 1, 1);
            p_buf = GKI_getpoolbuf(2);
            if (p_buf != NULL)
                btsnd_hcic_read_buffer_size(p_buf);
        }

        /* Send the device class (memcpy self-copy is the inlined
           BTM_SetDeviceClass(btm_cb.dev_class) first statement) */
        memcpy(btm_cb.dev_class, btm_cb.dev_class, DEV_CLASS_LEN);
        if (btm_cb.state != BTM_DEV_STATE_WAIT_RESET &&
            btm_cb.state != BTM_DEV_STATE_WAIT_AFTER_RESET)
        {
            p_buf = GKI_getpoolbuf(2);
            if (p_buf != NULL)
                btsnd_hcic_write_dev_class(p_buf, btm_cb.dev_class);
        }

        /* Send the local device name if set */
        p_name = btm_cb.bd_name;
        if (p_name != NULL)
        {
            if (btm_cb.state != BTM_DEV_STATE_WAIT_RESET &&
                btm_cb.state != BTM_DEV_STATE_WAIT_AFTER_RESET)
            {
                p_buf2 = GKI_getpoolbuf(2);
                if (p_buf2 != NULL)
                {
                    if (btm_cb.bd_name != p_name)
                    {
                        memset(p_name, 0, 0x20);
                        strncpy((char *)p_name, (const char *)p_name, 0x1F);
                    }
                    btsnd_hcic_change_name(p_buf2, p_name);
                }
            }
        }

        /* Send the pin type */
        BTM_SetPinType(btm_cb.pin_type, btm_cb.pin_code, btm_cb.pin_code_len);
    }
}

/* ------------------------------------------------------------------ */
/*  btm_read_hci_buf_size_complete - HCI read buffer size complete.   */
/* ------------------------------------------------------------------ */
void btm_read_hci_buf_size_complete(UINT8 *p, UINT16 evt_len)
{
    void   *p_buf;
    UINT16  lm_num_acl_bufs;

    if (p[0] == HCI_SUCCESS)
    {
        btu_cb.hcit_acl_data_size = 0 + ((UINT16)(*(p + 1)) << 0) + ((UINT16)(*(p + 2)) << 8);
        lm_num_acl_bufs = 0 + ((UINT16)(*(p + 4)) << 0) + ((UINT16)(*(p + 5)) << 8);
        btu_cb.hcit_acl_pkt_size = (UINT16)(btu_cb.hcit_acl_data_size + HCI_DATA_PREAMBLE_SIZE);
        l2c_link_processs_num_bufs(lm_num_acl_bufs);
        btsnd_hcic_set_host_buf_size(0x69B, 0x1E, 0x14, 0x0A);
    }

    btm_cb.state = BTM_DEV_STATE_WAIT_LOCAL_VER;
    btm_cb.rst_retry = BTM_DEV_RESET_RETRY_NUM - 1;

    if (btm_cb.rst_retry == 0)
    {
        /* no more retries: restart the reset */
        btm_cb.rst_retry = BTM_DEV_RESET_RETRY_NUM - 1;
        btm_cb.state = BTM_DEV_STATE_WAIT_FEATURES;
        btu_start_timer(&btm_cb.timer, 1, 4);
        btsnd_hcic_reset();
    }
    else
    {
        btu_start_timer(&btm_cb.timer, 1, 1);
        btsnd_hcic_read_local_ver();
        btsnd_hcic_read_bd_addr();
        btm_pm_reset();
    }
}

/* ------------------------------------------------------------------ */
/*  btm_read_local_version_complete - HCI read local version complete.*/
/* ------------------------------------------------------------------ */
void btm_read_local_version_complete(UINT8 *p, UINT16 evt_len)
{
    BtmDevctlCb *cb = &btm_cb;

    if (p[0] == HCI_SUCCESS)
    {
        cb->local_version.hci_version    = p[1];
        cb->local_version.hci_revision   = 0 + ((UINT16)(*(p + 2)) << 0) + ((UINT16)(*(p + 3)) << 8);
        cb->local_version.lmp_version    = p[4];
        cb->local_version.manufacturer   = 0 + ((UINT16)(*(p + 5)) << 0) + ((UINT16)(*(p + 6)) << 8);
        cb->local_version.lmp_subversion = 0 + ((UINT16)(*(p + 7)) << 0) + ((UINT16)(*(p + 8)) << 8);
    }

    btm_cb.state = BTM_DEV_STATE_WAIT_FEATURES;
    btm_cb.rst_retry = BTM_DEV_RESET_RETRY_NUM - 1;

    if (btm_cb.rst_retry == 0)
    {
        /* no more retries: restart the reset */
        btm_cb.rst_retry = BTM_DEV_RESET_RETRY_NUM - 1;
        btm_cb.state = BTM_DEV_STATE_WAIT_FEATURES;
        btu_start_timer(&btm_cb.timer, 1, 4);
        btsnd_hcic_reset();
    }
    else
    {
        btu_start_timer(&btm_cb.timer, 1, 1);
        btsnd_hcic_read_local_features();
    }
}

/* ------------------------------------------------------------------ */
/*  btm_read_local_features_complete - HCI read local features         */
/*  command complete event.  Save the feature set and finalise the    */
/*  device parameters (scan windows, packet types, AFH channels).     */
/* ------------------------------------------------------------------ */
void btm_read_local_features_complete(UINT8 *p)
{
    BtmDevcbWii *cb = (BtmDevcbWii *)((UINT8 *)&btm_cb + 0x568);
    UINT8 afh_last;
    tBTM_CMPL_CB *p_cb;
    UINT8 afh_first;

    p_cb = cb->p_reset_cmpl_cb;
    cb->p_reset_cmpl_cb = NULL;

    if (p[0] == HCI_SUCCESS)
    {
        /* The device is now ready */
        cb->state = BTM_DEV_STATE_READY;

        /* Save the local features */
        cb->local_features[0] = p[1];
        cb->local_features[1] = p[2];
        cb->local_features[2] = p[3];
        cb->local_features[3] = p[4];
        cb->local_features[4] = p[5];
        cb->local_features[5] = p[6];
        cb->local_features[6] = p[7];
        cb->local_features[7] = p[8];

        /* Update the page scan interval */
        cb->default_page_scan_interval = 0x18;
        if (cb->local_features[0] & 0x01)
            cb->default_page_scan_interval |= 0x0C00;
        if (cb->local_features[0] & 0x02)
            btm_cb.default_page_scan_interval |= 0xC000;

        if (btm_cb.local_version.hci_version >= 3)
        {
            if (!(cb->local_features[3] & 0x02))
                btm_cb.default_page_scan_interval |= 0x1102;
            if (!(cb->local_features[3] & 0x04))
                btm_cb.default_page_scan_interval |= 0x2204;
            if ((cb->local_features[3] & 0x02) ||
                (cb->local_features[3] & 0x04))
            {
                if (!(cb->local_features[4] & 0x80))
                    btm_cb.default_page_scan_interval |= 0x0300;
                if (!(cb->local_features[5] & 0x01))
                    btm_cb.default_page_scan_interval |= 0x3000;
            }
        }

        if (btm_cb.trace_level >= BT_TRACE_LEVEL_DEBUG)
            LogMsg_1(0xD0004, "Local supported ACL packet types: 0x%04x",
                     (const char *)btm_cb.default_page_scan_interval);

        /* Update the page scan window */
        btm_cb.default_page_scan_window = 0;
        btm_cb.page_scan_window_flag = 0;
        if (cb->local_features[1] & 0x08)
        {
            btm_cb.default_page_scan_window = 1;
            if (cb->local_features[1] & 0x10)
                btm_cb.default_page_scan_window |= 0x02;
            if (cb->local_features[1] & 0x20)
                btm_cb.default_page_scan_window |= 0x04;
        }
        if (cb->local_features[3] & 0x80)
            btm_cb.default_page_scan_window |= 0x08;
        if (cb->local_features[4] & 0x01)
            btm_cb.default_page_scan_window |= 0x10;
        if (cb->local_features[4] & 0x02)
            btm_cb.default_page_scan_window |= 0x20;

        if (btm_cb.default_page_scan_window & 0x38)
        {
            btm_cb.page_scan_window_flag = 1;
            if (cb->local_features[5] & 0x20)
            {
                if (!(cb->local_features[5] & 0x80))
                    btm_cb.default_page_scan_window |= 0x0100;
            }
            else
                btm_cb.default_page_scan_window |= 0x0140;
            if (cb->local_features[5] & 0x40)
            {
                if (!(cb->local_features[5] & 0x80))
                    btm_cb.default_page_scan_window |= 0x0200;
            }
            else
                btm_cb.default_page_scan_window |= 0x0280;
        }

        if (btm_cb.trace_level >= BT_TRACE_LEVEL_DEBUG)
            LogMsg_1(0xD0004, "Local supported SCO packet types: 0x%04x",
                     (const char *)btm_cb.default_page_scan_window);

        /* Supported ACL packet types */
        if (cb->local_features[0] & 0x20)
            btm_cb.acl_pkt_types_supported |= 0x0001;
        else
            btm_cb.acl_pkt_types_supported &= 0xFFFE;
        if (cb->local_features[0] & 0x40)
            btm_cb.acl_pkt_types_supported |= 0x0002;
        else
            btm_cb.acl_pkt_types_supported =
                (UINT16)DECOMP_PPC_RLWINM(btm_cb.acl_pkt_types_supported, 0, 31, 29);
        if (cb->local_features[0] & 0x80)
            btm_cb.acl_pkt_types_supported |= 0x0004;
        else
            btm_cb.acl_pkt_types_supported =
                (UINT16)DECOMP_PPC_RLWINM(btm_cb.acl_pkt_types_supported, 0, 30, 28);
        if (cb->local_features[1] & 0x01)
            btm_cb.acl_pkt_types_supported |= 0x0008;
        else
            btm_cb.acl_pkt_types_supported =
                (UINT16)DECOMP_PPC_RLWINM(btm_cb.acl_pkt_types_supported, 0, 29, 27);

        btm_sec_dev_reset();

        /* Restore the AFH channels if they were changed */
        afh_last = btm_cb.afh_last;
        if (afh_last != 0xFF)
        {
            afh_first = btm_cb.afh_first;
            btm_cb.afh_last = 0xFF;
            btm_cb.afh_first = 0xFF;
            BTM_SetAfhChannels(afh_first, afh_last);
        }

        if (cb->local_features[3] & 0x40)
            BTM_SetInquiryMode(1);
        BTM_SetPageScanType(1);
        BTM_SetInquiryScanType(1);

        /* Notify the application that the device is ready */
        if (btm_cb.p_dev_status_cb != NULL)
            (*btm_cb.p_dev_status_cb)(0);
        if (p_cb != NULL)
            (*p_cb)(NULL);
    }
}

/* ------------------------------------------------------------------ */
/*  BTM_SetLocalDeviceName - store the local device name and send it  */
/*  to the controller.                                                */
/* ------------------------------------------------------------------ */
tBTM_STATUS BTM_SetLocalDeviceName(char *p_name)
{
    void   *p_buf;
    UINT8  *p_bd_name;

    if (!p_name)
        return BTM_ILLEGAL_VALUE;

    if (btm_cb.state <= BTM_DEV_STATE_WAIT_AFTER_RESET)
        return BTM_DEV_RESET;

    p_bd_name = btm_cb.bd_name;

    p_buf = GKI_getpoolbuf(2);
    if (p_buf != NULL)
    {
        if (p_bd_name != (UINT8 *)p_name)
        {
            memset(p_bd_name, 0, 0x20);
            strncpy((char *)p_bd_name, p_name, 0x1F);
        }
        btsnd_hcic_change_name(p_buf, p_bd_name);
        return BTM_CMD_STARTED;
    }
    return BTM_NO_RESOURCES;
}

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

/* ------------------------------------------------------------------ */
/*  BTM_VendorSpecificCommand - send a vendor specific HCI command.   */
/* ------------------------------------------------------------------ */
tBTM_STATUS BTM_VendorSpecificCommand(UINT16 opcode, UINT8 param_len,
                                      UINT8 *p_param_buf, tBTM_VSC_CMPL_CB *p_cback)
{
    void *p_buf;

    if (btm_cb.trace_level >= BT_TRACE_LEVEL_EVENT)
        LogMsg_2(0xD0003, "BTM: BTM_VendorSpecificCommand: Opcode: 0x%04X, ParamLen: %i.",
                 opcode, param_len);

    /* Check if the device is busy */
    if (btm_cb.rsp_pending != BTM_VSC_CMD_DONE)
    {
        if (btm_cb.trace_level >= BT_TRACE_LEVEL_EVENT)
            LogMsg_0(0xD0003, "BTM: Unable to send vendor specific command (controller is busy).");
        return BTM_BUSY;
    }

    p_buf = GKI_getbuf((UINT16)(param_len + 11));
    if (p_buf != NULL)
    {
        btm_cb.p_vsc_callback = p_cback;
        if (p_cback != NULL)
            btm_cb.rsp_pending = 1;

        btsnd_hcic_vendor_spec_cmd(p_buf, opcode, param_len, p_param_buf);

        if (p_cback != NULL)
            return BTM_CMD_STARTED;
        return BTM_SUCCESS;
    }
    return BTM_NO_RESOURCES;
}

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

    if (cb->local_version.manufacturer != 0x12
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
        LogMsg_1(0xD0003, "BTM: BTM_WritePageTimeout: Timeout: %d.", (const char *)timeout);

    btm_cb.page_timeout = (UINT16)((timeout * 625) / 1000);

    p = (UINT8 *)GKI_getpoolbuf(2);
    if (p != NULL)
    {
        btsnd_hcic_write_page_tout(p, timeout);
        return BTM_SUCCESS;
    }

    return BTM_NO_RESOURCES;
}

/* Retail pools the log strings of BTM_WriteVoiceSettings and
 * BTM_EnableTestMode in this TU's .data between the WritePageTimeout and
 * ReadStoredLinkKey literals; both functions themselves are linker-GC'd
 * dead code, so only the strings survive in the split. */
char btm_write_voice_settings_msg[] = "BTM: BTM_WriteVoiceSettings: Settings: 0x%04x.";
char btm_enable_test_mode_msg[] = "BTM: BTM_EnableTestMode";

/* ------------------------------------------------------------------ */
/*  BTM_ReadStoredLinkKey - read the stored link key(s) from the      */
/*  controller's NVRAM.  BD_ANY means read all keys.                  */
/* ------------------------------------------------------------------ */
tBTM_STATUS BTM_ReadStoredLinkKey(BD_ADDR bd_addr, tBTM_CMPL_CB *p_cb)
{
    void *p_buf;
    UINT8 read_all = 0;
    BD_ADDR local_bd_addr;

    /* Check if the device is busy */
    if (btm_cb.p_stored_link_key_cmpl_cb != NULL)
        return BTM_BUSY;

    /* If BD_ANY is sent, read all */
    if (!bd_addr)
    {
        read_all = 1;
        bd_addr = local_bd_addr;
    }

    if (btm_cb.trace_level >= BT_TRACE_LEVEL_EVENT)
        LogMsg_1(0xD0003, "BTM: BTM_ReadStoredLinkKey: Read_All: %s",
                 (read_all) ? lbl_8066592C : lbl_80665934);

    p_buf = GKI_getpoolbuf(2);
    if (p_buf != NULL)
    {
        btm_cb.p_stored_link_key_cmpl_cb = p_cb;
        btsnd_hcic_read_stored_key(p_buf, bd_addr, read_all);
        return BTM_SUCCESS;
    }
    return BTM_NO_RESOURCES;
}

/* ------------------------------------------------------------------ */
/*  BTM_WriteStoredLinkKey - write link keys to the controller NVRAM. */
/* ------------------------------------------------------------------ */
tBTM_STATUS BTM_WriteStoredLinkKey(UINT8 num_keys, BD_ADDR *bd_addr,
                                   LINK_KEY *link_key, tBTM_CMPL_CB *p_cb)
{
    void *p_buf;

    /* Check if the device is busy */
    if (btm_cb.p_stored_link_key_cmpl_cb != NULL)
        return BTM_BUSY;

    if (btm_cb.trace_level >= BT_TRACE_LEVEL_EVENT)
        LogMsg_1(0xD0003, "BTM: BTM_WriteStoredLinkKey: num_keys: %d", (const char *)num_keys);

    if (num_keys > 0x0B)
        num_keys = 0x0B;

    p_buf = GKI_getpoolbuf(2);
    if (p_buf != NULL)
    {
        btm_cb.p_stored_link_key_cmpl_cb = p_cb;
        btsnd_hcic_write_stored_key(p_buf, num_keys, (UINT8 *)bd_addr, (UINT8 *)link_key);
        return BTM_SUCCESS;
    }
    return BTM_NO_RESOURCES;
}

/* ------------------------------------------------------------------ */
/*  BTM_DeleteStoredLinkKey - delete the stored link key(s) from the  */
/*  controller NVRAM.  BD_ANY means delete all keys.                  */
/* ------------------------------------------------------------------ */
tBTM_STATUS BTM_DeleteStoredLinkKey(BD_ADDR bd_addr, tBTM_CMPL_CB *p_cb)
{
    UINT8 delete_all_flag = 0;
    BD_ADDR local_bd_addr;

    /* Check if the device is busy */
    if (btm_cb.p_stored_link_key_cmpl_cb != NULL)
        return BTM_BUSY;

    /* If BD_ANY is sent, delete all keys */
    if (!bd_addr)
    {
        delete_all_flag = 1;
        bd_addr = local_bd_addr;
    }

    if (btm_cb.trace_level >= BT_TRACE_LEVEL_EVENT)
        LogMsg_1(0xD0003, "BTM: BTM_DeleteStoredLinkKey: delete_all_flag: %s",
                 (delete_all_flag) ? lbl_8066592C : lbl_80665934);

    btm_cb.p_stored_link_key_cmpl_cb = p_cb;

    if (!btsnd_hcic_delete_stored_key(bd_addr, delete_all_flag))
        return BTM_NO_RESOURCES;
    return BTM_SUCCESS;
}

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
