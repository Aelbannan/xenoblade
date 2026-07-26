// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/stack/sdp/sdp_api
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void SDP_InitDiscoveryDb() {}

void SDP_ServiceSearchRequest() {}

void SDP_ServiceSearchAttributeRequest() {}

void SDP_FindAttributeInRec() {}

void SDP_FindServiceInDb() {}

void SDP_FindServiceUUIDInDb() {}

void SDP_SetLocalDiRecord() {}

void SDP_GetLocalDiRecord() {}

extern unsigned char sdp_cb[];

unsigned char SDP_SetTraceLevel(unsigned int level) {
    if (level != 0xff) {
        sdp_cb[0x4630] = (unsigned char)level;
    }
    return sdp_cb[0x4630];
}
