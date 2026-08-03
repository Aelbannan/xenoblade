// RVL_SDK/src/revolution/bte/stack/btu/btu_init

#include <harness_catalog.h>

#include <string.h>

#include "revolution/bte/stack/include/btu.h"
#include "revolution/bte/stack/include/hcidefs.h"

/* Retail .sdata2 slice is 8 bytes: BT_BD_ANY (all-FF BD_ADDR) + 2 pad. */
const BD_ADDR BT_BD_ANY = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

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
