// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_bahx
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern void (*adxb_ahx_in_sj)(void*);
void ADXB_SetAhxInSj(void* ahx, void* sj) {
    void* p = *(void**)((u8*)ahx + 0xb8);
    if (p == NULL) return;
    ((void (*)(void*, void*))adxb_ahx_in_sj)(p, sj);
}


extern void (*adxb_ahx_dec_smpl)(void*, void*);
void ADXB_SetAhxDecSmpl(void* self, int val) {
    void* p = *(void**)((u8*)self + 0xb8);
    if (p != NULL)
        adxb_ahx_dec_smpl(p, (void*)val);
    *(u32*)((u8*)self + 0xbc) = val;
    int n = val / 96;
    *(u32*)((u8*)self + 0xc0) = n;
    if (n <= 0)
        *(u32*)((u8*)self + 0xc0) = 1;
}

extern char adxb_ahx_exec_one[];
void ADXB_ExecOneAhx(void* self) {
    ((void (*)(void*))(*(void**)(adxb_ahx_exec_one)))(self);
}

extern void (*adxb_ahx_term_supply)(void*);
void ADXB_AhxTermSupply(void* self) {
    void* p = *(void**)((u8*)self + 0xb8);
    if (p == NULL) return;
    adxb_ahx_term_supply(p);
}

/* .bss: reverse definition order reproduces the retail 0x10-byte block. */
char adxb_ahx_exec_one[4];
void (*adxb_ahx_term_supply)(void*);
void (*adxb_ahx_dec_smpl)(void*, void*);
void (*adxb_ahx_in_sj)(void*);
