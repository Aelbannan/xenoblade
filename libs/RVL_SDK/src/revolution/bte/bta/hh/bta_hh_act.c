// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/bta/hh/bta_hh_act
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-802e4a38
void bta_hh_api_enable() {}
// LLM-HARNESS-END: us-802e4a38

// LLM-HARNESS-BEGIN: us-802e4bac
void bta_hh_api_disable() {}
// LLM-HARNESS-END: us-802e4bac

// LLM-HARNESS-BEGIN: us-802e4cb0
void bta_hh_disc_cmpl() {}
// LLM-HARNESS-END: us-802e4cb0

// LLM-HARNESS-BEGIN: us-802e4d54
void bta_hh_sdp_cback() {}
// LLM-HARNESS-END: us-802e4d54

// LLM-HARNESS-BEGIN: us-802e4e88
void bta_hh_start_sdp() {}
// LLM-HARNESS-END: us-802e4e88

// LLM-HARNESS-BEGIN: us-802e501c
void bta_hh_sdp_cmpl() {}
// LLM-HARNESS-END: us-802e501c

// LLM-HARNESS-BEGIN: us-802e5184
void bta_hh_api_disc_act() {}
// LLM-HARNESS-END: us-802e5184

// LLM-HARNESS-BEGIN: us-802e51ec
void bta_hh_open_cmpl_act() {}
// LLM-HARNESS-END: us-802e51ec

// LLM-HARNESS-BEGIN: us-802e5314
void bta_hh_open_act() {}
// LLM-HARNESS-END: us-802e5314

// LLM-HARNESS-BEGIN: us-802e53cc
void bta_hh_data_act(void *p_cb, void *p_data)
{
    void *p_buf;
    unsigned short off;
    unsigned short len;
    unsigned char rpt;

    p_buf = *(void **)((char *)p_data + 0xc);
    off = *(unsigned short *)((char *)p_buf + 4);
    rpt = (unsigned char)(*(unsigned short *)((char *)p_data + 6));
    len = *(unsigned short *)((char *)p_buf + 2);
    bta_hh_co_data(rpt, (char *)p_buf + off + 8, len,
                   *((unsigned char *)p_cb + 0x1b),
                   *((unsigned char *)p_cb + 0x13),
                   *((unsigned char *)p_cb + 0x15));
    utl_freebuf(&p_buf);
}
// LLM-HARNESS-END: us-802e53cc

// LLM-HARNESS-BEGIN: us-802e5424
void bta_hh_handsk_act() {}
// LLM-HARNESS-END: us-802e5424

// LLM-HARNESS-BEGIN: us-802e568c
void bta_hh_ctrl_dat_act() {}
// LLM-HARNESS-END: us-802e568c

// LLM-HARNESS-BEGIN: us-802e5884
void bta_hh_close_act() {}
// LLM-HARNESS-END: us-802e5884

// LLM-HARNESS-BEGIN: us-802e5a84
void bta_hh_get_dscp_act() {}
// LLM-HARNESS-END: us-802e5a84

// LLM-HARNESS-BEGIN: us-802e5aa0
void bta_hh_maint_dev_act() {}
// LLM-HARNESS-END: us-802e5aa0

// LLM-HARNESS-BEGIN: us-802e5c04
void bta_hh_get_acl_q_info() {}
// LLM-HARNESS-END: us-802e5c04

// LLM-HARNESS-BEGIN: us-802e5d20
void bta_hh_write_dev_act() {}
// LLM-HARNESS-END: us-802e5d20

// LLM-HARNESS-BEGIN: us-802e5ee8
void bta_hh_cback() {}
// LLM-HARNESS-END: us-802e5ee8
