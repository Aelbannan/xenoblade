// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/stack/sdp/sdp_db
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-80306674
void sdp_db_service_search() {}
// LLM-HARNESS-END: us-80306674

// LLM-HARNESS-BEGIN: us-803067b0
void find_uuid_in_seq() {}
// LLM-HARNESS-END: us-803067b0

// LLM-HARNESS-BEGIN: us-80306888
void sdp_db_find_record() {}
// LLM-HARNESS-END: us-80306888

// LLM-HARNESS-BEGIN: us-803068e4
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
// LLM-HARNESS-END: us-803068e4

// LLM-HARNESS-BEGIN: us-80306924
void SDP_CreateRecord() {}
// LLM-HARNESS-END: us-80306924

// LLM-HARNESS-BEGIN: us-80306a04
void SDP_DeleteRecord() {}
// LLM-HARNESS-END: us-80306a04

// LLM-HARNESS-BEGIN: us-80306b18
void SDP_AddAttribute() {}
// LLM-HARNESS-END: us-80306b18

// LLM-HARNESS-BEGIN: us-80306eb0
void SDP_AddUuidSequence() {}
// LLM-HARNESS-END: us-80306eb0

// LLM-HARNESS-BEGIN: us-80306f88
void SDP_AddServiceClassIdList() {}
// LLM-HARNESS-END: us-80306f88

// LLM-HARNESS-BEGIN: us-80307104
void SDP_DeleteAttribute() {}
// LLM-HARNESS-END: us-80307104
