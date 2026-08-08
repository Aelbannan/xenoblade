// Decompiled from RVL_SDK/src/revolution/bte/stack/hid/hidd_mgmt
// hidd_proc_repage_timeout - timeout callback for HID device re-paging.

#include <harness_catalog.h>

/* ------------------------------------------------------------------ */
/*  Minimal BTE type definitions (mirroring bt_types.h / gki.h).      */
/* ------------------------------------------------------------------ */
typedef unsigned char   UINT8;
typedef unsigned short  UINT16;
typedef unsigned long   UINT32;
typedef signed   long   INT32;

typedef void   (TIMER_CBACK)(void *p_tle);
#ifndef TIMER_PARAM_TYPE
#define TIMER_PARAM_TYPE UINT32
#endif

typedef struct _tle {
    struct _tle   *p_next;
    struct _tle   *p_prev;
    TIMER_CBACK   *p_cback;
    INT32          ticks;
    TIMER_PARAM_TYPE param;
    UINT16         event;
    UINT8          in_use;
} TIMER_LIST_ENT;

/* ------------------------------------------------------------------ */
/*  External declarations (defined in other TUs of this TU-group).     */
/* ------------------------------------------------------------------ */
extern void LogMsg_0(UINT32 trace_set_mask, const char *p_str);
extern UINT8 hidd_conn_initiate(void);
extern void btu_start_timer(TIMER_LIST_ENT *p_tle, UINT16 type, UINT32 timeout);

/* ------------------------------------------------------------------ */
/*  hd_cb - HID device control block (BSS, 0x144 bytes).             */
/*  Partial layout covering only the fields accessed here.            */
/* ------------------------------------------------------------------ */
typedef void (*tHIDD_APP_CBACK)(UINT32 event, UINT32 data, UINT32 param);

typedef struct {
    UINT8           _pad0[0x08];
    UINT8           conn_state;                      /* +0x08 */
    UINT8           repage_count;                    /* +0x09 */
    UINT8           _pad1[0x1C - 0x0A];
    TIMER_LIST_ENT  repage_timer;                    /* +0x1C */
    UINT8           _pad2[0xC4 - (0x1C + sizeof(TIMER_LIST_ENT))];
    tHIDD_APP_CBACK app_cback;                       /* +0xC4 */
    UINT8           _pad3[0x141 - 0xC8];
    UINT8           trace_level;                     /* +0x141 */
} tHIDD_CB;

/* hd_cb - HID device control block; retail .bss at 0x805BEE70 (0x144
   bytes). Defined here because this TU owns the .bss split; the other
   hidd_* TUs reference it via their own `extern tHIDD_CB hd_cb`. */
tHIDD_CB hd_cb;

/* ------------------------------------------------------------------ */
/*  Trace constants from bt_types.h.                                   */
/* ------------------------------------------------------------------ */
#define TRACE_LAYER_HID      0x001e0000
#define TRACE_TYPE_DEBUG     0x00000004
#define BT_TRACE_LEVEL_DEBUG 5

/******************************************************************************
 *
 * Function:     hidd_proc_repage_timeout
 *
 * Description:  Timeout handler for HID device re-paging. Increments the
 *               retry count and attempts to re-initiate the HID connection.
 *               If the attempt fails and the retry budget (15) is exhausted,
 *               notifies the application via callback (event=1). Otherwise
 *               arms a short timer for another retry. On success notifies
 *               the application (event=2) with the current retry count.
 *
 * Parameters:   none
 *
 * Returns:      void
 *
 ******************************************************************************/
void hidd_proc_repage_timeout(void)
{
    if (hd_cb.trace_level >= BT_TRACE_LEVEL_DEBUG)
    {
        LogMsg_0(TRACE_LAYER_HID | TRACE_TYPE_DEBUG,
                 "hidd_proc_repage_timeout");
    }

    hd_cb.repage_count++;

    if (hidd_conn_initiate() != 0)
    {
        /* Connection re-initiation failed. */
        if (hd_cb.repage_count > 15)
        {
            hd_cb.conn_state = 0;
            hd_cb.app_cback(1, 0, 0);
        }
        else
        {
            btu_start_timer(&hd_cb.repage_timer, 0x16, 1);
        }
    }
    else
    {
        /* Connection re-initiation succeeded (or was already in progress). */
        hd_cb.app_cback(2, hd_cb.repage_count, 0);
    }
}
