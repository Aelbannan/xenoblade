// Decompiled btm_inq.c - high-level C reconstruction (Wii RVL SDK).
//
// Retail symbol btm_cb is a .bss object at 0x805BC2F8 (size 0x27C4).
// btm_int.h's tBTM_CB / tBTM_INQUIRY_VAR_ST layouts do not match the Wii
// binary (e.g. retail BTM_INQ_DB_SIZE is 12, not 40; the inq vars struct
// has extra padding), so we declare a local surrogate covering only the
// fields we touch (verified against
// build/us/asm/RVL_SDK/src/revolution/bte/stack/btm/btm_inq.s offsets).

#include <string.h>

#include "revolution/BTE/stack/include/bt_types.h"

/* ------------------------------------------------------------------ */
/*  Return codes / constants (from btm_api.h)                          */
/* ------------------------------------------------------------------ */
typedef UINT8 tBTM_STATUS;

#define BTM_SUCCESS          0
#define BTM_CMD_STARTED      1
#define BTM_BUSY             2
#define BTM_NO_RESOURCES     3
#define BTM_MODE_UNSUPPORTED 4
#define BTM_ILLEGAL_VALUE    5
#define BTM_WRONG_MODE       6
#define BTM_BAD_VALUE_RET    9

#define BTM_INQ_DB_SIZE      12
#define BTM_RMT_NAME_INACTIVE 0
#define BTM_RMT_NAME_EXT     0x01

#define BTM_INQ_INACTIVE_STATE  0
#define BTM_INQ_CLR_FILT_STATE  1
#define BTM_INQ_SET_FILT_STATE  2
#define BTM_INQ_REMNAME_STATE   4

#define BT_TRACE_LEVEL_API   3
#define BT_TRACE_LEVEL_DEBUG 5

#define HCI_SUCCESS          0x00
#define HCI_ERR_UNSPECIFIED  0x1F

#define BTM_ERR_PROCESSING   10
#define BTM_DEV_RESET        12

/* Local controller LMP feature bits (byte 3 of the feature pages) */
#define HCI_FEATURE_INTERLACED_INQ_SCAN_MASK   0x10
#define HCI_FEATURE_INTERLACED_PAGE_SCAN_MASK  0x20
#define HCI_FEATURE_INQ_RSSI_MASK              0x40

#define HCI_CMD_POOL_ID       2

#define BTM_INQ_RESULT_STANDARD     0
#define BTM_INQ_RESULT_WITH_RSSI    1

#define BTM_SCAN_TYPE_STANDARD      0
#define BTM_SCAN_TYPE_INTERLACED    1

#define BTM_NON_CONNECTABLE         0
#define BTM_CONNECTABLE             1

#define BTM_NON_DISCOVERABLE        0

#define BTM_DEFAULT_CONN_WINDOW     0x0012
#define BTM_DEFAULT_CONN_INTERVAL   0x0800
#define BTM_MIN_CONN_WINDOW         0x0012
#define BTM_MAX_CONN_WINDOW         0x1000
#define BTM_MIN_CONN_INTERVAL       0x0012
#define BTM_MAX_CONN_INTERVAL       0x1000

#define BTM_FILTER_TYPE_CLEAR       0
#define BTM_FILTER_TYPE_DEV_CLASS   1
#define BTM_FILTER_TYPE_BD_ADDR     2
#define HCI_FILTER_INQUIRY_RESULT   0x01

#define BTM_EXT_RMT_NAME_TIMEOUT    40
#define BTU_TTYPE_BTM_RMT_NAME      10
#define BTM_CLOCK_OFFSET_VALID      0x8000

#define HCI_PAGE_SCAN_REP_MODE_R1   0x01
#define HCI_MANDATARY_PAGE_SCAN_MODE 0x00

#define HCI_INQUIRY_SCAN_ENABLED    0x01
#define HCI_PAGE_SCAN_ENABLED       0x02

/* ------------------------------------------------------------------ */
/*  Callback / log types                                              */
/* ------------------------------------------------------------------ */
typedef void (tBTM_CMPL_CB)(void *p1);
typedef void (tBTM_INQ_DB_CHANGE_CB)(void *p_inq_info, BOOLEAN is_new);

extern void LogMsg_0 (UINT32 trace_set_mask, const char *p_str);
extern void LogMsg_1 (UINT32 trace_set_mask, const char *fmt_str, UINT32 p1);
extern void LogMsg_2 (UINT32 trace_set_mask, const char *fmt_str, UINT32 p1, UINT32 p2);
extern void LogMsg_3 (UINT32 trace_set_mask, const char *fmt_str, UINT32 p1, UINT32 p2,
                      UINT32 p3);
extern void LogMsg_6 (UINT32 trace_set_mask, const char *fmt_str, UINT32 p1, UINT32 p2,
                      UINT32 p3, UINT32 p4, UINT32 p5, UINT32 p6);
extern void btu_stop_timer (void *p_tle);
extern void btu_start_timer (void *p_tle, UINT16 type, UINT32 timeout);
extern void *GKI_getpoolbuf (UINT8 pool_id);
extern void GKI_freebuf (void *p_buf);

extern void btsnd_hcic_write_inqscan_type (void *p_buf, UINT8 type);
extern BOOLEAN BTM_IsDeviceUp (void);
extern void btsnd_hcic_write_pagescan_type (void *p_buf, UINT8 type);
extern void btsnd_hcic_write_inquiry_mode (void *p_buf, UINT8 mode);
extern void btsnd_hcic_write_pagescan_cfg (void *p_buf, UINT16 interval, UINT16 window);
extern void btsnd_hcic_write_scan_enable (void *p_buf, UINT8 mode);
extern BOOLEAN btsnd_hcic_inq_cancel (void);
extern void btsnd_hcic_set_event_filter (void *p_buf, UINT8 filt_type, UINT8 filt_cond_type,
                                         UINT8 *p_filt_cond, UINT8 filt_cond_len);
extern BOOLEAN btsnd_hcic_rmt_name_req (BD_ADDR remote_bda, UINT8 page_scan_rep_mode,
                                         UINT8 page_scan_mode, UINT16 clock_offset);
extern BOOLEAN btsnd_hcic_rmt_name_req_cancel (BD_ADDR remote_bda);
extern void btm_sec_rmt_name_request_complete (UINT8 *bd_addr, UINT8 *bd_name,
                                               UINT8 status);

/* ------------------------------------------------------------------ */
/*  Remote name result structure (from btm_api.h)                     */
/* ------------------------------------------------------------------ */
typedef struct
{
    UINT16  status;
    UINT16  length;
    BD_NAME remote_bd_name;
} tBTM_REMOTE_DEV_NAME;

/* Inquiry completion info (status and number of responses) */
typedef struct
{
    UINT8   status;
    UINT8   num_resp;
} tBTM_INQUIRY_CMPL;

/* Forward declaration (defined below; used by BTM_ReadRemoteDeviceName) */

/* ------------------------------------------------------------------ */
/*  Inquiry database structures (retail layout)                       */
/* ------------------------------------------------------------------ */
typedef struct
{
    UINT16  clock_offset;            /* 0x00 */
    BD_ADDR remote_bd_addr;          /* 0x02 */
    DEV_CLASS dev_class;             /* 0x08 */
    UINT8   page_scan_rep_mode;      /* 0x0B */
    UINT8   page_scan_per_mode;      /* 0x0C */
    UINT8   page_scan_mode;          /* 0x0D */
    INT8    rssi;                    /* 0x0E */
    UINT8   appl_knows_rem_name;     /* 0x0F */
    UINT8   pad[0x12 - 0x10];        /* 0x10-0x11 */
} tBTM_INQ_INFO;                     /* 0x12 */

/* Forward declaration (defined below; used by BTM_ReadRemoteDeviceName) */
extern tBTM_STATUS btm_initiate_rem_name (BD_ADDR remote_bda, tBTM_INQ_INFO *p_cur,
                                           UINT8 origin, UINT32 timeout,
                                           tBTM_CMPL_CB *p_cb);

typedef struct
{
    UINT32      time_of_resp;        /* 0x00 */
    UINT32      inq_count;           /* 0x04 */
    tBTM_INQ_INFO inq_info;          /* 0x08 */
    BOOLEAN     in_use;              /* 0x1A */
} tINQ_DB_ENT;                       /* 0x1C */

typedef struct
{
    UINT32  inq_count;               /* 0x00 */
    BD_ADDR bd_addr;                 /* 0x04 */
} tINQ_BDADDR;                       /* 0x0C */

/* ------------------------------------------------------------------ */
/*  Local btm_cb surrogate - only the fields we touch.                */
/* ------------------------------------------------------------------ */
typedef struct
{
    UINT8              _pad0[0x640];                     /* 0x0000-0x063F */
    UINT8              btm_features[8];                  /* 0x0640 - local LMP features */
    DEV_CLASS          dev_class;                        /* 0x0648 */
    UINT8              _pad0a[0x64E - 0x064B];           /* 0x064B-0x064D */
    UINT8              dev_state;                        /* 0x064E - device state */
    UINT8              _pad0b[0x167C - 0x064F];          /* 0x064F-0x167B */
    tBTM_CMPL_CB      *p_remname_cmpl_cb;                /* 0x167C */
    UINT8              rmt_name_timer_ent[0xC];          /* 0x1680 */
    UINT8              _pad1[0x1698 - 0x168C];           /* 0x168C-0x1697 */
    UINT16             discoverable_mode;                /* 0x1698 */
    UINT16             connectable_mode;                 /* 0x169A */
    UINT16             page_scan_window;                 /* 0x169C */
    UINT16             page_scan_period;                 /* 0x169E */
    UINT16             inq_scan_window;                  /* 0x16A0 */
    UINT16             inq_scan_period;                  /* 0x16A2 */
    UINT16             inq_scan_type;                    /* 0x16A4 */
    UINT16             page_scan_type;                   /* 0x16A6 */
    BD_ADDR            remname_bda;                      /* 0x16A8 */
    UINT8              remname_active;                   /* 0x16AE */
    UINT8              _pad2;                            /* 0x16AF */
    tBTM_CMPL_CB      *p_inq_cmpl_cb;                    /* 0x16B0 */
    void              *p_inq_results_cb;                 /* 0x16B4 */
    tBTM_CMPL_CB      *p_inqfilter_cmpl_cb;              /* 0x16B8 */
    tBTM_INQ_DB_CHANGE_CB *p_inq_change_cb;              /* 0x16BC */
    UINT32             inq_counter;                      /* 0x16C0 */
    UINT8              inq_timer_ent[0xC];               /* 0x16C4 */
    UINT8              _pad3[0x16DC - 0x16D0];           /* 0x16D0-0x16DB */
    tINQ_BDADDR       *p_bd_db;                          /* 0x16DC */
    UINT16             num_bd_entries;                   /* 0x16E0 */
    UINT16             max_bd_entries;                   /* 0x16E2 */
    tINQ_DB_ENT        inq_db[BTM_INQ_DB_SIZE];          /* 0x16E4 */
    UINT8              _pad4[0x183E - 0x16E4 -
                             sizeof(tINQ_DB_ENT) * BTM_INQ_DB_SIZE]; /* 0x1834-0x183D */
    tBTM_INQUIRY_CMPL  inq_cmpl_info;                    /* 0x183E */
    UINT8              _pad4a[0x1844 - 0x1840];          /* 0x1840-0x1843 */
    UINT8              inqfilt_active;                   /* 0x1844 */
    UINT8              inqfilt_type;                     /* 0x1845 */
    UINT8              _pad5;                            /* 0x1846 */
    UINT8              pending_filt_complete_event;      /* 0x1847 */
    UINT8              state;                            /* 0x1848 */
    UINT8              _pad6[0x27C0 - 0x1849];           /* 0x1849-0x27BF */
    UINT8              trace_level;                      /* 0x27C0 */
} tBTM_INQ_CB;

/* The real global */
extern tBTM_INQ_CB btm_cb;

void BTM_SetDiscoverability() {}

/* ------------------------------------------------------------------ */
/*  BTM_SetInquiryScanType - set the inquiry scan type (standard or    */
/*  interlaced).                                                       */
/* ------------------------------------------------------------------ */
tBTM_STATUS BTM_SetInquiryScanType (UINT16 scan_type)
{
    tBTM_STATUS status = BTM_SUCCESS;
    void       *p_buf;

    if ((scan_type != BTM_SCAN_TYPE_STANDARD) && (scan_type != BTM_SCAN_TYPE_INTERLACED))
        return (BTM_ILLEGAL_VALUE);

    /* Whatever app wants if device is not 1.2 scan type should be STANDARD */
    if (!(btm_cb.btm_features[3] & HCI_FEATURE_INTERLACED_INQ_SCAN_MASK))
        return (BTM_MODE_UNSUPPORTED);

    /* Check for scan type if configuration has been changed */
    if (scan_type != btm_cb.inq_scan_type)
    {
        if (BTM_IsDeviceUp())
        {
            if ((p_buf = GKI_getpoolbuf (HCI_CMD_POOL_ID)) != NULL)
            {
                btsnd_hcic_write_inqscan_type (p_buf, (UINT8)scan_type);
                btm_cb.inq_scan_type = scan_type;
            }
            else
                return (BTM_NO_RESOURCES);
        }
        else
            return (BTM_WRONG_MODE);
    }

    return (BTM_SUCCESS);
}

/* ------------------------------------------------------------------ */
/*  BTM_SetPageScanType - set the page scan type (standard or          */
/*  interlaced).                                                       */
/* ------------------------------------------------------------------ */
tBTM_STATUS BTM_SetPageScanType (UINT16 scan_type)
{
    tBTM_STATUS status = BTM_SUCCESS;
    void       *p_buf;

    if ((scan_type != BTM_SCAN_TYPE_STANDARD) && (scan_type != BTM_SCAN_TYPE_INTERLACED))
        return (BTM_ILLEGAL_VALUE);

    /* Whatever app wants if device is not 1.2 scan type should be STANDARD */
    if (!(btm_cb.btm_features[3] & HCI_FEATURE_INTERLACED_PAGE_SCAN_MASK))
        return (BTM_MODE_UNSUPPORTED);

    /* Check for scan type if configuration has been changed */
    if (scan_type != btm_cb.page_scan_type)
    {
        if (BTM_IsDeviceUp())
        {
            if ((p_buf = GKI_getpoolbuf (HCI_CMD_POOL_ID)) != NULL)
            {
                btsnd_hcic_write_pagescan_type (p_buf, (UINT8)scan_type);
                btm_cb.page_scan_type = scan_type;
            }
            else
                return (BTM_NO_RESOURCES);
        }
        else
            return (BTM_WRONG_MODE);
    }

    return (BTM_SUCCESS);
}

/* ------------------------------------------------------------------ */
/*  BTM_SetInquiryMode - set the inquiry result mode (standard or      */
/*  with RSSI).                                                        */
/* ------------------------------------------------------------------ */
tBTM_STATUS BTM_SetInquiryMode (UINT8 mode)
{
    tBTM_STATUS status = BTM_SUCCESS;
    void       *p_buf;

    if ((mode != BTM_INQ_RESULT_STANDARD) && (mode != BTM_INQ_RESULT_WITH_RSSI))
        return (BTM_ILLEGAL_VALUE);

    /* If mode is not supported by controller, return */
    if (!(btm_cb.btm_features[3] & HCI_FEATURE_INQ_RSSI_MASK))
        return (BTM_MODE_UNSUPPORTED);

    /* If the device is not up, return */
    if (!BTM_IsDeviceUp())
        return (BTM_WRONG_MODE);

    /* Send the HCI command */
    if ((p_buf = GKI_getpoolbuf (HCI_CMD_POOL_ID)) != NULL)
    {
        btsnd_hcic_write_inquiry_mode (p_buf, mode);
    }
    else
        return (BTM_NO_RESOURCES);

    return (BTM_SUCCESS);
}

/* ------------------------------------------------------------------ */
/*  BTM_SetConnectability - set the device into or out of connectable  */
/*  mode (page scans enabled).                                         */
/* ------------------------------------------------------------------ */
tBTM_STATUS BTM_SetConnectability (UINT16 page_mode, UINT16 window, UINT16 interval)
{
    UINT8        scan_mode = 0;
    void        *p_buf;
    tBTM_INQ_CB *p_inq = &btm_cb;

    /* Check mode parameter */
    if ((page_mode != BTM_NON_CONNECTABLE) && (page_mode != BTM_CONNECTABLE))
        return (BTM_ILLEGAL_VALUE);

    /* Make sure the controller is active */
    if (btm_cb.dev_state < 3)
        return (BTM_DEV_RESET);

    /* If the window and/or interval is '0', set to default values */
    if (!window)
        window = BTM_DEFAULT_CONN_WINDOW;

    if (!interval)
        interval = BTM_DEFAULT_CONN_INTERVAL;

    if (btm_cb.trace_level >= BT_TRACE_LEVEL_API)
        LogMsg_3 (TRACE_CTRL_GENERAL | TRACE_LAYER_BTM | TRACE_ORG_STACK | TRACE_TYPE_API,
                  "BTM_SetConnectability: mode %d [NonConn-0, Conn-1], window 0x%04x, interval 0x%04x",
                  page_mode, window, interval);

    /* Check for valid window and interval parameters */
    /* Only check window and duration if mode is connectable */
    if (page_mode == BTM_CONNECTABLE)
    {
        /* window must be less than or equal to interval */
        if ((window < BTM_MIN_CONN_WINDOW) || (window > BTM_MAX_CONN_WINDOW) ||
            (interval < BTM_MIN_CONN_INTERVAL) || (interval > BTM_MAX_CONN_INTERVAL) ||
            (window > interval))
        {
            return (BTM_ILLEGAL_VALUE);
        }

        scan_mode |= HCI_PAGE_SCAN_ENABLED;
    }

    /* Send down the page scan window and period if changed */
    if ((window != p_inq->page_scan_window) || (interval != p_inq->page_scan_period))
    {
        if ((p_buf = GKI_getpoolbuf (HCI_CMD_POOL_ID)) != NULL)
        {
            p_inq->page_scan_window = window;
            p_inq->page_scan_period = interval;
            btsnd_hcic_write_pagescan_cfg (p_buf, interval, window);
        }
        else
            return (BTM_NO_RESOURCES);
    }

    /* Keep the inquiry scan as previously set */
    if ((p_buf = GKI_getpoolbuf (HCI_CMD_POOL_ID)) != NULL)
    {
        if (p_inq->discoverable_mode)
            scan_mode |= HCI_INQUIRY_SCAN_ENABLED;

        p_inq->connectable_mode = page_mode;
        btsnd_hcic_write_scan_enable (p_buf, scan_mode);
    }
    else
        return (BTM_NO_RESOURCES);

    return (BTM_SUCCESS);
}
/* ------------------------------------------------------------------ */
/*  BTM_IsInquiryActive - return the current inquiry state byte.      */
/* ------------------------------------------------------------------ */
UINT16 BTM_IsInquiryActive(void)
{
    return btm_cb.state;
}

/* ------------------------------------------------------------------ */
/*  BTM_CancelInquiry - cancel an inquiry if active.                   */
/* ------------------------------------------------------------------ */
tBTM_STATUS BTM_CancelInquiry (void)
{
    tBTM_INQ_CB *p_inq = &btm_cb;
    tBTM_STATUS  status = BTM_SUCCESS;

    if (p_inq->trace_level >= BT_TRACE_LEVEL_API)
        LogMsg_0 (TRACE_CTRL_GENERAL | TRACE_LAYER_BTM | TRACE_ORG_STACK | TRACE_TYPE_API,
                  "BTM_CancelInquiry called");

    /* Make sure the device is up */
    if (!BTM_IsDeviceUp())
        return (BTM_WRONG_MODE);

    /* Only cancel if not in periodic mode, otherwise the caller should call */
    /* BTM_CancelPeriodicMode */
    if ((p_inq->state != BTM_INQ_INACTIVE_STATE) && (!(p_inq->state & BTM_INQ_REMNAME_STATE)))
    {
        p_inq->state = BTM_INQ_INACTIVE_STATE;
        p_inq->pending_filt_complete_event = 0;
        p_inq->p_inq_results_cb = NULL;    /* Do not notify caller any more */
        p_inq->p_inq_cmpl_cb = NULL;       /* Do not notify caller any more */

        /* If the event filter is in progress, mark it so that the processing of the return */
        /* event will be ignored */
        if (p_inq->inqfilt_active)
        {
            p_inq->inqfilt_active = FALSE;
            p_inq->inqfilt_type++;
        }
        /* Initiate the cancel inquiry */
        else
        {
            if (!btsnd_hcic_inq_cancel())
                status = BTM_NO_RESOURCES;
        }

        p_inq->inq_counter++;

        /* Clear the BD-addr results filter */
        if (btm_cb.p_bd_db)
        {
            GKI_freebuf (btm_cb.p_bd_db);
            btm_cb.p_bd_db = NULL;
        }
        p_inq->num_bd_entries = 0;
        p_inq->max_bd_entries = 0;
    }

    return (status);
}

void BTM_StartInquiry() {}

/* ------------------------------------------------------------------ */
/*  BTM_ReadRemoteDeviceName - initiate a remote device name request.  */
/* ------------------------------------------------------------------ */
tBTM_STATUS BTM_ReadRemoteDeviceName (BD_ADDR remote_bda, tBTM_CMPL_CB *p_cb)
{
    tBTM_INQ_INFO *p_cur = NULL;
    tINQ_DB_ENT   *p_ent;
    UINT16         xx;

    if (btm_cb.trace_level >= BT_TRACE_LEVEL_API)
        LogMsg_6 (TRACE_CTRL_GENERAL | TRACE_LAYER_BTM | TRACE_ORG_STACK | TRACE_TYPE_API,
                  "BTM_ReadRemoteDeviceName: bd addr [%02x%02x%02x%02x%02x%02x]",
                  remote_bda[0], remote_bda[1], remote_bda[2],
                  remote_bda[3], remote_bda[4], remote_bda[5]);

    /* Use the remote device's clock offset if it is in the local inquiry database */
    p_ent = btm_cb.inq_db;
    for (xx = 0; xx < BTM_INQ_DB_SIZE; xx++, p_ent++)
    {
        if (p_ent->in_use && memcmp (p_ent->inq_info.remote_bd_addr, remote_bda, BD_ADDR_LEN) == 0)
            break;
    }

    if (xx < BTM_INQ_DB_SIZE)
        p_cur = &p_ent->inq_info;
    else
        p_ent = NULL;

    return (btm_initiate_rem_name (remote_bda, p_cur, BTM_RMT_NAME_EXT,
                                   BTM_EXT_RMT_NAME_TIMEOUT, p_cb));
}
/* ------------------------------------------------------------------ */
/*  BTM_CancelRemoteDeviceName - cancel a pending remote name request.*/
/* ------------------------------------------------------------------ */
tBTM_STATUS BTM_CancelRemoteDeviceName(void)
{
    tBTM_INQ_CB *p = &btm_cb;

    if (p->trace_level >= BT_TRACE_LEVEL_API)
        LogMsg_0(TRACE_CTRL_GENERAL | TRACE_LAYER_BTM | TRACE_ORG_STACK | TRACE_TYPE_API,
                 "BTM_CancelRemoteDeviceName()");

    /* Make sure the device is not busy */
    if (p->remname_active & BTM_RMT_NAME_EXT)
    {
        if (btsnd_hcic_rmt_name_req_cancel(p->remname_bda))
            return (BTM_CMD_STARTED);
        else
            return (BTM_NO_RESOURCES);
    }
    else
    {
        return (BTM_WRONG_MODE);
    }
}

/* ------------------------------------------------------------------ */
/*  BTM_InqDbRead - look up an inquiry DB entry by BD address.        */
/* ------------------------------------------------------------------ */
tBTM_INQ_INFO *BTM_InqDbRead(BD_ADDR p_bda)
{
    UINT16      xx;
    tINQ_DB_ENT *p_ent = btm_cb.inq_db;

    if (btm_cb.trace_level >= BT_TRACE_LEVEL_API)
        LogMsg_6(TRACE_CTRL_GENERAL | TRACE_LAYER_BTM | TRACE_ORG_STACK | TRACE_TYPE_API,
                 "BTM_InqDbRead: bd addr [%02x%02x%02x%02x%02x%02x]",
                 p_bda[0], p_bda[1], p_bda[2], p_bda[3], p_bda[4], p_bda[5]);

    for (xx = 0; xx < BTM_INQ_DB_SIZE; xx++, p_ent++)
    {
        if (p_ent->in_use && memcmp(p_ent->inq_info.remote_bd_addr, p_bda, BD_ADDR_LEN) == 0)
            return (&p_ent->inq_info);
    }

    return (NULL);
}

/* ------------------------------------------------------------------ */
/*  BTM_InqDbFirst - return the first in-use inquiry DB entry.        */
/* ------------------------------------------------------------------ */
tBTM_INQ_INFO *BTM_InqDbFirst(void)
{
    UINT16      xx;
    tINQ_DB_ENT *p_ent = btm_cb.inq_db;

    for (xx = 0; xx < BTM_INQ_DB_SIZE; xx++, p_ent++)
    {
        if (p_ent->in_use)
            return (&p_ent->inq_info);
    }

    /* If here, no database entries */
    return (NULL);
}

/* ------------------------------------------------------------------ */
/*  BTM_InqDbNext - return the next in-use entry after p_cur.         */
/* ------------------------------------------------------------------ */
tBTM_INQ_INFO *BTM_InqDbNext(tBTM_INQ_INFO *p_cur)
{
    tINQ_DB_ENT *p_ent;
    UINT16      xx;

    if (p_cur)
    {
        /* If the current entry is valid, start from the next entry */
        xx = (UINT16)(((tINQ_DB_ENT *)((UINT8 *)p_cur - offsetof(tINQ_DB_ENT, inq_info)) -
                       btm_cb.inq_db) + 1);
        p_ent = &btm_cb.inq_db[xx];

        for (; xx < BTM_INQ_DB_SIZE; xx++, p_ent++)
        {
            if (p_ent->in_use)
                return (&p_ent->inq_info);
        }

        /* If here, no more database entries */
        return (NULL);
    }
    else
    {
        return (BTM_InqDbFirst());
    }
}

/* ------------------------------------------------------------------ */
/*  BTM_ClearInqDb - clear the inquiry database (optionally one addr).*/
/* ------------------------------------------------------------------ */
tBTM_STATUS BTM_ClearInqDb(BD_ADDR p_bda)
{
    UINT16      xx;
    tINQ_DB_ENT *p_ent;

    /* If the device is in the middle of an inquiry, return busy */
    if (btm_cb.state != BTM_INQ_INACTIVE_STATE ||
        btm_cb.remname_active ||
        btm_cb.inqfilt_active)
    {
        return (BTM_BUSY);
    }

    p_ent = btm_cb.inq_db;

    for (xx = 0; xx < BTM_INQ_DB_SIZE; xx++, p_ent++)
    {
        if (p_ent->in_use)
        {
            if (!p_bda || memcmp(p_ent->inq_info.remote_bd_addr, p_bda, BD_ADDR_LEN) == 0)
            {
                p_ent->in_use = FALSE;

                if (btm_cb.p_inq_change_cb)
                    btm_cb.p_inq_change_cb(&p_ent->inq_info, FALSE);
            }
        }
    }

    return (BTM_SUCCESS);
}

/* ------------------------------------------------------------------ */
/*  btm_inq_db_reset - clear the inquiry database and pending          */
/*  callbacks at reset.                                                */
/* ------------------------------------------------------------------ */
void btm_inq_db_reset (void)
{
    tBTM_REMOTE_DEV_NAME rem_name;
    UINT8               num_responses;
    UINT8               temp_state;
    tBTM_STATUS         status;
    tBTM_INQ_CB        *p_inq = &btm_cb;
    tINQ_DB_ENT        *p_ent;
    UINT16              xx;

    /* If an inquiry is active, cancel it */
    btu_stop_timer (p_inq->inq_timer_ent);

    /* If an inquiry or periodic inquiry is active, reset the mode to inactive */
    if (p_inq->state != BTM_INQ_INACTIVE_STATE)
    {
        temp_state = p_inq->state;    /* Save so state can change BEFORE callback is called */
        p_inq->state = BTM_INQ_INACTIVE_STATE;

        /* If not a periodic inquiry, the complete callback must be called to notify caller */
        if ((temp_state == BTM_INQ_CLR_FILT_STATE) || (temp_state == BTM_INQ_SET_FILT_STATE))
        {
            if (p_inq->p_inq_cmpl_cb)
            {
                num_responses = 0;
                (*p_inq->p_inq_cmpl_cb)(&num_responses);
            }
        }
    }

    /* Cancel a remote name request if active, and notify the caller (if waiting) */
    if (p_inq->remname_active & BTM_RMT_NAME_EXT)
    {
        btu_stop_timer (p_inq->rmt_name_timer_ent);

        p_inq->remname_active = BTM_RMT_NAME_INACTIVE;
        memset (p_inq->remname_bda, 0, BD_ADDR_LEN);

        if (p_inq->p_remname_cmpl_cb)
        {
            rem_name.status = BTM_DEV_RESET;
            (*p_inq->p_remname_cmpl_cb)(&rem_name);
            p_inq->p_remname_cmpl_cb = NULL;
        }
    }

    /* Cancel an inquiry filter request if active, and notify the caller (if waiting) */
    if (p_inq->inqfilt_active)
    {
        p_inq->inqfilt_active = FALSE;

        if (p_inq->p_inqfilter_cmpl_cb)
        {
            status = BTM_DEV_RESET;
            (*p_inq->p_inqfilter_cmpl_cb)(&status);
        }
    }

    p_inq->pending_filt_complete_event = 0;
    p_inq->inqfilt_type = 0;
    p_inq->p_inq_results_cb = NULL;

    /* Clear out all the entries in the database */
    p_ent = btm_cb.inq_db;
    for (xx = 0; xx < BTM_INQ_DB_SIZE; xx++, p_ent++)
    {
        if (p_ent->in_use)
        {
            p_ent->in_use = FALSE;

            if (btm_cb.p_inq_change_cb)
                (*btm_cb.p_inq_change_cb)(&p_ent->inq_info, FALSE);
        }
    }

    /* Clear the BD-addr results filter */
    if (btm_cb.p_bd_db)
    {
        GKI_freebuf (btm_cb.p_bd_db);
        btm_cb.p_bd_db = NULL;
    }
    p_inq->num_bd_entries = 0;
    p_inq->max_bd_entries = 0;
    p_inq->discoverable_mode = BTM_NON_DISCOVERABLE;
    p_inq->connectable_mode = BTM_NON_CONNECTABLE;
    p_inq->page_scan_type = BTM_SCAN_TYPE_STANDARD;
    p_inq->inq_scan_type = BTM_SCAN_TYPE_STANDARD;
}

void btm_inq_db_init(void) {}

/* ------------------------------------------------------------------ */
/*  btm_inq_find_bdaddr - search the inquiry BD-addr filter table.    */
/* ------------------------------------------------------------------ */
BOOLEAN btm_inq_find_bdaddr(BD_ADDR p_bda)
{
    tINQ_BDADDR *p_ent = btm_cb.p_bd_db;
    UINT16      xx;
    tBTM_INQ_CB *p = &btm_cb;

    if ((p->state & BTM_INQ_REMNAME_STATE) || !p_ent)
    {
        /* If the database hasn't been allocated, we can not have an entry */
        return (FALSE);
    }

    for (xx = 0; xx < p->num_bd_entries; xx++, p_ent++)
    {
        if (memcmp(p_ent->bd_addr, p_bda, BD_ADDR_LEN) == 0 &&
            p_ent->inq_count == p->inq_counter)
        {
            return (TRUE);
        }
    }

    /* If here, not found.  Add the address to the database. */
    if (xx < p->max_bd_entries)
    {
        p_ent->inq_count = p->inq_counter;
        memcpy(p_ent->bd_addr, p_bda, BD_ADDR_LEN);
        p->num_bd_entries++;
    }

    return (FALSE);
}

/* ------------------------------------------------------------------ */
/*  btm_inq_db_new - create a new record in the inquiry database.     */
/* ------------------------------------------------------------------ */
tINQ_DB_ENT *btm_inq_db_new(BD_ADDR p_bda)
{
    UINT16      xx;
    tINQ_DB_ENT *p_ent = btm_cb.inq_db;
    tINQ_DB_ENT *p_oldest = btm_cb.inq_db;
    UINT32      time_of_resp = 0xFFFFFFFF;

    /* Find an unused entry, or the oldest in-use entry */
    for (xx = 0; xx < BTM_INQ_DB_SIZE; xx++, p_ent++)
    {
        if (!p_ent->in_use)
        {
            memset(p_ent, 0, sizeof(tINQ_DB_ENT));
            memcpy(p_ent->inq_info.remote_bd_addr, p_bda, BD_ADDR_LEN);
            p_ent->in_use = TRUE;
            return (p_ent);
        }

        if (p_ent->time_of_resp < time_of_resp)
        {
            p_oldest = p_ent;
            time_of_resp = p_ent->time_of_resp;
        }
    }

    /* If the oldest entry is in use, discard it */
    if (btm_cb.p_inq_change_cb)
        btm_cb.p_inq_change_cb(&p_oldest->inq_info, FALSE);

    memset(p_oldest, 0, sizeof(tINQ_DB_ENT));
    memcpy(p_oldest->inq_info.remote_bd_addr, p_bda, BD_ADDR_LEN);
    p_oldest->in_use = TRUE;

    return (p_oldest);
}

/* ------------------------------------------------------------------ */
/*  btm_set_inq_event_filter - set the inquiry event filter.           */
/* ------------------------------------------------------------------ */
UINT8 btm_set_inq_event_filter (UINT8 filter_type, BD_ADDR bd_addr)
{
    UINT8  condition_buf[DEV_CLASS_LEN * 2];
    UINT8 *p_cond = condition_buf;
    UINT8  condition_length = DEV_CLASS_LEN * 2;
    void  *p_buf;

    if ((p_buf = GKI_getpoolbuf (HCI_CMD_POOL_ID)) != NULL)
    {
        switch (filter_type)
        {
        case BTM_FILTER_TYPE_DEV_CLASS:
            /* Copy the device class and device class fields into contiguous memory to send to HCI */
            memcpy (condition_buf, bd_addr, DEV_CLASS_LEN);
            memcpy (&condition_buf[DEV_CLASS_LEN], bd_addr + DEV_CLASS_LEN, DEV_CLASS_LEN);

            /* condition length should already be set as the default */
            break;

        case BTM_FILTER_TYPE_BD_ADDR:
            p_cond = bd_addr;

            /* condition length should already be set as the default */
            break;

        case BTM_FILTER_TYPE_CLEAR:
            condition_length = 0;
            break;

        default:
            return (BTM_ILLEGAL_VALUE);    /* Bad parameter was passed in */
        }

        btm_cb.inqfilt_active = TRUE;

        /* Filter the inquiry results for the specified condition type and value */
        btsnd_hcic_set_event_filter (p_buf, HCI_FILTER_INQUIRY_RESULT, filter_type,
                                     p_cond, condition_length);

        return (BTM_CMD_STARTED);
    }

    return (BTM_NO_RESOURCES);
}

void btm_event_filter_complete() {}

void btm_process_inq_results() {}

/* ------------------------------------------------------------------ */
/*  btm_process_inq_complete - handle the inquiry complete event.      */
/* ------------------------------------------------------------------ */
void btm_process_inq_complete (UINT8 status)
{
    tBTM_CMPL_CB *p_cb = btm_cb.p_inq_cmpl_cb;
    tBTM_INQ_CB  *p_inq = &btm_cb;
    tBTM_STATUS   btm_status;

    /* Ignore any stray or late complete messages if the inquiry is not active */
    if (!p_inq->state)
        return;

    /* Increment the inquiry counter, so the results can be distinguished */
    p_inq->inq_counter++;

    /* status is BTM_SUCCESS if HCI_STATUS is zero */
    btm_status = BTM_SUCCESS;
    if (status != HCI_SUCCESS)
        btm_status = BTM_ERR_PROCESSING;

    p_inq->inq_cmpl_info.status = btm_status;

    /* Do not notify the caller if a remote name request is active */
    if (p_inq->state & BTM_INQ_REMNAME_STATE)
        return;

    p_inq->p_inq_results_cb = NULL;
    p_inq->state = BTM_INQ_INACTIVE_STATE;
    p_inq->pending_filt_complete_event = 0;
    p_inq->p_inq_cmpl_cb = NULL;

    if (btm_cb.p_bd_db)
    {
        GKI_freebuf (btm_cb.p_bd_db);
        btm_cb.p_bd_db = NULL;
    }
    p_inq->num_bd_entries = 0;
    p_inq->max_bd_entries = 0;

    if (btm_cb.trace_level >= BT_TRACE_LEVEL_DEBUG)
        LogMsg_2 (TRACE_CTRL_GENERAL | TRACE_LAYER_BTM | TRACE_ORG_STACK | TRACE_TYPE_DEBUG,
                  "BTM Inq Compl Callback: status 0x%02x, num results %d",
                  p_inq->inq_cmpl_info.status, p_inq->inq_cmpl_info.num_resp);

    /* If we have a callback registered for inquiry complete, call it */
    if (p_cb)
        (*p_cb)(&p_inq->inq_cmpl_info);
}

/* ------------------------------------------------------------------ */
/*  btm_initiate_rem_name - initiate a remote name request.            */
/* ------------------------------------------------------------------ */
tBTM_STATUS btm_initiate_rem_name (BD_ADDR remote_bda, tBTM_INQ_INFO *p_cur, UINT8 origin,
                                   UINT32 timeout, tBTM_CMPL_CB *p_cb)
{
    tBTM_INQ_CB *p_inq = &btm_cb;
    BOOLEAN      start = TRUE;
    BOOLEAN      cmd_ok = TRUE;
    tBTM_STATUS  status = BTM_CMD_STARTED;

    /* Make sure the device is ready */
    if (!BTM_IsDeviceUp())
        return (BTM_WRONG_MODE);

    /* Make sure there are no two remote name requests from external API in progress */
    if (!(p_inq->remname_active & origin))
    {
        if (origin == BTM_RMT_NAME_EXT)
        {
            if (p_inq->remname_active)
            {
                /* If it is the same BD address, we are just re-registering the callback */
                if (memcmp (remote_bda, p_inq->remname_bda, BD_ADDR_LEN) == 0)
                    start = FALSE;
                else
                    return (BTM_BUSY);
            }
            p_inq->p_remname_cmpl_cb = p_cb;
        }
        else if (p_inq->remname_active)
        {
            return (BTM_BUSY);
        }

        if (start)
        {
            memcpy (p_inq->remname_bda, remote_bda, BD_ADDR_LEN);
            btu_start_timer (p_inq->rmt_name_timer_ent, BTU_TTYPE_BTM_RMT_NAME, timeout);

            /* If the database entry exists for the device, use its clock offset */
            if (p_cur)
                cmd_ok = btsnd_hcic_rmt_name_req (remote_bda, p_cur->page_scan_rep_mode,
                                                  p_cur->page_scan_mode,
                                                  (UINT16)(p_cur->clock_offset | BTM_CLOCK_OFFSET_VALID));
            else    /* Otherwise use defaults and mark the clock offset as invalid */
                cmd_ok = btsnd_hcic_rmt_name_req (remote_bda, HCI_PAGE_SCAN_REP_MODE_R1,
                                                  HCI_MANDATARY_PAGE_SCAN_MODE, 0);
        }

        if (cmd_ok)
            p_inq->remname_active |= origin;
        else
            status = BTM_NO_RESOURCES;
    }
    else
    {
        status = BTM_BUSY;
    }

    return (status);
}
/* ------------------------------------------------------------------ */
/*  btm_process_remote_name - handle a remote name response.          */
/* ------------------------------------------------------------------ */
void btm_process_remote_name(BD_ADDR bda, BD_NAME name, UINT16 evt_len, UINT8 hci_status)
{
    tBTM_INQ_CB *p = &btm_cb;
    tBTM_CMPL_CB *p_cb = p->p_remname_cmpl_cb;
    UINT8         remname_active = p->remname_active;
    tBTM_REMOTE_DEV_NAME rem_name;

    btu_stop_timer(p->rmt_name_timer_ent);
    p->remname_active = BTM_RMT_NAME_INACTIVE;

    /* If the HCI command failed, send a fake response */
    if (!hci_status)
    {
        UINT8 *d = rem_name.remote_bd_name;
        UINT8 *s = name;
        UINT16 len = (evt_len < BD_NAME_LEN) ? evt_len : BD_NAME_LEN;

        rem_name.length = len;
        rem_name.status = HCI_SUCCESS;

        while (len > 0)
        {
            *d = *s;
            d++;
            s++;
            len--;
        }
    }
    else
    {
        rem_name.length = 0;
        rem_name.status = BTM_BAD_VALUE_RET;
        rem_name.remote_bd_name[0] = 0;
    }

    memset(p->remname_bda, 0, BD_ADDR_LEN);

    /* If a callback is registered, call it */
    if (remname_active & BTM_RMT_NAME_EXT)
    {
        p->p_remname_cmpl_cb = NULL;

        if (p_cb)
            (*p_cb)(&rem_name);
    }
}

/* ------------------------------------------------------------------ */
/*  btm_inq_rmt_name_failed - remote name request failed.             */
/* ------------------------------------------------------------------ */
void btm_inq_rmt_name_failed(void)
{
    btm_process_remote_name(NULL, NULL, 0, HCI_ERR_UNSPECIFIED);
    btm_sec_rmt_name_request_complete(NULL, NULL, HCI_ERR_UNSPECIFIED);
}
