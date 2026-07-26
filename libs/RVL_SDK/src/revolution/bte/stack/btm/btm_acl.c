// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/stack/btm/btm_acl
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern char btm_cb[];
void btm_acl_init() {
    *(short*)(btm_cb + 0x4c6) = 0x7d00;
    btm_cb[0x27bf] = (char)0xff;
}

void btm_handle_to_acl_index() {}

void btm_acl_created() {}

void btm_acl_removed() {}

void btm_acl_device_down() {}

void BTM_SwitchRole() {}

void btm_acl_encrypt_change() {}

void BTM_SetLinkPolicy() {}

void BTM_SetDefaultLinkPolicy() {}

void btm_read_link_policy_complete() {}

void btm_read_remote_version_complete() {}

void btm_read_remote_features_complete() {}

void BTM_SetDefaultLinkSuperTout() {}

void BTM_IsAclConnectionUp() {}

u16 BTM_GetNumAclLinks(void)
{
    u16 num_links = 0;

    if (btm_cb[0x14d] != 0)
        num_links = 1;
    if (btm_cb[0x269] != 0)
        num_links = (u16)(num_links + 1);
    if (btm_cb[0x385] != 0)
        num_links = (u16)(num_links + 1);
    if (btm_cb[0x4a1] != 0)
        num_links = (u16)(num_links + 1);

    return num_links;
}

void btm_get_acl_disc_reason_code() {}

void BTM_GetHCIConnHandle() {}

void btm_process_clk_off_comp_evt() {}

void btm_acl_role_changed() {}

void btm_acl_timeout(void *p_acl_cb)
{
    void (*cback)(void *);
    unsigned char evt[0x10];

    if (*(unsigned int *)((char *)p_acl_cb + 0x10) == 4) {
        cback = *(void (**)(void *))(btm_cb + 0x5c4);
        evt[0] = 0xa;
        *(unsigned short *)(evt + 8) = 0;
        *(void (**)(void *))(btm_cb + 0x5c4) = 0;
        if (cback != 0) {
            cback(evt);
        }
    }
}

void btm_get_max_packet_size() {}

int BTM_AclRegisterForChanges(void* p_change_callback)
{
    if (p_change_callback == 0) {
        *(void**)((char*)&btm_cb + 0x4c8) = 0;
        return 0;
    }
    if (*(void**)((char*)&btm_cb + 0x4c8) != 0) {
        return 2;
    }
    *(void**)((char*)&btm_cb + 0x4c8) = p_change_callback;
    return 0;
}

void btm_qos_setup_complete() {}

void BTM_ReadRSSI() {}

void BTM_ReadLinkQuality() {}

void btm_read_rssi_complete() {}

void btm_read_link_quality_complete() {}

void btm_remove_acl() {}

void btm_chg_all_acl_pkt_types() {}
