// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/stack/btm/btm_sco
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

#include "revolution/BTE/include/bt_target.h"
#include "revolution/BTE/stack/include/bt_types.h"
#include "revolution/BTE/gki/common/gki.h"
#include "revolution/BTE/stack/include/hcidefs.h"

/* btm_cb (retail .bss at 0x805BC2F8, size 0x27C4) is declared in btm_int.h as
   the Broadcom tBTM_CB whose overall layout does not match the Wii binary.
   The flattened tSCO_CONN layout in btm_int.h IS retail-correct, so SCO DB
   entries are reached through it; the few other touched fields are read at
   their verified retail byte offsets. */

/* btm_int.h declares btm_sco_chk_pend_unpark with 2 params, but the retail
   function takes 3 (status, handle, mode); rename the header declaration out
   of the way so the 3-param definition below is the only one in this TU. */
#define btm_sco_chk_pend_unpark btm_sco_chk_pend_unpark_hdr
#include "revolution/BTE/stack/btm/btm_int.h"
#undef btm_sco_chk_pend_unpark

extern void LogMsg_2(UINT32 trace_set_mask, const char *fmt_str, UINT32 p1,
                     UINT32 p2);
extern void LogMsg_6(UINT32 trace_set_mask, const char *fmt_str, UINT32 p1,
                     UINT32 p2, UINT32 p3, UINT32 p4, UINT32 p5, UINT32 p6);

/* HCI command builders (hcicmds unit, declared locally like btm_acl.c) */
extern BOOLEAN btsnd_hcic_disconnect(UINT16 handle, UINT8 reason);
extern BOOLEAN btsnd_hcic_add_SCO_conn(UINT16 handle, UINT16 packet_types);
extern BOOLEAN btsnd_hcic_setup_esco_conn(UINT16 handle, UINT32 tx_bw,
                                          UINT32 rx_bw, UINT16 max_latency,
                                          UINT16 voice_contfmt,
                                          UINT8 retrans_effort,
                                          UINT16 packet_types);
extern void btm_chg_all_acl_pkt_types(BOOLEAN is_sco_active);

/* Retail-layout overlay of the btm_cb fields used by the SCO functions
   (offsets verified against build/us/asm/.../btm_sco.s):
   - 0x1850: tSCO_CB (app_sco_ind_cb at 0, sco_db at 4 -- btm_int.h layout
     with BTM_SCO_HCI_INCLUDED == FALSE is retail-correct)
   - 0x18F6: sco_disc_reason, 0x18F8: def_esco_parms
   - 0x1908: esco_supported, 0x1909: desired_sco_mode, 0x190A: xfer_sco_type
   - 0x27C0: trace_level
   The retail uses 3 SCO DB entries regardless of BTM_MAX_SCO_LINKS. */
typedef struct
{
    UINT8      pad0[0x636];                 /* devcb up to local_version */
    UINT8      local_version[8];            /* 0x636 devcb.local_version */
    UINT8      pad0b[0x656 - 0x63E];
    UINT16     btm_sco_pkt_types_supported; /* 0x656 */
    UINT8      pad1[0x1850 - 0x658];
    tBTM_SCO_IND_CBACK *app_sco_ind_cb;     /* 0x1850 sco_cb.app_sco_ind_cb */
    tSCO_CONN  sco_db[3];                   /* 0x1854 sco_cb.sco_db[3] */
    BD_ADDR    xfer_addr;                   /* 0x18F0 sco_cb.xfer_addr */
    UINT16     sco_disc_reason;             /* 0x18F6 sco_cb.sco_disc_reason */
    tBTM_ESCO_PARAMS def_esco_parms;        /* 0x18F8 sco_cb.def_esco_parms */
    UINT8      esco_supported;              /* 0x1908 sco_cb.esco_supported */
    UINT8      desired_sco_mode;            /* 0x1909 sco_cb.desired_sco_mode */
    UINT8      xfer_sco_type;               /* 0x190A sco_cb.xfer_sco_type */
    UINT8      pad2[0x27C0 - 0x190B];       /* pcm/codec/rest */
    UINT8      trace_level;                 /* 0x27C0 */
} tBTM_CB_LOCAL;

#define SCO_CB ((tBTM_CB_LOCAL *)&btm_cb)

void btm_sco_init(void)
{
    extern unsigned long btm_esco_defaults[];
    unsigned long* src = btm_esco_defaults;
    unsigned char* cb = (unsigned char*)&btm_cb;

    *(unsigned short*)(cb + 0x18f6) = 0xffff;
    *(unsigned long*)(cb + 0x18f8) = src[0];
    *(unsigned long*)(cb + 0x18fc) = src[1];
    *(unsigned long*)(cb + 0x1900) = src[2];
    *(unsigned long*)(cb + 0x1904) = src[3];
    cb[0x1909] = 2;
}

void btm_esco_conn_rsp(UINT8 hci_status, BD_ADDR bda, UINT16 hci_handle, tBTM_ESCO_DATA* p_esco_data) {}

/* SCO control-block states (retail values; btm_int.h does not define them) */
#define SCO_ST_UNUSED           0
#define SCO_ST_LISTENING        1
#define SCO_ST_W4_CONN_RSP      2
#define SCO_ST_CONNECTING       3
#define SCO_ST_CONNECTED        4
#define SCO_ST_DISCONNECTING    5
#define SCO_ST_PEND_UNPARK      6

static tBTM_STATUS btm_send_connect_request(UINT16 acl_handle,
                                               tBTM_ESCO_PARAMS *p_setup)
{
    UINT16 temp_pkt_types;

    btm_chg_all_acl_pkt_types(TRUE);

    if (!SCO_CB->esco_supported) {
        if (!btsnd_hcic_add_SCO_conn(acl_handle,
                                     BTM_ESCO_2_SCO(p_setup->packet_types))) {
            return BTM_NO_RESOURCES;
        }
    } else {
        temp_pkt_types = BTM_SCO_SUPPORTED_PKTS_MASK & p_setup->packet_types &
                         SCO_CB->btm_sco_pkt_types_supported;

        if (SCO_CB->local_version[0] >= HCI_PROTO_VERSION_2_0) {
            temp_pkt_types |=
                (p_setup->packet_types & BTM_SCO_EXCEPTION_PKTS_MASK) |
                (SCO_CB->btm_sco_pkt_types_supported & BTM_SCO_EXCEPTION_PKTS_MASK);
        }

        if (SCO_CB->trace_level >= BT_TRACE_LEVEL_API) {
            LogMsg_6(TRACE_CTRL_GENERAL | TRACE_LAYER_BTM | TRACE_ORG_STACK |
                     TRACE_TYPE_API,
                     "      txbw 0x%x, rxbw 0x%x, lat 0x%x, voice 0x%x, retrans 0x%02x, pkt 0x%04x",
                     p_setup->tx_bw, p_setup->rx_bw, p_setup->max_latency,
                     p_setup->voice_contfmt, p_setup->retrans_effort,
                     temp_pkt_types);
        }

        if (!btsnd_hcic_setup_esco_conn(acl_handle, p_setup->tx_bw,
                                        p_setup->rx_bw, p_setup->max_latency,
                                        p_setup->voice_contfmt,
                                        p_setup->retrans_effort,
                                        temp_pkt_types)) {
            return BTM_NO_RESOURCES;
        } else {
            p_setup->packet_types = temp_pkt_types;
        }
    }

    return BTM_CMD_STARTED;
}

void btm_sco_chk_pend_unpark(UINT8 hci_status, UINT16 hci_handle, UINT8 mode)
{
    UINT16 xx;
    UINT16 acl_handle;
    tSCO_CONN *p = &SCO_CB->sco_db[0];

    for (xx = 0; xx < 3; xx++, p++) {
        if ((p->state == SCO_ST_PEND_UNPARK) && (mode == HCI_MODE_ACTIVE) &&
            (hci_status == HCI_SUCCESS) &&
            ((acl_handle = BTM_GetHCIConnHandle(p->bd_addr)) == hci_handle)) {
            if (SCO_CB->trace_level >= BT_TRACE_LEVEL_API) {
                LogMsg_2(TRACE_CTRL_GENERAL | TRACE_LAYER_BTM | TRACE_ORG_STACK |
                         TRACE_TYPE_API,
                         "btm_sco_chk_pend_unpark -> (e)SCO Link for ACL handle 0x%04x, Desired Type %d",
                         acl_handle, SCO_CB->desired_sco_mode);
            }

            if ((btm_send_connect_request(acl_handle, &p->esco_setup)) == BTM_CMD_STARTED)
                p->state = SCO_ST_CONNECTING;
        }
    }
}

void btm_sco_conn_req(BD_ADDR bda, DEV_CLASS dev_class, UINT8 link_type) {}

void btm_sco_connected(UINT8 hci_status, BD_ADDR bda, UINT16 hci_handle,
                       tBTM_ESCO_DATA *p_esco_data)
{
    tSCO_CONN *p = &SCO_CB->sco_db[0];
    UINT16 xx;
    UINT16 state;
    BOOLEAN is_conn_cback = FALSE;
    tBTM_CHG_ESCO_PARAMS parms;

    SCO_CB->sco_disc_reason = hci_status;

    for (xx = 0; xx < 3; xx++, p++) {
        state = p->state;
        if ((UINT16)(state + 0xFFFF) <= 2) {
            if (!p->rem_bd_known)
                continue;
            if (bda && memcmp(p->bd_addr, bda, BD_ADDR_LEN))
                continue;

            if (hci_status != HCI_SUCCESS) {
                if (state == SCO_ST_CONNECTING) {
                    if (hci_status != HCI_ERR_LMP_ERR_TRANS_COLLISION) {
                        p->state = SCO_ST_UNUSED;
                        (*p->p_disc_cb)(xx);
                    }
                } else {
                    p->state = SCO_ST_LISTENING;
                }
                return;
            }

            if (state == SCO_ST_LISTENING)
                is_conn_cback = TRUE;

            p->state = SCO_ST_CONNECTED;
            p->hci_handle = hci_handle;

            if (!SCO_CB->esco_supported) {
                p->link_type = BTM_LINK_TYPE_SCO;
                if (is_conn_cback) {
                    parms.packet_types = p->esco_setup.packet_types;
                    /* Keep the other parameters the same for SCO */
                    parms.max_latency = p->esco_setup.max_latency;
                    parms.retrans_effort = p->esco_setup.retrans_effort;

                    BTM_ChangeEScoLinkParms(xx, &parms);
                }
            } else {
                if (p_esco_data)
                    *(tBTM_ESCO_DATA *)((UINT8 *)p + 0x24) = *p_esco_data;
            }

            (*p->p_conn_cb)(xx);

            return;
        }
    }
}

void btm_remove_sco_links(BD_ADDR bda)
{
    /* Iterate over 3 SCO links; if rem_bd_known flag (+0xd) is set
       and BD_ADDR at +0x28 matches, remove the SCO link */
    unsigned char* sco_cb = (unsigned char*)&btm_cb + 0x1854;
    UINT16 i;

    i = 0;
    while (i < 3) {
        if (sco_cb[0xd] != 0 && memcmp(sco_cb + 0x28, bda, 6) == 0) {
            BTM_RemoveSco((UINT16)i);
        }
        i++;
        sco_cb += 0x34;
    }
}

void btm_sco_removed(UINT16 hci_handle, UINT8 reason) {}

void btm_sco_acl_removed(BD_ADDR bda)
{
    /* Iterate over 3 SCO links; if link is active and matches bda (or bda is NULL),
       clear the link state and call the esco callback */
    unsigned char* sco_cb = (unsigned char*)&btm_cb + 0x1854;
    UINT32 i;

    i = 0;
    do {
        if (*(UINT16*)(sco_cb + 0x8) != 0) {
            if (bda == NULL) {
                goto remove_sco;
            }
            if (memcmp(sco_cb + 0x28, bda, 6) != 0) {
                goto next;
            }
            if (sco_cb[0xd] == 0) {
                goto next;
            }
remove_sco:
            *(UINT16*)(sco_cb + 0x8) = 0;
            *(UINT32*)(sco_cb + 0x10) = 0;
            (*(void(**)(UINT16))(sco_cb + 0x4))((UINT16)i);
        }
next:
        i++;
        sco_cb += 0x34;
    } while (i < 3);
}

void btm_route_sco_data(BT_HDR* p_msg)
{
    GKI_freebuf(p_msg);
}

/* Stub: not-yet-recovered retail body.  -ipa file must NOT inline the empty
   body, or callers (btm_sco_connected) would lose the bl call site and DCE
   the parms setup (same pattern as btm_sec_execute_procedure in btm_sec.c). */
#pragma auto_inline off
tBTM_STATUS BTM_ChangeEScoLinkParms(UINT16 sco_inx, tBTM_CHG_ESCO_PARAMS* p_parms) {}
#pragma auto_inline on

void btm_esco_proc_conn_chg(UINT8 status, UINT16 handle, UINT8 tx_interval,
                            UINT8 retrans_window, UINT16 rx_pkt_len,
                            UINT16 tx_pkt_len)
{
    /* Retail layout uses 3 SCO DB entries regardless of BTM_MAX_SCO_LINKS */
    tSCO_CONN *p = (tSCO_CONN *)((UINT8 *)&btm_cb + 0x1854);
    tBTM_CHG_ESCO_EVT_DATA data;
    UINT16 xx;

    if (*((UINT8 *)&btm_cb + 0x27c0) >= BT_TRACE_LEVEL_EVENT) {
        LogMsg_2(TRACE_CTRL_GENERAL | TRACE_LAYER_BTM | TRACE_ORG_STACK |
                 TRACE_TYPE_EVENT,
                 "btm_esco_proc_conn_chg -> handle 0x%04x, status 0x%02x",
                 (UINT32)handle, (UINT32)status);
    }

    for (xx = 0; xx < 3; xx++, p++) {
        /* SCO_ST_CONNECTED == 4 */
        if (p->state == 4 && handle == p->hci_handle) {
            /* If upper layer wants notification */
            if (p->p_esco_cback) {
                memcpy(data.bd_addr, p->bd_addr, BD_ADDR_LEN);
                data.hci_status = status;
                data.sco_inx = xx;
                data.rx_pkt_len = p->rx_pkt_len = rx_pkt_len;
                data.tx_pkt_len = p->tx_pkt_len = tx_pkt_len;
                data.tx_interval = p->tx_interval = tx_interval;
                data.retrans_window = p->retrans_window = retrans_window;
                (*p->p_esco_cback)(BTM_ESCO_CHG_EVT,
                                   (tBTM_ESCO_EVT_DATA *)&data);
            }
            return;
        }
    }
}

BOOLEAN btm_is_sco_active(UINT16 handle)
{
    /* Check each of the 3 SCO links: if hci_handle matches and
       state == SCO_ST_CONNECTED (4), the link is active */
    unsigned char* cb = (unsigned char*)&btm_cb;

    if (*(UINT16*)(cb + 0x185e) == handle && *(UINT16*)(cb + 0x185c) == 4) {
        return TRUE;
    }
    if (*(UINT16*)(cb + 0x1892) == handle && *(UINT16*)(cb + 0x1890) == 4) {
        return TRUE;
    }
    if (*(UINT16*)(cb + 0x18c6) == handle && *(UINT16*)(cb + 0x18c4) == 4) {
        return TRUE;
    }
    return FALSE;
}

UINT8 btm_num_sco_links_active(void)
{
    UINT8 num = 0;
    UINT16 state;

    state = btm_cb.sco_cb.sco_db[0].state;
    if (state >= 2 && state < 7) {
        num = 1;
    }

    state = btm_cb.sco_cb.sco_db[1].state;
    if (state >= 2 && state < 7) {
        num++;
    }

    state = btm_cb.sco_cb.sco_db[2].state;
    if (state >= 2 && state < 7) {
        num++;
    }

    return num;
}

BOOLEAN btm_is_sco_active_by_bdaddr(BD_ADDR remote_bda)
{
    /* Iterate over 3 SCO links; if BD_ADDR matches and state == SCO_ST_CONNECTED (4), return TRUE */
    UINT16 i;
    unsigned char* sco_cb;

    sco_cb = (unsigned char*)&btm_cb + 0x1854;
    i = 0;
    while (i < 3) {
        if (memcmp(sco_cb + 0x28, remote_bda, 6) == 0) {
            if (*(UINT16*)(sco_cb + 0x8) == 4) {
                return TRUE;
            }
        }
        i++;
        sco_cb += 0x34;
    }
    return FALSE;
}

/* btm_remove_sco_links calls BTM_RemoveSco; -ipa file must not inline the
   body or the caller's bl call site disappears (same pattern as
   BTM_ChangeEScoLinkParms above). */
#pragma auto_inline off
tBTM_STATUS BTM_RemoveSco(UINT16 sco_inx)
{
    tSCO_CONN *p = &SCO_CB->sco_db[sco_inx];
    UINT16 tempstate;

    /* Validity check */
    if (sco_inx >= 3 || (tempstate = p->state) == SCO_ST_UNUSED)
        return (BTM_UNKNOWN_ADDR);

    /* If no HCI handle, simply drop the connection and return */
    if (p->hci_handle == HCI_INVALID_HANDLE || tempstate == SCO_ST_PEND_UNPARK) {
        p->hci_handle = HCI_INVALID_HANDLE;
        p->state = SCO_ST_UNUSED;
        p->p_esco_cback = NULL;
        return (BTM_SUCCESS);
    }

    p->state = SCO_ST_DISCONNECTING;

    if (!btsnd_hcic_disconnect(p->hci_handle, HCI_ERR_PEER_USER)) {
        p->state = tempstate;
        return (BTM_NO_RESOURCES);
    }

    return (BTM_CMD_STARTED);
}
#pragma auto_inline on
