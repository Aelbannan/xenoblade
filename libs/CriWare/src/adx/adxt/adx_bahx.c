// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_bahx
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern void (*lbl_eu_805E5348)(void*);
void ADXB_SetAhxInSj(void* self) {
    void* p = *(void**)((u8*)self + 0xb8);
    if (p == NULL) return;
    ((void (*)(void*))lbl_eu_805E5348)(p);
}

void ADXB_SetAhxDecSmpl() {}

extern char lbl_eu_805E5354[];
void ADXB_ExecOneAhx(void* self) {
    ((void (*)(void*))(*(void**)(lbl_eu_805E5354)))(self);
}

extern void (*lbl_eu_805E5350)(void*);
void ADXB_AhxTermSupply(void* self) {
    void* p = *(void**)((u8*)self + 0xb8);
    if (p == NULL) return;
    lbl_eu_805E5350(p);
}
