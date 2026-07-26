// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/stack/btm/btm_sco
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void btm_sco_init(void)
{
    extern unsigned long btm_esco_defaults[];
    extern unsigned char btm_cb[];
    unsigned long* src = btm_esco_defaults;
    unsigned char* cb = btm_cb;

    *(unsigned short*)(cb + 0x18f6) = 0xffff;
    *(unsigned long*)(cb + 0x18f8) = src[0];
    *(unsigned long*)(cb + 0x18fc) = src[1];
    *(unsigned long*)(cb + 0x1900) = src[2];
    *(unsigned long*)(cb + 0x1904) = src[3];
    cb[0x1909] = 2;
}

void btm_esco_conn_rsp() {}

void btm_sco_chk_pend_unpark() {}

void btm_sco_conn_req() {}

void btm_sco_connected() {}

void BTM_RemoveSco() {}

void btm_remove_sco_links() {}

void btm_sco_removed() {}

void btm_sco_acl_removed() {}

void btm_route_sco_data(void) {}

void BTM_ChangeEScoLinkParms() {}

void btm_esco_proc_conn_chg() {}

void btm_is_sco_active() {}

void btm_num_sco_links_active() {}

void btm_is_sco_active_by_bdaddr() {}
