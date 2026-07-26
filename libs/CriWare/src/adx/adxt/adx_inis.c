// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_inis
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void adxini_rnaerr_cbfn(void) {}

void adxini_lscerr_cbfn(void) {}

int adxt_exec_main_thrd(void) {
    ADXT_ExecLscSvr();
    return 0;
}

int adxt_exec_tsvr(void) {
    ADXT_ExecServer();
    return 0;
}

int adxt_exec_fssvr(void) {
    ADXT_ExecFsSvr();
    return 0;
}

void ADXT_Init() {}

void ADXT_Finish() {}
