// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/stack/btm/btm_sco
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

#include "revolution/BTE/include/bt_target.h"
#include "revolution/BTE/stack/include/bt_types.h"
#include "revolution/BTE/stack/btm/btm_int.h"
#include "revolution/BTE/gki/common/gki.h"
#include "revolution/BTE/stack/include/hcidefs.h"

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

void btm_sco_chk_pend_unpark(UINT8 hci_status, UINT16 hci_handle) {}

void btm_sco_conn_req(BD_ADDR bda, DEV_CLASS dev_class, UINT8 link_type) {}

void btm_sco_connected(UINT8 hci_status, BD_ADDR bda, UINT16 hci_handle, tBTM_ESCO_DATA* p_esco_data) {}

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

tBTM_STATUS BTM_ChangeEScoLinkParms(UINT16 sco_inx, tBTM_CHG_ESCO_PARAMS* p_parms) {}

void btm_esco_proc_conn_chg(UINT8 status, UINT16 handle, UINT8 tx_interval, UINT8 retrans_window, UINT16 rx_pkt_len, UINT16 tx_pkt_len) {}

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
