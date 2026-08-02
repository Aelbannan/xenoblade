// Decompiled: bta_dm_pm.c - BTA device management power management
// High-level C reconstruction of RVL_SDK/src/revolution/bte/bta/dm/bta_dm_pm

#include <string.h>

#include <revolution/BTE/gki/common/gki.h>
#include <revolution/BTE/bta/include/bd.h>
#include <revolution/BTE/bta/sys/bta_sys.h>
#include <revolution/BTE/stack/include/btm_api.h>

/* DM PM timer event id (BTA_SYS_EVT_START(BTA_ID_DM) + 10) */
#define BTA_DM_PM_TIMER_EVT 0x10A

/* DM PM BTM status event id (BTA_SYS_EVT_START(BTA_ID_DM) + 9) */
#define BTA_DM_PM_BTM_STATUS_EVT 0x109

#define BTA_DM_PM_MODE_TIMER_ID_0   0
#define BTA_DM_PM_MODE_TIMER_ID_1   1
#define BTA_DM_PM_MODE_TIMER_ID_2   2
#define BTA_DM_PM_MODE_TIMER_ID_MAX 3

/* Retail PM action values (bta_api.h defaults are 0x10/0x20/0x40). */
#define BTA_DM_PM_NO_ACTION 0x00
#define BTA_DM_PM_PARK      0x01
#define BTA_DM_PM_SNIFF     0x02
#define BTA_DM_PM_ACTIVE    0x04

#define BTA_DM_PM_SNIFF_IDX 0 /* index into bta_dm_pm_md[] for SNIFF */
#define BTA_DM_PM_PARK_IDX  1 /* index into bta_dm_pm_md[] for PARK */

#define BTA_ALL_APP_ID      0xFF
#define BTA_DM_NUM_PM_TIMER 3

/* bta_dm_pm_cback compares actn_tbl values against the bta_api.h action
   constants (PARK=0x10, SNIFF=0x20, ACTIVE=0x40); bta_dm_pm_set_mode uses
   the reduced 0x01/0x02/0x04 set defined above. */
#define BTA_DM_PM_ACT_PARK 0x10

#define BTA_DM_NUM_CONN_SRVCS 5

struct bta_dm_msg;

/* Peer device entry: 0xB bytes, list at bta_dm_cb+0x0, count at +0x4D. */
typedef struct
{
    BD_ADDR peer_bdaddr;      /* 0x00 */
    UINT8 _pad[3];            /* 0x06 */
    UINT8 pm_mode_attempted;  /* 0x09 */
    UINT8 pm_mode_failed;     /* 0x0A */
} tBTA_DM_PEER_DEVICE;

/* Connected services table: byte 0 = count, 9-byte entries starting at +1,
   service id at +7. Retail bss owner of this symbol is bta_dm_pm
   (referenced by bta_dm_act). */
typedef struct
{
    BD_ADDR peer_bdaddr;      /* 0x01 */
    UINT8 id;                 /* 0x07 */
    UINT8 app_id;             /* 0x08 */
    UINT8 state;              /* 0x09 */
} tBTA_DM_SRVCS;              /* 0x09 */

typedef struct
{
    UINT8 count;              /* 0x00 */
    tBTA_DM_SRVCS conn_srvc[5]; /* 0x01, stride 0x09 */
} tBTA_DM_CONNECTED_SRVCS;    /* 0x2E */

tBTA_DM_CONNECTED_SRVCS bta_dm_conn_srvcs;

/* PM config / spec retail views (bta_dm_cfg.c). */
typedef struct
{
    UINT8 id;
    UINT8 app_id;
    UINT8 spec_idx;
} tBTA_DM_PM_CFG;

typedef struct
{
    UINT8 power_mode;
    UINT8 pad;
    UINT16 timeout;
} tBTA_DM_PM_ACTN;

typedef struct
{
    UINT8 allow_mask;
    UINT8 pad;
    tBTA_DM_PM_ACTN actn_tbl[9][2];
} tBTA_DM_PM_SPEC;

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

/* bta_dm_cb view used by this file (device_list at 0x0, count at 0x4D,
   pm_id at 0x95, pm_timer at 0x98) */
struct bta_dm_cb_pm_t
{
    tBTA_DM_PEER_DEVICE device_list[7]; /* 0x00 (0x0B each) */
    unsigned char device_list_count;    /* 0x4D */
    unsigned char _pad[0x95 - 0x4E];    /* 0x4E-0x94 */
    unsigned char pm_id;                /* 0x95 */
    unsigned char _pad2[2];             /* 0x96-0x97 */
    tBTA_DM_PM_TIMER pm_timer[3];       /* 0x98 (0x20 each) */
};

extern struct bta_dm_cb_pm_t bta_dm_cb;

/* bta_dm_cfg (bta_dm_cfg.c): link_policy at +0x04, retail symbol bta_dm_cfg */
struct bta_dm_cfg_pm_t
{
    UINT8 dev_class[3];   /* 0x00 */
    UINT8 pad;            /* 0x03 */
    UINT16 link_policy;   /* 0x04 */
    UINT16 page_timeout;  /* 0x06 */
    UINT16 link_timeout;  /* 0x08 */
    UINT8 avoid_scatter;  /* 0x0A */
    UINT8 pad2;           /* 0x0B */
};

extern struct bta_dm_cfg_pm_t bta_dm_cfg;

extern tBTA_DM_PM_CFG *p_bta_dm_pm_cfg;
extern tBTA_DM_PM_SPEC *p_bta_dm_pm_spec;
extern tBTM_PM_PWR_MD *p_bta_dm_pm_md;

extern UINT8 appl_trace_level;
extern void LogMsg_0(UINT32 trace_set_mask, const char *fmt_str);
extern void bta_dm_pm_set_mode(BD_ADDR bd_addr, BOOLEAN timed_out);
struct bta_dm_pm_btm_status;
extern void bta_dm_pm_btm_status(struct bta_dm_pm_btm_status *p_data);
extern void bta_dm_pm_timer_cback(void *p_tle);

/* PM BTM-status message: BT_HDR (8 bytes), peer BD address, then the
   BTM power-manager callback payload. */
struct bta_dm_pm_btm_status
{
    unsigned short event;      /* 0x00 */
    unsigned char _pad[6];     /* 0x02-0x07 */
    BD_ADDR bd_addr;           /* 0x08 */
    unsigned char status;      /* 0x0E */
    unsigned short value;      /* 0x10 */
    unsigned char mode;        /* 0x12 */
};                             /* 0x14 */

/* PM timer message: BT_HDR (8 bytes) followed by the peer BD address. */
struct bta_dm_pm_msg
{
    unsigned short event;      /* 0x00 */
    unsigned char _pad[6];     /* 0x02-0x07 */
    BD_ADDR bd_addr;           /* 0x08 */
};

extern void bta_sys_sendmsg(void *p_msg);

void bta_dm_pm_cback(tBTA_SYS_CONN_STATUS status, UINT8 id, UINT8 app_id,
                     BD_ADDR bd_addr);
void bta_dm_pm_btm_cback(BD_ADDR bd_addr, tBTM_PM_STATUS status,
                         UINT16 value, UINT8 mode);

void bta_dm_init_pm(void) {
    memset(&bta_dm_conn_srvcs, 0, sizeof(bta_dm_conn_srvcs));

    /* register PM callbacks when PM entries are configured */
    if (p_bta_dm_pm_cfg->app_id) {
        bta_sys_pm_register(bta_dm_pm_cback);
        BTM_PmRegister(BTM_PM_REG_SET | BTM_PM_REG_NOTIF, &bta_dm_cb.pm_id,
                       bta_dm_pm_btm_cback);
    }
}

void bta_dm_disable_pm() {
    BTM_PmRegister(BTM_PM_DEREG, &bta_dm_cb.pm_id, NULL);
}

void bta_dm_pm_cback(tBTA_SYS_CONN_STATUS status, UINT8 id, UINT8 app_id,
                     BD_ADDR bd_addr) {
    tBTM_VERSION_INFO version;
    UINT8 j;
    UINT8 i;
    UINT8 k;

    /* Broadcom (0x000F) controller with HCI version < 3.0: clear the SCO
       link policy on SCO open/close. */
    if ((UINT8)BTM_ReadLocalVersion(&version) == BTM_SUCCESS &&
        version.manufacturer == 0x000F && version.hci_version < 0x03) {
        UINT16 link_policy;
        if (status == BTA_SYS_SCO_OPEN) {
            link_policy = bta_dm_cfg.link_policy & 0x000B;
            BTM_SetLinkPolicy(bd_addr, &link_policy);
        } else if (status == BTA_SYS_SCO_CLOSE) {
            link_policy = bta_dm_cfg.link_policy;
            BTM_SetLinkPolicy(bd_addr, &link_policy);
        }
    }

    /* find the PM config entry matching this service */
    i = 1;
    while (i <= p_bta_dm_pm_cfg->app_id) {
        if (p_bta_dm_pm_cfg[i].id == id &&
            (p_bta_dm_pm_cfg[i].app_id == BTA_ALL_APP_ID ||
             p_bta_dm_pm_cfg[i].app_id == app_id))
            break;
        i++;
    }
    if (i > p_bta_dm_pm_cfg->app_id)
        return;

    /* stop the PM timer for this device */
    for (j = 0; j < BTA_DM_NUM_PM_TIMER; j++) {
        if (bta_dm_cb.pm_timer[j].in_use != 0 &&
            !bdcmp(bta_dm_cb.pm_timer[j].bd_addr, bd_addr)) {
            bta_sys_stop_timer((TIMER_LIST_ENT *)&bta_dm_cb.pm_timer[j].timer);
            bta_dm_cb.pm_timer[j].in_use = 0;
            break;
        }
    }

    if (p_bta_dm_pm_spec[p_bta_dm_pm_cfg[i].spec_idx]
            .actn_tbl[status][0].power_mode == BTA_DM_PM_NO_ACTION)
        return;

    /* find the service in the connected-services list */
    j = 0;
    while (j < bta_dm_conn_srvcs.count) {
        if (bta_dm_conn_srvcs.conn_srvc[j].id == id &&
            bta_dm_conn_srvcs.conn_srvc[j].app_id == app_id &&
            !bdcmp(bta_dm_conn_srvcs.conn_srvc[j].peer_bdaddr, bd_addr))
            break;
        j++;
    }

    if (p_bta_dm_pm_spec[p_bta_dm_pm_cfg[i].spec_idx]
            .actn_tbl[status][0].power_mode == BTA_DM_PM_ACT_PARK) {
        /* park action: drop the service from the list */
        if (j != bta_dm_conn_srvcs.count) {
            for (; j < bta_dm_conn_srvcs.count; j++) {
                memcpy(&bta_dm_conn_srvcs.conn_srvc[j].peer_bdaddr,
                       &bta_dm_conn_srvcs.conn_srvc[j + 1].peer_bdaddr,
                       sizeof(tBTA_DM_SRVCS));
            }
            bta_dm_conn_srvcs.count--;
        }
    } else {
        /* other actions: add the service if it is not yet listed */
        if (j == bta_dm_conn_srvcs.count) {
            if (bta_dm_conn_srvcs.count == BTA_DM_NUM_CONN_SRVCS) {
                if (appl_trace_level >= 2) {
                    LogMsg_0(0x501,
                             "bta_dm_act no more connected service cbs");
                }
                return;
            }
            bta_dm_conn_srvcs.conn_srvc[j].id = id;
            bta_dm_conn_srvcs.conn_srvc[j].app_id = app_id;
            bdcpy(bta_dm_conn_srvcs.conn_srvc[j].peer_bdaddr, bd_addr);
            bta_dm_conn_srvcs.count++;
        }
    }

    /* clear the failed/attempted power-mode flags for this device */
    for (k = 0; k < bta_dm_cb.device_list_count; k++) {
        if (!bdcmp(bta_dm_cb.device_list[k].peer_bdaddr, bd_addr)) {
            bta_dm_cb.device_list[k].pm_mode_attempted = 0;
            bta_dm_cb.device_list[k].pm_mode_failed = 0;
            break;
        }
    }

    bta_dm_conn_srvcs.conn_srvc[j].state = status;
    bta_dm_pm_set_mode(bd_addr, FALSE);
}

/* ---- bta_dm_pm_set_mode ------------------------------------------------ */
void bta_dm_pm_set_mode(BD_ADDR bd_addr, BOOLEAN timed_out)
{
    UINT8 pm_action = BTA_DM_PM_NO_ACTION;
    UINT16 timeout = 0;
    UINT8 i;
    UINT8 j;
    UINT8 failed_pm = 0;
    tBTA_DM_PEER_DEVICE *p_peer_device = NULL;
    UINT8 allowed_modes = 0;
    UINT8 pref_modes = 0;
    tBTA_DM_PM_CFG *p_pm_cfg;
    tBTA_DM_PM_SPEC *p_pm_spec;

    if (!bta_dm_cb.device_list_count) {
        return;
    }

    /* find the peer device */
    for (i = 0; i < bta_dm_cb.device_list_count; i++) {
        if (!bdcmp(bta_dm_cb.device_list[i].peer_bdaddr, bd_addr)) {
            p_peer_device = &bta_dm_cb.device_list[i];
            failed_pm = p_peer_device->pm_mode_failed;
            break;
        }
    }

    if (p_peer_device == NULL) {
        return;
    }

    /* find all the services for this device and select the action */
    for (i = 0; i < bta_dm_conn_srvcs.count; i++) {
        if (!bdcmp(bta_dm_conn_srvcs.conn_srvc[i].peer_bdaddr, bd_addr)) {
            for (j = 1; j <= p_bta_dm_pm_cfg[0].app_id; j++) {
                if ((p_bta_dm_pm_cfg[j].id == bta_dm_conn_srvcs.conn_srvc[i].id) &&
                    ((p_bta_dm_pm_cfg[j].app_id == BTA_ALL_APP_ID) ||
                     (p_bta_dm_pm_cfg[j].app_id ==
                      bta_dm_conn_srvcs.conn_srvc[i].app_id))) {
                    break;
                }
            }

            p_pm_cfg = &p_bta_dm_pm_cfg[j];
            p_pm_spec = &p_bta_dm_pm_spec[p_pm_cfg->spec_idx];

            allowed_modes |= p_pm_spec->allow_mask;

            if (!(failed_pm &
                  p_pm_spec
                      ->actn_tbl[bta_dm_conn_srvcs.conn_srvc[i].state][0]
                      .power_mode)) {
                pref_modes |=
                    p_pm_spec
                        ->actn_tbl[bta_dm_conn_srvcs.conn_srvc[i].state][0]
                        .power_mode;

                if (p_pm_spec
                        ->actn_tbl[bta_dm_conn_srvcs.conn_srvc[i].state][0]
                        .power_mode > pm_action) {
                    pm_action =
                        p_pm_spec
                            ->actn_tbl[bta_dm_conn_srvcs.conn_srvc[i].state][0]
                            .power_mode;
                    timeout =
                        p_pm_spec
                            ->actn_tbl[bta_dm_conn_srvcs.conn_srvc[i].state][0]
                            .timeout;
                }
            } else if (!(failed_pm &
                         p_pm_spec
                             ->actn_tbl[bta_dm_conn_srvcs.conn_srvc[i].state][1]
                             .power_mode)) {
                pref_modes |=
                    p_pm_spec
                        ->actn_tbl[bta_dm_conn_srvcs.conn_srvc[i].state][1]
                        .power_mode;

                if (p_pm_spec
                        ->actn_tbl[bta_dm_conn_srvcs.conn_srvc[i].state][1]
                        .power_mode > pm_action) {
                    pm_action =
                        p_pm_spec
                            ->actn_tbl[bta_dm_conn_srvcs.conn_srvc[i].state][1]
                            .power_mode;
                    timeout =
                        p_pm_spec
                            ->actn_tbl[bta_dm_conn_srvcs.conn_srvc[i].state][1]
                            .timeout;
                }
            }
        }
    }

    if (pm_action & (BTA_DM_PM_PARK | BTA_DM_PM_SNIFF)) {
        if (!(allowed_modes & pm_action)) {
            pm_action =
                (allowed_modes & (BTA_DM_PM_PARK | BTA_DM_PM_SNIFF) &
                 pref_modes);

            if (pm_action == BTA_DM_PM_NO_ACTION) {
                timeout = 0;
            }
        }
    }

    if (!timed_out && timeout) {
        for (i = 0; i < BTA_DM_NUM_PM_TIMER; i++) {
            if (!bta_dm_cb.pm_timer[i].in_use) {
                bta_dm_cb.pm_timer[i].in_use = TRUE;
                bdcpy(bta_dm_cb.pm_timer[i].bd_addr, bd_addr);
                bta_dm_cb.pm_timer[i].timer.p_cback = bta_dm_pm_timer_cback;
                bta_sys_start_timer((TIMER_LIST_ENT *)&bta_dm_cb.pm_timer[i].timer, 0, timeout);
                return;
            }
        }

        if (i == BTA_DM_NUM_PM_TIMER) {
            if (appl_trace_level >= 2) {
                LogMsg_0(0x501, "bta_dm_act dm_pm_timer no more");
            }
            return;
        }
    }

    if (pm_action == BTA_DM_PM_NO_ACTION) {
    } else if (pm_action == BTA_DM_PM_PARK) {
        tBTM_PM_MODE mode;

        p_peer_device->pm_mode_attempted = BTA_DM_PM_PARK;
        BTM_ReadPowerMode(bd_addr, &mode);
        if (mode != BTM_PM_MD_PARK) {
            BTM_SetPowerMode(bta_dm_cb.pm_id, bd_addr,
                             &p_bta_dm_pm_md[BTA_DM_PM_PARK_IDX]);
        }
    } else if (pm_action == BTA_DM_PM_SNIFF) {
        tBTM_PM_MODE mode;

        p_peer_device->pm_mode_attempted = BTA_DM_PM_SNIFF;
        BTM_ReadPowerMode(bd_addr, &mode);
        if (mode != BTM_PM_MD_SNIFF) {
            BTM_SetPowerMode(bta_dm_cb.pm_id, bd_addr,
                             &p_bta_dm_pm_md[BTA_DM_PM_SNIFF_IDX]);
        }
    } else if (pm_action == BTA_DM_PM_ACTIVE) {
        tBTM_PM_PWR_MD pm;

        pm.mode = BTM_PM_MD_ACTIVE;
        BTM_SetPowerMode(bta_dm_cb.pm_id, bd_addr, &pm);
    }
}

void bta_dm_pm_btm_cback(BD_ADDR bd_addr, tBTM_PM_STATUS status,
                         UINT16 value, UINT8 mode) {
    struct bta_dm_pm_btm_status *p_buf;

    if ((p_buf = (struct bta_dm_pm_btm_status *)GKI_getbuf(
             sizeof(struct bta_dm_pm_btm_status))) != NULL) {
        p_buf->event = BTA_DM_PM_BTM_STATUS_EVT;
        p_buf->status = status;
        p_buf->value = value;
        p_buf->mode = mode;
        bdcpy(p_buf->bd_addr, bd_addr);
        bta_sys_sendmsg(p_buf);
    }
}

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

void bta_dm_pm_btm_status(struct bta_dm_pm_btm_status *p_status)
{
    tBTA_DM_PEER_DEVICE *p_dev;
    UINT8 i;

    /* stop the PM timer for this device if one is running */
    for (i = 0; i < BTA_DM_NUM_PM_TIMER; i++) {
        if (bta_dm_cb.pm_timer[i].in_use &&
            !bdcmp(bta_dm_cb.pm_timer[i].bd_addr, p_status->bd_addr)) {
            bta_sys_stop_timer((TIMER_LIST_ENT *)&bta_dm_cb.pm_timer[i].timer);
            bta_dm_cb.pm_timer[i].in_use = FALSE;
            break;
        }
    }

    switch (p_status->status) {
    case BTM_PM_STS_ACTIVE:
        if (p_status->mode != 0) {
            for (i = 0; i < bta_dm_cb.device_list_count; i++) {
                if (!bdcmp(bta_dm_cb.device_list[i].peer_bdaddr,
                           p_status->bd_addr)) {
                    p_dev = &bta_dm_cb.device_list[i];
                    if (p_dev->pm_mode_attempted &
                        (BTA_DM_PM_PARK | BTA_DM_PM_SNIFF)) {
                        p_dev->pm_mode_failed |=
                            (BTA_DM_PM_PARK | BTA_DM_PM_SNIFF) &
                            p_dev->pm_mode_attempted;
                        bta_dm_pm_set_mode(p_status->bd_addr, FALSE);
                    }
                    break;
                }
            }
        } else {
            bta_dm_pm_set_mode(p_status->bd_addr, FALSE);
        }
        break;
    default:
        break;
    }
}

void bta_dm_pm_timer(struct bta_dm_msg *p_data) {
    struct bta_dm_pm_msg *msg = (struct bta_dm_pm_msg *)p_data;

    bta_dm_pm_set_mode(msg->bd_addr, 1);
}
