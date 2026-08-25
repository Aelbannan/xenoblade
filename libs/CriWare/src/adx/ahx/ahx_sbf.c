// Auto-scaffolded catalog TU for CriWare/src/adx/ahx/ahx_sbf
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern u32 lbl_eu_805E64B8;
void AHXDCD_SetupFtbl(u32 val) { lbl_eu_805E64B8 = val; }

extern u32 lbl_eu_805E64BC;
void AHXDCD_SetupWtbl(u32 val) { lbl_eu_805E64BC = val; }

extern u32 lbl_eu_805E64B0;

// MWCC literal-pool constants (retail .rodata @ 0x80517560): the SBF window
// scale factor 2147483648.0f plus an adjacent 0.0f pool entry.
const float lbl_eu_80517560[2] = {2147483648.0f, 0.0f};

void ahxsbf_init_filter(void);

void AHXSBF_Init(void) {
    if (lbl_eu_805E64B0 == 0) {
        ahxsbf_init_filter();
    }
    lbl_eu_805E64B0++;
}

/* SBF filter workspace (bss @ 0x805E64A8).
 * +0x00 dstF: aligned copy of the filter table (0x2000 bytes)
 * +0x04 dstW: aligned copy of the synthesis window table (0x800 bytes)
 * +0x0C flag: set once the tables have been prepared
 * +0x10 ftbl: source filter table (set via AHXDCD_SetupFtbl)
 * +0x14 wtbl: source window table (set via AHXDCD_SetupWtbl)
 */
typedef struct AhxSbfWork {
    void* dstF;
    void* dstW;
    u32 unk08;
    u32 flag;
    void* ftbl;
    void* wtbl;
} AhxSbfWork;

extern AhxSbfWork lbl_eu_805E64A8;

void ahxsbf_init_filter(void) {
    AhxSbfWork* w = &lbl_eu_805E64A8;
    u8* dst;
    u8* adst;
    u8* src;
    s32 i;
    s32 j;

    if (w->flag != 0) {
        return;
    }

    /* Copy the synthesis window table to a 32-byte aligned scratch buffer,
     * then normalize it in place (MWCC keeps the scale factor in f1). */
    src = (u8*)w->wtbl;
    w->dstW = dst = (u8*)(((u32)src + 0x1F) & ~0x1F);
    for (i = 0x800; i >= 0; i--) {
        dst[i] = src[i];
    }

    /* Scale factor comes from MWCC's literal pool (lbl_eu_80517560). */
    for (j = 0; j < 0x200; j++) {
        ((float*)w->dstW)[j] *= lbl_eu_80517560[0];
    }

    /* Copy the filter table to its aligned scratch buffer. */
    src = (u8*)w->ftbl;
    w->dstF = adst = (u8*)(((u32)src + 0x1F) & ~0x1F);
    for (j = 0x2000; j >= 0; j--) {
        adst[j] = src[j];
    }

    w->flag = 1;
}

void AHXSBF_Finish(void) {
    lbl_eu_805E64B0--;
}

extern u32 lbl_eu_805E64AC;

AhxSbfWork* AHXSBF_Create(AhxSbfWork* self, u32 size) {
    memset(self, 0, size);
    self->unk08 = 64;
    self->dstW = (void*)64;
    self->ftbl = (void*)*(u32*)&lbl_eu_805E64AC;
    self->flag = *(u32*)&lbl_eu_805E64A8;
    *(u8*)self = 1;
    return self;
}

void AHXSBF_Destroy(void* self) { memset(self, 0, 0x1014); }
