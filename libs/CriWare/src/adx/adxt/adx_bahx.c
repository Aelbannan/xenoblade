// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_bahx
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "libs/CriWare/src/adx/adxt/adx_bsc.h"

/* Opaque partner types of the AHX link layer (see ahx_link.c). */
typedef struct AHXSJD AHXSJD;
typedef struct AHXIO_ AHXIO;

extern void (*adxb_ahx_in_sj)(AHXSJD*, AHXIO*);
void ADXB_SetAhxInSj(ADXB* ahx, AHXIO* sj) {
    AHXSJD* p = ahx->ainf;
    if (p == NULL) return;
    adxb_ahx_in_sj(p, sj);
}

extern void (*adxb_ahx_dec_smpl)(AHXSJD*, s32);
void ADXB_SetAhxDecSmpl(ADXB* self, int val) {
    AHXSJD* p = self->ainf;
    if (p != NULL)
        adxb_ahx_dec_smpl(p, val);
    self->field_0xbc = val;
    int n = val / 96;
    self->field_0xc0 = n;
    if (n <= 0)
        self->field_0xc0 = 1;
}

extern void (*adxb_ahx_exec_one)(ADXB*);
void ADXB_ExecOneAhx(ADXB* self) {
    adxb_ahx_exec_one(self);
}

extern void (*adxb_ahx_term_supply)(AHXSJD*);
void ADXB_AhxTermSupply(ADXB* self) {
    AHXSJD* p = self->ainf;
    if (p == NULL) return;
    adxb_ahx_term_supply(p);
}

/* .bss: reverse definition order reproduces the retail 0x10-byte block. */
void (*adxb_ahx_exec_one)(ADXB*);
void (*adxb_ahx_term_supply)(AHXSJD*);
void (*adxb_ahx_dec_smpl)(AHXSJD*, s32);
void (*adxb_ahx_in_sj)(AHXSJD*, AHXIO*);
