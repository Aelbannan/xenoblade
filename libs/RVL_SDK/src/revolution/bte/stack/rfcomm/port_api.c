// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/stack/rfcomm/port_api
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <string.h>

#include "revolution/BTE/stack/rfcomm/rfc_int.h"
#include "revolution/BTE/include/bt_trace.h"

void RFCOMM_Init(void) {
    memset(&rfc_cb, 0, sizeof(tRFC_CB));
    rfc_cb.rfc.last_mux = MAX_BD_CONNECTIONS;
    rfc_cb.trace_level = BT_TRACE_LEVEL_DEBUG;
    rfcomm_l2cap_if_init();
}
