// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/stack/rfcomm/rfc_port_if
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void RFCOMM_StartRsp(unsigned short param1, unsigned short param2) {
    unsigned short local = param2;
    rfc_mx_sm_execute(param1, 7, &local);
}

void RFCOMM_DlcEstablishReq() {}

void RFCOMM_DlcEstablishRsp() {}

void RFCOMM_ParNegReq() {}

void RFCOMM_ParNegRsp() {}

void RFCOMM_PortNegReq() {}

void RFCOMM_PortNegRsp() {}

void RFCOMM_ControlReq() {}

void RFCOMM_FlowReq() {}

void RFCOMM_LineStatusReq() {}

void RFCOMM_DlcReleaseReq() {}

void RFCOMM_DataReq() {}
