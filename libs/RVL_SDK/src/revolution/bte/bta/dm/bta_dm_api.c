// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/bta/dm/bta_dm_api
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void BTA_EnableBluetooth() {}

void BTA_DisableBluetooth() {}

BOOL BTA_DmIsDeviceUp() {
    BOOL ret;
    GKI_sched_lock();
    ret = BTM_IsDeviceUp();
    GKI_sched_unlock();
    return ret;
}

void BTA_DmSetDeviceName() {}

void BTA_DmSetVisibility() {}

void BTA_DmSearch() {}

void BTA_DmSearchCancel() {}

void BTA_DmPinReply() {}

void BTA_DmAddDevice() {}

void BTA_DmRemoveDevice() {}

void BTA_DmSendHciReset() {}
