// Decompiled btm_acl.c - high-level C reconstruction (Wii RVL SDK BTE stack).
// Retail symbol btm_cb is a .bss object at 0x805BC2F8 (size 0x27C4);
// btu_cb is at 0x805BBDD0. The btm_int.h tBTM_CB/tACL_CONN layouts do not
// match the Wii binary, so local surrogates covering only the touched
// fields are declared below (offsets verified against
// build/us/asm/.../btm_acl.s).

#include <string.h>
#include <harness_catalog.h>
#include "revolution/bte/stack/include/bt_types.h"
#include "revolution/bte/stack/include/hcidefs.h"

extern void LogMsg_1(UINT32 trace_set_mask, const char *fmt_str, UINT32 p1);
extern void LogMsg_2(UINT32 trace_set_mask, const char *fmt_str, UINT32 p1,
                     UINT32 p2);
extern void LogMsg_6(UINT32 trace_set_mask, const char *fmt_str, UINT32 p1,
                     UINT32 p2, UINT32 p3, UINT32 p4, UINT32 p5, UINT32 p6);
extern void btu_start_timer(void *p_tle, UINT16 type, UINT32 timeout);
extern void btu_stop_timer(void *p_tle);

extern UINT8 *BTM_ReadLocalFeatures(void);

extern BOOLEAN btsnd_hcic_switch_role(BD_ADDR p, UINT8 role);
extern BOOLEAN btsnd_hcic_write_policy_set(UINT16 handle, UINT16 settings);
extern BOOLEAN btsnd_hcic_read_rssi(UINT16 handle);
extern BOOLEAN btsnd_hcic_get_link_quality(UINT16 handle);
extern BOOLEAN btsnd_hcic_disconnect(UINT16 handle, UINT8 reason);
extern BOOLEAN l2c_link_hci_disc_comp(UINT16 handle, UINT8 reason);

/* Maximum ACL payload lengths (no HCI_*_PKT_LEN constants in hcidefs.h). */
#define BTM_DM1_PKT_LEN      17    /* 0x11 */
#define BTM_DH1_PKT_LEN      27    /* 0x1B */
#define BTM_2DH1_PKT_LEN     54    /* 0x36 */
#define BTM_3DH1_PKT_LEN     83    /* 0x53 */
#define BTM_DM3_PKT_LEN      121   /* 0x79 */
#define BTM_DH3_PKT_LEN      183   /* 0xB7 */
#define BTM_DM5_PKT_LEN      224   /* 0xE0 */
#define BTM_2DH3_PKT_LEN     367   /* 0x16F */
#define BTM_DH5_PKT_LEN      339   /* 0x153 */
#define BTM_2DH5_PKT_LEN     679   /* 0x2A7 */
#define BTM_3DH3_PKT_LEN     552   /* 0x228 */
#define BTM_3DH5_PKT_LEN     1021  /* 0x3FD */
#define BTM_3DH5_SCO_LEN     1017  /* 0x3F9 */

/* Switch-role / change-link-key state machine (see btm_int.h tACL_CONN). */
#define BTM_ACL_SWKEY_STATE_IDLE            0
#define BTM_ACL_SWKEY_STATE_ENCRYPTION_OFF  2
#define BTM_ACL_SWKEY_STATE_SWITCHING       3
#define BTM_ACL_SWKEY_STATE_ENCRYPTION_ON   4

/* tBTM_STATUS values used by this unit (btm_api.h tBTM_STATUS enum). */
#define BTM_SUCCESS        0
#define BTM_CMD_STARTED    1
#define BTM_BUSY           2
#define BTM_NO_RESOURCES   3
#define BTM_UNKNOWN_ADDR   7
#define BTM_ERR_PROCESSING 10

/* Retail-layout ACL connection entry. This build's tBTM_CB.acl_db starts at
   btm_cb+0x34 with a 0x11C stride; the BTE header tACL_CONN carries extra
   tail fields (switch_role_state / change_key_state / encrypt_state) that
   are absent from the retail object, so a local layout is used. */
typedef struct
{
    UINT16  hci_handle;          /* 0x00 */
    UINT16  pkt_types_mask;      /* 0x02 */
    UINT16  restore_pkt_types;   /* 0x04 */
    UINT16  clock_offset;        /* 0x06 */
    UINT8   remote_addr[6];      /* 0x08 */
    UINT8   remote_dc[3];        /* 0x0E */
    UINT8   remote_name[248];    /* 0x11 */
    UINT16  manufacturer;        /* 0x10A */
    UINT16  lmp_subversion;      /* 0x10C */
    UINT16  link_super_tout;     /* 0x10E */
    UINT8   features[8];         /* 0x110 */
    UINT8   lmp_version;         /* 0x118 */
    UINT8   in_use;              /* 0x119 */
    UINT8   link_role;           /* 0x11A */
    UINT8   switch_role_state;   /* 0x11B */
} tACL_CONN;

typedef void (*tBTM_ACL_DB_CHANGE_CB)(UINT8 *p_bda, UINT8 *p_dc, UINT8 *p_bdn,
                                      UINT8 *features, BOOLEAN is_new);
typedef void (*tBTM_CMPL_CB)(void *p1);

/* Structure returned with link policy information (tBTM_CMPL_CB callback,
   in response to BTM_ReadLinkPolicy). */
typedef struct
{
    UINT8   status;             /* tBTM_STATUS */
    UINT8   hci_status;
    BD_ADDR rem_bda;
    UINT16  settings;
} tBTM_LNK_POLICY_RESULTS;

/* Structure returned with read RSSI event (tBTM_CMPL_CB callback). */
typedef struct
{
    UINT8   status;             /* tBTM_STATUS */
    UINT8   hci_status;
    INT8    rssi;
    BD_ADDR rem_bda;
} tBTM_RSSI_RESULTS;

/* Structure returned with read link quality event (tBTM_CMPL_CB callback). */
typedef struct
{
    UINT8   status;             /* tBTM_STATUS */
    UINT8   hci_status;
    UINT8   link_quality;
    BD_ADDR rem_bda;
} tBTM_LINK_QUALITY_RESULTS;

/* Structure returned with role switch information (tBTM_CMPL_CB callback). */
typedef struct
{
    UINT8   hci_status;
    UINT8   role;
    BD_ADDR remote_bd_addr;
} tBTM_ROLE_SWITCH_CMPL;

/* Structure returned with QoS setup complete event (tBTM_CMPL_CB callback) */
typedef struct
{
    FLOW_SPEC flow;
    UINT16 handle;
    UINT8 status;
} tBTM_QOS_SETUP_CMPL;

/* Retail-layout overlay of the tBTM_CB fields used by this unit. The BTE
   header tBTM_CB is compiled with different configuration (MAX_L2CAP_LINKS,
   local BD name length, extra tACL_CONN tail fields), so the retail offsets
   are reproduced here. */
typedef struct
{
    UINT8      pad0[0x34];                     /* cfg */
    tACL_CONN  acl_db[4];                      /* 0x34 */
    UINT8      pad1[0x4C4 - 0x34 - 0x11C * 4]; /* btm_scn */
    UINT16     btm_def_link_policy;            /* 0x4C4 */
    UINT16     btm_def_link_super_tout;        /* 0x4C6 */
    tBTM_ACL_DB_CHANGE_CB p_acl_changed_cb;    /* 0x4C8 */
    UINT8      pad1b[0x5AC - 0x4CC];           /* pm / devcb up to rlinkp timer */
    UINT8      rlinkp_timer[0x18];             /* 0x5AC devcb.rlinkp_timer */
    tBTM_CMPL_CB p_rlinkp_cmpl_cb;             /* 0x5C4 devcb.p_rlinkp_cmpl_cb */
    UINT8      rssi_timer[0x18];               /* 0x5C8 devcb.rssi_timer */
    tBTM_CMPL_CB p_rssi_cmpl_cb;               /* 0x5E0 devcb.p_rssi_cmpl_cb */
    UINT8      lnk_quality_timer[0x18];        /* 0x5E4 devcb.lnk_quality_timer */
    tBTM_CMPL_CB p_lnk_qual_cmpl_cb;           /* 0x5FC devcb.p_lnk_qual_cmpl_cb */
    UINT8      qossu_timer[0x18];              /* 0x600 devcb.qossu_timer */
    tBTM_CMPL_CB p_qossu_cmpl_cb;              /* 0x618 devcb.p_qossu_cmpl_cb */
    UINT8      pad1d[0x624 - 0x61C];           /* 0x61C unknown devcb fields */
    tBTM_ROLE_SWITCH_CMPL switch_role_ref_data;/* 0x624 devcb.switch_role_ref_data */
    tBTM_CMPL_CB p_switch_role_cb;             /* 0x62C devcb.p_switch_role_cb */
    BD_ADDR    local_addr;                     /* 0x630 devcb.local_addr */
    UINT8      pad3a[0x654 - 0x636];           /* 0x636 */
    UINT16     btm_acl_pkt_types_supported;    /* 0x654 */
    UINT8      pad3b[0x27BF - 0x656];          /* sec / inq / sco state */
    UINT8      acl_disc_reason;                /* 0x27BF */
    UINT8      trace_level;                    /* 0x27C0 */
} tBTM_CB_LOCAL;

extern tBTM_CB_LOCAL btm_cb;

/* Retail-layout overlay of the btu_cb fields used by this unit. The vendored
   btu.h tBTU_CB only models the timer/queue head. */
typedef struct
{
    UINT8   _pad0[0x7C];                      /* timer_reg/event_reg/queues */
    UINT16  hcit_acl_data_size;               /* 0x7C */
} tBTU_CB_LOCAL;

extern tBTU_CB_LOCAL btu_cb;

/* Locate the ACL connection entry for a BD address (retail-inlined helper). */
static tACL_CONN *btm_bda_to_acl_local(BD_ADDR bda);

void btm_acl_init() {
    /* String pool alignment: these three trace strings precede the
       BTM_SetLinkPolicy strings in the retail .data pool; keeping the same
       order (and sizes) preserves the pooled offsets / reloc addends. */
    static const char *const pool_dup = "Duplicate btm_acl_created: RemBdAddr: %02x%02x%02x%02x%02x%02x";
    static const char *const pool_pkt = "SetPacketType Mask -> 0x%04x";
    static const char *const pool_rs = "Role change request declined since the previous request for this device is not completed ";
    (void)pool_dup;
    (void)pool_pkt;
    (void)pool_rs;
    btm_cb.btm_def_link_super_tout = 0x7d00;
    btm_cb.acl_disc_reason = 0xff;
}

UINT8 btm_handle_to_acl_index(UINT16 hci_handle)
{
    tBTM_CB_LOCAL *cb = &btm_cb;
    UINT8 index = 0;

    if (!(cb->acl_db[0].in_use && cb->acl_db[0].hci_handle == hci_handle)) {
        index = 1;
        if (!(cb->acl_db[1].in_use && cb->acl_db[1].hci_handle == hci_handle)) {
            index = 2;
            if (!(cb->acl_db[2].in_use && cb->acl_db[2].hci_handle == hci_handle)) {
                index = 3;
                if (!(cb->acl_db[3].in_use && cb->acl_db[3].hci_handle == hci_handle))
                    index = 4;
            }
        }
    }

    return index;
}

void btm_acl_created() {}

void btm_acl_removed(BD_ADDR bda)
{
    UINT8 xx = 0;
    tACL_CONN *p_acl;

    for (p_acl = &btm_cb.acl_db[0]; xx < 4; xx++) {
        if (p_acl->in_use && memcmp(p_acl->remote_addr, bda, 6) == 0)
            goto found;
        p_acl++;
    }
    p_acl = NULL;

found:
    if (p_acl != NULL) {
        p_acl->in_use = FALSE;
        if (btm_cb.p_acl_changed_cb != NULL) {
            btm_cb.p_acl_changed_cb(bda, NULL, NULL, NULL, FALSE);
        }
    }
}

void btm_acl_device_down(void)
{
    tACL_CONN *p = &btm_cb.acl_db[0];
    UINT8 xx;

    for (xx = 0; xx < 4; xx++, p++) {
        if (p->in_use)
            l2c_link_hci_disc_comp(p->hci_handle, HCI_ERR_HW_FAILURE);
    }
}

void BTM_SwitchRole() {}

void btm_acl_encrypt_change(UINT16 handle, UINT8 status, UINT8 encr_enable)
{
    tBTM_CB_LOCAL *cb = &btm_cb;
    tACL_CONN *p;
    UINT8 index = 0;

    if (!(cb->acl_db[0].in_use && cb->acl_db[0].hci_handle == handle)) {
        index = 1;
        if (!(cb->acl_db[1].in_use && cb->acl_db[1].hci_handle == handle)) {
            index = 2;
            if (!(cb->acl_db[2].in_use && cb->acl_db[2].hci_handle == handle)) {
                index = 3;
                if (!(cb->acl_db[3].in_use && cb->acl_db[3].hci_handle == handle))
                    index = 4;
            }
        }
    }

    if (index < 4)
        p = &btm_cb.acl_db[index];
    else
        return;

    if (p->switch_role_state == BTM_ACL_SWKEY_STATE_ENCRYPTION_OFF) {
        if (encr_enable != 0)
            p->switch_role_state = BTM_ACL_SWKEY_STATE_IDLE;
        else
            p->switch_role_state = BTM_ACL_SWKEY_STATE_SWITCHING;

        if (btsnd_hcic_switch_role(p->remote_addr, !p->link_role))
            return;

        p->switch_role_state = BTM_ACL_SWKEY_STATE_IDLE;

        if (btm_cb.p_switch_role_cb != NULL) {
            (*btm_cb.p_switch_role_cb)(&btm_cb.switch_role_ref_data);
            btm_cb.p_switch_role_cb = NULL;
        }
    } else if (p->switch_role_state == BTM_ACL_SWKEY_STATE_ENCRYPTION_ON) {
        p->switch_role_state = BTM_ACL_SWKEY_STATE_IDLE;

        if (btm_cb.p_switch_role_cb != NULL) {
            (*btm_cb.p_switch_role_cb)(&btm_cb.switch_role_ref_data);
            btm_cb.p_switch_role_cb = NULL;
        }
    }
}

UINT8 BTM_SetLinkPolicy(BD_ADDR remote_bda, UINT16 *settings)
{
    tACL_CONN *p;
    UINT8 *local_features = BTM_ReadLocalFeatures();

    /* First, check if the requested link policies are supported. */
    if (*settings != HCI_DISABLE_ALL_LM_MODES) {
        if ((*settings & HCI_ENABLE_MASTER_SLAVE_SWITCH) &&
            !HCI_SWITCH_SUPPORTED(local_features)) {
            *settings &= ~HCI_ENABLE_MASTER_SLAVE_SWITCH;

            if (btm_cb.trace_level >= BT_TRACE_LEVEL_API)
                LogMsg_1(TRACE_CTRL_GENERAL | TRACE_LAYER_BTM | TRACE_ORG_STACK | TRACE_TYPE_API,
                         "BTM_SetLinkPolicy switch not supported (settings: 0x%04x)",
                         (UINT32)*settings);
        }

        if ((*settings & HCI_ENABLE_HOLD_MODE) &&
            !HCI_HOLD_MODE_SUPPORTED(local_features)) {
            *settings &= ~HCI_ENABLE_HOLD_MODE;

            if (btm_cb.trace_level >= BT_TRACE_LEVEL_API)
                LogMsg_1(TRACE_CTRL_GENERAL | TRACE_LAYER_BTM | TRACE_ORG_STACK | TRACE_TYPE_API,
                         "BTM_SetLinkPolicy hold not supported (settings: 0x%04x)",
                         (UINT32)*settings);
        }

        if ((*settings & HCI_ENABLE_SNIFF_MODE) &&
            !HCI_SNIFF_MODE_SUPPORTED(local_features)) {
            *settings &= ~HCI_ENABLE_SNIFF_MODE;

            if (btm_cb.trace_level >= BT_TRACE_LEVEL_API)
                LogMsg_1(TRACE_CTRL_GENERAL | TRACE_LAYER_BTM | TRACE_ORG_STACK | TRACE_TYPE_API,
                         "BTM_SetLinkPolicy sniff not supported (settings: 0x%04x)",
                         (UINT32)*settings);
        }

        if ((*settings & HCI_ENABLE_PARK_MODE) &&
            !HCI_PARK_MODE_SUPPORTED(local_features)) {
            *settings &= ~HCI_ENABLE_PARK_MODE;

            if (btm_cb.trace_level >= BT_TRACE_LEVEL_API)
                LogMsg_1(TRACE_CTRL_GENERAL | TRACE_LAYER_BTM | TRACE_ORG_STACK | TRACE_TYPE_API,
                         "BTM_SetLinkPolicy park not supported (settings: 0x%04x)",
                         (UINT32)*settings);
        }
    }

    if ((p = btm_bda_to_acl_local(remote_bda)) != NULL)
        return btsnd_hcic_write_policy_set(p->hci_handle, *settings) ? BTM_CMD_STARTED : BTM_NO_RESOURCES;

    /* If here, no BD Addr found */
    return BTM_UNKNOWN_ADDR;
}

void BTM_SetDefaultLinkPolicy(UINT16 settings)
{
    btm_cb.btm_def_link_policy = settings;
}

void btm_read_link_policy_complete(UINT8 *p)
{
    tBTM_CMPL_CB p_cb;
    tBTM_LNK_POLICY_RESULTS lnk_pol_res;
    tACL_CONN *p_acl = &btm_cb.acl_db[0];
    UINT16 handle;
    UINT16 settings;
    UINT8 xx;

    p_cb = btm_cb.p_rlinkp_cmpl_cb;
    btu_stop_timer(&btm_cb.rlinkp_timer);
    btm_cb.p_rlinkp_cmpl_cb = NULL;

    if (p_cb != NULL) {
        lnk_pol_res.hci_status = p[0];

        if (lnk_pol_res.hci_status == HCI_SUCCESS) {
            lnk_pol_res.status = BTM_SUCCESS;
            handle = ((UINT16)p[1] + ((UINT16)p[2] << 8));
            settings = ((UINT16)p[3] + ((UINT16)p[4] << 8));
            lnk_pol_res.settings = settings;

            for (xx = 0; xx < 4; xx++) {
                if (p_acl->in_use && handle == p_acl->hci_handle) {
                    memcpy(lnk_pol_res.rem_bda, p_acl->remote_addr, 6);
                    break;
                }
                p_acl++;
            }
        } else {
            lnk_pol_res.status = BTM_ERR_PROCESSING;
        }

        (*p_cb)(&lnk_pol_res);
    }
}

void btm_read_remote_version_complete(UINT8 *p)
{
    tACL_CONN *p_acl_cb = &btm_cb.acl_db[0];
    UINT16 handle;
    int xx;

    if (p[0] == 0) {
        handle = (UINT16)((p[2] << 8) + p[1]);

        for (xx = 0; xx < 4; xx++) {
            if (p_acl_cb->in_use && p_acl_cb->hci_handle == handle) {
                p_acl_cb->lmp_version = p[3];
                p_acl_cb->manufacturer = (UINT16)(p[4] + (p[5] << 8));
                p_acl_cb->lmp_subversion = (UINT16)(p[6] + (p[7] << 8));
                return;
            }
            p_acl_cb++;
        }
    }
}

void btm_read_remote_features_complete() {}

void BTM_SetDefaultLinkSuperTout(UINT16 timeout)
{
    btm_cb.btm_def_link_super_tout = timeout;
}

BOOLEAN BTM_IsAclConnectionUp(BD_ADDR remote_bda)
{
    UINT8 xx;
    tACL_CONN *p_acl;

    if (btm_cb.trace_level >= BT_TRACE_LEVEL_API) {
        LogMsg_6(TRACE_CTRL_GENERAL | TRACE_LAYER_BTM | TRACE_ORG_STACK | TRACE_TYPE_API,
                 "BTM_ReadClockOffset: RemBdAddr: %02x%02x%02x%02x%02x%02x",
                 (UINT32)remote_bda[0], (UINT32)remote_bda[1], (UINT32)remote_bda[2],
                 (UINT32)remote_bda[3], (UINT32)remote_bda[4], (UINT32)remote_bda[5]);
    }

    for (xx = 0, p_acl = &btm_cb.acl_db[0]; xx < 4; xx++) {
        if (p_acl->in_use && memcmp(p_acl->remote_addr, remote_bda, 6) == 0)
            goto found;
        p_acl++;
    }
    p_acl = NULL;

found:
    if (p_acl != NULL) {
        return TRUE;
    }

    return FALSE;
}

u16 BTM_GetNumAclLinks(void)
{
    u16 num_links = 0;

    if (btm_cb.acl_db[0].in_use != 0)
        num_links = 1;
    if (btm_cb.acl_db[1].in_use != 0)
        num_links = (u16)(num_links + 1);
    if (btm_cb.acl_db[2].in_use != 0)
        num_links = (u16)(num_links + 1);
    if (btm_cb.acl_db[3].in_use != 0)
        num_links = (u16)(num_links + 1);

    return num_links;
}

UINT16 btm_get_acl_disc_reason_code(void)
{
    return (UINT16)btm_cb.acl_disc_reason;
}

UINT16 BTM_GetHCIConnHandle(BD_ADDR remote_bda)
{
    UINT8 xx = 0;
    tACL_CONN *p_acl;

    for (p_acl = &btm_cb.acl_db[0]; xx < 4; xx++) {
        if (p_acl->in_use && memcmp(p_acl->remote_addr, remote_bda, 6) == 0)
            goto found;
        p_acl++;
    }
    p_acl = NULL;

found:
    if (p_acl != NULL) {
        return p_acl->hci_handle;
    }

    return 0xFFFF;
}

void btm_process_clk_off_comp_evt(UINT16 hci_handle, UINT16 clock_offset)
{
    tBTM_CB_LOCAL *cb = &btm_cb;
    UINT8 index = 0;

    if (!(cb->acl_db[0].in_use && cb->acl_db[0].hci_handle == hci_handle)) {
        index = 1;
        if (!(cb->acl_db[1].in_use && cb->acl_db[1].hci_handle == hci_handle)) {
            index = 2;
            if (!(cb->acl_db[2].in_use && cb->acl_db[2].hci_handle == hci_handle)) {
                index = 3;
                if (!(cb->acl_db[3].in_use && cb->acl_db[3].hci_handle == hci_handle))
                    index = 4;
            }
        }
    }

    if (index < 4) {
        btm_cb.acl_db[index].clock_offset = clock_offset;
    }
}

void btm_acl_role_changed() {}

void btm_acl_timeout(void *p_acl_cb)
{
    tBTM_CMPL_CB cback;
    unsigned char evt[0x10];

    if (*(unsigned int *)((char *)p_acl_cb + 0x10) == 4) {
        cback = btm_cb.p_rlinkp_cmpl_cb;
        evt[0] = 0xa;
        *(unsigned short *)(evt + 8) = 0;
        btm_cb.p_rlinkp_cmpl_cb = NULL;
        if (cback != 0) {
            cback(evt);
        }
    }
}

UINT16 btm_get_max_packet_size(BD_ADDR addr)
{
    tACL_CONN *p = btm_bda_to_acl_local(addr);
    UINT16 pkt_types = 0;
    UINT16 max_packet_size = 0;

    if (p != NULL)
        pkt_types = p->pkt_types_mask;
    else {
        /* Special case for when info for the local device is requested */
        if (memcmp(btm_cb.local_addr, addr, 6) == 0)
            pkt_types = btm_cb.btm_acl_pkt_types_supported;
    }

    if (pkt_types != 0) {
        /* Walk the supported packet types from largest to smallest slot. */
        if (!(pkt_types & HCI_PKT_TYPES_MASK_NO_3_DH5))
            max_packet_size = BTM_3DH5_PKT_LEN;
        else if (!(pkt_types & HCI_PKT_TYPES_MASK_NO_2_DH5))
            max_packet_size = BTM_2DH5_PKT_LEN;
        else if (!(pkt_types & HCI_PKT_TYPES_MASK_NO_3_DH3))
            max_packet_size = BTM_3DH3_PKT_LEN;
        else if (pkt_types & HCI_PKT_TYPES_MASK_DH5)
            max_packet_size = BTM_DH5_PKT_LEN;
        else if (!(pkt_types & HCI_PKT_TYPES_MASK_NO_2_DH3))
            max_packet_size = BTM_2DH3_PKT_LEN;
        else if (pkt_types & HCI_PKT_TYPES_MASK_DM5)
            max_packet_size = BTM_DM5_PKT_LEN;
        else if (pkt_types & HCI_PKT_TYPES_MASK_DH3)
            max_packet_size = BTM_DH3_PKT_LEN;
        else if (pkt_types & HCI_PKT_TYPES_MASK_DM3)
            max_packet_size = BTM_DM3_PKT_LEN;
        else if (!(pkt_types & HCI_PKT_TYPES_MASK_NO_3_DH1))
            max_packet_size = BTM_3DH1_PKT_LEN;
        else if (!(pkt_types & HCI_PKT_TYPES_MASK_NO_2_DH1))
            max_packet_size = BTM_2DH1_PKT_LEN;
        else if (pkt_types & HCI_PKT_TYPES_MASK_DH1)
            max_packet_size = BTM_DH1_PKT_LEN;
        else if (pkt_types & HCI_PKT_TYPES_MASK_DM1)
            max_packet_size = BTM_DM1_PKT_LEN;
    }

    /* 3-DH5 drops to 0x3F9 when the controller reports a smaller ACL
       data size (e.g. while SCO is active). */
    if (max_packet_size == BTM_3DH5_PKT_LEN &&
        btu_cb.hcit_acl_data_size == BTM_3DH5_SCO_LEN)
        max_packet_size = BTM_3DH5_SCO_LEN;

    return max_packet_size;
}

int BTM_AclRegisterForChanges(void* p_change_callback)
{
    if (p_change_callback == 0) {
        btm_cb.p_acl_changed_cb = NULL;
        return 0;
    }
    if (btm_cb.p_acl_changed_cb != 0) {
        return 2;
    }
    btm_cb.p_acl_changed_cb = p_change_callback;
    return 0;
}

void btm_qos_setup_complete(UINT8 status, UINT16 handle, FLOW_SPEC *p_flow)
{
    tBTM_QOS_SETUP_CMPL qos_setup_cmpl;
    tBTM_CMPL_CB p_cback;

    p_cback = btm_cb.p_qossu_cmpl_cb;
    btu_stop_timer(btm_cb.qossu_timer);
    btm_cb.p_qossu_cmpl_cb = NULL;

    if (p_cback != NULL) {
        qos_setup_cmpl.status = status;
        qos_setup_cmpl.handle = handle;

        if (p_flow != NULL) {
            qos_setup_cmpl.flow.qos_flags = p_flow->qos_flags;
            qos_setup_cmpl.flow.service_type = p_flow->service_type;
            qos_setup_cmpl.flow.token_rate = p_flow->token_rate;
            qos_setup_cmpl.flow.peak_bandwidth = p_flow->peak_bandwidth;
            qos_setup_cmpl.flow.latency = p_flow->latency;
            qos_setup_cmpl.flow.delay_variation = p_flow->delay_variation;
        }

        if (btm_cb.trace_level >= BT_TRACE_LEVEL_DEBUG) {
            LogMsg_1(TRACE_CTRL_GENERAL | TRACE_LAYER_BTM | TRACE_ORG_STACK | TRACE_TYPE_DEBUG,
                     "BTM: p_flow->delay_variation: 0x%02x",
                     (UINT32)p_flow->delay_variation);
        }

        p_cback(&qos_setup_cmpl);
    }
}

UINT8 BTM_ReadRSSI(BD_ADDR remote_bda, tBTM_CMPL_CB p_cb)
{
    tACL_CONN *p;

    if (btm_cb.trace_level >= BT_TRACE_LEVEL_API) {
        LogMsg_6(TRACE_CTRL_GENERAL | TRACE_LAYER_BTM | TRACE_ORG_STACK | TRACE_TYPE_API,
                 "BTM_ReadRSSI: RemBdAddr: %02x%02x%02x%02x%02x%02x",
                 (UINT32)remote_bda[0], (UINT32)remote_bda[1], (UINT32)remote_bda[2],
                 (UINT32)remote_bda[3], (UINT32)remote_bda[4], (UINT32)remote_bda[5]);
    }

    /* If someone already waiting for rssi, return busy. */
    if (btm_cb.p_rssi_cmpl_cb != NULL)
        return BTM_BUSY;

    p = btm_bda_to_acl_local(remote_bda);

    if (p != NULL) {
        btu_start_timer(&btm_cb.rssi_timer, 9, 3);
        btm_cb.p_rssi_cmpl_cb = p_cb;

        if (!btsnd_hcic_read_rssi(p->hci_handle)) {
            btm_cb.p_rssi_cmpl_cb = NULL;
            btu_stop_timer(&btm_cb.rssi_timer);
            return BTM_NO_RESOURCES;
        } else
            return BTM_CMD_STARTED;
    }

    /* If here, no BD Addr found */
    return BTM_UNKNOWN_ADDR;
}

UINT8 BTM_ReadLinkQuality(BD_ADDR remote_bda, tBTM_CMPL_CB p_cb)
{
    tACL_CONN *p;

    if (btm_cb.trace_level >= BT_TRACE_LEVEL_API) {
        LogMsg_6(TRACE_CTRL_GENERAL | TRACE_LAYER_BTM | TRACE_ORG_STACK | TRACE_TYPE_API,
                 "BTM_ReadLinkQuality: RemBdAddr: %02x%02x%02x%02x%02x%02x",
                 (UINT32)remote_bda[0], (UINT32)remote_bda[1], (UINT32)remote_bda[2],
                 (UINT32)remote_bda[3], (UINT32)remote_bda[4], (UINT32)remote_bda[5]);
    }

    /* If someone already waiting for link quality, return busy. */
    if (btm_cb.p_lnk_qual_cmpl_cb != NULL)
        return BTM_BUSY;

    p = btm_bda_to_acl_local(remote_bda);

    if (p != NULL) {
        btu_start_timer(&btm_cb.lnk_quality_timer, 9, 3);
        btm_cb.p_lnk_qual_cmpl_cb = p_cb;

        if (!btsnd_hcic_get_link_quality(p->hci_handle)) {
            btu_stop_timer(&btm_cb.lnk_quality_timer);
            btm_cb.p_lnk_qual_cmpl_cb = NULL;
            return BTM_NO_RESOURCES;
        } else
            return BTM_CMD_STARTED;
    }

    /* If here, no BD Addr found */
    return BTM_UNKNOWN_ADDR;
}

void btm_read_rssi_complete(UINT8 *p)
{
    tBTM_CMPL_CB p_cb;
    tBTM_RSSI_RESULTS rssi_results;
    tACL_CONN *p_acl = &btm_cb.acl_db[0];
    UINT16 handle;
    UINT8 xx;

    p_cb = btm_cb.p_rssi_cmpl_cb;
    btu_stop_timer(&btm_cb.rssi_timer);
    btm_cb.p_rssi_cmpl_cb = NULL;

    if (p_cb != NULL) {
        rssi_results.hci_status = p[0];

        if (rssi_results.hci_status == HCI_SUCCESS) {
            rssi_results.status = BTM_SUCCESS;
            handle = ((UINT16)p[1] + ((UINT16)p[2] << 8));
            rssi_results.rssi = (INT8)p[3];

            if (btm_cb.trace_level >= BT_TRACE_LEVEL_EVENT)
                LogMsg_2(TRACE_CTRL_GENERAL | TRACE_LAYER_BTM | TRACE_ORG_STACK | TRACE_TYPE_EVENT,
                         "BTM RSSI Complete: rssi %d, hci status 0x%02x",
                         (UINT32)rssi_results.rssi, (UINT32)rssi_results.hci_status);

            for (xx = 0; xx < 4; xx++) {
                if (p_acl->in_use && handle == p_acl->hci_handle) {
                    memcpy(rssi_results.rem_bda, p_acl->remote_addr, 6);
                    break;
                }
                p_acl++;
            }
        } else {
            rssi_results.status = BTM_ERR_PROCESSING;
        }

        (*p_cb)(&rssi_results);
    }
}

void btm_read_link_quality_complete(UINT8 *p)
{
    tBTM_CMPL_CB p_cb;
    tBTM_LINK_QUALITY_RESULTS lnk_qual_res;
    tACL_CONN *p_acl = &btm_cb.acl_db[0];
    UINT16 handle;
    UINT8 xx;

    p_cb = btm_cb.p_lnk_qual_cmpl_cb;
    btu_stop_timer(&btm_cb.rssi_timer); /* retail stops the rssi timer here */
    btm_cb.p_lnk_qual_cmpl_cb = NULL;

    if (p_cb != NULL) {
        lnk_qual_res.hci_status = p[0];

        if (lnk_qual_res.hci_status == HCI_SUCCESS) {
            lnk_qual_res.status = BTM_SUCCESS;
            handle = ((UINT16)p[1] + ((UINT16)p[2] << 8));
            lnk_qual_res.link_quality = p[3];

            if (btm_cb.trace_level >= BT_TRACE_LEVEL_EVENT)
                LogMsg_2(TRACE_CTRL_GENERAL | TRACE_LAYER_BTM | TRACE_ORG_STACK | TRACE_TYPE_EVENT,
                         "BTM Link Quality Complete: Link Quality %d, hci status 0x%02x",
                         (UINT32)lnk_qual_res.link_quality, (UINT32)lnk_qual_res.hci_status);

            for (xx = 0; xx < 4; xx++) {
                if (p_acl->in_use && handle == p_acl->hci_handle) {
                    memcpy(lnk_qual_res.rem_bda, p_acl->remote_addr, 6);
                    break;
                }
                p_acl++;
            }
        } else {
            lnk_qual_res.status = BTM_ERR_PROCESSING;
        }

        (*p_cb)(&lnk_qual_res);
    }
}

static tACL_CONN *btm_bda_to_acl_local(BD_ADDR bda)
{
    tACL_CONN *p = &btm_cb.acl_db[0];
    UINT8 xx;

    for (xx = 0; xx < 4; xx++, p++) {
        if (p->in_use && memcmp(p->remote_addr, bda, 6) == 0)
            return p;
    }

    return NULL;
}

UINT8 btm_remove_acl(BD_ADDR bd_addr)
{
    tACL_CONN *p;
    UINT16 handle;

    p = btm_bda_to_acl_local(bd_addr);

    if (p != NULL)
        handle = p->hci_handle;
    else
        handle = 0xFFFF;

    if (!btsnd_hcic_disconnect(handle, HCI_ERR_PEER_USER))
        return BTM_NO_RESOURCES;

    return BTM_SUCCESS;
}

void btm_chg_all_acl_pkt_types() {}
