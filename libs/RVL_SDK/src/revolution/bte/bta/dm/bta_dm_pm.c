// Decompiled: bta_dm_pm.c - BTA device management power management
// High-level C reconstruction of RVL_SDK/src/revolution/bte/bta/dm/bta_dm_pm

#include <string.h>

#include <revolution/BTE/gki/common/gki.h>
#include <revolution/BTE/bta/include/bd.h>

/* DM PM timer event id (BTA_SYS_EVT_START(BTA_ID_DM) + 10) */
#define BTA_DM_PM_TIMER_EVT 0x10A

#define BTA_DM_PM_MODE_TIMER_ID_0   0
#define BTA_DM_PM_MODE_TIMER_ID_1   1
#define BTA_DM_PM_MODE_TIMER_ID_2   2
#define BTA_DM_PM_MODE_TIMER_ID_MAX 3

struct bta_dm_msg;

/* Connected services table: byte 0 = count, 9-byte entries, service id at +7.
   Retail bss owner of this symbol is bta_dm_pm (referenced by bta_dm_act). */
unsigned char bta_dm_conn_srvcs[0x2e];

/* Minimal timer list entry matching the GKI TIMER_LIST_ENT layout (0x18 bytes) */
typedef struct
{
    void *p_prev;              /* 0x00 */
    void *p_next;              /* 0x04 */
    void *p_cback;             /* 0x08 */
    int ticks;                 /* 0x0C */
    int start_time;            /* 0x10 */
    unsigned short type;       /* 0x14 */
    unsigned char in_use;      /* 0x16 */
    unsigned char pad;         /* 0x17 */
} tBTA_DM_TIMER;

/* PM timer entry: 0x20 bytes, in_use at +0x1E */
typedef struct
{
    tBTA_DM_TIMER timer;       /* 0x00 */
    BD_ADDR bd_addr;           /* 0x18 */
    unsigned char in_use;      /* 0x1E */
    unsigned char pad;         /* 0x1F */
} tBTA_DM_PM_TIMER;

/* bta_dm_cb view used by this file (pm_timer at 0x98) */
struct bta_dm_cb_pm_t
{
    unsigned char _pad[0x98];           /* 0x00-0x97 */
    tBTA_DM_PM_TIMER pm_timer[3];       /* 0x98 (0x20 each) */
};

extern struct bta_dm_cb_pm_t bta_dm_cb;

/* PM timer message: BT_HDR (8 bytes) followed by the peer BD address. */
struct bta_dm_pm_msg
{
    unsigned short event;      /* 0x00 */
    unsigned char _pad[6];     /* 0x02-0x07 */
    BD_ADDR bd_addr;           /* 0x08 */
};

extern void bta_sys_sendmsg(void *p_msg);

void bta_dm_init_pm() {}

void bta_dm_disable_pm() {}

void bta_dm_pm_cback() {}

#pragma push
#pragma auto_inline off
void bta_dm_pm_set_mode(unsigned char *bd_addr, int timed_out) {}
#pragma pop

void bta_dm_pm_btm_cback() {}

void bta_dm_pm_timer_cback(void *p_tle) {
    struct bta_dm_pm_msg *p_msg;
    unsigned char timer_id = BTA_DM_PM_MODE_TIMER_ID_0;
    struct bta_dm_cb_pm_t *p_cb = &bta_dm_cb;

    if (p_cb->pm_timer[BTA_DM_PM_MODE_TIMER_ID_0].in_use &&
        (void *)&p_cb->pm_timer[BTA_DM_PM_MODE_TIMER_ID_0] == p_tle) {
        p_cb->pm_timer[BTA_DM_PM_MODE_TIMER_ID_0].in_use = FALSE;
    } else {
        timer_id = BTA_DM_PM_MODE_TIMER_ID_1;
        if (p_cb->pm_timer[BTA_DM_PM_MODE_TIMER_ID_1].in_use &&
            (void *)&p_cb->pm_timer[BTA_DM_PM_MODE_TIMER_ID_1] == p_tle) {
            p_cb->pm_timer[BTA_DM_PM_MODE_TIMER_ID_1].in_use = FALSE;
        } else {
            timer_id = BTA_DM_PM_MODE_TIMER_ID_2;
            if (p_cb->pm_timer[BTA_DM_PM_MODE_TIMER_ID_2].in_use &&
                (void *)&p_cb->pm_timer[BTA_DM_PM_MODE_TIMER_ID_2] == p_tle) {
                p_cb->pm_timer[BTA_DM_PM_MODE_TIMER_ID_2].in_use = FALSE;
            } else {
                timer_id = BTA_DM_PM_MODE_TIMER_ID_MAX;
            }
        }
    }

    if (timer_id != BTA_DM_PM_MODE_TIMER_ID_MAX) {
        if ((p_msg = (struct bta_dm_pm_msg *)GKI_getbuf(sizeof(struct bta_dm_pm_msg))) != NULL) {
            p_msg->event = BTA_DM_PM_TIMER_EVT;
            bdcpy(p_msg->bd_addr, bta_dm_cb.pm_timer[timer_id].bd_addr);
            bta_sys_sendmsg(p_msg);
        }
    }
}

void bta_dm_pm_btm_status() {}

void bta_dm_pm_timer(struct bta_dm_msg *p_data) {
    struct bta_dm_pm_msg *msg = (struct bta_dm_pm_msg *)p_data;

    bta_dm_pm_set_mode(msg->bd_addr, 1);
}
