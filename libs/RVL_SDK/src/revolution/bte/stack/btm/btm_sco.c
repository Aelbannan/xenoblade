// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/stack/btm/btm_sco
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-802ed4a4
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
// LLM-HARNESS-END: us-802ed4a4

// LLM-HARNESS-BEGIN: us-802ed4e8
void btm_esco_conn_rsp() {}
// LLM-HARNESS-END: us-802ed4e8

// LLM-HARNESS-BEGIN: us-802ed6c4
void btm_sco_chk_pend_unpark() {}
// LLM-HARNESS-END: us-802ed6c4

// LLM-HARNESS-BEGIN: us-802ed850
void btm_sco_conn_req() {}
// LLM-HARNESS-END: us-802ed850

// LLM-HARNESS-BEGIN: us-802eda30
void btm_sco_connected() {}
// LLM-HARNESS-END: us-802eda30

// LLM-HARNESS-BEGIN: us-802edbd0
void BTM_RemoveSco() {}
// LLM-HARNESS-END: us-802edbd0

// LLM-HARNESS-BEGIN: us-802edc88
void btm_remove_sco_links() {}
// LLM-HARNESS-END: us-802edc88

// LLM-HARNESS-BEGIN: us-802edd0c
void btm_sco_removed() {}
// LLM-HARNESS-END: us-802edd0c

// LLM-HARNESS-BEGIN: us-802ede20
void btm_sco_acl_removed() {}
// LLM-HARNESS-END: us-802ede20

// LLM-HARNESS-BEGIN: us-802eded4
void btm_route_sco_data(void) {}
// LLM-HARNESS-END: us-802eded4

// LLM-HARNESS-BEGIN: us-802eded8
void BTM_ChangeEScoLinkParms() {}
// LLM-HARNESS-END: us-802eded8

// LLM-HARNESS-BEGIN: us-802ee080
void btm_esco_proc_conn_chg() {}
// LLM-HARNESS-END: us-802ee080

// LLM-HARNESS-BEGIN: us-802ee184
void btm_is_sco_active() {}
// LLM-HARNESS-END: us-802ee184

// LLM-HARNESS-BEGIN: us-802ee1f4
void btm_num_sco_links_active() {}
// LLM-HARNESS-END: us-802ee1f4

// LLM-HARNESS-BEGIN: us-802ee254
void btm_is_sco_active_by_bdaddr() {}
// LLM-HARNESS-END: us-802ee254
