// Auto-scaffolded catalog TU for CriWare/src/adx/cvfs/cri_cvfs
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern void (*lbl_eu_805E66E8)(void*);
extern void* lbl_eu_805E66EC;
void cvFsCallUsrErrFn(void) {
    void (*fn)(void*) = lbl_eu_805E66E8;
    if (fn == NULL) return;
    fn(lbl_eu_805E66EC);
}

void cvFsAddDev() {}

void cvFsSetDefDev() {}

void cvFsOpen() {}

void getDevName() {}

void cvFsClose() {}

void cvFsTell() {}

void cvFsSeek() {}

void cvFsReqRd() {}

void cvFsStopTr() {}

void cvFsExecServer() {}

void cvFsGetStat() {}

void cvFsGetFileSize() {}

void cvFsEntryErrFunc() {}

void cvFsIsAvailableRetry() {}
