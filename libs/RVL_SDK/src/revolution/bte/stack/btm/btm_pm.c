#include <harness_catalog.h>

/******************************************************************************
 *
 *  Copyright (C) 2000-2012 Broadcom Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

/*******************************************************************************
 * headers
 */

/* Retail layout fixes:
 * - BTM_SSR_INCLUDED off ⇒ tBTM_PM_MCB is 0x22 bytes (retail mulli stride).
 * - btm_cb layout in this build differs from retail, so the PM functions below
 *   access it through tBTM_CB_COMPAT, which matches the retail offsets.
 */
#define BTM_MAX_PM_RECORDS 1
#define BTM_SSR_INCLUDED   FALSE
#define MAX_ACL_CONNECTIONS 4

#include <string.h>

#include "revolution/BTE/include/bt_target.h"
#include "revolution/BTE/include/bt_trace.h"
#include "revolution/BTE/stack/include/bt_types.h"
#include "revolution/BTE/gki/platform/data_types.h"

#include "revolution/BTE/stack/btm/btm_int.h"
#include "revolution/BTE/gki/common/gki.h"
#include "revolution/BTE/stack/include/hcidefs.h"

/* Retail build was compiled without BTM_PM_STS_SSR in the status enum, so
 * PENDING/ERROR are 4/5 here instead of the header's 5/6.  The retail values
 * are confirmed in btm_pm_proc_cmd_status (state=4, STS_PENDING=4,
 * STS_ERROR=5) and btm_pm_reset (STS_ERROR=5). */
#undef BTM_PM_ST_PENDING
#undef BTM_PM_STS_PENDING
#undef BTM_PM_STS_ERROR
#define BTM_PM_ST_PENDING 4
#define BTM_PM_STS_PENDING 4
#define BTM_PM_STS_ERROR   5

/*******************************************************************************
 * macros
 */

#define BTM_PM_NUM_SET_MODES 3

/*******************************************************************************
 * types
 */

enum {
    BTM_PM_GET_MD1 = 1,
    BTM_PM_GET_MD2 = 2,
    BTM_PM_GET_COMP = 3,
};

/* Local retail-layout view of the subset of btm_cb used by this unit.
 * The global tBTM_CB layout in our headers does not match the retail binary,
 * so we take &btm_cb and cast to this type when accessing PM/ACL fields.
 * Offsets are taken from the retail code (see btm_pm_reset / btm_pm_sm_alloc).
 */
typedef struct {
    UINT8 _pad0[0x08];
    BD_ADDR remote_addr;                  /* 0x08 */
    UINT8 _pad1[0x119 - 0x08 - BD_ADDR_LEN]; /* 0x0E..0x118 */
    UINT8 in_use;                         /* 0x119 */
    UINT8 _pad2[0x11C - 0x11A];           /* 0x11A..0x11B */
} tACL_CONN_COMPAT;

typedef struct {
    tBTM_PM_STATUS_CBACK* cback;
    UINT8 mask;
    UINT8 _pad[3];
} tBTM_PM_RCB_COMPAT;

typedef struct {
    UINT8 _pad0[0x34];
    tACL_CONN_COMPAT acl_db[4];           /* 0x34 */
    UINT8 _u1[0x4CC - 0x34 - 4 * sizeof(tACL_CONN_COMPAT)];
    tBTM_PM_MCB pm_mode_db[4];            /* 0x4CC */
    tBTM_PM_RCB_COMPAT pm_reg_db[2];      /* 0x554 */
    UINT8 pm_pend_link;                   /* 0x564 */
    UINT8 pm_pend_id;                     /* 0x565 */
} tBTM_CB_COMPAT;

/*******************************************************************************
 * local function declarations
 */

static int btm_pm_find_acl_ind(BD_ADDR remote_bda);
static tBTM_PM_PWR_MD* btm_pm_compare_modes(tBTM_PM_PWR_MD* p_md1, tBTM_PM_PWR_MD* p_md2, tBTM_PM_PWR_MD* p_res);
static tBTM_PM_MODE btm_pm_get_set_mode(UINT8 pm_id, tBTM_PM_MCB* p_cb, tBTM_PM_PWR_MD* p_mode, tBTM_PM_PWR_MD* p_res);
static tBTM_STATUS btm_pm_snd_md_req(UINT8 pm_id, int link_ind, tBTM_PM_PWR_MD* p_mode);

/*******************************************************************************
 * variables
 */

// .sdata2
UINT8 const btm_pm_mode_off[BTM_PM_NUM_SET_MODES] = {0, 0, 1};

UINT8 const btm_pm_mode_msk[BTM_PM_NUM_SET_MODES] = {0x40, 0x80, 0x01};

// .rodata
UINT8 const btm_pm_md_comp_matrix[BTM_PM_NUM_SET_MODES * BTM_PM_NUM_SET_MODES] = {
    BTM_PM_GET_COMP, BTM_PM_GET_MD2, BTM_PM_GET_MD2,
    BTM_PM_GET_MD1, BTM_PM_GET_COMP, BTM_PM_GET_MD1,
    BTM_PM_GET_MD1, BTM_PM_GET_MD2, BTM_PM_GET_COMP,
};

/*******************************************************************************
 * functions
 */

tBTM_STATUS BTM_PmRegister(UINT8 mask, UINT8* p_pm_id, tBTM_PM_STATUS_CBACK* p_cb) {
    int xx;

    if (mask & BTM_PM_DEREG) {
        if (*p_pm_id >= BTM_MAX_PM_RECORDS) {
            return BTM_ILLEGAL_VALUE;
        } else {
            btm_cb.pm_reg_db[*p_pm_id].mask = BTM_PM_REC_NOT_USED;
            return BTM_SUCCESS;
        }
    }

    for (xx = 0; xx < BTM_MAX_PM_RECORDS; ++xx) {
        if (btm_cb.pm_reg_db[xx].mask == BTM_PM_REC_NOT_USED) {
            if (mask & BTM_PM_REG_NOTIF) {
                if (!p_cb)
                    return BTM_ILLEGAL_VALUE;

                btm_cb.pm_reg_db[xx].cback = p_cb;
            }

            btm_cb.pm_reg_db[xx].mask = mask;
            *p_pm_id = xx;

            return BTM_SUCCESS;
        }
    }

    return BTM_NO_RESOURCES;
}

tBTM_STATUS BTM_SetPowerMode(UINT8 pm_id, BD_ADDR remote_bda, tBTM_PM_PWR_MD* p_mode) {
    UINT8* p_features;
    int ind;
    int acl_ind;
    tBTM_PM_MCB* p_cb = NULL;
    tBTM_PM_MODE mode;

    if (pm_id >= BTM_MAX_PM_RECORDS)
        pm_id = BTM_PM_SET_ONLY_ID;

    if (!p_mode)
        return BTM_ILLEGAL_VALUE;

    mode = p_mode->mode & ~BTM_PM_MD_FORCE;

    acl_ind = btm_pm_find_acl_ind(remote_bda);
    if (acl_ind == MAX_L2CAP_LINKS)
        return BTM_UNKNOWN_ADDR;

    p_cb = &btm_cb.pm_mode_db[acl_ind];

    if (mode != BTM_PM_MD_ACTIVE) {
        ind = mode - BTM_PM_MD_HOLD;
        p_features = BTM_ReadLocalFeatures();

        if (!(p_features[btm_pm_mode_off[ind]] & btm_pm_mode_msk[ind]))
            return BTM_MODE_UNSUPPORTED;
    }

    if (mode == p_cb->state) {
        if (mode == BTM_PM_MD_ACTIVE || (p_mode->max >= p_cb->interval && p_mode->min <= p_cb->interval)) {
            return BTM_SUCCESS;
        }
    }

    if (pm_id != BTM_PM_SET_ONLY_ID && btm_cb.pm_reg_db[pm_id].mask & BTM_PM_REG_SET) {
        p_cb->req_mode[pm_id] = *p_mode;
        p_cb->chg_ind = TRUE;
    }

    if (p_cb->state == BTM_PM_STS_HOLD || p_cb->state == BTM_PM_STS_PENDING || btm_cb.pm_pend_link != MAX_L2CAP_LINKS) {
        return BTM_CMD_STORED;
    }

    return btm_pm_snd_md_req(pm_id, acl_ind, p_mode);
}

tBTM_STATUS BTM_ReadPowerMode(BD_ADDR remote_bda, tBTM_PM_MODE* p_mode) {
    int acl_ind;

    if ((acl_ind = btm_pm_find_acl_ind(remote_bda)) == MAX_L2CAP_LINKS)
        return BTM_UNKNOWN_ADDR;

    *p_mode = ((tBTM_CB_COMPAT*)&btm_cb)->pm_mode_db[acl_ind].state;

    return BTM_SUCCESS;
}

void btm_pm_reset(void) {
    tBTM_PM_STATUS_CBACK* cb = NULL;

    /* Capture the callback for the party that has a pending request. */
    if (((tBTM_CB_COMPAT*)&btm_cb)->pm_pend_id != BTM_PM_SET_ONLY_ID &&
        (((tBTM_CB_COMPAT*)&btm_cb)->pm_reg_db[((tBTM_CB_COMPAT*)&btm_cb)->pm_pend_id].mask &
         BTM_PM_REG_NOTIF)) {
        cb = ((tBTM_CB_COMPAT*)&btm_cb)->pm_reg_db[((tBTM_CB_COMPAT*)&btm_cb)->pm_pend_id].cback;
    }

    /* Clear normal registration slot, reset pending link, then clear temp slot.
     * Retail order/offsets are reproduced exactly. The invalid pending-link
     * value in this build is 4. */
    ((tBTM_CB_COMPAT*)&btm_cb)->pm_reg_db[0].mask = BTM_PM_REC_NOT_USED;
    ((tBTM_CB_COMPAT*)&btm_cb)->pm_pend_link = 4;
    ((tBTM_CB_COMPAT*)&btm_cb)->pm_reg_db[1].mask = BTM_PM_REC_NOT_USED;

    /* Notify that the pending command was aborted due to device reset.
     * Retail uses status value 5 (BTM_PM_STS_ERROR with the no-SSR enum). */
    if (cb) {
        (*cb)(((tBTM_CB_COMPAT*)&btm_cb)->acl_db[((tBTM_CB_COMPAT*)&btm_cb)->pm_pend_link].remote_addr,
              BTM_PM_STS_ERROR, BTM_DEV_RESET, HCI_SUCCESS);
    }
}

void btm_pm_sm_alloc(UINT8 ind) {
    tBTM_PM_MCB* p_db = &((tBTM_CB_COMPAT*)&btm_cb)->pm_mode_db[ind];
    memset(p_db, 0, sizeof(*p_db));
    p_db->state = BTM_PM_ST_ACTIVE;
}

static int btm_pm_find_acl_ind(BD_ADDR remote_bda) {
    tACL_CONN_COMPAT* p = ((tBTM_CB_COMPAT*)&btm_cb)->acl_db;
    UINT8 xx;

    for (xx = 0; xx < MAX_L2CAP_LINKS; xx++, p++) {
        if (p->in_use && memcmp(p->remote_addr, remote_bda, BD_ADDR_LEN) == 0)
            break;
    }

    return xx;
}

static tBTM_PM_PWR_MD* btm_pm_compare_modes(tBTM_PM_PWR_MD* p_md1, tBTM_PM_PWR_MD* p_md2, tBTM_PM_PWR_MD* p_res) {
    UINT8 res;

    if (!p_md1) {
        *p_res = *p_md2;
        return p_md2;
    }

    if (p_md2->mode == BTM_PM_MD_ACTIVE || p_md1->mode == BTM_PM_MD_ACTIVE)
        return NULL;

    if (p_md1->mode & BTM_PM_MD_FORCE) {
        *p_res = *p_md1;
        p_res->mode &= ~BTM_PM_MD_FORCE;
        return p_res;
    }

    if (p_md2->mode & BTM_PM_MD_FORCE) {
        *p_res = *p_md2;
        p_res->mode &= ~BTM_PM_MD_FORCE;
        return p_res;
    }

    res = (p_md1->mode - BTM_PM_MD_HOLD) * BTM_PM_NUM_SET_MODES + (p_md2->mode - BTM_PM_MD_HOLD);
    res = btm_pm_md_comp_matrix[res];

    switch (res) {
    case BTM_PM_GET_MD1:
        *p_res = *p_md1;
        return p_md1;

    case BTM_PM_GET_MD2:
        *p_res = *p_md2;
        return p_md2;

    case BTM_PM_GET_COMP:
        p_res->mode = p_md1->mode;
        p_res->max = p_md1->max < p_md2->max ? p_md1->max : p_md2->max;
        p_res->min = p_md1->min > p_md2->min ? p_md1->min : p_md2->min;

        if (p_res->max < p_res->min)
            return NULL;

        if (p_res->mode == BTM_PM_MD_SNIFF) {
            p_res->attempt = p_md1->attempt > p_md2->attempt ? p_md1->attempt : p_md2->attempt;
            p_res->timeout = p_md1->timeout > p_md2->timeout ? p_md1->timeout : p_md2->timeout;
        }

        return p_res;
    }

    return NULL;
}

static tBTM_PM_MODE btm_pm_get_set_mode(UINT8 pm_id, tBTM_PM_MCB* p_cb, tBTM_PM_PWR_MD* p_mode, tBTM_PM_PWR_MD* p_res) {
    int xx;
    tBTM_PM_PWR_MD* p_md = NULL;

    if (p_mode && p_mode->mode & BTM_PM_MD_FORCE) {
        *p_res = *p_mode;
        p_res->mode &= ~BTM_PM_MD_FORCE;
        return p_res->mode;
    }

    for (xx = 0; xx < BTM_MAX_PM_RECORDS; ++xx) {
        if (btm_cb.pm_reg_db[xx].mask & BTM_PM_REG_SET) {
            if (p_cb->req_mode[xx].mode == BTM_PM_MD_ACTIVE) {
                return BTM_PM_MD_ACTIVE;
            } else {
                if (!btm_pm_compare_modes(p_md, &p_cb->req_mode[xx], p_res))
                    return BTM_PM_MD_ACTIVE;

                p_md = p_res;
            }
        }
    }

    if (!p_md) {
        if (p_mode)
            *p_res = *p_mode;
        else
            return BTM_PM_MD_ACTIVE;
    } else if (pm_id == BTM_PM_SET_ONLY_ID && !btm_pm_compare_modes(p_mode, p_md, p_res)) {
        return BTM_PM_MD_ACTIVE;
    }

    return p_res->mode;
}

static tBTM_STATUS btm_pm_snd_md_req(UINT8 pm_id, int link_ind, tBTM_PM_PWR_MD* p_mode) {
    tBTM_PM_PWR_MD md_res;
    tBTM_PM_MODE mode;
    BT_HDR* p_buf;
    tBTM_PM_MCB* p_cb = &btm_cb.pm_mode_db[link_ind];
    BOOLEAN chg_ind = FALSE;

    mode = btm_pm_get_set_mode(pm_id, p_cb, p_mode, &md_res);
    md_res.mode = mode;

    if (p_cb->state == mode) {
        if (mode == BTM_PM_MD_ACTIVE || (md_res.max >= p_cb->interval && md_res.min <= p_cb->interval)) {
            return BTM_CMD_STORED;
        }

        chg_ind = TRUE;
    }

    p_cb->chg_ind = chg_ind;

    if (mode != BTM_PM_MD_ACTIVE && p_cb->state != BTM_PM_MD_ACTIVE)
        p_cb->chg_ind = TRUE;

    if ((p_buf = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_CMD_HANDLE)) == NULL)
        return BTM_NO_RESOURCES;

    if (p_cb->chg_ind == TRUE)
        md_res.mode = BTM_PM_MD_ACTIVE;

    btm_cb.pm_pend_link = link_ind;
    btm_cb.pm_pend_id = pm_id;

    switch (md_res.mode) {
    case BTM_PM_MD_ACTIVE:
        switch (p_cb->state) {
        case BTM_PM_MD_SNIFF:
            btsnd_hcic_exit_sniff_mode(p_buf, btm_cb.acl_db[link_ind].hci_handle);
            break;

        case BTM_PM_MD_PARK:
            btsnd_hcic_exit_park_mode(p_buf, btm_cb.acl_db[link_ind].hci_handle);
            break;

        default:
            btm_cb.pm_pend_link = MAX_L2CAP_LINKS;
            break;
        }

        break;

    case BTM_PM_MD_HOLD:
        btsnd_hcic_hold_mode(p_buf, btm_cb.acl_db[link_ind].hci_handle, md_res.max, md_res.min);
        break;

    case BTM_PM_MD_SNIFF:
        btsnd_hcic_sniff_mode(p_buf, btm_cb.acl_db[link_ind].hci_handle, md_res.max, md_res.min, md_res.attempt, md_res.timeout);
        break;

    case BTM_PM_MD_PARK:
        btsnd_hcic_park_mode(p_buf, btm_cb.acl_db[link_ind].hci_handle, md_res.max, md_res.min);
        break;

    default:
        btm_cb.pm_pend_link = MAX_L2CAP_LINKS;
        break;
    }

    if (btm_cb.pm_pend_link == MAX_L2CAP_LINKS)
        GKI_freebuf(p_buf);

    return BTM_CMD_STARTED;
}

void btm_pm_proc_cmd_status(UINT8 status) {
    tBTM_PM_STATUS pm_status;
    tBTM_PM_MCB* p_cb;

    {
        tBTM_CB_COMPAT* p = (tBTM_CB_COMPAT*)&btm_cb;

        if (p->pm_pend_link >= MAX_L2CAP_LINKS)
            return;

        p_cb = &p->pm_mode_db[p->pm_pend_link];
    }

    if (status == HCI_SUCCESS) {
        p_cb->state = BTM_PM_ST_PENDING;
        pm_status = BTM_PM_STS_PENDING;
    } else {
        pm_status = BTM_PM_STS_ERROR;
    }

    {
        tBTM_CB_COMPAT* p = (tBTM_CB_COMPAT*)&btm_cb;

        if (p->pm_pend_id != BTM_PM_SET_ONLY_ID && p->pm_reg_db[p->pm_pend_id].mask & BTM_PM_REG_NOTIF) {
            (*p->pm_reg_db[p->pm_pend_id].cback)(p->acl_db[p->pm_pend_link].remote_addr, pm_status, 0, status);
        }
    }

    ((tBTM_CB_COMPAT*)&btm_cb)->pm_pend_link = MAX_L2CAP_LINKS;
}

void btm_pm_proc_mode_change(UINT8 hci_status, UINT16 hci_handle, UINT8 mode, UINT16 interval) {
    tACL_CONN* p;
    tBTM_PM_MCB* p_cb = NULL;
    int xx;
    int yy;
    tBTM_SEC_DEV_REC* p_dev_rec;

    if ((xx = btm_handle_to_acl_index(hci_handle)) >= MAX_L2CAP_LINKS)
        return;

    p = &btm_cb.acl_db[xx];

    if (mode == HCI_MODE_ACTIVE) {
        if (p->restore_pkt_types && btm_num_sco_links_active() == 0) {
            p->restore_pkt_types = 0;

            BTM_TRACE(0, "btm mode change AFTER unsniffing; hci hdl 0x%x, types 0x%02x", hci_handle, p->pkt_types_mask);

            btsnd_hcic_change_conn_type(p->hci_handle, p->pkt_types_mask);
        }
    }

    p_cb = &btm_cb.pm_mode_db[xx];
    p_cb->state = mode;
    p_cb->interval = interval;

    for (yy = 0; yy < BTM_MAX_PM_RECORDS; ++yy) {
        if (mode == BTM_PM_MD_ACTIVE && p_cb->req_mode[yy].mode == BTM_PM_MD_HOLD) {
            p_cb->req_mode[yy].mode = BTM_PM_MD_ACTIVE;
        }
    }

    if (p_cb->chg_ind == TRUE)
        btm_pm_snd_md_req(BTM_PM_SET_ONLY_ID, xx, NULL);

    for (yy = 0; yy < BTM_MAX_PM_RECORDS; ++yy) {
        if (btm_cb.pm_reg_db[yy].mask & BTM_PM_REG_NOTIF) {
            (*btm_cb.pm_reg_db[yy].cback)(p->remote_addr, mode, interval, hci_status);
        }
    }

    if (btm_cb.acl_db[xx].switch_role_state == BTM_ACL_SWKEY_STATE_MODE_CHANGE) {
        if (((p_dev_rec = btm_find_dev(btm_cb.acl_db[xx].remote_addr)) != NULL) && p_dev_rec->sec_flags & BTM_SEC_ENCRYPTED) {
            if (btsnd_hcic_set_conn_encrypt(hci_handle, FALSE)) {
                btm_cb.acl_db[xx].switch_role_state = BTM_ACL_SWKEY_STATE_ENCRYPTION_OFF;
                return;
            }
        } else if (btsnd_hcic_switch_role(btm_cb.acl_db[xx].remote_addr, !btm_cb.acl_db[xx].link_role)) {
            btm_cb.acl_db[xx].switch_role_state = BTM_ACL_SWKEY_STATE_IDLE;
            return;
        }

        btm_cb.acl_db[xx].switch_role_state = BTM_ACL_SWKEY_STATE_IDLE;

        if (btm_cb.devcb.p_switch_role_cb) {
            btm_cb.devcb.switch_role_ref_data.hci_status = hci_status;
            (*btm_cb.devcb.p_switch_role_cb)(&btm_cb.devcb.switch_role_ref_data.hci_status);
            btm_cb.devcb.p_switch_role_cb = NULL;
        }
    }
}

BOOLEAN BTM_IsPowerManagerOn(void) {
    return BTM_PWR_MGR_INCLUDED;
}
