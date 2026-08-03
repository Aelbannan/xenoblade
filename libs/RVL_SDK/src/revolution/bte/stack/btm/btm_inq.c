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
#define BTM_INQ_ACTIVE_STATE    3
#define BTM_INQ_REMNAME_STATE   4

#define BTM_MAX_BD_ENTRIES      150     /* entries in the inquiry bd-addr results db */

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
#define BTM_LIMITED_DISCOVERABLE    1
#define BTM_GENERAL_DISCOVERABLE    2

#define BTM_GENERAL_INQUIRY         0
#define BTM_LIMITED_INQUIRY         1

#define BTM_CLR_INQUIRY_FILTER          0
#define BTM_FILTER_COND_DEVICE_CLASS    1
#define BTM_FILTER_COND_BD_ADDR         2

#define BTM_INQ_RES_IGNORE_RSSI     0x7f

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
typedef void (tBTM_INQ_RESULTS_CB)(void *p_inq_info);
typedef BOOLEAN (tBTM_INQ_RESULTS_FILTER_CB)(BD_ADDR p_bda, DEV_CLASS p_dev_class);
typedef void (tBTM_INQ_DB_CHANGE_CB)(void *p_inq_info, BOOLEAN is_new);

/* Class-of-device field macros (from btm_api.h) */
#define BTM_COD_SERVICE_LMTD_DISCOVER   0x0020
#define BTM_COD_SERVICE_CLASS_LO_B      0x00E0
#define BTM_COD_MINOR_CLASS(u8, pd)     {u8 = pd[2]&0xFC;}
#define BTM_COD_MAJOR_CLASS(u8, pd)     {u8 = pd[1]&0x1F;}
#define BTM_COD_SERVICE_CLASS(u16, pd)  {u16 = pd[0]; u16<<=8; u16 += pd[1]&0xE0;}
#define FIELDS_TO_COD(pd, mn, mj, sv) {pd[2] = mn; pd[1] = mj + ((sv)&BTM_COD_SERVICE_CLASS_LO_B); pd[0] = ((sv)>>8);}

extern void LogMsg_0 (UINT32 trace_set_mask, const char *p_str);
extern void LogMsg_1 (UINT32 trace_set_mask, const char *fmt_str, UINT32 p1);
extern void LogMsg_2 (UINT32 trace_set_mask, const char *fmt_str, UINT32 p1, UINT32 p2);
extern void LogMsg_3 (UINT32 trace_set_mask, const char *fmt_str, UINT32 p1, UINT32 p2,
                      UINT32 p3);
extern void LogMsg_4 (UINT32 trace_set_mask, const char *fmt_str, UINT32 p1, UINT32 p2,
                      UINT32 p3, UINT32 p4);
extern void LogMsg_6 (UINT32 trace_set_mask, const char *fmt_str, UINT32 p1, UINT32 p2,
                      UINT32 p3, UINT32 p4, UINT32 p5, UINT32 p6);
extern void btu_stop_timer (void *p_tle);
extern void btu_start_timer (void *p_tle, UINT16 type, UINT32 timeout);
extern void *GKI_getpoolbuf (UINT8 pool_id);
extern void GKI_freebuf (void *p_buf);
extern void *GKI_getbuf (UINT16 size);
extern UINT32 GKI_get_tick_count (void);

extern void btsnd_hcic_write_inqscan_type (void *p_buf, UINT8 type);
extern BOOLEAN BTM_IsDeviceUp (void);
extern void btsnd_hcic_write_pagescan_type (void *p_buf, UINT8 type);
extern void btsnd_hcic_write_inquiry_mode (void *p_buf, UINT8 mode);
extern void btsnd_hcic_write_pagescan_cfg (void *p_buf, UINT16 interval, UINT16 window);
extern void btsnd_hcic_write_scan_enable (void *p_buf, UINT8 mode);
extern void btsnd_hcic_write_cur_iac_lap (void *p_buf, UINT8 num_laps, const UINT8 *p_lap_array);
extern BOOLEAN btsnd_hcic_inq_cancel (void);
extern BOOLEAN btsnd_hcic_inquiry (const UINT8 *p_lap, UINT8 duration, UINT8 max_resps);
extern BOOLEAN btsnd_hcic_per_inq_mode (UINT16 max_delay, UINT16 min_delay, const UINT8 *p_lap,
                                        UINT8 duration, UINT8 max_resps);
extern void btsnd_hcic_set_event_filter (void *p_buf, UINT8 filt_type, UINT8 filt_cond_type,
                                         UINT8 *p_filt_cond, UINT8 filt_cond_len);
extern BOOLEAN btsnd_hcic_rmt_name_req (BD_ADDR remote_bda, UINT8 page_scan_rep_mode,
                                         UINT8 page_scan_mode, UINT16 clock_offset);
extern BOOLEAN btsnd_hcic_rmt_name_req_cancel (BD_ADDR remote_bda);
extern void btm_sec_rmt_name_request_complete (UINT8 *bd_addr, UINT8 *bd_name,
                                               UINT8 status);
extern UINT8 *BTM_ReadDeviceClass (void);
extern tBTM_STATUS BTM_SetDeviceClass (DEV_CLASS dev_class);

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

/* Inquiry parameters passed to BTM_StartInquiry / BTM_SetPeriodicInquiryMode */
typedef struct
{
    UINT8   mode;                    /* 0x00 general or limited inquiry */
    UINT8   duration;                /* 0x01 duration in 1.28 sec increments */
    UINT8   max_resps;               /* 0x02 maximum number of responses */
    UINT8   filter_cond_type;        /* 0x03 BD_ADDR, DEV_CLASS, or clear */
    UINT8   filter_cond[DEV_CLASS_LEN * 2]; /* 0x04 filter condition value */
} tBTM_INQ_PARMS;                    /* 0x0A */

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
    UINT8   pad;                     /* 0x0F */
    UINT8   appl_knows_rem_name;     /* 0x10 */
    UINT8   pad2;                    /* 0x11 */
} tBTM_INQ_INFO;                     /* 0x12 */

/* Forward declaration (defined below; used by BTM_ReadRemoteDeviceName) */
extern tBTM_STATUS btm_initiate_rem_name (BD_ADDR remote_bda, tBTM_INQ_INFO *p_cur,
                                           UINT8 origin, UINT32 timeout,
                                           tBTM_CMPL_CB *p_cb);

/* Forward declaration (defined below; used by BTM_StartInquiry / btm_event_filter_complete) */
extern UINT8 btm_set_inq_event_filter (UINT8 filter_type, BD_ADDR bd_addr);

/* Forward declaration (defined below; used by btm_event_filter_complete) */
extern void btm_process_inq_complete (UINT8 status);

/* Forward declaration (defined below; used by btm_inq_rmt_name_failed) */
extern void btm_process_remote_name(BD_ADDR bda, BD_NAME name, UINT16 evt_len, UINT8 hci_status);

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
    tBTM_INQ_PARMS     inqparms;                         /* 0x1834 */
    tBTM_INQUIRY_CMPL  inq_cmpl_info;                    /* 0x183E */
    UINT16             per_min_delay;                    /* 0x1840 */
    UINT16             per_max_delay;                    /* 0x1842 */
    UINT8              inqfilt_active;                   /* 0x1844 */
    UINT8              inqfilt_type;                     /* 0x1845 */
    UINT8              _pad5;                            /* 0x1846 */
    UINT8              pending_filt_complete_event;      /* 0x1847 */
    UINT8              state;                            /* 0x1848 */
    UINT8              _pad6[0x184C - 0x1849];           /* 0x1849-0x184B */
    tBTM_INQ_RESULTS_FILTER_CB *p_inq_results_filter_cb; /* 0x184C */
    UINT8              _pad6a[0x27C0 - 0x1850];          /* 0x1850-0x27BF */
    UINT8              trace_level;                      /* 0x27C0 */
} tBTM_INQ_CB;

/* The real global */
extern tBTM_INQ_CB btm_cb;

/* Forward declarations (defined below; used by earlier functions in the
   reversed -ipa off emission order) */
extern tINQ_DB_ENT *btm_inq_db_new(BD_ADDR p_bda);
extern BOOLEAN btm_inq_find_bdaddr(BD_ADDR p_bda);
extern tBTM_INQ_INFO *BTM_InqDbRead(BD_ADDR p_bda);
extern tBTM_INQ_INFO *BTM_InqDbFirst(void);
extern tBTM_INQ_INFO *BTM_InqDbNext(tBTM_INQ_INFO *p_cur);

/* IAC values for general/limited discoverable modes (retail .sdata2 globals) */
const UINT8 general_inq_lap[4] = { 0x9E, 0x8B, 0x33, 0x00 };
const UINT8 limited_inq_lap[4] = { 0x9E, 0x8B, 0x00, 0x00 };

static __inline tINQ_DB_ENT *btm_inq_db_find (tBTM_INQ_CB *p_inq, BD_ADDR p_bda)
{
    UINT16      yy;
    tINQ_DB_ENT *p_ent = p_inq->inq_db;

    for (yy = 0; yy < BTM_INQ_DB_SIZE; yy++, p_ent++)
    {
        if (p_ent->in_use && memcmp (p_ent->inq_info.remote_bd_addr, p_bda, BD_ADDR_LEN) == 0)
            return (p_ent);
    }

    /* If here, not found */
    return (NULL);
}

void btm_inq_rmt_name_failed(void)
{
    btm_process_remote_name(NULL, NULL, 0, HCI_ERR_UNSPECIFIED);
    btm_sec_rmt_name_request_complete(NULL, NULL, HCI_ERR_UNSPECIFIED);
}

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
    btm_status = BTM_ERR_PROCESSING;
    if (status == HCI_SUCCESS)
        btm_status = BTM_SUCCESS;

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
void btm_process_inq_results (UINT8 *p, BOOLEAN use_rssi)
{
    UINT8         page_scan_per_mode = 0;
    UINT8         page_scan_rep_mode = 0;
    UINT8         num_resp;
    tBTM_INQ_CB  *p_inq = &btm_cb;
    UINT8         xx;
    tINQ_DB_ENT  *p_i;
    tBTM_INQ_INFO *p_cur;
    BOOLEAN       is_new = TRUE;
    tBTM_INQ_RESULTS_CB *p_inq_results_cb = p_inq->p_inq_results_cb;
    UINT8         page_scan_mode = 0;
    UINT8         rssi = 0;
    UINT16        clock_offset;
    BD_ADDR       bda;
    DEV_CLASS     dc;

    /* Only process the results if the inquiry is still active */
    if (p_inq->state == BTM_INQ_INACTIVE_STATE)
        return;

    num_resp = *p;
    p++;

    for (xx = 0; xx < num_resp; xx++)
    {
        /* Extract the inquiry results */
        bda[5] = *p++;
        bda[4] = *p++;
        bda[3] = *p++;
        bda[2] = *p++;
        bda[1] = *p++;
        bda[0] = *p++;
        page_scan_rep_mode = *p++;
        page_scan_per_mode = *p++;

        if (!use_rssi)
            page_scan_mode = *p++;

        dc[2] = *p++;
        dc[1] = *p++;
        dc[0] = *p++;
        clock_offset = (UINT16)((*p) + (((UINT16)(*(p + 1))) << 8));
        p += 2;
        if (use_rssi)
            rssi = *p++;

        /* If the device has already been reported for this inquiry, skip it */
        if (btm_inq_find_bdaddr (bda))
            continue;

        /* Check the inquiry results filter, if set */
        if (p_inq->p_inq_results_filter_cb &&
            (p_inq->p_inq_results_filter_cb (bda, dc) == FALSE))
            continue;

        /* Find the entry for this device in the inquiry database */
        p_i = btm_inq_db_find (p_inq, bda);

        if (p_i == NULL)
        {
            /* Not found - get a new entry (possibly reusing the oldest) */
            p_i = btm_inq_db_new (bda);
            is_new = TRUE;
        }
        else if (p_i->inq_count == p_inq->inq_counter)
        {
            /* If the entry is from the same inquiry, it is a duplicate response */
            is_new = FALSE;
        }

        if (is_new == TRUE)
        {
            p_cur = &p_i->inq_info;

            /* Save the information */
            p_cur->page_scan_rep_mode = page_scan_rep_mode;
            p_cur->page_scan_per_mode = page_scan_per_mode;
            p_cur->page_scan_mode = page_scan_mode;
            p_cur->dev_class[0] = dc[0];
            p_cur->dev_class[1] = dc[1];
            p_cur->dev_class[2] = dc[2];
            p_cur->clock_offset = clock_offset | BTM_CLOCK_OFFSET_VALID;
            if (use_rssi)
                p_cur->rssi = rssi;
            else
                p_cur->rssi = BTM_INQ_RES_IGNORE_RSSI;

            p_i->time_of_resp = GKI_get_tick_count ();
            p_i->inq_count = p_inq->inq_counter;

            p_inq->inq_cmpl_info.num_resp++;
            if (!(p_inq->state & BTM_INQ_REMNAME_STATE) && p_inq->inqparms.max_resps &&
                (p_inq->inq_cmpl_info.num_resp == p_inq->inqparms.max_resps))
            {
                btsnd_hcic_inq_cancel ();
            }

            p_i->inq_info.appl_knows_rem_name = FALSE;

            /* If a callback is registered for the results, call it */
            if (p_inq_results_cb)
                (p_inq_results_cb)(p_cur);

            /* Notify the database change callback, if registered */
            if (p_inq->p_inq_change_cb)
                (p_inq->p_inq_change_cb)(&p_i->inq_info, TRUE);
        }
    }
}

/* ------------------------------------------------------------------ */
/*  btm_process_inq_complete - handle the inquiry complete event.      */
/* ------------------------------------------------------------------ */
void btm_event_filter_complete (UINT8 *p)
{
    UINT8         hci_status;
    tBTM_STATUS   status;
    tBTM_INQ_CB  *p_inq = &btm_cb;
    const UINT8  *lap;
    tBTM_CMPL_CB *p_cb = p_inq->p_inqfilter_cmpl_cb;

    /* If the filter complete event is from an old or cancelled request, ignore it */
    if (p_inq->inqfilt_type)
    {
        p_inq->inqfilt_type--;
        return;
    }

    /* Only process the inquiry filter; ignore the connection filter until it is
       used by the upper layers */
    if (p_inq->inqfilt_active == TRUE)
    {
        /* Extract the returned status from the buffer */
        hci_status = *p;
        if (hci_status != HCI_SUCCESS)
        {
            /* If standalone operation, return the error status; if embedded in
               the inquiry, continue the inquiry */
            if (p_inq->trace_level >= BT_TRACE_LEVEL_WARNING)
                LogMsg_1 (TRACE_CTRL_GENERAL | TRACE_LAYER_BTM | TRACE_ORG_STACK | TRACE_TYPE_WARNING,
                          "BTM Warning: Set Event Filter Failed (HCI returned 0x%x)", hci_status);
            status = BTM_ERR_PROCESSING;
        }
        else
            status = BTM_SUCCESS;

        /* If the set filter was initiated externally (via BTM_SetInqEventFilter),
           call the callback function to notify the initiator that it has completed */
        if (!p_inq->pending_filt_complete_event)
        {
            p_inq->inqfilt_active = FALSE;

            if (p_cb)
                (*p_cb)(&status);

            return;
        }

        /* An inquiry is active (the set filter command was internally generated),
           process the next state of the process (set a new filter or start the inquiry) */
        if (status != BTM_SUCCESS)
        {
            /* Process the inquiry complete (error status) */
            btm_process_inq_complete (BTM_ERR_PROCESSING);

            p_inq->inqfilt_active = FALSE;
            p_inq->state = BTM_INQ_INACTIVE_STATE;
            p_inq->pending_filt_complete_event = 0;

            return;
        }

        /* Check to see if a new filter needs to be set up */
        if (p_inq->pending_filt_complete_event == BTM_INQ_CLR_FILT_STATE)
        {
            if ((status = btm_set_inq_event_filter (p_inq->inqparms.filter_cond_type,
                                                    p_inq->inqparms.filter_cond)) == BTM_CMD_STARTED)
            {
                p_inq->pending_filt_complete_event = BTM_INQ_SET_FILT_STATE;
                return;
            }

            /* Error setting the filter: process the inquiry complete */
            p_inq->inqfilt_active = FALSE;
            btm_process_inq_complete (BTM_ERR_PROCESSING);
        }
        else    /* Initiate the inquiry or periodic inquiry */
        {
            p_inq->pending_filt_complete_event = BTM_INQ_ACTIVE_STATE;
            p_inq->inqfilt_active = FALSE;

            /* Make sure the number of responses does not overflow the database */
            p_inq->inqparms.max_resps = (p_inq->inqparms.max_resps <= BTM_INQ_DB_SIZE) ?
                                        p_inq->inqparms.max_resps : BTM_INQ_DB_SIZE;

            /* Use the LAP based on the inquiry mode */
            lap = general_inq_lap;
            if (p_inq->state & BTM_INQ_CLR_FILT_STATE)
                lap = limited_inq_lap;

            if (p_inq->state & BTM_INQ_REMNAME_STATE)
            {
                /* Periodic inquiry mode */
                if (!btsnd_hcic_per_inq_mode (p_inq->per_max_delay, p_inq->per_min_delay, lap,
                                              p_inq->inqparms.duration, p_inq->inqparms.max_resps))
                    btm_process_inq_complete (BTM_NO_RESOURCES);
            }
            else
            {
                /* Clear the BD-addr results filter */
                if (btm_cb.p_bd_db)
                {
                    GKI_freebuf (btm_cb.p_bd_db);
                    btm_cb.p_bd_db = NULL;
                }
                p_inq->num_bd_entries = 0;
                p_inq->max_bd_entries = 0;

                /* Allocate memory to hold the bd-addrs responding */
                p_inq->p_bd_db = GKI_getbuf (BTM_MAX_BD_ENTRIES * sizeof (tINQ_BDADDR));
                if (p_inq->p_bd_db)
                {
                    p_inq->max_bd_entries = BTM_MAX_BD_ENTRIES;
                    memset (p_inq->p_bd_db, 0, BTM_MAX_BD_ENTRIES * sizeof (tINQ_BDADDR));
                }

                if (!btsnd_hcic_inquiry (lap, p_inq->inqparms.duration, 0))
                    btm_process_inq_complete (BTM_NO_RESOURCES);
            }
        }
    }
}

UINT8 btm_set_inq_event_filter (UINT8 filter_type, BD_ADDR bd_addr)
{
    void  *p_buf;
    UINT8  condition_length = DEV_CLASS_LEN * 2;
    UINT8  condition_buf[DEV_CLASS_LEN * 2];
    UINT8 *p_cond = condition_buf;

    p_buf = GKI_getpoolbuf (HCI_CMD_POOL_ID);
    if (p_buf != NULL)
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
void btm_inq_db_init(void) {}

/* ------------------------------------------------------------------ */
/*  btm_inq_find_bdaddr - search the inquiry BD-addr filter table.    */
/* ------------------------------------------------------------------ */
void btm_inq_db_reset (void)
{
    tBTM_INQ_CB        *p_inq = &btm_cb;
    tBTM_INQ_CB        *p_bd;
    UINT16              xx;
    tINQ_DB_ENT        *p_ent;
    tBTM_REMOTE_DEV_NAME rem_name;
    UINT8               num_responses;
    UINT8               temp_state;
    tBTM_STATUS         status;

    /* If an inquiry is active, cancel it */
    p_bd = &btm_cb;
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
    p_bd = &btm_cb;
    if (p_bd->p_bd_db)
    {
        GKI_freebuf (p_bd->p_bd_db);
        p_bd->p_bd_db = NULL;
    }
    p_bd->num_bd_entries = 0;
    p_bd->max_bd_entries = 0;
    p_inq->discoverable_mode = BTM_NON_DISCOVERABLE;
    p_inq->connectable_mode = BTM_NON_CONNECTABLE;
    p_inq->page_scan_type = BTM_SCAN_TYPE_STANDARD;
    p_inq->inq_scan_type = BTM_SCAN_TYPE_STANDARD;
}

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
tBTM_STATUS BTM_ReadRemoteDeviceName (BD_ADDR remote_bda, tBTM_CMPL_CB *p_cb)
{
    tBTM_INQ_INFO *p_cur = NULL;
    tINQ_DB_ENT   *p_ent;

    if (btm_cb.trace_level >= BT_TRACE_LEVEL_API)
        LogMsg_6 (TRACE_CTRL_GENERAL | TRACE_LAYER_BTM | TRACE_ORG_STACK | TRACE_TYPE_API,
                  "BTM_ReadRemoteDeviceName: bd addr [%02x%02x%02x%02x%02x%02x]",
                  remote_bda[0], remote_bda[1], remote_bda[2],
                  remote_bda[3], remote_bda[4], remote_bda[5]);

    /* Use the remote device's clock offset if it is in the local inquiry database */
    p_ent = btm_inq_db_find (&btm_cb, remote_bda);
    if (p_ent)
        p_cur = &p_ent->inq_info;

    return (btm_initiate_rem_name (remote_bda, p_cur, BTM_RMT_NAME_EXT,
                                   BTM_EXT_RMT_NAME_TIMEOUT, p_cb));
}
/* ------------------------------------------------------------------ */
/*  BTM_CancelRemoteDeviceName - cancel a pending remote name request.*/
/* ------------------------------------------------------------------ */
tBTM_STATUS BTM_StartInquiry (tBTM_INQ_PARMS *p_inqparms, tBTM_INQ_RESULTS_CB *p_results_cb,
                              tBTM_CMPL_CB *p_cmpl_cb)
{
    tBTM_INQ_CB *p_inq = &btm_cb;
    tBTM_STATUS  status;

    if (p_inq->trace_level >= BT_TRACE_LEVEL_API)
        LogMsg_4 (TRACE_CTRL_GENERAL | TRACE_LAYER_BTM | TRACE_ORG_STACK | TRACE_TYPE_API,
                  "BTM_StartInquiry: mode: %d, dur: %d, rsps: %d, flt: %d",
                  p_inqparms->mode, p_inqparms->duration,
                  p_inqparms->max_resps, p_inqparms->filter_cond_type);

    /* Only one active inquiry is allowed at a time; also fail if a filter is
       in the process of being updated */
    if ((p_inq->state != BTM_INQ_INACTIVE_STATE) || (p_inq->inqfilt_active))
        return (BTM_BUSY);

    /* If the inquiry mode is invalid, return */
    if ((p_inqparms->mode != BTM_GENERAL_INQUIRY) && (p_inqparms->mode != BTM_LIMITED_INQUIRY))
        return (BTM_ILLEGAL_VALUE);

    /*** Make sure the device is ready ***/
    if (!BTM_IsDeviceUp ())
        return (BTM_WRONG_MODE);

    /* Save the inquiry parameters */
    p_inq->inqparms = *p_inqparms;
    p_inq->pending_filt_complete_event = BTM_INQ_ACTIVE_STATE;
    p_inq->p_inq_cmpl_cb = p_cmpl_cb;
    p_inq->p_inq_results_cb = p_results_cb;
    p_inq->inq_cmpl_info.num_resp = 0;
    p_inq->state = (p_inqparms->mode == BTM_LIMITED_INQUIRY) ?
                   BTM_INQ_CLR_FILT_STATE : BTM_INQ_SET_FILT_STATE;

    switch (p_inqparms->filter_cond_type)
    {
    case BTM_CLR_INQUIRY_FILTER:
        p_inq->pending_filt_complete_event = BTM_INQ_SET_FILT_STATE;
        break;

    case BTM_FILTER_COND_DEVICE_CLASS:
    case BTM_FILTER_COND_BD_ADDR:
        p_inq->pending_filt_complete_event = BTM_INQ_CLR_FILT_STATE;
        p_inqparms->filter_cond_type = BTM_CLR_INQUIRY_FILTER;
        break;

    default:
        return (BTM_ILLEGAL_VALUE);
    }

    /* If the filter is being cleared or set, initiate the command; the inquiry
       itself is started when the filter complete event is processed */
    status = btm_set_inq_event_filter (p_inqparms->filter_cond_type,
                                       p_inqparms->filter_cond);
    if (status != BTM_CMD_STARTED)
        p_inq->pending_filt_complete_event = 0;

    return (status);
}

/* ------------------------------------------------------------------ */
/*  BTM_ReadRemoteDeviceName - initiate a remote device name request.  */
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

UINT16 BTM_IsInquiryActive(void)
{
    return btm_cb.state;
}

/* ------------------------------------------------------------------ */
/*  BTM_CancelInquiry - cancel an inquiry if active.                   */
/* ------------------------------------------------------------------ */
tBTM_STATUS BTM_SetConnectability (UINT16 page_mode, UINT16 window, UINT16 interval)
{
    tBTM_INQ_CB *p_inq = &btm_cb;
    UINT8        scan_mode = 0;
    void        *p_buf;

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
tBTM_STATUS BTM_SetDiscoverability (UINT16 inq_mode, UINT16 window, UINT16 interval)
{
    UINT8        scan_mode = 0;
    UINT16       service_class;
    UINT8       *p_cod;
    UINT8        major, minor;
    DEV_CLASS    cod;
    LAP          temp_lap[2];
    BOOLEAN      is_limited;
    BOOLEAN      cod_limited;
    void        *p_buf;

    /* Check for valid mode */
    if ((inq_mode != BTM_NON_DISCOVERABLE) && (inq_mode != BTM_LIMITED_DISCOVERABLE)
        && (inq_mode != BTM_GENERAL_DISCOVERABLE))
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
                  "BTM_SetDiscoverability: mode %d [NonDisc-0, Lim-1, Gen-2], window 0x%04x, interval 0x%04x",
                  inq_mode, window, interval);

    /* Check for valid window and interval parameters */
    if (inq_mode != BTM_NON_DISCOVERABLE)
    {
        if ((window < BTM_MIN_CONN_WINDOW) || (window > BTM_MAX_CONN_WINDOW) ||
            (interval < BTM_MIN_CONN_INTERVAL) || (interval > BTM_MAX_CONN_INTERVAL) ||
            (window > interval))
        {
            return (BTM_ILLEGAL_VALUE);
        }
    }

    /* Set the IAC if needed */
    if (inq_mode != BTM_NON_DISCOVERABLE)
    {
        if ((p_buf = GKI_getpoolbuf (HCI_CMD_POOL_ID)) != NULL)
        {
            if (inq_mode & BTM_LIMITED_DISCOVERABLE)
            {
                /* Use the GIAC and LIAC codes for limited discoverable mode */
                memcpy (temp_lap[0], limited_inq_lap, LAP_LEN);
                memcpy (temp_lap[1], general_inq_lap, LAP_LEN);
                btsnd_hcic_write_cur_iac_lap (p_buf, 2, (UINT8 *)temp_lap);
            }
            else
            {
                btsnd_hcic_write_cur_iac_lap (p_buf, 1, general_inq_lap);
            }

            scan_mode |= HCI_INQUIRY_SCAN_ENABLED;
        }
        else
            return (BTM_NO_RESOURCES);
    }

    /* Send down the inquiry scan window and period if changed */
    if ((window != btm_cb.inq_scan_window) || (interval != btm_cb.inq_scan_period))
    {
        if ((p_buf = GKI_getpoolbuf (HCI_CMD_POOL_ID)) != NULL)
        {
            btm_cb.inq_scan_window = window;
            btm_cb.inq_scan_period = interval;
            btsnd_hcic_write_inqscan_cfg (p_buf, interval, window);
        }
        else
            return (BTM_NO_RESOURCES);
    }

    /* Keep the inquiry scan as previously set */
    if ((p_buf = GKI_getpoolbuf (HCI_CMD_POOL_ID)) != NULL)
    {
        if (btm_cb.connectable_mode)
            scan_mode |= HCI_PAGE_SCAN_ENABLED;

        btm_cb.discoverable_mode = inq_mode;
        btsnd_hcic_write_scan_enable (p_buf, scan_mode);
    }
    else
        return (BTM_NO_RESOURCES);

    /* Change the service class bit if mode has changed */
    p_cod = BTM_ReadDeviceClass ();
    BTM_COD_SERVICE_CLASS (service_class, p_cod);
    is_limited = (inq_mode & BTM_LIMITED_DISCOVERABLE) ? TRUE : FALSE;
    cod_limited = (service_class & BTM_COD_SERVICE_LMTD_DISCOVER) ? TRUE : FALSE;
    if (is_limited ^ cod_limited)
    {
        BTM_COD_MINOR_CLASS (minor, p_cod);
        BTM_COD_MAJOR_CLASS (major, p_cod);
        if (is_limited)
            service_class |= BTM_COD_SERVICE_LMTD_DISCOVER;
        else
            service_class &= ~BTM_COD_SERVICE_LMTD_DISCOVER;

        FIELDS_TO_COD (cod, minor, major, service_class);
        (void)BTM_SetDeviceClass (cod);
    }

    return (BTM_SUCCESS);
}

/* ------------------------------------------------------------------ */
/*  BTM_SetInquiryScanType - set the inquiry scan type (standard or    */
/*  interlaced).                                                       */
/* ------------------------------------------------------------------ */