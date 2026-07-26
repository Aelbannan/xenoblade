// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/stack/btm/btm_devctl
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void btm_dev_init() {}

void btm_db_reset() {}

void BTM_DeviceReset() {}

void BTM_SendHciReset() {}

int BTM_IsDeviceUp(void) {
    extern unsigned char btm_cb[];
    return (btm_cb[0x64e] == 5) ? 1 : 0;
}

void BTM_SetAfhChannels() {}

void btm_dev_timeout() {}

void btm_reset_complete() {}

void btm_read_hci_buf_size_complete() {}

void btm_read_local_version_complete() {}

void btm_read_local_features_complete() {}

void BTM_SetLocalDeviceName() {}

void btm_read_local_name_complete() {}

void BTM_ReadLocalDeviceAddr() {}

void btm_read_local_addr_complete() {}

void BTM_ReadLocalVersion() {}

void BTM_SetDeviceClass() {}

void BTM_ReadDeviceClass() {}

void BTM_ReadLocalFeatures() {}

void BTM_RegisterForDeviceStatusNotif() {}

void BTM_VendorSpecificCommand() {}

void btm_vsc_complete() {}

void BTM_RegisterForVSEvents() {}

void btm_vendor_specific_evt() {}

void BTM_WritePageTimeout() {}

void BTM_ReadStoredLinkKey() {}

void BTM_WriteStoredLinkKey() {}

void BTM_DeleteStoredLinkKey() {}

void btm_read_stored_link_key_complete() {}

void btm_write_stored_link_key_complete() {}

void btm_delete_stored_link_key_complete() {}

void btm_return_link_keys_evt() {}

void btm_report_device_status() {}
