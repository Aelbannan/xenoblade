// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_inis
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-803823e4
void adxini_rnaerr_cbfn(void) {}
// LLM-HARNESS-END: us-803823e4

// LLM-HARNESS-BEGIN: us-803823ec
void adxini_lscerr_cbfn(void) {}
// LLM-HARNESS-END: us-803823ec

// LLM-HARNESS-BEGIN: us-803823f4
int adxt_exec_main_thrd(void) {
    ADXT_ExecLscSvr();
    return 0;
}
// LLM-HARNESS-END: us-803823f4

// LLM-HARNESS-BEGIN: us-80382418
int adxt_exec_tsvr(void) {
    ADXT_ExecServer();
    return 0;
}
// LLM-HARNESS-END: us-80382418

// LLM-HARNESS-BEGIN: us-8038243c
int adxt_exec_fssvr(void) {
    ADXT_ExecFsSvr();
    return 0;
}
// LLM-HARNESS-END: us-8038243c

// LLM-HARNESS-BEGIN: us-80382460
void ADXT_Init() {}
// LLM-HARNESS-END: us-80382460

// LLM-HARNESS-BEGIN: us-80382590
void ADXT_Finish() {}
// LLM-HARNESS-END: us-80382590
