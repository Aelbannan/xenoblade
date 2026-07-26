// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/bta/sys/bta_sys_main
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void bta_sys_init() {}

void bta_sys_event() {}

void bta_sys_timer_update() {}

void bta_sys_register() {}

unsigned char bta_sys_sendmsg(void *p_msg) {
    extern unsigned char bta_sys_cb[];
    extern unsigned char *p_bta_sys_cfg;
    extern unsigned char GKI_send_msg(unsigned char, unsigned char, void *);
    return GKI_send_msg(bta_sys_cb[0x7D], p_bta_sys_cfg[2], p_msg);
}

void bta_sys_start_timer() {}

void bta_sys_stop_timer() {}

void bta_sys_disable() {}

extern u8 appl_trace_level;
extern u8 bta_sys_set_trace_level(u8 val) { appl_trace_level = val; return val; }
