// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/stack/hid/hidh_conn
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void hidh_conn_reg() {}

void hidh_conn_disconnect() {}

void hidh_sec_check_complete_term() {}

void hidh_l2cif_connect_ind() {}

void hidh_proc_repage_timeout() {}

void hidh_sec_check_complete_orig() {}

void hidh_l2cif_connect_cfm() {}

void hidh_l2cif_config_ind() {}

void hidh_l2cif_config_cfm() {}

void hidh_l2cif_disconnect_ind() {}

void hidh_l2cif_disconnect_cfm() {}

void hidh_l2cif_cong_ind() {}

void hidh_l2cif_data_ind() {}

void hidh_conn_snd_data() {}

void hidh_conn_initiate() {}

void hidh_conn_dereg(void)
{
    L2CA_Deregister(0x11);
    L2CA_Deregister(0x13);
}
