// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/stack/btm/btm_sec
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void BTM_SecRegister() {}

void BTM_SecAddRmtNameNotifyCallback() {}

void BTM_SecDeleteRmtNameNotifyCallback() {}

void BTM_SetPinType() {}

void BTM_SetSecurityLevel() {}

void BTM_PINCodeReply() {}

void BTM_DeviceAuthorized() {}

void BTM_SecBond() {}

void btm_sec_l2cap_access_req() {}

void btm_sec_mx_access_request() {}

void btm_sec_conn_req() {}

void btm_sec_init() {}

void btm_sec_dev_reset() {
    extern unsigned char btm_cb[];
    if (btm_cb[0x1978] == 3) {
        btsnd_hcic_write_auth_enable(1);
        btsnd_hcic_write_encr_mode(1);
    }
}

void btm_sec_abort_access_req() {}

void btm_sec_rmt_name_request_complete() {}

void btm_sec_auth_complete() {}

void btm_sec_mkey_comp_event() {}

void btm_sec_encrypt_change() {}

void btm_sec_is_bonding() {}

void btm_sec_connected() {}

void btm_sec_disconnect() {}

void btm_sec_disconnected() {}

void btm_sec_link_key_notification() {}

void btm_sec_link_key_request() {}

void btm_sec_pin_code_request_timeout() {}

void btm_sec_pin_code_request() {}

void btm_sec_update_clock_offset() {}

void btm_sec_execute_procedure() {}

void btm_sec_start_authorization() {}

void btm_sec_collision_timeout() {}

void btm_read_trusted_mask() {}
