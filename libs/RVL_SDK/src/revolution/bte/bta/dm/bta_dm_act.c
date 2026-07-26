// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/bta/dm/bta_dm_act
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void bta_dm_enable() {}

void bta_dm_disable() {}

void bta_dm_disable_timer_cback() {}

extern void BTM_SetLocalDeviceName(void*);
void bta_dm_set_dev_name(void* self) { ((void(*)(void*))BTM_SetLocalDeviceName)((char*)self + 0x8); }

void bta_dm_set_visibility() {}

void bta_dm_bond() {}

void bta_dm_pin_reply() {}

void bta_dm_auth_reply() {}

void bta_dm_search_start() {}

void bta_dm_search_cancel() {}

void bta_dm_discover() {}

void bta_dm_inq_cmpl() {}

void bta_dm_rmt_name() {}

void bta_dm_disc_rmt_name() {}

void bta_dm_sdp_result() {}

void bta_dm_search_cmpl() {}

void bta_dm_disc_result() {}

void bta_dm_search_result() {}

void bta_dm_search_timer_cback() {}

void bta_dm_free_sdp_db() {}

void bta_dm_queue_search() {}

void bta_dm_queue_disc() {}

void bta_dm_search_clear_queue() {}

void bta_dm_search_cancel_cmpl() {}

void bta_dm_search_cancel_transac_cmpl() {}

void bta_dm_search_cancel_notify() {}

void bta_dm_find_services() {}

void bta_dm_discover_next_device() {}

void bta_dm_sdp_callback() {}

void bta_dm_inq_results_cb() {}

void bta_dm_inq_cmpl_cb() {}

void bta_dm_service_search_remname_cback() {}

void bta_dm_remname_cback() {}

void bta_dm_disc_remname_cback() {}

void bta_dm_cancel_rmt_name(void) { BTM_CancelRemoteDeviceName(); }

void bta_dm_authorize_cback() {}

void bta_dm_pinname_cback() {}

void bta_dm_pin_cback() {}

int bta_dm_link_key_request_cback(void) { return 0xb; }

void bta_dm_new_link_key_cback() {}

void bta_dm_authentication_complete_cback() {}

typedef void (*bta_dm_cback_t)(int, void *);

struct bta_dm_cb_t {
    unsigned char _pad0[0x50];
    bta_dm_cback_t cback;
};

extern struct bta_dm_cb_t bta_dm_cb;

void bta_dm_local_addr_cback(void *addr) {
    if (bta_dm_cb.cback != NULL) {
        bta_dm_cb.cback(0, addr);
    }
}

void bta_dm_signal_strength() {}

void bta_dm_signal_strength_timer_cback() {}

void bta_dm_acl_change_cback() {}

void bta_dm_acl_change() {}

void bta_dm_disable_conn_down_timer_cback() {
    typedef void (*bta_dm_cb_func_t)(int, int);
    extern struct { char reserved[0x50]; bta_dm_cb_func_t func; } bta_dm_cb;
    bta_dm_cb.func(1, 0);
}

void bta_dm_rssi_cback() {}

void bta_dm_link_quality_cback() {}

void bta_dm_l2cap_server_compress_cback() {}

void bta_dm_compress_cback() {}

void bta_dm_rm_cback() {}

void bta_dm_keep_acl() {}

void bta_dm_immediate_disable() {}

void bta_dm_reset_complete(void) {}

void bta_dm_send_hci_reset() {}
