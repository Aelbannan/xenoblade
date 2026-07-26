// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/bta/hh/bta_hh_act
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void bta_hh_api_enable() {}

void bta_hh_api_disable() {}

void bta_hh_disc_cmpl() {}

void bta_hh_sdp_cback() {}

void bta_hh_start_sdp() {}

void bta_hh_sdp_cmpl() {}

void bta_hh_api_disc_act() {}

void bta_hh_open_cmpl_act() {}

void bta_hh_open_act() {}

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

void bta_hh_handsk_act() {}

void bta_hh_ctrl_dat_act() {}

void bta_hh_close_act() {}

void bta_hh_get_dscp_act() {}

void bta_hh_maint_dev_act() {}

void bta_hh_get_acl_q_info() {}

void bta_hh_write_dev_act() {}

void bta_hh_cback() {}
