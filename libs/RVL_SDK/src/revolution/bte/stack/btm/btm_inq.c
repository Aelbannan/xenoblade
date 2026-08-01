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
#define BTM_BAD_VALUE_RET    9
#define BTM_WRONG_MODE       6

#define BTM_INQ_DB_SIZE      12
#define BTM_RMT_NAME_INACTIVE 0
#define BTM_RMT_NAME_EXT     0x01

#define BTM_INQ_INACTIVE_STATE  0
#define BTM_INQ_REMNAME_STATE   4

#define BT_TRACE_LEVEL_API   3

#define HCI_SUCCESS          0x00
#define HCI_ERR_UNSPECIFIED  0x1F

/* ------------------------------------------------------------------ */
/*  Callback / log types                                              */
/* ------------------------------------------------------------------ */
typedef void (tBTM_CMPL_CB)(void *p1);
typedef void (tBTM_INQ_DB_CHANGE_CB)(void *p_inq_info, BOOLEAN is_new);

extern void LogMsg_0 (UINT32 trace_set_mask, const char *p_str);
extern void LogMsg_6 (UINT32 trace_set_mask, const char *fmt_str, UINT32 p1, UINT32 p2,
                      UINT32 p3, UINT32 p4, UINT32 p5, UINT32 p6);
extern void btu_stop_timer (void *p_tle);
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
    UINT8              _pad0[0x167C];                    /* 0x0000-0x167B */
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
    UINT8              _pad4[0x1844 - 0x16E4 -
                             sizeof(tINQ_DB_ENT) * BTM_INQ_DB_SIZE]; /* 0x1834 */
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

void BTM_SetInquiryScanType() {}

void BTM_SetPageScanType() {}

void BTM_SetInquiryMode() {}

void BTM_SetConnectability() {}

/* ------------------------------------------------------------------ */
/*  BTM_IsInquiryActive - return the current inquiry state byte.      */
/* ------------------------------------------------------------------ */
UINT16 BTM_IsInquiryActive(void)
{
    return btm_cb.state;
}

void BTM_CancelInquiry() {}

void BTM_StartInquiry() {}

void BTM_ReadRemoteDeviceName() {}

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

void btm_inq_db_reset() {}

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

void btm_set_inq_event_filter() {}

void btm_event_filter_complete() {}

void btm_process_inq_results() {}

void btm_process_inq_complete() {}

void btm_initiate_rem_name() {}

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
