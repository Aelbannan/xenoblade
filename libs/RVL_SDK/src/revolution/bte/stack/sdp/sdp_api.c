// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/stack/sdp/sdp_api
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

#include "revolution/bte/stack/include/sdp_api.h"

BOOLEAN SDP_InitDiscoveryDb(tSDP_DISCOVERY_DB *p_db, UINT32 len, UINT16 num_uuid,
                            tSDP_UUID *p_uuid_list, UINT16 num_attr,
                            UINT16 *p_attr_list) {
    return 0;
}

BOOLEAN SDP_ServiceSearchRequest(UINT8 *p_bd_addr, tSDP_DISCOVERY_DB *p_db,
                                 tSDP_DISC_CMPL_CB *p_cb) {
    return 0;
}

BOOLEAN SDP_ServiceSearchAttributeRequest(UINT8 *p_bd_addr, tSDP_DISCOVERY_DB *p_db,
                                          tSDP_DISC_CMPL_CB *p_cb) {
    return 0;
}

tSDP_DISC_ATTR *SDP_FindAttributeInRec(tSDP_DISC_REC *p_rec, UINT16 attr_id) {
    tSDP_DISC_ATTR *p_attr = p_rec->p_first_attr;

    while (p_attr) {
        if (p_attr->attr_id == attr_id) {
            return p_attr;
        }

        p_attr = p_attr->p_next_attr;
    }

    return NULL;
}

tSDP_DISC_REC *SDP_FindServiceInDb(tSDP_DISCOVERY_DB *p_db, UINT16 attr_id, tSDP_DISC_REC *p_start_rec) { return NULL; }

tSDP_DISC_REC *SDP_FindServiceUUIDInDb(tSDP_DISCOVERY_DB *p_db, tBT_UUID *p_uuid, tSDP_DISC_REC *p_start_rec) { return NULL; }

UINT16 SDP_SetLocalDiRecord(tSDP_DI_RECORD *device_info, UINT32 *p_handle) { return 0; }

UINT16 SDP_GetLocalDiRecord(tSDP_DI_GET_RECORD *p_device_info, UINT32 *p_handle) { return 0; }

extern unsigned char sdp_cb[];

UINT8 SDP_SetTraceLevel(UINT8 new_level) {
    if (new_level != 0xff) {
        sdp_cb[0x4630] = (unsigned char)new_level;
    }
    return sdp_cb[0x4630];
}
