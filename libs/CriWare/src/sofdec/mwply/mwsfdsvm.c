// Auto-scaffolded catalog TU for CriWare/src/sofdec/mwply/mwsfdsvm
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void MWSFSVM_Init() {}

void MWSFSVM_EntryIdVfunc() {}

void MWSFSVM_EntryIdleFunc() {}

extern int lbl_eu_805FF1DC;

void MWSFSVM_EntryMainFunc(int arg1, int arg2, int arg3)
{
    int result = SVM_SetCbSvrWithString(5, arg1, arg2, arg3);
    lbl_eu_805FF1DC = result;
}
void MWSFSVM_TestAndSet(void* p) { SVM_TestAndSet(p); }


void MWSFSVM_Error() {}

void MWSFSVM_GotoIdleBorder() {}
