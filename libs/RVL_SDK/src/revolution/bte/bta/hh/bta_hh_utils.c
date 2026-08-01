// Decompiled: bta_hh_utils.c - BTA HID Host utility functions
// High-level C reconstruction of RVL_SDK/src/revolution/bte/bta/hh/bta_hh_utils

#include <string.h>

#include <revolution/BTE/gki/platform/data_types.h>
#include <revolution/BTE/stack/include/bt_types.h>
#include <revolution/BTE/bta/include/bd.h>
#include <revolution/BTE/bta/include/bta_hh_api.h>
#include <revolution/BTE/stack/include/hiddefs.h>
#include <revolution/BTE/include/bt_trace.h>
#include <revolution/BTE/gki/common/gki.h>

/* NOTE: The retail binary was compiled with BTA_HH_MAX_KNOWN = 16 (not the
 *       header default of 7).  We override it here to match.
 *       Must come after headers to avoid redefinition error. */
#undef BTA_HH_MAX_KNOWN
#define BTA_HH_MAX_KNOWN    16

/* Internal constants not in public headers */
#ifndef BTA_HH_INVALID_HANDLE
#define BTA_HH_INVALID_HANDLE   0xff
#endif

enum { BTA_HH_NULL_ST, BTA_HH_IDLE_ST, BTA_HH_W4_CONN_ST, BTA_HH_CONN_ST };

/* ------------------------------------------------------------------ */
/*  Internal types -- not in public SDK headers                        */
/* ------------------------------------------------------------------ */

/* Keyboard parsing control block (at offset 0 of bta_hh_cb) */
typedef struct
{
    BOOLEAN mod_key[4];
    BOOLEAN num_lock;
    BOOLEAN caps_lock;
    UINT8   last_report[8];
} tBTA_HH_KB_CB;

/* HID device control block (sizeof = 0x20, stride = 0x20 in kdev[]) */
typedef struct
{
    tHID_DEV_DSCP_INFO  dscp_info;   /* 0x00: dl_len(2) + pad(2) + dsc_list(4) = 8 */
    BD_ADDR             addr;        /* 0x08: 6 bytes                        */
    UINT16              attr_mask;   /* 0x0E                                 */
    UINT16              w4_evt;      /* 0x10                                 */
    UINT8               index;       /* 0x12                                 */
    UINT8               sub_class;   /* 0x13                                 */
    UINT8               sec_mask;    /* 0x14                                 */
    UINT8               app_id;      /* 0x15                                 */
    UINT8               hid_handle;  /* 0x16                                 */
    BOOLEAN             vp;          /* 0x17                                 */
    BOOLEAN             in_use;      /* 0x18                                 */
    BOOLEAN             incoming_conn; /* 0x19                              */
    UINT8               mode;        /* 0x1A   tBTA_HH_PROTO_MODE           */
    UINT8               pad1;        /* 0x1B                                 */
    UINT8               state;       /* 0x1C   tBTA_HH_STATE                */
    UINT8               pad2[3];     /* 0x1D-0x1F padding                   */
} tBTA_HH_DEV_CB;

/* Main BTA HH control block -- matches retail memory layout exactly.
   The fields are ORDERED as they appear in the retail binary:
   kb_cb first (offset 0, 0x10 bytes), then kdev[] at 0x10, then
   pointer/flag fields.  (Retail accesses kdev[i] via &bta_hh_cb + i*0x20
   + 0x10, so the array is NOT at offset 0.) */
typedef struct
{
    tBTA_HH_KB_CB    kb_cb;                   /* 0x000 - 0x00F            */
    tBTA_HH_DEV_CB   kdev[BTA_HH_MAX_KNOWN]; /* 0x010 - 0x20F (16 x 0x20) */
    void            *p_cur;                    /* 0x210                    */
    UINT8            cb_index[BTA_HH_MAX_KNOWN]; /* 0x214 - 0x223        */
    void            *p_cback;                  /* 0x224                    */
    void            *p_disc_db;                /* 0x228                    */
    UINT8            trace_level;              /* 0x22C                    */
    UINT8            cnt_num;                  /* 0x22D                    */
    BOOLEAN          w4_disable;               /* 0x22E                    */
} tBTA_HH_CB;

/* ------------------------------------------------------------------ */
/*  External globals referenced by these functions                     */
/* ------------------------------------------------------------------ */
extern tBTA_HH_CB   bta_hh_cb;
extern tBTA_HH_CFG *p_bta_hh_cfg;
extern void utl_freebuf(void **p);

/* ------------------------------------------------------------------ */
/*  bta_hh_find_cb - look up BD address in kdev[], or find free slot   */
/* ------------------------------------------------------------------ */
UINT8 bta_hh_find_cb(BD_ADDR bda)
{
    UINT8 i;

    /* Phase 1: try to match an existing, non-null address */
    for (i = 0; i < BTA_HH_MAX_KNOWN; i++) {
        if (bdcmp(bda, bta_hh_cb.kdev[i].addr) == 0) {
            if (bdcmp(bda, bd_addr_null) != 0) {
                if (appl_trace_level >= 5)
                    LogMsg_2(0x504, "found kdev_cb[%d] hid_handle = %d ",
                             i, bta_hh_cb.kdev[i].hid_handle);
                return i;
            }
        }
        if (appl_trace_level >= 5)
            LogMsg_4(0x504, "in_use ? [%d] kdev[%d].hid_handle = %d state = [%d]",
                     bta_hh_cb.kdev[i].in_use, i,
                     bta_hh_cb.kdev[i].hid_handle,
                     bta_hh_cb.kdev[i].state);
    }

    /* Phase 2: find first unused slot (counted loop to match retail mtctr/bdnz) */
    i = 0;
    while (i < BTA_HH_MAX_KNOWN) {
        if (!bta_hh_cb.kdev[i].in_use) {
            bdcpy(bta_hh_cb.kdev[i].addr, bda);
            break;
        }
        i++;
    }

    if (appl_trace_level >= 5)
        LogMsg_2(0x504, "bta_hh_find_cb:: index = %d while max = %d",
                 i, BTA_HH_MAX_KNOWN);

    return i;
}

/* ------------------------------------------------------------------ */
/*  bta_hh_clean_up_kdev - reset a device control block                */
/* ------------------------------------------------------------------ */
void bta_hh_clean_up_kdev(tBTA_HH_DEV_CB *p_cb)
{
    UINT8 saved_index;

    if (p_cb->hid_handle != BTA_HH_INVALID_HANDLE)
        bta_hh_cb.cb_index[p_cb->hid_handle] = BTA_HH_MAX_KNOWN;

    saved_index = p_cb->index;

    utl_freebuf((void **)&p_cb->dscp_info.dsc_list);

    memset(p_cb, 0, sizeof(*p_cb));

    p_cb->index      = saved_index;
    p_cb->state      = BTA_HH_IDLE_ST;
    p_cb->hid_handle = BTA_HH_INVALID_HANDLE;
}

/* ------------------------------------------------------------------ */
/*  bta_hh_add_device_to_list - populate a device control block        */
/* ------------------------------------------------------------------ */
void bta_hh_add_device_to_list(tBTA_HH_DEV_CB   *p_cb,
                               UINT8             handle,
                               UINT16            attr_mask,
                               tHID_DEV_DSCP_INFO *p_dscp_info,
                               UINT8             sub_class,
                               UINT8             app_id)
{
    if (appl_trace_level >= 5)
        LogMsg_1(0x504, "subclass = 0x%2x", sub_class);

    p_cb->hid_handle = handle;
    p_cb->in_use     = TRUE;
    p_cb->attr_mask  = attr_mask;
    p_cb->sub_class  = sub_class;
    p_cb->app_id     = app_id;

    if (p_dscp_info) {
        utl_freebuf((void **)&p_cb->dscp_info.dsc_list);

        if (p_dscp_info->dl_len != 0) {
            p_cb->dscp_info.dsc_list = (UINT8 *)GKI_getbuf(p_dscp_info->dl_len);
            if (p_cb->dscp_info.dsc_list != NULL) {
                p_cb->dscp_info.dl_len = p_dscp_info->dl_len;
                memcpy(p_cb->dscp_info.dsc_list,
                       p_dscp_info->dsc_list,
                       p_dscp_info->dl_len);
            }
        }
    }
}

/* ------------------------------------------------------------------ */
/*  bta_hh_tod_spt - check whether a device subclass's TOD is in the  */
/*                   supported-device-type list                        */
/* ------------------------------------------------------------------ */
BOOLEAN bta_hh_tod_spt(tBTA_HH_DEV_CB *p_cb, UINT8 sub_class)
{
    UINT8 xx;
    UINT8 cod = sub_class >> 2;

    for (xx = 0; xx < p_bta_hh_cfg->max_devt_spt; xx++) {
        if (cod == p_bta_hh_cfg->p_devt_list[xx].tod) {
            p_cb->app_id = p_bta_hh_cfg->p_devt_list[xx].app_id;
            return TRUE;
        }
    }
    return FALSE;
}

/* ------------------------------------------------------------------ */
/*  bta_hh_trace_dev_db - dump the entire device DB to trace log      */
/* ------------------------------------------------------------------ */
void bta_hh_trace_dev_db(void)
{
    UINT8 xx;

    if (appl_trace_level >= 5)
        LogMsg_0(0x504,
                 "bta_hh_trace_dev_db:: Device DB list********************");

    for (xx = 0; xx < BTA_HH_MAX_KNOWN; xx++) {
        if (appl_trace_level >= 5)
            LogMsg_3(0x504, "kdev[%d] in_use[%d]  handle[%d] ",
                     xx, bta_hh_cb.kdev[xx].in_use,
                     bta_hh_cb.kdev[xx].hid_handle);

        if (appl_trace_level >= 5)
            LogMsg_4(0x504,
                     "\t\t\t attr_mask[%04x] state [%d] sub_class[%02x] index = %d",
                     bta_hh_cb.kdev[xx].attr_mask,
                     bta_hh_cb.kdev[xx].state,
                     bta_hh_cb.kdev[xx].sub_class,
                     bta_hh_cb.kdev[xx].index);
    }

    if (appl_trace_level >= 5)
        LogMsg_0(0x504,
                 "*********************************************************");
}
