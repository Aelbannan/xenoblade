// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_bahx
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern void (*lbl_eu_805E5348)(void*);
void ADXB_SetAhxInSj(void* ahx, void* sj) {
    void* p = *(void**)((u8*)ahx + 0xb8);
    if (p == NULL) return;
    ((void (*)(void*, void*))lbl_eu_805E5348)(p, sj);
}


extern void (*lbl_eu_805E534C)(void*, void*);
void ADXB_SetAhxDecSmpl(void* self, int val) {
    void* p = *(void**)((u8*)self + 0xb8);
    if (p != NULL)
        lbl_eu_805E534C(p, (void*)val);
    *(u32*)((u8*)self + 0xbc) = val;
    int n = val / 96;
    *(u32*)((u8*)self + 0xc0) = n;
    if (n <= 0)
        *(u32*)((u8*)self + 0xc0) = 1;
}

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
