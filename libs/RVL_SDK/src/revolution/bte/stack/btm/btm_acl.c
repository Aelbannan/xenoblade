// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/stack/btm/btm_acl
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-802e6e50
extern char btm_cb[];
void btm_acl_init() {
    *(short*)(btm_cb + 0x4c6) = 0x7d00;
    btm_cb[0x27bf] = (char)0xff;
}
// LLM-HARNESS-END: us-802e6e50

// LLM-HARNESS-BEGIN: us-802e6e6c
void btm_handle_to_acl_index() {}
// LLM-HARNESS-END: us-802e6e6c

// LLM-HARNESS-BEGIN: us-802e6ef0
void btm_acl_created() {}
// LLM-HARNESS-END: us-802e6ef0

// LLM-HARNESS-BEGIN: us-802e7204
void btm_acl_removed() {}
// LLM-HARNESS-END: us-802e7204

// LLM-HARNESS-BEGIN: us-802e72c8
void btm_acl_device_down() {}
// LLM-HARNESS-END: us-802e72c8

// LLM-HARNESS-BEGIN: us-802e732c
void BTM_SwitchRole() {}
// LLM-HARNESS-END: us-802e732c

// LLM-HARNESS-BEGIN: us-802e7548
void btm_acl_encrypt_change() {}
// LLM-HARNESS-END: us-802e7548

// LLM-HARNESS-BEGIN: us-802e76b8
void BTM_SetLinkPolicy() {}
// LLM-HARNESS-END: us-802e76b8

// LLM-HARNESS-BEGIN: us-802e78a8
void BTM_SetDefaultLinkPolicy() {}
// LLM-HARNESS-END: us-802e78a8

// LLM-HARNESS-BEGIN: us-802e78b8
void btm_read_link_policy_complete() {}
// LLM-HARNESS-END: us-802e78b8

// LLM-HARNESS-BEGIN: us-802e79bc
void btm_read_remote_version_complete() {}
// LLM-HARNESS-END: us-802e79bc

// LLM-HARNESS-BEGIN: us-802e7a48
void btm_read_remote_features_complete() {}
// LLM-HARNESS-END: us-802e7a48

// LLM-HARNESS-BEGIN: us-802e7c74
void BTM_SetDefaultLinkSuperTout() {}
// LLM-HARNESS-END: us-802e7c74

// LLM-HARNESS-BEGIN: us-802e7c84
void BTM_IsAclConnectionUp() {}
// LLM-HARNESS-END: us-802e7c84

// LLM-HARNESS-BEGIN: us-802e7d5c
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
// LLM-HARNESS-END: us-802e7d5c

// LLM-HARNESS-BEGIN: us-802e7db8
void btm_get_acl_disc_reason_code() {}
// LLM-HARNESS-END: us-802e7db8

// LLM-HARNESS-BEGIN: us-802e7dc8
void BTM_GetHCIConnHandle() {}
// LLM-HARNESS-END: us-802e7dc8

// LLM-HARNESS-BEGIN: us-802e7e64
void btm_process_clk_off_comp_evt() {}
// LLM-HARNESS-END: us-802e7e64

// LLM-HARNESS-BEGIN: us-802e7f00
void btm_acl_role_changed() {}
// LLM-HARNESS-END: us-802e7f00

// LLM-HARNESS-BEGIN: us-802e80f4
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
// LLM-HARNESS-END: us-802e80f4

// LLM-HARNESS-BEGIN: us-802e8150
void btm_get_max_packet_size() {}
// LLM-HARNESS-END: us-802e8150

// LLM-HARNESS-BEGIN: us-802e82f0
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
// LLM-HARNESS-END: us-802e82f0

// LLM-HARNESS-BEGIN: us-802e8334
void btm_qos_setup_complete() {}
// LLM-HARNESS-END: us-802e8334

// LLM-HARNESS-BEGIN: us-802e840c
void BTM_ReadRSSI() {}
// LLM-HARNESS-END: us-802e840c

// LLM-HARNESS-BEGIN: us-802e8548
void BTM_ReadLinkQuality() {}
// LLM-HARNESS-END: us-802e8548

// LLM-HARNESS-BEGIN: us-802e8684
void btm_read_rssi_complete() {}
// LLM-HARNESS-END: us-802e8684

// LLM-HARNESS-BEGIN: us-802e87a0
void btm_read_link_quality_complete() {}
// LLM-HARNESS-END: us-802e87a0

// LLM-HARNESS-BEGIN: us-802e88b8
void btm_remove_acl() {}
// LLM-HARNESS-END: us-802e88b8

// LLM-HARNESS-BEGIN: us-802e8974
void btm_chg_all_acl_pkt_types() {}
// LLM-HARNESS-END: us-802e8974
