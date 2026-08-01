// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/stack/btm/btm_acl
// Replace stubs with high-level C/C++ during decomp.

#include <string.h>
#include <harness_catalog.h>
#include "revolution/bte/stack/include/bt_types.h"

extern void LogMsg_1(UINT32 trace_set_mask, const char *fmt_str, UINT32 p1);
extern void LogMsg_6(UINT32 trace_set_mask, const char *fmt_str, UINT32 p1,
                     UINT32 p2, UINT32 p3, UINT32 p4, UINT32 p5, UINT32 p6);
extern void btu_stop_timer(void *p_tle);

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
    UINT8   link_up_issued;      /* 0x11B */
} tACL_CONN;

typedef void (*tBTM_ACL_DB_CHANGE_CB)(UINT8 *p_bda, UINT8 *p_dc, UINT8 *p_bdn,
                                      UINT8 *features, BOOLEAN is_new);
typedef void (*tBTM_CMPL_CB)(void *p1);

/* Structure returned with QoS setup complete event (in tBTM_CMPL_CB callback) */
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
    UINT8      pad1b[0x5C4 - 0x4CC];           /* pm / devcb up to lsto callback */
    tBTM_CMPL_CB p_lsto_cback;                 /* 0x5C4 (retail layout; see btm_int.h pm section) */
    UINT8      pad2[0x600 - 0x5C8];            /* devcb up to qossu_timer */
    UINT8      qossu_timer[0x18];              /* 0x600 */
    tBTM_CMPL_CB p_qossu_cmpl_cb;              /* 0x618 */
    UINT8      pad3[0x27BF - 0x61C];           /* sec / inq / sco state */
    UINT8      acl_disc_reason;                /* 0x27BF */
    UINT8      trace_level;                    /* 0x27C0 */
} tBTM_CB_LOCAL;

extern tBTM_CB_LOCAL btm_cb;

void btm_acl_init() {
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

void btm_acl_device_down() {}

void BTM_SwitchRole() {}

void btm_acl_encrypt_change() {}

void BTM_SetLinkPolicy() {}

void BTM_SetDefaultLinkPolicy(UINT16 settings)
{
    btm_cb.btm_def_link_policy = settings;
}

void btm_read_link_policy_complete() {}

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
        cback = btm_cb.p_lsto_cback;
        evt[0] = 0xa;
        *(unsigned short *)(evt + 8) = 0;
        btm_cb.p_lsto_cback = NULL;
        if (cback != 0) {
            cback(evt);
        }
    }
}

void btm_get_max_packet_size() {}

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

void BTM_ReadRSSI() {}

void BTM_ReadLinkQuality() {}

void btm_read_rssi_complete() {}

void btm_read_link_quality_complete() {}

void btm_remove_acl() {}

void btm_chg_all_acl_pkt_types() {}
