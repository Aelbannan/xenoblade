// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/bta/dm/bta_dm_pm
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

struct bta_dm_msg;

/* Connected services table: byte 0 = count, 9-byte entries, service id at +7.
   Retail bss owner of this symbol is bta_dm_pm (referenced by bta_dm_act). */
unsigned char bta_dm_conn_srvcs[0x2e];

/* PM timer message: BT_HDR (8 bytes) followed by the peer BD address.
   Built by bta_dm_pm_timer_cback (event BTA_DM_PM_TIMER_EVT). */
struct bta_dm_pm_msg {
    unsigned char hdr[8];
    unsigned char bd_addr[6];
};

void bta_dm_init_pm() {}

void bta_dm_disable_pm() {}

void bta_dm_pm_cback() {}

#pragma push
#pragma auto_inline off
void bta_dm_pm_set_mode(unsigned char *bd_addr, int timed_out) {}
#pragma pop

void bta_dm_pm_btm_cback() {}

void bta_dm_pm_timer_cback() {}

void bta_dm_pm_btm_status() {}

void bta_dm_pm_timer(struct bta_dm_msg *p_data) {
    struct bta_dm_pm_msg *msg = (struct bta_dm_pm_msg *)p_data;

    bta_dm_pm_set_mode(msg->bd_addr, 1);
}
