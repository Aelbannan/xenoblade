// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/stack/sdp/sdp_db
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void sdp_db_service_search() {}

void find_uuid_in_seq() {}

void sdp_db_find_record() {}

void *sdp_db_find_attr_in_rec(void *rec, unsigned short attr_id_low, unsigned short attr_id_high) {
    unsigned short num_attr = *(unsigned short *)((char *)rec + 8);
    char *attr_ptr = (char *)rec + 12;
    unsigned short i = 0;
    while (i < num_attr) {
        unsigned short attr_id = *(unsigned short *)(attr_ptr + 8);
        if (attr_id >= attr_id_low && attr_id <= attr_id_high) {
            return attr_ptr;
        }
        i++;
        attr_ptr += 12;
    }
    return 0;
}

void SDP_CreateRecord() {}

void SDP_DeleteRecord() {}

void SDP_AddAttribute() {}

void SDP_AddUuidSequence() {}

void SDP_AddServiceClassIdList() {}

void SDP_DeleteAttribute() {}
