// Decompiled btm_dev.c - high-level C reconstruction
// Retail struct layout: sec_dev_rec entries are 0x88 bytes,
// BTM_SEC_MAX_DEVICE_RECORDS = 16, BTM_MAX_REM_BD_NAME_LEN = 64,
// BTM_SEC_SERVICE_ARRAY_SIZE = 2 (BTM_SEC_MAX_SERVICES <= 64).

#include <string.h>
#include "revolution/BTE/stack/include/bt_types.h"
#include "revolution/BTE/stack/include/btm_api.h"

/* ------------------------------------------------------------------ */
/*  Local struct matching the retail binary layout exactly.           */
/*  btm_int.h's tBTM_SEC_DEV_REC has wrong sizes for the Wii binary. */
/* ------------------------------------------------------------------ */
typedef struct
{
    void   *p_cur_service;       /* 0x00 */
    void   *p_callback;          /* 0x04 */
    void   *p_ref_data;          /* 0x08 */
    u32     timestamp;           /* 0x0C */
    u32     trusted_mask[2];     /* 0x10 - BTM_SEC_SERVICE_ARRAY_SIZE = 2 */
    u16     hci_handle;          /* 0x18 */
    u16     clock_offset;        /* 0x1A */
    u8      bd_addr[6];          /* 0x1C - BD_ADDR */
    u8      dev_class[3];        /* 0x22 - DEV_CLASS */
    u8      link_key[16];        /* 0x25 - LINK_KEY */
    char    sec_bd_name[65];     /* 0x35 - BTM_MAX_REM_BD_NAME_LEN = 64 + 1 */
    u8      sec_flags;           /* 0x76 */
    u8      features[8];         /* 0x77 - BD_FEATURES */
    u8      sec_state;           /* 0x7F */
    u8      is_originator;       /* 0x80 */
    u8      role_master;         /* 0x81 */
    u16     security_required;   /* 0x82 */
    u8      link_key_not_sent;   /* 0x84 */
    u8      link_key_type;       /* 0x85 */
    u8      _pad[2];             /* 0x86-0x87 */
} BtmSecDevRec;                  /* total 0x88 */

/* ------------------------------------------------------------------ */
/*  Local btm_cb surrogate - only the fields we touch.                */
/*  The real symbol is declared elsewhere; we supply our own layout.   */
/* ------------------------------------------------------------------ */
typedef struct
{
    u8      _pad0[0x1974];               /* 0x0000-0x1973 */
    u32     dev_rec_count;                /* 0x1974 */
    u8      _pad1[0x1f30 - 0x1978];       /* 0x1978-0x1F2F */
    BtmSecDevRec sec_dev_rec[16];        /* 0x1F30-0x27AF (16 * 0x88 = 0x880) */
    u8      _pad2[0x27ba - 0x27b0];       /* 0x27B0-0x27B9 (p_out_serv, mkey_cback, connecting_bda) */
    u8      default_dev_class[3];         /* 0x27BA - DEV_CLASS fallback */
} BtmCb;

/* The real global */
extern BtmCb btm_cb;

/* ------------------------------------------------------------------ */
/*  Flag bit definitions (from btm_int.h, kept inline)                */
/* ------------------------------------------------------------------ */
#define BTM_SEC_IN_USE         0x80
#define BTM_SEC_NAME_KNOWN     0x08
#define BTM_SEC_LINK_KEY_KNOWN 0x10

/* ------------------------------------------------------------------ */
/*  Forward declarations for internal linkage */
static __inline BtmSecDevRec *btm_sec_find_dev_bda(BD_ADDR bd_addr);
BtmSecDevRec *btm_sec_alloc_dev(BD_ADDR bd_addr);
BtmSecDevRec *btm_find_oldest_dev(void);

/* ------------------------------------------------------------------ */
/*  BTM_SecAddDevice - Add/modify a device in the security database.  */
/* ------------------------------------------------------------------ */
BOOLEAN BTM_SecAddDevice(BD_ADDR bd_addr, DEV_CLASS dev_class,
                         BD_NAME bd_name, BD_FEATURES features,
                         UINT32 trusted_mask[], LINK_KEY link_key,
                         UINT8 key_type, tBTM_IO_CAP io_cap)
{
    BtmSecDevRec *p_rec;

    /* Search for an existing record with this BD address */
    p_rec = btm_sec_find_dev_bda(bd_addr);

    if (p_rec == NULL)
    {
        /* Not found - allocate a new device record */
        p_rec = (BtmSecDevRec *)btm_sec_alloc_dev(bd_addr);
        if (p_rec == NULL)
            return FALSE;
    }
    else
    {
        /* Found existing - assign a fresh timestamp */
        p_rec->timestamp = btm_cb.dev_rec_count++;
    }

    /* Copy device class if provided */
    if (dev_class != NULL)
        memcpy(p_rec->dev_class, dev_class, 3);

    /* Clear the name field, then optionally set it */
    memset(p_rec->sec_bd_name, 0, 0x41);
    if (bd_name != NULL && bd_name[0] != 0)
    {
        p_rec->sec_flags |= BTM_SEC_NAME_KNOWN;
        strncpy(p_rec->sec_bd_name, (const char *)bd_name, 0x40);
    }

    /* Copy features if provided */
    if (features != NULL)
        memcpy(p_rec->features, features, 8);
    else
        memset(p_rec->features, 0, 8);

    /* Copy trusted service mask (2 words) */
    p_rec->trusted_mask[0] = trusted_mask[0];
    p_rec->trusted_mask[1] = trusted_mask[1];

    /* Copy link key if provided */
    if (link_key != NULL)
    {
        p_rec->sec_flags |= BTM_SEC_LINK_KEY_KNOWN;
        memcpy(p_rec->link_key, link_key, 16);
    }

    return TRUE;
}

/* ------------------------------------------------------------------ */
/*  BTM_SecDeleteDevice - Remove a device from the security database. */
/* ------------------------------------------------------------------ */
static __inline BtmSecDevRec *btm_sec_find_dev_bda(BD_ADDR bd_addr)
{
    BtmSecDevRec *p_rec;
    int i;

    p_rec = btm_cb.sec_dev_rec;
    for (i = 0; i < 16; i++)
    {
        if ((p_rec->sec_flags & BTM_SEC_IN_USE) &&
            memcmp(p_rec->bd_addr, bd_addr, 6) == 0)
        {
            return p_rec;
        }
        p_rec++;
    }

    return NULL;
}

BOOLEAN BTM_SecDeleteDevice(BD_ADDR bd_addr)
{
    BtmSecDevRec *p_rec;

    p_rec = btm_sec_find_dev_bda(bd_addr);

    if (p_rec == NULL)
        return FALSE;

    p_rec->sec_flags = 0;
    BTM_DeleteStoredLinkKey(bd_addr, NULL);
    return TRUE;
}

/* ------------------------------------------------------------------ */
/*  BTM_SecReadDevName - Look up the device name for a BD address.    */
/*  Returns pointer to the name string or NULL if not found.          */
/* ------------------------------------------------------------------ */
BtmSecDevRec *btm_find_dev(BD_ADDR bd_addr);

char *BTM_SecReadDevName(BD_ADDR bd_addr)
{
    char *p_name = NULL;
    BtmSecDevRec *p_srec;

    if ((p_srec = btm_find_dev(bd_addr)) != NULL)
        p_name = (char *)p_srec->sec_bd_name;

    return p_name;
}

/* ------------------------------------------------------------------ */
/*  btm_sec_alloc_dev - Allocate a new device record, zero it out,    */
/*  initialise fields, and look up inquiry data for the BD address.   */
/* ------------------------------------------------------------------ */
BtmSecDevRec *btm_sec_alloc_dev(BD_ADDR bd_addr)
{
    BtmSecDevRec *p_rec;
    int i;
    tBTM_INQ_INFO *p_inq;

    p_rec = NULL;

    /* Linear search for a free slot (sec_flags top bit clear) */
    for (i = 0; i < 16; i++)
    {
        if (!(btm_cb.sec_dev_rec[i].sec_flags & BTM_SEC_IN_USE))
        {
            p_rec = &btm_cb.sec_dev_rec[i];
            break;
        }
    }

    /* If no free slot, evict the oldest in-use device */
    if (p_rec == NULL)
        p_rec = (BtmSecDevRec *)btm_find_oldest_dev();

    /* Zero the entire record */
    memset(p_rec, 0, sizeof(BtmSecDevRec));

    /* Mark as in-use and clear link-key-type */
    p_rec->sec_flags = BTM_SEC_IN_USE;
    p_rec->link_key_type = 0;

    /* Read inquiry results; use default dev_class as fallback */
    p_inq = (tBTM_INQ_INFO *)BTM_InqDbRead(bd_addr);
    if (p_inq != NULL)
        memcpy(p_rec->dev_class, p_inq->results.dev_class, 3);
    else
        memcpy(p_rec->dev_class, (void *)btm_cb.default_dev_class, 3);

    /* Copy BD address */
    memcpy(p_rec->bd_addr, bd_addr, 6);

    /* Set connection handle and timestamp */
    p_rec->hci_handle = BTM_GetHCIConnHandle(bd_addr);
    p_rec->timestamp = btm_cb.dev_rec_count++;

    return p_rec;
}

/* ------------------------------------------------------------------ */
/*  btm_find_dev_by_handle - Locate a device record by HCI handle.    */
/* ------------------------------------------------------------------ */
BtmSecDevRec *btm_find_dev_by_handle(UINT16 handle)
{
    BtmSecDevRec *p_rec;
    int i;

    p_rec = btm_cb.sec_dev_rec;
    for (i = 0; i < 16; i++, p_rec++)
    {
        if ((p_rec->sec_flags & BTM_SEC_IN_USE) &&
            p_rec->hci_handle == handle)
        {
            return p_rec;
        }
    }

    return NULL;
}

/* ------------------------------------------------------------------ */
/*  btm_find_dev - Locate a device record by BD address.              */
/* ------------------------------------------------------------------ */
BtmSecDevRec *btm_find_dev(BD_ADDR bd_addr)
{
    BtmSecDevRec *p_rec;
    int i;

    p_rec = btm_cb.sec_dev_rec;
    for (i = 0; i < 16; i++)
    {
        if ((p_rec->sec_flags & BTM_SEC_IN_USE) &&
            memcmp(p_rec->bd_addr, bd_addr, 6) == 0)
        {
            return p_rec;
        }
        p_rec++;
    }

    return NULL;
}

/* ------------------------------------------------------------------ */
/*  btm_find_oldest_dev - Return the device record with the oldest    */
/*  timestamp that is still in use and not marked as keep.            */
/*  Fallback: if all candidates are marked keep, pick the oldest      */
/*  in-use record regardless of the keep flag.                        */
/* ------------------------------------------------------------------ */
BtmSecDevRec *btm_find_oldest_dev(void)
{
    BtmSecDevRec *p_rec;
    BtmSecDevRec *p_oldest;
    u32 oldest_ts;
    int i;

    p_rec    = btm_cb.sec_dev_rec;
    p_oldest = btm_cb.sec_dev_rec;
    oldest_ts = 0xFFFFFFFF;

    for (i = 0; i < 16; i++, p_rec++)
    {
        if ((p_rec->sec_flags & BTM_SEC_IN_USE) &&
            !(p_rec->sec_flags & BTM_SEC_LINK_KEY_KNOWN) &&
            p_rec->timestamp < oldest_ts)
        {
            p_oldest  = p_rec;
            oldest_ts = p_rec->timestamp;
        }
    }

    if ((oldest_ts + 0x10000) != 0xFFFF)
        return p_oldest;

    /* Second pass: include keep-flagged devices */
    p_rec = btm_cb.sec_dev_rec;
    for (i = 0; i < 16; i++, p_rec++)
    {
        if ((p_rec->sec_flags & BTM_SEC_IN_USE) &&
            p_rec->timestamp < oldest_ts)
        {
            p_oldest  = p_rec;
            oldest_ts = p_rec->timestamp;
        }
    }

    return p_oldest;
}
