// RVL_SDK/src/revolution/bte/stack/btu/btu_init

#include <harness_catalog.h>

#include <string.h>

#include "revolution/bte/stack/include/btu.h"
#include "revolution/bte/stack/include/hcidefs.h"

void btu_init_core() {
    btm_init();
    l2c_init();
    sdp_init();
    btm_discovery_db_init();
}

void BTE_Init() {
    memset(&btu_cb, 0, sizeof(btu_cb));

    btu_cb.hcit_acl_pkt_size = BTU_DEFAULT_DATA_SIZE + HCI_DATA_PREAMBLE_SIZE;
    btu_cb.controller_cmd_window = 1;
}
