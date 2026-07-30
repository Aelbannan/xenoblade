// Auto-scaffolded catalog TU for CriWare/src/sofdec/mwply/mwsfdsvm
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern u32 lbl_eu_805FF1D0;
extern u32 lbl_eu_805FF1D8;
extern int lbl_eu_805FF1DC;

void MWSFSVM_Init(void) {
    volatile u32* base;
    base = (volatile u32*)&lbl_eu_805FF1D0;
    SVM_Init();
    base[0] = 0;
    base[1] = 0;
    base[2] = 0;
    base[3] = 0;
}

extern int SVM_SetCbSvrIdWithString(int, void*, void*, void*, void*);
extern u32 lbl_eu_805FF1D4;

void MWSFSVM_EntryIdVfunc(void* self, void* a, void* b, void* c) {
    SVM_SetCbSvrIdWithString(2, self, a, b, c);
    lbl_eu_805FF1D4 = (u32)self;
}

void MWSFSVM_EntryIdleFunc(void* p1, void* p2, void* p3) {
    lbl_eu_805FF1D8 = SVM_SetCbSvrWithString(6, p1, p2, p3);
}

void MWSFSVM_EntryMainFunc(int arg1, int arg2, int arg3)
{
    int result = SVM_SetCbSvrWithString(5, arg1, arg2, arg3);
    lbl_eu_805FF1DC = result;
}
void MWSFSVM_TestAndSet(void* p) { SVM_TestAndSet(p); }


void MWSFSVM_Error() {}

void MWSFSVM_GotoIdleBorder() {}
